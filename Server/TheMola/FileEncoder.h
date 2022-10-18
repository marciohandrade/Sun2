#pragma once

//=============================================================================================================================
/// FileEncoder class
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


#include "CryptFileWriter.h"

class FileEncoder : public CryptFileWriter
{
public:
	BOOL	WriteStart( const CHAR* pFileName, QueryKey IN key );
	BOOL	WriteStart( const HANDLE hFD, QueryKey IN key );
	VOID	WriteEnd();

	BOOL	SetData( BYTE* IN pData, const DWORD dwSize );
private:
};


