#include "app_global.h"

#include "../../library/util.h"
#include "../../library/ssh.h"
#include "../../../common/library/string_x.h"

#include <tchar.h>
#include <strsafe.h>


XDirEvent g_DirEvent;
XSsh g_Ssh;

TCHAR g_ip [20] = _T("");
int g_port = 0;
TCHAR g_id [1024] = _T("");
TCHAR g_password [1024] = _T("");
TCHAR g_local_dir [1024] = _T("");
TCHAR g_remote_dir [1024] = _T("");


TCHAR g_old_file_name [1024] = _T("");

bool g_samba_create_flag = false;

XStringArray g_ExclusiveExt;
XStringArray g_ExclusiveDir;
Win32Callback g_func_ptr = NULL;
uint32 g_log_number = 0;
TCHAR g_log_buf [1024] = _T("");

bool _samba_create (TCHAR* p_ip, int port, TCHAR* p_id, TCHAR* p_password, TCHAR* p_local_dir, TCHAR* p_remote_dir, Win32Callback func_ptr)
{
	if (g_samba_create_flag)
	{
		return true;
	}
	g_samba_create_flag = true;


	StringCchCopy(g_ip, _countof(g_ip), p_ip);
	g_port = port;
	StringCchCopy(g_id, _countof(g_id), p_id);
	StringCchCopy(g_password, _countof(g_password), p_password);
	StringCchCopy(g_local_dir, _countof(g_local_dir), p_local_dir);
	StringCchCopy(g_remote_dir, _countof(g_remote_dir), p_remote_dir);
	g_func_ptr = func_ptr;
	if (false == g_Ssh.create(p_ip, port, p_id, p_password))
	{
		return false;
	}

	DWORD filter_flag = FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_LAST_ACCESS|FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_SECURITY;
	if (false == g_DirEvent.create(p_local_dir, true, filter_flag, dir_event_callback))
	{
		return false;
	}

	return true;
}

void _samba_clear ()
{
	if (false == g_samba_create_flag)
	{
		return ;
	}
	g_samba_create_flag = false;

	g_DirEvent.clear();
	g_Ssh.clear();
	return;
}

void _clear_exclusive_ext ()
{
	g_ExclusiveExt.clear();
	return;
}
bool _add_exclusive_ext (TCHAR* p_exclusive_ext)
{
	return g_ExclusiveExt.push(p_exclusive_ext);
}
void _clear_exclusive_dir ()
{
	g_ExclusiveDir.clear();
	return;
}
bool _add_exclusive_dir (TCHAR* p_exclusive_dir)
{
	return g_ExclusiveDir.push(p_exclusive_dir);
}

