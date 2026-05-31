#include "ucore.h"

void bugtrap_attach_thread();

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_THREAD_DETACH:
        {
            memory()->thread_detach();
        }
	    case DLL_PROCESS_ATTACH:
        {
            bugtrap_attach_thread();
            _clearfp();
            _control87(_PC_53, _MCW_PC);
            _control87(_RC_CHOP, _MCW_RC);
            _control87(_RC_NEAR, _MCW_RC);
            _control87(_EM_DENORMAL | _EM_INEXACT, _MCW_EM);
            memory()->thread_attach();
        }
    }
    return TRUE;
}

u_core::u_core()
{
    application_name.clear();
    content_root.clear();
    user_name.clear();
    comp_name.clear();
    game_version.clear();
    ui_game_nick[0] = 0;
    _build_key = "";
    params_valid = false;
    human_move_ng_mode = 0;
}

void log_flush(str_shared* result);

void u_core::_destroy(UINT res)
{
    if (init_counter-- == 1)
    {
        str_shared result;
        log_flush(&result);
        if (vfs::registry_exists())
        {
        //    delete vfs::registry();
        }
    }
    vfs::trace.end();
    TerminateProcess(GetCurrentProcess(), res);
}

u_core::u_core(const u_core* other)
{
    application_name = other->application_name;
    content_root = other->content_root;
    user_name = other->user_name;
    comp_name = other->comp_name;
    game_version = other->game_version;
    _build_key = other->_build_key;
    wcscpy(ui_game_nick, other->ui_game_nick);
    strcpy(params_string, other->params_string);
    _complete_edition = other->_complete_edition;
    params_valid = other->params_valid;
    trace = other->trace;
    human_move_ng_mode = other->human_move_ng_mode;
    io_log = other->io_log;
    package_downloading = other->package_downloading;
    package_downloading_progress = other->package_downloading_progress;
    level_loading_progress = other->level_loading_progress;
    level_loading_time = other->level_loading_time;
    dlc_corrupt = other->dlc_corrupt;
}

void bugtrap_attach_process();
void log_create(bool no_log);

u32 init_counter = 0;
void u_core::_initialize(LPCSTR _application_name, LPCSTR _data_path, bool no_log)
{
    if (!init_counter)
    {
        string256 app_temp;
        sz_cpy(app_temp, sizeof(app_temp), _application_name);
        _strlwr_s(app_temp, sizeof(app_temp));
        application_name.assign(app_temp, strlen(app_temp));

        Log = new logger();
        platform.initialize_begin();
        CoInitializeEx(NULL, COINIT_MULTITHREADED);
        threading::_initialize_cpu_thread();

        string256 tmp;
        GetUserNameA(tmp, (LPDWORD)sizeof(tmp));
        _strlwr_s(tmp, sizeof(tmp));
        user_name.assign(tmp, strlen(tmp));

        GetComputerNameA(tmp, (LPDWORD)sizeof(tmp));
        _strlwr_s(tmp, sizeof(tmp));
        comp_name.assign(tmp, strlen(tmp));

        content_root.assign(_data_path, strlen(_data_path));
        content_root.append("\\");
        rlog("* VFS: content_root = %s", content_root.c_str());
        vfs::registry();

        log_create(no_log);
        platform.initialize_end();
        bugtrap_attach_process();
        _complete_edition = false;

        if (strstr(params(), "-build_key "))
        {
            string64 key_string;
            sscanf(strstr(params(), "-build_key ") + 11, "%[^ ] ", key_string);
            _build_key = key_string;
        }

        if (!_build_key.size())
           _build_key = "2033";

        io_log = strstr(params(), "-io_log") != 0;
    }
    init_counter++;
}

str_shared u_core::menu_level(str_shared* result)
{
    str_shared temp = "";
    if (!build_33_34())
    {
        string_path Destination;
        R_ASSERT(_build_key.size() && (build_2033() || build_2034()));
        sz_cpy(Destination, sizeof(Destination), _build_key.c_str());
        sz_cat(Destination, sizeof(Destination), "\\000");
        temp = Destination;
    }
    return temp;
}

const str_shared u_core::build_key()
{
    R_ASSERT(_build_key.size());
    return _build_key;
}

void u_core::set_build_key_2033()
{
    _build_key = BUILD_ID_2033;
}

void u_core::set_build_key_2034()
{
    _build_key = BUILD_ID_2034;
}

bool u_core::complete_edition()
{
    R_ASSERT(_build_key.size());
    return _complete_edition;
}

char* u_core::params()
{
    if (!params_valid)
    {
        params_string[0] = 0;
        sz_cat(params_string, sizeof(params_string), " ");
        sz_cat(params_string, sizeof(params_string), GetCommandLineA());
        sz_cat(params_string, sizeof(params_string), " ");

        trace = strstr(params_string, " -trace ") != 0;
        human_move_ng_mode = strstr(params_string, " -human_move_ng_mode ") != 0;
        _strlwr_s(params_string, sizeof(params_string));

        params_valid = true;
        package_downloading = 0;
    }
    return params_string;
}

bool u_core::build_33_34()
{
    return !strcmp(_build_key.c_str(), BUILD_ID_33_34);
}

bool u_core::build_2034()
{
    return !strcmp(_build_key.c_str(), BUILD_ID_2034);
}

bool u_core::build_2033()
{
    return !strcmp(_build_key.c_str(), BUILD_ID_2033);
}

UCORE_API u_core core;