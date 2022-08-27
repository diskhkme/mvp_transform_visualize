#include "VirtualCamera.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h" 

VirtualCamera::VirtualCamera(Vector3 eye, Vector3 at, Vector3 worldUp, float fovy, float aspect, float near, float far)
	:eye{ eye }, at{ at }, worldUp{ worldUp }, fovy{ fovy }, aspect{ aspect }, near{ near }, far{ far }
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void VirtualCamera::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void VirtualCamera::Draw3D(SceneType sceneType) const
{
	DrawCamera(sceneType);
	DrawFrustum(sceneType);
}

void VirtualCamera::Draw2D()
{
	DrawGUI();
}

Matrix VirtualCamera::GetViewMat() const
{
	return viewMat;
}
Matrix VirtualCamera::GetProjMat() const
{
	return projectionMat;
}

void VirtualCamera::UpdateViewMatrix()
{
	n = Vector3Normalize(Vector3Subtract(eye, at));
	u = Vector3Normalize(Vector3CrossProduct(worldUp, n));
	v = Vector3CrossProduct(n, u);

	up = v;
	viewMat.m0 = u.x; viewMat.m4 = u.y; viewMat.m8 = u.z; viewMat.m12 = -Vector3DotProduct(u, eye);
	viewMat.m1 = v.x; viewMat.m5 = v.y; viewMat.m9 = v.z; viewMat.m13 = -Vector3DotProduct(v, eye);
	viewMat.m2 = n.x; viewMat.m6 = n.y; viewMat.m10 = n.z; viewMat.m14 = -Vector3DotProduct(n, eye);
	viewMat.m3 = 0.f; viewMat.m7 = 0.f; viewMat.m11 = 0.f; viewMat.m15 = 1.f;

	invViewMat = MatrixInvert(viewMat);
}
void VirtualCamera::UpdateProjectionMatrix()
{
	projectionMat.m0 = (1.0f / tanf(fovy / 2)) / aspect;
	projectionMat.m5 = (1.0f / tanf(fovy / 2));
	projectionMat.m10 = (far + near) / (far - near);
	projectionMat.m14 = (2 * near*far) / (far - near);
	projectionMat.m11 = -1.0f;
	projectionMat.m1 = projectionMat.m2 = projectionMat.m3 = projectionMat.m4 = projectionMat.m6 = projectionMat.m7 = 0;
	projectionMat.m8 = projectionMat.m9 = projectionMat.m12 = projectionMat.m13 = projectionMat.m15 = 0;

	invProjectionMat = MatrixInvert(projectionMat);
}
void VirtualCamera::DrawCamera(SceneType sceneType) const
{
	Vector3 camForward = Vector3Scale(n, -1);
	Vector3 camOrigin = eye;

	if (sceneType == SceneType::VIEW)
	{
		// In view, cam is at origin forwarding -z axis
		camForward = Vector3{ 0.0f, 0.0f, -1.0f };
		camOrigin = Vector3{ 0.0f, 0.0f, 0.0f };
	}
	else if (sceneType == SceneType::PROJECTION)
	{
		return;
	}

	Quaternion q = QuaternionFromVector3ToVector3(Vector3{ 0.0f,-1.0f,0.0f }, camForward);
	Vector3 axis;
	float angle;
	QuaternionToAxisAngle(q, &axis, &angle);

	rlPushMatrix();
	rlTranslatef(camOrigin.x, camOrigin.y, camOrigin.z);
	rlRotatef(angle*RAD2DEG, axis.x, axis.y, axis.z);
	DrawCylinderEx(Vector3{ 0.0f, -0.5f, 0.0f }, Vector3{ 0.0f, 0.5f, 0.0f }, 0.3f, 0.01f, 5, GREEN);
	DrawCylinderWiresEx(Vector3{ 0.0f, -0.5f, 0.0f }, Vector3{ 0.0f, 0.5f, 0.0f }, 0.3f, 0.01f, 5, DARKGRAY);
	rlPopMatrix();
}

