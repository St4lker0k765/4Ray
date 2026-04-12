#include "ucore.h"

_purecall_handler on_purecall()
{
    debug::fail("false", "pure virtual function call", "debug_bugtrap.cpp", "on_purecall", 43);
}

void bugtrap_attach_thread()
{
    _set_purecall_handler(on_purecall());
    set_terminate(BT_CallCppFilter);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_THREAD_DETACH:
        {
            u_memory::thread_detach(memory());
        }
	    case DLL_PROCESS_ATTACH:
        {
            bugtrap_attach_thread();
            _clearfp();
            _control87(_PC_53, _MCW_PC);
            _control87(_RC_CHOP, _MCW_RC);
            _control87(_RC_NEAR, _MCW_RC);
            _control87(_EM_DENORMAL | _EM_INEXACT, _MCW_EM);
            u_memory::thread_attach(memory());
        }
    }
    return TRUE;
}