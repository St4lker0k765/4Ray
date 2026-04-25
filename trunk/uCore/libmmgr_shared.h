#pragma once

#pragma pack(push,4)
//////////////////////////////////////////////////////////////////////////
typedef const char* str_c;
//////////////////////////////////////////////////////////////////////////
enum str_type : u16
{
	default_string = 0,
	object_clsid,
	script_clsid,
	locator_id,
	sdata_key,
	texture_set,
	type_count,
};
//////////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4200)
struct __declspec(align(8)) UCORE_API	str_value
{
	str_value*			next;
	u32					refs;
	u16					length;
	str_type			str_type;
	u32					crc;
	char				value[];
};
struct		UCORE_API	str_value_cmp
{ // less
	inline bool		operator ()	(const str_value* A, const str_value* B) const { return A->crc < B->crc; };
};
#pragma warning(default : 4200)
//////////////////////////////////////////////////////////////////////////
class					str_shared
{
private:
	str_value* p_;
protected:
	// ref-counting
	void				_dec() { if (0 == p_) return;	p_->refs--; 	if (0 == p_->refs)	p_ = 0; }
public:
	void				_set(str_c rhs) { str_value* v = g_string_container->dock(rhs, default_string); if (0 != v) v->refs++; _dec(); p_ = v; }
	void				_set(str_shared const& rhs) { str_value* v = rhs.p_; if (0 != v) v->refs++; _dec(); p_ = v; }
	const str_value* _get()	const { return p_; }
public:
	// construction
	str_shared() { p_ = 0; }
	str_shared(str_c rhs) { p_ = 0;	_set(rhs); }
	str_shared(str_shared const& rhs) { p_ = 0;	_set(rhs); }
	~str_shared() { _dec(); }

	// assignment & accessors
	str_shared& operator=	(str_c rhs) { _set(rhs);	return (str_shared&)*this; }
	str_shared& operator=	(str_shared const& rhs) { _set(rhs);	return (str_shared&)*this; }
	str_c				operator*	() const { return p_ ? p_->value : 0; }
	bool				operator!	() const { return p_ == 0; }
	char				operator[]	(size_t id) { return p_->value[id]; }
	str_c				c_str() const { return p_ ? p_->value : 0; }

	// misc func
	u32					size()						const { if (0 == p_) return 0; else return p_->length; }
	void				swap(str_shared& rhs) { str_value* tmp = p_; p_ = rhs.p_; rhs.p_ = tmp; }
	bool				equal(const str_shared& rhs) const { return (p_ == rhs.p_); }
	str_shared& __cdecl	sprintf(const char* format, ...)
	{
		string4096 	buf;
		va_list		p;
		va_start(p, format);
		int vs_sz = _vsnprintf(buf, sizeof(buf) - 1, format, p); buf[sizeof(buf) - 1] = 0;
		va_end(p);
		if (vs_sz)	_set(buf);
		return 		(str_shared&)*this;
	}
};

// res_ptr == res_ptr
// res_ptr != res_ptr
// const res_ptr == ptr
// const res_ptr != ptr
// ptr == const res_ptr
// ptr != const res_ptr
// res_ptr < res_ptr
// res_ptr > res_ptr
inline bool operator	==	(str_shared const& a, str_shared const& b) { return a._get() == b._get(); }
inline bool operator	!=	(str_shared const& a, str_shared const& b) { return a._get() != b._get(); }
inline bool operator	<	(str_shared const& a, str_shared const& b) { return a._get() < b._get(); }
inline bool operator	>	(str_shared const& a, str_shared const& b) { return a._get() > b._get(); }

//////////////////////////////////////////////////////////////////////////
class		UCORE_API	str_container : public threading::mutex
{
private:
	u_vector<str_value*> buckets;
	str_value*			gc_iterator;
	u32					gc_bucket;
	u32					amount;
	str_value*			zero_len_str;
public:
	str_container();
	str_container(const str_container* other);
	~str_container();

	str_value*			do_dock(str_c value, u32 s_len, str_type s_type);
	str_value*			dock(str_c value, u32 len, str_type s_type);
	str_value*			dock(str_c value, str_type s_type);
	str_value*			dock_crc(u32 s_crc, str_type s_type);
	void				clean();
	void				dump();
	void				gc_step();
	void				integrity();
	void				integrity_type(str_type str_type);
	// Debug function :(
	void				load_stable() {}
	u64					stats();
};
UCORE_API	extern		str_container* g_string_container;
//////////////////////////////////////////////////////////////////////////
struct smem_value // sizeof=0x20;variable_size
{
	smem_value * next;
	volatile int reference;
	u32 crc;
	u32 length;
	void* padding;
	char value[];
};
//////////////////////////////////////////////////////////////////////////
class UCORE_API smem_container : public threading::mutex
{
protected:
	u_vector<smem_value*> buckets;
public:
	smem_container();
	smem_container(const smem_container* other);
	virtual ~smem_container();

	void clean();
	smem_value* dock(u32 crc, u32 count, u32 elen, char* ptr);
	void integrity() {}
};

UCORE_API extern smem_container* g_shared_memory_container;
#pragma pack(pop)
