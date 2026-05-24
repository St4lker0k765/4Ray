// Abstract class for editor interface, inherited in uEditor (I think)
#pragma once

class editor_interface
{
public:
	virtual e_edit_mode edit_mode() = 0;
	virtual bool ai_physics() = 0;
	virtual void vs_hide(uobject*) = 0;
	virtual void prefetch_needed() = 0;
	virtual void connect() = 0;
	virtual void show() = 0;
	virtual void run() = 0;
	virtual void init() = 0;
	virtual void disconnect() = 0;
	virtual void switch_mode() = 0;
	virtual iedit_entity* create_entity(uobject*) = 0;
	virtual void destroy_entity(iedit_entity**) = 0;
	virtual iedit_cover_link* create_cover_link() = 0;
	virtual void destroy_cover_link(iedit_cover_link**) = 0;
	virtual iedit_cover* create_cover() = 0;
	virtual void destroy_cover(iedit_cover**) = 0;
	virtual void on_cover_global() = 0;
	virtual str_shared* generate_entity_name(str_shared* result, const char*) = 0;
	virtual guid* remap_guid(guid* result, const guid*) = 0;
	virtual void on_scene_changed() = 0;
	virtual void on_map() = 0;
	virtual void on_map_end() = 0;
	virtual void on_disconnect(int) = 0;
	virtual void on_render() = 0;
	virtual void make_screenshot(int, int) = 0;
	virtual const Fvector2 render_offset(float*) = 0;
	virtual const Fmatrix render_camera() = 0;
	virtual void start_video_capture() = 0;
	virtual void toggle_video_capture() = 0;
	virtual void stop_any_hq_snapshot_processing() = 0;
	virtual u32 hq_ss_iterations() = 0;
	virtual void send_environment(const env_handle_def*) = 0;
	virtual bool show_dbg_particles() = 0;
	virtual e_shapes_draw_mode shapes_draw_mode() = 0;
	virtual struct progress_item* progress_start(float, const char*) = 0;
	virtual void progress_end(struct progress_item**) = 0;
	virtual void progress_update(struct progress_item*, float) = 0;
	virtual bool place_on_ai_map(Fmatrix, bool) = 0;
	virtual void show_obstacles_state(const u_vector<str_shared>*) = 0;
	virtual void on_motion_load(const char*, const str_shared*) = 0;
	virtual void toggle_expert_mode_deferred() = 0;
	virtual void console_parameters_changed() = 0;
};