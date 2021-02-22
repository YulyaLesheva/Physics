#pragma once

struct Intervals {
	float min;
	float max;
};

struct Line {
	FPoint start;
	FPoint end;
};
class Body {
public:
	Body(Render::Texture* tex);
	Body(Render::Texture* tex, FPoint& pos);
	//Body() = default;
	~Body();
	
	static Body* Create(Render::Texture* tex);
	static Body* Create(Render::Texture* tex, FPoint& pos);

public:
	//FUNCTIONS
	void Draw();
	void Update(float dt);
	bool TestCollide(Body* body);
	bool PointOnPlane(const FPoint point, const Line& line);

	FPoint ClosestPoint(const FPoint point, const Line& line);

	bool TestEdgeOnPlane(Body* body);
public:
	//GETTERS
	IRect& GetRect(); //YA
	FPoint& GetPos();//YA
	Render::Texture* GetTex(); //YA
	FPoint GetMin(); // YA
	FPoint GetMax(); // YA
	void KeepInBorders();
	std::vector<FPoint> GetVertices();
	Line GetEdge();
public:
	//VARIABLES
	FPoint _pos; 
	FPoint velocity;

protected:
	Render::Texture* _tex;
	IRect _rect;
	void ReverseCurrentVectorX();
	void ReverseCurrentVectorY();

private:
	Intervals GetInterval(Body* body, const FPoint& axis);
	bool OverlapOnAxis(Body*a, Body*b, const FPoint &axis);
	bool OnBorder();

};