#pragma once

struct stack_tracer
{
	u64 (*_addr)[12];
	stack_tracer(u64 (*addr)[12]);
};

class mem_debugger : public threading::mutex
{
protected:
	struct record
	{
		void* ptr;
	};
	record records[];
	struct info
	{
		void* ptr;

		LPCSTR desc;
		u32 mopid;
		u32 size;

		u64 addr[12];
		LPCSTR functs[12];
	};
	info infos[];
	threading::spin_lock qlock;
	circular_buffer<mem_debugger::info, 9> queue;
	u32 count;
	u32 mopid;
	int enabled;
	int mt;
public:
	mem_debugger();
	virtual ~mem_debugger();

	void _alloc(void* ptr, u32 size, const char* desc);
	void _append(info* I);
	void _execute(info* I);
	// Debug config only... again
	void _flush() {}
	void _free(void* ptr);
	void _worker(void* __formal);


	void dump();
	void dump_memory(u32 state_id, const char* filemask);
	void dump_prepare();

	void cleanup();
	void loss();

	void state_diff(u32 state_id);
	inline u64 state_pointer() { return mopid; }
};

class u_memory : public threading::mutex
{
protected:
	struct poolreg
	{
		LPCSTR desc;
		void* pool;
		void* memblock;
		u64 memsize;
		threading::spin_lock lock;
		poolreg();
		virtual ~poolreg() {}
	};
	svector<u_memory::poolreg, 6> pools;
	struct tstack 
	{
		u64 min;
		u64 max;
		u32 tid;
	};
	svector<tstack, 4096> tstacks;
	threading::spin_lock tstats_locks;
	mem_debugger* debug;
	u32 stat_calls;
public:
	u_memory();
	u_memory(const u_memory* other);
	virtual ~u_memory();

	void _compact(u32 full_free);
	void _dump();
	void _dump_prepare();
	void _heap(void** memblock, u64* memsize);
	void _loss();
	void _state_diff(u32 state_id);
	u32 _state_pointer();
	void _stats(bool output_debug_string, bool vm);
	u64 _usage(u32* p_blocks_used, u32 *p_blocks_free);
	u64 _usage_2(u64* total, u64* largest_block);

	const u_memory::poolreg front_pool();
	// Another debug function?
	void log_usage(bool output_debug_string) {}
	void* main_realloc(void* ptr, u64 size, u64 align, const char* _, bool allow_out_of_memory);

	void mblock_lock(void* p) {}
	u64 mblock_size(void* ptr, u32 alignment);
	void mblock_unlock(void* p) {}

	u64 pool_block_size(void* ptr);
	void* pool_create(const char* desc, u64 size, void* memblk);
	void pool_destroy(void* pool);
	char* pool_realloc(void* pool, char* p, size_t size, u64 align, const char* _, bool allow_out_of_memory);
	void pool_validate(void* pool);
	void pools0info(void* memblock, u64* memsize);

	bool ptr_on_stack(u64 p);
	void thread_attach();
	void thread_detach();

	void xmem_enable();
	void xmem_free(void* ptr, const char* _);
	char* xmem_realloc(char* ptr, u64 size, u64 align, const char* _);
	char* xmem_realloc_aligned(char* ptr, u64 size, const char* _);
};
extern UCORE_API u_memory* memory();

