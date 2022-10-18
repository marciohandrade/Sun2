               // SockUtil.cpp: implementation of the CSockUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "SockUtil.h"

#include <WINSOCK.H>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
//#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
int so_init()
{
	WORD wversionRequested;
	WSADATA wsadata;
	int err; 
	wversionRequested = MAKEWORD( 2, 2 ); 
	err = ::WSAStartup( wversionRequested, &wsadata );
	if ( err != 0 ) 
	{
		_LOG("WSAStartup Err");
		return -1;
	} 
	if ( LOBYTE( wsadata.wVersion ) != 2 ||
		HIBYTE( wsadata.wVersion ) != 2 ) 
	{
		_LOG("wVersion invalid");
		return -1; 
	} 
	return 0;
}

int  so_select(SOCKET sockFd, int nTime)
{
	int	ret;
	fd_set	rfdSet;
	struct timeval	timeout;

	FD_ZERO(&rfdSet);
	FD_SET(sockFd, &rfdSet);

	if (nTime > 0)
	{
		timeout.tv_sec = nTime;
		timeout.tv_usec = 0;

		ret = select(sockFd+1, &rfdSet, (fd_set *)0, (fd_set *)0, (struct timeval *)&timeout);
	}
	else
	{
		ret = select(sockFd+1, &rfdSet, (fd_set *)0, (fd_set *)0, (struct timeval *)NULL);
	}

	if( ret == 0 )	/* Timeout */
		return(0);
	else if( ret < 0 )
		return(-1);

	if( FD_ISSET(sockFd, &rfdSet) )
		return(1);
	else
		return(-1);
}

//////////////////////////////////////////////////////////////////////////
int  so_select_write(SOCKET sockFd, int nTime)
{
	int	ret;
	fd_set	wfdSet;
	struct timeval	timeout;

	FD_ZERO(&wfdSet);
	FD_SET(sockFd, &wfdSet);

	if (nTime > 0)
	{
		timeout.tv_sec = nTime;
		timeout.tv_usec = 0;

		ret = select(sockFd+1, (fd_set *)0, &wfdSet, (fd_set *)0, (struct timeval *)&timeout);
	}
	else
	{
		ret = select(sockFd+1, (fd_set *)0, &wfdSet, (fd_set *)0, (struct timeval *)NULL);
	}

	if( ret == 0 )	/* Timeout */
		return(0);
	else if( ret < 0 )
		return(-1);

	if( FD_ISSET(sockFd, &wfdSet) )
		return(1);
	else
		return(-1);
}

//////////////////////////////////////////////////////////////////////////
int  so_readn(SOCKET fd, char *ptrP, int nbytes)
{
	int	nleft, nread;
	nleft = nbytes;

	while (nleft > 0)
	{
		if ( (nread = recv(fd, ptrP, nleft,0)) <= 0)
			return(nread);
		nleft -= nread;
		ptrP += nread;
	}
	return(nbytes - nleft);
}

//////////////////////////////////////////////////////////////////////////
int  so_readline(SOCKET fd, char *ptr, int maxlen)
{
	int	n;
	int	rc;
	char	c;

	for(n=1; n<maxlen; n++)
	{
		if( (rc = recv(fd, &c, 1,0)) == 1 )
		{
			*ptr++ = c;
			if( c == '\n' )
				break;
		}
		else if( rc == 0 )
		{
			if( n == 1 )
				return(0);
			else
				break;
		}
		else
			return(-1);
	}

	*ptr = 0;
	return(n);
}

//////////////////////////////////////////////////////////////////////////
int  so_readline_timeout(SOCKET fd, char *ptr, int maxlen, int nTime)
{
	int	n;
	int	rc;
	char	c;

	for(n=1; n<maxlen; n++)
	{
		if( so_select(fd, nTime) <= 0 )
		{
			if( n == 1)
				return(-1);
			else
				return(n-1);
		}

		if( (rc = recv(fd, &c, 1, 0)) == 1 )
		{
			*ptr++ = c;
			if( c == '\n' )
				break;
		}
		else if( rc == 0 )
		{
			if( n == 1 )
				return(0);
			else
				break;
		}
		else
			return(-1);
	}

	*ptr = 0;
	return(n);
}

//////////////////////////////////////////////////////////////////////////
int  so_readn_timeout(SOCKET fd, char *ptrP, int nbytes, int nTime)
{
	int	nleft, nread;
	nleft = nbytes;

	while (nleft > 0)
	{
		if( so_select(fd, nTime) <= 0 )
		{
			if( nleft == nbytes )
				return(-1);
			else
				return(nbytes - nleft);
		}
		if ( (nread = recv(fd, ptrP, nleft, 0)) <= 0)
			return(nread);
		nleft -= nread;
		ptrP += nread;
	}
	return(nbytes - nleft);
}

//////////////////////////////////////////////////////////////////////////
char*  recv_message(SOCKET fd, int nTime)
{
	char *pStr = NULL;

	uint16_t len;
	unsigned short size;

	if( so_select(fd, nTime) <= 0 )
		return NULL;
	else
		if( recv(fd, (char*)&len, sizeof(uint16_t), 0) <= 0 )
			return NULL;

	size = ntohs(len);

	pStr = (char *)calloc(size+1, sizeof(char));

	if( so_readn_timeout(fd, pStr, size, nTime) != size )
	{
		FREE_MEM(pStr);
		return NULL;
	}

	return pStr;
}

