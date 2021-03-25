#pragma once
#include "stdafx.h"
#include "BodyBox.h"
#include "Collide.h"
#include "Math.h"

FPoint GetInterval(BodyBox* a, FPoint axis){

	FPoint result(0, 0); //min, max

	std::vector<FPoint> vertices = a->GetVertices();

	//dot is a scalar product
	//find min and max
	result.x = axis.GetDotProduct(vertices[0]);
	result.y = result.x;

	for (int i = 1; i < 4; i++) {
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

bool OverlapOnAxis(BodyBox* a, BodyBox* b, FPoint& axis) {

	FPoint interval_a = GetInterval(a, axis);
	FPoint interval_b = GetInterval(b, axis);

	return ((interval_b.x <= interval_a.y) && (interval_a.x <= interval_b.y));
}

bool ABBcollideABB(BodyBox* a, BodyBox* b) {
	
	Math math;
	

	/*math.ROTATE(axisXY[2], b->rotationValue, FPoint(0,0));
	math.ROTATE(axisXY[3], b->rotationValue, FPoint(0, 0));
	math.ROTATE(axisXY[4], a->rotationValue, FPoint(0, 0));
	math.ROTATE(axisXY[5], a->rotationValue, FPoint(0, 0));*/
	
	std::vector<FPoint> axisToCheck = {
		FPoint(0, 1),FPoint(1, 0)
	};

	axisToCheck.push_back(math.ROTATE_RETURN(FPoint(0,1), b->rotationValue, FPoint(0, 0)));
	axisToCheck.push_back(math.ROTATE_RETURN(FPoint(1,0), b->rotationValue, FPoint(0, 0)));

	axisToCheck.push_back(math.ROTATE_RETURN(FPoint(0, 1), a->rotationValue, FPoint(0, 0)));
	axisToCheck.push_back(math.ROTATE_RETURN(FPoint(1, 0), a->rotationValue, FPoint(0, 0)));
	
	for (int i = 0; i < axisToCheck.size(); ++i) {
		if (!OverlapOnAxis(a, b, axisToCheck[i])) {
			Log::Info("No collision");
			return false;
		}
	}

	Log::Info("Collision");
	return true;
}