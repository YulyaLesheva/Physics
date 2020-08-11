#include "stdafx.h"
#include "BodyCollision.h"

//using namespace BodyColission;

bool BodyColission::CheckColissionAndGetNormal(Body* bodyOne, Body* bodyTwo) {

	auto bTwoPosition = bodyTwo->GetPos();
	auto bOnePosition = bodyOne->GetPos();
	
	auto bOneTex = bodyOne->GetTex();
	auto bTwoTex = bodyTwo->GetTex();

	auto n = math::Vector3(bTwoPosition.x - bOnePosition.x, bTwoPosition.y - bOnePosition.y, 0);

	float bOneExtent = (bOneTex->Width() * 0.5);
	float bTwoExtent = (bTwoTex->Width() * 0.5);

	float xOverlap = bOneExtent + bTwoExtent - abs(n.x);

	if (xOverlap > 0) {
		float bOneExtent = (bOneTex->Height() * 0.5);
		float bTwoExtent = (bTwoTex->Height() * 0.5);

		float yOverlap = bOneExtent + bTwoExtent - abs(n.y);

		if (yOverlap > 0) {
			if (xOverlap < yOverlap) {
				if (n.x < 0) {
					bodyOne->_normal = math::Vector3(-1, 0, 0); 
					return true;
				}
				else {
					bodyOne->_normal = math::Vector3(1, 0, 0);
					return true;
				}
			}
			else {
				if (n.y < 0) {
					bodyOne->_normal = math::Vector3(0, -1, 0);
					return true;
				}
				else {
					bodyOne->_normal = math::Vector3(0, 1, 0);
					return true;
				}
			}
		}
	}
	return false;
}

void BodyColission::ResolveColission(Body* bodyOne, Body* bodyTwo) {
	
	auto bTwoPosition = bodyTwo->velocity;
	auto bOnePosition = bodyOne->velocity;

	math::Vector3 rVelocity = math::Vector3(bTwoPosition.x-bOnePosition.x, bTwoPosition.y - bOnePosition.y, 0);
	///rVelocity.Normalize();
	math::Vector3 NORMAL = math::Vector3(-1,0,0);
	math::Vector3 normal = math::Vector3(bodyOne->GetNormal()); //1 , 0,  0

	// что то не то с релатив велосити. должно быть отрицательное значение 
	auto velocityAlongNormal = rVelocity.DotProduct(normal);
	 
	if (velocityAlongNormal > 0) return;

	auto elastic = 2.9;

	float j = -(1 + elastic) * velocityAlongNormal;

	j /= bodyOne->inverseMass + bodyTwo->inverseMass;

	math::Vector3 impulse = j * normal;

	bodyOne->velocity -= IPoint(bodyOne->inverseMass * impulse.x, bodyOne->inverseMass * impulse.y);
	bodyTwo->velocity += IPoint(bodyTwo->inverseMass * impulse.x, bodyTwo->inverseMass * impulse.y);
}
