#pragma once

#include <string>
// string(char)
class u_string : public	std::basic_string<char, std::char_traits<char>, u_alloc<char> >
{
private:
	typedef std::basic_string<char, std::char_traits<char>, u_alloc<char> > inherited;
public:
	u_string vset(const char* format, va_list arg_list);
	u_string sz_replace_file_ext(const char* src, const char* ext);
};

