#include "stdafx.h"
#include "Quad.h"

const FPoint GRAVITY_VALUE = FPoint(0, -1);


Quad::Quad(Render::Texture* tex, FPoint& pos, MoveState state, float m, float e)
	:_tex(tex),
	_pos(pos),
	_state(state), 
	mass(m),
	elastic(e),
	inverseMass(1/m),
	penetration(0),
	normal(0,0,0)
{
	if (inverseMass == 0) inverseMass = 0;
	else inverseMass = 1 / m;

}

std::unique_ptr<Quad> Quad::Create(Render::Texture* tex, FPoint& pos, MoveState state, float m, float e) {
	return std::unique_ptr<Quad>(new Quad(tex, pos, state, m, e));
}

void Quad::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(_pos);
	Render::device.MatrixTranslate(_tex->_bitmap_width * -0.5f, _tex->_bitmap_height * -0.5f, 0);
	_tex->Draw();
	Render::device.PopMatrix();
}

void Quad::Update(float dt) {

	if (_state == MoveState::yes) {
		IPoint mouse_position = Core::mainInput.GetMousePos();
		_pos = mouse_position;
	}
}


IRect& Quad::GetRect() {

	_rect =  IRect(IPoint(_pos.x -_tex->Width()*.5, _pos.y - _tex->Height()*.5), _tex->Width(), _tex->Height());
	return _rect;
}
void Quad::KeyPressed(int keyCode)
{
	
}

void Quad::KeyReleased(int keyCode) {

	
}

FPoint& Quad::GetPos() {
	return _pos;
}

void Quad::ApplyVector(math::Vector3 vector) {
	_pos += IPoint(vector.x, vector.y);
}

Render::Texture* Quad::GetTexture() {
	return _tex;
}