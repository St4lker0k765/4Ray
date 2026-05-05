#pragma once

class locale
{
protected:
	str_shared _key;
	enum locale_type : u8
	{
		LT_REGULAR,
		LT_CONSOLE,
		LT_GAMEPAD,
		LT_PLATFORM,
		LT_NAME,
		LT_NUMBER,
		LT_KEY,
		LT_COUNT,
	};
	locale_type _type : 3;
	u32 _number : 29;
	bool _exist = false;
public:
	locale(u32 number, locale_type type);
	locale(const locale* other);
	locale(const str_shared key);
	locale(const str_shared key, locale_type type, bool extended_stable);

	bool exist();
	locale* key();
	void trace(const str_shared key);
	locale_type type();
	str_shared value(str_shared result, int ext);
};