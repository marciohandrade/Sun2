#include "StdAfx.h"
#include <SCItemSlotContainer.h>
#include "StatisticsManager.h"
#include "ExtraNPCManager.h"
#include "RatioManager.h"
#include ".\ItemManager.h"
#include ".\ChaoState.h"
#include ".\Player.h"
#include ".\RankUpImplement.h"

#ifndef _NA_000251_20100727_SOCKET_SYSTEM
RankUpImplement::RankUpImplement( void ) 
{
}

RankUpImplement::~RankUpImplement( void )
{
}

VOID RankUpImplement::OnFailed( DWORD dwResult )
{
    MSG_CG_ITEM_RANKUP_NAK msg;
    msg.m_dwErrorCode = dwResult;
    m_pPlayer->SendPacket( &msg, sizeof(msg) );
}

VOID RankUpImplement::OnImplement( MSG_BASE* pRecvMessage )
{
    MSG_CG_ITEM_RANKUP_SYN* pRecvMsg = (MSG_CG_ITEM_RANKUP_SYN*)pRecvMessage;

    ItemManager* item_manager = m_pPlayer->GetItemManager();
    SCSlotContainer* inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);

    SCItemSlot& rTargetSlot1 = (SCItemSlot &)inventory->GetSlot( pRecvMsg->m_TargetPos[0] );
    SCItemSlot& rTargetSlot2 = (SCItemSlot &)inventory->GetSlot( pRecvMsg->m_TargetPos[1] );

    const BASE_ITEMINFO* pItemInfo1 = rTargetSlot1.GetItemInfo();
    const BASE_ITEMINFO* pItemInfo2 = rTargetSlot2.GetItemInfo();
    ASSERT( pItemInfo1 && pItemInfo2 );
    if( !pItemInfo1 || !pItemInfo2 )
    {
        OnFailed( RC::RC_ITEM_INVALIDPOS );
        return;
    }


    BOOLEAN bEquip1 = FALSE, bEquip2 = FALSE;
    if( pItemInfo1->IsWeapon() || pItemInfo1->IsArmor() || pItemInfo1->IsAccessory() )
        bEquip1 = TRUE;

    if( pItemInfo2->IsWeapon() || pItemInfo2->IsArmor() || pItemInfo2->IsAccessory() )
        bEquip2 = TRUE;


    RC::eITEM_RESULT Result = RC::RC_ITEM_INVALID_CONDITION;
    
    RankUpImplement::sOutput Output;

    if( bEquip1 && bEquip2 )
    {
        // 둘다 장비인 경우
        Result = RankUpEquipmentAndEquipment( rTargetSlot1, rTargetSlot2, Output );
    }
    else if( bEquip1 || bEquip2 )
    {
        // 장비와 랭크업 아이템 사용시
        if( pItemInfo1->IsRankUpRequirement() )
        {
            Result = RankUpEquipmentAndRequirement( rTargetSlot2, rTargetSlot1, Output );
        }
        else if( pItemInfo2->IsRankUpRequirement() )
        {
            Result = RankUpEquipmentAndRequirement( rTargetSlot1, rTargetSlot2, Output );
        }
    }


    switch( Result )
    {
    case RC::RC_ITEM_SUCCESS:
        {
            // 삭제할 아이템 삭제
            SCItemSlot& rSlot = (SCItemSlot&)inventory->GetSlot( Output.DeleteItemPos );
            if( rSlot.IsOverlap() && ( rSlot.GetNum() > 1 ) )
                inventory->UpdateSlot( Output.DeleteItemPos, -1 );
            else
                inventory->DeleteSlot( Output.DeleteItemPos, NULL );

            // 소모되는 돈 소비
            BOOL ret = m_pPlayer->MinusMoney( Output.RankUpFare );
            ASSERT(ret);

            GAMELOG->LogMoney( MONEY_RANKUP, m_pPlayer, Output.RankUpFare );
            g_pGameServer->NotifyHeimConsumption( m_pPlayer->GetCharGuid(), eHEIM_CONSUMPTION_RANKUP, Output.RankUpFare );
            g_StatisticsManager.PlusStatisticType( eSTATISTIC_RANKUP_COUNT, 1 );

            MSG_CG_ITEM_RANKUP_ACK msg;
            msg.m_TargetPos[0] = Output.RankUpItemPos;
            msg.m_TargetPos[1] = Output.DeleteItemPos;
            msg.m_Money		   = m_pPlayer->GetMoney();
            SCItemSlot& rRankUpSlot = (SCItemSlot&)inventory->GetSlot( Output.RankUpItemPos );
            rRankUpSlot.CopyOut( msg.m_TargetItemStream );
            m_pPlayer->SendPacket(&msg,sizeof(msg));

            //랭크업 성공 로그
            GAMELOG->LogItem( ITEM_RANKUP_AFTER, m_pPlayer, &rRankUpSlot, Output.RankUpFare );
            //랭크업 성공 로그 끝
        }
        break;

#ifdef __JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE
    case RC::RC_ITEM_RANKUP_FAILED_AND_DESTROY:
        {
            //1. 아이템 소멸시키고
            item_manager->Delete( Output.DeleteItemPos );

            //2. 돈도 소모 시킨다.
            BOOL ret = m_pPlayer->MinusMoney( Output.RankUpFare );
            ASSERT(ret);

            MSG_CG_ITEM_RANKUP_NAK nakMsg;
            nakMsg.m_dwErrorCode   = Result;
            nakMsg.m_DeleteItemPos = Output.DeleteItemPos;
            nakMsg.m_Money		   = m_pPlayer->GetMoney();
            m_pPlayer->SendPacket(&nakMsg, sizeof(nakMsg));
        }
        break;
#endif //__JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE

    default:
        OnFailed( Result );
        break;
    }
}


