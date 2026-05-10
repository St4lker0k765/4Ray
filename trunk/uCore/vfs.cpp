#include "stdafx.h"
#include "vfs.h"

u64 vfs::exist_os_path(const char* path, const int file, u32* size)
{
    R_ASSERT(!file || !size);

    _finddata_t r;
    intptr_t ff = _findfirst(path, &r);
    u64 time_write = 0;
    if (ff != -1 && (file != 0) == ((r.attrib & _A_SUBDIR) == 0))
    {
        if (size)
            *size = r.size;
        time_write = r.time_write;
    }
    _findclose(ff);
    return time_write;
}

void vfs::add_extension(char** dst, string1024* exts, const char* info, char* ext)
{
    rsize_t sz = sz_len(ext) + 4 + sz_len(info);

    sz_concat(*dst, sz, info, " (", ext, ")");

    if (exts)
    {
        LPCSTR any = "*.*";
        for (int i = 0; i < sz_len(any); i++)
        {
            if (strcmp(ext, "*.*"))
                break;
        }
        if (sz_len(*exts))
             sz_cat((char*)exts, sizeof(exts), ";");
        sz_cat((char*)exts, sizeof(exts), ext);
    }
}

const char* vfs::make_filter(string1024* dest, const char* info, char* ext)
{
    char* v3; // rbx
    char* v7; // rcx
    char v8; // al
    char* dst[2]; // [rsp+20h] [rbp-838h] BYREF
    string1024 buf;
    char exts[1024]; // [rsp+430h] [rbp-428h] BYREF

    v3 = ext;
    (*dest)[0] = 0;
    if (ext)
    {
        dst[0] = (char*)dest;
        buf[0] = 0;
        exts[0] = 0;
        char* v6 = strstr(ext, ";");
        memset(buf, 0, sizeof(buf));
        while (v6)
        {
            strncpy(buf, ext, v6 - ext);
            if (*v6 == 59)
                ++v6;
            vfs::add_extension(dst, &exts, info, buf);
            v3 = v6;
            v6 = strstr(v6, ";");
            memset(buf, 0, sizeof(buf));
        }
        v7 = (char*)(buf - ext);
        do
        {
            v8 = *v3++;
            v3[v7 - 1] = v8;
        } while (v8);
        vfs::add_extension(dst, &exts, info, buf);
        if (sz_len(exts))
            vfs::add_extension(dst, nullptr, "All Formats", exts);
    }
    return *dest;
}

u64 vfs::get_open_filename(
    const char* title,
    const char* init_dir,
    char* buffer,
    int sz_buffer,
    const char* mask,
    bool bMulti)
{
    R_ASSERT(buffer && (sz_buffer > 0));

    string1024 flt;
    vfs::make_filter(&flt, EXTENSION_DESCRIPTION, mask);

    tagOFNA ofn;
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = GetForegroundWindow();
    ofn.lpstrDefExt = mask;
    ofn.lpstrFile = buffer;
    ofn.lpstrFilter = flt;
    ofn.nMaxFile = sz_buffer;
    ofn.nFilterIndex = sz_item_count(mask, ';') + 1;
    ofn.lpstrTitle = title;
    ofn.lpstrInitialDir = init_dir;
    ofn.FlagsEx = OFN_EX_NOPLACESBAR;
    ofn.Flags = (bMulti ? OFN_EXPLORER | OFN_ALLOWMULTISELECT : 0) | OFN_NOVALIDATE | OFN_ENABLETEMPLATEHANDLE;
    u32 OpenFileNameA = GetOpenFileNameA(&ofn);
    if (!OpenFileNameA)
    {
        rlog("GetOpenFileName failed: %d", CommDlgExtendedError());
    }
    _strlwr(buffer);
    return OpenFileNameA;
}

u64 vfs::get_save_filename(
    const char* title,
    const char* init_dir,
    char* buffer,
    int sz_buffer,
    const char* mask)
{
    R_ASSERT(buffer && (sz_buffer > 0));

    string1024 flt;
    vfs::make_filter(&flt, EXTENSION_DESCRIPTION, mask);

    tagOFNA ofn;
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = GetForegroundWindow();
    ofn.lpstrDefExt = nullptr;
    ofn.lpstrFile = buffer;
    ofn.lpstrFilter = flt;
    ofn.nMaxFile = sz_buffer;
    ofn.lpstrTitle = title;
    ofn.lpstrInitialDir = init_dir;
    ofn.nFilterIndex = sz_item_count(mask, ';');
    ofn.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT;
    ofn.FlagsEx = OFN_EX_NOPLACESBAR;
    u32 SaveFileNameA = GetSaveFileNameA(&ofn);
    if (!SaveFileNameA)
    {
        rlog("GetSaveFileName failed: %d", CommDlgExtendedError());
    }
    _strlwr(buffer);
    return SaveFileNameA;
}