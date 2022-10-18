#include "StdAfx.h"
#include <SCItemSlotContainer.h>
#include <DropRatioInfoList.h>
#include <ItemTypeList.h>
#include ".\ItemManager.h"
#include ".\ChaoState.h"
#include ".\Player.h"
#include ".\RankUpImplement.h"
#include ".\RandomItemImplement.h"

#ifdef _NA_001990_ACSYSTEM_ADD
#include "AchievementManager.h"
#endif //_NA_001990_ACSYSTEM_ADD

RandomItemImplement::RandomItemImplement( void )
{
}

RandomItemImplement::~RandomItemImplement( void )
{
}

VOID RandomItemImplement::OnFailed( DWORD dwResult )
{
    MSG_CG_ITEM_USE_RANDOM_ITEM_NAK msg;
    msg.m_dwErrorCode = dwResult;
    m_pPlayer->SendPacket( &msg, sizeof(msg) );
}

VOID RandomItemImplement::OnImplement( MSG_BASE* pRecvMessage )
{
    MSG_CG_ITEM_USE_RANDOM_ITEM_SYN* pRecvMsg = (MSG_CG_ITEM_USE_RANDOM_ITEM_SYN*)pRecvMessage;

    ItemManager* item_manager = m_pPlayer->GetItemManager();
    SCSlotContainer* inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);

    SCItemSlot GeneratedItemSlot;
    SCItemSlot& RandomItemSlot = (SCItemSlot&)inventory->GetSlot( pRecvMsg->m_TargetPos );
    RC::eITEM_RESULT Result = GenerateRandomEquipment( RandomItemSlot, GeneratedItemSlot );
    if( RC::RC_ITEM_SUCCESS == Result )
    {
        MSG_CG_ITEM_USE_RANDOM_ITEM_ACK msg;

        // 랜덤 아이템 삭제.
        if( RandomItemSlot.IsOverlap() && (RandomItemSlot.GetNum() > 1) )
            inventory->UpdateSlot( pRecvMsg->m_TargetPos, -1 );
        else
            inventory->DeleteSlot( pRecvMsg->m_TargetPos, NULL );

        // 새로 생성된 아이템 인벤토리에 추가.
        DURATYPE num = GeneratedItemSlot.GetNum();
        GeneratedItemSlot.SetNum(1);
        item_manager->Obtain( GeneratedItemSlot, num, &(msg.m_ItemInfo) );
        m_pPlayer->SendPacket( &msg, msg.GetSize() );

#ifdef _NA_001990_ACSYSTEM_ADD
        const BASE_ITEMINFO* item_info = GeneratedItemSlot.GetItemInfo();
        if (item_info->m_wACCode)
            m_pPlayer->GetACManager()->ProcessItemObtain(item_info->m_Code, item_info->m_wACCode, 
                item_info->m_byACReferenceID, num);
#endif //_NA_001990_ACSYSTEM_ADD
    }
    else
    {
        OnFailed( Result );
    }
}

