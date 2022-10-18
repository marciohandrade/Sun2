#pragma once

#include <eXtreme/threading/Thread.h>

#define  UPDATE_DIR						"./update/"

class PatchRTTA 
{
	Singleton( PatchRTTA );

public:
			void			Start( char* szFtpAddress, DWORD dwFtpPort, char* szFtpUser, char* szFtpPassword, char* szFtpGetFilePath, char* szFtpGetFileName );
};
