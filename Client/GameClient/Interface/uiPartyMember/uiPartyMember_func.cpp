//------------------------------------------------------------------------------
//  uiPartyMember_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiPartyMan/uiPartyMan_def.h"
#include "ObjectManager.h"
#include "uitargetman/uitargetman.h"
#include "uiPartyMember.h"
#include "TargetDialog.h"
#include "HeroActionInput.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"
#include "Monster.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "GameFramework.h"
#include "SceneBase.h"
//------------------------------------------------------------------------------
/**
*/
uiPartyMan*
uiPartyMember::GetManager()
{
	uiPartyMan* partyMan =
		static_cast<uiPartyMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_PARTY));
	assert (partyMan);
	return partyMan;
}
//------------------------------------------------------------------------------
/**
*/
short
uiPartyMember::GetMyDlgPos()
{
    return (short)(m_nGap + 1);
}

//------------------------------------------------------------------------------
/**
*/
const uicPartyMemberInfoPlus&
uiPartyMember::GetMyMemberInfo()
{
	assert (this->GetManager());
	const uicPartyMemberInfoPlus& memInfo = this->GetManager()->GetParty(this->GetMyDlgPos());
	return memInfo;
}
//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::TargetShare(bool bAttack)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	DWORD assistKey;

	const uicPartyMemberInfoPlus& memInfo = this->GetMyMemberInfo();

	Character* pChar = (Character*)g_ObjectManager.GetObject(memInfo.Get().m_dwMemberKey);
    if (!pChar)
    {
        return;
    }

    Character* pTargetChar = (Character*)pChar->GetTargetObject();
    if (!pTargetChar)
    {
        return;
    }
    
    assistKey = pTargetChar->GetObjectKey();

    Monster* pMonster = (Monster*)g_ObjectManager.GetObject(assistKey);
    if (pMonster)
    {
        util::_tstring strPartyMemberName = memInfo.Get().m_szCharName;
		// 5922	'%s'의 타겟을 어시스트합니다.
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString(5922, szTemp, INTERFACE_STRING_LENGTH);
        Snprintf (szMessage,INTERFACE_STRING_LENGTH-1, szTemp, strPartyMemberName.c_str());	
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

        if (bAttack)
        {
		    g_HeroInput.OnLClickedCharacter( pTargetChar, GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pTargetChar, g_HeroInput.lsForceAttack() ), false );   
        }
        else
        {
            g_HeroInput.SetCurrentTarget(assistKey);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::SetTargeting()
{
    TargetDialog* pTargetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);

    if (!pTargetDlg)
    {
        return;
    }

	const uicPartyMemberInfoPlus& memInfo = GetMyMemberInfo();
	if (!memInfo.IsFill()) 
    {
        return;
    }

	DWORD compKey = memInfo.Get().m_dwMemberKey;

//파티멤버가 멀리있거나 같은지역(포탈이동)없을시에도 클릭하도록수정
	g_HeroInput.ClickPartyMember(compKey);

}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::doLeave()
{
	uiBase * partyLeavePopup = GET_DIALOG( IM_PARTY_MANAGER::PARTY_LEAVE_POPUP );

    if (partyLeavePopup)
    {
        partyLeavePopup->ShowInterface(TRUE);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::OnChangeInfo()
{
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
