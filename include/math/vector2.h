#pragma once

#include "math_types.h"
#include <stdint.h>
#include <memory>

namespace alfar
{
    namespace vector2
    {
        inline Vector2 create(float x, float y)
        {
            Vector2 ret;
            ret.x = x;
            ret.y = y;

			return ret;
        }

        inline Vector2 add(const Vector2& p_First, const Vector2& p_Second)
        {
            Vector2 ret;
            ret.x = p_First.x + p_Second.x;
            ret.y = p_First.y + p_Second.y;

            return ret;
        }

        //---------------------------------------------------------------------

        inline Vector2 sub(const Vector2& p_First, const Vector2& p_Second)
        {
            Vector2 ret;
            ret.x = p_First.x - p_Second.x;
            ret.y = p_First.y - p_Second.y;

            return ret;
        }

        //---------------------------------------------------------------------

        inline Vector2 mul(const Vector2& p_Vec, const float p_Scalar)
        {
            Vector2 ret;
            ret.x = p_Vec.x * p_Scalar;
            ret.y = p_Vec.y * p_Scalar;

            return ret;
        }

        //-----------------------------------------------------------------------

        inline Vector2 scale(const Vector2& p_First, const Vector2& p_Second)
        {
            Vector2 ret;
            ret.x = p_First.x * p_Second.x;
            ret.y = p_First.y * p_Second.y;

            return ret;
        }

        //---------------------------------------------------------------------------

        inline float dot(const Vector2& p_First, const Vector2& p_Second)
        {
            return p_First.x * p_Second.x + p_First.y * p_Second.y;
        }

        //-------------------------------------------------------------------------

        inline float sqrMagnitude(const Vector2& p_Vector)
        {
            return p_Vector.x * p_Vector.x + p_Vector.y * p_Vector.y;
        }

        inline float magnitude(const Vector2& p_Vector)
        {
            return sqrt(sqrMagnitude(p_Vector));
        }

		inline Vector2 normalize(const Vector2& p_First)
		{
			Vector2 ret;
			float mag = alfar::vector2::magnitude(p_First);

			ret.x = p_First.x / mag;
			ret.y = p_First.y / mag;

			return ret;
		}

		//--------------------

		inline Vector2 rotate(const Vector2& p_Vec, float angle)
		{
			Vector2 ret;
			ret.x = p_Vec.x * cos(angle) - p_Vec.y * sin(angle); 
			ret.y = p_Vec.x * sin(angle) + p_Vec.y * cos(angle);

			return ret;
		}

        //----- array version


        inline void init(Vector2* p_Array, uint32_t p_Number)
        {
            memset(p_Array, 0, p_Number * sizeof(Vector2));
        }

        //--------------------------------------------------------------------------

        inline void add(Vector2* p_Firsts, Vector2* p_Seconds, Vector2* p_Out, uint32_t p_Number)
        {
            for(uint32_t i = 0; i < p_Number; ++i)
            {
                alfar::Vector2& o = *(p_Out+i);
                alfar::Vector2& a = *(p_Firsts+i);
                alfar::Vector2& b = *(p_Seconds+i);

                o.x = a.x + b.x;
                o.y = a.y + b.y;
            }
        }

        //---------------------------------------------------------------------------

        inline void sub(Vector2* p_Firsts, Vector2* p_Seconds, Vector2* p_Out, uint32_t p_Number)
        {
            for(uint32_t i = 0; i < p_Number; ++i)
            {
                alfar::Vector2& o = *(p_Out+i);
                alfar::Vector2& a = *(p_Firsts+i);
                alfar::Vector2& b = *(p_Seconds+i);

                o.x = a.x - b.x;
                o.y = a.y - b.y;
            }
        }

        //------------------------------------------------------------------------------

        inline void mul(Vector2* p_Firsts, float* p_Scalars, Vector2* p_Out, uint32_t p_Number)
        {
            for(uint32_t i = 0; i < p_Number; ++i)
            {
                alfar::Vector2& o = *(p_Out+i);
                alfar::Vector2& a = *(p_Firsts+i);
                float& s = *(p_Scalars+i);

                o.x = a.x * s;
                o.y = a.y * s;
            }
        }

        //----------------------------------------------------------------------------------

        inline void scale(Vector2* p_Firsts, Vector2* p_Seconds, Vector2* p_Out, uint32_t p_Number)
        {
            for(uint32_t i = 0; i < p_Number; ++i)
            {
                alfar::Vector2& o = *(p_Out+i);
                alfar::Vector2& a = *(p_Firsts+i);
                alfar::Vector2& b = *(p_Seconds+i);

                o.x = a.x * b.x;
                o.y = a.y * b.y;
            }
        }

        //--------------------------------------------------------------------------------------

        inline void dot(Vector2* p_Firsts, Vector2* p_Seconds, float* p_Out, uint32_t p_Number)
        {
            for(uint32_t i = 0; i < p_Number; ++i)
            {
                float& o = *(p_Out+i);
                alfar::Vector2& a = *(p_Firsts+i);
                alfar::Vector2& b = *(p_Seconds+i);

                o = a.x * b.x + a.y * b.y;
            }
        }
    }

	//----------------------------------------------------------------------------------------

		inline alfar::Vector2 operator-(const alfar::Vector2&  p_First)
		{
			alfar::Vector2 ret;
			ret.x = -p_First.x;
			ret.y = -p_First.y;

			return ret;
		}

		inline alfar::Vector2 operator+(const alfar::Vector2& p_First, const alfar::Vector2& p_Second)
		{
			return alfar::vector2::add(p_First, p_Second);
		}

		inline alfar::Vector2 operator-(const alfar::Vector2& p_First, const alfar::Vector2& p_Second)
		{
			return alfar::vector2::sub(p_First, p_Second);
		}

		inline alfar::Vector2 operator*(const alfar::Vector2& p_First, float value)
		{
			return alfar::vector2::mul(p_First, value);
		}
}

