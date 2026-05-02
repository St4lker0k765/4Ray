#include "stdafx.h"
#include "input_manager.h"

void cinput_manager_core::_initialize(uinput::configuration* config)
{
	R_ASSERT(config);
	_config.handle = config->handle;
	_config.init_mouse = config->init_mouse;
	_timer.start();
}

void cinput_manager_core::_destroy()
{
	;
}

bool cinput_manager_core::gamepad_connected()
{
	return _gamepad_connected;
}

u32 cinput_manager_core::active_user()
{
	return _active_gamepad;
}

u64 cinput_manager_core::active_gamepad()
{
	return _active_gamepad;
}

uinput::configuration* cinput_manager_core::config()
{
	return &_config;
}

void cinput_manager_core::reset_active_user()
{
	deactivate_vibration();
	release_all();
	apply_release_all(true);
	_active_gamepad = -1;
}

void cinput_manager_core::apply_release_all(bool clear_key_state)
{
	release(0, _key_state.size(), clear_key_state);
	receiver()->r_on_release();
}

bool cinput_manager_core::gamepad_mode()
{
	return _gamepad_connected && _gamepad_mode;
}

#pragma todo("Fill this array later")
int vk_remap[255] =
{
	0,
};

u32 cinput_manager_core::e_vk_remap(int key)
{
	R_ASSERT(key < 0xFF);
	return vk_remap[key];
}

bool cinput_manager_core::from_gamepad(const int key)
{
	return (key - (COUNT_KEYBOARD+1)) <= COUNT_GAMEPAD_BUTTONS;
}

void cinput_manager_core::gamepad_mode(bool v)
{
	_gamepad_mode = v;
	if (v)
		activate_vibration();
	else
		deactivate_vibration();
}

bool cinput_manager_core::any_user_input()
{
	return _any_user_input;
}

void cinput_manager_core::any_user_input(bool v)
{
	_any_user_input = v;
}

void cinput_manager_core::deactivate_vibration()
{
	_feedback_enabled = false;
}

void cinput_manager_core::activate_vibration()
{
	_feedback_enabled = true;
}

void cinput_manager_core::mute(u32 frames)
{
	_muted = frames;
}

void cinput_manager_core::pause(bool on)
{
	if (on)
		deactivate_vibration();
	else
		activate_vibration();
}

void cinput_manager_core::get_vibration_final(float* L, float* R)
{
	*L = stat_motor_left;
	*R = stat_motor_right;
}

float cinput_manager_core::get_vibration_koef()
{
	return _vibration_koef;
}

void cinput_manager_core::resend_key_press()
{
	;
}

void cinput_manager_core::allow_option_vibration(bool val)
{
	_allow_option_vibration = val;
}

bool cinput_manager_core::allow_option_vibration()
{
	return _allow_option_vibration;
}

u8* cinput_manager_core::gamepad_buttons(u32* size)
{
	*size = 0;
	return nullptr;
}

bool cinput_manager_core::swap_circle_cross()
{
	return false;
}

void cinput_manager_core::swap_left_right_thumbs(bool val)
{
	_swap_left_right_thumbs = val;
}

bool cinput_manager_core::swap_left_right_thumbs()
{
	return _swap_left_right_thumbs;
}

void cinput_manager_core::swap_trigger_shoulder(bool val)
{
	_swap_trigger_shoulder = val;
}

bool cinput_manager_core::swap_trigger_shoulder()
{
	return !_swap_trigger_shoulder;
}

void cinput_manager_core::set_vibration_koef(float koef)
{
	_vibration_koef = koef;
}

void cinput_manager_core::set_option_vibration(bool v)
{
	_option_vibration = v;
}

void cinput_manager_core::set_vibration_game(float v0, float v1)
{
	_game_v0 = v0;
	_game_v1 = v1;
}

void cinput_manager_core::set_exclusive_mode(bool exclusive)
{
	;
}

