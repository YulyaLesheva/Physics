#include "stdafx.h"
#include "Arbiter.h"
#include "BodyBox.h"
#include "Math.h"
#include "Collide.h"
Arbiter::Arbiter(BodyBox* BodyA, BodyBox* BodyB):
	separation(FLT_MAX),
	colliding(false)
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
	numContacts = Collide(allContacts, a, b);
	friction = sqrtf(a->friction * b->friction);
}

void Arbiter::ApplyImpulse2D() {

	BodyBox* body_a = a;
	BodyBox* body_b = b;

	Math m;
	
	Log::Info("START IMPULSE APPLY");

	for (int i = 0; i < numContacts; ++i) {
		Contact* c = &allContacts[i];
		//сделать класс контактс, конструктор которого будет как контакт(х. у)
		c->r1 = c->position - body_a->position;
		c->r2 = c->position - body_b->position;
	
		//relative velocity at contact
		FPoint dv = body_b->velocity + m.Cross(body_b->angularVelocity, c->r2)
			- body_a->velocity - m.Cross(body_a->angularVelocity, c->r1);
		
		//проверить почему дает 0 при коллизии. 


		//compute normal impulse
		float vn = m.Dot(dv, c->contactNormal);
		
		if (vn >= 0) {
			return;
		} 
		
		float dPn = c->massNormal * (-vn + c->bias);
		
		if (true) {// if (World::accumulateImpulses)
			float Pn0 = c->Pn;
			c->Pn = math::max(Pn0 + dPn, 0.f);
			dPn = c->Pn - Pn0;
		}
		else
		{
			dPn = math::max(dPn, 0.0f);
		}

		//apply contact impulse
		FPoint Pn = dPn * c->contactNormal;
	
		body_a->velocity -= body_a->inverseMass * Pn;
		body_a->angularVelocity -= body_a->invI * m.Cross(c->r1, Pn);

		body_b->velocity += body_b->inverseMass * Pn;
		body_b->angularVelocity += body_b->invI * m.Cross(c->r2, Pn);

		//relative velocity at contact
		dv = body_b->velocity + m.Cross(body_b->angularVelocity, c->r2)
			- body_a->velocity - m.Cross(body_a->angularVelocity, c->r1);

		FPoint tangent = m.Cross(c->contactNormal, 1.0f);
		float vt = m.Dot(dv, tangent);
		float dPt = c->massTangent * (-vt);

		if (true) { //if (World::accumulateImpulses)

			//compute friction impulse 
			float maxPt = friction * c->Pn;
			
			//clamp friction 
			float oldTangentImpulse = c->Pt;
			c->Pt = m.Clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
			dPt = c->Pt - oldTangentImpulse;
		}
		else {
			float maxPt = friction * dPn;
			dPt = m.Clamp(dPt, -maxPt, maxPt);
		}
		//apply impulse

		FPoint  Pt = dPt * tangent;

		body_a->velocity -= body_a->inverseMass * Pt;
		body_a->angularVelocity -= body_a->invI * m.Cross(c->r1, Pt);

		body_b->velocity += body_b->inverseMass * Pt;
		body_b->angularVelocity += body_b->invI * m.Cross(c->r2, Pt);
	}
}

void Arbiter::PreStep(float inv_dt) {
	
	const float k_allowedPenetration = 0.01f;
	Math m;

	float k_biasFactor = true ? 0.2f : 0.0f;
	//float k_biasFactor = 0.2f;
	for (int i = 0; i < numContacts; ++i) {
		Contact* c = &allContacts[i];
		FPoint r1 = c->position - a->position;
		FPoint r2 = c->position - b->position;

		//precompute normal mass, tangent mass, bias
		float rn1 = m.Dot(r1, c->contactNormal);
		float rn2 = m.Dot(r2, c->contactNormal);
		float kNormal = a->inverseMass + b->inverseMass;
		kNormal += a->invI * (m.Dot(r1, r1) - rn1 * rn1) +
			b->invI * (m.Dot(r2, r2) - rn2 * rn2);
		c->massNormal = 1 / kNormal;

		FPoint tangent = m.Cross(c->contactNormal, 1.0);
		float rt1 = m.Dot(r1, tangent);
		float rt2 = m.Dot(r2, tangent);
		float kTangent = a->inverseMass + b->inverseMass;
		kTangent += a->invI * (m.Dot(r1, r1) - rt1 * rt1) +
			b->invI * (m.Dot(r2, r2) - rt2 * rt2);
		c->massTangent = 1.0f / kTangent;

		c->bias = -k_biasFactor * inv_dt
			* math::min(0.0f, c->depth + k_allowedPenetration);

		if (true) { //if (World::accumulateImpulses)
			// Apply normal and friction impulse
			FPoint P = c->Pn * c->contactNormal + c->Pt * tangent;

			a->velocity -= a->inverseMass * P;
			a->angularVelocity -= a->invI * m.Cross(r1, P);

			b->velocity += b->inverseMass * P;
			b->angularVelocity += b->invI * m.Cross(r2, P);
		}
	}
}

void Arbiter::ResolveCollision() {

	BodyBox* body_a = a;
	BodyBox* body_b = b;

	Math m;
	//мб потому чтоо это удаляется? 

	auto c = allContacts[0];

	auto invMassSum = body_a->inverseMass + body_b->inverseMass;

	FPoint relativeVel = body_b->velocity - body_a->velocity;
	FPoint relativeNorm = c.contactNormal;
	relativeNorm.Normalize();

	float e = fminf(body_a->elastic, body_b->elastic);
	float numerator = (-(1 + e) * m.Dot(relativeVel, relativeNorm));
	float j = (invMassSum == 0.f) ? 0.0f : numerator / invMassSum;

	if (allContacts.size() > 0.0f && j != 0.0f) {
		j /= 0.8f;
	}

	FPoint impulse = relativeNorm * j;
	
	body_a->velocity -= impulse * body_a->inverseMass;
	body_b->velocity += impulse * body_b->inverseMass;

}