//////////////////////////////////////////////////////////////////////////
int  send_message(SOCKET fd, char *res)
{
	uint16_t len = htons(strlen(res));

	if( send(fd, (char*)&len, sizeof(uint16_t), 0) <= 0 )
		return(-1);

	return(so_writen(fd, res, strlen(res)));
}

//////////////////////////////////////////////////////////////////////////
int  so_writen(SOCKET fd, char *ptrP, int nbytes)
{
	int	nleft, nwritten;
	int	ntry = 0;

	nleft = nbytes;
	while (nleft > 0 && ntry < 5)
	{
		nwritten = send(fd, ptrP, nleft, 0);
		if (nwritten <= 0)
		{
			ntry++;
			if (nwritten == 0)
				continue;

			if (WSAGetLastError() == WSAEWOULDBLOCK )
				continue;
			else
				return(-1);
		}
		nleft -= nwritten;
		ptrP += nwritten;
	}
	return(nbytes - nleft);
}

//////////////////////////////////////////////////////////////////////////
/*int  getRandomPort(int start, int num)
{

	_tzset();
	int	pidForRandom;
	struct	timeval		tp;
//	struct	_timezone	tz;
	
	
	if (num < 1)
		return(-1);
	if (num == 1)
		return(start);
	
	gettimeofday(&tp, &tz);
	pidForRandom = getpid();
	
	return(start + ((tp.tv_sec + pidForRandom) % num));
	return 0;
}
*/
//////////////////////////////////////////////////////////////////////////
/*
int  connect_nonblock(SOCKET sockfd, const struct sockaddr *ptrSaddr, int nSaddrlen, int nTimes)
{
	
	int n;
	int flags;
	int error;
	int len;
	fd_set rset;
	fd_set wset;
	struct timeval tval;
	
//	if( (flags = getsockopt(sockfd, F_GETFL, 0)) < 0 )
//	{
//		// fcntl() error 
//	}
//	
//	fcntl(sockfd, F_SETFL, flags|O_NONBLOCK);

	ULONG nonBlk = TRUE;
	ioctlsocket( sockfd, FIONBIO, &nonBlk );
	
	error = 0;
	if( (n = connect(sockfd, (struct sockaddr *)ptrSaddr, nSaddrlen)) == -1 )
//		if( n == 0 )
//			goto DONE;
//		else if( _errno != WSAEINPROGRESS )
//			return(-1);
		
		
		FD_ZERO(&rset);
		FD_SET(sockfd, &rset);
		wset = rset;
		
		tval.tv_sec = nTimes;
		tval.tv_usec = 0;
		
		if( (n = select(sockfd+1, &rset, &wset, NULL, nTimes ? &tval : NULL)) == 0 )
		{
			error = WSAETIMEDOUT;
			goto DONE;
		}
		
		if( FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset) )
		{
			len = sizeof(error);
			if( getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&error, &len) == -1 )
			{
				error = WSAETIMEDOUT;
				goto DONE;
			}
		}
		else
		{
			error = -1;
			goto DONE;
		}
		
		
DONE:
//		if( fcntl(sockfd, F_SETFL, flags) < 0 )
//			;	// fcntl() error 
		ioctlsocket( sockfd, FIONBIO, &nonBlk );

			if( error )
			{
				closesocket(sockfd);
				_errno = error;
				return(-1);
			}
			
			return(0);
}
*/

//////////////////////////////////////////////////////////////////////////
void*  cioReceiveMessage(SOCKET& sockfd, int* Error, int* length)
{
//	return ReceiveNetMessage(Error, length);
	int l;
	int TickCount = 0;
	*Error = 0;
	char* buf = new char[*length];
	char* tempbuf = buf;
	int recl = 0;
	do
	{
		l = recv(sockfd, tempbuf, *length-recl, 0);
		if (l==SOCKET_ERROR || l==0) 
		{
			*Error = 1;
			delete buf;
			return NULL;
		};
		recl+=l;
		tempbuf+=l;
		if (recl<*length) 
		{
			TickCount+=1;
			Sleep(50);//retry to receive every 10 miliseconds
		};
	}while(recl<*length && TickCount<2000);
	*length = recl;
	return buf;
}

//##########################################################################

u_long  get_netaddr( const char* address )
{
    struct hostent* host;
    const char*     ptr;

    /* First try it as aaa.bbb.ccc.ddd. */
    for( ptr = address; *ptr && (isdigit(*ptr) || *ptr == '.'); ptr++ );
    if( *ptr == 0 ) return( inet_addr( address ) );

    if( (host = gethostbyname( address )) )
        return( ((struct in_addr *)*host->h_addr_list)->s_addr );

    return( INADDR_NONE );
}

u_short  get_netport( const char* service, const char* proto )
{
    const char*     ptr;
    struct servent* serv;

    /* First try it as a number */
    for( ptr = service; *ptr && isdigit(*ptr); ptr++ );
    if( *ptr == 0 ) return( htons( (u_short)atoi( service ) ) );

    /* Try to read it from services database */
    if( (serv = getservbyname( service, proto )) ) return( serv->s_port );

    return( (u_short)-1 );
}

char*  get_peer_addr( SOCKET sock, char* buf, size_t len )
{
    struct sockaddr_in addr;
    int                addr_len;
    int                result;

    addr_len = sizeof( addr );
    result = getpeername( sock, (struct sockaddr FAR*)&addr, &addr_len );

    if( result == SOCKET_ERROR ) return( NULL );

    if( len > 0 )
    {
        strncpy( buf, inet_ntoa( addr.sin_addr ), len - 1 );
        buf[len - 1] = 0;
    }
    return( buf );
}

