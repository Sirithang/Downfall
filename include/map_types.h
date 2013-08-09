#pragma once

#include "math/math_types.h"
#include "collection_types.h"
#include "array.h"

struct LineInfo
{
	alfar::Vector2 start;
	alfar::Vector2 end;
	//0 right (start to end), 1 left (end to start), 2 doublesided.
	char side;
	int material;
};

struct MapInfo
{
	foundation::Array<LineInfo> _lines;

	MapInfo();
};

struct RayIntersect
{
	int lineID;
	float distance;
	///this contain the purcentage of the hitten wall (0 for start, 1 for end)
	float purcentage;
	alfar::Vector2 point;
};