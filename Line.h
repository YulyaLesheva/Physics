#pragma once

struct Line {
	Line(FPoint s, FPoint e);
	FPoint start;
	FPoint end;

	inline double Det(FPoint start, FPoint end);
	bool LineIntersect(Line& a, Line& b, float yOutput = -1, float xOutput =-1);
	
	FPoint lineline(Line& lineA, Line& lineB);
};