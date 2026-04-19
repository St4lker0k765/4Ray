#include "stdafx.h"
#include "u_string.h"

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