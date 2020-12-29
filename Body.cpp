#include "stdafx.h"
#include "Body.h"

float GRAVITY = -9.82;
//#define GRAVITY_CONST float (-9.82f);

Body::Body(Render::Texture* tex) :
	_tex(tex),
	velocity(0, 0),
	_normal(math::Vector3(0, 0, 0)),
	_gravity(0, 0)

{
}

Body::Body(Render::Texture* tex, FPoint& pos) :
	_tex(tex),
	_pos(pos),
	velocity(0,0),
	_normal(math::Vector3(0, 0, 0)),
	_gravity(0, 0)

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
	_normal(math::Vector3(0, 0, 0)),
	_anchored(false),
	_gravity(0, 0),
	penetrationDepth(0)

{
	if (mass == 0) inverseMass = 0;
	
	_gravity.y =  GRAVITY * mass;
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
	// by direction
	//if (mooveble) {
	//	velocity.x += 0.04;
	//	velocity.y += 0.01;
	//	
	//	/*moving.x += 0.04;
	//	moving.y += 0.01;*/
	//}
	//by mouse
	//by mouse

	/*if (mooveble) {
		IPoint mouse_position = Core::mainInput.GetMousePos();
		_pos = mouse_position;
		velocity = IPoint(0.2*( _lastPos.x - _pos.x), 0.2*(_lastPos.y - _pos.y));
		_lastPos = _pos;
	}*/

	if (_anchored) {
		_gravity.y = 0;
		IPoint mouse_position = Core::mainInput.GetMousePos();
		_pos = mouse_position;
	}

	FPoint forces = _gravity;
	auto accleration = forces * inverseMass;
	const float damping = 0.98;
	
	velocity += accleration;
	velocity *= damping;
	_pos += velocity;
	

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

math::Vector3& Body::GetNormal() {
	return _normal;
}

bool Body::MouseDown(const IPoint& mouse_pos) {
	
	if (GetRect().Contains(mouse_pos)) {
		//Log::Info("CONTAINS CONTAINS");
		_anchored = true;
		return true;
	}
	else {
//		Log::Info("......");
		return false;

	}
}

bool Body::MouseUp(const IPoint& mouse_pos) {
	_anchored = false;
	_gravity.y = GRAVITY;
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
