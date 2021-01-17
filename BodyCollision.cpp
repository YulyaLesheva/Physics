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




void BodyColission::ApplyImpulse(Body* a, Body* b, Manifold* m, int c) {

	auto invMassA = a->inverseMass;
	auto invMassB = b->inverseMass;

	auto invMassSum = invMassA + invMassB;

	if (invMassSum == 0) return;
	
	for (auto i = 0; i < c; i++) {
		//relative velocity
		auto relativeVel = FPoint(b->velocity - a->velocity);
		//relative collision normal 
		auto relativeNorm = m->mNormal;

		auto dotProduct = relativeVel.GetDotProduct(relativeNorm);

		if (dotProduct > 0) return;

		float e = math::min(a->elastic, b->elastic); //coefficient of restitution
		float numerator = (-(1.f + e) * dotProduct);
		float j = numerator / invMassSum;

		//// при соприкосновнеии нескольких точек, добавить условие уменьшения значения j \\\\
		 // на количество точек или хз что там стрю 394 и это должно работать;

		if (j != 0.0f) j /= invMassSum;

		auto impulse = relativeNorm * j;

		a->velocity -= FPoint(impulse * invMassA);
		b->velocity += FPoint(impulse * invMassB);

		//add friction implementation
		//relativeVel = math::Vector3(b->velocity.x - a->velocity.x,
		//	b->velocity.y - a->velocity.y, 0);
		//
		//dotProduct = relativeNorm.DotProduct(relativeVel);

		//auto t = relativeVel - (relativeNorm * (dotProduct));
		//t.Normalize();
		//numerator = -(relativeVel.DotProduct(t));
		//float jt = numerator / invMassSum;
		//jt /= invMassSum;
		//
		//float friction = sqrtf(a->friction * b->friction);
		//if (jt > j * friction) {
		//	jt = j * friction;
		//}
		//else if (jt < -j * friction) {
		//	jt = -j * friction;
		//}
		//
		//auto tangetImpulse = t * jt;

		//a->velocity -= FPoint(tangetImpulse.x * invMassA, tangetImpulse.y * invMassA);
		//b->velocity += FPoint(tangetImpulse.x * invMassB, tangetImpulse.y * invMassB);

		PositionalCorrection(a, b, m);
		//NewPositionCorrection(a,b,m);
	}
}

void BodyColission::PositionalCorrection(Body* a, Body* b, Manifold *m) {

	auto totalMass = a->mass + b->mass;

	if (totalMass == 0.f) return;

	auto linearProjectionPercent = 0.8;
	float penetrationSlack = 0.1;

	//auto penetrationCheck = m->depth;

	float depth = math::max(m->depth - penetrationSlack, 0.f);
	float scalar = depth / totalMass;
	auto correction = m->mNormal * scalar * linearProjectionPercent;

	a->_pos -= FPoint(correction * a->inverseMass);
	b->_pos += FPoint(correction * b->inverseMass);

}

bool BodyColission::CheckColission(Manifold *m){
	//
	//Body *bodyOne = m->bodyOne;
	//Body *bodyTwo = m->bodyTwo;

	//auto bOnePos = bodyOne->GetPos();
	//auto bTwoPos = bodyTwo->GetPos();

	//auto bOneTex = bodyOne->GetTex();
	//auto bTwoTex = bodyTwo->GetTex();

	//auto n = math::Vector3(bTwoPos.x - bOnePos.x, bTwoPos.y-bOnePos.y, 0);
	//
	//float bOneExtent = (bOneTex->Width() * 0.5);
	//float bTwoExtent = (bTwoTex->Width() * 0.5);

	//float xOverlap = bOneExtent + bTwoExtent - abs(n.x);

	//if (xOverlap > 0) {
	//	float bOneExtent = (bOneTex->Height() * 0.5);
	//	float bTwoExtent = (bTwoTex->Height() * 0.5);

	//	float yOverlap = bOneExtent + bTwoExtent - abs(n.y);

	//	if (yOverlap > 0) {

	//		if (xOverlap < yOverlap) {
	//			if (n.x < 0) {
	//				m->normal = math::Vector3(-1, 0, 0);
	//				//m->penetration = xOverlap;
	//				return true;
	//			}
	//			else {
	//				m->normal = math::Vector3(1, 0, 0);
	//				//m->penetration = xOverlap;
	//				return true;
	//			}
	//		}
	//		else {
	//			if (n.y < 0) {
	//				m->normal = math::Vector3(0, -1, 0);
	//				//m->penetration = yOverlap;
	//				return true;
	//			}
	//			else {
	//				m->normal = math::Vector3(0, 1, 0);
	//				//m->penetration = yOverlap;
	//				return true;
	//			}
	//		}
	//	}
	//}
	return false;
}