RC::eITEM_RESULT RandomItemImplement::GenerateRandomEquipment( SCItemSlot& IN ItemSlot, SCItemSlot& OUT Output )
{
    const BASE_ITEMINFO* pItemInfo = ItemSlot.GetItemInfo();
    ASSERT( pItemInfo );
    if( !pItemInfo )
    {
        return RC::RC_ITEM_NOINFO;
    }

    if( !pItemInfo->IsRandomItem() )
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    
    DropItemObj* pDropItem = static_cast<DropItemObj*>
        (DropRatioInfoList::Instance()->PopRandomItemTypeIndex( pItemInfo->m_wSkillCode ));
    ASSERT( pDropItem );
    if( !pDropItem )
    {
        SUNLOG( eCRITICAL_LOG, 
                "RandomItemImplement::GenerateRandomEquipment() Can't Find Item [index:%d]", 
                pItemInfo->m_wSkillCode );
        return RC::RC_ITEM_INVALIDSTATE;
    }

    if( pDropItem->GetType() != DROP_OBJ_ITEM )
    {
        SUNLOG( eCRITICAL_LOG, 
                "RandomItemImplement::GenerateRandomEquipment() Not Object Item [index:%d]", 
                pItemInfo->m_wSkillCode );
        return RC::RC_ITEM_INVALIDSTATE;
    }

    const BASE_ITEMINFO* pOutItemInfo = \
        ItemInfoParser::Instance()->GetItemInfo( pDropItem->GetItemCode() );
    ASSERT( pOutItemInfo );
    if( !pOutItemInfo )
    {
        SUNLOG( eCRITICAL_LOG, 
                "RandomItemImplement::GenerateRandomEquipment() Can't Find Item [code:%d]", 
                pDropItem->GetItemCode() );
        return RC::RC_ITEM_INVALIDSTATE;
    }

    // 아이템 설정
    Output.SetCode( (SLOTCODE)pDropItem->GetItemCode() );
    const nsSlot::ItemRuleInfo& ItemRule = Output.GetItemRuleInfo();
    if(ItemRule.is_valid_item == 0)
    {
        SUNLOG( eCRITICAL_LOG, 
                "RandomItemImplement::GenerateRandomEquipment() Can't Find Item Rule [code:%d]", 
                pDropItem->GetItemCode() );
        return RC::RC_ITEM_INVALIDSTATE;
    }
    // 아이템 갯수 지정.
    if( Output.IsOverlap() )
    {
        // TODO(WAVERIX) 관련 사항 검증할 필요가 있어 보인다.
        DWORD Count = pDropItem->GetReserved();
        const DURATYPE MaxNum = Output.GetMaxNum();
        if( MaxNum < Count )
            Output.SetNum( MaxNum );
        else if( Count )
            Output.SetNum( Count );
    }
    else
    {
        ItemType* pItemType = ItemTypeList::Instance()->FindItemType( pDropItem->GetReserved() );
        if( !pItemType )
            return RC::RC_ITEM_SUCCESS;
        if( ItemRule.IsEnableEnchant() )
            Output.SetEnchant( pItemType->GetEnchant() );

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
        // EP2에서는 아이템 타입 정보의 랭크 항목을 소켓 개수로 사용한다.
        const BYTE number_of_socket = pItemType->GetRank();
        if (number_of_socket != 0 && ItemRule.IsEnableSocket())
        {
            if (Output.CreateSocketItem(number_of_socket, false) == false)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|소켓 개수 설정에 실패했습니다.|아이템코드 = %u, 소켓개수 = &u"), 
                       __FUNCTION__, 
                       pDropItem->GetItemCode(), 
                       number_of_socket);
                return RC::RC_ITEM_INVALIDSTATE;
            }
        }
#else
        // (CHANGES) (WAVERIX) itemslot에서 엘리트 룰(Rankup)이 제거되었다.
        if(pItemInfo->IsElite() == 0 && ItemRule.IsEnableRank())
        {
            Output.SetRank( RANK_E );
            RankUpImplement RankUpImp;
            BYTE rank = pItemType->GetRank();
            for( BYTE RankIndex = RANK_E ; RankIndex < rank ; ++RankIndex )
            {
                if( !RankUpImp.DoRankUp( Output ) )
                {
                    SUNLOG( eCRITICAL_LOG, 
                            "RandomItemImplement::GenerateRandomEquipment() RankUp Failed [ItemCode:%d]", 
                            ItemSlot.GetItemCode() );
                    return RC::RC_ITEM_INVALIDSTATE;
                }
            }
        }
#endif // _NA_000251_20100727_SOCKET_SYSTEM

        nsSlot::ItemTypeChanger::ChangeItemByType(&Output, pItemType->GetItemType());
    }

    return RC::RC_ITEM_SUCCESS;
}


BOOL RandomItemImplement::OnCheck( MSG_BASE* pRecvMessage, DWORD& OUT dwResult )
{
    MSG_CG_ITEM_USE_RANDOM_ITEM_SYN* pMessage = (MSG_CG_ITEM_USE_RANDOM_ITEM_SYN*)pRecvMessage;

    dwResult = RC::RC_ITEM_SUCCESS;

    ItemManager* item_manager = m_pPlayer->GetItemManager();
    ASSERT(item_manager);
    if (item_manager == NULL)
    {
        dwResult = RC::RC_ITEM_FAILED;
        return FALSE;
    }

    SCSlotContainer* inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    ASSERT(inventory);
    if (inventory == NULL)
    {
        dwResult = RC::RC_ITEM_FAILED;
        return FALSE;
    }

    if( !m_pPlayer->GetField() )
    {
        dwResult = RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
        return FALSE;
    }

    if( !m_pPlayer->GetWarehouseInventoryProtector().INVENTORY().IsUsable() )
    {
        dwResult = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
        return FALSE;
    }

    if( m_pPlayer->GetBehave() != PLAYER_BEHAVE_IDLE_STATE )
    {
        dwResult = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return FALSE;
    }

    if( !item_manager->ValidState() ) 
    {
        dwResult = RC::RC_ITEM_INVALIDSTATE;
        return FALSE;
    }

    if( !item_manager->ValidPos(SI_INVENTORY, pMessage->m_TargetPos) )
    {
        dwResult = RC::RC_ITEM_INVALIDPOS;	
        return FALSE;
    }

    return TRUE;
}
