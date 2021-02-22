#pragma once
#include "stdafx.h"
#include "Body.h"

struct  Manifold
{
	float depth;
	bool colliding = false;
	FPoint mNormal;
	std::vector<FPoint> contacts;

	static Manifold* Create() {
		return new Manifold;
	}

	void ResetManifold(Manifold* result) {
		if (result != 0) {
			result->colliding = false;
			result->mNormal = FPoint(0, 0);
			result->depth = FLT_MAX;
			//result->contacts.clear();
		}
	}
};