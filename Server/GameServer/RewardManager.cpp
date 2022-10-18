#include "StdAfx.h"
#include ".\rewardmanager.h"
#include <RewardInfoList.h>
#include <ItemTypeList.h>
#include <DropRatioInfoList.h>
#include "ItemManager.h"
#include "Player.h"
#include "ChaoState.h"

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
#include "AccumulatePoint.h"
#endif

#ifdef _NA_001990_ACSYSTEM_ADD
#include "AchievementManager.h"
#endif //_NA_001990_ACSYSTEM_ADD

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationInfoParser.h"
#include "GameZone.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

SLOTCODE
RewardManager::m_sRewardItemCode[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];

POSTYPE
RewardManager::m_sRewardItemNum[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];

BYTE
RewardManager::m_sRewardItemEnchant[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];

BYTE
RewardManager::m_sRewardKindOfItem[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
DWORD
RewardManager::reward_item_timelimit[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];

#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
// (CHANGES) (f100525.1L) change the name of the m_sRewardItemSet
// to the name of the m_sRewardKindOfItem
BOOST_STATIC_ASSERT(sizeof(BYTE) == sizeof(KindOfItemType::value_type) &&
                    static_cast<BYTE>(-1) == static_cast<KindOfItemType::value_type>(-1));
BYTE
RewardManager::m_sRewardItemRank[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];

BYTE
RewardManager::m_TotalCount = 0;

// external interface object
RewardManager g_RewardManager;

//==================================================================================================

RewardManager::RewardManager(void)
{
}

RewardManager::~RewardManager(void)
{
}

void
RewardManager::Init( ItemTypeList* pItemTypeList, DropRatioInfoList* pDropRatioInfoList )
{
	m_pItemTypeList = pItemTypeList;
	m_pDropRatioInfoList = pDropRatioInfoList;
}

//--------------------------------------------------------------------------------------------------

RC::eREWARD_RESULT RewardManager::RewardItem(eREWARD_TYPE reward_type, RewardInfo* reward_info_ptr, 
    BYTE select_item_bits, Player* player_ptr, OUT INVENTORY_TOTAL_INFO& inventory_total_info)
{
    ASSERT(reward_info_ptr != NULL);
    ASSERT(player_ptr != NULL);

#ifdef _NA_20100307_BETAKEY_SYSTEM
    if(!player_ptr->GetBetaKeyInfo()->CanAbilityContent(reward_info_ptr->GetBetaKey()))
    {
        return RC::RC_REWARD_INSUFFICIENT_SPACE;
    }
#endif

	BYTE total_count = 0;
    m_TotalCount = 0;
    BOOST_STATIC_ASSERT(sizeof(total_count) == sizeof(m_TotalCount));

	inventory_total_info.m_TmpInvenCount = 0;
	inventory_total_info.m_InvenCount = 0;

    // (WAVERIX) (090202) (BUG-FIX) IGTYPE ó�� ���� ���� ���� �ε����� ����Ű��
    //������ �ڵ尡 ���� ��, ���� �ٿ�Ǵ� ���� ����.

    // ���� ������ �ڵ�
    if (SLOTCODE fixed_item_code = reward_info_ptr->GetItemCode())
    {
        m_sRewardItemCode[total_count] = fixed_item_code;
        m_sRewardItemEnchant[total_count] = 0;
        m_sRewardKindOfItem[total_count] = KindOfItemType::Default; // (f100525.1L)
        m_sRewardItemRank[total_count] = 0;

        DWORD fixed_item_type_index = reward_info_ptr->GetItemTypeIndex();

        if (SCItemSlot::IsOverlap(fixed_item_code))
        {
            ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, fixed_item_type_index, POSTYPE));
            m_sRewardItemNum[total_count] = static_cast<POSTYPE>(fixed_item_type_index);
        }
        else
        {
            m_sRewardItemNum[total_count] = 1;

            ItemType* item_type_ptr = m_pItemTypeList->FindItemType(fixed_item_type_index);
            if (item_type_ptr != NULL)
            {
                m_sRewardItemEnchant[total_count] = item_type_ptr->GetEnchant();
                m_sRewardKindOfItem[total_count] = item_type_ptr->GetItemType(); // (f100525.1L)
                m_sRewardItemRank[total_count] = item_type_ptr->GetRank();
            }
        }        
        ++total_count;
    } // if (SLOTCODE fixed_item_code = reward_info_ptr->GetItemCode())
    
    for (int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)
    {
        // ���� ó���� �������� �˻��Ѵ�.
        const int reward_ref_type = reward_info_ptr->GetRefType();
        {   
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
            bool is_need_proc1 = (reward_ref_type == (int)RewardInfo::REF_TYPE_EVENT_ITEMCODE) ||
                                 (reward_ref_type == (int)RewardInfo::REF_TYPE_EVENT_DROPINFO) ||
                                 (reward_ref_type == (int)RewardInfo::REF_TYPE_DATETIME);
#else
            bool is_need_proc1 = (reward_ref_type == RewardInfo::REF_TYPE_EVENT_ITEMCODE) ||
                                 (reward_ref_type == RewardInfo::REF_TYPE_EVENT_DROPINFO);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
            
            bool is_need_proc2 = false;
            if (is_need_proc1 == false)
            {
                is_need_proc2 = (reward_ref_type == RewardInfo::REF_TYPE_SELECT) &&
                                ((RewardInfo::ITEM1_SELECTED << i) & select_item_bits);
            }

            bool is_need_proc3 = false;
            if (is_need_proc1 == false && is_need_proc2 == false) 
            {
                is_need_proc3 = (reward_ref_type == RewardInfo::REF_TYPE_RANDOM) &&
                                ((RewardInfo::ITEM1_SELECTED << i) & select_item_bits);
            }

            if ((is_need_proc1 || is_need_proc2 || is_need_proc3) == false) // ���꺸�� �˻簡 ���� �� '|' -> '||'
            {
                continue;
            }
        }

        //if(reward_ref_type == RewardInfo::REF_TYPE_EVENT_ITEMCODE || //< ���ð����� �ʿ����.
        //    reward_ref_type == RewardInfo::REF_TYPE_EVENT_DROPINFO || //< ���ð����� �ʿ����.
        //    (reward_ref_type == RewardInfo::REF_TYPE_SELECT &&
        //     ((BYTE)RewardInfo::ITEM1_SELECTED << i) & SelectItemBits) ||
        //    (reward_ref_type == RewardInfo::REF_TYPE_RANDOM &&
        //     ((BYTE)RewardInfo::ITEM1_SELECTED << i) & SelectItemBits))
        //{
        SLOTCODE item_code = 0;
        POSTYPE item_num = 0;
        DWORD type_index = 0;
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        DWORD time_limit = 0;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

        switch (reward_ref_type)
        {
        case RewardInfo::REF_TYPE_SELECT:
        case RewardInfo::REF_TYPE_EVENT_ITEMCODE:   //< �̺�Ʈ�� �Ѱ��ִ� ���(������ �ڵ尪)
            {
                DWORD type_code = reward_info_ptr->GetTypeCode(i);
                ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, type_code, SLOTCODE));

                item_code = static_cast<SLOTCODE>(type_code);                
                type_index = reward_info_ptr->GetTypeIndex(i);
                
                // �������� ������ �����Ҷ���
                if (SCItemSlot::IsOverlap(item_code))
                {
                    // ������ ������ �����Ѵ�.
                    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, type_index, POSTYPE));
                    item_num = static_cast<POSTYPE>(type_index);
                }                
            }
            break;
        case RewardInfo::REF_TYPE_RANDOM:
        case RewardInfo::REF_TYPE_EVENT_DROPINFO:   //< �̺�Ʈ�� �Ѱ��ִ� ���(������ �ڵ尪)
            {
                DWORD type_code = reward_info_ptr->GetTypeCode(i);
                DropObj* drop_item_ptr = m_pDropRatioInfoList->PopRandomItemTypeIndex(type_code);
                
                if ((drop_item_ptr == NULL) || (drop_item_ptr->GetType() != DROP_OBJ_ITEM)) 
                { //< �� ������Ʈ�� ����
                    item_code = 0;
                }
                else 
                {
                    DropItemObj* concreate_drop_item_ptr = static_cast<DropItemObj*>(drop_item_ptr);
                    ASSERT(concreate_drop_item_ptr != NULL);
                    
                    DWORD drop_item_code = concreate_drop_item_ptr->GetItemCode();
                    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, drop_item_code, SLOTCODE));

                    item_code = static_cast<SLOTCODE>(drop_item_code);
                    type_index = concreate_drop_item_ptr->GetReserved();

                    // �������� ������ �����Ҷ��� 
                    if (SCItemSlot::IsOverlap(item_code))
                    {
                        // ������ ������ �����Ѵ�.
                        ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, type_index, POSTYPE));
                        item_num = static_cast<POSTYPE>(type_index);
                    }
                }
            }
            break;
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        case RewardInfo::REF_TYPE_DATETIME:
            {
                DWORD type_code = reward_info_ptr->GetTypeCode(i);
                item_code = static_cast<SLOTCODE>(type_code);
                type_index = reward_info_ptr->GetTypeIndex(i);
                time_limit = (reward_info_ptr->GetTimeLimit(i) * 60);

                // �������� ������ �����Ҷ��� 
                if (SCItemSlot::IsOverlap(item_code))
                {
                    // ������ ������ �����Ѵ�.
                    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, type_index, POSTYPE));
                    item_num = static_cast<POSTYPE>(type_index);
                }
            }
            break;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        default:
            break;
        } // switch (reward_ref_type)

        if (item_code == 0)
        {
            continue;
        }
        
        // (WAVERIX) (090902) (ADD-IN) Can't find 'item code'
        ItemInfoParser* const item_info_parser_ptr = ItemInfoParser::Instance();
        const BASE_ITEMINFO* item_info_ptr = item_info_parser_ptr->GetItemInfo(item_code);
        if (item_info_ptr == NULL) 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Can't find reward item code %d", item_code);
            continue;
        }

        m_sRewardItemCode[total_count] = item_code;
        m_sRewardItemEnchant[total_count] = 0;
        m_sRewardKindOfItem[total_count] = KindOfItemType::Default; // (f100525.1L)
        m_sRewardItemRank[total_count] = 0;
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        reward_item_timelimit[total_count] = time_limit;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

        if (SCItemSlot::IsOverlap(item_code))
        {
            m_sRewardItemNum[total_count] = item_num;              
            ASSERT(m_sRewardItemNum[total_count] > 0 && "RewardItem Script Error - No Item number!!!");

            if (m_sRewardItemNum[total_count] <= 0)
            {
                SUNLOG(eCRITICAL_LOG, "RewardItem Script Error - No Item number!!![Index:%u]",
                    reward_info_ptr->GetIndex());
                m_sRewardItemNum[total_count] = 1;
            }
        }
        else // (SCItemSlot::IsOverlap(item_code))
        {
            // ��� ������               
            m_sRewardItemNum[total_count] = 1;

            // ���� ��ũ��Ʈ�� IGtype�ʵ带 Ÿ��, ������ ��������� �ν� �Ұ��ΰ�.
            // ������ �Ұ����Ͻ� Ÿ������ �ν�
            // ����å�� ���� �������� ��� �������� �Ұ��������� IGtype���� ������ �о�� �Ѵ�. (���� �߻�)

            // ���: RewardInfoList.txt ��ũ��Ʈ�� IGtype�� �ν��Ҷ�, �ΰ����� �ν� �˴ϴ�. 
            //1. ����������� �ߺ�   ������ �������̸�(�����), �������� �ν�.
            //2. ����������� �ߺ� �Ұ����� �������̸�(����), Ÿ�� �ε����� �ν�.

            // ���� �ߺ� �Ұ����� �������̸鼭, �������� �ν� �ǰ� �ִ� ��Ȳ.
            // ItemTypeList�� ItemTypeIndex�� 10000���� ���� �ϹǷ�, Ÿ�Կ� ���� ���� 0
            // ���ſ� ������ ���� ��� �ִ� ���� �߻�.

            //__NA_1324_IGTYPE_IMPROVEMENT
            // CHANGES: f101028.1L, change a filter rule. acceptable types containing a item_type
            // are weapons, armors, and pets.
            // Old rule
            //if (item_info_ptr->m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_EQUIP ||
            //    item_info_ptr->m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_BOTH)
            const bool plus_option_item = (item_info_ptr->IsWeapon() ||
                                           item_info_ptr->IsArmor() ||
                                           item_info_ptr->m_wType == eITEMTYPE_PET);
            if (plus_option_item)
            {
                ItemType* item_type_ptr = m_pItemTypeList->FindItemType(type_index);
                if (item_type_ptr != NULL) 
                {
                    m_sRewardItemEnchant[total_count] = item_type_ptr->GetEnchant();
                    m_sRewardKindOfItem[total_count] = item_type_ptr->GetItemType(); // (f100525.1L)
                    m_sRewardItemRank[total_count] = item_type_ptr->GetRank();
                }
            }
            else
            {
                //�������. �ߺ� �Ұ��� �������̸鼭, ���ó�� ���Ұ͵��� �̸��� ������.
            }
        } // (SCItemSlot::IsOverlap(item_code))
        ++total_count;
    } // for (int i = 0; i < RewardInfo::MAX_DROP_INDEX_NUM; ++i)

    // �������� ��´�.
    if (total_count != 0)
    {
        ItemManager* item_manager_ptr = player_ptr->GetItemManager();
        ASSERT(item_manager_ptr != NULL);

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        if ((item_manager_ptr->ObtainAllKindOfItem(total_count, m_sRewardItemCode, m_sRewardItemNum, 
            m_sRewardItemEnchant, m_sRewardKindOfItem, m_sRewardItemRank, &inventory_total_info, 
            reward_item_timelimit)) != RC::RC_ITEM_SUCCESS)
#else
        if ((item_manager_ptr->ObtainAllKindOfItem(total_count, m_sRewardItemCode, m_sRewardItemNum, 
            m_sRewardItemEnchant, m_sRewardKindOfItem, m_sRewardItemRank, &inventory_total_info)) != 
            RC::RC_ITEM_SUCCESS)
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        {
            return RC::RC_REWARD_INSUFFICIENT_SPACE;
        }
    }

    m_TotalCount = total_count;
    
    ASSERT(SAFE_NUMERIC_TYPECAST(eREWARD_TYPE, reward_type, BYTE));
    GAMELOG->LogItemReward(static_cast<BYTE>(reward_type), player_ptr, total_count, m_sRewardItemCode, 
        m_sRewardItemNum, m_sRewardItemEnchant, m_sRewardKindOfItem, m_sRewardItemRank, reward_info_ptr->GetIndex());

