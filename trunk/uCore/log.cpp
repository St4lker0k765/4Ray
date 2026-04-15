#include "stdafx.h"
#include "log.h"

void slog(const char* s)
{
    string4096 split; // [rsp+20h] [rbp-1018h] BYREF

    u64 v1 = 0;
    u64 v2 = 0;
    for (const char* i = s; *i; ++i)
    {
        s = (const char*)*(u8*)i;
        if ((BYTE)s == 10)
        {
            if (v1 >= sizeof(split))
                _report_rangecheckfailure(s, v2);
            split[v1] = 0;
            if (!split[0])
                strcpy(split, " ");
            if ((_S4_2 & 1) == 0)
            {
                _S4_2 |= 1u;
                logger::logger(&impl);
                atexit(instance_::_2_::_dynamic_atexit_destructor_for__impl__);
            }
            logger::add(&impl, split);
            v2 = 0;
            v1 = 0;
        }
        else
        {
            split[v1] = (char)s;
            v2++;
            ++v1;
        }
    }
    if ((unsigned __int64)(int)v2 >= sizeof(split))
        _report_rangecheckfailure(s, v2);
    split[(int)v2] = 0;
    if ((_S4_2 & 1) == 0)
    {
        _S4_2 |= 1u;
        logger::logger(&impl);
        atexit(instance_::_2_::_dynamic_atexit_destructor_for__impl__);
    }
    logger::add(&impl, split);
}

void rlog(const char* format, ...)
{
    u_string<32> v4; // [rsp+38h] [rbp-450h] BYREF
    string1024 buf{};
    va_list va;

    va_start(va, format);
    int v1 = vsnprintf(buf, sizeof(buf), format, va);
    if (v1 >= 0)
    {
        if (v1)
            slog(buf);
    }
    else
    {
        v4.ptr = 0;
        *(_QWORD*)&v4.capacity = 32;
        u_string<32>::vset(&v4, format, va);
        const char* ptr = (const char*)v4.buf;
        if (v4.capacity > 0x20u)
            ptr = v4.ptr;
        slog(ptr);
        if (v4.ptr)
        {
            u_memory::main_realloc(memory(), v4.ptr, 0, 0, "u_string", 0);
        }
    }
}