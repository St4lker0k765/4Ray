#pragma once

class timer_queue : public threading::spin_lock
{
protected:
	u_vector<std::pair<u32, serializable_callback>> queue;
public:
};

class ENGINE_API engine_timing
{
protected:

};