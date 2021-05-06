#pragma once

struct Math;

struct Line {
	Line(FPoint s, FPoint e);
	FPoint start;
	FPoint end;

	
	bool lineline(const Line& lineA, FPoint& outPoint = FPoint(0,0));
};

struct Plane {

	Plane(const FPoint& n, float d) :
	normal(n), distance(d)
	{
	}
	
	Plane() = default;

	float PlaneEquation(const FPoint& pt, const Plane& plane);

	FPoint normal;
	float distance;

};