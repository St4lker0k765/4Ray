#include "stdafx.h"
#include "engine_timing.h"

void engine_timing::apply_slowmo()
{
	calc_slowmo();
	g_sound->logical_freq();
}

u32 engine_timing::async()
{
	return timer_os.get_elapsed_ms();
}

u32 engine_timing::async_delta()
{
	return _async_delta_ms;
}

u32 engine_timing::async_delta_mcs()
{
	return _async_delta_mcs;
}

float engine_timing::async_f()
{
	return timer_os.get_elapsed_sec();
}

u64 engine_timing::async_os()
{
	return timer_os_delta + timer_os.get_elapsed_ms();
}

u64 engine_timing::async_us()
{
	return timer_os.get_elapsed_us();
}

float engine_timing::calc_slowmo()
{
    if (g_editor)
    {
        if (!engine.p_editor || !engine.p_editor->ai_physics())
        {
            float min = ((slowmo_scale_outer * slowmo_scale) * slowmo_scale_debug) * slowmo_scale_changers;
            return fp_lerp(1.0f, min, 1.0f);
        }
    }
    if (engine.mp() && !g_editor)
    {
        return 1.0f;
    }
    float min = ((slowmo_scale_outer * slowmo_scale) * slowmo_scale_debug) * slowmo_scale_changers;
    return fp_lerp(1.0f, min, 1.0f);
}

float engine_timing::delta_f()
{
    return _delta_f;
}

float engine_timing::delta_f_prev()
{
    return _delta_f_prev;
}

u32 engine_timing::delta_ms()
{
    return _delta_ms;
}

u64 engine_timing::delta_us()
{
    return _delta_us;
}

void engine_timing::engine_loaded()
{
    rlog("Engine loaded in %d ms", timeGetTime() - engine_start_time);
}

void engine_timing::engine_started()
{
    engine_start_time = timeGetTime();
}

float engine_timing::fps()
{
    return _fps;
}

u32 engine_timing::global()
{
    return _global_ms;
}

u32 engine_timing::global_async()
{
    return _global_ms_async;
}

double engine_timing::global_d()
{
    return _global_d;
}

u32 engine_timing::global_no_slowmo()
{
    return _global_ms_no_slowmo;
}

u64 engine_timing::global_us()
{
    return _global_us;
}

void engine_timing::initialize()
{
    slowmo_scale = 1.0f;
    slowmo_scale_outer = 1.0f;
    slowmo_scale_debug = 1.0f;
    slowmo_scale_changers = 1.0f;
    _delta_f = 0.0f;
    _global_us = 0;
    _global_ms = 1000000000;
    u64 per_second = platform.qpc.per_second;
    per_second *= 1000000LL;
    accumulator_scaled = per_second;
    accumulator_not_scaled = per_second;
    _global_ms_no_slowmo = 1000000000;
    _global_d = 1000000.0;
    bpaused = false;
    bpaused_deferred = false;
    _fps = 30.0f;

    timer.start();
    timer_os.start();
    timer_os_delta = 0;
    DWORD Time = timeGetTime();
    while (timeGetTime() == Time)
        ;
    DWORD v4 = timeGetTime();
    timer_os_delta = v4 - timer_os.get_elapsed_ms();
}

void engine_timing::load_active(const gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* arch, u16 version)
{
    u_archive* object; // rbx
    u_archive* v6; // rbx
    const char* v8; // [rsp+20h] [rbp-18h]
    int v9; // [rsp+28h] [rbp-10h]
    gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> > r; // [rsp+40h] [rbp+8h] BYREF
    stream_checker<gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> > > __r__; // [rsp+48h] [rbp+10h] BYREF

    arch->_object->r_section(arch->_object, &r, 0, "engine_timing");
    object = r._object;
    __r__._arch._object = r._object;
    if (r._object)
    {
        if (r._object->_ref_count < 0)
            debug::fail(
                (debug*)"0 <= *(s32 *)&(o->base_type::_ref_count)",
                "d:\\trunk\\src\\ucore\\gtl/intrusive_ptr.h",
                "gtl::thread_unsafe_inc_dec<class intrusive_base_pool>::inc_unsafe",
                288);
        ++object->_ref_count;
    }
    timer_queue::load_active(&this->callbacks, &r, version);
    if (object)
    {
        if (!object->debug_eof(object))
        {
            LODWORD(v8) = 567;
            debug::fail(
                (debug*)"!_arch || _arch->debug_eof()",
                "unfinished reading detected",
                "d:\\trunk\\src\\ucore\\archive_def.h",
                "stream_checker<class gtl::intrusive_ptr<class u_archive,class intrusive_base_pool,class gtl::intrusive_default_f"
                "unctionality,struct gtl::thread_unsafe_inc_dec<class intrusive_base_pool> > >::~stream_checker",
                v8,
                v9);
        }
        gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool>>::dec_cold(&__r__._arch);
    }
    v6 = r._object;
    if (r._object)
    {
        if (r._object->_ref_count <= 0)
            debug::fail(
                (debug*)"0 < *(s32 *)&(o->base_type::_ref_count)",
                "d:\\trunk\\src\\ucore\\gtl/intrusive_ptr.h",
                "gtl::thread_unsafe_inc_dec<class intrusive_base_pool>::dec",
                290);
        if (v6->_ref_count-- == 1)
            gtl::thread_unsafe_inc_dec<intrusive_base_pool>::dec_cold<u_archive>(&r, v6);
    }
}

