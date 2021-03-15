#include "stdafx.h"
#include "PhysicBody.h"

#define GRAVITY_CONST FPoint(0, -9.82f)

PhysicBody::PhysicBody(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction)
	:Body(tex, pos),
	mass(mass),
	inverseMass(1 / mass),
	elastic(elastic),
	friction(friction),
	_anchored(false)
{

	if (mass <= 0) inverseMass = 0;
	else {
		inverseMass = 1 / mass;
	}

}

PhysicBody* PhysicBody::Create(Render::Texture * tex, FPoint & pos, float mass, float elastic, float friction)
{
	return new PhysicBody(tex, pos, mass, elastic, friction);
}

void PhysicBody::Update(float dt)
{
	if (_anchored) {
		IPoint mouse_position = Core::mainInput.GetMousePos();
		_pos = mouse_position;
	}

	/*float dampingFactor = 1.0 - 0.95;
	float frameDamping = powf(dampingFactor, dt);

	FPoint acceleration = _forces * inverseMass;
	acceleration += GRAVITY_CONST * mass;
	velocity += acceleration * dt;
	velocity *= frameDamping;*/

	const float damping = 0.98f;

	FPoint acceleration = _forces * inverseMass;
	velocity = velocity + acceleration * dt;
	velocity = velocity * damping;

	
	if (fabsf(velocity.x) < 0.001f) {
		velocity.x = 0.0f;
	}
	if (fabsf(velocity.y) < 0.001f) {
		velocity.y = 0.0f;
	}

	_pos += velocity;
}

bool PhysicBody::MouseDown(const IPoint & mouse_pos)
{
	if (GetRect().Contains(mouse_pos)) {
		_anchored = true;
		return true;
	}
	else {
		return false;
	}
}

bool PhysicBody::MouseUp(const IPoint & mouse_pos)
{
	_anchored = false;
	return false;
}

void PhysicBody::ApplyForces()
{
	_forces = GRAVITY_CONST * mass;
}

void PhysicBody::AddLinearImpulse(const FPoint & impulse)
{
	velocity += impulse;
}



