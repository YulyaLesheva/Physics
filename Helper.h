#pragma once
#include "stdafx.h"

namespace Helper {
	Render::Texture* UseTexture(char *texturesName) {
		return Core::resourceManager.Get<Render::Texture>(texturesName);
	}
}
