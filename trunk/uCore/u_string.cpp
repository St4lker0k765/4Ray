#include "stdafx.h"
#include "u_string.h"

u_string u_string::vset(const char* format, va_list arg_list)
{
	u_string temp;
	string4096 	buf;
	int vs_sz = _vsnprintf(buf, sizeof(buf) - 1, format, arg_list);
	buf[sizeof(buf) - 1] = 0;
	if (vs_sz)	temp.assign(buf);
	va_end(arg_list);
	return 		temp;
}

u_string sz_replace_file_ext(const char* src, const char* ext)
{
	LPCSTR split = strrchr(src, '.');
	u64 split_sz = strlen(src); // r8
	if (split)
	{
		split_sz = split - src;
	}
	else
	{
		split_sz = strlen(src);
	}

	u_string result;
	result.assign(src, split_sz);

	u16 count = 0;
	u_string ext_;
	if (strlen(ext))
	{
		ext_.assign(ext, strlen(ext));
		count = ext_.size();
	}
	result.append(ext_.c_str(), count);
	return result;
}

u32 u_string::find_first_not_of(const char* delims, int delims_len)
{
	u_string::iterator it = begin();
	for (; it < end(); it++)
	{

	}
	char* buf = (char*)this->buf;
	char* ptr = (char*)this->buf;
	u32 v7 = 0;
	char* v8 = &ptr[count];
	if (v8 == buf)
		return 0;

	while (memchr(delims, *buf, delims_len))
	{
		++buf;
		++v7;
		if (v8 == buf)
			return 0;
	}
	return *it;
}

u_string u_string::trim()
{
	__int64 v1; // rbx
	__int64 v3; // rax
	__int64 first_not_of; // rbp
	unsigned int last_not_of; // esi
	const char* buf; // rdx
	unsigned __int16 capacity; // r8
	char* ptr; // rcx
	unsigned int v9; // esi
	unsigned int* v10; // rdx
	u_memory* v12; // rax
	u_string v14;
	u_string v15; 

	v1 = -1;
	first_not_of = u_string::find_first_not_of(this, `u_string<32 > ::trim'::`2'::delims, v3);
	last_not_of = u_string::find_last_not_of(
		this,
		`u_string<32 > ::trim'::`2'::delims,
		`u_string<32 > ::trim'::`2'::delims_len);
	if (this->capacity <= 0x20u)
		buf = (const char*)this->buf;
	else
		buf = this->ptr;
	capacity = 32;
	*(_QWORD*)&v15.capacity = 32;
	ptr = nullptr;
	v15.ptr = nullptr;
	do
		++v1;
	while (buf[v1]);
	if ((_DWORD)v1)
	{
		u_string<32>::assign(&v15, buf, v1);
		capacity = v15.capacity;
		ptr = v15.ptr;
	}
	v14.ptr = nullptr;
	*(_QWORD*)&v14.capacity = 32;
	v9 = last_not_of - first_not_of;
	if (v9)
	{
		v10 = v15.buf;
		if (capacity > 0x20u)
			v10 = (unsigned int*)ptr;
		u_string<32>::assign(&v14, (const char*)v10 + first_not_of, v9);
	}
	u_string<32>::swap(&v14, this);
	v14.ptr = nullptr;
	*(_DWORD*)&v14.capacity = 32;
	return this;
}