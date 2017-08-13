#include "string_x.h"

#include <new>

#include "../../windows/library/util.h"
#include <tchar.h>
#include <strsafe.h>

XString::XString ()
{
	init ();
	m_create_flag = false;
}

XString::~XString ()
{
	clear ();
}

void XString::init ()
{

}

TCHAR* XString::create (const TCHAR* p_str)
{
	if (NULL == p_str)
	{
		this->clear();
		return NULL;
	}

	size_t len = _tcslen(p_str);
	if (0 == len)
	{
		this->clear();
		return NULL;
	}
	if (false == m_String.create(len+1))
	{
		return NULL;
	}
	TCHAR* p_buf = m_String.buf();
	StringCchCopy (p_buf, m_String.size(), p_str);
	p_buf[m_String.size()-1] = EOS;

	return p_buf;
}

TCHAR* XString::create (const char* p_str, size_t str_len)
{
	XBuf <TCHAR> Temp;
	if (false == Temp.create(str_len+1))
	{
		return NULL;
	}

#pragma warning (disable: 4267)
	//m2w (p_str, str_len, Temp.buf(), (int)Temp.size());
	m2w (p_str, Temp.buf(), (int)Temp.size());

#pragma warning (default: 4267)

	return this->create(Temp.buf());
}
TCHAR* XString::create (const char* p_str)
{
	size_t len = strlen(p_str);
	return create (p_str, len);
}

TCHAR* XString::create (const TCHAR* p_str, size_t len)
{
	//size_t len = _tcslen(p_str);
	if (0 == len)
	{
		return NULL;
	}
	if (false == m_String.create(len+1))
	{
		return NULL;
	}
	TCHAR* p_buf = m_String.buf();
	StringCchCopy (p_buf, m_String.size(), p_str);
	p_buf[m_String.size()-1] = EOS;

	return p_buf;
}

TCHAR* XString::create (TCHAR* p_str_a, TCHAR* p_str_b)
{
	if (NULL==p_str_a || NULL==p_str_b)
	{
		return NULL;
	}

	size_t lena, lenb;
	lena = _tcslen(p_str_a);
	lenb = _tcslen(p_str_b);

	if (false == m_String.create(lena+lenb+1))
	{
		return NULL;
	}
	TCHAR* p_buf = m_String.buf();
	StringCchCopy (p_buf, m_String.size(), p_str_a);
	StringCchCat (p_buf, m_String.size(), p_str_b);
	p_buf[m_String.size()-1] = EOS;

	return p_buf;
}

void XString::clear ()
{
	m_String.clear();
}

bool XString::reserve (size_t len)
{
	if (0 == len)
	{
		return false;
	}

	XBuf <TCHAR> Temp;
	if (m_String.buf())
	{
		if (false == Temp.create (m_String.size()+1))
		{
			return false;
		}
		
		StringCchCopy (Temp.buf(), Temp.size(), m_String.buf());
	
		m_String.clear();
		if (false == m_String.create(len+1))
		{
			return false;
		}
		StringCchCopy (m_String.buf(), m_String.size(), Temp.buf());
		return true;
	}

	m_String.clear();
	if (false == m_String.create(len+1))
	{
		return false;
	}
	m_String[(size_t)0] = EOS;

	return true;
}

TCHAR* XString::c_str()
{
	return m_String.buf();
}

size_t XString::size ()
{
	if (1 < m_String.size())
	{
		if (EOS != m_String[(size_t)0])
		{
			return _tcslen(m_String.buf());
		}
	}
	return 0;
}

bool XString::append (const TCHAR* p_str)
{
	if (NULL == p_str)
	{
		return false;
	}

	if (0 < m_String.size() && m_String.size() > ( _tcslen(m_String.buf()) + _tcslen(p_str) ))
	{
		StringCchCat (m_String.buf(), m_String.size(), p_str);
		TCHAR* p_temp = m_String.buf();
		p_temp[_tcslen(m_String.buf())] = EOS;
		return true;
	}

	size_t need_size = _tcslen(p_str);
	if (m_String.buf())
	{
		need_size += _tcslen(m_String.buf());
	}
	++need_size;
	need_size *= 2;	// 메모리 재할당 빈도를 줄이기 위해서
	XBuf <TCHAR> m_Temp;
	if (false == m_Temp.create (need_size))
	{
		return false;
	}
	if (m_String.buf())
	{
		StringCchCopy (m_Temp.buf(), m_Temp.size(), m_String.buf());
		StringCchCat (m_Temp.buf(), m_Temp.size(), p_str);
	}
	else
	{
		StringCchCopy (m_Temp.buf(), m_Temp.size(), p_str);
	}

	m_String.clear();
	if (false == m_String.create (m_Temp.size()))
	{
		return false;
	}
	StringCchCopy (m_String.buf(), m_String.size(), m_Temp.buf());
	return true;
}

bool XString::append (TCHAR _char)
{
	TCHAR temp [2] = _T("");
	temp [0] = _char;
	return this->append(temp);
}

int XString::compare (const TCHAR* p_str)
{
	return _tcscmp(m_String.buf(), p_str);
}

int XString::ncompare (const TCHAR* p_str, size_t count)
{
	return _tcsncmp(m_String.buf(), p_str, count);
}

int XString::nicompare (const TCHAR* p_str, size_t count)
{
	return _tcsnicmp(m_String.buf(), p_str, count);
}
int XString::icompare (const TCHAR* p_str)
{
	return _tcsicmp(m_String.buf(), p_str);
}

