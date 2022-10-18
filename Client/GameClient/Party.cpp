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
    // 아직 Hero 정보가 없다면 FALSE
    // PLAYER_ENTER_ACK 이후로 파티정보를 받도록 수정하였으므로 hero없이는
    // 이곳에 도달할 수 없다.
    // 단지 노파심으로 설정.
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
// ProgrammCommon/ResultCode.h 참조
/*
char *partyReason[] = 
{
    "성공",
    "그런 플레이어가 존재하지 않습니다.",
    "파티장이 아닙니다.",
    "파티원이 아닙니다.",
    "파티가 존재하지 않습니다.",
    "상대방이 파티초대 요청을 거절하였습니다.",
    "레벨조건이 맞지 않습니다.",
    "잘못된 방 상태입니다.",
    "파티장을 변경할 수 없습니다.",
    "이미 파티에 소속되어 있습니다.",
    "상대방이 파티를 요청하지 않았습니다.",
    "데이타 타입이 맞지 않습니다.( 유효하지 않은 값입니다. )",
    "알수 없는 오류입니다...",
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
    처음에 한번 세팅
*/
void
Party::SetInfos(MSG_CG_PARTY_MEMBER_INFO_CMD *pAck)
{
    m_dwMasterObjKey    = pAck->m_dwMasterObjKey;		// 파티장 오브젝트 키
    m_byDistributionType = pAck->m_byDistributionType;  // 아이템 분배방식
    m_byMemberNum       = pAck->m_byMemberNum;			// 파티원 수(파티장 포함)
    
    for (int i=0; i<m_byMemberNum; ++i)
        m_MemberInfo.push_back(pAck->m_MemberInfo[i]);
}

//------------------------------------------------------------------------------
/**
    파티원 추가는 그때마다
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

            // 파티에서 제외되었을 경우 본인은 LEAVE 메시지를 보낸다.
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
    // 미리 초기화
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

    // 다시 구성
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
