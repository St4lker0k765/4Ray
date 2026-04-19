#ifndef _STL_EXT_internal
#define _STL_EXT_internal

using std::swap;

#include <type_traits>

template <class T>
class	u_alloc	{
public:
	typedef	size_t		size_type;
	typedef ptrdiff_t	difference_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	typedef T&			reference;
	typedef const T&	const_reference;
	typedef T			value_type;

public:
	template<class _Other>	
	struct rebind			{	typedef u_alloc<_Other> other;	};
public:
							pointer					address			(reference _Val) const					{	return (&_Val);	}
							const_pointer			address			(const_reference _Val) const			{	return (&_Val);	}
													u_alloc			()										{	}
													u_alloc			(const u_alloc<T>&)						{	}
	template<class _Other>							u_alloc			(const u_alloc<_Other>&)					{	}
	template<class _Other>	u_alloc<T>&				operator=		(const u_alloc<_Other>&)					{	return (*this);	}
							pointer					allocate		(size_type n, const void* p=0) const	{	return malloc<T>((u32)n);	}
							char*					_charalloc		(size_type n)							{	return (char*)allocate(n); }
							void					deallocate		(pointer p, size_type n) const			{	free	(p);				}
							void					deallocate		(void* p, size_type n) const			{	free	(p);				}
							template<class T>
							void construct(T* p, const T& _Val) { new(p)T(_Val); }
							template<class T>
							void destroy(T* p) { p->~T(); }


							size_type				max_size		() const								{	size_type _Count = (size_type)(-1) / sizeof (T);	return (0 < _Count ? _Count : 1);	}
};

struct u_allocator {
	template <typename T>
	struct helper {
		typedef u_alloc<T>	result;
	};

	static	void	*alloc		(const u32 &n)	{	return malloc((u32)n);	}
	template <typename T>
	static	void	dealloc		(T *&p)			{	free(p);					}
};

template<class _Ty,	class _Other>	inline	bool operator==(const u_alloc<_Ty>&, const u_alloc<_Other>&)		{	return (true);							}
template<class _Ty, class _Other>	inline	bool operator!=(const u_alloc<_Ty>&, const u_alloc<_Other>&)		{	return (false);							}

namespace std
{
	template<class _Tp1, class _Tp2>	inline	u_alloc<_Tp2>&	__stl_alloc_rebind(u_alloc<_Tp1>& __a, const _Tp2*)	{	return (u_alloc<_Tp2>&)(__a);	}
	template<class _Tp1, class _Tp2>	inline	u_alloc<_Tp2>	__stl_alloc_create(u_alloc<_Tp1>&, const _Tp2*)		{	return u_alloc<_Tp2>();			}
};

// vector
#include <vector>
template	<typename T, typename allocator = u_alloc<T> >
class u_vector : public std::vector<T,allocator> {
private:
	typedef std::vector<T,allocator>	inherited;

public:
	typedef allocator					allocator_type;

public:
			u_vector			()									: inherited	()					{}
			u_vector			(size_t _count, const T& _value)	: inherited	(_count,_value)		{}
	explicit u_vector			(size_t _count)						: inherited (_count)			{}
	u32		size				() const							{ return (u32)inherited::size();} 

	void	clear_and_free		()									{ inherited::clear();			}
	void	clear_not_free		()									{ erase(begin(),end());			}
	void	clear_and_reserve	()									{ if ( capacity() <= (size()+size()/4) ) clear_not_free(); else { u32 old=size(); clear_and_free(); reserve(old); } }

	void	clear				()									{ clear_not_free	();			}

	const_reference operator[]	(size_type _Pos) const				{ {VERIFY(_Pos<size());} return (*(begin() + _Pos)); }
	reference operator[]		(size_type _Pos)					{ {VERIFY(_Pos<size());} return (*(begin() + _Pos)); }
};

// vector<bool>
template <>
class u_vector<bool,u_alloc<bool> >	: public std::vector<bool,u_alloc<bool> > {
private:
	typedef std::vector<bool,u_alloc<bool> > inherited;

public: 
	u32		size				() const							{ return (u32)inherited::size();} 
	void	clear				()									{ erase(begin(),end());			} 
};

template <typename allocator>
class u_vector<bool,allocator>	: public std::vector<bool,allocator> {
private:
	typedef std::vector<bool,allocator> inherited;

public: 
	u32		size				() const							{ return (u32)inherited::size();} 
	void	clear				()									{ erase(begin(),end());			} 
};

// stack
template <typename _Ty, class _C = u_vector<_Ty> >
class u_stack {
public:
	typedef typename _C::allocator_type						allocator_type;
	typedef typename allocator_type::value_type				value_type;
	typedef typename allocator_type::size_type				size_type;

