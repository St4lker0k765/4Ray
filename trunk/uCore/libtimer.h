#pragma once

namespace timing 
{
	class timer
	{
		protected:
			u64 _start = 0;
		public:
			void start();
			u64 get_elapsed_us();
			u64 get_elapsed_ticks();
			float get_elapsed_sec();
			u32 get_elapsed_ms();
	};
}
