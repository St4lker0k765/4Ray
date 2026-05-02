#include "stdafx.h"
#include "input_manager_pc.h"

cinput_manager_pc::cinput_manager_pc()
{
    _di = nullptr;
    _keyboard = nullptr;
    _mouse = nullptr;
    _resend_all_keyboard = false;
    memset(_offs.begin(), 0, sizeof(u32) * _offs.size());
    _eng_keyboard = LoadKeyboardLayoutA("00000409", 0);
    R_ASSERT(_eng_keyboard);
}

cinput_manager_pc::~cinput_manager_pc()
{
    if (_mouse)
    {
        _mouse->Unacquire();
        _mouse->Release();
    }

    if (_keyboard)
    {
        _keyboard->Unacquire();
        _keyboard->Release();
    }

    if (_di)
    {
        _di->Release();
    }
}


#define keyboard_buffer_size 256
#define mouse_buffer_size 256
void cinput_manager_pc::_initialize(uinput::configuration* config)
{
    inherited::_initialize(config);
    R_ASSERT(!_di && !_keyboard && !_mouse);
    R_CHK(DirectInput8Create(GetModuleHandleA(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8A, (PVOID *)&_di, NULL));

    if (_config.init_keyboard)
    {
        R_CHK(create_input_device(&_keyboard, GUID_SysKeyboard, &c_dfDIKeyboard, (_config.exclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE) | DISCL_FOREGROUND | DISCL_NOWINKEY, keyboard_buffer_size));
    }
    if (_config.init_mouse)
    {
        R_CHK(create_input_device(&_mouse, GUID_SysMouse, &c_dfDIMouse2, (_config.exclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE) | DISCL_FOREGROUND, mouse_buffer_size));
    }
}

void cinput_manager_pc::clear_offs()
{
    memset(_offs.begin(), 0, sizeof(u32) * _offs.size());
}

const char* cinput_manager_pc::get_input_locale()
{
    u16 KeyboardLayout = (u16)GetKeyboardLayout(0);
    if (KeyboardLayout > 0x807u)
    {
        if (KeyboardLayout > 0x180Cu)
        {
            if ((KeyboardLayout - 7178) > 0x3400 || ((KeyboardLayout - 7178) & 0x3FF) != 0)
                return inherited::get_input_locale();
            return "es";
        }
        if (KeyboardLayout == 6156)
            return "fr";
        if (KeyboardLayout > 0x1007u)
        {
            if (KeyboardLayout > 0x140Au)
            {
                if (KeyboardLayout == 5132)
                    return "fr";
                if (KeyboardLayout != 6154)
                    return "us";
            }
            else if (KeyboardLayout != 5130 && KeyboardLayout != 4106)
            {
                if (KeyboardLayout == 4108)
                    return "fr";
                if (KeyboardLayout == 5127)
                    return "de";
                return inherited::get_input_locale();
            }
        }
        else
        {
            if (KeyboardLayout == 4103)
                return "de";
            if (KeyboardLayout <= 0x813u)
            {
                if (KeyboardLayout == 2067)
                    return "nl";
                if (KeyboardLayout != 2058)
                {
                    if (KeyboardLayout != 2060)
                    {
                        if (KeyboardLayout == 2064)
                            return "it";
                        return inherited::get_input_locale();
                    }
                    return "fr";
                }
                return "es";
            }
            if (KeyboardLayout == 3079)
                return "de";
            if (KeyboardLayout != 3082)
            {
                if (KeyboardLayout == 3084)
                    return "fr";
                return inherited::get_input_locale();
            }
        }
        return "es";
    }
    if (KeyboardLayout == 2055)
        return "de";
    switch (KeyboardLayout)
    {
    case 0x405u:
        return "cz";
        break;
    case 0x407u:
        return "de";
    case 0x40Au:
        return "es";
    case 0x40Cu:
        return "fr";
    case 0x410u:
        return "it";
    case 0x411u:
        return "jp";
        break;
    case 0x412u:
        return "kr";
        break;
    case 0x413u:
        return "nl";
    case 0x415u:
        return "pl";
        break;
    case 0x419u:
        return "ru";
        break;
    }
    return inherited::get_input_locale();
}

HRESULT cinput_manager_pc::create_input_device( LPDIRECTINPUTDEVICE8A* device, GUID guid_device, const DIDATAFORMAT* pdid_data_format, u32 flags, u32 buf_size)
{
    // Obtain an interface to the input device
    CHK_DX(_di->CreateDevice(guid_device, device, nullptr));

    // Set the device data format. Note: a data format specifies which
	// controls on a device we are interested in, and how they should be
	// reported.
    CHK_DX((*device)->SetDataFormat(pdid_data_format));

   	// Set the cooperativity level to let DirectInput know how this device
	// should interact with the system and with other DirectInput applications.
    HRESULT _hr = (*device)->SetCooperativeLevel(*_config.handle, flags);
    if (FAILED(_hr) && (_hr == E_NOTIMPL)) rlog("! P INPUT: Can't set coop level. Emulation???");
    else R_CHK(_hr);

	// setup the buffer size for the keyboard data
	DIPROPDWORD				dipdw;
	dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj		= 0;
	dipdw.diph.dwHow		= DIPH_DEVICE;
	dipdw.dwData			= buf_size;

    CHK_DX((*device)->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph));

    return S_OK;
}

