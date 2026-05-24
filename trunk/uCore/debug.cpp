#include "stdafx.h"
#include "debug.h"

void bugtrap_show();
void bugtrap_message(const char* msg);
void log_flush(str_shared* result);

void debug::fatal(const char* F, ...)
{
	string4096 buffer;
    string4096 reason;
	va_list p;

	va_start(p, F);
	vsnprintf(buffer, sizeof(buffer), F, p);
	va_end(p);	
	sprintf_s(reason, sizeof(reason), "*** Fatal Error ***\n%s", buffer);
	backend(reason, 0, 0, 0);
}

LPCSTR debug::error2string(DWORD code)
{
	LPCSTR				result	= 0;
	static	string1024	desc_storage;

#ifdef _M_AMD64
#else
	result				= DXGetErrorDescription9	(code);
#endif
	if (!result) 
	{
		FormatMessageA	(FORMAT_MESSAGE_FROM_SYSTEM,0,code,0,desc_storage,sizeof(desc_storage)-1,0);
		result			= desc_storage;
	}
	return		result	;
}

void debug::backend(const char* reason, const char* file, const char* func, int line)
{
    MTX.lock();
    rlog("! [STOP] file '%s', function '%s', line %d.\n***Reason***: %s", file, func, line, reason);
 
    string4096 temp;
    if (file && func && line)
    {
        int length = 0;
        if (g_levelname._get())
            length = g_levelname._get()->length;

        LPCSTR value = "";
        if (length)
        {
            if (g_levelname._get())
                value = g_levelname.c_str();
        }
        sprintf_s(
            temp,
            sizeof(temp),
            "! [STOP] level '%s' file '%s', function '%s', line %d.\n***Reason***: %s",
            value,
            file,
            func,
            line,
            reason);
    }
    else
    {
        int length = 0;
        if (g_levelname._get())
            length = g_levelname._get()->length;

        LPCSTR value = "";
        if (length)
        {
            if (g_levelname._get())
                value = g_levelname.c_str();
        }
        sprintf_s(temp, sizeof(temp), "! [STOP] level '%s'.\n***Reason***: %s", value, reason);
    }
    bugtrap_message(temp);
    str_shared result;
    log_flush(&result);

    if (on_debug_break_cb_0)
        on_debug_break_cb_0();

    if (IsDebuggerPresent())
    {
        MessageBoxA(NULL, temp, "error", MB_ICONEXCLAMATION);
        __debugbreak();
    }
    else
    {
        bugtrap_show();
#pragma todo("Determine what this does. Modified externally?") 
//     *crash = 0;
    }
    MTX.unlock();
}

void debug::error(DWORD hr, const char* expr, const char* file, const char* func, int line)
{
    string4096 reason;

    u_string result;
    LPCSTR buf = error2string(hr);
    sprintf_s(reason, sizeof(reason), "*** API-failure ***\n%s\nExpression: %s", buf, expr);
    backend(reason, file, func, line);
}

void debug::fail(const char* e1, const char* file, const char* func, int line)
{
    string4096 reason;

    sprintf_s(reason, sizeof(reason), "*** Assertion failed ***\nExpression: %s\n", e1);
    backend(reason, file, func, line);
}

void debug::fail(const char* e1, const char* e2, const char* file, const char* func, int line)
{
    string4096 reason;

    sprintf_s(reason, sizeof(reason), "*** Assertion failed ***\nExpression: %s\n%s", e1, e2);
    backend(reason, file, func, line);
}

void debug::fail(
    const char* e1,
    const char* e2,
    const char* e3,
    const char* file,
    const char* func,
    int line)
{
    string4096 reason;

    sprintf_s(reason, sizeof(reason), "*** Assertion failed ***\nExpression: %s\n%s\n%s", e1, e2, e3);
    backend(reason, file, func, line);
}

void debug::fail(
    const char* e1,
    const char* e2,
    const char* e3,
    const char* e4,
    const char* file,
    const char* func,
    int line)
{
    string4096 reason;

    sprintf_s(reason, sizeof(reason), "*** Assertion failed ***\nExpression: %s\n%s\n%s\n%s", e1, e2, e3, e4);
    backend(reason, file, func, line);
}

void debug::fail(
    const char* e1,
    const char* e2,
    const char* e3,
    const char* e4,
    const char* e5,
    const char* file,
    const char* func,
    int line)
{
    string4096 reason;

    sprintf_s(reason, sizeof(reason), "*** Assertion failed ***\nExpression: %s\n%s\n%s\n%s\n%s", e1, e2, e3, e4, e5);
    backend(reason, file, func, line);
}

void debug::fail(
    const char* e1,
    const char* e2,
    const char* e3,
    const char* e4,
    const char* e5,
    const char* e6,
    const char* file,
    const char* func,
    int line)
{
    string4096 reason; // [rsp+50h] [rbp-1028h] BYREF

    sprintf_s(reason, sizeof(reason), "*** Assertion failed ***\nExpression: %s\n%s\n%s\n%s\n%s\n%s", e1, e2, e3, e4, e5, e6);
    backend(reason, file, func, line);
}
