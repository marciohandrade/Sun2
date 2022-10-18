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
        // �Ѵ� ����� ���
        Result = RankUpEquipmentAndEquipment( rTargetSlot1, rTargetSlot2, Output );
    }
    else if( bEquip1 || bEquip2 )
    {
        // ���� ��ũ�� ������ ����
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
            // ������ ������ ����
            SCItemSlot& rSlot = (SCItemSlot&)inventory->GetSlot( Output.DeleteItemPos );
            if( rSlot.IsOverlap() && ( rSlot.GetNum() > 1 ) )
                inventory->UpdateSlot( Output.DeleteItemPos, -1 );
            else
                inventory->DeleteSlot( Output.DeleteItemPos, NULL );

            // �Ҹ�Ǵ� �� �Һ�
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

            //��ũ�� ���� �α�
            GAMELOG->LogItem( ITEM_RANKUP_AFTER, m_pPlayer, &rRankUpSlot, Output.RankUpFare );
            //��ũ�� ���� �α� ��
        }
        break;

#ifdef __JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE
    case RC::RC_ITEM_RANKUP_FAILED_AND_DESTROY:
        {
            //1. ������ �Ҹ��Ű��
            item_manager->Delete( Output.DeleteItemPos );

            //2. ���� �Ҹ� ��Ų��.
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

    // ���� ��þƮ ���� üũ
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

    // ��ũ�� ����� �ִ��� üũ
    if( m_pPlayer->GetMoney() < Output.RankUpFare )
        return RC::RC_ITEM_INSUFFICIENT_MONEY;


    // �켱 ù��° �������� ��ũ�� ���������� ����
    int seed = random(0, 99);
    if( seed < 30 )
    {
        // 2��° ������ ����
        SCItemSlot* pTmpItemSlot = pSelectedItemSlot;
        pSelectedItemSlot = pDeletedItemSlot;
        pDeletedItemSlot = pTmpItemSlot;
    }
    

    Output.RankUpFare = RankUpFare;
    Output.RankUpItemPos = pSelectedItemSlot->GetPos();
    Output.DeleteItemPos = pDeletedItemSlot->GetPos();


    //�Ϻ� ��ũ�� ���� Ȯ�� ����
#ifdef __JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE
    if( !IsSuccessToRankUpRatio( *pSelectedItemSlot ) )
        return RC::RC_ITEM_RANKUP_FAILED_AND_DESTROY;
#endif //__JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE

    // �α�
    GAMELOG->LogItemEx( ITEM_RANKUP_BEFORE, m_pPlayer, *pSelectedItemSlot, *pDeletedItemSlot );

    // ���� ��ũ���� �õ��Ѵ�.
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

    // ������ �� ����
    if( Requirement.GetItemInfo()->IsRankUpGradeRaiser() )
    {
        eResult = CheckEnableForRankUp( Equipment );
    }
    else // �ʱ�ȭ�� �� ����
    {
        eResult = CheckEnableForRankOptionChange( Equipment );
    }

    if( eResult != RC::RC_ITEM_SUCCESS )
        return eResult;

    // ��ũ�� ����� �ִ��� üũ
    if( m_pPlayer->GetMoney() < Output.RankUpFare )
    {
        return RC::RC_ITEM_INSUFFICIENT_MONEY;
    }

    // �α�
    GAMELOG->LogItemEx( ITEM_RANKUP_BEFORE, m_pPlayer, Equipment, Requirement );

    // ������ �� ����
    BOOLEAN bSucc = TRUE;
    if( Requirement.GetItemInfo()->IsRankUpGradeRaiser() )
    {
        bSucc = DoEquipmentRankUp( Equipment );
    }
    else // �ʱ�ȭ�� �� ����
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

    // ��ũ �ɼ� ����
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

    // ��ũ ����
    Equipment.SetRank( Rank );

    // ��ũ �ɼ� ����
    Equipment.SetRankAttr( Rank, pRankOption );

    // ���� ���� ����
    CreateSocketHole( Equipment, Rank );

    return TRUE;
}

