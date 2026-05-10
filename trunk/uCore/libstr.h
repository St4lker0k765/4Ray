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

const char* sz_item_pos(const char* src, u32 pos, char separator)
{
    const char*	res = src;
    u32		p = 0;
    while ((p < pos) && (0 != (res = strchr(res, separator))))
    {
        res++;
        p++;
    }
    return		res;
}

const char* sz_copy_first(const char* src, char* dst, rsize_t dst_sz, char separator)
{
    LPCSTR	p;
    size_t	n;
    p = strchr(src, separator);
    n = (p > 0) ? (p - src) : sz_len(src);
    sz_ncpy(dst, dst_sz, src, n);
    dst[n] = 0;
    return		dst;
}

char* sz_item(const char* src, u32 index, char* dst, int dst_sz, char separator, const char* def, bool trim)
{
    LPCSTR ptr = sz_item_pos(src, index, separator);
    if (ptr)	sz_copy_first(ptr, dst, dst_sz, separator);
    else	strcpy(dst, def);
    if (trim)	sz_trim(dst);
    return		dst;
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

inline size_t sz_len(const char* S)
{
    return strlen(S);
}

inline int sz_item_count(const char* src, char separator)
{
    rsize_t	cnt = 0;
    if (src && src[0]) {
        const char*	res = src;
        const char*	last_res = res;
        while (0 != (res = strchr(res, separator)))
        {
            res++;
            last_res = res;
            cnt++;
            if (res[0] == separator) break;
        }
        if (sz_len(last_res)) cnt++;
    }
    return		cnt;
}

inline u_string sz_trim(u_string src)
{
    return src.trim();
}

inline const char* sz_concat(char* dest, rsize_t dest_sz, const char* S1, const char* S2)
{
    rsize_t l1 = sz_len(S1);
    sz_cpy(dest, dest_sz, S1);
    sz_cat(dest, dest_sz - l1, S2);
    return dest;
}

// dest = S1+S2+S3
inline const char* sz_concat(char* dest, rsize_t dest_sz, const char* S1, const char* S2, const char* S3)
{
    rsize_t l1 = sz_len(S1);
    rsize_t l2 = sz_len(S2);
    sz_cpy(dest, dest_sz, S1);
    sz_cat(dest, dest_sz - l1, S2);
    sz_cat(dest, dest_sz - l1 - l2, S3);

    return dest;
}

// dest = S1+S2+S3+S4
inline const char* sz_concat(char* dest, rsize_t dest_sz, const char* S1, const char* S2, const char* S3, const char* S4)
{
    rsize_t l1 = sz_len(S1);
    rsize_t l2 = sz_len(S2);
    rsize_t l3 = sz_len(S3);
    strcpy_s(dest, dest_sz, S1);
    strcat_s(dest, dest_sz - l1, S2);
    strcat_s(dest, dest_sz - l1 - l2, S3);
    strcat_s(dest, dest_sz - l1 - l2 - l3, S4);

    return dest;
}

// dest = S1+S2+S3+S4+S5
inline const char* sz_concat(char* dest, rsize_t dest_sz, const char* S1, const char* S2, const char* S3, const char* S4, const char* S5)
{
    rsize_t l1 = sz_len(S1);
    rsize_t l2 = sz_len(S2);
    rsize_t l3 = sz_len(S3);
    rsize_t l4 = sz_len(S4);
    sz_cpy(dest, dest_sz, S1);
    sz_cat(dest, dest_sz - l1, S2);
    sz_cat(dest, dest_sz - l1 - l2, S3);
    sz_cat(dest, dest_sz - l1 - l2 - l3, S4);
    sz_cat(dest, dest_sz - l1 - l2 - l3 - l4, S5);

    return dest;
}

// dest = S1+S2+S3+S4+S5+S6
inline const char* sz_concat(char* dest, rsize_t dest_sz, const char* S1, const char* S2, const char* S3, const char* S4, const char* S5, const char* S6)
{
    rsize_t l1 = sz_len(S1);
    rsize_t l2 = sz_len(S2);
    rsize_t l3 = sz_len(S3);
    rsize_t l4 = sz_len(S4);
    rsize_t l5 = sz_len(S5);
    sz_cpy(dest, dest_sz, S1);
    sz_cat(dest, dest_sz - l1, S2);
    sz_cat(dest, dest_sz - l1 - l2, S3);
    sz_cat(dest, dest_sz - l1 - l2 - l3, S4);
    sz_cat(dest, dest_sz - l1 - l2 - l3 - l4, S5);
    sz_cat(dest, dest_sz - l1 - l2 - l3 - l4 - l5, S6);

    return dest;
}
