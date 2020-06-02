#pragma once

enum Gravity {
	_TRUE, _FALSE
};

class Quad
{
public:
	Quad(Render::Texture* tex, IPoint& pos, Gravity value = Gravity::_FALSE);
	static std::unique_ptr<Quad>Create(Render::Texture* tex, IPoint& pos, Gravity value = Gravity::_FALSE);
	void Draw();
	void KeyPressed(int keyCode);
	void Update(float dt);
	void KeyReleased(int keyCode);
	Gravity _value;
	IRect& GetRect();
	IPoint& GetPos();
	IPoint& GetForceValue();

private:
	Render::Texture *_tex;
	IPoint _pos;
	IPoint _speedVector;
	const int _speedPixelsPerFrame;
	const float _gravity;
	IRect _rect;
	IPoint _newGravity;
	IPoint _testCurrentVector;
	IPoint _forces;


};

