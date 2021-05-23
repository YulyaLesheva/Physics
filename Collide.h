#pragma once
struct Arbiter;
struct Line;
struct Contact;
struct Plane;
struct ClipSegmentToLine;
struct ClipVertex;
struct Boundaries;
union FeaturePair;
struct Matrix22;

void Flip(FeaturePair& fp);

int ClipSegmentToLine(ClipVertex vOut[2], ClipVertex vIn[2],
	const FPoint& normal, float offset, char clipEdge);

int CollideBox(Contact* contacts, BodyBox* a, BodyBox* b);
int CollideBoundary(Contact* contacts, BodyBox* a, Boundaries* b);

static void ComputeIncidentEdge(ClipVertex c[2], const FPoint& h, const FPoint& pos,
	const Matrix22 Rot, const FPoint& normal);

