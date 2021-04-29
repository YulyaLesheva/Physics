#pragma once

class BodyBox;
// arbiter ����� �� manifold. �����, ��� �� ����� �������� manifold
// ����� ������� update � ������� applyImpulse;


struct Contact
{
	Contact(FPoint point) : Pn(0.0f), Pt(0.0f), Pnb(0.0f), depth(FLT_MAX)
	{
		position = point;
	}

	Contact() = default;
	FPoint position;
	float Pn;	// accumulated normal impulse
	float Pt;	// accumulated tangent impulse
	float Pnb;	// accumulated normal impulse for position bias
	float massNormal, massTangent;
	FPoint r1, r2;
	float bias;
	float depth;
	FPoint contactNormal;
};

struct Arbiter
{

	Arbiter(BodyBox* bodyA, BodyBox* bodyB);

	void Update(float dt);

	void ApplyImpulse2D();
	
	std::vector<FPoint> contacts;
	std::vector<Contact> contactsNEW;
	std::vector<Contact> allContacts;
	int numContacts;

	BodyBox* a;
	BodyBox* b;

	bool colliding;
	// Combined friction
	float friction;
	//collision features
	FPoint normal;
	float separation;

	Arbiter* arb = NULL;

	void Reset(Arbiter* result) {
		if (result != 0) {
			result->colliding = false;
			result->normal = FPoint(0, 0);
			result->separation = FLT_MAX;
			result->contactsNEW.clear();
			result->arb = NULL;
		}
	}

};