void cinput_manager_pc::set_acquire(bool acquire)
{
    if (_mouse)
    {
        if (acquire)
            _mouse->Acquire();
        else
            _mouse->Unacquire();
    }
    if (_keyboard)
    {
        if (acquire)
            _keyboard->Acquire();
        else
            _keyboard->Unacquire();
    }
}

void cinput_manager_pc::set_exclusive_mode(bool exclusive)
{
    if (_mouse)
        _mouse->Unacquire();

    if (_keyboard)
        _keyboard->Unacquire();

    if (_keyboard)
    {
        HRESULT _hr = _keyboard->SetCooperativeLevel(*_config.handle, (DISCL_NONEXCLUSIVE - exclusive) | DISCL_FOREGROUND | DISCL_NOWINKEY);
        if (FAILED(_hr) && (_hr == E_NOTIMPL)) rlog("! P INPUT: Can't set coop level. Emulation???");
        else R_CHK(_hr);
    }

    if (_mouse)
    {
        HRESULT _hr = _mouse->SetCooperativeLevel(*_config.handle, (DISCL_NONEXCLUSIVE - exclusive) | DISCL_FOREGROUND);
        if (FAILED(_hr) && (_hr == E_NOTIMPL)) rlog("! P INPUT: Can't set coop level. Emulation???");
        else R_CHK(_hr);
    }
    
    if (_mouse)
        _mouse->Acquire();

    if (_keyboard)
        _keyboard->Acquire();
}

void cinput_manager_pc::resend_key_press()
{
    inherited::resend_key_press();
    _resend_all_keyboard = true;
}

void cinput_manager_pc::on_app_activate()
{
    if (_mouse)
        _mouse->Acquire();

    if (_keyboard)
        _keyboard->Acquire();

    memset(_key_state.begin(), 0, sizeof(bool) * _key_state.size());
    memset(_offs.begin(), 0, sizeof(u32) * _offs.size());
}

void cinput_manager_pc::on_app_deactivate()
{
    if (_mouse)
        _mouse->Unacquire();

    if (_keyboard)
        _keyboard->Unacquire();

    deactivate_vibration();
    memset(_key_state.begin(), 0, sizeof(bool) * _key_state.size());
    memset(_offs.begin(), 0, sizeof(u32) * _offs.size());
}

void cinput_manager_pc::get_key_name(u32 id, bool current_locale, wchar_t* buffer)
{
    DIPROPSTRING keyname;
    keyname.diph.dwSize = sizeof(DIPROPSTRING);
    keyname.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    keyname.diph.dwObj = id;
    keyname.diph.dwHow = DIPH_DEVICE;
    HKL KeyboardLayout = GetKeyboardLayout(0);
    if (current_locale)
        KeyboardLayout = ActivateKeyboardLayout(_eng_keyboard, 0);

    _keyboard->GetProperty(DIPROP_KEYNAME, &keyname.diph);

    swprintf_s(buffer, sizeof(buffer), keyname.wsz);

    if (!current_locale)
    {
        if (KeyboardLayout)
            ActivateKeyboardLayout(KeyboardLayout, 0);
    }
}

bool cinput_manager_pc::update(bool* any_input)
{
    bool result = inherited::update(any_input);
    if (result)
    {
        if (_mouse)
            update_mouse();
        if (_keyboard)
            update_keyboard();
        return true;
    }
    return result;
}

