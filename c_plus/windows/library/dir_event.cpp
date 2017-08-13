#include "dir_event.h"
#include <process.h>

#include <tchar.h>
#include <strsafe.h>

XDirEvent::XDirEvent ()
{
	m_dir_event_handle	= INVALID_HANDLE_VALUE;
	m_iocp_handle					= NULL;

	ZeroMemory (&m_Dummy, sizeof(m_Dummy));
}

XDirEvent::~XDirEvent ()
{

}

bool XDirEvent::clear ()
{
	if (NULL != m_iocp_handle)
	{
		if (FALSE == ::PostQueuedCompletionStatus (m_iocp_handle, 0, 1, &m_OverLapped))
		{
			return false;
		}
	}
	Sleep(100);

	::CloseHandle (m_thread_handle);
	m_thread_handle = NULL;

	if (INVALID_HANDLE_VALUE != m_dir_event_handle)
	{
		if (FALSE == ::CloseHandle (m_dir_event_handle))
		{
			return false;
		}
	}
	m_dir_event_handle = INVALID_HANDLE_VALUE;

	if (NULL != m_iocp_handle)
	{
		if (FALSE == ::CloseHandle (m_iocp_handle))
		{
			return false;
		}
	}
	m_iocp_handle = NULL;

	return true;
}

bool XDirEvent::thread_function ()
{
	DWORD			NumberOfBytesTransferred	= 0;
    ULONG_PTR		CompletionKey				= 0;
    LPOVERLAPPED	pOverlapped					= NULL;
    
	if (FALSE == ::GetQueuedCompletionStatus (m_iocp_handle, &NumberOfBytesTransferred, &CompletionKey, &pOverlapped, INFINITE))
	{
		return false;
	}

	if (1 == CompletionKey)
	{
		return false;
	}

	if (0 == CompletionKey)
	{
		if (0 != NumberOfBytesTransferred)
		{
			FILE_NOTIFY_INFORMATION* pfni = NULL;
			pfni = (FILE_NOTIFY_INFORMATION*)m_Buf.buf();
			if (0 == memcmp (pfni, &m_Dummy, sizeof(m_Dummy)))
			{
				return true;
			}

			DWORD next_entry_offset = 0;
			do
			{
				SFileEventInfo FileEventInfo;
				ZeroMemory (&FileEventInfo, sizeof(FileEventInfo));
				FileEventInfo.pInfo = pfni;
				StringCbCopyNW (FileEventInfo.name, sizeof(FileEventInfo.name), pfni->FileName, pfni->FileNameLength);
				if (false == m_dir_event (&FileEventInfo))
				{
					return false;
				}

				next_entry_offset = pfni->NextEntryOffset;
				pfni = (FILE_NOTIFY_INFORMATION*)((PBYTE)pfni + next_entry_offset);
			} while (next_entry_offset > 0);
		}

		DWORD BytesReturned = 0;
		ZeroMemory (m_Buf.buf(), sizeof(FILE_NOTIFY_INFORMATION));
		BOOL return_code = ::ReadDirectoryChangesW (m_dir_event_handle, m_Buf.buf(), m_Buf.size(), m_sub_tree, m_notify_filter, &BytesReturned, &m_OverLapped, NULL);     
		if (!return_code)
		{         
			return false;   
		}

		return true;
	}

	return true;
}

bool XDirEvent::create (TCHAR* p_dir, bool sub_tree, DWORD filter, dir_event_t _dir_event)
{
	ZeroMemory (&m_OverLapped, sizeof(m_OverLapped));
	m_notify_filter = filter;

	m_dir_event_handle = ::CreateFile (p_dir, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, 0);
	if (INVALID_HANDLE_VALUE == m_dir_event_handle)
	{
		return false;
	}

	m_iocp_handle = ::CreateIoCompletionPort (m_dir_event_handle, m_iocp_handle, 0, 0);
	if (NULL == m_iocp_handle)
	{
		return false;
	}

	if (false == m_Buf.create (1024*1000))
	{
		return false;
	}
	m_Buf.zero_memory ();

	if (sub_tree)
	{
		m_sub_tree = TRUE;
	}
	else
	{
		m_sub_tree = FALSE;
	}

	m_dir_event = _dir_event;

	m_thread_handle = (HANDLE)_beginthreadex (NULL, 0, &_thread_function, (void*)this, 0, &m_thread_id);
	if (NULL == m_thread_handle)
	{
		return false;
	}

	if (FALSE == PostQueuedCompletionStatus (m_iocp_handle, 0, 0, &m_OverLapped))
	{
		return false;
	}

	return true;
}

unsigned __stdcall XDirEvent::_thread_function (void* p_arg)
{
	XDirEvent* pThread = (XDirEvent*)p_arg;

	for (;;)
	{
		if (false == pThread->thread_function ())
		{
			break;
		}
		::Sleep (0);
	}
	return NULL;
}
