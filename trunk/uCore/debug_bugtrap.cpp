#include "stdafx.h"
#include "debug.h"
#include "../3rd-party/BugTrap/BugTrap.h"
#include <direct.h>

void log_flush(str_shared* result)
{
    Log->flush_to_hdd();
    result = &Log->fname;
}

void bugtrap_show()
{
    HANDLE BTHandle = OpenThread(THREAD_ALL_ACCESS, 0, GetCurrentThreadId());
    if (BTHandle)
    {
        _CONTEXT Context;
        Context.ContextFlags = CONTEXT_FULL;
        if (GetThreadContext(BTHandle, &Context))
        {
            _EXCEPTION_POINTERS ep;
            ep.ExceptionRecord = 0;
            ep.ContextRecord = &Context;
            BT_SehFilter(&ep);
            CloseHandle(BTHandle);
        }
    }
}

void __stdcall bugtrap_handler(INT_PTR nErrHandlerParam)
{
    const char* DialogMessage; // rax
    str_shared result; // [rsp+38h] [rbp+10h] BYREF

    DialogMessage = (const char*)BT_GetDialogMessage(BTDM_INTRO2);
    if (!DialogMessage || (rlog("BUGTRAP message [%s]", DialogMessage), !*DialogMessage))
    {
        if (g_levelname._get() && g_levelname.size())
            BT_SetDialogMessage(BTDM_INTRO2, (LPCTSTR)g_levelname.c_str());
    }
    log_flush(&result);
}

_purecall_handler on_purecall()
{
    R_ASSERT2(false, "pure virtual function call");
}

void bugtrap_attach_thread()
{
    _set_purecall_handler(on_purecall());
    set_terminate(BT_CallCppFilter);
}

void bugtrap_message(const char* msg)
{
    BT_SetDialogMessage(BTDM_INTRO2, (LPCTSTR)msg);
}

void bugtrap_attach_process()
{
    BT_InstallSehFilter();
    BT_SetAppName(L"Metro");
    BT_SetSupportEMail(L"duke@4a-games.com");
    BT_SetFlags(
        BTF_DETAILEDMODE 
      | BTF_ATTACHREPORT
      | BTF_LISTPROCESSES
      | BTF_SHOWADVANCEDUI
      | BTF_SCREENCAPTURE
      | BTF_INTERCEPTSUEF);
    BT_SetReportFormat(BTRF_TEXT);

    LPCSTR params = core.params();
    u32 activity = BTA_SAVEREPORT;
    if (!strstr(params, "-nocrashdlg"))
    {
        activity |= BTA_SHOWUI;
    }
    BT_SetActivityType((BUGTRAP_ACTIVITY)activity);

#pragma todo("Determine what these flags mean")
    if (strstr(params, "-fulldmp"))
        BT_SetDumpType(6951);
    BT_SetSupportServer(L"localhost", 9999);
    BT_SetSupportURL(L"www.4a-games.com");

    string1024 Destination;
    _getcwd(Destination, sizeof(Destination));
    strcat_s(Destination, sizeof(Destination), "\\");
    BT_SetPreErrHandler(bugtrap_handler, 0);
    BT_SetReportFilePath((LPCTSTR)Destination);

    string1024 logName;
    sprintf_s(logName, sizeof(logName), "%s%s", Destination, log_fname);
    rlog("added %s [%s]", "rlog", logName);
    BT_AddLogFile((LPCTSTR)logName);

    string1024 userCfg;
    sprintf_s(userCfg, sizeof(userCfg), "%s%s", Destination, "user.cfg");
    rlog("added %s [%s]", "cfg", userCfg);
    BT_AddLogFile((LPCTSTR)userCfg);

    string_path moduleFN;
    moduleFN[0] = 0;
    GetModuleFileNameA(0, moduleFN, sizeof(moduleFN));
    if (moduleFN[0])
    {
        string1024 executable;
        sprintf_s(executable, sizeof(executable), "%s", moduleFN);
        rlog("added %s [%s]", "exe", executable);
        BT_AddLogFile((LPCTSTR)executable);

        u_string module_no_ext;
        module_no_ext.sz_replace_file_ext(moduleFN, ".pdb");

        string1024 pdbName;
        sprintf_s(pdbName, sizeof(pdbName), "%s", module_no_ext.c_str());
        rlog("added %s [%s]", "pdb", pdbName);
        BT_AddLogFile((LPCTSTR)pdbName);
    }
    str_shared v8;
    log_flush(&v8);
}