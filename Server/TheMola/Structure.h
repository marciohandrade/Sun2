#pragma once

//=============================================================================================================================
/// TheMOLA Structure Definitions
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 05. 15
	@remark
	- Webzen Crypt File
	@note
	- 파일에 대한 암호화/복호화 라이브러리(TheMOLA)를 위한 기본 정의
	@history 
	- 2006. 08. 04 : 외부로 도출되던 것 내부 해더로 변경
*/
//=============================================================================================================================


#include "WzCF.Define.h"

#define TOKEN_SIZE	((DWORD)(MAX_ID_SIZE+1))

#pragma pack(push, 1)

struct sCRYPT_FILE_HEADER_INFO
{
	DWORD	dwRndKey;
	//WORD	wJumpIndex;
	//WORD	wSize;
	DWORD	dwSize;
};

struct CRYPT_FILE_HEADER	//12B
{
	union
	{
		DWORD	dwMagicKey;
		struct 
		{
			BYTE	byW;	// 'W'
			BYTE	byZ;	// 'Z'
			BYTE	byC;	// 'C'
			BYTE	byF;	// 'F'
		};
	};

	sCRYPT_FILE_HEADER_INFO INFO;
};

struct CRYPT_FILE_TOKEN		// 32B - FixSize
{
	sCRYPT_FILE_HEADER_INFO		INFO;
	BYTE						TOKEN[TOKEN_SIZE];
};

#define BUF_ALIGN(size, bufsize)	(((size)+(bufsize)-1)&(~((bufsize)-1)))

#define CRYPT_FILE_CHUNK_SIZE()		(MIN_FILE_DATA_SIZE-sizeof(CRYPT_FILE_HEADER))

//struct CRYPT_FILE_CHUNK
//{
//	CRYPT_FILE_HEADER	Header;
//	BYTE				KeyData[CRYPT_FILE_CHUNK_SIZE()];
//};

struct CRYPT_FILE_CHUNK_RENEW
{
	CRYPT_FILE_HEADER	Header;
	CRYPT_FILE_TOKEN	KeyData[1];
};

#pragma pack(pop)
