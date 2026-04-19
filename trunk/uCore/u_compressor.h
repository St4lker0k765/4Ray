#pragma once

namespace compression
{
	u64 crc32(u8* P, u32 len, u32 init);
	u64 crc32_inc(u8* P, u32 len, u32 init);
	u64 crc32_init();
}