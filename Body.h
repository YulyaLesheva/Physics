#pragma once

class Body {
public:
	Body(Render::Texture* tex);
	Body(Render::Texture* tex, FPoint& pos);
	//Body(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction);
	//Body() = default;
	~Body();
	
	static Body* Create(Render::Texture* tex);
	static Body* Create(Render::Texture* tex, FPoint& pos);
//	static Body* Create(Render::Texture* tex, FPoint& pos, float mass, float elastic, float friction = 0.6);

	bool MouseDown(const IPoint& mouse_pos);
	bool MouseUp(const IPoint& mouse_pos);

public:
	//FUNCTIONS
	void Draw();
	void Update(float dt);
	bool OnBorder(); // no


public:
	//GETTERS
	IRect& GetRect(); //YA
	FPoint& GetPos();//YA
	Render::Texture* GetTex(); //YA
	FPoint GetMin(); // YA
	FPoint GetMax(); // YA
	

public:
	//VARIABLES
	FPoint _pos; 
	//float mass, inverseMass, elastic, friction; 
	//FPoint velocity;

	////For sleep stage
	//bool isAwake;
	//FPoint motion;
	//float rwaMotion;
	//float sleepEpsilon;
	//bool isCollided;
	//bool _needToCheckSleep;
	//bool canSleep;



protected:
	//VARIABLES
	Render::Texture* _tex;
	IRect _rect;
	FPoint _lastPos;
	FPoint moving;
	//bool _anchored;
	FPoint _gravity;
	FPoint _forces;

	

	
private:
	//FUNCTIONS 
	FPoint impulse; // NO

};