#include "stdafx.h"
#include "vfs_streams.h"

vfs::reader_base::reader_base(const u8* data, u32 size, u32 iterpos)
{
    __data = data;
    __pos = 0;
    __size = size;
    __iterpos = iterpos;
}

vfs::reader_base::reader_base()
{
    __data = nullptr;
    __pos = 0;
    __iterpos = 0;
}

void vfs::reader_base::init(const vfs::reader_base* from)
{
    __pos = from->__pos;
    __data = from->__data;
    __iterpos = from->__iterpos;
}

void vfs::reader_base::init(const u8* data, u32 size, u32 iterpos, u32 pos)
{
    __data = data;
    __size = size;
    __pos = pos;
    __iterpos = iterpos;
}

void vfs::reader_base::advance(int cnt)
{
    __pos += cnt;
    R_ASSERT((__pos<=__size) && (__pos>=0));
}

u64 vfs::reader_base::advance_term_string()
{
    u32 sz = 0;
    char* src = (char*)__data;
    while (!eof()) 
    {
        __pos++;
        sz++;
        if (!eof() && is_term(src[__pos])) 
        {
            while (!eof() && is_term(src[__pos])) __pos++;
            break;
        }
    }
    return sz;
}

u64 vfs::reader_base::chunk_find(u32 ID)
{
    u32	dwSize, dwType;
    rewind();
    while (!eof())
    {
        dwType = r_u32();
        dwSize = r_u32();

        if (dwType == ID)
            return dwSize;

        advance(dwSize);
    }
    return 0;
}

const u8* vfs::reader_base::current_pointer()
{
    return &__data[__pos];
}

const u8* vfs::reader_base::data()
{
    return __data;
}

u32 vfs::reader_base::elapsed()
{
    return __size - __pos;
}

bool vfs::reader_base::eof()
{
    return __size - __pos <= 0;
}

u32 vfs::reader_base::length()
{
    return __size;
}

void vfs::reader_base::r(void* p, int c)
{
    R_ASSERT(__pos+c<=__size);
    memcpy(p, &__data[__pos], c);
    advance(c);
}

float vfs::reader_base::r_angle16()
{
    return r_fp32_q16(0.0f, PI_MUL_2);
}

float vfs::reader_base::r_angle8()
{
    return r_fp32_q8(0.0f, PI_MUL_2);
}

void vfs::reader_base::r_dir(Fvector3* A)
{
    u16 t = r_u16();
    Fvector3 r;
    Fvector3 d = compression::decompress::normal(&r, &t);

    A->x = d.x;
    A->z = d.z;
}

float vfs::reader_base::r_fp32()
{
    float tmp;
    r(&tmp, sizeof(tmp));
    return tmp;
}

float vfs::reader_base::r_fp32_q16(float min, float max)
{
    u16	val = r_u16();
    float A = (float(val) * (max - min)) / 65535.f + min;		// floating-point-error possible
    VERIFY((A >= min - EPS_S) && (A <= max + EPS_S));
    return A;
}

float vfs::reader_base::r_fp32_q8(float min, float max)
{
    u8 val = r_u8();
    float	A = (float(val) / 255.0001f) * (max - min) + min;	// floating-point-error possible
    VERIFY((A >= min) && (A <= max));
    return	A;
}

void vfs::reader_base::r_matrix(Fmatrix& M)
{
    r(&M, sizeof(M));
}

void vfs::reader_base::r_matrix(Fmatrix& M)
{
    r(&M, sizeof(M));
}

void vfs::reader_base::r_matrix_43T(Fmatrix43& M)
{
    r(&M, sizeof(M));
}

s8 vfs::reader_base::r_s8()
{
    s8 tmp;
    r(&tmp, sizeof(tmp));
    return tmp;
}

s16 vfs::reader_base::r_s16()
{
    s16 tmp;
    r(&tmp, sizeof(tmp));
    return tmp;
}

s32 vfs::reader_base::r_s32()
{
    s32 tmp;
    r(&tmp, sizeof(tmp));
    return tmp;
}

s64 vfs::reader_base::r_s64()
{
    s64 tmp;
    r(&tmp, sizeof(tmp));
    return tmp;
}

void vfs::reader_base::r_string(char* dest, u32 tgt_sz)
{
    char* src = (char*)__data + __pos;
    u32 sz = advance_term_string();
    R_ASSERT2(sz < (tgt_sz - 1), "Dest string less than needed.");
    strncpy(dest, src, sz);
    dest[sz] = 0;
}

void vfs::reader_base::r_string(u_string& dest)
{
    char* src = (char*)__data + __pos;
    u32 sz = advance_term_string();
    dest.assign(src, sz);
}

void vfs::reader_base::r_stringz(char* dest, u32 tgt_sz)
{
    char* src = (char*)__data;
    u32 sz = strlen(src);
    R_ASSERT2(sz < tgt_sz, "Dest string less than needed.");
    while ((src[__pos] != 0) && (!eof())) *dest++ = src[__pos++];
    *dest = 0;
    __pos++;
}

void vfs::reader_base::r_stringz(str_shared& dest, str_type type)
{
    dest = (char*)(__data + __pos);
    __pos += (dest.size() + 1);
}

void vfs::reader_base::r_stringz(u_string& dest)
{
    dest.assign((char*)(__data + __pos));
    __pos += int(dest.size() + 1);
};

u8 vfs::reader_base::r_u8()
{
    u8 tmp;
    r(&tmp, sizeof(tmp));
    return tmp;
}

u16 vfs::reader_base::r_u16()
{
    u16 tmp;
    r(&tmp, sizeof(tmp));
    return tmp;
}

u32 vfs::reader_base::r_u32()
{
    u32 tmp;
    r(&tmp, sizeof(tmp));
    return tmp;
}

