#include "stdafx.h"

bool crc32_initialized = false;
u32 crc32_table[256];

u64 compression::crc32_init()
{
    int v0; // r10d
    unsigned int* v1; // r11
    int v2; // edx
    int v3; // edx
    int v4; // edx
    int v5; // edx
    __int64 result; // rax
    unsigned int v7; // r9d
    int v8; // r8d
    unsigned int v9; // eax

    R_ASSERT(!crc32_initialized);
    v0 = 0;
    v1 = crc32_table;
    for (u32 v0 = 0; v0 <= 0xFF; v0++)
    {
        v2 = 0;
        if ((v0 & 1) != 0)
            v2 = 128;
        if ((v0 & 2) != 0)
            v2 |= 0x40u;
        if ((v0 & 4) != 0)
            v2 |= 0x20u;
        if ((v0 & 8) != 0)
            v2 |= 0x10u;
        if ((v0 & 0x10) != 0)
            v2 |= 8u;
        if ((v0 & 0x20) != 0)
            v2 |= 4u;
        if ((v0 & 0x40) != 0)
            v2 |= 2u;
        if ((((unsigned int)v0 >> 6) & 2) != 0)
            v2 |= 1u;
        v3 = (2 * ((v2 << 25) ^ ((v2 & 0x80) != 0 ? 0x4C11DB7 : 0)))
            ^ (((v2 << 25) ^ ((v2 & 0x80) != 0 ? 0x4C11DB7 : 0)) < 0 ? 0x4C11DB7 : 0);
        v4 = (2 * ((2 * v3) ^ (v3 < 0 ? 0x4C11DB7 : 0))) ^ (((2 * v3) ^ (v3 < 0 ? 0x4C11DB7 : 0)) < 0 ? 0x4C11DB7 : 0);
        v5 = (2 * ((2 * v4) ^ (v4 < 0 ? 0x4C11DB7 : 0))) ^ (((2 * v4) ^ (v4 < 0 ? 0x4C11DB7 : 0)) < 0 ? 0x4C11DB7 : 0);
        result = (2 * ((2 * v5) ^ (v5 < 0 ? 0x4C11DB7 : 0)))
            ^ (((2 * v5) ^ (v5 < 0 ? 0x4C11DB7 : 0)) < 0 ? 0x4C11DB7 : 0);
        v7 = 0;
        v8 = 30;
        *v1 = result;
        do
        {
            if ((result & 1) != 0)
                v7 |= 1 << (v8 + 1);
            v9 = (unsigned int)result >> 1;
            if ((v9 & 1) != 0)
                v7 |= 1 << v8;
            v8 -= 2;
            result = v9 >> 1;
        } while (v8 > -2);
        *v1++ = v7;
    }

    crc32_initialized = true;
    return result;
}

u64 compression::crc32(u8* P, u32 len, u32 init)
{
    if (!crc32_initialized)
        compression::crc32_init();

    for (; len; --len)
    {
        init = (init >> 8) ^ crc32_table[*P++ ^ (u8)init];
    }
    return ~init;
}

u64 compression::crc32_inc(u8* P, u32 len, u32 init)
{
    if (!crc32_initialized)
        compression::crc32_init();

    for (; len; --len)
    {
        init = (init >> 8) ^ crc32_table[*P++ ^ (u8)init];
    }
    return init;
}

// upper 3 bits
#define pvSIGN_MASK		0xe000
#define pvXSIGN_MASK	0x8000
#define pvYSIGN_MASK	0x4000
#define pvZSIGN_MASK	0x2000

// middle 6 bits - xbits
#define pvTOP_MASK		0x1f80

// lower 7 bits - ybits
#define pvBOTTOM_MASK	0x007f

// static lookup table for unit vector3 decompression
float pvUVAdjustment[0x2000];
bool statics_initalized = false;

