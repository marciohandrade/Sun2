#include "SunClientPrecompiledHeader.h"
#include "targetdialog.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Monster.h"
#include "InterfaceManager.h"
#include "IconImage.h"
#include "StateInfoParser.h"
#include "../Utility/Nstring.h"
#include "Hero.h"
#include "Mouse.h"
#include "globalfunc.h"
#include "uiCharPopupMenu/uiCharPopupMenu.h"
#include "uiCharPopupMan/uiCharPopupMan.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "HeroActionInput.h"
#include "GameFunc.h"
#include "uiPartyMan/uiPartyMan.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemData.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "Player.h"
#include "GuildMarkInfoParser.h"
#include "uiRadarMan/uiRadarMan.h"
#include "RadarFullDialog.h"
#include "BaseRadarDialog.h"
#include "GuildRightInfoParser.h"

#include "uiTargetMan/uiTargetMan.h"
#include "uiTargetZoneMarkDialog/uiTargetZoneMarkDialog.h"         
#include "StatusIconManager.h"
#include "uiTargetAggro/uiTargetAggro.h"         
#ifdef _NA_002935_20110704_ITEM_SCORE
#include "equipmentcontainer.h"
#endif //_NA_002935_20110704_ITEM_SCORE
#define UPDATE_HP_TIME 3000

#include "SkillMain.h"

WzID TargetDialog::m_wzId[] = 
{
	StrToWzID("P000"),
	StrToWzID("PI00"),

#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
#else
	StrToWzID("P101"),
#endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI
	StrToWzID("P102"),
	StrToWzID("P103"),
	StrToWzID("P104"),
	StrToWzID("P105"),
	StrToWzID("P106"),
	StrToWzID("P107"),
	StrToWzID("P108"),
	StrToWzID("P109"),
	StrToWzID("P110"),
#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("P111"),
	StrToWzID("P112"),
	StrToWzID("P113"),
	StrToWzID("P114"),
	StrToWzID("P115"),
	StrToWzID("P116"),
	StrToWzID("P117"),
	StrToWzID("P118"),
	StrToWzID("P119"),
  #ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
  #else
	StrToWzID("P120"),
	StrToWzID("P121"),
  #endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI
	StrToWzID("P122"),
	StrToWzID("P123"),
	StrToWzID("P124"),
	StrToWzID("P125"),
	StrToWzID("P126"),
	StrToWzID("P127"),
	StrToWzID("P128"),
	StrToWzID("P129"),
	StrToWzID("P130"),
	StrToWzID("P131"),
	StrToWzID("P132"),
	StrToWzID("P133"),
	StrToWzID("P134"),
	StrToWzID("P135"),
	StrToWzID("P136"),
	StrToWzID("P137"),
	StrToWzID("P138"),
	StrToWzID("P139"),
  #ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
  #else
	StrToWzID("P140"),
  #endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI
#endif

	StrToWzID("ST00"),
	StrToWzID("T002"),
	StrToWzID("PI05"),
	StrToWzID("PI04"),
	StrToWzID("PI03"),
	StrToWzID("PI02"),
	StrToWzID("PI01"),//은색
	StrToWzID("P011"),
	StrToWzID("TI04"),					//TARGET_PIC_GUILD_MARK
	StrToWzID("ST00"),

    StrToWzID("P011"),//FIRE
    StrToWzID("P012"),//WATER
    StrToWzID("P013"),//WIND
    StrToWzID("P014"),//EARTH
    StrToWzID("P015"),//DARK
    StrToWzID("P016"),//금색
    StrToWzID("B000"),//열기
    StrToWzID("B001"),//닫기

#ifdef _NA_000000_20130114_RENEWER_UI
	StrToWzID("S001"),  //TARGET_SCORE_BG
#else
#ifdef _NA_002935_20110704_ITEM_SCORE
    StrToWzID("P017"),  //TARGET_SCORE_BG        // 장비평점
    StrToWzID("P018"),  //TARGET_SCORE_NUMBER1
    StrToWzID("P057"),  //TARGET_SCORE_NUMBER2
    StrToWzID("P058"),  //TARGET_SCORE_NUMBER3
    StrToWzID("P059"),  //TARGET_SCORE_NUMBER4
    StrToWzID("P060"),  //TARGET_SCORE_NUMBER5
#endif //_NA_002935_20110704_ITEM_SCORE
#endif

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    StrToWzID("P003"),  //TARGET_PIC_SD_GAUGE, // 쉴드 게이지
    StrToWzID("S002"),  //TARGET_TEXT_SD_INFO, // 쉴드 스트링
    StrToWzID("P002"),  //TARGET_PIC_SECOND_GAUGE, // 밑 게이지
    StrToWzID("P001"),  //TARGET_PIC_SECOND_GAUGE_BACK, // 밑 게이지 배경
    StrToWzID("S000"),  //TARGET_TEXT_SECOND_HP_INFO, // 밑 스트링
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
    StrToWzID("B003"),  //TARGET_BUTTON_SUMMON_CANCLE, // 소환수 취소(역소환)
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
};

TargetDialog::TargetDialog(InterfaceManager *pUIMan):
	uiBase(pUIMan)
{
}


TargetDialog::~TargetDialog(void)
{
}


VOID TargetDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	m_dwPrevTargetKEY = 0;
	m_dwTargetKEY = 0;

    m_fGaugeSize = (float)getControl(TARGET_PIC_GAUGE)->GetSize().right;

	m_fGaugeBackSize= (float)getControl(TARGET_PIC_GAUGE_BACK)->GetSize().right;

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    SetHP(100 , false);
    SetSD(100.0f);
#else
    SetHP( 100 ,1.0f);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

	m_iHpRequestTime=UPDATE_HP_TIME;

	status_icon_container_.reserve(10);

	CCtrlStaticWZ * pStatic =	(CCtrlStaticWZ *)getControl(TARGET_TEXT_MONSTER_INFO);
	if( pStatic )
	{
		pStatic->ShowWindowWZ(WZ_HIDE);
	}

	CCtrlButtonWZ * pButton =  (CCtrlButtonWZ *)getControl(TARGET_PIC_GUILD_MARK);
	if( pButton )
	{
		pButton->ShowWindowWZ(WZ_HIDE);
	}

#ifdef _SCJ_LONG_TEXT_REDUCING
    CCtrlStaticWZ* static_control = (CCtrlStaticWZ*)getControl(TARGET_TEXT_INFO);
    if ( static_control != NULL )
    {
        static_control->SetTextReducing(true);
    }
#endif //_SCJ_LONG_TEXT_REDUCING

    is_show_aggro_ = false;

    int i = 0;
    for (i = kElement_Color_Fire; i < kElement_Color_Max; ++i)
    {
        picture_element_frames_[i] = static_cast<CCtrlPictureWZ*>(getControl(TARGET_PICTURE_FIRE + i));
        if (picture_element_frames_[i])
        {
            picture_element_frames_[i]->ShowWindowWZ(WZ_HIDE);
        }
        else
        {
            assert(picture_element_frames_[i]);
        }
    }
    CCtrlPictureWZ* picture_element_frames_old = static_cast<CCtrlPictureWZ*>(getControl(TARGET_PIC_BER));
    if (picture_element_frames_old)
    {
        picture_element_frames_old->ShowWindowWZ(WZ_HIDE);
        picture_element_frames_[kElement_Color_Max-1] = picture_element_frames_old;
    }

    for (i = kShow_Value_Open; i < kShow_Value_Max; ++i)
    {
        show_aggro_window_buttons_[i] = static_cast<CCtrlButtonWZ*>(getControl(TARGET_BUTTON_OPEN + i));
        if (show_aggro_window_buttons_[i])
        {
            show_aggro_window_buttons_[i]->ShowWindowWZ(WZ_HIDE);
        }
        else
        {
            assert(show_aggro_window_buttons_[i]);
        }
    }

