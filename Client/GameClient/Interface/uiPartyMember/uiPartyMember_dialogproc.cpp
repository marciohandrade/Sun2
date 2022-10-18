//------------------------------------------------------------------------------
//  uiPartyMember_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiPartyMan/uiPartyMan.h"
#include "uiPartyMember.h"

#include "Player.h"
#include "ObjectManager.h"
#include "StateInfoParser.h"
#include "Mouse.h"
#include "IconImage.h"
#include "uitargetman/uitargetman.h"
#include "TargetDialog.h"

#include "character.h"
#include "globalfunc.h"
#include "InterfaceManager.h"
#include "uiToolTipMan/uiToolTipMan.h"

#include "uiBattleScoreMan\uiBattleScoreMan.h"

#include "StatusIconManager.h"

#include "AppearanceResourceCode.h"
#ifdef _DH_BATTLEZONE2_
#include "MapInfoParser.h"
#endif //_DH_BATTLEZONE2_
//------------------------------------------------------------------------------
WzID uiPartyMember::m_wzId[DIALOG_MAX] = 
{ 
        StrToWzID("P000"),    // DLGPOS_HPBAR,
        StrToWzID("P001"),    // DLGPOS_BUFF01,
        StrToWzID("P002"),    // DLGPOS_BUFF02,
        StrToWzID("P003"),    // DLGPOS_BUFF03,
        StrToWzID("P004"),    // DLGPOS_BUFF04,
        StrToWzID("P005"),    // DLGPOS_BUFF05,
        StrToWzID("P006"),    // DLGPOS_BUFF06,
        StrToWzID("P007"),    // DLGPOS_BUFF07,
        StrToWzID("P008"),    // DLGPOS_BUFF08,
        StrToWzID("P009"),    // DLGPOS_BUFF09,
        StrToWzID("P010"),    // DLGPOS_BUFF10,


        StrToWzID("PI01"),    // DLGPOS_FACE00, //초상화 버서커
        StrToWzID("PI02"),    // DLGPOS_FACE01, //       드라곤 나이트
        StrToWzID("PI03"),    // DLGPOS_FACE02, //       섀도우
        StrToWzID("PI04"),    // DLGPOS_FACE03, //       발키리
        StrToWzID("PI05"),    // DLGPOS_FACE04, //       엘리멘탈
        StrToWzID("PI11"),    // DLGPOS_FACE05, //       미스틱 //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        StrToWzID("PI12"),    // DLGPOS_FACE06
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        StrToWzID("P032"),    // DLGPOS_FACE07
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        StrToWzID("P011"),    // DLGPOS_CLASS_MARK00,  //마크 버서커
        StrToWzID("P012"),    // DLGPOS_CLASS_MARK01,  //     드라곤 나이트
        StrToWzID("P013"),    // DLGPOS_CLASS_MARK02,  //     섀도우
        StrToWzID("P014"),    // DLGPOS_CLASS_MARK03,  //     발키리
        StrToWzID("P015"),    // DLGPOS_CLASS_MARK04,  //     엘리멘탈
        StrToWzID("P017"),    // DLGPOS_CLASS_MARK05,  //     미스틱 //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        StrToWzID("P029"),    // DLGPOS_CLASS_MARK06
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        StrToWzID("P033"),    // DLGPOS_CLASS_MARK07
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        StrToWzID("T000"),    // DLGPOS_TXT,
        StrToWzID("BT00"),    // DLGPOS_LEAVE,
        StrToWzID("PI10"),    // DLGPOS_MINE,
        StrToWzID("B222"),    // DLGPOS_TARGETSHARE,
        StrToWzID("PI00"),    // DLGPOS_PCMARK

#ifdef _NA_000000_20130114_RENEWER_UI
#else
		StrToWzID("CT01"),    // DLGPOS_MIC_ON,
#endif

        StrToWzID("B100"),      // DLGPOS_MASTER_MARK,
        StrToWzID("BT01"),      // DLGPOS_BTN_SELECTED_MARK

		StrToWzID("PI06"),		// DLGPOS_MPBAR,
		StrToWzID("ST00"),	    // DLGPOS_LEVEL_TEXT,

        StrToWzID("PI07"),	    // DLGPOS_HPBAR_BK
#ifdef _NA_000000_20130114_RENEWER_UI
#else
		StrToWzID("PI00"),		//DLGPOS_LV_BK, // 레벨 배경
		StrToWzID("PI08"),	    //DLGPOS_FACE_BK
#endif
		StrToWzID("PI09"),	    //DLGPOS_NAME	


#ifdef _DH_BATTLEZONE2_
        StrToWzID("P016"),      //DLGPOS_BATTLEZONE_READY
#endif //_DH_BATTLEZONE2_

#ifdef  _NA_000000_20130114_RENEWER_UI
		StrToWzID("P019"),    // DLGPOS_BUFF19,
		StrToWzID("P020"),    // DLGPOS_BUFF20,
		StrToWzID("P021"),    // DLGPOS_BUFF21,
		StrToWzID("P022"),    // DLGPOS_BUFF22,
		StrToWzID("P023"),    // DLGPOS_BUFF23,
		StrToWzID("P024"),    // DLGPOS_BUFF24,
		StrToWzID("P025"),    // DLGPOS_BUFF25,
		StrToWzID("P026"),    // DLGPOS_BUFF26,
		StrToWzID("P027"),    // DLGPOS_BUFF27,
		StrToWzID("P028"),    // DLGPOS_BUFF28,

		StrToWzID("P018"),    // DLGPOS_BUFF28,
#endif
};

