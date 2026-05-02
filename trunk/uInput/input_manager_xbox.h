#include "input_manager.h"
#include <xinput.h>

class UINPUT_API cinput_manager_xbox : public cinput_manager_core
{
private:
	typedef cinput_manager_core inherited;
protected:
	bool _resend_all_controls = false;
public:
	virtual void resend_key_press();
	virtual bool update(bool* any_input);
	virtual void deactivate_vibration();
	void gamepad_connected(bool v);
	void update_xbox360ctrl();
};