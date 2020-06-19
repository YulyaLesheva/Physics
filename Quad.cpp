#include "stdafx.h"
#include "Quad.h"

const FPoint GRAVITY_VALUE = FPoint(0, -1);

Quad::Quad(Render::Texture* tex, FPoint& pos, Gravity value)
	:_tex(tex),
	_pos(pos),
	_value(value),
	_colissionFound(false),
	_mass(0.5),
	_velocity(0, 0, 0),
	_inverseMass(1/_mass)
{
}

std::unique_ptr<Quad> Quad::Create(Render::Texture* tex, FPoint& pos, Gravity value) {
	return std::unique_ptr<Quad>(new Quad(tex, pos, value));
}

void Quad::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(_pos);
	Render::device.MatrixTranslate(_tex->_bitmap_width * -0.5f, _tex->_bitmap_height * -0.5f, 0);
	//Render::device.MatrixScale(_vc);
	_tex->Draw();
	Render::device.PopMatrix();
}

void Quad::Update(float dt) {

	/*const float damping = 0.98f;
	math::Vector3 acceleration = _forces * InverseMass();
	_velocity = _velocity + acceleration;
	_velocity = _velocity * damping;
	 
	_pos = _pos + _velocity;*/

	if (_value == Gravity::_TRUE) {
		_pos += FPoint(_velocity.x, _velocity.y);
	}
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

	/*if (keyCode == VK_D) {
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
	}*/
}

void Quad::KeyReleased(int keyCode) {

	/*if (keyCode == VK_D) {
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
	}*/
}

FPoint& Quad::GetPos() {
	return _pos;
}

void Quad::Collide(std::unique_ptr<Quad>& a, std::unique_ptr<Quad>& b) {

	if (a->GetRect().Intersects(b->GetRect())) {
		math::Vector3  rv = math::Vector3(a->GetPos().x - b->GetPos().x,
		a->GetPos().y - b->GetPos().y, 0).Normalized(); // relative velocity 
	
		auto  velAlongNormal = rv.DotProduct(math::Vector3(0,1,0));

		if (velAlongNormal > 0) return;

		float e = 0.3f;

		float  j = -(1 + e) * velAlongNormal;
		j /= _inverseMass + _inverseMass;
		
		math::Vector3 inpulse = j * math::Vector3(0, 1, 0);

	}

}

void Quad::SetPos(FPoint newPos) {
	_pos = newPos;
}

float Quad::DotProduct(FPoint& a, FPoint& b) {
	auto multiplied = FPoint(a.x *  b.x, a.y * b.y);
	return multiplied.x + multiplied.y;
}

float Quad::GetMass() {
	return _mass;
}