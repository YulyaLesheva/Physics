#include "stdafx.h"
#include "Quad.h"



Quad::Quad(Render::Texture* tex, IPoint& pos, Gravity value)
	:_tex(tex),
	_pos(pos),
	_speedVector(0,0),
	_speedPixelsPerFrame(10),
	_gravity(-0.7),
	_value(value)
{
}

std::unique_ptr<Quad> Quad::Create(Render::Texture* tex, IPoint& pos, Gravity value) {
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
	
	if (_value == Gravity::_TRUE) {
		_pos.y += _gravity;

	}
	_pos += _speedVector;

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
}

void Quad::KeyReleased(int keyCode) {

	if (keyCode == VK_D) {
		_speedVector.x = 0;
	}

	if (keyCode == VK_A) {
		_speedVector.x = 0;
	}
}