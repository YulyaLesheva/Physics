#include "stdafx.h"
#include "BodyBox.h"
#include "Math.h"

#define GRAVITY_CONST FPoint(0, -9.82f)

BodyBox::BodyBox(char* tex, FPoint& pos, float m):
	texture(Core::resourceManager.Get<Render::Texture>(tex)),
	position(pos),
	rotationValue(0),
	anchored(false)
{
	rotation = 0.0;
	velocity = FPoint(0, 0);
	angularVelocity = 0.0f;
	force = FPoint(0, 0);
	torque = 0.0f;
	friction = 0.2;
	
	mass = FLT_MAX;
	invI = 0.0f;
	mass = m;
	width = FPoint(texture->Width(), texture->Height());

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

BodyBox* BodyBox::Create(char* tex, FPoint& pos, float m) {
	return new BodyBox(tex, pos, m);
}

void BodyBox::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(position);
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), rotationValue);
	Render::device.MatrixTranslate(texture->_bitmap_width * -0.5f, texture->_bitmap_height * -0.5f, 0);
	texture->Draw();
	Render::device.PopMatrix();
}

void BodyBox::Update(float dt) {
	//if (rotationValue < 0 || rotationValue > 180) rotationValue = 0;
	//Log::Info(std::to_string(rotationValue));

	if (inverseMass == 0.0f) return;

	/*velocity += dt * (GRAVITY_CONST + inverseMass * force);
	position += dt * velocity;
	force = FPoint(0, 0);*/
	//www.codeproject.com/Articles/1214829/Making-a-D-Physics-Engine-Shapes-Worlds-and-Integr
	//FPoint acceleration = force / mass + GRAVITY_CONST;        // Gravity will always act on the body
	//velocity += acceleration * dt;
	//position += velocity * dt;
	//force = FPoint(0, 0);
	//почему то масса не влияет на скорость. как это исправить? 

	//const float damping = 0.98;
	//FPoint acceleration = force * inverseMass;
	//velocity += acceleration * dt;
	//velocity *= damping;

	//position += velocity;

	if (anchored) {
		IPoint mouse_position = Core::mainInput.GetMousePos();
		position = mouse_position;
	}
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

	if (rotationValue != 0.0) {
		for (int i = 0; i < vertices.size(); ++i) {
			Math math;
			math.ROTATE(vertices[i], rotationValue, position);
		}
	}

	return vertices;
}

IRect BodyBox::GetRect() {
	return IRect(position.x - width.x * 0.5, position.y - width.y * 0.5, width.x, width.y);
}

void BodyBox::KeyPressed(int keyCode) {
	if (keyCode == VK_D) {
		rotationValue += 15;
	}
	if (keyCode == VK_A) {
		rotationValue -= 15;
	}
}

bool BodyBox::MouseDown(const IPoint& mouse_pos) {
	if (GetRect().Contains(mouse_pos)) {
		anchored = true;
		return true;
	}
	else {
		return false;
	}
}

bool BodyBox::MouseUp(const IPoint& mouse_pos) {
	anchored = false;
	return false;
}

void BodyBox::setdegrees(float deg) {
	rotationValue = deg;
}
