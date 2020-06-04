#include "stdafx.h"
#include "Quad.h"

const FPoint GRAVITY_VALUE = FPoint(0, -1);

Quad::Quad(Render::Texture* tex, FPoint& pos, Gravity value)
	:_tex(tex),
	_pos(pos.x, pos.y, 0),
	_speedVector(0, 0),
	_speedPixelsPerFrame(10),
	_value(value),
	_friction(0.95),
	_gravity(0.0, -0.7, 0),
	_mass(1.0), 
	_bounce(0.7)
{
	ApplyForces();
}

std::unique_ptr<Quad> Quad::Create(Render::Texture* tex, FPoint& pos, Gravity value) {
	return std::unique_ptr<Quad>(new Quad(tex, pos, value));
}

void Quad::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(_pos);
	Render::device.MatrixTranslate(_tex->_bitmap_width * -0.5f, _tex->_bitmap_height * -0.5f, 0);
	_tex->Draw();
	Render::device.PopMatrix();
}

void Quad::Update(float dt) {

	_oldPos = _pos;
	auto acceleration = _forces * (1.f / _mass);
	
	_velocity = _velocity * _friction + acceleration;
	_pos = _pos + _velocity;
}

void Quad::ApplyForces() {
	_forces = _gravity;
}

IRect& Quad::GetRect() {

	_rect = IRect(IPoint(_pos.x -_tex->Width()*.5, _pos.y - _tex->Height()*.5), _tex->Width(), _tex->Height());
	return _rect;
}
void Quad::KeyPressed(int keyCode)
{
	//
	// keyCode - виртуальный код клавиши.
	// ¬ качестве значений дл€ проверки нужно использовать константы VK_.
	//

	if (keyCode == VK_D) {
		_speedVector.x = _speedPixelsPerFrame;
	}

	else if (keyCode == VK_A) {
		_speedVector.x = -(_speedPixelsPerFrame);
	}

	else if (keyCode ==VK_W) {
		_speedVector.y = _speedPixelsPerFrame;
	}

	else if (keyCode == VK_S) {
		_speedVector.y = -(_speedPixelsPerFrame);
	}
}

void Quad::KeyReleased(int keyCode) {

	if (keyCode == VK_D) {
		_speedVector.x = 0;
	}

	if (keyCode == VK_A) {
		_speedVector.x = 0;
	}

	if (keyCode == VK_W) {
		_speedVector.y = 0;
	}

	if (keyCode == VK_S) {
		_speedVector.y = 0;
	}
}

FPoint& Quad::GetPos() {
	return FPoint(_pos.x,_pos.y);
}


