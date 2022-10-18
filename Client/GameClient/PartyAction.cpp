//------------------------------------------------------------------------------
//  PartyAction.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "Party.h"
#include "SolarDialog.h"
#include "InterfaceManager.h"
#include "AskPartyInvitingDialog.h"
#include "Hero.h"

//------------------------------------------------------------------------------
/**
    강제 탈퇴
*/
bool
Party::actForcedExplusion(DWORD objKey)
{
    if (!this->IsMaster())
    {
		//	강제탈퇴는 파티장의 고유 권한입니다.
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString( eST_PARTY_FORCED_EXPULSION, szMessage, INTERFACE_STRING_LENGTH );
        this->Notice( szMessage );
        return false;
    }

    MSG_CG_PARTY_FORCED_EXPULSION_SYN sendMsg;
    sendMsg.m_byCategory    = CG_PARTY;
    sendMsg.m_byProtocol    = CG_PARTY_FORCED_EXPULSION_SYN;
    sendMsg.m_dwObjKey      = objKey;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_FORCED_EXPULSION_SYN) );

    return true;
}

//------------------------------------------------------------------------------
/**
    파티 탈퇴
*/
bool
Party::actLeave()
{
    MSG_CG_PARTY_LEAVE_REQUEST_SYN sendMsg;
    sendMsg.m_byCategory    = CG_PARTY;
    sendMsg.m_byProtocol    = CG_PARTY_LEAVE_REQUEST_SYN;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_LEAVE_REQUEST_SYN) );

    return true;
}

//------------------------------------------------------------------------------
/**
    파티장 위임
*/
bool
Party::actMigration(DWORD objKey)
{
    if (!this->IsMaster())
    {
		//	파티장 위임은 파티장의 고유 권한입니다.
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString( eST_PARTY_CHANGE_MASTER, szMessage, INTERFACE_STRING_LENGTH );
        this->Notice ( szMessage );
        return false;
    }

    MSG_CG_PARTY_CHANGE_MASTER_SYN sendMsg;
    sendMsg.m_byCategory    = CG_PARTY;
    sendMsg.m_byProtocol    = CG_PARTY_CHANGE_MASTER_SYN;
    sendMsg.m_dwObjKey      = objKey;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_CHANGE_MASTER_SYN) );

    return true;
}

//------------------------------------------------------------------------------
/**
    초대하기
*/
void
Party::actInvite(char *pname)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if (!this->IsMaster())
    {
		//	초대는 파티장의 고유 권한입니다.
		g_InterfaceManager.GetInterfaceString( eST_PARTY_INVITE_REQUEST, szMessage, INTERFACE_STRING_LENGTH );
		this->Notice ( szMessage );
        return;
    }

    if (!g_pHero)
        return;

    if (!strcmp(g_pHero->GetCharInfo()->m_pszCharName, pname))
    {
		//	초대는 타인만 유효합니다.
		g_InterfaceManager.GetInterfaceString( eST_PARTY_INVITE_REQUEST_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
		this->Notice ( szMessage );
        return;
    }

    MSG_CG_PARTY_INVITE_REQUEST_SYN sendMsg;
    sendMsg.m_byCategory    = CG_PARTY;
    sendMsg.m_byProtocol    = CG_PARTY_INVITE_REQUEST_SYN;
    strcpy (sendMsg.m_szCharName, pname);

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_INVITE_REQUEST_SYN) );
}

//------------------------------------------------------------------------------
/**
*/
void
Party::actAskInviting(char *strInviterName, DWORD dwInviterKey)
{
    this->m_strInviterName = strInviterName;
    this->m_dwInviterKey = dwInviterKey;

    // 어떠한 ASK 다이얼로그가 생성되어 있어도 내린다
    if (g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_ASK))
    {
        g_InterfaceManager.DeleteInterface(StrToWzID("DASK"));
    }

    AskPartyInvitingDialog * pAskDlg = new AskPartyInvitingDialog;
    if ( !g_InterfaceManager.LoadInterface(
        pAskDlg,
        "Data\\Interface\\82_Asking_Window.iwz",
        StrToWzID("DASK"),
        FALSE) 
        )
    {
        return;
    }

    g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_ASK, TRUE);
}

//------------------------------------------------------------------------------
/**
    초대 승인
*/
void
Party::actInvitedYes()
{
    MSG_CG_PARTY_INVITE_RESPONSE_ACK sendMsg;
    sendMsg.m_byCategory    = CG_PARTY;
    sendMsg.m_byProtocol    = CG_PARTY_INVITE_RESPONSE_ACK;
    sendMsg.m_dwMasterUserKey = this->m_dwInviterKey;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_INVITE_RESPONSE_ACK) );

    if (g_InterfaceManager.IsShowDialog( InterfaceManager::DIALOG_ASK))
    {
        g_InterfaceManager.DeleteInterface(StrToWzID("DASK"));
    }
}

//------------------------------------------------------------------------------
/**
    초대 거부
*/
void
Party::actInvitedNo()
{
    MSG_CG_PARTY_INVITE_RESPONSE_NAK sendMsg;
    sendMsg.m_byCategory    = CG_PARTY;
    sendMsg.m_byProtocol    = CG_PARTY_INVITE_RESPONSE_NAK;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_INVITE_RESPONSE_NAK) );

    if (g_InterfaceManager.IsShowDialog( InterfaceManager::DIALOG_ASK))
    {
        g_InterfaceManager.DeleteInterface(StrToWzID("DASK"));
    }
}

//------------------------------------------------------------------------------
/**
     파티 해체
*/
bool
Party::actDestroyParty()
{
    MSG_CG_PARTY_DESTROY_PARTY_SYN sendMsg;
    sendMsg.m_byCategory    = CG_PARTY;
    sendMsg.m_byProtocol    = CG_PARTY_DESTROY_PARTY_SYN;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_DESTROY_PARTY_SYN) );
    return true;
}

//------------------------------------------------------------------------------
/**
    버프 정보 보이기/숨기기
*/
bool
Party::actToggleShowBuffInfo()
{
    this->m_bShowBuffInfo = !(this->m_bShowBuffInfo);

    // 변경된 파티원들의 UI를 표시한다.
    this->ShowPartyInfoDlgs(true);

    return true;
}
