#include <Windows.h>
#include <windowsx.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include "_types.h"
#include "u_string.h"
#include <string>
#include "debug.h"
#include <vector>

#ifdef UCORE_EXPORTS
#define UCORE_API __declspec(dllexport)
#else
#define UCORE_API __declspec(dllimport)
#endif

#include "_std_extensions.h"

typedef	std::string u_string;
