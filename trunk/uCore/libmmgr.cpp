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
    if (g_string_container)
        g_string_container->clean();

    if (g_shared_memory_container)
        g_shared_memory_container->clean();

    _heapmin();
    HeapCompact(GetProcessHeap(), 0);
    RegFlushKey(HKEY_CLASSES_ROOT);
    RegFlushKey(HKEY_CURRENT_USER);
    SetProcessWorkingSetSize(GetCurrentProcess(),  0xFFFFFFFFFFFFFFFFuLL, 0xFFFFFFFFFFFFFFFFuLL);

    u64 load[2];
    load[0] = 0;
    load[1] = 0;
    lock();
    tlsf_walk_pool(pools.front().pool, 0, load);
    unlock();

    u64 size = pools.front().memsize;
    rlog(
        "- pool (%10s): %2.1f load : %dK total, %dK used, %dK largest block, %dK str_shared",
        pools.front().desc,
        (float)(load[0] * 100 / size),
        size * 1024,
        load[0] * 1024,
        load[1] * 1024,
        g_string_container->stats() * 1024);
    if (output_debug_string)
    {
        string_path str;
        sprintf_s(
            str,
            sizeof(str),
            "- pool (%10s): %2.1f load : %dK total, %dK used, %dK free, %dK largest block, %dK str_shared\n",
            pools.front().desc,
            (float)(load[0] * 100.0 / size),
            size * 1024,
            load[0] * 1024,
            (size - load[0]) * 1024,
            load[1] * 1024,
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

volatile void* xmem_pool = nullptr;
threading::mutex xmem_lock;

#define XMEM_SIZE 0x8000000u
void u_memory::xmem_enable()
{
    xmem_lock.lock();
    if (!xmem_pool)
    {
        void* alloc = main_realloc(0, XMEM_SIZE, 0x10u, "xmem", 0);
        if (alloc)
            xmem_pool = pool_create("xmem", XMEM_SIZE, alloc);
    }
    xmem_lock.unlock();
}

void u_memory::xmem_free(void* ptr, const char* _)
{
    xmem_realloc((char*)ptr, 0, 0, _);
}

char* u_memory::xmem_realloc_aligned(char* ptr, u64 size, const char* _)
{
    return xmem_realloc(ptr, (size + 63) & 0xFFFFFFFFFFFFFFC0uLL, 0x40u, _);
}

char* u_memory::xmem_realloc(
    char* ptr,
    u64 size,
    u64 align,
    const char* _)
{
    u64 v6 = size;
    char* v7 = ptr;
    bool v9 = ptr >= xmem_pool && ptr <= (char*)xmem_pool + XMEM_SIZE;
    if (!xmem_pool)
        return (char*)main_realloc(ptr, size, align, _, 0);
    if (!size)
    {
        if (v9)
        {
            xmem_lock.lock();
            v7 = pool_realloc(&xmem_pool, v7, 0, align, _, 0);
            xmem_lock.unlock();
            return v7;
        }
        size = 0;
        return (char*)main_realloc(ptr, size, align, _, 0);
    }
    if (ptr)
    {
        int v14 = mblock_size(ptr, 0);
        if (v14 >= v6)
            return v7;
        char* ptra = (char*)xmem_realloc(0, v6, align, _);
        u32 v15 = v6 + ((v14 - v6) & ((v14 - (int)v6) >> 31));
        memcpy(ptra, v7, v15);
        xmem_realloc(v7, 0, 0, _);
        return ptra;
    }
    else
    {
        lock();
        char* v11 = pool_realloc(&xmem_pool, 0, v6, align, _, 1);
        unlock();
        if (v11)
        {
            return v11;
        }
        else
        {
            lock();
            char* v13 = pool_realloc(pools.front().pool, 0, v6, align, _, 1);
            unlock();
            return v13;
        }
    }
}