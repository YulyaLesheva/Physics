#pragma once

class BodyBox;
// arbiter похож на manifold. Думаю, что им можно заменить manifold
// имеет функции update и функцию applyImpulse;


struct Contact
{
	Contact(FPoint point) : Pn(0.0f), Pt(0.0f), Pnb(0.0f)
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
};

struct Arbiter
{

	Arbiter(BodyBox* bodyA, BodyBox* bodyB);

	void Update(float dt);

	void ApplyImpulse2D();
	
	std::vector<FPoint> contacts;
	std::vector<Contact> contactsNEW;
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