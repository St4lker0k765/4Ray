#pragma once

namespace vfs 
{
	class package_registry
	{
		struct file 
		{
			str_shared name;

			u64 flags;
			u64 flags_;

			u64 size_real;
			u64 count;

			u64 package;
			u64 _;

			u64 size_compressed;
			u64 start;

			u32 ptr;
			u32 link_file;

			u32 duplicates;
			u32 link_layer;

		};
		struct package 
		{
			str_shared name;
			str_shared path;
			u_vector<str_shared> levels;
			x_reader handle;
			u32 id;
		};
		struct pointer
		{
			u32 package;
			u32 offset;
		};
	protected:
		u_vector<file> files[8];
		u_vector<file> duplicates;
		pointer pointers[8];

		u32 _dlc_downloaded;
		svector<package, 127> packages;
		u32 max_chunk_id;
		volatile u32 max_downloaded_chunk_id;
		guid guid_;
		epackage_format format_;
	public:
		package_registry();
		package_registry(const char* path);
		package_registry(const package_registry* other);
		~package_registry();

		u64 dlc_downloaded();
		u64 format();
		u64 level_downloaded(const char* name);
		
		void load_filedesc(file* F, u64* name, vfs::ireader* istream, u32* base_id);
		void load_name(u64* dest, vfs::ireader, u16 size, u8 crc);
		u64 load_patch(const char* path, u32 layer);
		void load_tree(char* path);

		guid* package_guid(guid* result);

		void prepare_package(package* P, const char* path);
		// Debug function
		void reopen_packages() {}
		vfs::ireader ropen(vfs::ireader* result, const char* fn);


		file* find(const char* path, u32 nearest, u32* layer);
		file* find(str_shared path, u32 nearest);
		file* find(u32 layer, file* current, const char* path, u32 nearest, u32* _layer);
		file* find_nearest(u32 layer, file* F, u32 nearest);
		file* find_tail(file* F);


	};
}
