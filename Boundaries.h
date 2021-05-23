#pragma once
#include "BodyBox.h"
struct BodyBox;
struct Math;

enum BOUNDARY {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

struct Boundaries : public BodyBox {
	Boundaries(int thickPx);
	static Boundaries* Create(int thickPx);
	IRect up;
	IRect down;
	IRect left;
	IRect right;
	
	//Getters
	FPoint GetWidth(BOUNDARY type);
	FPoint GetWidth(IRect rect);
	FPoint GetPosition(IRect rect);
	void Draw();


	std::vector<IRect> allBoundaries;
	float w, h;
	int thick_px;
};