void cinput_manager_core::set_vibration_camera(float v0, float v1)
{
	_camera_v0 = v0;
	_camera_v1 = v1;
}

void cinput_manager_core::release_all()
{
	_release_all = 1;
}

void cinput_manager_core::on_app_deactivate()
{
	deactivate_vibration();
}

void cinput_manager_core::on_app_activate()
{
	;
}

void cinput_manager_core::unbind_all()
{
	memset(_key_binding.begin(), 0, sizeof(int) * _key_binding.size());
}

bool cinput_manager_core::update(bool* any_input)
{
    if (_muted)
    {
        _muted--;
        deactivate_vibration();
        return false;
    }
    else
    {
        if (_release_all)
        {
            _release_all = false;
            apply_release_all(false);
        }
        if (any_input)
            *any_input = _last_modif_time == _cur_time;

        bool found = false;
        _cur_time = _timer.get_elapsed_ms();
		for (int i = 0; i < COUNT_BUTTONS; i++)
        {
            if (_key_state[i])
            {
				receiver()->r_filter_keys(key_binding(i), i);
				found = true;
            }
        }
        if (found)
            _last_modif_time = _cur_time;
        return true;
    }
	return false;
}

u32 cinput_manager_core::get_shift_state()
{
	return 0;
}

bool cinput_manager_core::get_async_key_state(u32 key)
{
    return _key_state[key];
}

u32 cinput_manager_core::get_binding(int s, const bool gamepad)
{
    u32 max = 256;
    if (gamepad_mode())
		max = 288;

    u32 key = -1;
    for (int i = 0; i < max; i++)
    {
        if (_key_binding[i] == s)
        {
            key = i;
            if (!gamepad || from_gamepad(i))
                break;
        }
    }
    return key;
}

const int* cinput_manager_core::get_bindings(u32* size)
{
	*size = 288;
	return _key_binding.begin();
}

void cinput_manager_core::get_key_name(u32 id, bool current_locale, wchar_t* buffer)
{
	// St4lker0k765: very strange code 'round here
	/*
	__int16* v4; // rax
	s64 v5; // r9
	__int16 v6; // cx

	v4 = (__int16*)&unk_18000E9E8;
	v5 = (char*)buffer - (char*)&unk_18000E9E8;
	do
	{
		v6 = *v4++;
		*(__int16*)((char*)v4 + v5 - 2) = v6;
	} while (v6);
	*/
}

const char* cinput_manager_core::get_input_locale()
{
	return "us";
}

const char* cinput_manager_core::id_to_key(const int id)
{
	for (int i = 0; i < (sizeof(key_names) / sizeof(u_token)); i++)
	{
		if (key_names[i].id == id && key_names[i].name)
		{
			return key_names[i].name;
		}
	}
	return "";
}

u_token* cinput_manager_core::get_key_names()
{
	return key_names;
}

void cinput_manager_core::restore_active_state(const u32 size, u32 state, const int* bindings)
{
    S32Vec actions;

    receiver()->r_on_capture();
    if (_receivers.size())
    {
        for (u32 i = 0; i != COUNT_BUTTONS; ++i)
        {
            if (key_binding(i))
            {
                actions.push_back(key_binding(i));
            }
        }
        if (_action_names)
        {
            for (u32 i = 0; i < size; i++)
            {
                S32It j = actions.begin();
                for (; j != actions.end(); ++j)
                {
                    if (*j == bindings[i])
                        break;
                }
                if (j == actions.end())
                {
                    receiver()->r_on_key_hold(bindings[i], 0, state);
                }
                
            }
        }
        else
        {
            for (u32 i = 0; i < size; i++)
            {
                receiver()->r_on_key_hold(bindings[i], 0, state);
            }
        }
    }
}

u32 cinput_manager_core::key_binding(u32 idx)
{
    return _key_binding[idx];
}

