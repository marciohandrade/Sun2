
#ifndef _FLOATING_H
#define _FLOATING_H

#include "PreProcDefines.h"
#include <math.h>
#include <float.h>

extern float fl_isqrt( float a );
extern float frand();
extern int rand_chance(float frametime, float chance = 1.0f);
float frand_range(float min, float max);

// determine if a floating point number is NaN (Not a Number)
#define fl_is_nan(fl) _isnan(fl)

// Handy macros to prevent type casting all over the place

#define fl_sqrt(fl) (float)sqrt((float)(fl))
#define fl_isqrt(fl) (1.0f/(float)sqrt((float)(fl)))
#define fl_abs(fl) (float)fabs((double)(fl))
#define i2fl(i) ((float)(i))
#define fl2i(fl) ((int)(fl))
#define flceil(fl) (int)ceil(fl)
#define flfloor(fl) (int)floor(fl)
#define f2fl(fx) ((float)(fx)/65536.0f)
#define fl2f(fl) (int)((fl)*65536.0f)

// convert a measurement in degrees to radians
#define fl_radian(fl)	((float)((fl * 3.14159f)/180.0f))

// convert a measurement in radians to degrees
#define fl_degrees(fl)	((float)((fl * 180.0f)/3.14159))

// use this instead of:
// for:  (int)floor(x+0.5f) use fl_round_2048(x)
//       (int)ceil(x-0.5f)  use fl_round_2048(x)
//       (int)floor(x-0.5f) use fl_round_2048(x-1.0f)
//       (int)floor(x)      use fl_round_2048(x-0.5f)
// for values in the range -2048 to 2048
// use this instead of:
// for:  (int)floor(x+0.5f) use fl_round_2048(x)
//       (int)ceil(x-0.5f)  use fl_round_2048(x)
//       (int)floor(x-0.5f) use fl_round_2048(x-1.0f)
//       (int)floor(x)      use fl_round_2048(x-0.5f)
// for values in the range -2048 to 2048

extern const float *p_fl_magic;

inline int fl_round_2048( float x )
{
	double tmp_quad;
	tmp_quad = x + *p_fl_magic;
	return *((int *)&tmp_quad);
}

// rounds off a floating point number to a multiple of some number
extern float fl_roundoff(float x, int multiple);


#endif
