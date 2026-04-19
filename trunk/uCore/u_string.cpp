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