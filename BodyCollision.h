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
	void PositionalCorrection(Manifold *m);
	void PosCorr(Manifold *m); 
	void ApplyImpulse(Body* a, Body* b, Manifold* m, int c=0);
	Interval GetInterval(Body* a, const FPoint& axis);
	bool OverlapOnAxis(Body*a, Body*b, const FPoint &axis);
	Manifold ColissionFeatures(Body& a, Body& b);
	bool SAT(Body* a, Body* b);
}