#ifdef _NA_000000_20130114_RENEWER_UI
#else
	#ifdef _NA_002935_20110704_ITEM_SCORE
		CCtrlPictureWZ* picture_control;
		for (int index = TARGET_SCORE_START; index < TARGET_SCORE_END; ++index)
		{
			picture_control = static_cast<CCtrlPictureWZ*>(getControl(index));
			if (picture_control)
			{
				picture_control->ShowWindowWZ(WZ_HIDE);
			}
		}
	#endif //_NA_002935_20110704_ITEM_SCORE
#endif
}


VOID TargetDialog::Release()
{
}

VOID TargetDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch (pMessage->eResultMsg)
	{
	
#ifdef _KIKI_20080418_MOUSECLICK_BUG_FIX	// 엔진이 수정되면 CLICK으로 처리해도 됨
	case RT_MSG_RBUTTONCLICK:
		{
			_showCharPopupMenu(TRUE);
		}
#else
	case RT_MSG_RBUTTONDOWN: // 클릭시 문제가 있어서 DOWN으로 처리
		{
			_showCharPopupMenu(TRUE);
		}
		break;
#endif
    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE control_postion = getCtrlIDToPos(pMessage->dwCtrlID);
            switch (control_postion)
            {
            case TARGET_BUTTON_OPEN://열기
                {                    
                    is_show_aggro_ = true;
                    show_aggro_window_buttons_[kShow_Value_Open]->ShowWindowWZ(WZ_HIDE);
                    show_aggro_window_buttons_[kShow_Value_Close]->ShowWindowWZ(WZ_SHOW);
                    ShowAggroDialog();//열기 버튼은 몬스터 일때만 나타난다.
                }
                break;
            case TARGET_BUTTON_CLOSE://닫기
                {
                    is_show_aggro_ = false;
                    show_aggro_window_buttons_[kShow_Value_Close]->ShowWindowWZ(WZ_HIDE);
                    show_aggro_window_buttons_[kShow_Value_Open]->ShowWindowWZ(WZ_SHOW);
                    ShowAggroDialog();
                }
                break;
#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
            case TARGET_BUTTON_SUMMON_CANCLE:    // 소환수 취소(역소환)
                {
                    MSG_CG_SUMMON_COMMAND_SYN	Packet;
                    Packet.m_dwTargetKey = GetTargetKey();  // 타겟키에 오브젝트ID를 넣으면 해당 ID만 역소환
                    Packet.m_byCommand = eSUMMON_COMMAND_DESTROY;

                    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &Packet, sizeof(Packet));
                }
                break;
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
            }
        }
        break;
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}


VOID TargetDialog::NetworkProc( MSG_BASE * pMsg )
{

}


CControlWZ * TargetDialog::getControl( POSTYPE AtPos )
{
	if( AtPos < Dialog_Max )
	{
		return GetControlWZ( m_wzId[AtPos] );
	}
	else
	{
		return NULL;
	}
}


POSTYPE TargetDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
VOID TargetDialog::SetHP(float hp_ratio ,bool is_player)
{
    hp_ratio = min(max(hp_ratio, 0.0f), 100.0f);

    float ho_width = m_fGaugeSize * (hp_ratio / 100.0f);
    if (is_player)
    {
        CControlWZ* control_pointer = NULL;
        for (int i = TARGET_SECOND_GAUGE_START; i <= TARGET_SECOND_GAUGE_END; ++i)
        {
            control_pointer = getControl(i);
            if (control_pointer)
            {
                control_pointer->ShowWindowWZ(WZ_SHOW);
            }
        }
        control_pointer = getControl(TARGET_TEXT_HP_INFO); // 체력 수치
        if (control_pointer)
        {
            control_pointer->ShowWindowWZ(WZ_HIDE);
        }
        control_pointer = getControl(TARGET_PIC_GAUGE); // 체력 게이지
        if (control_pointer)
        {
            control_pointer->ShowWindowWZ(WZ_HIDE);
        }

        control_pointer = getControl(TARGET_PIC_SECOND_GAUGE); // 밑 체력 게이지
        if (control_pointer)
        {
            RECT rtPicGauge = control_pointer->GetSizeRect();
            control_pointer->MoveWindowWZ(
                (float)rtPicGauge.left, 
                (float)rtPicGauge.top, 
                (float)ho_width, 
                (float)rtPicGauge.bottom - rtPicGauge.top);
        }
    }
    else
    {
        CControlWZ* control_pointer = NULL;
        for (int i = TARGET_SECOND_GAUGE_START; i <= TARGET_SECOND_GAUGE_END; ++i)
        {
            control_pointer = getControl(i);
            if (control_pointer)
            {
                control_pointer->ShowWindowWZ(WZ_HIDE);
            }
        }
        control_pointer = getControl(TARGET_TEXT_HP_INFO); // 체력 수치
        if (control_pointer)
        {
            control_pointer->ShowWindowWZ(WZ_SHOW);
        }
        control_pointer = getControl(TARGET_PIC_GAUGE); // 체력 게이지
        if (control_pointer)
        {
            control_pointer->ShowWindowWZ(WZ_SHOW);
            RECT rtPicGauge = control_pointer->GetSizeRect();
            control_pointer->MoveWindowWZ(
                (float)rtPicGauge.left, 
                (float)rtPicGauge.top, 
                (float)ho_width, 
                (float)rtPicGauge.bottom - rtPicGauge.top);
        }
    }
}
VOID TargetDialog::SetSD(float sd_ratio)
{
    if (100.f < sd_ratio)
    {
        sd_ratio = 100.f;
    }

    if (sd_ratio >= 0.0f)
    {
        float sd_width = m_fGaugeSize * (sd_ratio / 100.0f);
        RECT rect_gauge = getControl(TARGET_PIC_SD_GAUGE)->GetSizeRect();

        getControl(TARGET_PIC_SD_GAUGE)->MoveWindowWZ( 
            static_cast<float>(rect_gauge.left), 
            static_cast<float>(rect_gauge.top), 
            static_cast<float>(sd_width), 
            static_cast<float>(rect_gauge.bottom - rect_gauge.top));

        CCtrlStaticWZ* control_pointer = static_cast<CCtrlStaticWZ*>(getControl(TARGET_TEXT_SD_INFO));
        if (control_pointer)
        {
            TCHAR sd_info_string[INTERFACE_STRING_LENGTH];
            Sprintf(sd_info_string, _T("%.0f%%"), sd_ratio);
            GlobalFunc::SetCaption(control_pointer, sd_info_string);
            control_pointer->SetTextColor(RGBA(255, 255, 255, 255));
        }
    }

}
#else //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
VOID TargetDialog::SetHP( float fHPPercent ,float fRadio)
{
    if(100.f < fHPPercent)
    {
        WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "[Warning]TargetDialog::HPPercent = %f", fHPPercent);
        fHPPercent = 100.f;
    }

    if (fHPPercent >= 0.0f )
    {
        float fHpWidth = m_fGaugeSize *(fHPPercent/100.0f);
        RECT rtPicGauge = getControl(TARGET_PIC_GAUGE)->GetSizeRect();

        getControl(TARGET_PIC_GAUGE)->MoveWindowWZ( 
            (float)rtPicGauge.left, 
            (float)rtPicGauge.top, 
            (float)fHpWidth, 
            (float)rtPicGauge.bottom - rtPicGauge.top);
    }
}
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
//=====================================================================================
//	DESC: 파티원의 타켓정보를 셋팅
//=====================================================================================
BOOL TargetDialog::SetPartyMember()
{
	uiPartyMan* partyMan  = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY );
	assert(partyMan);
	
	//해당키가파티멤버가 아니라면 리턴해버린다.
	if(partyMan->GetPartyMemberInfo(m_dwTargetKEY)==FALSE)
        return FALSE;
	
	_targetChange(TRUE); 
	
	//0.파티원의 정보얻기
	_PARTY_MEMBER_INFO rMember = partyMan->GetPartyMemberInfo(m_dwTargetKEY)->Get();
			
	//1.파티원이름,레벨셋팅
	Sprintf(m_szResultInfo, _T("%ld %s"), rMember.m_wLevel,rMember.m_szCharName);		
	Sprintf(m_szResultHpInfo, _T("%ld%%"),(DWORD)rMember.m_byHPRatio);
	float fHPPercent = (float)rMember.m_byHPRatio;
	
	m_dwTargetName = util::_tstring(rMember.m_szCharName);
	
	//2.다이얼로그에 업데이트
	CCtrlStaticWZ* pStaticControl = (CCtrlStaticWZ*)getControl(TARGET_TEXT_INFO); // 아이디
	assert(pStaticControl);
    pStaticControl->SetTextColor(RGBA(189, 198, 198, 255));
    GlobalFunc::SetCaption(pStaticControl, m_szResultInfo);
    pStaticControl->ShowWindowWZ(WZ_SHOW);

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    SetHP(fHPPercent, true);
    SetSD((float)rMember.m_bySDRatio); //(float)rMember.m_byHPRatio
    
    CCtrlStaticWZ* control_pointer = static_cast<CCtrlStaticWZ*>(getControl(TARGET_TEXT_SECOND_HP_INFO));
    if (control_pointer)
    {
        GlobalFunc::SetCaption(control_pointer, m_szResultHpInfo);
        control_pointer->SetTextColor(RGBA(255, 255, 255, 255));
    }