//------------------------------------------------------------------------------ 
void uiPartyMember::MoveDefaultWindowPos()
{
	uiBase::MoveDefaultWindowPos();

    if (IsVisible() == TRUE)
    {
        RECT rc;
        GetDialogWZ()->GetDlgSize(&rc);

        double height = rc.bottom - rc.top;

        DWORD dwGap = m_nGap;
        MoveWindowWZ( m_fLeft, m_fTop + (((float)height * (float)dwGap)) );
    }
}

//------------------------------------------------------------------------------
void uiPartyMember::OnShowWindow(BOOL val)
{
    //this->m_bShow = val;    // for uiBase::IsVisable()

    if (val)
    {
		uiBase::MoveDefaultWindowPos();

        RECT rc;
        GetDialogWZ()->GetDlgSize(&rc);

        double height = rc.bottom - rc.top;

		DWORD dwGap = m_nGap;
		MoveWindowWZ( m_fLeft, m_fTop + (((float)height * (float)dwGap)) );

        this->OffLock();
        this->OffPending();

        this->m_fGaugeSize = (float)this->m_rcInit.right;
		this->m_fMPGaugeSize = (float)this->m_rcMPInit.right;

        CCtrlStaticWZ* pControl = NULL;
        m_ctrlGaugeBackground =
        static_cast<CCtrlPictureWZ *>(this->getControl(DLGPOS_MINE));
        if (pControl)
        {
            pControl->ShowWindowWZ(WZ_HIDE);
        }
        pControl =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TARGETSHARE));
        if (pControl)
        {
            pControl->ShowWindowWZ(WZ_HIDE);
        }
#ifdef _NA_000000_20130114_RENEWER_UI
		CControlWZ* pSPBar =
			static_cast<CControlWZ *>(this->getControl(DLGPOS_SPBAR));
		if (pSPBar)
		{
			pSPBar->ShowWindowWZ(WZ_HIDE);
		}
#else
		CCtrlButtonCheckWZ* pChkBtnControl =
            static_cast<CCtrlButtonCheckWZ *>(this->getControl(DLGPOS_MIC_ON));
        if (pChkBtnControl)
        {
            pChkBtnControl->ShowWindowWZ(WZ_HIDE);
        }
