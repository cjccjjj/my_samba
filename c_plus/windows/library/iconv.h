#ifndef __ICONV_H__
#define __ICONV_H__

#include "../../common/library/data_type.h"
#include "../../common/library/buf.h"
#include <tchar.h>


// Iconv.open("UTF-16LE", "UTF-8");
class XIconv
{
public:
	XIconv	();
	XIconv	(const char* p_to_code, const char* p_from_code);
	~XIconv	();
	
	bool open		(const char* p_to_code, const char* p_from_code);
	void close		();
	bool is_open	();

	TCHAR* convert (TCHAR* p_in_buf, size_t in_buf_size, size_t* p_convert_size);

private:
	XBuf<TCHAR> m_UTF16LE;
	XBuf<char> m_UTF8;

	size_t m_convert_len;
	char m_from_code [100];
	char m_to_code [100];
	bool m_same_code_flag;
};

class XM2w
{
public:
	XM2w	();
	~XM2w	();

	TCHAR* convert (char* p_in_buf, size_t in_buf_size, size_t* p_convert_size);

private:
	XBuf<TCHAR> m_UTF16LE;
	size_t m_convert_len;
};

class XW2m
{
public:
	XW2m	();
	~XW2m	();

	char* convert (TCHAR* p_in_buf, size_t in_buf_size, size_t* p_convert_size);

private:
	XBuf<char> m_UTF8;
	size_t m_convert_len;
};

#endif
