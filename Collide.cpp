#pragma once
#include "stdafx.h"
#include "BodyBox.h"
#include "Collide.h"
#include "Math.h"
#include "Arbiter.h"
#include "Line.h"

std::vector<Line> GetEdges(BodyBox* a) {
	std::vector<Line> result;
	result.reserve(4);

	std::vector<FPoint> vertices = a->GetVertices();

	result = {
		Line(vertices[0],vertices[1]),//0
		Line(vertices[2],vertices[3]),//1
		Line(vertices[0],vertices[2]),//2
		Line(vertices[1],vertices[3]) //3
	};
	return result;
}

std::vector<FPoint> ClipToEdges(BodyBox* a, BodyBox* b) {

	std::vector<FPoint> result;
	FPoint intersectionPoint; 

	auto edgesA = GetEdges(a);
	auto edgesB = GetEdges(b);

	result.reserve(edgesA.size());
	for (int i = 0; i < edgesA.size(); ++i) {
		for (int j = 0; j < edgesB.size(); ++j) {
			auto res = edgesA[i].lineline(edgesB[j], intersectionPoint);
			if (res) {
				result.push_back(intersectionPoint);
			}
		}
	}
	return result;
}

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

float GetDepth(BodyBox* a, BodyBox* b, FPoint& axis, bool* shouldFlip) {
	
	auto interval_a = GetInterval(a, axis);
	auto interval_b = GetInterval(b, axis);

	if (!((interval_b.x <= interval_a.y) && (interval_a.x <= interval_b.y))) return 0.0f;

	float len_a = interval_a.y - interval_a.x;
	float len_b = interval_b.y - interval_b.x;

	float min = fminf(interval_a.x, interval_b.x);
	float max = fmaxf(interval_a.y, interval_b.y);

	float length = max - min;

	if (shouldFlip != 0) {
		*shouldFlip = (interval_b.x > interval_a.x);
	}

	return (len_a + len_b) - length;
}


Arbiter CollideFeatures(BodyBox* a, BodyBox* b) {
	Arbiter result(a,b);
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

	FPoint* hitNormal = 0;
	bool shouldFlip;

	for (int i = 0; i < axisToCheck.size(); ++i) {
		float depth = GetDepth(a, b, axisToCheck[i], &shouldFlip);

		if (depth <= 0.f) {
			//Log::Info("........");
			return result;
		} 

		else if (depth < result.separation) {
			if (shouldFlip) {
				axisToCheck[i] = axisToCheck[i] * -1.f;
			}
			result.separation = depth;
			hitNormal = &axisToCheck[i];
		}
	}

	if (hitNormal == 0) return result;

	FPoint axis = hitNormal->Normalized();
	
	std::vector<FPoint> c1 = ClipToEdges(a, b);
	std::vector<FPoint> c2 = ClipToEdges(b, a);
	
	//тут он определяет верные точки

	result.contacts.reserve(c1.size() + c2.size());
	result.contacts.insert(result.contacts.end(), c1.begin(), c1.end());
	result.contacts.insert(result.contacts.end(), c2.begin(), c2.end());

	auto interval = GetInterval(a, axis);
	float distance = (interval.y - interval.x) * 0.5f - result.separation * 0.5f;
	FPoint pointOnPlane = a->position + axis * distance;

	for (int i = result.contacts.size() - 1; i >= 0; --i) {
		FPoint contact = result.contacts[i];
		result.contacts[i] = contact + (axis * axis.GetDotProduct(pointOnPlane - contact));

		for (int j = result.contacts.size() - 1; j > i; --j) {
			if ((result.contacts[j] - result.contacts[i]).GetDotProduct(result.contacts[j]
				- result.contacts[i]) < 0.0001f) {
				result.contacts.erase(result.contacts.begin() + j);
				break;
			}
		}
	}
	
	result.normal = axis;
	result.colliding = true;
	//Log::Info("Depth is "+ std::to_string(result.separation));
	//Log::Info("NORMAL is "+ std::to_string(result.normal.x) + " and " + std::to_string(result.normal.y));
	//мне нужно, чтобы 
	return result;
}

