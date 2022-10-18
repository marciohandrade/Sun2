#include "SunClientPrecompiledHeader.h"

#include "uiBattleScoreMan.h"
#include "InterfaceManager.h"
#include "uiBattleScoreSingle\uiBattleScoreSingle.h"
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
#include "uiBattleScoreDouble\uiBattleScoreDouble.h"
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "uiBattleScoreBlood\uiBattleScoreBlood.h"
#include "uiBattleScoreBattleGround\uiBattleScoreBattleGround.h"
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "uiBattleScoreGoldRush\uiBattleScoreGoldRush.h"
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH


#include "uiPartyMan\uiPartyMan.h"
#include "Hero.h"
#include "GlobalFunc.h"

const DWORD IM_BATTLESCORE_MANAGER::DLG_BT_SINGLE		= StrToWzID("0673");
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
const DWORD IM_BATTLESCORE_MANAGER::DLG_BT_DOUBLE		= StrToWzID("0674");
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
const DWORD IM_BATTLESCORE_MANAGER::DLG_BT_BLOOD		= StrToWzID("0675");
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
const DWORD IM_BATTLESCORE_MANAGER::DLG_BT_GROUND		= StrToWzID("0645");
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
const DWORD IM_BATTLESCORE_MANAGER::DLG_BT_GOLDRUSH		= StrToWzID("0981");
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

uiBattleScoreMan::uiBattleScoreMan(InterfaceManager * pUIMAN)
:uiBaseMan(pUIMAN)
{
}