void VirtualCamera::DrawFrustum(SceneType sceneType) const
{
	Vector3 farRightTop = Vector3{ 1.0f,1.0f,1.0f };
	Vector3 farLeftTop = Vector3{ -1.0f,1.0f,1.0f };
	Vector3 farRightBot = Vector3{ 1.0f,-1.0f,1.0f };
	Vector3 farLeftBot = Vector3{ -1.0f,-1.0f,1.0f };

	Vector3 nearRightTop = Vector3{ 1.0f,1.0f,-1.0f };
	Vector3 nearLeftTop = Vector3{ -1.0f,1.0f,-1.0f };
	Vector3 nearRightBot = Vector3{ 1.0f,-1.0f,-1.0f };
	Vector3 nearLeftBot = Vector3{ -1.0f,-1.0f,-1.0f };
	Color drawColor = BLUE;

	if (sceneType == SceneType::VIEW)
	{
		farRightTop = MATH_EXT::MatrixVectorMultiplication(invProjectionMat, farRightTop);
		farLeftTop = MATH_EXT::MatrixVectorMultiplication(invProjectionMat, farLeftTop);
		farRightBot = MATH_EXT::MatrixVectorMultiplication(invProjectionMat, farRightBot);
		farLeftBot = MATH_EXT::MatrixVectorMultiplication(invProjectionMat, farLeftBot);

		nearRightTop = MATH_EXT::MatrixVectorMultiplication(invProjectionMat, nearRightTop);
		nearLeftTop = MATH_EXT::MatrixVectorMultiplication(invProjectionMat, nearLeftTop);
		nearRightBot = MATH_EXT::MatrixVectorMultiplication(invProjectionMat, nearRightBot);
		nearLeftBot = MATH_EXT::MatrixVectorMultiplication(invProjectionMat, nearLeftBot);
		drawColor = RED;
	}
	else if (sceneType == SceneType::WORLD)
	{

		// world to camera coordinate conversion
		farRightTop = MATH_EXT::MatrixVectorMultiplication(MatrixMultiply(invProjectionMat, invViewMat), farRightTop);
		farLeftTop = MATH_EXT::MatrixVectorMultiplication(MatrixMultiply(invProjectionMat, invViewMat), farLeftTop);
		farRightBot = MATH_EXT::MatrixVectorMultiplication(MatrixMultiply(invProjectionMat, invViewMat), farRightBot);
		farLeftBot = MATH_EXT::MatrixVectorMultiplication(MatrixMultiply(invProjectionMat, invViewMat), farLeftBot);

		nearRightTop = MATH_EXT::MatrixVectorMultiplication(MatrixMultiply(invProjectionMat, invViewMat), nearRightTop);
		nearLeftTop = MATH_EXT::MatrixVectorMultiplication(MatrixMultiply(invProjectionMat, invViewMat), nearLeftTop);
		nearRightBot = MATH_EXT::MatrixVectorMultiplication(MatrixMultiply(invProjectionMat, invViewMat), nearRightBot);
		nearLeftBot = MATH_EXT::MatrixVectorMultiplication(MatrixMultiply(invProjectionMat, invViewMat), nearLeftBot);
		drawColor = DARKGREEN;
	}

	// draw far rect
	DrawLine3D(farRightTop, farLeftTop, drawColor);
	DrawLine3D(farLeftTop, farLeftBot, drawColor);
	DrawLine3D(farLeftBot, farRightBot, drawColor);
	DrawLine3D(farRightBot, farRightTop, drawColor);
	//draw near rect
	DrawLine3D(nearRightTop, nearLeftTop, drawColor);
	DrawLine3D(nearLeftTop, nearLeftBot, drawColor);
	DrawLine3D(nearLeftBot, nearRightBot, drawColor);
	DrawLine3D(nearRightBot, nearRightTop, drawColor);
	//draw near-far connection lines
	DrawLine3D(nearRightTop, farRightTop, drawColor);
	DrawLine3D(nearLeftTop, farLeftTop, drawColor);
	DrawLine3D(nearLeftBot, farLeftBot, drawColor);
	DrawLine3D(nearRightBot, farRightBot, drawColor);
}

