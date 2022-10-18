#pragma once

//=============================================================================================================================
/// DBEncryptFileWriter class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 05. 15
	@remark
	- Webzen Crypt File
	@note
	- ���Ͽ� ���� ��ȣȭ/��ȣȭ ���̺귯��(TheMOLA)��
	�⺻������ ���ԵǾ� �ִ� DBProxy ���� Writer
	@history 
	- 
*/
//=============================================================================================================================


#include "CryptFileWriter.h"

class DBEncryptFileWriter : public CryptFileWriter
{
public:
	//m_pChunk->header.byJumpIndex = m_dwKey%(sizeof(m_Chunk)>>1);
	BOOL	WriteStart( const CHAR* pFileName, QueryKey IN dbp = "DBP" );
	VOID	WriteEnd();

	BOOL	SetIdentification( BYTE* IN OUT pID );
	BOOL	SetPassword( BYTE* IN OUT pPWD );
private:
	DWORD	m_dwServiceMode;	// 0: 1: 2:
};

