#pragma once

enum save_data_state
{
	sds_invalid = 0,
	sds_decompressed,
    sds_compressed
};

class save_data
{
	save_data_state state;
	
	vfs::istorage header;
	u32 header_size;

	vfs::istorage dynamic;
	u32 dynamic_size;

	vfs::istorage active;
	u32 active_size;

	u_vector<task_result> uncompressed_save;
};

class gather_context;
class human;

class igame_hud : public iconstructable
{
	virtual iconstructable*  _construct() = 0;
	virtual void _destruct() = 0;
	virtual void dtr_igame_hud() = 0;
	virtual void reload() = 0;
	virtual void render_cursor() = 0;
	virtual void render_ui() = 0;
	virtual void render_post(gather_context* context, const engine_camera* camera) = 0;
	virtual void render_forward(gather_context* context, const engine_camera* camera) = 0;
	virtual bool is_need_hide_menu() = 0;
	virtual void start_game_news(uobject* ent) = 0;
	virtual void end_game_news() = 0;
	virtual void add_game_news(const str_shared* news) = 0;
	virtual u32 get_popup_key_id(int action_id) = 0;
	virtual void full_reset() = 0;
	virtual void on_map() = 0;
	virtual void allow_inventory(bool ui_on, bool civil_on) = 0;
	virtual void update(bool, human* owner, const engine_camera* ec) = 0;
	virtual bool process_mouse_move(int action, float val, human* owner) = 0;
	virtual bool on_key_press(int action, int key, u32 flags, human* human) = 0;
	virtual bool on_key_hold(int action, int key, u32 flags) = 0;
	virtual bool on_key_release(int action, int key, u32 flags) = 0;
	virtual Fvector2 get_cursor_pos_i() = 0;
	virtual Fvector2 get_cursor_pos_p() = 0;
	virtual Fvector2 get_hotspot_offset() = 0;
	virtual void update_cursor_move() = 0;
	virtual void retranslate() = 0;
	virtual bool is_weapon_active() = 0;
	virtual bool is_weapon_enabled() = 0;
	virtual bool weapon_shown() = 0;
	virtual bool is_trade_show() = 0;
	virtual void force_menu_particles(engine_camera* cam) = 0;
	virtual void load_active(const u_archive* arch, u16 version) = 0;
	virtual void save_active(u_archive* arch) = 0;
	virtual void play_menu_particles(str_shared name, Fvector4 position) = 0;
	virtual void stop_menu_particles(str_shared name) =0;
	virtual void stop_all_menu_particles(u8 t, bool deferred) = 0;
	virtual void wipe_hud_particles() = 0;
	virtual void play_track_particles(str_shared name, u8 hud_type, bool constr) = 0;
	virtual void stop_track_particles(str_shared name, bool deferred) = 0;
	virtual void stop_all_hud_particles(u8 t, bool deferred) = 0;
};

class igame
{
protected:
	igame_hud* _hud;
	camera_manager* _cameras;
	cleaderboard* _leaderboard;
	cenvironment environment;
	flares::manager* flares;

	struct callback_manager
	{
		u_vector<serializable_callback> _callbacks_pend;
		u_vector<serializable_callback> _callbacks_proc;
		threading::spin_lock lock;
		bool _allow_unknowns;
	};
	callback_manager mt;
	callback_manager mt2;
	callback_manager serial;
	callback_manager gamebegin;

	threading::spin_lock vsd_block;
	u_vector<uobject*> vsd_queue;

	volatile int dbg_saving;
	bool disable_save;

	float _time;
	u32 _skip_frames;
	u32 _required_save_size;

	enum eserial_state
	{
		eserial_do_nothing,
		eserial_save,
		eserial_save_waiting_for_compress,
		eserial_save_waiting_for_safe_zone,
		eserial_save_complete,
		eserial_load,
		eserial_load_active,
		eserial_unpause
	};
	eserial_state _serial_state;
	str_shared _serial_save_fn;
	str_shared _serial_load_fn;

	const char* _pause_reason;
	bool _ingame_return_to_main_menu;

	u8 _start_profile_load_ok : 1;

	save_data _all_save;
	vfs::ireader _all_header;

	u_archive _level_active_state;
	std::array<u_archive, 1> _level_add_dynamic;
	std::array<u_archive, 1> _level_add_active;

	str_shared _level_name;
	str_shared _build_version;
	u16 _save_version;

	u32 _sync_time;

	vfs::istorage _player_storage;
	u_archive _file;

	struct game_data
	{
		control_presets_storage controls_defaults;
		ui_item* disk_reading_icon;
		ui_item* disk_writing_icon;
		ui_item* dbg_disable_save_icon;
		ui_item* disk_bg_icon;
	};
	game_data _data;

	u_vector<str_shared> _open_levels;
	str_shared _autosave_map_name;

	s32 _levels_corrupted : 5;
	s8 _preferences_corrupted : 1;
	s8 _autosave_corrupted : 1;
	s8 _start_from_dashboard_once : 1;

	save_data _save;
	struct last_checkpoint
	{
		threading::spin_lock locker;
		vfs::istorage short_save;
		save_data long_save;
	};
	last_checkpoint _last_cp;
	svector<save_content_wi*, 4> _pendings_wi;
};