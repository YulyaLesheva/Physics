#include "stdafx.h"
#include "BodyCollision.h"

//using namespace BodyColission;

void BodyCollision::ApplyImpulse(PhysicBody* a, PhysicBody* b, Manifold* m, int c) {

	auto invMassA = a->inverseMass;
	auto invMassB = b->inverseMass;

	auto invMassSum = invMassA + invMassB;

	FPoint relativeVel = b->velocity - a->velocity;
	FPoint relativeNorm = m->normal;
	
	relativeNorm.Normalize();
	if (relativeNorm.GetDotProduct(relativeVel) > 0.001f) return;

	float e = fminf(a->elastic, b->elastic);
	float numerator = (-(1.0 + e) * relativeVel.GetDotProduct(relativeNorm));
	float j = (invMassSum == 0.0f) ? 0.0f : numerator / invMassSum;

	if (m->contacts.size() > 0.0f && j != 0.0f) {
		j /= (float)m->contacts.size();
	}
	FPoint impulse = relativeNorm * j;

	a->WHATIMPULSE = impulse * a->inverseMass;
	b->WHATIMPULSE = impulse * b->inverseMass;

	if (a->WHATIMPULSE.y == 0 ) {
		Log::Info("impulse is zerooooo");
	}
	else {
		Log::Info(std::to_string(impulse.x) + " " + std::to_string(impulse.y));
	}

	a->velocity -= impulse * a->inverseMass;
	b->velocity += impulse * b->inverseMass;

	//friction impelementation 

	FPoint t = relativeVel - (relativeNorm *  relativeVel.GetDotProduct(relativeNorm));
	
	if (numerator == 0) return;
	numerator = -relativeVel.GetDotProduct(t);
	float jt = numerator / invMassSum;
	
	if (m->contacts.size() > 0.0f && jt != 0.0f) {
		jt /= (float)m->contacts.size();
	}
	float friction = sqrtf(a->friction * b->friction);
	if (jt > j * friction) {
		jt = j * friction;
	}
	else if (jt < -j * friction) {
		jt = -j * friction;
	}

	FPoint tangentImpulse = t * jt;
	a->velocity -= tangentImpulse * invMassA;
	b->velocity += tangentImpulse * invMassB;
}

Interval BodyCollision::GetInterval(PhysicBody* a, const FPoint& axis) {

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

bool BodyCollision::OverlapOnAxis(PhysicBody*a, PhysicBody*b, const FPoint &axis) {

	Interval iA = GetInterval(a, axis);
	Interval iB = GetInterval(b, axis);

	return ((iB.min <= iA.max) && (iA.min <= iB.max));
}

bool BodyCollision::SAT(PhysicBody* a, PhysicBody* b) {

	FPoint axisXY[] = {
	FPoint(1, 0), FPoint(0, 1)
	}; // доюавить две доп оси для вращения

	for (int i = 0; i < 2; ++i) {
		if (!OverlapOnAxis(a, b, axisXY[i])) {
			//Log::Info(".....");
			return false;

		}
	}
	//Log::Info("It's a colission!");
	return true;
}

float BodyCollision::PenetrationDepth(PhysicBody *a, PhysicBody* b, FPoint& axis, bool* outShouldFlip) {

	Interval iA = GetInterval(a, axis);
	Interval iB = GetInterval(b, axis);

	if (!OverlapOnAxis(a, b, axis)) {
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

Manifold BodyCollision::FindCollisionFeatures(PhysicBody* a, PhysicBody* b) {

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
				axisXY[i] = axisXY[i] * -1.f;
			}
			result.depth = depth;
			hitNormal = &axisXY[i];
		}
	}

	if (hitNormal == 0) return result;

	FPoint axis = hitNormal->Normalized();
	
	std::vector<FPoint> c1 = a->ClipToEdges(a, b);
	std::vector<FPoint> c2 = b->ClipToEdges(b, a);

	result.contacts.reserve(c1.size() + c2.size());
	result.contacts.insert(result.contacts.end(), c1.begin(), c1.end());
	result.contacts.insert(result.contacts.end(), c2.begin(), c2.end());

	Interval i = GetInterval(a, axis);
	float distance = (i.max - i.min) * 0.5f - result.depth * 0.5f;
	FPoint pointOnPlane = a->_pos + axis * distance;
	
	for (int i = result.contacts.size() - 1; i >= 0; --i) {
		FPoint contact = result.contacts[i];
		result.contacts[i] = contact + (axis * axis.GetDotProduct(pointOnPlane - contact));

		for (int j = result.contacts.size() - 1; j > i; --j) {
			if ((result.contacts[j] - result.contacts[i]).GetDotProduct(result.contacts[j] 
				- result.contacts[i]) < 0.0001f) {
				result.contacts.erase(result.contacts.begin() + j);
				break;
			}
		}
	}

	result.colliding = true;
	result.normal = axis;
	//Log::Info("Collision is found");
	return result;
}

void BodyCollision::ApplyImpulse2D(PhysicBody* a, PhysicBody * b, Manifold& m, float dt) {

	float kNormal = a->inverseMass + b->inverseMass;
	auto normalMass = 1 / kNormal;
	Contacts* c;
	Arbiter* arbiter;
	for (int i = 0; i < m.contacts.size(); ++i) {
		FPoint contact = m.contacts[i];
		auto r1 = contact - a->_pos;
		auto r2 = contact - b->_pos;

		//relativeVelocity at contact 
		FPoint dv = b->velocity - a->velocity;

		//normal impulse
		float vn = m.normal.GetDotProduct(dv);
		float dPn = normalMass * (-vn + c->bias);
		
		if (c->accumulateImpulses) {
			// clamp the accumulated impulse
			float Pn0 = c->Pn;
			c->Pn = fmaxf(Pn0 + dPn, 0.0f);
			dPn = c->Pn - Pn0;
		}
		else {
			dPn = fmaxf(dPn, 0.0f);
		}

		//apply contact impulse
		FPoint Pn = dPn * m.normal;
		
		a->velocity -= a->inverseMass * Pn;
		b->velocity += b->inverseMass * Pn;
	}

}

float BodyCollision::normalMass(PhysicBody* a, PhysicBody* b) {

}