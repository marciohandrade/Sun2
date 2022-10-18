//------------------------------------------------------------------------------
//  Party.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "Party.h"
#include "SolarDialog.h"
#include "PartyDialog.h"
#include "PartyMemberOptionDialog.h"
#include "Hero.h"

const int c_iPARTY_MSG_DELAY = 10000;

//------------------------------------------------------------------------------
/**
*/
Party::Party()
{
    this->Init();
    this->ClearInfos();
}

//------------------------------------------------------------------------------
/**
*/
Party::~Party()
{
	this->ClearInfos();
}

//------------------------------------------------------------------------------
/**
*/
void
Party::Init()
{
    m_dwMasterObjKey = 0;
    m_dwInviterKey = 0;
    m_bShowBuffInfo = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void
Party::ClearInfos ()
{
    m_byDistributionType = 0;
    m_byMemberNum = 0;
    m_bVisable = FALSE;

	m_MemberInfo.clear();
}

//------------------------------------------------------------------------------
/**
*/
InterfaceManager::eDIALOG_ID
Party::GetStartPartyDialog()
{
    InterfaceManager::eDIALOG_ID eid;

    if (this->m_bShowBuffInfo)
    {
        eid = InterfaceManager::DIALOG_PARTYBUFF0;
    }
    else
    {
        eid = InterfaceManager::DIALOG_PARTY0;
    }

    return eid;
}

//------------------------------------------------------------------------------
/**
*/
void
Party::SetMaster(DWORD objKey)
{
    this->m_dwMasterObjKey = objKey;
}

//------------------------------------------------------------------------------
/**
*/
BOOL
Party::IsMaster()
{
    // FIXME: ck
    // ���� Hero ������ ���ٸ� FALSE
    // PLAYER_ENTER_ACK ���ķ� ��Ƽ������ �޵��� �����Ͽ����Ƿ� hero���̴�
    // �̰��� ������ �� ����.
    // ���� ���Ľ����� ����.
    if (!g_pHero)
        return FALSE;

    return (g_pHero->GetObjectKey() == this->m_dwMasterObjKey);
}

//------------------------------------------------------------------------------
/**
*/
void
Party::Notice(TCHAR *pnotice, WzColor Color)
{
    g_InterfaceManager.ShowMessageBox( pnotice, c_iPARTY_MSG_DELAY, &Color );
}

//------------------------------------------------------------------------------
/**
*/
// ProgrammCommon/ResultCode.h ����
/*
char *partyReason[] = 
{
    "����",
    "�׷� �÷��̾ �������� �ʽ��ϴ�.",
    "��Ƽ���� �ƴմϴ�.",
    "��Ƽ���� �ƴմϴ�.",
    "��Ƽ�� �������� �ʽ��ϴ�.",
    "������ ��Ƽ�ʴ� ��û�� �����Ͽ����ϴ�.",
    "���������� ���� �ʽ��ϴ�.",
    "�߸��� �� �����Դϴ�.",
    "��Ƽ���� ������ �� �����ϴ�.",
    "�̹� ��Ƽ�� �ҼӵǾ� �ֽ��ϴ�.",
    "������ ��Ƽ�� ��û���� �ʾҽ��ϴ�.",
    "����Ÿ Ÿ���� ���� �ʽ��ϴ�.( ��ȿ���� ���� ���Դϴ�. )",
    "�˼� ���� �����Դϴ�...",
};
*/
void
Party::Reason(ePARTY_RESULT eReason, WzColor Color)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];
	g_InterfaceManager.GetInterfaceString( (eST_PARTY_RESULT_MESSAGE + eReason), szMessage, INTERFACE_STRING_LENGTH );
	this->Notice( szMessage, Color );
}

//------------------------------------------------------------------------------
/**
*/
DWORD
Party::name2key(char *pname)
{
    vector<PARTY_MEMBER_INFO>::iterator pmemIter;

    for (pmemIter = m_MemberInfo.begin();
        pmemIter != m_MemberInfo.end();
        ++pmemIter)
    {
        PARTY_MEMBER_INFO pmem = (*pmemIter);
        if (!strcmp(pmem.m_szCharName, pname))
            return pmem.m_dwObjKey;
    }

    // away confuse
    ASSERT(0);

    return 0;
}

//------------------------------------------------------------------------------
/**
*/
char *
Party::key2name(DWORD key)
{
    vector<PARTY_MEMBER_INFO>::iterator pmemIter;

    for (pmemIter = m_MemberInfo.begin();
        pmemIter != m_MemberInfo.end();
        ++pmemIter)
    {
        PARTY_MEMBER_INFO pmem = (*pmemIter);
        if (pmem.m_dwObjKey == key)
            return pmem.m_szCharName;
    }

    // away confuse
    ASSERT(0);

    return NULL;
}

//------------------------------------------------------------------------------
/**
    ó���� �ѹ� ����
*/
void
Party::SetInfos(MSG_CG_PARTY_MEMBER_INFO_CMD *pAck)
{
    m_dwMasterObjKey    = pAck->m_dwMasterObjKey;		// ��Ƽ�� ������Ʈ Ű
    m_byDistributionType = pAck->m_byDistributionType;  // ������ �й���
    m_byMemberNum       = pAck->m_byMemberNum;			// ��Ƽ�� ��(��Ƽ�� ����)
    
    for (int i=0; i<m_byMemberNum; ++i)
        m_MemberInfo.push_back(pAck->m_MemberInfo[i]);
}

