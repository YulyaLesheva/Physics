#pragma once

class Background
{
public:
	Background(Render::Texture* tex);
	static std::unique_ptr<Background>Create(Render::Texture* tex);
	void Draw();
private:
	Render::Texture* _tex;
};



