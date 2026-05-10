#pragma once

enum e_edit_mode : int
{
    em_game = 0,
    em_level_editor,
    em_model_editor,
    em_other
};

ENGINE_API bool g_editor = false;
ENGINE_API bool g_nopressanykey = false;
ENGINE_API bool g_diff = false;
ENGINE_API bool g_summary = false;
ENGINE_API bool g_disable_mboxes = false;
ENGINE_API bool g_resave = false;
ENGINE_API bool g_trace = false;
