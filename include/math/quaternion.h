#pragma once

#include "math_types.h"
#include <math.h>

namespace alfar
{
    namespace quaternion
    {
        inline Quaternion create(float x, float y, float z, float w)
        {
            Quaternion quat;
            quat.x = x;
            quat.y = y;
            quat.z = z;
            quat.w = w;

            return quat;
        }

        //-------------------------------------------------------------

        inline float sqrMagnitude(const Quaternion& p_Quat)
        {
            return p_Quat.x * p_Quat.x + p_Quat.y * p_Quat.y + p_Quat.z * p_Quat.z + p_Quat.w * p_Quat.w;
        }

        inline float magnitude(const Quaternion& p_Quat)
        {
            return sqrt(sqrMagnitude(p_Quat));
        }

        //---------------------------------------------------------------

        inline Quaternion normalized(const Quaternion& p_Quat)
        {
            Quaternion quat;
            float mag = magnitude(p_Quat);
            quat.x = p_Quat.x / mag;
            quat.y = p_Quat.y / mag;
            quat.z = p_Quat.z / mag;
            quat.w = p_Quat.w / mag;
            
            return quat;
        }

        //---------------------------------------------------------------------
    }
}