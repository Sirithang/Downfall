#pragma once

#include "map_types.h"

namespace lineinfo
{
	RayIntersect rayCheck(const alfar::Ray2D inRay, const LineInfo* inLines, int number = 1);
}

//----------------------------



//----------------------------

namespace mapinfo
{
	void addLine(MapInfo& map, const LineInfo& info);
}