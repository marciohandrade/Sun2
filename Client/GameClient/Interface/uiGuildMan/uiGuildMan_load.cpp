//------------------------------------------------------------------------------
//  uiGuildMan_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMan.h"
#include "uiGuildMain/uiGuildMain.h"
#include "uiGuildCreate/uiGuildCreate.h"
#include "uiGuildMenu/uiGuildMenu.h"
#include "uiGuildWindow/uiGuildWindow.h"
#include "TargetDialog.h"
#include "ObjectManager.h"
#include "InterfaceManager.h"
#include "uiGuildConvertGP.h"
#include "uiGuildDonateUP.h"
#include "uiGuildRankup.h"
#include "uiGuildNotice/uiGuildNotice.h"
#include "uiGuildRightDelegation/uiGuildRightDelegation.h"
#include "uiGuildDutyVest/uiGuildDutyVest.h"
#include "uiGuildMemberRightView/uiGuildMemberRightView.h"
#include "uiGuildCorpsChange/uiGuildCorpsChange.h"
#include "uiGuildMarkCombination/uiGuildMarkCombination.h"
#include "uiGuildMarkRegisteConfirm/uiGuildMarkRegisteConfirm.h"
#include "uiGuildAlliance/uiGuildAlliance.h"
#include "uiGuildHostility/uiGuildHostility.h"
#include "uiGuildOneSideHostility/uiGuildOneSideHostility.h"
#include "uiGuildRelationAsk/uiGuildRelationAsk.h"
#include "uiGuildAllianceRequest/uiGuildAllianceRequest.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "GuildSystem/uiGuildWareHouse.h"
#else
    #include "uiGuildWareHouse/uiGuildWareHouse.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "uiGuildWareHouseRight/uiGuildWareHouseRight.h"
#include "uiGuildWareHouseLog/uiGuildWareHouseLog.h"
#include "uiChangeGuildName/uiChangeGuildName.h"

const DWORD  IM_GUILD_MANAGER::GUILD_CREATE					= StrToWzID("0730");
const DWORD  IM_GUILD_MANAGER::GUILD_WINDOW					= StrToWzID("0733");
const DWORD  IM_GUILD_MANAGER::GUILD_MENU					= StrToWzID("0731");
const DWORD  IM_GUILD_MANAGER::GUILD_MAIN					= StrToWzID("0732");
const DWORD  IM_GUILD_MANAGER::GUILD_CONVERT_GP				= StrToWzID("0735");
const DWORD  IM_GUILD_MANAGER::GUILD_DONATE_UP				= StrToWzID("0734");
const DWORD  IM_GUILD_MANAGER::GUILD_NOTICE					= StrToWzID("0736");
const DWORD  IM_GUILD_MANAGER::GUILD_RANKUP					= StrToWzID("0737");
const DWORD  IM_GUILD_MANAGER::GUILD_DUTY_VEST				= StrToWzID("7323");
const DWORD  IM_GUILD_MANAGER::GUILD_RIGHT_DELEGATION		= StrToWzID("7322");
const DWORD  IM_GUILD_MANAGER::GUILD_MEMBER_RIGHT_VIEW		= StrToWzID("7327");
const DWORD  IM_GUILD_MANAGER::GUILD_CORPS_CHANGE			= StrToWzID("7330");
const DWORD	 IM_GUILD_MANAGER::GUILD_CORPS_DESTORY			= g_IDGenerator.GenerateID();
const DWORD  IM_GUILD_MANAGER::GUILD_MARK_COMBINATION		= StrToWzID("7329");
const DWORD  IM_GUILD_MANAGER::GUILD_MARK_REGISTE_CONFIRM	= StrToWzID("7328");
const DWORD  IM_GUILD_MANAGER::GUILD_ALLIANCE				= StrToWzID("7324");
const DWORD  IM_GUILD_MANAGER::GUILD_HOSTILITY				= StrToWzID("7325");
const DWORD  IM_GUILD_MANAGER::GUILD_ONE_SIDE_HOSTILITY		= StrToWzID("7326");
const DWORD  IM_GUILD_MANAGER::GUILD_RELATION_ASK			= StrToWzID("7311");
const DWORD  IM_GUILD_MANAGER::GUILD_ALLIANCE_REQUEST		= g_IDGenerator.GenerateID();
const DWORD  IM_GUILD_MANAGER::GUILD_WAREHOUSE				= StrToWzID("0738");
const DWORD  IM_GUILD_MANAGER::GUILD_WAREHOUSE_RIGHT		= StrToWzID("0739");
const DWORD  IM_GUILD_MANAGER::GUILD_WAREHOUSE_LOG			= StrToWzID("7310");
const DWORD  IM_GUILD_MANAGER::GUILD_CHANGE_GUILD_NAME		= StrToWzID("7332");

