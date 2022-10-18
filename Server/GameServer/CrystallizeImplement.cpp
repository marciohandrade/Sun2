#include "StdAfx.h"
#include ".\CrystallizeImplement.h"
#include <ItemCrystalizeListParser.h>
#include "StatisticsManager.h"
#include "ExtraNPCManager.h"
#include <SCItemSlotContainer.h>
#include ".\ItemManager.h"
#include ".\ChaoState.h"
#include ".\Player.h"

CrystallizeImplement::CrystallizeImplement( void )
{
}

CrystallizeImplement::~CrystallizeImplement( void )
{
}

VOID CrystallizeImplement::OnFailed( DWORD dwResult )
{
    MSG_CG_ITEM_CRYSTALIZE_NAK Message;
    Message.m_byErrorCode = (BYTE)dwResult;
    m_pPlayer->SendPacket( &Message, sizeof(Message) );
}

VOID CrystallizeImplement::OnImplement( MSG_BASE* pRecvMessage )
{
    MSG_CG_ITEM_CRYSTALIZE_SYN* pRecvMsg = (MSG_CG_ITEM_CRYSTALIZE_SYN*)pRecvMessage;

    ItemManager* item_manager = m_pPlayer->GetItemManager();
    SCSlotContainer* inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);

    SCItemSlot& rTargetItem = (SCItemSlot &)inventory->GetSlot( pRecvMsg->m_TargetPos );

    static CrystallizeImplement::sOutput Output;
    RC::eITEM_RESULT Result = Crystallize( rTargetItem, Output );
    if( Result != RC::RC_ITEM_SUCCESS )
    {
        OnFailed( Result );
        return;
    }

    if( Output.TotalCount > 0 )
    {
        MSG_CG_ITEM_CRYSTALIZE_SUCCESS_ACK Message;
        RC::eITEM_RESULT Result = item_manager->ObtainAllKindOfItem(Output.TotalCount, 
                                                                    Output.ObtainCodeArray, 
                                                                    Output.ObtainCountArray,
                                                                    &Message.m_ItemInfo);
        if( Result == RC::RC_ITEM_SUCCESS )
        {
            // 결정화 성공 로그
            GAMELOG->LogCrystalizeSuccess(m_pPlayer, 
                                          rTargetItem, 
                                          Output.TotalCount, 
                                          Output.ObtainCodeArray, 
                                          Output.ObtainCountArray);

            g_StatisticsManager.PlusStatisticType(eSTATISTIC_CRYSTALIZE_COUNT, 1);

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
            DeleteTargetItem(rTargetItem);
#else
            inventory->DeleteSlot(pRecvMsg->m_TargetPos, NULL);
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE

            Message.m_Money = m_pPlayer->GetMoney();
            m_pPlayer->SendPacket( &Message, Message.GetSize() );
        }
        else
        {
            OnFailed( Result );
        }
    }
    else
    {
        // 결정화 실패 로그
        GAMELOG->LogItem( ITEM_CRYSTALIZE_FAILED, m_pPlayer, &rTargetItem );

        g_StatisticsManager.PlusStatisticType( eSTATISTIC_CRYSTALIZE_COUNT, 1 );
#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
        DeleteTargetItem(rTargetItem);
#else
        inventory->DeleteSlot(pRecvMsg->m_TargetPos, NULL);
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE
        MSG_CG_ITEM_CRYSTALIZE_FAILED_ACK Message;
        Message.m_Money = m_pPlayer->GetMoney();
        m_pPlayer->SendPacket( &Message, sizeof(Message) );
    }
}

