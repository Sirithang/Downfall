#pragma once

#include "math_types.h"
#include "vector4.h"
#include <math.h>

namespace alfar
{
    namespace mat4x4
    {
        inline Matrix4x4 create(const Vector4& x, const Vector4& y, const Vector4& z, const Vector4& t)
        {
            Matrix4x4 mat;
            mat.x = x;
            mat.y = y;
            mat.z = z;
            mat.t = t;

            return mat;
        }

        //----------------------------------------------------------------------------------------

        inline Matrix4x4 mul(const Matrix4x4& a, const Matrix4x4& b)
        {
            Matrix4x4 out;

            out.x.x = a.x.x * b.x.x + a.x.y * b.y.x + a.x.z * b.z.x + a.x.w *b.t.x;
            out.x.y = a.x.x * b.x.y + a.x.y * b.y.y + a.x.z * b.z.y + a.x.w *b.t.y;
            out.x.z = a.x.x * b.x.z + a.x.y * b.y.z + a.x.z * b.z.z + a.x.w *b.t.z;
            out.x.w = a.x.x * b.x.w + a.x.y * b.y.w + a.x.z * b.z.w + a.x.w *b.t.w;

            out.y.x = a.y.x * b.x.x + a.y.y * b.y.x + a.y.z * b.z.x + a.y.w *b.t.x;
            out.y.y = a.y.x * b.x.y + a.y.y * b.y.y + a.y.z * b.z.y + a.y.w *b.t.y;
            out.y.z = a.y.x * b.x.z + a.y.y * b.y.z + a.y.z * b.z.z + a.y.w *b.t.z;
            out.y.w = a.y.x * b.x.w + a.y.y * b.y.w + a.y.z * b.z.w + a.y.w *b.t.w;

            out.z.x = a.z.x * b.x.x + a.z.y * b.y.x + a.z.z * b.z.x + a.z.w *b.t.x;
            out.z.y = a.z.x * b.x.y + a.z.y * b.y.y + a.z.z * b.z.y + a.z.w *b.t.y;
            out.z.z = a.z.x * b.x.z + a.z.y * b.y.z + a.z.z * b.z.z + a.z.w *b.t.z;
            out.z.w = a.z.x * b.x.w + a.z.y * b.y.w + a.z.z * b.z.w + a.z.w *b.t.w;

            out.t.x = a.t.x * b.x.x + a.t.y * b.y.x + a.t.z * b.z.x + a.t.w *b.t.x;
            out.t.y = a.t.x * b.x.y + a.t.y * b.y.y + a.t.z * b.z.y + a.t.w *b.t.y;
            out.t.z = a.t.x * b.x.z + a.t.y * b.y.z + a.t.z * b.z.z + a.t.w *b.t.z;
            out.t.w = a.t.x * b.x.w + a.t.y * b.y.w + a.t.z * b.z.w + a.t.w *b.t.w;

            return out;
        }

        //---------------------------------------------------------------------------------------------

        inline Matrix4x4 ortho(float right, float left, float top, float bottom, float zFar, float zNear)
        {
            Matrix4x4 mat;

            mat.x.x = 2.0f / (right - left);
            mat.x.y = 0;
            mat.x.z = 0;
            mat.x.w = 0;

            mat.y.x = 0;
            mat.y.y = 2.0f/(top-bottom);
            mat.y.z = 0;
            mat.y.w = 0;

            mat.z.x = 0;
            mat.z.y = 0;
            mat.z.z = 2.0f/(zFar - zNear);
            mat.z.w = 0;

            mat.t.x = -(right + left)/(right - left);
            mat.t.y = -(top + bottom) / (top - bottom);
            mat.t.z = -(zFar + zNear) / (zFar - zNear);
            mat.t.w = 1;

			return mat;
        }

        //----------------------------------------------------------------------------------------

		inline Matrix4x4 identity()
		{
			return create(alfar::vector4::create(1,0,0,0), alfar::vector4::create(0,1,0,0), alfar::vector4::create(0,0,1,0), alfar::vector4::create(0,0,0,1));
		}

		//=========================================================================================

		inline Matrix4x4 translation (alfar::Vector3 p_Translate)
		{
			Matrix4x4 ret = identity();

			ret.x.w = p_Translate.x;
			ret.y.w = p_Translate.y;
			ret.z.w = p_Translate.z;

			return ret;
		}
    }
}