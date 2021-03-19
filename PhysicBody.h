#pragma once
#include "Body.h"
#include "Arbiter.h"

class PhysicBody : public Body 
{
public:
	PhysicBody(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction);
	static PhysicBody* Create(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction = 0.6);
	void Update(float dt);


public: 
	bool _anchored;
	float mass, inverseMass, elastic, friction;
	bool MouseDown(const IPoint& mouse_pos);
	bool MouseUp(const IPoint& mouse_pos);
	FPoint WHATIMPULSE;
	void UpdatePosition(float dt);
	//ARBITER
	Contacts contact;
	
	FPoint _forces;
	void ApplyForces();
private:
	void AddLinearImpulse(const FPoint& impulse);
	bool _isAwake;
	FPoint _motion;
	float _rwaMotion;
	float _sleepEpsilon;
	bool _canSleep;
public:
	FPoint lastPosBeforeSleep;
	std::vector<PhysicBody*> haveCollisionsWithVector;
};