#pragma once

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
	struct receiver 
	{

	};
}

// 256 keys for keyboard & mouse
// + 32 keys for gamepad
#define KEYS_COUNT 288
class cinput_manager_core
{
protected:
	uinput::configuration _config;
	u_token* _action_names;
	timing::timer _timer;
	u32 _muted;
	svector<int, KEYS_COUNT> _key_binding;
	svector<bool, KEYS_COUNT> _key_state;

	u_vector<cinput_manager_core::receiver_p> _receivers;
	u64 _active_gamepad;
	bool _allow_option_vibration;
	bool _gamepad_connected;
	bool _gamepad_mode;
	bool _any_user_input;
	bool _swap_left_right_thumbs;
	bool _swap_trigger_shoulder;
	bool _feedback_enabled;
	bool _option_vibration;
	bool _release_all;

	u32 _last_modif_time;
	u32 _cur_time;
	float _camera_v0;
	float _camera_v1;
	float _game_v0;
	float _game_v1;
	float _vibration_koef;
	float stat_motor_left;
	float stat_motor_right;
public:
	void _initialize(uinput::configuration* config);
	void _destroy();
	u32 active_user();
	u64 active_gamepad();
	void reset_active_user();
	uinput::configuration* config();
	void any_user_input(bool v);
	bool any_user_input();
	void capture(uinput::receiver* p, u32 priority);
	void release(uinput::receiver* p);
	int captured(uinput::receiver* p);
	bool get_async_key_state(u32 key);
	int get_shift_state();
	int update(bool* any_input);
	void on_app_activate();
	void on_app_deactivate();

	void set_action_names(const u_token* actions);
	const u_token* get_action_names();
	const u_token* get_key_names();
	void restore_active_state(const unsigned int, int, const int*);
	int action_to_id(const char*);
	int key_to_id(const char*);
	const char* id_to_action(const int);
	const char* id_to_key(const int);
	const char* get_input_locale();
	void get_key_name(const int, bool, wchar_t*);
	const int* get_bindings(unsigned int*);
	u32 get_binding(int, const int);
	void bind_action(int, int);
	void unbind_action(int, bool);
	void unbind_all();
	unsigned int find_action_bind(int, bool);
	void release_all();

	void set_exclusive_mode(bool val);
	void set_vibration_camera(float, float);
	void set_vibration_game(float, float);
	void set_option_vibration(bool);
	void set_vibration_koef(float koef);
	float get_vibration_koef();
	void get_vibration_final(float* L, float* R);
	void activate_vibration();
	void deactivate_vibration();
	void mute(unsigned int);
	void pause(const int);

	bool swap_left_right_thumbs();
	void swap_left_right_thumbs(bool val);
	bool swap_trigger_shoulder();
	void swap_trigger_shoulder(bool val);
	bool swap_circle_cross();

	u8* gamepad_buttons(unsigned int* size);
	void allow_option_vibration(bool val);
	bool allow_option_vibration();
	void resend_key_press();
	int from_gamepad(const int key);
	int e_vk_remap(int key);
	bool gamepad_connected();
	bool gamepad_mode();
	void apply_release_all(bool clear_key_state);
};