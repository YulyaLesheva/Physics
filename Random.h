#pragma once
#include "stdafx.h"
#include <random>

	std::vector<std::string> textures = {

		"Blue",
		"Grey",
		"Green",
		"LightBlue",
		"Pink",
		"Purple",
		"Yellow"
	};

	std::string RandomQuad()
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(0, 6);

		auto random_integer = uni(rng);
		return textures[random_integer];
	}

	float randomRotation() {
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(-360, 360);

		auto random_float = uni(rng);
		return random_float;
	}