#ifdef _NA_001990_ACSYSTEM_ADD
    ItemInfoParser* const item_info_parser_ptr = ItemInfoParser::Instance();
    for(BYTE i = 0; i < total_count; i++)
    {
        SLOTCODE item_code  = m_sRewardItemCode[i];
        int item_count      = m_sRewardItemNum[i];

        const BASE_ITEMINFO* item_info_ptr = item_info_parser_ptr->GetItemInfo(item_code);
        if (item_info_ptr == NULL) 
            continue;

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
        // GetNotificationTypeFromRewardType()
        NotificationType notification_type = eNotificationNone;
        switch(reward_type)
        {
        case eREWARD_AC:
            notification_type = eNotificationAcReward;
            break;
        case eREWARD_MISSION:
            notification_type = eNotificationMissionReward;
            break;
		case eREWARD_MINIGAME_BINGO:
			notification_type = eNotificationBingoReward;
			break;
        }
        if (NotificationInfoParser::Instance()->IsNotificationItem(
            item_code, notification_type, 0, 0) == true)
        {
            MSG_DG_NOTIFICATION_ITEM_SYN msg;
            msg.info.SetInfo(player_ptr->GetCharName(),
                (player_ptr->GetGameZonePtr() != NULL) ? player_ptr->GetGameZonePtr()->GetMapCode() : 0,
                GetTickCount(), notification_type, item_code);
            g_pGameServer->SendToSessionType(GAME_DBPROXY, &msg, sizeof(MSG_DG_NOTIFICATION_ITEM_SYN));
        }
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

        player_ptr->GetACManager()->ProcessItemObtain(item_code, item_info_ptr->m_wACCode, 
            item_info_ptr->m_byACReferenceID, item_count);
    }