u32 cinput_manager_core::key_to_id(const char* key)
{
    for (int i = 0; i < (sizeof(key_names) / sizeof(u_token)); i++)
    {
        if (_stricmp(key_names[i].name, key))
        {
            return key_names[i].id;
        }
    }
    return 0;
}

void cinput_manager_core::capture(uinput::receiver* p, u32 priority)
{    
    R_ASSERT(p);
    release_all();
    apply_release_all(false);

    u_vector<receiver_p>::iterator it = _receivers.end();
    if (_receivers.size())
    {
        for (; it != _receivers.begin(); it--)
        {
            receiver_p p = (receiver_p)*it;
            if (p.priority <= priority)
                break;
        }
    }
    cinput_manager_core::receiver_p rec;
    rec.receiver = p;
    rec.priority = priority;
    _receivers.insert(it, rec);
    if (receiver() == p)
    {
        change_focus(receiver(), p);
    }
}

void cinput_manager_core::release(uinput::receiver* p)
{
    R_ASSERT(p);
    if (receiver() == p)
    {
        change_focus(p, receiver());
    }
    else
    {
        if (_receivers.size())
        {
            for (u32 i = 0; i < _receivers.size(); i++)
            {
                if (p == _receivers[i])
                {
                    _receivers.erase(_receivers[i]);
                    break;
                }
            }
        }
    }
}

void cinput_manager_core::release(int from, int to, bool clear_key_state)
{
    R_ASSERT(from < to);
    for (int i = 0; from != to; i++)
    {
        if (_key_state[i])
        {
            receiver()->r_on_key_release(key_binding(i), i, _key_state[i], false);
            if (clear_key_state)
            {
                _key_state[i] = false;
            }
        }
    }
}

void cinput_manager_core::change_focus(uinput::receiver* from, uinput::receiver* to)
{
    if (from != to)
    {
        for (int i = 0; i < COUNT_BUTTONS; i++)
        {
            if (_key_state[i])
            {
                from->r_on_key_release(key_binding(i), i, get_shift_state(), false);
            }
        }
        from->r_on_release();
        to->r_on_capture();
    }
}

bool cinput_manager_core::captured(uinput::receiver* pc)
{
    return pc == receiver();
}

uinput::receiver* cinput_manager_core::receiver()
{
    R_ASSERT(_receivers.size() && _receivers.back().receiver);
    return _receivers.back().receiver;
}

void cinput_manager_core::process_pad(const u32 key, int nstate, bool* resending)
{
    if (!_key_state[key] || !resending)
    {
        if (process_key(key, nstate, true, resending))
        {
            _gamepad_mode = true;
            activate_vibration();
        }
    }
}

bool cinput_manager_core::process_key(const u32 key, const int new_state, int skip_duplicates, bool* resending)
{
    if (_key_state[key] != new_state)
        resending = false;

    if (skip_duplicates && _key_state[key] == new_state)
        return false;

    _key_state[key] = new_state;
    if (_key_state[key])
        on_key_release(key, resending);
    else
        on_key_press(key, resending);

    if (_key_state[key] == new_state)
    {
        if (new_state)
            on_key_press(key, resending);
        else
            on_key_release(key, resending);
    }
    return true;
}

void cinput_manager_core::process_kbd(const u32 key, int nstate, bool* resending)
{
    if (!_key_state[key] || !resending)
    {
        if (process_key(key, nstate, true, resending))
        {
            _gamepad_mode = false;
            deactivate_vibration();
        }
    }
}

void cinput_manager_core::on_key_press(u32 key, bool* resending)
{
    if (key != DIK_TAB || get_shift_state())
    {
        receiver()->r_on_key_press(key_binding(key), key, get_shift_state(), resending);
        _last_modif_time = _cur_time;
    }
}

void cinput_manager_core::on_key_release(u32 key, bool* resending)
{
    receiver()->r_on_key_release(key_binding(key), key, get_shift_state(), resending);
    _last_modif_time = _cur_time;
}

UINPUT_API cinput_manager_core* input_manager_impl = nullptr;