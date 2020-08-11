#pragma once
#include "Quad.h"

struct Manifold {
	std::unique_ptr<Quad>* Box1;
	std::unique_ptr<Quad>* Box2;
	float penetration;
	math::Vector3 normal;
};

namespace Collission {

	bool AABBvsAABB(const IRect& box1, const IRect& box2);
	
	//template <typename Object>

	float MinElastic(float e1, float e2);

	void ResolveCollision(std::unique_ptr<Quad>& box1, std::unique_ptr<Quad>& box2);
	
	void PositionalCorrection(std::unique_ptr<Quad>& box1, std::unique_ptr<Quad>& box2);

	bool ResolveCollissionAABB(Manifold* m);
}