u32 cinput_manager_pc::get_shift_state()
{
    if (!_config.exclusive)
    {
        u32 res = GetAsyncKeyState(VK_SHIFT) < 0; // Left Shift
        if (GetAsyncKeyState(VK_MENU) < 0) // Left Alt
            res |= 2u;
        if (GetAsyncKeyState(VK_CONTROL) < 0) // Left Ctrl
            res |= 4u;

        if (GetAsyncKeyState(VK_LBUTTON) < 0) // Left Mouse Button
            res |= 0x10u;
        if (GetAsyncKeyState(VK_RBUTTON) < 0) // Right Mouse Button
            res |= 0x20u;
        if (GetAsyncKeyState(VK_MBUTTON) < 0) // Middle Mouse Button
            res |= 0x40u;

        return res;
    }


    // Ignoring Shift/Alt/Ctrl presses in fullscreen, I guess...
    u32 res = 1;
    res |= 2u;
    res |= 4u;

    if (_key_state[MOUSE_0])
        res |= 0x10u;
    if (_key_state[MOUSE_1])
        res |= 0x20u;
    if (_key_state[MOUSE_2])
        res |= 0x40u;

    return res;
}

void cinput_manager_pc::update_keyboard()
{
    HRESULT						hr;
    DWORD dwElements = keyboard_buffer_size;
    DIDEVICEOBJECTDATA			od[keyboard_buffer_size];

    VERIFY(_keyboard);

    hr = _keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &od[0], &dwElements, 0);
    if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) 
    {
        hr = _keyboard->Acquire();
        if (hr != S_OK) 
            return;

        hr = _keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &od[0], &dwElements, 0);
        if (hr != S_OK) 
            return;
    }

    for (u32 i = 0; i < dwElements; i++)
    {
        process_kbd(od[i].dwOfs, od[i].dwData & 0x80, &_resend_all_mouse);
    }
    for (u32 i = 0; i < 0xF0; i++)
    {
        if (_key_state[i])
        {
            receiver()->r_on_key_hold(key_binding(i), i, get_shift_state());
            _camera_v0 = false;
            _game_v0 = false;
            _gamepad_mode = false;
            deactivate_vibration();
        }
    }
    _resend_all_keyboard = false;

}
void cinput_manager_pc::update_mouse()
{
    HRESULT hr;
    DWORD dwElements = mouse_buffer_size;
    DIDEVICEOBJECTDATA	od[mouse_buffer_size];

    VERIFY(_mouse);

    hr = _mouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &od[0], &dwElements, 0);
    if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) 
    {
        hr = _mouse->Acquire();
        if (hr != S_OK) 
            return;

        hr = _mouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &od[0], &dwElements, 0);
        if (hr != S_OK) 
            return;
    };
    BOOL				mouse_prev[COUNT_MOUSE_BUTTONS];

    mouse_prev[0] = _key_state[MOUSE_0];
    mouse_prev[1] = _key_state[MOUSE_1];
    mouse_prev[2] = _key_state[MOUSE_2];

    _offs[0] = _offs[1] = _offs[2] = 0;
    for (u32 i = 0; i < dwElements; i++)
    {
        switch (od[i].dwOfs) 
        {
        case DIMOFS_X:	
            _offs[0] += od[i].dwData; 
        case DIMOFS_Y:	
            _offs[1] += od[i].dwData; 
        case DIMOFS_Z:	
            _offs[2] += od[i].dwData; 
        default:
            process_kbd(od[i].dwOfs, od[i].dwData & 0x80, &_resend_all_mouse);
            break;
        }
    }

    if (_key_state[MOUSE_0] && mouse_prev[0])
    {
        receiver()->r_on_key_hold(key_binding(MOUSE_0), MOUSE_0, get_shift_state());
    }

    if (_key_state[MOUSE_1] && mouse_prev[1])
    {
        receiver()->r_on_key_hold(key_binding(MOUSE_1), MOUSE_1, get_shift_state());
    }

    if (_key_state[MOUSE_2] && mouse_prev[2])
    {
        receiver()->r_on_key_hold(key_binding(MOUSE_2), MOUSE_2, get_shift_state());
    }
    if (dwElements) 
    {
        if (_offs[0] || _offs[1] || _offs[2])
        {
            receiver()->r_on_mouse_move(_offs[0], _offs[1], _offs[2]);
            _camera_v0 = false;
            _game_v0 = false;
            _gamepad_mode = false;
            deactivate_vibration();
        }
    }
}
