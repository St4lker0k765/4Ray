#pragma once

class edit_cover;

class iedit_cover 
{
	guid* guid;
	virtual edit_cover* dcast_edit_cover() = 0;
	virtual editable* dcast_editable() = 0;
	virtual void dtr_iedit_cover() = 0;
	virtual const Fmatrix xform() = 0;
	virtual str_shared get_name(str_shared* result) = 0;
	virtual int get_selected() = 0;
	virtual int get_forbidden() = 0;
	virtual void unload_dynamic() = 0;
	virtual void e_load_dynamic(u_archive*, bool, u16);
	virtual void e_save_dynamic(u_archive*);
	virtual void e_mark_for_save() = 0;
	virtual void e_set_owner(const ucover::cover*) = 0;
};