BOOLEAN RankUpImplement::DoRankUp( SCItemSlot& INOUT Equipment )
{
    if( RC::RC_ITEM_SUCCESS == CheckEnableForRankUp( Equipment ) )
    {
        return DoEquipmentRankUp( Equipment );
    }
    return FALSE;
}


RC::eITEM_RESULT RankUpImplement::RankUpEquipmentAndEquipment( SCItemSlot& IN Equipment1, 
                                                            SCItemSlot& IN Equipment2, 
                                                            RankUpImplement::sOutput& OUT Output )
{
    if( Equipment1.GetItemCode() != Equipment2.GetItemCode() || 
        Equipment1.GetRank() != Equipment2.GetRank() ||
        Equipment1.IsDivine() != Equipment2.IsDivine() )
    {
        return RC::RC_ITEM_INVALID_CONDITION;
    }

    // 동급 인첸트 인지 체크
    if( Equipment1.GetEnchantLV() != Equipment2.GetEnchantLV() )
        return RC::RC_ITEM_INVALID_CONDITION;


    if( (CheckEnableForRankUp( Equipment1 ) != RC::RC_ITEM_SUCCESS) ||
        (CheckEnableForRankUp( Equipment2 ) != RC::RC_ITEM_SUCCESS) )
        return RC::RC_ITEM_INVALID_CONDITION;


    SCItemSlot * pSelectedItemSlot = NULL, * pDeletedItemSlot = NULL;

    MONEY RankUpFare = 0;
    MONEY RankUpFare1 = GetRankUpPrice( Equipment1 );
    MONEY RankUpFare2 = GetRankUpPrice( Equipment2 );
    if( RankUpFare1 > RankUpFare2 )
    {
        RankUpFare = RankUpFare1;
        pSelectedItemSlot = &Equipment1;
        pDeletedItemSlot = &Equipment2;
    }
    else
    {
        RankUpFare = RankUpFare2;
        pSelectedItemSlot = &Equipment2;
        pDeletedItemSlot = &Equipment1;
    }

    // 랭크업 요금은 있는지 체크
    if( m_pPlayer->GetMoney() < Output.RankUpFare )
        return RC::RC_ITEM_INSUFFICIENT_MONEY;


    // 우선 첫번째 아이템을 랭크업 아이템으로 선택
    int seed = random(0, 99);
    if( seed < 30 )
    {
        // 2번째 아이템 선택
        SCItemSlot* pTmpItemSlot = pSelectedItemSlot;
        pSelectedItemSlot = pDeletedItemSlot;
        pDeletedItemSlot = pTmpItemSlot;
    }
    

    Output.RankUpFare = RankUpFare;
    Output.RankUpItemPos = pSelectedItemSlot->GetPos();
    Output.DeleteItemPos = pDeletedItemSlot->GetPos();


    //일본 랭크업 실패 확률 설정
#ifdef __JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE
    if( !IsSuccessToRankUpRatio( *pSelectedItemSlot ) )
        return RC::RC_ITEM_RANKUP_FAILED_AND_DESTROY;
#endif //__JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE

    // 로그
    GAMELOG->LogItemEx( ITEM_RANKUP_BEFORE, m_pPlayer, *pSelectedItemSlot, *pDeletedItemSlot );

    // 실제 랭크업을 시도한다.
    if( !DoEquipmentRankUp( *pSelectedItemSlot ) )
    {
        return RC::RC_ITEM_CANNT_RANKUP_ITEM;
    }
    //__NA_DKJH0002_MODIFY_POSSESSION_ITEM_RANKUP
    if( ( pSelectedItemSlot->IsPossessionItem() && pSelectedItemSlot->GetIdentifyPossession() ) ||
        ( pDeletedItemSlot->IsPossessionItem() && pDeletedItemSlot->GetIdentifyPossession() ) )
    {
        pSelectedItemSlot->SetIdentifyPossession( TRUE );
    }
    //
    return RC::RC_ITEM_SUCCESS;
}


