#pragma once

class Body {
public:
	Body(Render::Texture* tex);
	Body(Render::Texture* tex, FPoint& pos);
	Body(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction);

	~Body();
	
	static Body* Create(Render::Texture* tex);
	static Body* Create(Render::Texture* tex, FPoint& pos);
	static Body* Create(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction = 0.6);

	bool MouseDown(const IPoint& mouse_pos);
	bool MouseUp(const IPoint& mouse_pos);

public:
	//FUNCTIONS
	void Draw();
	void Update(float dt);
	void KeepInBorders();
	bool OnBorder();
	void ReverseCurrentVectorX();
	void ReverseCurrentVectorY();
	void SynchPosition();

public:
	//GETTERS
	IRect& GetRect();
	FPoint& GetPos();
	Render::Texture* GetTex();
	FPoint GetMin();
	FPoint GetMax();
	void ApplyForces();//new

public:
	//VARIABLES
	FPoint _pos; 
	float mass, inverseMass, elastic, friction; 
	bool mooveble;
	FPoint velocity;
	float penetrationDepth;
private:
	//VARIABLES
	Render::Texture* _tex;
	IRect _rect;
	IPoint _lastPos;
	FPoint moving;
	bool _anchored;
	FPoint _gravity;
	FPoint _forces;

private:
	//FUNCTIONS 
	void AddLinearImpulse(const FPoint& impulse);//new

	FPoint impulse;

	
};