BOOL CrystallizeImplement::OnCheck( MSG_BASE* pRecvMsg, DWORD& OUT dwResult )
{
    MSG_CG_ITEM_CRYSTALIZE_SYN* pMessage = (MSG_CG_ITEM_CRYSTALIZE_SYN *)pRecvMsg;

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

    if( item_manager->IsRestrictFunction( eFUNCTIONALRESTRICT_CRYSTALIZE ) )
    {
        dwResult = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return FALSE;
    }

#ifdef __NA_676_PK_SYSTEM_IMPROVEMENT
    if( !m_pPlayer->GetChaoState().CanUseFuction( CHAO_USE_CRYSTALIZE ) )
    {
        dwResult = RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
        return FALSE;
    }
#else //__NA_676_PK_SYSTEM_IMPROVEMENT
    if( m_pPlayer->GetChaoState().IsChaoState() )
    {
        dwResult = RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
        return FALSE;
    }
#endif //__NA_676_PK_SYSTEM_IMPROVEMENT

    if( m_pPlayer->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING )
    {
        dwResult = RC::RC_ITEM_INVALIDSTATEOFPLAYER;
        return FALSE;
    }

    if( !m_pPlayer->GetWarehouseInventoryProtector().INVENTORY().IsUsable() )
    {
        dwResult = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
        return FALSE;
    }

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
#else
    if( m_pPlayer->GetLevel() < LIMIT_LEVEL_CRYSTALIZE )
    {
        dwResult = RC::RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL;
        return FALSE;
    }
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE

    if( !item_manager->ValidState() ) 
    {
        dwResult = RC::RC_ITEM_INVALIDSTATE;
        return FALSE;
    }
    
    if( !item_manager->ValidPos( SI_INVENTORY, pMessage->m_TargetPos ) )
    {
        dwResult = RC::RC_ITEM_INVALIDPOS;
        return FALSE;
    }

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
#else
    if( !g_ExtraNPCManager.IsLengthCanUseNPC( pMessage->m_NPCCode, m_pPlayer, eNPC_TYPE_ITEM_MIX ) )
    {
        dwResult = RC::RC_ITEM_LENGTH_CANNOT_USE_NPC;
        return FALSE;
    }
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE

    return TRUE;
}

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
RC::eITEM_RESULT CrystallizeImplement::Crystallize( SCItemSlot& rTargetItem, CrystallizeImplement::sOutput& OUT Output )
{
    BYTE TotalCount = 0;
    SLOTCODE* ObtainCodeArray = Output.ObtainCodeArray;
    BYTE* ObtainCountArray = Output.ObtainCountArray;

    //정렬용
    STLX_MAP<SLOTCODE, DWORD> created_materials;  // 먼저, 만들어진 재료들을 이곳에 담는다.
    STLX_MAP<SLOTCODE, DWORD>::iterator miter;

    // Check Validity---------------------------------------------------------------------------
    const BASE_ITEMINFO* const pItemInfo = rTargetItem.GetItemInfo();
    FASSERT( pItemInfo );
    if( !pItemInfo )
        return RC::RC_ITEM_INVALID_VALUE;

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    SLOTCODE CrystallizeCode = ItemCrystalizeListParser::Instance()->GetCrystalizeCode( rTargetItem, rTargetItem.GetEnchant() );
#else
    SLOTCODE CrystallizeCode = ItemCrystalizeListParser::Instance()->GetCrystalizeCode( rTargetItem );
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
    sITEM_CRYSTALIZE_LIST* pListInfo = ItemCrystalizeListParser::Instance()->GetCrystalizeListInfo( CrystallizeCode );
    if( !pListInfo )
    {
        SUNLOG( eCRITICAL_LOG, "CrystallizeImplement::Crystallize() Can't Find Crystallize List Information [Code : %d]", CrystallizeCode );
        return RC::RC_ITEM_INVALID_VALUE;
    }

    if( pListInfo->m_ItemType != pItemInfo->m_wType )
        return RC::RC_ITEM_INVALID_VALUE;
    if (rTargetItem.CanUseMixture(MIXTURE_CRYSTALIZE) == false) {
        return RC::RC_ITEM_NOT_MIXTURE;
    }

    //------------------------------------------------------------------------------------------
    // Count Materials, 생성될 결정체들을 우선 구한다
    DWORD seed = 1;
    DWORD ObtainCount = 0;
    SLOTCODE material_code = 0;
    for( int count = 0 ; count < sITEM_CRYSTALIZE_LIST::MAX_MATERIALS ; ++count )
    {
        material_code = pListInfo->m_pResult[count].m_MaterialCode;
        // 결정체 획득 확률
        seed = random( 1, 100 );
        if( seed > pListInfo->m_pResult[count].m_ResultRatio )
            continue;

        if( material_code == 0 )
            continue;

        // 생성될 결정체 개수 계산
        ObtainCount = pListInfo->m_pResult[count].m_MaterialCount;

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
        SLOTCODE temp_crystallize_code = ItemCrystalizeListParser::Instance()->GetCrystalizeCode( rTargetItem, 0 );
        sITEM_CRYSTALIZE_LIST* list_info = ItemCrystalizeListParser::Instance()->GetCrystalizeListInfo( temp_crystallize_code );
        if( !list_info )
        {
            continue;
        }
        ObtainCount += (list_info->m_pResult[count].m_MaterialCount * rTargetItem.GetAwakening());
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

        // 결정체의 금액이 0 이 아닌 경우 아이템의 상점 구입 가격/ MaterialMoney로 추가되는 결정체의 갯수가 결정된다.
        if( 0 != pListInfo->m_pResult[count].m_MaterialMoney )
        {
            ObtainCount += (rTargetItem.GetPriceForBuy() / pListInfo->m_pResult[count].m_MaterialMoney);
        }

        miter = created_materials.find(material_code);
        if (miter == created_materials.end()) {
            created_materials[material_code] = ObtainCount;
        }
        else /* if (miter != created_materials.end())*/ {
            miter->second += ObtainCount;
        }
    }

    //created_materials의 재료들을 적당하게 output의 배열에 담아준다--------------------------------
    for (miter = created_materials.begin(); miter != created_materials.end(); ++miter)
    {
        SLOTCODE material_code = miter->first;
        DWORD  material_count = miter->second;
        // 결정체의 갯수가 255가 넘을 경우 다음 슬롯으로 넘긴다. 한 슬롯당 최대 255개의 동일 아이템 수용 가능.
        // 또한 최대 가질 수 있는 결정체의 갯수를 총 2개의 슬롯으로 제한 시킨다.
        material_count = min( material_count, UCHAR_MAX*2 );
        while( material_count > UCHAR_MAX )
        {
            ObtainCodeArray[TotalCount] = material_code;
            ObtainCountArray[TotalCount] = UCHAR_MAX;
            material_count -= UCHAR_MAX;
            ++TotalCount;
        }

        if( material_count > 0 )
        {
            ObtainCodeArray[TotalCount] = material_code;
            ObtainCountArray[TotalCount] = (BYTE)material_count;
            ++TotalCount;
        }
    }

    Output.TotalCount = TotalCount;

    ASSERT( TotalCount <= MAX_INVENTORY_SLOT_NUM );
    if( TotalCount > MAX_INVENTORY_SLOT_NUM )
    {
        SUNLOG( eCRITICAL_LOG, "CrystallizeImplement::Crystallize() Crystallize Overflow to Max Slot Count" );
        return RC::RC_ITEM_FAILED;
    }

    return RC::RC_ITEM_SUCCESS;
}

