#include "stdafx.h"
#include "Line.h"


Line::Line(FPoint s, FPoint e)
	:start(s), end(e) 
{
}

inline double Line::Det(FPoint start, FPoint end) {
	auto result = start.x * end.y - start.y * end.x;
	return result ;
}

bool Line::LineIntersect(Line& a, Line& b, float yOutput, float xOutput) {
	auto detA = Det(a.start, a.end);
	auto detB = Det(b.start, b.end);

	auto x1mx2 = a.start.x - a.end.x;
	auto x3mx4 = b.start.x - b.end.x;

	auto y1my2 = a.start.y - a.end.y;
	auto y3my4 = b.start.y - b.end.y;

	auto xNomerator = Det(FPoint(detA, x1mx2), FPoint(detB, x3mx4));
	auto yNomerator = Det(FPoint(detA, y1my2), FPoint(detB, y3my4));
	auto denomenator = Det(FPoint(x1mx2, y1my2), FPoint(x3mx4, y3my4));

	if (denomenator == 0.f) {
		xOutput = NAN;
		yOutput = NAN;
		Log::Info("......");
		return false;
	}
	
	xOutput = xNomerator / denomenator;
	yOutput = xNomerator / denomenator;

	if (!isfinite(xOutput)|| !isfinite(yOutput)) {
		return false;
	}
	Log::Info("interseeeeeeeeeeects");
	return true;
}

FPoint Line::lineline(Line& lineA, Line& lineB) {

	FPoint result;
	float x1 = lineA.start.x, x2 = lineA.end.x, x3 = lineB.start.x, x4 = lineB.end.x;
	float y1 = lineA.start.y, y2 = lineA.end.y, y3 = lineB.start.y, y4 = lineB.end.y;

	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

	if (d == 0) return FPoint(NULL, NULL);

	float pre = (x1*y2 - y1 * x2), post = (x3*y4 - y3 * x4);
	float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

	if (x < math::min(x1, x2) || x > math::max(x1, x2) ||
		x < math::min(x3, x4) || x > math::max(x3, x4)) 
	{
		Log::Info("no interseeeeeeeeeeects");
		return FPoint(NULL, NULL);
	} 
	if (y < math::min(y1, y2) || y > math::max(y1, y2) ||
		y < math::min(y3, y4) || y > math::max(y3, y4)) {
		Log::Info("no interseeeeeeeeeeects");
		return FPoint(NULL, NULL);
	} 
	Log::Info("inter");
	result = FPoint(x, y);
	return result;
}