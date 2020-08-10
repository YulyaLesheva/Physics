#pragma once

class Body {
public:
	Body(Render::Texture* tex);
	Body(Render::Texture* tex, IPoint& pos);
	Body(Render::Texture* tex, IPoint& pos, float mass);

	static Body* Create(Render::Texture* tex);
	static Body* Create(Render::Texture* tex, IPoint& pos);
	static Body* Create(Render::Texture* tex, IPoint& pos, float mass);

public:
	//FUNCTIONS
	void Draw();
	void Update(float dt);

public:
	//GETTERS
	IRect& GetRect();
	IPoint& GetPos();
	Render::Texture* GetTex();
	math::Vector3 GetNormal();
public:
	//VARIABLES
	float mass, inverseMass;
	bool mooveble;
	math::Vector3& _normal;
	IPoint velocity;

private:
	//VARIABLES
	Render::Texture* _tex;
	IPoint _pos;
	IRect _rect;

};