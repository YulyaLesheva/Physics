#include "stdafx.h"
#include "BodyBox.h"
#include "Math.h"
#include "Collide.h"
#define GRAVITY_CONST FPoint(0, -59.82f)

//than mass closer to 0, than object is heaver 
BodyBox::BodyBox()
{
	rotation = 0.0;
	velocity = FPoint(0, 0);
	angularVelocity = 0.0f;
	force = FPoint(0, 0);
	torque = 0.0;//0
	friction = 0.2;

	mass = FLT_MAX;
	invI = 0.0f;
	mass = 0;
	
	inverseMass = 0.0f;
	I = FLT_MAX;
	invI = 0.0f;
}

BodyBox::BodyBox(char* tex, FPoint& pos, float m):
	texture(Core::resourceManager.Get<Render::Texture>(tex)),
	position(pos),
	anchored(false)
{
	rotation = 0.0;
	velocity = FPoint(0, 0);
	angularVelocity = 0.0f;
	force = FPoint(0,0);
	torque = 0.0;//0
	friction = 0.2;
	
	mass = FLT_MAX;
	invI = 0.0f;
	mass = m;
	width = FPoint(texture->Width(), texture->Height());

	if (mass > 0)
	{
		inverseMass = 1.0f / mass;
		I = mass * (width.x * width.x  + width.y * width.y) / 12.0f;
		invI = 1.0f / I;
	}
	else
	{
		inverseMass = 0.0f;
		I = FLT_MAX;
		invI = 0.0f;
	}

}


BodyBox::BodyBox(std::string tex, FPoint& pos, float m, float degrees)
	:texture(Core::resourceManager.Get<Render::Texture>(tex)),
	position(pos),
	anchored(false)
{
	rotation = math->DEG2RAD(degrees);
	velocity = FPoint(0, 0);
	angularVelocity = 0.0f;
	force = FPoint(0, 0);
	torque = 0.0f;
	friction = 0.2;

	mass = FLT_MAX;
	invI = 0.0f;
	mass = m;
	width = FPoint(texture->Width(), texture->Height());

	if (mass > 0)
	{
		inverseMass = 1.0f / mass;
		I = mass * (width.x * width.x + width.y * width.y) / 12.0f;
		invI = 1.0f / I;
	}
	else
	{
		inverseMass = 0.0f;
		I = FLT_MAX;
		invI = 0.0f;
	}

}

BodyBox* BodyBox::Create(char* tex, FPoint& pos, float m) {
	return new BodyBox(tex, pos, m);
}

BodyBox* BodyBox::Create(std::string tex, FPoint& pos, float m, float degrees) {
	return new BodyBox(tex, pos, m, degrees);
}

void BodyBox::Set(const FPoint& w, float m) {
	width = w;
	mass = m;

	position.Set(0.0f, 0.0f);
	rotation = 0.0f;
	velocity.Set(0.0f, 0.0f);
	angularVelocity = 0.0f;
	force.Set(0.0f, 0.0f);
	torque = 0.0f;
	friction = 0.2f;

	width = w;
	mass = m;

	if (mass < FLT_MAX)
	{
		inverseMass = 1.0f / mass;
		I = mass * (width.x * width.x + width.y * width.y) / 12.0f;
		invI = 1.0f / I;
	}
	else
	{
		inverseMass = 0.0f;
		I = FLT_MAX;
		invI = 0.0f;
	}
}

void BodyBox::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(position);	
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), math->RAD2DEG(rotation));
	Render::device.MatrixTranslate(width.x * -0.5f, width.y * -0.5f, 0);
	texture->Draw();
	Render::device.PopMatrix();
}

void BodyBox::Update(float dt) {
	
	Math m;
	/*if (rotation > 360 || rotation < - 360) 
		rotation = 0;*/

	//angularVelocity -= m.DEG2RAD(rotation) * 0.05;

	if (anchored) {
		IPoint mouse_position = Core::mainInput.GetMousePos();
		position = mouse_position;
	}

	if (inverseMass == 0.0f) return;

}

void BodyBox::ApplyForces() {
	force = GRAVITY_CONST;
}

FPoint BodyBox::GetMin() {
	auto r = GetRect();
	return FPoint(r.LeftBottom());
}

FPoint BodyBox::GetMax() {
	auto r = GetRect();
	return FPoint(r.RightTop());
}

std::vector<FPoint> BodyBox::GetVertices() {

	FPoint min = GetMin();
	FPoint max = GetMax();

	std::vector<FPoint> vertices = {
		FPoint(min.x, min.y), FPoint(min.x, max.y),
		FPoint(max.x, min.y), FPoint(max.x, max.y)
	};

	if (rotation != 0.0) {
		for (int i = 0; i < vertices.size(); ++i) {
			Math math;
			math.ROTATE(vertices[i], rotation, position);
		}
	}

	return vertices;
}

IRect BodyBox::GetRect() {
	return IRect(position.x - width.x * 0.5, position.y - width.y * 0.5, width.x, width.y);
}

void BodyBox::KeyPressed(int keyCode) {
	if (keyCode == VK_D) {
		rotation += 15;
	}
	if (keyCode == VK_A) {
		rotation -= 15;
	}
	if (keyCode == VK_SPACE) {
		AddMass();
	}
}

bool BodyBox::MouseDown(const IPoint& mouse_pos) {
	if (PointInBodyBox(mouse_pos, this)) {
		anchored = true;
		return true;
	}

	return false;
}

bool BodyBox::MouseUp(const IPoint& mouse_pos) {
	anchored = false;
	return false;
}

void BodyBox::SetDegrees(float deg) {
	rotation += math->DEG2RAD(deg);
}
