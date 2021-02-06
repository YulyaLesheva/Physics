#include "stdafx.h"
#include "PhysicBody.h"

PhysicBody::PhysicBody(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction)
	:Body(tex, pos)
{
}

PhysicBody* PhysicBody::Create(Render::Texture * tex, FPoint & pos, float mass, float elastic, float friction)
{
	return new PhysicBody(tex, pos, mass, elastic, friction);
}


void PhysicBody::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(_pos);
	Render::device.MatrixTranslate(_tex->_bitmap_width * -0.5f, _tex->_bitmap_height * -0.5f, 0);
	_tex->Draw();
	Render::device.PopMatrix();
}

void PhysicBody::Update(float dt)
{
}

void PhysicBody::ReverseCurrentVectorX()
{
}

void PhysicBody::ReverseCurrentVectorY()
{
}

void PhysicBody::KeepInBorders()
{
}

bool PhysicBody::OnBorder()
{
	return false;
}

void PhysicBody::ApplyGravity()
{
}
