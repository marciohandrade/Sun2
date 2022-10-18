#ifndef __SOCKUTIL_H__
#define __SOCKUTIL_H__
// sockutil.h: interface for the CSockUtil class.
//


typedef unsigned short		uint16_t;

#define FREE_MEM(x) { if ((x)!=NULL) {free(x);(x)=NULL;} }
//////////////////////////////////////////////////////////////////////////
int so_init();
int so_select(SOCKET sockFd, int nTime);
int so_select_write(SOCKET sockFd, int nTime);
int so_readn(SOCKET fd, char *ptrP, int nbytes);
int so_readline(SOCKET fd, char *ptr, int maxlen);
int so_readline_timeout(SOCKET fd, char *ptr, int maxlen, int nTime);
int so_readn_timeout(SOCKET fd, char *ptrP, int nbytes, int nTime);
char* recv_message(SOCKET fd, int nTime);
int send_message(SOCKET fd, char *res);
int so_writen(SOCKET fd, char *ptrP, int nbytes);
void* cioReceiveMessage(SOCKET& sockfd, int* Error, int* length);
//	int connect_nonblock(SOCKET sockfd, const struct sockaddr *ptrSaddr, int nSaddrlen, int nTimes);
//	SOCKET connect( const char* address, u_short port, int nTimeout );
//	int getRandomPort(SOCKET start, int num);

BOOL sendCommand(char* ip, USHORT port, int* p_timeout, 
		 const char* pSendBuffer, int nSendSize, 
		 char* pRecvBuffer, int nRecvSize );

//////////////////////////////////////////////////////////////////////////

u_long  get_netaddr( const char* address );
u_short get_netport( const char* service, const char* proto );
char*   get_peer_addr( SOCKET sock, char* buf, size_t len );
char*   get_peer_hostname( SOCKET sock, char* buf, size_t len );
int     get_peer_port( SOCKET sock );
char*   get_sock_addr( SOCKET sock, char* buf, size_t len );
char*   get_sock_hostname( SOCKET sock, char* buf, size_t len );
int     get_sock_port( SOCKET sock );
int     set_nonblk_mode( SOCKET sock, u_long mode );
SOCKET  sock_accept( SOCKET listen_sock, int* p_timeout );
int     sock_close( SOCKET sock );
SOCKET  sock_connect( const char* address, u_short port, int type, int* p_timeout );

int     sock_init( LPWSADATA lpWSAData );
int     sock_uninit();
SOCKET  sock_listen( const char* address, u_short port, int type, int backlog );

int     sock_read( SOCKET sock, char* buf, size_t count);
int     sock_read( SOCKET sock, char* buf, size_t count, int* p_timeout );

int     sock_send( SOCKET sock, const char *buf, size_t count );
int     sock_send( SOCKET sock, const char *buf, size_t count, int* p_timeout );

int     sock_wait( SOCKET sock, int timeout, int for_read );

//////////////////////////////////////////////////////////////////////////

#endif //__SOCKUTIL_H__