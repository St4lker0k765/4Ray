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
		bool check_lock(u32 value);
		void lock();
		bool locked();
		// Names for debug?
		spin_lock(const char* __formal) {}
		bool trylock();
		void unlock();
	};
	void yield(u32 ms);
	void _initialize_fpu();
	void _initialize_cpu_thread();
};