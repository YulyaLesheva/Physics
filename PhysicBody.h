#pragma once
#include "Body.h"

class PhysicBody : public Body 
{
public:
	PhysicBody(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction);
	static PhysicBody* Create(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction = 0.5);
	void Draw();
	void Update(float dt);


public: 
	float mass, inverseMass, elastic, friction;
	FPoint velocity;
private:
	void ReverseCurrentVectorX();
	void ReverseCurrentVectorY();
	void KeepInBorders();
	bool OnBorder();
	void ApplyGravity();

private:


};