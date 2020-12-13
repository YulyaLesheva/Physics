#pragma once
#include "Body.h"

namespace SapAlgorithm {
	
	std::vector <std::vector<Body*>> SAP(std::vector<Body*>& axisList);
	std::vector <Body*> SortByMinX(std::vector<Body*>& vectorToSort);

}