//------------------------------------------------------------------------------
/**
    ��Ƽ�� �߰��� �׶�����
*/
void
Party::AddInfo(MSG_CG_PARTY_MEMBER_JOIN_BRD *pAck)
{
    m_MemberInfo.push_back(pAck->m_NewMemberInfo);
}

//------------------------------------------------------------------------------
/**
*/
void
Party::DelInfo(DWORD dwObjKey)
{
    vector<PARTY_MEMBER_INFO>::iterator pmemIter;

    for (pmemIter = m_MemberInfo.begin();
         pmemIter != m_MemberInfo.end();
         ++pmemIter)
    {
        PARTY_MEMBER_INFO pmem = (*pmemIter);
 
        if ( pmem.m_dwObjKey == dwObjKey )
        {
            m_MemberInfo.erase(pmemIter);

            // ��Ƽ���� ���ܵǾ��� ��� ������ LEAVE �޽����� ������.
            if (g_pHero && g_pHero->GetObjectKey() == dwObjKey)
            {
                this->LeaveParty();
            }
            return;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Party::LeaveParty()
{
    // �̸� �ʱ�ȭ
    this->Init();

    switch (GameFramework::GetCurScene() )
    {
    case SCENE_TYPE_HUNTING:
        {
            MSG_CG_ZONE_LEAVEHUNTINGREQ_SYN SendPacket;

            SendPacket.m_HuntingKey = BattleScene::GetCurRoomKey();                               						

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }
        break;

    case SCENE_TYPE_MISSION:
        {
            MSG_CG_ZONE_LEAVEMISSIONREQ_SYN SendPacket;

            SendPacket.m_MissionKey = BattleScene::GetCurRoomKey();                               

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }
        break;

    case SCENE_TYPE_LOBBY:
        {
            MSG_CG_ZONE_LEAVELOBBYREQ_SYN SendPacket;

            SendPacket.m_LobbyKey = BattleScene::GetCurRoomKey();                               

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
PARTY_MEMBER_INFO*
Party::GetInfo (DWORD objKey)
{
    vector<PARTY_MEMBER_INFO>::iterator pmemIter;

    for (pmemIter = m_MemberInfo.begin();
        pmemIter != m_MemberInfo.end();
        ++pmemIter)
    {
        PARTY_MEMBER_INFO *pmem = &(*pmemIter);
        if (pmem->m_dwObjKey == objKey)
            return pmem;
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void
Party::ShowPartyInfoDlgs(BOOL bFlag)
{
    int i = 0;

    for (i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        PartyDialog * ptDlg = static_cast<PartyDialog *>(g_InterfaceManager.GetDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTY0 + i) ));
        ptDlg->Clear();
        ptDlg->ShowWindow(FALSE);

        PartyDialog * ptbuffDlg = static_cast<PartyDialog *>(g_InterfaceManager.GetDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYBUFF0 + i) ));
        ptbuffDlg->Clear();
        ptbuffDlg->ShowWindow(FALSE);

        PartyMemberOptionDialog * ptoptDlg = static_cast<PartyMemberOptionDialog *>(g_InterfaceManager.GetDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYOPTION0 + i) ));
        ptoptDlg->Clear();
        ptoptDlg->ShowWindow(FALSE);
    }

    list<PARTY_MEMBER_INFO>  listMemberInfo;
    vector<PARTY_MEMBER_INFO>::iterator pmemIter;

    for (pmemIter = m_MemberInfo.begin();
        pmemIter != m_MemberInfo.end();
        ++pmemIter)
    {
        PARTY_MEMBER_INFO pmem = (*pmemIter);

        if (pmem.m_dwObjKey == this->m_dwMasterObjKey)
        {
            listMemberInfo.push_front(pmem);
        }
        else
        {
            listMemberInfo.push_back(pmem);
        }
    }

    // �ٽ� ����
    this->ClearInfos();

    list<PARTY_MEMBER_INFO>::iterator pmemListIter;

    i = 0;

    for (pmemListIter = listMemberInfo.begin();
        pmemListIter != listMemberInfo.end();
        ++pmemListIter)
    {
        PARTY_MEMBER_INFO pmem = (*pmemListIter);

        m_MemberInfo.push_back(pmem);

        PartyDialog * ptDlg = static_cast<PartyDialog *>(g_InterfaceManager.GetDialog( (InterfaceManager::eDIALOG_ID)(GetStartPartyDialog() + i) ));
        ptDlg->SetIdx (i);
        ptDlg->SetObjKey (pmem.m_dwObjKey);
        ptDlg->ShowWindow(bFlag);

        PartyMemberOptionDialog * ptoptDlg = static_cast<PartyMemberOptionDialog *>(g_InterfaceManager.GetDialog( (InterfaceManager::eDIALOG_ID)(InterfaceManager::DIALOG_PARTYOPTION0 + i) ));
        ptoptDlg->SetIdx (i);
        ptoptDlg->SetObjKey (pmem.m_dwObjKey);
        ptoptDlg->ShowWindow(FALSE);

        i++;
    }

    BattleScene::SetMonsterHPMultiplier(i);
}
