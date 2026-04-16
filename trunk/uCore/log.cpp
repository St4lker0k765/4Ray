#include "stdafx.h"
#include "log.h"

void slog(const char* s)
{
    string4096 split;

    u64 v1 = 0;
    u64 v2 = 0;
    for (const char* i = s; *i; ++i)
    {
        s = i;
        if ((BYTE)s == 10)
        {
            split[v1] = 0;
            if (!split[0])
                strcpy(split, " ");
            Log->add(split);
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
    split[(int)v2] = 0;
    Log->add(split);
}

void rlog(const char* format, ...)
{
    string1024 buf{};
    va_list va;

    va_start(va, format);
    int v1 = vsnprintf(buf, sizeof(buf), format, va);
    if (v1 >= 0)
    {
        slog(buf);
    }
    else
    {
        u_string str;
        str.vset(format, va);
        slog(str.c_str());
    }
}


static int day_in_month[12] = 
{
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static LPCSTR month[12] = 
{
    "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

static int start_year = 2005;	// 2005

u64 build_number()
{
    int v0; // esi
    int v1; // ebp
    int v2; // ebx
    int v3; // edi
    const char** v4; // r14
    u64 v6; // rcx
    unsigned int v7; // eax
    __m128i v8; // xmm2
    __m128i v9; // xmm1
    int v10; // edx
    __int64 v11; // rax
    __m128i v12; // xmm0
    __m128i v13; // xmm1
    __m128i v14; // xmm1
    int v15; // edx
    int ynum; // [rsp+30h] [rbp-B8h] BYREF
    int dnum; // [rsp+34h] [rbp-B4h] BYREF

    v0 = 0;
    v1 = 0;
    v2 = 0;
    string128 buf; //
    R_ASSERT(!strcpy_s(buf, sizeof(buf), __DATE__));

    string16 mon;
    sscanf(buf, "%s %d %d", mon, &dnum, &ynum);
    v3 = 0;
    v4 = month;
    for (int i = 0; i < 12; i++)
    {
        if (_stricmp(month[i], mon))
            continue;
        months = i;
        break;
    }
    LODWORD(v6) = 6;
    v7 = v2 - 6;
    if (v2 > 6 && v7 >= 8)
    {
        v8 = 0;
        v9 = 0;
        v10 = v2 - (int)v7 % 8;
        do
        {
            v11 = (int)v6;
            LODWORD(v6) = v6 + 8;
            v8 = _mm_add_epi32(_mm_loadu_si128((const __m128i*) & day_in_month[v11]), v8);
            v12 = _mm_add_epi32(_mm_loadu_si128((const __m128i*) & day_in_month[v11 + 4]), v9);
            v9 = v12;
        } while ((int)v6 < v10);
        v13 = _mm_add_epi32(v12, v8);
        v14 = _mm_add_epi32(v13, _mm_srli_si128(v13, 8));
        v1 = _mm_cvtsi128_si32(_mm_add_epi32(v14, _mm_srli_si128(v14, 4)));
    }
    v6 = (int)v6;
    v15 = 0;
    if ((int)v6 < (__int64)v2)
    {
        if (v2 - (__int64)(int)v6 >= 2)
        {
            do
            {
                v0 += day_in_month[v6];
                v15 += day_in_month[v6 + 1];
                v6 += 2;
            } while (v6 < v2 - 1LL);
        }
        if (v6 < v2)
            v1 += day_in_month[v6];
        v1 += v15 + v0;
    }
    return v1 + dnum + 365 * (ynum - start_year);

    int build_id = days + 365 * (ynum - start_year);

    for (int i = 0; i < months; ++i)
        build_id += day_in_month[i];

    for (int i = 0; i < start_month - 1; ++i)
        build_id -= day_in_month[i];
}

logger::logger()
{
    fname._set("");
    strings.clear();
    callbacks.clear();

    MTX = new threading::mutex("log");
#pragma todo("Implement params for uCore")
    this->flush_forced = false; //strstr(core.params, "-forcelog ");
}

logger::~logger()
{
    callbacks.clear();
    strings.clear();
    delete MTX;
}

void logger::add(const char* split)
{
    logger* zero_len_str; // rbx
    str_container* v5; // rcx
    __int64 v6; // rax
    unsigned int v9; // r12d
    threading_rw_check* v10; // rbx
    void(__fastcall * *v11)(const char*); // rdi

    OutputDebugStringA(split);
    OutputDebugStringA("\n");
    MTX->lock();
    if (split)
    {
        if (*split)
        {
            v6 = -1;
            do
                ++v6;
            while (split[v6]);
            zero_len_str = (logger*)g_string_container->do_dock(split, v6, 0);
        }
        else
        {
            v5 = g_string_container;
            _InterlockedIncrement(&g_string_container->zero_len_str->refs);
            zero_len_str = (logger*)v5->zero_len_str;
        }
    }
    else
    {
        zero_len_str = 0;
    }
    MTX->unlock();
    u32 id = 0;
    if (callbacks.size())
    {
        v9 = 0;
        do
        {
            if (this == (logger*)-88LL)
            {
                v10 = 0;
            }
            else
            {
                v10 = &this->callbacks.threading_rw_check;
                if (this != (logger*)-104LL)
                {
                    _InterlockedIncrement(&v10->_state.readers);
                    if (this->callbacks._state.writers)
                    {
                        while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                            ;
                        __debugbreak();
                        if (this->callbacks._state.writers)
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
            R_ASSERT(id < callbacks.size());
            v11 = callbacks[v9];
            if (v10)
            {
                if (v10->_state.writers)
                {
                    while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                        ;
                    __debugbreak();
                    if (v10->_state.writers)
                        debug::fail(
                            "0 == _t->_state.writers",
                            "read exit when writing is active",
                            "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                            "threading_rw_check::read_lock::~read_lock",
                            81);
                    threading::thread_check_lock._lock = 0;
                }
                _InterlockedDecrement(&v10->_state.readers);
            }
            (*v11)(split);
            ++id;
            v9 += sizeof(void*);
        } while (id < callbacks.size());
    }
    if (this->flush_forced)
        flush_to_hdd();
}

void logger::flush_to_hdd()
{
    const char* buf; // rbx
    int v4; // eax
    str_value* v5; // rdx
    const char* value; // rdx
    unsigned int v7; // r15d
    unsigned int v8; // r12d
    threading_rw_check* v9; // rbx
    _QWORD* v10; // rsi
    __int64 v11; // rax
    const char* v12; // rdx
    vfs::iwriter* object; // rbx
    int v14; // eax
    vfs::iwriter* v15; // rcx
    gtl::intrusive_ptr<vfs::iwriter, gtl::intrusive_base, gtl::intrusive_default_functionality, gtl::thread_safe_inc_dec<gtl::intrusive_base> > f; // [rsp+60h] [rbp+8h] BYREF

    str_value* p = this->fname.p_;
    if (fname.size())
    {
        if (!flush_forced)
        {
            rlog("log flushed to [%s]", fname.c_str());
            rlog("%s build %d[SVN=%s], %s, %s\n", "uCore :: 4A platform", build_number(), core.game_version.c_str(), __DATE__, __TIME__);
        }
        MTX->lock();
        v5 = this->fname.p_;
        if (v5)
            value = v5->value;
        else
            value = 0;
        vfs::wopen_os(&f, value);
        if (f._object)
        {
            v7 = 0;
            if (strings.size())
            {
                v8 = 0;
                do
                {
                    if (this == (logger*)-56LL)
                    {
                        v9 = 0;
                    }
                    else
                    {
                        v9 = &this->strings.threading_rw_check;
                        if (this != (logger*)-72LL)
                        {
                            _InterlockedIncrement(&v9->_state.readers);
                            if (this->strings._state.writers)
                            {
                                while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                                    ;
                                __debugbreak();
                                if (this->strings._state.writers)
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
                    if (v7 >= this->strings._size)
                        debug::fail(
                            "id < count()",
                            "d:\\trunk\\src\\ucore\\u_vector_base.h",
                            "uvector_base<8,8,struct vector_base<class allocator_t<8,8>,unsigned int> >::unsafe_at",
                            76);
                    v10 = (char*)this->strings._array + v8;
                    if (v9)
                    {
                        if (v9->_state.writers)
                        {
                            while (_InterlockedCompareExchange(&threading::thread_check_lock._lock, -1, 0))
                                ;
                            __debugbreak();
                            if (v9->_state.writers)
                                debug::fail(
                                    "0 == _t->_state.writers",
                                    "read exit when writing is active",
                                    "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                                    "threading_rw_check::read_lock::~read_lock",
                                    81);
                            threading::thread_check_lock._lock = 0;
                        }
                        _InterlockedDecrement(&v9->_state.readers);
                    }
                    if (*v10)
                        v11 = *v10 + 20LL;
                    else
                        v11 = 0;
                    v12 = def;
                    if (v11)
                        v12 = (const char*)v11;
                    vfs::writer_base_t<vfs::iwriter>::w_string(&f._object->vfs::writer_base_t<vfs::iwriter>, v12);
                    ++v7;
                    v8 += 8;
                } while (v7 < uvector_base<8, 8, vector_base<allocator_t<8, 8>, unsigned int>>::size(&this->strings));
            }
            object = f._object;
            if (f._object)
            {
                if (f._object->_ref_count <= 0)
                    debug::fail(
                        "0 < *(s32 *)&(o->base_type::_ref_count)",
                        "d:\\trunk\\src\\ucore\\gtl/intrusive_ptr.h",
                        "gtl::thread_safe_inc_dec<struct gtl::intrusive_base>::dec",
                        271);
                if (!_InterlockedDecrement(&object->_ref_count))
                {
                    v14 = gtl::thread_safe_inc_dec<gtl::intrusive_base>::dec_cold<vfs::iwriter>(&f, object);
                    v15 = f._object;
                    if (v14)
                        v15 = 0;
                    f._object = v15;
                }
            }
        }
        MTX->unlock();
    }
}
logger* Log = nullptr;