void engine_timing::pause_deferred(bool b)
{
    bpaused_deferred = b;
}

void engine_timing::paused(bool b)
{
    bpaused = b;
}

bool engine_timing::paused()
{
    return bpaused && !bforce_notpaused;
}

void engine_timing::register_slowmo_changer(fastdelegate::FastDelegate<float()> sc)
{
    _slowmo_changers.push_back(sc);
}

void engine_timing::reset_slowmo(bool with_debug)
{
    slowmo_scale = 1.0f;
    slowmo_scale_outer = 1.0f;
    if (with_debug)
    {
        slowmo_scale_debug = 1.0f;
    }
    slowmo_scale_changers = 1.0f;
    calc_slowmo();
    g_sound->logical_freq();
}

void engine_timing::save_active(const gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> >* arch)
{
    gtl::intrusive_ptr<u_archive, intrusive_base_pool, gtl::intrusive_default_functionality, gtl::thread_unsafe_inc_dec<intrusive_base_pool> > w; // [rsp+40h] [rbp+8h] BYREF

    arch->w_section(&w, 0, "engine_timing", 0);
    callbacks.save_active(&w);
}

void engine_timing::slowmo(float f)
{
    slowmo_scale = f;
    calc_slowmo();
    g_sound->logical_freq();
}

float engine_timing::slowmo()
{
    return slowmo_scale;
}

void engine_timing::slowmo_debug(float f)
{
    slowmo_scale_debug = f;
    calc_slowmo();
    g_sound->logical_freq();
}

float engine_timing::slowmo_debug()
{
    return slowmo_scale_debug;
}

void engine_timing::slowmo_debug_decrease()
{
    float slowmo_scale_debug_old = slowmo_scale_debug;
    if (slowmo_scale_debug_old <= 0.5f)
    {
        if (slowmo_scale_debug_old <= 0.25f)
        {
            if (slowmo_scale_debug_old <= 0.12f)
            {
                if (slowmo_scale_debug_old <= 0.06f)
                {
                    if (slowmo_scale_debug_old <= 0.03f)
                        slowmo_scale_debug = 0.01f;
                    else
                        slowmo_scale_debug = 0.03f;
                }
                else
                {
                    slowmo_scale_debug = 0.06f;
                }
            }
            else
            {
                slowmo_scale_debug = 0.12f;
            }
        }
        else
        {
            slowmo_scale_debug = 0.25f;
        }
    }
    else
    {
        slowmo_scale_debug = fmaxf(fminf(slowmo_scale_debug - 0.25f, 1.0f), 0.005f);
    }
    calc_slowmo();
    g_sound->logical_freq();
}

void engine_timing::slowmo_debug_increase()
{
    float slowmo_scale_debug_old = slowmo_scale_debug;
    if (slowmo_scale_debug_old > 0.01f)
    {
        if (slowmo_scale_debug_old > 0.03f)
        {
            if (slowmo_scale_debug_old > 0.06f)
            {
                if (slowmo_scale_debug_old > 0.12f)
                {
                    slowmo_scale_debug = fmaxf(fminf(slowmo_scale_debug_old + 0.25f, 10.0f), 0.25f);
                }
                else
                {
                    slowmo_scale_debug = 0.25f;
                }
            }
            else
            {
                slowmo_scale_debug = 0.12f;
            }
        }
        else
        {
            slowmo_scale_debug = 0.06f;
        }
    }
    else
    {
        slowmo_scale_debug = 0.03f;
    }
    calc_slowmo();
    g_sound->logical_freq();
}

void engine_timing::slowmo_debug_set_max()
{
    slowmo_scale_debug = 10.0f;
    calc_slowmo();
    g_sound->logical_freq();
}

void engine_timing::slowmo_debug_set_min()
{
    slowmo_scale_debug = 0.01f;
    calc_slowmo();
    g_sound->logical_freq();
}

void engine_timing::slowmo_outer(float f)
{
    slowmo_scale_outer = f;
    calc_slowmo();
    g_sound->logical_freq();
}

float engine_timing::slowmo_outer()
{
    return slowmo_scale_outer;
}

