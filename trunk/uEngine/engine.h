#pragma once
#include "locale.h"
#include "u_window.h"
#include "stable.h"

class iconstructable
{
	virtual void _destruct() {}
};

// Class creation/destroying interface
extern "C" {
typedef DLL_API  iconstructable*	  __cdecl Factory_Create	(str_shared clsid, str_shared static_data_key);
typedef DLL_API  void		  __cdecl Factory_Destroy	(iconstructable* O);
};

class ENGINE_API cengine
{
protected:
	HINSTANCE h_game;
	HINSTANCE h_render;
	HINSTANCE h_editor;
	HINSTANCE h_particles;
	HINSTANCE h_sound;
	HINSTANCE h_skeleton;
	HINSTANCE h_vtune;

	Factory_Create* p_create;
	Factory_Destroy* p_destroy;

	editor_interface* p_editor;
	enum espawn_save : u8
	{                                     
		espawn_save_none,
		espawn_save_clone,
		espawn_save_level
	};
	espawn_save spawn_save;
	bool e_load_dynamic;
	bool _first_play;
	bool _first_play_checked;

	localization::stable* _new_stable;
	u_vector<language_type_rec> _language_types;
	u_vector<str_shared> _text_languages;
	str_shared _lang_text;

	bool _ideogram;
	bool _changing_stable;

	enum emp_mode : u8
	{                     
		emp_none,
		emp_dm,
		emp_team_dm,
		emp_coop,
		emp_siege,
		emp_capture_point,
		emp_coop_survival,
		emp_count
	};
	emp_mode _mp;

	cwindow window;

	u32 frame;
	u32 warmup_frame;
	u32 warmup_total_frames;
	bool flags_bench_enabled;
	bool session_flag;

	game_stats game_stats;

	bool steam_ready;
	bool flags_ready;
	bool flags_rendering;
	bool flags_serial_rendering;

	volatile bool flags_skip_load;
	volatile bool maploader_quit_to_menu;
	volatile bool core_level_downloaded;
	volatile bool core_level_downloaded_thread_exit;
	volatile bool language_changed;
	str_shared core_level_downloaded_name;
	
	volatile enum mapstate : u32
	{
		mapstate_loading = 1 << 0,
		mapstate_loaded_and_waiting = 1 << 1,
		mapstate_quickload = 1 << 2,
		mapstate_running = 1 << 3,
		mapstate_initializing = 1 << 4
	};
	volatile mapstate flags_mapstate;
	u32 mapstate_delay_start;

	bool showing_error;
	centityscheduler scheduler;
	engine_timing time;
	engine_camera camera;
	engine_material_factory material;

	locale message;
	u32 message_ttl;
	Fvector2 message_pos;
	float message_height;
	u32 message_color;
	urender::font message_font;

	locale popup_message;
	bool popup_big;
	u32 popup_start;
	u32 popup_ttl;

	threading::spin_lock _message_icon_lock;
	u32 _message_icon_counter;
	const ui_item* _message_icon;
	const ui_item* _message_icon_bg;
	u32 _message_icon_start;
	u32 _message_icon_ttl;
	float message_icon_scale;

	cprofiler profiler;
	threading::taskpool mtrpc;
	
	timing::timer loader_timer;
	u32 _loader_iotimebase;
	const char* loader_name;
	const char* save_file_name;
	void* save_data;
	int save_net;

	threading::semaphore secondary_ready2simulate;
	threading::semaphore secondary_finished;
	volatile bool secondary_must_exit;

	Fmatrix _hud_offset;
	float _hud_fov_coef;
	float _hud_fov_collision;

	iwaiter* _waiter;
	u_vector<str_shared> _pause_stack;
	u_vector<u_string> _error_messages;
	threading::spin_lock _error_messages_lock;
public:
	cengine();
	~cengine();

	void append_error_message(const char* msg);
	void check_language_change();
	void core_downloaded_thread(void* __formal);

	void create();
	void destroy();

	bool do_not_play_video();
	e_edit_mode edit_mode();

	void hide_message(str_shared key);


};

extern ENGINE_API cengine engine;