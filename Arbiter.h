#pragma once

class BodyBox;
// arbiter ����� �� manifold. �����, ��� �� ����� �������� manifold
// ����� ������� update � ������� applyImpulse;

union FeaturePair
{
	struct Edges
	{
		char inEdge1;
		char outEdge1;
		char inEdge2;
		char outEdge2;
	} 
	e;
	int value;
};

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
	FeaturePair feature;
};

struct Arbiter
{
	enum { MAX_POINTS = 2 };

	Arbiter(BodyBox* bodyA, BodyBox* bodyB);

	void Update(float dt);

	void ApplyImpulse2D();

	Contacts contacts[MAX_POINTS];
	int numContacts;

	BodyBox* a;
	BodyBox* b;

	// Combined friction
	float friction;
};