char*  get_peer_hostname( SOCKET sock, char* buf, size_t len )
{
    struct sockaddr_in addr;
    int                addr_len;
    struct hostent*    phe;
    int                result;

    addr_len = sizeof( addr );
    result = getpeername( sock, (struct sockaddr FAR*)&addr, &addr_len );

    if( result == SOCKET_ERROR ) return( NULL );

    phe = gethostbyaddr( (const char FAR*)&addr.sin_addr.s_addr,
                         sizeof(addr.sin_addr.s_addr),
                         addr.sin_family );

    if( !phe ) return( NULL );

    if( len > 0 )
    {
        if( phe->h_name )
        {
            strncpy( buf, phe->h_name, len - 1 );
            buf[len - 1] = 0;
        }
        else
        {
            strncpy( buf, inet_ntoa( addr.sin_addr ), len - 1 );
        }
    }
    return( buf );
}

int  get_peer_port( SOCKET sock )
{
    struct sockaddr_in addr;
    int                addr_len;
    int                result;
    int                port;

    addr_len = sizeof( addr );
    result = getpeername( sock, (struct sockaddr FAR*)&addr, &addr_len );

    if( result == SOCKET_ERROR ) return( SOCKET_ERROR );

    port = ntohs( addr.sin_port );

    return( port );
}

char*  get_sock_addr( SOCKET sock, char* buf, size_t len )
{
    struct sockaddr_in addr;
    int                addr_len;
    int                result;

    addr_len = sizeof( addr );
    result = getsockname( sock, (struct sockaddr FAR*)&addr, &addr_len );

    if( result == SOCKET_ERROR ) return( NULL );

    if( len > 0 )
    {
        strncpy( buf, inet_ntoa( addr.sin_addr ), len - 1 );
        buf[len - 1] = 0;
    }
    return( buf );
}

char*  get_sock_hostname( SOCKET sock, char* buf, size_t len )
{
    struct sockaddr_in addr;
    int                addr_len;
    struct hostent*    phe;
    int                result;

    addr_len = sizeof( addr );
    result = getsockname( sock, (struct sockaddr FAR*)&addr, &addr_len );

    if( result == SOCKET_ERROR ) return( NULL );

    phe = gethostbyaddr( (const char FAR*)&addr.sin_addr.s_addr,
                         sizeof(addr.sin_addr.s_addr),
                         addr.sin_family );

    if( !phe ) return( NULL );

    if( len > 0 )
    {
        if( phe->h_name )
        {
            strncpy( buf, phe->h_name, len - 1 );
            buf[len - 1] = 0;
        }
        else
        {
            strncpy( buf, inet_ntoa( addr.sin_addr ), len - 1 );
        }
    }
    return( buf );
}

int  get_sock_port( SOCKET sock )
{
    struct sockaddr_in addr;
    int                addr_len;
    int                result;
    int                port;

    addr_len = sizeof( addr );
    result = getsockname( sock, (struct sockaddr FAR*)&addr, &addr_len );

    if( result == SOCKET_ERROR ) return( SOCKET_ERROR );

    port = ntohs( addr.sin_port );

    return( port );
}

int  set_nonblk_mode( SOCKET sock, u_long mode )
{
    return( ioctlsocket( sock, FIONBIO, &mode ) );
}

SOCKET  sock_accept( SOCKET listen_sock, int* p_timeout )
{
    int rs;
    if( p_timeout && (rs = sock_wait( listen_sock, *p_timeout, TRUE )) <= 0 )
    {
        if( rs == 0 ) *p_timeout = -1;
        return( INVALID_SOCKET );
    }
    return( accept( listen_sock, NULL, NULL ) );
}

int  sock_close( SOCKET sock )
{
    return( closesocket( sock ) );
}

