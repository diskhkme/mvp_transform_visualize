#pragma once

#include "VertexObject.h"


VertexObject::VertexObject(float px, float py, float pz)
{
	position.x = px;
	position.y = py;
	position.z = pz;

	displayPosition = position;
}

void VertexObject::ApplyTransform(const Matrix& transform)
{
	displayPosition = MATH_EXT::MatrixVectorMultiplication(transform, position);
}

void VertexObject::Draw3D() const
{
	DrawCube(displayPosition, 0.1f, 0.1f, 0.1f, RED);
}

void VertexObject::Draw2D(const Camera& cam, const Rectangle& targetRect) const
{
	Vector2 vertPos2D = GetWorldToScreen(displayPosition, cam);
	Vector2 vertPos2DInRT = MATH_EXT::RtRectToRtRectConversion(targetRect, vertPos2D);

	DrawText(TextFormat("[%.2f, %.2f, %.2f]", displayPosition.x, displayPosition.y, displayPosition.z), vertPos2DInRT.x, vertPos2DInRT.y + 10, 20, RED);
}

