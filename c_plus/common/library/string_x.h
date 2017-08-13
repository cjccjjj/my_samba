#ifndef __STRING_X_H__
#define __STRING_X_H__

#include "data_type.h"
#include "buf.h"
#include "tree.h"

class XString
{
public:
	XString	();
	~XString ();
	
	TCHAR*	create	(const TCHAR* p_str);
	TCHAR*	create	(const TCHAR* p_str, size_t len);
	TCHAR*	create (TCHAR* p_str_a, TCHAR* p_str_b);
	TCHAR*	create (const char* p_str, size_t str_len);
	TCHAR*	create (const char* p_str);
	
	void	clear	();
	bool	reserve	(size_t len);
	TCHAR*	c_str();
	size_t	size();

	bool	append (const TCHAR* p_str);
	bool	append (TCHAR _char);
	int		compare (const TCHAR* p_str);
	int		ncompare (const TCHAR* p_str, size_t count);
	int		nicompare (const TCHAR* p_str, size_t count);
	int		icompare (const TCHAR* p_str);

	TCHAR*	to_string ();
	uint08* to_uint08 (uint08* p_val);
	uint16* to_uint16 (uint16* p_val);
	uint32* to_uint32 (uint32* p_val);
	uint64* to_uint64 (uint64* p_val);

	sint08* to_sint08 (sint08* p_val);
	sint16* to_sint16 (sint16* p_val);
	sint32* to_sint32 (sint32* p_val);
	sint64* to_sint64 (sint64* p_val);

	uint08 to_uint08 ();
	uint16 to_uint16 ();
	uint32 to_uint32 ();
	uint64 to_uint64 ();

	sint08 to_sint08 ();
	sint16 to_sint16 ();
	sint32 to_sint32 ();
	sint64 to_sint64 ();

	//operator TCHAR* ()
	//{
	//	return NULL;
	//}
	operator TCHAR* ();
	operator uint08 ();
	operator uint16 ();
	operator uint32 ();
	operator uint64 ();
	operator sint08 ();
	operator sint16 ();
	operator sint32 ();
	operator sint64 ();

private:
	bool	m_create_flag;
	void	init	();

	XBuf <TCHAR> m_String;
};

void string_erase_trigger (void* p_data, size_t len, void* p_object);
class XStringArray
{
public:
	XStringArray ();
	~XStringArray ();
	
	bool	create	();
	void	clear	();
	bool	push		(TCHAR* p_data);

	uint32 get_count ();
	TCHAR* get_string (uint32 offset);

private:
	
	bool	m_create_flag;
	void	init	();
	XTree m_Tree;
	uint32 m_count;
};

int str_replace (TCHAR* p_src, TCHAR src, TCHAR target);

#endif
