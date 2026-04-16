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
	}
}