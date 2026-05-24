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

void uengine_initialize()
{
    threading* v8; // rcx
    __int64 v10; // rax
    u_archive* object; // rdi
    u_archive* v12; // rbx
    bool v13; // zf
    u_archive* v14; // rbx
    u_archive* v15; // rdi
    u_archive* v16; // rbx
    u_archive* v17; // rbx
    gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> > result; // [rsp+60h] [rbp+8h] BYREF
    gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> > v19; // [rsp+68h] [rbp+10h] BYREF

    engine.loader_iotimebase = vfs::rtime;
    engine.loader_log("uengine-run");
    engine.flags_mapstate = mapstate_initializing;
    g_trace_frame = 20;
    g_test_run = -(strstr(core.params(), " -test ") != 0);
    if (strstr(core.params(), " -openautomate "))
        goto LABEL_16;
    if (strstr(core.params(), " \"-openautomate\" "))
        goto LABEL_16;
    if (strstr(core.params(), " -benchmark "))
        goto LABEL_16;
    if (strstr(core.params(), " \"-benchmark\" "))
        LABEL_16:
    g_benchmark = 1;
    else
        g_benchmark = 0;
    g_tester = strstr(core.params(), " -tester ") != 0;
    g_simulation = strstr(core.params(), " -simulation ") != 0;
    g_nv_refly = strstr(core.params(), "-nv_refly");
    global_init_timer.start();
    core._initialize("UEngine", path, false);
    engine.loader_log("core-init");
    localization::stable::initialize(v8);
    engine.loader_log("stable-init");
    engine_timing::initialize(&engine.time);
    script_engine();
    (*(void(__fastcall**)(__int64))(*(_QWORD*)v10 + 40LL))(v10);
    engine.loader_log("reload-scripts");
    object = create_u_archive_lua(&result, "_G", 1, 1, 1)->_object;
    if (object)
    {
        ++object->_ref_count;
    }
    v12 = g_base_arch._object;
    if (g_base_arch._object)
    {
        if (v12->_ref_count-- == 1)
            gtl::thread_unsafe_inc_dec<intrusive_base_pool>::dec_cold<u_archive>(&g_base_arch, v12);
    }
    v14 = result._object;
    g_base_arch._object = object;
    if (result._object)
    {
        v13 = v14->_ref_count-- == 1;
        if (v13)
            gtl::thread_unsafe_inc_dec<intrusive_base_pool>::dec_cold<u_archive>(&result, v14);
    }
    v15 = create_u_archive_lua(&v19, "config", 0, 1, 1)->_object;
    if (v15)
    {
        ++v15->_ref_count;
    }
    v16 = g_config_arch._object;
    if (g_config_arch._object)
    {
        v13 = v16->_ref_count-- == 1;
        if (v13)
            gtl::thread_unsafe_inc_dec<intrusive_base_pool>::dec_cold<u_archive>(&g_config_arch, v16);
    }
    v17 = v19._object;
    g_config_arch._object = v15;
    if (v19._object)
    {
        v13 = v17->_ref_count-- == 1;
        if (v13)
            gtl::thread_unsafe_inc_dec<intrusive_base_pool>::dec_cold<u_archive>(&v19, v17);
    }
    engine.loader_log("create-base-lua-arch");
    engine.create();
    engine.loader_log("engine-create");
    engine.prepare_oa_tests();
    if (!g_editor)
        entrypoint_initialize();
}

void uengine_runcycle()
{
    if (g_editor)
        engine.p_editor->run();
    else
        engine.run();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lp_cmd_line, int n_cmd_show)
{
    engine.loader_log("WinMain");
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
    g_editor = strstr(GetCommandLineA(), "-editor");
    if (!g_editor && core.params())
    {
        char* dir = 0;
        sscanf(strstr(core.params(), "-trace ") + 7, "%[^ ] ", &dir);
        strlwr(dir);
        vfs::trace.output_dir(dir);
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