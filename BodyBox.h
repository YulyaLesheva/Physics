#pragma once

struct BodyBox {

	BodyBox(char* tex, FPoint& pos, float m); 
	BodyBox(std::string tex, FPoint& pos, float m, float degrees);
	static BodyBox* Create(char* tex, FPoint& pos, float m);
	static BodyBox* Create(std::string tex, FPoint& pos, float m, float degrees);
	FPoint position;
	FPoint velocity;
	FPoint force;

	float angularVelocity;
	float torque;

	float const elastic;
	FPoint width;

	float friction;
	float mass, inverseMass;
	float I, invI;

	void AddMass() {
		mass -= 1;
	}

	void AddForce(const FPoint& f) {
		force += f;
	}
	Render::Texture* texture;

	//FUNCTIONS
	void Draw();
	void Update(float dt);
	void ApplyForces();

	//test
	float rotation;

	//getters
	FPoint GetMin();
	FPoint GetMax();

	IRect GetRect();
	std::vector<FPoint> GetVertices();
	//help shit
	IRect rect;

	//INPUT
	void KeyPressed(int keyCode);
	bool MouseDown(const IPoint& mouse_pos);
	bool MouseUp(const IPoint& mouse_pos);

	bool anchored;
	void setdegrees(float deg);

};