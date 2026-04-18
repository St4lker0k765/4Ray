#include "stdafx.h"
#include "threading.h"

threading::mutex::mutex(const char* name)
{
	_name = name;
	InitializeCriticalSection(_os);
}

threading::mutex::~mutex()
{
	DeleteCriticalSection(_os);
}

void threading::mutex::lock()
{
	EnterCriticalSection(_os);
}

bool threading::mutex::trylock()
{
	return TryEnterCriticalSection(_os);
}

void threading::mutex::unlock()
{
	LeaveCriticalSection(_os);
}

void threading::yield(u32 ms)
{
	if (!ms)
	{
		if (SwitchToThread())
			return;
		ms = 0;
	}
	Sleep(ms);
}

bool threading::spin_lock::check_lock(u32 value)
{
	return value == _lock;
}

void threading::spin_lock::lock()
{
	while (_InterlockedCompareExchange((long*)_lock, -1L, 0L));
}

bool threading::spin_lock::locked()
{
	return _lock != 0;
}

bool threading::spin_lock::trylock()
{
	return _InterlockedCompareExchange((long*)_lock, -1L, 0L) == 0;
}

void threading::spin_lock::unlock()
{
	_lock = 0;
}