#endif //_NA_001990_ACSYSTEM_ADD

    return RC::RC_REWARD_SUCCESS;
}

//--------------------------------------------------------------------------------------------------

RC::eREWARD_RESULT
RewardManager::RewardItem( eREWARD_TYPE	eRtype, DWORD dwRewardInfoIndex, BYTE SelectItemBits, Player* pPlayer, INVENTORY_TOTAL_INFO& OUT TotalInfo )
{
	RewardInfo* pRewardInfo = RewardInfoList::Instance()->FindRewardInfo(dwRewardInfoIndex);
	if( !pRewardInfo ) return RC::RC_REWARD_FAILED;
	return RewardItem( eRtype, pRewardInfo, SelectItemBits, pPlayer, TotalInfo );
}

//--------------------------------------------------------------------------------------------------

RC::eREWARD_RESULT
RewardManager::RewardExp( RewardInfo* pRewardInfo, Player* pPlayer, EXPTYPE& OUT dwExp, WORD wApplyRatio )
{
	dwExp = pRewardInfo->GetExp();
	dwExp = (dwExp*wApplyRatio)/100;

	pPlayer->AddExp( dwExp, 0, 0, FALSE );
    #ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY 
    if (pPlayer->GetGroupLowMemberInfo() == GROUP_HAVE_ONLINE_LOW)
    {
        pPlayer->GroupBonusAcceptForEXP( pRewardInfo->GetExp(), dwExp, eEXP_ACQUISITION_QUEST );
    }
    #else
    if (pPlayer->GetGroupHighMemberInfo() == GROUP_HAVE_OFFLINE_HIGH
        || pPlayer->GetGroupHighMemberInfo() == GROUP_HAVE_ONLINE_HIGH)
    {
        pPlayer->GroupBonusAcceptForEXP( pRewardInfo->GetExp(), dwExp, eEXP_ACQUISITION_QUEST );
    }
    #endif
	dwExp = pPlayer->GetExp();

	return RC::RC_REWARD_SUCCESS;
}

