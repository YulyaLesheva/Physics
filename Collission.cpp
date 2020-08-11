#include "stdafx.h"
#include "Collission.h"

using namespace Collission;



bool Collission::AABBvsAABB(const IRect& box1, const IRect& box2) {

	if (box1.LeftBottom().x < box2.RightBottom().x && box1.RightBottom().x > box2.LeftBottom().x
		&& box1.LeftBottom().y < box2.LeftTop().y && box1.LeftTop().y > box2.LeftBottom().y) {
		return true;
	}

	return false;
}

float Collission::MinElastic(float e1, float e2) {
	if (e1 < e2) return e1;
	else return e2;
}

void Collission::ResolveCollision(std::unique_ptr<Quad>& box1, std::unique_ptr<Quad>& box2) {
	
	math::Vector3 relativeVelocity = math::Vector3(box1->GetPos().x - box2->GetPos().x, box1->GetPos().y - box2->GetPos().y, 0);

	auto normal = math::Vector3(1, 0, 0);
	float velocityALongNormal = relativeVelocity.DotProduct(normal);

	if (velocityALongNormal > 0) return;

	float e = MinElastic(0.5, 0.2);

	float j = -(1 + e) * velocityALongNormal;

	j /= box1->inverseMass + box2->inverseMass;

	math::Vector3 impulse = j * normal;
	
	float massSum = box1->mass + box2->mass;
	float ratio = box1->mass / massSum;
	box1->ApplyVector(-(ratio * impulse));

	ratio = box2->mass / massSum;
	box2->ApplyVector(ratio * impulse);

	PositionalCorrection(box1, box2);
}

void Collission::PositionalCorrection(std::unique_ptr<Quad>& box1, std::unique_ptr<Quad>& box2){
	
	auto penetrationDepth = 0.2;
	math::Vector3 normal = math::Vector3(0, 1, 0);
	const float precent = 0.3;
	math::Vector3 correction = penetrationDepth / (box1->inverseMass + box2->inverseMass) * normal;

	box1->ApplyVector(-(box1->inverseMass * correction));
	box2->ApplyVector(box2->inverseMass * correction);
}

bool Collission::ResolveCollissionAABB(Manifold* m) {
	
	auto Box1 = m->Box1;
	auto Box2 = m->Box2;

	math::Vector3 normal = math::Vector3(Box1->get()->GetPos().x - Box2->get()->GetPos().x,
	Box1->get()->GetPos().y - Box2->get()->GetPos().y, 0);

	auto b1Extent = Box1->get()->GetRect().width / 2;
	auto b2Extent = Box2->get()->GetRect().width / 2;

	float x_overlap = b1Extent + b2Extent - math::abs(normal.x);

	if (x_overlap > 0) {
		auto b1Extent = Box1->get()->GetRect().height / 2;
		auto b2Extent = Box2->get()->GetRect().height / 2;
		
		float y_overlap = b1Extent + b2Extent - math::abs(normal.y);

		if (y_overlap > 0) {
			if (x_overlap > y_overlap) {
				if (normal.x < 0) m->normal = math::Vector3(-1,0,0);
				else {
					m->normal = math::Vector3(0, 0, 0);
					m->penetration = x_overlap;
					return true;
				}

			}
			else {
				if (normal.y < 0) m->normal = math::Vector3(0, -1, 0);
				else{
					m->normal = math::Vector3(0,1,0);
					m->penetration = y_overlap;
					return true;
				}
			}
		}
	}

}