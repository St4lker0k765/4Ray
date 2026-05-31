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

	class istorage
	{
	protected:
		u8** __data = 0;
		u8* _data = 0;
		u8* __pointer = 0;
		int __mem_size = 0;
		int __file_size = 0;
	public:
		u64 calc_mem_size(u32 cur_mem_size, u32 req_mem_size, const u32 mul, const u32 div)
		{
			u32 sz = cur_mem_size;
			if (cur_mem_size < 0x80)
				sz = 128;
			for (; sz <= req_mem_size; sz = mul * sz / div)
				;
			return (sz + 127) & 0xFFFFFF80;
		}
		void clear()
		{
			if (__mem_size < 0)
				reserve_cold(0, 0, 0);
			__file_size = 0;
			R_ASSERT(__data);
			__pointer = *__data;
		}
		u8* data()
		{
			R_ASSERT(__data);
			return *__data;
		}
		void freemem()
		{
			R_ASSERT(__data);
			__mem_size = 0;
			__pointer = nullptr;
			// port memory macroses from xr?
			memory()->xmem_realloc((char*)__data, 0, 0, "storage_data_t::freemem");
		}
		void init(u8** data, int size)
		{
			__data = data;
			__mem_size = size;
			__pointer = *data;
			__file_size = size;
		}
		u8* pointer()
		{
			if (data())
				return __pointer;
			return 0;
		}
		void reserve(int reserve_mem_size, int exact, int allow_out_of_memory)
		{
			if (reserve_mem_size > __mem_size)
				reserve_cold(reserve_mem_size, exact, allow_out_of_memory);
		}
		void reserve_cold(int reserve_mem_size,	const int exact, const int allow_out_of_memory)
		{
			unsigned int mem_size; // r14d

			R_ASSERT(__data);
			char* v13;
			if (exact)
			{
				mem_size = reserve_mem_size + 16;
				v13 = memory()->xmem_realloc(nullptr, (u32)(reserve_mem_size + 16), 0x80u, "storage");
			}
			u32 v10 = reserve_mem_size + 16;
			if (__mem_size < 0x80)
				__mem_size = 128;
			for (; __mem_size <= v10; __mem_size = (3 * __mem_size) >> 1)
				;
			mem_size = (__mem_size + 127) & 0xFFFFFF80;
			v13 = memory()->xmem_realloc(nullptr, mem_size, 0x80u, "storage");
			if (!v13)
			{
				if (__mem_size < 0x80)
					__mem_size = 128;
				for (; __mem_size <= v10; __mem_size = 11 * __mem_size / 0xA)
					;
				mem_size = (__mem_size + 127) & 0xFFFFFF80;
				v13 = memory()->xmem_realloc(nullptr, mem_size, 0x80u, "storage");
				if (!v13)
				{
				LABEL_14:
					mem_size = reserve_mem_size + 16;
					v13 = memory()->xmem_realloc(nullptr, (u32)(reserve_mem_size + 16), 0x80u, "storage");
				}
			}
			R_ASSERT(mem_size >= 16);
			if (v13)
			{
				if (__data)
				{
					size_t v18 = __mem_size + 16;
					memcpy(v13, __data, v18);
					memory()->xmem_realloc((char*)__data, 0, 0, "storage_data_t::reserve_cold");
				}
				*__data = (u8*)v13;
			}
			else
			{
				if (allow_out_of_memory)
					return;

				debug::fatal("! out of memory");
			}
			__pointer = &data()[tell()];
			__mem_size = mem_size - 16;
		}
		void resize(int count)
		{
			if (count > __mem_size)
				reserve_cold(count, 0, 0);
			__file_size = count;
			__pointer = data();
		}
		void seek(u32 pos)
		{
			__pointer = &data()[pos];
		}
		size_t size()
		{
			int v2 = tell();
			if ((__file_size ^ (__file_size >> 31)) + (v2 ^ (v2 >> 31)) - (__file_size >> 31) - (v2 >> 31) == 0x7FFFFFFF)
				debug::fail("_abs(x)+_abs(y) < s32(u32(u32(1)<<31) - u32(1))", "d:\\trunk\\src\\ucore\\libstdext.h", "_max", 299);
			return v2 - ((v2 - __file_size) & (u32)((v2 - __file_size) >> 31));
		}

		virtual bool valid()
		{
			return __data && *__data;
		}
		virtual void seek(u32 pos)
		{
			__pointer = &data()[pos];
		}
		virtual u32 tell()
		{
			return (__pointer - data());
		}
		virtual void w(const char* ptr, u32 count)
		{
			if (count)
			{
				if (count > 16 && count + tell() > __mem_size)
				{
					_mm_prefetch(ptr, 2);
					if (tell() + count > __mem_size)
						reserve_cold(tell() + count, 0, 0);
				}
				u8* v8 = __pointer;
				s64 v9 = ptr - (const char*)__pointer;
				do
				{
					char v10 = (v8++)[v9];
					*(v8 - 1) = v10;
				} while (v8 != &__pointer[count]);
				__pointer = &__pointer[count];
				if (count <= 16)
				{
					if ((tell() + count) > __mem_size)
						reserve_cold(tell() + count, 0, 0);
				}
			}
		}
		virtual void space(u32 size)
		{
			if (size + 16 > __mem_size)
				reserve_cold(size + 16, 1, 0);
		}
	};

	class UCORE_API iwriter
	{
	protected:
		u_stack<u32> chunk_pos{};
		str_shared f_name = "";
	public:
		void chunk_close();
		void chunk_open(u32 type);
		u32 chunk_size();
		void chunk_write(u32 type, void* data, u32 size);
		inline str_shared file_name() { return f_name; }
		virtual void seek(u32 pos) = 0;
		virtual void space() {}
		virtual u32 tell() = 0;
		virtual bool valid() = 0;

		virtual void w(const void* ptr, u64 count) = 0;
		inline void w1(const void* ptr) { w(ptr, 1); }
		inline void w12(const void* ptr) { w(ptr, 12); }
		inline void w12_4(s32 v0, s32 v1, s32 v2)
		{
			w(&v0, sizeof(v0));
			w(&v1, sizeof(v1));
			w(&v2, sizeof(v2));
		}
		inline void w16(const void* ptr) { w(ptr, 16); }
		inline void w12_4(s32 v0, s32 v1, s32 v2, s32 v3)
		{
			w(&v0, sizeof(v0)); 
			w(&v1, sizeof(v1));
			w(&v2, sizeof(v2));
			w(&v3, sizeof(v3));
		}
		inline void w2(const void* ptr) { w(ptr, 2); }
		inline void w4(const void* ptr) { w(ptr, 4); }
		inline void w4_4(s32 v) { w(&v, sizeof(v)); }
		inline void w8(const void* ptr) { w(ptr, 8); }
		inline void w8_4(s32 v0, s32 v1) 
		{ 
			w(&v0, sizeof(v0));
			w(&v1, sizeof(v1));
		}
		inline void w_printf(const char* format, ...)
		{
			string1024 buf;
			va_list va;
			va_start(va, format);
			vsprintf(buf, format, va);
			w(buf, strlen(buf));
		}

		inline void w_angle16(float a) { w_fp32_q16(angle_normalize(a), 0, PI_MUL_2); }
		inline void w_angle8(float a) { w_fp32_q8(angle_normalize(a), 0, PI_MUL_2); }
		void w_chunk_close8(u32 position);
		void w_chunk_open8(u32* position);
		inline void w_dir(Fvector3 D) { w_u16(compression::compress::normal(D)); }
		inline void w_fp32(float v) { w(&v, sizeof(v)); }
		inline void w_fp32_q16(float a, float min, float max)
		{
			VERIFY2(a >= min && a <= max, make_string("incorrect values for quantize [%f] <= [%f] <= [%f]", min, a, max));
			float q = (a - min) / (max - min);
			w_u16(u16(iFloor(q * 65535.f + .5f)));
		}
		inline void w_fp32_q8(float a, float min, float max)
		{
			VERIFY2(a >= min && a <= max, make_string("incorrect values for quantize [%f] <= [%f] <= [%f]", min, a, max));
			float q = (a - min) / (max - min);
			w_u8(u8(iFloor(q * 255.f + .5f)));
		}
		inline void w_matrix(Fmatrix M) { w(&M, sizeof(M)); }
		inline void w_matrix_43T(Fmatrix43 M) { w(&M, sizeof(M)); }
		void w_racc(u32 pos, const void* p, u32 count);
		
		inline void w_s16(s16 v) { w(&v, sizeof(v)); }
		inline void w_s32(s32 v) { w(&v, sizeof(v)); }
		inline void w_s64(s64 v) { w(&v, sizeof(v)); }
		inline void w_s8(s8 v) { w(&v, sizeof(v)); }

		inline void w_string(const char* p) { w(p,strlen(p));w_u8(13);w_u8(10); }
		void w_stringz(const char* p) { w(p,strlen(p)+1); }
		void w_stringz(str_shared& p) { w(*p?*p:"",p.size());w_u8(0); }
		void w_stringz(u_string& p) { w(p.c_str()?p.c_str():"",p.size());w_u8(0); }

		inline void w_u16(u16 v) { w(&v, sizeof(v)); }
		inline void w_u32(u32 v) { w(&v, sizeof(v)); }
		inline void w_u64(u64 v) { w(&v, sizeof(v)); }
		inline void w_u8(u8 v) { w(&v, sizeof(v)); }

		inline void w_vec2f(Fvector2 v) 
		{ 
			w_fp32(v.x);
			w_fp32(v.y);
		}
		inline void w_vec2i(Ivector2 v)
		{
			w_s32(v.x);
			w_s32(v.y);
		}

		inline void w_vec3f(Fvector3 v)
		{ 
			w_fp32(v.x);
			w_fp32(v.y);
			w_fp32(v.z);
		}
		inline void w_vec3i(Ivector3 v) 
		{
			w_s32(v.x);
			w_s32(v.y);
			w_s32(v.z);
		}

		inline void w_vec4f(Fvector4 v) 
		{
			w_fp32(v.x);
			w_fp32(v.y);
			w_fp32(v.z);
			w_fp32(v.w);
		}
		inline void w_vec4i(Ivector4 v) 
		{
			w_s32(v.x);
			w_s32(v.y);
			w_s32(v.z);
			w_s32(v.w);
		}
	};
	class file_writer : public iwriter
	{
	protected:
		int hf;
		//path_notifier::excluder _excluder;
	public:
		file_writer(const char* name)
		{
			//vfs::path_notifier::excluder::excluder(&_excluder, name);
			hf = 0;
			if (name && *name)
			{
				f_name = name;
				vfs::path_build_os(f_name.c_str());
				hf = open(f_name.c_str(), 33537, 384);
			}
		}
		virtual ~file_writer()
		{
			if (hf > 0)
				_close(hf);

			//if (vfs::path_notifier::_instance)
			//	vfs::path_notifier::exclude_end(vfs::path_notifier::_instance, &this->_excluder._file);
		}
		virtual void seek(u32 pos)
		{
			R_ASSERT(valid());
			lseek(hf, pos, 0);
		}
		virtual u32 tell()
		{
			R_ASSERT(valid());
			return ::tell(hf);
		}
		virtual bool valid()
		{
			return hf > 0;
		}
		virtual void w(const void* _ptr, u32 count)
		{
			R_ASSERT(hf > 0);
			if (count)
			{
				R_ASSERT(_ptr);

				int res = write(hf, _ptr, count);
				R_ASSERT2(res==int(count), "Can't write mem block to file. Disk maybe full.");
			}
		}
	};
}