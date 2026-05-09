#include "stdafx.h"
#include <direct.h>
#include "u_splash_window.h"

bool already_running()
{
    HANDLE MutexW = CreateMutexW(0, 1, L"UENGINE-85E7D263-C58D-40df-A4D8-1D33A7AFB1F5");
    bool exists = GetLastError() == ERROR_ALREADY_EXISTS;
    if (MutexW)
        ReleaseMutex(MutexW);
    return exists;
}

HHOOK g_keyboard_hook;
LRESULT ll_keyboard_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode)
        return CallNextHookEx(g_keyboard_hook, nCode, wParam, lParam);

    if ((wParam == WPARAM(-1)) || engine.window._b_windowed || !engine.window._b_active)
        return CallNextHookEx(g_keyboard_hook, 0, wParam, lParam);

    return true;
}

HHOOK hook_keyboard()
{
    HMODULE ModuleHandleA; // rax
    HHOOK result; // rax

    ModuleHandleA = GetModuleHandleA(0);
    result = SetWindowsHookExA(WH_KEYBOARD_LL, ll_keyboard_proc, ModuleHandleA, 0);
    g_keyboard_hook = result;
    return result;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lp_cmd_line, int n_cmd_show)
{
    char* ptr; // rax
    char* v11; // rbx
    const struct str_shared* v13; // rax
    str_shared Data; // [rsp+58h] [rbp-A8h] BYREF
    char v28; // [rsp+70h] [rbp-90h] BYREF

    cengine::loader_log(&engine, "WinMain");
    engine_timing::engine_started(&engine.time);
    if (!IsDebuggerPresent())
        hook_keyboard();

    Splash.show();
    if (strstr(GetCommandLineA(), "-nvperf"))
    {
        _chdrive(21); // drive U:
        _chdir("\\");
    }
    if (strstr(GetCommandLineA(), "-agperfmon"))
    {
        _chdrive(21); // drive U:
        _chdir("\\");
    }
    ptr = core.application_name.ptr;
    g_editor = strstr(GetCommandLineA(), "-editor");
    if (!g_editor && core.params())
    {
        v28 = 0;
        v11 = strstr(core.params(), "-trace ");

        _BYTE v29[271]; // [rsp+71h] [rbp-8Fh] BYREF
        memset(v29, 0, 0x103u);
        sscanf(v11 + 7, "%[^ ] ", &v28);
        strlwr(v28);
        v13 = (const struct str_shared*)str_shared::str_shared(&Data, &v28, 0);
        vfs::tracer::output_dir((vfs::tracer*)vfs::trace._types[0].p_, v13);
        str_shared::~str_shared(&Data);
        ptr = core.application_name.ptr;
    }
    if (strstr(core.params(), "-branch_trace "))
        g_trace = true;

    g_disable_mboxes = g_trace;
    g_resave = strstr(GetCommandLineA(), "-resave") != nullptr;
    g_diff = strstr(GetCommandLineA(), "-diff") != nullptr;
    g_summary = strstr(GetCommandLineA(), "-summary") != nullptr;
    g_nopressanykey = strstr(GetCommandLineA(), "-nopressanykey") != nullptr;
    if (IsDebuggerPresent() || strstr(GetCommandLineA(), "-forcestart") || !g_editor || g_resave || !already_running())
    {
        DWORD dwDisposition[4];
        HKEY hKey;

        if (!RegCreateKeyExA(
            HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Wow6432Node\\NVIDIA Corporation\\Global\\Stereo3D\\GameConfigs\\uengine_m",
            0,
            nullptr,
            0,
            KEY_ALL_ACCESS,
            nullptr,
            &hKey,
            dwDisposition))
        {
            RegSetValueExA(hKey, "StereoTextureEnable", 0, REG_DWORD, (const BYTE*)131, 4u);
            RegCloseKey(hKey);
        }
        if (!RegCreateKeyExA(
            HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Wow6432Node\\NVIDIA Corporation\\Global\\Stereo3D\\GameConfigs\\uengine_m",
            0,
            nullptr,
            0,
            KEY_ALL_ACCESS,
            nullptr,
            &hKey,
            dwDisposition))
        {
            RegSetValueExA(hKey, "StereoCutoff", 0, REG_DWORD, (const BYTE*)1, 4u);
            RegCloseKey(hKey);
        }
        uengine_initialize();
        uengine_runcycle();
        uengine_destroy();
        TerminateProcess(GetCurrentProcess(), 0);
    }
    else
    {
        MessageBoxA(nullptr, "Editor already running!", "Error", MB_ICONERROR);
    }
    return 0;
}