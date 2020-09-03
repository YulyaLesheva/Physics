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
					bodyOne->penetrationDepth = xOverlap;
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
					bodyOne->penetrationDepth = yOverlap;
					return true;
				}
			}
		}
	}
	return false;
}

void BodyColission::ResolveColission(Body* bodyOne, Body* bodyTwo) {
	
	auto bTwoVelocity = bodyTwo->velocity;
	auto bOneVelocity = bodyOne->velocity;

	math::Vector3 rVelocity = math::Vector3(bTwoVelocity.x-bOneVelocity.x, bTwoVelocity.y - bOneVelocity.y, 0);
	math::Vector3 normal = math::Vector3(bodyOne->GetNormal()); //1 , 0,  0

	// что то не то с релатив велосити. должно быть отрицательное значение 
	auto velocityAlongNormal = rVelocity.DotProduct(normal);
	 
	if (velocityAlongNormal > 0) return;

	auto elastic = MinElastic(bodyOne, bodyTwo);

	float j = -(1 + elastic) * velocityAlongNormal;

	j /= bodyOne->inverseMass + bodyTwo->inverseMass;

	math::Vector3 impulse = j * normal;

	bodyOne->velocity -= FPoint(bodyOne->inverseMass * impulse.x, bodyOne->inverseMass * impulse.y);
	bodyTwo->velocity += FPoint(bodyTwo->inverseMass * impulse.x, bodyTwo->inverseMass * impulse.y);
	   
	PosCorrection(bodyOne, bodyTwo);
}

float BodyColission::MinElastic(Body* bodyOne, Body* bodyTwo){
	
	if (bodyOne->elastic < bodyTwo->elastic) return bodyOne->elastic;
	else return bodyTwo->elastic;
}

void BodyColission::PosCorrection(Body* bodyOne, Body* bodyTwo) {
	
	const float percent = 0.7;
	FPoint correction = FPoint(bodyOne->penetrationDepth / (bodyOne->inverseMass + bodyTwo->inverseMass)
		* percent * bodyOne->_normal.x, bodyOne->penetrationDepth / (bodyOne->inverseMass + bodyTwo->inverseMass)
		* percent * bodyOne->_normal.y);

	bodyOne->_pos -= bodyOne->inverseMass * correction;
	bodyTwo->_pos += bodyTwo->inverseMass * correction;
}