	allocator_type		get_allocator	() const							{return (c.get_allocator()); }
	bool				empty			() const							{return (c.empty()); }
	u32					size			() const							{return c.size(); } 
	value_type&			top				()									{return (c.back()); }
	const value_type&	top				() const							{return (c.back()); }
	void				push			(const value_type& _X)				{c.push_back(_X); }
	void				pop				()									{c.pop_back(); }
	bool				operator==		(const u_stack<_Ty, _C>& _X) const	{return (c == _X.c); }
	bool				operator!=		(const u_stack<_Ty, _C>& _X) const	{return (!(*this == _X)); }
	bool				operator<		(const u_stack<_Ty, _C>& _X) const	{return (c < _X.c); }
	bool				operator>		(const u_stack<_Ty, _C>& _X) const	{return (_X < *this); }
	bool				operator<=		(const u_stack<_Ty, _C>& _X) const	{return (!(_X < *this)); }
	bool				operator>=		(const u_stack<_Ty, _C>& _X) const	{return (!(*this < _X)); }

protected:
	_C c;
};

template	<typename T, typename allocator = u_alloc<T> >									class	u_list 			: public std::list<T,allocator>			{ public: u32 size() const {return (u32)__super::size(); } };
template	<typename K, class P=std::less<K>, typename allocator = u_alloc<K> >				class	u_set			: public std::set<K,P,allocator>		{ public: u32 size() const {return (u32)__super::size(); } };
template	<typename K, class P=std::less<K>, typename allocator = u_alloc<K> >				class	u_multiset		: public std::multiset<K,P,allocator>	{ public: u32 size() const {return (u32)__super::size(); } };
template	<typename K, class V, class P=std::less<K>, typename allocator = u_alloc<std::pair<K,V> > >	class	u_map 			: public std::map<K,V,P,allocator>		{ public: u32 size() const {return (u32)__super::size(); } };
template	<typename K, class V, class P=std::less<K>, typename allocator = u_alloc<std::pair<K,V> > >	class	u_multimap		: public std::multimap<K,V,P,allocator>	{ public: u32 size() const {return (u32)__super::size(); } };

#endif

template	<class _Ty1, class _Ty2> inline	std::pair<_Ty1, _Ty2>		mk_pair		(_Ty1 _Val1, _Ty2 _Val2)	{	return (std::pair<_Ty1, _Ty2>(_Val1, _Val2));	}

struct pred_str
{	
	inline bool operator()(const char* x, const char* y) const				{	return strcmp(x,y)<0;	}
};
struct pred_stri
{	
	inline bool operator()(const char* x, const char* y) const				{	return stricmp(x,y)<0;	}
};

// STL extensions
#define DEF_VECTOR(N,T)				typedef u_vector< T > N;		typedef N::iterator N##_it;
#define DEF_LIST(N,T)				typedef u_list< T > N;			typedef N::iterator N##_it;
#define DEF_MAP(N,K,T)				typedef u_map< K, T > N;		typedef N::iterator N##_it;

#define DEFINE_LIST(T,N,I)			typedef u_list< T > N;			typedef N::iterator I;
#define DEFINE_VECTOR(T,N,I)		typedef u_vector< T > N;		typedef N::iterator I;
#define DEFINE_MAP(K,T,N,I)			typedef u_map< K , T > N;		typedef N::iterator I;
#define DEFINE_MAP_PRED(K,T,N,I,P)	typedef u_map< K, T, P > N;	typedef N::iterator I;
#define DEFINE_MMAP(K,T,N,I)		typedef u_multimap< K, T > N;	typedef N::iterator I;
#define DEFINE_SVECTOR(T,C,N,I)		typedef svector< T, C > N;		typedef N::iterator I;
#define DEFINE_SET(T,N,I)			typedef u_set< T > N;			typedef N::iterator I;
#define DEFINE_SET_PRED(T,N,I,P)	typedef u_set< T, P > N;		typedef N::iterator I;
#define DEFINE_STACK(T,N)			typedef u_stack< T > N;

#include "u_svector.h"

// auxilary definition
DEFINE_VECTOR(bool,boolVec,boolIt);
DEFINE_VECTOR(BOOL,BOOLVec,BOOLIt);
DEFINE_VECTOR(BOOL*,LPBOOLVec,LPBOOLIt);
DEFINE_VECTOR(LPSTR,LPSTRVec,LPSTRIt);
DEFINE_VECTOR(LPCSTR,LPCSTRVec,LPCSTRIt);
DEFINE_VECTOR(u_string,SStringVec,SStringVecIt);

DEFINE_VECTOR(s8,S8Vec,S8It);
DEFINE_VECTOR(s8*,LPS8Vec,LPS8It);
DEFINE_VECTOR(s16,S16Vec,S16It);
DEFINE_VECTOR(s16*,LPS16Vec,LPS16It);
DEFINE_VECTOR(s32,S32Vec,S32It);
DEFINE_VECTOR(s32*,LPS32Vec,LPS32It);
DEFINE_VECTOR(u8,U8Vec,U8It);
DEFINE_VECTOR(u8*,LPU8Vec,LPU8It);
DEFINE_VECTOR(u16,U16Vec,U16It);
DEFINE_VECTOR(u16*,LPU16Vec,LPU16It);
DEFINE_VECTOR(u32,U32Vec,U32It);
DEFINE_VECTOR(u32*,LPU32Vec,LPU32It);
DEFINE_VECTOR(float,FloatVec,FloatIt);
DEFINE_VECTOR(float*,LPFloatVec,LPFloatIt);
DEFINE_VECTOR(int,IntVec,IntIt);
DEFINE_VECTOR(int*,LPIntVec,LPIntIt);
