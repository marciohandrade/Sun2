#pragma once

//=============================================================================================================================
/// FileDecoder class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 03
	@remark
	- Webzen Crypt File
	@note
	- 파일에 대한 암호화/복호화 라이브러리(TheMOLA)의 확장판 (TheMOLA4File)에서
	주어진 파일에 대한 암호화 처리를 수행하는 라이브러리
	@history 
	- 
*/
//=============================================================================================================================


#include "CryptFileReader.h"

class FileDecoder : public CryptFileReader
{
public:
	BOOL	ReadStart( const CHAR* pFileName, QueryKey IN key );
	VOID	ReadEnd();

	BOOL	GetData( BYTE* IN OUT pData, DWORD IN dwSize );
};

