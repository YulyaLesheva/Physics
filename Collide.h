#pragma once
FPoint GetInterval1(BodyBox* a, FPoint axis);
bool OverlapOnAxis1(BodyBox* a, BodyBox* b, FPoint& axis);
bool ABBcollideABB1(BodyBox* a, BodyBox* b);