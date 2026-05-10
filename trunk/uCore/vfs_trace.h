#pragma once

namespace vfs
{
	struct internal_data
	{
		FILE* F;
		u_string marker;
		u_string id;
		u_string branch_dir;
		bool making_branch;
		bool trace_allowed;
		u_map<str_shared, u_map<u_string, bool>>* tr;
	};
	internal_data Stream;
	class UCORE_API tracer
	{
	protected:
		str_shared _types[rt_count];
		str_shared _output_dir = "";
		str_shared _map_name = "";

		bool _enabled = false;
	public:
		tracer();

		void begin();
		void end();

		void camera_track(const char* name);
		void cc(const char* name);
		void cform(const char* name);
		void config_bin(const char* name);
		void effect(const char* name);
		void env(const char* name);
		void file(const char* name);
		u_string file_name(u_string& result, const char* map);
		u_string file_name2(u_string& result, const char* map);
		void font(const char* name);
		void fxa(const char* name);
		void get_branch_files(u_vector<str_shared>* list);
		void group(const char* name);
		void identifier(const char* name, str_type stype);
		void loc_str(const char* name);
		void mark_editor();
		void mark_map(const char* name);
		void mark_running();
		void mesh(const char* name);
		void model(const char* name);
		void modifier(const char* name);
		void motion(const char* name);
		void motion_src(const char* name);
		void music(const char* name);
		void output_dir(str_shared dir);
		str_shared output_dir();
		void physx_cloth(const char* name);
		void physx_model(const char* name);
		void shader(const char* name, const char* info);
		void sound(const char* name, bool pf);
		void stable(const char* name);
		void system(const char* name);
		void texture(const char* name);
		u32 type_id(str_shared name);
		str_shared type_name(u32 id);
		void video(const char* name);
	};
	UCORE_API tracer trace;
}