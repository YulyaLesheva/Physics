#include "stdafx.h"
#include "Body.h"

Body::Body(Render::Texture* tex) :
	_tex(tex)
{
}

Body::Body(Render::Texture* tex, FPoint& pos) :
	_tex(tex),
	_pos(pos),
	velocity(0,0)
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

Intervals Body::GetInterval(Body* body, const FPoint& axis) {
	Intervals result;

	FPoint min = body->GetMin();
	FPoint max = body->GetMax();

	FPoint verst[] = {
	FPoint(min.x, min.y), FPoint(min.x, max.y),
	FPoint(max.x, max.y), FPoint(max.x, min.y)
	};

	result.min = result.max = axis.GetDotProduct(verst[0]);

	for (int i = 1; i < 4; ++i) {
		float projection = axis.GetDotProduct(verst[i]);
		if (projection < result.min) result.min = projection;
		if (projection > result.max) result.max = projection;
	}

	return result;
}

bool Body::OverlapOnAxis(Body*a, Body*b, const FPoint &axis) {
	
	Intervals iA = GetInterval(a, axis);
	Intervals iB = GetInterval(b, axis);

	return ((iB.min <= iA.max) && (iA.min <= iB.max));
}

bool Body::TestCollide(Body * body)
{
	auto bodyA = this;

	FPoint axisXY[] = {
	FPoint(1, 0), FPoint(0, 1)
	}; //�������� ��� �������������� ��� ��� ����������� ������ �����

	for (int i = 0; i < 2; ++i) {
		if (!OverlapOnAxis(bodyA, body, axisXY[i])) {
			//Log::Info(".....");
			return false;

		}
	}
	//Log::Info("It's a colission!");
	return true;
}

void Body::KeepInBorders()
{
	auto rect = GetRect();

	if (rect.LeftBottom().x <= 0) {
		_pos.x = _tex->Width() * 0.5;
		ReverseCurrentVectorX();
	}

	if (rect.RightBottom().x >= Render::device.Width()) {
		_pos.x = Render::device.Width() - _tex->Width() * 0.5;
		ReverseCurrentVectorX();
	}

	if (rect.LeftTop().y >= Render::device.Height()) {
		_pos.y = Render::device.Height() - _tex->Height() * 0.5;
		ReverseCurrentVectorY();
	}

	if (rect.LeftBottom().y <= 0) {
		_pos.y = _tex->Height() * 0.5;
		ReverseCurrentVectorY();
	}
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

void Body::ReverseCurrentVectorY() {
	velocity.y = velocity.y * -1;
}

void Body::ReverseCurrentVectorX() {
	velocity.x = velocity.x * -1;
}
