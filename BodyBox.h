#pragma once

struct BodyBox {

	BodyBox(char* texe, FPoint& pos, float m); 
	static BodyBox* Create(char* tex, FPoint& pos, float m);
	FPoint position;
	FPoint velocity;
	FPoint force;

	float rotation;
	float angularVelocity;
	float torque;

	FPoint width;

	float friction;
	float mass, inverseMass;
	float I, invI;

	Render::Texture* texture;

	//FUNCTIONS
	void Draw();
	void Update(float dt);

	//test
	float rotationValue;

	//getters
	FPoint GetMin();
	FPoint GetMax();
	IRect GetRect();
	//help shit
	IRect rect;

	//INPUT
	void KeyPressed(int keyCode);
	bool MouseDown(const IPoint& mouse_pos);
	bool MouseUp(const IPoint& mouse_pos);

	bool anchored;

};