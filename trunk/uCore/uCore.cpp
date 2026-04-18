#include "ucore.h"

void bugtrap_attach_thread();

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_THREAD_DETACH:
        {
            memory().thread_detach();
        }
	    case DLL_PROCESS_ATTACH:
        {
            bugtrap_attach_thread();
            _clearfp();
            _control87(_PC_53, _MCW_PC);
            _control87(_RC_CHOP, _MCW_RC);
            _control87(_RC_NEAR, _MCW_RC);
            _control87(_EM_DENORMAL | _EM_INEXACT, _MCW_EM);
            memory().thread_attach();
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

void u_core::_initialize(LPCSTR _application_name, LPCSTR _data_path, bool no_log)
{
    __int64 v8; // r8
    char* buf; // rcx
    char* ptr; // rax
    char* i; // rdx
    char v12; // al
    #7895 * v13; // rcx
    u_string p_user_name; // rdi
    __int64 v15; // r8
    unsigned int* v16; // rcx
    unsigned int* v17; // rax
    unsigned int* j; // rdx
    char v19; // al
    u_string p_comp_name; // rdi
    __int64 v21; // r8
    unsigned int* v22; // rcx
    unsigned int* v23; // rax
    unsigned int* k; // rdx
    char v25; // al
    __int64 v26; // r8
    __int64 v27; // rax
    const char* v28; // rdx
    u_memory* v29; // rax
    const char* v30; // rdx
    str_shared* p_build_key; // rdi
    str_value* p; // rax
    const char* v33; // rsi
    char* v34; // rax
    sz_string<1024>* p_key_string; // rax
    int length; // ecx
    const char* v37; // rdi
    unsigned int sz; // [rsp+30h] [rbp-D0h] BYREF
    u_string<32> v39; // [rsp+38h] [rbp-C8h] BYREF
    sz_string<1024> key_string; // [rsp+70h] [rbp-90h] BYREF
    char tmp[256]; // [rsp+470h] [rbp+370h] BYREF

    u32 v4 = init_counter;
    if (!init_counter)
    {
        v8 = -1;
        do
            ++v8;
        while (_application_name[v8]);
        u_string<32>::assign(&this->application_name, _application_name, v8);
        if (this->application_name.capacity <= 0x20u)
            buf = (char*)this->application_name.buf;
        else
            buf = this->application_name.ptr;
        if (this->application_name.capacity <= 0x20u)
            ptr = (char*)this->application_name.buf;
        else
            ptr = this->application_name.ptr;
        for (i = &ptr[this->application_name.count]; buf != i; ++buf)
        {
            v12 = *buf;
            if ((unsigned __int8)(*buf - 65) <= 0x19u)
                v12 += 32;
            *buf = v12;
        }
        u_platform::initialize_begin(&platform);
        CoInitializeEx(0, 0);
        threading::_initialize_cpu_thread(v13);
        sz = 256;
        GetUserNameA(tmp, &sz);
        p_user_name = &this->user_name;
        v15 = -1;
        do
            ++v15;
        while (tmp[v15]);
        u_string<32>::assign(&this->user_name, tmp, v15);
        if (this->user_name.capacity <= 0x20u)
            v16 = this->user_name.buf;
        else
            v16 = (unsigned int*)p_user_name->ptr;
        if (this->user_name.capacity <= 0x20u)
            v17 = this->user_name.buf;
        else
            v17 = (unsigned int*)p_user_name->ptr;
        for (j = (unsigned int*)((char*)v17 + this->user_name.count); v16 != j; v16 = (unsigned int*)((char*)v16 + 1))
        {
            v19 = *(_BYTE*)v16;
            if ((unsigned __int8)(*(_BYTE*)v16 - 65) <= 0x19u)
                v19 += 32;
            *(_BYTE*)v16 = v19;
        }
        sz = 256;
        GetComputerNameA(tmp, &sz);
        p_comp_name = &this->comp_name;
        v21 = -1;
        do
            ++v21;
        while (tmp[v21]);
        u_string<32>::assign(&this->comp_name, tmp, v21);
        if (this->comp_name.capacity <= 0x20u)
            v22 = this->comp_name.buf;
        else
            v22 = (unsigned int*)p_comp_name->ptr;
        if (this->comp_name.capacity <= 0x20u)
            v23 = this->comp_name.buf;
        else
            v23 = (unsigned int*)p_comp_name->ptr;
        for (k = (unsigned int*)((char*)v23 + this->comp_name.count); v22 != k; v22 = (unsigned int*)((char*)v22 + 1))
        {
            v25 = *(_BYTE*)v22;
            if ((unsigned __int8)(*(_BYTE*)v22 - 65) <= 0x19u)
                v25 += 32;
            *(_BYTE*)v22 = v25;
        }
        v26 = -1;
        do
            ++v26;
        while (_data_path[v26]);
        u_string<32>::assign(&this->content_root, _data_path, v26);
        v27 = -1;
        do
            ++v27;
        while (_data_path[v27]);
        if ((_DWORD)v27 && _data_path[(unsigned int)(v27 - 1)] != 92)
        {
            v39.ptr = 0;
            *(_QWORD*)&v39.capacity = 32;
            u_string<32>::assign(&v39, "\\", 1u);
            v28 = (const char*)v39.buf;
            if (v39.capacity > 0x20u)
                v28 = v39.ptr;
            u_string<32>::append(&this->content_root, v28, v39.count);
            if (v39.ptr)
            {
                v29 = memory();
                u_memory::main_realloc(v29, v39.ptr, 0, 0, "u_string", 0);
            }
        }
        if (this->content_root.capacity <= 0x20u)
            v30 = (const char*)this->content_root.buf;
        else
            v30 = this->content_root.ptr;
        rlog("* VFS: content_root = %s", v30);
        vfs::registry();
        log_create(no_log);
        u_platform::initialize_end(&platform);
        bugtrap_attach_process();
        p_build_key = &this->_build_key;
        this->_complete_edition = 0;
        p = this->_build_key.p_;
        if (p)
        {
            _InterlockedAdd(&p->refs, 0xFFFFFFFF);
            p_build_key->p_ = 0;
        }
        p_build_key->p_ = 0;
        v33 = u_core::params(this);
        if (!v33)
            debug::fail("string && sub_string", "d:\\trunk\\src\\ucore\\libstr.h", "sz_strstr", 108);
        v34 = strstr(v33, "-build_key ");
        if (v34)
        {
            if (strcpy_s(key_string._data, 0x400u, v34 + 11))
                debug::fail("0==strcpy_s(dest,sz,src)", "d:\\trunk\\src\\ucore\\libstr.h", "sz_cpy", 37);
            if (key_string._data[0])
            {
                p_key_string = &key_string;
                while (p_key_string->_data[0] != 32)
                {
                    p_key_string = (sz_string<1024> *)((char*)p_key_string + 1);
                    if (!p_key_string->_data[0])
                        goto LABEL_64;
                }
                p_key_string->_data[0] = 0;
            LABEL_64:
                str_shared::operator=(&this->_build_key, key_string._data);
            }
        }
        if (p_build_key->p_)
            length = p_build_key->p_->length;
        else
            length = 0;
        if (!length)
            str_shared::operator=(&this->_build_key, "2033");
        v37 = u_core::params(this);
        if (!v37)
            debug::fail("string && sub_string", "d:\\trunk\\src\\ucore\\libstr.h", "sz_strstr", 108);
        this->io_log = strstr(v37, "-io_log") != 0;
        v4 = init_counter;
    }
    init_counter = v4 + 1;
}

str_shared* u_core::menu_level(str_shared* result)
{
    str_shared* temp;
    temp->_set("");
    if (!build_33_34())
    {
        string_path Destination;
        R_ASSERT(_build_key.size() && (build_2033() || build_2034()));
        R_ASSERT(strcpy_s(Destination, sizeof(Destination), _build_key.c_str()));
        R_ASSERT(strcat_s(Destination, sizeof(Destination), "\\000"));
        temp->_set(Destination);
    }
    return temp;
}

void u_core::set_build_key_2033()
{
    _build_key = BUILD_ID_2033;
}

void u_core::set_build_key_2034()
{
    _build_key = BUILD_ID_2034;
}

u64 u_core::complete_edition()
{
    R_ASSERT(_build_key.size());
    return _complete_edition;
}

char* u_core::params()
{
    if (!params_valid)
    {
        params_string[0] = 0;
        strcat_s(params_string, sizeof(params_string), " ");
        strcat_s(params_string, sizeof(params_string), GetCommandLineA());
        strcat_s(params_string, sizeof(params_string), " ");

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