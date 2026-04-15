#pragma once
#include "../uCore/uCore.h"
#include <dinput.h>
#include <xinput.h>

#ifdef UINPUT_EXPORTS
#define UINPUT_API __declspec(dllexport)
#else
#define UINPUT_API __declspec(dllimport)
#endif