#else //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    SetHP(fHPPercent, 90.0f/100.0f);
    CCtrlStaticWZ* pHpStaticControl = (CCtrlStaticWZ*)getControl(TARGET_TEXT_HP_INFO); // 체력
    assert(pHpStaticControl);
    pHpStaticControl->SetTextColor(RGBA(255, 255, 255, 255));
    GlobalFunc::SetCaption(pHpStaticControl, m_szResultHpInfo);
    pHpStaticControl->ShowWindowWZ(WZ_SHOW);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    
    CCtrlPictureWZ* picture_control = static_cast<CCtrlPictureWZ*>(getControl(TARGET_PIC_BER));
    if (picture_control)
    {
        picture_control->ShowWindowWZ(WZ_SHOW);
    }

	return TRUE;
		
}

//=====================================================================================
//	DESC: 타켓팅리셋(중복구문 분리)
//=====================================================================================
void TargetDialog::_targetChange(BOOL bPlayer)
{
	if (m_dwPrevTargetKEY != m_dwTargetKEY)
	{
		m_dwPrevTargetKEY = m_dwTargetKEY;
		g_InterfaceManager.UpdateTargetHeadTexture();

		if (bPlayer)
		{
			// 마을일때 Player 타겟이 바꼈으면 정보 요청
			if (GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE||
				GameFramework::GetCurScene() == SCENE_TYPE_FIELD)
			{
				MSG_CG_CHARINFO_SELECT_INFO_SYN msg;
				msg.m_dwObjectKey = m_dwTargetKEY;
				GlobalFunc::SendPacket( CI_GAMESERVERIDX, &msg, sizeof(msg) );
                _showCharPopupMenu(FALSE);
			}
		}
	}

}
//=====================================================================================
//	DESC: 타켓정보셋팅
//=====================================================================================
VOID TargetDialog::SetTargetInfo()
{
    uiTargetZoneMarkDialog* target_zonemark_dialog = GET_CAST_DIALOG(uiTargetZoneMarkDialog, IM_TARGET_MANAGER::TARGET_ZONEMARK_DIALOG);
    uiTargetAggro* target_aggro_dialog = GET_CAST_DIALOG(uiTargetAggro, IM_TARGET_MANAGER::TARGET_AGGRO);

	Object * pTargetObject = g_ObjectManager.GetObject(m_dwTargetKEY);

	if(!pTargetObject)
	{
		if(SetPartyMember()) 
		{
			return;
		}
	}

    for (int i = 0; i < kElement_Color_Max; ++i)
    {
        picture_element_frames_[i]->ShowWindowWZ(WZ_HIDE);
    }
    for (int i = kShow_Value_Open; i < kShow_Value_Max; ++i)
    {
        show_aggro_window_buttons_[i]->ShowWindowWZ(WZ_HIDE);
    }

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
    if (getControl(TARGET_BUTTON_SUMMON_CANCLE))
    {
        getControl(TARGET_BUTTON_SUMMON_CANCLE)->ShowWindowWZ(WZ_HIDE);
    }
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL

	CCtrlStaticWZ* pStaticControl = (CCtrlStaticWZ *)getControl(TARGET_TEXT_INFO); // 이름
	assert(pStaticControl);

    CCtrlStaticWZ* pHpStaticControl = (CCtrlStaticWZ *)getControl(TARGET_TEXT_HP_INFO); // 체력
    assert(pHpStaticControl);

	if (pTargetObject && g_pHero)
	{
        picture_element_frames_[kElement_Color_Common]->ShowWindowWZ(WZ_SHOW);
		_targetChange(pTargetObject->IsKindOfObject(PLAYER_OBJECT));

		if (pTargetObject->IsKindOfObject(PLAYER_OBJECT))
		{
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
            pHpStaticControl = (CCtrlStaticWZ *)getControl(TARGET_TEXT_SECOND_HP_INFO); // 밑 체력
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

			Player * pPlayer = static_cast<Player *>(pTargetObject);

			DWORD maxhp = pPlayer->GetMaxHP();

			if(maxhp==0)
				maxhp=1;

			DWORD curhp = pPlayer->GetHP();


            BASE_PLAYERINFO *bp = pPlayer->GetCharInfo();

			m_dwTargetName = util::_tstring(bp->m_tszCharName);

			if( pPlayer->IsGM() )
			{
				//	GM 캐릭터는 이름만 출력
				Sprintf(m_szResultInfo, _T("%s"), bp->m_tszCharName );
                pStaticControl->SetTextColor(RGBA(189, 198, 198, 255));
				GlobalFunc::SetCaption( pStaticControl, m_szResultInfo );
				pHpStaticControl->SetTextWZ(_T(""));
			}
			else
			{
				Sprintf(m_szResultInfo, _T("%ld %s"), 
					pPlayer->GetLevel(), 
					bp->m_tszCharName
					);

				float percent=( (float)curhp / (float)maxhp ) * 100.0f;

                uiPartyMan* const party_man = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
                if ((party_man) &&
                    (m_dwTargetKEY != g_pHero->GetObjectKey()) &&
                    (party_man->IsPartyMember(m_dwTargetKEY) == true))
                {
                    _PARTY_MEMBER_INFO member_info = party_man->GetPartyMemberInfo(m_dwTargetKEY)->Get();
                    DWORD hp_ratio = static_cast<DWORD>(member_info.m_byHPRatio);
                    Sprintf(m_szResultHpInfo, _T("%ld%%"),hp_ratio);
                }
				else
				{
					Sprintf(m_szResultHpInfo, _T("%ld%%"),(DWORD)percent);
				}

                WzColor FinalColor=RGBA(189, 198, 198, 255);

				if( GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pPlayer, g_HeroInput.lsForceAttack() ) == RELATIONKIND::Enemy )
					FinalColor=RGBA( 255, 0, 0, 255 );
				
				if(pPlayer->GetChaoState()==eCHAO_STATE_PREV_CHAO)
					FinalColor=RGBA( 255, 128, 0, 255 );

				if(pPlayer->IsAttackBeginPlayer())
					FinalColor=RGBA( 128, 0, 255, 255 );

				if(pPlayer->GetChaoState()==eCHAO_STATE_CHAO)
					FinalColor=RGBA( 255, 0,0, 255 );

	
				pStaticControl->SetTextColor(FinalColor);
				GlobalFunc::SetCaption( pStaticControl, m_szResultInfo );

				pHpStaticControl->SetTextColor(RGBA(255,255,255,255));
				GlobalFunc::SetCaption( pHpStaticControl, m_szResultHpInfo );
			}

            assert(maxhp > 0);
            if (curhp > maxhp)
                curhp = maxhp;

            uiPartyMan* const party_man = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
			if ((party_man) &&
                (m_dwTargetKEY != g_pHero->GetObjectKey()) &&
                (party_man->IsPartyMember(m_dwTargetKEY) == true))
			{
                _PARTY_MEMBER_INFO member_info = party_man->GetPartyMemberInfo(m_dwTargetKEY)->Get();
				float hp_ratio = static_cast<float>(member_info.m_byHPRatio);

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
                SetHP(hp_ratio, true);
                SetSD(static_cast<float>(member_info.m_bySDRatio)); //member_info.m_byHPRatio
#else 
				SetHP(hp_ratio, 90.0f/100.0f);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
			}
			else
			{
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
                SetHP( ((float)curhp / (float)maxhp) * 100.0f, true);
                if (pPlayer->HasSDInfo() == FALSE)
                {
                    SetSD(100.0f);
                }
                else if (pPlayer->GetMaxSD() != 0)
                {
                    SetSD((float)pPlayer->GetSD() / (float)pPlayer->GetMaxSD() * 100);
                }
                else
                {
                    SetSD(0.0f);
                }
#else
				this->SetHP( ( (float)curhp / (float)maxhp ) * 100.0f,90.0f/100.0f);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
			}

			if (pPlayer->HasHPInfo())
			{
				pStaticControl->ShowWindowWZ(WZ_SHOW);
				pHpStaticControl->ShowWindowWZ(WZ_SHOW);
			}
			else
			{
				pStaticControl->ShowWindowWZ(WZ_HIDE);
				pHpStaticControl->ShowWindowWZ(WZ_HIDE);
			}
            if (target_aggro_dialog && target_aggro_dialog->IsVisible() == TRUE)
            {
                target_aggro_dialog->ShowInterface(FALSE);
            }  
            if (target_zonemark_dialog)
            {
#ifdef _NA_008334_20150608_SONNENSCHEIN
                if (GlobalFunc::HideSonnenScheinnBuffScene() == TRUE)
                {
                    // 소넨샤인 버프 가리는 분기인데 점령전 버프 숫자까지 가리려고 여기로 옴
                    if (target_zonemark_dialog->IsVisible() == TRUE) 
                    {
                        target_zonemark_dialog->ShowInterface(FALSE);
                    }
                }
                else
#else
  #ifdef _NA_000000_20120527_CHAOS_ZONE
    #ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                if (GameFramework::GetCurScene() &&
                    ((GameFramework::GetCurScene() == SCENE_TYPE_FREEPVP) ||
                    (GameFramework::GetCurScene() == SCENE_TYPE_BATTLEGROUND) ||
                    (GameFramework::GetCurScene() == SCENE_TYPE_GOLDRUSH)))                    
    #else
                if (GameFramework::GetCurScene() &&
                    ((GameFramework::GetCurScene() == SCENE_TYPE_FREEPVP) ||
                    (GameFramework::GetCurScene() == SCENE_TYPE_BATTLEGROUND)))
    #endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                {
                    // PVP룸 or 전장에서 마크 숨김
                    if (target_zonemark_dialog->IsVisible() == TRUE) 
                    {
                        target_zonemark_dialog->ShowInterface(FALSE);
                    }
                }
                else
  #endif //_NA_000000_20120527_CHAOS_ZONE
#endif //_NA_008334_20150608_SONNENSCHEIN
                {
                    size_t size_of_Info = pPlayer->GetDominationStateInfo().size();

#ifdef _NA_008334_20150608_SONNENSCHEIN
                    BYTE sonnenschein_level = 0;
                    if (pPlayer->IsHero())
                    {
                        sonnenschein_level = GuildSystem::GetGuildSystemData().get_sonnenschein_phase();
                    }
                    else
                    {
                        if (GlobalFunc::HideSonnenScheinnBuffScene() == FALSE)
                        {
                            sonnenschein_level = pPlayer->GetGuildRenderInfo().sonnenschein_phase;
                        }
                    }
                    
                    size_of_Info += sonnenschein_level;
#endif //_NA_008334_20150608_SONNENSCHEIN

                    if (size_of_Info > 0)
                    {
#ifdef _NA_008334_20150608_SONNENSCHEIN
                        target_zonemark_dialog->SetZoneMarkInfo(pPlayer->GetDominationStateInfo(), sonnenschein_level);
#else
                        target_zonemark_dialog->SetZoneMarkInfo(pPlayer->GetDominationStateInfo());
#endif //_NA_008334_20150608_SONNENSCHEIN
                    
                        if (target_zonemark_dialog->IsVisible() == FALSE) 
                        {
                            target_zonemark_dialog->ShowInterface(TRUE);
                        }
                    }

                    else
                    {
                        if (target_zonemark_dialog->IsVisible() == TRUE) 
                        {
                            target_zonemark_dialog->ShowInterface(FALSE);
                        }
                    }
                }
            }            
		}
		else if (pTargetObject->IsKindOfObject(MONSTER_OBJECT))
		{
            if (target_zonemark_dialog && target_zonemark_dialog->IsVisible() == TRUE)
            {
                target_zonemark_dialog->ShowInterface(FALSE);
            }            

			Monster * pMonster = static_cast<Monster *>(pTargetObject);
			DWORD maxhp = pMonster->GetMaxHP();
			if(maxhp==0)
				maxhp=1;
			DWORD curhp = pMonster->GetHP();
			TCHAR		szMessage[MAX_MESSAGE_LENGTH];
			int         stringId=0;

			float fRadio=1.0f;


#ifdef _NA_007113_SHADOW_PATCH_UNTARGET_MONSTER_DIE
            if( xGetSkillData()->GetSkillDataBySkillClassCode( eSKILL_SOUL_CONTROL ) == NULL )
            {
                if (0 == curhp)
                {
                    // 아직 정의된것이없다.
                    SolarDialog::ShowWindow(FALSE);
                }
            }

#else // _NA_007113_SHADOW_PATCH_UNTARGET_MONSTER_DIE
            if (0 == curhp)
            {
                // 아직 정의된것이없다.
                SolarDialog::ShowWindow(FALSE);
            }
#endif // _NA_007113_SHADOW_PATCH_UNTARGET_MONSTER_DIE

			nString str;

		    switch(pMonster->GetMonsterInfo()->m_byGrade)
			{

			case eNPC_SUMMON_NPC:
				{
					stringId=11501;
					fRadio=80.0f/100.0f;
				}
				break;

			case eNPC_MAPOBJECT_NPC:
				{
					stringId=11501;
					fRadio=80.0f/100.0f;
				}
				break;


			case eNPC_GENERAL:
				{
					stringId=11501;
					fRadio=80.0f/100.0f;
				}
				break;

			case eNPC_ELITE:
				{
					stringId=11503;
					fRadio=95.0f/100.0f;				
				}
				break;

			case eNPC_LEADER:
				{
					stringId=11502;
					fRadio=85.0f/100.0f;
				}
				break;

			case eNPC_MIDDLEBOSS:
				{
					stringId=11504;
					fRadio=95.0f/100.0f;
				}
				break;

			case eNPC_BOSS:
				{
					stringId=11505;
					fRadio=110.0f/100.0f;
				}
				break;

			case eNPC_LUCKY_MONSTER:
				{
					//11500	럭키
					stringId=11500;
					fRadio=85.0f/100.0f;
				}
				break;

			default:
				{
					stringId=11501;
					fRadio=1.0f;
				}
				break;
			}

			g_InterfaceManager.GetInterfaceString( stringId, szMessage, MAX_MESSAGE_LENGTH );
	

            if (pMonster->IsShadowSummon())
            {
                //몬스터이름재정의 :속박된 + 몬스터이름
                // 1724 속박된
                Sprintf(m_szResultInfo, _T("%s %ld %s %s"),
                    szMessage,
                    pMonster->GetMonsterInfo()->m_DisplayLevel, 
                    g_InterfaceManager.GetInterfaceString(1724),
                    pMonster->GetName()
                    );	
            }
            else
            {
                Sprintf(m_szResultInfo, _T("%s %ld %s"),
                    szMessage,
                    pMonster->GetMonsterInfo()->m_DisplayLevel, 
                    pMonster->GetName()
                    );	
            }

            if (pMonster->GetMonsterInfo()->m_byGrade !=  eNPC_SUMMON_NPC &&
                pMonster->GetMonsterInfo()->m_byGrade != eNPC_MAPOBJECT_NPC && !pMonster->IsShadowSummon())
            {
                if (is_show_aggro_)
                {
                    show_aggro_window_buttons_[kShow_Value_Close]->ShowWindowWZ(WZ_SHOW);
                    if (target_aggro_dialog && target_aggro_dialog->IsVisible() == FALSE)
                    {
                        target_aggro_dialog->ShowInterface(TRUE);
                    }                
                }
                else
                {
                    show_aggro_window_buttons_[kShow_Value_Open]->ShowWindowWZ(WZ_SHOW);
                    if (target_aggro_dialog && target_aggro_dialog->IsVisible() == TRUE)
                    {
                        target_aggro_dialog->ShowInterface(FALSE);
                    }
                }
            }
            else
            {
                if (target_aggro_dialog && target_aggro_dialog->IsVisible() == TRUE)
                {
                    target_aggro_dialog->ShowInterface(FALSE);
                }
            }

			if (pMonster->IsShadowSummon() &&
                (g_pHero->GetObjectKey() == pMonster->GetParentId()))
			{
                picture_element_frames_[kElement_Color_Common]->ShowWindowWZ(WZ_SHOW);
				pStaticControl->SetTextColor(RGBATOARGB(128, 128, 128, 255));

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
                if (getControl(TARGET_BUTTON_SUMMON_CANCLE))
                {
                    getControl(TARGET_BUTTON_SUMMON_CANCLE)->ShowWindowWZ(WZ_SHOW);
                }
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
			}
			else
			{
                if (pMonster)
                {
                    picture_element_frames_[pMonster->GetMyResist()]->ShowWindowWZ(WZ_SHOW);
                }
                pStaticControl->SetTextColor(GetColorText(pMonster));
			}

			m_dwTargetName = util::_tstring(pMonster->GetName());

			GlobalFunc::SetCaption(pStaticControl, m_szResultInfo);

			float percent = ((float)curhp / (float)maxhp) * 100.0f;
	
			Sprintf(m_szResultHpInfo, _T("%ld%%"), (DWORD)percent);

			pHpStaticControl->SetTextColor(RGBA(198,178,137,255));

			GlobalFunc::SetCaption( pHpStaticControl, m_szResultHpInfo );

            assert(maxhp > 0);
            if (curhp > maxhp)
                curhp = maxhp;

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
            SetHP( ( (float)curhp / (float)maxhp ) * 100.0f, false);
#else
           SetHP( ( (float)curhp / (float)maxhp ) * 100.0f,fRadio);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

        }
		else
		{
			SolarDialog::ShowWindow(FALSE);
		}

		pStaticControl->ShowWindowWZ(WZ_SHOW);
		pHpStaticControl->ShowWindowWZ(WZ_SHOW);
        picture_element_frames_[kElement_Color_Common]->ShowWindowWZ(WZ_SHOW);
	}
	else
	{
        if (target_zonemark_dialog && target_zonemark_dialog->IsVisible() == TRUE)
        {
            target_zonemark_dialog->ShowInterface(FALSE);
        }            
        if (target_aggro_dialog && target_aggro_dialog->IsVisible() == TRUE)
        {
            target_aggro_dialog->ShowInterface(FALSE);
        }  

		SolarDialog::ShowWindow(FALSE);

		m_dwPrevTargetKEY = 0;
	}

	
}

