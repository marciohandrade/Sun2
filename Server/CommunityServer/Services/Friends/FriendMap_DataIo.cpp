#include "WorldServerPch.hxx"
#include "./FriendMap.h"

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING

#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
#include "Channel/WorldChannel.h"

#include "Services/Friends/FriendInvitation.h"

//==================================================================================================

bool ns_world::FriendMap::StoreTotalFriendContentsList(ns_world::WorldUser* const owner,
    const ns_functor::DBFriendsListFilter::MemberInfoArray& friend_array, size_t number_of_nodes)
{
    // NOTE: f110817.3L, MSG_DW_FRIEND_GETLIST_ACK event handling...
    //
    const CHARGUID owner_char_guid = owner->GetConnectedCharInfo().char_guid;
    //ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();

    const STRUCT_FRIEND_DB_INFO* friend_it = friend_array,
                               * const friend_end = &friend_array[number_of_nodes];
    for (; friend_it != friend_end; ++friend_it)
    {
        //ns_world::WorldUser* related_user = NULL;
        switch (friend_it->Sts)
        {
        //_KR_0_20091021_FIRENDINVITATION_EXTEND //친구 추가
        case friend_it->INFO_INVIAT:
        case friend_it->INFO_BEINVIATED:
        //
        case friend_it->INFO_FRIEND:
            {
                STRUCT_FRIEND_INFO friend_data = { 0, };
                STRUCT_FRIEND_PACKET_INFO* const sub_data = &friend_data.sPacketInfo;
                //related_user = user_manager->FindUserByCharGuid(friend_it->dwFriendGuid);
                //if (related_user != NULL) {
                //    *online_friend_users_end++ = related_user;
                //};
#if SUN_CODE_BACKUP
                if (bool online = (related_user && !FindBlockChar(related_user, owner_char_guid)))
                {
                    sub_data->bOnline = online;
                    sub_data->byChannel = static_cast<uint8_t>(\
                        related_user->GetUserAccountInfo().channel->GetChannelID());
#if WORLD_CONTENTS_BLOCK
                    // 실제 친구 데이터와 다를 수 있으므로 업데이트
                    related_user->CheckOnlineLevel(friend_it->Level);
                    related_user->AddOnlineFriend(owner, false); // 아직 실제 접속 상황이 아니므로 2번째 매개변수 false 세팅
#endif
                }
                else
                {
                    //sub_data->bOnline = online;
                    //sub_data->byChannel = 0;
                };
#endif //SUN_CODE_BACKUP
                friend_data.dwFriendGuid = friend_it->dwFriendGuid;
                _tcsncpy(sub_data->ptszFriendName, friend_it->ptszFriendName,
                         _countof(sub_data->ptszFriendName));
                sub_data->ptszFriendName[_countof(sub_data->ptszFriendName) - 1] = _T('\0');
                sub_data->byClass = friend_it->byClass;
                sub_data->Level = friend_it->Level;
                //
                this->InsertNewFriend(owner, friend_data);
                //_KR_0_20091021_FIRENDINVITATION_EXTEND //친구 추가
                ns_world::FriendInvitationInWorld::Instance()->OnAddFriend(\
                    friend_data, *friend_it, owner);
            };
            break;
        case friend_it->INFO_BLOCK:
            {
                STRUCT_BLOCK_INFO block_info = { 0, };
                block_info.dwBlockGuid = friend_it->dwFriendGuid;
                _tcsncpy(block_info.ptszBlockName, friend_it->ptszFriendName,
                         _countof(block_info.ptszBlockName));
                block_info.ptszBlockName[_countof(block_info.ptszBlockName) - 1] = _T('\0');
                //
                this->InsertNewBlockChar(owner, block_info);
            };
            break;
        case friend_it->INFO_REVENGE:
            {
                STRUCT_REVENGE_INFO revenge_data = { 0, };
                STRUCT_REVENGE_PACKET_INFO* const sub_data = &revenge_data.sPacketInfo;

                //related_user = user_manager->FindUserByCharGuid(friend_it->dwFriendGuid);
                //if (related_user != NULL) {
                //    *online_revenge_users_end++ = related_user;
                //};
#if SUN_CODE_BACKUP
                if (bool online = (related_user != NULL))
                {
                    sub_data->bOnline = online;
                    sub_data->byChannelID = static_cast<uint8_t>(\
                        related_user->GetUserAccountInfo().channel->GetChannelID());
#if WORLD_CONTENTS_BLOCK
                    // 실제 데이터와 다를 수 있으므로 업데이트
                    related_user->CheckOnlineLevel(friend_it->Level);
                    related_user->AddOnlineRevenge(owner, false); // 아직 실제 접속 상황이 아니므로 2번째 매개변수 false 세팅
#endif
                }
                else
                {
                    //sub_data->bOnline = online;
                };
#endif //SUN_CODE_BACKUP
                revenge_data.dwRevengeCharGuid = friend_it->dwFriendGuid;
                _tcsncpy(sub_data->ptszRevengeName, friend_it->ptszFriendName,
                         _countof(sub_data->ptszRevengeName));
                sub_data->ptszRevengeName[_countof(sub_data->ptszRevengeName) - 1] = _T('\0');
                sub_data->byClass = friend_it->byClass;
                sub_data->Level = friend_it->Level;
                //
                this->InsertNewRevengeChar(owner, revenge_data);
            };
            break;
        case friend_it->INFO_REVENGE_POINT:
            {
                ns_world::RevengeLink* const editable_revenge_link = \
                    const_cast<ns_world::RevengeLink*>(&owner->GetRevengeInfo());
                editable_revenge_link->revenge_point_ = friend_it->RevengePoint;
            };
            break;
        default:
            {
                MessageOut(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=invalid friend node type(%d)|")
                    _T("CharGuid=%d, CharName='%s'|"),
                    friend_it->Sts, owner_char_guid, owner->GetCharName());
                //2010-01-05 문상현 리턴을 주석 처리 한다. (리스트 하나 잘못 되었다고 전체 친구목록이 이상해 지면 안된다)
                //return;
            };
            break;
        }; //end 'switch'
        //
    }; //end 'foreach friend_info in friend_info_list'
    // NOTE: notify to related users
    this->NotifyOnlineStatusChangedFriendData(owner, true);
    //
#if WORLD_CONTENTS_BLOCK
    // updates...
    ns_world::WorldUserManager::Instance()->AddFriend(owner);   // 친구 목록 업데이트
    ns_world::WorldUserManager::Instance()->AddRevenge(owner);
#endif
    return true;
};

