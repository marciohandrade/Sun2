#include "stdafx.h"
#include <PacketControl/PacketControl.h>

#include <AgentServer.h>
//
#include <PacketHandler/PacketHandler.h>
//
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>
//
#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>
#include <UserSessions/TempUser.h>

#include <TimeCheck/HeartbeatChecker.h>
#include <Zone/ZoneManager.h>
#include <Zone/ZoneInterface.h>
#include <Party/PartyManager.h>

// (WAVERIX) (090519) (CHANGES) 패킷 관련 매크로 제거후 EnDecrypter 코드 분리
class EnDecrypter
{
public:
    class UserIO
    {
    public:
        inline static void  SendPacket(User* pUser, MSG_BASE*& pMsg, WORD& wSize,
                                       BOOLEAN bUsePacking = TRUE);
        inline static BOOLEAN OnRecv(User* pUser, BYTE*& pMsg, WORD& wSize);
    };

    class TempUserIO
    {
    public:
        inline static BOOLEAN SendPacket(TempUser* pTUser, MSG_BASE*& pMsg, WORD& wSize,
                                         BOOLEAN bUsePacking = TRUE);
        inline static BOOLEAN OnRecv(TempUser* pTUser, BYTE*& pMsg, WORD& wSize);
    };
};

inline void
EnDecrypter::UserIO::SendPacket(User* pUser, MSG_BASE*& pMsg, WORD& wSize, BOOLEAN bUsePacking)
{
    SHOW_PACKET_STREAM(pMsg, wSize, "A2C");
    //-----------------------------------------------------------------------
    // 포인터 및 크기 보정
    union
    {
        MSG_BASE*			pBASE;
        MSG_BASE_INTERNAL*	pINTERNAL;
        BYTE*				pBYTE;
        MSG_CG_CRYPTOGRAPHY_PACKET_ENCODING* pENCODING;
    };

    if(bUsePacking)
    {
        nsPacket::PacketInfo packetInfo = nsPacket::PacketControl::Pack(pMsg, wSize);
        pMsg = packetInfo.pMsg;
        wSize = packetInfo.Size;
    }

    pBASE = PACKET_CONTROL::GetPacketPtr_BASE_A2C();
    if(pBASE != pMsg)
        CopyMemory(pBYTE, pMsg, (SIZE_T)wSize);
    wSize = ((WORD)wSize-(WORD)sizeof(DWORD));
    pBYTE = PACKET_CONTROL::GetPacketPtr_A2C();
    //-----------------------------------------------------------------------

    SecurePackNode4User& rSECURE = pUser->GetSecureNode();
    CryptPack4Packet& rPACKET = rSECURE.ACTIVE_CRYPT_PACKET_INFO();
    if(rSECURE.IsUsePacketCrypt())
    {
        pENCODING = PACKET_CONTROL::GetPacketPtr_A2C_ENCODING();
        static BOOL bDummy = CryptManager::SetGarbageBlock((BYTE*)pENCODING, (DWORD)(__int64)pENCODING);

        ASSERT(wSize && wSize<16000);

        pENCODING->SetSNKey(rPACKET.GetSendSNKey());
        rSECURE.EncryptPacket(pENCODING->GetPacketStream(), wSize, pENCODING->GetCRC());

        // remove info
        //if(wSize < sizeof(DWORD))		memset(pMsg, 0, wSize);
        //else *(DWORD*)pMsg = 0;	// protection overflow

        pMsg = (MSG_BASE*)pBYTE;
        wSize += (WORD)pENCODING->GetHeaderSize();

        rPACKET.NextRoundSNKey4Send();	// 새로운 SN키를 발급받는다. 
        // <END>
    }
} //SendPacket

