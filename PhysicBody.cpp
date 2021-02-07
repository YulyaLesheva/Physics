#include "stdafx.h"
#include "PhysicBody.h"

#define GRAVITY_CONST FPoint(0, -9.82f)

PhysicBody::PhysicBody(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction)
	:Body(tex, pos),
	mass(mass),
	inverseMass(1/mass),
	velocity(0,0),
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

	FPoint force = _forces;
	FPoint acceleration = force * inverseMass;
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

void PhysicBody::KeepInBorders()
{
	auto rect = GetRect();

	if (rect.LeftBottom().x <= 0) {
		_pos.x = _tex->Width() * 0.5;
		//_pos.x = _pos.x + 1;
		ReverseCurrentVectorX();
	}

	if (rect.RightBottom().x >= Render::device.Width()) {
		_pos.x = Render::device.Width() - _tex->Width() * 0.5;
		//_pos.x = _pos.x - 1;
		ReverseCurrentVectorX();
	}

	if (rect.LeftTop().y >= Render::device.Height()) {
		_pos.y = Render::device.Height() - _tex->Height() * 0.5;
		//_pos.y = _pos.y - 1;
		ReverseCurrentVectorY();
	}

	if (rect.LeftBottom().y <= 0) {
		_pos.y = _tex->Height() * 0.5;
		//_pos.y = _pos.y + 1;
		ReverseCurrentVectorY();
	}
}

bool PhysicBody::OnBorder()
{
	auto rect = GetRect();

	if (rect.LeftBottom().x < 0 || rect.RightBottom().x > Render::device.Width()) {
		return true;
	}

	if (rect.LeftTop().y > Render::device.Height() || rect.LeftBottom().y < 0) {
		return true;
	}
	return false;
}

void PhysicBody::ApplyGravity()
{
	velocity = GRAVITY_CONST * mass;
}

void PhysicBody::AddLinearImpulse(const FPoint & impulse)
{
	velocity += impulse;
}


void PhysicBody::ReverseCurrentVectorY() {
	velocity.y = velocity.y * -1;
}

void PhysicBody::ReverseCurrentVectorX() {
	velocity.x = velocity.x * -1;
}