#endif

        CCtrlButtonWZ* pBtnControl =
            static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_SELECTED_MARK));
        if (pBtnControl)
        {
            pBtnControl->ShowWindowWZ(WZ_HIDE);
        }

		pBtnControl =
            static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_LEAVE));
        if (pBtnControl)
        {
            pBtnControl->ShowWindowWZ(WZ_HIDE);
        }

		CControlWZ* pControlPos = NULL;

		pControlPos = getControl(DLGPOS_NAME);
		if(pControlPos)
		{
			rect_pos_init[0] = pControlPos->GetSizeRect();
		}
		pControlPos = getControl(DLGPOS_BTN_SELECTED_MARK);
		if(pControlPos)
		{
			rect_pos_init[1] = pControlPos->GetSizeRect();
		}
		pControlPos = getControl(DLGPOS_LEVEL_TEXT);	
		if(pControlPos)
		{
			rect_pos_init[2] = pControlPos->GetSizeRect();
		}
		pControlPos = getControl(DLGPOS_TXT);
		if(pControlPos)
		{
			rect_pos_init[3] = pControlPos->GetSizeRect();
		}

		pBtnControl =
			static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_LEAVE));
		if (pBtnControl)
		{
			pBtnControl->ShowWindowWZ(WZ_HIDE);
		}

		pControlPos = getControl(DLGPOS_MINE);
		
		if(pControlPos)
		{
			rect_pos_string = pControlPos->GetSizeRect();
		}
    }
    else
    {
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::MessageProc( SI_MESSAGE * pMessage )
{

    if (this->IsLock())
        return;

	switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            if( pos <= DIALOG_MAX ) 
            {
#ifdef _NA_000000_20130114_RENEWER_UI
				if (pos != DLGPOS_LEAVE && pos != DLGPOS_PCMARK)
#else
				if (pos != DLGPOS_LEAVE && pos != DLGPOS_MIC_ON)
#endif
                {
                    this->SetTargeting();
                }
            }
        }
        break;

    case RT_MSG_RBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            if( pos <= DIALOG_MAX ) 
            {
                if (pos != DLGPOS_LEAVE)
                {
                    this->TargetShare(false);
                }
            }
        }
        break;
    case RT_MSG_MOUSEOVER:
        {
#ifdef _DH_BATTLEZONE2_
            mouse_over_ = true;
#endif //_DH_BATTLEZONE2_
        }
        break;
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::SetPartView()
{
	RECT rc;
    GetDialogWZ()->GetDlgSize(&rc);
	RECT rc_MemberWin = rc;
    double height = rc.bottom - rc.top;
	DWORD dwGap = m_nGap;
	float fWinTop = this->m_fTop + (((float)height * (float)dwGap));

	bool bIsShowFace = false;

	if(this->GetManager() == NULL)
	{
		return;
	}
#ifdef _NA_000000_20130114_RENEWER_UI
	bool isFaceOff = false;
	int	 Move_window_scale =0;

	CControlWZ* pControl = getControl(DLGPOS_BATTLEZONE_READY);
	if(pControl)
	{
		if(this->GetManager()->GetShowFace() == true)
		{
			Move_window_scale = 0;
			this->MoveWindowWZ(m_fLeft, fWinTop);
			bIsShowFace = true;
			isFaceOff = false;
		}
		else
		{
			Move_window_scale = -(pControl->GetSizeRect().right - pControl->GetSizeRect().left);
			this->MoveWindowWZ((float)(Move_window_scale), fWinTop);
			isFaceOff = true;
		}
	}

	if(this->GetManager()->GetShowGague() == false)//게이지가 안보일때 이름 위치 이동
	{
		CControlWZ* pControl = NULL;

		pControl = getControl(DLGPOS_NAME);
		CControlWZ* pTargetControl = getControl(DLGPOS_BATTLEZONE_READY);
		RECT rt;
		//현재위치.
		//이동후 위치
		int  isTop= 0;
		RECT MoveRt = pTargetControl->GetSizeRect();
		if(pControl)
		{
			isTop = pControl->GetSizeRect().top;
			rt = pControl->GetSize();
			pControl->MoveWindowWZ((float)((MoveRt.right+1)+(isFaceOff ? 0 : Move_window_scale)), 
				(float)(isTop),
				(float)rt.right, 
				(float)rt.bottom);

			MoveRt = pControl->GetSizeRect();
		}
		
		pControl = getControl(DLGPOS_BTN_SELECTED_MARK);
		if(pControl)
		{
			isTop = pControl->GetSizeRect().top;
			rt = pControl->GetSize();
			pControl->MoveWindowWZ((float)(MoveRt.left+1)+ (isFaceOff ? 0 : Move_window_scale), 
				(float)(isTop)+1,
				(float)rt.right, (float)rt.bottom);

			MoveRt = pControl->GetSizeRect();
		}
	
		pControl = getControl(DLGPOS_LEVEL_TEXT);
		if(pControl)
		{
			isTop = pControl->GetSizeRect().top;
			rt = pControl->GetSize();
			pControl->MoveWindowWZ((float)(MoveRt.left+1)+ (isFaceOff ? 0 : Move_window_scale)
				, (float)(isTop),
				(float)rt.right, (float)rt.bottom);

			MoveRt = pControl->GetSizeRect();
		}

		pControl = getControl(DLGPOS_TXT);
		if(pControl)
		{
			isTop = pControl->GetSizeRect().top;
			rt = pControl->GetSize();
			pControl->MoveWindowWZ((float)(MoveRt.right+1)+ (isFaceOff ? 0 : Move_window_scale)
				, (float)(isTop),
				(float)rt.right, (float)rt.bottom);
		}
	}
	else
	{
		//원래 위치로 이동
		CControlWZ* pControl = NULL;
		pControl = getControl(DLGPOS_NAME);
		if(pControl)
		{
			pControl->MoveWindowWZ((float)rect_pos_init[0].left + Move_window_scale
				+(isFaceOff ? -m_fLeft : 0), 
				(float)rect_pos_init[0].top,
				(float)pControl->GetSize().right, (float)pControl->GetSize().bottom);
		}
		pControl = getControl(DLGPOS_BTN_SELECTED_MARK);
		if(pControl)
		{
			pControl->MoveWindowWZ((float)rect_pos_init[1].left + Move_window_scale
				+(isFaceOff ? -m_fLeft : 0)
				, (float)rect_pos_init[0].top+1,
				(float)pControl->GetSize().right, (float)pControl->GetSize().bottom);
		}
		pControl = getControl(DLGPOS_LEVEL_TEXT);	
		if(pControl)
		{
			pControl->MoveWindowWZ((float)rect_pos_init[2].left + Move_window_scale
				+(isFaceOff ? -m_fLeft : 0)
				, (float)rect_pos_init[0].top,
				(float)pControl->GetSize().right, (float)pControl->GetSize().bottom);
		}
		pControl = getControl(DLGPOS_TXT);
		if(pControl)
		{
			pControl->MoveWindowWZ((float)rect_pos_init[3].left + Move_window_scale
				+(isFaceOff ? -m_fLeft : 0)
				, (float)rect_pos_init[0].top,
				(float)pControl->GetSize().right, (float)pControl->GetSize().bottom);
		}
	}
#else
	CControlWZ* pControl = getControl(DLGPOS_FACE00);
	if(pControl)
	{
		if(this->GetManager()->GetShowFace() == true)
		{
			this->MoveWindowWZ(m_fLeft, fWinTop);
			bIsShowFace = true;
		}
		else
		{
			this->MoveWindowWZ((m_fLeft - (pControl->GetSizeRect().right - pControl->GetSizeRect().left)), fWinTop);
		}
	}

	if(m_bInitPartPos != true)	// 컨트롤들의 rect가 초기화 되지 않았으면 위치 변경을 하지 않는다.
	{
		return;
	}

	if(this->GetManager()->GetShowGague() == false)	// 게이지가 안보일때 버프 위치 이동
	{
		for(int i = DLGPOS_BUFF01; i <= DLGPOS_BUFF10; ++i)
		{
			pControl = getControl(i);
			if(pControl)
			{
				pControl->MoveWindowWZ((float)(pControl->GetSizeRect().left), (float)(fWinTop + m_rcGague.top),
					(float)(pControl->GetSizeRect().right - pControl->GetSizeRect().left), (float)(m_rcBuf.bottom));				
			}
		}
		pControl = getControl(DLGPOS_MASTER_MARK);
		if(pControl)
		{
			height = pControl->GetSizeRect().right - pControl->GetSizeRect().left;
			pControl->MoveWindowWZ((float)(pControl->GetSizeRect().left), (float)(fWinTop + m_rcName.top),
					(float)(pControl->GetSizeRect().right - pControl->GetSizeRect().left), (float)(height));
		}
	}
	else
	{
		for(int i = DLGPOS_BUFF01; i <= DLGPOS_BUFF10; ++i)
		{
			pControl = getControl(i);
			if(pControl)
			{
				pControl->MoveWindowWZ((float)(pControl->GetSizeRect().left), (float)(fWinTop + m_rcBuf.top),
					(float)(pControl->GetSizeRect().right - pControl->GetSizeRect().left), (float)(m_rcBuf.bottom));
			}
		}
		pControl = getControl(DLGPOS_MASTER_MARK);
		if(pControl)
		{
			height = pControl->GetSizeRect().right - pControl->GetSizeRect().left;
			pControl->MoveWindowWZ((float)(pControl->GetSizeRect().left), (float)(fWinTop + m_rcGague.top),
					(float)(pControl->GetSizeRect().right - pControl->GetSizeRect().left), (float)(height));			
		}
	}
#endif
}
//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::OnUpdateSolarDialog()
{
    if (this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_BATTLE_LOBBY))
    {
        this->ShowInterface(FALSE);
        return;
    }

    if (this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_BATTLEZONE_CREATE))
    {
        this->ShowInterface(FALSE);
        return;
    }

    if (this->GetUIMan()->IsShowDialog(InterfaceManager::DIALOG_BATTLEZONE_LIST))
    {
        this->ShowInterface(FALSE);
        return;
    }

    const uicPartyMemberInfoPlus& memInfo = this->GetMyMemberInfo();

    CCtrlButtonWZ* ctrlbtn_Selected =
        static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_SELECTED_MARK));

    if (ctrlbtn_Selected)
    {
        ctrlbtn_Selected->ShowWindowWZ(WZ_HIDE);

        TargetDialog* pTargetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);

        if (pTargetDlg)
        {
			DWORD compKey = memInfo.Get().m_dwMemberKey;

            if (pTargetDlg->GetTargetKey() == compKey)
            {
                ctrlbtn_Selected->ShowWindowWZ(WZ_SHOW);
            }
        }
    }

    if (m_ctrlBtnMaster)
    {
        if (this->GetManager()->IsLeader())
        {
            m_ctrlBtnMaster->ShowWindowWZ(WZ_HIDE);
        }
        else
        {
			if (memInfo.Get().m_dwMemberKey == GetManager()->GetMasterKey())
            {
                m_ctrlBtnMaster->ShowWindowWZ(WZ_SHOW);
            }
            else
            {
                m_ctrlBtnMaster->ShowWindowWZ(WZ_HIDE);
            }
        }
    }

