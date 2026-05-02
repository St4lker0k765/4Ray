#pragma once

enum
{
     COUNT_MOUSE_BUTTONS = 3,
     COUNT_MOUSE_AXIS = 3,
     COUNT_GAMEPAD_BUTTONS = 16,
     COUNT_KEYBOARD = 256,
     COUNT_BUTTONS = 288,
};

#define MOUSE_0		0xF0
#define MOUSE_1		MOUSE_0 + 1
#define MOUSE_2		MOUSE_0 + 2

#define MOUSE_3		MOUSE_0 + 3
#define MOUSE_4		MOUSE_0 + 4
#define MOUSE_5		MOUSE_0 + 5
#define MOUSE_6		MOUSE_0 + 6
#define MOUSE_7		MOUSE_0 + 7

#define KINECT_GO_BACK 0xFA
#define KINECT_PAUSE KINECT_GO_BACK + 1
#define KINECT_PLAY KINECT_GO_BACK + 2

#define X_DPAD_UP COUNT_KEYBOARD + 1
#define X_DPAD_DOWN COUNT_KEYBOARD + 2
#define X_DPAD_LEFT COUNT_KEYBOARD + 3
#define X_DPAD_RIGHT COUNT_KEYBOARD + 4

#define X_START COUNT_KEYBOARD + 5
#define X_BACK COUNT_KEYBOARD + 6

#define X_LEFT_THUMB COUNT_KEYBOARD + 7
#define X_RIGHT_THUMB COUNT_KEYBOARD + 8
#define X_LEFT_SHOULDER COUNT_KEYBOARD + 9
#define X_RIGHT_SHOULDER COUNT_KEYBOARD + 10

#define X_A COUNT_KEYBOARD + 11
#define X_B COUNT_KEYBOARD + 12
#define X_X COUNT_KEYBOARD + 13
#define X_Y COUNT_KEYBOARD + 14

#define X_LEFT_TRIGGER COUNT_KEYBOARD + 15
#define X_RIGHT_TRIGGER COUNT_KEYBOARD + 16

