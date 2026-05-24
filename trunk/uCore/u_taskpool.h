#pragma once
#include "debug.h"

namespace threading
{
	class taskpool
	{
	protected:
		semaphore tasks;
		mutex access;

		circular_buffer<fastdelegate::FastDelegate<void* ()>, 8> queue_r;
		volatile int inprogress;

		circular_buffer<fastdelegate::FastDelegate<void* ()>, 12> queue_n;
		volatile u32 num_waiters;

		circular_buffer<fastdelegate::FastDelegate<void* ()>, 9> queue_s;
		mutex access_s;
		volatile int num_overflows;
		volatile u32 exit_counter;
		int exit_flag;
		tls dbg_lock;
	public:
		taskpool()
		{

		}

		inline void add(fastdelegate::FastDelegate<void* ()>* t, bool realtime)
		{
			circular_buffer<fastdelegate::FastDelegate<void __cdecl(void)>, 8, thread_unsafe_incdec>* p_queue_r; // rcx
			int v7; // eax
			circular_buffer<fastdelegate::FastDelegate<void __cdecl(void)>, 12, thread_unsafe_incdec>* p_queue_n; // rcx
			int v9; // esi

			R_ASSERT((int)dbg_lock.value() <= 0);
			while (TRUE)
			{
				access.lock();
				if ((realtime & 1) != 0)
				{
					p_queue_r = &this->queue_r;
					v7 = (realtime & 2) != 0
						? circular_buffer<fastdelegate::FastDelegate<void(void)>, 8, thread_unsafe_incdec>::write_head(p_queue_r, t)
						: circular_buffer<fastdelegate::FastDelegate<void(void)>, 8, thread_unsafe_incdec>::write_tail(p_queue_r, t);
				}
				else
				{
					p_queue_n = &this->queue_n;
					v7 = (realtime & 2) != 0
						? circular_buffer<fastdelegate::FastDelegate<void(void)>, 12, thread_unsafe_incdec>::write_head(p_queue_n, t)
						: circular_buffer<fastdelegate::FastDelegate<void(void)>, 12, thread_unsafe_incdec>::write_tail(p_queue_n, t);
				}
				v9 = v7;
				access.unlock();
				if (v9)
					break;

				num_overflows++;
				if (queue_r.counter + queue_n.counter)
				{
					execute_nrm();
				}
				else if (queue_s.counter)
				{
					fastdelegate::FastDelegate<void*()> e;
					access_s.lock();
					if (queue_s.read(&e))
					{
						access_s.unlock();
						if (e)
							e();
					}
					else
					{
						access_s.unlock();
					}
				}
				else
				{
					yield(1);
				}
			}
			if (num_waiters)
				tasks.signal();
		}
		void add_sep(fastdelegate::FastDelegate<void* ()>* t);
		bool empty();
		void execute_all_nrm();
		bool execute_nrm();
		void execute_or_yield(DWORD ms);
		void execute_or_yield_rtp(DWORD mcs);
		bool execute_rtp();
		bool execute_sep();
		bool exist(fastdelegate::FastDelegate<void* ()>* t, bool realtime);
		bool exist(bool realtime);
		void exitworkers();
		u32 has_tasks_flag();
		volatile u32 has_tasks_flag_ptr();
		u32 has_tasks_unsafe();
		u32 has_tasks_unsafe_rtp();
		u32 overflows();
		bool remove(fastdelegate::FastDelegate<void* ()>* t, bool realtime);
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
		inline void worker(void* __formal)
		{
			while (!exit_flag)
			{
				num_waiters++;
				tasks.wait();
				num_waiters--;
				do
				{
					execute_all_nrm();
					while (!(queue_n.counter + queue_r.counter))
					{
						fastdelegate::FastDelegate<void* ()> e;
						access_s.lock();
						if (!queue_s.read(&e))
						{
							access_s.unlock();
							break;
						}
						access_s.unlock();
						if (e)
							e();
					}
				} while (queue_n.counter + queue_r.counter);
			}
			exit_counter++;
		}
		inline volatile int workinprogress() { return inprogress; }
	};
}