#ifdef _DH_BATTLEZONE2_
    CControlWZ* ctrl_pointer = getControl(DLGPOS_BATTLEZONE_READY);
    if (ctrl_pointer && this->GetManager()->GetShowFace())
    {
        if ((memInfo.is_ready() == true) &&
            (memInfo.Get().m_dwMemberKey != GetManager()->GetMasterKey()))
        {
            ctrl_pointer->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            ctrl_pointer->ShowWindowWZ(WZ_HIDE);
        }
    }
#endif //_DH_BATTLEZONE2_

    this->updateInfos();
}

//------------------------------------------------------------------------------
/**i
*/
void
uiPartyMember::updateInfos()
{
    this->updateHP();
	this->updateMP();
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    //this->updateSD(); // 현재 파티멤버ui에 SD 게이지를 표시하지 않음
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
}
//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::updateHP()
{
    const uicPartyMemberInfoPlus& memInfo = this->GetMyMemberInfo();
	SetHP ((float)(memInfo.Get().m_byHPRatio));
}


//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::updateMP()
{
	const uicPartyMemberInfoPlus& memInfo = this->GetMyMemberInfo();
	SetMP ((float)(memInfo.Get().m_byMPRatio));
}
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::updateSD()
{
    // 현재 파티멤버ui에 SD 게이지를 표시하지 않음
    //const uicPartyMemberInfoPlus& memInfo = this->GetMyMemberInfo();
    //SetSD ((float)(memInfo.Get().m_bySDRatio));
}
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
//------------------------------------------------------------------------------
/**
*/
void uiPartyMember::UpdateStatusIcon()
{
    status_icon_container_.clear();

    bool is_show = (this->GetManager()->IsShowBuffInfos() && this->GetManager()->GetShowBuf() == true);
    if (is_show != true)
    {
        return;
    }

    const _PARTY_MEMBER_INFO& party_member_info = GetMyMemberInfo().Get();

    Player* party_member = 
        static_cast<Player*>(g_ObjectManager.GetObject(party_member_info.m_dwMemberKey));
    if ((party_member == NULL) ||
        (party_member->IsKindOfObject(PLAYER_OBJECT) != TRUE))
    {
        return;
    }

    int mouse_x_position = Mouse::Instance()->GetMouseX();
    int mouse_y_position = Mouse::Instance()->GetMouseY();

    // 타겟 의 SKILL_EFFECT 를 순회
    BASE_STATEINFO* state_info = NULL;

    SKILL_EFFECT_IMAGE status_icon;
    DWORD hero_object_key = party_member_info.m_dwMemberKey;
    int status_Index = 0;

    CControlWZ* buff_control = NULL;

    std::vector<SKILL_EFFECT*>& skill_effect_array = party_member->GetSkillEffectArray();
    for( size_t i=0;i!=skill_effect_array.size();++i)
    {
        SKILL_EFFECT* skill_effect = skill_effect_array[i];

        state_info = StateInfoParser::Instance()->GetStateInfo(skill_effect->dwStatusID);
        status_icon.iEffectID = skill_effect->iEffectID;
        status_icon.dwStatusID = skill_effect->dwStatusID;
        status_icon.dwOwnerID = hero_object_key;
        status_icon.pSkillEffect = skill_effect;
        status_icon.pStateInfo = state_info;

		//여기서 알파값 단계를 내리면 되지만.. 음.?

		if(status_Index < 10)
		{
			status_icon.m_byAlpha = 0xFF;
		}
		else
		{
			status_icon.m_byAlpha = StatusIconManager::ALPHA_VALUE_MOUSE_OVER_ICON;
		}
        // draw_rect 계산\
	
#ifdef _NA_000000_20130114_RENEWER_UI
		buff_control = status_Index < 10 ? (getControl(DLGPOS_BUFF01 + status_Index)):\
											(getControl(DLGPOS_BUFF19 + status_Index - 10));
#else
		buff_control = getControl(DLGPOS_BUFF01 + status_Index);
#endif

		if (buff_control != NULL)
        {
            status_icon.rcDrawRect = buff_control->GetSizeRect();

            // 마우스 오버체크
            if ((status_icon.rcDrawRect.left <= mouse_x_position) && 
                (status_icon.rcDrawRect.right >= mouse_x_position) &&
                (status_icon.rcDrawRect.top <= mouse_y_position) && 
                (status_icon.rcDrawRect.bottom >= mouse_y_position))
            {
                // 툴팁 출력
                uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
                if (tooltip_manager)
                {
                    tooltip_manager->RegisterSkillEffectInfoTooltip(&status_icon, TRUE);
					tooltip_manager->SetTooltipDialogID(GetDialogKey());
                }
            }
            status_icon_container_.push_back(status_icon);
        }        

        ++status_Index;

        if (status_Index >= StatusIconManager::MAX_PARTYMEMBER_ICON)
        {
            break;
        }
    }

    // status_icon_manager에 삽입
    StatusIconManager* status_icon_manager = g_InterfaceManager.GetStatusIconManager();
    if (status_icon_manager)
    {
        status_icon_manager->RegisterStatusIconInfo(status_icon_container_);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::renderStringInfo()
{
	const uicPartyMemberInfoPlus& memInfo = this->GetMyMemberInfo();

    this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(StrToWzID("st10"));

    CCtrlStaticWZ* pControl = (CCtrlStaticWZ*)getControl(DLGPOS_TXT);

    if (pControl)
    {
        RECT rc, rc2;

        rc2 = pControl->GetSizeRect();
		rc2.top += 1;

		SetRect(&rc,
			rc2.left + 5,
			rc2.top,
			rc2.right - 10,
			rc2.top + 14); 

#ifdef _INTERNATIONAL_UI
		SetRect(&rc, rc2.left + 5, rc2.top,rc2.right,rc2.top+ 14); 
#else
		SetRect(&rc,rc2.left + 5,  rc2.top,rc2.right - 10,rc2.top + 14); 
#endif//_INTERNATIONAL_UI

        WzColor _color = COLORDEF_UIPARTY_MEMBER_AT_TIME_LIVE;

		if(memInfo.Get().m_byHPRatio <= 0 )
		{
			_color = COLORDEF_UIPARTY_MEMBER_AT_TIME_DEAD;
		}

        char* charName = const_cast<char*>( memInfo.Get().m_szCharName );

        this->GetDrawBase()->x_pManagerTextOut->DrawText(
            charName,
            &rc,
            _color,
            WzColor_RGBA(0,0,0,0),
            TP_HLEFT,
            ETS_SHADOW
            );

		
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        if (memInfo.IsConnected() == false)
#else
		if( memInfo.IsUnexpectedTerminate())
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
		{

			CControlWZ* control_pointer = getControl(DLGPOS_MINE);

			rc.top  += 18;
			rc.left = rect_pos_string.left;
			rc.bottom += 18;

			// 5240	비정상종료
			this->GetDrawBase()->x_pManagerTextOut->DrawText(
                (TCHAR *) g_InterfaceManager.GetInterfaceString(5240), 
                &rc,
                WzColor_RGBA(255,0,0,255),
                WzColor_RGBA(0,0,0,0),
                TP_HLEFT,
                ETS_SHADOW
                );

            /////////////////////elfleeTest
			uiBattleScoreMan* pBattleScoreMan = static_cast<uiBattleScoreMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));

			if(pBattleScoreMan && pBattleScoreMan->GetMode() != uiBattleScoreMan::eBTS_NONE_MODE)
			{
                int index = pBattleScoreMan->FindMember(memInfo.Get().m_dwMemberKey, eHomeTeam);
			    if(index >= 0)
			    {
				    pBattleScoreMan->DisconnectMember(index, (E_TEAM)eHomeTeam);
				    pBattleScoreMan->RefreshListCtrlData();
			    }
			}
		}
#ifdef _DH_BATTLEZONE2_
        else if (output_zone_type_ != OUTPUT_ZONE_NONE)
        {
            TCHAR message_text[INTERFACE_STRING_LENGTH] ={0,};
            if (output_zone_type_ == OUTPUT_ZONE_FILED)
            {
                // 2489 필드
                g_InterfaceManager.GetInterfaceString(2489, message_text, INTERFACE_STRING_LENGTH);
            }
            else if (output_zone_type_ == OUTPUT_ZONE_BATTLEZONE)
            {
                // 2490 배틀존
                g_InterfaceManager.GetInterfaceString(2490, message_text, INTERFACE_STRING_LENGTH);
            }

			rc.top  += 18;
			rc.left = rect_pos_string.left;
			rc.bottom += 18;

            // 필드 or 배틀존
            this->GetDrawBase()->x_pManagerTextOut->DrawText(
                message_text, 
                &rc, 
                WzColor_RGBA(255,0,0,255),
                WzColor_RGBA(0,0,0,0),
                TP_HLEFT,
                ETS_SHADOW
                );
        }
#endif //_DH_BATTLEZONE2_
    }

	if(this->GetManager() && this->GetManager()->GetShowFace() == true)
	{
		if(m_ctrlLevelStatic)
		{
			char buf[8] = {0,};

			sprintf( buf, "%d", memInfo.Get().m_wLevel );

			WzColor LvColor = COLORDEF_UIPARTY_MEMBER_AT_TIME_LIVE;
			RECT rcLevel = m_ctrlLevelStatic->GetSizeRect();
			rcLevel.top += 1;

			this->GetDrawBase()->x_pManagerTextOut->DrawText(
				buf,
				&rcLevel,
				LvColor,
				WzColor_RGBA(0,0,0,0),
				TP_HCENTER,
				ETS_SHADOW);	
		}
	}
	
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::OnRenderSolarDialog()
{
	renderStringInfo();

    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

#ifdef _DH_BATTLEZONE2_
    //위치 출력
    RECT rect;
    CControlWZ* control_pointer;
    bool is_draw_tooltip = false;
    control_pointer = getControl(DLGPOS_BTN_SELECTED_MARK);
    if (control_pointer)
    {
        rect = control_pointer->GetSizeRect();
        if (rect.left <= iMouseX && rect.right >= iMouseX)
        {
            if (rect.top <= iMouseY && rect.bottom >= iMouseY)
            {
                is_draw_tooltip = true;
            }
        }
    }

#ifdef _NA_000000_20130114_RENEWER_UI
#else
    if (this->GetManager()->GetShowFace() == true)
    {
        control_pointer = getControl(DLGPOS_FACE_BK);
        if (control_pointer)
        {
            rect = control_pointer->GetSizeRect();
            if (rect.left <= iMouseX && rect.right >= iMouseX)
            {
                if (rect.top <= iMouseY && rect.bottom >= iMouseY)
                {
                    is_draw_tooltip = true;
                }
            }
        }
    }
#endif

    if (this->GetManager()->GetShowGague() == true)
    {
        control_pointer = getControl(DLGPOS_MINE);
        if (control_pointer)
        {
            rect = control_pointer->GetSizeRect();
            if (rect.left <= iMouseX && rect.right >= iMouseX)
            {
                if (rect.top <= iMouseY && rect.bottom >= iMouseY)
                {
                    is_draw_tooltip = true;
                }
            }
        }
    }

    if ((is_draw_tooltip == true) &&
        (mouse_over_ == true))
    {
        uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
        if (tooltip_manager && (GetMyMemberInfo().map_code() != 0))
        {
            // 현재지역
            const sMAPINFO* current_map_info =
                MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(GetMyMemberInfo().map_code()));
            if (current_map_info != NULL)
            {
                TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
                g_InterfaceManager.GetInterfaceString(current_map_info->dwNCode, message);
                tooltip_manager->RegisterTooltipSimpleText(message);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());
            }
        }
    }
    else
    {
        mouse_over_ = false;
    }