RC::eITEM_RESULT RankUpImplement::RankUpEquipmentAndRequirement( SCItemSlot& IN Equipment, 
                                                                SCItemSlot& IN Requirement,
                                                                RankUpImplement::sOutput& OUT Output )
{
    Output.RankUpItemPos = Equipment.GetPos();
    Output.DeleteItemPos = Requirement.GetPos();
    Output.RankUpFare    = GetRankUpPrice( Equipment );

    RC::eITEM_RESULT eResult = RC::RC_ITEM_SUCCESS;

    // 부적의 돌 사용시
    if( Requirement.GetItemInfo()->IsRankUpGradeRaiser() )
    {
        eResult = CheckEnableForRankUp( Equipment );
    }
    else // 초기화의 돌 사용시
    {
        eResult = CheckEnableForRankOptionChange( Equipment );
    }

    if( eResult != RC::RC_ITEM_SUCCESS )
        return eResult;

    // 랭크업 요금은 있는지 체크
    if( m_pPlayer->GetMoney() < Output.RankUpFare )
    {
        return RC::RC_ITEM_INSUFFICIENT_MONEY;
    }

    // 로그
    GAMELOG->LogItemEx( ITEM_RANKUP_BEFORE, m_pPlayer, Equipment, Requirement );

    // 부적의 돌 사용시
    BOOLEAN bSucc = TRUE;
    if( Requirement.GetItemInfo()->IsRankUpGradeRaiser() )
    {
        bSucc = DoEquipmentRankUp( Equipment );
    }
    else // 초기화의 돌 사용시
    {
        bSucc = DoEquipmentRankOptionRandomChange( Equipment );
    }

    if( !bSucc )
    {
        return RC::RC_ITEM_CANNT_RANKUP_ITEM;
    }
    //__NA_DKJH0002_MODIFY_POSSESSION_ITEM_RANKUP
    if( Equipment.IsPossessionItem() && Equipment.GetIdentifyPossession() )
    {
        Equipment.SetIdentifyPossession( TRUE );
    }
    //
    return RC::RC_ITEM_SUCCESS;
}


BOOLEAN RankUpImplement::DoEquipmentRankOptionRandomChange( SCItemSlot& IN Equipment )
{
    WORD wType = Equipment.GetItemInfo()->m_wType;
    eRANK Rank = Equipment.GetRank();
    ASSERT( Rank < RANK_MAX );

    sRANK_OPTION* pRankOption = CreateRankOption( wType, Rank, Equipment.GetRankAttrIndex( Rank ) );
    ASSERT( pRankOption );
    if( !pRankOption )
        return FALSE;

    // 랭크 옵션 설정
    Equipment.SetRankAttr( Rank, pRankOption );

    return TRUE;
}

