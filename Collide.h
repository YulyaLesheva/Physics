#pragma once
class Contacts;
FPoint GetInterval(BodyBox* a, FPoint axis);
bool OverlapOnAxis(BodyBox* a, BodyBox* b, FPoint& axis);
bool ABBcollideABB(BodyBox* a, BodyBox* b);
bool OBBCollideOBB(BodyBox* a, BodyBox* b);
void Collide(Contacts* c, BodyBox* a, BodyBox* b);
float GetDepth(BodyBox* a, BodyBox* b, FPoint& axis, bool* shouldFlip);
Contacts CollideFeatures(BodyBox* a, BodyBox* b);
