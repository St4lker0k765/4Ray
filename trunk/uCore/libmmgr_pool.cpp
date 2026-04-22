#include "stdafx.h"
#include "libmmgr.h"
#include "tlsf.h"

u64 u_memory::pool_block_size(void* ptr)
{
    return tlsf_block_size(ptr);
}

void* u_memory::pool_create(const char* desc, u64 size, void* memblk)
{
    lock();
    u_memory::poolreg r;
    r.desc = desc;
    r.memsize = size;
    r.memblock = memblk;
    if (!memblk)
    {
        if (size > 0x4000000)
        {
            while (true)
            {
                r.memblock = HeapAlloc(GetProcessHeap(), 0, r.memsize);
                R_ASSERT2(r.memblock, "download more ram");
                if ((r.memsize & 0xFFFFFFFF00000000uLL) != 0)
                    break;
            }
        }
        else
        {
            r.memblock = tlsf_memalign(r.pool, 0x80u, r.memsize);
        }
    }
    r.pool = tlsf_create_with_pool(r.memblock, r.memsize);
    pools.push_back(r);
    unlock();
    return r.pool;
}

void u_memory::pool_destroy(void* pool)
{
    if (pools.size())
    {
        u_memory::poolreg poolTarget;
        for (u32 i = 0; i < pools.size(); i++)
        {
            if (pool == pools[i].pool)
            {
                poolTarget = pools[i];
                break;
            }
        }
        tlsf_destroy(poolTarget.pool);
        if (poolTarget.memsize > 0x4000000)
        {
            HeapFree(GetProcessHeap(), 0, poolTarget.memblock);
        }
        else
        {
            u_memory::poolreg reg_front = pools.front();
            tlsf_free(reg_front.pool, reg_front.memblock);
        }
        pools.erase(&poolTarget);
    }
}

char* u_memory::pool_realloc(void* pool, char* p, size_t size, u64 align, const char* _, bool allow_out_of_memory)
{
    u64 current_size = 0; // r14
    u64 current_mblock = 0;
    threading::spin_lock current_lock = nullptr;
    for (int i = 0; i < pools.size(); i++)
    {
        u_memory::poolreg v16 = pools[i];
        if (v16.pool == pool)
        {
            current_size = v16.memsize;
            current_lock = v16.lock;
            current_mblock = (u64)v16.memblock;
            break;
        }
    }
    threading::spin_lock pa = current_lock;
    if (p)
    {
        R_ASSERT(uintptr_t(p)>=current_mblock);
        R_ASSERT(uintptr_t(p)<(current_mblock+current_size));
    }
    R_ASSERT(&current_lock);
    if (size)
    {
        if ((size & 0xF) != 0)
            align = 4;
        if (p)
        {
            size_t v21 = tlsf_block_size(p);
            if (size >= v21)
            {
                char* v22; // rax
                if (align < 0x10)
                    v22 = (char*)tlsf_malloc(pool, size);
                else
                    v22 = (char*)tlsf_memalign(pool, align, size);
                if (debug)
                    debug->_alloc(v22, size, _);
                if (size < v21)
                    v21 = size;
                memcpy(v22, p, v21);
                if (debug)
                    debug->_free(p);
                tlsf_free(pool, p);
                p = v22;
            }
        }
        else
        {
            char* result; // rax
            if (align < 0x10)
                result = (char*)tlsf_malloc(pool, size);
            else
                result = (char*)tlsf_memalign(pool, align, size);
            p = result;
            if (!result)
            {
                if (allow_out_of_memory)
                {
                    pa = 0;
                    return result;
                }
                _stats(0, 0);
                Debug.fatal("! out of memory");
            }
            if (debug)
            {
                debug->_alloc(p, size, _);
                pa = 0;
                return p;
            }
        }
        pa = 0;
        return p;
    }
    else
    {
        if (p)
        {
            if (debug)
                debug->_free(p);
            tlsf_free(pool, p);
        }
        current_lock = 0;
        return 0;
    }
}

void u_memory::pool_validate(void* pool)
{
    lock();
    tlsf_check_pool(pool);
    unlock();
}

void u_memory::pools0info(void* memblock, u64* memsize)
{
    memblock = pools.front().memblock;
    *memsize = pools.front().memsize;
}