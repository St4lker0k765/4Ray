#include "stdafx.h"
#include "vfs.h"

vfs::tracer::tracer()
{
    _types[rt_file] = "file";
    _types[rt_track] = "track";
    _types[rt_model] = "model";
    _types[rt_mesh] = "mesh";
    _types[rt_cform] = "cform";
    _types[rt_physx] = "physx_model";
    _types[rt_physx_cloth] = "physx_model_cloth";
    _types[rt_tex] = "tex";
    _types[rt_sound] = "sound";
    _types[rt_sound_pf] = "sound_pf";
    _types[rt_shader] = "shader";
    _types[rt_video] = "video";
    _types[rt_config_bin] = "config_bin";
    _types[rt_fxa] = "facefx";
    _types[rt_stable] = "stable";
    _types[rt_font] = "font";
    _types[rt_cc] = "color_cube";
    _types[rt_motion] = "motion";
    _types[rt_system] = "system";
    _types[rt_modifier] = "modifier";
    _types[rt_effect] = "effect";
    _types[rt_music] = "music";
    _types[rt_loc_str] = "loc_str";
    _types[rt_identifier] = "identifier";
    _types[rt_env] = "env";
    _types[rt_group] = "group";

    for (int k = 0; k < rt_count; k++)
    {
        R_ASSERT(_types[k].size());
    }
}

void vfs::tracer::begin()
{
    if (!strstr(core.params(), "-trace") || strstr(core.params(), "-editor"))
    {
        Stream.F = nullptr;
        if (strstr(core.params(), "-trcdbg"))
            Stream.tr = u_new<u_map<str_shared, u_map<u_string, bool>>>();
        else
            Stream.tr = nullptr;
    }
    else
    {
        _enabled = true;
        char* map = strstr(core.params(), "-map");
        R_ASSERT2(map, "cannot trace without a valid MAP (failed to find -map)");
        string_path Source;
        sscanf(map + 4, "%s", Source);
        u_string file_path; 
        file_path.assign(Source);
        file_path.trim();
        file_path.lwr();
        _map_name = file_path.c_str();

        const char* out = _output_dir.c_str();
        if (!_output_dir.size())
            out = "\\";

        u_string trace_path;
        trace_path.set("%slogs\\trace\\%s.trace", out, _map_name.c_str());
        file_path.assign(trace_path.c_str());
        vfs::path_build_os(file_path.c_str());
        Stream.F = fopen(file_path.c_str(), "wt");
        Stream.marker = "startup";
        Stream.id = "";
        Stream.tr = nullptr;
    }
    if (strstr(core.params(), "-branch_trace"))
    {
        Stream.making_branch = true;
        if (!Stream.tr)
            Stream.tr = u_new<u_map<str_shared, u_map<u_string, bool>>>();
    }
    Stream.trace_allowed = true;
}

void vfs::tracer::end()
{
    if (Stream.F)
    {
        fclose(Stream.F);
        u_string dst; 
        dst.set("%strace\\%s.trace", "logs\\", _map_name.c_str());

        u_string src;
        const char* o_dir = _output_dir.size() ? _output_dir.c_str() : "\\";
        src.set("%slogs\\trace\\%s.trace", o_dir, _map_name.c_str());

        vfs::file_copy_os(src.c_str(), dst.c_str(), true);
    }
    if (Stream.tr)
    {
        delete Stream.tr;
    }
}

void vfs::tracer::camera_track(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_track], name, false, nullptr);
        vfs::copy_resource(rt_track, name);
    }
}

void vfs::tracer::cc(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_cc], name, true, nullptr);
        vfs::copy_resource(rt_cc, name);
    }
}

void vfs::tracer::cform(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_cform], name, false, nullptr);
        vfs::copy_resource(rt_cform, name);
    }
}

void vfs::tracer::config_bin(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_config_bin], name, false, nullptr);
        vfs::copy_resource(rt_config_bin, name);
    }
}

void vfs::tracer::effect(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_effect], name, false, nullptr);
        vfs::copy_resource(rt_effect, name);
    }
}

void vfs::tracer::env(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_env], name, false, nullptr);
        vfs::copy_resource(rt_env, name);
    }
}

void vfs::tracer::file(const char* name)
{
    if (_enabled)
    {
        vfs::log_file(_types[rt_file].c_str(), name);
        vfs::copy_resource(rt_file, name);
    }
}

u_string vfs::tracer::file_name(u_string& result, const char* map)
{
    result.set("%slogs\\trace\\%s.trace", _output_dir.c_str(), map);
    return result;
}

u_string vfs::tracer::file_name2(u_string& result, const char* map)
{
    result.set("%strace\\%s.trace", "logs\\", map);
    return result;
}

void vfs::tracer::font(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_font], name, false, nullptr);
        vfs::copy_resource(rt_font, name);
    }
}

void vfs::tracer::fxa(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_fxa], name, false, nullptr);
        vfs::copy_resource(rt_fxa, name);
    }
}

void vfs::tracer::get_branch_files(u_vector<str_shared>* list)
{
    vfs::commit_copying(list);
}

