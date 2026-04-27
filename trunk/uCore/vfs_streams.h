#pragma once

namespace vfs
{
	class reader_base
	{
	protected:
		const u8* __data;
		u32 __pos;
		u32 __size;
		u32 __iterpos;
	public:
		reader_base();
		reader_base(const u8* data, u32 size, u32 pos);

		void advance(int cnt);
		u64 advance_term_string();
		u64 chunk_find(u32 ID);
		const u8* current_pointer();
		const u8* data();
		u32 elapsed();
		bool eof();
		void init(const reader_base* from);
		void init(const u8* data, u32 size, u32 iterpos, u32 pos);
		u32 length();
		void r(void* p, int c);
		float r_angle16();
		float r_angle8();
		void r_dir(Fvector3* A);
		float r_fp32();
		float r_fp32_q16(float min, float max);
		float r_fp32_q8(float min, float max);
		void r_matrix(Fmatrix& M);
		void r_matrix_43T(Fmatrix43& M);
		s16 r_s16();
		s32 r_s32();
		s64 r_s64();
		s8 r_s8();
		void r_string(char* dest, u32 tgt_sz);
		void r_string(u_string& dest);
		void r_stringz(char* dest, u32 tgt_sz);
		void r_stringz(str_shared& dest, str_type type);
		void r_stringz(u_string& dest);
		u16 r_u16();
		u32 r_u32();
		u64 r_u64();
		u8 r_u8();
		void r_vec2f(Fvector2* v);
		void r_vec2i(Ivector2* v);
		void r_vec3f(Fvector3* v);
		void r_vec3i(Ivector3* v);
		void r_vec4f(Fvector4* v);
		void r_vec4i(Ivector4* v);
		void rewind();
		void seek(u32 ptr);
		void skip_stringz();
		u32 tell();
	};
	class UCORE_API ireader : public reader_base
	{
	public:
		ireader();
		ireader(const u8* data, u32 size, u32 iterpos);
		ireader(const ireader* other);

		ireader* chunk_open(ireader* result, u32 ID);
		ireader* chunk_open_at_current_position(ireader* result, u32 ID);
		u32 chunk_open_at_current_position(u32 ID, ireader* result);
		ireader* chunk_open_iterator(ireader* result, u32* ID, ireader* prev);
		bool chunk_open_try_current(u32 ID, ireader* result);
		bool chunk_read(u32 ID, void* dest, u32 dest_size);
		u64 chunk_skip_at_current_position(u32 ID);
		u64 chunk_test(u32 ID);
		ireader* chunk_try_open_at_current_position(ireader* result, u32 ID);
		bool chunk_try_open_at_current_position(u32 ID, u32* size);
		bool chunk_try_open_at_current_position(u32 ID, ireader* result);
	};
}