bool dir_event_callback (SFileEventInfo* pFileEventInfo)
{
	// 제외 확장자 체크
	TCHAR file_ext [MAX_PATH] = _T("");
	get_file_ext (pFileEventInfo->name, file_ext, _countof(file_ext));
	for (uint32 i=0;i<g_ExclusiveExt.get_count();++i)
	{
		if (0 == _tcscmp(file_ext, g_ExclusiveExt.get_string(i)))
		{
			ZeroMemory(g_log_buf, sizeof(g_log_buf));
			StringCchPrintf(g_log_buf, _countof(g_log_buf), _T("exclusive extension, %s\r\n"), pFileEventInfo->name);
			g_func_ptr(g_log_number, g_log_buf);
			++g_log_number;
			return true;
		}
	}
	// 제외 디렉터리 체크
	//TCHAR local_full_path [MAX_PATH] = _T("");
	//StringCchPrintf(local_full_path, _countof(local_full_path), _T("%s%s"), g_local_dir, pFileNotifyInfo->name);
	//TCHAR exclusive_full_path [MAX_PATH] = _T("");
	//for (uint32 i=0;i<g_ExclusiveDir.get_count();++i)
	//{
	//	ZeroMemory(exclusive_full_path, sizeof(exclusive_full_path));
	//	StringCchPrintf(exclusive_full_path, _countof(exclusive_full_path), _T("%s%s"), g_local_dir, g_ExclusiveDir.get_string(i));
	//	if (0 == _tcsncmp(local_full_path, exclusive_full_path, _tcslen(exclusive_full_path)))
	//	{
	//		ZeroMemory(g_log_buf, sizeof(g_log_buf));
	//		StringCchPrintf(g_log_buf, _countof(g_log_buf), _T("exclusive directory, %s\r\n"), local_full_path);
	//		g_func_ptr(g_log_number, g_log_buf);
	//		++g_log_number;
	//		return true;
	//	}
	//}
	
	if (FILE_ACTION_RENAMED_OLD_NAME == pFileEventInfo->pInfo->Action)
	{
		ZeroMemory(g_old_file_name, sizeof(g_old_file_name));
		StringCchPrintf(g_old_file_name, _countof(g_old_file_name), _T("%s%s"), g_remote_dir, pFileEventInfo->name);
		return true;
	}

	bool remove_flag = false;
	if (FILE_ACTION_REMOVED	== pFileEventInfo->pInfo->Action)
	{
		remove_flag = true;
	}
	if (FILE_ACTION_RENAMED_NEW_NAME == pFileEventInfo->pInfo->Action)
	{
		if (0 == _tcslen(g_old_file_name))
		{
			return true;
		}

		TCHAR new_file_path [1024] = _T("");
		StringCchPrintf(new_file_path, _countof(new_file_path), _T("%s%s"), g_remote_dir, pFileEventInfo->name);
		TCHAR command_line [1024] = _T("");
		StringCchPrintf(command_line, _countof(command_line), _T("mv '%s' '%s'"), g_old_file_name, new_file_path);
		str_replace(command_line, _T('\\'), _T('/'));
		g_Ssh.command(command_line);

		ZeroMemory(g_log_buf, sizeof(g_log_buf));
		StringCchPrintf(g_log_buf, _countof(g_log_buf), _T("ssh command: %s\r\n"), command_line);
		g_func_ptr(g_log_number, g_log_buf);

		ZeroMemory (g_old_file_name, sizeof(g_old_file_name));
		
		return true;
	}

	TCHAR dir_file [MAX_PATH] = _T("");
	StringCchPrintf (dir_file, _countof(dir_file), _T("%s%s"), g_local_dir, pFileEventInfo->name);
	bool dir_flag = is_dir (dir_file);
	if (dir_flag)
	{
		TCHAR remote_full_path [MAX_PATH] = _T("");
		StringCchPrintf(remote_full_path, _countof(remote_full_path), _T("%s%s"), g_remote_dir, pFileEventInfo->name);
		str_replace(remote_full_path, _T('\\'), _T('/'));

		TCHAR command_line [1024] = _T("");
		StringCchPrintf(command_line, _countof(command_line), _T("mkdir -rf %s"), remote_full_path);
		g_Ssh.command(command_line);

		ZeroMemory(g_log_buf, sizeof(g_log_buf));
		StringCchPrintf(g_log_buf, _countof(g_log_buf), _T("ssh command: %s\r\n"), command_line);
		g_func_ptr(g_log_number, g_log_buf);
		return true;
	}
	
	if (remove_flag)
	{
		TCHAR remote_full_path [MAX_PATH] = _T("");
		StringCchPrintf(remote_full_path, _countof(remote_full_path), _T("%s%s"), g_remote_dir, pFileEventInfo->name);
		str_replace(remote_full_path, _T('\\'), _T('/'));
		TCHAR command_line [1024] = _T("");
		StringCchPrintf(command_line, _countof(command_line), _T("rm -rf '%s'"), remote_full_path);
		g_Ssh.command(command_line);

		ZeroMemory(g_log_buf, sizeof(g_log_buf));
		StringCchPrintf(g_log_buf, _countof(g_log_buf), _T("ssh command: %s\r\n"), command_line);
		g_func_ptr(g_log_number, g_log_buf);
	}
	else
	{
		TCHAR local_full_path [MAX_PATH] = _T("");
		StringCchPrintf(local_full_path, _countof(local_full_path), _T("%s%s"), g_local_dir, pFileEventInfo->name);
		TCHAR remote_full_path [MAX_PATH] = _T("");
		StringCchPrintf(remote_full_path, _countof(remote_full_path), _T("%s%s"), g_remote_dir, pFileEventInfo->name);
		
		str_replace(remote_full_path, _T('\\'), _T('/'));
		
		ZeroMemory(g_log_buf, sizeof(g_log_buf));
		if (false == g_Ssh.copy(local_full_path, remote_full_path))
		{
			StringCchPrintf(g_log_buf, _countof(g_log_buf), _T("scp fail, %s\r\n"), pFileEventInfo->name);
		}
		else
		{
			StringCchPrintf(g_log_buf, _countof(g_log_buf), _T("scp, %s\r\n"), pFileEventInfo->name);
		}		
		g_func_ptr(g_log_number, g_log_buf);
	}

	return true;
}

