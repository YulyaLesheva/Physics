#pragma once
struct Arbiter;
struct Line;
struct Contact;
struct Plane;

FPoint GetInterval(BodyBox* a, FPoint axis);
bool OverlapOnAxis(BodyBox* a, BodyBox* b, FPoint& axis);

bool OBBCollideOBB(BodyBox* a, BodyBox* b);
Arbiter CollideFeatures(BodyBox* a, BodyBox* b);
int Collide(std::vector<Contact>& contacts, BodyBox* a, BodyBox* b);

std::vector<Plane> GetPlanes(BodyBox* a);
std::vector<Line> GetEdges(BodyBox* a);
float GetDepth(BodyBox* a, BodyBox* b, FPoint& axis, bool* shouldFlip);


bool ClipToPlane(const Plane& plane, const Line& line, FPoint& outPoint);
void ClipSegmentToLine(FPoint& vertixOut, FPoint& vertixIn,
	const FPoint& normal, float offset, Line& edge);
std::vector<FPoint> ClipToEdges(BodyBox* a, BodyBox* b);
std::vector<Contact> ClipToEdgesCONTACTS(BodyBox* a, BodyBox* b);
std::vector<FPoint> ClipEdgesToBodyBox(const std::vector<Line>& edges, BodyBox* bodyBox);
bool PointInBodyBox(const FPoint& point, BodyBox* bodyBox);
FPoint ClosestPoint(BodyBox* bodyBox, const FPoint& point);