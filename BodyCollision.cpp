#include "stdafx.h"
#include "BodyCollision.h"

//using namespace BodyColission;


bool BodyColission::CheckColissionAndGetNormal(Body& bodyOne, Body& bodyTwo) {

	auto bTwoPosition = bodyTwo.GetPos();
	auto bOnePosition = bodyOne.GetPos();
	
	auto bOneTex = bodyOne.GetTex();
	auto bTwoTex = bodyTwo.GetTex();

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
					bodyOne._normal = math::Vector3(-1, 0, 0); 
					bodyOne.penetrationDepth = xOverlap;
					return true;
				}
				else {
					bodyOne._normal = math::Vector3(1, 0, 0);
					bodyOne.penetrationDepth = xOverlap;
					return true;
				}
			}
			else {
				if (n.y < 0) {
					bodyOne._normal = math::Vector3(0, -1, 0);
					bodyOne.penetrationDepth = yOverlap;
					return true;
				}
				else {
					bodyOne._normal = math::Vector3(0, 1, 0);
					bodyOne.penetrationDepth = yOverlap;
					return true;
				}
			}
		}
	}
	return false;
}

void BodyColission::ResolveColission(Body& bodyOne, Body& bodyTwo) {
	
	auto bTwoVelocity = bodyTwo.velocity;
	auto bOneVelocity = bodyOne.velocity;

	math::Vector3 rVelocity = math::Vector3(bTwoVelocity.x-bOneVelocity.x, bTwoVelocity.y - bOneVelocity.y, 0);
	math::Vector3 normal = math::Vector3(bodyOne.GetNormal()); //1 , 0,  0

	// ��� �� �� �� � ������� ��������. ������ ���� ������������� �������� 
	auto velocityAlongNormal = rVelocity.DotProduct(normal);
	 
	if (velocityAlongNormal > 0) return;

	auto elastic = MinElastic(bodyOne, bodyTwo);

	float j = -(1 + elastic) * velocityAlongNormal;

	j /= bodyOne.inverseMass + bodyTwo.inverseMass;

	math::Vector3 impulse = j * normal;

	bodyOne.velocity -= FPoint(bodyOne.inverseMass * impulse.x, bodyOne.inverseMass * impulse.y);
	bodyTwo.velocity += FPoint(bodyTwo.inverseMass * impulse.x, bodyTwo.inverseMass * impulse.y);
	
	//LATER HERE WILL BE A FRICTION IMPLIMENTING

	PositionalCorrection(bodyOne, bodyTwo);

}

float BodyColission::MinElastic(Body& bodyOne, Body& bodyTwo){
	
	if (bodyOne.elastic < bodyTwo.elastic) return bodyOne.elastic;
	else return bodyTwo.elastic;
}

void BodyColission::PositionalCorrection(Body& bodyOne, Body& bodyTwo) {

	auto totalMass = bodyOne.mass + bodyTwo.mass;
	auto linearProjeectionPercent = 0.3f;
	auto penetrationSlack = 0.01f;
	auto impulseIteration = 5;

	float depth = math::max(bodyOne.penetrationDepth - penetrationSlack, 0.0f);

	float scalar = depth / totalMass;

	FPoint correction = FPoint(bodyOne._normal.x * scalar * linearProjeectionPercent,
		bodyOne._normal.y * scalar * linearProjeectionPercent);

	bodyOne._pos -= correction * bodyOne.inverseMass;
	bodyTwo._pos += correction * bodyTwo.inverseMass;
}

void BodyColission::ApplyImpulse(Body* a, Body* b, Manifold* m, int c) {

	auto invMassA = a->inverseMass;
	auto invMassB = b->inverseMass;

	auto invMassSum = invMassA + invMassB;

	if (invMassSum == 0) return;

	//relative velocity
	auto relativeVel = math::Vector3(b->velocity.x - a->velocity.x, b->velocity.y - a->velocity.y, 0);
	//relative collision normal 
	auto relativeNorm = m->normal.Normalized();
	
	auto dotProduct = relativeVel.DotProduct(relativeNorm);
	
	if (dotProduct > 0) return;

	float e = math::min(a->elastic, b->elastic); //coefficient of restitution
	float numerator = (-(1.f + e) * dotProduct);
	float j = numerator / invMassSum;
	
	//// ��� ��������������� ���������� �����, �������� ������� ���������� �������� j \\\\
	
	//j /= �� ���������� ����� ��� �� ��� ��� ���� 394 � ��� ������ �������� ;

	auto impulse = relativeNorm * j;
	
	a->velocity -= FPoint(impulse.x * invMassA, impulse.y * invMassA);
	b->velocity += FPoint(impulse.x * invMassB, impulse.y *invMassB);

}

