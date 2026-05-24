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
    bool read(T* e)
    {
        u64 v10; // xmm0

        if (!counter)
            return false;

        char* v8 = &buffer[readptr];
        if (((e | v8) & 0xF) != 0)
        {
            memcpy(e, v8, sizeof(T));
        }
        else
        {
            memset(e, , sizeof(T));
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
        readptr = (readptr + 1) & (1 << shift);
        --counter;
        return 1;
    }
    bool write_head(
        const fastdelegate::FastDelegate<void __cdecl(void)>* e)
    {
        u_sarray<fastdelegate::FastDelegate<void __cdecl(void)>, 4096>* p_buffer; // rbp
        unsigned int v6; // edi
        threading_rw_check* v7; // rbx
        T** v8; // rdi

        if (this->counter == 4096)
            return false;

        v6 = ((unsigned __int16)this->readptr - 1) & 0xFFF;
        readptr = v6;
        v8 = (T**)((char*)buffer + 16 * v6);
        v8[1] = (fastdelegate::detail::GenericClass*)e->m_Closure.m_pFunction;
        *v8 = e->m_Closure.m_pthis;
        ++counter;
        return true;
    }
};