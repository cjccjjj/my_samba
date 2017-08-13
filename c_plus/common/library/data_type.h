#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <windows.h>

typedef	unsigned char		uint08;
typedef	unsigned short		uint16;
typedef	unsigned int		uint32;
typedef	unsigned __int64	uint64;

typedef	signed char			sint08;
typedef	signed short		sint16;
typedef	signed int			sint32;
typedef	signed __int64		sint64;

#define EOS			_T('\0')

#endif
