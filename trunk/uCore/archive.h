#pragma once
#include "gtl/intrusive_ptr.h"

class archive_def 
{
public:
	enum array_item_type
	{
		ait_custom = 0,
		ait_file,
		ait_groups,
		ait_entity,
		ait_cover,
		ait_vs,
		ait_sound,
		ait_sound_effect_reverb,
		ait_sound_effect_echo,
		ait_music_scheme,
		ait_music_theme,
		ait_texture,
		ait_shader,
		ait_color_anim,
		ait_particles_system,
		ait_particles_effect,
		ait_particles_modifier,
		ait_camera_track,
		ait_model,
		ait_skeleton,
		ait_bone,
		ait_anim,
		ait_bone_part,
		ait_attp,
		ait_tpreset,
		ait_material_game,
		ait_material_melee,
		ait_material_step,
		ait_material_pair,
		ait_fx_group,
		ait_fx_anim,
		ait_sound_scheme,
		ait_sound_scheme_item,
		ait_environment,
		ait_use_icon,
		ait_flares,
		ait_string,
		ait_loc_string,
		ait_cc,
		ait_mp_classes,
		ait_ammo_types,
		ait_font,
		ait_total,
		ait_invalid = u32(-1),
	};
	u32 save_chunk_no(u32 save_chunk_mask);
	u32 save_chunk_no_from_flag(u64 fl);
};

class u_archive;
class archive;

typedef intrusive_ptr<archive> ARCHIVE_PTR;

