#include "util.h"

#include  <io.h>
#include <tchar.h>
#include <strsafe.h>

bool get_file_ext (TCHAR* p_file_path, TCHAR* p_file_ext, int file_ext_size)
{
	if (MAX_PATH > file_ext_size)
	{
		return false;
	}

	TCHAR szDrive     [MAX_PATH] = _T("");
	TCHAR szDir       [MAX_PATH] = _T("");
	TCHAR szFileName  [MAX_PATH] = _T("");

	_tsplitpath_s (p_file_path,
				szDrive, MAX_PATH,
				szDir, MAX_PATH,
				szFileName , MAX_PATH,
				p_file_ext, file_ext_size);

	return true;
}

int m2w (const char* p_multi_str, wchar_t* p_wide_str, int size)
{
	int need_size = ::MultiByteToWideChar (CP_ACP, 0, p_multi_str, -1, NULL, NULL);
	if (size < need_size)
	{
		return NULL;
	}
	return MultiByteToWideChar(CP_ACP, 0, p_multi_str, -1, p_wide_str, need_size);

}

int m2w (const char* p_multi_str, int cbMultiByte, wchar_t* p_wide_str, int size)
{

	if (cbMultiByte >= size)
	{
		return NULL;
	}

	return MultiByteToWideChar (CP_ACP, 0, p_multi_str, -1, p_wide_str, (int)size);

}

int w2m (const wchar_t* p_wide_str, char* p_multi_str, int size)
{
	int need_size = ::WideCharToMultiByte (CP_ACP, 0, p_wide_str, -1, NULL, 0, NULL, NULL);
	if (size < need_size)
	{
		return 0;
	}

	int ret_val = ::WideCharToMultiByte (CP_ACP, 0, p_wide_str, -1, p_multi_str, need_size, NULL, NULL);

	return ret_val;
}

int w2m (const wchar_t* p_wide_str, int size_w, char* p_multi_str, int size_a)
{
	if (size_w >= size_a)
	{
		return 0;
	}
	return ::WideCharToMultiByte (CP_ACP, 0, p_wide_str, size_w, p_multi_str, size_a, NULL, NULL);

}

sint08 str2sint08 (const TCHAR* p_str)
{
	TCHAR* p_end = NULL;
	sint64 file_size_n = _tcstoi64 (p_str, &p_end, 10);	// 10 진수

	return (sint08)file_size_n;
}

uint08	str2uint08	(const TCHAR* p_str)
{
	TCHAR* p_end = NULL;
	uint64 file_size_n = _tcstoui64 (p_str, &p_end, 10);

	return (uint08)file_size_n;
}

sint16	str2sint16	(const TCHAR* p_str)
{
	TCHAR* p_end = NULL;
	sint64 file_size_n = _tcstoi64 (p_str, &p_end, 10);

	return (sint16)file_size_n;
}
uint16	str2uint16	(const TCHAR* p_str)
{
	TCHAR* p_end = NULL;
	uint64 file_size_n = _tcstoui64 (p_str, &p_end, 10);

	return (uint16)file_size_n;
}
sint32	str2sint32	(const TCHAR* p_str)
{
	TCHAR* p_end = NULL;
	sint64 file_size_n = _tcstoi64 (p_str, &p_end, 10);

	return (sint32)file_size_n;
}
uint32	str2uint32	(const TCHAR* p_str)
{
	TCHAR* p_end = NULL;
	uint64 file_size_n = _tcstoui64 (p_str, &p_end, 10);

	return (uint32)file_size_n;
}
sint64	str2sint64	(const TCHAR* p_str)
{
	TCHAR* p_end = NULL;
	sint64 file_size_n = _tcstoi64 (p_str, &p_end, 10);

	return file_size_n;
}
uint64	str2uint64	(const TCHAR* p_str)
{
	TCHAR* p_end = NULL;
	uint64 file_size_n = _tcstoui64 (p_str, &p_end, 10);

	return file_size_n;
}

sint08 str2sint08 (const char* p_str)
{
	char* p_end = NULL;
	sint64 file_size_n = _strtoi64(p_str, &p_end, 10);	// 10 진수

	return (sint08)file_size_n;
}

