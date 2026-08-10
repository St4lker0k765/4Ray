#pragma once
#include "gtl/intrusive_ptr.h"

#define chunk_data_id 1
#define chunk_stable 2
#define WHASH_SIZE 61

class archive_stable : public intrusive_base
{
protected:
	u32 windex = 0;
	u_map<str_shared, u32> whash[WHASH_SIZE];
	u_vector<str_shared> rtable;
public:
    void check_end()
    {
        R_ASSERT2(rtable.empty(), "call read_end first");
        R_ASSERT2(!windex, "call write_end first");
    }
	u64 crc2crc(const u32 crc)
	{
		return crc | HIWORD(crc);
	}
    const str_shared read(u32 id)
    {
        return rtable[id];
    }
    bool read_begin(vfs::ireader r)
    {
        check_end();
        if (r.length() < 20)
        {
            return false;
        }

        u32 pos = r.tell();
        u32 data_size;
        if (!r.chunk_try_open_at_current_position(chunk_data_id, &data_size)
            || data_size + pos >= r.length() - 12)
        {
            r.seek(pos);
            return false;
        }
        r.advance(data_size);
        u32 stable_size;
        if (r.chunk_try_open_at_current_position(chunk_stable, &stable_size))
        {
            rtable.resize(r.r_u32());
            for (int i = 0; i < rtable.size(); i++)
            {
                r.r_stringz(rtable[i], default_string);
            }
            r.seek(pos);
            R_ASSERT(r.chunk_try_open_at_current_position(chunk_data_id, &data_size));
            return true;
        }
        else
        {
            r.seek(pos);
            return false;
        }
    }
    void read_end(vfs::ireader* r)
    {
        rtable.clear();
    }
    size_t write(const str_shared s)
    {
        u32 crc = crc2crc(s._get()->crc) % WHASH_SIZE;
        if (!whash[crc].size())
        {
            windex++;
            whash[crc].emplace(s, windex);
            return windex;
        }
        int i = 0;
        for (; i < whash[crc].size(); i++)
        {
            if (i >= whash[crc].size())
            {
                windex++;
                whash[crc].emplace(s, windex);
                return windex;
            }
        }
        return 0;
    }
    void write_begin(vfs::iwriter w)
    {
        check_end();
        w.chunk_open(chunk_data_id);
    }
};