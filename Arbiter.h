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

	void PreStep(float inv_dt);
	void ApplyImpulse2D();
	void ResolveCollision();


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

};