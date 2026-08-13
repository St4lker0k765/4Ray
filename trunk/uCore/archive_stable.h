#pragma once
#include "gtl/intrusive_ptr.h"

#define chunk_data_id 1
#define chunk_stable 2
#define WHASH_SIZE 61

class archive_stable : public intrusive_base
{
protected:
	u32 windex = 0;
	u_map<str_shared, u32> whash[WHASH_SIZE];
	u_vector<str_shared> rtable;
public:
    void check_end()
    {
        R_ASSERT2(rtable.empty(), "call read_end first");
        R_ASSERT2(!windex, "call write_end first");
    }
	u64 crc2crc(const u32 crc)
	{
		return crc | HIWORD(crc);
	}
    const str_shared read(u32 id)
    {
        return rtable[id];
    }
    bool read_begin(vfs::ireader r)
    {
        check_end();
        if (r.length() < 20)
        {
            return false;
        }

        u32 pos = r.tell();
        u32 data_size;
        if (!r.chunk_try_open_at_current_position(chunk_data_id, &data_size)
            || data_size + pos >= r.length() - 12)
        {
            r.seek(pos);
            return false;
        }
        r.advance(data_size);
        u32 stable_size;
        if (r.chunk_try_open_at_current_position(chunk_stable, &stable_size))
        {
            rtable.resize(r.r_u32());
            for (int i = 0; i < rtable.size(); i++)
            {
                r.r_stringz(rtable[i], default_string);
            }
            r.seek(pos);
            R_ASSERT(r.chunk_try_open_at_current_position(chunk_data_id, &data_size));
            return true;
        }
        else
        {
            r.seek(pos);
            return false;
        }
    }
    void read_end(vfs::ireader* r)
    {
        rtable.clear();
    }
    size_t write(const str_shared s)
    {
        u32 crc = crc2crc(s._get()->crc) % WHASH_SIZE;
        if (!whash[crc].size())
        {
            windex++;
            whash[crc].emplace(s, windex);
            return windex;
        }
        int i = 0;
        for (; i < whash[crc].size(); i++)
        {
            if (i >= whash[crc].size())
            {
                windex++;
                whash[crc].emplace(s, windex);
                return windex;
            }
        }
        return 0;
    }
    void write_begin(vfs::iwriter w)
    {
        check_end();
        w.chunk_open(chunk_data_id);
    }
    void write_end(vfs::iwriter* w)
    {
        u_vector<pair<str_shared, unsigned int>, allocator_t, unsigned short>* v5; // rbx
        unsigned int v6; // edi
        __int64 v7; // rsi
        unsigned int v8; // eax
        const char* v9; // rax
        threading_rw_check* v10; // rdx
        const char* dbg_info; // rbx
        u_vector<pair<str_shared, unsigned int>, allocator_t, unsigned short>* v12; // r14
        __int64 v13; // r13
        u_vector<pair<str_shared, unsigned int>, allocator_t, unsigned short>* v14; // r12
        const pair<str_shared, unsigned int>* v15; // rbp
        const pair<str_shared, unsigned int>* v16; // rbx
        const char* v17; // rax
        const char* v18; // rax
        const threading_rw_check* v19; // rdx
        const char* v20; // r15
        char* array; // rax
        __int64 v22; // rdx
        void* v23; // rbp
        const char* v24; // rbx
        u_memory* v25; // rax
        const threading_rw_check* t; // rbx
        uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short> >* v27; // r14
        _BYTE* v28; // rbx
        pair<str_shared, unsigned int>* v29; // rax
        unsigned int v30; // eax
        vfs::iwriter* v31; // r13
        threading_rw_check* v32; // rsi
        threading_rw_check* v33; // rcx
        unsigned int v34; // ebp
        unsigned int v35; // r12d
        volatile signed __int32* p_readers; // rbx
        __int64 v37; // r15
        char* v38; // rdi
        char* v39; // rdi
        __int64 v40; // rax
        __int64 v41; // r8
        const char* v42; // rdx
        threading_rw_check* v43; // rcx
        threading_rw_check* v44; // rcx
        const char* v45; // rax
        const threading_rw_check* v46; // rdx
        const char* v47; // rdi
        char* v48; // rax
        __int64 v49; // rdx
        void* v50; // rbx
        const char* v51; // rsi
        u_memory* v52; // rax
        const threading_rw_check* v53; // rbx
        unsigned int v54; // [rsp+38h] [rbp-80h] BYREF
        threading_rw_check::write_lock v55; // [rsp+40h] [rbp-78h] BYREF
        threading_rw_check::write_lock v56; // [rsp+48h] [rbp-70h] BYREF
        threading_rw_check::write_lock v58; // [rsp+58h] [rbp-60h] BYREF
        vfs::writer_base_t<vfs::iwriter>* v59; // [rsp+60h] [rbp-58h]
        vfs::iwriter* wa; // [rsp+C8h] [rbp+10h] BYREF
        char v62; // [rsp+D0h] [rbp+18h] BYREF
        char v63; // [rsp+D8h] [rbp+20h]

        v59 = &w->vfs::writer_base_t<vfs::iwriter>;
        vfs::writer_base_t<vfs::iwriter>::chunk_close(&w->vfs::writer_base_t<vfs::iwriter>);
        w->chunk_open(chunk_stable);
        v5 = this->whash;
        v6 = 0;
        v7 = 61;
        do
        {
            v8 = uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short>>::size(v5++);
            v6 += v8;
            --v7;
        } while (v7);
        v9 = type_info::_name_internal_method(
            (type_info*)&pair<str_shared, unsigned int> `RTTI Type Descriptor',
            & __type_info_root_node);
        v10 = &this->whash[0].threading_rw_check;
        dbg_info = v9;
        if (this == (archive_stable*)-24LL)
            v10 = nullptr;
        threading_rw_check::write_lock::write_lock(&v58, v10);
        v63 = 0;
        uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short>>::inner_try_reserve<1, 0>(
            &this->whash[0],
            v6,
            nullptr,
            0,
            dbg_info);
        threading_rw_check::write_lock::~write_lock(&v58);
        v12 = &this->whash[1];
        v13 = 60;
        v14 = &this->whash[1];
        do
        {
            v15 = (const pair<str_shared, unsigned int> *)uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short>>::end(v14);
            v16 = (const pair<str_shared, unsigned int> *)uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short>>::begin(v14);
            v17 = type_info::_name_internal_method(
                (type_info*)&pair<str_shared, unsigned int> `RTTI Type Descriptor',
                & __type_info_root_node);
            uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short>>::append<pair<str_shared, unsigned int>, _nonrelocable_ptr__tag>(
                whash,
                v16,
                v15,
                (_nonscalar_ptr__tag)this,
                (_nonrelocable_ptr__tag)this,
                v17);
            v18 = type_info::_name_internal_method(
                (type_info*)&pair<str_shared, unsigned int> `RTTI Type Descriptor',
                & __type_info_root_node);
            v19 = &v12->threading_rw_check;
            v20 = v18;
            if (!v12)
                v19 = nullptr;
            threading_rw_check::write_lock::write_lock(&v56, v19);
            array = (char*)v12->_array;
            v22 = (__int64)v12->_array + 16 * v12->_size;
            if (v12->_array != (void*)v22)
            {
                do
                {
                    if (*(_QWORD*)array)
                    {
                        _InterlockedDecrement((volatile signed __int32*)(*(_QWORD*)array + 8LL));
                        *(_QWORD*)array = 0;
                    }
                    array += 16;
                } while (array != (char*)v22);
            }
            v23 = v12->_array;
            v12->_size = 0;
            if (v23)
            {
                v24 = "eastl";
                if (v20)
                    v24 = v20;
                v25 = memory();
                u_memory::main_realloc(v25, v23, 0, 0, v24, 0);
            }
            v12->_array = nullptr;
            v12->_capacity = 0;
            t = v56._t;
            if (v56._t)
            {
                if (v56._t->_state.readers)
                {
                    while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                        ;
                    __debugbreak();
                    t = v56._t;
                    if (v56._t->_state.readers)
                        debug::fail(
                            "0 == _t->_state.readers",
                            "write exit with reader active",
                            "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                            "threading_rw_check::write_lock::~write_lock",
                            120);
                    threading::thread_check_lock._lock = 0;
                }
                if (_InterlockedExchangeAdd(&t->_state.writers, 0xFFFFFFFF) != 1)
                {
                    while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                        ;
                    __debugbreak();
                    debug::fail(
                        "1 == initial_writers",
                        "write unlock: more than one writer detected",
                        "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                        "threading_rw_check::write_lock::~write_lock",
                        125);
                    threading::thread_check_lock._lock = 0;
                }
            }
            whash = (u_vector<pair<str_shared, unsigned int>, allocator_t, unsigned short> *)v57;
            ++v14;
            ++v12;
            --v13;
        } while (v13);
        v27 = whash;
        LOBYTE(wa) = 0;
        v28 = whash.end();
        v29 = (pair<str_shared, unsigned int> *)uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short>>::begin(whash);
        _shell_sort<pair<str_shared, unsigned int>, __int64, archive_stable::sort_by_index>(
            v29,
            (v28 - (_BYTE*)v29) >> 4,
            (const archive_stable::sort_by_index*)&wa);
        v30 = uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short>>::size(whash);
        v31 = v59;
        v32 = nullptr;
        v33 = &w->chunk_pos.c.threading_rw_check;
        if (!v59)
            v33 = nullptr;
        v54 = v30;
        (*(void(__fastcall**)(threading_rw_check*, unsigned int*, __int64))(*(_QWORD*)&v33->_state.readers + 32LL))(
            v33,
            &v54,
            4);
        v34 = 0;
        v35 = uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short>>::size(whash);
        if (v35)
        {
            p_readers = &whash->_state.readers;
            v37 = 0;
            do
            {
                if (v27)
                {
                    v32 = &v27->threading_rw_check;
                    if (v27 != (uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short> > *) - 16LL)
                    {
                        _InterlockedIncrement(p_readers);
                        if (v27->_state.writers)
                        {
                            while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                                ;
                            __debugbreak();
                            if (v27->_state.writers)
                                debug::fail(
                                    "0 == _t->_state.writers",
                                    "cannot read when writing is active",
                                    "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                                    "threading_rw_check::read_lock::read_lock",
                                    72);
                            threading::thread_check_lock._lock = 0;
                        }
                    }
                }
                if (v34 >= v27->_size)
                    debug::fail(
                        "id < count()",
                        "d:\\trunk\\src\\ucore\\u_vector_base.h",
                        "uvector_base<16,8,struct vector_base<class allocator_t<16,8>,unsigned short> >::unsafe_at",
                        76);
                v38 = (char*)v27->_array;
                if (v32)
                {
                    if (v32->_state.writers)
                    {
                        while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                            ;
                        __debugbreak();
                        if (v32->_state.writers)
                            debug::fail(
                                "0 == _t->_state.writers",
                                "read exit when writing is active",
                                "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                                "threading_rw_check::read_lock::~read_lock",
                                81);
                        threading::thread_check_lock._lock = 0;
                    }
                    _InterlockedDecrement(&v32->_state.readers);
                }
                if (v34 != *(_DWORD*)&v38[v37 + 8])
                    debug::fail(
                        "it == wtable[it].second",
                        "d:\\trunk\\src\\ucore\\archive_stable.h",
                        "archive_stable::write_end",
                        81);
                if (v27 == (uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short> > *) - 16LL
                    || (_InterlockedIncrement(p_readers), !v27->_state.writers))
                {
                    v32 = nullptr;
                }
                else
                {
                    while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                        ;
                    __debugbreak();
                    if (v27->_state.writers)
                        debug::fail(
                            "0 == _t->_state.writers",
                            "cannot read when writing is active",
                            "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                            "threading_rw_check::read_lock::read_lock",
                            72);
                    v32 = nullptr;
                    threading::thread_check_lock._lock = 0;
                }
                if (v34 >= v27->_size)
                    debug::fail(
                        "id < count()",
                        "d:\\trunk\\src\\ucore\\u_vector_base.h",
                        "uvector_base<16,8,struct vector_base<class allocator_t<16,8>,unsigned short> >::unsafe_at",
                        76);
                v39 = (char*)v27->_array;
                if (v27 != (uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short> > *) - 16LL)
                {
                    if (v27->_state.writers)
                    {
                        while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                            ;
                        __debugbreak();
                        if (v27->_state.writers)
                            debug::fail(
                                "0 == _t->_state.writers",
                                "read exit when writing is active",
                                "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                                "threading_rw_check::read_lock::~read_lock",
                                81);
                        threading::thread_check_lock._lock = 0;
                    }
                    _InterlockedDecrement(p_readers);
                }
                v40 = *(_QWORD*)&v39[v37];
                if (v40)
                    v41 = *(unsigned __int16*)(v40 + 12);
                else
                    v41 = 0;
                v42 = (const char*)(v40 + 20);
                if (!v40)
                    v42 = def;
                v43 = &v31[-1].chunk_pos.c.threading_rw_check;
                if (!v31)
                    v43 = nullptr;
                (*(void(__fastcall**)(threading_rw_check*, const char*, __int64))(*(_QWORD*)&v43->_state.readers + 32LL))(
                    v43,
                    v42,
                    v41);
                v62 = 0;
                v44 = &v31[-1].chunk_pos.c.threading_rw_check;
                if (!v31)
                    v44 = nullptr;
                (*(void(__fastcall**)(threading_rw_check*, char*, __int64))(*(_QWORD*)&v44->_state.readers + 32LL))(
                    v44,
                    &v62,
                    1);
                ++v34;
                v37 += 16;
            } while (v34 < v35);
        }
        v45 = type_info::_name_internal_method(
            (type_info*)&pair<str_shared, unsigned int> `RTTI Type Descriptor',
            & __type_info_root_node);
        v46 = &v27->threading_rw_check;
        v47 = v45;
        if (!v27)
            v46 = nullptr;
        threading_rw_check::write_lock::write_lock(&v55, v46);
        v48 = (char*)v27->_array;
        v49 = (__int64)v27->_array + 16 * v27->_size;
        if (v27->_array != (void*)v49)
        {
            do
            {
                if (*(_QWORD*)v48)
                {
                    _InterlockedDecrement((volatile signed __int32*)(*(_QWORD*)v48 + 8LL));
                    *(_QWORD*)v48 = 0;
                }
                v48 += 16;
            } while (v48 != (char*)v49);
        }
        v50 = v27->_array;
        v27->_size = 0;
        if (v50)
        {
            v51 = "eastl";
            if (v47)
                v51 = v47;
            v52 = memory();
            u_memory::main_realloc(v52, v50, 0, 0, v51, 0);
        }
        v53 = v55._t;
        v27->_array = nullptr;
        v27->_capacity = 0;
        if (v53)
        {
            if (v53->_state.readers)
            {
                while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                    ;
                __debugbreak();
                v53 = v55._t;
                if (v55._t->_state.readers)
                    debug::fail(
                        "0 == _t->_state.readers",
                        "write exit with reader active",
                        "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                        "threading_rw_check::write_lock::~write_lock",
                        120);
                threading::thread_check_lock._lock = 0;
            }
            if (_InterlockedExchangeAdd(&v53->_state.writers, 0xFFFFFFFF) != 1)
            {
                while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                    ;
                __debugbreak();
                debug::fail(
                    "1 == initial_writers",
                    "write unlock: more than one writer detected",
                    "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                    "threading_rw_check::write_lock::~write_lock",
                    125);
                threading::thread_check_lock._lock = 0;
            }
        }
        windex = 0;
        v31->chunk_close();
    }
};