inline BOOLEAN
EnDecrypter::UserIO::OnRecv(User* pUser, BYTE*& pMsg, WORD& wSize)
{
    union
    {
        MSG_BASE*			pBASE;
        MSG_BASE_INTERNAL*	pINTERNAL;
        BYTE*				pBYTE;
        MSG_CG_CRYPTOGRAPHY_PACKET_ENCODED*	pENCODED;
    };

    //-----------------------------------------------------------------------
    // 포인터 및 크기 보정
    pENCODED = PACKET_CONTROL::GetPacketPtr_C2A_ENCODED();

    if(pBYTE != pMsg)
        CopyMemory(pBYTE, pMsg, (SIZE_T)wSize);

    pMsg = pBYTE;
    //-----------------------------------------------------------------------

    DWORD dwSize = (DWORD)wSize - pENCODED->GetHeaderSize();

    SecurePackNode4User& rSECURE = pUser->GetSecureNode();
    CryptPack4Packet& rPACKET = rSECURE.ACTIVE_CRYPT_PACKET_INFO();

    if(rPACKET.GetRecvSNKey() == pENCODED->GetSNKey())
    {
        if(dwSize && (dwSize < 16000) && rSECURE.DecryptPacket(pENCODED->GetPacketStream(), dwSize, pENCODED->GetCRC()))
        {
            rPACKET.NextRoundSNKey4Recv();	// 다음에 사용할 SN키를 생성한다.

            // 포인터 및 크기 보정
            pBASE = PACKET_CONTROL::GetPacketPtr_BASE_C2A();
            dwSize += (DWORD)sizeof(DWORD);

            pMsg = pBYTE;
            wSize = (WORD)dwSize;

            SHOW_PACKET_STREAM(pBASE, wSize, "C2A");

            return TRUE;
        }
    }
    else if(sSECURE_FLAG::TRANS_SIMPLEX_SNK == rSECURE.GetErrStatus())
    {
        //printf("not implementation\n");
    }

    return FALSE;
}

inline BOOLEAN
EnDecrypter::TempUserIO::SendPacket(TempUser* pTUser, MSG_BASE*& pMsg, WORD& wSize,
                                    BOOLEAN bUsePacking)
{
    __UNUSED(pTUser);
    union
    {
        MSG_BASE*			pBASE;
        MSG_BASE_INTERNAL*	pINTERNAL;
        BYTE*				pBYTE;
    };

    SHOW_PACKET_STREAM(pMsg, wSize, "A2C");

    if(bUsePacking)
    {
        nsPacket::PacketInfo packetInfo = nsPacket::PacketControl::Pack(pMsg, wSize);
        pMsg = packetInfo.pMsg;
        wSize = packetInfo.Size;
    }

    pBASE = PACKET_CONTROL::GetPacketPtr_BASE_A2C();
    if(pBASE != pMsg)
        CopyMemory(pBYTE, pMsg, (SIZE_T)wSize);

    pBYTE = PACKET_CONTROL::GetPacketPtr_A2C();

    wSize = ((WORD)wSize-(WORD)sizeof(DWORD));
    ASSERT(wSize && wSize < 16000);
    if(wSize && wSize < 16000)
    {
        pMsg = pBASE;
        return TRUE;
    }
    // --error
    return FALSE;
}

inline BOOLEAN
EnDecrypter::TempUserIO::OnRecv(TempUser* pTUser, BYTE*& pMsg, WORD& wSize)
{
    __UNUSED(pTUser);
    //-----------------------------------------------------------------------
    // 포인터 및 크기 보정
    union
    {
        MSG_BASE*			pBASE;
        MSG_BASE_INTERNAL*	pINTERNAL;
        BYTE*				pBYTE;
    };

    pBYTE = PACKET_CONTROL::GetPacketPtr_C2A();
    if(pBYTE != pMsg)	
        CopyMemory(pBYTE, pMsg, (SIZE_T)wSize);
    pBASE = PACKET_CONTROL::GetPacketPtr_BASE_C2A();
    pMsg = pBYTE;
    wSize += sizeof(DWORD);

    //-----------------------------------------------------------------------
    nsPacket::PacketInfo packetInfo = nsPacket::PacketControl::UnPack(pBASE, wSize);
    pBASE = packetInfo.pMsg;
    pMsg = pBYTE;
    wSize = packetInfo.Size;

    SHOW_PACKET_STREAM(pBASE, wSize, "C2A");
    return TRUE;
}



void
User::OnRecv(BYTE* pMsg, WORD wSize)
{
	if(DisconnectProcessing())
		return;
#if USING_DUMMY_CLIENT == 0
	if(EnDecrypter::UserIO::OnRecv(this, pMsg, wSize))
#endif
	{
		PacketHandler::GetHandler()->ParsePacketCA(UPT_USER, this, (MSG_BASE*)pMsg, wSize);
		return;
	}

	DisconnectProcessing() = TRUE;
	//sSECURE_CONTROL_STATUS::RECEIVED_CRACKED_PACKET
	GetSecureNode().SetErrStatus(sSECURE_FLAG::ERR_CORRUPTED_PACKET);
	SecureManager::RemoveCryptedUser(this);	//정확히 리스트의 어느 위치에 있는지는 상관없음.
	SecureManager::AddCryptingUser(this, sSECURE_FLAG::TRANS_FORCEDCLOSING);
}

