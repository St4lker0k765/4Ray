#pragma once

class timer_queue : public threading::spin_lock
{
protected:
	u_vector<std::pair<u32, serializable_callback>> queue;
public:
};

class ENGINE_API engine_timing
{
protected:
	timer_queue callbacks;
	u_vector<fastdelegate::FastDelegate<float()>> _slowmo_changers;
	float _slowmo_changers_scale;
	bool bpaused;
	bool bforce_notpaused;
	bool bpaused_deferred;
	float slowmo_scale;
	float slowmo_scale_outer;
	float slowmo_scale_debug;
	float slowmo_scale_changers;
	u64 accumulator_scaled;
	u64 accumulator_not_scaled;
	float _delta_f_prev;
	float _delta_f;
	u32 _delta_ms;
	u64 _delta_us;
	float _global_f;
	u32 _global_ms;
	u32 _global_ms_no_slowmo;
	u32 _global_ms_async;
	u64 _global_us;
	double _global_d;
	u32 _async_delta_ms;
	u32 _async_delta_mcs;
	float _fps;
	timing::timer timer;
	timing::timer timer_os;
	u32 timer_os_delta;
	u32 engine_start_time;
public:
	void apply_slowmo();
	u32 async();
	u32 async_delta();
	u32 async_delta_mcs();
	float async_f();
	u64 async_os();
	u64 async_us();
	float calc_slowmo();
	float delta_f();
	float delta_f_prev();
	u32 delta_ms();
	u64 delta_us();
	void engine_loaded();
	void engine_started();
	float fps();
	u32 global();
	u32 global_async();
	double global_d();
	u32 global_no_slowmo();
	u64 global_us();
	void initialize();
	void load_active(const gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* arch, u16 version);
	void pause_deferred(bool b);
	void paused(bool b);
	bool paused();
	void register_slowmo_changer(fastdelegate::FastDelegate<float()> sc);
	void reset_slowmo(bool with_debug);
	void save_active(const gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* arch);
	void slowmo(float f);
	float slowmo();
	void slowmo_debug(float f);
	float slowmo_debug();
	void slowmo_debug_decrease();
	void slowmo_debug_increase();
	void slowmo_debug_set_max();
	void slowmo_debug_set_min();
	void slowmo_outer(float f);
	float slowmo_outer();
	void unregister_slowmo_changer(fastdelegate::FastDelegate<float()> sc);
	void update();
	void update_slowmo_changers(float dt);
};