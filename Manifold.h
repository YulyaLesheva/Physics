#pragma once
#include "stdafx.h"
#include "Body.h"

struct  Manifold
{
	Body* bodyOne = NULL;
	Body* bodyTwo = NULL;
	float penetration = 0;
	float depth = 0;
	bool colliding;
	math::Vector3 normal;
	FPoint mNormal;

	static Manifold* Create() {
		return new Manifold;
	}

	void ResetManifold(Manifold* result) {
		if (result != 0) {
			result->colliding = false;
			result->mNormal = FPoint(0, 0);
			result->depth = FLT_MAX;
		}
	}
};