#include "StdAfx.h"
#include "./FriendInvitationReWardproc.h"

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND

#include "ItemManager.h"//#include "SCItemSlot.h"
#include "GFriendInvitationController.h"
#include "RewardManager.h"


FriendInvitationReWardProc::FriendInvitationReWardProc(void)
{
}

FriendInvitationReWardProc::~FriendInvitationReWardProc(void)
{
}

VOID FriendInvitationReWardProc::ReceiveRewardItem(Player* pPlayer, LEVELTYPE RewardType)
{
    MSG_CG_EVENT_INVITATEFRIEND_REWARD_ACK msg;

    RC::eITEM_RESULT rcResult = pPlayer->GetItemManager()->Obtain(GetRewardItemCode(RewardType), 1, &msg.m_TotalInfo);

    if(RC::RC_ITEM_SUCCESS == rcResult)
    {
        GAMELOG->LogItemInvitatedFriendReward(pPlayer, 1, GetRewardItemCode(RewardType), RewardType);

        pPlayer->SendPacket( &msg, sizeof(msg) );
    }
    else
    {
        //인벤에 장착가능한지 파악하고 DBPROXY 다녀온 후라 여기로 들어오면 안된당~
        //DB 에 보상받았다는 정보를 RollBack 해줘야 한다.~
        SendRewardFailMsg(pPlayer, MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK::NOTOBTAIN_INVEN);
    }
}

VOID FriendInvitationReWardProc::ReQuestReWardItem(Player* pPlayer, CHARGUID FriendGuid, LEVELTYPE RewardType)
{
    if(CheckInsertITem(pPlayer, RewardType))
    {
        SendRewardRequestMsg(pPlayer, FriendGuid, RewardType);
    }
    else
    {
        SendRewardFailMsg(pPlayer, MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK::NOTOBTAIN_INVEN);
    }
}

VOID FriendInvitationReWardProc::SendRewardFailMsg(Player* pPlayer, BYTE ErrorCode)
{
    MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK Msg;
    Msg.m_byErrorCode = ErrorCode;

    pPlayer->SendPacket( &Msg, sizeof(Msg) );
}

VOID FriendInvitationReWardProc::SendRewardRequestMsg(Player* pPlayer, CHARGUID FriendGuid, LEVELTYPE RewardType)
{
    MSG_DG_EVENT_INVITATEFRIEND_REWARDUPDATE_SYN msg;
    msg.CharGUID = pPlayer->GetCharGuid();
    msg.FriendGUID = FriendGuid;
    msg.ReWardType = RewardType;

    if(!pPlayer->SendToGameDBPServer(&msg, sizeof(msg)))
    {
        MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK Msg;
        Msg.m_byErrorCode = MSG_CG_EVENT_INVITATEFRIEND_REWARD_NAK::DBPROXY_FAIL;
        pPlayer->SendPacket( &Msg, sizeof(Msg) );
    }
}

BOOL FriendInvitationReWardProc::CheckInsertITem(Player* pPlayer, LEVELTYPE level)
{
    ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
    DWORD checkInvenLock = rPROTECT.IsUsable() ? ULONG_MAX : RC::RC_ITEM_INVENTORY_NEED_PASSWORD;    

    if(RC::RC_ITEM_INVENTORY_NEED_PASSWORD == checkInvenLock) //인벤 잠긴 상태
    {        
        return FALSE;
    }

#if SUN_CODE_RENEWAL
    // (CHANGES) (f100327.3L) changes codes to based on the rearranged CheckInsertItem specification
    const POSTYPE kDesiredNumberOfItems = 1;
    const SLOTCODE item_code = GetRewardItemCode(level);
    if (item_code == 0) {
        return FALSE;
    }

    int number_of_insertable_items = \
        pPlayer->GetItemManager()->CheckInsertItem(SI_INVENTORY, item_code, kDesiredNumberOfItems);

    return number_of_insertable_items == kDesiredNumberOfItems;
#else //if SUN_CODE_BACKUP
    SLOTCODE ItemCode = GetRewardItemCode(level);
    SCItemSlot ItemSlot;
    ItemSlot.Clear();
    ItemSlot.SetCode( ItemCode );

    SCSlotContainer* pInven = (SCSlotContainer*)pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);    
    POS_INFO inven_pos_info[MAX_INVENTORY_SLOT_NUM];
    POSTYPE inven_pos_info_count = 0;

    int InserCount = pPlayer->GetItemManager()->CheckInsertItem( pInven, ItemSlot, 1, MAX_INVENTORY_SLOT_NUM, inven_pos_info, &inven_pos_info_count );
    if(1 == InserCount)
    {
        return TRUE;
    }
    return FALSE;
#endif
}

SLOTCODE FriendInvitationReWardProc::GetRewardItemCode(LEVELTYPE level)
{
    EVENT_INFO * friend_event_info = GFriendInvitationController::Instance()->GetFriendEvent();
    if (friend_event_info == NULL) {
        return 0;
    }

    return friend_event_info->m_dwRewardInfo[ConvertItemIndexToLevel(level)];
}

int FriendInvitationReWardProc::ConvertItemIndexToLevel(LEVELTYPE level)
{
    switch(level)
    {
    case eINVITATION_REWARDSECTION_1: return 0;
    case eINVITATION_REWARDSECTION_2: return 1;
    case eINVITATION_REWARDSECTION_3: return 2;
    case eINVITATION_REWARDSECTION_4: return 3;
    }
    return 0;
}