u64 vfs::reader_base::r_u64()
{
    u64 tmp;
    r(&tmp, sizeof(tmp));
    return tmp;
}

void vfs::reader_base::r_vec2f(Fvector2* v)
{
    r(&v, sizeof(Fvector2));
}

void vfs::reader_base::r_vec2i(Ivector2* v)
{
    r(&v, sizeof(Ivector2));
}

void vfs::reader_base::r_vec3f(Fvector3* v)
{
    r(&v, sizeof(Fvector3));
}

void vfs::reader_base::r_vec3i(Ivector3* v)
{
    r(&v, sizeof(Ivector3));
}

void vfs::reader_base::r_vec4f(Fvector4* v)
{
    r(&v, sizeof(Fvector4));
}

void vfs::reader_base::r_vec4i(Ivector4* v)
{
    r(&v, sizeof(Ivector4));
}

void vfs::reader_base::rewind()
{
    seek(0);
}

void vfs::reader_base::seek(u32 ptr)
{
    __pos = ptr;
    R_ASSERT((__pos <= __size) && (__pos >= 0));
}

void vfs::reader_base::skip_stringz()
{
    char* src = (char*)__data;
    while ((src[__pos] != 0) && (!eof())) __pos++;
    __pos++;
}

u32 vfs::reader_base::tell()
{
    return __pos;
}

vfs::ireader::ireader()
{
    __data = 0;
    __pos = 0;
    __iterpos = 0;
}

vfs::ireader::ireader(const u8* data, u32 size, u32 iterpos)
{
    __pos = 0;
    __data = data;
    __size = size;
    __iterpos = iterpos;
}

vfs::ireader::ireader(const vfs::ireader* other)
{
    __pos = other->__pos;
    __data = other->__data;
    __iterpos = other->__iterpos;
}

vfs::ireader* vfs::ireader::chunk_open(vfs::ireader* result, u32 ID)
{
    u64 sz = chunk_find(ID);
    if (sz)
    {
        return new vfs::ireader(__data, __size, __iterpos);
    }
    return nullptr;
}

vfs::ireader* vfs::ireader::chunk_open_at_current_position(vfs::ireader* result, u32 ID)
{
    u32 type = r_u32();
    R_ASSERT2(!ID || type == ID, "incorrent chunk ID");
    u32 sz = r_u32();
    if (sz)
    {
        vfs::ireader* reader = new vfs::ireader(__data, sz, __pos + __size);
        advance(sz);
        return reader;
    }
    return nullptr;
}

u32 vfs::ireader::chunk_open_at_current_position(u32 ID, vfs::ireader* result)
{
    u32 type = r_u32();
    R_ASSERT2(!ID || type == ID, "incorrect chunk ID");

    u32 sz = r_u32();
    if (!sz)
        return 0;

    if (!result)
    {
        result = new vfs::ireader();
    }
    result->__size = sz;
    result->__pos = 0;
    result->__data = &__data[__pos];
    result->__iterpos = __pos + sz;
    advance(sz);
    return sz;
}

vfs::ireader* vfs::ireader::chunk_open_iterator(vfs::ireader* result, u32* ID, vfs::ireader* prev)
{
    seek(sizeof(void*));
    if (!eof())
    {
        u32 sz = r_u32();
        vfs::ireader* r = new vfs::ireader(__data, sz, __pos + sz);
        return r;
    }
    return result;
}

bool vfs::ireader::chunk_open_try_current(u32 ID, vfs::ireader* result)
{
    int v16; // eax

    u32 type = r_u32();
    u32 sz;
    if (type == ID)
    {
        sz = r_u32();
    }
    else
    {
        sz = chunk_find(ID);
    }

    if (!sz)
        return false;

    if (!result)
    {
        result = new vfs::ireader();
    }
    result->__size = sz;
    result->__pos = 0;
    result->__data = __data;
    result->__iterpos = __pos + sz;
    return true;
}

bool vfs::ireader::chunk_read(u32 ID, void* dest, u32 dest_size)
{
    u32 sz = chunk_find(ID);
    if (!sz || sz > dest_size)
        return false;

    r(dest, sz);
    return true;
}

u64 vfs::ireader::chunk_skip_at_current_position(u32 ID)
{
    u32 type = r_u32();

    R_ASSERT2(!ID || type == ID, "incorrect chunk ID");

    u32 sz = r_u32();
    return sz;
}

u64 vfs::ireader::chunk_test(u32 ID)
{
    rewind();
    if (eof())
        return 0;

    u32 sz = 0;
    while (TRUE)
    {
        u32 id = r_u32();

        sz = r_u32();
        if (id == ID)
            break;

        return 0;
    }
    return sz;
}

vfs::ireader* vfs::ireader::chunk_try_open_at_current_position(vfs::ireader* result, u32 ID)
{
    u32 type = r_u32();
    if (type != ID)
        return nullptr;

    {
        u32 sz = r_u32();
        if (sz)
        {
            vfs::ireader* res = new vfs::ireader(__data, sz, __pos + sz);
            return res;
        }
    }
    return nullptr;
}

bool vfs::ireader::chunk_try_open_at_current_position(u32 ID, u32* size)
{
    u32 type = r_u32();

    if (type != ID)
        return false;

    *size = r_u32();
    return true;
}

bool vfs::ireader::chunk_try_open_at_current_position(u32 ID, vfs::ireader* result)
{
    u32 id_search = r_u32();
    if (id_search != ID)
        return false;

    u32 sz = r_u32();
    if (eof())
        return false;

    if (!result)
    {
        result = new vfs::ireader();
    }
    result->__size = sz;
    result->__pos = 0;
    result->__data = __data;
    result->__iterpos = __pos + sz;
    return true;
}