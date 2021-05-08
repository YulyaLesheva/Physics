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
		Line(vertices[0],vertices[1]),//0                                              1...|3|..3   
		Line(vertices[2],vertices[3]),//1											   .	    .			
		Line(vertices[0],vertices[2]),//2                                              |0|    |1|
		Line(vertices[1],vertices[3]) //3											   .	    .
	};								  //										       0...|2|..2
	return result;
}

std::vector<FPoint> ClipToEdges(BodyBox* a, BodyBox* b) {
	//���������� ����� �����������
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

std::vector<Contact> ClipToEdgesCONTACTS(BodyBox* a, BodyBox* b) {
	std::vector<Contact> result;
	Contact intersectionPoint;

	auto edgesA = GetEdges(a);
	auto edgesB = GetEdges(b);

	result.reserve(edgesA.size());
	for (int i = 0; i < edgesA.size(); ++i) {
		for (int j = 0; j < edgesB.size(); ++j) {
			auto res = edgesA[i].lineline(edgesB[j], intersectionPoint.position);
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
	//����� �����������. �� ����� �������� � ������? ��� ����� ��������������? 
	std::vector<FPoint> c1 = ClipToEdges(a, b);
	std::vector<FPoint> c2 = ClipToEdges(b, a);
	
	std::vector<Contact> co1 = ClipToEdgesCONTACTS(a, b);
	std::vector<Contact> co2 = ClipToEdgesCONTACTS(b, a);
	
	//��� �� ���������� ������ �����

	result.contacts.reserve(c1.size() + c2.size());
	result.contacts.insert(result.contacts.end(), c1.begin(), c1.end());
	result.contacts.insert(result.contacts.end(), c2.begin(), c2.end());

	result.contactsNEW.reserve(co1.size() + co2.size());
	result.contactsNEW.insert(result.contactsNEW.end(), co1.begin(), co1.end());
	result.contactsNEW.insert(result.contactsNEW.end(), co2.begin(), co2.end());

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
	// ��� ��������� �������. �������� ���������� � ��������� ����� 
	for (int i = result.contactsNEW.size() - 1; i >= 0; --i) {
		Contact contact = result.contactsNEW[i];
		result.contactsNEW[i] = contact.position + (axis * axis.GetDotProduct(pointOnPlane - contact.position));

		for (int j = result.contactsNEW.size() - 1; j > i; --j) {
			if ((result.contactsNEW[j].position - result.contactsNEW[i].position).GetDotProduct(result.contactsNEW[j].position
				- result.contactsNEW[i].position) < 0.0001f) {
				result.contactsNEW.erase(result.contactsNEW.begin() + j);
				break;
			}
		}
	}

	//Log::Info(std::to_string(result.contactsNEW.size()));
	result.normal = axis;
	result.colliding = true;
	/*Log::Info("FIRST FUNCTION: depth is " + std::to_string(result.separation));
	Log::Info("FIRST FUNCTION: normal is " + std::to_string(result.normal.x) + " " + std::to_string(result.normal.y));*/
	//Log::Info("Depth is "+ std::to_string(result.separation));
	//Log::Info("NORMAL is "+ std::to_string(result.normal.x) + " and " + std::to_string(result.normal.y));
	return result;
}



int Collide(std::vector<Contact>& contacts, BodyBox* a, BodyBox* b) {
	
	Math math;
	float separation(FLT_MAX);

	std::vector<FPoint> axisToCheck = {
		FPoint(0, 1),FPoint(1, 0), //����������� � � �
		FPoint(0, 1),FPoint(1, 0), // � � � ��� ������� �������, ���������
		FPoint(0, 1),FPoint(1, 0), // � � � ��� ������� �������, ���������
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
			return 0;
		}

		else if (depth < separation) {
			if (shouldFlip) {
				axisToCheck[i] = axisToCheck[i] * -1.f;
			}
			separation = depth;
			hitNormal = &axisToCheck[i];
		}
	}

	if (hitNormal == 0) return 0;

	FPoint axis = hitNormal->Normalized();

	std::vector<Contact> co1 = ClipEdgesToBodyBox(GetEdges(a), b);
	std::vector<Contact> co2 = ClipEdgesToBodyBox(GetEdges(b) ,a);

	contacts.reserve(co1.size() + co2.size());
	contacts.insert(contacts.end(), co1.begin(), co1.end());
	contacts.insert(contacts.end(), co2.begin(), co2.end());

	auto interval = GetInterval(a, axis);
	float distance = (interval.y - interval.x) * 0.5f - separation * 0.5f;
	FPoint pointOnPlane = a->position + axis * distance;
	
	int numContacts = 0;

	for (int i = contacts.size() - 1; i >= 0; --i) {
		Contact contact = contacts[i];
		contacts[i] = contact.position + (axis * axis.GetDotProduct(pointOnPlane - contact.position));
		for (int j = contacts.size() - 1; j > i; --j) {
			if ((contacts[j].position - contacts[i].position).GetDotProduct(contacts[j].position
				- contacts[i].position) < 0.0001f) {
				contacts.erase(contacts.begin() + j);
				break;
			}
		}
	}

	for (int i = 0; i < contacts.size(); ++i) {
		numContacts++;
		contacts[i].contactNormal = axis;
		contacts[i].depth = separation;
	}

	//a->TESTNORMAL = axis;

	Log::Info(std::to_string(numContacts));
	//Log::Info("SECOND FUNCTION: depth is " + std::to_string(separation));
	//Log::Info("SECOND FUNCTION: normal is " + std::to_string(axis.x) + " " + std::to_string(axis.y));

	return numContacts;
}

std::vector<Plane> GetPlanes(BodyBox* a) {

	Math m; 

	FPoint centre = a->position;
	FPoint e = a->width * 0.5;
	
	std::vector<FPoint> BoxAxis = {
		FPoint(0, 1),FPoint(1, 0),
	};
	
	m.ROTATE(BoxAxis[0], a->rotationValue, FPoint(0, 0));
	m.ROTATE(BoxAxis[1], a->rotationValue, FPoint(0, 0));
	
	std::vector<Plane> result;
	result.resize(4);

	result[0] = Plane(BoxAxis[0], m.Dot(BoxAxis[0], (centre + BoxAxis[0] * e.x)));
	result[1] = Plane(BoxAxis[0] * -1.f, - m.Dot(BoxAxis[0], (centre - BoxAxis[0] * e.x)));
	result[2] = Plane(BoxAxis[1], m.Dot(BoxAxis[1], (centre + BoxAxis[1] * e.y)));
	result[3] = Plane(BoxAxis[1] * -1.f, -m.Dot(BoxAxis[1], (centre - BoxAxis[1] * e.y)));

	return result;
}

bool ClipToPlane(const Plane& plane, const Line& line, FPoint* outPoint) {

	Math m;
	FPoint ab = line.end - line.start;

	float nA = m.Dot(plane.normal, line.start);
	float nAB = m.Dot(plane.normal, ab);

	float t = (plane.distance - nA) / nAB;
	if (t >= 0.0f && t <= 1.0f) {
		if (outPoint != 0) {
			*outPoint = line.start + ab * t;
		}
		//Log::Info("TRUE RETURNED");
		return true;
	}

	return false;
}

std::vector<Contact> ClipEdgesToBodyBox(const std::vector<Line>& edges, BodyBox* bodyBox) {

	std::vector<Contact> result;
	result.reserve(edges.size());
	FPoint intersection;

	std::vector<Plane>& planes = GetPlanes(bodyBox);
	for (int i = 0; i < planes.size(); ++i) {
		for (int j = 0; j < edges.size(); ++j) {
			auto res = ClipToPlane(planes[i], edges[j], &intersection);
			if (res) {
				auto test = intersection; /////����� ����� ��������
				if (PointInBodyBox(intersection, bodyBox)) {
					result.push_back(intersection);
				}
			}
		}
	}
	return result;
}

bool PointInBodyBox(const FPoint& point, BodyBox* bodyBox) {
	
	Math m;

	FPoint dir = point - bodyBox->position;
	
	std::vector<FPoint> BoxAxis = {
		FPoint(0, 1),FPoint(1, 0),
	};
	
	m.ROTATE(BoxAxis[0], bodyBox->rotationValue, FPoint(0, 0));
	m.ROTATE(BoxAxis[1], bodyBox->rotationValue, FPoint(0, 0));
	
	for (int i = 0; i < BoxAxis.size(); ++i) {
		float distance = m.Dot(dir, BoxAxis[i]);

		if (distance > bodyBox->width.x * 0.5 || distance > bodyBox->width.y * 0.5) {
			//Log::Info("NOT IN THE BOX");
			return false;
		}

		if (distance < -bodyBox->width.x * 0.5 || distance < -bodyBox->width.y * 0.5) {
			//Log::Info("NOT IN THE BOX");
			return false;
		}
	}

	//Log::Info("YES IN THE BOX");
	return true;
}

bool PointInBodyBox(const Contact& point, BodyBox* bodyBox) {

	Math m;

	FPoint dir = point.position - bodyBox->position;

	std::vector<FPoint> BoxAxis = {
		FPoint(0, 1),FPoint(1, 0),
	};

	m.ROTATE(BoxAxis[0], bodyBox->rotationValue, FPoint(0, 0));
	m.ROTATE(BoxAxis[1], bodyBox->rotationValue, FPoint(0, 0));

	for (int i = 0; i < BoxAxis.size(); ++i) {
		float distance = m.Dot(dir, BoxAxis[i]);

		if (distance > bodyBox->width.x * 0.5 || distance > bodyBox->width.y * 0.5) {
			Log::Info("NOT IN THE BOX");
			return false;
		}

		if (distance < -bodyBox->width.x * 0.5 || distance < -bodyBox->width.y * 0.5) {
			Log::Info("NOT IN THE BOX");
			return false;
		}
	}

	Log::Info("YES IN THE BOX");
	return true;
}

FPoint ClosestPoint(BodyBox* bodyBox, const FPoint& point) {
	
	Math m;

	FPoint result = bodyBox->position;
	
	FPoint dir = point - bodyBox->position;

	std::vector<FPoint> BoxAxis = {
		FPoint(0, 1),FPoint(1, 0),
	};

	m.ROTATE(BoxAxis[0], bodyBox->rotationValue, FPoint(0, 0));
	m.ROTATE(BoxAxis[1], bodyBox->rotationValue, FPoint(0, 0));

	for (int i = 0; i < BoxAxis.size(); ++i) {
		float distance = m.Dot(dir, BoxAxis[i]);
		 
		if (distance > bodyBox->width.x * 0.5) {
			distance = bodyBox->width.x * 0.5;
		}
		
		if (distance > bodyBox->width.y * 0.5) {
			distance = bodyBox->width.y * 0.5;
		}

		if (distance < -bodyBox->width.x * 0.5) {
			distance = -bodyBox->width.x * 0.5;
		}

		if (distance < -bodyBox->width.y * 0.5) {
			distance = -bodyBox->width.y * 0.5;
		}
		result = result + (BoxAxis[i] * distance);
	}
	return result;
}