uint08	str2uint08	(const char* p_str)
{
	char* p_end = NULL;
	uint64 file_size_n = _strtoui64(p_str, &p_end, 10);

	return (uint08)file_size_n;
}

sint16	str2sint16	(const char* p_str)
{
	char* p_end = NULL;
	sint64 file_size_n = _strtoi64 (p_str, &p_end, 10);

	return (sint16)file_size_n;
}
uint16	str2uint16	(const char* p_str)
{
	char* p_end = NULL;
	uint64 file_size_n = _strtoui64 (p_str, &p_end, 10);

	return (uint16)file_size_n;
}
sint32	str2sint32	(const char* p_str)
{
	char* p_end = NULL;
	sint64 file_size_n = _strtoi64 (p_str, &p_end, 10);

	return (sint32)file_size_n;
}
uint32	str2uint32	(const char* p_str)
{
	char* p_end = NULL;
	uint64 file_size_n = _strtoui64 (p_str, &p_end, 10);

	return (uint32)file_size_n;
}
sint64	str2sint64	(const char* p_str)
{
	char* p_end = NULL;
	sint64 file_size_n = _strtoi64 (p_str, &p_end, 10);

	return file_size_n;
}
uint64	str2uint64	(const char* p_str)
{
	char* p_end = NULL;
	uint64 file_size_n = _strtoui64 (p_str, &p_end, 10);

	return file_size_n;
}

TCHAR* num2str (sint08 num, TCHAR* p_str, size_t strlen_)
{
	return num2str ((sint64)num, p_str, strlen_);
}
TCHAR* num2str (sint16 num, TCHAR* p_str, size_t strlen_)
{
	return num2str ((sint64)num, p_str, strlen_);
}
TCHAR* num2str (sint32 num, TCHAR* p_str, size_t strlen_)
{
	return num2str ((sint64)num, p_str, strlen_);
}

TCHAR* num2str (uint08 num, TCHAR* p_str, size_t strlen_)
{
	return num2str ((uint64)num, p_str, strlen_);
}
TCHAR* num2str (uint16 num, TCHAR* p_str, size_t strlen_)
{
	return num2str ((uint64)num, p_str, strlen_);
}
TCHAR* num2str (uint32 num, TCHAR* p_str, size_t strlen_)
{
	return num2str ((uint64)num, p_str, strlen_);
}

TCHAR* num2str (sint64 num, TCHAR* p_str, size_t strlen_)
{
	if (21 > strlen_)
	{
		return NULL;
	}

	if (0 > num)
	{
		num = _abs64 (num);
		if (false == num2str ((uint64)num, p_str+1, --strlen_))
		{
			return false;
		}
		p_str[0] = _T('-');
		return p_str;
	}

	return num2str ((uint64)num, p_str, strlen_);
}

TCHAR* num2str (uint64 num, TCHAR* p_str, size_t strlen_)
{
	if (20 > strlen_)
	{
		return false;
	}

	if (0 == num)
	{
		StringCchCopy (p_str, strlen_, _T("0"));
		return p_str;
	}
	
	char temp [20];
	ZeroMemory (temp, sizeof(temp));
	
	int j = 18;
	for (int i = 0; i < 19; ++i)
	{
		if (0 == num)
		{
			break;
		}
		char remain = (char)(num % 10);
		temp[j] = remain + '0';
		--j;
		num /= 10;
	}

	char* p = temp;
	for (int i = 0; i < _countof(temp); ++i)
	{
		if (0 == *p)
		{
			++p;
		}
		else
		{
			break;
		}
	}

	if (0 < m2w (p, p_str, strlen_))
	{
		return p_str;
	}
	return NULL;
}


char* num2str (sint08 num, char* p_str, size_t strlen_)
{
	return num2str ((sint64)num, p_str, strlen_);
}
char* num2str (sint16 num, char* p_str, size_t strlen_)
{
	return num2str ((sint64)num, p_str, strlen_);
}
char* num2str (sint32 num, char* p_str, size_t strlen_)
{
	return num2str ((sint64)num, p_str, strlen_);
}

