#ifndef __SSH_H__
#define __SSH_H__

#include "../../common/library/data_type.h"

#include "libssh2_config.h"
#include <libssh2.h>

#ifdef HAVE_WINSOCK2_H
# include <winsock2.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
# include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
# ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>

class XSsh
{
public:
	XSsh ();
	~XSsh();

	bool create (TCHAR* p_ip, USHORT port, TCHAR* pid, TCHAR* p_password);
	void	clear ();

	bool copy (TCHAR* p_local_file, TCHAR* p_remote_file);
	bool command (TCHAR* p_command, TCHAR* p_response = NULL, size_t response_len = 0);

private:

	TCHAR m_ip [20];
	USHORT m_port;
	TCHAR m_id [1024];
	TCHAR m_password [1024];

	 LIBSSH2_SESSION* m_pSession;
	 int m_sock;
	 bool m_create_flag;
};

#endif