bool ns_world::FriendMap::LoadFriendList(ns_world::WorldUser* user,
    ns_functor::FriendListFilter::MemberInfoArray& result_friend_list,
    uint8_t* const result_number_of_friend_nodes) const
{
    *result_number_of_friend_nodes = 0;
    //
    const ns_world::FriendLink& friend_link = user->GetFriendInfo();
    const FRIEND_INFO_MAP* const friends = friend_link.link_;
    if (friends == NULL) {
        return true;
    };
    const CHARGUID char_guid = user->GetConnectedCharInfo().char_guid;
    if (FlowControl::FCAssert(char_guid != 0) == false) {
        return false;
    };
    BOOST_STATIC_ASSERT(_countof(result_friend_list) == MAX_FRIENDBLOCK_STAT_NUM);
    // WARNING: MAX_FRIENDBLOCK_STAT_NUM (400), numbers field (255)
    // MAX_FRIENDBLOCK_STAT_NUM include the number of friends and the number of blocked_characters
    int number_of_nodes = static_cast<int>(friends->size());
    ;   number_of_nodes = min(_countof(result_friend_list), number_of_nodes);
    ;   number_of_nodes = min((MAX_FRIENDBLOCK_STAT_NUM >> 1), number_of_nodes);

    STRUCT_FRIEND_INFO* dest_it = &result_friend_list[0];
    FRIEND_INFO_MAP::const_iterator it = friends->begin();
    const FRIEND_INFO_MAP::const_iterator end = friends->end();
    int counter = 0;
    for (; counter < number_of_nodes && it != end; ++counter, ++it, ++dest_it)
    {
        const STRUCT_FRIEND_INFO& friend_info = *it->second;
        *dest_it = friend_info;
        //
        const ns_world::CharStatusDataNode* const status_data_node = \
            char_status_data_accessor_->GetCharStatusData(friend_info.dwFriendGuid);
        if ((status_data_node->online_user != NULL) &&
            (this->FindBlockChar(status_data_node->online_user, char_guid)))
        {
            assert(dest_it->sPacketInfo.bOnline != false);
            dest_it->sPacketInfo.bOnline = false;
            dest_it->sPacketInfo.byChannel = 0;
        };
    };

    *result_number_of_friend_nodes = static_cast<uint8_t>(counter);
    return true;
}