//--------------------------------------------------------------------------------------------------

RC::eREWARD_RESULT
RewardManager::RewardHime( RewardInfo* pRewardInfo, Player* pPlayer, MONEY& OUT Hime, WORD wApplyRatio )
{
	Hime = pRewardInfo->GetHime();
	Hime = (Hime*wApplyRatio)/100;

	BOOL rt = pPlayer->PlusMoney( Hime );
	ASSERT(rt);

    #ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY
    if (pPlayer->GetGroupHighMemberInfo() == GROUP_HAVE_ONLINE_HIGH ||
        pPlayer->GetGroupHighMemberInfo() == GROUP_HAVE_OFFLINE_HIGH)
    {
        pPlayer->GroupBonusAcceptForMoney( pRewardInfo->GetHime(), Hime, eHEIM_ACQUISITION_QUEST );
    }
    #else
    if (pPlayer->GetGroupLowMemberInfo() == GROUP_HAVE_ONLINE_LOW) //�Ѹ��� �׷���̶� �¶����̶��
    {
        pPlayer->GroupBonusAcceptForMoney( pRewardInfo->GetHime(), Hime, eHEIM_ACQUISITION_QUEST );
    }
    #endif

	Hime = pPlayer->GetMoney();

	return RC::RC_REWARD_SUCCESS;
}

