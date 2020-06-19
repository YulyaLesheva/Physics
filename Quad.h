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
	void Collide(std::unique_ptr<Quad>& a, std::unique_ptr<Quad>& b);
	float DotProduct(FPoint& a, FPoint& b);
	bool _colissionFound;
	float GetMass();
	void SetPos(FPoint newPos);
private:
	Render::Texture *_tex;
	///From the book 
	///
	IRect _rect;
	float _timer;
	float _vc;
	FPoint _pos;
	float _mass;
	const float _inverseMass;
	math::Vector3 _velocity;
};

