#pragma once
#include "gtl/intrusive_ptr.h"
#include "archive_impl.h"
class archive;
typedef archive_array_base<archive> ArchiveArrBase;
typedef archive_array_impl<archive> ArchiveArrImpl;

typedef intrusive_ptr<archive_array_base<archive>> ArchiveArrBasePtr;
typedef intrusive_ptr<archive_array_impl<archive>> ArchiveArrImplPtr;
#include "archive_def.h"

class u_archive;

typedef intrusive_ptr<archive> ARCHIVE_PTR;

class archive : public intrusive_base
{
public:
	virtual void destroy_me() = 0;
	virtual void dtr_archive() = 0;
	virtual bool reading() = 0;
	virtual bool writing() = 0;
	virtual bool is_empty() = 0;
	virtual bool is_text() = 0;
	virtual bool enum_sections(bool sections_only, bool lowercase) = 0;
	virtual bool enum_sections(bool sections_only, bool lowercase) = 0;
	virtual void delete_section(const char* key) = 0;
	virtual void skip_section(u64 fl, const char* key) = 0;
	virtual bool save_as(const char* file_name) = 0;
	virtual void reset_for_reading(archive_stable* stable) = 0;

	virtual u16 r_version(u16 version) = 0;
	virtual bool r_bool(u64 fl, const char* key) = 0;
	virtual void r_bool8(u64 fl, const char* key, 
		const char* k0, bool* v0, 
		const char* k1, bool* v1, 
		const char* k2, bool* v2, 
		const char* k3, bool* v3, 
		const char* k4, bool* v4, 
		const char* k5, bool* v5, 
		const char* k6, bool* v6,
		const char* k7, bool* v7) = 0;
	virtual u8 r_u8(u64 fl, const char* key) = 0;
	virtual u16 r_u16(u64 fl, const char* key) = 0;
	virtual u32 r_u32(u64 fl, const char* key) = 0;
	virtual s32 r_s32(u64 fl, const char* key) = 0	;
	virtual Fvector2 r_vec2f(u64 fl, const char* key) = 0;
	virtual Fvector3 r_vec3f(u64 fl, const char* key) = 0;
	virtual Fvector4 r_vec4f(u64 fl, const char* key) = 0;
	virtual Fvector2 r_ang2f(u64 fl, const char* key) = 0;
	virtual Fvector3 r_ang3f(u64 fl, const char* key) = 0;
	virtual Fvector4 r_color(u64 fl, const char* key) = 0;
	virtual u32 r_color_u32(u64 fl, const char* key) = 0;
	virtual Ivector3 r_vec3i(u64 fl, const char* key) = 0;
	virtual Ivector4 r_vec4i(u64 fl, const char* key) = 0;
	virtual Svector4 r_vec4s16(u64 fl, const char* key) = 0;
	virtual str_shared r_str(u64 fl, const char* key, str_type type) = 0;
	virtual void r_str(u64 fl, const char* key, u_string* result) = 0;
	virtual float r_fp32(u64 fl, const char* key) = 0;
	virtual float r_fp32_q16(u64 fl, const char* key, float min, float max) = 0;
	virtual float r_fp32_q8(u64 fl, const char* key, float min, float max) = 0;
	virtual float r_angle(u64 fl, const char* key) = 0;
	virtual float r_angle16(u64 fl, const char* key) = 0;
	virtual float r_angle8(u64 fl, const char* key) = 0;
	virtual Fvector3 r_dir(u64 fl, const char* key) = 0;
	virtual flags8 r_flags(u64 fl, const char* key, const u_quadra* captions) = 0;
	virtual flags16 r_flags16(u64 fl, const char* key, const u_quadra* captions) = 0;
	virtual flags32 r_flags32(u64 fl, const char* key, const u_quadra* captions) = 0;
	virtual Fmatrix r_pose(u64 fl, const char* key) = 0;
	virtual Fmatrix43 r_pose_43T(u64 fl, const char* key) = 0;
	virtual str_shared r_texture(u64 fl, const char* key) = 0;
	virtual str_shared r_material(u64 fl, const char* key) = 0;
	virtual str_shared r_shader(u64 fl, const char* key) = 0;
	virtual str_shared r_file(u64 fl, const char* key) = 0;
	virtual u16 r_tex_frame(u64 fl, const char* key) = 0;
	virtual void r_tex_frame(u64 fl, const char* key, u_vector<u16>* arr) = 0;
	virtual void r_tex_pixel(u64 fl, const char* key, u_vector<u32>* arr) = 0;
	virtual void r_button(u64 fl, const char* key) = 0;
	virtual void r_guid(u64 fl, const char* key, guid* value) = 0;
	virtual ARCHIVE_PTR r_section_s(u64 fl, const char* key) = 0;
	virtual intrusive_ptr<archive_array_base<archive>>* r_array_s(u64 fl, const char* key, const char* value_key_format, bool sort_needed) = 0;
	virtual intrusive_ptr<archive_array_base<archive>>* r_array_i_s(u64 fl, const char* value_key_format, bool sort_needed) = 0;
	virtual void r_u8_array(u64 fl, const char* key, u8* arr, u32* arr_size) = 0;
	virtual void r_u8_array(u64 fl, const char* key, u_vector<u8>* arr) = 0;
	virtual void r_u16_array(u64 fl, const char* key, u_vector<u16>* arr) = 0;
	virtual void r_u32_array(u64 fl, const char* key, u32* arr, u32* arr_size) = 0;
	virtual void r_u32_array(u64 fl, const char* key, u_vector<u32>* arr) = 0;
	virtual void r_s32_array(u64 fl, const char* key, u_vector<int>* arr) = 0;
	virtual void r_fp32_array(u64 fl, const char* key, u_vector<float>* arr) = 0;
	virtual u_identifier* r_identifier(u64 fl, const char* key, str_type type) = 0;
	virtual str_shared r_choose(u64 fl, const char* key, archive_def::array_item_type type) = 0;
	virtual u32 r_choose_array(u64 fl, const char* key, u_vector<str_shared>* res, archive_def::array_item_type type) = 0;
	virtual u32 r_choose_array(u64 fl, const char* key, u_string* res, archive_def::array_item_type type) = 0;

