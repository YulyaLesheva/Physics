#pragma once
struct Math;
struct BodyBox {

	BodyBox(char* tex, FPoint& pos, float m); 
	BodyBox();
	BodyBox(std::string tex, FPoint& pos, float m, float degrees);
	static BodyBox* Create(char* tex, FPoint& pos, float m);
	static BodyBox* Create(std::string tex, FPoint& pos, float m, float degrees);
	static BodyBox* Create() { return new BodyBox(); };

	FPoint position;
	FPoint velocity;
	FPoint force;

	float angularVelocity;
	float torque;

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

	//SETTER 
	void Set(const FPoint& w, float m);

	void SetTexture(Render::Texture* tex) {
		texture = tex;
	};
	//FUNCTIONS
	void Draw();
	void Update(float dt);
	void ApplyForces();

	//test
	float rotation;
	//MATH 
	Math* math;

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
	void SetDegrees(float deg);

};