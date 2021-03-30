#pragma once

class BodyBox;
// arbiter ����� �� manifold. �����, ��� �� ����� �������� manifold
// ����� ������� update � ������� applyImpulse;


struct Contact
{
	Contact() : Pn(0.0f), Pt(0.0f), Pnb(0.0f){}

	FPoint position;
	float Pn;	// accumulated normal impulse
	float Pt;	// accumulated tangent impulse
	float Pnb;	// accumulated normal impulse for position bias
};

struct Arbiter
{

	Arbiter(BodyBox* bodyA, BodyBox* bodyB);

	void Update(float dt);

	void ApplyImpulse2D();
	
	std::vector<FPoint> contacts;
	int numContacts;

	BodyBox* a;
	BodyBox* b;

	// Combined friction
	float friction;

	//collision features
	FPoint normal;
	FPoint r1, r2;
	float separation;
	float massNormal, massTangent;
	float bias;
};