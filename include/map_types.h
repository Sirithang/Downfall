#pragma once

#include "math/math_types.h"
#include "collection_types.h"
#include "array.h"
#include "core/objectmanager.h"

struct MapVertex
{
	struct MapLink
	{
		uint32_t idx;
		char side; // 0 start 1 end
	};

	alfar::Vector2 _position;

	MapLink _linkedLines[128];
	char _nbLines;

	int16_t _index;
};

typedef ObjectManager<MapVertex, 512> VertexManager;

//==================================================

struct LineInfo
{
	alfar::Vector2 start;
	alfar::Vector2 end;

	int startVertex;
	int endVertex;

	int _idx;//if this is -1 this is an unitialized line

	//0 right (start to end), 1 left (end to start), 2 doublesided.
	char side;
	int material;
};

struct MapInfo
{
	foundation::Array<LineInfo> _lines;
	foundation::Array<int> _freeLinesIds;

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

//------------------------