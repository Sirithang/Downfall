#pragma once

namespace alfar
{
    struct Vector2
    {
            float x, y;
    };

    struct Vector3
    {
            float x, y, z;
    };

    struct Vector4
    {
            float x, y, z, w;
    };

    struct Quaternion
    {
            float x, y, z, w;
    };

    struct Matrix3x3
    {
            Vector3 x, y, z;
    };

    struct Matrix4x4
    {
            Vector4 x, y, z, t;
    };

    struct Rect
    {
            Vector2 min, max;
    };

    struct AABB
    {
            Vector3 min, max;
    };

    struct OOBB
    {
            Matrix4x4 tm;
            AABB aabb;
    };

	struct Ray2D
	{
		alfar::Vector2 start;
		alfar::Vector2 direction;
		float dist;
	};
}