#pragma once

class archive_array_base : public intrusive_base
{
protected:
	u64 _attr_flag = 0;
	const char* _key_format = "";
	const char* _caption_format = "";
	ARCHIVE_PTR _array_archive = nullptr;
	ARCHIVE_PTR _item_acrhive = nullptr;
	u32 _item_idx = 0;
	u32 _count = 0;
public:
	inline void construct(ARCHIVE_PTR array_archive, u64 attr_flag)
	{
		_item_idx = 0;
		_array_archive = array_archive;
		_attr_flag = attr_flag;
		_count = 0;
	}
	virtual void destroy_me() {}
	inline virtual bool need_value_key_caption() { return true; }
	inline u32 size() { return _count; }
};


class archive_array_impl : public archive_array_base
{
private:
	typedef archive_array_base inherited;
public:
	inline void construct(ARCHIVE_PTR array_archive, u64 attr_flag, u32 size, bool store_count)
	{
		inherited::construct(array_archive, attr_flag);
		if (array_archive->writing())
		{
			if (store_count)
				array_archive->w_u32_2(attr_flag, "count", nullptr, size, 0, -1u, 0);
			_count = size;
			return;
		}
		if (!store_count)
		{
			_count = size;
			return;
		}
		_count = array_archive->r_u32(attr_flag, "count");
	}
	static archive_array_impl* create(ARCHIVE_PTR array_archive, u64 attr_flag, u32 size, bool store_count)
	{
		archive_array_impl* array = u_new<archive_array_impl>();
		array->construct(array_archive, attr_flag, size, store_count);
		return array;
	}
	ARCHIVE_PTR* create_section_for_reading(
		ARCHIVE_PTR* result,
		const ARCHIVE_PTR array_archive,
		u64 attr_flag,
		u32 item_idx,
		const char* value_key)
	{
		array_archive->r_section_s(result, attr_flag, value_key);
		return result;
	}
	ARCHIVE_PTR* create_section_for_writing(
		ARCHIVE_PTR* result,
		const ARCHIVE_PTR array_archive,
		u64 attr_flag,
		u32 item_idx,
		const char* value_key,
		const char* value_caption)
	{
		array_archive->w_section_s(result, attr_flag, value_key, value_caption);
		return result;
	}
};