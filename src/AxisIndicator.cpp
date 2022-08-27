#include "AxisIndicator.h"

#include "math_ext.h"

AxisIndicator::AxisIndicator(Vector3 origin, Vector3 e1, Vector3 e2, Vector3 e3)
	: origin{ origin },
	e1Endpoint{ Vector3Add(origin,Vector3Normalize(e1)) },
	e2Endpoint{ Vector3Add(origin,Vector3Normalize(e2)) },
	e3Endpoint{ Vector3Add(origin,Vector3Normalize(e3)) }
{
}
AxisIndicator::AxisIndicator()
	: AxisIndicator{ Vector3{0,0,0}, Vector3{1,0,0}, Vector3{0,1,0}, Vector3{0,0,1} }
{
}

void AxisIndicator::Draw3D() const
{
	DrawLine3D(origin, e1Endpoint, RED);
	DrawLine3D(origin, e2Endpoint, GREEN);
	DrawLine3D(origin, e3Endpoint, BLUE);
}

void AxisIndicator::Draw2D(const Camera& cam, const Rectangle& targetRect) const
{
	DrawCaption(e1Endpoint, RED, "x", cam, targetRect);
	DrawCaption(e2Endpoint, GREEN, "y", cam, targetRect);
	DrawCaption(e3Endpoint, BLUE, "z", cam, targetRect);
}

void AxisIndicator::DrawCaption(Vector3 endPoint, Color color, const char* text, const Camera& cam, const Rectangle& targetRect) const
{
	Vector2 vertPos2D = GetWorldToScreen(endPoint, cam);
	Vector2 vertPos2DInRT = MATH_EXT::RtRectToRtRectConversion(targetRect, vertPos2D);

	DrawText(text, vertPos2DInRT.x, vertPos2DInRT.y, 15, color);
}
