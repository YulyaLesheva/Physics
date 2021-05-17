#pragma once

class BodyBox;
// arbiter похож на manifold. Думаю, что им можно заменить manifold
// имеет функции update и функцию applyImpulse;

union FeaturePair
{
	struct Edges
	{
		char inEdge1;
		char outEdge1;
		char inEdge2;
		char outEdge2;
	} e;
	int value;

};


struct Contact
{
	Contact() : Pn(0.0f), Pt(0.0f), Pnb(0.0f), depth(FLT_MAX)
	{
	}


	FPoint position;
	float Pn;	// accumulated normal impulse
	float Pt;	// accumulated tangent impulse
	float Pnb;	// accumulated normal impulse for position bias
	float massNormal, massTangent;
	FPoint r1, r2;
	float bias;
	float depth;
	FPoint contactNormal;
	FeaturePair feature;
};


struct ArbiterKey
{
	ArbiterKey(BodyBox* BodyA, BodyBox* BodyB)
	{
		if (BodyA < BodyB) {
			a = BodyA;
			b = BodyB;
		}
		else {
			a = BodyB;
			b = BodyA;
		}
	}
	BodyBox* a;
	BodyBox* b;
};


struct Arbiter
{

	enum { MAX_POINTS = 2 };

	Arbiter(BodyBox* bodyA, BodyBox* bodyB);

	void Update(Contact* contacts, int numContacts);

	void PreStep(float inv_dt);
	void ApplyImpulse2D();
	Contact contactsArray[MAX_POINTS];

	int numContacts;

	BodyBox* a;
	BodyBox* b;

	// Combined friction
	float friction;
	//collision features
	FPoint normal;
	float separation;

};
int CollideNEW(Contact* contacts, BodyBox* a, BodyBox* b);

inline bool operator < (const ArbiterKey& a1, const ArbiterKey& a2)
{
	if (a1.a < a2.a)
		return true;

	if (a1.a == a2.a && a1.b < a2.b)
		return true;

	return false;
}

