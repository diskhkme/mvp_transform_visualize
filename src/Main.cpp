#include "raylib.h"
#include "raymath.h"

#include "CameraController.h"
#include "VirtualCamera.h"
#include "VertexObject.h"
#include "AxisIndicator.h"

int main(void)
{
	const int screenWidth = 960;
	const int screenHeight = 540;

	InitWindow(screenWidth, screenHeight, "MVP Transform Viewer");

	SetTargetFPS(60);

	//--- World View Rendering Camera
	Camera mainCamera;
	mainCamera.position = Vector3{ 0.0f, 10.0f, 10.0f };  // Camera position
	mainCamera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	mainCamera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	mainCamera.fovy = 45.0f;                                // Camera field-of-view Y
	mainCamera.projection = CAMERA_PERSPECTIVE;
	CameraController camControl{ &mainCamera };

	//--- World Scene RT
	RenderTexture mainSceneRT = LoadRenderTexture(screenWidth / 2, screenHeight);
	Rectangle mainSceneRect = Rectangle{ 0.0f, 0.0f, (float)mainSceneRT.texture.width, (float)-mainSceneRT.texture.height };

	//--- View Scene RT
	RenderTexture viewSceneRT = LoadRenderTexture(screenWidth / 2, screenHeight / 2);
	Rectangle viewSceneRect = Rectangle{ 0.0f, 0.0f, (float)viewSceneRT.texture.width, (float)-viewSceneRT.texture.height };

	//--- Projection Scene RT
	RenderTexture projSceneRT = LoadRenderTexture(screenWidth / 2, screenHeight / 2);
	Rectangle projSceneRect = Rectangle{ 0.0f, 0.0f, (float)projSceneRT.texture.width, (float)-projSceneRT.texture.height };

	//--- Virtual Camera for Illustration
	Vector3 vCameraEye = Vector3{ 0.0f,0.0f,-5.0f };
	Vector3 vCameraAt = Vector3{ 0.0f,0.0f,0.0f };
	Vector3 vCameraWorldUp = Vector3{ 0.0f,1.0f,0.0f };
	float vCameraFOVY = 45.0f * DEG2RAD;

	VirtualCamera vCamera = VirtualCamera{ vCameraEye,vCameraAt,vCameraWorldUp,vCameraFOVY,16.0f / 9.0f,1.0f,10.0f };
	
	//--- Vertex for Illustration
	VertexObject v{ 0.0f,2.0f,0.0f };

	//--- Axis Indicator
	AxisIndicator axisIndicator;

	while (!WindowShouldClose())   
	{
		// Update
		camControl.Update();
		vCamera.Update();
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		// 1. Draw World Scene to texture
		BeginTextureMode(mainSceneRT);
			ClearBackground(RAYWHITE);
			BeginMode3D(mainCamera);
				DrawGrid(10, 1.0f);
				vCamera.Draw3D(SceneType::WORLD);

				v.ApplyTransform(MatrixIdentity());
				v.Draw3D();

				axisIndicator.Draw3D();
			EndMode3D();
			// 2D/UI Context Drawing
			vCamera.Draw2D();
			v.Draw2D(mainCamera, mainSceneRect);
			axisIndicator.Draw2D(mainCamera, mainSceneRect);

			DrawText("World Scene", 10, 10, 10, DARKGRAY);
		EndTextureMode();

		// 2. Draw View Scene to Texture
		BeginTextureMode(viewSceneRT);
			ClearBackground(RAYWHITE);
			BeginMode3D(mainCamera);
				DrawGrid(10, 1.0f);
				vCamera.Draw3D(SceneType::VIEW);
				// for view scene, we have to transform vertex to view space
				v.ApplyTransform(vCamera.GetViewMat());
				v.Draw3D();

				axisIndicator.Draw3D();
			EndMode3D();
			// 2D/UI Context Drawing
			vCamera.Draw2D();
			v.Draw2D(mainCamera, viewSceneRect);
			axisIndicator.Draw2D(mainCamera, viewSceneRect);

			DrawText("View Scene", 10, 10, 10, DARKGRAY);

			Matrix viewMat = vCamera.GetViewMat();
			DrawText(TextFormat("[%5.2f, %5.2f, %5.2f, %5.2f]", viewMat.m0, viewMat.m4, viewMat.m8, viewMat.m12), 10, 30, 10, BLACK);
			DrawText(TextFormat("[%5.2f, %5.2f, %5.2f, %5.2f]", viewMat.m1, viewMat.m5, viewMat.m9, viewMat.m13), 10, 40, 10, BLACK);
			DrawText(TextFormat("[%5.2f, %5.2f, %5.2f, %5.2f]", viewMat.m2, viewMat.m6, viewMat.m10, viewMat.m14), 10, 50, 10, BLACK);
			DrawText(TextFormat("[%5.2f, %5.2f, %5.2f, %5.2f]", viewMat.m3, viewMat.m7, viewMat.m11, viewMat.m15), 10, 60, 10, BLACK);

			DrawText(TextFormat("View Matrix"), 10, 20, 10, BLACK);
		EndTextureMode();

		// 2. Draw View Scene to Texture
		BeginTextureMode(projSceneRT);
			ClearBackground(RAYWHITE);
			BeginMode3D(mainCamera);
				DrawGrid(10, 1.0f);
				vCamera.Draw3D(SceneType::PROJECTION);
				// for view scene, we have to transform vertex to view space to clip space(& NDC)
				v.ApplyTransform(MatrixMultiply(vCamera.GetViewMat(), vCamera.GetProjMat()));
				v.Draw3D();

				axisIndicator.Draw3D();
			EndMode3D();
			// 2D/UI Context Drawing
			vCamera.Draw2D();
			v.Draw2D(mainCamera, projSceneRect);
			axisIndicator.Draw2D(mainCamera, projSceneRect);

			Matrix projMat = vCamera.GetProjMat();
			DrawText(TextFormat("[%5.2f, %5.2f, %5.2f, %5.2f]", projMat.m0, projMat.m4, projMat.m8, projMat.m12), 10, 30, 10, BLACK);
			DrawText(TextFormat("[%5.2f, %5.2f, %5.2f, %5.2f]", projMat.m1, projMat.m5, projMat.m9, projMat.m13), 10, 40, 10, BLACK);
			DrawText(TextFormat("[%5.2f, %5.2f, %5.2f, %5.2f]", projMat.m2, projMat.m6, projMat.m10,projMat.m14), 10, 50, 10, BLACK);
			DrawText(TextFormat("[%5.2f, %5.2f, %5.2f, %5.2f]", projMat.m3, projMat.m7, projMat.m11,projMat.m15), 10, 60, 10, BLACK);

			DrawText("Projection Scene", 10, 10, 10, DARKGRAY);
		EndTextureMode();

		BeginDrawing();
			// RT Texture Drawing
			DrawTextureRec(mainSceneRT.texture, mainSceneRect, Vector2{ 0,0 }, WHITE);
			DrawTextureRec(viewSceneRT.texture, mainSceneRect, Vector2{ screenWidth/2,0 }, WHITE);
			DrawTextureRec(projSceneRT.texture, mainSceneRect, Vector2{ screenWidth / 2, screenHeight/2 }, WHITE);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	{
		UnloadRenderTexture(mainSceneRT);
		UnloadRenderTexture(viewSceneRT);
		UnloadRenderTexture(projSceneRT);

		CloseWindow();
	}
	   

	return 0;
}