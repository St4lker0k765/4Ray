#pragma once
#include "../uCore/uCore.h"
#include "../uInput/stdafx.h"

#ifdef UENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#define DLL_API __declspec(dllimport)
#else
#define ENGINE_API __declspec(dllimport)
#define DLL_API __declspec(dllexport)
#endif

#include "defines.h"
#include "engine.h"