#ifdef DEBUG_MEMORY_NAME
// new(0)
template <class T>
IC	T* u_new()
{
	T* ptr = (T*)Memory.mem_alloc(sizeof(T), typeid(T).name());
	return new (ptr) T();
}
// new(1)
template <class T, class P1>
IC	T* u_new(const P1& p1) {
	T* ptr = (T*)Memory.mem_alloc(sizeof(T), typeid(T).name());
	return new (ptr) T(p1);
}
// new(2)
template <class T, class P1, class P2>
IC	T* u_new(const P1& p1, const P2& p2) {
	T* ptr = (T*)Memory.mem_alloc(sizeof(T), typeid(T).name());
	return new (ptr) T(p1, p2);
}
// new(3)
template <class T, class P1, class P2, class P3>
IC	T* u_new(const P1& p1, const P2& p2, const P3& p3) {
	T* ptr = (T*)Memory.mem_alloc(sizeof(T), typeid(T).name());
	return new (ptr) T(p1, p2, p3);
}
// new(4)
template <class T, class P1, class P2, class P3, class P4>
IC	T* u_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4) {
	T* ptr = (T*)Memory.mem_alloc(sizeof(T), typeid(T).name());
	return new (ptr) T(p1, p2, p3, p4);
}
// new(5)
template <class T, class P1, class P2, class P3, class P4, class P5>
IC	T* u_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5) {
	T* ptr = (T*)Memory.mem_alloc(sizeof(T), typeid(T).name());
	return new (ptr) T(p1, p2, p3, p4, p5);
}
// new(6)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6>
IC	T* xr_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6) {
	T* ptr = (T*)Memory.mem_alloc(sizeof(T), typeid(T).name());
	return new (ptr) T(p1, p2, p3, p4, p5, p6);
}
// new(7)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
IC	T* xr_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7) {
	T* ptr = (T*)Memory.mem_alloc(sizeof(T), typeid(T).name());
	return new (ptr) T(p1, p2, p3, p4, p5, p6, p7);
}
// new(8)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
IC	T* xr_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8) {
	T* ptr = (T*)Memory.mem_alloc(sizeof(T), typeid(T).name());
	return new (ptr) T(p1, p2, p3, p4, p5, p6, p7, p8);
}
// new(9)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
IC	T* xr_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P8& p9) {
	T* ptr = (T*)Memory.mem_alloc(sizeof(T), typeid(T).name());
	return new (ptr) T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
}
#else // DEBUG_MEMORY_NAME
// new(0)
template <class T>
inline T* u_new()
{
	T* ptr = (T*)memory().main_realloc(T, sizeof(T), 0, "", false);
	return new (ptr) T();
}
// new(1)
template <class T, class P1>
inline T* u_new(const P1& p1) 
{
	T* ptr = (T*)memory().main_realloc(T, sizeof(T), 0, "", false);
	return new (ptr) T(p1);
}
// new(2)
template <class T, class P1, class P2>
inline T* u_new(const P1& p1, const P2& p2) {
	T* ptr = (T*)memory().main_realloc(T, sizeof(T), 0, "", false);
	return new (ptr) T(p1, p2);
}
// new(3)
template <class T, class P1, class P2, class P3>
inline T* u_new(const P1& p1, const P2& p2, const P3& p3) 
{
	T* ptr = (T*)memory().main_realloc(T, sizeof(T), 0, "", false);
	return new (ptr) T(p1, p2, p3);
}
// new(4)
template <class T, class P1, class P2, class P3, class P4>
inline T* u_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4) 
{
	T* ptr = (T*)memory().main_realloc(T, sizeof(T), 0, "", false);
	return new (ptr) T(p1, p2, p3, p4);
}
// new(5)
template <class T, class P1, class P2, class P3, class P4, class P5>
inline T* u_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5) 
{
	T* ptr = (T*)memory().main_realloc(T, sizeof(T), 0, "", false);
	return new (ptr) T(p1, p2, p3, p4, p5);
}
// new(6)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6>
inline T* u_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6) 
{
	T* ptr = (T*)memory().main_realloc(T, sizeof(T), 0, "", false);
	return new (ptr) T(p1, p2, p3, p4, p5, p6);
}
// new(7)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
inline T* u_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7) 
{
	T* ptr = (T*)memory().main_realloc(T, sizeof(T), 0, "", false);
	return new (ptr) T(p1, p2, p3, p4, p5, p6, p7);
}
// new(8)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
inline T* u_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8) 
{
	T* ptr = (T*)memory().main_realloc(T, sizeof(T), 0, "", false);
	return new (ptr) T(p1, p2, p3, p4, p5, p6, p7, p8);
}
// new(9)
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
inline T* u_new(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P8& p9) 
{
	T* ptr = (T*)memory().main_realloc(T, sizeof(T), 0, "", false);
	return new (ptr) T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
}
#endif // DEBUG_MEMORY_NAME

template <bool _is_pm, typename T>
struct u_special_free
{
	inline void operator()(T*& ptr)
	{
		void* _real_ptr = dynamic_cast<void*>(ptr);
		ptr->~T();
		memory().main_realloc(0, sizeof(T), 0, "", false);
	}
};

template <typename T>
struct u_special_free<false, T>
{
	inline void operator()(T*& ptr)
	{
		ptr->~T();
		memory().main_realloc(0, sizeof(T), 0, "", false);
	}
};

template <class T>
inline void u_delete(T*& ptr)
{
	if (ptr)
	{
		u_special_free<std::is_polymorphic<T>::value, T>()(ptr);
		ptr = nullptr;
	}
}
template <class T>
inline void u_delete(T* const& ptr)
{
	if (ptr)
	{
		u_special_free<std::is_polymorphic<T>::value, T>()(const_cast<T*&>(ptr));
		const_cast<T*&>(ptr) = nullptr;
	}
}
