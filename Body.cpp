#include "stdafx.h"
#include "Body.h"
#include "BodyCollision.h"

#define GRAVITY_CONST FPoint(0, -9.82f)

Body::Body(Render::Texture* tex) :
	_tex(tex),
	velocity(0, 0)

{
}

Body::Body(Render::Texture* tex, FPoint& pos) :
	_tex(tex),
	_pos(pos),
	velocity(0,0)

{
}



Body::Body(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction) :
	_tex(tex),
	_pos(pos),
	mass(mass),
	inverseMass(1 / mass),
	velocity(0, 0),
	elastic(elastic),
	friction(friction),
	_anchored(false)
{
	if (mass == 0) inverseMass = 0;
	
	sleepEpsilon = 0.9f;
	canSleep = false;
	SetAwake(true);
}

Body::~Body() {
	delete this;
}

Body* Body::Create(Render::Texture* tex) {
	return new Body(tex);
}

Body* Body::Create(Render::Texture * tex, FPoint & pos){
	return new Body(tex, pos);
}


Body* Body::Create(Render::Texture * tex, FPoint & pos, float mass, float elastic, float friction) {
	return new Body(tex, pos, mass, elastic, friction);
}

void Body::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(_pos);
	Render::device.MatrixTranslate(_tex->_bitmap_width * -0.5f, _tex->_bitmap_height * -0.5f, 0);
	_tex->Draw();
	Render::device.PopMatrix();
}

void Body::Update(float dt) {

	if (_anchored) {
		velocity = FPoint(0,0);
		IPoint mouse_position = Core::mainInput.GetMousePos();
		_pos = mouse_position;
	}

	if (!isAwake) return;

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
	rwaMotion = bias * rwaMotion + (1 - bias) * motion;

	if (rwaMotion > 50.f) rwaMotion = 5.0f;

	if (rwaMotion < sleepEpsilon) {
		SetAwake(false);
	}

	else if (rwaMotion > 10 * sleepEpsilon) {
		rwaMotion = 10 * sleepEpsilon;
		SetAwake(true);
	}
}

void Body::ApplyGravity() {
	velocity = GRAVITY_CONST * mass;
}

void Body::AddLinearImpulse(const FPoint& impulse) {
	velocity += impulse;
}

IRect& Body::GetRect() {
	_rect = IRect(IPoint(_pos.x - _tex->Width()*.5, _pos.y - _tex->Height()*.5), _tex->Width(), _tex->Height());

	return _rect;
}

FPoint& Body::GetPos() {
	return _pos;
}
Render::Texture* Body::GetTex() {
	return _tex;
}

bool Body::MouseDown(const IPoint& mouse_pos) {
	
	if (GetRect().Contains(mouse_pos)) {
		_anchored = true;
	SetAwake(false);
		return true;
	}
	else {
		return false;
	}
}

bool Body::MouseUp(const IPoint& mouse_pos) {
	_anchored = false;
	SetAwake(true);
	return false;
}

bool Body::OnBorder() {
	auto rect = GetRect();

	if (rect.LeftBottom().x < 0 || rect.RightBottom().x > Render::device.Width()) {
		return true;
	}

	if (rect.LeftTop().y > Render::device.Height() || rect.LeftBottom().y < 0) {
		return true;
	}
	return false;
}

void Body::KeepInBorders() {
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

void Body::ReverseCurrentVectorY() {
	velocity.y = velocity.y * -1;
}

void Body::ReverseCurrentVectorX() {
	velocity.x = velocity.x * -1;
}

FPoint Body::GetMin() {
	
	FPoint resultMin;

	auto rect = GetRect();
	auto minX = rect.LeftBottom().x;
	auto minY = rect.LeftBottom().y;

	resultMin = FPoint(minX, minY);

	return resultMin;
}

FPoint Body::GetMax() {

	FPoint resultMax;
	
	auto rect = GetRect();
	auto maxX = rect.RightBottom().x;
	auto maxY = rect.RightTop().y;

	resultMax = FPoint(maxX, maxY);
	return resultMax;
}

void Body::SetAwake(const bool awake) {
	if (awake) {
		isAwake = true;
		rwaMotion = sleepEpsilon * 2.0f;

	}
	else {
		isAwake = false;
		velocity = FPoint(0,0);
	}
}

void Body::SetCanSleep(const bool sleep) {
	if (sleep) {
		canSleep = true;
	}
	else {
		canSleep = false;
	}
}