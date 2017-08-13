#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "../../../common/library/data_type.h"
#include "../../../windows/library/dir_event.h"

#include <tchar.h>


typedef void(__stdcall *Win32Callback)(int value1, const wchar_t *text1);

bool _samba_create (TCHAR* p_ip, int port, TCHAR* p_id, TCHAR* p_password, TCHAR* p_local_dir, TCHAR* p_remote_dir, Win32Callback func_ptr);
void _samba_clear ();
void _clear_exclusive_ext ();
bool _add_exclusive_ext (TCHAR* p_exclusive_ext);
void _clear_exclusive_dir ();
bool _add_exclusive_dir (TCHAR* p_exclusive_dir);

bool dir_event_callback (SFileEventInfo* pFileEventInfo);

#endif
