#ifndef __DIR_EVENT_H__
#define __DIR_EVENT_H__

#include "../../common/library/data_type.h"
#include "../../common/library/buf.h"

#include <tchar.h>

struct SFileEventInfo
{
	FILE_NOTIFY_INFORMATION* pInfo;
	TCHAR name [MAX_PATH];
};

typedef bool (*dir_event_t) (SFileEventInfo*);

class XDirEvent
{
public:
	XDirEvent ();
	~XDirEvent ();

	bool init_thread_function ();
	bool thread_function ();
	bool clear_thread_function ();

	bool create		(TCHAR* p_dir, bool sub_tree, DWORD filter, dir_event_t _dir_event);
	bool clear		();

	HANDLE					m_dir_event_handle;
	HANDLE					m_iocp_handle;
	XBuf <BYTE>	m_Buf;

	DWORD				m_notify_filter;
	BOOL				m_sub_tree;
	dir_event_t	m_dir_event;

	OVERLAPPED				m_OverLapped;
	FILE_NOTIFY_INFORMATION	m_Dummy;


	static unsigned __stdcall _thread_function (void* p_arg);

	size_t				m_thread_id;
	HANDLE				m_thread_handle;
};

#endif