#include <dinput.h>
u_token key_names[] =
{
	{ "kESCAPE",			DIK_ESCAPE		},	{ "k1",					DIK_1			},
	{ "k2",					DIK_2			},	{ "k3",					DIK_3			},
	{ "k4",					DIK_4			},	{ "k5",					DIK_5			},
	{ "k6",					DIK_6			},	{ "k7",					DIK_7			},
	{ "k8",					DIK_8			},	{ "k9",					DIK_9			},
	{ "k0",					DIK_0			},	{ "kMINUS",				DIK_MINUS		},
	{ "kEQUALS",			DIK_EQUALS		},	{ "kBACK",				DIK_BACK		},
	{ "kTAB",				DIK_TAB 		},	{ "kQ",					DIK_Q			},
	{ "kW",					DIK_W			},	{ "kE",					DIK_E			},
	{ "kR",					DIK_R			},	{ "kT",					DIK_T			},
	{ "kY",					DIK_Y			},	{ "kU",					DIK_U			},
	{ "kI",					DIK_I			},	{ "kO",					DIK_O			},
	{ "kP",					DIK_P			},	{ "kLBRACKET",			DIK_LBRACKET	},
	{ "kRBRACKET",			DIK_RBRACKET	},	{ "kRETURN",			DIK_RETURN		},
	{ "kLCONTROL",			DIK_LCONTROL	},	{ "kA",					DIK_A			},
	{ "kS",					DIK_S			},	{ "kD",					DIK_D			},
	{ "kF",					DIK_F			},	{ "kG",					DIK_G			},
	{ "kH",					DIK_H			},	{ "kJ",					DIK_J			},
	{ "kK",					DIK_K			},	{ "kL",					DIK_L			},
	{ "kSEMICOLON",			DIK_SEMICOLON	},	{ "kAPOSTROPHE",		DIK_APOSTROPHE	},
	{ "kGRAVE",				DIK_GRAVE		},	{ "kLSHIFT",	 		DIK_LSHIFT		},
	{ "kBACKSLASH",			DIK_BACKSLASH	},	{ "kZ",					DIK_Z			},
	{ "kX",					DIK_X			},	{ "kC",					DIK_C			},
	{ "kV",					DIK_V			},	{ "kB",					DIK_B			},
	{ "kN",					DIK_N			},	{ "kM",					DIK_M			},
	{ "kCOMMA",				DIK_COMMA		},	{ "kPERIOD",			DIK_PERIOD		},
	{ "kSLASH",				DIK_SLASH		},	{ "kRSHIFT",			DIK_RSHIFT		},
	{ "kMULTIPLY",			DIK_MULTIPLY	},	{ "kLMENU",				DIK_LMENU		},
	{ "kSPACE",				DIK_SPACE		},	{ "kCAPITAL",			DIK_CAPITAL		},
	{ "kF1",				DIK_F1			},	{ "kF2",				DIK_F2			},
	{ "kF3",				DIK_F3			},	{ "kF4",				DIK_F4			},
	{ "kF5",				DIK_F5			},	{ "kF6",				DIK_F6			},
	{ "kF7",				DIK_F7			},	{ "kF8",				DIK_F8			},
	{ "kF9",				DIK_F9			},	{ "kF10",				DIK_F10			},
	{ "kNUMLOCK",			DIK_NUMLOCK		},	{ "kSCROLL",			DIK_SCROLL		},
	{ "kNUMPAD7",			DIK_NUMPAD7		},	{ "kNUMPAD8",			DIK_NUMPAD8		},
	{ "kNUMPAD9",			DIK_NUMPAD9		},	{ "kSUBTRACT",			DIK_SUBTRACT	},
	{ "kNUMPAD4",			DIK_NUMPAD4		},	{ "kNUMPAD5",			DIK_NUMPAD5		},
	{ "kNUMPAD6",			DIK_NUMPAD6		},	{ "kADD",				DIK_ADD			},
	{ "kNUMPAD1",			DIK_NUMPAD1		},	{ "kNUMPAD2",			DIK_NUMPAD2		},
	{ "kNUMPAD3",			DIK_NUMPAD3		},	{ "kNUMPAD0",			DIK_NUMPAD0		},
	{ "kDECIMAL",			DIK_DECIMAL		},	{ "kF11",				DIK_F11			},
	{ "kF12",				DIK_F12			},	{ "kF13",				DIK_F13			},
	{ "kF14",				DIK_F14			},	{ "kF15",				DIK_F15			},
	{ "kKANA",				DIK_KANA		},	{ "kCONVERT",			DIK_CONVERT		},
	{ "kNOCONVERT",			DIK_NOCONVERT	},	{ "kYEN",				DIK_YEN			},
	{ "kNUMPADEQUALS",		DIK_NUMPADEQUALS},	{ "kCIRCUMFLEX",		DIK_CIRCUMFLEX	},
	{ "kAT",				DIK_AT			},	{ "kCOLON",				DIK_COLON		},
	{ "kUNDERLINE",			DIK_UNDERLINE	},	{ "kKANJI",				DIK_KANJI		},
	{ "kSTOP",				DIK_STOP		},	{ "kAX",				DIK_AX			},
	{ "kUNLABELED",			DIK_UNLABELED	},	{ "kNUMPADENTER",		DIK_NUMPADENTER	},
	{ "kRCONTROL",			DIK_RCONTROL	},	{ "kNUMPADCOMMA",		DIK_NUMPADCOMMA	},
	{ "kDIVIDE",			DIK_DIVIDE		},	{ "kSYSRQ",				DIK_SYSRQ		},
	{ "kRMENU",				DIK_RMENU		},	{ "kHOME",				DIK_HOME		},
	{ "kUP",				DIK_UP			},	{ "kPRIOR",				DIK_PRIOR		},
	{ "kLEFT",				DIK_LEFT		},	{ "kRIGHT",				DIK_RIGHT		},
	{ "kEND",				DIK_END			},	{ "kDOWN",				DIK_DOWN		},
	{ "kNEXT",				DIK_NEXT		},	{ "kINSERT",			DIK_INSERT		},
	{ "kDELETE",			DIK_DELETE		},	{ "kLWIN",				DIK_LWIN		},
	{ "kRWIN",				DIK_RWIN		},	{ "kAPPS",				DIK_APPS		},
	{ "kPAUSE",				DIK_PAUSE		},	{ "mouse0",				MOUSE_0			},
	{ "mouse1",				MOUSE_1			},	{ "mouse2",				MOUSE_2			},
	{ "mouse3",				MOUSE_3			},	{ "mouse4",				MOUSE_4			},
	{ "mouse5",				MOUSE_5			},	{ "mouse6",				MOUSE_6			},
	{ "mouse7",				MOUSE_7			},	{ "kinect_go_back",		KINECT_GO_BACK	},
	{ "kinect_pause",		KINECT_PAUSE	},	{ "kinect_play", 		KINECT_PLAY		},
	{ "x_dpad_up",			X_DPAD_UP		},	{ "x_dpad_down", 		X_DPAD_DOWN		},
	{ "x_dpad_left",		X_DPAD_LEFT		},	{ "x_dpad_right", 		X_DPAD_RIGHT	},
	{ "x_start",			X_START			},	{ "x_back", 			X_BACK			},
	{ "x_left_thumb",		X_LEFT_THUMB	},	{ "x_right_thumb",		X_RIGHT_THUMB	},
	{ "x_left_shoulder",	X_LEFT_SHOULDER	},	{ "x_right_shoulder", 	X_RIGHT_SHOULDER},
	{ "x_a",				X_A				},	{ "x_b", 				X_B				},
	{ "x_x",				X_X				},	{ "x_y", 				X_Y				},
	{ "x_left_trigger",		X_LEFT_TRIGGER	},	{ "x_right_trigger", 	X_RIGHT_TRIGGER	},
	{ NULL, 				0				}
};

