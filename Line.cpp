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
