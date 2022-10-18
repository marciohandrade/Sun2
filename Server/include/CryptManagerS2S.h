//=============================================================================================================================
/// CryptManagerS2S class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 10. 12
	@remark
		- 서버와 서버 사이에 사용되게 되는 패킷 암호화 처리 관리자
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
	// 서버간에 서버 역할을 하는가, 클라이언트 역할을 하는 가를 지정,
	// 서버간에 반드시 서로 다른 역할을 맡아야 한다.
	enum eROLE_SIDE { ROLE_NOALIGN, ROLE_SERVER, ROLE_CLIENT };

public:
	CryptManagerS2S();
	~CryptManagerS2S();

	VOID	Initialize( eROLE_SIDE role /*추가사항 있는지 확인할 것*/ );
	VOID	Release();

	BOOL	Encrypt( BYTE* IN OUT stream, int size, BYTE& OUT crc );
	BOOL	Decrypt( BYTE* IN OUT stream, int size, BYTE IN crc );

	BOOL	StartLogic( BOOL bset );

	// 200 Byte 크기의 블록을 할당해 줘야 한다.
	static VOID				MakeGarbageBlock( CRYPT_KEY_STREAM IN OUT pCryptKeyStream, DWORD dwGabageKey );

	// 암호화 정보, 키교환을 위한 스트림 생성,
	//  cryptInfo는 MSG_CRYPTOGRAPHY_CMD::m_byCryptBuffer로 그 크기는 200이다.
	//  bySymmetrySNKey는 보내고자 하는 SNKey과 쌍을 이루는 키를 입력한다.
	// <return>
	//   byPairSNKey%(MAX_KEY_SIZE)
	BYTE	CreateCryptKeyStream( BYTE byPairSNKey, CRYPT_KEY_STREAM IN OUT pCryptKeyStream, DWORD dwKey );

	VOID	AllocateCryptKey( PVOID encKey, PVOID decKey );
	// 
	//  다음은 클라이언트 역할을 수행하는 쪽에서 사용하는 것들...
	static DWORD			ExtractUserKeyFromCryptKeyStream( CRYPT_KEY_STREAM IN OUT cryptInfo );
	static CryptManagerS2S*	CreateCryptManagerFromCryptKeyStream( BYTE byLocalSNKey, CRYPT_KEY_STREAM IN pCryptKeyStream, BYTE encodedIdx );

private:
	// <FIELDs>
	BOOL			m_Inited;
	eROLE_SIDE		m_RoleSide;
	PVOID			m_pCryptAlgorithm;
};

#endif //_CRYPTBASE_S2S_H

