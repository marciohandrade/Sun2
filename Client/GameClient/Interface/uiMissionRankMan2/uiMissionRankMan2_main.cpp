#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)

#include "uiMissionRankMan2.h"
#include "uiMissionRankTimer/uiMissionRankTimer.h"
#include "uiMissionRankEnding/uiMissionRankEnding.h"
#include "uiMissionRankList2/uiMissionRankList2.h"
#include "uiMissionRankGamble/uiMissionRankGamble.h"
#include "uiRankMenu/uiRankMenu.h"

#include "uiPartyMan/uiPartyMan.h"
#include "ItemManager.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "ObjectManager.h"
#include "Map.h"
#include "uiPartyMan/uiPartyMan.h"

#include "ArchiveOfStream.h"

#include "ResultCode.h"
#include "GameFunc.h"

#include "MissionManager_Concrete.h"
#include "QuestManager_Concrete.h"

#include "Application.h"
#include "uiBattleZoneMan/uiBattleZoneMan_def.h"
#include "ChatMsg.h"
#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "SceneBase.h"

#ifdef _INTERNATIONAL_DATE_TOOLTIP
#include"uiToolTipMan/uiToolTipMan.h"
#endif//_INTERNATIONAL_DATE_TOOLTIP

const int c_iCheckPoint = 1000;

using namespace RC;

//==================================================================================================
typedef MissionRanking::RANKING_INFO_LIST RANKING_INFO_LIST;
typedef MissionRankingRecord::MEMBER_HASH MEMBER_HASH;
//==================================================================================================
/**
*/

inline BOOL RewardInfo_Funter::operator ()(stRewardInfo lhs, stRewardInfo rhs) const
{
    return ((lhs.ranking_id) < (rhs.ranking_id));
}

