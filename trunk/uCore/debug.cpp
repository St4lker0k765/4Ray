#include "stdafx.h"
#include "debug.h"

void debug::fatal(const char* F, ...)
{
	string4096 buffer; // [rsp+20h] [rbp-2018h] BYREF
	string4096 reason; // [rsp+1020h] [rbp-1018h] BYREF
	va_list p; // [rsp+2048h] [rbp+10h] BYREF

	va_start(p, F);
	vsnprintf(buffer, 0xFFFu, F, p);
	va_end(p);	
	sprintf_s(reason, sizeof(reason), "*** Fatal Error ***\n%s", buffer);
	debug::backend(reason, 0, 0, 0);
}

void debug::backend(const char* reason, const char* file, const char* func, int line)
{
    int length; // ecx
    char* value; // r9
    int v10; // ecx
    char* v11; // r9
    str_shared result; // [rsp+40h] [rbp-1048h] BYREF
    string4096 temp; // [rsp+50h] [rbp-1038h] BYREF

    if (IsDebuggerPresent())
        __debugbreak();
    if ((_S4_0 & 1) == 0)
    {
        _S4_0 |= 1u;
        threading::mutex::mutex(&m, "debug::backend");
        atexit(debug::backend_::_4_::_dynamic_atexit_destructor_for__m__);
    }
    threading::mutex::lock(&m);
    rlog("! [STOP] file '%s', function '%s', line %d.\n***Reason***: %s", file, func, line, reason);
    if (file && func && line)
    {
        if (g_levelname.p_)
            length = g_levelname.p_->length;
        else
            length = 0;
        if (length)
        {
            if (g_levelname.p_)
                value = g_levelname.p_->value;
            else
                value = 0;
        }
        else
        {
            value = (char*)def;
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
        if (g_levelname.p_)
            v10 = g_levelname.p_->length;
        else
            v10 = 0;
        if (v10)
        {
            if (g_levelname.p_)
                v11 = g_levelname.p_->value;
            else
                v11 = 0;
        }
        else
        {
            v11 = (char*)def;
        }
        sz_printf(temp, 0x1000u, "! [STOP] level '%s'.\n***Reason***: %s", v11, reason);
    }
    bugtrap_message(temp);
    log_flush(&result);
    if (result.p_)
    {
        _InterlockedDecrement(&result.p_->refs);
        result.p_ = 0;
    }
    if (on_debug_break_cb_0.m_Closure.m_pthis || on_debug_break_cb_0.m_Closure.m_pFunction)
        on_debug_break_cb_0.m_Closure.m_pFunction(on_debug_break_cb_0.m_Closure.m_pthis);
    if (IsDebuggerPresent())
    {
        MessageBoxA(NULL, temp, "error", MB_ICONEXCLAMATION);
        __debugbreak();
    }
    if (!IsDebuggerPresent())
    {
        bugtrap_show();
        *crash = 0;
    }
    threading::mutex::unlock(&m);
}

void debug::error(DWORD hr, const char* expr, const char* file, const char* func, int line)
{
    u_string<32>* v8; // rax
    const char* buf; // r9
    u_memory* v10; // rax
    u_string<32> result; // [rsp+30h] [rbp-1058h] BYREF
    string4096 reason; // [rsp+60h] [rbp-1028h] BYREF

    v8 = debug::error2string(&result, hr);
    if (v8->capacity <= 0x20u)
        buf = (const char*)v8->buf;
    else
        buf = v8->ptr;
    sz_printf(reason, 0x1000u, "*** API-failure ***\n%s\nExpression: %s", buf, expr);
    if (result.ptr)
    {
        v10 = memory();
        u_memory::main_realloc(v10, result.ptr, 0, 0, "u_string", 0);
    }
    debug::backend(reason, file, func, line);
}