//--------------------------------------------------------------------------------------------------

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
RC::eREWARD_RESULT
RewardManager::RewardAccumulatePoint( RewardInfo * pRewardInfo, Player * pPlayer, WORD & OUT wAccumulatePoint )
{
    wAccumulatePoint = 0;

    AccumulatePoint* pAccumulate = pPlayer->GetAccumulatePoint();
    ASSERT(pAccumulate);
    if( !pAccumulate ) return RC::RC_REWARD_FAILED;

    if (pRewardInfo->GetCoin() > 0)
    {
        pAccumulate->PlusAccumulatePoint(pRewardInfo->GetCoin());
        wAccumulatePoint = pAccumulate->get_accumulate_point();

        return RC::RC_REWARD_SUCCESS;
    }
    
    return RC::RC_REWARD_EMPTY;
}
#endif

//--------------------------------------------------------------------------------------------------

RC::eREWARD_RESULT
RewardManager::RewardSkill( RewardInfo* pRewardInfo, Player* /*pPlayer*/, SLOTCODE& OUT SkillCode )
{
	SkillCode = pRewardInfo->GetSkillCode();

	//	1. ��ų ���� ó��
	//	2. (WAVERIX)(080327) - ���� ó���� �����ؼ� ��ȹ�� ���ǵ� ����
	//	-	��ų�� �ٷ� ȹ������ �ʴ´�.
	//	-	��ų ������ ������� �ʴ� �� �ش� ��ų�� ��� �� ������ �ϸ�,
	//	-	���� ���� ���ĺ��� ����Ʈ�� ������ ��� �� �ֵ��� �Ѵ�.

	return RC::RC_REWARD_SUCCESS;
}

//--------------------------------------------------------------------------------------------------

