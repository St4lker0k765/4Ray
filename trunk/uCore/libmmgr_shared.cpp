#include "stdafx.h"
#include "u_string.h"

UCORE_API	str_container* g_string_container = nullptr;

str_container::str_container()
{
	threading::mutex::mutex("memory::shared::string");
	buckets.clear();
	gc_bucket = 0;
	gc_iterator = 0;
	amount = 1;

	zero_len_str = new str_value();
	zero_len_str->next = buckets.front();
	zero_len_str->length = 0;
	zero_len_str->str_type = default_string;
	zero_len_str->crc = 0;
	zero_len_str->value[0] = 0;
}

str_container::str_container(const str_container* other)
{
	threading::mutex::mutex("memory::shared::string");

	_os = other->_os;
	buckets = other->buckets;
	gc_iterator = other->gc_iterator;
	gc_bucket = other->gc_bucket;
	amount = other->amount;
	zero_len_str = other->zero_len_str;
}

str_container::~str_container()
{
	clean();
	buckets.clear();
}

#define		S_HEADER		sizeof(str_value)

str_value* str_container::do_dock(
    const char* value,
    u32 s_len,
    str_type s_type)
{
    unsigned int v7; // r13d
    u_vector<str_value*, allocator_t, unsigned short>* p_buckets; // r14
    unsigned int v9; // edi
    threading_rw_check* v10; // rbx
    __int64 v11; // rcx
    unsigned __int16** v12; // rdi
    unsigned __int16* v13; // rsi
    u16* v14; // rbx
    unsigned __int16* v15; // rdi
    threading_rw_check* v16; // rdi
    threading_rw_check* v17; // rdi
    _QWORD* v18; // rsi
    __int64* v19; // rsi
    __int64 v20; // rax
    threading_rw_check* v21; // rdi
    _QWORD* v22; // rsi
    __int64 v24; // [rsp+30h] [rbp-48h]
    void* pptr; // [rsp+38h] [rbp-40h] BYREF
    unsigned int s_lena; // [rsp+90h] [rbp+18h]

    R_ASSERT(g_string_container);

	u32	s_len_with_zero = (u32)s_len + 1;
	R_ASSERT(S_HEADER + s_len_with_zero < 4096);

	str_value* result;
	result->crc = compression::crc32((u8*)value, s_len, 0xFFFFFFFF);
	result->length = s_len + 1;
	result->next = ;
	result->refs = 0;

    p_buckets = &this->buckets;
    u32 v9 = v7 % buckets.size();
    s_lena = v9;
    lock();
    gc_step();

    v11 = 8 * v9;
    v12 = (unsigned __int16**)((char*)p_buckets->_array + v11);
    v24 = v11;
    v13 = *v12;
    v14 = v13;
    v15 = *v12;
    if (v13)
    {
        while (*(v14 + 4) != v7 || v14[6] != v4 || v14[7] != s_type || memcmp(v14 + 10, value, v4))
        {
            v15 = v14;
            v14 = *(u16**)v14;
            if (!v14)
                goto LABEL_28;
        }
        if (v15 != v14)
        {
            v15 = v14;
            v14 = v13;
            v18 = (char*)p_buckets->_array + v24;
            *v18 = v14;
        }
    }
    else
    {
    LABEL_28:
        ++amount;
        suballocator::allocate(&alloc.pool, &pptr, v4 + 21);
        v14 = (unsigned __int16*)pptr;
        *((_DWORD*)pptr + 2) = 0;
        v19 = (__int64*)((char*)p_buckets->_array + v24);
        v20 = *v19;
        v14[6] = v4;
        *((_DWORD*)v14 + 4) = v7;
        *(_QWORD*)v14 = v20;
        v14[7] = s_type;
        memcpy_0(v14 + 10, value, v4);
        v14 + v4 + S_HEADER = 0;
        v22 = (char*)p_buckets->_array + v24;
        *v22 = v14;
        if (s_type)
            vfs::trace->identifier((const char*)v14 + S_HEADER, s_type);
    }
    unlock();
    return (str_value*)v14;
}

str_value* str_container::dock(str_c value)
{
	if (0 == value)				return 0;

	cs.Enter();
#ifdef DEBUG_MEMORY_MANAGER
	Memory.stat_strdock++;
#endif // DEBUG_MEMORY_MANAGER

	str_value* result = 0;

	// calc len
	u32		s_len = strlen(value);
	u32		s_len_with_zero = (u32)s_len + 1;
	VERIFY(HEADER + s_len_with_zero < 4096);

	// setup find structure
	string16	header;
	str_value* sv = (str_value*)header;
	sv->refs = 0;
	sv->length = s_len;
	sv->crc = crc32(value, s_len);

	// search
	cdb::iterator	I = container.find(sv);	// only integer compares :)
	if (I != container.end()) {
		// something found - verify, it is exactly our string
		cdb::iterator	save = I;
		for (; I != container.end() && (*I)->crc == sv->crc; ++I) {
			str_value* V = (*I);
			if (V->length != sv->length)			continue;
			if (0 != memcmp(V->value, value, s_len))	continue;
			result = V;				// found
			break;
		}
	}

	// it may be the case, string is not fount or has "non-exact" match
	if (0 == result) {
		// Insert string

		result = (str_value*)malloc(HEADER + s_len_with_zero);

		result->refs = 0;
		result->length = sv->length;
		result->crc = sv->crc;
		CopyMemory(result->value, value, s_len_with_zero);
		container.insert(result);
	}
	cs.Leave();

	return	result;
}

void str_container::clean()
{
	R_ASSERT(g_string_container);
	lock();

	for (s32 i = amount; i > 0; --i)
	{
		gc_step();
	}
	unlock();
}

void str_container::dump()
{
	FILE* F = fopen("u:\\strings.txt", "wt");
	R_ASSERT(g_string_container);
	lock();
	for (int v3 = 0; v3 < buckets.size(); v3++)
	{
		fprintf(F, "%s\n", buckets[v3].value);
	}
	fclose(F);
	unlock();
}

void str_container::integrity_type(str_type str_type)
{
	for (int i = 0; i < buckets.size(); i++)
	{
		buckets[i]->str_type == str_type;
	}
}

u64 str_container::stats()
{
#pragma todo("Implement str_allocator")
	return 0;
	//	return alloc.pool.memsize * alloc.pool.pools.size();
}

void str_container::integrity()
{
	R_ASSERT(g_string_container);
    lock();

    for (u32 v2 = 0; v2 < buckets.size(); v2++)
    {
		const char* v5 = buckets[v2].value;
		for (const char* i = *v5; i; i = *(const char**)i)
		{
			compression::crc32((u8*)(i + 20), strlen(v5), 0xFFFFFFFF);
		}
    }

	for (u16 j = object_clsid; j < type_count; ++j)
	{
		integrity_type((str_type)j);
	}
    unlock();
}