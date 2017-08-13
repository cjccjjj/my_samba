#include "iconv.h"
#include "../../common/library/string_x.h"

#include <tchar.h>
#include <strsafe.h>

XIconv::XIconv ()
{
	//m_iconv = (iconv_t)(-1);
	m_convert_len = 0;
	m_same_code_flag = false;
}

XIconv::XIconv (const char* p_to_code, const char* p_from_code)
{
	m_convert_len = 0;
	m_same_code_flag = false;

	this->open(p_to_code, p_from_code);
}

XIconv::~XIconv ()
{
	close ();
}

bool XIconv::open (const char* p_to_code, const char* p_from_code)
{
	if (!strcmp(p_to_code, p_from_code))
	{
		m_same_code_flag = true;
		return true;
	}

	StringCchCopyA(m_from_code, _countof(m_from_code), p_from_code);
	StringCchCopyA(m_to_code, _countof(m_to_code), p_to_code);
	return true;
}

bool XIconv::is_open ()
{


	return true;
}

void XIconv::close ()
{

}

TCHAR* XIconv::convert (TCHAR* p_in_buf, size_t in_buf_size, size_t* p_convert_size)
{
	if (m_same_code_flag)
	{
		if (!strcmp("UTF-16LE", m_from_code))
		{
			m_UTF16LE.clear();
			if (false == m_UTF16LE.create((int)in_buf_size+1))
			{
				return false;
			}
			*p_convert_size = in_buf_size;
			StringCchCopy(m_UTF16LE.buf(), m_UTF16LE.size(), p_in_buf);
			return m_UTF16LE.buf();
		}
		if (!strcmp("UTF-8", m_from_code))
		{
			m_UTF8.clear();
			if (false == m_UTF8.create((int)in_buf_size+1))
			{
				return false;
			}
			*p_convert_size = in_buf_size;
			StringCchCopyA(m_UTF8.buf(), m_UTF8.size(), (char*)p_in_buf);
			return (TCHAR*)m_UTF8.buf();
		}
		return NULL;
	}

	// UTF-16LE -> UTF-8
	if (!strcmp("UTF-16LE", m_from_code) && !strcmp("UTF-8", m_to_code))
	{
		int need_size = ::WideCharToMultiByte(CP_UTF8, 0, p_in_buf, (int)in_buf_size, NULL, NULL, NULL, NULL);
		m_UTF8.clear();
		if (false == m_UTF8.create(need_size+1))
		{
			return false;
		}
		::WideCharToMultiByte(CP_UTF8, 0, p_in_buf, (int)in_buf_size, m_UTF8.buf(), m_UTF8.size(), NULL, NULL);
		memset(&m_UTF8[need_size], 0x00, sizeof(char));		
		*p_convert_size = need_size;
		return (TCHAR*)m_UTF8.buf();
	}

	// UTF-8 -> UTF-16LE
	if (!strcmp("UTF-8", m_from_code) && !strcmp("UTF-16LE", m_to_code))
	{
		int need_size = ::MultiByteToWideChar(CP_UTF8, 0, (char*)p_in_buf, (int)in_buf_size, NULL, NULL);
		m_UTF16LE.clear();
		if (false == m_UTF16LE.create((int)in_buf_size+1))
		{
			return false;
		}
		::MultiByteToWideChar(CP_UTF8, 0, (char*)p_in_buf, (int)in_buf_size, m_UTF16LE.buf(), m_UTF16LE.size());
		memset(&m_UTF16LE[need_size], 0x00, sizeof(TCHAR));
		*p_convert_size = need_size;
		return m_UTF16LE.buf();
	}

	// UTF-16LE -> CP_ACP
	if (!strcmp("UTF-16LE", m_from_code) && !strcmp("CP_ACP", m_to_code))
	{
		int need_size = ::WideCharToMultiByte (CP_ACP, 0, p_in_buf, (int)in_buf_size, NULL, 0, NULL, NULL);
		m_UTF8.clear();
		if (false == m_UTF8.create(need_size+1))
		{
			return false;
		}

		// 변환후에 NULL 문자는  WideCharToMultiByte 에서 붙여준다.
		int ret_val = ::WideCharToMultiByte (CP_ACP, 0, p_in_buf, (int)in_buf_size, m_UTF8.buf(), m_UTF8.size(), NULL, NULL);
		memset(&m_UTF8[need_size], 0x00, sizeof(char));
		*p_convert_size = need_size;
		return (TCHAR*)m_UTF8.buf();
	}

	// CP_ACP -> UTF-16LE 
	if (!strcmp("CP_ACP", m_from_code) && !strcmp("UTF-16LE", m_to_code))
	{
		int need_size = ::MultiByteToWideChar (CP_ACP, 0, (char*)p_in_buf, in_buf_size, NULL, NULL);
		m_UTF16LE.clear();
		if (false == m_UTF16LE.create((int)need_size+1))
		{
			return false;
		}
		int ret_val = ::MultiByteToWideChar(CP_ACP, 0, (char*)p_in_buf, -1, m_UTF16LE.buf(), m_UTF16LE.size());
		memset(&m_UTF16LE[need_size], 0x00, sizeof(TCHAR));
		*p_convert_size = need_size;
		return m_UTF16LE.buf();
	}

	return NULL;	
}

XM2w::XM2w ()
{}
XM2w::~XM2w	()
{}
TCHAR* XM2w::convert (char* p_in_buf, size_t in_buf_size, size_t* p_convert_size)
{
	// CP_ACP -> UTF-16LE 
	int need_size = ::MultiByteToWideChar (CP_ACP, 0, p_in_buf, in_buf_size, NULL, NULL);
	m_UTF16LE.clear();
	if (false == m_UTF16LE.create((int)need_size+1))
	{
		return false;
	}
	int ret_val = ::MultiByteToWideChar(CP_ACP, 0, p_in_buf, -1, m_UTF16LE.buf(), m_UTF16LE.size());
	memset(&m_UTF16LE[need_size], 0x00, sizeof(TCHAR));
	if (p_convert_size)
	{
		*p_convert_size = need_size;
	}
	return m_UTF16LE.buf();
}
XW2m::XW2m ()
{}
XW2m::~XW2m	()
{}
char* XW2m::convert (TCHAR* p_in_buf, size_t in_buf_size, size_t* p_convert_size)
{
	int need_size = ::WideCharToMultiByte (CP_ACP, 0, p_in_buf, (int)in_buf_size, NULL, 0, NULL, NULL);
	m_UTF8.clear();
	if (false == m_UTF8.create(need_size+1))
	{
		return false;
	}
	int ret_val = ::WideCharToMultiByte (CP_ACP, 0, p_in_buf, (int)in_buf_size, m_UTF8.buf(), m_UTF8.size(), NULL, NULL);
	memset(&m_UTF8[need_size], 0x00, sizeof(char));
	if (p_convert_size)
	{
		*p_convert_size = need_size;
	}
	return m_UTF8.buf();
}

