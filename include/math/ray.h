#pragma once

#include "math_types.h"

namespace alfar
{
	namespace ray2d
	{
		void inline create(Ray2D& ray, alfar::Vector2 start, alfar::Vector2 direction, float dist = 1.0f)
		{
			ray.start = start;
			ray.direction = direction;
			ray.dist = dist;
		}
	}
}