void pvInitializeStatics(void)
{
    for (int idx = 0; idx < 0x2000; idx++)
    {
        long xbits = idx >> 7;
        long ybits = idx & pvBOTTOM_MASK;

        // map the numbers back to the triangle (0,0)-(0,127)-(127,0)
        if ((xbits + ybits) >= 127)
        {
            xbits = 127 - xbits;
            ybits = 127 - ybits;
        }

        // convert to 3D vectors
        float x = float(xbits);
        float y = float(ybits);
        float z = float(126 - xbits - ybits);

        // calculate the amount of normalization required
        pvUVAdjustment[idx] = 1.0f / sqrt(y * y + z * z + x * x);
    }
    statics_initalized = true;
}

u16 compression::compress::normal(const Fvector3 vec)
{
    if (!statics_initalized)
    {
        pvInitializeStatics();
    }
    // save copy
    Fvector3 tmp = vec;

    // input vector3 does not have to be unit length
    u16 mVec = 0;

    if (negative(tmp.x)) { mVec |= pvXSIGN_MASK; set_positive(tmp.x); }
    if (negative(tmp.y)) { mVec |= pvYSIGN_MASK; set_positive(tmp.y); }
    if (negative(tmp.z)) { mVec |= pvZSIGN_MASK; set_positive(tmp.z); }

    // project the normal onto the plane that goes through
    // X0=(1,0,0),Y0=(0,1,0),Z0=(0,0,1).

    // on that plane we choose an (projective!) coordinate system
    // such that X0->(0,0), Y0->(126,0), Z0->(0,126),(0,0,0)->Infinity

    // a little slower... old pack was 4 multiplies and 2 adds. 
    // This is 2 multiplies, 2 adds, and a divide....
    float w = 126.0f / (tmp.x + tmp.y + tmp.z);
    int		xbits = iFloor(tmp.x * w);
    int		ybits = iFloor(tmp.y * w);

    /*
    VERIFY( xbits <  127 );
    VERIFY( xbits >= 0   );
    VERIFY( ybits <  127 );
    VERIFY( ybits >= 0   );
    */

    // Now we can be sure that 0<=xp<=126, 0<=yp<=126, 0<=xp+yp<=126

    // however for the sampling we want to transform this triangle 
    // into a rectangle.
    if (xbits >= 64)
    {
        xbits = 127 - xbits;
        ybits = 127 - ybits;
    }

    // now we that have xp in the range (0,127) and yp in the range (0,63), 
    // we can pack all the bits together
    mVec |= (xbits << 7);
    mVec |= ybits;

    return mVec;
}


Fvector3 compression::decompress::normal(Fvector3 result, u16 N)
{
    if (!statics_initalized)
    {
        pvInitializeStatics();
    }
    // if we do a straightforward backward transform
    // we will get points on the plane X0,Y0,Z0
    // however we need points on a sphere that goes through these points.
    // therefore we need to adjust x,y,z so that x^2+y^2+z^2=1

    // by normalizing the vector3. We have already precalculated the amount
    // by which we need to scale, so all we do is a table lookup and a 
    // multiplication

    // get the x and y bits
    int xbits = ((N & pvTOP_MASK) >> 7);
    int ybits = (N & pvBOTTOM_MASK);

    // map the numbers back to the triangle (0,0)-(0,126)-(126,0)
    if ((xbits + ybits) >= 127)
    {
        xbits = 127 - xbits;
        ybits = 127 - ybits;
    }

    // do the inverse transform and normalization
    // costs 3 extra multiplies and 2 subtracts. No big deal.         
    float uvadj = pvUVAdjustment[N & ~pvSIGN_MASK];

    Fvector3 vec;
    vec.x = uvadj * float(xbits);
    vec.y = uvadj * float(ybits);
    vec.z = uvadj * float(126 - xbits - ybits);

    // set all the sign bits
    if (N & pvXSIGN_MASK) set_negative(vec.x);
    if (N & pvYSIGN_MASK) set_negative(vec.y);
    if (N & pvZSIGN_MASK) set_negative(vec.z);
    return vec;
}
