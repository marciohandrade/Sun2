#ifndef  __ClientSocket_H
#define  __ClientSocket_H

#pragma  comment(lib, "ws2_32.lib")

#include <Winsock2.h>
#include <Windows.h>
#include <stdio.h>

class ClientSocket
{
public:
			ClientSocket();
			~ClientSocket();

			bool		Connect( LPCTSTR ipAddress, u_short portNo );
			void		Close();

			int			Send( LPVOID buffer, int size );
			int			Recv( LPVOID buffer, int size );

private:
			SOCKET		m_socketValue;
};

#endif