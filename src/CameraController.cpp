#include "CameraController.h"

CameraController::CameraController(Camera* camera) 
	: m_Camera{ camera }
{
	Vector3 atToEye = Vector3Subtract(m_Camera->position, m_Camera->target);
	offset = Vector3Length(atToEye);

	atToEye = Vector3Normalize(atToEye);
	pitchAngle = acosf(atToEye.z);
	yawAngle = atan2f(atToEye.y, atToEye.x);
}

void CameraController::Update()
{
	bool isChanged = false;
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		Vector2 mouseDelta = GetMouseDelta();
		yawAngle += mouseDelta.x * turnSpeed * GetFrameTime();
		pitchAngle += mouseDelta.y * turnSpeed * GetFrameTime();
		if (mouseDelta.x != 0 || mouseDelta.y != 0)
		{
			isChanged = true;
		}

		if (pitchAngle > pitchLimit)
			pitchAngle = pitchLimit;
		if (pitchAngle < -pitchLimit)
			pitchAngle = -pitchLimit;
	}

	float wheel = GetMouseWheelMove();
	if (wheel != 0)
	{
		offset -= wheel * zoomSpeed * GetFrameTime();
		isChanged = true;
	}

	if (isChanged)
	{
		Vector3 frontVec;
		frontVec.x = cosf(yawAngle) * cosf(pitchAngle);
		frontVec.y = sinf(pitchAngle);
		frontVec.z = sinf(yawAngle) * cosf(pitchAngle);
		frontVec = Vector3Scale(frontVec, offset);

		Vector3 newPosition = Vector3Add(m_Camera->target, frontVec);

		m_Camera->position = newPosition;
	}
}

