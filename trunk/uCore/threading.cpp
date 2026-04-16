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
