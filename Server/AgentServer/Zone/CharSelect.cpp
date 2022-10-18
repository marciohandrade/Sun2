#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <UserSessions/User.h>
#include "CharSelect.h"
#include "PortalManager.h"

#include <AgentParty/AgentPartyManager.h>

_IMPL_POOL(CharSelect)
CharSelect::CharSelect(void)
{
}

CharSelect::~CharSelect(void)
{
}

VOID
CharSelect::Create(KEYTYPE key)
{
	SetKey(key);
    SetMapCode(11000);  // (WAVERIX) (090520) (CHANGES) non-changed hard-code value
	SetLinkedServer(NULL);

	On_create();
}

VOID
CharSelect::On_create()
{
    notification_timer_updater_.SetTimer(10 * 1000);
}

VOID
CharSelect::On_destroy()
{
}

VOID
CharSelect::On_readyuser(User* pUser)
{
	pUser->SetZoneState(ePRS_BEFORE_ENTER_CHARSELECT);
	pUser->SetZoneKey(GetKey());
	pUser->SetZoneType(eZONETYPE_CHARSELECT);
    pUser->ChangeHeIsHereNotificationTimer(false); // NOTE: f110622.2L
}

VOID
CharSelect::On_enteruser(User* pUser)
{
	ZoneInterface::On_enteruser(pUser);

	ReadyUser(pUser, eSEND_NONE);

	pUser->SetZoneState(ePRS_AT_CHARSELECT);

	// 방에 들어올때 파티관련 처리를 한다.
	AgentPartyManager::Instance()->EnterZone(pUser, this);

	// 선택한 캐릭터를 초기화한다.
	pUser->SelectChar(MAX_CHARACTER_LIST_NUM);
    PortalManager::ClearUserInfo(pUser);
    pUser->SetReservedHP(0);
}

VOID
CharSelect::On_leaveuser(User* pUser)
{
    if (pUser->GetPartyState().IsPartyMember() == true)
    {
        AgentPartyManager::Instance()->LeaveZone(pUser, this);
    }
}

// NOTE: f110622.2L, when an user return to the character selection zone,
// the user may rarely miss the activated updating server in case the user that was leaving
// on a field of a game server. this protocol would notify the database proxy server
// in order to refresh the zombie user detection timeout.
BOOL CharSelect::Process()
{
    if (notification_timer_updater_.IsExpired() == false) {
        return true;
    };
    //
    const ZONE_USER_LIST& user_list = this->GetUserList();
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& node, user_list, ZONE_USER_LIST)
    {
        User* const user = node.second;
        user->NotifyHeIsHereIfSatisfied();
    };
    return true;
}
