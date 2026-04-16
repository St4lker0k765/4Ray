#include <Windows.h>
#include <windowsx.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include "fastdelegate.h"
#include "_types.h"
#include "threading.h"

#ifdef UCORE_EXPORTS
#define UCORE_API __declspec(dllexport)
#else
#define UCORE_API __declspec(dllimport)
#endif

#include "debug.h"
#include "_std_extensions.h"
#include "_stl_extensions.h"
#include "u_string.h"
#include "log.h"

UCORE_API str_shared g_levelname;

