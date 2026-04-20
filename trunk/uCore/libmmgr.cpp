#include "stdafx.h"
#include "libmmgr.h"
#include "tlsf.h"

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

u_memory::u_memory(const u_memory* other)
{
    _os = other->_os;
    pools = other->pools;
    tstacks = other->tstacks;
    debug = other->debug;
    stat_calls = other->stat_calls;
}

u_memory::~u_memory()
{
    delete debug;
    tstacks.clear();
    pools.clear();
}

void u_memory::thread_attach()
{
    u64 stack[7];
    _MEMORY_BASIC_INFORMATION inf;
    memset(&stack[1], 0, sizeof(inf));

    memset(&inf, 0, sizeof(inf));
    size_t sz = VirtualQuery(stack, &inf, sizeof(inf));
    R_ASSERT(sz==sizeof(inf));

    u_memory::tstack s;
    s.min = (u64)inf.AllocationBase;
    s.max = (u64)inf.BaseAddress + inf.RegionSize;
    s.tid = GetCurrentThreadId();
}

void u_memory::thread_detach()
{
    for (int i = 0; i < tstacks.size(); i++)
    {
        if (tstacks[i].tid == GetCurrentThreadId())
        {
            tstacks.erase(i);
        }
    }
}

bool u_memory::ptr_on_stack(u64 p)
{
    bool result = false;

    for (int i = 0; i < tstacks.size(); i++)
    {
        if (tstacks[i].min < p)
        {
            if (tstacks[i].max > p)
            {
                result = true;
                break;
            }
        }
    }
    return result;
}

const u_memory::poolreg u_memory::front_pool()
{
    return pools.front();
}

void* u_memory::main_realloc(void* ptr, u64 size, u64 align, const char* _, bool allow_out_of_memory)
{
    ++stat_calls;

    lock();
    void* alloc = pool_realloc(&pools.front().pool, (char*)ptr, size, align, _, allow_out_of_memory);
    unlock();

    return alloc;
}

u64 u_memory::mblock_size(void* ptr, u32 alignment)
{
    return tlsf_block_size(ptr);
}

// Argument for debug config?
void u_memory::_compact(u32 full_free)
{
    if (g_string_container)
        g_string_container->clean();

    if (g_shared_memory_container)
        g_shared_memory_container->clean();

    _heapmin();
    HeapCompact(GetProcessHeap(), 0);
    RegFlushKey(HKEY_CLASSES_ROOT);
    RegFlushKey(HKEY_CURRENT_USER);
    SetProcessWorkingSetSize(GetCurrentProcess(), 0xFFFFFFFFFFFFFFFFuLL, 0xFFFFFFFFFFFFFFFFuLL);
}

void u_memory::_dump()
{
    if (g_string_container)
        g_string_container->clean();

    if (g_shared_memory_container)
        g_shared_memory_container->clean();

    _heapmin();
    HeapCompact(GetProcessHeap(), 0);
    RegFlushKey(HKEY_CLASSES_ROOT);
    RegFlushKey(HKEY_CURRENT_USER);
    SetProcessWorkingSetSize(GetCurrentProcess(), 0xFFFFFFFFFFFFFFFFuLL, 0xFFFFFFFFFFFFFFFFuLL);
    if (debug)
        debug->dump();
}

void u_memory::_dump_prepare()
{
    if (g_string_container)
        g_string_container->clean();

    if (g_shared_memory_container)
        g_shared_memory_container->clean();

    _heapmin();
    HeapCompact(GetProcessHeap(), 0);
    RegFlushKey(HKEY_CLASSES_ROOT);
    RegFlushKey(HKEY_CURRENT_USER);
    SetProcessWorkingSetSize(GetCurrentProcess(), 0xFFFFFFFFFFFFFFFFuLL, 0xFFFFFFFFFFFFFFFFuLL);
    if (debug)
        debug->dump_prepare();
}

void u_memory::_heap(void** memblock, u64* memsize)
{
    *memblock = 0;
    *memsize = 0;
    if (pools.size())
    {
        *memblock = pools.front().memblock;
        *memsize = pools.front().memsize;
    }
}

void u_memory::_loss()
{
    if (g_string_container)
        g_string_container->clean();

    if (g_shared_memory_container)
        g_shared_memory_container->clean();

    _heapmin();
    HeapCompact(GetProcessHeap(), 0);
    RegFlushKey(HKEY_CLASSES_ROOT);
    RegFlushKey(HKEY_CURRENT_USER);
    SetProcessWorkingSetSize(GetCurrentProcess(), 0xFFFFFFFFFFFFFFFFuLL, 0xFFFFFFFFFFFFFFFFuLL);
    if (debug)
        debug->loss();
}