SOCKET  sock_connect( const char* address, u_short port, int type, int* p_timeout )
{
    struct sockaddr_in addr;
    SOCKET             connect_sock;

    ZeroMemory( (PVOID)&addr, sizeof(addr) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( port );
    if( (addr.sin_addr.s_addr = get_netaddr( address )) == INADDR_NONE )
        return( INVALID_SOCKET );

    if( (connect_sock = socket( AF_INET, type, 0 )) == INVALID_SOCKET )
        return( INVALID_SOCKET );

    if( type == SOCK_STREAM )
    {
        if( p_timeout )
        {
            fd_set         fds_error;
            fd_set         fds_write;
            int            rs = -1;
            struct timeval tv;

            tv.tv_sec  = *p_timeout;
            tv.tv_usec = 0;

            FD_ZERO( &fds_write );
            FD_SET( connect_sock, &fds_write );
            FD_ZERO( &fds_error );
            FD_SET( connect_sock, &fds_error );

            if( set_nonblk_mode( connect_sock, 1 ) ||
                ( connect( connect_sock, (struct sockaddr *) &addr, sizeof(addr) ) == SOCKET_ERROR &&
                  ( (rs = select( 0, NULL, &fds_write, &fds_error, &tv )) <= 0 ||
                    !FD_ISSET( connect_sock, &fds_write ) ) ) ||
                set_nonblk_mode( connect_sock, 0 ) )
            {
                int last_error = WSAGetLastError();
                closesocket( connect_sock );
                if( rs == 0 ) *p_timeout = -1;
                WSASetLastError( last_error );
                return( INVALID_SOCKET );
            }
        }
        else if( connect( connect_sock, (struct sockaddr *) &addr, sizeof(addr) ) == SOCKET_ERROR )
        {
            int last_error = WSAGetLastError();
            closesocket( connect_sock );
            WSASetLastError( last_error );
            return( INVALID_SOCKET );
        }
        return( connect_sock );
    }
    /* Otherwise, must be for udp, so bind to address. */
    if( bind( connect_sock, (struct sockaddr *) &addr, sizeof(addr) ) == SOCKET_ERROR )
    {
        int last_error = WSAGetLastError();
        closesocket( connect_sock );
        WSASetLastError( last_error );
        return( INVALID_SOCKET );
    }
    return( connect_sock );
}

int  sock_init( LPWSADATA lpWSAData )
{
    WSADATA WSAData;
    if( !lpWSAData ) lpWSAData = &WSAData;
    return( WSAStartup( MAKEWORD(2,2), lpWSAData ) );
}

SOCKET  sock_listen( const char* address, u_short port, int type, int backlog )
{
    struct sockaddr_in addr;
    SOCKET             listen_sock;
    int                reuse_addr = 1;

    /* Setup internet address information.
       This is used with the bind() call */
    ZeroMemory( (PVOID)&addr, sizeof(addr) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( port );
    if( address )
    {
        if( (addr.sin_addr.s_addr = get_netaddr( address )) == INADDR_NONE )
            return( INVALID_SOCKET );
    }
    else
    {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    if( (listen_sock = socket( AF_INET, type, 0 )) == INVALID_SOCKET )
        return( INVALID_SOCKET );

    setsockopt( listen_sock, SOL_SOCKET, SO_REUSEADDR,
                (char*) &reuse_addr, sizeof(reuse_addr) );

    if( bind( listen_sock, (struct sockaddr *) &addr,
              sizeof(addr) ) == SOCKET_ERROR )
    {
        int last_error = WSAGetLastError();
        closesocket( listen_sock );
        WSASetLastError( last_error );
        return( INVALID_SOCKET );
    }
    if( type == SOCK_STREAM && listen( listen_sock, backlog ) == SOCKET_ERROR )
    {
        int last_error = WSAGetLastError();
        closesocket( listen_sock );
        WSASetLastError( last_error );
        return( INVALID_SOCKET );
    }
    return( listen_sock );
}

int  sock_read( SOCKET sock, char* buf, size_t count )
{
	int	nleft, nread;
	nleft = count;

	while (nleft > 0)
	{
		if ( (nread = recv(sock, buf, nleft,0)) <= 0)
			return(nread);
		nleft -= nread;
		buf += nread;
	}
	return(count - nleft);

 /*
    size_t bytes_read = 0;
     int    rs;
 
     if( stop_byte == -1 )
     {
         if( p_timeout && (rs = sock_wait( sock, *p_timeout, TRUE )) <= 0 )
         {
             if( rs == 0 ) *p_timeout = -1;
             return( SOCKET_ERROR );
         }
         bytes_read = recv( sock, buf, count, 0 );
         return( bytes_read >= 0 ? bytes_read : SOCKET_ERROR );
     }
     else
     {
         int this_read;
 
         while( bytes_read < count )
         {
             if( p_timeout && (rs = sock_wait( sock, *p_timeout, TRUE )) <= 0 )
             {
                 if( rs == 0 ) *p_timeout = -1;
                 return( SOCKET_ERROR );
             }
             this_read = recv( sock, buf, 1, 0 );
             if( this_read == 0 ) return( bytes_read );
             if( this_read == SOCKET_ERROR ) return( SOCKET_ERROR );
             bytes_read++;
             if( *(buf++) == (char)stop_byte ) return( bytes_read );
         }
     }
     return( count );*/
 
}


int  sock_send( SOCKET sock, const char *buf, size_t count )
{
    size_t bytes_sent = 0;
//    int    rs;
    int    this_send;

    while( bytes_sent < count )
    {
//        if( p_timeout && (rs = sock_wait( sock, *p_timeout, FALSE )) <= 0 )
//        {
//            if( rs == 0 ) *p_timeout = -1;
//            return( SOCKET_ERROR );
//        }
        this_send = send( sock, buf, count - bytes_sent, 0 );
        if( this_send == 0 ) return( bytes_sent );
        if( this_send == SOCKET_ERROR ) return( SOCKET_ERROR );

        bytes_sent += this_send;
        buf += this_send;
    }
    return( count );
}

int  sock_uninit()
{
    return( WSACleanup() );
}

int  sock_wait( SOCKET sock, int timeout, int for_read )
{
    fd_set         fds_test;
    int            rstatus;
    struct timeval tv;

    tv.tv_sec  = timeout;
    tv.tv_usec = 0;

    FD_ZERO( &fds_test );
    FD_SET( sock, &fds_test );

    if( for_read )
        rstatus = select( 0, &fds_test, NULL, NULL, &tv );
    else
        rstatus = select( 0, NULL, &fds_test, NULL, &tv );

    if( rstatus == SOCKET_ERROR )
        return( SOCKET_ERROR );
    if( rstatus == 0 || !FD_ISSET( sock, &fds_test ) )
        return( 0 );
    return( 1 );
}



int sock_read( SOCKET sock, char* buf, size_t count, int* p_timeout )
{
	int	nleft, nread;
	int    rs;
	nleft = count;
	
	while(nleft > 0)
	{
		if( p_timeout && (rs = sock_wait( sock, *p_timeout, TRUE )) <= 0 )
		{
			if( rs == 0 ) *p_timeout = -1;
			return( SOCKET_ERROR );
		}
		
		if ( (nread = recv(sock, buf, nleft,0)) <= 0)
			return(nread);
		nleft -= nread;
		buf += nread;
	}
	return(count - nleft);
}

int sock_send( SOCKET sock, const char *buf, size_t count, int* p_timeout )
{
    size_t bytes_sent = 0;
    int    rs;
    int    this_send;

    while( bytes_sent < count )
    {
        if( p_timeout && (rs = sock_wait( sock, *p_timeout, FALSE )) <= 0 )
        {
            if( rs == 0 ) *p_timeout = -1;
            return( SOCKET_ERROR );
        }
        this_send = send( sock, buf, count - bytes_sent, 0 );
        if( this_send == 0 ) return( bytes_sent );
        if( this_send == SOCKET_ERROR ) return( SOCKET_ERROR );

        bytes_sent += this_send;
        buf += this_send;
    }
    return( count );
}

BOOL sendCommand(char* ip, USHORT port, int* p_timeout, 
		 const char* pSendBuffer, int nSendSize, 
		 char* pRecvBuffer, int nRecvSize )
{
	SOCKET hSocket = sock_connect(ip, port, SOCK_STREAM, p_timeout);
	if(INVALID_SOCKET == hSocket)
		return FALSE;
	
	if(SOCKET_ERROR == sock_send(hSocket, pSendBuffer, nSendSize, p_timeout) )
	{
		sock_close(hSocket);
		return FALSE;
	}

	if( SOCKET_ERROR == sock_read(hSocket, pRecvBuffer, nRecvSize, p_timeout) )
	{
		sock_close(hSocket);
		return FALSE;
	}
	
	sock_close(hSocket);
	return TRUE;
}

//#########################################################################################33
/*

//////////////////////////////////////////////////////////////////////////
// nonBlock_connect, Block_SendRecv

char*  processIO_nbConnect_bSendRecv( const char* address, u_short port, int nTimeout,
						 const char* szSendData, int& nError)
{

	if(strlen(szSendData) <= 0)
	{
//		_LOG("No Data to send !");
		nError = ERR_SENDDATA_SIZE;
		return NULL;
	}

	nError = ERR_UNKNOWN;
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// winsock startup
	
	if (0 != SockInit())
	{
		nError = ERR_SOCK_STARTUP;
		return NULL;
	}
	
	//////////////////////////////////////////////////////////////////////////
	char* pStr = NULL;
	int	nByteLeft, nByteToRecv, nByteToSend, nByteSent, nByteRecved;
	uint16_t nByteOfData = 0;
	int result = -1;
	u_long option;
		
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// socket creation

	SOCKET client = ::socket(AF_INET, SOCK_STREAM,0);
	if (INVALID_SOCKET == client)
	{
//		_LOG("socket creation fail");
		nError = ERR_SOCK_CREATION;
		goto DONE;
	}
	
	// change to nonBlock
	option = 1;
	result = ioctlsocket(client, FIONBIO, &option);
	if (SOCKET_ERROR == result)
	{
//		_LOG("ioctl fail : %d", ::GetLastError() );
		nError = ERR_SOCK_IOCTL;
		goto DONE;
	}
	
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// connect
	struct timeval tv;
	tv.tv_sec  = nTimeout;
	tv.tv_usec = 0;

	fd_set wfd, efd;
	
	// connect
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(address);		// local machine address
	addr.sin_port = htons(port);
	result = ::connect(client, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (SOCKET_ERROR == result)
	{
		if (WSAEWOULDBLOCK != WSAGetLastError())
		{
			nError = ERR_SOCK_CONNECT;
			goto DONE;
		}
		else
		{
			// wait for connet result
			FD_ZERO(&wfd);
			FD_ZERO(&efd);
			FD_SET(client, &wfd);
			FD_SET(client, &efd);

			//  The select function returns the total number of socket handles that are ready and contained 
			// in the fd_set structures, zero if the time limit expired, or 
			// SOCKET_ERROR if an error occurred. If the return value is SOCKET_ERROR, 
			// WSAGetLastError can be used to retrieve a specific error code.
			if( select(0, 0, &wfd, &efd,  &tv) <= 0)
		//	if (SOCKET_ERROR == result || 0 == rfd.fd_count)
			{
//				_LOG("connection fail");
				nError = ERR_SOCK_CONNECT;
				goto DONE;
			}
		}
	}

	_LOG("connection established (%s:%d)", address, port);

	//
	// change to Blocking mode

	option = 0;
	result = ioctlsocket(client, FIONBIO, &option);

	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// send 

	//----------------------------------------------
	// send nByteOfData
	nByteOfData = htons(strlen(szSendData));
	
	
	result = ::send(client, (char*)&nByteOfData, sizeof(uint16_t), 0);
	if (SOCKET_ERROR == result)
	{
//		_LOG("send error : %d", ::WSAGetLastError() );
		nError = ERR_SOCK_SEND;
		goto DONE;
	}

	_LOG("sent nByteOfData [%d]", result);
	//----------------------------------------------
	// send Data

	nByteToSend = strlen(szSendData);
	nByteSent = 0;
	do
	{
		result = ::send(client, &szSendData[nByteSent], nByteToSend - nByteSent, 0);
		if (SOCKET_ERROR == result)
		{
//			_LOG("send error : %d", ::WSAGetLastError() );
			nError = ERR_SOCK_SEND;
			goto DONE;
		}
		else
		{
			nByteSent += result;
		}
	}
	while(nByteSent != nByteToSend);

//	_LOG("sent Data [%d]", nByteSent);

	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// recv
	
	//----------------------------------------------------------
	// recv nByteOfData

	if( recv(client, (char*)&nByteOfData, sizeof(uint16_t), 0) <= 0 )
	{
//		_LOG("recv error : %d", ::WSAGetLastError() );
		nError = ERR_SOCK_RECV;
		FREE_MEM(pStr);
		goto DONE;		
	}

	//----------------------------------------------------------
	// recv Data
	
	nByteToRecv = ntohs(nByteOfData);
	if(nByteToRecv <= 0 || nByteToRecv > MAX_RECVDATA_LEN)
	{
		nError = ERR_RECVDATA_SIZE;
		goto DONE;
	}

//	_LOG("nByteToRecv (%d)", nByteOfData);
	pStr = (char *)calloc(nByteToRecv+1, sizeof(char));

	nByteLeft = nByteToRecv;
	nByteRecved = 0;
	//--------------------------------------------

	do
	{
		result = ::recv(client, &pStr[nByteRecved], nByteLeft, 0);
		if (result <= 0 )
		{
//			_LOG("recv error : %d", ::WSAGetLastError() );
			nError = ERR_SOCK_RECV;
			FREE_MEM(pStr);
			goto DONE;			
		}
		else
		{			
			nByteLeft -= result;
			nByteRecved += result;
		}
	}
	while(nByteLeft > 0);

//	_LOG(pStr);
	
	closesocket(client);
	::WSACleanup();
	
	return pStr;
	//-------------------------------------------
DONE:

	if (INVALID_SOCKET != client)
	{
		closesocket(client);
	}

	::WSACleanup();
	return NULL;	

}

//////////////////////////////////////////////////////////////////////////
// nonBlock_connect, SendRecv

char*  processIO_nbConnect_nbSendRecv( const char* address, u_short port, int nTimeout,
						 const char* szSendData, int& nError)
{

	if(strlen(szSendData) <= 0)
	{
		nError = ERR_SENDDATA_SIZE;
		return NULL;
	}

	nError = ERR_UNKNOWN;
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// winsock startup
	
	if (0 != SockInit())
	{
		nError = ERR_SOCK_STARTUP;
		return NULL;
	}
	
	//////////////////////////////////////////////////////////////////////////
	char* pStr = NULL;
	int	nByteLeft, nByteToRecv, nByteToSend, nByteSent, nByteRecved;
	uint16_t nByteOfData = 0;
	int result = -1;
	u_long option;
		
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// socket creation

	SOCKET client = ::socket(AF_INET, SOCK_STREAM,0);
	if (INVALID_SOCKET == client)
	{
		nError = ERR_SOCK_CREATION;
		goto DONE;
	}
	
	// Release for reuse this port by another users or processes...
//	const int on = 1;
//	setsockopt(client, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));

	option = 1;
	result = ioctlsocket(client, FIONBIO, &option);
	if (SOCKET_ERROR == result)
	{
		nError = ERR_SOCK_IOCTL;
		goto DONE;
	}
	
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// connect
	struct timeval tv;
	tv.tv_sec  = nTimeout;
	tv.tv_usec = 0;

	fd_set rfd, wfd, efd;
	
	// connect
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(address);		// local machine address
	addr.sin_port = htons(port);
	result = ::connect(client, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (SOCKET_ERROR == result)
	{
		if (WSAEWOULDBLOCK != WSAGetLastError())
		{
			nError = ERR_SOCK_CONNECT;
			goto DONE;
		}
		else
		{
			FD_ZERO(&wfd);
			FD_ZERO(&efd);
			FD_SET(client, &wfd);
			FD_SET(client, &efd);

			//  The select function returns the total number of socket handles that are ready and contained 
			// in the fd_set structures, zero if the time limit expired, or 
			// SOCKET_ERROR if an error occurred. If the return value is SOCKET_ERROR, 
			// WSAGetLastError can be used to retrieve a specific error code.
			if( (select(0, 0, &wfd, &efd,  &tv) <= 0) ||
				 !FD_ISSET( client, &wfd ) ) 
		//	if (SOCKET_ERROR == result || result//0 == rfd.fd_count)
			{
				nError = ERR_SOCK_CONNECT;
				goto DONE;
			}
		}
	}

//	option = 0;
//	result = ioctlsocket(client, FIONBIO, &option);

	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// send 
	
	//----------------------------------------------
	// send nByteOfData
	nByteOfData = htons(strlen(szSendData));
	result = ::send(client, (char*)&nByteOfData, sizeof(uint16_t), 0);
	if (SOCKET_ERROR == result)
	{
		nError = ERR_SOCK_SEND;
		goto DONE;				
	}

	//----------------------------------------------
	// send Data
	nByteToSend = strlen(szSendData);
	nByteSent = 0;
	do
	{
		FD_ZERO( &wfd );
		FD_SET( client, &wfd );
		if( ( select( 0, NULL, &wfd, NULL, &tv ) <= 0) ||
			!FD_ISSET(client, &wfd) )
		{
			nError = ERR_SOCK_SEND;
			goto DONE;				
		}
			
		result = ::send(client, &szSendData[nByteSent], nByteToSend - nByteSent, 0);
		if (SOCKET_ERROR == result)
		{
			nError = ERR_SOCK_SEND;
			goto DONE;				
		}
		else
		{
			nByteSent += result;
		}
	}
	while(nByteSent != nByteToSend);

	
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// recv
	
	//----------------------------------------------------------
	// recv nByteOfData
	FD_ZERO(&rfd);
	FD_SET(client, &rfd);
	
	if( (select(0, &rfd, 0, 0, &tv) == SOCKET_ERROR) ||
		!FD_ISSET( client, &rfd ) )
	{
		nError = ERR_SOCK_RECV;
		goto DONE;
	}	
	
	if( recv(client, (char*)&nByteOfData, sizeof(uint16_t), 0) <= 0 )
	{
		nError = ERR_SOCK_RECV;
		goto DONE;
	}

	
	//----------------------------------------------------------
	// recv Data
	
	nByteToRecv = ntohs(nByteOfData);
//	_LOG("###############  READ BYTESIZE : (%d)", nByteToRecv);

	if(nByteToRecv <= 0 || nByteToRecv > MAX_RECVDATA_LEN)
	{
		nError = ERR_RECVDATA_SIZE;
		goto DONE;
	}

	pStr = (char *)calloc(nByteToRecv+1, sizeof(char));
	
	nByteLeft = nByteToRecv;
	nByteRecved = 0;
	//--------------------------------------------

	do
	{
		FD_ZERO(&rfd);
		FD_SET(client, &rfd);
		if((select(0, &rfd, 0, 0, &tv) <= 0 ) ||
			!FD_ISSET(client, &rfd) )
		{
			nError = ERR_SOCK_RECV;
			FREE_MEM(pStr);
			goto DONE;				
		}

		result = ::recv(client, &pStr[nByteRecved], nByteLeft, 0);
		if (result <= 0 )
		{
			nError = ERR_SOCK_RECV;
			FREE_MEM(pStr);
			goto DONE;		
		}
		else
		{			
			nByteLeft -= result;
			nByteRecved += result;
		}

	//	_LOG("###############  READ ByteLeft : (%d)", nByteLeft);

	}
	while(nByteLeft > 0);

//	_LOG("###############  READ DATA : (%s)", pStr);

//	_LOG("ByteSent(%d), ByteRecved(%d)", nByteSent, nByteToRecv);
	
	nError = ERR_SUCCESS;
	
	if (INVALID_SOCKET != client)
	{
		closesocket(client);
	}

	::WSACleanup();
	
	return pStr;

	//-------------------------------------------
DONE:
//	_LOG("DONE : ###############  READ DATA : (%s)", pStr);

	if (INVALID_SOCKET != client)
		closesocket(client);
	
	::WSACleanup();
	return NULL;
}

char*  processNonBlockIO( const char* address, u_short port, int nTimeout,
						 const char* szSendData, int& nError)
{

	if(strlen(szSendData) <= 0)
	{
		nError = ERR_SENDDATA_SIZE;
		return NULL;
	}

	nError = ERR_UNKNOWN;
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// winsock startup
	
	if (0 != SockInit())
	{
		nError = ERR_SOCK_STARTUP;
		return NULL;
	}
	
	//////////////////////////////////////////////////////////////////////////
	char* pStr = NULL;
	int	nByteLeft, nByteToRecv, nByteToSend, nByteSent, nByteRecved;
	uint16_t nByteOfData = 0;
	int result = -1;
	u_long option;
		
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// socket creation

	SOCKET client = ::socket(AF_INET, SOCK_STREAM,0);
	if (INVALID_SOCKET == client)
	{
		nError = ERR_SOCK_CREATION;
		goto DONE;
	}
	
	// Release for reuse this port by another users or processes...
//	const int on = 1;
//	setsockopt(client, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));

	option = 1;
	result = ioctlsocket(client, FIONBIO, &option);
	if (SOCKET_ERROR == result)
	{
		nError = ERR_SOCK_IOCTL;
		goto DONE;
	}
	
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// connect
	struct timeval tv;
	tv.tv_sec  = nTimeout;
	tv.tv_usec = 0;

	fd_set rfd, wfd, efd;
	
	// connect
	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(address);		// local machine address
	addr.sin_port = htons(port);
	result = ::connect(client, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (SOCKET_ERROR == result)
	{
		if (WSAEWOULDBLOCK != WSAGetLastError())
		{
			nError = ERR_SOCK_CONNECT;
			goto DONE;
		}
		else
		{
			FD_ZERO(&wfd);
			FD_ZERO(&efd);
			FD_SET(client, &wfd);
			FD_SET(client, &efd);

			//  The select function returns the total number of socket handles that are ready and contained 
			// in the fd_set structures, zero if the time limit expired, or 
			// SOCKET_ERROR if an error occurred. If the return value is SOCKET_ERROR, 
			// WSAGetLastError can be used to retrieve a specific error code.
			if( (select(0, 0, &wfd, &efd,  &tv) <= 0) ||
				 !FD_ISSET( client, &wfd ) ) 
		//	if (SOCKET_ERROR == result || result//0 == rfd.fd_count)
			{
				nError = ERR_SOCK_CONNECT;
				goto DONE;
			}
		}
	}

//	option = 0;
//	result = ioctlsocket(client, FIONBIO, &option);

	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// send 
	
	//----------------------------------------------
	// send nByteOfData
//	nByteOfData = htons(strlen(szSendData));
//	result = ::send(client, (char*)&nByteOfData, sizeof(uint16_t), 0);
//	if (SOCKET_ERROR == result)
//	{
//		nError = ERR_SOCK_SEND;
//		goto DONE;				
//	}

	//----------------------------------------------
	// send Data
	nByteToSend = strlen(szSendData);
	nByteSent = 0;
	do
	{
		FD_ZERO( &wfd );
		FD_SET( client, &wfd );
		if( ( select( 0, NULL, &wfd, NULL, &tv ) <= 0) ||
			!FD_ISSET(client, &wfd) )
		{
			nError = ERR_SOCK_SEND;
			goto DONE;				
		}
			
		result = ::send(client, &szSendData[nByteSent], nByteToSend - nByteSent, 0);
		if (SOCKET_ERROR == result)
		{
			nError = ERR_SOCK_SEND;
			goto DONE;				
		}
		else
		{
			nByteSent += result;
		}
	}
	while(nByteSent != nByteToSend);

	
	//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
	// recv
	
	//----------------------------------------------------------
	// recv nByteOfData
	FD_ZERO(&rfd);
	FD_SET(client, &rfd);
	
	if( (select(0, &rfd, 0, 0, &tv) == SOCKET_ERROR) ||
		!FD_ISSET( client, &rfd ) )
	{
		nError = ERR_SOCK_RECV;
		goto DONE;
	}	
	
	if( recv(client, (char*)&nByteOfData, sizeof(uint16_t), 0) <= 0 )
	{
		nError = ERR_SOCK_RECV;
		goto DONE;
	}

	
	//----------------------------------------------------------
	// recv Data
	
	nByteToRecv = ntohs(nByteOfData);
//	_LOG("###############  READ BYTESIZE : (%d)", nByteToRecv);

	if(nByteToRecv <= 0 || nByteToRecv > MAX_RECVDATA_LEN)
	{
		nError = ERR_RECVDATA_SIZE;
		goto DONE;
	}

	pStr = (char *)calloc(nByteToRecv+1, sizeof(char));
	
	nByteLeft = nByteToRecv;
	nByteRecved = 0;
	//--------------------------------------------

	do
	{
		FD_ZERO(&rfd);
		FD_SET(client, &rfd);
		if((select(0, &rfd, 0, 0, &tv) <= 0 ) ||
			!FD_ISSET(client, &rfd) )
		{
			nError = ERR_SOCK_RECV;
			FREE_MEM(pStr);
			goto DONE;				
		}

		result = ::recv(client, &pStr[nByteRecved], nByteLeft, 0);
		if (result <= 0 )
		{
			nError = ERR_SOCK_RECV;
			FREE_MEM(pStr);
			goto DONE;		
		}
		else
		{			
			nByteLeft -= result;
			nByteRecved += result;
		}

	//	_LOG("###############  READ ByteLeft : (%d)", nByteLeft);

	}
	while(nByteLeft > 0);

//	_LOG("###############  READ DATA : (%s)", pStr);

//	_LOG("ByteSent(%d), ByteRecved(%d)", nByteSent, nByteToRecv);
	
	nError = ERR_SUCCESS;
	
	if (INVALID_SOCKET != client)
	{
		closesocket(client);
	}

	::WSACleanup();
	
	return pStr;

	//-------------------------------------------
DONE:
//	_LOG("DONE : ###############  READ DATA : (%s)", pStr);

	if (INVALID_SOCKET != client)
		closesocket(client);
	
	::WSACleanup();
	return NULL;
}


char*  processIO_nbConnect_nbSendRecv_util( const char* address, u_short port, int nTimeout,
													 const char* szSendData, int& nError)
{
	
	nError = ERR_UNKNOWN;
	
	WSADATA       wsd;
	SOCKET        sClient;
	
	int           ret = 0;
	struct sockaddr_in server;
	struct hostent    *host = NULL;
	char* szRecvData = NULL;
	
	// Load Winsock
	//
	
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
		_LOG("Failed to load Winsock library!");
			
		::WSACleanup();
		nError = ERR_SOCK_STARTUP;
		return NULL;

	}
	
	//
	// Create the socket, and attempt to connect to the server
	//
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		_LOG("socket() failed: %d", WSAGetLastError());
			
		::WSACleanup();
		nError = ERR_SOCK_CREATION;
		return NULL;
	}
    
	// Release for reuse this port by another users or processes...
//	const int on = 1;
//	setsockopt(sClient, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	
    
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(address);
    
	do {
		
		
		if (connect(sClient, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
		{
			_LOG("connect() failed: %d", WSAGetLastError());
			nError = ERR_SOCK_CONNECT;
			break;
		}
		//////////////////////////////////////////////////////////////////////////
		// Send data 
		//
		ret = send_message(sClient, (char*)szSendData);
		if (ret < 1)
		{
			_LOG("send() failed: %d", WSAGetLastError());
			nError = ERR_SOCK_SEND;
			break;
		}
		
		_LOG("SEND [%d bytes]: %s", ret, szSendData);
		//////////////////////////////////////////////////////////////////////////
		// Recv data 
		
		
		szRecvData = recv_message(sClient, nTimeout);
		
		if(!szRecvData)
		{
			_LOG("recv() failed: %d", WSAGetLastError());
			nError = ERR_SOCK_RECV;
			break;
		}
		
		_LOG("RECV [%d bytes]: %s", strlen(szRecvData), szRecvData);
		closesocket(sClient);
		WSACleanup();
		nError = ERR_SUCCESS;
		return szRecvData;		
		
	} while(FALSE);
    
	if (INVALID_SOCKET != sClient)
		closesocket(sClient);
	
	::WSACleanup();
	
	return NULL;
}
*/
