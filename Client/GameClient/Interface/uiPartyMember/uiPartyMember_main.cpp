//------------------------------------------------------------------------------
//  uiPartyMember_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiPartyMember.h"

#ifdef _DH_BATTLEZONE2_
#include "BattlezoneMissionTreeParser.h"
#include "map.h"
#endif //_DH_BATTLEZONE2_

//------------------------------------------------------------------------------
/**
*/
uiPartyMember::uiPartyMember(InterfaceManager *pUIMan) :
    uiBase(pUIMan)
{
    this->m_fGaugeSize = 1.0f;
	this->m_fMPGaugeSize = 1.0f;


    m_blinkTick = 0;
#ifdef _NA_000000_20130114_RENEWER_UI
	m_fTop = 138.0f;
	m_fLeft = 2.0f;
#else
	m_fTop = 130.0f;
	m_fLeft = 10.0f;
#endif

    m_bHpBlinkType = false;
#ifdef _DH_BATTLEZONE2_
    mouse_over_ = false;
#endif //_DH_BATTLEZONE2_
}

//------------------------------------------------------------------------------
/**
*/
uiPartyMember::~uiPartyMember()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    CControlWZ* ctl;

    m_ctrlBtnMaster = (CCtrlButtonWZ *)getControl(DLGPOS_MASTER_MARK);

    if (m_ctrlBtnMaster) 
    {
        m_ctrlBtnMaster->ShowWindowWZ(WZ_HIDE);
    }

    ctl = getControl(DLGPOS_HPBAR);
	if (ctl)
	{
		m_rcInit = ctl->GetSize();
	}
	
    ctl = getControl(DLGPOS_MPBAR);
	if (ctl)
	{
		m_rcMPInit = ctl->GetSize();
	}

    ctl = getControl( DLGPOS_TXT );
    if( ctl )
    {
        ((CCtrlStaticWZ*)ctl)->SetTextWZ( _T("") );
        RECT rect = ctl->GetSizeRect();
        //rect.top = rect.top-2;
        FRECT frect= { static_cast<float>(rect.left),
            static_cast<float>(rect.top),
            static_cast<float>(rect.right),
            static_cast<float>(rect.bottom)
            };
        ctl->MoveWindowWZ(frect.left, frect.top-2, frect.right, frect.bottom);
        
    }

	m_ctrlLevelStatic = (CCtrlStaticWZ*)getControl(DLGPOS_LEVEL_TEXT);
	assert(m_ctrlLevelStatic);
	if(m_ctrlLevelStatic)
	{
		m_ctrlLevelStatic->ShowWindowWZ(WZ_HIDE);
	}

	InitPartViewPos();
    status_icon_container_.reserve(10);
#ifdef _DH_BATTLEZONE2_
    CControlWZ* ctrl_pointer = getControl(DLGPOS_BATTLEZONE_READY);
    if (ctrl_pointer)
    {
        ctrl_pointer->ShowWindowWZ(WZ_HIDE);
    }
    output_zone_type_ = OUTPUT_ZONE_NONE;
#endif //_DH_BATTLEZONE2_



}
//------------------------------------------------------------------------------
/**
*/
void uiPartyMember::InitPartViewPos()
{
	RECT rcTmp = { 0, 0, 0, 0 };
	int iInitCnt = 0;

	CControlWZ* pControl = NULL;
	CControlWZ* pControl2 = NULL;

	RECT rc;
    GetDialogWZ()->GetDlgSize(&rc);
    double height = rc.bottom - rc.top;
	DWORD dwGap = m_nGap;

	float fWinTop = this->m_fTop + (((float)height * (float)dwGap)+2.0f);
	float fWinLeft = this->m_fLeft;

	pControl = getControl(DLGPOS_NAME);
	if(pControl)
	{
		m_rcName = pControl->GetSizeRect();
		
		// right를 width로 bottom을 height로 계산
		m_rcName.right = m_rcName.right - m_rcName.left;
		m_rcName.bottom = m_rcName.bottom - m_rcName.top;
		++iInitCnt;
	}
	
	pControl  = getControl(DLGPOS_HPBAR);
	pControl2 = getControl(DLGPOS_MPBAR);
	if(pControl && pControl2)
	{
		m_rcGague = pControl->GetSizeRect();
		rcTmp   = pControl2->GetSizeRect();
		m_rcGague.bottom = rcTmp.bottom;

		// right 를 width로 bottom을 height로 계산
		m_rcGague.right = m_rcGague.right - m_rcGague.left;
		m_rcGague.bottom = m_rcGague.bottom - m_rcGague.top;
		++iInitCnt;
	}

	pControl  = getControl(DLGPOS_BUFF01);

#ifdef _NA_000000_20130114_RENEWER_UI
	pControl2 = getControl(DLGPOS_BUFF28);
#else
	pControl2 = getControl(DLGPOS_BUFF10);
#endif

	if(pControl)
	{
		m_rcBuf = pControl->GetSizeRect();
		rcTmp = pControl2->GetSizeRect();
		m_rcBuf.right = rcTmp.right;

		// right 를 width로 bottom을 height로 계산
		m_rcBuf.right = m_rcBuf.right - m_rcBuf.left;
		m_rcBuf.bottom = m_rcBuf.bottom - m_rcBuf.top;
		++iInitCnt;
	}

	if(iInitCnt == 3)
	{
		m_bInitPartPos = true;
	}
	else
	{
		m_bInitPartPos = false;
		assert(!"Can not find controls 22_2_Part_Member2.iwz");// 컨트롤이 부족하여 초기화 불가
	}


}
//------------------------------------------------------------------------------
/**
*/
void uiPartyMember::Release()
{
}
//------------------------------------------------------------------------------
/**
*/
#ifdef _DH_BATTLEZONE2_
void uiPartyMember::CheckOutputZoneType()
{
    SetOutputZoneType(OUTPUT_ZONE_NONE);

    const uicPartyMemberInfoPlus& member_info = this->GetMyMemberInfo();
    if (member_info.map_code() != 0)
    {
        MAPID current_map_code = g_pMap->GetMapID();
        //히어로 위치
        const BattlezoneMissionTreeInfo* find_info_hero =
            BattlezoneMissionTreeParser::Instance()->FindData(static_cast<MAPCODE>(current_map_code));
        //플레이어 위치
        const BattlezoneMissionTreeInfo* find_info_member =
            BattlezoneMissionTreeParser::Instance()->FindData(static_cast<MAPCODE>(member_info.map_code()));
        if ((find_info_hero == NULL) && (find_info_member != NULL))
        {
            SetOutputZoneType(OUTPUT_ZONE_BATTLEZONE);
        }
        else if ((find_info_hero != NULL) && (find_info_member == NULL))
        {
            SetOutputZoneType(OUTPUT_ZONE_FILED);
        }
    }
}
#endif //_DH_BATTLEZONE2_

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
