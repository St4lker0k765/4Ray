#include "stdafx.h"
#include "libtimer.h"

void timing::timer::start()
{
	u64		_dest;
	QueryPerformanceCounter((PLARGE_INTEGER)_dest);
	_start = _dest;
}

u64 timing::timer::get_elapsed_ms()
{
	u64		_dest;
	QueryPerformanceCounter((PLARGE_INTEGER)&_dest);
	return 1000 * (_dest - _start) / platform.qpc.per_second;
}

float timing::timer::get_elapsed_sec()
{
	u64		_dest;

	QueryPerformanceCounter((PLARGE_INTEGER)&_dest);
	double v2 = _dest - _start;
	if (v2 < 0)
		v2 += 1.844674407370955e19;
	double per_second_low = platform.qpc.per_second;
	if ((platform.qpc.per_second & 0x8000000000000000uLL) != 0LL)
		per_second_low = per_second_low + 1.844674407370955e19;
	return v2 / per_second_low;
}

u64 timing::timer::get_elapsed_ticks()
{
	u64		_dest;

	QueryPerformanceCounter((PLARGE_INTEGER)&_dest);
	return _dest - _start;
}

u64 timing::timer::get_elapsed_us()
{
	u64		_dest;

	QueryPerformanceCounter((PLARGE_INTEGER)&_dest);
	return 1000000 * (_dest - _start) / platform.qpc.per_second;
}