void vfs::tracer::group(const char* name)
{
    if (_enabled)
        vfs::copy_resource(rt_group, name);
}

void vfs::tracer::identifier(const char* name, str_type stype)
{
    if (_enabled && name && *name)
    {
        R_ASSERT(stype);
        if (Stream.F)
        {
            fprintf(Stream.F, "%s\t%s\t%s\t%d,%s\n", Stream.marker.c_str(), Stream.id.c_str(), _types[rt_identifier].c_str(), stype, name);
        }
    }
}

void vfs::tracer::loc_str(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_loc_str], name, false, nullptr);
        vfs::copy_resource(rt_loc_str, name);
    }
}

void vfs::tracer::mark_editor()
{
    Stream.marker.assign("editor");
    Stream.trace_allowed = true;
}

void vfs::tracer::mark_map(const char* name)
{
    Stream.marker.assign("mapload");
    Stream.id.assign(name);
    Stream.trace_allowed = true;
}

void vfs::tracer::mark_running()
{
    Stream.marker.assign("run");
    Stream.trace_allowed = false;
}

void vfs::tracer::mesh(const char* name)
{
    if (_enabled)
    {
        int cnt = sz_item_count(name, ',');
        if (cnt)
        {
            for (int i = 0; i < cnt; i++)
            {
                string_path buf;
                vfs::log_resource(&_types[rt_mesh], sz_item(name, i, buf, sizeof(buf), ',', "", false), false, false);
                vfs::copy_resource(rt_mesh, buf);
            }
        }
    }
}

void vfs::tracer::model(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_model], name, false, nullptr);
        vfs::copy_resource(rt_model, name);
    }
}

void vfs::tracer::modifier(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_modifier], name, false, nullptr);
        vfs::copy_resource(rt_modifier, name);
    }
}

void vfs::tracer::motion(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_motion], name, false, nullptr);
        vfs::copy_resource(rt_motion, name);
    }
}

void vfs::tracer::motion_src(const char* name)
{
    if (_enabled)
        vfs::copy_resource(rt_motion, name);
}

void vfs::tracer::music(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_music], name, false, nullptr);
        vfs::copy_resource(rt_music, name);
    }
}

void vfs::tracer::output_dir(str_shared dir)
{
    _output_dir = dir;
}

str_shared vfs::tracer::output_dir()
{
    return _output_dir;
}

void vfs::tracer::physx_cloth(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_physx_cloth], name, false, nullptr);
        vfs::copy_resource(rt_physx_cloth, name);
    }
}

void vfs::tracer::physx_model(const char* name)
{
    if (_enabled)
    {
        string_path nm;
        sz_cpy(nm, sizeof(nm), name);
        vfs::log_resource(&_types[rt_physx], strrchr(nm, '.'), false, nullptr);
        vfs::copy_resource(rt_physx, name);
    }
}

void vfs::tracer::shader(const char* name, const char* info)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_shader], name, false, info);
        vfs::copy_resource(rt_shader, name);
    }
}

void vfs::tracer::sound(const char* name, bool pf)
{
    if (_enabled)
    {
        str_shared type = _types[rt_sound];
        if (pf)
            type = _types[rt_sound_pf];

        vfs::log_resource(&type, name, false, nullptr);
        vfs::copy_resource((vfs::resourse_type)(pf + rt_sound), name);
    }
}

void vfs::tracer::stable(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_stable], name, false, nullptr);
        vfs::copy_resource(rt_stable, name);
    }
}

void vfs::tracer::system(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_system], name, false, nullptr);
        vfs::copy_resource(rt_system, name);
    }
}

void vfs::tracer::texture(const char* name)
{
    vfs::reader_base* i; // rcx

    if (_enabled && Stream.trace_allowed)
    {
        string_path dest;
        sz_concat(dest, sizeof(dest), "content\\textures\\", name, ".ani");

        vfs::ireader* result;
        vfs::ropen(result, dest);
        if (result)
        {
            string256 temp; 
            result->r_string(temp, sizeof(temp));
            while (!result->eof())
            {
                result->r_string(temp, sizeof(temp));
                sz_trim(temp);
                if (temp[0])
                {
                    vfs::log_resource(&_types[rt_tex], temp, true, nullptr);
                    vfs::copy_resource(rt_tex, temp);
                }
            }
        }
        if (name && *name != '$')
        {
            vfs::log_resource(&_types[rt_tex], name, true, nullptr);
            vfs::copy_resource(rt_tex, name);
        }
    }
}

u32 vfs::tracer::type_id(str_shared name)
{
    for (u32 i = 0; i < rt_count; i++)
    {
        if (_types[i] == name)
        {
            return i;
        }
    }
    return u32(-1);
}

str_shared vfs::tracer::type_name(u32 id)
{
    R_ASSERT(id < rt_count);
    return _types[id];
}

void vfs::tracer::video(const char* name)
{
    if (_enabled)
    {
        vfs::log_resource(&_types[rt_video], name, false, nullptr);
        vfs::copy_resource(rt_video, name);
    }
}