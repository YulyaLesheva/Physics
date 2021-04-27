#pragma once
#include "stdafx.h"

struct Math {

	float DEG2RAD(float& degree) {
		return (degree * (math::PI / 180));
	}

	void ROTATE(FPoint& vector, float& angleDegrees, FPoint& origin) {

		float x = vector.x - origin.x;
		float y = vector.y - origin.y;

		float cos = (float)math::cos(DEG2RAD(angleDegrees));
		float sin = (float)math::sin(DEG2RAD(angleDegrees));

		float xPrime = (x * cos) - (y * sin);
		float yPrime = (x * sin) + (y * cos);

		xPrime += origin.x;
		yPrime += origin.y;

		vector.x = xPrime;
		vector.y = yPrime;
	}

	FPoint ROTATE_RETURN(FPoint vector, float angleDegrees, FPoint origin) {

		float x = vector.x - origin.x;
		float y = vector.y - origin.y;

		float cos = (float)math::cos(DEG2RAD(angleDegrees));
		float sin = (float)math::sin(DEG2RAD(angleDegrees));

		float xPrime = (x * cos) - (y * sin);
		float yPrime = (x * sin) + (y * cos);

		xPrime += origin.x;
		yPrime += origin.y;

		vector.x = xPrime;
		vector.y = yPrime;

		return FPoint(xPrime, yPrime);
	}

	FPoint Cross(FPoint& a, float s) {
		return FPoint(s * a.y, -s * a.x);
	}
	FPoint Cross(float s, FPoint& a) {
		return FPoint(-s * a.y, s * a.x);
	}
	
	float  Cross(FPoint& a, FPoint& b) {
		return a.x * b.y - a.y * b.x;
	}

	float Dot(const FPoint& a, const FPoint& b) {
		return a.x * b.x + a.y * b.y;
	}

	float Clamp(float a, float low, float high) {
		return math::max(low, math::min(a, high));
	}
};

