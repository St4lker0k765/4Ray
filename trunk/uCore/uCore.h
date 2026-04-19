#include <Windows.h>
#include <windowsx.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include "fastdelegate.h"
#include "_types.h"
#include "threading.h"

#ifdef UCORE_EXPORTS
#define UCORE_API __declspec(dllexport)
#else
#define UCORE_API __declspec(dllimport)
#endif

#include "debug.h"
#include "ext_std.h"
#include "ext_stl.h"
#include "u_string.h"
#include "libmmgr_shared.h"
#include "vfs.h"
#include "log.h"
#include "u_platform.h"
#include "libmmgr.h"
#include "u_random.h"
#include "u_compressor.h"

UCORE_API str_shared g_levelname;

#define BUILD_ID_2033 "2033"
#define BUILD_ID_2034 "2034"
#define BUILD_ID_33_34 "33_34"

class UCORE_API u_core 
{
protected:
	u_string content_root;
	u_string comp_name;
	wstring64 ui_game_nick;
	str_shared _build_key;
	int _complete_edition;
	string4096 params_string;
	bool params_valid;
	bool trace;
	bool human_move_ng_mode;
	bool io_log;
	volatile u32 package_downloading;
	volatile u32 package_downloading_progress;
	volatile u32 level_loading_progress;
	volatile float level_loading_time;
	char dlc_corrupt;
public:
	u_string user_name;
	u_string application_name;
	u_string game_version;

	u_core();
	u_core(const u_core* other);

	void _initialize(LPCSTR application_name, LPCSTR _data_path, bool no_log);
	void _destroy(UINT res);

	bool build_2033();
	bool build_2034();
	bool build_33_34();
	const str_shared build_key();

	u64 complete_edition();
	u64 dlc_downloaded() { return 1; }
	u64 level_downloaded() { return 1; }

	str_shared menu_level(str_shared* result);
	char* params();

	void set_build_key_2033();
	void set_build_key_2034();
};

extern UCORE_API u_core core;