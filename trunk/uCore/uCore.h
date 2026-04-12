#include <Windows.h>
#include <windowsx.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include "_types.h"
#include "u_string.h"

#ifdef UCORE_EXPORTS
#define UCORE_API __declspec(dllexport)
#else
#define UCORE_API __declspec(dllimport)
#endif