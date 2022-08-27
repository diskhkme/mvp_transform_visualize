#pragma once

#include "raylib.h"
#include "raymath.h"

#define CAMERA_IMPLEMENTATION

class CameraController
{
public:
	CameraController(Camera* camera);
	
	void Update();

private:
	Camera* m_Camera;

	// Camera state
	float yawAngle, pitchAngle;
	float offset;

	// Control Parameters
	float turnSpeed = 0.5f;
	float zoomSpeed = 50.0f;
	float pitchLimit = 89.0f * DEG2RAD;
};
