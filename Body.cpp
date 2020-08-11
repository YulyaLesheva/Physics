#include "stdafx.h"
#include "Body.h"

Body::Body(Render::Texture* tex):
_tex(tex),
velocity(0, 0),
_normal(math::Vector3(0,0,0))

{
}

Body::Body(Render::Texture* tex, IPoint& pos) :
	_tex(tex),
	_pos(pos),
	velocity(0,0),
	_normal(math::Vector3(0, 0, 0))
{
}

Body::Body(Render::Texture* tex, IPoint& pos, float mass):
	_tex(tex),
	_pos(pos),
	mass(mass),
	inverseMass(1/mass),
	velocity(0, 0),
	_normal(math::Vector3(0, 0, 0))

{
}

Body* Body::Create(Render::Texture* tex) {
	return new Body(tex);
}

Body* Body::Create(Render::Texture * tex, IPoint & pos){
	return new Body(tex, pos);
}

Body* Body::Create(Render::Texture * tex, IPoint & pos, float mass) {
	return new Body(tex, pos, mass);
}

void Body::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(_pos);
	Render::device.MatrixTranslate(_tex->_bitmap_width * -0.5f, _tex->_bitmap_height * -0.5f, 0);
	_tex->Draw();
	Render::device.PopMatrix();
}

void Body::Update(float dt) {
	if (mooveble) {
	//	IPoint mouse_position = Core::mainInput.GetMousePos();
	//	_pos = mouse_position;
		velocity.x = 1;

	}
	
	_pos += velocity;

}

IRect& Body::GetRect() {
	_rect = IRect(IPoint(_pos.x - _tex->Width()*.5, _pos.y - _tex->Height()*.5), _tex->Width(), _tex->Height());

	return _rect;
}

IPoint& Body::GetPos() {
	return _pos;
}
Render::Texture* Body::GetTex() {
	return _tex;
}

math::Vector3 Body::GetNormal() {
	return _normal;
}