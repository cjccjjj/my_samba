// dllmain.cpp : Defines the entry point for the DLL application.
//#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "app_global.h"
#include "../../../common/library/data_type.h"
#include "../../../windows/library/util.h"
#include <stdlib.h>
#include <tchar.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) bool samba_create (char* p_ip, int port, char* p_id, char* p_password, char* p_local_dir, char* p_remote_dir, Win32Callback callback)
{
	TCHAR ip [20] = _T("");
	TCHAR id [1024] = _T("");
	TCHAR password [1024] = _T("");
	TCHAR local_dir [1024] = _T("");
	TCHAR remote_dir [1024] = _T("");
	m2w(p_ip, ip, _countof(ip));
	m2w(p_id, id, _countof(id));
	m2w(p_password, password, _countof(password));
	m2w(p_local_dir, local_dir, _countof(local_dir));
	m2w(p_remote_dir, remote_dir, _countof(remote_dir));
	return _samba_create(ip, port, id, password, local_dir, remote_dir, callback);
}
extern "C" __declspec(dllexport) void samba_clear ()
{
	_samba_clear();
}
extern "C" __declspec(dllexport) void samba_clear_exclusive_ext ()
{
	_clear_exclusive_ext();
}
extern "C" __declspec(dllexport) bool samba_add_exclusive_ext (char* p_exclusive_ext)
{
	TCHAR exclusive_ext [MAX_PATH] = _T("");
	m2w(p_exclusive_ext, exclusive_ext, _countof(exclusive_ext));
	return _add_exclusive_ext(exclusive_ext);
}
extern "C" __declspec(dllexport) void samba_clear_exclusive_dir ()
{
	_clear_exclusive_dir();
}
extern "C" __declspec(dllexport) bool samba_add_exclusive_dir (char* p_exclusive_dir)
{
	TCHAR exclusive_dir [MAX_PATH] = _T("");
	m2w(p_exclusive_dir, exclusive_dir, _countof(exclusive_dir));
	return _add_exclusive_dir(exclusive_dir);
}