DWORD TargetDialog::GetColorText(Monster* monster_ptr)
{
    DWORD color;
    switch (monster_ptr->GetMyResist())
    {
    case NpcElementWeakness::eElementWeakness_Fire:
        color = RGBA(255, 29, 75, 255);
        break;
    case NpcElementWeakness::eElementWeakness_Water:
        color = RGBA(108, 165, 250, 255);
        break;
    case NpcElementWeakness::eElementWeakness_Wind:
        color = RGBA(79, 210, 226, 255);
        break;
    case NpcElementWeakness::eElementWeakness_Earth:
        color = RGBA(252, 176, 86, 255);
        break;
    case NpcElementWeakness::eElementWeakness_Dark:
        color = RGBA(171, 153, 209, 255);
        break;
    //case Monster::kElement_Common:
    default:
        color = RGBA(189, 198, 198, 255);
        break;
    }

    return color;
}

VOID TargetDialog::OnShowWindow( BOOL val )
{
	if (val)
	{
		if(false == CheckShowState())
		{
			SolarDialog::ShowWindow(FALSE);
			return;
		}
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Target;
		msg.DoSomething = GlobalFunc::DoTarget;

        // 타겟창은 맨 마지막에 닫히도록 함.
		g_KeyQueueManager.PushFront(msg);

		SetTargetInfo();

	}
	else
	{
        uiTargetZoneMarkDialog* target_zonemark_dialog = GET_CAST_DIALOG(uiTargetZoneMarkDialog, IM_TARGET_MANAGER::TARGET_ZONEMARK_DIALOG);
        if (target_zonemark_dialog)
        {
            target_zonemark_dialog->ShowInterface(FALSE);
        }            

        uiTargetAggro* target_aggro_dialog = GET_CAST_DIALOG(uiTargetAggro, IM_TARGET_MANAGER::TARGET_AGGRO);
        if (target_aggro_dialog && target_aggro_dialog->IsVisible())
        {
            target_aggro_dialog->ShowInterface(FALSE);
        }

		_showCharPopupMenu(FALSE);
		m_dwPrevTargetKEY = 0;
		g_KeyQueueManager.DeleteMsg(eDoType_Target);
	}
}

