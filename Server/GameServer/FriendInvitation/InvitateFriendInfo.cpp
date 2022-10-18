#include "stdafx.h"
#include "./InvitateFriendInfo.h"

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
#include "TimeConverter.h"

#include "GFriendInvitationController.h"

#include "PlayerManager.h"
#include "PublicMath.h"

InvitateFriendInfo::InvitateFriendInfo(Player* player) : player_(player)
{
}

InvitateFriendInfo::~InvitateFriendInfo(void)
{
}

VOID InvitateFriendInfo::UpdateReCallTime(CHARGUID FriendGuid, DWORD64 Recalltime)
{
    UpdateReCallTime(FRIEND_INVIATE, FriendGuid, Recalltime);
    UpdateReCallTime(FRIEND_BEINVIATED, FriendGuid, Recalltime);
}

VOID InvitateFriendInfo::UpdateReCallTime(FRIEND_TYPE type, CHARGUID FriendGuid, DWORD64 Recalltime)
{
    INVITATFRIEND_INFO_MAP* const FriendList = GetFriendList(type);    

    INVITATFRIEND_INFO_MAP_ITER itr = FriendList->find(FriendGuid);

    if(FriendList->end() != itr)
    {
        itr->second = Recalltime;
    }
}

VOID InvitateFriendInfo::InitReCallTime(MSG_DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK* pRecvMsg)
{
    for(int i = 0 ; i < pRecvMsg->Count ; i++)
    {
        UpdateReCallTime(FRIEND_INVIATE, pRecvMsg->m_Recall_Info[i].FriendGUID, 
            util::ConvertTMidTimeToDWORD64(pRecvMsg->m_Recall_Info[i].tszRecallTime));
        UpdateReCallTime(FRIEND_BEINVIATED, pRecvMsg->m_Recall_Info[i].FriendGUID, 
            util::ConvertTMidTimeToDWORD64(pRecvMsg->m_Recall_Info[i].tszRecallTime));
    }
}

INVITATFRIEND_INFO_MAP* const InvitateFriendInfo::GetFriendList(FRIEND_TYPE type)
{
	ASSERT(type == FRIEND_INVIATE || type == FRIEND_BEINVIATED || type == FRIEND_INVIATEINPARTY);

	switch(type)
	{
	case FRIEND_INVIATE:        return &m_vecInvitateFriend;
	case FRIEND_BEINVIATED:     return &m_vecBeInvitatedFriend;
    case FRIEND_INVIATEINPARTY: return &m_vecnvitateFriendInParty;
	}
	return NULL;
}

// (CHANGES) (f100515.2L) rule out the possibility of a stack overflow
UINT InvitateFriendInfo::GetFriendList(const FRIEND_TYPE type,
                                       DWORD OUT friend_guid_array[], int number_of_guid_array)
{
    const INVITATFRIEND_INFO_MAP& friend_list = *GetFriendList(type);
    INVITATFRIEND_INFO_MAP::const_iterator it = friend_list.begin(),
                                           end = friend_list.end();
    int number_of_friends = 0;
    for ( ; number_of_friends < number_of_guid_array && it != end; ++number_of_friends, ++it)
    {
        friend_guid_array[number_of_friends] = it->first;
    }
    return number_of_friends;
}

VOID InvitateFriendInfo::AddFriend(FRIEND_TYPE type, DWORD FriendGUID[], WORD FriendCount)
{
    INVITATFRIEND_INFO_MAP* const FriendList = GetFriendList(type);    
    

    for(int i = 0 ; i < FriendCount ; i++)
    {
        FriendList->insert( INVITATFRIEND_INFO_MAP_PAIR( FriendGUID[i], 0 ) );
    }    
}

VOID InvitateFriendInfo::AddFriend(FRIEND_TYPE type, DWORD FriendGUID)
{
    INVITATFRIEND_INFO_MAP* const FriendList = GetFriendList(type);   

    FriendList->insert( INVITATFRIEND_INFO_MAP_PAIR( FriendGUID, 0 ) );
}

VOID InvitateFriendInfo::ClearAllFriendList()
{
    m_vecInvitateFriend.clear();
    m_vecBeInvitatedFriend.clear();
    m_vecnvitateFriendInParty.clear();
}

VOID InvitateFriendInfo::ClearFriendList(FRIEND_TYPE type)
{
    INVITATFRIEND_INFO_MAP* const FriendList = GetFriendList(type);    

    FriendList->clear();
}