void BodyColission::ResolveCollide(Manifold *m) {
//
//	Body *bodyOne = m->bodyOne;
//	Body *bodyTwo = m->bodyTwo;
//	
//	auto bTwoVelocity = bodyTwo->velocity;
//	auto bOneVelocity = bodyOne->velocity;
//
//	auto inverseMassSum = bodyOne->inverseMass + bodyTwo->inverseMass;
//
//	math::Vector3 rVelocity = math::Vector3(bTwoVelocity.x - bOneVelocity.x, bTwoVelocity.y - bOneVelocity.y, 0);
//	math::Vector3 normal = m->normal; //1 , 0,  0
//
//// что то не то с релатив велосити. должно быть отрицательное значение 
//auto velocityAlongNormal = rVelocity.DotProduct(normal);
//
//if (velocityAlongNormal > 0) return;
//
//auto elastic = math::min(bodyOne->elastic, bodyTwo->elastic);
//
//float j = -(1 + elastic) * velocityAlongNormal;
//
//j /= inverseMassSum;
//
//math::Vector3 impulse = j * normal;
//
//bodyOne->velocity -= FPoint(bodyOne->inverseMass * impulse.x, bodyOne->inverseMass * impulse.y);
//bodyTwo->velocity += FPoint(bodyTwo->inverseMass * impulse.x, bodyTwo->inverseMass * impulse.y);

//LATER HERE WILL BE A FRICTION IMPLIMENTING
//PositionalCorrection(m);

}

void BodyColission::PosCorr(Body* a, Body* b, Manifold *m) {

	auto totalMass = a->mass + b->mass;
	auto linearProjeectionPercent = 0.8f; //при значении 0.8 проникновение меньше. стр.400 книги
	auto penetrationSlack = 0.02f;
	auto impulseIteration = 7;
	//0.45 0.01 7 - works good


	auto checkDepth = math::max(m->depth - penetrationSlack, 0.0f);
	auto depth = math::max(m->depth - penetrationSlack, 0.0f) / (totalMass)* linearProjeectionPercent * m->mNormal;

	a->_pos -= FPoint(depth * a->inverseMass);
	b->_pos += FPoint(depth * b->inverseMass);
}

Interval BodyColission::GetInterval(Body* a, const FPoint& axis) {

	Interval result;

	FPoint min = a->GetMin();
	FPoint max = a->GetMax();

	FPoint verst[] = {
	FPoint(min.x, min.y), FPoint(min.x, max.y),
	FPoint(max.x, max.y), FPoint(max.x, min.y)
	};

	result.min = result.max = axis.GetDotProduct(verst[0]);

	for (int i = 1; i < 4; ++i) {
		float projection = axis.GetDotProduct(verst[i]);
		if (projection < result.min) result.min = projection;
		if (projection > result.max) result.max = projection;
	}

	return result;
}

bool BodyColission::OverlapOnAxis(Body*a, Body*b, const FPoint &axis) {

	Interval iA = GetInterval(a, axis);
	Interval iB = GetInterval(b, axis);

	return ((iB.min <= iA.max) && (iA.min <= iB.max));
}

bool BodyColission::SAT(Body* a, Body* b) {

	FPoint axisXY[] = {
	FPoint(1, 0), FPoint(0, 1)
	}; //добавить две дополнительные оси для вращающихся квадов позже

	for (int i = 0; i < 2; ++i) {
		if (!OverlapOnAxis(a, b, axisXY[i])) {
			Log::Info(".....");
			return false;

		}
	}
	Log::Info("It's a colission!");
	return true;
}

float BodyColission::PenetrationDepth(Body *a, Body* b, FPoint& axis, bool* outShouldFlip) {
	
	Interval iA = GetInterval(a, axis);
	Interval iB = GetInterval(b, axis);

	if (!OverlapOnAxis(a,b,axis)){
		return 0.0f;
	}
	
	float lengthA = iA.max - iA.min;
	float lengthB = iB.max - iB.min;

	float min = fminf(iA.min, iB.min);
	float max = fmaxf(iA.max, iB.max);

	float length = max - min;

	if (outShouldFlip != 0) {
		*outShouldFlip = (iB.min < iA.min);
	}

	auto total = (lengthA + lengthB) - length;
	
	return total;
}

Manifold BodyColission::FindCollisionFeatures(Body* a, Body* b) {

	Manifold result;
	//here function reset collision manifold
	result.ResetManifold(&result);
	
	FPoint axisXY[] = {
	FPoint(1, 0), FPoint(0, 1)
	};

	FPoint* hitNormal = 0; 
	bool shouldFlip;

	for (int i = 0; i < 2; ++i) {
		
		float depth = PenetrationDepth(a, b, axisXY[i], &shouldFlip);

		if (depth <= 0.f) {
			return result;
		}
		
		else if (depth < result.depth) {
			if (shouldFlip) {
				axisXY[i] = axisXY[i] * -1;
			}
			result.depth = depth;
			hitNormal = &axisXY[i];
		}
	}

	if (hitNormal == 0) return result;
	
	FPoint axis = hitNormal->Normalized();

	result.colliding = true;
	result.mNormal = axis;

	return result;
}