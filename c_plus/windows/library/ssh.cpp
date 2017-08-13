#include "ssh.h"
#include "util.h"
#include "iconv.h"

#include <tchar.h>
#include <strsafe.h>

static int waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    FD_ZERO(&fd);

    FD_SET(socket_fd, &fd);

    /* now make sure we wait in the correct direction */
    dir = libssh2_session_block_directions(session);

    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;

    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;

    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);

    return rc;
}

XSsh::XSsh ()
{
	m_create_flag = false;
	m_sock = -1;
}

XSsh::~XSsh ()
{
}

bool XSsh::create (TCHAR* p_ip, USHORT port, TCHAR* p_id, TCHAR* p_password)
{
	if (m_create_flag)
	{
		return true;
	}
	m_create_flag = true;
	 
	struct sockaddr_in sin;
	char ip [20] = "";
	char id [1024] = "";
	char password [1024] = "";
	unsigned long hostaddr = 0;
	const char *fingerprint = NULL;
	int err;
	WSADATA wsadata;

	StringCchCopy(m_ip, _countof(m_ip), p_ip);
	m_port = port;
	StringCchCopy(m_id, _countof(m_id), p_id);
	StringCchCopy(m_password, _countof(m_password), p_password);

	
	w2m(p_ip, ip, _countof(ip));
	hostaddr = inet_addr(ip);

	w2m(m_id, id, _countof(id));
	w2m(m_password, password, _countof(password));

	

	
    

    err = WSAStartup(MAKEWORD(2,0), &wsadata);
    if (err != 0) {
        fprintf(stderr, "WSAStartup failed with error: %d\n", err);
        return false;
    }

	int rc = libssh2_init (0);
    if (rc != 0) {
        fprintf (stderr, "libssh2 initialization failed (%d)\n", rc);
        return false;
    }

	 /* Ultra basic "connect to port 22 on localhost"
     * Your code is responsible for creating the socket establishing the
     * connection
     */
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == m_sock) {
        fprintf(stderr, "failed to create socket!\n");
        return false;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(m_sock, (struct sockaddr*)(&sin),
            sizeof(struct sockaddr_in)) != 0) {
        fprintf(stderr, "failed to connect!\n");
        return false;
    }

    /* Create a session instance
     */
    m_pSession = libssh2_session_init();
    if(!m_pSession)
	{
        return false;
	}
    /* ... start it up. This will trade welcome banners, exchange keys,
     * and setup crypto, compression, and MAC layers
     */
    rc = libssh2_session_handshake(m_pSession, m_sock);
    if(rc) {
        fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
        return false;
    }

    /* At this point we havn't yet authenticated.  The first thing to do
     * is check the hostkey's fingerprint against our known hosts Your app
     * may have it hard coded, may go to a file, may present it to the
     * user, that's your call
     */
    fingerprint = libssh2_hostkey_hash(m_pSession, LIBSSH2_HOSTKEY_HASH_SHA1);
    fprintf(stderr, "Fingerprint: ");
    for(int i = 0; i < 20; i++) {
        fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
    }
    fprintf(stderr, "\n");

    //if (auth_pw) {
        /* We could authenticate via password */
        if (libssh2_userauth_password(m_pSession, id, password)) {
            fprintf(stderr, "Authentication by password failed.\n");
            //goto shutdown;
			return false;
        }
    //} else {
    //    /* Or by public key */
    //    if (libssh2_userauth_publickey_fromfile(session, username,
    //                        "/home/username/.ssh/id_rsa.pub",
    //                        "/home/username/.ssh/id_rsa",
    //                        password)) {
    //        fprintf(stderr, "\tAuthentication by public key failed\n");
    //        goto shutdown;
    //    }
    //}


	return true;
}



void XSsh::clear ()
{
	if (false == m_create_flag)
	{
		return;
	}
	m_create_flag = false;
	if(m_pSession) 
	{
        libssh2_session_disconnect(m_pSession, "Normal Shutdown, Thank you for playing");
        libssh2_session_free(m_pSession);
    }
	m_pSession = NULL;
    closesocket(m_sock);
	m_sock = -1;
    libssh2_exit();
}

