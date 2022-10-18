#pragma once

//=======================================================================================================================
/// RewardManager class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 4. 7
	@note
			- 보상스크립트 체계 통합 버전 ( 미션, 퀘스트, 이벤트 등등 )
	@history
			- 2006.4.14 : RewardInfo의 RefType이 0이고 TypeCode의 IsOverlap이 true이면 TypeIndex는 개수를 나타낸다.
			- 2006.4.14 : 랜덤 아이템 보상 함수 추가
*/
//=======================================================================================================================
class ItemTypeList;
class RewardInfo;
class DropRatioInfoList;

class RewardManager
{
public:
	// 게임 로그를 위해 타입 추가
	enum eREWARD_TYPE
	{
		eREWARD_MISSION,
		eREWARD_AC,
		eREWARD_QUEST,
		eREWARD_EVENT,
		eREWARD_COMPETITION,
		eREWARD_COMPOSE,
		eREWARD_MISSIONRANKING,
		eREWARD_EXCHANGE_ITEM,
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        eREWARD_ATTENDANCE,
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
        eREWARD_MINIGAME_BINGO,
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
	};
    struct ObtainedResult // 가장 최근에 보상 받기에 성공한 내역을 저장
    {
        enum { kMaxNumberOfKinds = 9 };
        BYTE number_of_kinds;
        SLOTCODE item_code_list[kMaxNumberOfKinds];
        POSTYPE item_num_list[kMaxNumberOfKinds];
        BYTE kind_of_item_list[kMaxNumberOfKinds];

        ObtainedResult()
        {
            Clear();
        }
        void Clear()
        {
            number_of_kinds = 0;
            ::ZeroMemory(item_code_list, sizeof(item_code_list));
            ::ZeroMemory(item_num_list, sizeof(item_num_list));
            ::ZeroMemory(kind_of_item_list, sizeof(kind_of_item_list));
        }
    };
	
public:
	RewardManager(void);
	virtual ~RewardManager(void);

	void Init( ItemTypeList * pItemTypeList, DropRatioInfoList * pDropRatioInfoList );

	RC::eREWARD_RESULT RewardItem( eREWARD_TYPE	eRtype,DWORD dwRewardInfoIndex, BYTE SelectItemBits,
		Player * pPlayer, INVENTORY_TOTAL_INFO & OUT TotalInfo );

	RC::eREWARD_RESULT RewardItem( eREWARD_TYPE	eRtype, RewardInfo * pRewardInfo, BYTE SelectItemBits, 
		Player * pPlayer, INVENTORY_TOTAL_INFO & OUT TotalInfo );

	RC::eREWARD_RESULT RewardExp( DWORD dwRewardInfoIndex, Player * pPlayer, EXPTYPE & OUT Exp, WORD wApplyRatio = 100 );
	RC::eREWARD_RESULT RewardExp( RewardInfo * pRewardInfo, Player * pPlayer, EXPTYPE & OUT Exp, WORD wApplyRatio = 100 );

	RC::eREWARD_RESULT RewardHime( DWORD dwRewardInfoIndex, Player * pPlayer, MONEY & OUT Hime, WORD wApplyRatio = 100 );
	RC::eREWARD_RESULT RewardHime( RewardInfo * pRewardInfo, Player * pPlayer, MONEY & OUT Hime, WORD wApplyRatio = 100 );

	RC::eREWARD_RESULT RewardSkill( DWORD dwRewardInfoIndex, Player * pPlayer, SLOTCODE & OUT SkillCode );
	RC::eREWARD_RESULT RewardChaoTime( DWORD dwRewardInfoIndex, Player * pPlayer );
	
	RC::eREWARD_RESULT RewardSkill( RewardInfo * pRewardInfo, Player * pPlayer, SLOTCODE & OUT SkillCode );
	RC::eREWARD_RESULT RewardChaoTime( RewardInfo * pRewardInfo, Player * pPlayer );

	SLOTCODE * GetRewardItemCode() { return m_sRewardItemCode; }

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    RC::eREWARD_RESULT RewardAccumulatePoint( DWORD dwRewardInfoIndex, Player * pPlayer, WORD & OUT wAccumulatePoint );
    RC::eREWARD_RESULT RewardAccumulatePoint( RewardInfo * pRewardInfo, Player * pPlayer, WORD & OUT wAccumulatePoint );
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    RC::eREWARD_RESULT RewardHonorTitle(DWORD reward_index, Player* player, WORD& OUT honor_title);
    RC::eREWARD_RESULT RewardReputePoint(DWORD reward_index, Player* player);
#endif //_NA_003027_20111013_HONOR_SYSTEM

    // 가장 최근에 보상받은 내역을 얻는다.
    void GetRecentObtainedResult(ObtainedResult& OUT obtained_result) const
    {
        obtained_result.Clear();
        if (m_TotalCount <= 0)
        {
            return;
        }
        if (m_TotalCount < ObtainedResult::kMaxNumberOfKinds)
        {
            obtained_result.number_of_kinds = m_TotalCount;
        }
        else
        {
            obtained_result.number_of_kinds = ObtainedResult::kMaxNumberOfKinds;
        }
        BOOST_STATIC_ASSERT(sizeof(obtained_result.item_code_list[0]) == sizeof(m_sRewardItemCode[0]));
        ::CopyMemory(
            obtained_result.item_code_list, 
            m_sRewardItemCode, 
            sizeof(m_sRewardItemCode[0]) * obtained_result.number_of_kinds);
        BOOST_STATIC_ASSERT(sizeof(obtained_result.item_num_list[0]) == sizeof(m_sRewardItemNum[0]));
        ::CopyMemory(
            obtained_result.item_num_list, 
            m_sRewardItemNum, 
            sizeof(m_sRewardItemNum[0]) * obtained_result.number_of_kinds);
        BOOST_STATIC_ASSERT(sizeof(obtained_result.kind_of_item_list[0]) == sizeof(m_sRewardKindOfItem[0]));
        ::CopyMemory(
            obtained_result.kind_of_item_list, 
            m_sRewardKindOfItem, 
            sizeof(m_sRewardKindOfItem[0]) * obtained_result.number_of_kinds);
        // CHANGES: f110306.1L, hummm... temporary logic. prevent another usage case.
        m_TotalCount = 0;
    }

private:
	static SLOTCODE	m_sRewardItemCode[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];
	static POSTYPE	m_sRewardItemNum[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];
	static BYTE		m_sRewardItemEnchant[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];
	static BYTE		m_sRewardKindOfItem[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];
	static BYTE		m_sRewardItemRank[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    static DWORD    reward_item_timelimit[INVENTORY_TOTAL_INFO::MAX_SLOT_NUM];
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
	static BYTE		m_TotalCount;
	ItemTypeList *	m_pItemTypeList;
	DropRatioInfoList *	m_pDropRatioInfoList;
};

extern RewardManager g_RewardManager;
