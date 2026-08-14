#pragma once

class music_player
{
protected:
	s64 _switch_start_ns;
public:
	u64 global_fade_start_ns;
	u64 global_fade_end_ns;
	intrusive_ptr<music_theme> theme = nullptr;
	sound_source* source = nullptr;
	intrusive_ptr<sound_reader> wave = nullptr;
	u32 cursor = 0;
	u32 fade_type;
	
	virtual void attach_source();
	virtual void reset();
};