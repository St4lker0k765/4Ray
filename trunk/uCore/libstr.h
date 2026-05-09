#pragma once

inline char* sz_cat(char* dest, rsize_t sz, const char* S1)
{
    VERIFY(0==strcat_s(dest,sz,S1));
    return dest;
}

inline void sz_change_symbol(char* name, char src, char dst)
{
    for (; *name; ++name)
    {
        if (*name == src)
            *name = dst;
    }
}

inline char* sz_chr(char* S, char C)
{
    return strchr(S, C);
}

inline const char* sz_chr(const char* S, char C)
{
    return strchr(S, C);
}

char* sz_item(const char* src, u32 index, char* dst, int dst_sz, char separator, const char* def, bool trim)
{
    u32 v7 = 0;
    if (index)
    {
        while (1)
        {
            const char* v12 = strchr(src, separator);
            if (!v12)
                break;
            ++v7;
            src = v12 + 1;
            if (v7 >= index)
            {
                if (src)
                {
                    sz_copy_first(src, dst, dst_sz, separator);
                    if (trim)
                        sz_trim(dst);

                    return dst;
                }
            }
        }
    }
    else
    {
        if (src)
        {
            sz_copy_first(src, dst, dst_sz, separator);
            if (trim)
                sz_trim(dst);

            return dst;
        }
    }
    sz_cpy(dst, dst_sz, def);
    if (trim)
        sz_trim(dst);
    return dst;
}

inline char* sz_cpy(char* dest, rsize_t sz, const char* src)
{
    VERIFY(0==strcpy_s(dest,sz,src));
    return dest;
}

inline char* sz_ncpy(char* dest, rsize_t sz, const char* src, unsigned int count)
{
    VERIFY(0==strncpy_s(dest,sz,src,(size_t)count));
    return dest;
}

inline const char* sz_token_name(const u_token* tokens, int key)
{
    for (int k = 0; tokens[k].name; k++)
    {
        if (key == tokens[k].id)
        {
            return tokens[k].name;
        }
    }
    return nullptr;
}

inline u_string sz_trim(u_string src)
{
    return src.trim();
}