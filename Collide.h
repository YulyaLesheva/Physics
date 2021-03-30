#pragma once
struct Arbiter;
struct Line;

FPoint GetInterval(BodyBox* a, FPoint axis);
bool OverlapOnAxis(BodyBox* a, BodyBox* b, FPoint& axis);
bool OBBCollideOBB(BodyBox* a, BodyBox* b);
float GetDepth(BodyBox* a, BodyBox* b, FPoint& axis, bool* shouldFlip);
Arbiter CollideFeatures(BodyBox* a, BodyBox* b);
std::vector<Line> GetEdges(BodyBox* a);
std::vector<FPoint> ClipToEdges(BodyBox* a, BodyBox* b);
