#pragma once

#include "../uCore/uCore.h"
#include <xaudio2.h>

#ifdef USOUND_EXPORTS
#define USOUND_API __declspec(dllexport)
#else
#define USOUND_API __declspec(dllimport)
#endif