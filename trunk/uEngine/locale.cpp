#include "stdafx.h"
#include "locale.h"

locale::locale(u32 number, locale::locale_type type)
{
    R_ASSERT(type == LT_NUMBER);
    R_ASSERT(number < 1<<30);

    _type = type;
    _exist = 1;
    _number = number;
    _key = "";
}

locale::locale(const locale* other)
{
    _key = other->_key;
    _type = other->_type;
    _number = other->_number;
    _exist = other->_exist;
}

locale::locale(const str_shared key)
{
    _key = key;
    _type = LT_KEY;
    _exist = false;
}

locale::locale(const str_shared key, locale_type type, bool extended_stable)
{
    int v7; // [rsp+20h] [rbp-8h]

    _key = key;
    R_ASSERT(type < LT_COUNT && LT_COUNT < 1<<4);
    _exist = false;
    _type = type;
}

bool locale::exist()
{
	return _exist;
}

locale* locale::key()
{
	return this;
}

locale::locale_type locale::type()
{
    return _type;
}

// WTF?
void locale::trace(const str_shared key)
{
    ;
}

str_shared locale::value(str_shared result, int ext)
{
    unsigned int v6; // r8d
    localization::stable* v7; // rbp
    const str_shared* v8; // rax
    locale* v9; // rdx
    const char* v10; // rax
    str_shared* v11; // rax
    const str_shared* v12; // r14
    str_shared* v13; // rax
    const char* v14; // r14
    const char* v15; // rax
    str_shared* v16; // rax
    const char* v18; // rax
    str_shared* v19; // rax
    const char* v20; // rax
    const struct str_shared* v21; // rax
    const char* v22; // rdi
    unsigned int v23; // eax
    wchar_t* v24; // rdx
    __int64 v25; // r8
    wchar_t v26; // cx
    unsigned __int64 v27; // rax
    str_shared resulta; // [rsp+20h] [rbp-A78h] BYREF
    str_shared v29; // [rsp+28h] [rbp-A70h] BYREF
    str_shared v30; // [rsp+30h] [rbp-A68h] BYREF
    str_shared v31; // [rsp+38h] [rbp-A60h] BYREF
    char console_key[272]; // [rsp+40h] [rbp-A58h] BYREF
    char build_key[272]; // [rsp+150h] [rbp-948h] BYREF
    wchar_t buffer[1024]; // [rsp+260h] [rbp-838h] BYREF

    str_shared res;
    _exist = false;
    if (str_shared::size(&this->_key) || (*((_BYTE*)this + 8) & 7) == 5)
    {
        v6 = *((_DWORD*)this + 2);
        v7 = localization::g_stable;
        if (ext)
            v7 = localization::g_stable_ext;

        switch (_type)
        {
        case 0:
        case 3:
            goto $LN18;
        case 1:
            if (gamepad_mode())
            {
                v10 = str_shared::c_str(&this->_key);
                if (strcpy_s(console_key, 0x104u, v10))
                    debug::fail((debug*)"0==strcpy_s(dest,sz,src)", "d:\\trunk\\src\\ucore\\libstr.h", "sz_cpy", 37);
                if (strcat_s(console_key, 0x104u, "_xbox"))
                    debug::fail(
                        (debug*)"0==strcat_s(sz_cpy(dest,sz,S1),sz,S2)",
                        "d:\\trunk\\src\\ucore\\libstr.h",
                        "sz_concat",
                        70);
                v11 = (str_shared*)str_shared::str_shared(&v29, console_key, 0);
                v12 = localization::stable::translate(v7, v11, &this->_exist);
                str_shared::~str_shared(&v29);
                if (this->_exist)
                    goto LABEL_23;
            }
            v8 = localization::stable::translate(v7, &this->_key, &this->_exist);
            if (this->_exist)
                goto LABEL_8;
            v13 = (str_shared*)u_core::build_key((u_core*)core.application_name.ptr);
            v14 = str_shared::c_str(v13);
            v15 = str_shared::c_str(&this->_key);
            if (strcpy_s(build_key, 0x104u, v15))
                debug::fail((debug*)"0==strcpy_s(dest,sz,src)", "d:\\trunk\\src\\ucore\\libstr.h", "sz_cpy", 37);
            if (strcat_s(build_key, 0x104u, "_"))
                debug::fail(
                    (debug*)"0==strcat_s(sz_cpy(dest,sz,S1),sz,S2)",
                    "d:\\trunk\\src\\ucore\\libstr.h",
                    "sz_concat",
                    70);
            if (strcat_s(build_key, 0x104u, v14))
                debug::fail((debug*)"0==strcat_s(dest,sz,S1)", "d:\\trunk\\src\\ucore\\libstr.h", "sz_cat", 59);
            v16 = (str_shared*)str_shared::str_shared(&v30, build_key, 0);
            v12 = localization::stable::translate(v7, v16, &this->_exist);
            str_shared::~str_shared(&v30);
            if (!_exist)
                goto LABEL_35;
        LABEL_23:
            v9 = (locale*)v12;
            goto LABEL_24;
        case 2:
            v18 = str_shared::c_str(&this->_key);
            if (strcpy_s(console_key, 0x104u, v18))
                debug::fail((debug*)"0==strcpy_s(dest,sz,src)", "d:\\trunk\\src\\ucore\\libstr.h", "sz_cpy", 37);
            if (strcat_s(console_key, 0x104u, "_xbox"))
                debug::fail(
                    (debug*)"0==strcat_s(sz_cpy(dest,sz,S1),sz,S2)",
                    "d:\\trunk\\src\\ucore\\libstr.h",
                    "sz_concat",
                    70);
            v19 = (str_shared*)str_shared::str_shared(&v31, console_key, 0);
            v12 = localization::stable::translate(v7, v19, &this->_exist);
            str_shared::~str_shared(&v31);
            if (this->_exist)
                goto LABEL_23;
        $LN18:
            v8 = localization::stable::translate(v7, &this->_key, &this->_exist);
            if (this->_exist)
            {
            LABEL_8:
                v9 = (locale*)v8;
            LABEL_24:
                str_shared::str_shared(result, &v9->_key);
            }
            else
            {
            LABEL_35:
                v22 = str_shared::c_str(&this->_key);
                v23 = str_shared::size(&this->_key);
                if (v23)
                {
                    v24 = buffer;
                    v25 = v23;
                    do
                    {
                        v26 = *v22;
                        ++v24;
                        ++v22;
                        *(v24 - 1) = v26;
                        --v25;
                    } while (v25);
                }
                v27 = v23;
                if (v27 >= 1024)
                    _report_rangecheckfailure();
                buffer[v27] = 0;
                localization::stable::encode(v7, result, buffer);
            }
            break;
        case 4:
            _exist = true;
            v20 = str_shared::c_str(&this->_key);
            v21 = subtitle_manager::translate_name(&resulta, v20, v7);
            str_shared::str_shared(result, v21);
            str_shared::~str_shared(&resulta);
            return result;
        case 5:
            _exist = true;
            subtitle_manager::translate_number(subtitle_manager::_instance, res, v6 >> 3);
            return result;
        case 6:
            this->_exist = 1;
            v9 = this;
            goto LABEL_24;
        default:
            goto LABEL_35;
        }
    }
    else
    {
        res = "";
    }
    return res;
}