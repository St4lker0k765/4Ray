#include "stdafx.h"
#include "libmmgr.h"

u_memory* memInstance = nullptr;
UCORE_API u_memory* memory()
{
    if (!memInstance)
    {
        memInstance = new u_memory();
    }
    return memInstance;
}

u_memory::u_memory()
{
    mem_debugger* v4; // rax
    mem_debugger* v5; // rax

    threading::mutex::mutex("global-heap");
    void* pool = pool_create("global", 0x50000000u, 0);
    R_ASSERT(pool = pools.front().pool);

    debug = nullptr;
    if (strstr(GetCommandLineA(), "-memdbg"))
    {
        debug = new mem_debugger();
    }
    stat_calls = 0;
}