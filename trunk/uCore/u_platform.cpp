#include "stdafx.h"
#include "u_platform.h"
#include <array>
#include <bitset>

u64 temp_start_clk = 0;
u64 temp_start_timer = 0;

void u_platform::initialize_begin()
{
	//int cpuInfo[4] = {-1};
	std::array<int, 4> cpui;

	// Calling __cpuid with 0x0 as the function_id argument
	// gets the number of the highest valid function ID.
	__cpuid(cpui.data(), 0);
	u32 nIds_ = cpui[0];
	std::vector<std::array<int, 4>> data_;

	for (int i = 0; i <= nIds_; ++i)
	{
		__cpuidex(cpui.data(), i, 0);
		data_.push_back(cpui);
	}
	std::bitset<32> f_1_EDX_ = data_[1][3];

	features._vmx = (cpui.data()[2] & (1 << 5)) != 0;
	features._sse2 = f_1_EDX_[26];

	_SYSTEM_INFO sys_inf;
	GetSystemInfo(&sys_inf);
	os_allocgranularity = sys_inf.dwAllocationGranularity;
	features._cpucount = sys_inf.dwNumberOfProcessors;
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	timeBeginPeriod(1);
	if (temp_start_timer != timeGetTime())
		temp_start_timer = timeGetTime();

	temp_start_clk = __rdtsc();
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	qpc.per_second = 1;
}

void u_platform::initialize_end()
{
	double v15; // xmm2_8
	float v16; // xmm0_4
	float v17; // xmm0_4
	double v21; // xmm6_8
	float v22; // xmm0_4

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	for (DWORD i = timeGetTime(); i - temp_start_timer < 1000; i = timeGetTime())
	{
		threading::yield(0);
	}
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);

	cycles.overhead = 0;
	cycles.per_second = 1000 * (get_clocks() - temp_start_clk) / (timeGetTime() - temp_start_timer);
	u64 overhead; // r8
	for (u64 i = 64; i > 0; i--)
	{
		overhead = ((get_clocks() << 32) | get_clocks()) + cycles.overhead - get_clocks();
		cycles.overhead = overhead;
	}
	cycles.overhead = overhead >> 6;

	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	urandom::primary::seed(timeGetTime());
	urandom::game::seed(timeGetTime());

	cycles.per_second -= cycles.overhead;
	u64 per_second = cycles.per_second;
	double v14 = per_second;
	cycles.per_milisec = per_second / 1000;
	cycles.per_microsec = per_second / 1000 / 1000;

	if ((per_second & 0x8000000000000000uLL) != 0LL)
		v14 = v14 + 1.844674407370955e19;
	v15 = 1.0 / v14;
	v16 = 1.0 / v14;
	cycles.to_seconds = v16;
	v17 = v15 * 1000.0;
	cycles.to_milisec = v17;
	cycles.to_microsec = v15 * 1000000.0;

	u64 qwTimeFreq;
	QueryPerformanceFrequency((PLARGE_INTEGER)&qwTimeFreq);
	qpc.overhead = 0;
	qpc.per_second = qwTimeFreq;
	double v20 = qwTimeFreq;
	qpc.per_milisec = qwTimeFreq / 1000;
	qpc.per_microsec = qwTimeFreq / 1000000;
	if (qwTimeFreq < 0)
		v20 = v20 + 1.844674407370955e19;
	v21 = 1.0 / v20;
	qpc.to_seconds = 1.0 / v20;
	qpc.to_milisec = v21 * 1000.0;
	qpc.to_microsec = v21 * 1000000.0;
	u_platform::fpu_set24r();
}

void u_platform::fpu_set24r()
{
	_controlfp(_SW_INEXACT, _EM_INEXACT);
	_control87(_MCW_EM, _MCW_EM);
}

void u_platform::fpu_set64r()
{
	_controlfp(_EM_AMBIGUOUS, _MCW_PC);
	_controlfp(_EM_AMBIGUOUS, _MCW_RC);
	_control87(_MCW_EM, _MCW_EM);
}

u64 u_platform::get_clocks()
{
	return __rdtsc();
}

u64 u_platform::get_QPC()
{
	u64 _dest;

	QueryPerformanceCounter((PLARGE_INTEGER)&_dest);
	return _dest;
}

UCORE_API u_platform platform;