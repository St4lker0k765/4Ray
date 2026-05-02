#include "input_manager_xbox.h"

class UINPUT_API cinput_manager_pc : public cinput_manager_xbox
{
private:
	typedef cinput_manager_xbox inherited;
protected:
	LPDIRECTINPUT8A _di;
	LPDIRECTINPUTDEVICE8A _keyboard;
	LPDIRECTINPUTDEVICE8A _mouse;
	HKL _eng_keyboard;
	svector<int, 3> _offs;
	bool _resend_all_keyboard;
	bool _resend_all_mouse;
public:
	cinput_manager_pc();
	~cinput_manager_pc();

	virtual void _initialize(uinput::configuration* config);
	virtual const char* get_input_locale();
	virtual void get_key_name(u32 id, bool current_locale, wchar_t* buffer);
	virtual u32 get_shift_state();
	virtual void on_app_activate();
	virtual void on_app_deactivate();
	virtual void resend_key_press();
	virtual void set_exclusive_mode(bool val);
	virtual bool update(bool* any_input);

	void clear_offs();
	HRESULT create_input_device(LPDIRECTINPUTDEVICE8A* device, GUID guid_device, const DIDATAFORMAT* pdid_data_format, u32 flags, u32 buf_size);
	void set_acquire(bool acquire);
	void update_keyboard();
	void update_mouse();
};