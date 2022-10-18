#include <stdafx.h>
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "SonnenScheinRoom.h"

#include <Zone/ZoneManager.h>
#include <Zone/InstanceDungeon.h>
#include <UserSessions/User.h>

SonnenScheinRoom::~SonnenScheinRoom()
{
    Release();
}

VOID SonnenScheinRoom::KickAllUsers()
{
    ZoneInterface* dungeon = g_ZoneManager.FindZone(room_key_);
    if (NULL == dungeon) {
        return;
    }

    const ZoneInterface::ZONE_USER_LIST& user_list = dungeon->GetUserList();
    ZoneInterface::ZONE_USER_LIST::const_iterator itr = user_list.begin();

    while (itr != user_list.end())
    {
        User* user = itr->second;
        ++itr;
        _KickUser(user);
    }
}

VOID SonnenScheinRoom::_KickUser(User* user)
{
    if(user->IsBeginTransaction())
    {
        SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"] User State Transaction!"));
        return;
    }

    //돌아갈 곳
    MAPCODE return_map_code = user->GetSelectedCharMapCode();

    ZoneInterface* return_map = NULL;
    return_map = (ZoneInterface*)g_ZoneManager.FindVillage(return_map_code);
    if(return_map == NULL)
    {
        return_map = (ZoneInterface*)g_ZoneManager.FindHuntingField(return_map_code);
        if(return_map == NULL) {
            return;
        }
    }

    CheckMoveZonePolicy* const pCheckMoveZonePolicy =
        MoveZonePolicy<MZTF_ROOM_TO_STATIC>::AlignInfo(user,
        //{		From		|			To		}
          eZONETYPE_INSTANCE,   return_map->GetType()   //	eZONETYPE
        , user->GetZoneKey(),	return_map->GetKey()	//	ZoneKey
        );

    const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());
    if(RC::RC_ROOM_SUCCESS == rt)
    {
        user->BeginTransaction(TR_LEAVE_INSTANCE_DUNGEON);
        user->SetTRRoomKey(return_map->GetKey());

        GAMELOG->MoveVillage(user, return_map->GetMapCode(), return_map_code, TRUE);
    }
    else {
        SUNLOG(eFULL_LOG,  "["__FUNCTION__"] LeaveRoom Error(%d) ", rt);
    }
}

VOID SonnenScheinRoom::Release()
{
    KickAllUsers();

    InstanceDungeon* dungeon = static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(room_key_));
    if (dungeon != NULL) {
        dungeon->SetRoomState(eROOMSTATE_DESTROY);
    }
}

INT SonnenScheinRoom::GetPlayerList_CharGuid(CHARGUID* list OUT)
{
    ZoneInterface* dungeon = g_ZoneManager.FindZone(room_key_);
    if (NULL == dungeon) {
        return 0;
    }

    const ZoneInterface::ZONE_USER_LIST& user_list = dungeon->GetUserList();
    ZoneInterface::ZONE_USER_LIST::const_iterator itr = user_list.begin();

    INT player_count = 0;
    while (itr != user_list.end())
    {
        User* user = itr->second;
        if (user == NULL) {
            continue;
        }

        list[player_count] = user->GetSelectedCharGuid();

        ++itr;
        ++player_count;
    }

    return player_count;
}

User* SonnenScheinRoom::FindUser( DWORD player_key )
{
    ZoneInterface* dungeon = g_ZoneManager.FindZone(room_key_);
    if (NULL == dungeon) {
        return NULL;
    }

    return dungeon->FindUser(player_key);
}

#endif //_NA_008334_20150608_SONNENSCHEIN