char* num2str (uint08 num, char* p_str, size_t strlen_)
{
	return num2str ((uint64)num, p_str, strlen_);
}
char* num2str (uint16 num, char* p_str, size_t strlen_)
{
	return num2str ((uint64)num, p_str, strlen_);
}
char* num2str (uint32 num, char* p_str, size_t strlen_)
{
	return num2str ((uint64)num, p_str, strlen_);
}

char* num2str (sint64 num, char* p_str, size_t strlen_)
{
	if (21 > strlen_)
	{
		return NULL;
	}

	if (0 > num)
	{
		num = _abs64 (num);
		if (false == num2str ((uint64)num, p_str+1, --strlen_))
		{
			return false;
		}
		p_str[0] = _T('-');
		return p_str;
	}

	return num2str ((uint64)num, p_str, strlen_);
}
 
char* num2str (uint64 num, char* p_str, size_t strlen_)
{
	if (20 > strlen_)
	{
		return false;
	}

	if (0 == num)
	{
		StringCchCopyA(p_str, strlen_, ("0"));
		return p_str;
	}
	
	char temp [20];
	ZeroMemory (temp, sizeof(temp));
	
	int j = 18;
	for (int i = 0; i < 19; ++i)
	{
		if (0 == num)
		{
			break;
		}
		char remain = (char)(num % 10);
		temp[j] = remain + '0';
		--j;
		num /= 10;
	}

	char* p = temp;
	for (int i = 0; i < _countof(temp); ++i)
	{
		if (0 == *p)
		{
			++p;
		}
		else
		{
			break;
		}
	}

	StringCchCopyA(p_str, strlen_, p);
	return p_str;
}

char* num2str (double num, char* p_str, size_t strlen_)
{
	StringCchPrintfA(p_str, strlen_, ("%lf"), num);
	return p_str;
}

char* num2str (long double num, char* p_str, size_t strlen_)
{
	StringCchPrintfA (p_str, strlen_, ("%Lf"), num);
	return p_str; 
}

char* num2str (unsigned long num, char* p_str, size_t strlen_)
{
	StringCchPrintfA (p_str, strlen_, ("%lu"), num);
	return p_str;
}

TCHAR* tostr (const TCHAR* p, TCHAR* p_str, size_t strlen_)
{
	UNREFERENCED_PARAMETER(p);
	UNREFERENCED_PARAMETER(strlen_);
	return p_str;
}
TCHAR* tostr (TCHAR* p, TCHAR* p_str, size_t strlen_)
{
	UNREFERENCED_PARAMETER(p);
	UNREFERENCED_PARAMETER(strlen_);
	return p_str;
}
TCHAR* tostr (sint08 num, TCHAR* p_str, size_t strlen)
{
	return num2str (num, p_str, strlen);
}
TCHAR* tostr (sint16 num, TCHAR* p_str, size_t len)
{
	return num2str (num, p_str, len);
}
TCHAR* tostr (sint32 num, TCHAR* p_str, size_t len)
{
	return num2str (num, p_str, len);
}
TCHAR* tostr (sint64 num, TCHAR* p_str, size_t len)
	{
	return num2str (num, p_str, len);
}
TCHAR* tostr (uint08 num, TCHAR* p_str, size_t len)
	{
	return num2str (num, p_str, len);
}
TCHAR* tostr (uint16 num, TCHAR* p_str, size_t len)
	{
	return num2str (num, p_str, len);
}
TCHAR* tostr (uint32 num, TCHAR* p_str, size_t len)
	{
	return num2str (num, p_str, len);
}
TCHAR* tostr (uint64 num, TCHAR* p_str, size_t len)
	{
	return num2str (num, p_str, len);
}

bool is_dir (TCHAR* p_dir)
{
	DWORD dwAttr = GetFileAttributes (p_dir);
	DWORD last_error = 0;
	if (INVALID_FILE_ATTRIBUTES == dwAttr)
	{
		last_error = ::GetLastError ();
		return false;
	}

	if (FILE_ATTRIBUTE_DIRECTORY & dwAttr)
	{
		return true;
	}

	return false;
}
