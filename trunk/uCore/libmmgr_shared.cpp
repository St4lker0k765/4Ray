#include "stdafx.h"
#include "u_string.h"

UCORE_API	str_container* g_string_container = nullptr;
UCORE_API smem_container* g_shared_memory_container = nullptr;

str_container::str_container()
{
	threading::mutex::mutex("memory::shared::string");
	buckets.clear();
	gc_bucket = 0;
	gc_iterator = 0;
	amount = 1;

	zero_len_str = new str_value();
	zero_len_str->next = nullptr;
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

str_value* str_container::do_dock(str_c value, u32 s_len, str_type s_type)
{
    lock();

	str_value* result = 0;

	u32	s_len_with_zero = (u32)s_len + 1;
	R_ASSERT(S_HEADER + s_len_with_zero < 4096);

	char header[24];
	str_value* sv = (str_value*) header;
	sv->crc = compression::crc32((u8*)value, s_len, 0xFFFFFFFF);
	sv->length = s_len + 1;
	sv->next = buckets.front();
	sv->str_type = s_type;
	sv->refs = 0;

    gc_step();

	for (int i = 0; i < buckets.size(); i++)
	{
		str_value* temp = buckets[i];
		if (!temp)
			continue;

		if (sv->crc != temp->crc)
			continue;

		if (sv->length != temp->length)
			continue;

		if (sv->str_type != temp->str_type)
			continue;

		if (!memcmp(temp->value, value, s_len))
			continue;

		result = temp;
		break;
	}

	if (!result)
	{
		result = (str_value*)malloc(S_HEADER+s_len_with_zero);

		result->crc = sv->crc;
		result->length = sv->length;
		result->next = sv->next;
		result->str_type = sv->str_type;
		result->refs = 0;

		memcpy(result->value, value, s_len_with_zero);

		buckets.push_back(result);
	}

    unlock();
	return result;
}

str_value* str_container::dock(str_c value, u32 len, str_type s_type)
{
	if (!value)
		return 0;

	if (len)
		return do_dock(value, len, s_type);

	return zero_len_str;
}

str_value* str_container::dock(str_c value, str_type s_type)
{
	if (*value)
	{
		return do_dock(value, strlen(value), s_type);
	}
	return zero_len_str;
}

str_value* str_container::dock_crc(u32 s_crc, str_type s_type)
{
	for (int i = 0; i < buckets.size(); i++)
	{
		str_value* temp = buckets[i];
		if (!temp)
			continue;

		if (temp->str_type != s_type)
			continue;

		if (temp->crc == s_crc)
		{
			return temp;
		}
	}
	return zero_len_str;
}

void str_container::clean()
{
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
#pragma todo("Determine meaning of this function")
	/*
	for (int i = 0; i < buckets.size(); i++)
	{
		buckets[i]->str_type == str_type;
	}*/
}

u64 str_container::stats()
{
#pragma todo("Implement str_allocator")
	return 0;
//	return alloc.pool.memsize * alloc.pool.pools.size();
}

void str_container::integrity()
{
    lock();

	for (u16 j = object_clsid; j < type_count; ++j)
	{
		integrity_type((str_type)j);
	}
    unlock();
}

void str_container::gc_step()
{
#pragma todo("This code looks like absolute nonsense, refactor later")
	/*
    if (!gc_iterator)
    {
        ++gc_bucket;
        gc_iterator = buckets.back();
    }
    if (gc_iterator)
    {
		u32 crc = compression::crc32((u8*)gc_iterator->value, gc_iterator->length, 0xFFFFFFFF);
		R_ASSERT2(crc==gc_iterator->crc, "!P mem_corrupt: shared-strings");
        if (_InterlockedCompareExchange(&gc_iterator->refs, -1, 0) || gc_iterator->str_type)
        {
            gc_iterator = gc_iterator->next;
        }
        else
        {
            --amount;
			str_value* next = gc_iterator->next;
            if (buckets.back() == gc_iterator)
            {
                next = buckets.back();
            }
            gc_iterator = next;
        }
    }*/
}

smem_container::smem_container()
{
	threading::mutex::mutex("memory::shared::block");
	buckets.clear();
}

smem_container::~smem_container()
{
	clean();
	buckets.clear();
}

void smem_container::clean()
{
    lock();
    for (int i = 0; i < buckets.size(); i++)
    {
		buckets.pop_back();
    }
    unlock();
}

smem_container::smem_container(const smem_container* other)
{
	_os = other->_os;
	buckets = other->buckets;
}

str_value* str_shared::crc()
{
	if (p_)
		return (str_value*)p_->crc;

	return p_;
}