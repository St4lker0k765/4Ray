#include "stdafx.h"
#include "threading.h"
#include <locale>

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

threading::spin_lock::spin_lock()
{
	_lock = 0;
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

threading::semaphore::semaphore()
{
	_os = CreateSemaphoreA(nullptr, 0, INT_MAX, nullptr);
}

threading::semaphore::~semaphore()
{
	CloseHandle(_os);
}

bool threading::semaphore::signal()
{
	return ReleaseSemaphore(_os, 1, nullptr);
}

bool threading::semaphore::signal(long cnt)
{
	return ReleaseSemaphore(_os, cnt, nullptr);
}

bool threading::semaphore::wait()
{
	return WaitForSingleObject(_os, INT_MAX) == 0;
}

bool threading::semaphore::trywait(u32 multiply)
{
	return WaitForSingleObject(_os, 0) == 0;
}

void threading::_initialize_fpu()
{
	platform.fpu_set24r();
#pragma todo("Determine meaning of those flags")
	_mm_setcsr(_mm_getcsr() | 0x8000);
	_mm_setcsr(_mm_getcsr() | 0x40);
}

void threading::_initialize_cpu_thread()
{
	threading::_initialize_fpu();
}

threading::tls::~tls()
{
	if (_index != -1)
		TlsFree(_index);
}

void threading::tls::deinit()
{
	if (_index != -1)
		TlsFree(_index);
}

bool threading::tls::valid()
{
	return _index != -1;
}

void* threading::tls::value()
{
	return TlsGetValue(_index);
}
