#pragma once
#include "Body.h"

class PhysicBody : public Body 
{
public:
	PhysicBody(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction);
	static PhysicBody* Create(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction = 0.5);
	void Update(float dt);


public: 
	bool _anchored;
	float mass, inverseMass, elastic, friction;
	bool MouseDown(const IPoint& mouse_pos);
	bool MouseUp(const IPoint& mouse_pos);

	
	FPoint _forces;

	void SetCanSleep(const bool sleep);
	void SetAwake(const bool awake);
	bool IsAwake();
	bool CanSleep();
	std::vector<bool> CollisionState;
	bool _isCollided;

private:
	void ApplyGravity();
	void AddLinearImpulse(const FPoint& impulse);
	bool _isAwake;
	FPoint _motion;
	float _rwaMotion;
	float _sleepEpsilon;
	bool _canSleep;



};