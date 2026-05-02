#include "stdafx.h"
#include "input_manager.h"

void cinput_manager_core::unbind_action(int action, bool ignore_gamepad)
{
    u32 max = COUNT_BUTTONS;
    if (ignore_gamepad)
        max = COUNT_KEYBOARD;

    for (int i = 0; i < max; i++)
    {
        if (_key_binding[i] == action)
        {
            _key_binding[i] = 0;
        }
    }
}

void cinput_manager_core::set_action_names(u_token* actions)
{
    _action_names = actions;
}

u_token* cinput_manager_core::get_action_names()
{
    return _action_names;
}

u32 cinput_manager_core::find_action_bind(int action, bool ignore_gamepad)
{
    u32 max = COUNT_BUTTONS;
    if (ignore_gamepad)
        max = COUNT_KEYBOARD;

    for (int i = 0; i < max; i++)
    {
        if (_key_binding[i] == action)
        {
            return _key_binding[i];
        }
    }
    return u32(-1);
}

void cinput_manager_core::bind_action(u32 key, int action)
{
    _key_binding[key] = action;
}

const char* cinput_manager_core::id_to_action(const int id)
{
    for (int i = 0; i < (sizeof(_action_names) / sizeof(u_token)); i++)
    {
        if (_action_names[i].id == id && _action_names[i].name)
        {
            return _action_names[i].name;
        }
    }
    return "";
}

u32 cinput_manager_core::action_to_id(const char* action)
{
    int v5; // edi
    __int64 v6; // rbx
    __int64 v7; // rax
    int v9; // [rsp+20h] [rbp-8h]

    R_ASSERT(_action_names);
    v5 = 0;
    v6 = 0;
    if (!_action_names->name)
        return 0;

    v7 = 0;
    while (_stricmp(_action_names[v7].name, action))
    {
        ++v6;
        ++v5;
        v7 = v6;
        if (!_action_names[v6].name)
            return 0;
    }
    return (unsigned int)this->_action_names[v5].id;
}