Manifold BodyColission::ColissionFeatures(Body& a, Body& b) {
	Manifold result;
	return result;
}

bool BodyColission::CheckColission(Manifold *m){
	
	Body *bodyOne = m->bodyOne;
	Body *bodyTwo = m->bodyTwo;

	auto bOnePos = bodyOne->GetPos();
	auto bTwoPos = bodyTwo->GetPos();

	auto bOneTex = bodyOne->GetTex();
	auto bTwoTex = bodyTwo->GetTex();

	auto n = math::Vector3(bTwoPos.x - bOnePos.x, bTwoPos.y-bOnePos.y, 0);
	
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
					m->normal = math::Vector3(-1, 0, 0);
					m->penetration = xOverlap;
					return true;
				}
				else {
					m->normal = math::Vector3(1, 0, 0);
					m->penetration = xOverlap;
					return true;
				}
			}
			else {
				if (n.y < 0) {
					m->normal = math::Vector3(0, -1, 0);
					m->penetration = yOverlap;
					return true;
				}
				else {
					m->normal = math::Vector3(0, 1, 0);
					m->penetration = yOverlap;
					return true;
				}
			}
		}
	}
	return false;
}

void BodyColission::ResolveCollide(Manifold *m) {

	Body *bodyOne = m->bodyOne;
	Body *bodyTwo = m->bodyTwo;
	
	auto bTwoVelocity = bodyTwo->velocity;
	auto bOneVelocity = bodyOne->velocity;

	math::Vector3 rVelocity = math::Vector3(bTwoVelocity.x - bOneVelocity.x, bTwoVelocity.y - bOneVelocity.y, 0);
	math::Vector3 normal = m->normal; //1 , 0,  0

	// ��� �� �� �� � ������� ��������. ������ ���� ������������� �������� 
	auto velocityAlongNormal = rVelocity.DotProduct(normal);

	if (velocityAlongNormal > 0) return;

	auto elastic = MinElastic(*bodyOne, *bodyTwo);

	float j = -(1 + elastic) * velocityAlongNormal;

	j /= bodyOne->inverseMass + bodyTwo->inverseMass;

	math::Vector3 impulse = j * normal;

	bodyOne->velocity -= FPoint(bodyOne->inverseMass * impulse.x, bodyOne->inverseMass * impulse.y);
	bodyTwo->velocity += FPoint(bodyTwo->inverseMass * impulse.x, bodyTwo->inverseMass * impulse.y);

	//LATER HERE WILL BE A FRICTION IMPLIMENTING

	PosCorr(m);
}

void BodyColission::PosCorr(Manifold *m) {

	Body *bodyOne = m->bodyOne;
	Body *bodyTwo = m->bodyTwo;

	auto totalMass = bodyOne->mass + bodyTwo->mass;
	auto linearProjeectionPercent = 0.45f; //��� �������� 0.8 ������������� ������. ���.400 �����
	auto penetrationSlack = 0.01f;
	auto impulseIteration = 7;
		 //0.45 0.01 7 - works good
	
	/*for (auto i = 0; i < impulseIteration; i++) {*/
		
		float depth = math::max(m->penetration - penetrationSlack, 0.0f);

		float scalar = depth / totalMass;

		FPoint correction = FPoint(m->normal.x * scalar * linearProjeectionPercent,
			m->normal.y * scalar * linearProjeectionPercent);

		bodyOne->_pos -= correction * bodyOne->inverseMass;
		bodyTwo->_pos += correction * bodyTwo->inverseMass;
	//}
}