//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::InitInfo()
{
	switch(GetMode())
	{
	case eBTS_MISSION:
	case eBTS_SSQ:
	{
		if(m_pBattleScoreSingle)
		{ 
			m_pBattleScoreSingle->InitInfo();
			SetIsInit(true);
		} 
	}
	break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case eBTS_BATTLE_GROUND:
        {            
            if(battle_score_battle_ground)
            {
                battle_score_battle_ground->InitInfo();
                SetIsInit(true);                
            }                        
        }
        break;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    case eBTS_GOLD_RUSH:
        {
            if (battle_score_gold_rush_ptr_)
            {
                battle_score_gold_rush_ptr_->InitInfo();
                SetIsInit(true);
            }
        }
        break;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
	case eBTS_HUNTING:
	{
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
		if(m_pBattleScoreDouble)
		{ 
			if(!m_pBattleScoreDouble->IsVisible())
			{			
				m_pBattleScoreDouble->InitInfo();
				SetIsInit(true);
			}
		}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
	}
	case eBTS_PVP_PRIVATE:
	case eBTS_PVP_TEAM:
    case eBTS_DOMINATION:
	{
		SetIsInit(true);
	}
	default://eBTS_NONE_MODE
		{
			// 헌팅존, 미션존 등 밖에서는 열지 않는다.
		}
		break;

	}// switch
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::RefreshInfo(EXPTYPE add_experience, MONEY add_money)
{
	switch(GetMode())
	{
	case eBTS_MISSION:
	case eBTS_SSQ:
	{
		if(m_pBattleScoreSingle)
		{ 
			m_pBattleScoreSingle->RefreshInfo(add_experience, add_money);
		} 
	}
	break;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
	case eBTS_HUNTING:
	{
		if(m_pBattleScoreDouble)
		{ 
			m_pBattleScoreDouble->RefreshInfo();
		}
	}
	break;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case eBTS_BATTLE_GROUND:
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
	case eBTS_PVP_PRIVATE:
	case eBTS_PVP_TEAM:
    case eBTS_DOMINATION:
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    case eBTS_GOLD_RUSH:
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
	default://eBTS_NONE_MODE
		{
			// 헌팅존, 미션존 등 밖에서는 열지 않는다.
		}
		break;

	}// switch
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::Reset()
{
	SetIsInit(false);
    set_first_open(true);
	ClearMemberData();
    RefreshListCtrlData();

	// 승수 0으로 초기화
	for(int i = 0; i < eTeamCNT; ++i)
	{
		SetWinPoint(0, (E_TEAM) i);
	}

	ClearSelection();

	SetHeroTeam(0);	// 개인전으로 초기화
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::ToggleBattleScoreWindow()
{
    if (IsBattleScoreWinOpen())
    {
        ShowBattleScoreWin();
    }
    else
    {
        SendPacketBattleResultSYN();
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::SendPacketBattleResultSYN()
{
    MSG_CG_ZONE_PLAYER_BATTLE_RESULT_SYN packet;
    if (GetMode() == eBTS_DOMINATION)
    {
        if (m_pBattleScoreBlood->home_team_selected() == true)
        {
            packet.m_OwnerGuild = 0;
        }
        else
        {
            packet.m_OwnerGuild = 1;
        }
    }
    else
    {
        packet.m_OwnerGuild = 0;
    }
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof (packet));
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::PartyRefresh()
{
    bool opened = false;
    switch (GetMode())
	{
	case eBTS_MISSION:
	case eBTS_SSQ:
	case eBTS_PVP_TEAM:
    case eBTS_HUNTING:
	{
		if (m_pBattleScoreSingle && m_pBattleScoreSingle->IsVisible())
		{
			opened = true;
        }
	}
	break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case eBTS_BATTLE_GROUND:
        {
            if (battle_score_battle_ground && battle_score_battle_ground->IsVisible())
            {
                opened = true;
            }
        }
        break;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    case eBTS_GOLD_RUSH:
        {
            if (battle_score_gold_rush_ptr_ && battle_score_gold_rush_ptr_->IsVisible())
            {
                opened = true;
            }
        }
        break;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    default:
    {
        opened = false;
    }
    break;
	}// switch

    if(opened == true)
    {
        set_toggle_ui_open(true);
        SendPacketBattleResultSYN();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool uiBattleScoreMan::IsBattleScoreWinOpen()//(bool bIsOpen)
{
    switch (GetMode())
	{
	case eBTS_MISSION:
	case eBTS_SSQ:
	case eBTS_PVP_PRIVATE:
	{
		if (m_pBattleScoreSingle)
		{ 
			if (m_pBattleScoreSingle->IsVisible())
			{
			    return true;
			}
			else
			{
				return false;
			}
		} 
	}
	break;
	case eBTS_HUNTING:
	case eBTS_PVP_TEAM:
	{
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
		if (m_pBattleScoreDouble)
		{ 
			if (m_pBattleScoreDouble->IsVisible())
			{			
			    return true;	
			}
			else
			{
			    return false;
			}
		}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
	}
	break;
    case  eBTS_DOMINATION:
    {
        if (m_pBattleScoreBlood)
        {
			if (m_pBattleScoreBlood->IsVisible())
			{			
			    return true;
			}
			else
			{
				return false;
			}
        }
    }
    break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case eBTS_BATTLE_GROUND:
        {
            if (battle_score_battle_ground)
            {
                if (battle_score_battle_ground->IsVisible())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        break;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    case eBTS_GOLD_RUSH:
        {
            if (battle_score_gold_rush_ptr_)
            {
                if (battle_score_gold_rush_ptr_->IsVisible())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        break;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH 
	default://eBTS_NONE_MODE
		{
			return false;
		}
		break;
	}// switch 
    return false;
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::ShowBattleScoreWin()
{ 
    bool bIsOpen = toggle_ui_open();
	switch(GetMode())
	{
	case eBTS_MISSION:
	case eBTS_SSQ:
	case eBTS_PVP_PRIVATE:
	{
		if(m_pBattleScoreSingle)
		{ 
			if(!m_pBattleScoreSingle->IsVisible())
			{
				m_pBattleScoreSingle->ShowInterface(TRUE);
			}
			else
			{
				if(bIsOpen == false)	// 이 값이 true이면 닫지 않는다.
				{
					m_pBattleScoreSingle->ShowInterface(FALSE);
				}
			}
		} 
	}
	break;

	case eBTS_HUNTING:
	case eBTS_PVP_TEAM:
	{
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
		if(m_pBattleScoreDouble)
		{ 
			if(!m_pBattleScoreDouble->IsVisible())
			{			
				m_pBattleScoreDouble->ShowInterface(TRUE);
			}
			else
			{
				if(bIsOpen == false)	// 이 값이 true이면 닫지 않는다.
				{
					m_pBattleScoreDouble->ShowInterface(FALSE);
				}
			}
		}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
	}
	break;
    case  eBTS_DOMINATION:
    {
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
        if (m_pBattleScoreBlood)
        {
			if (m_pBattleScoreBlood->IsVisible() == false)
			{
				m_pBattleScoreBlood->ShowInterface(TRUE);
			}
			else
			{
				if (bIsOpen == false)	// 이 값이 true이면 닫지 않는다.
				{
					m_pBattleScoreBlood->ShowInterface(FALSE);
				}
			}
        }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    }
    break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case eBTS_BATTLE_GROUND:
        {            
            if (battle_score_battle_ground)
            {
                if (battle_score_battle_ground->IsVisible() == false)
                {
                    battle_score_battle_ground->ShowInterface(TRUE);
                }
                else
                {
                    if (bIsOpen == false)	// 이 값이 true이면 닫지 않는다.
                    {
                        battle_score_battle_ground->ShowInterface(FALSE);
                    }
                }
            }
        }
        break;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    case eBTS_GOLD_RUSH:
        {
            if (battle_score_gold_rush_ptr_)
            {
                if (battle_score_gold_rush_ptr_->IsVisible() == false)
                {
                    battle_score_gold_rush_ptr_->ShowInterface(TRUE);
                }
                else
                {
                    if (bIsOpen == false) // 이 값이 true이면 닫지 않는다.
                    {
                        battle_score_gold_rush_ptr_->ShowInterface(FALSE);
                    }
                }
            }
        }
        break;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
	default://eBTS_NONE_MODE
		{
			// 헌팅존, 미션존 등 밖에서는 열지 않는다.
		}
		break;

	}// switch    
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::ClearMemberData()
{
	for(int i = eHomeTeam; i < eTeamCNT; ++i)
	{
		m_BTSMemberArray[i].Clear();
	}
	SetMode(eBTS_NONE_MODE);
}
//------------------------------------------------------------------------------
/**
*/// 헌팅 이긴 후 결과 초기화, Away팀 초기화, 데미지등 0으로 초기화
void uiBattleScoreMan::ResetHuntingData()
{
	m_BTSMemberArray[eAwayTeam].Clear();	
	int cnt = m_BTSMemberArray[eHomeTeam].Size();
	for(int i = 0; i<cnt; ++i)
	{
		m_BTSMemberArray[eHomeTeam][i].dwTotalDamageToEnemy = 0;   // 적에게 입힌 데미지 총량
		m_BTSMemberArray[eHomeTeam][i].dwTotalDamageByEnemy = 0;   // 적에게 당한 데미지 총량
		m_BTSMemberArray[eHomeTeam][i].dwTotalHealToParty = 0;     // 자신및 파티에게 사용한 총 힐량
		m_BTSMemberArray[eHomeTeam][i].dwMaxDamageToEnemy = 0;      // 적에게 입힌 최대 데미지
		m_BTSMemberArray[eHomeTeam][i].wKillCount = 0;             // 죽인 횟수
		m_BTSMemberArray[eHomeTeam][i].wDeadCount = 0;             // 죽은 횟수							
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
        m_BTSMemberArray[eHomeTeam][i].wAssistCount = 0;           // 어시스트 횟수
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
	}
    if (cnt > 0)
    {
        RefreshListCtrlData();
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::OnInitialize()
{
    SetIsInit(false);
    SetMode(eBTS_NONE_MODE);
    ClearSelection();

    for(int i = 0; i < eTeamCNT; ++i)
    {
        SetWinPoint(0, (E_TEAM) i);
    }
    m_iHeroTeam = 0;
    toggle_ui_open_ = true;


	m_pBattleScoreSingle = CreateUIDialog<uiBattleScoreSingle>(IM_BATTLESCORE_MANAGER::DLG_BT_SINGLE, 
		"Data\\Interface\\67_3_result_mission_pvp.iwz",this, TRUE);
	SUN_ASSERT(m_pBattleScoreSingle);
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
	m_pBattleScoreDouble = CreateUIDialog<uiBattleScoreDouble>(IM_BATTLESCORE_MANAGER::DLG_BT_DOUBLE, 
		"Data\\Interface\\67_4_result_time_survival_death_pvp.iwz",this, TRUE);
	SUN_ASSERT(m_pBattleScoreDouble);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    m_pBattleScoreBlood = CreateUIDialog<uiBattleScoreBlood>(IM_BATTLESCORE_MANAGER::DLG_BT_BLOOD, 
		"Data\\Interface\\67_5_result_blood.iwz",this, TRUE);
	SUN_ASSERT(m_pBattleScoreBlood);
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    battle_score_battle_ground = CreateUIDialog<uiBattleScoreBattleGround>(IM_BATTLESCORE_MANAGER::DLG_BT_GROUND, 
        "Data\\Interface\\64_5_chaos_result.iwz",this, TRUE);
    SUN_ASSERT(battle_score_battle_ground);
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    battle_score_gold_rush_ptr_ = CreateUIDialog<uiBattleScoreGoldRush>(IM_BATTLESCORE_MANAGER::DLG_BT_GOLDRUSH,
        "Data\\Interface\\98_1_Gold_Rush.iwz", this, TRUE);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::ClearSelection()
{
	m_iCurTeam01Select		= -1;
	m_iCurTeam02Select		= -1;
}
//------------------------------------------------------------------------------
/**
*/
int uiBattleScoreMan::FindMember(DWORD character_guid, E_TEAM eTeam)
{
	for(int i=0;i<m_BTSMemberArray[eTeam].Size();++i)
	{
		if (m_BTSMemberArray[eTeam][i].dwCharGuid == character_guid)
		{
			return i;
		}
	}
	return -1;
}
//------------------------------------------------------------------------------
/**
*/
int uiBattleScoreMan::FindMemberToName(LPCTSTR name, E_TEAM eTeam)
{
    if (name)
    {
  	    for (int i=0;i<m_BTSMemberArray[eTeam].Size();++i)
	    {
		    if (m_BTSMemberArray[eTeam][i].name == name)
		    {
			    return i;
		    }
	    }
    }
	return -1;
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::RemoveMember(int index, E_TEAM eTeam)
{
	unsigned int size = m_BTSMemberArray[eTeam].Size();
	if(index >= 0 && size > 0)
	{
		m_BTSMemberArray[eTeam].Erase(index);
	}
	//제거후 정렬해줘야함
    SortbyList();
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::ConnectMember(int index, E_TEAM eTeam)
{
	unsigned int size = m_BTSMemberArray[eTeam].Size();
	if (index >= 0 && size > 0)
	{
		m_BTSMemberArray[eTeam][index].bConnected = true;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::DisconnectMember(int index, E_TEAM eTeam)
{
	unsigned int size = m_BTSMemberArray[eTeam].Size();
	if(index >= 0 && size > 0)
	{
		m_BTSMemberArray[eTeam][index].bConnected = false;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::RefreshListCtrlData()
{
	switch(GetMode())
	{
	case eBTS_MISSION:
	case eBTS_SSQ:
	case eBTS_PVP_PRIVATE:
	{
		if(m_pBattleScoreSingle)
		{
			m_pBattleScoreSingle->_RefreshListCtrlData(m_BTSMemberArray[eHomeTeam]);
		}
	}
	break;
	case eBTS_HUNTING:
	case eBTS_PVP_TEAM:
	{
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#else
		if(m_pBattleScoreDouble)
		{
			for(int i = eHomeTeam; i < eTeamCNT; ++i)
			{
				m_pBattleScoreDouble->_RefreshListCtrlData(m_BTSMemberArray[i], (E_TEAM)i);
			}
		}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
	}
	break;
    case eBTS_DOMINATION:
    {
        if (m_pBattleScoreBlood)
		{
            if (m_pBattleScoreBlood->home_team_selected() == true)
            {
			    m_pBattleScoreBlood->refresh_list_control_data(m_BTSMemberArray[eHomeTeam]);
            }
            else
            {
                m_pBattleScoreBlood->refresh_list_control_data(m_BTSMemberArray[eAwayTeam]);
            }
		}
    }
    break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case eBTS_BATTLE_GROUND:
        {
            RefreshBattleGroundList();
        }
        break;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    case eBTS_GOLD_RUSH:
        {            
            RefreshGoldRushList();
        }
        break;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

	}
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::NetworkProc( MSG_BASE * pMsg )
{
	if (pMsg->m_byCategory == CG_ZONE)
	{
		switch (pMsg->m_byProtocol)
		{
		case CG_ZONE_INSERT_BATTLE_PLAYER_CMD:
		{	
            int player_count = 0;
            int list_index = 0;
            WORD team_index = (WORD)eHomeTeam;
            int find_index = -1;

            MSG_CG_ZONE_INSERT_BATTLE_PLAYER_CMD* battle_player_packet =
                (MSG_CG_ZONE_INSERT_BATTLE_PLAYER_CMD*)pMsg;
            BATTLESCORE_DATA battle_player_data;
            memset(&battle_player_data, 0, sizeof(BATTLESCORE_DATA));

            if (battle_player_packet != NULL)
            {
                player_count = battle_player_packet->m_ZonePlayers.m_PlayerCount;
            }
            if (player_count <= 0 || player_count > MAX_PLAYER)
            {
                player_count = 0;
            }

            for (list_index = 0; list_index < player_count; ++list_index)
            {
                battle_player_data.dwCharGuid = 
                    battle_player_packet->m_ZonePlayers.m_Info[list_index].CharGuid;
                battle_player_data.byClassCode =
                    battle_player_packet->m_ZonePlayers.m_Info[list_index].byClass;
                battle_player_data.wLevel =
                    battle_player_packet->m_ZonePlayers.m_Info[list_index].byLevel;
                battle_player_data.name =
                    battle_player_packet->m_ZonePlayers.m_Info[list_index].szName;
                team_index = battle_player_packet->m_ZonePlayers.m_Info[list_index].wTeam;
                battle_player_data.gender = 
                    battle_player_packet->m_ZonePlayers.m_Info[list_index].gender;
                battle_player_data.bConnected = true;
                battle_player_data.dwTotalDamageByEnemy = 0;
                battle_player_data.dwTotalDamageToEnemy = 0;
                battle_player_data.dwTotalHealToParty = 0;
                battle_player_data.dwMaxDamageToEnemy = 0;
                battle_player_data.wKillCount = 0;
                battle_player_data.wDeadCount = 0;
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
                battle_player_data.wAssistCount = 0;
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                battle_player_data.gold_point_ = 0;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

                // 중복되면 이전것을 지운다(나갔다 들어온 경우)
                for (int team_temp_index = eHomeTeam; team_temp_index < eTeamCNT; ++team_temp_index)
                {
                    find_index = FindMember(battle_player_data.dwCharGuid, (E_TEAM)team_temp_index);
                    if (find_index >= 0)
                    {
                        RemoveMember(find_index, (E_TEAM)team_temp_index);
                        RefreshListCtrlData();
                        break;
                    }
                }
                if (team_index < eTeamCNT)
                {
                    m_BTSMemberArray[team_index].PushBack(battle_player_data);
                }
            }

            // 최초 패킷에서만 나를 맨위로 정렬
            if (first_open())
            {
                set_first_open(false);
                // 나를 찾는다
                const TCHAR* hero_name = g_pHero->GetName();
                for (int team_temp_index = eHomeTeam; team_temp_index < eTeamCNT; ++team_temp_index)
                {
                    find_index = FindMemberToName(hero_name, (E_TEAM)team_temp_index);
                    if (find_index >= 0)
                    {   // swap
                        BATTLESCORE_DATA temp_player = m_BTSMemberArray[team_temp_index][0];
                        m_BTSMemberArray[team_temp_index][0] =
                            m_BTSMemberArray[team_temp_index][find_index];
                        m_BTSMemberArray[team_temp_index][find_index] = temp_player;
                        break;
                    }
                }
            }
            // 팀구분을 위해 이 패킷을 받기전에 GetParty()가 먼저 호출된 상태여야 한다.
		}
		break;
		case CG_ZONE_REMOVE_BATTLE_PLAYER_CMD:
		{
            MSG_CG_ZONE_REMOVE_BATTLE_PLAYER_CMD* battle_player_packet =
                (MSG_CG_ZONE_REMOVE_BATTLE_PLAYER_CMD*)pMsg;
            int find_index = -1;
            CHARGUID player_guid = 0;
            BYTE disconnect_type = 0;
            int player_count = battle_player_packet->m_ZonePlayers.m_PlayerCount;
            for (int player_index = 0; player_index < player_count; ++player_index)
            {
                player_guid = battle_player_packet->m_ZonePlayers.m_Info[player_index].CharGuid;
                disconnect_type = 
                    battle_player_packet->m_ZonePlayers.m_Info[player_index].AbNormalDisconnect;
                for (int team_index = eHomeTeam; team_index < eTeamCNT; ++team_index)
                {
                    find_index = FindMember(player_guid, (E_TEAM)team_index);
                    if (find_index >= 0)
                    {
                        // 점령전인 경우는 삭제, 그외는 접속종료
                        if (GetMode() == eBTS_DOMINATION)
                        {
                            RemoveMember(find_index, (E_TEAM)team_index);
                        }
                        else
                        {
                            DisconnectMember(find_index, (E_TEAM)team_index);
                        }
                        RefreshListCtrlData();
                        break;
                    }//if
                }//for
            }//for
		}//case
		break;
        case CG_ZONE_PLAYER_BATTLE_RESULT_ACK:
        {
            MSG_CG_ZONE_PLAYER_BATTLE_RESULT_ACK* battle_player_packet = 
                (MSG_CG_ZONE_PLAYER_BATTLE_RESULT_ACK*)pMsg;

            int find_index = -1;
            CHARGUID player_guid = 0;
            int player_count = battle_player_packet->m_ZonePlayers.m_PlayerCount;
            for (int player_index = 0; player_index < player_count; ++player_index)
            {
                player_guid = battle_player_packet->m_ZonePlayers.m_Info[player_index].CharGuid;
                for (int team_index = eHomeTeam; team_index < eTeamCNT; ++team_index)
                {
                    find_index = FindMember(player_guid, (E_TEAM)team_index);
                    if (find_index >= 0)
                    {
                        m_BTSMemberArray[team_index][find_index].dwTotalDamageByEnemy =
                            battle_player_packet->m_ZonePlayers.m_Info[player_index].dwTotalDamageByEnemy;
                        m_BTSMemberArray[team_index][find_index].dwTotalDamageToEnemy =
                            battle_player_packet->m_ZonePlayers.m_Info[player_index].dwTotalDamageToEnemy;
                        m_BTSMemberArray[team_index][find_index].dwTotalHealToParty =
                            battle_player_packet->m_ZonePlayers.m_Info[player_index].dwTotalHealToParty;
                        m_BTSMemberArray[team_index][find_index].wDeadCount =
                            battle_player_packet->m_ZonePlayers.m_Info[player_index].wDeadCount;
                        m_BTSMemberArray[team_index][find_index].wKillCount =
                            battle_player_packet->m_ZonePlayers.m_Info[player_index].wKillCount;
                        m_BTSMemberArray[team_index][find_index].wLevel =
                            battle_player_packet->m_ZonePlayers.m_Info[player_index].wLevel;
                        m_BTSMemberArray[team_index][find_index].dwMaxDamageToEnemy =
                            battle_player_packet->m_ZonePlayers.m_Info[player_index].dwMaxDamageToEnemy;
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
                        m_BTSMemberArray[team_index][find_index].wAssistCount = 
                            battle_player_packet->m_ZonePlayers.m_Info[player_index].assist_count;
#endif // _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                        if (GetMode() == eBTS_BATTLE_GROUND)
                        {
                            m_BTSMemberArray[team_index][find_index].strongpoint_attack =
                                battle_player_packet->m_ZonePlayers.m_Info[player_index].strongpoint_attack;
                            m_BTSMemberArray[team_index][find_index].strongpoint_defense =
                                battle_player_packet->m_ZonePlayers.m_Info[player_index].strongpoint_defense;
                            m_BTSMemberArray[team_index][find_index].series_kill =
                                battle_player_packet->m_ZonePlayers.m_Info[player_index].series_kill;
                        }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                        if (GetMode() == eBTS_GOLD_RUSH)
                        {
                            m_BTSMemberArray[team_index][find_index].gold_point_ =
                                battle_player_packet->m_ZonePlayers.m_Info[player_index].gold_point;
                        }                        
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                        break;
                    }
                }
            }
            RefreshListCtrlData();
            uiBattleScoreMan* battle_score_manager_ptr = 
                static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
			battle_score_manager_ptr->ShowBattleScoreWin();            
        }
        break;
        case CG_ZONE_CHUNTING_BATTLE_INFO_CMD:
        {
            MSG_CG_ZONE_CHUNTING_BATTLE_INFO_CMD* battle_player_packet = 
                (MSG_CG_ZONE_CHUNTING_BATTLE_INFO_CMD*)pMsg;

            SetWinPoint(battle_player_packet->m_HomeTeamWinCount, eHomeTeam);
            SetWinPoint(battle_player_packet->m_AwayTeamWinCount, eAwayTeam);
            RefreshInfo(0, 0);
        }
        break;
        case CG_ZONE_PLAYER_BATTLE_RESULT_NAK:
        {
#ifdef _DEV_VER
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, 
                "CG_ZONE_PLAYER_BATTLE_RESULT_NAK: Can not open Battle Score Window"); 
#endif
        }
        break;
		}//switch
	}
}
//------------------------------------------------------------------------------
/**
*/
int uiBattleScoreMan::GetPartyMember()
{
	BATTLESCORE_DATA stData;
	memset(&stData, 0, sizeof(BATTLESCORE_DATA));

	if (!g_pHero)
	{
		return 0;
	}

	int team = eHomeTeam;
	//if(GetMode() == eBTS_PVP_TEAM && g_pHero->GetTeam() == 2)	// 2홍팀, 1청팀
	if (GetHeroTeam() == 2)
	{
		team = eAwayTeam;
	}

	if (g_pHero->IsParty())
	{
		uiPartyMan* partyMan = static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
		_PARTY_MEMBER_INFO rMember;
		if (partyMan)
		{
			int partyNum = partyMan->GetPartyNum();
			for (int i = 0; i < partyNum; ++i)
			{
				rMember = partyMan->GetPartyMemberInfoFromIndex(i)->Get();
				stData.dwCharGuid = rMember.m_dwMemberKey;
				stData.name = rMember.m_szCharName;
				stData.wLevel = rMember.m_wLevel;
				stData.byClassCode = rMember.m_byClassType;
                stData.bConnected = true;
				if ((stData.dwCharGuid != 0) && (FindMember(stData.dwCharGuid, (E_TEAM)team) < 0))
				{
					m_BTSMemberArray[team].PushBack(stData);
				}
			}
		}
	}
	else
	{	// 자신정보(1인 파티, 혼자인 경우)
		stData.dwCharGuid = g_pHero->GetObjectKey();
		stData.name = g_pHero->GetName();
		stData.wLevel = g_pHero->GetLevel();
		stData.byClassCode = g_pHero->GetClass();
        stData.bConnected = true;
		if ((stData.dwCharGuid != 0) && (FindMember(stData.dwCharGuid, (E_TEAM)team) < 0))
		{
			m_BTSMemberArray[team].PushBack(stData);
		}
	}
	return m_BTSMemberArray[team].Size();
}
//------------------------------------------------------------------------------
/** 기본 소트
*/
bool _BaseSort_Name(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
	if(bOrderBy)
	{
		return (a.name < b.name);
	}
	else
	{
		return (a.name >= b.name);
	}
}
//------------------------------------------------------------------------------
/**
*/
bool _BaseSort_Lv(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
	if(a.wLevel == b.wLevel)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return  (a.wLevel < b.wLevel);
	}
	else
	{
		return  (a.wLevel >= b.wLevel);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool _BaseSort_Class(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
	int j;
	TCHAR szTemp[2][INTERFACE_STRING_LENGTH] = {0,};

	for(int i = 0; i < 2; ++i)
	{
		if(i == 0)
			j = a.byClassCode;
		else if(i == 1)
			j = b.byClassCode;
		else
			j = -1;

        Snprintf(szTemp[i], 
            INTERFACE_STRING_LENGTH, 
            _T("%s"), 
            GlobalFunc::GetClassString(static_cast<eCHAR_TYPE>(j)));
	}

	if(strcmp(szTemp[0], szTemp[1])==0)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return (strcmp(szTemp[0], szTemp[1])<0);
	}
	else
	{
		return (strcmp(szTemp[0], szTemp[1])>=0);
	}
}
//------------------------------------------------------------------------------
/** 
*/
bool _BaseSort_MaxDmg(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
	if(a.dwMaxDamageToEnemy == b.dwMaxDamageToEnemy)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return  (a.dwMaxDamageToEnemy < b.dwMaxDamageToEnemy);
	}
	else
	{
		return  (a.dwMaxDamageToEnemy >= b.dwMaxDamageToEnemy);
	}
}
//------------------------------------------------------------------------------
/** 
*/
bool _BaseSort_GiveDmg(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
	if(a.dwTotalDamageToEnemy == b.dwTotalDamageToEnemy)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return  (a.dwTotalDamageToEnemy < b.dwTotalDamageToEnemy);
	}
	else
	{
		return  (a.dwTotalDamageToEnemy >= b.dwTotalDamageToEnemy);
	}
}
//------------------------------------------------------------------------------
/** 
*/
bool _BaseSort_GetDmg(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
	if(a.dwTotalDamageByEnemy == b.dwTotalDamageByEnemy)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return  (a.dwTotalDamageByEnemy < b.dwTotalDamageByEnemy);
	}
	else
	{
		return  (a.dwTotalDamageByEnemy >= b.dwTotalDamageByEnemy);
	}
}
//------------------------------------------------------------------------------
/** 
*/
bool _BaseSort_Heal(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
	if(a.dwTotalHealToParty == b.dwTotalHealToParty)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return  (a.dwTotalHealToParty < b.dwTotalHealToParty);
	}
	else
	{
		return  (a.dwTotalHealToParty >= b.dwTotalHealToParty);
	}
}
//------------------------------------------------------------------------------
/** 
*/
bool _BaseSort_Kill(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
	if(a.wKillCount == b.wKillCount)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return  (a.wKillCount < b.wKillCount);
	}
	else
	{
		return  (a.wKillCount >= b.wKillCount);
	}
}
//------------------------------------------------------------------------------
/** 
*/
bool _BaseSort_Die(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
	if(a.wDeadCount == b.wDeadCount)	// 같은 경우 다시 이름순으로 정렬
	{
		return (a.name < b.name);
	}

	if(bOrderBy)
	{
		return  (a.wDeadCount < b.wDeadCount);
	}
	else
	{
		return  (a.wDeadCount >= b.wDeadCount);
	}
}
//------------------------------------------------------------------------------
/** 
*/
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
bool _BaseSort_StrongPoint_Attack(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
    if(a.strongpoint_attack == b.strongpoint_attack)	// 같은 경우 다시 이름순으로 정렬
    {
        return (a.name < b.name);
    }

    if(bOrderBy)
    {
        return  (a.strongpoint_attack < b.strongpoint_attack);
    }
    else
    {
        return  (a.strongpoint_attack >= b.strongpoint_attack);
    }
}
//------------------------------------------------------------------------------
/** 
*/
bool _BaseSort_StrongPoint_Defense(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
    if(a.strongpoint_defense == b.strongpoint_defense)	// 같은 경우 다시 이름순으로 정렬
    {
        return (a.name < b.name);
    }

    if(bOrderBy)
    {
        return  (a.strongpoint_defense < b.strongpoint_defense);
    }
    else
    {
        return  (a.strongpoint_defense >= b.strongpoint_defense);
    }
}
//------------------------------------------------------------------------------
/** 
*/
bool _BaseSort_Series_Kill(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
    if(a.series_kill == b.series_kill)	// 같은 경우 다시 이름순으로 정렬
    {
        return (a.name < b.name);
    }

    if(bOrderBy)
    {
        return  (a.series_kill < b.series_kill);
    }
    else
    {
        return  (a.series_kill >= b.series_kill);
    }
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
//------------------------------------------------------------------------------
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
bool _BaseSort_Gold_Point(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b, bool bOrderBy)
{
    if(bOrderBy)
    {
        return  (a.gold_point_ < b.gold_point_);
    }
    else
    {
        if (a.gold_point_ == b.gold_point_)
        {
            if (a.wKillCount == b.wKillCount)
            {
                if (a.wDeadCount == b.wDeadCount)
                {
                    if (a.dwTotalDamageToEnemy == b.dwTotalDamageToEnemy)
                    {
                        if (a.dwTotalHealToParty == b.dwTotalHealToParty)
                        {
                            return (a.name < b.name);
                        }
                        else
                        {
                            return (a.dwTotalHealToParty > b.dwTotalHealToParty);
                        }
                    }
                    else
                    {
                        return (a.dwTotalDamageToEnemy > b.dwTotalDamageToEnemy);
                    }
                }
                else
                {
                    return (a.wDeadCount < b.wDeadCount);
                }
            }
            else
            {
                return (a.wKillCount > b.wKillCount);
            }
        }
        else
        {
            return (a.gold_point_ > b.gold_point_);
        }
    }
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
//------------------------------------------------------------------------------
/** 오름차순 GetOrderBy()==true / 내림차순  GetOrderBy()==false
*/
uiBattleScoreMan* GetBTSMan()
{
	static uiBattleScoreMan* pBTSMan = static_cast<uiBattleScoreMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
	return pBTSMan;
}
bool _Sort_Name(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
	return _BaseSort_Name(a, b, GetBTSMan()->GetOrderBy());
}
bool _Sort_Lv(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
    return _BaseSort_Lv(a, b, GetBTSMan()->GetOrderBy());
}
bool _Sort_Class(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
	return _BaseSort_Class(a, b, GetBTSMan()->GetOrderBy());
}
bool _Sort_MaxDmg(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
	return _BaseSort_MaxDmg(a, b, GetBTSMan()->GetOrderBy());
}
bool _Sort_GiveDmg(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
	return _BaseSort_GiveDmg(a, b, GetBTSMan()->GetOrderBy());
}
bool _Sort_GetDmg(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
    return _BaseSort_GetDmg(a, b, GetBTSMan()->GetOrderBy());
}
bool _Sort_Heal(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
	return _BaseSort_Heal(a, b, GetBTSMan()->GetOrderBy());
}
bool _Sort_Kill(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
	return _BaseSort_Kill(a, b, GetBTSMan()->GetOrderBy());
}
bool _Sort_Die(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
	return _BaseSort_Die(a, b, GetBTSMan()->GetOrderBy());
}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
bool _Sort_StrongPoint_Attack(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
    return _BaseSort_StrongPoint_Attack(a, b, GetBTSMan()->GetOrderBy());
}
bool _Sort_StrongPoint_Defense(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
    return _BaseSort_StrongPoint_Defense(a, b, GetBTSMan()->GetOrderBy());
}
bool _Sort_Series_Kill(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
    return _BaseSort_Series_Kill(a, b, GetBTSMan()->GetOrderBy());
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
bool _Sort_Gold_Point(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b)
{
    return _BaseSort_Gold_Point(a, b, GetBTSMan()->GetOrderBy());
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
//------------------------------------------------------------------------------
/**
*/
void _SortNone(nArray<BATTLESCORE_DATA> &Array)
{
	if(g_pHero == NULL)
	{
		return;
	}	
	BATTLESCORE_DATA temp;
	if(Array.Size() > 2)
	{
		temp = Array[0];
		for(int i = 0; i < Array.Size(); ++i)
		{
			if (Array[i].dwCharGuid == g_pHero->GetCharInfo()->m_CharGuid)
			{
				Array[0] = Array[i];
				Array[i] = temp;
			}
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiBattleScoreMan::SortbyList()
{
	bool (*fpSortBy)(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b);

	// (OrderBy) true 오름차순, false 내림차순

	//SetOrderBy(OrderBy);
	bool OrderBy = GetOrderBy();
	int iSortBy = GetSortBy();

	for(int i = eHomeTeam; i < eTeamCNT; ++i)
	{
		if(m_BTSMemberArray[i].Size() <= 1)
		{
			continue;
		}

		switch(iSortBy)
		{
		case BTSCROE_LIST_RANK:
			fpSortBy = NULL;
			// hero가 1등 나머진 상관없음
			_SortNone(m_BTSMemberArray[i]);
			break;
		case BTSCROE_LIST_ID:
			fpSortBy = _Sort_Name;
			break;
		case BTSCROE_LIST_LV:
			fpSortBy = _Sort_Lv;
			break;
		case BTSCROE_LIST_CLASS:
			fpSortBy = _Sort_Class;
			break;
		case BTSCROE_LIST_MAXDAMAGE:
			fpSortBy = _Sort_MaxDmg;
			break;
		case BTSCROE_LIST_GIVEDAMAGE:
			fpSortBy = _Sort_GiveDmg;
			break;
		case BTSCROE_LIST_GETDAMAGE:
			fpSortBy = _Sort_GetDmg;
			break;
		case BTSCROE_LIST_HEAL:
			fpSortBy = _Sort_Heal;
			break;
		case BTSCROE_LIST_KILL:
			fpSortBy = _Sort_Kill;
			break;
		case BTSCROE_LIST_DIE:
			fpSortBy = _Sort_Die;
			break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        case BTSCROE_LIST_BATTLE_GROUND_POINT_ATTACK:
            fpSortBy = _Sort_StrongPoint_Attack;
            break;
        case BTSCROE_LIST_BATTLE_GROUND_POINT_DEFENSE:
            fpSortBy = _Sort_StrongPoint_Defense;
            break;
        case BTSCROE_LIST_BATTLE_GROUND_SERIES_KILL:
            fpSortBy = _Sort_Series_Kill;
            break;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        case BTSCROE_LIST_GOLDRUSH_GOLDPOINT:
            fpSortBy = _Sort_Gold_Point;
            break;            
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
		default:
			fpSortBy = NULL;
			break;
		}

		if(fpSortBy != NULL)
		{
			std::sort(m_BTSMemberArray[i].Begin(), m_BTSMemberArray[i].End(), fpSortBy);
		}
	}
}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
void uiBattleScoreMan::RefreshBattleGroundList()
{
    if (battle_score_battle_ground)
    {
        if (battle_score_battle_ground->select_tap() == uiBattleScoreBattleGround::TAB_BUTTON_ALL_TEAM)
        {
            nArray<BATTLESCORE_DATA> all_team_array;
            AllMemberScoreArray(&all_team_array);
            AllMemberSortbyList(all_team_array);
            battle_score_battle_ground->refresh_list_control_data(all_team_array);
        }
        else if (battle_score_battle_ground->select_tap() == uiBattleScoreBattleGround::TAB_BUTTON_HOME_TEAM)
        {
            SortbyList();

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
            battle_score_battle_ground->refresh_list_control_data(m_BTSMemberArray[eHomeTeam]);
#else
            //ui상 블루 자리
            if (g_pHero && (g_pHero->GetTeam() == kBlue))
            {
                battle_score_battle_ground->refresh_list_control_data(m_BTSMemberArray[eHomeTeam]);
            }
            else
            {
                battle_score_battle_ground->refresh_list_control_data(m_BTSMemberArray[eAwayTeam]);
            }
#endif //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        }
        else
        {
            SortbyList();

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
            battle_score_battle_ground->refresh_list_control_data(m_BTSMemberArray[eAwayTeam]);
#else
            //ui상 레드 자리
            if (g_pHero && (g_pHero->GetTeam() == kRed))
            {
                battle_score_battle_ground->refresh_list_control_data(m_BTSMemberArray[eHomeTeam]);
            }
            else
            {
                battle_score_battle_ground->refresh_list_control_data(m_BTSMemberArray[eAwayTeam]);
            }
#endif //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        }
    }
}

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
void uiBattleScoreMan::RefreshGoldRushList()
{    
    if(battle_score_gold_rush_ptr_)
    {
        // 소팅
        bool is_sort = false;
        this->SetSortBy(uiBattleScoreMan::BTSCROE_LIST_GOLDRUSH_GOLDPOINT);
        this->SetOrderBy(is_sort);
        this->SortbyList();
        for(int i = eHomeTeam; i < eTeamCNT; ++i)
        {
            battle_score_gold_rush_ptr_->_RefreshListCtrlData(m_BTSMemberArray[i], (E_TEAM)i);
        }
    }
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

void uiBattleScoreMan::AllMemberScoreArray(nArray<BATTLESCORE_DATA>* all_member_array)
{
    //모든 팀 멤버 점수
    if (all_member_array == NULL)
    {
        return;
    }
    
    all_member_array->Clear();

    for (int i = eHomeTeam; i < eTeamCNT; ++i)
    {
        if (m_BTSMemberArray[i].Size() < 1)
        {
            continue;
        }

        for (int j = 0; j < m_BTSMemberArray[i].Size(); ++j)
        {
            all_member_array->PushBack(m_BTSMemberArray[i][j]);
        }
    }
}

void uiBattleScoreMan::AllMemberSortbyList(nArray<BATTLESCORE_DATA>& all_member_array)
{
    bool (*fpSortBy)(const BATTLESCORE_DATA &a, const BATTLESCORE_DATA &b);

    // (OrderBy) true 오름차순, false 내림차순

    //SetOrderBy(OrderBy);
    bool OrderBy = GetOrderBy();
    int iSortBy = GetSortBy();

    
    if(all_member_array.Size() <= 1)
    {
        return;
    }

    switch(iSortBy)
    {
    case BTSCROE_LIST_RANK:
        fpSortBy = NULL;
        // hero가 1등 나머진 상관없음
        _SortNone(all_member_array);
        break;
    case BTSCROE_LIST_ID:
        fpSortBy = _Sort_Name;
        break;
    case BTSCROE_LIST_LV:
        fpSortBy = _Sort_Lv;
        break;
    case BTSCROE_LIST_CLASS:
        fpSortBy = _Sort_Class;
        break;
    case BTSCROE_LIST_MAXDAMAGE:
        fpSortBy = _Sort_MaxDmg;
        break;
    case BTSCROE_LIST_GIVEDAMAGE:
        fpSortBy = _Sort_GiveDmg;
        break;
    case BTSCROE_LIST_GETDAMAGE:
        fpSortBy = _Sort_GetDmg;
        break;
    case BTSCROE_LIST_HEAL:
        fpSortBy = _Sort_Heal;
        break;
    case BTSCROE_LIST_KILL:
        fpSortBy = _Sort_Kill;
        break;
    case BTSCROE_LIST_DIE:
        fpSortBy = _Sort_Die;
        break;
    case BTSCROE_LIST_BATTLE_GROUND_POINT_ATTACK:
        fpSortBy = _Sort_StrongPoint_Attack;
        break;
    case BTSCROE_LIST_BATTLE_GROUND_POINT_DEFENSE:
        fpSortBy = _Sort_StrongPoint_Defense;
        break;
    case BTSCROE_LIST_BATTLE_GROUND_SERIES_KILL:
        fpSortBy = _Sort_Series_Kill;
        break;
    default:
        fpSortBy = NULL;
        break;
    }

    if(fpSortBy != NULL)
    {
        std::sort(all_member_array.Begin(), all_member_array.End(), fpSortBy);
    }

}

void uiBattleScoreMan::EnableBattleGroundExitButton()
{
    if (battle_score_battle_ground)
    {
        battle_score_battle_ground->enable_exit_button(true);
    }
}

void uiBattleScoreMan::EnableGoldRushExitButton()
{
    if (battle_score_gold_rush_ptr_)
    {
        battle_score_gold_rush_ptr_->enable_exit_button(true);
    }
}

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
int uiBattleScoreMan::get_team_gold_point( E_TEAM eTeam )
{
    int add_int = 0;
    int cnt = m_BTSMemberArray[eTeam].Size();
    for(int i = 0; i<cnt; ++i)
    {
        add_int += m_BTSMemberArray[eTeam][i].gold_point_;   // 총 골드량
    }

    return add_int;
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

