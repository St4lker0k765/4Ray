#pragma once

#define STABLE_HASH_SIZE 1021
namespace localization
{
	struct entry
	{
		str_shared key;
		str_shared value;

		entry* next;
	};
	class sconverter
	{
	protected:
		u_vector<std::pair<wchar_t, u32>> frequency;
		u_vector<wchar_t> charmap;
		u32 append_counter;
	public:
		void append(wchar_t* text);
		u_vector<wchar_t> mapping() { return charmap; }
		void sort();
	};
	class stable
	{
	protected:
		u_vector<wchar_t> charmap;
		entry* hash[STABLE_HASH_SIZE];
		u_vector<entry> storage;
		str_shared _lang;
	public:
		stable();
		stable(const stable* other);
		~stable();

		void append(str_shared key, str_shared value);
		void assign_cmap(u_vector<wchar_t> ctable);
		void decode(str_shared text, wchar_t* dest, u32 size);
		void destroy();
		str_shared encode(wchar_t* text);
		u32 extract_char(const char* text, char* dest, u32 size);
		u_vector<wchar_t> get_charmap() { return charmap; }
		entry* get_entry(int bucket) { return hash[bucket]; }
		void initialize();
		str_shared lang() { return _lang; };
		void load(const char* lng_id, const char* path);
		void load(vfs::ireader* R, str_shared lng_id);
		void patch(u_vector<entry>* items);
		str_shared translate(str_shared key, bool* exist);
		str_shared translate(str_shared key) { return translate(key, false); }
		void unload();
		bool valid_char(wchar_t C);
		bool valid_string(wchar_t* text);
	};
}