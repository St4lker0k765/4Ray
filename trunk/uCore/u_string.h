#pragma once

#include <string>
class u_string : public	std::basic_string<char, std::char_traits<char>, u_alloc<char> >
{
private:
	typedef std::basic_string<char, std::char_traits<char>, u_alloc<char> > inherited;
public:
	u_string vset(const char* format, va_list arg_list);
	u_string sz_replace_file_ext(const char* src, const char* ext);
	u_string set( const char* format, ...)
	{
		va_list va; // [rsp+40h] [rbp+18h] BYREF

		va_start(va, format);
		return vset(format, va);
	}
	u_string lwr()
	{
		for (u_string::iterator it = begin(); it < end(); it++)
		{
			std::tolower(*it);
		}
		return *this;
	}
	u32 find_first_not_of(const char* delims, int delims_len);
	u_string trim();
	u_string& operator=	(const char* rhs) { assign(rhs);	return (u_string&)*this; }
};

