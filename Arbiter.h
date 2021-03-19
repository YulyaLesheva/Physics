#pragma once

class PhysicBody;
// arbiter похож на manifold. Думаю, что им можно заменить manifold
// имеет функции update и функцию applyImpulse;
struct Contacts
{
	Contacts() : Pn(0.0f), Pt(0.0f), Pnb(0.0f) {}
	FPoint position;
	FPoint normal;
	FPoint r1, r2;
	float separation;
	float Pn;	// accumulated normal impulse
	float Pt;	// accumulated tangent impulse
	float Pnb;	// accumulated normal impulse for position bias
	float massNormal, massTangent;
	float bias;

	//mine
	bool accumulateImpulses = true;
};

struct Arbiter
{
	enum { MAX_POINTS = 2 };
	Arbiter(PhysicBody* a, PhysicBody* b);
	void Update(float dt);
	float friction;
	PhysicBody* a;
	PhysicBody* b;
	Contacts contacts[MAX_POINTS];
};