#pragma once

namespace vfs
{
	class x_reader
	{
	protected:
		int _h;
		u32 _pos;
		u32 _size;
		str_shared _dbg_path;
	public:
		enum x_status
		{
			downloading = 0x1,
			opened = 0x2,
		};
		x_reader();
		x_reader(const x_reader* other);
		~x_reader();

		void clear();
		void close();
		bool downloading();
		bool invalid();
		u64 open(const char* path, const int sequential);
		bool opened();
		bool read(void* dest, u32 size);
		bool seek(int pos);
		u64 size();
		u64 status();
	};
}