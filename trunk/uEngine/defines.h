#pragma once

enum e_edit_mode : int
{
    em_game = 0,
    em_level_editor,
    em_model_editor,
    em_other
};

ENGINE_API bool g_editor = false;
