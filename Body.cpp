#include "stdafx.h"
#include "Body.h"

Body::Body(Render::Texture* tex) :
	_tex(tex),
	velocity(0, 0),
	_normal(math::Vector3(0, 0, 0)),
	_gravity(-0.7)

{
}

Body::Body(Render::Texture* tex, FPoint& pos) :
	_tex(tex),
	_pos(pos),
	velocity(0,0),
	_normal(math::Vector3(0, 0, 0)),
	_gravity(-0.7)

{
}



Body::Body(Render::Texture* tex, FPoint& pos, float mass, float elastic, bool moveState) :
	_tex(tex),
	_pos(pos),
	mass(mass),
	inverseMass(1 / mass),
	velocity(0, 0),
	elastic(elastic),
	_normal(math::Vector3(0, 0, 0)),
	_anchored(false),
	_gravity(-0.7)

{
	if (mass == 0) inverseMass = 0;
	if (moveState == true) velocity = FPoint(10.0, 3.0);
}


Body* Body::Create(Render::Texture* tex) {
	return new Body(tex);
}

Body* Body::Create(Render::Texture * tex, FPoint & pos){
	return new Body(tex, pos);
}


Body* Body::Create(Render::Texture * tex, FPoint & pos, float mass, float elastic, bool moveState) {
	return new Body(tex, pos, mass, elastic, moveState);
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

	/*if (mooveble) {
		IPoint mouse_position = Core::mainInput.GetMousePos();
		_pos = mouse_position;
		velocity = IPoint(0.2*( _lastPos.x - _pos.x), 0.2*(_lastPos.y - _pos.y));
		_lastPos = _pos;
	}*/

	if (_anchored) {
		IPoint mouse_position = Core::mainInput.GetMousePos();
		_pos = mouse_position;
	}
	_pos += velocity;
	_pos.y += _gravity;
	//if(mooveble) velocity.y -= 0.3;
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

math::Vector3 Body::GetNormal() {
	return _normal;
}

bool Body::MouseDown(const IPoint& mouse_pos) {
	
	if (GetRect().Contains(mouse_pos)) {
		Log::Info("CONTAINS CONTAINS");
		_anchored = true;
		return true;
	}
	else {
		Log::Info("......");
		return false;

	}
}

bool Body::MouseUp(const IPoint& mouse_pos) {
	_anchored = false;
	return false;
}