#pragma once

#include "map_types.h"

struct Renderer
{
	unsigned int* buffer;
	int w,h;
};

namespace renderer
{
	void clearBuffer(Renderer& rend, int value);
	void raytraceMap(Renderer& renderer, const MapInfo& map);
}