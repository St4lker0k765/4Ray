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
