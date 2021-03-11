#pragma once

struct Line {
	Line(FPoint s, FPoint e);
	FPoint start;
	FPoint end;

	
	bool lineline(const Line& lineA, FPoint& outPoint = FPoint(0,0));
};