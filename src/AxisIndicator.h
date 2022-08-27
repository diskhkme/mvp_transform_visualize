#pragma once

#include "raylib.h"

class AxisIndicator
{
public:
	AxisIndicator(Vector3 origin, Vector3 e1, Vector3 e2, Vector3 e3);
	AxisIndicator();

	void Draw3D() const;
	void Draw2D(const Camera& cam, const Rectangle& targetRect) const;

private:
	void DrawCaption(Vector3 endPoint, Color color, const char* text, const Camera& cam, const Rectangle& targetRect) const;

private:
	Vector3 origin;
	Vector3 e1Endpoint;
	Vector3 e2Endpoint;
	Vector3 e3Endpoint;
};
