#pragma once

enum Gravity {
	_TRUE, _FALSE
};

class Quad
{
public:
	Quad(Render::Texture* tex, FPoint& pos, Gravity value = Gravity::_FALSE);
	static std::unique_ptr<Quad>Create(Render::Texture* tex, FPoint& pos, Gravity value = Gravity::_FALSE);
	void Draw();
	void KeyPressed(int keyCode);
	void Update(float dt);
	void KeyReleased(int keyCode);
	Gravity _value;
	IRect& GetRect();
	FPoint& GetPos();
	void ApplyForces();

private:
	Render::Texture *_tex;
	///From the book 
	math::Vector3 _pos, _oldPos;
	math::Vector3 _forces, _velocity;
	float  _mass, bounce;
	const math::Vector3 _gravity;
	float _friction, _bounce;
	///
	FPoint _speedVector;
	const int _speedPixelsPerFrame;
	IRect _rect;
};

