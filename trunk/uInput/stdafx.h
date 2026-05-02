#pragma once
#include "../uCore/uCore.h"

#ifdef UINPUT_EXPORTS
#define UINPUT_API __declspec(dllexport)
#else
#define UINPUT_API __declspec(dllimport)
#endif

#include "u_input.h"
#include "input_manager.h"
