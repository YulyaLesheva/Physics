#pragma once
#include "stdafx.h"
struct Math {

	float DEG2RAD(float degree) {
		return (degree * (math::PI / 180));
	}

	void ROTATE(FPoint vector, float angleDegrees, FPoint origin) {

		float x = vector.x - origin.x;
		float y = vector.y - origin.y;

		float cos = (float)math::cos(angleDegrees);
		float sin = (float)math::sin(angleDegrees);

		float xPrime = (x * cos) - (y * sin);
		float yPrime = (x * sin) + (y * cos);

		xPrime = origin.x;
		yPrime = origin.y;

		vector.x = xPrime;
		vector.y = yPrime;
	}
};