RC::eREWARD_RESULT
RewardManager::RewardChaoTime( RewardInfo* pRewardInfo, Player* pPlayer )
{
	int iTime  = pRewardInfo->GetChaoTime()* 60;
	if( iTime < 0 )
	{
		SUNLOG(eCRITICAL_LOG, "RewardChaoTime is Minus - [%s]", pPlayer->GetCharName());
		return RC::RC_REWARD_FAILED;
	}
	pPlayer->GetChaoState().DecreaseChaoTime( (WORD)iTime );	

	return RC::RC_REWARD_SUCCESS;
}

//--------------------------------------------------------------------------------------------------

RC::eREWARD_RESULT
RewardManager::RewardExp( DWORD dwRewardInfoIndex, Player* pPlayer, EXPTYPE& OUT dwExp, WORD wApplyRatio )
{
	RewardInfo* pRewardInfo = RewardInfoList::Instance()->FindRewardInfo(dwRewardInfoIndex);
	if( !pRewardInfo ) return RC::RC_REWARD_FAILED;

	return RewardExp( pRewardInfo, pPlayer, dwExp, wApplyRatio );
}

//--------------------------------------------------------------------------------------------------

RC::eREWARD_RESULT
RewardManager::RewardHime( DWORD dwRewardInfoIndex, Player* pPlayer, MONEY& OUT dwHime, WORD wApplyRatio )
{
	RewardInfo* pRewardInfo = RewardInfoList::Instance()->FindRewardInfo(dwRewardInfoIndex);
	if( !pRewardInfo ) return RC::RC_REWARD_FAILED;

	return RewardHime( pRewardInfo, pPlayer, dwHime, wApplyRatio );
}

//--------------------------------------------------------------------------------------------------

RC::eREWARD_RESULT
RewardManager::RewardSkill( DWORD dwRewardInfoIndex, Player* pPlayer, SLOTCODE& OUT SkillCode )
{
	RewardInfo* pRewardInfo = RewardInfoList::Instance()->FindRewardInfo(dwRewardInfoIndex);
	if( !pRewardInfo ) return RC::RC_REWARD_FAILED;
	return RewardSkill( pRewardInfo, pPlayer, SkillCode );
}

//--------------------------------------------------------------------------------------------------

RC::eREWARD_RESULT
RewardManager::RewardChaoTime( DWORD dwRewardInfoIndex, Player* pPlayer )
{
	RewardInfo* pRewardInfo = RewardInfoList::Instance()->FindRewardInfo(dwRewardInfoIndex);
	if( !pRewardInfo ) return RC::RC_REWARD_FAILED;

	return RewardChaoTime( pRewardInfo, pPlayer);
}

//--------------------------------------------------------------------------------------------------

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
RC::eREWARD_RESULT
RewardManager::RewardAccumulatePoint( DWORD dwRewardInfoIndex, Player * pPlayer, WORD & OUT wAccumulatePoint )
{
    RewardInfo* pRewardInfo = RewardInfoList::Instance()->FindRewardInfo(dwRewardInfoIndex);
    if( !pRewardInfo ) return RC::RC_REWARD_FAILED;

    return RewardAccumulatePoint( pRewardInfo, pPlayer, wAccumulatePoint );

}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
RC::eREWARD_RESULT RewardManager::RewardHonorTitle( DWORD reward_index, Player* player, WORD& OUT honor_title )
{
    RewardInfo* reward_info = RewardInfoList::Instance()->FindRewardInfo(reward_index);
    if( !reward_info )
        return RC::RC_REWARD_FAILED;

    honor_title = reward_info->GetTitle();
    if (honor_title > 0)
        player->GetHonorManager()->RequestAddHonorTitle(honor_title);

    return RC::RC_REWARD_SUCCESS;
}

RC::eREWARD_RESULT RewardManager::RewardReputePoint( DWORD reward_index, Player* player )
{
    RewardInfo* reward_info = RewardInfoList::Instance()->FindRewardInfo(reward_index);
    if( !reward_info )
        return RC::RC_REWARD_FAILED;

    WORD repute_code  = reward_info->GetReputeCode();
    WORD repute_point = reward_info->GetReputePoint();

    if(repute_code > 0 && repute_point > 0)
        player->GetHonorManager()->AddReputePoint(repute_code, repute_point);

    return RC::RC_REWARD_SUCCESS;
}
#endif //_NA_003027_20111013_HONOR_SYSTEM

#endif
