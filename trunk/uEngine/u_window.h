#pragma once

u32 get_safe_thread()
{
	return (*(NtCurrentTeb()->Reserved1[11] + tls_index) + 4LL);
}

class wnd_thread_check
{
protected:
	u32 _construction_thread;
public:
	wnd_thread_check()
	{
		_construction_thread = GetCurrentThreadId();
	}
	void validate() 
	{
		DWORD id = GetCurrentThreadId();
		R_ASSERT2(id == _construction_thread || id == get_safe_thread(), "cannot call outside creator's thread or not from on_frame_end_work function");
	}
};

class cwindow_base : public wnd_thread_check
{
protected:
	u32 _width;
	u32 _height;

	u32 _width_out;
	u32 _height_out;

	u32 _width_override_saved;
	u32 _height_override_saved;

	float _aspect_ratio_hardware;
	float _aspect_ratio;
	float _aspect_ratio_pixel;

	bool _b_active;
	bool _b_windowed;
	bool _b_app_active;
	bool _ef3_mode;
	float supersample_factor;
public:
	cwindow_base();

	void active(bool val);
	bool active();

	float aspect_hw();
	float aspect_pixel();
	float aspect_safe();

	bool ef3_mode();

	void get(u32* w, u32* h, float* a, float* ap);
	void get(u32* w, u32* h, u32* wo, u32* ho, float* a, float* ap);

	u32 height();
	u32 height_out();

	void set(u32 w, u32 h, float a, float ap);
	void set(u32 w, u32 h, u32 wo, u32 ho, float a, float ap);

	u32 width();
	u32 width_out();

	bool windowed();
};

class cwindow : public cwindow_base
{
protected:
	u32 m_dw_window_style;

	bool b_initialized;
	bool b_own_handle;

	u32 hoffset;
	tagRECT m_rc_window_bounds;
	tagRECT m_rc_window_client;

	HWND m_hwnd;
	HWND m_hwnd_render;
	HWND m_hwnd_render_override;
public:
	cwindow();

	void destroy();
	void initialize(u32 w, u32 h, bool wnd, HWND hwnd, HWND hwnd_render);

	void offset(u32 offs);
	void resize(u32 w, u32 h, bool wnd);
	void set_override_hwnd(HWND wnd);
	void show_cursor(bool value);
	void update_supersampling();
};