#pragma once

class engine_camera
{
protected:
	Fvector position;
	Fvector direction;
	Fvector up;
	Fvector right;
	
	Fmatrix m_view;
	Fmatrix m_project;
	Fmatrix m_viewproject;

	frustum view;
	float view_distance;

	Fvector3 lum_color;
	Fvector3 lum_dir;

	struct spp
	{
		Fvector4 dof;
		float yfov;
		float hud_fov;
		float hud_fov_total_weight;
		Fvector4 dof_total_weight;

		float noise;
		float noise_weight;

		Fvector3 cadd;
		float cadd_weight;

		Fvector4 cdot;
		float cdot_weight;

		Fvector3 cmul;
		float cmul_weight;

		enum e_flags : u32
		{
			e_dof = 1 << 0,
			e_noise = 1 << 1,
			e_cmul = 1 << 2,
			e_cadd = 1 << 3,
			e_cdot = 1 << 4,
			e_fov = 1 << 5,
			e_hud_fov = 1 << 6,
			e_dof_sky = 1 << 7,
		};
		e_flags flags;

		spp();

		void e_zero();
		void normalize();
	};
	spp m_pp;

	float _yfov;
	float _aspect_ratio;

	float _motion_blur;
	float _bloom_scale;
	float _game_gasmask;

	float _force_feedback_0;
	float _force_feedback_1;

	float _game_gasmask_approach;
	float _target_gasmask_approach;
	float _last_game_gasmask_approach;
public:
	engine_camera();
	engine_camera(const engine_camera* other);

	void approach_step(float dt);
	void aspect_ratio(float value);
	float aspect_ratio() { return _aspect_ratio; }

	void bloom_scale(float value);
	float bloom_scale() { return _bloom_scale; }

	void build_precache_frame();

	void disable_gasmask();
	bool disabled_gasmask() { return !fis_zero(_target_gasmask_approach); }

	void force_feedback_0(float value);
	float force_feedback_0() { return _force_feedback_0; }

	void force_feedback_1(float value);
	float force_feedback_1() { return _force_feedback_1; }

	float game_gasmask_approach();

	void motion_blur(float value);
	float motion_blur() { return _motion_blur; }

	void reset_gasmask();
	void set_gasmask(float value);

	void yfov(float value)
	{

	}
	float yfov() { return _yfov; }
};