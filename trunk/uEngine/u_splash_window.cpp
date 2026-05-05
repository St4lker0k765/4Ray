#include "stdafx.h"
#include "u_splash_window.h"
#include "resource.h"

u_splash_window::~u_splash_window()
{
	DestroyWindow(_hwnd);
	DeleteObject(_hBitmap);
}

void u_splash_window::show()
{
	show_splash();
}

LRESULT SplashWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_PAINT:
		{
			Splash.Paint(hWnd);
			break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void u_splash_window::show_splash()
{
	tagRECT rect; // [rsp+40h] [rbp-28h] BYREF

	_hBitmap = LoadBitmap(GetModuleHandleA(0), MAKEINTRESOURCE(IDB_BITMAP1));
	_dwWidth = 640;
	_dwHeight = 480;
	_hwnd = CreateDialogParam(GetModuleHandleA(0), MAKEINTRESOURCE(IDD_DIALOG1), 0, SplashWndProc, 0);
	GetWindowRect(_hwnd, &rect);
	SetWindowPos(_hwnd, 0, (rect.right + rect.left) / 2 - 350, (rect.bottom + rect.top) / 2 - 216, 640, 480, SWP_SHOWWINDOW);
	Paint(_hwnd);
}

void u_splash_window::hide()
{
	DestroyWindow(_hwnd);
}

void u_splash_window::Paint(HWND hWnd)
{
	if (_hBitmap)
	{
		tagPAINTSTRUCT ps;
		HDC v4 = BeginPaint(hWnd, &ps);

		tagBITMAP bitmapData;
		GetObjectA(_hBitmap, 32, &bitmapData);

		tagRECT rect;
		GetClientRect(hWnd, &rect);

		HDC hdcSrc = CreateCompatibleDC(v4);
		HGDIOBJ v6 = SelectObject(hdcSrc, _hBitmap);
		BitBlt(v4, 0, 0, rect.right, rect.bottom, hdcSrc, 0, 0, 0xCC0020u);
		SelectObject(hdcSrc, v6);
		DeleteDC(hdcSrc);
		EndPaint(hWnd, &ps);
	}
}