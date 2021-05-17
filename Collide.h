#pragma once
struct Arbiter;
struct Line;
struct Contact;
struct Plane;
struct ClipSegmentToLine;
struct ClipVertex;
union FeaturePair;
struct Matrix22;

FPoint GetInterval(BodyBox* a, FPoint axis);
bool OverlapOnAxis(BodyBox* a, BodyBox* b, FPoint& axis);

bool OBBCollideOBB(BodyBox* a, BodyBox* b);
Arbiter CollideFeatures(BodyBox* a, BodyBox* b);
int Collide(std::vector<Contact>& contacts, BodyBox* a, BodyBox* b);

std::vector<Plane> GetPlanes(BodyBox* a);
std::vector<Line> GetEdges(BodyBox* a);
float GetDepth(BodyBox* a, BodyBox* b, FPoint& axis, bool* shouldFlip);

void Flip(FeaturePair& fp);

int ClipSegmentToLine(ClipVertex vOut[2], ClipVertex vIn[2],
	const FPoint& normal, float offset, char clipEdge);

bool ClipToPlane(const Plane& plane, const Line& line, FPoint* outPoint);

std::vector<FPoint> ClipToEdges(BodyBox* a, BodyBox* b);
std::vector<Contact> ClipToEdgesCONTACTS(BodyBox* a, BodyBox* b);
std::vector<Contact> ClipEdgesToBodyBox(const std::vector<Line>& edges, BodyBox* bodyBox);
bool PointInBodyBox(const FPoint& point, BodyBox* bodyBox);
bool PointInBodyBox(const Contact& point, BodyBox* bodyBox);
FPoint ClosestPoint(BodyBox* bodyBox, const FPoint& point);

int CollideNEW(Contact* contacts, BodyBox* a, BodyBox* b);

static void ComputeIncidentEdge(ClipVertex c[2], const FPoint& h, const FPoint& pos,
	const Matrix22 Rot, const FPoint& normal);
