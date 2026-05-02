#include "stdafx.h"
#include "u_input.h"
#include "input_manager_pc.h"

cinput_manager_core* uinput::create(uinput::configuration* config)
{
    R_ASSERT(NULL == input_manager_impl);
    input_manager_impl = new cinput_manager_pc();
    input_manager_impl->_initialize(config);
    return input_manager_impl;
}

void uinput::destroy()
{
    input_manager_impl->_destroy();
    delete input_manager_impl;
}