#pragma once
#include "Body.h"

namespace BodyColission {
	
	bool CheckColissionAndGetNormal(Body* bodyOne, Body* bodyTwo);
	void ResolveColission(Body* bodyOne, Body* bodyTwo);
	float MinElastic(Body* bodyOne, Body* bodyTwo);
	void PosCorrection(Body* bodyOne, Body* bodyTwo);
	void PositionalCorrection(Body* bodyOne, Body* bodyTwo);
}