bool XSsh::copy (TCHAR* p_local_file, TCHAR* p_remote_file)
{
	char local_file [MAX_PATH] = "";
	char remote_file [MAX_PATH] = "";
	LIBSSH2_CHANNEL* pChannel = NULL;

	w2m(p_local_file, local_file, _countof(local_file));
	w2m(p_remote_file, remote_file, _countof(remote_file));

	char mem[1024];
	size_t nread;
	char *ptr;
	struct stat FileInfo;
	int rc;

	FILE* pLocalFile = fopen(local_file, "rb");
	if (!pLocalFile) 
	{
		fprintf(stderr, "Can't open local file %s\n", local_file);
		return false;
	}

	stat(local_file, &FileInfo);

	/* Send a file via scp. The mode parameter must only have permissions! */
	pChannel = libssh2_scp_send(m_pSession, remote_file, FileInfo.st_mode & 0777, (unsigned long)FileInfo.st_size);
	if (!pChannel) 
	{
		//char *errmsg;
		//int errlen;
		//int err = libssh2_session_last_error(session, &errmsg, &errlen, 0);
		//fprintf(stderr, "Unable to open a session: (%d) %s\n", err, errmsg);
		//goto shutdown;
		return false;
	}

	fprintf(stderr, "SCP session waiting to send file\n");
	do {
		nread = fread(mem, 1, sizeof(mem), pLocalFile);
		if (nread <= 0) {
			/* end of file */
			break;
		}
		ptr = mem;

		do {
			/* write the same data over and over, until error or completion */
			rc = libssh2_channel_write(pChannel, ptr, nread);
			if (rc < 0) {
				fprintf(stderr, "ERROR %d\n", rc);
				break;
			}
			else {
				/* rc indicates how many bytes were written this time */
				ptr += rc;
				nread -= rc;
			}
		} while (nread);

	} while (1);

	fclose(pLocalFile);
	pLocalFile = NULL;

	fprintf(stderr, "Sending EOF\n");
	libssh2_channel_send_eof(pChannel);

	fprintf(stderr, "Waiting for EOF\n");
	libssh2_channel_wait_eof(pChannel);

	fprintf(stderr, "Waiting for channel to close\n");
	libssh2_channel_wait_closed(pChannel);

	libssh2_channel_free(pChannel);
	pChannel = NULL;

	return true;
}

bool XSsh::command (TCHAR* p_command, TCHAR* p_response, size_t response_len)
{
	char command [2048] = "";
	w2m(p_command, command, _countof(command));
    unsigned long hostaddr;
    int sock;
    struct sockaddr_in sin;
    const char *fingerprint;
    //LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
    int rc;
    int exitcode;
    char *exitsignal=(char *)"none";
    int bytecount = 0;
    size_t len;
    LIBSSH2_KNOWNHOSTS *nh;
    int type;
	int j=0;
	char iconv_buffer [90000] = "";

	/* Exec non-blocking on the remove host */
    while( (channel = libssh2_channel_open_session(m_pSession)) == NULL &&
           libssh2_session_last_error(m_pSession,NULL,NULL,0) ==
           LIBSSH2_ERROR_EAGAIN )
    {
        waitsocket(m_sock, m_pSession);
    }
    if( channel == NULL )
    {
        fprintf(stderr,"Error\n");
        exit( 1 );
    }
    while( (rc = libssh2_channel_exec(channel, command)) ==
           LIBSSH2_ERROR_EAGAIN )
    {
        waitsocket(sock, m_pSession);
    }
    if( rc != 0 )
    {
        fprintf(stderr,"Error\n");
        exit( 1 );
    }
    for( ;; )
    {
        /* loop until we block */
        int rc;
        do
        {
            char buffer[0x4000];
            rc = libssh2_channel_read( channel, buffer, sizeof(buffer) );
            if( rc > 0 )
            {
                int i;
                bytecount += rc;
                fprintf(stderr, "We read:\n");
                for( i=0; i < rc; ++i )
				{
                    fputc( buffer[i], stderr);
					iconv_buffer[j] = buffer[i]; ++j;
				}
				fprintf(stderr, "\n");
				iconv_buffer[j] = '\n'; ++j;
            }
            else {
                if( rc != LIBSSH2_ERROR_EAGAIN )
                    /* no need to output this for the EAGAIN case */
                    fprintf(stderr, "libssh2_channel_read returned %d\n", rc);
            }
        }
        while( rc > 0 );

        /* this is due to blocking that would occur otherwise so we loop on
           this condition */
        if( rc == LIBSSH2_ERROR_EAGAIN )
        {
            waitsocket(sock, m_pSession);
        }
        else
            break;
    }
    exitcode = 127;
    while( (rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN )
        waitsocket(sock, m_pSession);

    if( rc == 0 )
    {
        exitcode = libssh2_channel_get_exit_status( channel );
        libssh2_channel_get_exit_signal(channel, &exitsignal,
                                        NULL, NULL, NULL, NULL, NULL);
    }

    if (exitsignal)
        fprintf(stderr, "\nGot signal: %s\n", exitsignal);
    else 
        fprintf(stderr, "\nEXIT: %d bytecount: %d\n", exitcode, bytecount);

    libssh2_channel_free(channel);
    channel = NULL;

	XIconv Iconv("UTF-16LE", "UTF-8");
	size_t converted_len=0;
	TCHAR* p_string = Iconv.convert((TCHAR*)iconv_buffer, strlen(iconv_buffer), &converted_len);
	if (p_string)
	{
		if (p_response)
		{
			StringCchCopy(p_response, response_len, p_string);
		}	
		return false;
	}

	return true;
}

