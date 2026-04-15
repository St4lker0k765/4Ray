#include "stdafx.h"
#include "input_manager.h"

void cinput_manager_core::_initialize(uinput::configuration* config)
{
	R_ASSERT(config);
	_config.handle = config->handle;
	_config.init_mouse = config->init_mouse;
	timing::timer::start(_timer);
}

bool cinput_manager_core::gamepad_connected()
{
	return _gamepad_connected;
}

u32 cinput_manager_core::active_user()
{
	return _active_gamepad;
}

void __fastcall cinput_manager_core::reset_active_user()
{
	deactivate_vibration();
	release_all();
	apply_release_all(true);
	_active_gamepad = -1;
}

void cinput_manager_core::apply_release_all(bool clear_key_state)
{
	_QWORD* v6; // rax

	int v4 = uvector_base<1, 1, sarray_base<1, 1, 288>>::size(&this->_key_state);
	release(0, v4, clear_key_state);
	R_ASSERT(_receivers.size() && _receivers.back().receiver);
	v6 = uvector_base<16, 8, vector_base<allocator_t<16, 8>, unsigned short>>::back(_receivers);
	(*(void(__fastcall**)(_QWORD))(*(_QWORD*)*v6 + 32LL))(*v6);
}