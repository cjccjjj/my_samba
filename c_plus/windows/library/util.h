#ifndef __UTIL_H__
#define __UTIL_H__
#include "../../common/library/data_type.h"
#include "../../common/library/buf.h"

#include <tchar.h>

bool get_file_ext (TCHAR* p_file_path, TCHAR* p_file_ext, int file_ext_size);

int m2w(const char* p_multi_str, wchar_t* p_wide_str, int size);
int m2w(const char* p_multi_str, int cbMultiByte, wchar_t* p_wide_str, int size);
int w2m(const wchar_t* p_wide_str, char* p_multi_str, int size);
int	w2m(const wchar_t* p_wide_str, int size_w, char* p_multi_str, int size_a);

sint08	str2sint08	(const TCHAR* p_str);
uint08	str2uint08	(const TCHAR* p_str);
sint16	str2sint16	(const TCHAR* p_str);
uint16	str2uint16	(const TCHAR* p_str);
sint32	str2sint32	(const TCHAR* p_str);
uint32	str2uint32	(const TCHAR* p_str);
sint64	str2sint64	(const TCHAR* p_str);
uint64	str2uint64	(const TCHAR* p_str);

sint08	str2sint08	(const char* p_str);
uint08	str2uint08	(const char* p_str);
sint16	str2sint16	(const char* p_str);
uint16	str2uint16	(const char* p_str);
sint32	str2sint32	(const char* p_str);
uint32	str2uint32	(const char* p_str);
sint64	str2sint64	(const char* p_str);
uint64	str2uint64	(const char* p_str);

TCHAR* num2str (sint08 num, TCHAR* p_str, size_t len);
TCHAR* num2str (sint16 num, TCHAR* p_str, size_t len);
TCHAR* num2str (sint32 num, TCHAR* p_str, size_t len);
TCHAR* num2str (sint64 num, TCHAR* p_str, size_t len);
TCHAR* num2str (uint08 num, TCHAR* p_str, size_t len);
TCHAR* num2str (uint16 num, TCHAR* p_str, size_t len);
TCHAR* num2str (uint32 num, TCHAR* p_str, size_t len);
TCHAR* num2str (uint64 num, TCHAR* p_str, size_t len);

char* num2str (sint08 num, char* p_str, size_t len);
char* num2str (sint16 num, char* p_str, size_t len);
char* num2str (sint32 num, char* p_str, size_t len);
char* num2str (sint64 num, char* p_str, size_t len);
char* num2str (uint08 num, char* p_str, size_t len);
char* num2str (uint16 num, char* p_str, size_t len);
char* num2str (uint32 num, char* p_str, size_t len);
char* num2str (uint64 num, char* p_str, size_t len);

TCHAR* tostr (const TCHAR* p, TCHAR* p_str, size_t strlen_);
TCHAR* tostr (TCHAR* p, TCHAR* p_str, size_t strlen_);
TCHAR* tostr (sint08 num, TCHAR* p_str, size_t strlen);
TCHAR* tostr (sint16 num, TCHAR* p_str, size_t strlen);
TCHAR* tostr (sint32 num, TCHAR* p_str, size_t strlen);
TCHAR* tostr (sint64 num, TCHAR* p_str, size_t strlen_);
TCHAR* tostr (uint08 num, TCHAR* p_str, size_t strlen);
TCHAR* tostr (uint16 num, TCHAR* p_str, size_t strlen);
TCHAR* tostr (uint32 num, TCHAR* p_str, size_t strlen);
TCHAR* tostr (uint64 num, TCHAR* p_str, size_t strlen_);
TCHAR* tostr (double num, TCHAR* p_str, size_t strlen_);
TCHAR* tostr (long double num, TCHAR* p_str, size_t strlen_);
TCHAR* tostr (unsigned long num, TCHAR* p_str, size_t strlen_);

bool is_dir (TCHAR* p_dir);

#endif
