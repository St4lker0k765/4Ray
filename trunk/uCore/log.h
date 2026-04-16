#pragma once

void slog(const char* s);
void rlog(const char* format, ...);

class logger
{
protected:
	bool flush_forced;
	threading::mutex* MTX;
public:
	str_shared fname;
	u_vector<str_shared> strings;
	u_vector<void*> callbacks;
	logger();
	virtual ~logger();
	void add(const char* split);
	void flush_to_hdd();
};

extern logger* Log;