uiMissionRankMan2::uiMissionRankMan2(InterfaceManager *pUIMan) :
uiBaseMan(pUIMan)
{
    ui_rank_timer_ptr_	= NULL;
    ui_rank_ending_ptr_ = NULL;
    ui_rank_list_ptr_	= NULL;
    ui_rank_gamble_ptr_ = NULL;

    InitMission();
    ClearRankData();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::OnInitialize()
{
    _LoadUI();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::OnRelease()
{
    vecRqstMission.clear();
    ranking_reward_vector_.clear();
    reward_info_vector_.clear();
    confirmed_member_party_confirmed_list_.clear();
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::InitMission()
{
    m_dwTotalMonsterKillNum = 0;
    m_dwMaxMonsterKillNum = 0; 

    m_dwResultMissionClear			= 0;	//����!!�̼�����ð�
    m_dwTotalMonsterDecTimeSecond	= 0;	//�� ���� ���� �ð�
    m_dwGeneralMonsterDecSecond		= 0;	//�Ϲ� ���� ���� �ð�
    m_dwLeaderMonsterDecSecond		= 0;	//���� ���� �ð�

    m_dwMissionTime	= 0;
    m_dwStartMissionTime = 0; 
    m_dwUpdateMissionTime = 0;
    m_dwMissionClearTime = 0;



    m_bMissionCleared = false;
    m_dwMissionBestTime = 0;
    Is_enable_mission_ranking_ = false;

    m_iMyRank = 0;
    m_iMyPartyNum = 0;
    m_strMyPartyMasterName.clear();

    Is_wait_receive_packet_ = false;
    m_bRecvStartTimer = FALSE;
    Is_can_reward_ = false;

    confirmed_member_party_confirmed_list_.clear();
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::init_ranking_list_vector_()
{
    ranking_reward_vector_.clear();

    const RankingManager::MISSION_LEVEL_AREA_HASH& area_hash = GET_MISSION_RANK_MANAGER()->GetMissionLevelAreaTable();
    RankingManager::MISSION_LEVEL_AREA_HASH::const_iterator iter = area_hash.begin();

    stRewardSearchInfo reward_search_info_struct;

    for ( ; iter != area_hash.end(); ++iter)
    {
        RankingManager::LEVEL_AREA_BASE* area_ptr = const_cast<RankingManager::LEVEL_AREA_BASE*>(iter->second);

        if ( area_ptr )
        {
            reward_search_info_struct.ranking_id = area_ptr->m_Id;
            reward_search_info_struct.mission_number = area_ptr->m_MissionNo;
            reward_search_info_struct.mission_string = area_ptr->m_MissionString;
            reward_search_info_struct.ranking_system_type = static_cast<eRANKING_SYSTEM_TYPE>(area_ptr->m_RankingType);

            ranking_reward_vector_.push_back(reward_search_info_struct);
        }
    }   
    
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::ClearRankData()
{
    m_dwMyRankForReward	              = 0;
    current_mission_number_		      = 0;
    current_mission_system_rank_type_ = eRANKING_TYPE_NONE;
    m_CurRankPage		              = 0;

    GET_MISSION_RANK_MANAGER()->Release();
    GET_MISSION_RANK_MANAGER()->Initialize();
    GET_MISSION_RANK_MANAGER()->SetStatus( eRANKING_STATUS_NORMAL );

    ZeroMemory(&mission_party_info_array_, sizeof(mission_party_info_array_));

    ZeroMemory(&my_ranking_view_data_, sizeof(my_ranking_view_data_));
    ZeroMemory(&pre_week_rank_view_data_, sizeof(pre_week_rank_view_data_));
    ZeroMemory(&current_week_rank_view_data_, sizeof(current_week_rank_view_data_));
    ZeroMemory(&member_view_data_, sizeof(member_view_data_));

    m_PendingRankingType	= eRANKING_MAX;
    m_PendingMissionNo		= MAX_MISSION_NUM;
    m_PendingRankingKey		= -1;
    m_PendingPage			= 0;

    // ��� ��û�� ���
    m_RqstRankingType = eRANKING_MAX;
    m_RqstMissionNo = MAX_MISSION_NUM;

    vecRqstMission.clear();

    settle_date_dword_ = 0;
    finish_day_string_.clear();    

    Is_can_reward_ = false;
    Is_reward_list_packet_received_ = false;
    init_ranking_list_vector_();
    Is_wait_receive_packet_ = false;
    
    reward_info_vector_.clear();
    confirmed_member_party_confirmed_list_.clear();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::SetReceivedStartMissionTime(DWORD dwRecievedTime, DWORD dwMissionTime)
{
    m_dwStartMissionTime = dwRecievedTime;
    m_dwMissionTime = dwMissionTime;
    m_bRecvStartTimer = TRUE;
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::SetReceivedUpdateMissionTime(DWORD dwRecievedTime, DWORD dwMissionTime)
{
    m_dwUpdateMissionTime = dwRecievedTime;
    m_dwMissionTime = dwMissionTime;
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::SetClearTime(DWORD dwClearTime)
{
    m_dwMissionClearTime = dwClearTime;
    m_bMissionCleared = true;
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::DisplayTime()
{
    if(g_pMap && (IsMissionRanking((RANKINGNO)g_pMap->GetMapID(), eRANKING_TYPE_TIME_ATTACK) == false))	// �̼Ƿ�ŷ(Ÿ�Ӿ��ø��)�� �ش����� ������ Ÿ�̸Ӹ� �������� ����
        return;


    if ( ui_rank_timer_ptr_ )
    {
        //Ÿ�̸ӽ��� BRD�� �����ʾҴٸ� â�� ������ʴ´�.
        if(!m_bRecvStartTimer)
        {
            ui_rank_timer_ptr_->ShowInterface(FALSE);
            return;
        }


        if(ui_rank_timer_ptr_->IsVisibleWindow())
        {
            ui_rank_timer_ptr_->UpdateTime();
        }
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
        else
        {
            ui_rank_timer_ptr_->ShowInterface(TRUE);
        }
#else
        else
        {
            //�̼� �κ�â�� ���ִٸ� ��ũŸ�̸�â �ݴ´�.
            if( GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_MISSION_CFG )->IsVisible() )
            {
                ui_rank_timer_ptr_->ShowInterface(FALSE);
            }
            else
            {
                ui_rank_timer_ptr_->ShowInterface(TRUE);
            }
        }
#endif // defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::DisplayEnding(int iRank, int iPartyNum, TCHAR* pMasterName)
{
    m_iMyRank = iRank;
    m_iMyPartyNum = iPartyNum;
    m_strMyPartyMasterName = pMasterName;

    // �����ٰ��� �̸� �������
    m_dwGeneralMonsterDecSecond = m_dwMonsterKillNum_General * GENERAL_MON;	//�Ϲ� ���� ���� �ð�
    m_dwLeaderMonsterDecSecond = m_dwMonsterKillNum_Leader * LEADER_MON;		//���� ���� �ð�    
    m_dwTotalMonsterDecTimeSecond = m_dwGeneralMonsterDecSecond + m_dwLeaderMonsterDecSecond;	//�� ���� ���� �ð�
    
    m_dwResultMissionClear = m_dwMissionClearTime + m_dwTotalMonsterDecTimeSecond; // ���� kill �� ���� �� �ð�

    if ( ui_rank_ending_ptr_ )
    {
        ui_rank_ending_ptr_->ShowInterface(TRUE);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::CloseEndingDlg()
{
    if ( ui_rank_ending_ptr_ )
    {
        ui_rank_ending_ptr_->ShowInterface(FALSE);
    }
}
//------------------------------------------------------------------------------
/**
*/
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
TCHAR* uiMissionRankMan2::GetMemberName(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type, eRANKING_SYSTEM_TYPE ranking_system_type)
{
    if (MissionRanking* const mission_ranking =
        GET_MISSION_RANK_MANAGER()->FindMissionRanking(type, no, ranking_system_type))
    {
        if (MissionRankingRecord* record = mission_ranking->FindRanking(key))
        {
            const MEMBER_HASH& members = record->GetMemberHash();
            MEMBER_HASH::const_iterator it = members.begin(),
                end = members.end();
            for (; it != end; ++it)
            {
                if (RankingMember* member = it->second)
                {                    
                    return const_cast<TCHAR*>(member->GetCharName());
                }
            }
        }
    }    

    return NULL;
}
//------------------------------------------------------------------------------
/**
*/
int uiMissionRankMan2::GetMemberLV(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type, eRANKING_SYSTEM_TYPE ranking_system_type)
{
    if (MissionRanking* const mission_ranking =
        GET_MISSION_RANK_MANAGER()->FindMissionRanking(type, no, ranking_system_type))
    {
        if (MissionRankingRecord* record = mission_ranking->FindRanking(key))
        {
            const MEMBER_HASH& members = record->GetMemberHash();
            MEMBER_HASH::const_iterator it = members.begin(),
                end = members.end();
            for (; it != end; ++it)
            {
                if (RankingMember* member = it->second)
                {
                    return member->GetLV();
                }
            }
        }
    }
    return 0;
}
#else
TCHAR* uiMissionRankMan2::GetMemberName(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type)
{
    if (MissionRanking* const mission_ranking =
        GET_MISSION_RANK_MANAGER()->FindMissionRanking(type, no))
    {
        if (MissionRankingRecord* record = mission_ranking->FindRanking(key))
        {
            const MEMBER_HASH& members = record->GetMemberHash();
            MEMBER_HASH::const_iterator it = members.begin(),
                end = members.end();
            for (; it != end; ++it)
            {
                if (RankingMember* member = it->second)
                {                    
                    return const_cast<TCHAR*>(member->GetCharName());
                }
            }
        }
    }    

    return NULL;
}
//------------------------------------------------------------------------------
/**
*/
int uiMissionRankMan2::GetMemberLV(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type)
{
    if (MissionRanking* const mission_ranking =
        GET_MISSION_RANK_MANAGER()->FindMissionRanking(type, no))
    {
        if (MissionRankingRecord* record = mission_ranking->FindRanking(key))
        {
            const MEMBER_HASH& members = record->GetMemberHash();
            MEMBER_HASH::const_iterator it = members.begin(),
                end = members.end();
            for (; it != end; ++it)
            {
                if (RankingMember* member = it->second)
                {
                    return member->GetLV();
                }
            }
        }
    }
    return 0;
}
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::ShowRankList(int iType)
{    
    if (iType == TRUE)
    {
        ui_rank_list_ptr_->ShowInterface( TRUE );
    }

    if ( ui_rank_gamble_ptr_ )
    {
        ui_rank_gamble_ptr_->ShowInterface(FALSE);
    }
}


//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::ShowRankGamble()
{
    if ( ui_rank_gamble_ptr_ )
    {
        ui_rank_gamble_ptr_->ShowInterface(TRUE);
    }

    if ( ui_rank_list_ptr_ )
    {
        ui_rank_list_ptr_->ShowInterface( FALSE );
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::CloseMissionRunningDlg()
{
    if ( ui_rank_timer_ptr_ )
    {
        ui_rank_timer_ptr_->ShowInterface(FALSE);
    }

    if ( ui_rank_ending_ptr_ )
    {
        ui_rank_ending_ptr_->ShowInterface(FALSE);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::CloseMissionRankingDlg()
{
    if ( ui_rank_list_ptr_ )
    {
        ui_rank_list_ptr_->ShowInterface( FALSE );
    }

    if ( ui_rank_gamble_ptr_ )
    {
        ui_rank_gamble_ptr_->ShowInterface(FALSE);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::GetTimeString(DWORD dwTime, util::_tstring &strTime)
{
    TCHAR szTime[INTERFACE_STRING_LENGTH] = {0,};

    DWORD minTick   = 0;
    DWORD secTick   = 0;
    DWORD msecTick  = 0;

    if(dwTime)
    {
        DWORD dwTimeTemp1 = dwTime / SEC_PER_TICK;
        DWORD dwTimeTemp2 = dwTime % SEC_PER_TICK;

        if(dwTimeTemp1)
        {
            minTick   = dwTimeTemp1 / 60;
            secTick   = dwTimeTemp1 % 60;
        }

        if(dwTimeTemp2)
            msecTick  = dwTimeTemp2 / 10;
    }

    strTime.clear();
    Snprintf (szTime, INTERFACE_STRING_LENGTH-1, _T("%03d'%02d''%02d"), minTick, secTick, msecTick);
    strTime = szTime;
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::SetFinishDay(DWORD dwSettledDate)
{
    finish_day_string_.clear();

    settle_date_dword_ = dwSettledDate;

    if(settle_date_dword_ > 0)
    {

        int iMonth = settle_date_dword_ % 10000;
        int iYear = settle_date_dword_ / 10000;

        if(iMonth)
        {
            int iDay = iMonth % 100;
            iMonth = iMonth / 100;

            TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
#ifdef _INTERNATIONAL_DATE_TOOLTIP
            uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
            if(pUIMan)
            {
                int nIndex = pUIMan->GetStringMonth(iMonth);
                TCHAR szMonth[INTERFACE_STRING_LENGTH] = {0,};
                g_InterfaceManager.GetInterfaceString(nIndex, szMonth);//������
                Snprintf(szResult, INTERFACE_STRING_LENGTH-1, _T(" %s %02d, %02d"), 
                    szMonth, 
                    iDay, 
                    iYear);
            }
#else
            Snprintf(szResult, INTERFACE_STRING_LENGTH-1, _T("%02d-%02d-%02d"), 
                iYear, 
                iMonth, 
                iDay);
#endif//_INTERNATIONAL_UI

            finish_day_string_ = szResult;

        }
    }    

    if ( ui_rank_list_ptr_ )
    {
        ui_rank_list_ptr_->UpdateFinishDay();
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::SetReceiveData(BYTE type, RANKINGNO no, BYTE page)	// �����κ��� �޾ƿ� �����͸� �����Ѵ�
{    
    bool bPointRanking = false;

#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)    
    if ( current_mission_system_rank_type_ >= eRANKING_TYPE_POINT 
        && current_mission_system_rank_type_ < eRANKING_TYPE_MAX )
    {
        // ����Ʈ ��ŷ Ÿ�� �̻��̸� ���� ������ ����ϱ� ������ bPointRanking���� ���� �Ѵ�.
        bPointRanking = true;
    }
    else if ( current_mission_system_rank_type_ <= eRANKING_TYPE_TIME_ATTACK 
        && current_mission_system_rank_type_ > eRANKING_TYPE_NONE )
    {

    }
    else
    {
        // ���⿡ ������ ���ƾ� �Ѵ�.
        return;
    }
#else
    if ( ui_rank_list_ptr_ && 
        ui_rank_list_ptr_->get_current_select_ranking_type_() == eRANKING_TYPE_POINT ) //����Ʈ��ŷ
    {
        bPointRanking = true;
    }
    if ( ui_rank_list_ptr_ && (ui_rank_list_ptr_->get_current_select_ranking_type_() < 1 || 
        ui_rank_list_ptr_->get_current_select_ranking_type_() > eRANKING_TYPE_MAX))
    {
        return;
    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

    current_mission_number_ = no;
    current_mission_system_rank_type_ = ui_rank_list_ptr_->get_current_select_ranking_type_();    
    m_CurRankPage = page;

    ZeroMemory(&member_view_data_, sizeof(member_view_data_));

    MissionRanking *pMR = NULL;
    int iRank=1;
    int iIndex = 0;    

    if((type == eRANKING_MYRANKING) && (page == 0))	// �������� �������� 0�϶��� ���� ����
    {
        ZeroMemory(&my_ranking_view_data_, sizeof(my_ranking_view_data_));
        // �� ����

#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_MYRANKING, current_mission_number_, current_mission_system_rank_type_);
#else
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_MYRANKING, current_mission_number_);
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

        if(pMR)
        {
            const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();

            iRank=1;
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                end = mission_list.end();
            for (; it != end; ++it)
            {
                if(iRank > 1)	// ���� ��ŷ ������ 1�� �ִ�
                    break;

                MissionRankingRecord* const record = (*it);
                // Ŭ���� Ÿ�� �� ����Ʈ�� 0���� �۰ų� ������ continue �� �߰����� �ʴ´�.
                if (static_cast<DWORD>(record->GetClearTime()) <= 0)
                    break;
                
                my_ranking_view_data_.iPartyNum = record->GetKey();				// ��Ƽ��ȣ
                my_ranking_view_data_.dwTime = (DWORD)(record->GetClearTime());	// ����ð�
                //m_FameData.iRank = pMR->GetRanking(m_FameData.iPartyNum); // ����
                // ������ �ִ� �ſ��� ���� ������ �����´�.
                std::vector<stRqstMissionRankInfo>::iterator iter = vecRqstMission.begin();
                for ( ; iter != vecRqstMission.end(); ++iter)
                {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                    if ((*iter).no == current_mission_number_
                        && (*iter).ranking_system_type == current_mission_system_rank_type_)
                    {
                        my_ranking_view_data_.iRank = (*iter).myranking;
                        break;
                    }
#else
                    if ((*iter).no == current_mission_number_)
                    {
                        my_ranking_view_data_.iRank = (*iter).myranking;
                        break;
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
                }

                if(bPointRanking)
                {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                    // ����
                    my_ranking_view_data_.iLevel = GetMemberLV(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING, current_mission_system_rank_type_);	// ����

                    //�̸�
                    if (GetMemberName(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING, current_mission_system_rank_type_) != NULL)
                    {
                        StrnCopy(my_ranking_view_data_.strName, GetMemberName(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING, current_mission_system_rank_type_), MAX_CHARNAME_LENGTH);
                    }
#else
                    // ����
                    my_ranking_view_data_.iLevel = GetMemberLV(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING);	// ����

                    //�̸�
                    if (GetMemberName(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING) != NULL)
                    {
                        StrnCopy(my_ranking_view_data_.strName, GetMemberName(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING), MAX_CHARNAME_LENGTH);
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
                    else
                    {
                        TCHAR empty_name[INTERFACE_STRING_LENGTH] = {0, };
                        // 5477 %s ������ ĳ����
                        g_InterfaceManager.GetInterfaceString(5477, empty_name, INTERFACE_STRING_LENGTH);
                        StrnCopy(my_ranking_view_data_.strName, empty_name, MAX_CHARNAME_LENGTH);
                    }
                }
                else
                {
                    my_ranking_view_data_.iLevel = record->GetAvrgLVClient();				// ��շ���
                }
                ++iRank;
            }
        }        
    }
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
    else if(type == eRANKING_LASTWEEK)
#else
    else if((type == eRANKING_LASTWEEK) && (page == 0))	// ������ ������ �������� 0�϶��� ���� ����
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
    {
        ZeroMemory(&pre_week_rank_view_data_, sizeof(pre_week_rank_view_data_));
        // ������ ��ŷ

#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
        int column_count = 0;
        int start_preweek_rank = (page*MAX_PREWEEK_RANK) + 1;
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_LASTWEEK, current_mission_number_, current_mission_system_rank_type_);
#else
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_LASTWEEK, current_mission_number_);
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

        if(pMR)
        {
            const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();

            iIndex = 0;
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                end = mission_list.end();
            for (; it != end; ++it)
            {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                ++column_count;
                if(column_count < start_preweek_rank)
                    continue;
                
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

                MissionRankingRecord* const record = (*it);
                // Ŭ���� Ÿ�� �� ����Ʈ�� 0���� �۰ų� ������ continue �� �߰����� �ʴ´�.
                if (static_cast<DWORD>(record->GetClearTime()) <= 0)
                    continue;

                if(iIndex >= MAX_PREWEEK_RANK)	//  �����ִ� 3��������
                    break;
                
                pre_week_rank_view_data_[iIndex].iPartyNum = record->GetKey();				// ��Ƽ��ȣ
                pre_week_rank_view_data_[iIndex].dwTime = (DWORD)(record->GetClearTime());	// ����ð�
                pre_week_rank_view_data_[iIndex].iRank = pMR->GetRanking(pre_week_rank_view_data_[iIndex].iPartyNum);	// ����

                if(bPointRanking)
                {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                    pre_week_rank_view_data_[iIndex].iLevel = 
                        GetMemberLV(no, pre_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type, current_mission_system_rank_type_);		// ����
                    // �̸�           
                    if ( GetMemberName(no, pre_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type, current_mission_system_rank_type_) != NULL )
                    {
                        StrnCopy(pre_week_rank_view_data_[iIndex].strName, 
                            GetMemberName(no, pre_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type, current_mission_system_rank_type_),
                            MAX_CHARNAME_LENGTH);
                    }
#else
                    pre_week_rank_view_data_[iIndex].iLevel = 
                        GetMemberLV(no, pre_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type);		// ����
                    // �̸�           
                    if ( GetMemberName(no, pre_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type) != NULL )
                    {
                        StrnCopy(pre_week_rank_view_data_[iIndex].strName, 
                            GetMemberName(no, pre_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type),
                            MAX_CHARNAME_LENGTH);
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
                    else
                    {
                        TCHAR empty_name[INTERFACE_STRING_LENGTH] = {0, };
                        // 5477 %s ������ ĳ����
                        g_InterfaceManager.GetInterfaceString(5477, empty_name, INTERFACE_STRING_LENGTH);
                        StrnCopy(pre_week_rank_view_data_[iIndex].strName, empty_name, MAX_CHARNAME_LENGTH);
                    }
                }
                else
                {
                    pre_week_rank_view_data_[iIndex].iLevel = record->GetAvrgLVClient();				// ��շ���
                }

                ++iIndex;
            }
        }
    }
    else if(type == eRANKING_TODAYWEEK)
    {
        ZeroMemory(&current_week_rank_view_data_, sizeof(current_week_rank_view_data_));

        int iStartWeekRank = (page*MAX_WEEK_RANK) + 1;

        // �̹��� ��ŷ

#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
        int column_count = 0;
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_TODAYWEEK, current_mission_number_, current_mission_system_rank_type_);
#else
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_TODAYWEEK, current_mission_number_);
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

        if(pMR)
        {
            const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();

            iIndex = 0;
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                end = mission_list.end();
            for (; it != end; ++it)
            {
                MissionRankingRecord* const record = (*it);
                int iKey = record->GetKey();				// ��Ƽ��ȣ
                int iWeekRank = pMR->GetRanking(iKey);	// ����

#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                ++column_count;
                if(column_count < iStartWeekRank)
                    continue;

                // Ŭ���� Ÿ�� �� ����Ʈ�� 0���� �۰ų� ������ continue �� �߰����� �ʴ´�.
                if (static_cast<DWORD>(record->GetClearTime()) <= 0)
                    continue;
#else
                if(iWeekRank < iStartWeekRank)
                    continue;
                
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

                if(iIndex >= MAX_WEEK_RANK)	// ���������� 10���� �� 50��
                    break;

                current_week_rank_view_data_[iIndex].iPartyNum = iKey;							// ��Ƽ��ȣ
                current_week_rank_view_data_[iIndex].dwTime = (DWORD)(record->GetClearTime());	// ����ð�
                current_week_rank_view_data_[iIndex].iRank = iWeekRank;	// ����

                if(bPointRanking)
                {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                    current_week_rank_view_data_[iIndex].iLevel = 
                        GetMemberLV(no, current_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type, current_mission_system_rank_type_);		//����
                    //�̸�
                    if ( GetMemberName(no, current_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type, current_mission_system_rank_type_) != NULL )
                    {
                        StrnCopy(current_week_rank_view_data_[iIndex].strName, 
                            GetMemberName(no, current_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type, current_mission_system_rank_type_),
                            MAX_CHARNAME_LENGTH);
                    }
#else
                    current_week_rank_view_data_[iIndex].iLevel = 
                        GetMemberLV(no, current_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type);		//����
                    //�̸�
                    if ( GetMemberName(no, current_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type) != NULL )
                    {
                        StrnCopy(current_week_rank_view_data_[iIndex].strName, 
                            GetMemberName(no, current_week_rank_view_data_[iIndex].iPartyNum, (eRANKING_TYPE)type),
                            MAX_CHARNAME_LENGTH);
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
                    else
                    {
                        TCHAR empty_name[INTERFACE_STRING_LENGTH] = {0, };
                        // 5477 %s ������ ĳ����
                        g_InterfaceManager.GetInterfaceString(5477, empty_name, INTERFACE_STRING_LENGTH);
                        StrnCopy(current_week_rank_view_data_[iIndex].strName, empty_name, MAX_CHARNAME_LENGTH);
                    }
                }
                else
                {
                    current_week_rank_view_data_[iIndex].iLevel = record->GetAvrgLVClient();				// ��շ���
                }

                ++iIndex;
            }
        }
    }

    if ( ui_rank_list_ptr_ )
    {
        ui_rank_list_ptr_->UpdateRankList();
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::UpdateReceiveData(eRANKING_SYSTEM_TYPE type, RANKINGNO no, BYTE page)
{	
    bool bPointRanking = false;

#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
    if ( type >= eRANKING_TYPE_POINT 
         && type < eRANKING_TYPE_MAX )
    {
        // ����Ʈ ��ŷ Ÿ�� �̻��̸� ���� ������ ����ϱ� ������ bPointRanking���� ���� �Ѵ�.
        bPointRanking = true;
    }
    else if ( type <= eRANKING_TYPE_TIME_ATTACK 
              && type > eRANKING_TYPE_NONE )
    {

    }
    else
    {
        // ���⿡ ������ ���ƾ� �Ѵ�.
        return;
    }
#else
    if(GET_MISSION_RANK_MANAGER()->GetRankingType(no) == eRANKING_TYPE_POINT) //����Ʈ��ŷ
    {
        bPointRanking = true;
    }
    else if(GET_MISSION_RANK_MANAGER()->GetRankingType(no) == eRANKING_TYPE_TIME_ATTACK) //�̼Ƿ�ŷ
    {
    }
    else if(GET_MISSION_RANK_MANAGER()->GetRankingType(no) == eRANKING_TYPE_BATTLEGROUND_WIN) // �·�
    {

    }
    else if (GET_MISSION_RANK_MANAGER()->GetRankingType(no) == eRANKING_TYPE_BATTLEGROUND_KILL) // ų
    {

    }
    else if (GET_MISSION_RANK_MANAGER()->GetRankingType(no) == eRANKING_TYPE_BATTLEGROUND_ASSIST) // ��ý�Ʈ
    {

    }
    else
    {
        return;
    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

    current_mission_number_ = no;
    current_mission_system_rank_type_ = type;
    m_CurRankPage = page;

    // ���� ���� �ش� �����͸� ������ �ִٸ� ����Ʈ�� ����
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
    if ( IsRankData(no, page, type ) )
#else
    if(IsRankData(no, page))
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
    {
        ZeroMemory(&my_ranking_view_data_, sizeof(my_ranking_view_data_));
        ZeroMemory(&pre_week_rank_view_data_, sizeof(pre_week_rank_view_data_));
        ZeroMemory(&current_week_rank_view_data_, sizeof(current_week_rank_view_data_));
        ZeroMemory(&member_view_data_, sizeof(member_view_data_));

        MissionRanking *pMR = NULL;
        int iRank=1;
        int iIndex = 0;

        // ���� ��ŷ ����
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_MYRANKING, current_mission_number_, current_mission_system_rank_type_);
#else
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_MYRANKING, current_mission_number_);
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

        if(pMR)
        {
            const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();

            iRank=1;	
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                end = mission_list.end();
            for (; it != end; ++it)
            {
                if(iRank > MAX_FAME_RANK)	// ���� ��ŷ ������ 1�� �ִ�
                    break;
                MissionRankingRecord* const record = (*it);
                if (static_cast<DWORD>(record->GetClearTime()) <= 0)
                    break;
                my_ranking_view_data_.iPartyNum = record->GetKey();				// ��Ƽ��ȣ
                my_ranking_view_data_.dwTime = (DWORD)(record->GetClearTime());	// ����ð�

                // ������ �ִ� �ſ��� ���� ������ �����´�.
                std::vector<stRqstMissionRankInfo>::iterator iter = vecRqstMission.begin();
                for ( ; iter != vecRqstMission.end(); ++iter)
                {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                    if ((*iter).no == current_mission_number_ && 
                        (*iter).ranking_system_type == current_mission_system_rank_type_)
                    {
                        my_ranking_view_data_.iRank = (*iter).myranking;
                    }
#else
                    if ((*iter).no == current_mission_number_)
                    {
                        my_ranking_view_data_.iRank = (*iter).myranking;
                        break;
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
                }                

                if(bPointRanking)
                {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                    my_ranking_view_data_.iLevel = GetMemberLV(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING, current_mission_system_rank_type_);	//����
                    //�̸�
                    if ( GetMemberName(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING, current_mission_system_rank_type_) != NULL)
                    {
                        StrnCopy(my_ranking_view_data_.strName, 
                            GetMemberName(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING, current_mission_system_rank_type_),
                            MAX_CHARNAME_LENGTH);
                    }
#else
                    my_ranking_view_data_.iLevel = GetMemberLV(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING);	//����
                    //�̸�
                    if ( GetMemberName(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING) != NULL)
                    {
                        StrnCopy(my_ranking_view_data_.strName, 
                            GetMemberName(no, my_ranking_view_data_.iPartyNum, eRANKING_MYRANKING),
                            MAX_CHARNAME_LENGTH);
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
                    else
                    {
                        TCHAR empty_name[INTERFACE_STRING_LENGTH] = {0, };
                        // 5477 %s ������ ĳ����
                        g_InterfaceManager.GetInterfaceString(5477, empty_name, INTERFACE_STRING_LENGTH);
                        StrnCopy(my_ranking_view_data_.strName, empty_name, MAX_CHARNAME_LENGTH);
                    }
                }
                else
                {
                    my_ranking_view_data_.iLevel = record->GetAvrgLVClient();				// ��շ���
                }


                ++iRank;

            }
        }        

        // ������ ��ŷ
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
        int column_count = 0;
        int start_preweek_rank = (page*MAX_PREWEEK_RANK) + 1;
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_LASTWEEK, current_mission_number_, current_mission_system_rank_type_);
#else
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_LASTWEEK, current_mission_number_);
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

        if(pMR)
        {
            const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();

            iIndex = 0;
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                end = mission_list.end();
            for (; it != end; ++it)
            {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                ++column_count;
                if(column_count < start_preweek_rank)
                    continue;

                MissionRankingRecord* const record = (*it);
                if (static_cast<DWORD>(record->GetClearTime()) <= 0)
                    continue;
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
                if(iIndex >= MAX_PREWEEK_RANK)	//  �����ִ� 3��������
                    break;
                
                pre_week_rank_view_data_[iIndex].iPartyNum = record->GetKey();				// ��Ƽ��ȣ
                pre_week_rank_view_data_[iIndex].dwTime = (DWORD)(record->GetClearTime());	// ����ð�
                pre_week_rank_view_data_[iIndex].iRank = pMR->GetRanking(pre_week_rank_view_data_[iIndex].iPartyNum);	// ����

                if(bPointRanking)
                {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                    pre_week_rank_view_data_[iIndex].iLevel = 
                        GetMemberLV(no, pre_week_rank_view_data_[iIndex].iPartyNum, eRANKING_LASTWEEK, current_mission_system_rank_type_);	//����
                    //�̸�
                    if ( GetMemberName(no, pre_week_rank_view_data_[iIndex].iPartyNum, eRANKING_LASTWEEK, current_mission_system_rank_type_) != NULL )
                    {
                        StrnCopy(pre_week_rank_view_data_[iIndex].strName, 
                            GetMemberName(no, pre_week_rank_view_data_[iIndex].iPartyNum, eRANKING_LASTWEEK, current_mission_system_rank_type_),
                            MAX_CHARNAME_LENGTH);
                    }
#else
                    pre_week_rank_view_data_[iIndex].iLevel = 
                        GetMemberLV(no, pre_week_rank_view_data_[iIndex].iPartyNum, eRANKING_LASTWEEK);	//����
                    //�̸�
                    if ( GetMemberName(no, pre_week_rank_view_data_[iIndex].iPartyNum, eRANKING_LASTWEEK) != NULL )
                    {
                        StrnCopy(pre_week_rank_view_data_[iIndex].strName, 
                            GetMemberName(no, pre_week_rank_view_data_[iIndex].iPartyNum, eRANKING_LASTWEEK),
                            MAX_CHARNAME_LENGTH);
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
                    else
                    {
                        TCHAR empty_name[INTERFACE_STRING_LENGTH] = {0, };
                        // 5477 %s ������ ĳ����
                        g_InterfaceManager.GetInterfaceString(5477, empty_name, INTERFACE_STRING_LENGTH);
                        StrnCopy(pre_week_rank_view_data_[iIndex].strName, empty_name, MAX_CHARNAME_LENGTH);
                    }
                }
                else
                {
                    pre_week_rank_view_data_[iIndex].iLevel = record->GetAvrgLVClient(); // ��շ���
                }

                ++iIndex;
            }
        }

        int iStartWeekRank = (page*MAX_WEEK_RANK) + 1;

        // �̹��� ��ŷ
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
        column_count = 0;
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_TODAYWEEK, current_mission_number_, current_mission_system_rank_type_);
#else
        pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(eRANKING_TODAYWEEK, current_mission_number_);
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

        if(pMR)
        {
            const RANKING_INFO_LIST& mission_list = pMR->GetMemberHash();

            iIndex = 0;
            RANKING_INFO_LIST::const_iterator it = mission_list.begin(),
                end = mission_list.end();
            for (; it != end; ++it)
            {
                MissionRankingRecord* const record = (*it);
                int iKey = record->GetKey();				// ��Ƽ��ȣ
                int iWeekRank = pMR->GetRanking(iKey);	// ����

#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                ++column_count;
                if(column_count < iStartWeekRank)
                    continue;

                if (static_cast<DWORD>(record->GetClearTime()) <= 0)
                    continue;
#else
                if(iWeekRank < iStartWeekRank)
                    continue;
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

                if(iIndex >= MAX_WEEK_RANK)	// ���������� 10���� �� 50��
                    break;

                current_week_rank_view_data_[iIndex].iPartyNum = iKey;							// ��Ƽ��ȣ
                current_week_rank_view_data_[iIndex].dwTime = (DWORD)(record->GetClearTime());	// ����ð�
                current_week_rank_view_data_[iIndex].iRank = iWeekRank;	// ����

                if(bPointRanking)
                {
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                    current_week_rank_view_data_[iIndex].iLevel = 
                        GetMemberLV(no, current_week_rank_view_data_[iIndex].iPartyNum, eRANKING_TODAYWEEK, current_mission_system_rank_type_);	//����
                    //�̸�
                    if ( GetMemberName(no, current_week_rank_view_data_[iIndex].iPartyNum, eRANKING_TODAYWEEK, current_mission_system_rank_type_) != NULL )
                    {
                        StrnCopy(current_week_rank_view_data_[iIndex].strName, 
                            GetMemberName(no, current_week_rank_view_data_[iIndex].iPartyNum, eRANKING_TODAYWEEK, current_mission_system_rank_type_),
                            MAX_CHARNAME_LENGTH);
                    }
#else
                    current_week_rank_view_data_[iIndex].iLevel = 
                        GetMemberLV(no, current_week_rank_view_data_[iIndex].iPartyNum, eRANKING_TODAYWEEK);	//����
                    //�̸�
                    if ( GetMemberName(no, current_week_rank_view_data_[iIndex].iPartyNum, eRANKING_TODAYWEEK) != NULL )
                    {
                        StrnCopy(current_week_rank_view_data_[iIndex].strName, 
                            GetMemberName(no, current_week_rank_view_data_[iIndex].iPartyNum, eRANKING_TODAYWEEK),
                            MAX_CHARNAME_LENGTH);
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
                    else
                    {
                        TCHAR empty_name[INTERFACE_STRING_LENGTH] = {0, };
                        // 5477 %s ������ ĳ����
                        g_InterfaceManager.GetInterfaceString(5477, empty_name, INTERFACE_STRING_LENGTH);
                        StrnCopy(current_week_rank_view_data_[iIndex].strName, empty_name, MAX_CHARNAME_LENGTH);
                    }
                }
                else
                {
                    current_week_rank_view_data_[iIndex].iLevel = record->GetAvrgLVClient();				// ��շ���
                }

                ++iIndex;
            }
        }
        
        if ( ui_rank_list_ptr_ )
        {
            ui_rank_list_ptr_->UpdateRankList();
        }
    }
    else
    {
        // �����Ͱ� ������ ������ sync ����        
        m_PendingRankingType    = eRANKING_MYRANKING;
        pending_ranking_system_type_ = current_mission_system_rank_type_;
        m_PendingMissionNo		= current_mission_number_;
        m_PendingPage			= m_CurRankPage;	

        this->Send_CG_MISSION_RANKING_INFO_SYN(m_PendingRankingType, m_PendingMissionNo, current_mission_system_rank_type_, m_PendingPage);
    }
}
//------------------------------------------------------------------------------
/**
*/
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
bool uiMissionRankMan2::IsRankData(RANKINGNO no, BYTE page, eRANKING_SYSTEM_TYPE ranking_system_type)
{
    for(unsigned int i=0; i<vecRqstMission.size(); ++i)
    {
        if((vecRqstMission[i].no == no) && (vecRqstMission[i].page == page) && 
            (vecRqstMission[i].ranking_system_type == ranking_system_type))
            return true;
    }

    return false;
}
#else
bool uiMissionRankMan2::IsRankData(RANKINGNO no, BYTE page)
{
    for(unsigned int i=0; i<vecRqstMission.size(); ++i)
    {
        if((vecRqstMission[i].no == no) && (vecRqstMission[i].page == page))
            return true;
    }

    return false;
}
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::Send_CG_MISSION_RANKING_MEMBER_INFO_SYN(RANKINGNO no, int iIndex, eRANKING_TYPE type)
{

    if(!Is_wait_receive_packet_)
    {
        ZeroMemory(&member_view_data_, sizeof(member_view_data_));

        RANKINGKEY key;
        
        if (type == eRANKING_MYRANKING)
        {
            // �� ����
            key = my_ranking_view_data_.iPartyNum;
        }
        else if(type == eRANKING_LASTWEEK)
        {
            // ������ ��ŷ
            key = pre_week_rank_view_data_[iIndex].iPartyNum;

        }
        else if(type == eRANKING_TODAYWEEK)
        {
            // �̹��� ��ŷ
            key = current_week_rank_view_data_[iIndex].iPartyNum;
        }

        // �̹� �����Ͱ� �ִ��� üũ        
        MissionRanking* pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(type, no);        

        if(pMR)
        {
            MissionRankingRecord * pRecord = pMR->FindRanking(key);
            if(pRecord)
            {
                const MEMBER_HASH& members = pRecord->GetMemberHash();
                stMemberConfirmedInfo member_confirmed_info;
                member_confirmed_info.pending_ranking_type = type;
                member_confirmed_info.ranking_key = key;
                member_confirmed_info.ranking_no  = no;
                if(members.empty() && get_confirmed_member_party_key(member_confirmed_info) == false)		// �����͸� ���� ���� ������ �������� ��û
                {
                    // �ѹ� ã������ ���� ���ʹ� ��Ŷ�� ��û���� �ʴ´�.
                    confirmed_member_party_confirmed_list_.push_back(member_confirmed_info);

                    m_RqstRankingType		= type;
                    m_RqstMissionNo			= no;
                    m_PendingRankingKey		= key;

                    MSG_CG_MISSION_RANKING_MEMBER_INFO_SYN sync;
                    sync.m_Type			= m_RqstRankingType;
                    sync.m_MissionNo	= m_RqstMissionNo;
                    sync.m_Key			= m_PendingRankingKey;
                    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
                    Is_wait_receive_packet_ = true;
                }                
                else	// ������ ���� �ִ°ɷ� �ذ�
                {                   
                    SetMemberData(no, key, type);
                    
                    if ( ui_rank_list_ptr_ )
                    {
                        ui_rank_list_ptr_->UpdateMemberList();
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::SetMemberData(RANKINGNO no, RANKINGKEY key, eRANKING_TYPE type)
{
    ZeroMemory(&member_view_data_, sizeof(member_view_data_));    

    if (MissionRanking* const mission_ranking =
        GET_MISSION_RANK_MANAGER()->FindMissionRanking(type, no))
    {
        if (MissionRankingRecord* record = mission_ranking->FindRanking(key))
        {
            const MEMBER_HASH& members = record->GetMemberHash();
            MEMBER_HASH::const_iterator it = members.begin(),
                end = members.end();
            if ( members.empty() )
            {
                member_view_data_[0].iLevel = 0;
                TCHAR empty_name[INTERFACE_STRING_LENGTH] = {0, };
                // 5477 %s ������ ĳ����
                g_InterfaceManager.GetInterfaceString(5477, empty_name, INTERFACE_STRING_LENGTH);
                StrnCopy(member_view_data_[0].strName, empty_name, MAX_CHARNAME_LENGTH);
                return;
            }
            for (int i = 0; i < _countof(member_view_data_) && it != end; ++i, ++it)
            {
                RankingMember* member = it->second;
                if (member == NULL)
                {
                    assert(!"unexpected status");
                    continue;
                };
                if (stMissionRankMemberData* dest = &member_view_data_[i])
                {
                    dest->iLevel = member->GetLV();                   
                    StrnCopy(dest->strGuild, member->GetGuildName(), MAX_GUILDNAME_LENGTH);
                    StrnCopy(dest->strName, member->GetCharName(), MAX_CHARNAME_LENGTH);
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::CheckEnableMissionRankingRecord()	// �̼� ���۽� ó������ üũ
{
    if(!g_pMap)
    {
        return;
    }

    RANKINGNO no = (RANKINGNO)g_pMap->GetMapID();
    if(IsMissionRanking(no, eRANKING_TYPE_TIME_ATTACK) == false)
    {
        Is_enable_mission_ranking_ = false;
        return;
    }

    ZeroMemory(&mission_party_info_array_, sizeof(mission_party_info_array_));
    uiPartyMan* partyMan = static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
    int iMemberCount = 0;

    if(partyMan)
    {
        STLX_VECTOR<uicPartyMemberInfoPlus>& vPartyMem = partyMan->GetPartyListRef();
        for (int i=0; i<partyMan->GetPartyNum(); ++i)
        {
            if(!vPartyMem[i].IsDead())
            {
                ++iMemberCount;
                mission_party_info_array_[i].m_Class	= vPartyMem[i].Get().m_byClassType;
                mission_party_info_array_[i].m_LV		= vPartyMem[i].Get().m_wLevel;
            }
        }			
    }

    RC::eRANKING_RESULT result = GET_MISSION_RANK_MANAGER()->CanRegisterMissionRecord(no, iMemberCount, mission_party_info_array_);

    if(result == RC_RANKING_SUCCESS)
        Is_enable_mission_ranking_ = true;
    else
    {
        Is_enable_mission_ranking_ = false;

        _processErrorCode(result, eOUTPUT_MSG_TOP_SHOW);
        _processErrorCode(result);
    }


}

//------------------------------------------------------------------------------
/**
*/
bool uiMissionRankMan2::IsEnableMissionRanking()
{
    return Is_enable_mission_ranking_;
}

//------------------------------------------------------------------------------
/**
*/
bool uiMissionRankMan2::DecPage()
{
    if(m_CurRankPage <= 0 || Is_wait_receive_packet_ == true)
        return false;

    --m_CurRankPage;

    UpdateReceiveData(current_mission_system_rank_type_, current_mission_number_, m_CurRankPage);
    return true;

}

//------------------------------------------------------------------------------
/**
*/
bool uiMissionRankMan2::IncPage()
{
    if(m_CurRankPage >= MAX_PAGE || Is_wait_receive_packet_ == true)
        return false;

    ++m_CurRankPage;

    UpdateReceiveData(current_mission_system_rank_type_, current_mission_number_, m_CurRankPage);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::Send_CG_MISSION_RANKING_INFO_SYN(BYTE byPendingRankingType, RANKINGNO PendingMissionNo, eRANKING_SYSTEM_TYPE ranking_system_type, BYTE byPendingPage)
{
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
    if ( !Is_wait_receive_packet_ )
    {
        RANKINGID ranking_id = GetSearchRankingId(PendingMissionNo, ranking_system_type);
        if (0 == ranking_id)
            return;

        MSG_CG_MISSION_RANKING_INFO_SYN sync;
        sync.m_Type			= byPendingRankingType;
        sync.m_RankingID    = ranking_id;
        sync.m_Page			= byPendingPage;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
        Is_wait_receive_packet_ = true;
    }
#else
    if ( !Is_wait_receive_packet_ )
    {
        MSG_CG_MISSION_RANKING_INFO_SYN sync;
        sync.m_Type			= byPendingRankingType;
        sync.m_RankingID    = GetSearchRankingId(PendingMissionNo, ranking_system_type);
        sync.m_Page			= byPendingPage;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
        Is_wait_receive_packet_ = true;
    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::Send_CG_MISSION_RANKING_REWARD_SYN()
{
    if ( reward_info_vector_.empty() )
        return;

    std::vector<stRewardInfo>::iterator iter = reward_info_vector_.begin();
    
    if ( !Is_wait_receive_packet_ )
    {
        MSG_CG_MISSION_RANKING_REWARD_SYN sync;
        sync.ranking_id = (*iter).ranking_id;
        sync.m_Key = (*iter).reward_ranking_key;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
        Is_wait_receive_packet_ = true;        
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::Send_CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN()
{
    MSG_CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN sync;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::NetworkProc( MSG_BASE * pMsg )
{

    TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR   szTemp[INTERFACE_STRING_LENGTH] = {0,};

    switch (pMsg->m_byCategory)
    {
    case CG_MISSION:
        {
            switch (pMsg->m_byProtocol)
            {
            case CG_MISSION_RECORD_ELAPSEDTIME_CMD:
                {
                    MSG_CG_MISSION_RECORD_ELAPSEDTIME_CMD *pPacket = (MSG_CG_MISSION_RECORD_ELAPSEDTIME_CMD *)pMsg;

                    SetReceivedStartMissionTime(clock_function::GetTickCount(), pPacket->m_MissionElapsedTimePerSecond * 1000);
                    this->m_dwMissionBestTime = pPacket->m_MissionBestClearTimePerSecond * 1000;

                    if(g_pMap)
                    {
                        //��ũ��Ʈ���� �о�;���
                        RANKINGNO no = (RANKINGNO)g_pMap->GetMapID();

                        RankingManager::LEVEL_AREA* pArea = GET_MISSION_RANK_MANAGER()->FindLVArea(no);

                        if(pArea)
                            m_dwMaxMonsterKillNum = pArea->m_MaxMonsterKill;
                    }

                    CheckEnableMissionRankingRecord();
                }
                break;


            case CG_MISSION_TIMER_CHANGED_BRD:	
                {
                    MSG_CG_MISSION_TIMER_CHANGED_BRD *pPacket = (MSG_CG_MISSION_TIMER_CHANGED_BRD *)pMsg;

                    m_dwTotalMonsterKillNum = pPacket->m_TotalMonsterKillNum;
#ifdef _DEBUG
                    TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
                    Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("\nCG_MISSION_TIMER_CHANGED_BRD: %ld\n"),pPacket->m_TotalMonsterKillNum);
                    OutputDebugString(szDebug);
#endif
                }
                break;

            case CG_MISSION_CLEAR_RECORD_CMD:
                {
                    MSG_CG_MISSION_CLEAR_RECORD_CMD *pPacket = (MSG_CG_MISSION_CLEAR_RECORD_CMD *)pMsg;
                    m_dwMonsterKillNum_General = pPacket->m_MonsterKillNum_General;
                    m_dwMonsterKillNum_Leader = pPacket->m_MonsterKillNum_Leader;

                    //////////////////////////////////////////////////////////////////////////				
                    // �ش系���� ������ �߸��� �κж����� �̺κп��� �̼� ī��Ʈ�� ���� ��Ű��� ������ ���� �߽��ϴ�.
                    if (g_pMission_Concrete && g_pMap)
                    {
                        // Ʈ���� �ʿ��� SetMissionPoint�� �÷� �ֱ� ������ ���⿡�� �Ǵٽ� �÷��� �ʿ䰡 ����.

                        if ( g_pQMGR_Real )
                        {
                            g_pQMGR_Real->Event( QUEST_EVENT_MISSION, NULL);
                            g_InterfaceManager.UpdateQuestInfo();
                        }					
                    }
                    //////////////////////////////////////////////////////////////////////////				
                }
                break;

            case CG_MISSION_RANKING_SUCCESS_BRD:
                {

                    MSG_CG_MISSION_RANKING_SUCCESS_BRD *pPacket = (MSG_CG_MISSION_RANKING_SUCCESS_BRD *)pMsg;

                    SetClearTime(pPacket->m_ClearTime);

                    if(g_pHero)
                    {
                        uiPartyMan* partyMan =
                            static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
                        if(partyMan)
                        {
                            uicPartyMemberInfoPlus *pInfo =	const_cast<uicPartyMemberInfoPlus *>(partyMan->GetPartyLeader());
                            if(pInfo)
                            {
                                TCHAR szMaster[MAX_CHARNAME_LENGTH+1] ={0,};
                                StrnCopy(szMaster, pInfo->Get().m_szCharName, MAX_CHARNAME_LENGTH);
                                DisplayEnding(pPacket->m_Ranking, pPacket->m_Key, szMaster);
                            }
                        }

                        //  1~50�� ������ �ý���â���� ���~
                        g_InterfaceManager.GetInterfaceString( 9908, szTemp, INTERFACE_STRING_LENGTH );	// �̼Ƿ�ŷ %d��
                        Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, pPacket->m_Ranking);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                    }
                }
                break;

            case CG_MISSION_RANKING_FAILED_BRD:
                {
                    MSG_CG_MISSION_RANKING_FAILED_BRD *pPacket = (MSG_CG_MISSION_RANKING_FAILED_BRD *)pMsg;

                    SetClearTime(pPacket->m_ClearTime);

                    _processErrorCode(pPacket->m_dwErrorCode, eOUTPUT_MSG_CENTER_SHOW);

                    _processErrorCode(pPacket->m_dwErrorCode, eOUTPUT_MSG_CHAT);
                }
                break;

            case CG_MISSION_RANKING_INFO_ACK:
                {
                    Is_wait_receive_packet_ = false;
                    // ���� ��Ŷ
                    MSG_CG_MISSION_RANKING_INFO_ACK *pPacket = (MSG_CG_MISSION_RANKING_INFO_ACK *)pMsg;

                    if (m_PendingRankingType == eRANKING_MYRANKING)
                    {
                        stRqstMissionRankInfo mssionInfo;
                        mssionInfo.no      = m_PendingMissionNo;
                        mssionInfo.page    = m_PendingPage;
                        mssionInfo.myranking = pPacket->myranking;
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                        // ��ŷ �ý��� Ÿ���� �߰�
                        mssionInfo.ranking_system_type = 
                            current_mission_system_rank_type_;
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

                        vecRqstMission.push_back(mssionInfo);
                    }

#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
                    if ((m_PendingRankingType != eRANKING_MYRANKING + 1) && m_PendingMissionNo != MAX_MISSION_NUM)
                    {                        
                        if ( current_mission_system_rank_type_ >= eRANKING_TYPE_POINT 
                            && current_mission_system_rank_type_ < eRANKING_TYPE_MAX )
                        {
                            // ����Ʈ ��ŷ Ÿ�� �̻��̸� ���� ������ ����ϱ� ������ ���߿� Ȯ���� ���ؼ� ���ǹ����� ó��
                            GET_MISSION_RANK_MANAGER()->Serialize((eRANKING_TYPE)m_PendingRankingType, m_PendingMissionNo, m_PendingPage, pPacket->m_pBUFFER, pPacket->m_Size, RANKING_SERIALIZE_SAVE, current_mission_system_rank_type_);                            
                        }
                        else if ( current_mission_system_rank_type_ <= eRANKING_TYPE_TIME_ATTACK 
                            && current_mission_system_rank_type_ > eRANKING_TYPE_NONE )
                        {
                            GET_MISSION_RANK_MANAGER()->SerializeForClient(static_cast<eRANKING_TYPE>(m_PendingRankingType),
                                m_PendingMissionNo, m_PendingPage, pPacket->m_pBUFFER, pPacket->m_Size, RANKING_SERIALIZE_CLIENT_SAVE);
                        }
                        else
                        {
                            // ���⿡ ������ ���ƾ� �Ѵ�.
                            break;
                        }

                        SetReceiveData(m_PendingRankingType, m_PendingMissionNo, m_PendingPage);
                    }
#else                    
                    if((m_PendingRankingType != eRANKING_MYRANKING + 1) && (m_PendingMissionNo != MAX_MISSION_NUM))
                    {
                        if(GET_MISSION_RANK_MANAGER()->GetRankingType(m_PendingMissionNo) == eRANKING_TYPE_TIME_ATTACK) //�̼Ƿ�ŷ
                        {                            
                            GET_MISSION_RANK_MANAGER()->SerializeForClient((eRANKING_TYPE)m_PendingRankingType, m_PendingMissionNo, m_PendingPage, pPacket->m_pBUFFER, pPacket->m_Size, RANKING_SERIALIZE_CLIENT_SAVE);                                                        
                        }
                        else if(GET_MISSION_RANK_MANAGER()->GetRankingType(m_PendingMissionNo) == eRANKING_TYPE_POINT) //����Ʈ��ŷ
                        {                            
                            GET_MISSION_RANK_MANAGER()->Serialize((eRANKING_TYPE)m_PendingRankingType, m_PendingMissionNo, m_PendingPage, pPacket->m_pBUFFER, pPacket->m_Size, RANKING_SERIALIZE_SAVE);
                        }
                        else
                        {
                            break;
                        }
                        
                        SetReceiveData(m_PendingRankingType, m_PendingMissionNo, m_PendingPage);
                    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR
                    
                    // ��¿�� ���� ����ó��.                    
                    if (m_PendingRankingType == eRANKING_MYRANKING)
                    {
                        m_PendingRankingType = eRANKING_LASTWEEK;
                    }
                    else
                    {
                        ++m_PendingRankingType;
                    }                    
                    
                    if(m_PendingRankingType < eRANKING_MAX)
                    {
                        m_PendingMissionNo		= current_mission_number_;
                        Send_CG_MISSION_RANKING_INFO_SYN(m_PendingRankingType, m_PendingMissionNo, pending_ranking_system_type_, m_PendingPage);	// ���� Ÿ�� ��û
                    }
                }
                break;

            case CG_MISSION_RANKING_INFO_NAK:
                {
                    Is_wait_receive_packet_ = false;

                    MSG_CG_MISSION_RANKING_INFO_NAK *pPacket = (MSG_CG_MISSION_RANKING_INFO_NAK *)pMsg;
                    _processErrorCode(pPacket->m_dwErrorCode);

                    if ( ui_rank_list_ptr_ )
                    {
                        ui_rank_list_ptr_->non_existent_my_ranking_list_control_item();
                    }

                    if(m_PendingRankingType < eRANKING_TODAYWEEK)
                    {
                        ++m_PendingRankingType;
                        m_PendingMissionNo		= current_mission_number_;
                        Send_CG_MISSION_RANKING_INFO_SYN(m_PendingRankingType, m_PendingMissionNo, pending_ranking_system_type_, m_PendingPage);	// ���� Ÿ�� ��û
                    }
                }
                break;

                // ��ŷ ��� ����
            case CG_MISSION_RANKING_MEMBER_INFO_ACK:
                {
                    Is_wait_receive_packet_ = false;

                    MSG_CG_MISSION_RANKING_MEMBER_INFO_ACK *pPacket = (MSG_CG_MISSION_RANKING_MEMBER_INFO_ACK *)pMsg;                    

                    MissionRanking* pMR = GET_MISSION_RANK_MANAGER()->FindMissionRanking(m_RqstRankingType, m_RqstMissionNo);

                    if(pMR)
                    {
                        MissionRankingRecord * pRecord = pMR->FindRanking(m_PendingRankingKey);
                        if(pRecord)
                        {
                            ArchiveOfStream lArchive( pPacket->m_pBUFFER, pPacket->m_Size );
                            pRecord->SerializeMember(lArchive, RANKING_SERIALIZE_CLIENT_SAVE);

                            SetMemberData(m_RqstMissionNo, m_PendingRankingKey, m_RqstRankingType);                            

                            if (ui_rank_list_ptr_)
                            {
                                ui_rank_list_ptr_->UpdateMemberList();
                            }
                        }
                    }

                }
                break;

            case CG_MISSION_RANKING_MEMBER_INFO_NAK:
                {
                    Is_wait_receive_packet_ = false;

                    MSG_CG_MISSION_RANKING_MEMBER_INFO_NAK *pPacket = (MSG_CG_MISSION_RANKING_MEMBER_INFO_NAK *)pMsg;
                    _processErrorCode(pPacket->m_dwErrorCode);
                }
                break;
            case CG_MISSION_RANKING_SETTLEDDATE_INFO_ACK:
                {
                    MSG_CG_MISSION_RANKING_SETTLEDDATE_INFO_ACK* pPacket = (MSG_CG_MISSION_RANKING_SETTLEDDATE_INFO_ACK* )pMsg;

                    if (pPacket)
                    {
                        SetFinishDay(pPacket->settled_date);

                        ui_rank_list_ptr_->set_Is_setting_finish_day(true);

                        // ����� ��Ŷ�� ������� �� �ִ� ��ϵ� ���� �޾��ش�.
                        for ( int i = 0; 
                            i < MSG_CG_MISSION_RANKING_SETTLEDDATE_INFO_ACK::MAX_REWARDABLE_MISSION_LIST_SIZE
                            ; ++i)
                        {
                            if ( pPacket->reward_mission_list[i].ranking_id != 0 && 
                                pPacket->reward_mission_list[i].ranking_key != 0 )
                            {
                                stRewardInfo reward_info;
                                reward_info.ranking_id = pPacket->reward_mission_list[i].ranking_id;
                                reward_info.reward_ranking_key = pPacket->reward_mission_list[i].ranking_key;
                                reward_info_vector_.push_back(reward_info);

                                Is_can_reward_ = true;

                                if ( ui_rank_list_ptr_ )
                                {
                                    ui_rank_list_ptr_->set_reward_yellow_texture_show( TRUE );
                                }
                            }
                        }

                        if ( Is_can_reward_ == false )
                        {
                            if ( ui_rank_list_ptr_ )
                            {
                                ui_rank_list_ptr_->set_reward_yellow_texture_show( FALSE );
                            }                           
                        }
                    }
                }
                break;
            case CG_MISSION_RANKING_SETTLEDDATE_INFO_NAK:
                {
                    Is_wait_receive_packet_ = false;

                    MSG_CG_MISSION_RANKING_INFO_NAK *pPacket = (MSG_CG_MISSION_RANKING_INFO_NAK *)pMsg;
                    _processErrorCode(pPacket->m_dwErrorCode);                    
                }
                break;

            case CG_MISSION_RANKING_REWARD_ACK:
                {
                    Is_wait_receive_packet_ = false;                    

                    MSG_CG_MISSION_RANKING_REWARD_ACK *pPacket = (MSG_CG_MISSION_RANKING_REWARD_ACK *)pMsg;
                    ItemManager::Instance()->InsertInventoryTotalInfo(REWARD, pPacket->m_ItemInfo);                    
                    
                    LONGLONG	current_money = 0;
                    TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0, };
                    if (g_pHero) 
                    {
                        current_money = g_pHero->GetMoney();
                        g_pHero->SetMoney(pPacket->rewarded_money);
                    }

                    TCHAR chat_result[INTERFACE_STRING_LENGTH] = {0,};

                    LONGLONG acquired_money = pPacket->rewarded_money - current_money;
                    int bonus_money = 0;
                    if( pPacket->hime_bonus_ratio > 0)
                    {                            
                        bonus_money = static_cast<int>(ceil(static_cast<float>(acquired_money) * 
                            (static_cast<float>(pPacket->hime_bonus_ratio) / (100.0f + static_cast<float>(pPacket->hime_bonus_ratio) ) ) ) );
                    }
                    if( bonus_money > 0)
                    {
                        //5095 %I64u ������ ȹ���Ͽ����ϴ�.(�߰� ���� %d ȹ��)
                        g_InterfaceManager.GetInterfaceString(5095, temp_message);
                        Snprintf(chat_result, INTERFACE_STRING_LENGTH-1, temp_message, acquired_money - bonus_money, bonus_money);
                    }
                    else
                    {
                        //	%I64u ���Ӹ� ȹ���Ͽ����ϴ�.
                        g_InterfaceManager.GetInterfaceString( eST_CODE_GET_MONEY, temp_message, MAX_MESSAGE_LENGTH );
                        _stprintf(chat_result, temp_message, acquired_money);
                    }
#ifdef _DEV_VER
                    TCHAR debug_message[INTERFACE_STRING_LENGTH] ={0,};
                    Snprintf(debug_message,  INTERFACE_STRING_LENGTH -1,"DEBUG!! ȹ�� ���� = %I64u , BonusRatio = %d" , acquired_money, pPacket->hime_bonus_ratio );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, debug_message);
#endif //_DEV_VER
                    // ȹ�� ������ �ִٴ� �޽����� ����.
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,chat_result, CHAT_TYPE_SYSTEM);

                    // ���� ������ �˸��� ���� ���� ��� �˸���.
                    if( GAMEOPTION->GetPickUpItemNotify() )
                    {
                        GameFramework::GetCurrentScene()->InserCenterTextMessage( chat_result, EVENT );
                    }
                   
                    if ( reward_info_vector_.empty() )
                    {
                        break;
                    }

                    std::vector<stRewardInfo>::iterator iter = reward_info_vector_.begin();
                    
                    if ( pPacket->ranking_id == (*iter).ranking_id )
                    {   
                        std::vector<stRewardSearchInfo>::iterator it = ranking_reward_vector_.begin();
                        bool Is_search = false;
                        for( ; it != ranking_reward_vector_.end(); ++it)
                        {
                            if ((*it).ranking_id == (*iter).ranking_id)
                            {
                                Is_search = true;
                                break;
                            }
                        }

                        if ( Is_search )
                        {
                            // 2787 %s���� ��ŷ %s�� �޼� ������ ���޵Ǿ����ϴ�.
                            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                            TCHAR mission_name[INTERFACE_STRING_LENGTH] = {0,};
                            TCHAR rank_string[INTERFACE_STRING_LENGTH] = {0,};

                            ZeroMemory(mission_name, sizeof(mission_name));
                            g_InterfaceManager.GetInterfaceString( 
                                (*it).mission_string, 
                                mission_name, 
                                INTERFACE_STRING_LENGTH );

                            _itoa(pPacket->reward_ranking, rank_string, 10);
                            g_InterfaceManager.GetInterfaceStringFormat(temp_string, INTERFACE_STRING_LENGTH, 2787, mission_name, rank_string);

                            GlobalFunc::OutputMsg_SysMsg(temp_string);
                        }

                        reward_info_vector_delete_check((*iter).ranking_id);
                    }
                }
                break;

            case CG_MISSION_RANKING_REWARD_NAK:
                {
                    Is_wait_receive_packet_ = false;

                    MSG_CG_MISSION_RANKING_REWARD_NAK *pPacket = (MSG_CG_MISSION_RANKING_REWARD_NAK *)pMsg;
                    _processErrorCode(pPacket->m_dwErrorCode);
                }
                break;


            case CG_MISSION_GET_MISSION_POINT_BRD:
                {
                    MSG_CG_MISSION_GET_MISSION_POINT_BRD *pPacket = (MSG_CG_MISSION_GET_MISSION_POINT_BRD *)pMsg;
                    g_InterfaceManager.GetInterfaceString(1462 , szTemp ,INTERFACE_STRING_LENGTH);	//1462	���ֹ��� ��ȥ�� ȹ���ϼ̽��ϴ�(����Ʈ��ŷ %d�� ���)
                    Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, pPacket->m_dwPoint);

                    GlobalFunc::OutputMsg_SysMsg(szMessage);

                    if(pPacket->m_dwPoint && ((pPacket->m_dwPoint / c_iCheckPoint) >= 1))	// 1000 �� ������ �����޽��� ���
                    {
                        //1463	�̹�ȸ�� ����Ʈ��ŷ�� ��������Ʈ�� %d���� �Ѿ����ϴ�. \n��ŷ�� ������ �ִ� ����Ʈ��ŷ �����ڷκ��� Ȯ���� �����մϴ�.
                        g_InterfaceManager.GetInterfaceString(1463 , szTemp ,INTERFACE_STRING_LENGTH);
                        Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, pPacket->m_dwPoint);
                        GlobalFunc::OutputMsg_SysMsg(szMessage);
                    }
                }
                break;


            case CG_MISSION_POINT_CANNOT_REGISTER_BRD:
                {
                    //5471 ����Ʈ��ŷ:����Ʈ��ŷ ���������� ���Ե��� ���� ĳ���� �����մϴ�.
                    g_InterfaceManager.GetInterfaceString(5471 , szMessage ,INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
                }
                break;

            }            
        }
        break;
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::_processErrorCode(DWORD dwErrorCode, eOUTPUT_MSG_TYPE type/* = eOUTPUT_MSG_CHAT*/)
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
    int iMessage = 0;

    switch(dwErrorCode)
    {
    case RC_RANKING_FAILED:						// �� �� ���� ����
        // 5481	�̼Ƿ�ŷ: �� �� ���� ���������Դϴ�.
        iMessage = eST_MISSION_RANK_UNKNOWN_ERROR;
        break;

    case RC_RANKING_INVALID_STATE:				// �߸��� ����
        // 5482	�̼Ƿ�ŷ: �߸��� �����Դϴ�. 
        iMessage = eST_MISSION_RANK_INVALID_STATUS;
        break;

    case RC_RANKING_ALREADY_REWARDED:			// �̹� ���� ���� ����
        // 5483	�̼Ƿ�ŷ: �̹� ������ �޾ҽ��ϴ�.
        iMessage = eST_MISSION_RANK_ALREADY_REWARDED;
        break;

    case RC_RANKING_DOING_TRANSACTION:			// Ʈ����� ������, �̹� �ٸ� �۾� Ȥ�� �� �۾��� �������� ( ��Ŷ 2���� ���ɼ�! )
        // 5484	�̼Ƿ�ŷ: ó�����Դϴ�.
        iMessage = eST_MISSION_RANK_DOING_TRANSACTION;
        break;

    case RC_RANKING_NETWORK_ERROR_FOR_PORTAL:	// ��Ż ������ ������ �ִ�. ��Ʈ��ũ ��ַ� �Ͻ� ��� ����
        // 5485	�̼Ƿ�ŷ: ��Ʈ��ũ ��ַ� �Ͻ� �������
        iMessage = eST_MISSION_RANK_NETWORK_PROBLEM;
        break;

    case RC_RANKING_NETWORK_ERROR_FOR_DBPROXY:	// DBP ������ ������ �ִ�. ��Ʈ��ũ ��ַ� �Ͻ� ��� ����
        // 5485	�̼Ƿ�ŷ: ��Ʈ��ũ ��ַ� �Ͻ� �������
        iMessage = eST_MISSION_RANK_NETWORK_PROBLEM;
        break;

    case RC_RANKING_NOT_SPACE_FOR_REWARD:		// ������ ���� ������ ������ �����ϴ�.
        // 5486	�̼Ƿ�ŷ: ������ ���� �κ��丮 ������ �����մϴ�.
        iMessage = eST_MISSION_RANK_NEDD_INVENTORY_SPACE;
        break;

        // ���� ���� ���� ����
    case RC_RANKING_INSUFFICEINT_PARTY_MEMBER:	// ��ŷ�� ����ϱ� ���� ��Ƽ���(����5��)�� �����ϴ�.
        // 5487	�̼Ƿ�ŷ: ��ŷ�� ����ϱ� ���� ��Ƽ���� �����մϴ�.
        iMessage = eST_MISSION_RANK_REQUIRE_PARTY_MEMBER;
        break;

    case RC_RANKING_A_CLASS_OBTAIN_MAJORITY:	// �� Ŭ������ ���ݼ��� ���� �ϸ� �ȵȴ�.
        // 5488	�̼Ƿ�ŷ: �� Ŭ������ ���ݼ� �̻��� �����ؼ��� �ȵ˴ϴ�.
        iMessage = eST_MISSION_RANK_ONE_CLASS_OVER_HALF;
        break;

    case RC_RANKING_DONT_INCLUDE_MISSION_LEVEL_AREA:	// �̼Ƿ��������� ���Ե��� �ʴ� �÷��̾ �����Ѵ�.
        // 5489	�̼Ƿ�ŷ: �̼Ƿ��������� ���Ե��� �ʴ� ĳ���Ͱ� �����մϴ�.
        iMessage = eST_MISSION_RANK_INVALID_LEVEL_AREA;
        break;

    case RC_RANKING_INVALID_PARAM:					// �߸��� �����Դϴ�.
        // 5490	�̼Ƿ�ŷ: �߸��� �����Դϴ�.
        iMessage = eST_MISSION_RANK_INVALID_PARAMETER;
        break;

    case RC_RANKING_DONT_INCLUDE_RANKING:			// ��ŷ�� ���Ե��� �ʾҽ��ϴ�.
        // 5491	�̼Ƿ�ŷ: ��ŷ�� ���յ��� �ʾҽ��ϴ�.
        iMessage = eST_MISSION_RANK_NOT_INClUDE;
        break;

    case RC_RANKING_INVALID_REWARD_INDEX:			// ���� �ε����� ��ȿ���� �ʽ��ϴ�.
        // �̼Ƿ�ŷ: ���� �ε����� ��ȿ���� �ʽ��ϴ� 5513
        iMessage =  5513;			// ��Ʈ�� ��û
        break;

    case RC_RANKING_SETTLING_INFO:					// ��ŷ ������, ��ŷ���� ���� ������
        // �̼Ƿ�ŷ: ��ŷ ������, ��ŷ���� ���� �������Դϴ�. 5514
        iMessage =  5514;			// ��Ʈ�� ��û
        break;

    case RC_RANKING_UNSATISFY_KILL_MONSTER_NUM:
        //5510 �̼Ƿ�ŷ:�ּ� ���� ų ���� �����մϴ�.
        iMessage = 5510;
        break;

        // �̼Ƿ�ŷ�� ������� ������ ã�� ���Ͽ����ϴ�.
    case RC_RANKING_NOT_FIND_REWARD_USER:			
        iMessage = 5511;
        break;

        // �̼Ƿ�ŷ������ ã�� ���Ͽ����ϴ�.
    case RC_RANKING_NOT_FIND_RANKING_INFO:
        iMessage = 5512;
        break;


    case RC_RANKING_REWARD_FAIL_FATIGUE:
        iMessage = 6045;
        break;


    case RC_RANKING_NOT_REGIST_INVALID_PERIOD:
#if defined (_JAPAN)
        // �Ϻ� ����ó��		
        return;
#else
        iMessage = 5512;
#endif		
        break;
    default:
        iMessage = 5481;					// �̼Ƿ�ŷ: �� �� ���� ���������Դϴ�.
        break;
    }

    g_InterfaceManager.GetInterfaceString( iMessage , szMessage ,INTERFACE_STRING_LENGTH);
    Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szMessage );

    GlobalFunc::OutputMsg_Notice(type, szMessage);
}

//------------------------------------------------------------------------------
/**
*/
bool uiMissionRankMan2::IsMissionRanking(RANKINGNO no, int iRankSystemType)
{
    RankingManager::LEVEL_AREA* pArea = GET_MISSION_RANK_MANAGER()->FindLVArea(no);

    if(pArea)
    {
        if((int)pArea->m_RankingType == iRankSystemType)
            return true;

    }

    return false;

}
//------------------------------------------------------------------------------
/**
*/
RANKINGID uiMissionRankMan2::GetSearchRankingId( RANKINGNO no , eRANKING_SYSTEM_TYPE type )
{
#if defined(_NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR)
    RankingManager::LEVEL_AREA* level_area_ptr = GET_MISSION_RANK_MANAGER()->FindLVArea(no, type);

    if ( level_area_ptr )
    {        
        return static_cast<RANKINGID>(level_area_ptr->m_Id);        
    }
#else
    RankingManager::LEVEL_AREA* level_area_ptr = GET_MISSION_RANK_MANAGER()->FindLVArea(no);

    if ( level_area_ptr )
    {
        if(static_cast<int>(level_area_ptr->m_MissionNo) == no
             && level_area_ptr->m_RankingType == type)
        {
            return static_cast<RANKINGID>(level_area_ptr->m_Id);
        }
    }
#endif // _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR

    return 0;
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::fame_data_and_weeks_and_member_data_clar()
{
    ZeroMemory(&my_ranking_view_data_, sizeof(my_ranking_view_data_));
    ZeroMemory(&pre_week_rank_view_data_, sizeof(pre_week_rank_view_data_));
    ZeroMemory(&current_week_rank_view_data_, sizeof(current_week_rank_view_data_));
    ZeroMemory(&member_view_data_, sizeof(member_view_data_));
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::reward_check()
{
    if ( !reward_info_vector_.empty() )
    {
        Is_can_reward_ = true;        
    }
    else
    {
        Is_can_reward_ = false;
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionRankMan2::reward_info_vector_delete_check(RANKINGID id)
{
    std::vector<stRewardInfo>::iterator iter = reward_info_vector_.begin();

    for ( ; iter != reward_info_vector_.end(); )
    {
        if ((*iter).ranking_id == id)
        {
            iter = reward_info_vector_.erase(iter);

            if (reward_info_vector_.empty())
            {
                Is_can_reward_ = false;

                if ( ui_rank_list_ptr_ )
                {
                    ui_rank_list_ptr_->set_reward_yellow_texture_show( FALSE );
                }
            }

            break;
        }
        else
        {
            ++iter;
        }
    }
}

MISSIONNO uiMissionRankMan2::get_reward_id_to_mission_number( RANKINGID id )
{
    std::vector<stRewardSearchInfo>::iterator iter = ranking_reward_vector_.begin();

    for ( ; iter != ranking_reward_vector_.end(); )
    {
        if ( (*iter).ranking_id == id )
        {
            return static_cast<MISSIONNO>((*iter).mission_number);
        }
        else
        {
            ++iter;
        }
    }

    return 0;
}

bool uiMissionRankMan2::get_confirmed_member_party_key(const stMemberConfirmedInfo& confirmed_info)
{    
    std::list<stMemberConfirmedInfo>::iterator iter = confirmed_member_party_confirmed_list_.begin();

    for ( ; iter != confirmed_member_party_confirmed_list_.end(); ++iter)
    {
        if ( (*iter).ranking_key == confirmed_info.ranking_key 
            && (*iter).pending_ranking_type == confirmed_info.pending_ranking_type
            && (*iter).ranking_no == confirmed_info.ranking_no)
        {
            return true;
        }
    }
    return false;
}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
