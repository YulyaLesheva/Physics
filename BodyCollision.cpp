#include "stdafx.h"
#include "BodyCollision.h"

//using namespace BodyColission;

void BodyColission::ApplyImpulse(Body* a, Body* b, Manifold* m, int c) {

	auto invMassA = a->inverseMass;
	auto invMassB = b->inverseMass;

	auto invMassSum = invMassA + invMassB;

	if (invMassSum == 0) return;
	
	//relative velocity
	FPoint relativeVelocity = b->velocity - a->velocity;
	//relative collision normal 
	float velocityAlnogNormal = relativeVelocity.GetDotProduct(m->mNormal);
	
	if (velocityAlnogNormal > 0) return;
	
	float e = math::min(a->elastic, b->elastic);
	float j = -(1 + e) * velocityAlnogNormal;
	
	if (j != 0.0f) j /= invMassSum;
	
	FPoint impulse = j * m->mNormal;
	
	//j += (m->depth * 1.5f);

	a->velocity -= impulse * a->inverseMass;
	b->velocity += impulse * b->inverseMass;

	//add friction implementation
	
	FPoint t = relativeVelocity - (m->mNormal * velocityAlnogNormal);
	float jt = -relativeVelocity.GetDotProduct(t);
	
	jt /= invMassSum;
	if (jt == 0.f) return;

	float friction = sqrtf(a->friction * b->friction);
	if (jt > j*friction) jt = j * friction;
	else if (jt < -j * friction) jt = -j * friction;
	
	FPoint tangetImpulse = t * jt;
	a->velocity -= tangetImpulse * a->inverseMass;
	b->velocity += tangetImpulse * b->inverseMass;
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