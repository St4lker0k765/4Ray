#pragma once

typedef intrusive_ptr<vfs::ireader, intrusive_base>* ireader_ptr;
typedef intrusive_ptr<vfs::iwriter, intrusive_base>* iwriter_ptr;
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
	struct file 
	{
		u_string name;
		u64 time_write;
		u64 size;
		u32 attrib;
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
	void add_extension(char** dst, string1024* exts, const char* info, char* ext);
	u_string append_name_prefix(u_string result, const char* fn);
	void check_file_name(char* fn);
	u64 check_package(const char* filename, const void* data, u32 size);
	void commit_copying(u_vector<str_shared> list);
	void copy_resource(u32 type, const char* res_name);
	void critical_io_error(const char* path, const char* expr, const char* file, const char* func, int line);
	void dll_get_func<void* (void*, u32, u32, u32)>(
		void* (** func)(void*, u32, u32, u32),
		HMODULE hm,
		const char* func_name);
	HMODULE dll_load(const char* name);
	u64 exist_os_path(const char* path, const int file, u32* size);
	bool exists(const char* fn);
	bool exists_os(const char* fn);
	bool exists_os(const char* fn, string_path* path);
	bool exists_os_impl(string_path* path, const char* _fn, int file, u32* size);
	u64 file_age_os(const char* fn);
	void file_age_os(const char* nm, u64 age);
	void file_backup_os(const char* fn);
	u64 file_copy(const char* src, const char* dest);
	bool file_copy_os(const char* src, const char* dest, const int overwrite);
	u64 file_create_time_os(const char* fn);
	u64 file_delete_ex(const char* fn);
	u32 file_delete_os(const char* fn);
	u64 file_list(u_vector<vfs::file> dest, const char* path, const char* mask, bool b_files, bool b_folders, bool b_clamp_ext, bool b_root_only, fastdelegate::FastDelegate1<vfs::iterator_file&, int>* cb);
	u64 file_list(u_vector<vfs::file> dest, vfs::package_registry* package);
	bool file_readonly_os(const char* fn);
	void file_rename_os(const char* src, const char* dest, const int overwrite, const int wait);
	u64 file_size(const char* fn);
	u64 file_size_os(const char* fn);
	bool folder_exists_os(const char* fn);
	u64 get_open_filename(const char* title, const char* init_dir, char* buffer, int sz_buffer, const char* mask, bool bMulti);
	u64 get_save_filename(const char* title, const char* init_dir, char* buffer, int sz_buffer, const char* mask);
	bool is_term(char a);
	__time64_t local_time();
	void log_file(const char* type, const char* name);
	void log_resource(const str_shared* type, char* name, bool force_trace_allowed, const char* info);
	const char* make_filter(string1024* dest, const char* info, char* ext);
	void make_path(string_path* path, const char* fn);
	void mark(const char* name) {}
	u64 package_version();
	void path_build_os(char* path);
	void rbuffered(const char* fn, const fastdelegate::FastDelegate<bool __cdecl(void*&, u64)>* cb);
	void rbuffered_os(const char* fn, const fastdelegate::FastDelegate<bool __cdecl(void*&, u64)>* cb);
	void rbuffered_package(vfs::package_registry* package, const char* fn, const fastdelegate::FastDelegate<bool __cdecl(void*&, u64)>* cb, const int force_raw);
	void registry();
	bool registry_exists();
	ireader_ptr ropen(ireader_ptr result, const char* fn);
	ireader_ptr ropen_os(ireader_ptr result, char* fn);
	ireader_ptr ropen_package(ireader_ptr result, vfs::package_registry* package, const char* fn, bool force_raw, u32* uncompressed_size);
	ireader_ptr ropen_raw(ireader_ptr result, char* fn);
	u64 test_package_permanent(const char* fn);
	__time64_t to_time_t(const _SYSTEMTIME* st);
	intrusive_base* wopen_ex(intrusive_base* result, const char* fn);
	iwriter_ptr wopen_os(iwriter_ptr result, const char* fn);
	iwriter_ptr wopen_os_buf(iwriter_ptr result, const char* fn, u32 buf_size);
	iwriter_ptr wopen_safe(iwriter_ptr result, const char* fn, const int __formal);

}

#include "vfs_package_registry.h"
#include "vfs_trace.h"
#include "vfs_x_reader.h"