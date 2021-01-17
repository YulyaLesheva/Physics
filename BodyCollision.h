#pragma once
#include "Body.h"
#include "Manifold.h"

struct Interval {
	float min;
	float max;
};

namespace BodyColission {

	bool CheckColissionAndGetNormal(Body& bodyOne, Body& bodyTwo);
	bool CheckColission(Manifold *m);
	void ResolveCollide(Manifold *m);
	void PositionalCorrection(Body* a, Body* b, Manifold *m);
	void PosCorr(Body* a, Body* b, Manifold *m); 
	void ApplyImpulse(Body* a, Body* b, Manifold* m, int c=0);
	Interval GetInterval(Body* a, const FPoint& axis);
	bool OverlapOnAxis(Body*a, Body*b, const FPoint &axis);
	bool SAT(Body* a, Body* b);
	float PenetrationDepth(Body* a, Body* b, FPoint& axis, bool* outShouldFlip);
	Manifold FindCollisionFeatures(Body* a, Body* b);
}