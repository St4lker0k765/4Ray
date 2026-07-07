#pragma once

template <typename T>
class archive_array_base : public intrusive_base
{
private:
	typedef intrusive_ptr<T> ClassPtr;
protected:

	u64 _attr_flag = 0;
	ClassPtr _array_archive = nullptr;
	ClassPtr _item_acrhive = nullptr;
	u32 _item_idx = 0;
public:
	const char* _key_format = "";
	const char* _caption_format = "";
	u32 _count = 0;
public:
	virtual inline void construct(ClassPtr array_archive, u64 attr_flag)
	{
		_item_idx = 0;
		_array_archive = array_archive;
		_attr_flag = attr_flag;
		_count = 0;
	}
	virtual void destroy_me() {}
	inline virtual bool need_value_key_caption() { return true; }
	inline u32 size() { return _count; }
	virtual ClassPtr create_section_for_reading(
		ClassPtr array_archive,
		u64 attr_flag,
		u32 item_idx,
		const char* value_key)
	{
		return nullptr;
	}
	virtual ClassPtr create_section_for_writing(
		ClassPtr array_archive,
		u64 attr_flag,
		u32 item_idx,
		const char* value_key,
		const char* value_caption)
	{
		return nullptr;
	}
};


template <typename T>
class archive_array_impl : public archive_array_base<T>
{
private:
	typedef archive_array_base<T> inherited;
	typedef intrusive_ptr<T> ClassPtr;
public:
	virtual inline void construct(ClassPtr array_archive, u64 attr_flag)
	{
		inherited::construct(array_archive, attr_flag);
	}
	inline void construct(ClassPtr array_archive, u64 attr_flag, u32 size, bool store_count)
	{
		inherited::construct(array_archive, attr_flag);
		if (array_archive->writing())
		{
			if (store_count)
				array_archive->w_u32(attr_flag, "count", nullptr, size, 0, -1u, 0);
			inherited::_count = size;
			return;
		}
		if (!store_count)
		{
			inherited::_count = size;
			return;
		}
		inherited::_count = array_archive->r_u32(attr_flag, "count");
	}
	static archive_array_impl<T>* create(ClassPtr array_archive, u64 attr_flag, u32 size, bool store_count)
	{
		archive_array_impl<T>* array = u_new<archive_array_impl<T>>();
		array->construct(array_archive, attr_flag, size, store_count);
		return array;
	}
	virtual ClassPtr create_section_for_reading(
		ClassPtr array_archive,
		u64 attr_flag,
		u32 item_idx,
		const char* value_key)
	{
		array_archive->r_section_s(attr_flag, value_key);
		return array_archive;
	}
	virtual ClassPtr create_section_for_writing(
		ClassPtr array_archive,
		u64 attr_flag,
		u32 item_idx,
		const char* value_key,
		const char* value_caption)
	{
		array_archive->w_section_s(attr_flag, value_key, value_caption);
		return array_archive;
	}
};