RC::eITEM_RESULT RankUpImplement::CheckEnableForRankOptionChange( SCItemSlot& IN Equipment )
{
    if (Equipment.CanUseMixture(MIXTURE_RANKUP) == false) {
        return RC::RC_ITEM_NOT_MIXTURE;
    }

    // ��ũ�� �Ұ� ������.
    if( !Equipment.GetItemRuleInfo().IsEnableRank() )
        return RC::RC_ITEM_NOT_MIXTURE;

    // ����Ʈ �������� ��ũ�� �Ұ�
    if( Equipment.GetItemInfo()->IsElite() )
        return RC::RC_ITEM_CANNOT_RANKUP_ELITE;

    //����Ƽ�� �������� ��ũ�� �Ұ�
    if( Equipment.IsLimited() )
        return RC::RC_ITEM_CANNOT_RANKUP_LIMITED;


    // ��Ű �������� ��ũ�� �Ұ�
    if( Equipment.IsFateItem() )
        return RC::RC_ITEM_CANNOT_RANKUP_FATE;


    // ����ũ �������� ��ũ�� �Ұ�
    if( Equipment.GetItemInfo()->IsUnique() )
        return RC::RC_ITEM_CANNOT_RANKUP_UNIQUE;


    // E ��ũ �ɼ� �Ұ���
    if( Equipment.GetRank() <= RANK_E )
        return RC::RC_ITEM_INVALID_CONDITION;

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT RankUpImplement::CheckEnableForRankUp( SCItemSlot& IN Equipment )
{
    if (Equipment.CanUseMixture(MIXTURE_RANKUP) == false) {
        return RC::RC_ITEM_NOT_MIXTURE;
    }
    // ��ũ�� �Ұ� ������.
    if( !Equipment.GetItemRuleInfo().IsEnableRank() )
        return RC::RC_ITEM_NOT_MIXTURE;

    // ����Ʈ �������� ��ũ�� �Ұ�
    if( Equipment.GetItemInfo()->IsElite() )
        return RC::RC_ITEM_CANNOT_RANKUP_ELITE;

    // +S ��ũ�̻� ��ũ �Ұ���
    if( Equipment.GetRank() >= RANK_PS )
        return RC::RC_ITEM_NO_MORE_RANK;

    // ����Ƽ�� �������� ��ũ�� �Ұ�
    if( Equipment.IsLimited() )
        return RC::RC_ITEM_CANNOT_RANKUP_LIMITED;

    // ����ũ �������� ��ũ�� �Ұ�
    if( Equipment.GetItemInfo()->IsUnique() )
        return RC::RC_ITEM_CANNOT_RANKUP_UNIQUE;
    
    // ��Ű �������� ��ũ�� �Ұ�
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
                // �߱��̻��� ��þƮ���� ��
                if( Equipment.GetEnchantLV() < ENCHANT_MIDDLE )
                    return RC::RC_ITEM_INVALID_CONDITION;
            }break;
        case RANK_PA: 
        case RANK_MS: 
        case RANK_S:
            {
                // ����̻��� ��þƮ���� ��
                if( Equipment.GetEnchantLV() < ENCHANT_HIGH )
                    return RC::RC_ITEM_INVALID_CONDITION;
            }break;
        }
    }
    else if( Equipment.GetItemInfo()->IsAccessory() )
    {
        // ���� ����
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
        // ����
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
        // ��
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
                    // ����
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

    // ��ũ���� Ư�� �ɼ��� ������ ���� �ʴ´�.
    // ��å�� ���� ��� �̹� �پ� �ִ� �ɼ��� ���� ���� �ʰ� �ϱ� ���� DB�Ǵ� �Ľ� �κ���
    // ������ ū �۾��̹Ƿ�, ������ ����Ǵ� ��ũ������ Ư�� �Ӽ��� �����ؼ� �����ϱ� �����ڵ�.
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
    // Seed�� 100�� ������ byRankUpRatio�� 100�Ͻ� �����Ѵ�.
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