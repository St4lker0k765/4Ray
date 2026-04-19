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

    int i, j;
    for (i = 0, j = 0; s[i] != 0; i++) 
    {
        if (s[i] == '\n') 
        {
            split[j] = 0;	// end of line
            if (split[0] == 0) 
            { 
                split[0] = ' '; 
                split[1] = 0; 
            }
            Log->add(split);
            j = 0;
        }
        else 
        {
            split[j++] = s[i];
        }
    }

    split[j] = 0;
    Log->add(split);
}

void rlog(const char* format, ...)
{
    string1024 buf{};
    va_list va;

    va_start(va, format);
    if (vsnprintf(buf, sizeof(buf), format, va) >= 0)
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
    OutputDebugStringA(split);
    OutputDebugStringA("\n");

    MTX->lock();
    strings.push_back(split);
    MTX->unlock();

    for (int i = 0; i < callbacks.size(); i++)
    {
        callbacks[i](split);
    }

    if (flush_forced)
        flush_to_hdd();
}

void logger::flush_to_hdd()
{
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