bool TargetDialog::CheckShowState()
{
	if(NULL == g_pHero)
		return false;


#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
	Object* pObj = g_ObjectManager.GetObject(m_dwTargetKEY);
	if(pObj && pObj->IsKindOfObject(MONSTER_OBJECT))
	{
		Monster* pMon = (Monster*)pObj;
		//eNPC_DOMINATION_MAPOBJECT_NPC일 경우 주인길드에게만 타겟창 출력
		if(eNPC_DOMINATION_MAPOBJECT_NPC == pMon->GetMonsterInfo()->m_byGrade)
		{
			if(g_pHero->GetPlayerGuildID() != pMon->GetMasterGuildID())
				return false;
		}
	}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

	return true;
}

VOID TargetDialog::SetTargetName(TCHAR* pszTargetName)
{
    Object* pObject = g_ObjectManager.GetObject(pszTargetName);

    // 타락한 사원내에서 타겟팅 명령어 사용되면 안됨
    if ((pObject != NULL) &&
        (GlobalFunc::IsSSQScene() != TRUE))
    {		
        if( pObject->GetSCObject().GetObjectType() == PLAYER_OBJECT)
        {
            g_HeroInput.SetCurrentTarget(pObject->GetObjectKey());
        }
    }
    else
    {
        TCHAR szMessage[INTERFACE_STRING_LENGTH];
        // 85143 타겟을 찾을 수 없습니다.
        g_InterfaceManager.GetInterfaceString(85143, szMessage, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
}

VOID TargetDialog::SetTargetKey(DWORD dwTargetKEY)
{
	m_dwTargetKEY = dwTargetKEY;
}

VOID TargetDialog::OnUpdateSolarDialog()
{
	SetTargetInfo();

#ifdef _NA_000000_20130114_RENEWER_UI
	UpdateItemScore();
#else
#ifdef _NA_002935_20110704_ITEM_SCORE
    UpdateItemScore();
#endif //_NA_002935_20110704_ITEM_SCORE
#endif
}

VOID TargetDialog::OnRenderSolarDialog()
{
	if( g_pHero == NULL )
	{
		return;
	}

	RadarFullDialog* pRFD = GET_CAST_DIALOG(RadarFullDialog, IM_RADAR_MANAGER::RADAR_DIALOG_FULL);
	if(pRFD)
	{
		if(eFULL_RADAR_STATE_ALL == pRFD->GetRadarState())
			return;
	}

	_renderGuildMark();

#ifdef _SCJ_LONG_TEXT_REDUCING
    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    RECT control_rect;  
    control_rect = getControl(TARGET_TEXT_INFO)->GetSizeRect();

    if ( control_rect.left <= mouse_x && control_rect.right >= mouse_x )
    {
        if ( control_rect.top <= mouse_y && control_rect.bottom >= mouse_y )
        {            
             uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
             if ( tooltip_manager != NULL )
             {
                TCHAR* result_string;
                result_string = GetTargetName();               
                tooltip_manager->RegisterTooltipSimpleText( result_string );
				tooltip_manager->SetTooltipDialogID(GetDialogKey());

             }
        }
    }
#endif //_SCJ_LONG_TEXT_REDUCING

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
    if (getControl(TARGET_BUTTON_SUMMON_CANCLE) &&
        (getControl(TARGET_BUTTON_SUMMON_CANCLE)->IsState(WZ_SHOW) == true))
    {
        int mouse_x = Mouse::Instance()->GetMouseX();
        int mouse_y = Mouse::Instance()->GetMouseY();

        RECT control_rect = getControl(TARGET_BUTTON_SUMMON_CANCLE)->GetSizeRect();

        if ( control_rect.left <= mouse_x && control_rect.right >= mouse_x )
        {
            if ( control_rect.top <= mouse_y && control_rect.bottom >= mouse_y )
            {            
                uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
                if ( tooltip_manager != NULL )
                {
                    TCHAR string_[INTERFACE_STRING_LENGTH + 1] = {0,};
                    g_InterfaceManager.GetInterfaceString(11263, string_);
                    tooltip_manager->RegisterTooltipSimpleText(string_);
                    tooltip_manager->SetTooltipDialogID(GetDialogKey());
                }
            }
        }
    }
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL

#ifdef _NA_000000_20130114_RENEWER_UI
	DrawItemScore();
#else
#ifdef _NA_002935_20110704_ITEM_SCORE
     DrawItemScore();
#endif //_NA_002935_20110704_ITEM_SCORE
#endif
}

static DWORD GetIconIDForStatus(DWORD dwStatus)
{
	

	BASE_STATEINFO *pInfo = StateInfoParser::Instance()->GetStateInfo(dwStatus);
	if (pInfo)
	{
		return pInfo->m_dwIconCode;
	}
	else
	{
		return 0xFFFFFFFF;
	}


}

void TargetDialog::UpdateStatusIcon()
{
    if (IsVisible() == FALSE)
    {
        return;
    }

    status_icon_container_.clear();

    Character* target_character = static_cast<Character*>(g_ObjectManager.GetObject(m_dwTargetKEY));
    if ((target_character == NULL) ||
        (target_character->IsKindOfObject(CHARACTER_OBJECT) != TRUE))
    {
        return;
    }

    int mouse_x_position = Mouse::Instance()->GetMouseX();
    int mouse_y_position = Mouse::Instance()->GetMouseY();

    // 타겟 의 SKILL_EFFECT 를 순회
    BASE_STATEINFO* state_info = NULL;
    SKILL_EFFECT_IMAGE status_icon;
    DWORD hero_object_key = m_dwTargetKEY;
    int status_Index = 0;
    CControlWZ* buff_control = NULL;

    std::vector<SKILL_EFFECT*>& skill_effect_array = target_character->GetSkillEffectArray();
    for( size_t i=0;i!=skill_effect_array.size();++i)
    {
        SKILL_EFFECT* skill_effect = skill_effect_array[i];

        state_info = StateInfoParser::Instance()->GetStateInfo(skill_effect->dwStatusID);
        status_icon.iEffectID = skill_effect->iEffectID;
        status_icon.dwStatusID = skill_effect->dwStatusID;
        status_icon.dwOwnerID = hero_object_key;
        status_icon.pSkillEffect = skill_effect;
        status_icon.pStateInfo = state_info;

		if(status_Index < 20)
		{
			status_icon.m_byAlpha = 0xFF;
		}
		else
		{
			status_icon.m_byAlpha = StatusIconManager::ALPHA_VALUE_MOUSE_OVER_ICON;
		}

        // draw_rect 계산
#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
        buff_control = getControl(TARGET_SKILL_EFFECT2 + status_Index);
#else
        buff_control = getControl(TARGET_SKILL_EFFECT1 + status_Index);
#endif //_NA_008139_20150318_MODIFY_BUFF_ICON_UI
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
        if (status_Index >= StatusIconManager::MAX_TARGET_ICON)
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

VOID 
TargetDialog::Process(DWORD dwTick)
{
    if (GameFramework::GetCurrentScene() &&
        (GameFramework::GetCurrentScene()->IsEnablePkScene() ||
        GameFramework::GetCurrentScene()->InPVPScene()))
	{
		m_iHpRequestTime-=dwTick;

		if(m_iHpRequestTime<=0)
		{
			m_iHpRequestTime=UPDATE_HP_TIME;
			Object * pTargetObject = g_ObjectManager.GetObject(m_dwTargetKEY);
			if (pTargetObject)
			{
				if (pTargetObject!=g_pHero&&pTargetObject->IsKindOfObject(PLAYER_OBJECT))
				{
					MSG_CG_CHARINFO_SELECT_INFO_SYN msg;
					msg.m_dwObjectKey = m_dwTargetKEY;
					GlobalFunc::SendPacket( CI_GAMESERVERIDX, &msg, sizeof(msg) );
				}
			}
		}
	}

    static int monster_info_time = 0;
    static DWORD pre_target_monster_key = 0;
    Object * pTargetObject = g_ObjectManager.GetObject(m_dwTargetKEY);

    if (pTargetObject)
    {
        if (monster_info_time >= 0)
        {
            monster_info_time -= dwTick;
        }

        if (pTargetObject->IsKindOfObject(MONSTER_OBJECT) && pre_target_monster_key != m_dwTargetKEY)
        {
            pre_target_monster_key = m_dwTargetKEY;
            monster_info_time = 0;//새 타겟이 몬스터이고 바뀐 경우 바로보냄
        }

        if (monster_info_time <= 0 && is_show_aggro_)
        {
            monster_info_time = 2000;//같은 타겟은 2초 딜레이
            if (pTargetObject->IsKindOfObject(MONSTER_OBJECT))  
            {
                MSG_CG_MONSTER_SELECT_INFO_SYN msg;
                msg.monster_key = m_dwTargetKEY;
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &msg, sizeof(msg));
            }
        }
    }
}

void TargetDialog::_showCharPopupMenu(BOOL bShow)
{
	uiBase* popupMenu = GET_DIALOG(IM_CHAR_POPUP_MANAGER::CHAR_POPUP_MENU);

	if(!popupMenu)
		return;

	if(bShow && g_pHero)
	{
		if(m_dwTargetKEY == g_pHero->GetObjectKey()) return;	// 내가 타겟이면 리턴

		Object	*pObj = g_ObjectManager.GetObject(m_dwTargetKEY);
		if( pObj )
		{
			if(!pObj->IsKindOfObject(PLAYER_OBJECT))			// 일반유저 아니면 리턴
				return;
		}

#ifdef _DH_SHOW_VENDOR_LIST_POPUP
        //popupMenu->ShowInterface(FALSE);//껏다켜기 초기화를 위해

        uiCharPopupMan* popup_menu_manager = GET_DIALOG_MANAGER(uiCharPopupMan, UIMAN_CHAR_POPUP);
        if (popup_menu_manager)
        {
            popup_menu_manager->set_is_vendor_popup(false);
            if (pObj)
            {
                popup_menu_manager->CheckPopupType(m_dwTargetKEY, pObj->GetName(), pObj->GetObjectType());
            }
            else
            {
                popup_menu_manager->CheckPopupType(m_dwTargetKEY);
            }
        }
#endif//_DH_SHOW_VENDOR_LIST_POPUP
		popupMenu->ShowInterface(TRUE);
	}
	else
	{
		popupMenu->ShowInterface(FALSE);
	}
}

void TargetDialog::_renderGuildMark()
{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    HANDLE hGuildMarkTexture = GuildSystem::GetGuildSystemData().GetGuildMarkTexture();
#else
    uiGuildMan * pGuildMan = (uiGuildMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
    if( !pGuildMan)
        return;

    HANDLE hGuildMarkTexture = pGuildMan->GetGuildMarkTexture();    
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    
    if( hGuildMarkTexture == INVALID_HANDLE_VALUE )
        return;

	Object * pObj = g_ObjectManager.GetObject(m_dwTargetKEY);

	if( pObj && pObj->IsKindOfObject(PLAYER_OBJECT) )
	{
		Player * pPlayer = (Player *)pObj;

		if( !pPlayer->IsExistGuild() )
			return;

		CCtrlButtonWZ * pButton =  (CCtrlButtonWZ *)getControl(TARGET_PIC_GUILD_MARK);
		if( pPlayer && pButton )
		{
			RECT rtSize = pButton->GetSizeRect();

			float fWidth = (float) rtSize.right - rtSize.left;
			float fHeight = (float) rtSize.bottom - rtSize.top;
			BASE_GuildMarkInfo * pMarkInfo = NULL;

			//길드마크가 없어도 관계 테두리는 그리자.
			BASE_GuildMarkInfo* pMarkInfo_Relation = NULL;
			switch(pPlayer->GetGuildRenderInfo().m_RelationType)
			{
			case eGUILD_RELATION_ALLIENCE:
				{
					//길드 등급만 체크 한다.(개인 직위는 체크 생략)
                    if (GuildRightInfoParser::Instance()->CheckGradeForRight(eGUILD_RIGHTS_RELATION, 
                        pPlayer->GetGuildRenderInfo().m_GuildGrade))
					{
						//3레벨 이상일 경우 무조건 출력
						pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_BLUE);
					}
					else
					{
						//3레벨 이하일 경우 길드마크가 있을때만 출력
						if( pPlayer->IsExistGuildMark() && g_pHero)
						{
							if((g_pHero->GetGuildRenderInfo().m_GuildGuid == pPlayer->GetGuildRenderInfo().m_GuildGuid)) //Hero와 같은 길드다
							{
								pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_BLUE);
							}
							else
							{
								pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_EMPTY);
							}
						}
					}
				}
				break;

			case eGUILD_RELATION_BOTH_HOSTILITY:
			case eGUILD_RELATION_PROCLAIM_HOSTILITY:
				{
					pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_RED);
				}
				break;
			case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
				{
					pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_ORANGE);
				}
				break;

			case eGUILD_RELATION_NONE:
			default:
				{
					if (g_pHero)
					{
						//길드 등급만 체크 한다.(개인 직위는 체크 생략)
                        if (GuildRightInfoParser::Instance()->CheckGradeForRight(eGUILD_RIGHTS_RELATION, 
                            pPlayer->GetGuildRenderInfo().m_GuildGrade))
						{
							if ((g_pHero->GetGuildRenderInfo().m_GuildGuid == pPlayer->GetGuildRenderInfo().m_GuildGuid)) //Hero와 같은 길드다
							{
								pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_BLUE);
							}
							else
							{
								pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_EMPTY);
							}
						}
						else
						{
							if( pPlayer->IsExistGuildMark() )
							{
								if((g_pHero->GetGuildRenderInfo().m_GuildGuid == pPlayer->GetGuildRenderInfo().m_GuildGuid)) //Hero와 같은 길드다
								{
									pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_BLUE);
								}
								else
								{
									pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_EMPTY);
								}
							}
						}
					}
				}
				break;
			}
			if(pMarkInfo_Relation)
			{
				g_pSunRenderer->RenderTexture(hGuildMarkTexture, (float)rtSize.left , (float)rtSize.top, fWidth , fHeight,
					(float)pMarkInfo_Relation->iRow * 32.0f , (float) pMarkInfo_Relation->iColumn * 32.0f, 32.0f, 32.0f);
			}


			if( !pPlayer->IsExistGuildMark())
				return;

			//외곽선 드러나게
			rtSize.left += 3;
			rtSize.top  += 3;
			fWidth		-= 6.f;
			fHeight     -= 6.f;

		 	ENUM_ALPHABLEND eOldAlphaMode =	g_pSunRenderer->GetAlphaBlendMode();
			g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);
			for(int i =0; i < 2; ++i)
			{
				if( i == 0)
					pMarkInfo =  GuildMarkInfoParser::Instance()->GetGuildMarkInfo(pPlayer->GetGuildRenderInfo().m_BackGroundIndex);
				else
					pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(pPlayer->GetGuildRenderInfo().m_PatternIndex);

				if( pMarkInfo )
				{
					g_pSunRenderer->RenderTexture(hGuildMarkTexture, (float)rtSize.left , (float)rtSize.top, fWidth , fHeight,
						(float)pMarkInfo->iRow * 32.0f , (float) pMarkInfo->iColumn * 32.0f, 32.0f, 32.0f);
				}
			}
			g_pSunRenderer->SetAlphaBlendMode(eOldAlphaMode);
		}

	}
}