VOID CrystallizeImplement::DeleteTargetItem( SCItemSlot& rTargetItem )
{
    ItemManager* item_manager = m_pPlayer->GetItemManager();
    SCSlotContainer* inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    
    MSG_CG_ITEM_LOSE_ACK delete_msg;
    delete_msg.m_ItemInfo.Clear();

    BYTE item_count = delete_msg.m_ItemInfo.m_Count;
    delete_msg.m_ItemInfo.m_Slot[item_count].m_Num = rTargetItem.GetNum();
    delete_msg.m_ItemInfo.m_Slot[item_count].m_Pos = rTargetItem.GetPos();
    ++delete_msg.m_ItemInfo.m_Count;

    inventory->DeleteSlot(rTargetItem.GetPos(), NULL);
    m_pPlayer->SendPacket( &delete_msg, sizeof(delete_msg) );
}

#else
RC::eITEM_RESULT CrystallizeImplement::Crystallize( SCItemSlot& rTargetItem, CrystallizeImplement::sOutput& OUT Output )
{
    const BASE_ITEMINFO* const pItemInfo = rTargetItem.GetItemInfo();
    FASSERT( pItemInfo );
    if( !pItemInfo )
        return RC::RC_ITEM_INVALID_VALUE;

    SLOTCODE CrystallizeCode = ItemCrystalizeListParser::Instance()->GetCrystalizeCode( rTargetItem );
    sITEM_CRYSTALIZE_LIST* pListInfo = ItemCrystalizeListParser::Instance()->GetCrystalizeListInfo( CrystallizeCode );
    if( !pListInfo )
    {
        SUNLOG( eCRITICAL_LOG, "CrystallizeImplement::Crystallize() Can't Find Crystallize List Information [Code : %d]", CrystallizeCode );
        return RC::RC_ITEM_INVALID_VALUE;
    }

    if( pListInfo->m_ItemType != pItemInfo->m_wType )
        return RC::RC_ITEM_INVALID_VALUE;
    if (rTargetItem.CanUseMixture(MIXTURE_CRYSTALIZE) == false) {
        return RC::RC_ITEM_NOT_MIXTURE;
    }
    // 일반적인 아이템인 경우 1레벨 0랭크 E인챈트인 무기는 분해 불가능
    if( pItemInfo->m_wSubType == eITEMSUBTYPE_GENERAL )
    {
        if( pItemInfo->m_LV == 1 && rTargetItem.GetRank() == RANK_E && rTargetItem.GetEnchant() == 0 )
            return RC::RC_ITEM_INVALID_VALUE;
    }

    // 기본적으로 pListInfo->m_ItemLevel = 0값을 가지며, 악세사리의 경우 1, 2의 Level값을 가진다.
    if( pListInfo->m_ItemLevel && ( pItemInfo->m_LV != pListInfo->m_ItemLevel ) )
        return RC::RC_ITEM_INVALID_VALUE;

    if( pListInfo->m_EnchantLv != rTargetItem.GetEnchant() )
        return RC::RC_ITEM_INVALID_VALUE;


    POSTYPE TotalCount = 0;
    DWORD seed = 1;
    int i = 0;
    MONEY ObtainCount = 0;

    SLOTCODE* ObtainCodeArray = Output.ObtainCodeArray;
    POSTYPE* ObtainCountArray = Output.ObtainCountArray;

    // 1차 결정체 획득 확률
    for( i = 0 ; i < sITEM_CRYSTALIZE_LIST::MAX_RESULT ; ++i )
    {
        seed = random( 1, 100 );
        if( seed > pListInfo->m_pResult[i].m_ResultRatio )
            continue;

        if( pListInfo->m_pResult[i].m_MaterialCode == 0 )
            continue;

        ObtainCount = pListInfo->m_pResult[i].m_MaterialCount;

        // 결정체의 금액이 0 이 아닌 경우 아이템의 상점 구입 가격/ MaterialMoney로 추가되는 결정체의 갯수가 결정된다.
        if( 0 != pListInfo->m_pResult[i].m_MaterialMoney )
        {
            ObtainCount += (rTargetItem.GetPriceForBuy() / pListInfo->m_pResult[i].m_MaterialMoney);
        }

        // 결정체의 갯수가 255가 넘을 경우 다음 슬롯으로 넘긴다. 한 슬롯당 최대 255개의 동일 아이템 수용 가능.
        // 또한 최대 가질 수 있는 결정체의 갯수를 총 2개의 슬롯으로 제한 시킨다.
        ObtainCount = min( ObtainCount, UCHAR_MAX*2 );
        while( ObtainCount > UCHAR_MAX )
        {
            ObtainCodeArray[TotalCount] = pListInfo->m_pResult[i].m_MaterialCode;
            ObtainCountArray[TotalCount] = UCHAR_MAX;
            ObtainCount -= UCHAR_MAX;
            ++TotalCount;
        }

        if( ObtainCount > 0 )
        {
            ObtainCodeArray[TotalCount] = pListInfo->m_pResult[i].m_MaterialCode;
            ObtainCountArray[TotalCount] = (POSTYPE)ObtainCount;
            ++TotalCount;
        }
    }

    // 랜덤 결정체 획득 확률
    for( i = 0 ; i < sITEM_CRYSTALIZE_LIST::MAX_RANDOMRESULT ; ++i )
    {
        seed = (DWORD)dRandom( 1, pListInfo->m_pRandomResult[i].m_RandomStandardRatio );
        if( seed > pListInfo->m_pRandomResult[i].m_RandomRatio )
            continue;

        if( pListInfo->m_pRandomResult[i].m_MaterialCode == 0 )
            continue;

        // 결정체의 갯수가 255가 넘을 경우 다음 슬롯으로 넘긴다. 한 슬롯당 최대 255개의 동일 아이템 수용 가능.
        // 또한 최대 가질 수 있는 결정체의 갯수를 총 1개의 슬롯으로 제한 시킨다.
        ObtainCount = min( pListInfo->m_pRandomResult[i].m_MaterialCount, UCHAR_MAX );

        if( ObtainCount > 0 )
        {
            ObtainCodeArray[TotalCount] = pListInfo->m_pRandomResult[i].m_MaterialCode;
            ObtainCountArray[TotalCount] = (POSTYPE)ObtainCount;
            ++TotalCount;
        }
    }

    Output.TotalCount = TotalCount;

    ASSERT( TotalCount <= MAX_INVENTORY_SLOT_NUM );
    if( TotalCount > MAX_INVENTORY_SLOT_NUM )
    {
        SUNLOG( eCRITICAL_LOG, "CrystallizeImplement::Crystallize() Crystallize Overflow to Max Slot Count" );
        return RC::RC_ITEM_FAILED;
    }
    return RC::RC_ITEM_SUCCESS;
}
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE

