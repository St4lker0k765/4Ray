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
	class taskpool
	{
	protected:
		semaphore tasks;
		mutex access;

		circular_buffer<fastdelegate::FastDelegate<void*()>, 8> queue_r;
		volatile int inprogress;

		circular_buffer<fastdelegate::FastDelegate<void*()>, 12> queue_n;
		volatile DWORD num_waiters;

		circular_buffer<fastdelegate::FastDelegate<void*()>, 9> queue_s;
		mutex access_s;
		volatile int num_overflows;
		volatile DWORD exit_counter;
		int exit_flag;
		tls dbg_lock;
	public:
		taskpool();
		taskpool(const taskpool* other);

		void add(fastdelegate::FastDelegate<void*()>* t, bool realtime);
		void add_sep(fastdelegate::FastDelegate<void*()>* t);
		bool empty();
		void execute_all_nrm();
		bool execute_nrm();
		void execute_or_yield(DWORD ms);
		void execute_or_yield_rtp(DWORD mcs);
		bool execute_rtp();
		bool execute_sep();
		bool exist(fastdelegate::FastDelegate<void*()>* t, bool realtime);
		bool exist(bool realtime);
		void exitworkers();
		u32 has_tasks_flag();
		volatile u32 has_tasks_flag_ptr();
		u32 has_tasks_unsafe();
		u32 has_tasks_unsafe_rtp();
		u32 overflows();
		bool remove(fastdelegate::FastDelegate<void*()>* t, bool realtime);
		bool remove(bool realtime);
		bool remove_func(fastdelegate::FastDelegate<void* ()>* t, bool realtime);
		void signal();
		void spawnworkers();
		void wait_signal(semaphore* signal);
		void wait_signal_impl(void* handle_);
		void wait_var(s32* flag, int flag_value);
		void wait_var(volatile s32* flag, int flag_value, DWORD optional_yield_ms);
		void wait_var(u32* flag, int flag_value);
		void wait_var(volatile u32* flag, int flag_value, DWORD optional_yield_ms);
		void worker(void* __formal);
		volatile int workinprogress();
	};
	void yield(u32 ms);
	void _initialize_fpu();
	void _initialize_cpu_thread();
};