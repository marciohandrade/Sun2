#pragma once

//=============================================================================================================================
/// FileDecoder class
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


#include "CryptFileReader.h"

class FileDecoder : public CryptFileReader
{
public:
	BOOL	ReadStart( const CHAR* pFileName, QueryKey IN key );
	VOID	ReadEnd();

	BOOL	GetData( BYTE* IN OUT pData, DWORD IN dwSize );
};

