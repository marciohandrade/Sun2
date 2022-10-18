//------------------------------------------------------------------------------
//  uiPartyLeader_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyLeader.h"
#include "uiPartyMember/uiPartyMember.h"
#include "uiPartyInvitingPopup/uiPartyInvitingPopup.h"
#include "Hero.h"
#include "TargetDialog.h"
#include "HeroActionInput.h"
#include "ObjectManager.h"
#include "Monster.h"

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::InitMemberInfos(MSG_CG_PARTY_MEMBER_INFO_CMD *pMsg)
{
#ifdef _CK_RENEW_PARTY
#else
    TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];

    SAFE_DELETE(this->m_PartyInfos);

    this->m_PartyInfos =
        new MSG_CG_PARTY_MEMBER_INFO_CMD;

    memcpy (this->m_PartyInfos,
            pMsg,
            sizeof (MSG_CG_PARTY_MEMBER_INFO_CMD));

    assert (g_pHero && "��Ƽ: Hero ������ �����ϴ�.");
    if (!g_pHero)
        return;

    this->m_bLeader = false;

    this->SetMemberInfo(NULL, 0);
    this->ResetHPGauge();

    int i;

    for (i=1; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        uiPartyMember* partyMember =
            static_cast<uiPartyMember *>(this->GetUIMan()->GetUserInterface(UI_PARTY_LEADER + i));

        if (partyMember)
        {
            partyMember->SetMemberInfo(NULL, 0);
            partyMember->ResetHPGauge();
        }
    }

    int addMemberCounts = 0;

    for (i=0; i<this->m_PartyInfos->m_byMemberNum; i++)
    {
        if (g_pHero->GetObjectKey() != this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
        {
			//	%s ���� �����ϼ̽��ϴ�.
			this->GetUIMan()->GetInterfaceString( eST_PARTY_MEMBER_ENTER_NOTICE_1, szText, INTERFACE_STRING_LENGTH );
            Sprintf (szMessage, szText, this->m_PartyInfos->m_MemberInfo[i].m_szCharName);
            this->PrintInterfaceMessage(szMessage);

			//	������ HP�� ���� �� �ִ� ����ġ�� �����մϴ�.
			this->GetUIMan()->GetInterfaceString( eST_PARTY_MEMBER_ENTER_NOTICE_2, szMessage, INTERFACE_STRING_LENGTH );
            this->PrintInterfaceMessage(szMessage);
        }

        if (this->m_PartyInfos->m_MemberInfo[i].m_bMaster)
        {
            // ��Ƽ��
            this->SetMemberInfo(&this->m_PartyInfos->m_MemberInfo[i], 0);
            this->ResetHPGauge();

            if (g_pHero->GetObjectKey() == this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
            {
                this->SetLeader();
            }
        }
        else
        {
            uiPartyMember* partyMember =
                static_cast<uiPartyMember *>(this->GetUIMan()->GetUserInterface(UI_PARTY_MEMBER1 + addMemberCounts));

            if (partyMember)
            {
                partyMember->SetMemberInfo(&this->m_PartyInfos->m_MemberInfo[i], (addMemberCounts+1));
                partyMember->ResetHPGauge();

                if (g_pHero->GetObjectKey() == this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
                    partyMember->SetOwned();
                else
                    partyMember->UnsetOwnded();

                addMemberCounts ++;
            }
        }
    }
#endif
}

//------------------------------------------------------------------------------
/**
*/
#ifdef _CK_RENEW_PARTY
void
uiPartyLeader::AddMemberInfo(MSG_CG_PARTY_JOIN_BRD* pMsg)
#else
void
uiPartyLeader::AddMemberInfo(MSG_CG_PARTY_MEMBER_JOIN_BRD* pMsg)
#endif
{
#ifdef _CK_RENEW_PARTY
#else
    TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];

    ASSERT (this->m_PartyInfos);
    if (!this->m_PartyInfos)
        return;

    int i;

    for (i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        if (this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey == 0)
        {
            memcpy (&this->m_PartyInfos->m_MemberInfo[i],
                    &pMsg->m_NewMemberInfo,
                    sizeof (PARTY_MEMBER_INFO));
            this->m_PartyInfos->m_byMemberNum++;
            break;
        }
    }

    if (!g_pHero)
    {
#ifdef _DEBUG
        this->GetUIMan()->Sys_VerifyLock(_T("g_pHero ������ ����"));
#endif
        return;
    }

	//	%s ���� �����ϼ̽��ϴ�.
	this->GetUIMan()->GetInterfaceString( eST_PARTY_MEMBER_ENTER_NOTICE_1, szText, INTERFACE_STRING_LENGTH );
	Sprintf (szMessage, szText, pMsg->m_NewMemberInfo.m_szCharName);
	this->PrintInterfaceMessage(szMessage);

	//	������ HP�� ���� �� �ִ� ����ġ�� �����մϴ�.
	this->GetUIMan()->GetInterfaceString( eST_PARTY_MEMBER_ENTER_NOTICE_2, szMessage, INTERFACE_STRING_LENGTH );
	this->PrintInterfaceMessage(szMessage);



    this->SetMemberInfo(NULL, 0);
    this->ResetHPGauge();

    for (i=1; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        uiPartyMember* partyMember =
            static_cast<uiPartyMember *>(this->GetUIMan()->GetUserInterface(UI_PARTY_LEADER + i));

        if (partyMember)
        {
            partyMember->SetMemberInfo(NULL, 0);
            partyMember->ResetHPGauge();
        }
    }

    int addMemberCounts = 0;

    for (i=0; i<this->m_PartyInfos->m_byMemberNum; i++)
    {
        if (this->m_PartyInfos->m_MemberInfo[i].m_bMaster)
        {
            // ��Ƽ��
            this->SetMemberInfo(&this->m_PartyInfos->m_MemberInfo[i], 0);
            this->ResetHPGauge();

            if (g_pHero->GetObjectKey() == this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
            {
                this->SetLeader();
            }
        }
        else
        {
            uiPartyMember* partyMember =
                static_cast<uiPartyMember *>(this->GetUIMan()->GetUserInterface(UI_PARTY_MEMBER1 + addMemberCounts));

            if (partyMember)
            {
                partyMember->SetMemberInfo(&this->m_PartyInfos->m_MemberInfo[i], (addMemberCounts+1));
                partyMember->ResetHPGauge();
                if (g_pHero->GetObjectKey() == this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
                    partyMember->SetOwned();
                else
                    partyMember->UnsetOwnded();

                addMemberCounts ++;
            }
        }
    }
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::RemoveMemberInfo(DWORD dwObjKey)
{
#ifdef _CK_RENEW_PARTY
#else
    TCHAR	szText[INTERFACE_STRING_LENGTH], szMessage[INTERFACE_STRING_LENGTH];

    ASSERT (this->m_PartyInfos);
    if (!this->m_PartyInfos)
        return;

    PARTY_MEMBER_INFO	temp_MemberInfo[MAX_PARTYMEMBER_NUM];

    int i;

    for (i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        memset ((PARTY_MEMBER_INFO *)&temp_MemberInfo[i],
                0,
                sizeof (PARTY_MEMBER_INFO));
    }

    for (i=0; i<this->m_PartyInfos->m_byMemberNum; ++i)
    {
        memcpy ((PARTY_MEMBER_INFO *)&temp_MemberInfo[i],
                (PARTY_MEMBER_INFO *)&this->m_PartyInfos->m_MemberInfo[i],
                sizeof (PARTY_MEMBER_INFO));
    }

    int removerIdx = -1;

    // ��Ƽ ����ü���� �����
    for (i=0; i<m_PartyInfos->m_byMemberNum; ++i)
    {
        PARTY_MEMBER_INFO* memInfo =
            &this->m_PartyInfos->m_MemberInfo[i];

        if (memInfo->m_dwObjKey == dwObjKey)
        {
            removerIdx = i;
        }
    }

    if (removerIdx == -1)
        return;

    for (i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        memset ((PARTY_MEMBER_INFO *)&this->m_PartyInfos->m_MemberInfo[i],
                0,
                sizeof (PARTY_MEMBER_INFO));
    }

    int addingCounts = 0;

    for (i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        if (i==removerIdx)
            continue;

        memcpy ((PARTY_MEMBER_INFO *)&this->m_PartyInfos->m_MemberInfo[addingCounts],
                (PARTY_MEMBER_INFO *)&temp_MemberInfo[i],
                sizeof (PARTY_MEMBER_INFO));

        addingCounts ++;
    }

    this->m_PartyInfos->m_byMemberNum = this->m_PartyInfos->m_byMemberNum - 1;

    // ��Ƽ���� ���ܵǾ��� ��� ������ LEAVE �޽����� ������.
    if (g_pHero
        && g_pHero->GetObjectKey() == dwObjKey)
    {
        this->ComebackToVillage();
        this->PrintInterfaceMessage(eST_PARTY_LEAVED);
    }
    else
    {
        // %s ���� ��Ƽ�� �����̽��ϴ�. ������ HP�� ���� �� �ִ� ����ġ�� �����մϴ�.
        //this->GetUIMan()->GetInterfaceString( eST_PARTY_LEAVE_NOTICE, szText, INTERFACE_STRING_LENGTH );
        //Sprintf(szMessage, szText, temp_MemberInfo[removerIdx].m_szCharName);
        //this->PrintInterfaceMessage(szMessage);


		//	%s ���� ��Ƽ�� �����̽��ϴ�.
		this->GetUIMan()->GetInterfaceString( eST_PARTY_MEMBER_LEAVE_NOTICE_1, szText, INTERFACE_STRING_LENGTH );
		Sprintf (szMessage, szText, temp_MemberInfo[removerIdx].m_szCharName);
		this->PrintInterfaceMessage(szMessage);

		//	������ HP�� ���� �� �ִ� ����ġ�� �����մϴ�.
		this->GetUIMan()->GetInterfaceString( eST_PARTY_MEMBER_LEAVE_NOTICE_2, szMessage, INTERFACE_STRING_LENGTH );
		this->PrintInterfaceMessage(szMessage);
    }

    this->SetMemberInfo(NULL, 0);
    this->ResetHPGauge();

    for (i=1; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        uiPartyMember* partyMember =
            static_cast<uiPartyMember *>(this->GetUIMan()->GetUserInterface(UI_PARTY_LEADER + i));

        if (partyMember)
        {
            partyMember->SetMemberInfo(NULL, 0);
            partyMember->ResetHPGauge();
        }
    }

    int addMemberCounts = 0;

    for (i=0; i<this->m_PartyInfos->m_byMemberNum; i++)
    {
        if (this->m_PartyInfos->m_MemberInfo[i].m_bMaster)
        {
            // ��Ƽ��
            this->SetMemberInfo(&this->m_PartyInfos->m_MemberInfo[i], 0);
            this->ResetHPGauge();

            if (g_pHero->GetObjectKey() == this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
            {
                this->SetLeader();
            }
        }
        else
        {
            uiPartyMember* partyMember =
                static_cast<uiPartyMember *>(this->GetUIMan()->GetUserInterface(UI_PARTY_MEMBER1 + addMemberCounts));

            if (partyMember)
            {
                partyMember->SetMemberInfo(&this->m_PartyInfos->m_MemberInfo[i], (addMemberCounts+1));
                partyMember->ResetHPGauge();
                if (g_pHero->GetObjectKey() == this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
                    partyMember->SetOwned();
                else
                    partyMember->UnsetOwnded();

                addMemberCounts ++;
            }
        }
    }
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::Migration(DWORD dwObjKey)
{
#ifdef _CK_RENEW_PARTY
#else
    ASSERT(g_pHero && "��Ƽ: Hero ������ �����ϴ�.");

    if (!g_pHero)
        return;

    if (!this->m_PartyInfos)
        return;

    this->m_bLeader = false;

    int idxOrgMaster = -1;
    int idxNewMaster = -1;

    int i;

    for (i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        if (!this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
            continue;

        if (this->m_PartyInfos->m_MemberInfo[i].m_bMaster)
        {
            idxOrgMaster = i;
            this->m_PartyInfos->m_MemberInfo[i].m_bMaster = false;
            break;
        }
    }

    for (i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        if (!this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
            continue;

        if (this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey == dwObjKey)
        {
            idxNewMaster = i;
            this->m_PartyInfos->m_MemberInfo[i].m_bMaster = true;
            break;
        }
    }

    if (idxOrgMaster == -1)
    {
        // ��Ƽ���� ���� �����ȵ�.
    }
    else
    {
        ASSERT (idxNewMaster != -1);
        if (idxNewMaster == -1)
        {
    #ifdef _DEBUG
            this->GetUIMan()->Sys_VerifyLock (_T("��Ƽ�� ���� ���ӿ��� �ɰ��� ���� �߻�"), FALSE);
    #endif
            return;
        }

        PARTY_MEMBER_INFO temp_MemberInfo;

        memcpy ((PARTY_MEMBER_INFO *)&temp_MemberInfo,
                (PARTY_MEMBER_INFO *)&this->m_PartyInfos->m_MemberInfo[idxNewMaster],
                sizeof (PARTY_MEMBER_INFO));

        memcpy ((PARTY_MEMBER_INFO *)&this->m_PartyInfos->m_MemberInfo[idxNewMaster],
                (PARTY_MEMBER_INFO *)&this->m_PartyInfos->m_MemberInfo[idxOrgMaster],
                sizeof (PARTY_MEMBER_INFO));

        memcpy ((PARTY_MEMBER_INFO *)&this->m_PartyInfos->m_MemberInfo[idxOrgMaster],
                (PARTY_MEMBER_INFO *)&temp_MemberInfo,
                sizeof (PARTY_MEMBER_INFO));
    }

    this->SetMemberInfo(NULL, 0);
    this->ResetHPGauge();

    for (i=1; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        uiPartyMember* partyMember =
            static_cast<uiPartyMember *>(this->GetUIMan()->GetUserInterface(UI_PARTY_LEADER + i));

        if (partyMember)
        {
            partyMember->SetMemberInfo(NULL, 0);
            partyMember->ResetHPGauge();
        }
    }

    int addMemberCounts = 0;

    for (i=0; i<this->m_PartyInfos->m_byMemberNum; i++)
    {
        if (this->m_PartyInfos->m_MemberInfo[i].m_bMaster)
        {
            // ��Ƽ��
            this->SetMemberInfo(&this->m_PartyInfos->m_MemberInfo[i], 0);
            this->ResetHPGauge();

            if (g_pHero->GetObjectKey() == this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
            {
                this->SetLeader();
            }
        }
        else
        {
            uiPartyMember* partyMember =
                static_cast<uiPartyMember *>(this->GetUIMan()->GetUserInterface(UI_PARTY_MEMBER1 + addMemberCounts));

            if (partyMember)
            {
                partyMember->SetMemberInfo(&this->m_PartyInfos->m_MemberInfo[i], (addMemberCounts+1));
                partyMember->ResetHPGauge();
                if (g_pHero->GetObjectKey() == this->m_PartyInfos->m_MemberInfo[i].m_dwObjKey)
                    partyMember->SetOwned();
                else
                    partyMember->UnsetOwnded();

                addMemberCounts ++;
            }
        }
    }
#endif
}

//------------------------------------------------------------------------------
/**
    �ʴ��ϱ�
*/
void
uiPartyLeader::Invite(TCHAR *pname)
{
#ifdef _CK_RENEW_PARTY
#else
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    if (!this->IsLeader())
    {
		//	�ʴ�� ��Ƽ���� ���� �����Դϴ�.
		g_InterfaceManager.GetInterfaceString( eST_PARTY_INVITE_REQUEST, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
        return;
    }

    if (!g_pHero)
        return;

	int		iRet;
#ifdef	UNICODE
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, g_pHero->GetCharInfo()->m_pszCharName, MAX_CHARNAME_LENGTH, szMessage, MAX_CHARNAME_LENGTH );
	iRet = _tcscmp(szMessage, pname);
#else
	iRet = strcmp(g_pHero->GetCharInfo()->m_tszCharName, pname);
#endif
    if (!iRet)
    {
		//	�ʴ�� Ÿ�θ� ��ȿ�մϴ�.
		g_InterfaceManager.GetInterfaceString( eST_PARTY_INVITE_REQUEST_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return;
    }

    MSG_CG_PARTY_INVITE_REQUEST_SYN sendMsg;
    sendMsg.m_byCategory    = CG_PARTY;
    sendMsg.m_byProtocol    = CG_PARTY_INVITE_REQUEST_SYN;
#ifdef	UNICODE
	WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, pname, -1, sendMsg.m_szCharName, MAX_CHARNAME_LENGTH, NULL, NULL );
#else
	StrCopy (sendMsg.m_szCharName, pname);
#endif

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_INVITE_REQUEST_SYN) );
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::AskInviting(const TCHAR *str, DWORD dwObjKey)
{
    g_InterfaceManager.ShowInterfaceWithReload(UI_PARTY_INVITING_POPUP, TRUE, TRUE);

    uiPartyInvitingPopup *partyInvitingPopup =
        static_cast<uiPartyInvitingPopup *>(this->GetUIMan()->GetUserInterface(UI_PARTY_INVITING_POPUP));

    if (partyInvitingPopup)
    {
        partyInvitingPopup->ShowInterface(TRUE);
        partyInvitingPopup->SetInviterName(str);
        partyInvitingPopup->SetInviterKey(dwObjKey);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::ComebackToVillage()
{
	BattleScene::BackToTheVillageSyn();
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::SetTargeting()
{
    if (!this->m_memInfo.IsFill())
        return;

    TargetDialog * pTargetDlg =
        static_cast<TargetDialog *>(this->GetUIMan()->GetDialog(InterfaceManager::DIALOG_TARGET));

    if (!pTargetDlg)
        return;

    g_HeroInput.SetCurrentTarget(this->m_memInfo.GetInfo().m_dwObjKey);

    //if (pTargetDlg->GetTargetKey() != this->m_memInfo->m_dwObjKey)
    //{
    //    pTargetDlg->SetTargetKey(this->m_memInfo->m_dwObjKey);
    //    pTargetDlg->ShowWindow(TRUE);		
    //}
}

//------------------------------------------------------------------------------
/**
*/
bool
uiPartyLeader::Assist_SetPrimaryTarget(DWORD dwTarget)
{
    if (!g_pHero)
        return false;

    if (!this->m_PartyInfos)
        return false;

    if (!this->m_memInfo.IsFill())
        return false;

    // ��Ƽ���� �ƴҰ�� �켱 Ÿ�� ���� �ȵ�
    if (!this->m_memInfo.GetInfo().m_bMaster)
        return false;

    if (!dwTarget)
    {
        TargetDialog * pTargetDlg =
            static_cast<TargetDialog *>(this->GetUIMan()->GetDialog(InterfaceManager::DIALOG_TARGET));

        if (!pTargetDlg)
            return false;

        Monster *pMonster = (Monster *)g_ObjectManager.GetObject(pTargetDlg->GetTargetKey());

        if (!pMonster)
            return false;

        MSG_CG_PARTY_SELECT_TARGET_SYN sendPacket;
        sendPacket.m_dwObjectKey = pMonster->GetObjectKey();

        GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendPacket, sizeof (MSG_CG_PARTY_SELECT_TARGET_SYN) );
    }
    else
    {
        if (this->Is_AssistPartyLeader())
        {
            // �ߺ� Ÿ���� ���
            if (this->Get_PartyLeaderTargetKey() == dwTarget)
                return false;
        }

        Monster *pMonster = (Monster *)g_ObjectManager.GetObject(dwTarget);

        if (!pMonster)
            return false;

        MSG_CG_PARTY_SELECT_TARGET_SYN sendPacket;
        sendPacket.m_dwObjectKey = pMonster->GetObjectKey();

        GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendPacket, sizeof (MSG_CG_PARTY_SELECT_TARGET_SYN) );
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::NET_RECV_CG_PARTY_SELECT_TARGET_BRD(MSG_CG_PARTY_SELECT_TARGET_BRD *msg)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	//	�ƹ���
	const int	c_iPartyLeaderName = 70395;
	g_InterfaceManager.GetInterfaceString( c_iPartyLeaderName, szMessage, INTERFACE_STRING_LENGTH );
    util::_tstring strPartyLeaderName = szMessage;

	//	�����ΰ�
	const int	c_iTargetMonsterName = 70396;
	g_InterfaceManager.GetInterfaceString( c_iTargetMonsterName, szMessage, INTERFACE_STRING_LENGTH );
    util::_tstring strTargetMonsterName = szMessage;

    Monster *pMonster = (Monster *)g_ObjectManager.GetObject(msg->m_dwObjectKey);
    if (pMonster)
    {
        strTargetMonsterName = pMonster->GetName();
        this->Set_PartyLeaderTargetKey(msg->m_dwObjectKey);
        this->Set_AssistPartyLeader(true);
    }

    if (this->m_memInfo.IsFill())
	{
#ifdef	UNICODE
		MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, this->m_memInfo.GetInfo().m_szCharName, MAX_CHARNAME_LENGTH, szMessage, MAX_CHARNAME_LENGTH );
		strPartyLeaderName = szMessage;
#else
		strPartyLeaderName = this->m_memInfo.GetInfo().m_szCharName;
#endif
	}

	//	��Ƽ���� '%s' �� '%s' �� �켱 ���� ������� �����Ͽ����ϴ�.
	g_InterfaceManager.GetInterfaceString( eST_SET_ASSISTPARTYLEADER, szMessage, INTERFACE_STRING_LENGTH );
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, strPartyLeaderName.c_str(), strTargetMonsterName.c_str() );

    if (!this->m_bAssistHelpOnce && !this->IsLeader())
    {
		//	��Ƽ���� �󱼿� �������� Ŭ���Ͻø�
		g_InterfaceManager.GetInterfaceString( eST_SET_ASSISTPARTYLEADER_CLICK_FACE, szMessage, INTERFACE_STRING_LENGTH );
        this->PrintInterfaceMessage(szMessage);
		//	��Ƽ���� Ÿ���� �����ϰ� �˴ϴ�.
		g_InterfaceManager.GetInterfaceString( eST_SET_ASSISTPARTYLEADER_SHARE_TARGET, szMessage, INTERFACE_STRING_LENGTH );
        this->PrintInterfaceMessage(szMessage);

        this->m_bAssistHelpOnce = true;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
uiPartyLeader::Assist_SetTargetToLeader()
{
    if (!g_pHero)
        return false;

    if (!this->Is_AssistPartyLeader())
        return false;

    if (!this->Get_PartyLeaderTargetKey())
        return false;

    TargetDialog * pTargetDlg =
        static_cast<TargetDialog *>(this->GetUIMan()->GetDialog(InterfaceManager::DIALOG_TARGET));

    if (!pTargetDlg)
        return false;

    g_HeroInput.SetCurrentTarget(this->Get_PartyLeaderTargetKey());

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::Set_NextTarget()
{
    if (this->Is_AssistPartyLeader())
        return;

    if (this->m_PartyInfos->m_byMemberNum >= this->m_idxCurrentFreeTargetAssistMember)
    {
        this->m_idxCurrentFreeTargetAssistMember = 0;
    }

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    Player *pPlayer = (Player *)g_ObjectManager.GetObject(this->m_PartyInfos->m_MemberInfo[this->m_idxCurrentFreeTargetAssistMember].m_dwObjKey);
    if (pPlayer)
    {
        Object* obj = pPlayer->GetTargetObject();
        if (obj)
        {
			//	�ƹ���
			const int	c_iPartyMemberName = 70395;
			g_InterfaceManager.GetInterfaceString( c_iPartyMemberName, szMessage, INTERFACE_STRING_LENGTH );
			util::_tstring strPartyMemberName = szMessage;

			//	�����ΰ�
			const int	c_iTargetMonsterName = 70396;
			g_InterfaceManager.GetInterfaceString( c_iTargetMonsterName, szMessage, INTERFACE_STRING_LENGTH );
			util::_tstring strTargetMonsterName = szMessage;

            Monster *pMonster = (Monster *)g_ObjectManager.GetObject(obj->GetObjectKey());
            if (pMonster)
                strTargetMonsterName = pMonster->GetName();

            strPartyMemberName = pPlayer->GetName();

			TCHAR	szMessage[INTERFACE_STRING_LENGTH];

			//	��Ƽ�� '%s' �� Ÿ�����ϰ� �ִ� '%s' �� ���� ��ý�Ʈ�� �����Ͽ����ϴ�.
			g_InterfaceManager.GetInterfaceString( eST_ASSIST_START, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, strPartyMemberName.c_str(), strTargetMonsterName.c_str());

            g_HeroInput.SetCurrentTarget(obj->GetObjectKey());
            this->m_idxCurrentFreeTargetAssistMember++;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::ClearMemberKeys()
{
    m_vdwMemberKeys.clear();
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::AddMemberKey(DWORD dwKey)
{
    bool has = false;

    for (int i=0; i<this->m_vdwMemberKeys.size(); ++i)
    {
        if (this->m_vdwMemberKeys[i] == dwKey)
            has = true;
    }

    if (!has)
        this->m_vdwMemberKeys.push_back(dwKey);
}

//------------------------------------------------------------------------------
/**
*/
const std::vector<DWORD>&
uiPartyLeader::GetMemberKeys()
{
    return this->m_vdwMemberKeys;
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::OnChangeInfo()
{
	g_InterfaceManager.UpdatePartyHeadTexture(0);
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
