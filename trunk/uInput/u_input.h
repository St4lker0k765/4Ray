#pragma once

class cinput_manager_core;

namespace uinput
{
	struct configuration
	{
		HWND* handle;
		bool init_mouse;
		bool init_keyboard;
		bool init_xbox;
		bool exclusive;
	};
	struct manager
	{
		manager() {}
	};
	struct receiver
	{
		virtual void r_on_key_press(int action, int key, int state, bool resending) {}
		virtual void r_on_key_release(int action, int key, int state, bool resending) {}
		virtual void r_on_key_hold(int action, int key, int state) {}
		virtual void r_on_mouse_move(float x, float y, int z) {}
		virtual void r_on_pad(float lx, float ly, float rx) {}
		virtual u64 r_filter_keys(int action, int key) { return 0; }
		virtual void r_on_capture() {}
		virtual void r_on_release() {}
		receiver() {}
	};
	cinput_manager_core* create(configuration* config);
	void destroy();
}
