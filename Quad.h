#pragma once

enum MoveState
{
	yes, no

};

class Quad
{
public:

	Quad(Render::Texture* tex, FPoint& pos, MoveState state, float m, float e);
	static std::unique_ptr<Quad>Create(Render::Texture* tex, FPoint& pos, MoveState state = no, float m = 0.5, float e = 0.5);
	void Draw();
	void KeyPressed(int keyCode);
	void Update(float dt);
	void KeyReleased(int keyCode);
	IRect& GetRect();
	FPoint& GetPos();
	void ApplyVector(math::Vector3 vector);
	Render::Texture* GetTexture();
public:
	float mass, elastic, inverseMass, penetration;
	math::Vector3 normal;
	float PUBLICVAR;
private:
	Render::Texture *_tex;
	///From the book 
	///
	IRect _rect;
	FPoint _pos;
	MoveState _state;

	float PRIVATEVAR;

};

