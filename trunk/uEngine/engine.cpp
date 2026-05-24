#include "stdafx.h"
#include "engine.h"

HMODULE dll_load(const char* dll, const char* postfix, const char* comment)
{
    string_path name;
    sz_cpy(name, sizeof(name), dll);
    sz_cat(name, sizeof(name), postfix);

    rlog("[DLL] Loading %s", name);
    HMODULE result = LoadLibraryA(name);
    R_CHK(GetLastError());
    R_ASSERT3(result, "cannot load dll", name);

    engine.loader_log(comment);
    return result;
}

template <class T>
T dll_get_func(HMODULE hm, const char* func_name)
{
    T* func = (T*)GetProcAddress(hm, func_name);
    R_ASSERT2(func, func_name);
    return *func;
}

void cengine::create()
{
    cengine* v1; // r14
    const char* v2; // rbx
    u_memory* v3; // rax
    streaming::server* v4; // rax
    struct streaming::wi* v5; // r12
    streaming::server* v6; // rcx
    FARPROC v9; // rax
    __int64 (*v11)(void); // rbx
    __int64 v12; // rax
    u_vector<language_type_rec, allocator_t, unsigned short>* p_language_types; // r13
    const char* v14; // rax
    const threading_rw_check* v15; // rdx
    const char* v16; // rbx
    const gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* v17; // rax
    u_archive* object; // rbx
    int v20; // eax
    u_archive* v21; // rcx
    char* v22; // rbx
    language_type_rec* v23; // rax
    uvector_base<8, 8, vector_base<allocator_t<8, 8>, unsigned short> >* p_text_languages; // rsi
    const char* v25; // rax
    const threading_rw_check* v26; // rdx
    const char* v27; // rbx
    unsigned int v28; // r15d
    unsigned int v29; // ebx
    threading_rw_check* v30; // r14
    char* v31; // r15
    threading_rw_check* v32; // r14
    const str_shared* v33; // r15
    const char* v34; // rax
    threading_rw_check* v35; // rdi
    str_shared* array; // rsi
    const char* v37; // rbx
    int v38; // ecx
    localization::stable* new_stable; // rbx
    localization::stable* v40; // rcx
    u_memory* v41; // rax
    const char* v42; // rbx
    u_memory* v43; // rax
    load_new_stable_wi* v44; // rax
    struct streaming::wi* v45; // rax
    char* lua_section; // [rsp+20h] [rbp-E0h]
    char* lua_sectiona; // [rsp+20h] [rbp-E0h]
    int lua_section2; // [rsp+28h] [rbp-D8h]
    int lua_section2a; // [rsp+28h] [rbp-D8h]
    load_save_bin<u_vector<language_type_rec, allocator_t, unsigned short>, languages_loader_saver, 0> v50; // [rsp+41h] [rbp-BFh] BYREF
    unsigned int v51; // [rsp+44h] [rbp-BCh]
    less<language_type_rec> pred; // [rsp+48h] [rbp-B8h] BYREF
    char v53; // [rsp+49h] [rbp-B7h]
    unsigned int v54; // [rsp+4Ch] [rbp-B4h]
    char v55; // [rsp+50h] [rbp-B0h]
    char v56; // [rsp+51h] [rbp-AFh]
    str_shared l; // [rsp+58h] [rbp-A8h] BYREF
    gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> > result; // [rsp+60h] [rbp-A0h] BYREF
    int v59; // [rsp+68h] [rbp-98h]
    languages_loader_saver ls; // [rsp+70h] [rbp-90h] BYREF
    uvector_base<8, 8, vector_base<allocator_t<8, 8>, unsigned short> >* v61; // [rsp+78h] [rbp-88h]
    threading_rw_check::write_lock v62; // [rsp+80h] [rbp-80h] BYREF
    cengine* v63; // [rsp+88h] [rbp-78h]
    threading_rw_check::write_lock v64; // [rsp+90h] [rbp-70h] BYREF

    v1 = this;
    v63 = this;
    this->_mp = emp_none;
    v2 = type_info::_name_internal_method((type_info*)&streaming::server `RTTI Type Descriptor', &__type_info_root_node);
        v3 = memory();
    v4 = (streaming::server*)u_memory::main_realloc(v3, 0, 0x1C8u, 8u, v2, 0);
    v5 = 0;
    if (v4)
        v6 = (streaming::server*)streaming::server::server(v4);
    else
        v6 = 0;
    v55 = 0;
    g_streamer = v6;
    streaming::server::initialize(v6);
    loader_log("streamer-init");
    string_path postfix; 
    strcpy_s(postfix, sizeof(postfix),
#ifdef DEBUG
        "_d.dll"
#else
        "_m.dll"
#endif
    );
    h_render = dll_load("uRender", postfix, "dll-render");
    h_particles = dll_load("uParticles", postfix, "dll-particles");
    h_sound = dll_load("uSound", postfix, "dll-sound");
    h_skeleton = dll_load("uSkeleton", postfix, "dll-skeleton");
    h_game = dll_load("uGame", postfix, "dll-game");
    
    p_create = dll_get_func<Factory_Create*>(h_game, "factory_create");
    p_destroy = dll_get_func<Factory_Destroy*>(h_game, "factory_destroy");
    p_editor = nullptr;
    if (g_editor)
    {
        CoUninitialize();
        CoInitializeEx(0, 2u);
        h_editor = dll_load("uEditor", postfix, "dll-editor");
        v11 = GetProcAddress(h_editor, "create_editor_interface");
        if (!v11)
        {
            LODWORD(lua_section) = 1428;
            debug::fail((debug*)"func", "create_editor_interface", "engine.cpp", "dll_get_func", lua_section, lua_section2);
        }
        v12 = v11();
        p_editor = (editor_interface*)v12;
        (*(void(__fastcall**)(__int64))(*(_QWORD*)v12 + 32LL))(v12);
        engine.loader_log("editor-connect");
    }
    v53 = 0;
    p_language_types = &v1->_language_types;
    v14 = type_info::_name_internal_method((type_info*)&language_type_rec `RTTI Type Descriptor', &__type_info_root_node);
        v15 = &v1->_language_types.threading_rw_check;
    v16 = v14;
    if (v1 == (cengine*)-96LL)
        v15 = 0;
    threading_rw_check::write_lock::write_lock(&v62, v15);
    v56 = 0;
    uvector_base<24, 8, vector_base<allocator_t<24, 8>, unsigned short>>::inner_release<language_type_rec>(
        &v1->_language_types,
        0,
        0,
        v16);
    threading_rw_check::write_lock::~write_lock(&v62);
    v50 = 0;
    ls._key = "language_type";
    v17 = load_save_bin<u_vector<language_type_rec, allocator_t, unsigned short>, languages_loader_saver, 0>::operator()(
        &v50,
        &result,
        "language_type",
        0,
        0,
        0,
        &ls);
    types_loader_saver<language_type_rec>::load(&ls, &v1->_language_types, v17);
    object = result._object;
    if (result._object)
    {
        if (result._object->_ref_count <= 0)
            debug::fail(
                (debug*)"0 < *(s32 *)&(o->base_type::_ref_count)",
                "d:\\trunk\\src\\ucore\\gtl/intrusive_ptr.h",
                "gtl::thread_unsafe_inc_dec<class intrusive_base_pool>::dec",
                290);
        if (object->_ref_count-- == 1)
        {
            v20 = gtl::thread_unsafe_inc_dec<intrusive_base_pool>::dec_cold<u_archive>(&result, object);
            v21 = result._object;
            if (v20)
                v21 = 0;
            result._object = v21;
        }
    }
    v22 = uvector_base<24, 8, vector_base<allocator_t<24, 8>, unsigned short>>::end(&v1->_language_types);
    v23 = (language_type_rec*)uvector_base<24, 8, vector_base<allocator_t<24, 8>, unsigned short>>::begin(&v1->_language_types);
    _shell_sort<language_type_rec, __int64, less<language_type_rec>>(v23, (v22 - (char*)v23) / 24, &pred);
    if (uvector_base<24, 8, vector_base<allocator_t<24, 8>, unsigned short>>::empty(&v1->_language_types))
        debug::fail((debug*)"!_language_types.empty()", "engine.cpp", "cengine::create", 1555);
    p_text_languages = &v1->_text_languages;
    v61 = &v1->_text_languages;
    v25 = type_info::_name_internal_method((type_info*)&str_shared `RTTI Type Descriptor', &__type_info_root_node);
        v26 = &v1->_text_languages.threading_rw_check;
    v27 = v25;
    if (v1 == (cengine*)-128LL)
        v26 = 0;
    threading_rw_check::write_lock::write_lock(&v64, v26);
    uvector_base<8, 8, vector_base<allocator_t<8, 8>, unsigned short>>::inner_release<str_shared>(
        &v1->_text_languages,
        0,
        0,
        v27);
    threading_rw_check::write_lock::~write_lock(&v64);
    v28 = 0;
    v54 = 0;
    v59 = uvector_base<24, 8, vector_base<allocator_t<24, 8>, unsigned short>>::size(&v1->_language_types);
    if (v59)
    {
        v29 = 0;
        v51 = 0;
        do
        {
            v31 = (char*)p_language_types->_array + v29;
            if (v30)
            {
                if (v30->_state.writers)
                {
                    while (_InterlockedCompareExchange(*(volatile signed __int32**)&threading::thread_check_lock._lock, -1, 0))
                        ;
                    __debugbreak();
                    if (v30->_state.writers)
                    {
                        LODWORD(lua_sectiona) = 81;
                        debug::fail(
                            (debug*)"0 == _t->_state.writers",
                            "read exit when writing is active",
                            "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                            "threading_rw_check::read_lock::~read_lock",
                            lua_sectiona,
                            lua_section2a);
                    }
                    threading::thread_check_lock._lock = 0;
                    v29 = v51;
                }
                _InterlockedDecrement(&v30->_state.readers);
            }
            if (v31[16])
            {
                if (p_language_types)
                {
                    v32 = &p_language_types->threading_rw_check;
                    if (p_language_types != (u_vector<language_type_rec, allocator_t, unsigned short> *) - 16LL)
                    {
                        _InterlockedIncrement(&v32->_state.readers);
                        if (p_language_types->_state.writers)
                        {
                            while (_InterlockedCompareExchange(
                                *(volatile signed __int32**)&threading::thread_check_lock._lock,
                                -1,
                                0))
                                ;
                            __debugbreak();
                            if (p_language_types->_state.writers)
                            {
                                LODWORD(lua_sectiona) = 72;
                                debug::fail(
                                    (debug*)"0 == _t->_state.writers",
                                    "cannot read when writing is active",
                                    "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                                    "threading_rw_check::read_lock::read_lock",
                                    lua_sectiona,
                                    lua_section2a);
                            }
                            threading::thread_check_lock._lock = 0;
                        }
                    }
                }
                else
                {
                    v32 = 0;
                }
                if (v54 >= p_language_types->_size)
                    debug::fail(
                        (debug*)"id < count()",
                        "d:\\trunk\\src\\ucore\\u_vector_base.h",
                        "uvector_base<24,8,struct vector_base<class allocator_t<24,8>,unsigned short> >::unsafe_at",
                        76);
                v33 = (const str_shared*)((char*)p_language_types->_array + v51);
                if (v32)
                {
                    if (v32->_state.writers)
                    {
                        while (_InterlockedCompareExchange(*(volatile signed __int32**)&threading::thread_check_lock._lock, -1, 0))
                            ;
                        __debugbreak();
                        if (v32->_state.writers)
                        {
                            LODWORD(lua_sectiona) = 81;
                            debug::fail(
                                (debug*)"0 == _t->_state.writers",
                                "read exit when writing is active",
                                "d:\\trunk\\src\\ucore\\threading_rw_check.h",
                                "threading_rw_check::read_lock::~read_lock",
                                lua_sectiona,
                                lua_section2a);
                        }
                        threading::thread_check_lock._lock = 0;
                    }
                    _InterlockedDecrement(&v32->_state.readers);
                }
                v34 = type_info::_name_internal_method((type_info*)&str_shared `RTTI Type Descriptor', &__type_info_root_node);
                    uvector_base<8, 8, vector_base<allocator_t<8, 8>, unsigned short>>::push_back<str_shared, _nonrelocable_ptr__tag>(
                        v61,
                        v33 + 1,
                        0,
                        0,
                        v34);
                v29 = v51;
            }
            v29 += 24;
            v28 = v54 + 1;
            v51 = v29;
            v54 = v28;
        } while (v28 != v59);
        v1 = v63;
        p_text_languages = v61;
    }
    array = (str_shared*)p_text_languages->_array;
    v37 = str_shared::c_str(array);
    str_shared::str_shared(&l, v37, 0);
    if (v1->_lang_text.p_ != l.p_)
    {
        if (cengine::is_lang_supported(v1, &l))
        {
            v1->_changing_stable = 1;
            str_shared::operator=(&v1->_lang_text, &l);
            v38 = *(u8*)v37 - (u8)aJp[0];
            if (!v38)
            {
                v38 = *((u8*)v37 + 1) - (u8)aJp[1];
                if (!v38)
                    v38 = *((u8*)v37 + 2) - (u8)aJp[2];
            }
            v1->_ideogram = v38 == 0;
            new_stable = engine._new_stable;
            if (engine._new_stable)
            {
                v40 = engine._new_stable;
                engine._new_stable = 0;
                localization::stable::~stable(v40);
                _mm_prefetch((const char*)&new_stable[-1]._lang.p_ + 4, 2);
                v41 = memory();
                u_memory::main_realloc(v41, new_stable, 0, 4u, "C++ delete", 0);
                if (engine._new_stable)
                    debug::fail((debug*)"0 == engine._new_stable", "engine.cpp", "cengine::lang_text", 581);
            }
            v42 = type_info::_name_internal_method(
                (type_info*)&load_new_stable_wi `RTTI Type Descriptor',
                & __type_info_root_node);
            v43 = memory();
            v44 = (load_new_stable_wi*)u_memory::main_realloc(v43, 0, 0x28u, 8u, v42, 0);
            if (v44)
            {
                load_new_stable_wi::load_new_stable_wi(v44, &engine._new_stable);
                v5 = v45;
            }
            streaming::server::add(g_streamer, v5);
        }
        else
        {
            rlog("Unsupported language: %s", v37);
        }
    }
    R_ASSERT(lang_text().size());
    flags_ready = true;
}