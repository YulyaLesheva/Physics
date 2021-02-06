#pragma once
#include "PhysicBody.h"
#include "Manifold.h"

struct Interval {
	float min;
	float max;
};

namespace BodyColission {

	void ApplyImpulse(Body* a, Body* b, Manifold* m, int c=0);
	Interval GetInterval(Body* a, const FPoint& axis);
	bool OverlapOnAxis(Body*a, Body*b, const FPoint &axis);
	bool SAT(Body* a, Body* b);
	float PenetrationDepth(Body* a, Body* b, FPoint& axis, bool* outShouldFlip);
	Manifold FindCollisionFeatures(Body* a, Body* b);
}