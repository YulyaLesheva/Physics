#pragma once
#include "Body.h"
#include "Manifold.h"

namespace BodyColission {

	bool CheckColissionAndGetNormal(Body& bodyOne, Body& bodyTwo);
	void ResolveColission(Body& bodyOne, Body& bodyTwo);
	bool CheckColission(Manifold *m);
	void ResolveCollide(Manifold *m);
	float MinElastic(Body& bodyOne, Body& bodyTwo);
	void PositionalCorrection(Body& bodyOne, Body& bodyTwo);
	void PosCorr(Manifold *m); 
	
	void ApplyImpulse(Body* a, Body* b, Manifold* m, int c=0);

	Manifold ColissionFeatures(Body& a, Body& b);
}