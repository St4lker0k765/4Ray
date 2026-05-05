#pragma once
#include "../uCore/uCore.h"
#include "../uInput/stdafx.h"

#ifdef UENGINE_EXPORTS
#define UENGINE_API __declspec(dllexport)
#else
#define UENGINE_API __declspec(dllimport)
#endif

#include "engine.h"