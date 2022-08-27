#pragma once

#include "raylib.h"
#include "raymath.h"

#include "rlgl.h"

#include "math_ext.h"
#include <sstream>

enum class SceneType
{
	WORLD, VIEW, PROJECTION
};

class VirtualCamera
{
public:
	VirtualCamera(Vector3 eye, Vector3 at, Vector3 worldUp, float fovy, float aspect, float near, float far);

	void Update();

	void Draw3D(SceneType sceneType) const;

	void Draw2D();

	Matrix GetViewMat() const;
	Matrix GetProjMat() const;

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void DrawCamera(SceneType sceneType) const;
	void DrawFrustum(SceneType sceneType) const;
	void DrawGUI();

private:
	// View parameters
	Vector3 eye;
	Vector3 at;
	Vector3 worldUp;
	Vector3 up;

	Vector3 u, v, n;
	Matrix viewMat;
	Matrix invViewMat;

	// Projection parameters
	float fovy;
	float aspect;
	float near;
	float far;
	Matrix projectionMat;
	Matrix invProjectionMat;
};
