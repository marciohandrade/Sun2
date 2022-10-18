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
    ���� Ż��
*/
bool
Party::actForcedExplusion(DWORD objKey)
{
    if (!this->IsMaster())
    {
		//	����Ż��� ��Ƽ���� ���� �����Դϴ�.
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
    ��Ƽ Ż��
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
    ��Ƽ�� ����
*/
bool
Party::actMigration(DWORD objKey)
{
    if (!this->IsMaster())
    {
		//	��Ƽ�� ������ ��Ƽ���� ���� �����Դϴ�.
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
    �ʴ��ϱ�
*/
void
Party::actInvite(char *pname)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if (!this->IsMaster())
    {
		//	�ʴ�� ��Ƽ���� ���� �����Դϴ�.
		g_InterfaceManager.GetInterfaceString( eST_PARTY_INVITE_REQUEST, szMessage, INTERFACE_STRING_LENGTH );
		this->Notice ( szMessage );
        return;
    }

    if (!g_pHero)
        return;

    if (!strcmp(g_pHero->GetCharInfo()->m_pszCharName, pname))
    {
		//	�ʴ�� Ÿ�θ� ��ȿ�մϴ�.
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

    // ��� ASK ���̾�αװ� �����Ǿ� �־ ������
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
    �ʴ� ����
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
    �ʴ� �ź�
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
     ��Ƽ ��ü
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
    ���� ���� ���̱�/�����
*/
bool
Party::actToggleShowBuffInfo()
{
    this->m_bShowBuffInfo = !(this->m_bShowBuffInfo);

    // ����� ��Ƽ������ UI�� ǥ���Ѵ�.
    this->ShowPartyInfoDlgs(true);

    return true;
}