void VirtualCamera::DrawGUI()
{
	int screenHeight = GetScreenHeight();
	float elementWidth = 80.0f;
	float elementHeight = 15.0f;
	float elementOffsetY = 17.0f;
	float elementOffsetX = 150.0f;

	float groupAnchorX = 20.0f;
	float groupAnchorY = screenHeight - 7.0f * elementHeight;

	Rectangle GroupGUIbound;
	GroupGUIbound.x = groupAnchorX; GroupGUIbound.y = groupAnchorY;
	GroupGUIbound.width = elementWidth * 5.5f; GroupGUIbound.height = elementHeight * 6.5f;
	GuiGroupBox(GroupGUIbound, "Camera");

	float padX = 20.0f;
	float padY = 10.0f;
	float eyeSliderAnchorX = groupAnchorX + padX;
	float eyeSliderAnchorY = groupAnchorY + padY;

	Rectangle eyeGUIbound;
	eyeGUIbound.x = eyeSliderAnchorX; eyeGUIbound.y = eyeSliderAnchorY;
	eyeGUIbound.width = elementWidth; eyeGUIbound.height = elementHeight;
	GuiLabel(eyeGUIbound, "Eye");
	eyeGUIbound.y = eyeSliderAnchorY + elementOffsetY;
	eye.x = GuiSliderBar(eyeGUIbound, "x :", TextFormat("%.2f", eye.x), eye.x, -10.0f, 10.0f);
	eyeGUIbound.y = eyeSliderAnchorY + 2.0f * elementOffsetY;
	eye.y = GuiSliderBar(eyeGUIbound, "y :", TextFormat("%.2f", eye.y), eye.y, -10.0f, 10.0f);
	eyeGUIbound.y = eyeSliderAnchorY + 3.0f * elementOffsetY;
	eye.z = GuiSliderBar(eyeGUIbound, "z :", TextFormat("%.2f", eye.z), eye.z, -10.0f, 10.0f);

	float atSliderAnchorX = eyeSliderAnchorX + elementOffsetX;
	float atSliderAnchorY = eyeSliderAnchorY;

	Rectangle atGUIbound;
	atGUIbound.x = atSliderAnchorX; atGUIbound.y = atSliderAnchorY;
	atGUIbound.width = elementWidth; atGUIbound.height = elementHeight;
	GuiLabel(atGUIbound, "At");
	atGUIbound.y = atSliderAnchorY + elementOffsetY;
	at.x = GuiSliderBar(atGUIbound, "x :", TextFormat("%.2f", at.x), at.x, -10.0f, 10.0f);
	atGUIbound.y = atSliderAnchorY + 2.0f * elementOffsetY;
	at.y = GuiSliderBar(atGUIbound, "y :", TextFormat("%.2f", at.y), at.y, -10.0f, 10.0f);
	atGUIbound.y = atSliderAnchorY + 3.0f * elementOffsetY;
	at.z = GuiSliderBar(atGUIbound, "z :", TextFormat("%.2f", at.z), at.z, -10.0f, 10.0f);

	float projSliderAnchorX = atSliderAnchorX + elementOffsetX;
	float projSliderAnchorY = atSliderAnchorY;

	Rectangle projGUIbound;
	projGUIbound.x = projSliderAnchorX; projGUIbound.y = projSliderAnchorY;
	projGUIbound.width = elementWidth; projGUIbound.height = elementHeight;
	GuiLabel(projGUIbound, "Projection");
	projGUIbound.y = projSliderAnchorY + elementOffsetY;
	fovy = GuiSliderBar(projGUIbound, "fovy :", TextFormat("%.2f", fovy), fovy, 0.1f, 3.0f);
	projGUIbound.y = projSliderAnchorY + 2.0f * elementOffsetY;
	aspect = GuiSliderBar(projGUIbound, "aspect :", TextFormat("%.2f", aspect), aspect, 0.1f, 3.0f);
	projGUIbound.y = projSliderAnchorY + 3.0f * elementOffsetY;
	near = GuiSliderBar(projGUIbound, "near :", TextFormat("%.2f", near), near, 1.0f, 10.0f);
	projGUIbound.y = projSliderAnchorY + 4.0f * elementOffsetY;
	far = GuiSliderBar(projGUIbound, "far :", TextFormat("%.2f", far), far, 10.0f, 50.0f);
}

