#pragma once

class debug
{
protected:
	static threading::mutex MTX;
public:
	static fastdelegate::FastDelegate0<void> on_debug_break_cb_0;
	static void fatal(const char* F, ...);
	static void error(DWORD hr, const char* expr, const char* file, const char* func, int line);
	static void backend(const char* reason, const char* file, const char* func, int line);
	static LPCSTR error2string(DWORD code);

	static void fail(const char* e1, const char* file, const char* func, int line);
	static void fail(const char* e1, const char* e2, const char* file, const char* func, int line);
	static void fail(const char* e1, const char* e2, const char* e3, const char* file, const char* func, int line);
	static void fail(const char* e1, const char* e2, const char* e3, const char* e4, const char* file, const char* func, int line);
	static void fail(const char* e1, const char* e2, const char* e3, const char* e4, const char* e5, const char* file, const char* func, int line);
	static void fail(const char* e1, const char* e2, const char* e3, const char* e4, const char* e5, const char* e6, const char* file, const char* func, int line);
};

// warning
// this function can be used for debug purposes only
inline std::string __cdecl	make_string(LPCSTR format, ...)
{
	va_list		args;
	va_start(args, format);

	char		temp[4096];
	vsprintf(temp, format, args);

	return		(temp);
}

#define DEBUG_INFO					__FILE__,__FUNCTION__,__LINE__
	
#	define R_ASSERT(expr)				do {if (!(expr)) debug::fail(#expr,DEBUG_INFO);} while(0)
#	define R_ASSERT2(expr,e2)			do {if (!(expr)) debug::fail(#expr,e2,DEBUG_INFO);} while(0)
#	define R_ASSERT3(expr,e2,e3)		do {if (!(expr)) debug::fail(#expr,e2,e3,DEBUG_INFO);} while(0)
#	define R_ASSERT4(expr,e2,e3,e4)		do {if (!(expr)) debug::fail(#expr,e2,e3,e4,DEBUG_INFO);} while(0)
#	define R_ASSERT5(expr,e2,e3,e4,e5)	do {if (!(expr)) debug::fail(#expr,e2,e3,e4,e5,DEBUG_INFO);} while(0)
#	define R_ASSERT6(expr,e2,e3,e4,e5,e6)	do {if (!(expr)) debug::fail(#expr,e2,e3,e4,e5,e6,DEBUG_INFO);} while(0)
#	define R_CHK(expr)					do {HRESULT hr = expr; if (FAILED(hr)) debug::error(hr,#expr,DEBUG_INFO);} while(0)
#	define R_CHK2(expr,e2)				do {HRESULT hr = expr; if (FAILED(hr)) debug::error(hr,#expr,e2,DEBUG_INFO);} while(0)
#	define FATAL(description)			debug::fatal(DEBUG_INFO,description)

#	ifdef VERIFY
#		undef VERIFY
#	endif // VERIFY

#	ifdef DEBUG
#		define NODEFAULT				FATAL("nodefault reached")
#		define VERIFY(expr)				do {if (!(expr)) debug::fail(#expr,DEBUG_INFO);} while(0)
#		define VERIFY2(expr, e2)		do {if (!(expr)) debug::fail(#expr,e2,DEBUG_INFO);} while(0)
#		define VERIFY3(expr, e2, e3)	do {if (!(expr)) debug::fail(#expr,e2,e3,DEBUG_INFO);} while(0)
#		define VERIFY4(expr, e2, e3, e4)do {if (!(expr)) debug::fail(#expr,e2,e3,e4,DEBUG_INFO);} while(0)
#		define CHK_DX(expr)				do {HRESULT hr = expr; if (FAILED(hr)) debug::error(hr,#expr,DEBUG_INFO);} while(0)
#	else // DEBUG
#		ifdef __BORLANDC__
#			define NODEFAULT
#		else
#			define NODEFAULT __assume(0)
#		endif
#		define VERIFY(expr)				do {} while (0)
#		define VERIFY2(expr, e2)		do {} while (0)
#		define VERIFY3(expr, e2, e3)	do {} while (0)
#		define VERIFY4(expr, e2, e3, e4)do {} while (0)
#		define CHK_DX(a) a
#	endif // DEBUG
