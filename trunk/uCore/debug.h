#pragma once

namespace debug
{
	void fatal(const char* F, ...);
	void error(DWORD hr, const char* expr, const char* file, const char* func, int line);
	void backend(const char* reason, const char* file, const char* func, int line);

	void fail(const char* e1, const char* file, const char* func, int line);
	void fail(const char* e1, const char* e2, const char* file, const char* func, int line);
	void fail(const char* e1, const char* e2, const char* e3, const char* file, const char* func, int line);
	void fail(const char* e1, const char* e2, const char* e3, const char* e4, const char* file, const char* func, int line);
	void fail(const char* e1, const char* e2, const char* e3, const char* e4, const char* e5, const char* file, const char* func, int line);
	void fail(const char* e1, const char* e2, const char* e3, const char* e4, const char* e5, const char* e6, const char* file, const char* func, int line);
}

#define DEBUG_INFO					__FILE__,__FUNCTION__,__LINE__
	
#define _TRE(arg)	arg

#	define R_ASSERT(expr)				do {if (!(expr)) debug::fail(_TRE(#expr),DEBUG_INFO);} while(0)
#	define R_ASSERT2(expr,e2)			do {if (!(expr)) debug::fail(_TRE(#expr),_TRE(e2),DEBUG_INFO);} while(0)
#	define R_ASSERT3(expr,e2,e3)		do {if (!(expr)) debug::fail(_TRE(#expr),_TRE(e2),_TRE(e3),DEBUG_INFO);} while(0)
#	define R_ASSERT4(expr,e2,e3)		do {if (!(expr)) debug::fail(_TRE(#expr),_TRE(e2),_TRE(e3),_TRE(e4),DEBUG_INFO);} while(0)
#	define R_CHK(expr)					do {HRESULT hr = expr; if (FAILED(hr)) debug::error(hr,_TRE(#expr),DEBUG_INFO);} while(0)
#	define R_CHK2(expr,e2)				do {HRESULT hr = expr; if (FAILED(hr)) debug::error(hr,_TRE(#expr),_TRE(e2),DEBUG_INFO);} while(0)
#	define FATAL(description)			debug::fatal(DEBUG_INFO,description)

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
//---------------------------------------------------------------------------------------------
// FIXMEs / TODOs / NOTE macros
//---------------------------------------------------------------------------------------------
#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define NOTE( x )  message( x )
#define FILE_LINE  message( __FILE__LINE__ )

#define TODO( x )  message( __FILE__LINE__"\n"           \
	" ------------------------------------------------\n" \
	"|  TODO :   " #x "\n" \
	" -------------------------------------------------\n" )
#define FIXME( x )  message(  __FILE__LINE__"\n"           \
	" ------------------------------------------------\n" \
	"|  FIXME :  " #x "\n" \
	" -------------------------------------------------\n" )
#define todo( x )  message( __FILE__LINE__" TODO :   " #x "\n" ) 
#define fixme( x )  message( __FILE__LINE__" FIXME:   " #x "\n" ) 

//--------- static assertion
template<bool>	struct CompileTimeError;
template<>		struct CompileTimeError<true>	{};
#define STATIC_CHECK(expr, msg) \
{ \
	CompileTimeError<((expr) != 0)> ERROR_##msg; \
	(void)ERROR_##msg; \
}
