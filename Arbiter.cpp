#include "stdafx.h"
#include "Arbiter.h"
#include "BodyBox.h"

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

	for (int i = 0; i < contacts.size(); ++i) {
		//r1 = 
		//������� ����� ��������, ����������� �������� ����� ��� �������(�. �)

	
	}
}

void Arbiter::Update(float dt) {

}