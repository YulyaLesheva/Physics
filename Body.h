#pragma once

class Body {
public:
	Body(Render::Texture* tex);
	Body(Render::Texture* tex, FPoint& pos);
	Body(Render::Texture* tex, FPoint& pos, float mass, float elastic, bool moveState);

	static Body* Create(Render::Texture* tex);
	static Body* Create(Render::Texture* tex, FPoint& pos);
	static Body* Create(Render::Texture* tex, FPoint& pos, float mass, float elastic, bool moveState = false);

	bool MouseDown(const IPoint& mouse_pos);
	bool MouseUp(const IPoint& mouse_pos);

public:
	//FUNCTIONS
	void Draw();
	void Update(float dt);
	void KeepInBorders();
	bool OnBorder();

public:
	//GETTERS
	IRect& GetRect();
	FPoint& GetPos();
	Render::Texture* GetTex();
	math::Vector3 GetNormal();
public:
	//VARIABLES
	float mass, inverseMass, elastic;
	bool mooveble;
	math::Vector3& _normal;
	FPoint velocity;

private:
	//VARIABLES
	Render::Texture* _tex;
	FPoint _pos;
	IRect _rect;
	IPoint _lastPos;
	FPoint moving;
	bool _anchored;
	FPoint _gravity;
private:
	//FUNCTIONS 
	
};