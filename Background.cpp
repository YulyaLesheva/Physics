#include "stdafx.h"
#include "Background.h"

Background::Background(Render::Texture* tex)
	:_tex(tex)
{
}

std::unique_ptr<Background> Background::Create(Render::Texture* tex) {
	return std::unique_ptr<Background>(new Background(tex));
}

void Background::Draw() {
	Render::device.PushMatrix();
	_tex->Draw();
	Render::device.PopMatrix();
}