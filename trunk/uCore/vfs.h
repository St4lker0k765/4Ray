#pragma once

namespace vfs 
{
	enum epackage_format
	{
		epackage_format_uncompressed = 0,
		epackage_format_invalid,
		epackage_format_lz4,
		epackage_format_count,
	};
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
	class reader_base
	{
		const u8* __data;
		u32 __pos;
		u32 __size;
		u32 __iterpos;
	};
	class ireader : public reader_base
	{

	};
}

#include "vfs_package_registry.h"
#include "vfs_trace.h"