void u_memory::_state_diff(u32 state_id)
{
    if (g_string_container)
        g_string_container->clean();

    if (g_shared_memory_container)
        g_shared_memory_container->clean();

    _heapmin();
    HeapCompact(GetProcessHeap(), 0);
    RegFlushKey(HKEY_CLASSES_ROOT);
    RegFlushKey(HKEY_CURRENT_USER);
    SetProcessWorkingSetSize(GetCurrentProcess(), 0xFFFFFFFFFFFFFFFFuLL, 0xFFFFFFFFFFFFFFFFuLL);
    if (debug)
        debug->state_diff(state_id);
}

u32 u_memory::_state_pointer()
{
    if (g_string_container)
        g_string_container->clean();

    if (g_shared_memory_container)
        g_shared_memory_container->clean();

    _heapmin();
    HeapCompact(GetProcessHeap(), 0);
    RegFlushKey(HKEY_CLASSES_ROOT);
    RegFlushKey(HKEY_CURRENT_USER);
    SetProcessWorkingSetSize(GetCurrentProcess(), 0xFFFFFFFFFFFFFFFFuLL, 0xFFFFFFFFFFFFFFFFuLL);
    if (debug)
        return debug->state_pointer();

    return -1;
}

void u_memory::_stats(bool output_debug_string, bool vm)
{
    u_svector<u_memory::poolreg, 6, unsigned short>* p_pools; // rdi
    __int64 v9; // rbx
    float v11; // xmm0_4
    unsigned int v12; // eax
    float v13; // xmm0_4
    float v14; // xmm1_4
    __int64 v15; // rbx
    float v17; // xmm0_4
    float v19; // xmm0_4
    float v20; // xmm1_4
    u64 load[2]; // [rsp+50h] [rbp-158h] BYREF

    if (g_string_container)
        g_string_container->clean();

    if (g_shared_memory_container)
        g_shared_memory_container->clean();

    _heapmin();
    HeapCompact(GetProcessHeap(), 0);
    RegFlushKey(HKEY_CLASSES_ROOT);
    RegFlushKey(HKEY_CURRENT_USER);
    SetProcessWorkingSetSize(GetCurrentProcess(), 0xFFFFFFFFFFFFFFFFuLL, 0xFFFFFFFFFFFFFFFFuLL);

    p_pools = &this->pools;
    load[0] = 0;
    load[1] = 0;
    lock();
    tlsf_walk_pool(pools.front().pool, 0, load);
    unlock();

    v9 = pools.front().memsize;
    v11 = (float)SLODWORD(load[0]);
    v12 = v10 * 1024;
    if ((load[0] & 0x8000000000000000uLL) != 0LL)
        v11 = v11 + 1.8446744e19;
    v13 = load[0] * 100.0;
    v14 = (float)(int)v9;
    if (v9 < 0)
        v14 = v14 + 1.8446744e19;
    rlog(
        "- pool (%10s): %2.1f load : %zdK total, %dK used, %dK largest block, %dK str_shared",
        pools.front().desc,
        (float)(v13 / v14),
        v9 * 1024,
        load[0] * 1024,
        load[1] * 1024,
        g_string_container->stats() * 1024);
    if (output_debug_string)
    {
        v15 = *((_QWORD*)&pools.data.align + 3);
        v17 = (float)SLODWORD(load[0]);
        if ((load[0] & 0x8000000000000000uLL) != 0LL)
            v17 = v17 + 1.8446744e19;
        v19 = v17 * 100.0;
        v20 = (float)(int)v15;
        if (v15 < 0)
            v20 = v20 + 1.8446744e19;

        string_path str; // [rsp+60h] [rbp-148h] BYREFy
        sprintf_s(
            str,
            sizeof(str),
            "- pool (%10s): %2.1f load : %dK total, %dK used, %dK free, %dK largest block, %dK str_shared\n",
            pools.front().desc,
            (float)(v19 / v20),
            (unsigned __int64)v15 >> 10,
            load[0] >> 10,
            (v15 - load[0]) >> 10,
            load[1] >> 10,
            g_string_container->stats() * 1024);
        OutputDebugStringA(str);
    }
}

u64 u_memory::_usage(u32* p_blocks_used, u32* p_blocks_free)
{
    if (g_string_container)
        g_string_container->integrity();

    if (g_shared_memory_container)
        g_shared_memory_container->integrity();

    R_ASSERT(_HEAPOK==_heapchk());
    return 0;
}

u64 u_memory::_usage_2(u64* total, u64* largest_block)
{

    if (g_string_container)
        g_string_container->clean();

    if (g_shared_memory_container)
        g_shared_memory_container->clean();

    _heapmin();
    HeapCompact(GetProcessHeap(), 0);
    RegFlushKey(HKEY_CLASSES_ROOT);
    RegFlushKey(HKEY_CURRENT_USER);
    SetProcessWorkingSetSize(GetCurrentProcess(), 0xFFFFFFFFFFFFFFFFuLL, 0xFFFFFFFFFFFFFFFFuLL);

    u64 load[5];
    load[0] = 0;
    load[1] = 0;

    lock();
    tlsf_walk_pool(pools.front().pool, 0, load);
    unlock();

    if (total)
        *total = pools.front().memsize;

    if (largest_block)
        *largest_block = load[1];

    return load[0];
}