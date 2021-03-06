#pragma once

struct Line {
	Line(FPoint s, FPoint e);
	FPoint start;
	FPoint end;

	
	bool lineline(Line& lineA);
};