bool ns_world::FriendMap::LoadBlockList(ns_world::WorldUser* user,
    ns_functor::BlockCharListFilter::MemberInfoArray& result_blockchar_list,
    uint8_t* const result_number_of_blockchar_nodes) const
{
    *result_number_of_blockchar_nodes = 0;
    //
    const ns_world::BlockCharLink& blockchar_link = user->GetBlockCharInfo();
    const BLOCK_INFO_MAP* const blockchars = blockchar_link.link_;
    if (blockchars == NULL) {
        return true;
    };
    BOOST_STATIC_ASSERT(_countof(result_blockchar_list) == MAX_BLOCK_STAT_NUM);
    int number_of_nodes = static_cast<int>(blockchars->size());
    ;   number_of_nodes = min(_countof(result_blockchar_list), number_of_nodes);

    STRUCT_BLOCK_INFO* dest_it = &result_blockchar_list[0];
    BLOCK_INFO_MAP::const_iterator it = blockchars->begin();
    const BLOCK_INFO_MAP::const_iterator end = blockchars->end();
    int counter = 0;
    for (; counter < number_of_nodes && it != end; ++counter, ++it, ++dest_it)
    {
        const STRUCT_BLOCK_INFO& blockchar_info = *it->second;
        *dest_it = blockchar_info;
    };

    *result_number_of_blockchar_nodes = static_cast<uint8_t>(counter);
    return true;
}


bool ns_world::FriendMap::LoadRevengeList(
    ns_world::WorldUser* user,
    ns_functor::RevengeListFilter::MemberInfoArray& result_revenge_list,
    uint8_t* const result_number_of_revenge_nodes,
    uint8_t* const result_revenge_points) const
{
    *result_number_of_revenge_nodes = 0;
    *result_revenge_points = 0;
    //
    const ns_world::RevengeLink& revenge_link = user->GetRevengeInfo();
    const REVENGE_INFO_MAP* const revenges = revenge_link.link_;
    if (revenges == NULL) {
        return true;
    };
    *result_revenge_points = revenge_link.revenge_point_;
    //
    BOOST_STATIC_ASSERT(_countof(result_revenge_list) == MAX_REVENGE_STAT_NUM);
    int number_of_nodes = static_cast<int>(revenges->size());
    ;   number_of_nodes = min(_countof(result_revenge_list), number_of_nodes);

    STRUCT_REVENGE_INFO* dest_it = &result_revenge_list[0];
    REVENGE_INFO_MAP::const_iterator revenge_it = revenges->begin();
    const REVENGE_INFO_MAP::const_iterator revenge_end = revenges->end();
    int counter = 0;
    for (; counter < number_of_nodes && revenge_it != revenge_end;
         ++counter, ++revenge_it, ++dest_it)
    {
        const STRUCT_REVENGE_INFO& revenge_info = *revenge_it->second;
        *dest_it = revenge_info;
    };

    *result_number_of_revenge_nodes = static_cast<uint8_t>(counter);
    return true;
};

#endif //_NA_0_20091104_FIRENDMAP_REFACTORYING