//=============================================================================================================================
/// CryptManagerS2S class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 10. 12
	@remark
		- ������ ���� ���̿� ���ǰ� �Ǵ� ��Ŷ ��ȣȭ ó�� ������
	@note
	@history 
		-  
*/
//=============================================================================================================================

#ifndef	_CRYPTBASE_S2S_H
#define	_CRYPTBASE_S2S_H

#include "RandomSNKeyS2S.h"

typedef struct sCRYPT_KEY_STREAM
{
	BYTE	STREAM[200];
}* CRYPT_KEY_STREAM;

class CryptManagerS2S
{
public:
	// �������� ���� ������ �ϴ°�, Ŭ���̾�Ʈ ������ �ϴ� ���� ����,
	// �������� �ݵ�� ���� �ٸ� ������ �þƾ� �Ѵ�.
	enum eROLE_SIDE { ROLE_NOALIGN, ROLE_SERVER, ROLE_CLIENT };

public:
	CryptManagerS2S();
	~CryptManagerS2S();

	VOID	Initialize( eROLE_SIDE role /*�߰����� �ִ��� Ȯ���� ��*/ );
	VOID	Release();

	BOOL	Encrypt( BYTE* IN OUT stream, int size, BYTE& OUT crc );
	BOOL	Decrypt( BYTE* IN OUT stream, int size, BYTE IN crc );

	BOOL	StartLogic( BOOL bset );

	// 200 Byte ũ���� ����� �Ҵ��� ��� �Ѵ�.
	static VOID				MakeGarbageBlock( CRYPT_KEY_STREAM IN OUT pCryptKeyStream, DWORD dwGabageKey );

	// ��ȣȭ ����, Ű��ȯ�� ���� ��Ʈ�� ����,
	//  cryptInfo�� MSG_CRYPTOGRAPHY_CMD::m_byCryptBuffer�� �� ũ��� 200�̴�.
	//  bySymmetrySNKey�� �������� �ϴ� SNKey�� ���� �̷�� Ű�� �Է��Ѵ�.
	// <return>
	//   byPairSNKey%(MAX_KEY_SIZE)
	BYTE	CreateCryptKeyStream( BYTE byPairSNKey, CRYPT_KEY_STREAM IN OUT pCryptKeyStream, DWORD dwKey );

	VOID	AllocateCryptKey( PVOID encKey, PVOID decKey );
	// 
	//  ������ Ŭ���̾�Ʈ ������ �����ϴ� �ʿ��� ����ϴ� �͵�...
	static DWORD			ExtractUserKeyFromCryptKeyStream( CRYPT_KEY_STREAM IN OUT cryptInfo );
	static CryptManagerS2S*	CreateCryptManagerFromCryptKeyStream( BYTE byLocalSNKey, CRYPT_KEY_STREAM IN pCryptKeyStream, BYTE encodedIdx );

private:
	// <FIELDs>
	BOOL			m_Inited;
	eROLE_SIDE		m_RoleSide;
	PVOID			m_pCryptAlgorithm;
};

#endif //_CRYPTBASE_S2S_H