BOOL	SEND_CG_GUILD_CHANGE_CORPS_SYN()
{ 
	uiGuildCorpsChange * pDlg = GET_CAST_DIALOG( uiGuildCorpsChange, IM_GUILD_MANAGER::GUILD_CORPS_CHANGE );

	if( pDlg )
		return pDlg->NET_SEND_CG_GUILD_CHANGE_CORPS_SYN();

	return FALSE;
}

BOOL	SEND_CG_GUILD_DESTROY_CORPS_SYN()
{	
	uiGuildCorpsChange * pDlg  = GET_CAST_DIALOG( uiGuildCorpsChange, IM_GUILD_MANAGER::GUILD_CORPS_DESTORY );

	if( pDlg )
		return pDlg->NET_SEND_CG_GUILD_DESTROY_CORPS_SYN();

	return FALSE;
}


BOOL uiGuildMan::_LoadUI()
{
	m_pUIGuildCreate				= CreateUIDialog<uiGuildCreate>( IM_GUILD_MANAGER::GUILD_CREATE, "Data\\Interface\\73_Guild_Create.iwz", this );
	m_pUIGuildWindow				= CreateUIDialog<uiGuildWindow>( IM_GUILD_MANAGER::GUILD_WINDOW, "Data\\Interface\\73_3_Guild_Window2.iwz", this );
	m_pUIGuildMenu					= CreateUIDialog<uiGuildMenu>( IM_GUILD_MANAGER::GUILD_MENU, "Data\\Interface\\73_1_Guild_menu.iwz", this );
	m_pUIGuildMain					= CreateUIDialog<uiGuildMain>( IM_GUILD_MANAGER::GUILD_MAIN, "Data\\Interface\\73_2_Guild_Window.iwz", this );
	m_pUIGuildConvertGP				= CreateUIDialog<uiGuildConvertGP>( IM_GUILD_MANAGER::GUILD_CONVERT_GP, "Data\\interface\\73_5_Guild_GP.iwz", this );
	m_pUIGuildDonateUP				= CreateUIDialog<uiGuildDonateUP>( IM_GUILD_MANAGER::GUILD_DONATE_UP, "Data\\interface\\73_4_Guild_up.iwz", this );
	m_pUIGuildNotice				= CreateUIDialog<uiGuildNotice>( IM_GUILD_MANAGER::GUILD_NOTICE, "Data\\interface\\73_6_Guild_Message.iwz", this );
	m_pUIGuildRankup				= CreateUIDialog<uiGuildRankup>( IM_GUILD_MANAGER::GUILD_RANKUP, "Data\\interface\\73_7_Guild_Rankup.iwz", this );
	m_pUIGuildDutyVest				= CreateUIDialog<uiGuildDutyVest>( IM_GUILD_MANAGER::GUILD_DUTY_VEST, "Data\\Interface\\73_23_Guild_DutyVest.iwz", this );
	m_pUIGuildRightDelegation		= CreateUIDialog<uiGuildRightDelegation>( IM_GUILD_MANAGER::GUILD_RIGHT_DELEGATION, "Data\\Interface\\73_22_Guild_RightDelegation.iwz", this, TRUE );
	m_pUIGuildMemberRightView		= CreateUIDialog<uiGuildMemberRightView>( IM_GUILD_MANAGER::GUILD_MEMBER_RIGHT_VIEW, "Data\\Interface\\73_27_Guild_Authority.iwz", this );
	m_pUIGuildMarkCombination		= CreateUIDialog<uiGuildMarkCombination>( IM_GUILD_MANAGER::GUILD_MARK_COMBINATION, "Data\\Interface\\73_29_Guild_MarkCombination.iwz", this );
	m_pUIGuildMakrRegisteConfirm	= CreateUIDialog<uiGuildMarkRegisteConfirm>( IM_GUILD_MANAGER::GUILD_MARK_REGISTE_CONFIRM, "Data\\Interface\\73_28_Guild_Record.iwz", this );
	m_pUIGuildAlliance				= CreateUIDialog<uiGuildAlliance>( IM_GUILD_MANAGER::GUILD_ALLIANCE, "Data\\interface\\73_24_Guild_Relation.iwz", this );
	m_pUIGuildHostility				= CreateUIDialog<uiGuildHostility>( IM_GUILD_MANAGER::GUILD_HOSTILITY, "Data\\interface\\73_25_Guild_Relation2.iwz", this );
	m_pUIGuildOneSideHostility		= CreateUIDialog<uiGuildOneSideHostility>( IM_GUILD_MANAGER::GUILD_ONE_SIDE_HOSTILITY, "Data\\interface\\73_26_Guild_Relation3.iwz", this );
	m_pUIGuildRelationAsk			= CreateUIDialog<uiGuildRelationAsk>( IM_GUILD_MANAGER::GUILD_RELATION_ASK, "Data\\interface\\73_11_Guild_Relation_Ask.iwz", this );
	m_pUIGuildAllianceRequest		= CreateUIDialog<uiGuildAllianceRequest>( IM_GUILD_MANAGER::GUILD_ALLIANCE_REQUEST, "Data\\Interface\\82_Asking_Window.iwz", this, FALSE, TRUE );
	m_pUIGuildWareHouse				= CreateUIDialog<uiGuildWareHouse>( IM_GUILD_MANAGER::GUILD_WAREHOUSE, "Data\\interface\\73_8_Guild_Ware_House.iwz", this, TRUE );
	m_pUIGuildWareHouseLog			= CreateUIDialog<uiGuildWareHouseLog>( IM_GUILD_MANAGER::GUILD_WAREHOUSE_LOG, "Data\\interface\\73_10_Guild_Ware_House.iwz", this );
	m_pUiGuildCorpsChange			= CreateUIDialog<uiGuildCorpsChange>( IM_GUILD_MANAGER::GUILD_CORPS_CHANGE, "Data\\Interface\\73_30_Guild_CorpsChange.iwz", this, TRUE );
	m_pUIGuildCorpsDestory			= CreateUIDialog<uiGuildCorpsChange>( IM_GUILD_MANAGER::GUILD_CORPS_DESTORY, "Data\\Interface\\73_30_Guild_CorpsChange.iwz", this, TRUE, TRUE );
	m_pUIChangeGuildName			= CreateUIDialog<uiChangeGuildName>( IM_GUILD_MANAGER::GUILD_CHANGE_GUILD_NAME, "Data\\interface\\73_32_Guild_NameChange.iwz", this, TRUE );

    if (m_pUiGuildCorpsChange)
    {
	    m_pUiGuildCorpsChange->SetOkBtnFnPtr( SEND_CG_GUILD_CHANGE_CORPS_SYN );
	    m_pUiGuildCorpsChange->SetCaptionTitleName(1358);
	    m_pUiGuildCorpsChange->SetCaptionSubTitleName(6249);
    }

	//m_pUIGuildCorpsDestory = CreateCloneDialog<uiGuildCorpsChange>( IM_GUILD_MANAGER::GUILD_CORPS_CHANGE, IM_GUILD_MANAGER::GUILD_CORPS_DESTORY );
    if (m_pUIGuildCorpsDestory)
    {
	    m_pUIGuildCorpsDestory->SetOkBtnFnPtr( SEND_CG_GUILD_DESTROY_CORPS_SYN );
	    //1489	군단 해산
	    m_pUIGuildCorpsDestory->SetCaptionTitleName(1489);
	    //6273	해산하실 군단을 선택하세요.
	    m_pUIGuildCorpsDestory->SetCaptionSubTitleName(6273);
    }

	return TRUE;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------