#endif //_DH_BATTLEZONE2_

    //this->renderStringInfo();
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::SetHP( float fHPPercent )
{
    CCtrlStaticWZ* ctlHp = (CCtrlStaticWZ*)getControl(DLGPOS_HPBAR);
    assert( ctlHp );

    if( !ctlHp )
    {
        return;
    }

    fHPPercent = max( 0.0f, min( 100.0f, fHPPercent ) );

    RECT rc = ctlHp->GetSize();
    float width = (m_fGaugeSize / 100.f) * fHPPercent;

    ctlHp->MoveWindowWZ( (float)rc.left, (float)rc.top, width, (float)rc.bottom );

    CCtrlStaticWZ* ctlHpBk = (CCtrlStaticWZ*)getControl(DLGPOS_HPBAR_BK);
    assert( ctlHpBk );

    if( !ctlHpBk )
    {
        return;
    }

    ctlHpBk->MoveWindowWZ( (float)rc.left, (float)rc.top, width, (float)rc.bottom );

    if( fHPPercent < 30.0f )
    {
        ctlHp->ShowWindowWZ( WZ_HIDE );
        DWORD curTick = clock_function::GetTickCount();
        m_bHpBlinkType = true;
        if (this->GetManager() && this->GetManager()->GetShowGague() != true)
        {
            return;
        }
        else
        {
            // 0.06초마다 깜빡임
            if( curTick - m_blinkTick > 60 )
            {
                m_blinkTick = curTick;

                ENUM_STATEWZ st = ( ctlHpBk->GetState() == WZ_SHOW ? WZ_HIDE : WZ_SHOW );
                ctlHpBk->ShowWindowWZ( st );
            }
        }
    }
    else
    {
        if (m_bHpBlinkType == true && this->GetManager())
        {
            m_bHpBlinkType = false;
		    if(this->GetManager()->GetShowGague() != true)
		    {
			    ctlHp->ShowWindowWZ(WZ_HIDE);
		    }
            else
            {
                ctlHp->ShowWindowWZ(WZ_SHOW);
            }
            ctlHpBk->ShowWindowWZ(WZ_HIDE);
        }
    }

}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::SetMP( float fMPPercent )
{
    CCtrlStaticWZ* ctlMp = (CCtrlStaticWZ*)getControl(DLGPOS_MPBAR);
    assert( ctlMp );

    if( !ctlMp )
    {
        return;
    }

    fMPPercent = max( 0.0f, min( 100.0f, fMPPercent ) );

    RECT rc = ctlMp->GetSize();
    float width = (m_fMPGaugeSize / 100.f) * fMPPercent;

    ctlMp->MoveWindowWZ( (float)rc.left, (float)rc.top, width, (float)rc.bottom );
}

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
//------------------------------------------------------------------------------
/**
*/
void
uiPartyMember::SetSD( float fMPPercent )
{
    // 현재 파티멤버ui에 SD를 표시하지 않음
}
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
//------------------------------------------------------------------------------
/**
*/
void uiPartyMember::ShowGagueControl(bool visibility)
{
    CCtrlStaticWZ* ctlMp = (CCtrlStaticWZ*)getControl(DLGPOS_MPBAR);
    if (ctlMp != NULL)
    {
	    if(visibility == true)
	    {
		    ctlMp->ShowWindowWZ(WZ_SHOW);
	    }
	    else
	    {
		    ctlMp->ShowWindowWZ(WZ_HIDE);
	    }
    }

    CCtrlStaticWZ* ctlHp = (CCtrlStaticWZ*)getControl(DLGPOS_HPBAR);
	if (ctlHp != NULL)
	{
		if (visibility == true)
		{
			ctlHp->ShowWindowWZ(WZ_SHOW);
		}
		else
		{
			ctlHp->ShowWindowWZ(WZ_HIDE);
		}
	}

    CCtrlStaticWZ* ctlHpBk = (CCtrlStaticWZ*)getControl(DLGPOS_HPBAR_BK);
    if (ctlHpBk != NULL)
    {
        ctlHpBk->ShowWindowWZ(WZ_HIDE);
    }

    if (m_ctrlGaugeBackground != NULL)
    {
        if (visibility == true)
        {
            m_ctrlGaugeBackground->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            m_ctrlGaugeBackground->ShowWindowWZ(WZ_HIDE);
        }
    }
}