class UINPUT_API cinput_manager_core : public uinput::manager
{
	struct receiver_p
	{
		uinput::receiver* receiver;
		u32 priority;
	};
protected:
	uinput::configuration _config;
	u_token* _action_names;
	timing::timer _timer;
	u32 _muted;
	svector<int, COUNT_BUTTONS> _key_binding;
	svector<bool, COUNT_BUTTONS> _key_state;

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
	virtual void _initialize(uinput::configuration* config);
	void _destroy();
	u32 active_user();
	u64 active_gamepad();
	void reset_active_user();
	uinput::configuration* config();
	void any_user_input(bool v);
	bool any_user_input();
	void capture(uinput::receiver* p, u32 priority);
	void release(uinput::receiver* p);
	void release(int from, int to, bool clear_key_state);
	bool captured(uinput::receiver* p);
	bool get_async_key_state(u32 key);
	virtual u32 get_shift_state();
	virtual bool update(bool* any_input);
	virtual void on_app_activate();
	virtual void on_app_deactivate();

	void set_action_names(u_token* actions);
	u_token* get_action_names();
	u_token* get_key_names();
	void restore_active_state(const u32, u32, const int*);
	u32 action_to_id(const char*);
	u32 key_to_id(const char*);
	u32 key_binding(u32 idx);
	const char* id_to_action(const int);
	const char* id_to_key(const int);
	virtual const char* get_input_locale();
	virtual void get_key_name(u32, bool, wchar_t*);
	const int* get_bindings(u32*);
	u32 get_binding(int s, const bool gamepad);
	void bind_action(u32 key, int action);
	void unbind_action(int, bool);
	void unbind_all();
	u32 find_action_bind(int, bool);
	void release_all();

	virtual void set_exclusive_mode(bool val);
	void set_vibration_camera(float, float);
	void set_vibration_game(float, float);
	void set_option_vibration(bool v);
	void set_vibration_koef(float koef);
	float get_vibration_koef();
	void get_vibration_final(float* L, float* R);
	void activate_vibration();
	virtual void deactivate_vibration();
	void mute(u32 frames);
	void pause(bool on);

	bool swap_left_right_thumbs();
	void swap_left_right_thumbs(bool val);
	bool swap_trigger_shoulder();
	void swap_trigger_shoulder(bool val);
	bool swap_circle_cross();

	u8* gamepad_buttons(u32* size);
	void allow_option_vibration(bool val);
	bool allow_option_vibration();
	virtual void resend_key_press();
	bool from_gamepad(const int key);
	u32 e_vk_remap(int key);
	virtual bool gamepad_connected();
	bool gamepad_mode();
	void gamepad_mode(bool v);
	void apply_release_all(bool clear_key_state);
	void change_focus(uinput::receiver* from, uinput::receiver* to);
	uinput::receiver* receiver();
	void process_pad(const u32 key, int nstate, bool* resending);
	void process_kbd(const u32 key, int nstate, bool* resending);
	bool process_key(const u32 key, const int new_state, int skip_duplicates, bool* resending);
	void on_key_press(u32 key, bool* resending);
	void on_key_release(u32 key, bool* resending);
};

extern UINPUT_API cinput_manager_core* input_manager_impl;