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

    threading::mutex("log");
    flush_forced = strstr(core.params(), "-forcelog ");
}

logger::~logger()
{
    callbacks.clear();
    strings.clear();
}

void logger::add(const char* split)
{
    OutputDebugStringA(split);
    OutputDebugStringA("\n");

    lock();
    strings.push_back(split);
    unlock();

    for (int i = 0; i < callbacks.size(); i++)
    {
        callbacks[i](split);
    }

    if (flush_forced)
        flush_to_hdd();
}

void logger::flush_to_hdd()
{
    gtl::intrusive_ptr<vfs::iwriter, gtl::intrusive_base, gtl::intrusive_default_functionality, gtl::thread_safe_inc_dec<gtl::intrusive_base> > f; // [rsp+60h] [rbp+8h] BYREF

    if (fname.size())
    {
        if (!flush_forced)
        {
            rlog("log flushed to [%s]", fname.c_str());
            rlog("%s build %d[SVN=%s], %s, %s\n", "uCore :: 4A platform", build_number(), core.game_version.c_str(), __DATE__, __TIME__);
        }
        lock();
        vfs::wopen_os(&f, fname.c_str());
        if (f._object)
        {
            for (int i = 0; i < strings.size(); i++)
            {
                vfs::writer_base_t<vfs::iwriter>::w_string(&f._object->vfs::writer_base_t<vfs::iwriter>, strings[i].c_str());
            }
        }
        unlock();
    }
}
logger* Log = nullptr;