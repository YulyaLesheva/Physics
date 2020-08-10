#pragma once
#include "Body.h"

namespace BodyColission {
	
	bool CheckColissionAndGetNormal(Body* bodyOne, Body* bodyTwo);
	void ResolveColission(Body* bodyOne, Body* bodyTwo);
}