VOID InvitateFriendInfo::RemoveFriend(FRIEND_TYPE type, DWORD FriendGUID)
{
    INVITATFRIEND_INFO_MAP* const FriendList = GetFriendList(type);

    INVITATFRIEND_INFO_MAP_ITER itr = FriendList->find(FriendGUID);
    if(FriendList->end() != itr)
    {
        FriendList->erase(itr);
    }
}

int InvitateFriendInfo::GetSize(FRIEND_TYPE type)
{
    INVITATFRIEND_INFO_MAP* const FriendList = GetFriendList(type);

    return FriendList->size();
}

BOOL InvitateFriendInfo::IsInvitateFriend(CHARGUID charguid)
{
    if(m_vecInvitateFriend.end()    != m_vecInvitateFriend.find(charguid)) return TRUE;
    if(m_vecBeInvitatedFriend.end() != m_vecBeInvitatedFriend.find(charguid)) return TRUE;
    return FALSE;
}

BOOL InvitateFriendInfo::IsInvitateFriendParty()
{
    return m_vecnvitateFriendInParty.size() > 0 ? TRUE : FALSE;
}

float InvitateFriendInfo::GetAddExpRate(const BYTE radius, const WzVector* const pwvNpcPos, const FLOAT fRadius_Q2)
{
    INVITATFRIEND_INFO_MAP_ITER iter = m_vecnvitateFriendInParty.begin();

    for(; iter != m_vecnvitateFriendInParty.end() ; ++iter)
    {
        Player* const player = PlayerManager::Instance()->FindPlayerByCharKey(iter->first);

        if(player)
        {
            if( radius && ( Math_VectorLength2( *pwvNpcPos, *player->GetPosPtr() ) > fRadius_Q2 ) )
                continue;
            else
                return GFriendInvitationController::Instance()->GetAddExpRatio(player_->GetLevel());
        }
    }
    
    return 0.0f;    
}

BYTE InvitateFriendInfo::GetAddQuestExpRate(FIELDCODE field_code)
{
    INVITATFRIEND_INFO_MAP_ITER iter = m_vecnvitateFriendInParty.begin();

    for(; iter != m_vecnvitateFriendInParty.end() ; ++iter)    
    {
        Player* const player = PlayerManager::Instance()->FindPlayerByCharKey(iter->first);

        if(player)
        {
            GameField* const field = player->GetField();
            if(field)
            {
                if(field->GetFieldCode() == field_code)
                {
                    return static_cast<BYTE>(GFriendInvitationController::Instance()->GetAddExpRatio(player_->GetLevel()) * 100);
                }                
            }

        }        
    }    
    return 0; 
}

WORD InvitateFriendInfo::GetAddMissionExpRate(FIELDCODE field_code)
{
    INVITATFRIEND_INFO_MAP_ITER iter = m_vecnvitateFriendInParty.begin();

    for(; iter != m_vecnvitateFriendInParty.end() ; ++iter)    
    {
        Player* const player = PlayerManager::Instance()->FindPlayerByCharKey(iter->first);

        if(player)
        {
            GameField* const field = player->GetField();
            if(field)
            {
                if(field->GetFieldCode() == field_code)
                {
                    return static_cast<WORD>(GFriendInvitationController::Instance()->GetAddExpRatio(player_->GetLevel()) * 100);
                }                
            }
        }        
    }    
    return 0; 
}

DWORD64 InvitateFriendInfo::GetRecallTime(CHARGUID CharGuid)
{
    INVITATFRIEND_INFO_MAP* const InvitateFriendList = GetFriendList(FRIEND_INVIATE);    
    INVITATFRIEND_INFO_MAP_ITER Invitateitr = InvitateFriendList->find(CharGuid);

    if(InvitateFriendList->end() != Invitateitr)
    {
        if(Invitateitr->second > 0)
        {
            return Invitateitr->second;
        }        
    }

    INVITATFRIEND_INFO_MAP* const InvitatedFriendList = GetFriendList(FRIEND_BEINVIATED);        
    INVITATFRIEND_INFO_MAP_ITER Invitateditr = InvitatedFriendList->find(CharGuid);    

    if(InvitatedFriendList->end() != Invitateditr)
    {
        if(Invitateditr->second > 0)
        {
            return Invitateditr->second;
        }
    }

    return 0;
}
