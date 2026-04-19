#include "stdafx.h"
#include "log.h"

UCORE_API string_path log_fname;

void flusher(const char* s)
{
    Log->flush_to_hdd();
}

void log_add_callback(LogCallback callback)
{
    Log->callbacks.push_back(callback);
}

void log_create(bool no_log)
{
    log_fname[0] = 0;
    if (!no_log)
    {
        LPCSTR l_path = strstr(core.params(), "-logpath");
        if (l_path)
        {
            sscanf(l_path + 8, "%s", log_fname);
        }
        else
        {
            strcpy_s(log_fname, sizeof(log_fname), core.application_name.c_str());
            strcat_s(log_fname, sizeof(log_fname), "_");
            strcat_s(log_fname, sizeof(log_fname), core.user_name.c_str());
            strcat_s(log_fname, sizeof(log_fname), ".log");
        }
        Log->fname = log_fname;
    }

    if (strstr(core.params(), "-logflush"))
        log_add_callback(flusher);

    if (!no_log)
    {
        rlog("%s build %d, %s, %s\n", "uCore :: 4A platform", build_number(), "Aug  6 2014", "18:20:42");
    }
}

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


static int days_in_month[12] = 
{
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static LPCSTR month_id[12] = 
{
    "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

static int start_day = 30;	    // 30
static int start_month = 6;	    // June
static int start_year = 2005;	// 2005

u32 build_number()
{
	int days;
	int months = 0;
	int years;

	string16 month;

	string256 buffer;
	strcpy_s(buffer, sizeof(buffer), __DATE__);

	sscanf(buffer, "%s %d %d", month, &days, &years);

	for (int i = 0; i < 12; i++)
	{
		if (_stricmp(month_id[i], month))
			continue;

		months = i;
		break;
	}

	int build_id = (years - start_year) * 365 + days - start_day;

	for (int i = 0; i < months; ++i)
		build_id += days_in_month[i];

	for (int i = 0; i < start_month - 1; ++i)
		build_id -= days_in_month[i];

    return build_id;
}

logger::logger()
{
    fname._set("");
    strings.clear();
    callbacks.clear();

    MTX = new threading::mutex("log");
    flush_forced = strstr(core.params(), "-forcelog ");
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

    if (flush_forced)
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
        vfs::wopen_os(&f, fname.c_str());
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