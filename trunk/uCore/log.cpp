#include "stdafx.h"
#include "log.h"

void rlog(const char* format, ...)
{
    u_memory* v3; // rax
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
            v3 = memory();
            u_memory::main_realloc(v3, v4.ptr, 0, 0, "u_string", 0);
        }
    }
}