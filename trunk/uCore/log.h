#pragma once

void slog(const char* s);
void rlog(const char* format, ...);

class logger : public threading::mutex
{
protected:
	bool flush_forced;
public:
	str_shared fname;
	u_vector<str_shared> strings;
	u_vector<LogCallback> callbacks;
	logger();
	virtual ~logger();
	void add(const char* split);
	void flush_to_hdd();
};

typedef void	(*LogCallback)	(LPCSTR string);

extern logger* Log;
extern UCORE_API string_path log_fname;
