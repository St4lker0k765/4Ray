#include "stdafx.h"
#include "libmmgr.h"

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
        mem_debugger* v4 = u_alloc<mem_debugger>(1u);
        if (v4)
            mem_debugger::mem_debugger(v4);
        else
            v5 = 0;
        debug = v5;
    }
    stat_calls = 0;
}