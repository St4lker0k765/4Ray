#pragma once
#include <math.h>

namespace urandom 
{
	class simple
	{
	protected:
		static volatile int holdrand;
	public:
		inline static float f()
		{
			volatile int rand = 214013L * holdrand + 2531011L;
			holdrand = rand;
			return (float)((rand >> 16) & 0x7FFF)/max_f();
		}
		inline static u64 i()
		{
			volatile int rand = 214013L * holdrand + 2531011L;
			holdrand = rand;
			return (rand >> 16) & 0x7FFF;
		}
		inline static float max_f()
		{
			return 32767.0f;
		}
		inline static void seed(volatile int val)
		{
			holdrand = val;
		}
		simple(volatile int _seed)
		{
			holdrand = _seed;
		}
	};
	class normal : public simple
	{
		inline static float f(float max)
		{
			return simple::f() * max;
		}
		inline static float f(float min, float max)
		{
			return (float)(simple::f() * (float)(max - min)) + min;
		}
		inline static float f_norm(const float sigma, float min_r2)
		{
			double v4; // fp30
			double v6; // fp31

			do
			{
				do
				{
					holdrand = 214013L * holdrand + 2531011L;
					__int64 v3 = holdrand;
					v3 = (holdrand >> 16) & 0x7FFF;
					holdrand = 214013L * holdrand + 2531011L;
					v3 = (holdrand >> 16) & 0x7FFF;
					v4 = (((v3 + 4) * 0.000061037019) - 1.0);
					double v5 = ((v3 * 0.000061037019) - 1.0);
					v6 = v5 * v5 + (v4 * v4);
				} while (v6 <= 0.60644531);
			} while (v6 > 1.0);
			double v9 = sqrtf((((log(min_r2) / v6) * -2.0) * v4) * 0.5);
			return *((float*)&v9 + 1);
		}
		inline static float f()
		{
			return fmaxf(fminf(f_norm(0.5f, 0.60644531f) + 0.5, 1.0), 0.0);
		}
		inline static float fs(float range)
		{
			return f(-range, range);
		}
		inline static float fs(float range, float offs)
		{
			return offs+fs(range);
		}
	};
	class UCORE_API game : public simple {};
	class UCORE_API primary : public simple {};
}
