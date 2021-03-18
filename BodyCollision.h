#pragma once
#include "PhysicBody.h"
#include "Manifold.h"

struct Interval {
	float min;
	float max;
};

namespace BodyCollision {

	void ApplyImpulse(PhysicBody* a, PhysicBody* b, Manifold* m, int c=0);
	void ApplyImpulse2D(PhysicBody* a, PhysicBody * b, Manifold& m, float dt);
	Interval GetInterval(PhysicBody* a, const FPoint& axis);
	bool OverlapOnAxis(PhysicBody*a, PhysicBody*b, const FPoint &axis);
	bool SAT(PhysicBody* a, PhysicBody* b);
	float PenetrationDepth(PhysicBody* a, PhysicBody* b, FPoint& axis, bool* outShouldFlip);
	Manifold FindCollisionFeatures(PhysicBody* a, PhysicBody* b);
	float normalMass(PhysicBody* a, PhysicBody* b);
}