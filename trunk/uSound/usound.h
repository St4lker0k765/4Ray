#pragma once

class sound_handle_def;
class scheme_item;

enum effect_type
{
	et_reverb = 0,
	et_echo
};

namespace usound
{
	struct configuration
	{
		int render_targets;
		int cache_size_L1;
		int cache_size_L2;
		float cull_volume;
	};
	enum handle_type 
	{
		ht_source = 0,
		ht_bank,
		ht_mask,
		ht_voice_set,
		ht_voice,
		ht_mix
	};
	class handle_def
	{
		virtual const str_shared name() = 0;
		virtual usound::handle_type type() = 0;
		virtual void load(const intrusive_ptr<u_archive>) = 0;
		virtual void save(const intrusive_ptr<u_archive>) = 0;
		virtual void on_modified() = 0;
		virtual bool modified() = 0;
		virtual bool valid(bool, u_string) = 0;
		virtual float max_time_total() = 0;
		virtual u16 channels_count() = 0;
		virtual bool using_as_looped() = 0;
		virtual u32 max_file_size() = 0;
		virtual u_string caption() = 0;
		virtual u32 item_count() = 0;
		virtual const str_shared item_name(u32) = 0;
		virtual const str_shared item_language(u32) = 0;
		virtual const char* item_caption(u32) = 0;
		virtual void item_load(u32, const intrusive_ptr<u_archive>) = 0;
		virtual void item_save(u32, const intrusive_ptr<u_archive>) = 0;
		virtual bool item_append(str_shared) = 0;
		virtual bool item_remove(u32) = 0;
		virtual bool item_valid(u32, bool, u_string) = 0;
		virtual u32 voice_folders(u_vector<str_shared>) = 0;
		virtual void voice_set(str_shared, str_shared) = 0;
		virtual void e_sync(u32) = 0;
		virtual bool e_localizable() = 0;
		virtual float e_loundness_spl() = 0;
		virtual bool e_subtitle() = 0;
		virtual sound_handle_def cast_handle_def() = 0;
	};
	class music_link
	{
		virtual void load(const intrusive_ptr<u_archive>) = 0;
		virtual void save(const intrusive_ptr<u_archive>) = 0;
	};
	class music_theme
	{
		virtual void load(const intrusive_ptr<u_archive>) = 0;
		virtual void save(const intrusive_ptr<u_archive>) = 0;
		virtual void e_name(const str_shared) = 0;
		virtual const str_shared name() = 0;
	};
	class music_scheme
	{
		virtual void load(const intrusive_ptr<u_archive>) = 0;
		virtual void save(const intrusive_ptr<u_archive>) = 0;
		virtual void e_name(const str_shared) = 0;
		virtual u32 e_themes_count() = 0;
		virtual std::weak_ptr<usound::music_theme> e_theme(const u32) = 0;
		virtual std::weak_ptr<usound::music_theme> e_add_theme(const str_shared) = 0;
		virtual void e_remove_theme(const std::weak_ptr<usound::music_theme>) = 0;
		virtual u32 e_links_count() = 0;
		virtual std::weak_ptr<usound::music_link> e_link(const u32);
		virtual std::weak_ptr<usound::music_link> e_add_link() = 0;
		virtual void e_remove_link(const u32) = 0;
		virtual const str_shared name() = 0;
	};
	class scheme_item
	{
		sound* sound;
		virtual const str_shared name() = 0;
		virtual const u32* priority() = 0;
		virtual const u32* dependency() = 0;
		virtual const u32* game_type() = 0;
		virtual const float* max_start_time() = 0;
		virtual const float* min_start_time() = 0;
		virtual const float* max_stop_time() = 0;
		virtual const float* min_stop_time() = 0;
		virtual bool enabled() = 0;
		virtual u8 force_out() = 0;
		virtual void load(const intrusive_ptr<u_archive>) = 0;
		virtual void save(const intrusive_ptr<u_archive>, bool) = 0;
		virtual bool valid(bool) = 0;
		virtual scheme_item* dcast_scheme_item() = 0;
	};
	class scheme
	{
		virtual intrusive_ptr<usound::scheme_item> item(u32) = 0;
		virtual const u_vector<intrusive_ptr<usound::scheme_item>> items() = 0;
		virtual const str_shared name() = 0;
		virtual void load(const intrusive_ptr<u_archive>) = 0;
		virtual void save(const intrusive_ptr<u_archive>) = 0;
		virtual bool valid(bool) = 0;
		virtual u32 item_count() = 0;
		virtual void item_append(str_shared, str_shared) = 0;
		virtual void item_clone(str_shared, str_shared) = 0;
		virtual void item_remove(u32);
	};
	class source_type
	{
		virtual const char* name(u32) = 0;
		virtual void load(const intrusive_ptr<u_archive>) = 0;
		virtual void save(const intrusive_ptr<u_archive>, bool) = 0;
	};
	enum bus_type : u8
	{
		bt_master = 0,
		bt_music,
		bt_effects,
		bt_dialogs,
		bt_video,
		bt_user0 = 8,
		bt_user1,
		bt_user2,
		bt_user3,
		bt_total_count,
		bt_from_source = u8(-1),
	};
	class effect_def
	{
		virtual const char* prefix() = 0;
		virtual bool load(const intrusive_ptr<archive>) = 0;
		virtual void save(const intrusive_ptr<archive>) = 0;
		effect_type type;
		str_shared _name;
	};
	class voice_type
	{
		u32 id;
		str_shared prefix;
		str_shared caption;
	};
	enum filter_type : u8
	{
		ft_none,
		ft_gasmask,
		ft_radio,
		ft_distance,
		ft_vmeter,
		ft_horn,
		ft_intercom,
		ft_closed_door,
		ft_total_count,
	};
	class play_params
	{
		flags32 _play_flags;
		float _priority_scale;
		u32 _delay;
		float _accrue_interval;
		u32 _time_offset_us;
		u32 _time_end_us;
		str_shared _source_name;
		str_shared _voice;
		Fvector3 _position;
		const uobject* _object;
		u16 _sector;
		bus_type _bus;
		filter_type _filter;
		str_shared _lang;
	};
	class user_data : public intrusive_base
	{
		// empty. wtf?
	};
	class play_feedback : public intrusive_base
	{
		Fvector4 _global_pos;
		Fvector4 _local_pos;
		const play_params _play_params;
		serializable_callback _end_cb;
		intrusive_ptr<user_data> _user_data;
		float _volume;
		float _pitch;
		float _fade_interval;
		float _time_current;
		volatile int _position_initialized;
		enum state : u8
		{
			fs_playing = 0,
			fs_stop_deferred,
			fs_stop,
			fs_stopped,
		};
		state _server_state;
		state _client_state;
		threading::spin_lock _data_lock;
	};
	class sound
	{
		u32 _cache_id;
		std::weak_ptr<handle_def> _handle;
		intrusive_ptr<play_feedback> _feedback;
	};
	struct event
	{
		intrusive_ptr<user_data> user_data;
		Fvector3 start;
		Fvector3 finish;
		float volume;
		u32 start_time;
		u16 sector;
		bool non_positional;
		str_shared name;
	};
	class statistic
	{
		int _rendered;
		int _looped;
		int _simulated;
		int _io_pending;
		int _L1_cache_hits;
		int _L1_cache_misses;
		int _L2_cache_hits;
		int _L2_cache_misses;
		float _v_dist_time;
		float _bus_volume[12];
		struct source_info
		{
			str_shared name;
			u32 count_s;
			u32 count_r;
			float dbSPL;
			u32 count_3d;
			u32 count_2d;
			u32 as_music;
			int voice_over;
			u32 ai_snd_type;
		};
		svector<source_info, 64> _sources;
	};
	class manager
	{
		virtual void i_destroy_handle_def(usound::handle_def*) = 0;
		virtual void _initialize(usound::configuration*) = 0;
		virtual void _destroy() = 0;
		virtual void load() = 0;
		virtual void loader(bool) = 0;
		virtual void hold_music(bool) = 0;
		virtual void mute(bool) = 0;
		virtual void pause(bool) = 0;
		virtual std::weak_ptr<usound::scheme> find_scheme(const str_shared) = 0;
		virtual const usound::configuration config() = 0;
		virtual bool language_enabled(u32) = 0;
		virtual void vfs_trace(const str_shared, bool) = 0;
		virtual void save(const s32) = 0;
		virtual void save_bin(const s32) = 0;
		virtual void e_stop_sounds() = 0;
		virtual void e_refresh() = 0;
		virtual u32 e_refresh_masks(const u_vector<std::weak_ptr<usound::handle_def>>, u_vector<std::weak_ptr<usound::handle_def>>) = 0;
		virtual void e_invalidate_targets() = 0;
		virtual u32 e_source_type_count() = 0;
		virtual usound::source_type e_source_type(u32) = 0;
		virtual u32 e_handle_count() = 0;
		virtual usound::handle_def* e_handle(u32) = 0;
		virtual void toggle_mute() = 0;
		virtual usound::handle_def* e_handle_create(usound::handle_type, const str_shared*, bool) = 0;
		virtual void e_change_source_localizible(const u_vector<str_shared>, bool) = 0;
		virtual usound::handle_def* e_handle_rename(str_shared, str_shared) = 0;
		virtual void e_handle_delete(const str_shared, u32*, u_vector<std::weak_ptr<usound::handle_def>>) = 0;
		virtual usound::handle_def* e_handle_find(const str_shared) = 0;
		virtual void e_voice_set_clear(const str_shared, bool, u32*) = 0;
		virtual void e_clear_invalid_handles() = 0;
		virtual u32 e_scheme_count() = 0;
		virtual usound::scheme* e_scheme(u32) = 0;
		virtual usound::scheme* e_scheme_create(const str_shared) = 0;
		virtual usound::scheme* e_scheme_clone(const str_shared, const str_shared) = 0;
		virtual usound::scheme* e_scheme_rename(const str_shared, const str_shared) = 0;
		virtual void e_scheme_delete(const str_shared) = 0;
		virtual usound::scheme* e_scheme_find(const str_shared) = 0;
		virtual usound::effect_def* e_effect(u32) = 0;
		virtual u32 e_effects_count() = 0;
		virtual void e_invalidate_effects() = 0;
		virtual void e_portal_stat(u32, float*, bool*) = 0;
		virtual void e_dbg_initialize() = 0;
		virtual u32 e_voice_count() = 0;
		virtual const usound::voice_type* e_voice(u32) = 0;
		virtual const str_shared e_voice_caption(const str_shared) = 0;
		virtual void create(usound::sound*, const str_shared) = 0;
		virtual void play(usound::sound*, const usound::play_params*) = 0;
		virtual str_shared select(usound::sound*, const str_shared, const str_shared) = 0;
		virtual void language(const char*) = 0;
		virtual const str_shared language() = 0;
		virtual const str_shared language_target() = 0;
		virtual const u_vector<str_shared> languages() = 0;
		virtual void remove_end_callback(const serializable_callback*) = 0;
		virtual void update_begin(const Fvector4, const Fvector4, const Fvector4, bool);
		virtual void update_end_sync(u_vector<usound::event>, u_vector<serializable_callback>) = 0;
		virtual void statistic(usound::statistic*) = 0;
		virtual void level_load() = 0;
		virtual void level_unload() = 0;
		virtual void apply_language() = 0;
		virtual void level_load_active(const intrusive_ptr<u_archive>) = 0;
		virtual void level_save_active(const intrusive_ptr<u_archive>) = 0;
		virtual bool load_occlusion(const intrusive_ptr<vfs::ireader>) = 0;
		virtual void prefetch_sounds() = 0;
		virtual u16 listener_sector() = 0;
		virtual void listener_sector(u16) = 0;
		virtual void set_sector_id_cb(const fastdelegate::FastDelegate<u16 (uspatial::collider&, Fvector4 const&)>*) = 0;
		virtual bool virtual_direction(Fvector4, u16, const Fvector4) = 0;
		virtual float virtual_distance_precise(u16, u16, const Fvector4, const Fvector4, float, float, bool) = 0;
		virtual float virtual_distance_approximate(u16, u16, const Fvector4, const Fvector4, float, float, bool) = 0;
		virtual bool portal_throughput(const str_shared, float, float, bool) = 0;
		virtual void reset_portal_throughput() = 0;
		virtual void direct_volume(usound::bus_type, float) = 0;
		virtual void blend_volume(usound::bus_type, float, float) = 0;
		virtual void logical_freq(float) = 0;
		virtual float user_master_volume() = 0;
		virtual float user_music_volume() = 0;
		virtual void video_playing(bool) = 0;
		virtual void music_mute(bool, bool) = 0;
		virtual void background_music(const str_shared, float, float, float) = 0;
		virtual const str_shared background_music() = 0;
		virtual void background_music_reset() = 0;
		virtual void unload_dynamic() = 0;
		virtual void on_level_start() = 0;
		virtual void on_map_end() = 0;
		virtual IXAudio2 xaudio() = 0;
		virtual void set_music_scheme(const str_shared) = 0;
		virtual void play_music_theme(const str_shared) = 0;
		virtual void stop_music(const u32) = 0;
		virtual void vfs_trace_music_scheme(const str_shared) = 0;
		virtual void e_fill_music_schemes(u_vector<str_shared>) = 0;
		virtual void e_fill_music_themes(u_vector<str_shared>) = 0;
		virtual u32 e_music_schemes_count() = 0;
		virtual std::weak_ptr<usound::music_scheme> e_music_scheme(u32) = 0;
		virtual std::weak_ptr<usound::music_scheme> e_add_music_scheme(const str_shared) = 0;
		virtual void e_remove_music_scheme(const std::weak_ptr<usound::music_scheme>) = 0;
	};
}

extern USOUND_API usound::manager* g_sound;