void TargetDialog::ShowAggroDialog()
{
    uiTargetAggro* target_aggro_dialog = GET_CAST_DIALOG(uiTargetAggro, IM_TARGET_MANAGER::TARGET_AGGRO);
    if (is_show_aggro_)//켜기 상태
    {
        if (target_aggro_dialog && target_aggro_dialog->IsVisible() == FALSE)
        {
            target_aggro_dialog->ShowInterface(TRUE);
        }
    }
    else//끄기 상태
    {
        if (target_aggro_dialog && target_aggro_dialog->IsVisible() == TRUE)
        {
            target_aggro_dialog->ShowInterface(FALSE);
        }
    }
}

#ifdef _NA_000000_20130114_RENEWER_UI
void TargetDialog::MoveWindowForce( float addx, float addy )
{
	float x = this->GetDialogWZ()->GetDlgX();
	float y = this->GetDialogWZ()->GetDlgY();

	x += addx;
	y += addy;

	this->GetDialogWZ()->MoveWindowWZ(x,y);
}
#endif


#ifdef _NA_000000_20130114_RENEWER_UI
int TargetDialog::GetTargetItemScore()
{
#ifdef _RU_0000_20120227_RUSSIA_BLOCK_ITEM_SCORE
	return -1;
#endif //_RU_0000_20120227_RUSSIA_BLOCK_ITEM_SCORE

	Object * target_object = g_ObjectManager.GetObject(m_dwTargetKEY);
	if ((target_object == NULL) ||
		(g_pHero == NULL))
	{
		return -1;
	}

	if (target_object->IsKindOfObject(PLAYER_OBJECT) == FALSE)
	{
		return -1;
	}

	if( Player* player = static_cast<Player*>(target_object) )
	{
        return player->GetItemScore();
	}

    return -1;
}

