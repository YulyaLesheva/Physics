#pragma once
#include "stdafx.h"
#include "Body.h"

struct  Manifold
{
	Body* bodyOne = NULL;
	Body* bodyTwo = NULL;
	float depth = 0;
	bool colliding = false;
	math::Vector3 normal;
	FPoint mNormal;
	FPoint mImpulse;

	static Manifold* Create() {
		return new Manifold;
	}

	void ResetManifold(Manifold* result) {
		if (result != 0) {
			result->colliding = false;
			result->mNormal = FPoint(0, 0);
			result->depth = FLT_MAX;
			result->mImpulse = FPoint(0,0);
		}
	}
};