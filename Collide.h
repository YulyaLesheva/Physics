#pragma once
FPoint GetInterval(BodyBox* a, FPoint axis);
bool OverlapOnAxis(BodyBox* a, BodyBox* b, FPoint& axis);
bool ABBcollideABB(BodyBox* a, BodyBox* b);