void TargetDialog::DrawItemScore()
{
    int item_score = GetTargetItemScore();
    if (item_score < 0)
    {
        return;
    }

	//창이 나타날 때 툴팁 출력
	int mouse_pos_x = Mouse::Instance()->GetMouseX();
	int mouse_pos_y = Mouse::Instance()->GetMouseY();
	RECT control_rect;
	bool is_draw_score_tooltip = false;

	CCtrlStaticWZ* pstatic_control = static_cast<CCtrlStaticWZ*>(getControl(TARGET_SCORE_STRING_S001));
	control_rect = pstatic_control->GetSizeRect();

	if ( control_rect.left <= mouse_pos_x && control_rect.right >= mouse_pos_x )
	{
		if ( control_rect.top <= mouse_pos_y && control_rect.bottom >= mouse_pos_y )
		{
			is_draw_score_tooltip = true;
		}
	}

	if (is_draw_score_tooltip == true)
	{
		uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
		if ( tooltip_manager != NULL )
		{
			// 11478 : 장비 평점
			tooltip_manager->RegisterTooltipWithStringCode(11478);
			tooltip_manager->SetTooltipDialogID(GetDialogKey());
		}
	}
}
void TargetDialog::UpdateItemScore()
{
	CCtrlStaticWZ* pstatic_control = static_cast<CCtrlStaticWZ*>(getControl(TARGET_SCORE_STRING_S001));
	int score_item = GetTargetItemScore();
	if (score_item >= 0)
	{
		TCHAR string_score[256] = {0,};
		sprintf(string_score,"%d",score_item);

		pstatic_control->ShowWindowWZ(WZ_SHOW);
		GlobalFunc::SetCaption(pstatic_control,string_score);
	}
	else
	{
		pstatic_control->ShowWindowWZ(WZ_HIDE);
		//GlobalFunc::SetCaption(pStaticControl, m_szResultInfo);
	}
}

