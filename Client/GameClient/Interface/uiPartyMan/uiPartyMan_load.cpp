//------------------------------------------------------------------------------
//  uiPartyMan_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiPartyMan.h"
#include "uiPartyMember/uiPartyMember.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"
#include "uiPartyOption/uiPartyOption.h"
#include "interfacemanager.h"
#include "ItemShareSystem.h"



const char  c_strDlgFname_PartyMember1[]            = ("Data\\Interface\\22_1_Party_Member1.iwz");	
const DWORD c_dwCompareID_PartyMember1              = StrToWzID("0221");
const char  c_strDlgFname_PartyMember2[]            = ("Data\\Interface\\22_2_Party_Member2.iwz");
const DWORD c_dwCompareID_PartyMember2              = StrToWzID("0222");
const char  c_strDlgFname_PartyMember3[]            = ("Data\\Interface\\22_3_Party_Member3.iwz");
const DWORD c_dwCompareID_PartyMember3              = StrToWzID("0223");
const char  c_strDlgFname_PartyMember4[]            = ("Data\\Interface\\22_4_Party_Member4.iwz");
const DWORD c_dwCompareID_PartyMember4              = StrToWzID("0224");
const char  c_strDlgFname_PartyMember5[]            = ("Data\\Interface\\22_5_Party_Member5.iwz");
const DWORD c_dwCompareID_PartyMember5              = StrToWzID("0225");
const char  c_strDlgFname_PartyMember6[]            = ("Data\\Interface\\22_6_Party_Member6.iwz");
const DWORD c_dwCompareID_PartyMember6              = StrToWzID("0226");
const char  c_strDlgFname_PartyMember7[]            = ("Data\\Interface\\22_7_Party_Member7.iwz");
const DWORD c_dwCompareID_PartyMember7              = StrToWzID("0227");
const char  c_strDlgFname_PartyMember8[]            = ("Data\\Interface\\22_8_Party_Member8.iwz");
const DWORD c_dwCompareID_PartyMember8              = StrToWzID("0228");
const char  c_strDlgFname_PartyMember9[]            = ("Data\\Interface\\22_9_Party_Member9.iwz");
const DWORD c_dwCompareID_PartyMember9              = StrToWzID("0229");
const char  c_strDlgFname_PartyOption[]             = ("Data\\Interface\\22_Party_Option.iwz");
const DWORD c_dwCompareID_PartyOption               = StrToWzID("1220");
const char  c_strDlgFname_PartyLeavePopup[]         = ("Data\\Interface\\82_Asking_Window.iwz");
const DWORD c_dwCompareID_PartyLeavePopup           = StrToWzID("0820");


const DWORD	IM_PARTY_MANAGER::PARTY_OPTION			= StrToWzID("1220");
const DWORD	IM_PARTY_MANAGER::PARTY_MEMBER1			= StrToWzID("0221");
const DWORD	IM_PARTY_MANAGER::PARTY_MEMBER2			= StrToWzID("0222");
const DWORD	IM_PARTY_MANAGER::PARTY_MEMBER3			= StrToWzID("0223");
const DWORD	IM_PARTY_MANAGER::PARTY_MEMBER4			= StrToWzID("0224");
const DWORD	IM_PARTY_MANAGER::PARTY_MEMBER5			= StrToWzID("0225");
const DWORD	IM_PARTY_MANAGER::PARTY_MEMBER6			= StrToWzID("0226");
const DWORD	IM_PARTY_MANAGER::PARTY_MEMBER7			= StrToWzID("0227");
const DWORD	IM_PARTY_MANAGER::PARTY_MEMBER8			= StrToWzID("0228");
const DWORD	IM_PARTY_MANAGER::PARTY_MEMBER9			= StrToWzID("0229");
const DWORD	IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM = StrToWzID("2210");
const DWORD	IM_PARTY_MANAGER::PARTY_LEAVE_POPUP		= g_IDGenerator.GenerateID();

//------------------------------------------------------------------------------
/**
*/
BOOL uiPartyMan::_LoadUI()
{
	m_pUIPartyOption = CreateUIDialog<uiPartyOption>(
		               IM_PARTY_MANAGER::PARTY_OPTION,
					   "Data\\Interface\\22_Party_Option.iwz",
					   this );


	m_pUIPartyLeavePopup = CreateUIDialog<uiPartyLeavePopup>( 
							  IM_PARTY_MANAGER::PARTY_LEAVE_POPUP,
							  "Data\\Interface\\82_Asking_Window.iwz",
							  this, TRUE, TRUE );

	DWORD	dwKeyArray[c_sMAX_PARTY_MEMBER_NUM] = { IM_PARTY_MANAGER::PARTY_MEMBER1,
													IM_PARTY_MANAGER::PARTY_MEMBER2,
													IM_PARTY_MANAGER::PARTY_MEMBER3,
													IM_PARTY_MANAGER::PARTY_MEMBER4,
													IM_PARTY_MANAGER::PARTY_MEMBER5,
													IM_PARTY_MANAGER::PARTY_MEMBER6,
													IM_PARTY_MANAGER::PARTY_MEMBER7,
													IM_PARTY_MANAGER::PARTY_MEMBER8,
													IM_PARTY_MANAGER::PARTY_MEMBER9 };

	for( int i = 0; i < c_sMAX_PARTY_MEMBER_NUM; ++i )
	{
		m_pUIPartyMembers[i] = CreateUIDialog<uiPartyMember>(
			                   dwKeyArray[i],
							   "Data\\Interface\\22_1_Party_Member1.iwz",
							   this, FALSE, TRUE );


		m_pUIPartyMembers[i]->SetGap( i );
	}


	m_pUIItemShareSystem = CreateUIDialog<ItemShareSystem>( 
							  IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM,
							  "Data\\Interface\\22_10_Party_Itemdivision.iwz",
							  this, TRUE );

	return TRUE;
}
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
