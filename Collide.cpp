#pragma once
#include "stdafx.h"
#include "BodyBox.h"
#include "Collide.h"

FPoint GetInterval1(BodyBox* a, FPoint axis){

	FPoint result(0, 0); //min, max

	FPoint min = a->GetMin();
	FPoint max = a->GetMax();

	FPoint vertices[] = {
		FPoint(min.x, min.y),
		FPoint(min.x, max.y),
		FPoint(max.x, min.y),
		FPoint(max.x, max.y)
	};

	//dot is a scalar product
	//find min and max
	result.x = axis.GetDotProduct(vertices[0]);
	result.y = result.x;
	for (int i = 0; i < 4; ++i) {
		float projection = axis.GetDotProduct(vertices[i]);
		if (projection < result.x) {
			result.x = projection;
		}
		if (projection > result.y) {
			result.y = projection;
		}
	}
	return result;
	
}

bool OverlapOnAxis1(BodyBox* a, BodyBox* b, FPoint& axis) {

	FPoint interval_a = GetInterval1(a, axis);
	FPoint interval_b = GetInterval1(b, axis);

	return ((interval_b.x <= interval_a.y) && (interval_a.x <= interval_b.y));
}

bool ABBcollideABB1(BodyBox* a, BodyBox* b) {
	FPoint axisXY[] = {
	FPoint(1, 0),
	FPoint(0, 1)
	};
	
	for (int i = 0; i < 2; ++i) {
		if (!OverlapOnAxis1(a, b, axisXY[i])) {
			Log::Info("No collision");
			return false;
		}
	}

	Log::Info("Collision");
	return true;
}