BOOLEAN RankUpImplement::DoEquipmentRankUp( SCItemSlot& IN Equipment )
{
    WORD wType = Equipment.GetItemInfo()->m_wType;
    eRANK Rank = (eRANK)(Equipment.GetRank() + 1);
    ASSERT( Rank < RANK_MAX );
    if( Rank >= RANK_MAX )
        return FALSE;

    sRANK_OPTION* pRankOption = CreateRankOption( wType, Rank );
    ASSERT( pRankOption );
    if( !pRankOption )
        return FALSE;

    // 랭크 설정
    Equipment.SetRank( Rank );

    // 랭크 옵션 설정
    Equipment.SetRankAttr( Rank, pRankOption );

    // 소켓 개수 생성
    CreateSocketHole( Equipment, Rank );

    return TRUE;
}

RC::eITEM_RESULT RankUpImplement::CheckEnableForRankOptionChange( SCItemSlot& IN Equipment )
{
    if (Equipment.CanUseMixture(MIXTURE_RANKUP) == false) {
        return RC::RC_ITEM_NOT_MIXTURE;
    }

    // 랭크업 불가 아이템.
    if( !Equipment.GetItemRuleInfo().IsEnableRank() )
        return RC::RC_ITEM_NOT_MIXTURE;

    // 엘리트 아이템은 랭크업 불가
    if( Equipment.GetItemInfo()->IsElite() )
        return RC::RC_ITEM_CANNOT_RANKUP_ELITE;

    //리미티드 아이템은 랭크업 불가
    if( Equipment.IsLimited() )
        return RC::RC_ITEM_CANNOT_RANKUP_LIMITED;


    // 럭키 아이템은 랭크업 불가
    if( Equipment.IsFateItem() )
        return RC::RC_ITEM_CANNOT_RANKUP_FATE;


    // 유니크 아이템은 랭크업 불가
    if( Equipment.GetItemInfo()->IsUnique() )
        return RC::RC_ITEM_CANNOT_RANKUP_UNIQUE;


    // E 랭크 옵션 불가능
    if( Equipment.GetRank() <= RANK_E )
        return RC::RC_ITEM_INVALID_CONDITION;

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT RankUpImplement::CheckEnableForRankUp( SCItemSlot& IN Equipment )
{
    if (Equipment.CanUseMixture(MIXTURE_RANKUP) == false) {
        return RC::RC_ITEM_NOT_MIXTURE;
    }
    // 랭크업 불가 아이템.
    if( !Equipment.GetItemRuleInfo().IsEnableRank() )
        return RC::RC_ITEM_NOT_MIXTURE;

    // 엘리트 아이템은 랭크업 불가
    if( Equipment.GetItemInfo()->IsElite() )
        return RC::RC_ITEM_CANNOT_RANKUP_ELITE;

    // +S 랭크이상 랭크 불가능
    if( Equipment.GetRank() >= RANK_PS )
        return RC::RC_ITEM_NO_MORE_RANK;

    // 리미티드 아이템은 랭크업 불가
    if( Equipment.IsLimited() )
        return RC::RC_ITEM_CANNOT_RANKUP_LIMITED;

    // 유니크 아이템은 랭크업 불가
    if( Equipment.GetItemInfo()->IsUnique() )
        return RC::RC_ITEM_CANNOT_RANKUP_UNIQUE;
    
    // 럭키 아이템은 랭크업 불가
    if( Equipment.IsFateItem() )
        return RC::RC_ITEM_CANNOT_RANKUP_FATE;

    if( Equipment.GetItemInfo()->IsWeapon() || Equipment.GetItemInfo()->IsArmor() )
    {
        switch( Equipment.GetRank() )
        {
        case RANK_B: 
        case RANK_MA: 
        case RANK_A:
            {
                // 중급이상의 인첸트여야 함
                if( Equipment.GetEnchantLV() < ENCHANT_MIDDLE )
                    return RC::RC_ITEM_INVALID_CONDITION;
            }break;
        case RANK_PA: 
        case RANK_MS: 
        case RANK_S:
            {
                // 상급이상의 인첸트여야 함
                if( Equipment.GetEnchantLV() < ENCHANT_HIGH )
                    return RC::RC_ITEM_INVALID_CONDITION;
            }break;
        }
    }
    else if( Equipment.GetItemInfo()->IsAccessory() )
    {
        // 조건 없음
    }
    else
    {
        return RC::RC_ITEM_INVALID_CONDITION;
    }

    return RC::RC_ITEM_SUCCESS;
}

BOOLEAN RankUpImplement::CreateSocketHole( SCItemSlot& ItemSlot, BYTE byCurRank )
{
    if (ItemSlot.CanUseMixture(MIXTURE_SOCKET_HOLE_FILL) == false) {
        return false;
    }
    BOOL bCreatedSocket = FALSE;
    switch( ItemSlot.GetItemInfo()->m_wType )
    {
        // 무기
    case eITEMTYPE_TWOHANDAXE:	
    case eITEMTYPE_TWOHANDSWORD:	
    case eITEMTYPE_ONEHANDSWORD:
    case eITEMTYPE_SPEAR:
    case eITEMTYPE_WHIP:		
    case eITEMTYPE_DAGGER:	
    case eITEMTYPE_ONEHANDCROSSBOW:
    case eITEMTYPE_ETHERWEAPON:
    case eITEMTYPE_STAFF:
    case eITEMTYPE_ORB:
    case eITEMTYPE_GUNTLET: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eITEMTYPE_POLEARM: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eITEMTYPE_HELLROID_1:
    case eITEMTYPE_HELLROID_2:
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eITEMTYPE_ARCBLADE:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        // 방어구
    case eITEMTYPE_ARMOR:
    case eITEMTYPE_PANTS:
    case eITEMTYPE_BOOTS:
    case eITEMTYPE_GLOVE:
    case eITEMTYPE_HELMET:
    case eITEMTYPE_PROTECTOR:
    case eITEMTYPE_BELT:
    case eITEMTYPE_SHIRTS:
        {	

            BYTE SocketNum	= ItemSlot.GetSocketNum();
            if( (byCurRank+1) <= RANK_B && SocketNum < 3 )
            {
                ASSERT( ItemSlot.GetRank() == (byCurRank+1) );
                BYTE SelectRank	= ItemSlot.GetRank() - RANK_D;
                BYTE seed = random( 1, 100 );
                if( seed <= g_Ratio.m_SocketRatio[SelectRank].Ratio[SocketNum] )
                {
                    // 성공
                    ItemSlot.SetSocketNum(ItemSlot.GetSocketNum()+1);
                    bCreatedSocket = TRUE;
                }
            }
        }break;
    }
    return bCreatedSocket;
}


sRANK_OPTION* RankUpImplement::CreateRankOption( WORD wItemType, eRANK Rank, DWORD dwExceptAttribute )
{
    ASSERT( Rank < RANK_MAX );

    sPTION_SELECTOR_FOR_RANK* pSelector = RankOptionParser::Instance()->GetRankOptionSelector( wItemType, Rank );
    ASSERT( pSelector );
    if( !pSelector ) 
        return NULL;

    // 랭크업시 특정 옵션의 적용을 하지 않는다.
    // 정책상 기존 장비에 이미 붙어 있는 옵션은 제거 되지 않게 하기 위해 DB또는 파싱 부분의
    // 변경은 큰 작업이므로, 앞으로 적용되는 랭크업에서 특정 속성만 제거해서 적용하기 위한코드.
    DWORD idx = 0;
    sPTION_SELECTOR_FOR_RANK TempSelector;
    TempSelector.m_OptionCount = 0;
    for(DWORD i = 0 ; i < pSelector->m_OptionCount ; ++i)
    {
        idx = pSelector->m_OptionIndex[i];
        if( (idx >= 3 && idx <= 13) || (idx >= 16 && idx <= 20) )
            continue;

        if( idx == dwExceptAttribute )
            continue;

        TempSelector.m_OptionIndex[TempSelector.m_OptionCount] = idx;
        TempSelector.m_OptionCount++;
    }

    if( TempSelector.m_OptionCount <= 0 )
        return NULL;

    BYTE seed = random( 0, TempSelector.m_OptionCount-1 );

    DWORD OptionType = TempSelector.m_OptionIndex[seed];

    sRANK_OPTION* pRankOption = RankOptionParser::Instance()->GetRankOption( wItemType, OptionType );
    ASSERT( pRankOption );
    if( !pRankOption )
        return NULL;

    ASSERT( pRankOption->m_iOptionIndex != 0 );
    ASSERT( pRankOption->m_OptionNCode != 0 );
    ASSERT( pRankOption->m_Value[Rank] != 0 );

    return pRankOption;
}

MONEY RankUpImplement::GetRankUpPrice( SCItemSlot& IN Equipment )
{
    return (Equipment.GetPriceForBuy()*Equipment.GetItemInfo()->m_LV);
}

BOOLEAN RankUpImplement::IsSuccessToRankUpRatio( SCItemSlot& IN Equipment )
{
    BYTE curRank = Equipment.GetRank();
    BYTE byRankUpRatio = 0;
    switch(curRank)
    {
    case RANK_E:
    case RANK_D:    byRankUpRatio = 100;    break;
    case RANK_C:    byRankUpRatio = 90;     break;
    case RANK_B:    byRankUpRatio = 80;     break;
    case RANK_MA:   byRankUpRatio = 70;     break;
    case RANK_A:    byRankUpRatio = 60;     break;
    case RANK_PA:   byRankUpRatio = 50;     break;
    case RANK_MS:   byRankUpRatio = 40;     break;
    default:        byRankUpRatio = 0;      break;
    }
    // Seed가 100이 나오면 byRankUpRatio가 100일시 실패한다.
    BYTE RankUpSeed = random( 0, 99 );
    if( RankUpSeed > byRankUpRatio )
        return FALSE;

    return TRUE;
}


BOOL RankUpImplement::OnCheck( MSG_BASE* pRecvMessage, DWORD& OUT dwResult )
{
    MSG_CG_ITEM_RANKUP_SYN* pMessage = (MSG_CG_ITEM_RANKUP_SYN*)pRecvMessage;

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
        SUNLOG( eCRITICAL_LOG, "RankUpImplement::OnCheck() Player Is Not Exist GameField" );
        dwResult = RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
        return FALSE;
    }

    if( !g_ExtraNPCManager.IsLengthCanUseNPC( pMessage->m_NPCCode, m_pPlayer, eNPC_TYPE_ITEM_MIX ) )
    {
        dwResult = RC::RC_ITEM_LENGTH_CANNOT_USE_NPC;
        return FALSE;
    }

    if( !m_pPlayer->GetWarehouseInventoryProtector().INVENTORY().IsUsable() )
    {
        dwResult = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
        return FALSE;
    }

    if( item_manager->IsRestrictFunction( eFUNCTIONALRESTRICT_RANKUP ) )
    {
        dwResult = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return FALSE;
    }

#ifdef __NA_676_PK_SYSTEM_IMPROVEMENT
    if( !m_pPlayer->GetChaoState().CanUseFuction( CHAO_USE_RANKUP ) )
    {
        dwResult = RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
        return FALSE;
    }
#else
    if( m_pPlayer->GetChaoState().IsChaoState() )
    {
        dwResult = RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
        return FALSE;
    }
#endif

    if( m_pPlayer->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING )
    {
        dwResult = RC::RC_ITEM_INVALIDSTATEOFPLAYER;
        return FALSE;
    }

    if( m_pPlayer->GetLevel() < LIMIT_LEVEL_RANKUP )
    {
        dwResult = RC::RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL;
        return FALSE;
    }

    if( !item_manager->ValidState() ) 
    {
        dwResult = RC::RC_ITEM_INVALIDSTATE;
        return FALSE;
    }

    if( pMessage->m_TargetPos[0] == pMessage->m_TargetPos[1] )
    {
        dwResult = RC::RC_ITEM_INVALIDPOS;
        return FALSE;
    }

    if( !item_manager->ValidPos( SI_INVENTORY, pMessage->m_TargetPos[0] ) || 
        !item_manager->ValidPos( SI_INVENTORY, pMessage->m_TargetPos[1] ) )
    {
        dwResult = RC::RC_ITEM_INVALIDPOS;
        return FALSE;
    }

    return TRUE;
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM