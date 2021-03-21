#include "stdafx.h"
#include "Arbiter.h"
#include "BodyBox.h"

Arbiter::Arbiter(BodyBox* BodyA, BodyBox* BodyB) {
	
	a = BodyA;
	b = BodyB;

	friction = sqrtf(a->friction * b->friction);
}

void Arbiter::ApplyImpulse2D() {


}

void Arbiter::Update(float dt) {

}