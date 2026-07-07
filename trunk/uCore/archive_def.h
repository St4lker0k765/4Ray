#pragma once

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

    inline u32 save_chunk_no(u32 save_chunk_mask)
    {
        u32 mask = save_chunk_mask;
        if (!mask)
            return 0;

        u32 res = 0;
        if (!(s16)mask)
        {
            res = 16;
            mask >>= 16;
        }
        if (!(s8)mask)
        {
            res += 8;
            mask >>= 8;
        }
        if ((mask & 0xF) == 0)
        {
            res += 4;
            mask >>= 4;
        }
        if ((mask & 3) == 0)
        {
            res += 2;
            mask >>= 2;
        }
        if ((mask & 1) == 0)
            ++res;

        // wtf is ARCHIVE_CHUNKS?
        //R_ASSERT(res <= ARCHIVE_CHUNKS);
        return res;
    }

    inline u32 save_chunk_no_from_flag(u64 fl)
    {
        return save_chunk_no(fl >> 32 & 0xFFFFFFFF);
    }

    ArchiveArrImplPtr w_array_t(
        archive* self,
        u64 fl,
        const char* key,
        const char* caption,
        u32 size,
        const char* value_key_format,
        const char* value_caption_format,
        const fastdelegate::FastDelegate2<u32, u32, void>* _1,
        const fastdelegate::FastDelegate2<u32, u32, void>* _2)
    {
        ArchiveArrImplPtr result;
        ARCHIVE_PTR array_arch = self->w_section_s(fl, key, caption);
        result = w_array_t(fl, array_arch, size, value_key_format, value_caption_format, _1, _2);
        return result;
    }
    inline ArchiveArrImplPtr w_array_t(
        u64 fl,
        const ARCHIVE_PTR array_arch,
        u32 size,
        const char* value_key_format,
        const char* value_caption_format,
        const fastdelegate::FastDelegate2<u32, u32, void>* _1,
        const fastdelegate::FastDelegate2<u32, u32, void>* _2)
    {
        R_ASSERT(array_arch);
        ArchiveArrImpl* array = u_new<archive_array_impl<archive>>();
        array->construct(array_arch, fl);

        if (!array_arch->writing())
        {
            size = array_arch->r_u32(fl, "count");
        }
        else
        {
            array_arch->w_u32(fl, "count", nullptr, size, 0, u32(-1), 0);
        }
        array->_count = size;
        array->_key_format = value_key_format;
        array->_caption_format = value_caption_format;
        return array;
    }
};