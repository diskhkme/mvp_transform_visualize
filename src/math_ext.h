#pragma once

#include "raymath.h"

class MATH_EXT
{
public:
	static Vector3 MatrixVectorMultiplication(const Matrix& m, const Vector3& v)
	{
		Vector3 ret;

		// assume w=1 homogeneous coord
		float x = m.m0 * v.x + m.m4 * v.y + m.m8 * v.z + m.m12 * 1.0f;
		float y = m.m1 * v.x + m.m5 * v.y + m.m9 * v.z + m.m13 * 1.0f;
		float z = m.m2 * v.x + m.m6 * v.y + m.m10 * v.z + m.m14 * 1.0f;
		float w = m.m3 * v.x + m.m7 * v.y + m.m11 * v.z + m.m15 * 1.0f;

		ret.x = x / w;
		ret.y = y / w;
		ret.z = z / w;

		return ret;
	}

	static Vector2 RtRectToRtRectConversion(const Rectangle& target, const Vector2& sourceVec)
	{
		// source rect is always assumed to be {0,0,screenWidth, screenHeight}
		Vector2 targetVec;

		float scaleX = target.width / GetScreenWidth();
		float scaleY = -target.height / GetScreenHeight();;

		targetVec.x = sourceVec.x * scaleX;
		targetVec.y = sourceVec.y * scaleY;

		return targetVec;
	}
};