TCHAR* XString::to_string ()
{
	return m_String.buf();
}

uint08* XString::to_uint08 (uint08* p_val)
{
	if (m_String.buf())
	{
		*p_val = str2uint08 (m_String.buf());
		return p_val;
	}	
	return NULL;
}
uint16* XString::to_uint16 (uint16* p_val)
{
	if (m_String.buf())
	{
		*p_val = str2uint16 (m_String.buf());
		return p_val;
	}
	return NULL;
}
uint32* XString::to_uint32 (uint32* p_val)
{
	if (m_String.buf())
	{
		*p_val = str2uint32 (m_String.buf());
		return p_val;
	}
	return NULL;
}
uint64* XString::to_uint64 (uint64* p_val)
{
	if (m_String.buf())
	{
		*p_val = str2uint64 (m_String.buf());
		return p_val;
	}
	return NULL;
}
sint08* XString::to_sint08 (sint08* p_val)
{
	if (m_String.buf())
	{
		*p_val = str2sint08 (m_String.buf());
		return p_val;
	}	
	return NULL;
}
sint16* XString::to_sint16 (sint16* p_val)
{
	if (m_String.buf())
	{
		*p_val = str2sint16 (m_String.buf());
		return p_val;
	}
	return NULL;
}
sint32* XString::to_sint32 (sint32* p_val)
{
	if (m_String.buf())
	{
		*p_val = str2sint32 (m_String.buf());
		return p_val;
	}
	return NULL;
}
sint64* XString::to_sint64 (sint64* p_val)
{
	if (m_String.buf())
	{
		*p_val = str2sint64 (m_String.buf());
		return p_val;
	}
	return NULL;
}

uint08 XString::to_uint08 ()
{
	if (m_String.buf())
	{
		return str2uint08 (m_String.buf());
	}
	return 0;
}
uint16 XString::to_uint16 ()
{
	if (m_String.buf())
	{
		return str2uint16 (m_String.buf());
	}
	return 0;
}
uint32 XString::to_uint32 ()
{
	if (m_String.buf())
	{
		return str2uint32 (m_String.buf());
	}
	return 0;
}
uint64 XString::to_uint64 ()
{
	if (m_String.buf())
	{
		return str2uint64 (m_String.buf());
	}
	return 0;
}
sint08 XString::to_sint08 ()
{
	if (m_String.buf())
	{
		return str2sint08 (m_String.buf());
	}	
	return 0;
}
sint16 XString::to_sint16 ()
{
	if (m_String.buf())
	{
		return str2sint16 (m_String.buf());
	}
	return 0;
}
sint32 XString::to_sint32 ()
{
	if (m_String.buf())
	{
		return str2sint32 (m_String.buf());
	}
	return 0;
}
sint64 XString::to_sint64 ()
{
	if (m_String.buf())
	{
		return str2sint64 (m_String.buf());
	}
	return 0;
}

XString::operator TCHAR* ()
{
	return m_String.buf();
}
XString::operator uint08 ()
{
	if (m_String.buf())
	{
		return str2uint08 (m_String.buf());
	}
	return 0;
}
XString::operator uint16 ()
{
	if (m_String.buf())
	{
		return str2uint16 (m_String.buf());
	}
	return 0;
}
XString::operator uint32 ()
{
	if (m_String.buf())
	{
		return str2uint32 (m_String.buf());
	}
	return 0;
}
XString::operator uint64 ()
{
	if (m_String.buf())
	{
		return str2uint32 (m_String.buf());
	}
	return 0;
}
XString::operator sint08 ()
{
	if (m_String.buf())
	{
		return str2sint08 (m_String.buf());
	}
	return 0;
}
XString::operator sint16 ()
{
	if (m_String.buf())
	{
		return str2sint16 (m_String.buf());
	}
	return 0;
}
XString::operator sint32 ()
{
	if (m_String.buf())
	{
		return str2sint32 (m_String.buf());
	}
	return 0;
}
XString::operator sint64 ()
{
	if (m_String.buf())
	{
		return str2sint32 (m_String.buf());
	}
	return 0;
}

/////////////////////////////////////////////
// XStringArray
void string_erase_trigger (void* p_data, size_t len, void* p_object)
{
	
}
XStringArray::XStringArray()
{
	m_count = 0;
}
XStringArray::~XStringArray()
{
	m_Tree.clear();
}
bool XStringArray::create ()
{
	m_Tree.create(sizeof(XString*), string_erase_trigger);
	return true;
}
void XStringArray::clear ()
{
	m_Tree.clear();
	m_count = 0;
}
bool XStringArray::push (TCHAR* p_str)
{	
	XString* pString = new(std::nothrow) XString;
	if (!pString)
	{
		return false;
	}
	if (false == pString->create (p_str))
	{
		return false;
	}

	bool ret_val = m_Tree.insert(m_count, pString);
	++m_count;
	return ret_val;
}

TCHAR* XStringArray::get_string (uint32 offset)
{
	XString* pString = (XString*)m_Tree.search(offset);
	return *pString;
}

uint32 XStringArray::get_count ()
{
	return m_Tree.get_key_count();
}

int str_replace (TCHAR* p_src, TCHAR src, TCHAR target)
{
	int ret_val = 0;
	size_t src_len = _tcslen(p_src);
	for (size_t i=0;i<src_len;++i)
	{
		if (src == p_src[i])
		{
			 p_src[i] = target;
			 ++ret_val;
		}
	}
	return ret_val;
}