void uiPartyMember::ShowFaceControl(bool visibility)
{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	CControlWZ* pControl = getControl(DLGPOS_FACE_BK);
	if(pControl)
	{
		if(this->GetManager()->GetShowFace() == true)
		{
			pControl->ShowWindowWZ(WZ_SHOW);
		}
		else
		{
			pControl->ShowWindowWZ(WZ_HIDE);
		}
	}
#endif

#ifdef _NA_000000_20130114_RENEWER_UI
#else
	pControl = getControl(DLGPOS_LV_BK);
	if(pControl)
	{
		if(this->GetManager()->GetShowFace() == true)
		{
			pControl->ShowWindowWZ(WZ_SHOW);
		}
		else
		{
			pControl->ShowWindowWZ(WZ_HIDE);
		}
	}
#endif

    // 정리좀하자
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    //클래스로 파티창의 마크 설정
    CControlWZ* ctrl_pointer;
    const uicPartyMemberInfoPlus& member_info = this->GetMyMemberInfo();
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    for (int i=DLGPOS_CLASS_MARK00; i<(DLGPOS_CLASS_MARK07+1); ++i) 
#else
    for (int i=DLGPOS_CLASS_MARK00; i<(DLGPOS_CLASS_MARK06+1); ++i) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
    {
        ctrl_pointer = getControl(i);
        if (ctrl_pointer)
        {
            ctrl_pointer->ShowWindowWZ(WZ_HIDE);
        }
    }
    ctrl_pointer = getControl(DLGPOS_CLASS_MARK00 + (member_info.Get().m_byClassType - 1));
    if (ctrl_pointer)
    {
        ctrl_pointer->ShowWindowWZ(WZ_SHOW);
    }
#else //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    //클래스로 파티창의 마크 설정
    CControlWZ* ctrl_pointer;
    const uicPartyMemberInfoPlus& member_info = this->GetMyMemberInfo();
    for (int i=DLGPOS_CLASS_MARK00; i<(DLGPOS_CLASS_MARK05+1); ++i) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    {
        ctrl_pointer = getControl(i);
        if (ctrl_pointer)
        {
            ctrl_pointer->ShowWindowWZ(WZ_HIDE);
        }
    }
    ctrl_pointer = getControl(DLGPOS_CLASS_MARK00 + (member_info.Get().m_byClassType - 1));
    if (ctrl_pointer)
    {
        ctrl_pointer->ShowWindowWZ(WZ_SHOW);
    }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID

    //외형으로 파티창의 초상화 설정
    eCHAR_TYPE class_type    = static_cast<eCHAR_TYPE>(member_info.Get().m_byClassType);
    GenderType::Value gender = static_cast<GenderType::Value>(member_info.Get().m_byGender);
    eCHAR_TYPE image_type = ResourceCode::GetImageClassType(class_type ,gender);

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    for (int i=DLGPOS_FACE00; i < (DLGPOS_FACE07 + 1); ++i) 
#else
    for (int i=DLGPOS_FACE00; i < (DLGPOS_FACE06 + 1); ++i) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
    {
        ctrl_pointer = getControl(i);
        if (ctrl_pointer)
        {
            ctrl_pointer->ShowWindowWZ(WZ_HIDE);
        }
    }
    if ( image_type > eCHAR_NONE && image_type < eCHAR_TYPE_MAX)
    {
        ctrl_pointer = getControl(DLGPOS_FACE00 + (image_type - 1));
        if (ctrl_pointer != NULL)
        {
            ctrl_pointer->ShowWindowWZ(WZ_SHOW);
        }
    }
#else //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    for (int i=DLGPOS_FACE00; i < (DLGPOS_FACE05 + 1); ++i) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    {
        ctrl_pointer = getControl(i);
        if (ctrl_pointer)
        {
            ctrl_pointer->ShowWindowWZ(WZ_HIDE);
        }
    }
    if ( image_type > eCHAR_NONE && image_type < eCHAR_TYPE_MAX)
    {
        ctrl_pointer = getControl(DLGPOS_FACE00 + (image_type - 1));
        if (ctrl_pointer != NULL)
        {
            ctrl_pointer->ShowWindowWZ(WZ_SHOW);
        }
    }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID

    

}

