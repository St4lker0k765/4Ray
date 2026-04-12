#pragma once

namespace debug 
{
	void fatal(const char* F, ...);
	void error(DWORD hr, const char* expr, const char* file, const char* func, int line);
	void backend(const char* reason, const char* file, const char* func, int line);
}