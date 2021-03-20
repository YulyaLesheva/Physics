#include "stdafx.h"
#include "Arbiter.h"
#include "PhysicBody.h"
#include "BodyCollision.h"

Arbiter::Arbiter(PhysicBody* BodyA, PhysicBody* BodyB) {
	
	a = BodyA;
	b = BodyB;

	friction = sqrtf(a->friction * b->friction);
}

void Arbiter::ApplyImpulse2D() {

}

void Arbiter::Update(float dt) {

}