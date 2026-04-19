#pragma once
// token type definition
struct UCORE_API u_token
{
	LPCSTR	name;
	int 	id;
};

template <class T, const int shift>
struct circular_buffer
{
	u32 readptr;
	u32 writeptr;
	u32 counter;
    std::array<T, 1 << shift> buffer;
};