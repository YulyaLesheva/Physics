#include "stdafx.h"
#include "Arbiter.h"
#include "BodyBox.h"
#include "Math.h"

Arbiter::Arbiter(BodyBox* BodyA, BodyBox* BodyB):
	separation(FLT_MAX) 
{
	
	a = BodyA;
	b = BodyB;

	friction = sqrtf(a->friction * b->friction);
}

void Arbiter::ApplyImpulse2D() {

	BodyBox* body_a = a;
	BodyBox* body_b = b;

	Math m;

	for (int i = 0; i < contacts.size(); ++i) {
		 
		//сделать класс контактс, конструктор которого будет как контакт(х. у)
		r1 = contacts[i] - body_a->position;
		r2 = contacts[i] - body_b->position;
	
		FPoint dv = body_b->velocity + m.Cross(body_b->angularVelocity, r2);

		//normal impulse

		float vn = normal.GetDotProduct(dv);
		float dPn = massNormal * (-vn + bias);
		
		if (true) {
			//clamp accumulated impulse
			//float Pn0 = Pn
			
			//FPoint Pn = dPn * normal;
			
		}

	}
}

void Arbiter::Update(float dt) {

}