#pragma once

class Quad
{
public:
	Quad(Render::Texture* tex, IPoint& pos);
	static std::unique_ptr<Quad>Create(Render::Texture* tex, IPoint& pos);
	void Draw();
	void KeyPressed(int keyCode);
	void Update(float dt);
	void KeyReleased(int keyCode);

private:
	Render::Texture *_tex;
	IPoint _pos;
	IPoint _speedVector;
	const int _speedPixelsPerFrame;

};

