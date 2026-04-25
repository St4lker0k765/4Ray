#pragma once
// token type definition
struct UCORE_API u_token
{
	LPCSTR	name;
	int 	id;
};

#include <array>
template <class T, const int shift>
struct circular_buffer
{
	u32 readptr;
	u32 writeptr;
	u32 counter;
    std::array<T, 1 << shift> buffer;

	bool empty() { return counter == 0; }
    __int64 read(T* e)
    {
        __int128 v10; // xmm0

        if (!counter)
            return 0;

        char* v8 = &buffer->data.storage[216 * readptr];
        if (((e | v8) & 0xF) != 0)
        {
            memcpy(e, v8, sizeof(T));
        }
        else
        {
            u64 v9 = 1;
            do
            {
                v10 = *(_OWORD*)v8;
                e = (T)((char*)e + 128);
                v8 += 128;
                *(_OWORD*)&e[-1].addr[8] = v10;
                *(_OWORD*)&e[-1].addr[10] = *((_OWORD*)v8 - 7);
                *(_OWORD*)e[-1].funcs = *((_OWORD*)v8 - 6);
                *(_OWORD*)&e[-1].funcs[2] = *((_OWORD*)v8 - 5);
                *(_OWORD*)&e[-1].funcs[4] = *((_OWORD*)v8 - 4);
                *(_OWORD*)&e[-1].funcs[6] = *((_OWORD*)v8 - 3);
                *(_OWORD*)&e[-1].funcs[8] = *((_OWORD*)v8 - 2);
                *(_OWORD*)&e[-1].funcs[10] = *((_OWORD*)v8 - 1);
                --v9;
            } while (v9);
            *(_OWORD*)&e->ptr = *(_OWORD*)v8;
            *(_OWORD*)&e->mopid = *((_OWORD*)v8 + 1);
            *(_OWORD*)&e->addr[1] = *((_OWORD*)v8 + 2);
            *(_OWORD*)&e->addr[3] = *((_OWORD*)v8 + 3);
            *(_OWORD*)&e->addr[5] = *((_OWORD*)v8 + 4);
            e->addr[7] = *((_QWORD*)v8 + 10);
        }
        readptr = (readptr + 1) & 0x1FF;
        --counter;
        return 1;
    }
};