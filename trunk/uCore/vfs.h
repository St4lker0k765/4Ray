#pragma once

namespace vfs 
{
	struct db_file
	{
		void* handle;
		const int _not_packaged;
		int _io_finished;
		u8* pf_buffer;
		u32 pf_begin;
		u32 pf_end;
		u32 pf_pending;
	};

}