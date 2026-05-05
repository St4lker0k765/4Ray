#include "stdafx.h"
#include "u_window.h"

cwindow_base::cwindow_base()
{
	_aspect_ratio = -1.0;
	_construction_thread = GetCurrentThreadId();
	_b_active = false;
	_width = 0;
	_width_override_saved = 0;
	_b_app_active = false;
	supersample_factor = 1.0;
}

void cwindow_base::active(bool val)
{
	_b_active = val;
}

bool cwindow_base::active()
{
	return _b_active;
}

float cwindow_base::aspect_hw()
{
	validate();
	return _aspect_ratio_hardware;
}

float cwindow_base::aspect_pixel()
{
	validate();
	return _aspect_ratio_pixel;
}

float cwindow_base::aspect_safe()
{
	return _aspect_ratio;
}

bool cwindow_base::ef3_mode()
{
	return _ef3_mode;
}

void cwindow_base::get(u32* w, u32* h, float* a, float* ap)
{
	validate();
	*w = _width;
	*h = _height;
	*a = _aspect_ratio;
	*ap = _aspect_ratio_pixel;
}

void cwindow_base::get(u32* w, u32* h, u32* wo, u32* ho, float* a, float* ap)
{
	get(w, h, a, ap);
	*wo = _width_out;
	*ho = _height_out;
}

u32 cwindow_base::height()
{
	validate();
	return _height;
}

u32 cwindow_base::height_out()
{
	validate();
	return _height_out;
}

void cwindow_base::set(u32 w, u32 h, float a, float ap)
{
	_aspect_ratio = a;
	_width = w;
	_height = h;
	_aspect_ratio_pixel = ap;
}

void cwindow_base::set(u32 w, u32 h, u32 wo, u32 ho, float a, float ap)
{
	_width = w;
	_height = h;
	_width_out = wo;
	_aspect_ratio = a;
	_aspect_ratio_pixel = ap;
	_height_out = ho;
}

u32 cwindow_base::width()
{
	validate();
	return _width;
}

u32 cwindow_base::width_out()
{
	validate();
	return _width_out;
}

bool cwindow_base::windowed()
{
	return _b_windowed;
}

cwindow::cwindow()
{
	m_hwnd = 0;
	m_hwnd_render = 0;
	m_hwnd_render_override = 0;
	hoffset = 0;
	b_initialized = false;
}

void cwindow::show_cursor(bool value)
{
	if (value)
	{
		while (ShowCursor(1) < 0)
			;
	}
	else
	{
		while (ShowCursor(0) >= 0)
			;
	}
}

void cwindow::offset(u32 offs)
{
	hoffset = offs;
}

void cwindow::destroy()
{
	b_initialized = false;
}

LRESULT wnd_proc(HWND h_wnd, UINT u_msg, WPARAM w_param, LPARAM l_param)
{
    if (u_msg <= 0x16)
    {
        switch (u_msg)
        {
        case 0x16u:
            if (l_param == 1)
            {
                uconsole::server* v11 = console((threading*)h_wnd);
                v11->execute_deferred("quit");
                return 0;
            }
            return DefWindowProcA(h_wnd, u_msg, w_param, l_param);
        case WM_ACTIVATE:
            bool v9 = (_WORD)w_param && !WORD1(w_param);
            if (engine.window._b_active != v9)
            {
                engine.on_window_activate(v9);
                if (!engine.window._b_windowed && !v9)
                    ShowWindow(engine.window.m_hwnd, 6);
            }
            break;
        case WM_CLOSE:
            break;
        case WM_QUERYENDSESSION:
            if (l_param == 1)
                return 1;
        default:
            return DefWindowProcA(h_wnd, u_msg, w_param, l_param);
        }
        return 0;
    }
    switch (u_msg)
    {
    case WM_ACTIVATEAPP:
        if (engine.window._b_active != (w_param != 0))
        {
            cengine::on_window_activate(&engine, w_param != 0);
            if (!engine.window._b_windowed && !w_param)
                ShowWindow(engine.window.m_hwnd, 6);
        }
        return DefWindowProcA(h_wnd, u_msg, w_param, l_param);
    case WM_SETCURSOR:
        return 1;
    case WM_SYSCOMMAND:
        // Prevent moving/sizing and power loss in fullscreen mode
        switch (w_param)
        {
        case SC_MOVE:
        case SC_SIZE:
        case SC_MAXIMIZE:
        case SC_MONITORPOWER:
            return 1;
            break;
        }
        break;
    }
    return DefWindowProcA(h_wnd, u_msg, w_param, l_param);
}

