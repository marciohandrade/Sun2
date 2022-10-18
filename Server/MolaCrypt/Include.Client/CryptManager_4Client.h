#ifndef _CRYPTBASE_H
#define _CRYPTBASE_H
#pragma once

//������������������������������������������������������������������������������
//
#ifdef _MOLA_LIB
#	include "RandomSNKey.h"
#else
#	include "RandomSNKey_4Client.h"
#endif //

//������������������������������������������������������������������������������
class ISecureWorkNode;

//������������������������������������������������������������������������������
//
//
class EXPORTS_API CryptManager
{
public:		typedef DWORD		eCRYPT_T;
public:		VOID				Release();

			BOOL				Encrypt( BYTE* IN OUT pSTREAM, INT iSize, BYTE& OUT rCRC );
			BOOL				Decrypt( BYTE* IN OUT pSTREAM, INT iSize, BYTE IN rCRC );

			BOOL				StartLogic( BOOL bSET );

			// <USE>: SERVER/CLIENT
			// <GLOBAL><INITIALIZER>
			static BOOL			Init1st();
			static BOOL			InitInfo( PVOID arg );
			static BOOL			InitSecureInfoParser();
			VOID				Update( DWORD dwCurTick );

			BOOL				IsCanDelete();
			VOID				DecRefCount();

			// <USE>: CLIENT
			//   ������ Ŭ���̾�Ʈ������ ����� �͵�...
			static
			DWORD				GetHeartbeatSize();
			static
			ISecureWorkNode*	GetISecureWorkNode();

			static
			DWORD				GetMolaProtocolCRC();

			static
			DWORD				GetUserKeyInCryptInfo( BYTE* IN OUT pCryptINFO );
			static
			CryptManager*		SetCryptAlgorithmInfo( BYTE byLocalSNKey, BYTE* IN pCryptINFO, BYTE byEncodedIDX );

//////////////////////////////////////////////////////////////////////////
// <FIELDs>
#pragma pack(push, 4)	// 24BYTEs
private:	DWORD				m_Members[6];
			//
			//
			//
			//
			//
#pragma pack(pop)





//////////////////////////////////////////////////////////////////////////
// <CONSTRUCTOR><DESTRUCTOR>
private:	CryptManager();
public:		~CryptManager();
};


//������������������������������������������������������������������������������
// GameClient / CProtocolCryptography ���� ����� ����
class CryptManagerDeletator
{
public:		CryptManagerDeletator() : m_POINTER(NULL) {}
			~CryptManagerDeletator() { m_POINTER = NULL; }
			CryptManagerDeletator( const CryptManagerDeletator& rCryptManager )
				: m_POINTER(rCryptManager.m_POINTER) {}
			CryptManagerDeletator( CryptManager* pManager ) : m_POINTER(pManager) {}

			VOID	Release()
			{
				if( !m_POINTER ) return;
				if( m_POINTER->IsCanDelete() )
					delete m_POINTER;
				else
					m_POINTER->DecRefCount();
				m_POINTER = NULL;
			}

			inline BOOL operator == ( PVOID ptr ) const { return m_POINTER == ptr; }
			inline BOOL operator != ( PVOID ptr ) const { return !(m_POINTER == ptr); }

			inline CryptManagerDeletator* operator->()	{ return this; }

			inline BOOL	Encrypt( BYTE* IN OUT pbySTREAM, int iSize, BYTE& OUT byCRC )
			{
				if( m_POINTER )
					return m_POINTER->Encrypt( pbySTREAM, iSize, byCRC );
				return FALSE;
			}
			inline BOOL	Decrypt( BYTE* IN OUT pbySTREAM, int iSize, BYTE IN byCRC )
			{
				if( m_POINTER )
					return m_POINTER->Decrypt( pbySTREAM, iSize, byCRC );
				return FALSE;
			}

			inline BOOL	StartLogic( BOOL bset )
			{
				if( m_POINTER )
					return m_POINTER->StartLogic( bset );
				return FALSE;
			}
//////////////////////////////////////////////////////////////////////////
private:	// <FIELDs>
			CryptManager*		m_POINTER;
};


//������������������������������������������������������������������������������
// Client CProtocolCryptography
struct sCLIENT_CRYPT_INFO
{
	BOOL					m_bUseCryptography;
	ClientSNKey				m_SNKey;
	CryptManagerDeletator	m_pCryptManager;
	DWORD					m_dwKey;
};

//������������������������������������������������������������������������������
//
#pragma pack(push, 1)
// ��Ŷ ���� ��Ʈ��
struct SPacketBuffer
{
#define PACKET_ALIGN32BYTE(size)			(((size) + 0x1F) & ~0x1F)
	WORD		__Dummy;
	union {
		WORD	wSIZE;
		WORD	NETWORK_PACKET_PTR[1];
	};
	BYTE		PACKET_PTR[/*Network::MAX_CRYPTOGRAM_STREAM_SIZE*/ 16000];