// __WAVERIX_PACKET_HANDLING__ 매크로에 의한...
// 사용자에게 보내는 패킷은 
BOOL
User::SendPacket(MSG_BASE* pMsg, WORD wSize, const BOOLEAN bUsePacking)
{
#if USING_DUMMY_CLIENT == 0
	EnDecrypter::UserIO::SendPacket(this, pMsg, wSize, bUsePacking);
	return UserSession::SendPacket(pMsg, wSize, bUsePacking);
#else
    test_unit::SendToDummyClient(pMsg, wSize);
    return true;
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BOOL
TempUser::SendPacket(MSG_BASE* pMsg, WORD wSize, const BOOLEAN bUsePacking)
{
#if USING_DUMMY_CLIENT == 0
	if(EnDecrypter::TempUserIO::SendPacket(this, pMsg, wSize, bUsePacking))
	{
		return UserSession::SendPacket(pMsg, wSize, bUsePacking);
	}
	return FALSE;
#else
    test_unit::SendToDummyClient(pMsg, wSize);
    return true;
#endif
}


void
TempUser::OnRecv(BYTE * pMsg, WORD wSize)
{
	//#define STRESS_TEST


#ifdef STRESS_TEST
	Send(pMsg, wSize);
	return;
#endif

	const WORD wCONNECT_SYN = MAKEWORD(CG_CONNECTION, CG_CONNECTION_ENTERSERVER_SYN);
	const WORD wCRYPT_ACK = MAKEWORD(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_ACK);
	const WORD wCRYPT_NAK = MAKEWORD(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_NAK);

	EnDecrypter::TempUserIO::OnRecv(this, pMsg, wSize);
	const MSG_BASE* pBASE = (MSG_BASE*)pMsg;

	WORD wCompare = MAKEWORD(pBASE->m_byCategory, pBASE->m_byProtocol);

	const WORD wWORLD_CONNECT_SYN = MAKEWORD(CG_CONNECTION, CG_PREPARE_WORLD_CONNECT_SYN);
	const WORD wHEARTBEAT = MAKEWORD(CG_CONNECTION, CG_CONNECTION_HEARTBEAT_SYN);
	switch(wCompare)
	{
	case wHEARTBEAT:
	case wWORLD_CONNECT_SYN:
		return;
	}

	switch(m_dwChangeMod)
	{
	case IDLING:
		{
			if(wCONNECT_SYN == wCompare)
			{
				if(ProcessCHECKING(/*(MSG_CG_CONNECTION_ENTERSERVER_SYN*)*/pMsg, wSize))
				{
					//SUNCONNLOG(eFULL_LOG, "ProcessCHECKING");
					//SUNLOG(eCRITICAL_LOG, "ProcessCHECKING\n");
					m_dwChangeMod = CHECKING;
				}
				else
				{
					if(!DisconnectProcessing())
					{
						this->SetDisconnectCode(RC::RC_CONNECTION_UNEXPECTED_PACKET);
						this->DisconnectUser();	// 앞에서 Disconnect시에 못받게 할 필요는 있을 것이다.

						SECURELOG(SECURELOG_TEMPUSERINFO(this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_TEMPUSER_AUTH_DISCONNECT), 
							_T("[UserID:%s][%02u:%02u] [Violation] Game Authentication Flow (1st-1) Failed"), GetUserID(), pBASE->m_byCategory, pBASE->m_byProtocol);
					}
					//SUNLOG(eCRITICAL_LOG, "이상한 접속 상태\n");
					//SUNCONNLOG(eFULL_LOG, "이상한 접속 상태");
				}
			}
			else
			{
				//SUNCONNLOG(eFULL_LOG, "MSG_CG_CONNECTION_ENTERSERVER_NAK");
				//SUNLOG(eCRITICAL_LOG, "MSG_CG_CONNECTION_ENTERSERVER_NAK\n");
				//MSG_CG_CONNECTION_ENTERSERVER_NAK msgNAK;
				//msgNAK.m_dwErrorCode = RC::RC_CONNECTION_UNEXPECTED_PACKET;		// 현 패킷 플로우에 예기치 못한 패킷이 수신되었습니다.
				//this->SendPacket(&msgNAK, sizeof(MSG_CG_CONNECTION_ENTERSERVER_NAK));

				if(!DisconnectProcessing())
				{
					this->SetDisconnectCode(RC::RC_CONNECTION_UNEXPECTED_PACKET);
					this->DisconnectUser();	// 앞에서 Disconnect시에 못받게 할 필요는 있을 것이다.

					SECURELOG(SECURELOG_TEMPUSERINFO(this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_TEMPUSER_AUTH_DISCONNECT),
						_T("[UserID:%s][%02u:%02u] [Violation] Game Authentication Flow (1st-2) Failed"), GetUserID(), pBASE->m_byCategory, pBASE->m_byProtocol);
				}
			}
		}
		break;
	case CHECKING:
		{
			if(wCRYPT_ACK == wCompare)
			{
				if(!ProcessPASSING(/*(MSG_CG_CRYPTOGRAPHY_ACK*)*/pMsg, wSize))
				{
					if(!DisconnectProcessing())
					{
						this->SetDisconnectCode(RC::RC_CONNECTION_UNEXPECTED_PACKET);
						this->DisconnectUser();	// 앞에서 Disconnect시에 못받게 할 필요는 있을 것이다.

						SECURELOG(SECURELOG_TEMPUSERINFO(this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_TEMPUSER_AUTH_DISCONNECT),
							_T("[UserID:%s][%02u:%02u] [Violation] Game Authentication Flow (2nd-1) Failed"), GetUserID(), pBASE->m_byCategory, pBASE->m_byProtocol);
					}
				}
				m_dwChangeMod = IDLING;
				//SUNCONNLOG(eFULL_LOG, "ProcessPASSING");
				//SUNLOG(eCRITICAL_LOG, "ProcessPASSING\n");
			}
			else if(wCRYPT_NAK == wCompare)
			{
				if(!DisconnectProcessing())
				{
					this->SetDisconnectCode(RC::RC_DISCONNECT_CRYPTOGRAPHY_KEYEXCHANGE_NAK);
					this->DisconnectUser();
					m_dwChangeMod = IDLING;
					//SUNCONNLOG(eFULL_LOG, "[TempUser::OnRecv] CG_CRYPTOGRAPHY_NAK");
					SECURELOG(SECURELOG_TEMPUSERINFO(this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_TEMPUSER_AUTH_DISCONNECT),
						_T("[UserID:%s][%02u:%02u] [Violation] Game Authentication Flow (2nd-2) Failed-CRYPT_NAK"), GetUserID(), pBASE->m_byCategory, pBASE->m_byProtocol);
				}
			}
			else
			{
				// 테스트용 코드
				//pMsg+=4;
				//wSize -= sizeof(DWORD);
				//m_pNewUser->OnRecv(pMsg, wSize);
				//MSG_CG_CONNECTION_ENTERSERVER_NAK msgNAK;
				//msgNAK.m_dwErrorCode = RC::RC_CONNECTION_UNEXPECTED_PACKET;		// 현 패킷 플로우에 예기치 못한 패킷이 수신되었습니다.
				//this->SendPacket(&msgNAK, sizeof(MSG_CG_CONNECTION_ENTERSERVER_NAK));
				//this->SetDisconnectCode(RC::RC_CONNECTION_UNEXPECTED_PACKET);
				//this->DisconnectUser(TRUE);	// 앞에서 Disconnect시에 못받게 할 필요는 있을 것이다.
				//SUNCONNLOG(eFULL_LOG, "이상한 패킷옴");

				if(!DisconnectProcessing())
				{
					this->SetDisconnectCode(RC::RC_CONNECTION_UNEXPECTED_PACKET);
					this->DisconnectUser();	// 앞에서 Disconnect시에 못받게 할 필요는 있을 것이다.

					SECURELOG(SECURELOG_TEMPUSERINFO(this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_TEMPUSER_AUTH_DISCONNECT),
						_T("[UserID:%s][%02u:%02u] [Violation] Game Authentication Flow (2nd-3) Failed"), GetUserID(), pBASE->m_byCategory, pBASE->m_byProtocol);
				}
			}
		}
		break;
	default:
		{
			this->SetDisconnectCode(RC::RC_CONNECTION_UNEXPECTED_PACKET);
			this->DisconnectUser();
			m_dwChangeMod = IDLING;

			SECURELOG(SECURELOG_TEMPUSERINFO(this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_TEMPUSER_AUTH_DISCONNECT),
				_T("[UserID:%s][%02u:%02u] [Violation] Game Authentication Flow (Unknown) Failed - 상태모드 인식 불가"), GetUserID(), pBASE->m_byCategory, pBASE->m_byProtocol);
		}
		break;
	};
}












