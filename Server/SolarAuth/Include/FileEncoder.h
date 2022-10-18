#pragma once

//=============================================================================================================================
/// FileEncoder class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 03
	@remark
	- Webzen Crypt File
	@note
	- ���Ͽ� ���� ��ȣȭ/��ȣȭ ���̺귯��(TheMOLA)�� Ȯ���� (TheMOLA4File)����
	  �־��� ���Ͽ� ���� ��ȣȭ ó���� �����ϴ� ���̺귯��
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


