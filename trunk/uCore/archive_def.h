#pragma

u32 archive_def::save_chunk_no(u32 save_chunk_mask)
{
    u32 mask = save_chunk_mask;
    if (!mask)
        return 0;

    u32 res = 0;
    if (!mask)
    {
        res = 16;
        mask >>= 16;
    }
    if (!mask)
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

u32 archive_def::save_chunk_no_from_flag(u64 fl)
{
    return save_chunk_no(fl >> 32 & 0xFFFFFFFF);
}

ARCHIVE_PTR archive_def::w_array_t(
    ARCHIVE_PTR result,
    u64 fl,
    const ARCHIVE_PTR array_arch,
    unsigned int size,
    const char* value_key_format,
    const char* value_caption_format)
{
    archive_array_impl<archive>* v10; // rbx
    int v11; // eax
    archive* object; // rcx
    bool v13; // zf
    archive_vtbl* v14; // rax

    R_ASSERT(array_arch);
    v10 = u_new<archive_array_impl<archive>>();
    archive_array_base<archive>::construct(v10, array_arch, fl);
    v11 = array_arch->_object->writing(array_arch->_object);
    object = array_arch->_object;
    v13 = v11 == 0;
    v14 = array_arch->_object->__vftable;
    if (v13)
        size = v14->r_u32(object, fl, "count");
    else
        v14->w_u32_2(object, fl, "count", nullptr, size, 0, -1u, 0);
    v10->_count = size;
    if (++v10->_ref_count <= 0)
        v10 = nullptr;
    result->_object = v10;
    v10->_key_format = value_key_format;
    v10->_caption_format = value_caption_format;
    return result;
}