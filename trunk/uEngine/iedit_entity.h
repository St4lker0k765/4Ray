#pragma once

class edit_entity;

class iedit_entity
{
	guid* guid;
	guid* guid_2;
	guid* guid_3;
	virtual iedit_entity*  dcast_iedit_entity() = 0;
	virtual edit_entity* dcast_edit_entity() = 0;
	virtual struct editable* dcast_editable() = 0;
	virtual ieditable* dcast_ieditable() = 0;
	virtual void dtr_iedit_entity() = 0;
	virtual uobject* owner(uobject* result) = 0;
	virtual void selected(bool) = 0;
	virtual bool selected_2() = 0;
	virtual void uniform_scale(bool) = 0;
	virtual bool uniform_scale_2() = 0;
	virtual bool hidden_editor() = 0;
	virtual bool hidden_pattern() = 0;
	virtual bool draw_fixed_bb() = 0;
	virtual float draw_bb_scale() = 0;
	virtual void hide_entity(bool) = 0;
	virtual void skip_check_invisible(bool) = 0;
	virtual _aabb3* get_aabb_local(_aabb3* result) = 0;
	virtual void unload_dynamic() = 0;
	virtual void e_load_dynamic(u_archive*) = 0;
	virtual void e_save_dynamic(u_archive*) = 0;
	virtual void e_load_after_vs(u_archive*) = 0;
	virtual void e_save_after_vs(u_archive*, const char*) = 0;
	virtual void clear_guid() = 0;
	virtual void attached_params(u16, Fmatrix43) = 0;
	virtual void on_parent_changed() = 0;
	virtual void on_spatial_move() = 0;
	virtual void on_modified() = 0;
};