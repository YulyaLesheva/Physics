#include "stdafx.h"
#include "SapAlgorithm.h" 

std::vector <Body*> SapAlgorithm::SortByMinX(std::vector<Body*>& vectorToSort) {
	
	std::sort(vectorToSort.begin(), vectorToSort.end(), [](Body* a, Body* b) -> bool
	{
		return a->_pos.x < b->_pos.x;
	});
	
	return vectorToSort;
}

std::vector <std::vector<Body*>> SapAlgorithm::SAP(std::vector<Body*>& axisList) {
	
	axisList = SortByMinX(axisList);

	std::vector<Body*> activeList;
	std::vector<std::vector<Body*>> allPairs; 

	for (auto i = 0; i < axisList.size(); ++i) {
		for (auto j = 0; j < activeList.size(); ++j) {
			if (axisList[i]->GetRect().LeftBottom().x > activeList[j]->GetRect().RightBottom().x) {
				activeList.erase(activeList.begin() + j);
				j--;
			}
			else {
				std::vector<Body*> mPair;
				mPair.push_back(axisList[i]);
				mPair.push_back(activeList[j]);
				allPairs.push_back(mPair);
				mPair.push_back(axisList[i]);
			}
		}
		activeList.push_back(axisList[i]);
	}
	return allPairs;
}