	inline VOID		CopyFrom( const BYTE* pBuffer, const WORD wSize )
	{
		MSG_CG_CRYPTOGRAPHY_PACKET_ENCODED* pEncodedMsg = (MSG_CG_CRYPTOGRAPHY_PACKET_ENCODED*)pBuffer;
		wSIZE = wSize - (WORD)pEncodedMsg->GetHeaderSize();
		WORD wFSize = (WORD)PACKET_ALIGN32BYTE(wSIZE);
		CopyMemory( PACKET_PTR, pEncodedMsg->GetPacketStream(), wSIZE );
		ZeroMemory( PACKET_PTR+wSIZE, wFSize-wSIZE );
	}

	inline static SPacketBuffer*	GetPacketBuffer( BYTE* pDecodedMsg )
	{
		return (SPacketBuffer*)( pDecodedMsg-(sizeof(WORD)<<1) );
	}
};
#pragma pack(pop)

//������������������������������������������������������������������������������
// Client �ʱ�ȭ ��ƾ
class WzGResTable;
struct EXPORTS_API sCLIENT_INIT_PRE_ENTRY
{
	sCLIENT_INIT_PRE_ENTRY( WzGResTable* pGResTable );
};

struct sSIG_MSG;
//	[for CLIENT] ��Ŷ ���Ž�
//		pPacketMsg : ���ŵ� ��Ŷ ������ (�̰��� SPacketBuffer�� ����� BUFFER���� �Ѵ�.)
//		wSIZE : Ŭ��� wSIZE�˻縦 ���� �����Ƿ� 0 �Է�)



//������������������������������������������������������������������������������
//
class EXPORTS_API ISecureWorkNode
{
//////////////////////////////////////////////////////////////////////////
// <FIELDs>
protected:	DWORD	m_dwRecvRoundKey;

public:		ISecureWorkNode() {}
			~ISecureWorkNode() {}

public:		//	[for CLIENT] Client::Application::Heartbeat���� ȣ�� (TRUE, PTR, SZ) ��� ���� ��� ��Ŷ ����
			virtual BOOL		DoEarlyProcess( BYTE*& rpSendingMsg, WORD& rwSendingSize );
			//	[for CLIENT] Client::Application::Heartbeat���� ȣ�� (TRUE, PTR, SZ) ��� ���� ��� ��Ŷ ����
			virtual BOOL		DoLatelyProcess( BYTE*& rpSendingMsg, WORD& rwSendingSize );

public:		//	[for CLIENT] Application::Heartbeat���� ȣ��
			static VOID			Update( DWORD dwDeltaTick );

public:		//	[for CLIENT] Scene ��ȯ��
			struct EXPORTS_API OnChange
			{
				//	[for CLIENT] BattleScene�� ��� ��� ȣ�� - Init �迭���� �ѹ�
				static VOID		Scene( eZONETYPE ztZONE );
			};

			//	[for CLIENT] ��Ŷ ���Ž�
			//		pPacketMsg : ���ŵ� ��Ŷ ������ (�̰��� SPacketBuffer�� ����� BUFFER���� �Ѵ�.)
			//		wSIZE : Ŭ��� wSIZE�˻縦 ���� �����Ƿ� 0 �Է�)
			struct EXPORTS_API OnMsgRecvSignal
			{
				// [for CLIENT] CG_SECURE_QUERY_BUILTIN_CHK_CMD
				static VOID		BuiltinCodeCheck( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_QUERY_RESOURCE_CHK_T1_CMD
				static VOID		ResourceCheckT1( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_QUERY_RESOURCE_CHK_T2_CMD
				static VOID		ResourceCheckT2( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_QUERY_RESOURCE_CHK_T3_CMD
				static VOID		ResourceCheckT3( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_QUERY_RESOURCE_CHK_T4_CMD
				static VOID		ResourceCheckT4( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_CMD
				static VOID		SimplexSNK_T1( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_CMD
				static VOID		SimplexSNK_T2( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_CMD
				static VOID		SimplexSNK_T3( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_CMD
				static VOID		SimplexSNK_T4( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_KEY
				static VOID		DuplexSNK_T1( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_CMD
				static VOID		MOLA_Query( PVOID pPacketMsg, WORD wSIZE );
				// [for CLIENT] CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_CMD
				// [for CLIENT] CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_CMD
				static VOID		MOLA_Configure( PVOID pPacketMsg, WORD wSIZE );
			};

			struct EXPORTS_API OnMsgSend
			{
				// [for CLIENT] CG_SECURE_QUERY_BUILTIN_CHK_CMD
				static BOOL		BuiltinCodeCheck( BYTE*& prSendingMsg, WORD& rwSIZE );
			};

			struct EXPORTS_API OnGResInit
			{
				// [for CLIENT] ��� CS���� ��ũ��Ʈ ���� �񱳼ҽ� ������ ���� ����
				static VOID		RegisterData( eSCRIPT_CODE sccSCRIPT, DWORD dwKEY, PVOID pDATA );
			};


















};

#endif //_CRYPTBASE_H