STICKYKEYS g_StartupStickyKeys;
TOGGLEKEYS g_StartupToggleKeys;
FILTERKEYS g_StartupFilterKeys;

void cwindow::initialize(u32 w, u32 h, bool wnd, HWND hwnd, HWND hwnd_render)
{
    tagRECT rc; // [rsp+B0h] [rbp+7h] BYREF

    if (!b_initialized)
    {
        if (!w && !h)
        {
            w = 1280;
            h = 720;
        }
        b_initialized = true;
        _width = w;
        _height = h;
        _ef3_mode = w >= 3 * h;
        if (hwnd)
        {
            R_ASSERT(( NULL != hwnd ) && ( NULL != hwnd_render ));
            _b_windowed = true;
            b_own_handle = false;
            m_hwnd = hwnd;
            m_hwnd_render = hwnd_render;
        }
        else
        {
            _b_windowed = wnd;
            b_own_handle = true;
            R_ASSERT(m_hwnd == NULL);

            WNDCLASSA WndClass;
            WndClass.style = 0;
            WndClass.cbClsExtra = 0;
            WndClass.hIcon = 0;
            WndClass.lpfnWndProc = wnd_proc;
            WndClass.hInstance = GetModuleHandleA(nullptr);
            WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
            WndClass.lpszMenuName = nullptr;
            WndClass.lpszClassName = "_uengine_";
            WndClass.hIcon = LoadIconA(GetModuleHandleA(nullptr), "APP_ICON");
            RegisterClassA(&WndClass);
            SystemParametersInfoA(SPI_GETSTICKYKEYS, sizeof(g_StartupStickyKeys), &g_StartupStickyKeys, 0);
            SystemParametersInfoA(SPI_GETTOGGLEKEYS, sizeof(g_StartupToggleKeys), &g_StartupToggleKeys, 0);
            SystemParametersInfoA(SPI_GETFILTERKEYS, sizeof(g_StartupFilterKeys), &g_StartupFilterKeys, 0);
            m_dw_window_style = WS_CAPTION;

            SetRect(&rc, 0, 0, width(), height());
            AdjustWindowRect(&rc, this->m_dw_window_style, 0);
            DWORD flags = !_b_windowed ? WS_EX_TOPMOST : 0;
            if (!g_editor)
                flags |= WS_EX_APPWINDOW;

            HWND Window = CreateWindowExA(
                flags,
                "_uengine_",
                "Metro Redux",
                m_dw_window_style,
                0,
                0,
                rc.right - rc.left,
                rc.bottom - rc.top,
                nullptr,
                nullptr,
                GetModuleHandleA(nullptr),
                nullptr);
            m_hwnd = Window;
            m_hwnd_render = Window;
            if (!g_editor)
                ShowWindow(m_hwnd, SW_SHOW);
        }
        m_dw_window_style = GetWindowLongA(m_hwnd_render, -16);
        GetWindowRect(m_hwnd_render, &m_rc_window_bounds);
        GetClientRect(m_hwnd_render, &m_rc_window_client);

        if (_b_windowed)
        {
            _width = m_rc_window_client.right - m_rc_window_client.left;
            _height = m_rc_window_client.bottom - m_rc_window_client.top;
            _aspect_ratio = width() / height();
        }
        else
        {
            _aspect_ratio = aspect_hw();
        }
        rlog("* [window] initial config: [%d x %d]", width(), height());
        update_supersampling();
    }
}

