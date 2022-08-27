#pragma once

#include "raylib.h"

#include "math_ext.h"

class VertexObject
{
public:
	VertexObject(float px, float py, float pz);

	void ApplyTransform(const Matrix& transform);

	void Draw3D() const;

	void Draw2D(const Camera& cam, const Rectangle& targetRect) const;

private:
	Vector3 position;
	Vector3 displayPosition;

};
