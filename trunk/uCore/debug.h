#pragma once

class debug
{
protected:
	threading::mutex MTX;
public:
	fastdelegate::FastDelegate0<void> on_debug_break_cb_0;
	void fatal(const char* F, ...);
	void error(DWORD hr, const char* expr, const char* file, const char* func, int line);
	void backend(const char* reason, const char* file, const char* func, int line);
	LPCSTR error2string(DWORD code);

	void fail(const char* e1, const char* file, const char* func, int line);
	void fail(const char* e1, const char* e2, const char* file, const char* func, int line);
	void fail(const char* e1, const char* e2, const char* e3, const char* file, const char* func, int line);
	void fail(const char* e1, const char* e2, const char* e3, const char* e4, const char* file, const char* func, int line);
	void fail(const char* e1, const char* e2, const char* e3, const char* e4, const char* e5, const char* file, const char* func, int line);
	void fail(const char* e1, const char* e2, const char* e3, const char* e4, const char* e5, const char* e6, const char* file, const char* func, int line);
};

extern UCORE_API debug Debug;

#define DEBUG_INFO					__FILE__,__FUNCTION__,__LINE__
	
#	define R_ASSERT(expr)				do {if (!(expr)) Debug.fail(#expr,DEBUG_INFO);} while(0)
#	define R_ASSERT2(expr,e2)			do {if (!(expr)) Debug.fail(#expr,e2,DEBUG_INFO);} while(0)
#	define R_ASSERT3(expr,e2,e3)		do {if (!(expr)) Debug.fail(#expr,e2,e3,DEBUG_INFO);} while(0)
#	define R_ASSERT4(expr,e2,e3,e4)		do {if (!(expr)) Debug.fail(#expr,e2,e3,e4,DEBUG_INFO);} while(0)
#	define R_ASSERT5(expr,e2,e3,e4,e5)	do {if (!(expr)) Debug.fail(#expr,e2,e3,e4,e5,DEBUG_INFO);} while(0)
#	define R_ASSERT6(expr,e2,e3,e4,e5,e6)	do {if (!(expr)) Debug.fail(#expr,e2,e3,e4,e5,e6,DEBUG_INFO);} while(0)
#	define R_CHK(expr)					do {HRESULT hr = expr; if (FAILED(hr)) Debug.error(hr,#expr,DEBUG_INFO);} while(0)
#	define R_CHK2(expr,e2)				do {HRESULT hr = expr; if (FAILED(hr)) Debug.error(hr,#expr,e2,DEBUG_INFO);} while(0)
#	define FATAL(description)			Debug.fatal(DEBUG_INFO,description)

#	ifdef VERIFY
#		undef VERIFY
#	endif // VERIFY

#	ifdef DEBUG
#		define NODEFAULT				FATAL("nodefault reached")
#		define VERIFY(expr)				do {if (!(expr)) ::Debug.fail(#expr,DEBUG_INFO);} while(0)
#		define VERIFY2(expr, e2)		do {if (!(expr)) ::Debug.fail(#expr,e2,DEBUG_INFO);} while(0)
#		define VERIFY3(expr, e2, e3)	do {if (!(expr)) ::Debug.fail(#expr,e2,e3,DEBUG_INFO);} while(0)
#		define VERIFY4(expr, e2, e3, e4)do {if (!(expr)) ::Debug.fail(#expr,e2,e3,e4,DEBUG_INFO);} while(0)
#		define CHK_DX(expr)				do {HRESULT hr = expr; if (FAILED(hr)) ::Debug.error(hr,#expr,DEBUG_INFO);} while(0)
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