void engine_timing::unregister_slowmo_changer(fastdelegate::FastDelegate<float()> sc)
{
    for (u_vector<fastdelegate::FastDelegate<float()>>::iterator it = _slowmo_changers.begin(); it != _slowmo_changers.end(); it++)
    {
        fastdelegate::FastDelegate<float()> f = (*it);
        if (f == sc)
        {
            _slowmo_changers.erase(it);
            break;
        }
    }
}

void engine_timing::update()
{
    float fps; // xmm3_4
    float v6; // xmm2_4
    int v7; // edx
    unsigned int v8; // esi
    unsigned int v9; // ebx
    bool v11; // al
    unsigned __int64 elapsed_ticks; // r15
    __int64 v16; // rax
    signed __int64 v17; // rbx
    __int64 v19; // rax
    __int64 v20; // rdi
    signed __int64 v21; // rax
    float v22; // xmm0_4
    float v23; // xmm0_4
    unsigned __int64 v24; // rax
    double v25; // xmm7_8
    signed __int64 per_second; // rax
    double v27; // xmm1_8
    float v28; // xmm1_4
    float v29; // ecx
    unsigned __int64 v32; // rdi
    double v33; // xmm0_8
    signed __int64 v35; // rdx
    double v36; // xmm1_8
    double v37; // xmm0_8
    signed __int64 v38; // rcx
    double v39; // xmm1_8
    float v40; // xmm0_4
    signed __int64 v41; // rcx
    double v42; // xmm0_8
    double v43; // xmm7_8
    cprofile_sample frame_ms; // [rsp+30h] [rbp-68h] BYREF

    R_ASSERT2(bpaused || cengine::mapstate_quickload != engine.flags_mapstate, "NO PAUSE during QUICKLOAD!");
    _global_ms_async = timer_os.get_elapsed_ms();
    if (bpaused_deferred)
    {
        engine.pause(1);
        bpaused_deferred = false;
    }
    fps = this->_fps;
    u64 dbg_accumulator_scaled = accumulator_scaled;
    v6 = (float)*target_fps.uconsole::cmd_integer::value;
    if (fps > v6)
    {
        v7 = 158 - ((1000.0 / fps) >> 23);
        v8 = (COERCE_INT(1000.0 / fps) >> 31)
            ^ ((v7 - 32) >> 31)
            & (((COERCE_UNSIGNED_INT(1000.0 / fps) | 0xFF800000) << 8 >> v7)
                - ((COERCE_INT(1000.0 / fps) >> 31) & (((((1 << (v7 - 96)) - 1) >> 8) & COERCE_UNSIGNED_INT(1000.0 / fps)) == 0)));
        v9 = (COERCE_INT(1000.0 / v6) >> 31)
            ^ ((158 - (unsigned __int8)(COERCE_INT(1000.0 / v6) >> 23) - 32) >> 31)
            & (((COERCE_UNSIGNED_INT(1000.0 / v6) | 0xFF800000) << 8 >> (-98 - (COERCE_INT(1000.0 / v6) >> 23)))
                - ((COERCE_INT(1000.0 / v6) >> 31)
                    & (((((1 << (-98 - (COERCE_INT(1000.0 / v6) >> 23) - 96)) - 1) >> 8) & COERCE_UNSIGNED_INT(1000.0 / v6)) == 0)));
        R_ASSERT(frame_time <= target_time);
        if (v8 > v9)
            debug::fail((debug*)"frame_time <= target_time", "engine_timing.cpp", "engine_timing::update", 47);
        Sleep(v9 - v8);
    }
    bforce_notpaused = bpaused && COERCE_FLOAT(*(_DWORD*)engine_step.uconsole::cmd_float::value & _xmm) >= 0.0000001;
    float v12 = 0.0;
    if (!bpaused)
        v12 = calc_slowmo();
    elapsed_ticks = timer.get_elapsed_ticks();
    timer.start();
    s32 v14 = 200;
    v16 = 200;
    v17 = (elapsed_ticks
        * ((unsigned int)(COERCE_INT(v12 * 1024.0) >> 31)
            ^ (unsigned __int64)(((158 - (unsigned __int8)(COERCE_INT(v12 * 1024.0) >> 23) - 32) >> 31)
                & (((COERCE_UNSIGNED_INT(v12 * 1024.0) | 0xFF800000) << 8 >> (-98
                    - (COERCE_INT(v12 * 1024.0) >> 23)))
                    - ((COERCE_INT(v12 * 1024.0) >> 31)
                        & (((((1 << (-98 - (COERCE_INT(v12 * 1024.0) >> 23) - 96)) - 1) >> 8)
                            & COERCE_UNSIGNED_INT(v12 * 1024.0)) == 0)))))) >> 10;
    if (g_benchmark)
        v16 = 5000;
    u64 v18 = platform.qpc.per_milisec * v16;
    if (v17 > v18)
        v17 = v18;
    this->_async_delta_ms = elapsed_ticks / platform.qpc.per_milisec;
    this->_async_delta_mcs = elapsed_ticks / platform.qpc.per_microsec;
    cprofile_sample::clear(&frame_ms);
    frame_ms.name = "! *frame*";
    frame_ms.time = elapsed_ticks;
    cprofiler::add_profile_portion(&engine.profiler, (size_align<1, 32, 8> *) & frame_ms);
    if ((_S4_1 & 1) == 0)
    {
        _S4_1 |= 1u;
        memset(&flt, 0, 0x60u);
        flt.samples_valid = 0;
    }
    accum_r += this->_async_delta_mcs;
    v19 = time_filter::filter(&flt, accum_r, accum_s, 0x63u, 0xAu);
    v20 = v19 - accum_s;
    accum_s = v19;
    cprofile_sample::clear(&frame_ms);
    frame_ms.name = "! *frame*-s";
    frame_ms.time = v20 * platform.qpc.per_microsec;
    cprofiler::add_profile_portion(&engine.profiler, (size_align<1, 32, 8> *) & frame_ms);
    if (bpaused)
    {
        if (COERCE_FLOAT(*(_DWORD*)engine_step.uconsole::cmd_float::value & _xmm) >= 0.0000001)
        {
            v21 = platform.qpc.per_milisec;
            v22 = (float)(int)v21;
            if (v21 < 0)
                v22 = v22 + 1.8446744e19;
            v23 = v22 * *engine_step.uconsole::cmd_float::value;
            v24 = 0;
            if (v23 >= 9.223372e18)
            {
                v23 = v23 - 9.223372e18;
                if (v23 < 9.223372e18)
                    v24 = 0x8000000000000000uLL;
            }
            elapsed_ticks = v24 + (unsigned int)(int)v23;
        }
        else
        {
            elapsed_ticks = 0;
        }
        v17 = elapsed_ticks;
        engine_step.value = 0.0;
    }
    v25 = (double)(int)elapsed_ticks;
    if ((elapsed_ticks & 0x8000000000000000uLL) != 0LL)
        v25 = v25 + 1.844674407370955e19;
    per_second = platform.qpc.per_second;
    v27 = (double)(int)per_second;
    if (per_second < 0)
        v27 = v27 + 1.844674407370955e19;
    v28 = v25 / v27;
    v29 = 1.0 / v28;
    if ((u32(1.0 / v28) & 0x7F800000) != 0x7F800000
        && (((v29 + 0x800000) & 0x7FBFFFFF) != 0 || v29 == NAN))
    {
        _fps = (_fps * 0.99) + (float)((float)(1.0 / v28) * 0.0099999998);
    }
    accumulator_not_scaled += elapsed_ticks;
    this->accumulator_scaled += v17;
    u32 saved_global_ms = this->_global_ms;
    v32 = 1000 * this->accumulator_not_scaled;
    v33 = accumulator_scaled;
    u64 saved_global_us = this->_global_us;
    this->_global_ms = 1000 * accumulator_scaled / platform.qpc.per_second;
    this->_global_ms_no_slowmo = v32 / platform.qpc.per_second;
    if (accumulator_scaled < 0)
        v33 = v33 + 1.844674407370955e19;
    v35 = platform.qpc.per_second;
    v36 = (double)(int)v35;
    if (v35 < 0)
        v36 = v36 + 1.844674407370955e19;
    this->_global_d = v33 / v36;
    this->_global_us = 1000000 * accumulator_scaled / platform.qpc.per_second;
    R_ASSERT(_global_ms >= saved_global_ms);
    R_ASSERT(accumulator_scaled >= dbg_accumulator_scaled);
    _delta_f_prev = _delta_f;
    v37 = (double)(int)v17;
    if (v17 < 0)
        v37 = 0.0;
    v38 = platform.qpc.per_second;
    v39 = platform.qpc.per_second;
    if (platform.qpc.per_second < 0)
        v39 = 0.0;
    this->_delta_ms = _global_ms - saved_global_ms;
    this->_delta_us = _global_us - saved_global_us;
    v40 = v37 / v39;
    this->_delta_f = v40;
    v41 = platform.qpc.per_second;
    v42 = (double)(int)v41;
    if (v41 < 0)
        v42 = v42 + 1.844674407370955e19;
    v43 = v25 / v42;
    if (g_benchmark)
        v14 = 5000;

    float dt = v43;
    if (dt > (v14 * 0.001))
        dt = v14 * 0.001;
    update_slowmo_changers(dt);
}

void engine_timing::update_slowmo_changers(float dt)
{
    float val = 1.0f;
    for (u32 i = 0; i < _slowmo_changers.size(); i++)
    {
        fastdelegate::FastDelegate<float()> f = _slowmo_changers[i];
        val = f();
    }
    slowmo_scale_changers = val;
}