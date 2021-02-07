#include "stdafx.h"
#include "Body.h"
#include "BodyCollision.h"


Body::Body(Render::Texture* tex) :
	_tex(tex)
{
}

Body::Body(Render::Texture* tex, FPoint& pos) :
	_tex(tex),
	_pos(pos)
{
}

Body::~Body() {
	delete this;
}

Body* Body::Create(Render::Texture* tex) {
	return new Body(tex);
}

Body* Body::Create(Render::Texture * tex, FPoint & pos){
	return new Body(tex, pos);
}

void Body::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(_pos);
	Render::device.MatrixTranslate(_tex->_bitmap_width * -0.5f, _tex->_bitmap_height * -0.5f, 0);
	_tex->Draw();
	Render::device.PopMatrix();
}

void Body::Update(float dt) {

}

IRect& Body::GetRect() {
	_rect = IRect(IPoint(_pos.x - _tex->Width()*.5, _pos.y - _tex->Height()*.5), _tex->Width(), _tex->Height());

	return _rect;
}

FPoint& Body::GetPos() {
	return _pos;
}
Render::Texture* Body::GetTex() {
	return _tex;
}


bool Body::OnBorder() {
	auto rect = GetRect();

	if (rect.LeftBottom().x < 0 || rect.RightBottom().x > Render::device.Width()) {
		return true;
	}

	if (rect.LeftTop().y > Render::device.Height() || rect.LeftBottom().y < 0) {
		return true;
	}
	return false;
}



FPoint Body::GetMin() {
	
	FPoint resultMin;

	auto rect = GetRect();
	auto minX = rect.LeftBottom().x;
	auto minY = rect.LeftBottom().y;

	resultMin = FPoint(minX, minY);

	return resultMin;
}

FPoint Body::GetMax() {

	FPoint resultMax;
	
	auto rect = GetRect();
	auto maxX = rect.RightBottom().x;
	auto maxY = rect.RightTop().y;

	resultMax = FPoint(maxX, maxY);
	return resultMax;
}

