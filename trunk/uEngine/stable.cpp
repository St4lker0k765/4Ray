#include "stdafx.h"
#include "stable.h"
#include <algorithm>

namespace localization
{
    struct pred_freq
    {
        bool operator()
            (
            const std::pair<wchar_t, u32>* _1,
            const std::pair<wchar_t, u32>* _2)
        {
            return _1->second > _2->second;
        }
    };
}

void localization::sconverter::sort()
{
    std::sort(frequency.begin(), frequency.end(), localization::pred_freq::operator());
}

void localization::sconverter::append(wchar_t* text)
{
    const wchar_t* v4; // r14
    wchar_t v5; // r12
    unsigned int v6; // r15d
    unsigned int v7; // r14d
    threading_rw_check* v8; // rdi
    _WORD* v9; // rsi
    const char* v10; // rbx
    threading_rw_check* v11; // rdx
    int size; // edx
    _DWORD* v13; // rcx
    unsigned int v14; // ebx
    const threading_rw_check* t; // rdi
    threading_rw_check* v16; // rdi
    char* v17; // rsi
    bool v18; // zf
    char* v19; // rbx
    pair<wchar_t, unsigned int>* v20; // rax
    const char* v21; // [rsp+20h] [rbp-78h]
    int v22; // [rsp+28h] [rbp-70h]
    threading_rw_check::write_lock v23; // [rsp+38h] [rbp-60h] BYREF

    const wchar_t* texta = text;
    v4 = text;
    if (wcslen(text))
    {
        u32 v24 = wcslen(text);
        do
        {
            v5 = *v4;
            v6 = 0;
            if (frequency.size())
            {
                u32 v7 = 0;
                do
                {
                    v9 = (char*)this->frequency._array + v7;
                    if (*v9 == v5)
                        break;
                    ++v6;
                    v7 += 8;
                } while (v6 < frequency.size());
                v4 = texta;
            }
            if (v6 == (unsigned int)uvector_base<8, 4, vector_base<allocator_t<8, 4>, unsigned short>>::size(&this->frequency))
            {
                frequency.push_back(v5,);
                u64 e = v5 | 0x100000000LL;
                v10 = type_info::_name_internal_method(
                    (type_info*)&pair<wchar_t, unsigned int> `RTTI Type Descriptor',
                    & __type_info_root_node);
                if (this)
                    v11 = &this->frequency.threading_rw_check;
                else
                    v11 = 0;
                threading_rw_check::write_lock::write_lock(&v23, v11);
                size = this->frequency._size;
                if (this->frequency._capacity < (unsigned int)(size + 1))
                {
                    uvector_base<8, 4, vector_base<allocator_t<8, 4>, unsigned short>>::inner_push_back(
                        &this->frequency,
                        (size_align<1, 8, 4> *) & e,
                        v10);
                }
                else
                {
                    v13 = (char*)this->frequency._array + (unsigned int)(8 * size);
                    *v13 = v5;
                    v13[1] = 1;
                    v14 = this->frequency._size + 1;
                    if (v14 > 0xFFFF)
                        debug::fail(
                            (debug*)"new_count <= etype_max(meter_type)",
                            "d:\\trunk\\src\\ucore\\ext_stl.h",
                            "vector_base<class allocator_t<8,4>,unsigned short>::amount",
                            80);
                    this->frequency._size = v14;
                }
                t = v23._t;
            }
            else
            {
                v17 = (char*)this->frequency._array + 8 * v6;
                ++ * ((_DWORD*)v17 + 1);
            }
            ++v4;
            v18 = v24-- == 1;
            texta = v4;
        } while (!v18);
    }
    append_counter += wcslen(text);
    if (append_counter >= 0x1000)
    {
        append_counter = 0;
        sort();
    }
}

localization::stable::stable()
{
    _lang = "";
    memset(hash, 0, sizeof(hash));
}

localization::stable::stable(const localization::stable* other)
{
    charmap = other->charmap;
    if (other->hash[0])
    {
        memcpy(hash, other->hash, sizeof(hash));
    }

    storage = other->storage;
    _lang = other->_lang;
}

u32 localization::stable::extract_char(const char* text, char* dest, u32 size)
{
    char* v6; // rbx
    char v8; // cl

    u32 count = 0;
    v6 = dest;
    if (count < strlen(text))
    {
        count++;
        u64 v7 = text - dest;
        do
        {
            R_ASSERT(count < size);
            v8 = (v6++)[v7];
            *(v6 - 1) = v8;
        } while (v8 >= 224 && v6[v7]);
        *v6 = 0;
    }
    else
    {
        *dest = 0;
    }
    return count;
}

void localization::stable::load(const char* lng_id, const char* path)
{
    const char* v4; // rbx
    vfs::ireader* R; // [rsp+20h] [rbp-148h] BYREF

    const char* v4 = "content\\localization\\";
    if (path)
        v4 = path;

    rlog("begin stable::load(%s)");
    string_path dest; // [rsp+30h] [rbp-138h] BYREF
    sz_concat(dest, sizeof(dest), v4, "stable_", lng_id, ".lng");
    vfs::ropen(R, dest);
    if (R)
    {
        load(R, lng_id);
    }
    else
    {
        rlog("fail stable::load(%s)", lng_id);
    }
}

void localization::stable::unload()
{
    storage.clear_and_free();
}

str_shared localization::stable::translate(str_shared key, bool* exist)
{
    str_shared* v4; // rdi
    __int64 v6; // r8
    char* v7; // r9
    str_shared* v8; // r8
    str_shared* p; // rax
    str_shared* v10; // rdx

    v4 = key;
    if (exist)
        *exist = false;
    if (key.size())
    {
        v6 = key.crc() % STABLE_HASH_SIZE;
        v7 = (char*)this + 8 * v6;
        v8 = (str_shared*)*(v7 + 5);
        p = v8;
        v10 = v8;
        if (v8)
        {
            while (p->p_ != v4->p_)
            {
                v10 = p;
                p = (str_shared*)p[2].p_;
                if (!p)
                    return key;
            }
            if (v10 != p)
            {
                v10[2].p_ = p[2].p_;
                p[2].p_ = (str_value*)v8;
                *((_QWORD*)v7 + 5) = p;
            }
            if (exist)
                *exist = true;
            v4 = p + 1;
        }
    }
    return v4;
}

bool localization::stable::valid_char(const wchar_t C)
{
    R_ASSERT(charmap.size());
    if (!charmap.size())
        return false;

    for (u32 i = 0; i < charmap.size(); i++)
    {
        if (C == charmap[i])
        {
            return true;
        }
    }
    return false;
}

bool localization::stable::valid_string(wchar_t* text)
{
    R_ASSERT(charmap.size());

    if (!wcslen(text))
        return true;

    u32 i = 0;
    while (valid_char(text[i]))
    {
        ++i;
        if (i >= wcslen(text))
            return true;
    }
    return false;
}