	virtual void reset_for_writing(archive_stable*) = 0;

	virtual void w_version(u16 version) = 0;
	virtual void w_bool(u64 fl, const char* key, const char* caption, bool value, const char* caption_false, const char* caption_true) = 0;
	virtual void w_bool8(u64 f, const char* key, const char* caption, 
						u64 f0, const char* k0, const char* c0, bool v0, 
						u64 f1, const char* k1, const char* c1, bool v1, 
						u64 f2, const char* k2, const char* c2, bool v2, 
						u64 f3, const char* k3, const char* c3, bool v3, 
						u64 f4, const char* k4, const char* c4, bool v4, 
						u64 f5, const char* k5, const char* c5, bool v5, 
						u64 f6, const char* k6, const char* c6, bool v6, 
						u64 f7, const char* k7, const char* c7, bool v7) = 0;
	virtual void w_bool8(u64 fl, const char* key, const char* caption, 
						const char* k0, const char* c0, bool v0, 
						const char* k1, const char* c1, bool v1, 
						const char* k2, const char* c2, bool v2, 
						const char* k3, const char* c3, bool v3, 
						const char* k4, const char* c4, bool v4, 
						const char* k5, const char* c5, bool v5, 
						const char* k6, const char* c6, bool v6, 
						const char* k7, const char* c7, bool v7) = 0;
	virtual void w_u8(u64 fl, const char* key, const char* caption, const u_token* values, u8 value) = 0;
	virtual void w_u8(u64 fl, const char* key, const char* caption, u8 value, u8 min, u8 max, u8 inc) = 0;
	virtual void w_u16(u64 fl, const char* key, const char* caption, const u_token* values, u16 value) = 0;
	virtual void w_u16(u64 fl, const char* key, const char* caption, u16 value, u16 min, u16 max, u16 inc) = 0;
	virtual void w_u32(u64 fl, const char* key, const char* caption, const u_token* values, u32 value) = 0;
	virtual void w_u32(u64 fl, const char* key, const char* caption, u32 value, u32 min, u32 max, u32 inc) = 0;
	virtual void w_s32(u64 fl, const char* key, const char* caption, const u_token* values, s32 value) = 0;
	virtual void w_s32(u64 fl, const char* key, const char* caption, s32 value, s32 min, s32 max, s32 inc) = 0;
	virtual void w_vec2f(u64 fl, const char* key, const char* caption, Fvector2 value, Fvector2 min, Fvector2 max, float inc, int decim) = 0;
	virtual void w_vec3f(u64 fl, const char* key, const char* caption, Fvector3 value, Fvector3 min, Fvector3 max, float inc, int decim) = 0;
	virtual void w_vec4f(u64 fl, const char* key, const char* caption, Fvector4 value, Fvector4 min, Fvector4 max, float inc, int decim) = 0;
	virtual void w_ang2f(u64 fl, const char* key, const char* caption, Fvector2 value, Fvector2 min, Fvector2 max, float inc, int decim) = 0;
	virtual void w_ang3f(u64 fl, const char* key, const char* caption, Fvector3 value, Fvector3 min, Fvector3 max, float inc, int decim) = 0;
	virtual void w_color(u64 fl, const char* key, const char* caption, Fvector4 value) = 0;
	virtual void w_color_u32(u64 fl, const char* key, const char* caption, u32 value) = 0;
	virtual void w_vec3i(u64 fl, const char* key, const char* caption, Ivector3 value, Ivector3 min, Ivector3 max, int inc) = 0;
	virtual void w_vec4i(u64 fl, const char* key, const char* caption, Ivector4 value, Ivector4 min, Ivector4 max, int inc) = 0;
	virtual void w_vec4s16(u64 fl, const char* key, const char* caption, const Svector4 value, Svector4 min, Svector4 max, s16 inc) = 0;
	virtual void w_str(u64 fl, const char* key, const char* caption, const str_shared value, const u_token* tokens, int sel_count) = 0;
	virtual void w_str(u64 fl, const char* key, const char* caption, const u_string value, const u_token* tokens, int sel_count) = 0;
	virtual void w_str(u64 fl, const char* key, const char* caption, const char* value, const u_token* tokens, int sel_count) = 0;
	virtual void w_str(u64 fl, const char* key, const char* caption, const str_shared value, const char* list, int sel_count) = 0;
	virtual void w_str(u64 fl, const char* key, const char* caption, const u_string value, const char* list, int sel_count) = 0;
	virtual void w_str(u64 fl, const char* key, const char* caption, const char* value, const char* list, int sel_count) = 0;
	virtual void w_fp32(u64 fl, const char* key, const char* caption, float value, float min, float max, float inc, int decim) = 0;
	virtual void w_fp32_q16(u64 fl, const char* key, const char* caption, float value, float min, float max, float inc, int decim) = 0;
	virtual void w_fp32_q8(u64 fl, const char* key, const char* caption, float value, float min, float max, float inc, int decim) = 0;
	virtual void w_angle(u64 fl, const char* key, const char* caption, float value, float min, float max, float inc, int decim) = 0;
	virtual void w_angle16(u64 fl, const char* key, const char* caption, float value, float inc, int decim) = 0;
	virtual void w_angle8(u64 fl, const char* key, const char*, float, float, int) = 0;
	virtual void w_dir(u64 fl, const char* key, const char* caption, const Fvector3 value) = 0;
	virtual void w_flags8(u64 fl, const char* key, const u_quadra* captions, const flags8 value) = 0;
	virtual void w_flags16(u64 fl, const char* key, const u_quadra* captions, const flags16 value) = 0;
	virtual void w_flags32(u64 fl, const char* key, const u_quadra* captions, const flags32 value) = 0;
	virtual void w_pose(u64 fl, const char* key, const char* caption, const Fmatrix value, float scale_min, float scale_max) = 0;
	virtual void w_pose_43T(u64 fl, const char* key, const char* caption, const Fmatrix43 value, float scale_min, float scale_max) = 0;
	virtual void w_texture(u64 fl, const char* key, const char* caption, const str_shared value) = 0;
	virtual void w_material(u64 fl, const char* key, const char* caption, const u8 type, const str_shared value) = 0;
	virtual void w_shader(u64 fl, const char* key, const char* caption, const str_shared value) = 0;
	virtual void w_file(u64 fl, const char* key, const char* caption, const str_shared value, const char* path, const char* mask, u32 limit, u32 flags) = 0;
	virtual void w_tex_frame(u64 fl, const char* key, const char* caption, const u16 value, const char* tex_name, u16 w, u16 h) = 0;
	virtual void w_tex_frame(u64 fl, const char* key, const char* caption, const u_vector<u16> value, const char* tex_name, u16 w, u16 h, u8 limit) = 0;
	virtual void w_tex_pixel(u64 fl, const char* key, const char* caption, const u_vector<u32> value, const char* tex_name, u16 w, u16 h) = 0;
	virtual void w_button(u64 fl, const char* key, const char* caption, const char* buttons, const fastdelegate::FastDelegate2<u32, u32, void>* cb) = 0;
	virtual void w_guid(u64 fl, const char* key, const char* caption, const guid* value) = 0;
	virtual ARCHIVE_PTR w_section_s(u64, const char* key, const char* caption) = 0;
	virtual ArchiveArrBase w_array_s(u64 fl, const char* key, const char* caption, u32 size, const char* value_key_format, const char*, const fastdelegate::FastDelegate2<u32, u32, void>*, const fastdelegate::FastDelegate2<u32, u32, void>*) = 0;
	virtual ArchiveArrBase w_array_i_s(u64, u32, const char*, const char*, const fastdelegate::FastDelegate2<u32, u32, void>*, const fastdelegate::FastDelegate2<u32, u32, void>*) = 0;
	virtual void w_u8_array(u64 fl, const char* key, const char* caption, const u8* arr, const u32 arr_size, u8 min, u8 max, u8 inc) = 0;
	virtual void w_u8_array(u64 fl, const char* key, const char* caption, const u_vector<u8> arr, u8 min, u8 max, u8 inc) = 0;
	virtual void w_u16_array(u64 fl, const char* key, const char* caption, const u_vector<u16> arr, u16 min, u16 max, u16 inc) = 0;
	virtual void w_u32_array(u64 fl, const char* key, const char* caption, const u32* arr, const u32 arr_size, u32 min, u32 max, u32 inc) = 0;
	virtual void w_u32_array(u64 fl, const char* key, const char* caption, const u_vector<u32> arr, u32 min, u32 max, u32 inc) = 0;
	virtual void w_s32_array(u64 fl, const char* key, const char* caption, const u_vector<s32> arr, s32 min, s32 max, s32 inc) = 0;
	virtual void w_fp32_array(u64 fl, const char* key, const char* caption, const u_vector<float>* arr, float min, float max, float inc) = 0;
	virtual void w_identifier(u64 fl, const char* key, const char* caption, const u_identifier* id, const u_token*, int count) = 0;
	virtual void w_identifier(u64 fl, const char* key, const char* caption, const u_identifier* id) = 0;
	virtual void w_choose(u64 fl, const char* key, const char* caption, const str_shared*, archive_def::array_item_type, const fastdelegate::FastDelegate2<choose::pattern*, u_vector<choose::item>&, void> *, const intrusive_ptr<choose::fill_data> *) = 0;
	virtual void w_choose_array(u64 fl, const char* key, const char* caption, const u_vector<str_shared>, u32, u32, archive_def::array_item_type, const fastdelegate::FastDelegate2<choose::pattern*, u_vector<choose::item>&, void>*, const intrusive_ptr<choose::fill_data>*) = 0;
	virtual void w_choose_array(u64 fl, const char* key, const char* caption, const u_string, u32, u32, archive_def::array_item_type, const fastdelegate::FastDelegate2<choose::pattern*, u_vector<choose::item> &, void> *, const intrusive_ptr<choose::fill_data> *) = 0;
	
	virtual const ARCHIVE_PTR default_archive_s() = 0;
	virtual void default_archive_s(const ARCHIVE_PTR arch) = 0;
	virtual const char* archive_id() = 0;
	virtual u_archive* dcast_u_archive() = 0;
};
