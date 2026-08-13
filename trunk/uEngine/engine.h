#pragma once
#include "locale.h"
#include "u_window.h"
#include "stable.h"

class iconstructable
{
	virtual iconstructable* _construct() {}
	virtual void _destruct() {}
	virtual void dtr_iconstructable() {}
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
	volatile enum mapstate : u32
	{
		mapstate_loading = 1 << 0,
		mapstate_loaded_and_waiting = 1 << 1,
		mapstate_quickload = 1 << 2,
		mapstate_running = 1 << 3,
		mapstate_initializing = 1 << 4
	};
	volatile mapstate flags_mapstate;
	editor_interface* p_editor;

	cengine();
	~cengine();

	void append_error_message(const char* msg);
	void check_language_change();
	void core_level_downloaded_thread();

	void create();
	void destroy();

	inline bool do_not_play_video() { return g_benchmark || (!g_editor || p_editor && p_editor->ai_physics()) && _mp || g_editor; }
	inline e_edit_mode edit_mode() { g_editor ? p_editor->edit_mode() : em_game; }

	void hide_message(str_shared key);
	void hide_popup(str_shared message);
	void show_message(urender::font* fnt, float x, float y, float size, u32 color, locale* message, u32 ttl);
	void show_message(locale* message, u32 ttl);
	void show_popup(locale* message, bool big, u32 ttl);

	inline float hud_fov() { return (r_base_hud_fov.value * _hud_fov_coef) * _hud_fov_collision; }
	inline void hud_fov_coef(float v) { R_ASSERT(_valid(v)); _hud_fov_coef = v; }
	inline float hud_fov_coef() { return _hud_fov_coef; }
	inline void hud_fov_collision(float v) { R_ASSERT(_valid(v)); _hud_fov_collision = v; }
	inline float hud_fov_collision() { return _hud_fov_collision; }
	inline float hud_fov_custom(float hud_fov_collision) { return (r_base_hud_fov.value * _hud_fov_coef) * hud_fov_collision; }

	inline void hud_offset(Fmatrix m) { _hud_offset = m; }
	inline Fmatrix hud_offset() { return _hud_offset; }

	inline bool ideogram() { return _ideogram; }
	
	inline bool is_ai_physics() { return g_editor ? p_editor->ai_physics() : true; }
	inline bool is_changing_lang() { return _changing_stable; }
	inline bool is_edit_mode() { return g_editor && p_editor->edit_mode(); }
	bool is_lang_supported(str_shared l);
	inline bool is_spawn_save() { return spawn_save; }
	bool is_very_first_start();

	void lang_text(const char* l, bool force);
	inline str_shared lang_text() { return _lang_text; }
	u_vector<language_type_rec> language_types() { return _language_types; }

	bool loader_execute(const char* mname, u32 from_continue, int from_gameload);
	void loader_execute_QL(const char* sname, void* sdata, int net);
	void loader_finish();
	void loader_log(const char* phase);
	void loader_render_while_loading();
	void loader_secondary();
	void loader_secondary_QL();
	void loader_wait_finish();
	void loader_wait_start();

	bool mapstate_delayed();

	inline emp_mode mp()
	{
		if (!g_editor)
			return _mp;
		if (p_editor && p_editor->ai_physics())
			return _mp;

		return emp_none;
	}
	inline bool mp_coop() { return _mp == emp_coop; }
	inline bool mp_not_coop() { return _mp != emp_coop; }
	inline bool mp_pure() { return _mp; }

	void on_window_activate(bool active);

	void pause(bool b_on);
	inline size_t pause_stack_size() { return _pause_stack.size(); }
	inline bool paused() { return time.bpaused && !time.bforce_notpaused; }
	void pop_pause(str_shared reason, bool nocrash);
	void print_pause_stack();
	void push_pause(str_shared reason);

	void prepare_oa_tests();
	void process_error_messages();

	inline bool r_editor() { return *r_editor.uconsole::cmd_integer::value && flags_mapstate == mapstate_running && !g_trace; }
	
	inline u32 r_force_actor_pos()
	{
		return *r_force_actor_pos.uconsole::cmd_integer::value;
	}
	
	void retranslate();

	void run();
	void run_oa_tests();
	void run_prepare();
	void run_primary();
	void run_secondary();
	void run_serial();
	void run_unprepare();

	bool save_config(u_vector<str_shared>* files);

	// WTF?
	inline bool skip_on_restart() { return false; }

	ui_item* storage_icon();
	void storage_icon_clear();
	void storage_icon_draw();
	void storage_icon_hide();
	void storage_icon_lock(int add);
	void storage_icon_show(ui_item* icon, ui_item* bg, u32 ttl, float scale);

	void switch_to_edit_mode();

	inline u_vector<str_shared> text_languages() { return _text_languages; }
};

extern ENGINE_API cengine engine;