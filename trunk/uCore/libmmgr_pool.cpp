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
    u_svector<u_memory::poolreg, 6, unsigned short>* p_pools; // r14
    unsigned __int8 v14; // r12
    char* v16; // rdi
    u64 current_size; // r14
    char* result; // rax
    size_t v21; // rdi
    char* v22; // rax
    char* v24; // rbx

    p_pools = &this->pools;
    PDWORD v9 = 0;
    PDWORD pa = 0;
    u64 current_mblock = 0;
    v14 = 0;
    if (!uvector_base<40, 8, svector_base<40, 8, 6, unsigned short>>::size(&this->pools))
    {
    LABEL_24:
        current_size = 0;
        goto LABEL_25;
    }
    
    if (pools.size())
    {

    }


    while (1)
    {
        v16 = &p_pools->data.storage[(unsigned int)(40 * current_mblock)];
        if (*((void**)v16 + 1) != pool)
            goto LABEL_22;
        if (!_InterlockedCompareExchange((volatile signed __int32*)v16 + 8, -1, 0))
            break;
        Debug.fatal(" !!! pool_realloc() threading check fail !!!");
    LABEL_22:
        current_mblock = ++v14;
        if (v14 >= uvector_base<40, 8, svector_base<40, 8, 6, unsigned short>>::size(p_pools))
        {
            v9 = 0;
            current_mblock = 0;
            goto LABEL_24;
        }
    }
    current_size = *((_QWORD*)v16 + 3);
    v9 = v16 + 32;
    current_mblock = *((_QWORD*)v16 + 2);
    pa = v9;
LABEL_25:
    if (p)
    {
        R_ASSERT(uintptr_t(p)>=current_mblock);
        R_ASSERT(uintptr_t(p)<(current_mblock+current_size));
    }
    if (!v9)
        debug::fail("current_lock", "libmmgr_pool.cpp", "u_memory::pool_realloc", 399);
    if (size)
    {
        if ((size & 0xF) != 0)
            align = 4;
        if (p)
        {
            v21 = tlsf_block_size(p);
            if (size >= v21)
            {
                if (align < 0x10)
                    v22 = (char*)tlsf_malloc(pool, size);
                else
                    v22 = (char*)tlsf_memalign(pool, align, size);
                v24 = v22;
                if (debug)
                    debug->_alloc(v22, size, _);
                if ((unsigned int)size < (unsigned int)v21)
                    v21 = (unsigned int)size;
                memcpy(v24, p, v21);
                if (debug)
                    debug->_free(p);
                tlsf_free(pool, p);
                p = v24;
            }
        }
        else
        {
            if (align < 0x10)
                result = (char*)tlsf_malloc(pool, size);
            else
                result = (char*)tlsf_memalign(pool, align, size);
            p = result;
            if (!result)
            {
                if (allow_out_of_memory)
                {
                    *pa = 0;
                    return result;
                }
                _stats(0, 0);
                Debug.fatal("! out of memory");
            }
            if (debug)
            {
                debug->_alloc(p, size, _);
                *pa = 0;
                return p;
            }
        }
        *pa = 0;
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
        *v9 = 0;
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