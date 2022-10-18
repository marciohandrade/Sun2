//------------------------------------------------------------------------------
//  uiBattleZoneMan_load.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneMan.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "uiBattleZoneCreateEp2/uiBattleZoneCreateEp2.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "uiBattleZonePassword/uiBattleZonePassword.h"
#include "uiBattleZoneReadyCHuntingConfig/uiBattleZoneReadyCHuntingConfig.h"
#include "uiBattleZoneReadyMissionConfig/uiBattleZoneReadyMissionConfig.h"
#include "uiBattleZoneWinDialog/uiBattleZoneWinDialog.h"
#include "uiBattleZoneCHuntingScoreBoard/uiBattleZoneCHuntingScoreBoard.h"
#include "uiBattleZoneDoing/uiBattleZoneDoing.h"
#include "uiBattleZoneReadyPVPConfig/uiBattleZoneReadyPVPConfig.h"
#include "interfacemanager.h"

const DWORD IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_PVP_CFG			= StrToWzID("0634");
const DWORD IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST					= StrToWzID("0610");
const DWORD IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE				= StrToWzID("0620");
const DWORD IM_BATTLEZONE_MANAGER::BATTLEZONE_PASSWORD				= StrToWzID("0622");
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
const DWORD IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_MISSION_CFG		= StrToWzID("0632");
const DWORD IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG	= StrToWzID("0633");
const DWORD IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_WIN			= StrToWzID("0670");
const DWORD IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_SCORE_BOARD	= StrToWzID("0671");
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
const DWORD IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_DOING		= StrToWzID("0672");
const DWORD IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE_EP2			= StrToWzID("0623");

BOOL uiBattleZoneMan::_LoadUI()
{
	m_pUIBattleZoneReadyPVPConfig = CreateUIDialog<uiBattleZoneReadyPVPConfig>(
								    IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_PVP_CFG,
								    "Data\\Interface\\63_1_BattleZone_setting_pvp.iwz",
								    this );

	m_pUIBattleZoneList =  CreateUIDialog<uiBattleZoneList>(
						   IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST,
						   "Data\\Interface\\61_BattleZone_List_New.iwz",
						   this );

	m_pUIBattleZoneCreate = CreateUIDialog<uiBattleZoneCreate>(
							IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE,
							"Data\\Interface\\62_BattleZone_Create_New.iwz",
							this );

    ui_battlezone_create_ep2_ = CreateUIDialog<uiBattleZoneCreateEp2>(
        IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE_EP2,
        "Data\\Interface\\62_3_BattleZone_Create_New2.iwz",
        this );

	m_pUIBattleZonePassword = CreateUIDialog<uiBattleZonePassword>(
							  IM_BATTLEZONE_MANAGER::BATTLEZONE_PASSWORD,
							  "Data\\Interface\\62_2_Input_Password.iwz",
							  this );


#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	m_pUIBattleZoneReadyMissionConfig = CreateUIDialog<uiBattleZoneReadyMissionConfig>(
										IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_MISSION_CFG,
										"Data\\Interface\\63_2_BattleZone_mission_new.iwz",
										this, TRUE );


	m_pUIBattleZoneReadyCHuntingConfig = CreateUIDialog<uiBattleZoneReadyCHuntingConfig>(
										 IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG,
										 "Data\\Interface\\63_1_BattleZone_setting_com.iwz",
										 this, TRUE );

	m_pUIBattleZoneWinDialog = CreateUIDialog<uiBattleZoneWinDialog>(
							   IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_WIN,
							   "Data\\Interface\\67_Competition_win.iwz",
							   this, TRUE );

	m_pUIBattleZoneCHuntingScoreBoard = CreateUIDialog<uiBattleZoneCHuntingScoreBoard>(
										IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_SCORE_BOARD,
										"Data\\Interface\\67_1_Competition_result.iwz",
										this, TRUE );
#endif //_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

	m_pUIBattleZoneDoing = CreateUIDialog<uiBattleZoneDoing>(
						   IM_BATTLEZONE_MANAGER::BATTLEZONE_CHUNTING_DOING,
						   "Data\\Interface\\67_2_Competition_doing.iwz",
						   this, TRUE );


	
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------