void cwindow::resize(u32 w, u32 h, bool wnd)
{
    _width = w;
    _height = h;
    if (b_own_handle)
    {
        _ef3_mode = w >= 3 * h;
        if (_b_windowed)
        {
            SetWindowLongA(m_hwnd, -16, -1879048192);
        }
        else
        {
            SetWindowLongA(m_hwnd, -16, 281673728);
            tagRECT Rect;
            Rect.left = 0;
            Rect.right = width();
            Rect.bottom = height();
            AdjustWindowRect(&Rect, WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_GROUP, 0);
            char* cmd = strstr(core.params(), "-screenpos ");
            if (cmd)
            {
                int sx, sy;
                R_ASSERT(2 == sscanf(cmd + 11, "%d:%d", &sx, &sy));
                SetWindowPos(m_hwnd, nullptr, sx, sy, Rect.right - Rect.left, Rect.bottom - Rect.top, 0x160u);
            }
            else
            {
                SetWindowPos(
                    m_hwnd,
                    nullptr,
                    Rect.left + hoffset,
                    Rect.top,
                    Rect.right - Rect.left,
                    Rect.bottom - Rect.top,
                    0x160u);
            }
        }
    }
    m_dw_window_style = GetWindowLongA(m_hwnd_render, -16);
    GetWindowRect(m_hwnd_render, &m_rc_window_bounds);
    GetClientRect(m_hwnd_render, &m_rc_window_client);
    if (_b_windowed && b_own_handle)
    {
        _width = m_rc_window_client.right - m_rc_window_client.left;
        _height = m_rc_window_client.bottom - m_rc_window_client.top;
    }
    rlog("* [window] resize: [%d x %d]", width(), height());


    EXECUTION_STATE es = ES_CONTINUOUS;
    if (!_b_windowed || g_trace || g_resave)
        es = -1;
    SetThreadExecutionState(es);
    if (_b_windowed)
    {
        _aspect_ratio = width() / height();
    }
    else
    {
        _aspect_ratio = aspect_hw();
    }
    update_supersampling();
    if (g_editor)
    {
        R_ASSERT(g_render);
        g_render->capture_context();
    }
}

void cwindow::set_override_hwnd(HWND wnd)
{
    m_hwnd_render_override = wnd;
    if (wnd)
        g_commonflags.flags |= 2;
    else
        g_commonflags.flags &= ~2u;

    if (wnd)
    {
        _width_override_saved = width();
        _height_override_saved = height();
    }
}

void cwindow::update_supersampling()
{
    _width_out = width();
    _height_out = height();
    if (_b_windowed)
    {
        _aspect_ratio = width() / height();
    }
    else
    {
        _aspect_ratio = aspect_hw();
    }

    if (_b_windowed)
    {
        _aspect_ratio_pixel = 1.0;
    }
    else
    {
        _aspect_ratio_pixel = (width() / height()) / aspect_hw();
    }

    // ну нахер...
    float value = r_supersample.value;
    if (g_render)
    {
        value = g_render->cur_ssaa();
    }
    supersample_factor = sqrt(value);
    int thisa = width_out() * supersample_factor;
    _width = (thisa >> 31)
        ^ ((158 - (u8)(thisa >> 23) - 32) >> 31)
        & (((thisa | 0xFF800000) << 8 >> (-98 - (thisa >> 23)))
            - ((thisa >> 31) & (((((1 << (-98 - (thisa >> 23) - 96)) - 1) >> 8) & thisa) == 0)));

    int thisb = (float)(int)height_out() * this->supersample_factor;
    u8 v8 = 158 - (u8)(thisb >> 23);
    int v2 = ((((1 << (v8 - 96)) - 1) >> 8) & thisb) == 0;
    _height = (thisb >> 31) ^ ((v8 - 32) >> 31) & (((thisb | 0xFF800000) << 8 >> v8) - ((thisb >> 31) & v2));
}