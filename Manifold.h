#pragma once
#include "stdafx.h"
#include "Body.h"

struct  Manifold
{
	Body* bodyOne = NULL;
	Body* bodyTwo = NULL;
	float penetration = 0;
	math::Vector3 normal;

	static Manifold* Create() {
		return new Manifold;
	}
};