class archive : public intrusive_base
{
public:
	virtual void destroy_me() {}
	virtual void dtr_archive() = 0;
	virtual bool reading() = 0;
	virtual bool writing() = 0;
	virtual bool is_empty() = 0;
	virtual bool is_text() = 0;
	virtual bool enum_sections(u_vector<str_shared>* result, bool sections_only, bool lowercase) = 0;
	virtual bool enum_sections_2(u_vector<u_string>* result, bool sections_only, bool lowercase) = 0;
	virtual void delete_section(const char* key) = 0;
	virtual void skip_section(u64 fl, const char* key) = 0;
	virtual bool save_as(const char* file_name) = 0;
	virtual void reset_for_reading(archive_stable* stable) = 0;
	virtual u16 r_version(u16 version) = 0;
	virtual bool r_bool(u64 fl, const char* key) = 0;
	virtual void r_bool8(u64 fl, const char* key, const char* k0, bool* v0, const char* k1, bool* v1, const char* k2, bool* v2, const char* k3, bool* v3, const char* k4, bool* v4, const char* k5, bool* v5, const char* k6, bool* v6, const char* k7, bool* v7) = 0;
	virtual u8 r_u8(u64 fl, const char* key) = 0;
	virtual u16 r_u16(u64 fl, const char* key) = 0;
	virtual u32 r_u32(u64 fl, const char* key) = 0;
	virtual s32 r_s32(u64 fl, const char* key) = 0	;
	virtual Fvector2 r_vec2f(u64 fl, const char* key) = 0;
	virtual Fvector3 r_vec3f(Fvector3* result, u64 fl, const char* key) = 0;
	virtual Fvector4 r_vec4f(Fvector4* result, u64 fl, const char* key) = 0;
	virtual Fvector2 r_ang2f(u64 fl, const char* key) = 0;
	virtual Fvector3 r_ang3f(Fvector3 *result, u64 fl, const char* key) = 0;
	virtual Fvector4 r_color(Fvector4 *result, u64 fl, const char* key) = 0;
	virtual u32 r_color_u32(u64 fl, const char* key) = 0;
	virtual Ivector3 r_vec3i(Ivector3 *result, u64 fl, const char* key) = 0;
	virtual Ivector4 r_vec4i(Ivector4 *result, u64 fl, const char* key) = 0;
	virtual Svector4 r_vec4s16(u64 fl, const char* key) = 0;
	virtual str_shared r_str(str_shared* result, u64 fl, const char* key, str_type type) = 0;
	virtual void r_str(u64 fl, const char* key, u_string* result) = 0;
	virtual float r_fp32(u64 fl, const char* key) = 0;
	virtual float r_fp32_q16(u64 fl, const char* key, float min, float max) = 0;
	virtual float r_fp32_q8(u64 fl, const char* key, float min, float max) = 0;
	virtual float r_angle(u64 fl, const char* key) = 0;
	virtual float r_angle16(u64 fl, const char* key) = 0;
	virtual float r_angle8(u64 fl, const char* key) = 0;
	virtual Fvector3 r_dir(Fvector3 result, u64 fl, const char* key) = 0;
	virtual flags8 r_flags(u64 fl, const char* key, const u_quadra* captions) = 0;
	virtual flags16 r_flags16(u64 fl, const char* key, const u_quadra* captions) = 0;
	virtual flags32 r_flags32(u64 fl, const char* key, const u_quadra* captions) = 0;
	virtual Fmatrix r_pose(Fmatrix* result, u64 fl, const char* key) = 0;
	virtual Fmatrix43 r_pose_43T(Fmatrix43* result, u64 fl, const char* key) = 0;
	virtual str_shared r_texture(str_shared* result, u64 fl, const char* key) = 0;
	virtual str_shared r_material(str_shared* result, u64 fl, const char* key) = 0;
	virtual str_shared r_shader(str_shared* result, u64 fl, const char* key) = 0;
	virtual str_shared r_file(str_shared* result, u64 fl, const char* key) = 0;
	virtual u16 r_tex_frame(u64 fl, const char* key) = 0;
	virtual void r_tex_frame(u64 fl, const char* key, u_vector<u16>* arr) = 0;
	virtual void r_tex_pixel(u64 fl, const char* key, u_vector<u32>* arr);
	virtual void r_button(u64 fl, const char* key) = 0;
	virtual void r_guid(u64 fl, const char* key, guid* value) = 0;
	virtual ARCHIVE_PTR r_section_s(ARCHIVE_PTR* result, u64 fl, const char* key) = 0;
	000001A8     gtl::intrusive_ptr<archive_array_base<archive>, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* (__fastcall* r_array_s)(archive* this, gtl::intrusive_ptr<archive_array_base<archive>, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* result, u64, const char*, const char*, bool);
	000001B0     gtl::intrusive_ptr<archive_array_base<archive>, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* (__fastcall* r_array_i_s)(archive* this, gtl::intrusive_ptr<archive_array_base<archive>, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* result, u64, const char*, bool);
	000001B8     void(__fastcall * r_u8_array)(archive * this, u64, const char*, u8*, u32*);
	000001C0     void(__fastcall * r_u8_array_2)(archive * this, u64, const char*, u_vector32<unsigned char, allocator_t, u32> *);
	000001C8     void(__fastcall * r_u16_array)(archive * this, u64, const char*, u_vector<u16, allocator_t, u16> *);
	000001D0     void(__fastcall * r_u32_array)(archive * this, u64, const char*, u32*, u32*);
	000001D8     void(__fastcall * r_u32_array_2)(archive * this, u64, const char*, u_vector<u32, allocator_t, u16> *);
	000001E0     void(__fastcall * r_s32_array)(archive * this, u64, const char*, u_vector<int, allocator_t, u16> *);
	000001E8     void(__fastcall * r_fp32_array)(archive * this, u64, const char*, u_vector<float, allocator_t, u16> *);
	virtual u_identifier* r_identifier(u_identifier * result, u64 fl, const char* key, str_shared::str_type type);
	virtual str_shared r_choose(str_shared* result, u64 fl, const char* key, archive_def::array_item_type type) = 0;
	virtual u32 r_choose_array(u64 fl, const char* key, u_vector<str_shared>* res, archive_def::array_item_type type) = 0;
	virtual u32 r_choose_array(u64 fl, const char* key, u_string* res, archive_def::array_item_type type) = 0;
	virtual void reset_for_writing(archive_stable*) = 0;
	00000218     void(__fastcall* w_version)(archive* this, u16);
	00000220     void(__fastcall * w_bool)(archive * this, u64, const char*, const char*, bool, const char*, const char*);
	00000228     void(__fastcall* w_bool8)(archive* this, u64, const char*, const char*, u64, const char*, const char*, bool, u64, const char*, const char*, bool, u64, const char*, const char*, bool, u64, const char*, const char*, bool, u64, const char*, const char*, bool, u64, const char*, const char*, bool, u64, const char*, const char*, bool, u64, const char*, const char*, bool);
	00000230     void(__fastcall * w_bool8_2)(archive * this, u64, const char*, const char*, const char*, const char*, bool, const char*, const char*, bool, const char*, const char*, bool, const char*, const char*, bool, const char*, const char*, bool, const char*, const char*, bool, const char*, const char*, bool, const char*, const char*, bool);
	00000238     void(__fastcall* w_u8)(archive* this, u64, const char*, const char*, const u_token*, u8);
	00000240     void(__fastcall * w_u8_2)(archive * this, u64, const char*, const char*, u8, u8, u8, u8);
	00000248     void(__fastcall* w_u16)(archive* this, u64, const char*, const char*, const u_token*, u16);
	00000250     void(__fastcall * w_u16_2)(archive * this, u64, const char*, const char*, u16, u16, u16, u16);
	00000258     void(__fastcall* w_u32)(archive* this, u64, const char*, const char*, const u_token*, u32);
	00000260     void(__fastcall * w_u32_2)(archive * this, u64, const char*, const char*, u32, u32, u32, u32);
	00000268     void(__fastcall* w_s32)(archive* this, u64, const char*, const char*, const u_token*, int);
	00000270     void(__fastcall * w_s32_2)(archive * this, u64, const char*, const char*, int, int, int, int);
	00000278     void(__fastcall* w_vec2f)(archive* this, u64, const char*, const char*, const _vec2<float>*, const _vec2<float>*, const _vec2<float>*, float, int);
	00000280     void(__fastcall* w_vec3f)(archive* this, u64, const char*, const char*, const _vec3<float>*, const _vec3<float>*, const _vec3<float>*, float, int);
	00000288     void(__fastcall* w_vec4f)(archive* this, u64, const char*, const char*, const _vec4<float>*, const _vec4<float>*, const _vec4<float>*, float, int);
	00000290     void(__fastcall* w_ang2f)(archive* this, u64, const char*, const char*, const _vec2<float>*, const _vec2<float>*, const _vec2<float>*, float, int);
	00000298     void(__fastcall* w_ang3f)(archive* this, u64, const char*, const char*, const _vec3<float>*, const _vec3<float>*, const _vec3<float>*, float, int);
	000002A0     void(__fastcall * w_color)(archive * this, u64, const char*, const char*, const _vec4<float> *);
	000002A8     void(__fastcall * w_color_u32)(archive * this, u64, const char*, const char*, u32);
	000002B0     void(__fastcall * w_vec3i)(archive * this, u64, const char*, const char*, const _vec3<int> *, const _vec3<int> *, const _vec3<int> *, int);
	000002B8     void(__fastcall * w_vec4i)(archive * this, u64, const char*, const char*, const _vec4<int> *, const _vec4<int> *, const _vec4<int> *, int);
	000002C0     void(__fastcall * w_vec4s16)(archive * this, u64, const char*, const char*, const _vec4<short> *, const _vec4<short> *, const _vec4<short> *, __int16);
	000002C8     void(__fastcall * w_str)(archive * this, u64, const char*, const char*, const str_shared*, const u_token*, int);
	000002D0     void(__fastcall * w_str_2)(archive * this, u64, const char*, const char*, const u_string<32> *, const u_token*, int);
	000002D8     void(__fastcall * w_str_3)(archive * this, u64, const char*, const char*, const char*, const u_token*, int);
	000002E0     void(__fastcall * w_str_4)(archive * this, u64, const char*, const char*, const str_shared*, const char*, int);
	000002E8     void(__fastcall * w_str_5)(archive * this, u64, const char*, const char*, const u_string<32> *, const char*, int);
	000002F0     void(__fastcall * w_str_6)(archive * this, u64, const char*, const char*, const char*, const char*, int);
	000002F8     void(__fastcall * w_fp32)(archive * this, u64, const char*, const char*, float, float, float, float, int);
	00000300     void(__fastcall * w_fp32_q16)(archive * this, u64, const char*, const char*, float, float, float, float, int);
	00000308     void(__fastcall* w_fp32_q8)(archive* this, u64, const char*, const char*, float, float, float, float, int);
	00000310     void(__fastcall * w_angle)(archive * this, u64, const char*, const char*, float, float, float, float, int);
	00000318     void(__fastcall* w_angle16)(archive* this, u64, const char*, const char*, float, float, int);
	00000320     void(__fastcall * w_angle8)(archive * this, u64, const char*, const char*, float, float, int);
	00000328     void(__fastcall* w_dir)(archive* this, u64, const char*, const char*, const _vec3<float>*);
	00000330     void(__fastcall * w_flags8)(archive * this, u64, const char*, const u_quadra*, const _flags<unsigned char> *);
	00000338     void(__fastcall* w_flags16)(archive* this, u64, const char*, const u_quadra*, const _flags<u16>*);
	00000340     void(__fastcall * w_flags32)(archive * this, u64, const char*, const u_quadra*, const _flags<u32> *);
	00000348     void(__fastcall* w_pose)(archive* this, u64, const char*, const char*, const _matrix*, float, float);
	00000350     void(__fastcall * w_pose_43T)(archive * this, u64, const char*, const char*, const matrix_43T*, float, float);
	00000358     void(__fastcall* w_texture)(archive* this, u64, const char*, const char*, const str_shared*);
	00000360     void(__fastcall * w_material)(archive * this, u64, const char*, const char*, const u8, const str_shared*);
	00000368     void(__fastcall* w_shader)(archive* this, u64, const char*, const char*, const str_shared*);
	00000370     void(__fastcall * w_file)(archive * this, u64, const char*, const char*, const str_shared*, const char*, const char*, u32, u32);
	00000378     void(__fastcall* w_tex_frame)(archive* this, u64, const char*, const char*, const u16, const char*, u16, u16);
	00000380     void(__fastcall* w_tex_frame_2)(archive* this, u64, const char*, const char*, const u_vector<u16, allocator_t, u16>*, const char*, u16, u16, u8);
	00000388     void(__fastcall* w_tex_pixel)(archive* this, u64, const char*, const char*, const u_vector<u32, allocator_t, u16>*, const char*, u16, u16);
	00000390     void(__fastcall* w_button)(archive* this, u64, const char*, const char*, const char*, const fastdelegate::FastDelegate2<u32, u32, void>*);
	00000398     void(__fastcall* w_guid)(archive* this, u64, const char*, const char*, const guid*);
	000003A0     ARCHIVE_PTR * (__fastcall* w_section_s)(archive* this, ARCHIVE_PTR* result, u64, const char*, const char*);
	000003A8     gtl::intrusive_ptr<archive_array_base<archive>, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* (__fastcall* w_array_s)(archive* this, gtl::intrusive_ptr<archive_array_base<archive>, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* result, u64, const char*, const char*, u32, const char*, const char*, const fastdelegate::FastDelegate2<u32, u32, void>*, const fastdelegate::FastDelegate2<u32, u32, void>*);
	000003B0     gtl::intrusive_ptr<archive_array_base<archive>, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* (__fastcall* w_array_i_s)(archive* this, gtl::intrusive_ptr<archive_array_base<archive>, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* result, u64, u32, const char*, const char*, const fastdelegate::FastDelegate2<u32, u32, void>*, const fastdelegate::FastDelegate2<u32, u32, void>*);
	000003B8     void(__fastcall * w_u8_array)(archive * this, u64, const char*, const char*, const u8*, const u32, u8, u8, u8);
	000003C0     void(__fastcall * w_u8_array_2)(archive * this, u64, const char*, const char*, const u_vector32<unsigned char, allocator_t, u32> *, u8, u8, u8);
	000003C8     void(__fastcall * w_u16_array)(archive * this, u64, const char*, const char*, const u_vector<u16, allocator_t, u16> *, u16, u16, u16);
	000003D0     void(__fastcall * w_u32_array)(archive * this, u64, const char*, const char*, const u32*, const u32, u32, u32, u32);
	000003D8     void(__fastcall * w_u32_array_2)(archive * this, u64, const char*, const char*, const u_vector<u32, allocator_t, u16> *, u32, u32, u32);
	000003E0     void(__fastcall * w_s32_array)(archive * this, u64, const char*, const char*, const u_vector<int, allocator_t, u16> *, int, int, int);
	000003E8     void(__fastcall * w_fp32_array)(archive * this, u64, const char*, const char*, const u_vector<float, allocator_t, u16> *, float, float, float);
	000003F0     void(__fastcall * w_identifier)(archive * this, u64, const char*, const char*, const u_identifier*, const u_token*, int);
	000003F8     void(__fastcall * w_identifier_2)(archive * this, u64, const char*, const char*, const u_identifier*);
	00000400     void(__fastcall * w_choose)(archive * this, u64, const char*, const char*, const str_shared*, archive_def::array_item_type, const fastdelegate::FastDelegate2<choose::pattern*, u_vector<choose::item, allocator_t, u16> &, void> *, const gtl::intrusive_ptr<choose::fill_data, gtl::intrusive_base, gtl::intrusive_default_functionality, gtl::thread_safe_inc_dec<gtl::intrusive_base> > *);
	00000408     void(__fastcall* w_choose_array)(archive* this, u64, const char*, const char*, const u_vector16<str_shared, allocator_t, u16>*, u32, u32, archive_def::array_item_type, const fastdelegate::FastDelegate2<choose::pattern*, u_vector<choose::item, allocator_t, u16>&, void>*, const gtl::intrusive_ptr<choose::fill_data, gtl::intrusive_base, gtl::intrusive_default_functionality, gtl::thread_safe_inc_dec<gtl::intrusive_base> >*);
	00000410     void(__fastcall * w_choose_array_2)(archive * this, u64, const char*, const char*, const u_string<32> *, u32, u32, archive_def::array_item_type, const fastdelegate::FastDelegate2<choose::pattern*, u_vector<choose::item, allocator_t, u16> &, void> *, const gtl::intrusive_ptr<choose::fill_data, gtl::intrusive_base, gtl::intrusive_default_functionality, gtl::thread_safe_inc_dec<gtl::intrusive_base> > *);
	virtual const ARCHIVE_PTR default_archive_s() = 0;
	virtual void default_archive_s(const ARCHIVE_PTR arch) = 0;
	virtual const char* archive_id() { return "unknown"; }
	virtual u_archive* dcast_u_archive() { return nullptr; }
};

#include "archive_def.h"
#include "archive_impl.h"