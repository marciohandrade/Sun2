#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <PacketStruct_AG.h>
#include <PacketStruct_AW.h>

//////////////////////////////////////////////////////////////////////////
#include <UserSessions/User.h>
#include ".\HuntingField.h"
#include <AgentParty/AgentPartyManager.h>
#include <AgentParty/AgentParty.h>

_IMPL_POOL(HuntingField)
HuntingField::HuntingField(void)
{
}

HuntingField::~HuntingField(void)
{
}

VOID
HuntingField::Create(KEYTYPE key, MAPCODE mapCode, ServerSession* pServerSession)
{
	SetKey(key);
	SetMapCode(mapCode);
	SetLinkedServer(pServerSession);

	On_create();
}

VOID
HuntingField::On_create()
{
	MSG_AG_ZONE_FIELD_CREATE_CMD msg;
	msg.m_dwKey		= 0;
	msg.m_MapCode	= GetMapCode();
	msg.m_RoomKey	= GetKey();
	SendToLinkedServer(&msg, sizeof(msg));
}

VOID
HuntingField::On_destroy()
{
}

VOID
HuntingField::On_readyuser(User* pUser)
{
	pUser->SetZoneState(ePRS_BEFORE_ENTER_FIELD);
	pUser->SetZoneKey(GetKey());
	pUser->SetZoneType(eZONETYPE_FIELD);
}

VOID
HuntingField::On_enteruser(User* pUser)
{
	ZoneInterface::On_enteruser(pUser);

	pUser->SetZoneState(ePRS_AT_FIELD);
	pUser->SetSelectedCharMapCode(GetMapCode());

	// 방에 들어올때 파티관련 처리를 한다.
	AgentPartyManager::Instance()->EnterZone(pUser, this);
}

VOID
HuntingField::On_leaveuser(User* pUser)
{
    if (pUser->GetPartyState().IsPartyMember())
    {
        AgentPartyManager::Instance()->LeaveZone(pUser, this);
    }
}

RC::eROOM_RESULT
HuntingField::_CanJoinEach(User* pUser, eZONEPUBLIC roomPublic, TCHAR* pszRoomPWD)
{
	// 파티원 수가 MAX_FIELD_PARTYMEMBER_NUM 이상이면 들어올 수 없다.
	DWORD MemberNum = AgentPartyManager::Instance()->GetMemberNum(pUser->GetPartyState().GetPartyKey());
	if(MemberNum > MAX_FIELD_PARTYMEMBER_NUM)
		return RC::RC_ROOM_PARTY_MEMBER_ACCESS;

	return RC::RC_ROOM_SUCCESS;
}
