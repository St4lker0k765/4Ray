#include "stdafx.h"
#pragma hdrstop

#include "u_string.h"

UCORE_API	str_container*	g_string_container	= nullptr;

#define		HEADER		12			// ref + len + crc

str_value*	str_container::dock		(str_c value)
{
	if (0==value)				return 0;

	cs.Enter					();
#ifdef DEBUG_MEMORY_MANAGER
	Memory.stat_strdock			++	;
#endif // DEBUG_MEMORY_MANAGER

	str_value*	result			= 0	;

	// calc len
	u32		s_len				= strlen(value);
	u32		s_len_with_zero		= (u32)s_len+1;
	VERIFY	(HEADER+s_len_with_zero < 4096);

	// setup find structure
	string16	header;
	str_value*	sv				= (str_value*)header;
	sv->refs					= 0;
	sv->length					= s_len;
	sv->crc						= crc32	(value,s_len);
	
	// search
	cdb::iterator	I			= container.find	(sv);	// only integer compares :)
	if (I!=container.end())		{
		// something found - verify, it is exactly our string
		cdb::iterator	save	= I;
		for (; I!=container.end() && (*I)->crc == sv->crc; ++I)	{
			str_value*	V		= (*I);
			if	(V->length!=sv->length)			continue;
			if	(0!=memcmp(V->value,value,s_len))	continue;
			result				= V;				// found
			break;
		}
	}

	// it may be the case, string is not fount or has "non-exact" match
	if (0==result)				{
		// Insert string

		result					= (str_value*)malloc(HEADER+s_len_with_zero);

		result->refs			= 0;
		result->length			= sv->length;
		result->crc				= sv->crc;
		CopyMemory				(result->value,value,s_len_with_zero);
		container.insert		(result);
	}
	cs.Leave					();

	return	result;
}

void		str_container::clean	()
{
	cs.Enter	();
	cdb::iterator	it	= container.begin	();
	cdb::iterator	end	= container.end		();
	for (; it!=end; )	{
		str_value*	sv = *it;
		if (0==sv->refs)	
		{
			cdb::iterator	i_current	= it;
			cdb::iterator	i_next		= ++it;
			free			(sv);
			container.erase	(i_current);
			it							= i_next;
		} else {
			it++;
		}
	}
	if (container.empty())	container.clear	();
	cs.Leave	();
}

void		str_container::verify	()
{
	cs.Enter	();
	cdb::iterator	it	= container.begin	();
	cdb::iterator	end	= container.end		();
	for (; it!=end; ++it)	{
		str_value*	sv		= *it;
		u32			crc		= crc32	(sv->value,sv->length);
		string32	crc_str;
		R_ASSERT3	(crc==sv->crc, "CorePanic: read-only memory corruption (shared_strings)", itoa(sv->crc,crc_str,16));
		R_ASSERT3	(sv->length == strlen(sv->value), "CorePanic: read-only memory corruption (shared_strings, internal structures)", sv->value);
	}
	cs.Leave	();
}

void		str_container::dump	()
{
	cs.Enter	();
	cdb::iterator	it	= container.begin	();
	cdb::iterator	end	= container.end		();
	FILE* F		= fopen("x:\\$str_dump$.txt","w");
	for (; it!=end; it++)
		fprintf		(F,"ref[%4d]-len[%3d]-crc[%8X] : %s\n",(*it)->dwReference,(*it)->dwLength,(*it)->dwCRC,(*it)->value);
	fclose		(F);
	cs.Leave	();
}

u32			str_container::stat_economy		()
{
	cs.Enter	();
	cdb::iterator	it		= container.begin	();
	cdb::iterator	end		= container.end		();
	int				counter	= 0;
	counter			-= sizeof(*this);
	counter			-= sizeof(cdb::allocator_type);
	const int		node_size = 20;
	for (; it!=end; it++)	{
		counter		-= HEADER;
		counter		-= node_size;
		counter		+= int((int((*it)->dwReference) - 1)*int((*it)->dwLength + 1));
	}
	cs.Leave		();

	return			u32(counter);
}

str_container::~str_container		()
{
	clean	();
	//R_ASSERT(container.empty());
}

u_string u_string::vset(const char* format, va_list arg_list)
{
	u_string temp;
	string4096 	buf;
	int vs_sz = _vsnprintf(buf, sizeof(buf) - 1, format, arg_list); 
	buf[sizeof(buf) - 1] = 0;
	if (vs_sz)	temp.assign(buf);
	va_end(arg_list);
	return 		temp;
}