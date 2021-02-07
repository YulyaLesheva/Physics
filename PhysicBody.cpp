#include "stdafx.h"
#include "PhysicBody.h"

#define GRAVITY_CONST FPoint(0, -9.82f)

PhysicBody::PhysicBody(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction)
	:Body(tex, pos),
	mass(mass),
	inverseMass(1/mass),
	elastic(elastic), 
	friction(friction),
	_anchored(false)
	
{
	if (mass == 0) inverseMass = 0;

	_sleepEpsilon = 0.9f;
	_canSleep = false;
	SetAwake(true);

}

PhysicBody* PhysicBody::Create(Render::Texture * tex, FPoint & pos, float mass, float elastic, float friction)
{
	return new PhysicBody(tex, pos, mass, elastic, friction);
}

void PhysicBody::Update(float dt)
{
	if (_anchored) {
		velocity = FPoint(0, 0);
		IPoint mouse_position = Core::mainInput.GetMousePos();
		_pos = mouse_position;
	}

	if (!_isAwake) return;

	float dampingFactor = 1.0 - 0.95;
	float frameDamping = powf(dampingFactor, dt);

	FPoint acceleration = _forces * inverseMass;
	acceleration += GRAVITY_CONST * mass;
	velocity += acceleration * dt;
	velocity *= frameDamping;
	_pos += velocity;

	/*const float mDamping = 0.98f;
	FPoint mAcceleration = _forces * inverseMass;
	velocity += mAcceleration;
	velocity *= mDamping;
	_pos += velocity;*/

	float motion = velocity.GetDotProduct(velocity);
	float bias = 0.98f;
	_rwaMotion = bias * _rwaMotion + (1 - bias) * motion;

	if (_rwaMotion > 50.f) _rwaMotion = 5.0f;

	if (_rwaMotion < _sleepEpsilon) {
		SetAwake(false);
	}

	else if (_rwaMotion > 10 * _sleepEpsilon) {
		_rwaMotion = 10 * _sleepEpsilon;
		SetAwake(true);
	}
}

bool PhysicBody::MouseDown(const IPoint & mouse_pos)
{ 
	if (GetRect().Contains(mouse_pos)) {
		_anchored = true;
		SetAwake(false);
		return true;
	}
	else {
		return false;
	}
}

bool PhysicBody::MouseUp(const IPoint & mouse_pos)
{
	_anchored = false;
	SetAwake(true);
	return false;
}

void PhysicBody::SetCanSleep(const bool sleep)
{
	if (sleep) {
		_canSleep = true;
	}
	else {
		_canSleep = false;
	}
}

void PhysicBody::SetAwake(const bool awake)
{
	if (awake) {
		_isAwake = true;
		_rwaMotion = _sleepEpsilon * 2.0f;

	}
	else {
		_isAwake = false;
		velocity = FPoint(0, 0);
	}
}

void PhysicBody::ApplyGravity()
{
	_forces = GRAVITY_CONST * mass;
}

void PhysicBody::AddLinearImpulse(const FPoint & impulse)
{
	velocity += impulse;
}

bool PhysicBody::GetAwakeStatus() {
	return _isAwake;
}