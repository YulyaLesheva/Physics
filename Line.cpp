#include "stdafx.h"
#include "Line.h"


Line::Line(FPoint s, FPoint e)
	:start(s), end(e) 
{
}


bool Line::lineline(const Line& lineA, FPoint& outPoint) {

	float x1 = lineA.start.x;
	float y1 = lineA.start.y;
	float x2 = lineA.end.x;
	float y2 = lineA.end.y;
	
	float x3 = this->start.x;
	float y3 = this->start.y;
	float y4 = this->end.y;
	float x4 = this->end.x;

	float determinant = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

	if (determinant == 0) return false;

	float pre = (x1*y2 - y1 * x2), post = (x3*y4 - y3 * x4);
	float x = (pre * (x3 - x4) - (x1 - x2) * post) / determinant;
	float y = (pre * (y3 - y4) - (y1 - y2) * post) / determinant;

	// Point(x,y) is the point where lines are intersected, 
	//so you may return it if needed
	
	if (x < math::min(x1, x2) || x > math::max(x1, x2) ||
		x < math::min(x3, x4) || x > math::max(x3, x4)) 
	{
		//Log::Info("no interseeeeeeeeeeects");
		outPoint = FPoint(NULL, NULL);
		return false;
	} 
	if (y < math::min(y1, y2) || y > math::max(y1, y2) ||
		y < math::min(y3, y4) || y > math::max(y3, y4)) {

		//Log::Info("no interseeeeeeeeeeects");
		outPoint = FPoint(NULL, NULL);
		return false;
	} 
	
	//Log::Info("inter");
	outPoint = FPoint(x, y);
	return true;
}