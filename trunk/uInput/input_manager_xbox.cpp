#include "stdafx.h"
#include "input_manager_xbox.h"

void cinput_manager_xbox::resend_key_press()
{
	inherited::resend_key_press();
	_resend_all_controls = true;
}

bool cinput_manager_xbox::update(bool* any_input)
{
	bool result = inherited::update(any_input);
	if (result)
	{
		update_xbox360ctrl();
		return true;
	}
	return result;
}

void cinput_manager_xbox::deactivate_vibration()
{
	inherited::deactivate_vibration();
	if (_active_gamepad != -1)
	{
		_XINPUT_VIBRATION vibration;
		XInputSetState(_active_gamepad, &vibration);
	}
}

void cinput_manager_xbox::gamepad_connected(bool v)
{
	if (v != _gamepad_connected)
	{
		_gamepad_connected = v;
		_gamepad_mode = v;
	}
}

void cinput_manager_xbox::update_xbox360ctrl()
{
    u8 i = 0;
    for (; i < 4; ++i)
    {
        _XINPUT_STATE state;
        if (XInputGetState(i, &state))
        {
            _gamepad_connected = true;
            _active_gamepad = i;
        }
    }
    if (_active_gamepad != -1)
    {
        _XINPUT_STATE state;
        state.dwPacketNumber = 0;
        state.Gamepad.sThumbLX = 0;
    LABEL_12:

        gamepad_connected(!XInputGetState(_active_gamepad, &state));
        if (_gamepad_connected)
        {
            if (_option_vibration)
            {
                stat_motor_left = _vibration_koef;
                stat_motor_right = _vibration_koef;
            }
            else if (_feedback_enabled)
            {
                if (_camera_v0 <= _game_v0)
                    _camera_v0 = _game_v0;
                stat_motor_left = _vibration_koef * _camera_v0;

                if (_camera_v1 <= _game_v1)
                    _camera_v1 = _game_v1;
                stat_motor_right = _vibration_koef * _camera_v1;
            }
            else
            {
                stat_motor_left = 0.0f;
            }

            _XINPUT_VIBRATION vibration;
            vibration.wLeftMotorSpeed = stat_motor_left * 65535.0;
            vibration.wRightMotorSpeed = stat_motor_right * 65535.0;
            if (_active_gamepad != -1)
                XInputSetState(_active_gamepad, &vibration);

            if (state.Gamepad.sThumbLX
                || state.Gamepad.sThumbLY
                || state.Gamepad.sThumbRX
                || state.Gamepad.sThumbRY
                || state.Gamepad.bLeftTrigger
                || state.Gamepad.bRightTrigger)
            {
                float LX = update_xbox360joy(state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                float LY = update_xbox360joy(state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                float RX = update_xbox360joy(state.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
                float RY = update_xbox360joy(state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
                if (swap_left_right_thumbs())
                {
                    float LX_temp = LX;
                    float LY_temp = LY;
                    LX = RX;
                    LY = RY;
                    RX = LX_temp;
                    RY = LY_temp;
                }
                if (LX + LY + RX + RY
                    + update_xbox360trig(state.Gamepad.bLeftTrigger)
                    + update_xbox360trig(state.Gamepad.bRightTrigger) 
                    > 0.0000001)
                {
                    _gamepad_mode = true;
                    activate_vibration();
                    _last_modif_time = _cur_time;
                }
                receiver()->r_on_capture();
            }
            u8 wButtons = state.Gamepad.wButtons;
            process_pad(X_DPAD_UP, wButtons & XINPUT_GAMEPAD_DPAD_UP, &_resend_all_controls);
            process_pad(X_DPAD_DOWN, wButtons & XINPUT_GAMEPAD_DPAD_DOWN, &_resend_all_controls);
            process_pad(X_DPAD_LEFT, wButtons & XINPUT_GAMEPAD_DPAD_LEFT, &_resend_all_controls);
            process_pad(X_DPAD_RIGHT, wButtons & XINPUT_GAMEPAD_DPAD_RIGHT, &_resend_all_controls);
            process_pad(X_START, wButtons & XINPUT_GAMEPAD_START, &_resend_all_controls);
            process_pad(X_BACK, wButtons & XINPUT_GAMEPAD_BACK, &_resend_all_controls);
            u32 thumb_key;
            if (_swap_left_right_thumbs)
            {
                process_pad(X_RIGHT_THUMB, wButtons & XINPUT_GAMEPAD_LEFT_THUMB, &_resend_all_controls);
                thumb_key = X_LEFT_THUMB;
            }
            else
            {
                process_pad(X_LEFT_THUMB, wButtons & XINPUT_GAMEPAD_LEFT_THUMB, &_resend_all_controls);
                thumb_key = X_RIGHT_THUMB;
            }
            process_pad(thumb_key, wButtons & XINPUT_GAMEPAD_RIGHT_THUMB, &_resend_all_controls);

            int triggerActive;
            u32 trigger_key;
            if (_swap_trigger_shoulder)
            {
                process_pad(X_RIGHT_SHOULDER, state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD, &_resend_all_controls);
                process_pad(X_LEFT_SHOULDER, state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD, &_resend_all_controls);
                process_pad(X_RIGHT_TRIGGER, wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER, &_resend_all_controls);
                trigger_key = X_LEFT_TRIGGER;
                triggerActive = wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
            }
            else
            {
                process_pad(X_LEFT_SHOULDER, wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER, &_resend_all_controls);
                process_pad(X_RIGHT_SHOULDER, wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER, &_resend_all_controls);
                process_pad(X_LEFT_TRIGGER, state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD, &_resend_all_controls);
                trigger_key = X_RIGHT_TRIGGER;
                triggerActive = state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
            }
            process_pad(trigger_key, triggerActive, &_resend_all_controls);

            process_pad(X_A, wButtons & XINPUT_GAMEPAD_A, &_resend_all_controls);
            process_pad(X_B, wButtons & XINPUT_GAMEPAD_B, &_resend_all_controls);
            process_pad(X_X, wButtons & XINPUT_GAMEPAD_X, &_resend_all_controls);
            process_pad(X_Y, wButtons & XINPUT_GAMEPAD_Y, &_resend_all_controls);
        }
        else
        {
            _active_gamepad = -1;
        }
        _resend_all_controls = false;
        return;
    }
}

float update_xbox360joy(int value, int deadzone)
{
    float res = 0.0;
    if (value > deadzone)
        res = (value - deadzone) / (32767 - deadzone);

    if (value < -deadzone)
        res = (value + deadzone) / (32767 - deadzone);

    return fmaxf(fminf(res * 1.010101, 1.0), -1.0);
}

float update_xbox360trig(u8 value)
{
    return value/255;
}