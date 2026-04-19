#pragma once

class mem_debugger : threading::mutex
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
	void _state_pointer();
	void _stats(bool output_debug_string, bool vm);
	u64 _usage(u32* p_blocks_used, u32 *p_blocks_free);
	u64 _usage_2(u64* total, u64* largest_block);

	const u_memory::poolreg front_pool();
	// Another debug function?
	void log_usage(bool output_debug_string) {}
	void main_realloc(void* ptr, u64 size, u64 align, const char* _, bool allow_out_of_memory);

	void mblock_lock(void* p) {}
	u64 mblock_size(void* ptr, u32 alignment);
	void mblock_unlock(void* p) {}

	u64 pool_block_size(void* ptr);
	void* pool_create(const char* desc, u64 size, void* memblk);
	void pool_destroy(void* pool);
	char* pool_realloc(void* pool, char* p, size_t size, u64 align, const char* _, bool allow_out_of_memory);
	void pool_validate(void* pool);
	void pools0info(u64* memblock, u64* memsize);

	bool ptr_on_stack(u64 p);
	void thread_attach();
	void thread_detach();

	void xmem_enable();
	void xmem_free(void* ptr, const char* _);
	void xmem_realloc(char* ptr, u64 size, u64 align, const char* _);
	void xmem_realloc_aligned(char* ptr, u64 size, const char* _);
};

extern UCORE_API u_memory* memory();