void TargetDialog::MoveDefaultWindowPos()
{
	uiBase::MoveDefaultWindowPos();
	MoveWindowForce(0.0f,-6.0f);
}

#else
#ifdef _NA_002935_20110704_ITEM_SCORE
void TargetDialog::DrawItemScore()
{
    int item_score = GetTargetItemScore();
    if (item_score >= 0)
    {
        CControlWZ* number_control = getControl(TARGET_SCORE_NUMBER1);
        if (number_control)
        {
            RECT first_position = number_control->GetSizeRect();
            g_InterfaceManager.RenderDrawItemScore(first_position,item_score);
        }

        //창이 나타날 때 툴팁 출력
        int mouse_pos_x = Mouse::Instance()->GetMouseX();
        int mouse_pos_y = Mouse::Instance()->GetMouseY();
        RECT control_rect;
        bool is_draw_score_tooltip = false;
        for (int index = TARGET_SCORE_START; index < TARGET_SCORE_END; ++index)
        {
            control_rect = getControl(index)->GetSizeRect();

            if ( control_rect.left <= mouse_pos_x && control_rect.right >= mouse_pos_x )
            {
                if ( control_rect.top <= mouse_pos_y && control_rect.bottom >= mouse_pos_y )
                {
                    is_draw_score_tooltip = true;
                }
            }
        }
        if (is_draw_score_tooltip == true)
        {
            uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
            if ( tooltip_manager != NULL )
            {
                // 11478 : 장비 평점
                tooltip_manager->RegisterTooltipWithStringCode(11478);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());
            }
        }
    }
}


void TargetDialog::UpdateItemScore()
{
    CCtrlPictureWZ* picture_control;
    if (GetTargetItemScore() >= 0)
    {
        for (int index = TARGET_SCORE_START; index < TARGET_SCORE_END; ++index)
        {
            picture_control = static_cast<CCtrlPictureWZ*>(getControl(index));
            if (picture_control)
            {
                picture_control->ShowWindowWZ(WZ_SHOW);
            }
        }
    }
    else
    {
        for (int index = TARGET_SCORE_START; index < TARGET_SCORE_END; ++index)
        {
            picture_control = static_cast<CCtrlPictureWZ*>(getControl(index));
            if (picture_control)
            {
                picture_control->ShowWindowWZ(WZ_HIDE);
            }
        }
    }
}

int TargetDialog::GetTargetItemScore()
{
#ifdef _RU_0000_20120227_RUSSIA_BLOCK_ITEM_SCORE
    return -1;
#endif //_RU_0000_20120227_RUSSIA_BLOCK_ITEM_SCORE

    Object * target_object = g_ObjectManager.GetObject(m_dwTargetKEY);
    if ((target_object == NULL) ||
        (g_pHero == NULL))
    {
        return -1;
    }
    
    if (target_object->IsKindOfObject(PLAYER_OBJECT) == FALSE)
    {
        return -1;
    }
    
    if( Player* player = static_cast<Player*>(target_object) )
    {
        return player->GetItemScore();
    }

    return -1;
}

#endif //_NA_002935_20110704_ITEM_SCORE
#endif