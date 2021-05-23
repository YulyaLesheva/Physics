#include "stdafx.h"
#include "Arbiter.h"
#include "BodyBox.h"
#include "Math.h"
#include "Collide.h"

Arbiter::Arbiter(BodyBox* BodyA, BodyBox* BodyB):
	separation(FLT_MAX)
{
	if (BodyA < BodyB) {
		a = BodyA;
		b = BodyB;
	} 
	else {
		a = BodyB;
		b = BodyA;
	}
	
	//добавить оператор сравнения для двух тел
	numContacts = CollideBox(contactsArray, a, b);
	friction = sqrtf(a->friction * b->friction);
}
void Arbiter::Update(Contact* newContacts, int numNewContacts) {
	
	Contact mergedContacts[2];

	for (int i = 0; i < numNewContacts; ++i){
		Contact* cNew = newContacts + i;
		int k = -1;
		for (int j = 0; j < numContacts; ++j){
			Contact* cOld = contactsArray + j;
			if (cNew->feature.value == cOld->feature.value)
			{
				k = j;
				break;
			}
		}

		if (k > -1){
			Contact* c = mergedContacts + i;
			Contact* cOld = contactsArray + k;
			*c = *cNew;
			if (true){
				c->Pn = cOld->Pn;
				c->Pt = cOld->Pt;
				c->Pnb = cOld->Pnb;
			}
			else{
				c->Pn = 0.0f;
				c->Pt = 0.0f;
				c->Pnb = 0.0f;
			}
		}
		else{
			mergedContacts[i] = newContacts[i];
		}
	}
	for (int i = 0; i < numNewContacts; ++i)
		contactsArray[i] = mergedContacts[i];

	numContacts = numNewContacts;
}

void Arbiter::ApplyImpulse2D() {

	BodyBox* b1 = a;
	BodyBox* b2 = b;
	Math m;
	for (int i = 0; i < numContacts; ++i)
	{
		Contact* c = contactsArray+ i;
		c->r1 = c->position - b1->position;
		c->r2 = c->position - b2->position;

		// Relative velocity at contact
		auto dv = b2->velocity + m.Cross(b2->angularVelocity, c->r2)
			- b1->velocity - m.Cross(b1->angularVelocity, c->r1);

		// Compute normal impulse
		float vn = m.Dot(dv, c->contactNormal);

		float dPn = c->massNormal * (-vn + c->bias);

		if (true)
		{
			// Clamp the accumulated impulse
			float Pn0 = c->Pn;
			c->Pn = math::max(Pn0 + dPn, 0.0f);
			dPn = c->Pn - Pn0;
		}
		else
		{
			dPn = math::max(dPn, 0.0f);
		}

		// Apply contact impulse
		//тут начинается ошибка, почему в PN
		auto Pn = dPn * c->contactNormal;

		b1->velocity -= b1->inverseMass * Pn;
		b1->angularVelocity -= b1->invI * m.Cross(c->r1, Pn);

		b2->velocity += b2->inverseMass * Pn;
		b2->angularVelocity += b2->invI * m.Cross(c->r2, Pn);

		// Relative velocity at contact
		dv = b2->velocity + m.Cross(b2->angularVelocity, c->r2)
			- b1->velocity - m.Cross(b1->angularVelocity, c->r1);

		auto tangent = m.Cross(c->contactNormal, 1.0f);
		float vt = m.Dot(dv, tangent);
		float dPt = c->massTangent * (-vt);

		if (true)
		{
			// Compute friction impulse
			float maxPt = friction * c->Pn;

			// Clamp friction
			float oldTangentImpulse = c->Pt;
			c->Pt = m.Clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
			dPt = c->Pt - oldTangentImpulse;
		}
		else
		{
			float maxPt = friction * dPn;
			dPt = m.Clamp(dPt, -maxPt, maxPt);
		}

		// Apply contact impulse
		auto Pt = dPt * tangent;

		b1->velocity -= b1->inverseMass * Pt;
		b1->angularVelocity -= b1->invI * m.Cross(c->r1, Pt);

		b2->velocity += b2->inverseMass * Pt;
		b2->angularVelocity += b2->invI * m.Cross(c->r2, Pt);
	}
}

void Arbiter::PreStep(float inv_dt) {
	Math m;
	const float k_allowedPenetration = 0.01f;
	float k_biasFactor = true ? 0.2f : 0.0f;

	for (int i = 0; i < numContacts; ++i)
	{
		Contact* c = contactsArray + i;

		auto r1 = c->position - a->position;
		auto r2 = c->position - b->position;

		// Precompute normal mass, tangent mass, and bias.
		float rn1 = m.Dot(r1, c->contactNormal);
		float rn2 = m.Dot(r2, c->contactNormal);
		float kNormal = a->inverseMass + b->inverseMass;
		kNormal += a->invI * (m.Dot(r1, r1) - rn1 * rn1) +
			b->invI * (m.Dot(r2, r2) - rn2 * rn2);
		c->massNormal = 1.0f / kNormal;

		auto tangent = m.Cross(c->contactNormal, 1.0f);
		float rt1 = m.Dot(r1, tangent);
		float rt2 = m.Dot(r2, tangent);
		float kTangent = a->inverseMass + b->inverseMass;

		kTangent += a->invI * (m.Dot(r1, r1) - rt1 * rt1)
			+ b->invI * (m.Dot(r2, r2) - rt2 * rt2);
		c->massTangent = 1.0f / kTangent;

		c->bias = -k_biasFactor * inv_dt *
			math::min(0.0f, c->depth + k_allowedPenetration);

		if (true)
		{
			// Apply normal + friction impulse//тут ошибка. почему PN
			auto P = c->Pn * c->contactNormal + c->Pt * tangent;

			a->velocity -= a->inverseMass * P;
			a->angularVelocity -= a->invI * m.Cross(r1, P);

			b->velocity += b->inverseMass * P;
			b->angularVelocity += b->invI * m.Cross(r2, P);
		}
	}
}
