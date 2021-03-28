#pragma once
#include "stdafx.h"
#include "BodyBox.h"
#include "Collide.h"
#include "Math.h"
#include "Arbiter.h"

FPoint GetInterval(BodyBox* a, FPoint axis) {

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

	std::vector<FPoint> axisToCheck = {
		FPoint(0, 1),FPoint(1, 0)
	};

	axisToCheck.push_back(math.ROTATE_RETURN(FPoint(0, 1), b->rotationValue, FPoint(0, 0)));
	axisToCheck.push_back(math.ROTATE_RETURN(FPoint(1, 0), b->rotationValue, FPoint(0, 0)));

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

bool OBBCollideOBB(BodyBox* a, BodyBox* b) {

	Math math;

	std::vector<FPoint> axisToCheck = {
		FPoint(0, 1),FPoint(1, 0),
		FPoint(0, 1),FPoint(1, 0),
		FPoint(0, 1),FPoint(1, 0),
	};

	math.ROTATE(axisToCheck[2], a->rotationValue, FPoint(0, 0));
	math.ROTATE(axisToCheck[3], a->rotationValue, FPoint(0, 0));

	math.ROTATE(axisToCheck[4], b->rotationValue, FPoint(0, 0));
	math.ROTATE(axisToCheck[5], b->rotationValue, FPoint(0, 0));

	for (int i = 0; i < axisToCheck.size(); ++i) {
		if (!OverlapOnAxis(a, b, axisToCheck[i])) {
			Log::Info("No collision");
			return false;
		}
	}

	Log::Info("Collision");
	return true;

}

void Collide(Contacts* c, BodyBox* a, BodyBox* b) {

}
float Depth(BodyBox* a, BodyBox* b) {
	
	Math math;

	std::vector<FPoint> axisToCheck = {
		FPoint(0, 1),FPoint(1, 0),
		FPoint(0, 1),FPoint(1, 0),
		FPoint(0, 1),FPoint(1, 0),
	};

	math.ROTATE(axisToCheck[2], a->rotationValue, FPoint(0, 0));
	math.ROTATE(axisToCheck[3], a->rotationValue, FPoint(0, 0));

	math.ROTATE(axisToCheck[4], b->rotationValue, FPoint(0, 0));
	math.ROTATE(axisToCheck[5], b->rotationValue, FPoint(0, 0));
	
	for (int i = 0; i < axisToCheck.size(); ++i) {

		auto interval_a = GetInterval(a, axisToCheck[i]);
		auto interval_b = GetInterval(b, axisToCheck[i]);

		if (!((interval_b.x <= interval_a.y) && (interval_a.x <= interval_b.y))) {
			//Log::Info("No collision");
			return 0.0f;
		}
	

		float len_a = interval_a.y - interval_a.x;
		float len_b = interval_b.y - interval_b.x;

		float min = fminf(interval_a.x, interval_b.x);
		float max = fmaxf(interval_a.y, interval_b.y);

		float length = max - min;
		auto total = (len_a + len_b) - length;
		return total ;
	}
	
	/*if(outshouldFlip !=0){
		outshouldFlip = (interval_b.x < interval_a.x);
	}*/
	//return 0;
}