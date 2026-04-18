#pragma once

class u_platform
{
protected:
	struct _features 
	{
		u32 _vmx : 1;
		u32 _sse2 : 1;
		u32 _cpucount : 8;
	};
	_features features;
	struct _cvt
	{
		u64 overhead;
		u64 per_second;
		u64 per_milisec;
		u64 per_microsec;

		float to_seconds;
		float to_milisec;
		float to_microsec;
	};
	_cvt qpc;
	_cvt cycles;
	u32 os_allocgranularity;
public:
	void initialize_begin();
	void initialize_end();

	void fpu_set24r();
	void fpu_set64r();

	u64 get_clocks();
	u64 get_QPC();
};

extern UCORE_API u_platform platform;