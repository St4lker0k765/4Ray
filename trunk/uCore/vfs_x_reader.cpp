#include "stdafx.h"
#include "vfs_x_reader.h"
#include <fcntl.h>

vfs::x_reader::x_reader()
{
    _dbg_path = "";
    _h = -1;
    _pos = 0;
}

vfs::x_reader::x_reader(const vfs::x_reader* other)
{
    _h = other->_h;
    _pos = other->_pos;
    _dbg_path = other->_dbg_path;
    _size = other->_size;
}

vfs::x_reader::~x_reader()
{
    close();
}

void vfs::x_reader::clear()
{
    _h = -1;
    _pos = 0;
    _dbg_path = "";
}

void vfs::x_reader::close()
{
    if (_h != -1)
    {
        _close(_h);
        _h = -1;
        _pos = 0;
        _dbg_path = "";
    }
}

// St4lker0k765: Multiplayer code missing?
bool vfs::x_reader::downloading()
{
    int result = 0;
    if (_h != -1)
        result = x_status::opened;

    return result == x_status::downloading;
}

bool vfs::x_reader::invalid()
{
    return _h == -1;
}

u64 vfs::x_reader::open(const char* path, const int sequential)
{
    R_ASSERT(!opened());
    int flags = (sequential != 0 ? _O_SEQUENTIAL : _O_RDONLY) | _O_BINARY;
    _h = ::open(path, flags);
    if (_h != -1)
    {
        u32 len = _filelength(_h);
        if (len == -1)
            close();
        else
            _size = len;

        if (_h != -1)
        {
            _dbg_path = path;
        }
    }
    else
    {
        threading::yield(10);
        _h = ::open(path, flags);
        if (_h == -1)
        {
            rlog("_open error '%d', *v10");
        }
    }
    return _h != 1;
}

bool vfs::x_reader::opened()
{
    return _h != -1;
}

bool vfs::x_reader::read(void* dest, u32 size)
{
    if (_pos + size > _size)
    {
        rlog("_pos = %d, size = %d, _size = %d", _pos, size, _size);

        const char* file_name = "unknown file";
        if (_dbg_path.size())
            file_name = _dbg_path.c_str();

        debug::fatal("!!error reading '%s'", file_name);
    }
    R_ASSERT2(dest, _dbg_path.size() ? _dbg_path.c_str() : "unknown file");

    int result = ::read(_h, dest, size);
    if (result != -1)
        _pos += result;

    R_ASSERT(result == int(size));
    return result == size;
}

bool vfs::x_reader::seek(int pos)
{
    u32 s = lseek(_h, pos, 0);
    if (s != -1)
        _pos = s;

    return s != -1;
}

u64 vfs::x_reader::size()
{
    return _size;
}

u64 vfs::x_reader::status()
{
    u64 result = 0;
    if (_h != -1)
        return x_status::opened;

    return result;
}