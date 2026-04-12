#ifndef xrstringH
#define xrstringH
#pragma once

#pragma pack(push,4)
//////////////////////////////////////////////////////////////////////////
typedef const char*		str_c;

//////////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4200)
struct		UCORE_API	str_value
{
	u32					dwReference		;
	u32					dwLength		;
	u32					dwCRC			;
	char				value		[]	;
};
struct		UCORE_API	str_value_cmp	{ // less
	IC bool		operator ()	(const str_value* A, const str_value* B) const	{ return A->dwCRC<B->dwCRC;	};
};
#pragma warning(default : 4200)

//////////////////////////////////////////////////////////////////////////
class		UCORE_API	str_container
{
private:
	typedef xr_multiset<str_value*,str_value_cmp>	cdb;
	xrCriticalSection								cs;
	cdb												container;
public:
	str_value*			dock			(str_c value);
	void				clean			();
	void				dump			();
	void				verify			();
	u32					stat_economy	();
						~str_container	();
};
UCORE_API	extern		str_container*	g_pStringContainer;

//////////////////////////////////////////////////////////////////////////
class					str_shared
{
private:
	str_value*			p_;
protected:
	// ref-counting
	void				_dec		()								{	if (0==p_) return;	p_->dwReference--; 	if (0==p_->dwReference)	p_=0;						}
public:
	void				_set		(str_c rhs) 					{	str_value* v = g_pStringContainer->dock(rhs); if (0!=v) v->dwReference++; _dec(); p_ = v;	}
	void				_set		(str_shared const &rhs)			{	str_value* v = rhs.p_; if (0!=v) v->dwReference++; _dec(); p_ = v;							}
	const str_value*	_get		()	const						{	return p_;																					}
public:
	// construction
						str_shared	()								{	p_ = 0;											}
						str_shared	(str_c rhs) 					{	p_ = 0;	_set(rhs);								}
						str_shared	(str_shared const &rhs)			{	p_ = 0;	_set(rhs);								}
						~str_shared()								{	_dec();											}

	// assignment & accessors
	str_shared&			operator=	(str_c rhs)						{	_set(rhs);	return (str_shared&)*this;			}
	str_shared&			operator=	(str_shared const &rhs)			{	_set(rhs);	return (str_shared&)*this;			}
	str_c				operator*	() const						{	return p_?p_->value:0;							}
	bool				operator!	() const						{	return p_ == 0;									}
	char				operator[]	(size_t id)						{	return p_->value[id];							}
	str_c				c_str		() const						{	return p_?p_->value:0;							}

	// misc func
	u32					size		()						const	{	if (0==p_) return 0; else return p_->dwLength;	}
	void				swap		(str_shared& rhs)				{	str_value* tmp = p_; p_ = rhs.p_; rhs.p_ = tmp;	}
	bool				equal		(const str_shared& rhs) const	{	return (p_ == rhs.p_);							}
	str_shared& __cdecl	sprintf		(const char* format, ...)		
	{
		string4096 	buf;
		va_list		p;
		va_start	(p,format);
		int vs_sz	= _vsnprintf(buf,sizeof(buf)-1,format,p); buf[sizeof(buf)-1]=0;
		va_end		(p);
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
inline bool operator	==	(str_shared const & a, str_shared const & b)		{ return a._get() == b._get();					}
inline bool operator	!=	(str_shared const & a, str_shared const & b)		{ return a._get() != b._get();					}
inline bool operator	<	(str_shared const & a, str_shared const & b)		{ return a._get() <  b._get();					}
inline bool operator	>	(str_shared const & a, str_shared const & b)		{ return a._get() >  b._get();					}

// externally visible standart functionality
IC void swap			(shared_str & lhs, shared_str & rhs)				{ lhs.swap(rhs);		}
IC u32	xr_strlen		(shared_str & a)									{ return a.size();		}
IC int	xr_strcmp		(const shared_str & a, const char* b)				{ return xr_strcmp(*a,b);	}
IC int	xr_strcmp		(const char* a, const shared_str & b)				{ return xr_strcmp(a,*b);	}
IC int	xr_strcmp		(const shared_str & a, const shared_str & b)		{ 
	if (a.equal(b))		return 0;
	else				return xr_strcmp(*a,*b);
}
IC void	xr_strlwr		(xr_string& src)									{ for(xr_string::iterator it=src.begin(); it!=src.end(); it++) *it=xr_string::value_type(tolower(*it));}
IC void	xr_strlwr		(shared_str& src)									{ if (*src){LPSTR lp=xr_strdup(*src); xr_strlwr(lp); src=lp; xr_free(lp);} }

#pragma pack(pop)

#endif
