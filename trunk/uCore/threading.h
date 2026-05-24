#pragma once

namespace threading
{
	class mutex
	{
	protected:
		LPCRITICAL_SECTION _os;
		const char* _name;
	public:
		mutex(const char* name);
		mutex() { mutex(__FUNCTION__); }
		virtual ~mutex();

		void lock();
		bool trylock();
		void unlock();
	};
	class spin_lock
	{
	protected:
		volatile int _lock = 0;
	public:
		spin_lock();
		~spin_lock() {}
		// Names for debug?
		spin_lock(const char* __formal) {}

		bool check_lock(u32 value);
		void lock();
		bool locked();
		bool trylock();
		void unlock();
	};
	class semaphore
	{
	protected:
		void* _os;
	public:
		semaphore();
		~semaphore();

		bool signal();
		bool signal(long cnt);
		bool wait();
		bool trywait(u32 multiply);
	};
	class tls
	{
	protected:
		u32 _index = -1;
	public:
		~tls();

		void deinit();
		bool valid();
		void* value();
	};
	void yield(u32 ms);
	void _initialize_fpu();
	void _initialize_cpu_thread();
};
#include "u_taskpool.h"