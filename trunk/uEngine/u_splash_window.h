#pragma once

class u_splash_window
{
protected:
	HWND _hwnd;
	HBITMAP _hBitmap;
	u32 _dwWidth;
	u32 _dwHeight;
public:
	~u_splash_window();

	void Paint(HWND hWnd);
	void hide();
	void show();
	void show_splash();
};

u_splash_window Splash;