

#include "SunClientPrecompiledHeader.h"
#include "Application.h"
#include "OptionGameDialog.h"
#include "ResourceManager.h"
#include "InterfaceManager.h"
#include "Map.h"
#include "Sound.h"
#include "SoundBGM.h"
#include "globalfunc.h"
#include "Hero.h"

#include "BattleScene.h"
#include "HeroActionInput.h"
#include "Mouse.h"
#include "Camera.h"

#include "CommandManager.h"

#pragma warning ( push )
#pragma warning(disable : 4800)

#ifdef _NA_000000_20130114_RENEWER_UI
#else

const int	c_iMaxList = 3;

const	int	c_iDamageOptionMaxNum = 2;
int		iDamage = 0;
int		iOptionDamage[c_iMaxList] = { 70419, 70420, 70314 };	//	모두, 나만, 끔
#include "uiHelpMan/uiHelpMan.h"
#include "HeroData.h"



float GetCameraPos(int index)
{
    static float data[3] = {0.f, 0.35f, 0.45f};

    if ((index >= 0) &&
        (index < 3))
    {
        return data[index];
    }

    return 0.35f;
}

WzID OptionGameDialog::m_wzId[OPTION_GAME_MAX] = 
{ 
	StrToWzID("B999"),	//	버튼 - 종료

	StrToWzID("TI06"),	//	버튼 - 초기화
	StrToWzID("TI07"),	//	버튼 - 확인
	
	StrToWzID("T022"),	//	피해 수치 택스트

	StrToWzID("C000"),	//	플레이어 이름 체크 버튼
	StrToWzID("C001"),	//	플레이어 계급 체크 버튼
	StrToWzID("C002"),	//	길드 이름 체크 버튼
	StrToWzID("CT07"),	//	펫네임 보이기

#ifdef __CN_ADD_HEAD_EFFECT_FLAG
	StrToWzID("C017"),	//	INVEN악세사리이펙트버튼
#endif//__CN_ADD_HEAD_EFFECT_FLAG

	StrToWzID("C003"),	//	NPC 이름 체크 버튼
	StrToWzID("C004"),	//	투구 보이기 체크 버튼
	StrToWzID("C005"),	//	공격시 자동 이동 체크 버튼
	StrToWzID("C006"),	//	도움말 체크 버튼
	StrToWzID("C013"),	//	인터페이스 고정

    StrToWzID("C007"),  //  자동 타겟
    StrToWzID("C008"),  //  전방 우선 타겟
    StrToWzID("C009"),  //  타겟 고정
    StrToWzID("C011"),  //  시야 마우스 변경
    StrToWzID("C012"),  //  카메라 방향으로 이동
    StrToWzID("H001"),  //  마우스 감도 조절
    StrToWzID("H002"),  //  시점 전환 속도

	StrToWzID("C010"),	// 스킬 큐잉
	StrToWzID("H003"),	// 마우스 시점 속도

	StrToWzID("B001"),	//	버튼 - ( 피해 수치 표시) LEFT
	StrToWzID("B002"),	//	버튼 - ( 피해 수치 표시) RIGHT

	StrToWzID("C014"),
	StrToWzID("C015"),
#ifdef _DH_BATTLEZONE2_
	StrToWzID("C017"), // 동영상
#else
    StrToWzID("C016"),
#endif //_DH_BATTLEZONE2_
	StrToWzID("CT00"),
	StrToWzID("CT02"),	// 스프링 카메라 

	StrToWzID("CT03"),	// OPTION_GAME_CHECK_WHISPER_BLOCK
	StrToWzID("CT04"),	// OPTION_GAME_CHECK_TRADE_BLOCK,
	StrToWzID("CT05"),	// 
	StrToWzID("CT01"), //OPTION_GAME_CHECK_SHOW_MY_RENDERINFO

#ifdef _YJ_PK_OPTION_001120
	StrToWzID("CT06"), //OPTION_GAME_CHECK_PK_WITHOUT_CTRL_KEY
#endif //_YJ_PK_OPTION_001120

	StrToWzID("CT08"), //OPTION_GAME_CHECK_PICK_UP_ITEM_NOTIFY	
	StrToWzID("CT09"), //OPTION_GAME_CHECK_NOTIFY_DROP_ITEM

	StrToWzID("CT10"), //OPTION_GAME_CHECK_SHOW_PLAYERS	
	StrToWzID("CT11"), //OPTION_GAME_CHECK_SHOW_VENDORS	
#ifdef _DH_OS_LANGUAGETYPE
	StrToWzID("S001"), //OPTION_GAME_TEXT_LANGUAGE	
	StrToWzID("L000"), //OPTION_GAME_LIST_LANGUAGE	
	StrToWzID("P001"), //OPTION_GAME_BACK_LANGUAGE	
	StrToWzID("P002"), //OPTION_GAME_SCROLL_BACK_LANGUAGE	
	StrToWzID("V000"), //OPTION_GAME_SCORLL_LANGUAGE
	StrToWzID("B000"), //OPTION_GAME_POPUP_BUTTON
#endif//_DH_OS_LANGUAGETYPE

    StrToWzID("C018"), //OPTION_GAME_CHANNEL_ACCEPT

    StrToWzID("H004"), //OPTION_GAME_HSCROLL_CAMERA_VIEW_POS

#ifdef _DH_BATTLEZONE2_
    StrToWzID("C016"), //OPTION_GAME_PICKING_FRIEND
#endif //_DH_BATTLEZONE2_
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    StrToWzID("C019"), //OPTION_VISIBLE_WING
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
};



OptionGameDialog::OptionGameDialog( InterfaceManager * pUIMan )
:uiBase( pUIMan )
{
	int		i;
	for( i = 0 ; i < OGCB_MAX ; ++i )
	{
		m_pCheckBox[i] = NULL;
	}

    for (i=0; i<OGHS_MAX; ++i)
    {
        m_pHScroll[i] = NULL;
    }

}

OptionGameDialog::~OptionGameDialog()
{
}


VOID OptionGameDialog::Init( CDrawBase * pDrawBase )
{
	//UpdatePosition();

	TCHAR	szMessage[INTERFACE_STRING_LENGTH]	= {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH]		= {0,};


	//	각 컨트롤의 포인터 얻어오기
	

	DWORD	dwTextColor	= RGBA( 235, 192, 38, 255 );
	DWORD	dwWideTextColor	= RGBA( 245, 166, 46, 255 );
	DWORD	dwSelectColor = RGBA( 0, 0, 255,255 );
	DWORD	dwBGColor = RGBA( 32, 32, 32, 0 );
	DWORD	dwSetColorSelBarOnText = RGBA( 255, 255, 255, 255 );

	//	체크 박스 얻어오기( 효과 끄고 켜기 )
	m_pCheckBox[OGCB_NAME] = static_cast<CCtrlButtonCheckWZ*>(getControl(OPTION_GAME_CHECK_NAME));
	m_pCheckBox[OGCB_GRADE] = static_cast<CCtrlButtonCheckWZ*>(getControl(OPTION_GAME_CHECK_GRADE));
	m_pCheckBox[OGCB_GUILD] = static_cast<CCtrlButtonCheckWZ*>(getControl(OPTION_GAME_CHECK_GUILD));
	m_pCheckBox[OGCB_PET_NAME] = static_cast<CCtrlButtonCheckWZ*>(getControl(OPTION_GAME_CHECK_PET_NAME));

#ifdef __CN_ADD_HEAD_EFFECT_FLAG
	m_pCheckBox[OGCB_HEAD_EFFECT]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_HEAD_EFFECT));
#endif//__CN_ADD_HEAD_EFFECT_FLAG(INVEN악세사리이펙트버튼)

	m_pCheckBox[OGCB_NPC]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_NPC));
	m_pCheckBox[OGCB_HELM]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_HELM));
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    m_pCheckBox[OGCB_WING]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_VISIBLE_WING));
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
	m_pCheckBox[OGCB_AUTOATTACK]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_AUTOATTACK));
	m_pCheckBox[OGCB_HELP]= static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_HELP));
	m_pCheckBox[OGCB_FIX_UI] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_FIX_UI));

    m_pCheckBox[OGCB_AUTOTARGET] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_AUTOTARGET));
    m_pCheckBox[OGCB_FORWARDTARGET] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_FORWARDTARGET));
    m_pCheckBox[OGCB_FIXTARGET] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_FIXTARGET));
    m_pCheckBox[OGCB_CHANGE_EYE_MOUSE] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_CHANGE_EYE_MOUSE));
    m_pCheckBox[OGCB_MOVE_TO_CAMERA] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_MOVE_TO_CAMERA));
	
	m_pCheckBox[OGCB_MOVE_TO_CAMERA]->SetCheckState(TRUE);

	m_pCheckBox[OGCB_MARKET_TITLE] = static_cast<CCtrlButtonCheckWZ * >(getControl(OPTION_GAME_CHECK_MARKET_TITLE));
	m_pCheckBox[OGCB_MULTITHREAD]  = static_cast<CCtrlButtonCheckWZ * >(getControl(OPTION_GAME_CHECK_MULTITHREAD));
	m_pCheckBox[OGCB_MOVIE_PLAY]   = static_cast<CCtrlButtonCheckWZ * >(getControl(OPTION_GAME_CHECK_MOVIE_PLAY));
	m_pCheckBox[OGCB_CAMERA_DISTANCE] = static_cast<CCtrlButtonCheckWZ * >(getControl(OPTION_GAME_CHECK_CAMERA_DISTANCE));

	m_pCheckBox[OGCB_SPRING_CAMERA] = static_cast<CCtrlButtonCheckWZ * >(getControl(OPTION_GAME_CHECK_SPRING_CAMERA));

	m_pCheckBox[OGCB_WHISPER_BLOCK] = static_cast<CCtrlButtonCheckWZ * >(getControl(OPTION_GAME_CHECK_WHISPER_BLOCK));
	assert(m_pCheckBox[OGCB_WHISPER_BLOCK]);

	m_pCheckBox[OGCB_TRADE_BLOCK] = static_cast<CCtrlButtonCheckWZ * >(getControl(OPTION_GAME_CHECK_TRADE_BLOCK));
	assert(m_pCheckBox[OGCB_TRADE_BLOCK]);

	m_pCheckBox[OGCB_DISPLAY_MONSTERINFO] = static_cast<CCtrlButtonCheckWZ * >(getControl(OPTION_GAME_CHECK_DISPLAY_MONSTERINFO));
	m_pCheckBox[OGCB_SHOW_MY_RENDERINFO] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_SHOW_MY_RENDERINFO));
	assert(m_pCheckBox[OGCB_SHOW_MY_RENDERINFO]);

#ifdef _YJ_PK_OPTION_001120
	m_pCheckBox[OGCB_PK_WITHOUT_CTRL_KEY] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_PK_WITHOUT_CTRL_KEY));
	assert(m_pCheckBox[OGCB_PK_WITHOUT_CTRL_KEY]);
#endif //_YJ_PK_OPTION_001120

	m_pCheckBox[OGCB_PICK_UP_ITEM_NOTIFY] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_PICK_UP_ITEM_NOTIFY));
	assert(m_pCheckBox[OGCB_PICK_UP_ITEM_NOTIFY]);
	
	m_pCheckBox[OGCB_NOTIFY_DROP_ITEM] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_NOTIFY_DROP_ITEM));
	assert(m_pCheckBox[OGCB_NOTIFY_DROP_ITEM]);

	m_pCheckBox[OGCB_SHOW_PLAYERS] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_SHOW_PLAYERS));
	m_pCheckBox[OGCB_SHOW_VENDORS] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CHECK_SHOW_VENDORS));

    m_pHScroll[OGHS_MOUSE_SENS] =  static_cast<CCtrlHScrollWZ *>(getControl(OPTION_GAME_HSCROLL_MOUSE_SENS));
    
    // ckTODO: 마우스 감도조절 스크롤 값 정하기
    if (m_pHScroll[OGHS_MOUSE_SENS])
    {
        m_pHScroll[OGHS_MOUSE_SENS]->SetScrollRangeWZ(1, 5);
        m_pHScroll[OGHS_MOUSE_SENS]->SetScrollPosWZ(GAMEOPTION->GetMouseViewSens());
    }


    m_pHScroll[OGHS_MOUSE_VIEW_SENS] =  static_cast<CCtrlHScrollWZ *>(getControl(OPTION_GAME_HSCROLL_MOUSE_VIEW_SENS));

    if (m_pHScroll[OGHS_MOUSE_VIEW_SENS])
    {
        m_pHScroll[OGHS_MOUSE_VIEW_SENS]->SetScrollRangeWZ(1, 5);
        m_pHScroll[OGHS_MOUSE_VIEW_SENS]->SetScrollPosWZ(GAMEOPTION->GetMouseSens());
    }

	 m_pCheckBox[OGCB_SKILL_QUEUEING] = static_cast<CCtrlButtonCheckWZ *>(getControl(OPTION_GAME_CKECK_SKILL_QUEUEING));

	 m_pHScroll[OGHS_MOUSE_VIEW_SPEED] =  static_cast<CCtrlHScrollWZ *>(getControl(OPTION_GAME_HSCROLL_MOUSE_VIEW_SPEED));
	 
	if (m_pHScroll[OGHS_MOUSE_VIEW_SPEED])
	{
		m_pHScroll[OGHS_MOUSE_VIEW_SPEED]->SetScrollRangeWZ(1, 5);
		m_pHScroll[OGHS_MOUSE_VIEW_SPEED]->SetScrollPosWZ(GAMEOPTION->GetMouseViewSpeed());
	}

    m_pHScroll[OGHS_CAMERA_VIEW_POS] =  static_cast<CCtrlHScrollWZ *>(getControl(OPTION_GAME_HSCROLL_CAMERA_VIEW_POS));
    if (m_pHScroll[OGHS_CAMERA_VIEW_POS])
    {
        m_pHScroll[OGHS_CAMERA_VIEW_POS]->SetScrollRangeWZ(0, 2);
        m_pHScroll[OGHS_CAMERA_VIEW_POS]->SetScrollPosWZ(GAMEOPTION->GetCameraViewPos());
    }


	m_pStaticDamage = static_cast<CCtrlStaticWZ *>(getControl( OPTION_GAME_TEXT_DAMAGE ));

#ifdef _DH_OS_LANGUAGETYPE
	language_text_ = static_cast<CCtrlStaticWZ*>(getControl(OPTION_GAME_TEXT_LANGUAGE));
	language_list_ = static_cast<CCtrlListWZ*>(getControl(OPTION_GAME_LIST_LANGUAGE));
	language_back_ = static_cast<CCtrlPictureWZ*>(getControl(OPTION_GAME_BACK_LANGUAGE));
	language_vscroll_back_ = static_cast<CCtrlPictureWZ*>(getControl(OPTION_GAME_SCROLL_BACK_LANGUAGE));
	language_vscroll_ = static_cast<CCtrlVScrollWZ*>(getControl(OPTION_GAME_SCORLL_LANGUAGE));
	ShowLanguageOption(false);
	if(language_list_)
	{
		language_list_->DeleteItemAll();
		language_list_->SetBeginDrawIndex(0);

		int		iSellHeight = language_list_->GetSellheight();
		RECT	rc = language_list_->GetSizeRect();

		language_list_->MoveWindowWZ((float)rc.left,(float)rc.top+5,(float)(rc.right - rc.left),(float)(iSellHeight *5 ));

		int index;
		DWORD c_dwTextColor = RGBA(255, 255, 255, 255);
		DWORD c_dwSelTextColor = RGBA(0,220,0,255);
		DWORD c_dwBackColor = RGBA(0,0,0,0);

		TCHAR string[INTERFACE_STRING_LENGTH] = {0};
		index = language_list_->InsertItemBack();
		g_InterfaceManager.GetInterfaceString(GetLanguage(kLanguageGlobal), string);
		language_list_->SetItemFontID(0, 0, StrToWzID("mn12") );
		GlobalFunc::SetItemText(language_list_, index, string);
		GlobalFunc::SetItem(language_list_, index, 0, string, StrToWzID("mn12"), DT_VCENTER |  DT_CENTER, 
			c_dwBackColor, c_dwTextColor, c_dwSelTextColor);

		index = language_list_->InsertItemBack();
		g_InterfaceManager.GetInterfaceString(GetLanguage(kLanguageGermany), string);
		language_list_->SetItemFontID(1, 0, StrToWzID("mn12") );
		GlobalFunc::SetItemText(language_list_, index, string);
		GlobalFunc::SetItem(language_list_, index, 0, string, StrToWzID("mn12"), DT_VCENTER |  DT_CENTER, 
			c_dwBackColor, c_dwTextColor, c_dwSelTextColor);
	}
	SetLanguage(CGameOption::Instance()->GetLanguageType());
#endif//_DH_OS_LANGUAGETYPE

    m_pCheckBox[OGCB_CHANNEL_ACCEPT] = static_cast<CCtrlButtonCheckWZ * >(getControl(OPTION_GAME_CHANNEL_ACCEPT));
    assert(m_pCheckBox[OGCB_CHANNEL_ACCEPT]);

#ifdef _DH_BATTLEZONE2_
    m_pCheckBox[OGCB_PICKING_FRIEND] = static_cast<CCtrlButtonCheckWZ * >(getControl(OPTION_GAME_PICKING_FRIEND));
    assert(m_pCheckBox[OGCB_PICKING_FRIEND]);
#endif //_DH_BATTLEZONE2_

	//-------------------------------------------------------------------------
	//		옵션 데이터 설정
	//-------------------------------------------------------------------------

	m_OptionParam = GAMEOPTION->GetGameOption();
	m_InitOptionParam = m_OptionParam;

	ApplyControl();
}

#ifdef _DH_OS_LANGUAGETYPE
DWORD OptionGameDialog::GetLanguage(LanguageType language_type)
{
	/*2420	영어 kLanguageGlobal
	2421	독일어 kLanguageGermany
	2422	프랑스어
	2423	스페인어
	2424	브라질어
	2425	터키어*/
	DWORD text_id = 0;
	switch(language_type)
	{
	case kLanguageGermany:
		text_id = 2421;
		break;
	case kLanguageGlobal:
		text_id = 2420;
		break;
	default:
		text_id = 2420;
		break;
	}
	return text_id;
}
void OptionGameDialog::SetLanguage(LanguageType language_type)
{
	//CGameOption::Instance()->SetLanguageType(language_type);
	DWORD text_id = GetLanguage(language_type);
	if (language_text_)
	{
		language_text_->SetTextID(text_id);	
	}
}

void OptionGameDialog::ShowLanguageOption(bool is_show)
{
	ENUM_STATEWZ show;
	is_language_popup_ = is_show;
	if (is_show)
	{
		show = WZ_SHOW;
	}
	else
	{
		show = WZ_HIDE;
	}

	if (language_list_)
	{
		language_list_->ShowWindowWZ(show);
	}
	if (language_back_)
	{
		language_back_->ShowWindowWZ(show);
	}
	if (language_vscroll_back_)
	{
		language_vscroll_back_->ShowWindowWZ(show);
	}
	if (language_vscroll_)
	{
		language_vscroll_->ShowWindowWZ(show);
	}
}
#endif//_DH_OS_LANGUAGETYPE

VOID OptionGameDialog::Release()
{
}


VOID OptionGameDialog::MessageProc( SI_MESSAGE * pMessage )
{
	int		iIndex = -1;
	int		iSelect;
	TCHAR	szText[INTERFACE_STRING_LENGTH];
	int		iScrollPos;
	float	fScrollStep = 0.15f;	

	switch( pMessage->eResultMsg ) 
	{
		//-----------------------------------------------------------------------------------------
		//		마우스 왼쪽 클릭 이벤트가 일어났을 때, 어디냐?
		//-----------------------------------------------------------------------------------------
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
				//---------------------------------------------------------------------------------
				//		체크 박스들~
				//---------------------------------------------------------------------------------
			case OPTION_GAME_CHECK_NAME:
				{
                    if (m_pCheckBox[OGCB_NAME])
                    {
                        BOOL ShowPlayerName = m_pCheckBox[OGCB_NAME]->GetCheckState();
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_PLAYER_NAME, ShowPlayerName );
                        if( ShowPlayerName )
                        {
                            m_pCheckBox[OGCB_GRADE]->ShowWindowWZ( WZ_SHOW );
                            m_pCheckBox[OGCB_GUILD]->ShowWindowWZ( WZ_SHOW );
							m_pCheckBox[OGCB_MARKET_TITLE]->ShowWindowWZ( WZ_SHOW );

                        }
                        else
                        {
                            m_pCheckBox[OGCB_GRADE]->ShowWindowWZ( WZ_HIDE );
                            m_pCheckBox[OGCB_GUILD]->ShowWindowWZ( WZ_HIDE );
							m_pCheckBox[OGCB_MARKET_TITLE]->ShowWindowWZ( WZ_HIDE );
                        }
                    }
				}
				break;

			case OPTION_GAME_CHECK_GRADE:
				{
                    if (m_pCheckBox[OGCB_GRADE])
                    {
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_PLAYER_GRADE, m_pCheckBox[OGCB_GRADE]->GetCheckState() );
                    }
				}
				break;

			case OPTION_GAME_CHECK_GUILD:
				{
                    if (m_pCheckBox[OGCB_GUILD])
                    {
					    m_OptionParam.SetOptionValue( nOptionType::SHOW_PLAYER_GUILD, m_pCheckBox[OGCB_GUILD]->GetCheckState() );
                    }
				}
				break;

			case OPTION_GAME_CHECK_PET_NAME:
				{
					if (m_pCheckBox[OGCB_PET_NAME])
                    {
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_PLAYER_PETNAME, m_pCheckBox[OGCB_PET_NAME]->GetCheckState() );
                    }
				}
				break;

#ifdef __CN_ADD_HEAD_EFFECT_FLAG
			case OPTION_GAME_CHECK_HEAD_EFFECT:
				{
					if (m_pCheckBox[OGCB_HEAD_EFFECT])
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_HEAD_EFFECT, m_pCheckBox[OGCB_HEAD_EFFECT]->GetCheckState() );
				}
				break;
#endif//__CN_ADD_HEAD_EFFECT_FLAG(INVEN악세사리이펙트버튼)
	
			case OPTION_GAME_CHECK_NPC:
				{
                    if (m_pCheckBox[OGCB_NPC])
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_NPC_NAME, m_pCheckBox[OGCB_NPC]->GetCheckState() );
				}
				break;

			case OPTION_GAME_CHECK_HELM:
				{
                    if (m_pCheckBox[OGCB_HELM])
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_HELMET, m_pCheckBox[OGCB_HELM]->GetCheckState() );
				}
				break;
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
            case OPTION_VISIBLE_WING:
                {
                    if (m_pCheckBox[OGCB_WING])
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_WING, m_pCheckBox[OGCB_WING]->GetCheckState() );
                }
                break;
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
			case OPTION_GAME_CHECK_AUTOATTACK:
				{
                    if (m_pCheckBox[OGCB_AUTOATTACK])
                        m_OptionParam.SetOptionValue( nOptionType::USE_AUTO_MOVE, m_pCheckBox[OGCB_AUTOATTACK]->GetCheckState() );
				}
				break;

			case OPTION_GAME_CHECK_HELP:
				{
                    if (m_pCheckBox[OGCB_HELP])
                        m_OptionParam.SetOptionValue( nOptionType::USE_HELP, m_pCheckBox[OGCB_HELP]->GetCheckState() );
				}
				break;
			case OPTION_GAME_CHECK_FIX_UI:
				{
					if(m_pCheckBox[OGCB_FIX_UI])
                        m_OptionParam.SetOptionValue( nOptionType::USE_FIXED_UI_POSITION, m_pCheckBox[OGCB_FIX_UI]->GetCheckState() );
				}
				break;

            case OPTION_GAME_CHECK_AUTOTARGET:
                {
                    if (m_pCheckBox[OGCB_AUTOTARGET])
                    {
                        int Value = m_OptionParam.GetOptionValue( nOptionType::USE_AUTO_TARGET );
                        Value = !Value;
                        m_pCheckBox[OGCB_AUTOTARGET]->SetCheckState(Value);
                        m_OptionParam.SetOptionValue( nOptionType::USE_AUTO_MOVE, Value );
                    }
                }
                break;

            case OPTION_GAME_CHECK_FORWARDTARGET:
                {
                    if (m_pCheckBox[OGCB_FORWARDTARGET])
                    {
                        int Value = m_OptionParam.GetOptionValue( nOptionType::USE_FORWARD_TARGET );
                        Value = !Value;
                        m_pCheckBox[OGCB_FORWARDTARGET]->SetCheckState(Value);
                        m_OptionParam.SetOptionValue( nOptionType::USE_FORWARD_TARGET, Value );
                    }
                }
                break;

            case OPTION_GAME_CHECK_FIXTARGET:
                {
                    if (m_pCheckBox[OGCB_FIXTARGET])
                    {
                        int Value = m_OptionParam.GetOptionValue( nOptionType::USE_FIXED_TARGET );
                        Value = !Value;
                        m_pCheckBox[OGCB_FIXTARGET]->SetCheckState(Value);
                        m_OptionParam.SetOptionValue( nOptionType::USE_FIXED_TARGET, Value );
                    }
                }
                break;

            case OPTION_GAME_CHECK_CHANGE_EYE_MOUSE:
                {
                    if (m_pCheckBox[OGCB_CHANGE_EYE_MOUSE])
                        m_OptionParam.SetOptionValue( nOptionType::USE_CHANGE_EYE_MOUSE, m_pCheckBox[OGCB_CHANGE_EYE_MOUSE]->GetCheckState() );
                }
                break;

            case OPTION_GAME_CHECK_MOVE_TO_CAMERA:
                {
                    if (m_pCheckBox[OGCB_MOVE_TO_CAMERA])
                        m_OptionParam.SetOptionValue( nOptionType::USE_MOVE_TO_CAMERA, m_pCheckBox[OGCB_MOVE_TO_CAMERA]->GetCheckState() );
                }
                break;

			case OPTION_GAME_CHECK_SPRING_CAMERA:
				{
					if (m_pCheckBox[OGCB_SPRING_CAMERA])
                        m_OptionParam.SetOptionValue( nOptionType::USE_SPRING_CAMERA, m_pCheckBox[OGCB_SPRING_CAMERA]->GetCheckState() );
				}
				break;


			case OPTION_GAME_CHECK_WHISPER_BLOCK:
				{
					if (m_pCheckBox[OGCB_WHISPER_BLOCK])
                        m_OptionParam.SetOptionValue( nOptionType::USE_BLOCK_WHISPER, m_pCheckBox[OGCB_WHISPER_BLOCK]->GetCheckState() );
				}
				break;

			case OPTION_GAME_CHECK_TRADE_BLOCK:
				{
					if (m_pCheckBox[OGCB_TRADE_BLOCK])
                        m_OptionParam.SetOptionValue( nOptionType::USE_BLOCK_TRADE, m_pCheckBox[OGCB_TRADE_BLOCK]->GetCheckState() );
				}
				break;


			case OPTION_GAME_CKECK_SKILL_QUEUEING:		// 스킬큐잉
				{
					if (m_pCheckBox[OGCB_SKILL_QUEUEING])
                        m_OptionParam.SetOptionValue( nOptionType::USE_SKILLQUEUEING, m_pCheckBox[OGCB_SKILL_QUEUEING]->GetCheckState() );
				}
				break;

			case OPTION_GAME_CHECK_MARKET_TITLE:
				{
					if( m_pCheckBox[OGCB_MARKET_TITLE] )
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_VENDOR_TITLE, m_pCheckBox[OGCB_MARKET_TITLE]->GetCheckState() );
				}
				break;

			case OPTION_GAME_CHECK_MULTITHREAD:
				{
					if( m_pCheckBox[OGCB_MULTITHREAD] )
                        m_OptionParam.SetOptionValue( nOptionType::USE_MULTITHREAD, m_pCheckBox[OGCB_MULTITHREAD]->GetCheckState() );
				}
				break;

			case OPTION_GAME_CHECK_MOVIE_PLAY:
				{
					if( m_pCheckBox[OGCB_MOVIE_PLAY] )
                        m_OptionParam.SetOptionValue( nOptionType::USE_PLAY_MOVIE, m_pCheckBox[OGCB_MOVIE_PLAY]->GetCheckState() );
				}
				break;
			case OPTION_GAME_CHECK_CAMERA_DISTANCE:
				{
					if( m_pCheckBox[OGCB_CAMERA_DISTANCE] )
                        m_OptionParam.SetOptionValue( nOptionType::USE_EXTENSION_CAMERA_DISTANCE, m_pCheckBox[OGCB_CAMERA_DISTANCE]->GetCheckState() );
				}
				break;
            case OPTION_GAME_CHANNEL_ACCEPT:
                {
                    if( m_pCheckBox[OGCB_CHANNEL_ACCEPT] )
                        m_OptionParam.SetOptionValue( nOptionType::USE_BLOCK_CHANNEL_INVITE, m_pCheckBox[OGCB_CHANNEL_ACCEPT]->GetCheckState() );
                }
                break;
#ifdef _DH_BATTLEZONE2_
            case OPTION_GAME_PICKING_FRIEND:
                {
                    if( m_pCheckBox[OGCB_PICKING_FRIEND] )
                        m_OptionParam.SetOptionValue( nOptionType::PICKING_FRIEND, m_pCheckBox[OGCB_PICKING_FRIEND]->GetCheckState() );
                }
                break;
#endif //_DH_BATTLEZONE2_
				//---------------------------------------------------------------------------------
				//		버튼
				//---------------------------------------------------------------------------------
			case OPTION_GAME_BT_DAMAGE_RIGHT:				//	버튼 - ( 피해 수치 표시) RIGHT
				{
                    iSelect = m_OptionParam.GetOptionValue( nOptionType::DAMAGE_TEXT_OUTPUT );

					if(iSelect >= eODO_NONE)
					{
						iSelect = eODO_SHOWALL;
					}
					else
					{
						iSelect++;
					}

                    m_OptionParam.SetOptionValue( nOptionType::DAMAGE_TEXT_OUTPUT, iSelect );
					g_InterfaceManager.GetInterfaceString( iOptionDamage[iSelect], szText, INTERFACE_STRING_LENGTH );
					GlobalFunc::SetCaption( m_pStaticDamage, szText );
				}
				break;

			case OPTION_GAME_BT_DAMAGE_LEFT:			//	버튼 - ( 피해 수치 표시)  LEFT
				{
                    iSelect = m_OptionParam.GetOptionValue( nOptionType::DAMAGE_TEXT_OUTPUT );

					if(iSelect <= eODO_SHOWALL)
					{
						iSelect = eODO_NONE;
					}
					else
					{
						iSelect--;
					}

                    m_OptionParam.SetOptionValue( nOptionType::DAMAGE_TEXT_OUTPUT, iSelect );

					g_InterfaceManager.GetInterfaceString( iOptionDamage[iSelect], szText, INTERFACE_STRING_LENGTH );
					GlobalFunc::SetCaption( m_pStaticDamage, szText );

				}
				break;

			case OPTION_GAME_BT_EXIT:			//	종료 버튼 - 취소의 의미
				{
					m_OptionParam = m_InitOptionParam;
					ApplyControl();
					ShowDialog(FALSE);
				}
				break;

			case OPTION_GAME_BT_INIT:			//	초기화
				{
					iDamage = 0;

					m_OptionParam = m_InitOptionParam;
					ApplyControl();
				}
				break;

			case OPTION_GAME_BT_OK:				//	확인
				{
					ApplyScroll();
					ApplyControl();
					Apply();
					ShowDialog(FALSE);
				}
				break;

			case OPTION_GAME_CHECK_DISPLAY_MONSTERINFO:
				{
					if( m_pCheckBox[OGCB_DISPLAY_MONSTERINFO] )
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_MONSTERINFO, m_pCheckBox[OGCB_DISPLAY_MONSTERINFO]->GetCheckState() );
				}
				break;

			case OPTION_GAME_CHECK_SHOW_MY_RENDERINFO:
				{
					if (m_pCheckBox[OGCB_SHOW_MY_RENDERINFO])
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_MY_RENDER_INFO, m_pCheckBox[OGCB_SHOW_MY_RENDERINFO]->GetCheckState() );
				}
				break;

#ifdef _YJ_PK_OPTION_001120
			case OPTION_GAME_CHECK_PK_WITHOUT_CTRL_KEY:
				{
					if(m_pCheckBox[OGCB_PK_WITHOUT_CTRL_KEY])
                        m_OptionParam.SetOptionValue( nOptionType::USE_PK_WITH_CONTROL_KEY, m_pCheckBox[OGCB_PK_WITHOUT_CTRL_KEY]->GetCheckState() );
				}
				break;
#endif //_YJ_PK_OPTION_001120

			case OPTION_GAME_CHECK_PICK_UP_ITEM_NOTIFY:
				{
					if( m_pCheckBox[OGCB_PICK_UP_ITEM_NOTIFY] )
                        m_OptionParam.SetOptionValue( nOptionType::NOTIFY_PICKUP_ITEM, m_pCheckBox[OGCB_PICK_UP_ITEM_NOTIFY]->GetCheckState() );
				}
				break;
	
			case OPTION_GAME_CHECK_NOTIFY_DROP_ITEM:
				{
					if( m_pCheckBox[OGCB_NOTIFY_DROP_ITEM] )
                        m_OptionParam.SetOptionValue( nOptionType::NOTIFY_DROP_ITEM, m_pCheckBox[OGCB_NOTIFY_DROP_ITEM]->GetCheckState() );
				}
				break;

			case OPTION_GAME_CHECK_SHOW_PLAYERS:
				{
					if( m_pCheckBox[OGCB_SHOW_PLAYERS] )
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_PLAYERS, m_pCheckBox[OGCB_SHOW_PLAYERS]->GetCheckState() );
				}
				break;
			case OPTION_GAME_CHECK_SHOW_VENDORS:
				{
					if( m_pCheckBox[OGCB_SHOW_VENDORS] )
                        m_OptionParam.SetOptionValue( nOptionType::SHOW_VENDERS, m_pCheckBox[OGCB_SHOW_VENDORS]->GetCheckState() );
				}
				break;
#ifdef _DH_OS_LANGUAGETYPE
			case OPTION_GAME_POPUP_BUTTON:
				{
					ShowLanguageOption(!is_language_popup_);					
				}
				break;
				//---------------------------------------------------------------------------------
				//		리스트 박스
				//---------------------------------------------------------------------------------
			case OPTION_GAME_LIST_LANGUAGE:
				{
					if (language_list_)
					{
						int iSelect = language_list_->GetCurSel();
						if( iSelect > -1 && iSelect < 2)// 2 : 영어, 독일어
						{
							LanguageType language_type = kLanguageGlobal;
							ShowLanguageOption(false);
							switch(iSelect)
							{
							case 1: language_type = kLanguageGermany; break;
							default: language_type = kLanguageGlobal;; break;
							}
							SetLanguage(language_type);
							//CGameOption::Instance()->SetLanguageType(language_type);
							g_CommandManager.ChangeLanguage(language_type);
						}
					}
				}
				break;
#endif//_DH_OS_LANGUAGETYPE
			default:
				{
				}
				break;
			}

		}
		break;

	//////////////////////////////////////////////////////////////////////////
    case RT_MSG_SB_THUMBTRACK:
	case RT_MSG_SB_THUMBPOSITION:
		{
			switch(getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case OPTION_GAME_HSCROLL_MOUSE_SENS:
				{
					iScrollPos = m_pHScroll[OGHS_MOUSE_SENS]->GetScrollPosWZ();
                    m_OptionParam.SetOptionValue( nOptionType::MOUSE_SENS_VALUE, iScrollPos );
					Mouse::Instance()->SetMoveLevel(iScrollPos);

				}
				break;

			case OPTION_GAME_HSCROLL_MOUSE_VIEW_SENS:
				{
					iScrollPos = m_pHScroll[OGHS_MOUSE_VIEW_SENS]->GetScrollPosWZ();
                    m_OptionParam.SetOptionValue( nOptionType::MOUSE_VIEW_SENS_VALUE, iScrollPos );
					g_Camera.SetRotationLevel(iScrollPos);
				}
				break;

			case OPTION_GAME_HSCROLL_MOUSE_VIEW_SPEED:
				{
					iScrollPos = m_pHScroll[OGHS_MOUSE_VIEW_SPEED]->GetScrollPosWZ();
                    m_OptionParam.SetOptionValue( nOptionType::MOUSE_VIEW_SPEED_VALUE, iScrollPos );
				}
				break;
            case OPTION_GAME_HSCROLL_CAMERA_VIEW_POS:
                {
                    iScrollPos = m_pHScroll[OGHS_CAMERA_VIEW_POS]->GetScrollPosWZ();
                    //m_OptionParam.SetOptionValue( nOptionType::MOUSE_VIEW_SPEED_VALUE, iScrollPos );
                    m_OptionParam.SetOptionValue( nOptionType::CAMERA_VIEW_HEIGHT_OFFSET, iScrollPos );
                    g_Camera.MoveUpDown(GetCameraPos(iScrollPos));
                }
                break;
			}

		}
		break;
		//-----------------------------------------------------------------------------------------
		//		사운드 스크롤 바 왼쪽 버튼 클릭
		//-----------------------------------------------------------------------------------------
	case RT_MSG_SB_LINELEFT:
		{
			switch(getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case OPTION_GAME_HSCROLL_MOUSE_SENS:
				{
                    int Value = m_OptionParam.GetOptionValue( nOptionType::MOUSE_SENS_VALUE);
                    if( Value > 1) 
					{
                        -- Value;
                        m_OptionParam.SetOptionValue( nOptionType::MOUSE_SENS_VALUE, Value );
						m_pHScroll[OGHS_MOUSE_SENS]->SetScrollPosWZ( Value );
						Mouse::Instance()->SetMoveLevel(Value);
					}
					break;
				}
			case OPTION_GAME_HSCROLL_MOUSE_VIEW_SENS:
				{
                    int Value = m_OptionParam.GetOptionValue( nOptionType::MOUSE_VIEW_SENS_VALUE);
					if( Value > 1)
					{
                        -- Value;
                        m_OptionParam.SetOptionValue( nOptionType::MOUSE_VIEW_SENS_VALUE, Value );
						m_pHScroll[OGHS_MOUSE_VIEW_SENS]->SetScrollPosWZ( Value );
						g_Camera.SetRotationLevel(Value);
					}
					break;
				}

			case OPTION_GAME_HSCROLL_MOUSE_VIEW_SPEED:
				{
                    int Value = m_OptionParam.GetOptionValue( nOptionType::MOUSE_VIEW_SPEED_VALUE );
					if( Value > 1 )
					{
                        -- Value;
                        m_OptionParam.SetOptionValue( nOptionType::MOUSE_VIEW_SPEED_VALUE, Value );
						m_pHScroll[OGHS_MOUSE_VIEW_SPEED]->SetScrollPosWZ( Value );
					}
					break;
				}
            case OPTION_GAME_HSCROLL_CAMERA_VIEW_POS:
                {
                    int Value = m_OptionParam.GetOptionValue( nOptionType::CAMERA_VIEW_HEIGHT_OFFSET );
                    if( Value > 0 )
                    {
                        -- Value;
                        m_OptionParam.SetOptionValue( nOptionType::CAMERA_VIEW_HEIGHT_OFFSET, Value );

                        g_Camera.MoveUpDown(GetCameraPos(Value));
                        m_pHScroll[OGHS_CAMERA_VIEW_POS]->SetScrollPosWZ(Value);
                    }
                }
                break;
			}
		}
		break;
		//-----------------------------------------------------------------------------------------
		//		사운드 스크롤 바 오른쪽 버튼 클릭
		//-----------------------------------------------------------------------------------------
	case RT_MSG_SB_LINERIGHT:
		{
			switch(getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case OPTION_GAME_HSCROLL_MOUSE_SENS:
				{
                    int Value = m_OptionParam.GetOptionValue( nOptionType::MOUSE_SENS_VALUE );
					if( Value < 5 )
					{
                        ++ Value;
                        m_OptionParam.SetOptionValue( nOptionType::MOUSE_SENS_VALUE, Value );

						m_pHScroll[OGHS_MOUSE_SENS]->SetScrollPosWZ(Value);
						Mouse::Instance()->SetMoveLevel(Value);
					}
					break;
				}
			case OPTION_GAME_HSCROLL_MOUSE_VIEW_SENS:
				{
                    int Value = m_OptionParam.GetOptionValue( nOptionType::MOUSE_VIEW_SENS_VALUE );
                    if( Value < 5 )
                    {
                        ++ Value;
                        m_OptionParam.SetOptionValue( nOptionType::MOUSE_VIEW_SENS_VALUE, Value );

						m_pHScroll[OGHS_MOUSE_VIEW_SENS]->SetScrollPosWZ(Value);
						g_Camera.SetRotationLevel(Value);
					}
					break;
				}

			case OPTION_GAME_HSCROLL_MOUSE_VIEW_SPEED:
				{
                    int Value = m_OptionParam.GetOptionValue( nOptionType::MOUSE_VIEW_SPEED_VALUE );
                    if( Value < 5 )
                    {
                        ++ Value;
                        m_OptionParam.SetOptionValue( nOptionType::MOUSE_VIEW_SPEED_VALUE, Value );

						m_pHScroll[OGHS_MOUSE_VIEW_SPEED]->SetScrollPosWZ(Value);
					}
					break;
				}
            case OPTION_GAME_HSCROLL_CAMERA_VIEW_POS:
                {
                    int Value = m_OptionParam.GetOptionValue( nOptionType::CAMERA_VIEW_HEIGHT_OFFSET );
                    if( Value < 2 )
                    {
                        ++ Value;
                        m_OptionParam.SetOptionValue( nOptionType::CAMERA_VIEW_HEIGHT_OFFSET, Value );

                        g_Camera.MoveUpDown(GetCameraPos(Value));
                        m_pHScroll[OGHS_CAMERA_VIEW_POS]->SetScrollPosWZ(Value);
                    }
                }
                break;
                
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

VOID OptionGameDialog::NetworkProc( MSG_BASE * pMsg )
{
}

CControlWZ *OptionGameDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < OPTION_GAME_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE OptionGameDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < OPTION_GAME_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return OPTION_GAME_MAX;
}

VOID OptionGameDialog::OnShowWindow( BOOL val )
{
	if( val == TRUE )
	{
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_Option_Game;
		msg.wParam=InterfaceManager::DIALOG_OPTION_GAME;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		ApplyControl();
	}
	else
	{
		ApplyControl();
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_Option_Game);
	}
}
//
//void OptionGameDialog::UpdatePosition()
//{
//	//POINT pt = g_pApplication->Get2DModeMapping(); 
//
//	//// dialog size
//	//RECT rcDialog;
//	//GetDialogWZ()->GetDlgSize(&rcDialog);
//
//	//long dWidth, dHeight;
//	//dWidth = rcDialog.right / 2;
//	//dHeight = rcDialog.bottom / 2;
//
//
//	////	화면 가운데 설정
//	//MoveWindowWZ( (float)((pt.x / 2) - dWidth), (float)((pt.y / 2) - dHeight) );
//}


void OptionGameDialog::ApplyControl( void )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};

	//	체크 박스 설정
	m_pCheckBox[OGCB_NAME]->SetCheckState((bool)m_OptionParam.GetOptionValue(nOptionType::SHOW_PLAYER_NAME));
	m_pCheckBox[OGCB_GRADE]->SetCheckState((bool)m_OptionParam.GetOptionValue(nOptionType::SHOW_PLAYER_GRADE));
	m_pCheckBox[OGCB_GUILD]->SetCheckState((bool)m_OptionParam.GetOptionValue(nOptionType::SHOW_PLAYER_GUILD));
	m_pCheckBox[OGCB_PET_NAME]->SetCheckState((bool)m_OptionParam.GetOptionValue(nOptionType::SHOW_PLAYER_PETNAME));

#ifdef __CN_ADD_HEAD_EFFECT_FLAG
	m_pCheckBox[OGCB_HEAD_EFFECT]->SetCheckState( (bool)m_OptionParam.GetOptionValue(SHOW_HEAD_EFFECT));
#endif//__CN_ADD_HEAD_EFFECT_FLAG(INVEN악세사리이펙트버튼)

	m_pCheckBox[OGCB_NPC]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::SHOW_NPC_NAME));
	m_pCheckBox[OGCB_HELM]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::SHOW_HELMET));
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    if (m_pCheckBox[OGCB_WING] != NULL)
    {
        m_pCheckBox[OGCB_WING]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::SHOW_WING));
    }
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    m_pCheckBox[OGCB_AUTOATTACK]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_AUTO_MOVE));
	m_pCheckBox[OGCB_HELP]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_HELP));
	m_pCheckBox[OGCB_FIX_UI]->SetCheckState((bool)m_OptionParam.GetOptionValue(nOptionType::USE_FIXED_UI_POSITION));


    m_pCheckBox[OGCB_AUTOTARGET]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_AUTO_TARGET));
    m_pCheckBox[OGCB_FORWARDTARGET]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_FORWARD_TARGET));
    m_pCheckBox[OGCB_FIXTARGET]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_FIXED_TARGET));
    m_pCheckBox[OGCB_CHANGE_EYE_MOUSE]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_CHANGE_EYE_MOUSE));
    m_pCheckBox[OGCB_MOVE_TO_CAMERA]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_MOVE_TO_CAMERA));
	
    m_pCheckBox[OGCB_MARKET_TITLE]->SetCheckState((bool) m_OptionParam.GetOptionValue(nOptionType::SHOW_VENDOR_TITLE));
    m_pCheckBox[OGCB_MULTITHREAD]->SetCheckState((bool) m_OptionParam.GetOptionValue(nOptionType::USE_MULTITHREAD));
    m_pCheckBox[OGCB_MOVIE_PLAY]->SetCheckState((bool) m_OptionParam.GetOptionValue(nOptionType::USE_PLAY_MOVIE));
    m_pCheckBox[OGCB_CAMERA_DISTANCE]->SetCheckState((bool) m_OptionParam.GetOptionValue(nOptionType::USE_EXTENSION_CAMERA_DISTANCE));

	if (m_pCheckBox[OGCB_WHISPER_BLOCK])
    {
        m_pCheckBox[OGCB_WHISPER_BLOCK]->SetCheckState((bool) m_OptionParam.GetOptionValue(nOptionType::USE_BLOCK_WHISPER));
    }

	if (m_pCheckBox[OGCB_TRADE_BLOCK])
    {
        m_pCheckBox[OGCB_TRADE_BLOCK]->SetCheckState((bool) m_OptionParam.GetOptionValue(nOptionType::USE_BLOCK_TRADE));
    }

	if (m_pCheckBox[OGCB_DISPLAY_MONSTERINFO])
    {
        m_pCheckBox[OGCB_DISPLAY_MONSTERINFO]->SetCheckState((bool) m_OptionParam.GetOptionValue(nOptionType::SHOW_MONSTERINFO));
    }

	if (m_pCheckBox[OGCB_SHOW_MY_RENDERINFO])
	{
        m_pCheckBox[OGCB_SHOW_MY_RENDERINFO]->SetCheckState((bool) m_OptionParam.GetOptionValue(nOptionType::SHOW_MY_RENDER_INFO));
	}

#ifdef _YJ_PK_OPTION_001120
	if(m_pCheckBox[OGCB_PK_WITHOUT_CTRL_KEY])
	{
        m_pCheckBox[OGCB_PK_WITHOUT_CTRL_KEY]->SetCheckState((bool) m_OptionParam.GetOptionValue(nOptionType::USE_PK_WITH_CONTROL_KEY));
	}
#endif //_YJ_PK_OPTION_001120

	if( m_pCheckBox[OGCB_PICK_UP_ITEM_NOTIFY] )
	{
        m_pCheckBox[OGCB_PICK_UP_ITEM_NOTIFY]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::NOTIFY_PICKUP_ITEM));
	}

	if( m_pCheckBox[OGCB_NOTIFY_DROP_ITEM] )
	{
        m_pCheckBox[OGCB_NOTIFY_DROP_ITEM]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::NOTIFY_DROP_ITEM));
	}

	if( m_pCheckBox[OGCB_SHOW_PLAYERS] )
	{
        m_pCheckBox[OGCB_SHOW_PLAYERS]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::SHOW_PLAYERS));
	}
	if( m_pCheckBox[OGCB_SHOW_VENDORS] )
	{
        m_pCheckBox[OGCB_SHOW_VENDORS]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::SHOW_VENDERS));
	}

	if (m_pHScroll[OGHS_MOUSE_SENS])
    {
        m_pHScroll[OGHS_MOUSE_SENS]->SetScrollRangeWZ(1, 5);
		m_pHScroll[OGHS_MOUSE_SENS]->SetScrollPosWZ(GAMEOPTION->GetMouseSens());
    }

    if (m_pHScroll[OGHS_MOUSE_VIEW_SENS])
    {
        m_pHScroll[OGHS_MOUSE_VIEW_SENS]->SetScrollRangeWZ(1, 5);
		m_pHScroll[OGHS_MOUSE_VIEW_SENS]->SetScrollPosWZ(GAMEOPTION->GetMouseViewSens());
    }

    int DamageTextOutput = m_OptionParam.GetOptionValue(nOptionType::DAMAGE_TEXT_OUTPUT);
    g_InterfaceManager.GetInterfaceString( iOptionDamage[DamageTextOutput], szText, INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption( m_pStaticDamage, szText );

    m_pCheckBox[OGCB_SKILL_QUEUEING]->SetCheckState( m_OptionParam.GetOptionValue(nOptionType::USE_SKILLQUEUEING));

	if (m_pHScroll[OGHS_MOUSE_VIEW_SPEED])
	{
		m_pHScroll[OGHS_MOUSE_VIEW_SPEED]->SetScrollRangeWZ(1, 5);
		m_pHScroll[OGHS_MOUSE_VIEW_SPEED]->SetScrollPosWZ(GAMEOPTION->GetMouseViewSpeed());
	}

    if (m_pHScroll[OGHS_CAMERA_VIEW_POS])
    {
        m_pHScroll[OGHS_CAMERA_VIEW_POS]->SetScrollRangeWZ(0, 2);
        m_pHScroll[OGHS_CAMERA_VIEW_POS]->SetScrollPosWZ(GAMEOPTION->GetCameraViewPos());
    }

	if( m_pCheckBox[OGCB_SPRING_CAMERA])
        m_pCheckBox[OGCB_SPRING_CAMERA]->SetCheckState( (bool)m_OptionParam.GetOptionValue(nOptionType::USE_SPRING_CAMERA));

    if( m_OptionParam.GetOptionValue(nOptionType::SHOW_PLAYER_NAME))
    {
        m_pCheckBox[OGCB_GRADE]->ShowWindowWZ( WZ_SHOW );
        m_pCheckBox[OGCB_GUILD]->ShowWindowWZ( WZ_SHOW );
		m_pCheckBox[OGCB_MARKET_TITLE]->ShowWindowWZ( WZ_SHOW );

    }
    else
    {
        m_pCheckBox[OGCB_GRADE]->ShowWindowWZ( WZ_HIDE );
        m_pCheckBox[OGCB_GUILD]->ShowWindowWZ( WZ_HIDE );
		m_pCheckBox[OGCB_MARKET_TITLE]->ShowWindowWZ( WZ_HIDE );
    }
    if(m_pCheckBox[OGCB_CHANNEL_ACCEPT])
    {
        m_pCheckBox[OGCB_CHANNEL_ACCEPT]->SetCheckState((BOOL)m_OptionParam.GetOptionValue(nOptionType::USE_BLOCK_CHANNEL_INVITE));
    }
#ifdef _DH_BATTLEZONE2_
    if(m_pCheckBox[OGCB_PICKING_FRIEND])
    {
        m_pCheckBox[OGCB_PICKING_FRIEND]->SetCheckState((BOOL)m_OptionParam.GetOptionValue(nOptionType::PICKING_FRIEND));
    }
#endif //_DH_BATTLEZONE2_
}

void OptionGameDialog::ApplyScroll()
{
	if (m_pHScroll[OGHS_MOUSE_SENS])
	{
        m_OptionParam.SetOptionValue(nOptionType::MOUSE_SENS_VALUE, m_pHScroll[OGHS_MOUSE_SENS]->GetScrollPosWZ() );
		m_pHScroll[OGHS_MOUSE_SENS]->SetScrollRangeWZ(1, 5);
		m_pHScroll[OGHS_MOUSE_SENS]->SetScrollPosWZ(GAMEOPTION->GetMouseSens());
	}

	if (m_pHScroll[OGHS_MOUSE_VIEW_SENS])
	{
        m_OptionParam.SetOptionValue(nOptionType::MOUSE_VIEW_SENS_VALUE, m_pHScroll[OGHS_MOUSE_VIEW_SENS]->GetScrollPosWZ() );
		m_pHScroll[OGHS_MOUSE_VIEW_SENS]->SetScrollRangeWZ(1, 5);
		m_pHScroll[OGHS_MOUSE_VIEW_SENS]->SetScrollPosWZ(GAMEOPTION->GetMouseViewSens());
	}

	if (m_pHScroll[OGHS_MOUSE_VIEW_SPEED])
	{
        m_OptionParam.SetOptionValue(nOptionType::MOUSE_VIEW_SPEED_VALUE, m_pHScroll[OGHS_MOUSE_VIEW_SPEED]->GetScrollPosWZ() );
		m_pHScroll[OGHS_MOUSE_VIEW_SPEED]->SetScrollRangeWZ(1, 5);
		m_pHScroll[OGHS_MOUSE_VIEW_SPEED]->SetScrollPosWZ(GAMEOPTION->GetMouseViewSpeed());
	}

    if (m_pHScroll[OGHS_CAMERA_VIEW_POS])
    {
        m_OptionParam.SetOptionValue(nOptionType::CAMERA_VIEW_HEIGHT_OFFSET, m_pHScroll[OGHS_CAMERA_VIEW_POS]->GetScrollPosWZ() );
        m_pHScroll[OGHS_CAMERA_VIEW_POS]->SetScrollRangeWZ(0, 2);
        m_pHScroll[OGHS_CAMERA_VIEW_POS]->SetScrollPosWZ(GAMEOPTION->GetCameraViewPos());
    }
}

void OptionGameDialog::_OnApplyChangedOption( nOptionType::eOptionTypeIndex TypeIndex, int NewOptionValue )
{
    if( TypeIndex == nOptionType::USE_HELP )
    {
        if( uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP)) )
        {
            helpMan->ShowHelpMenu( NewOptionValue, TRUE );
        }
    }
    else if( TypeIndex == nOptionType::SHOW_PLAYER_NAME )
    {
        if( NewOptionValue )
        {
            m_pCheckBox[OGCB_GRADE]->ShowWindowWZ( WZ_SHOW );
            m_pCheckBox[OGCB_GUILD]->ShowWindowWZ( WZ_SHOW );
        }
        else
        {
            m_pCheckBox[OGCB_GRADE]->ShowWindowWZ( WZ_HIDE );
            m_pCheckBox[OGCB_GUILD]->ShowWindowWZ( WZ_HIDE );
        }
    }
	else if( TypeIndex == nOptionType::SHOW_HELMET )
    {
        if( g_pHero )
        {
            g_pHero->HideHelmet( !NewOptionValue );

            MSG_CG_STATUS_RENDER_OPTION_SYN msg;

            msg.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_HELMET_OPTION;
            msg.m_byOptionLevel = NewOptionValue ? 0 : 1;

            g_pHero->SendPacket(&msg,sizeof(msg));
        }
        else
        {
            xGetHeroData()->SetHideHelmet( !NewOptionValue );
        }
    }
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    else if( TypeIndex == nOptionType::HIDE_COSTUME )
    {
        if( g_pHero )
        {
            g_pHero->HideCostume( !NewOptionValue );

            MSG_CG_STATUS_RENDER_OPTION_SYN msg;

            msg.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_COSTUME_OPTION;
            msg.m_byOptionLevel = NewOptionValue ? 0 : 1;

            g_pHero->SendPacket(&msg,sizeof(msg));
        }
		else
		{
			xGetHeroData()->SetHideCostume( !NewOptionValue );
		}
    }
#endif
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    else if( TypeIndex == nOptionType::SHOW_WING )
    {
        if( g_pHero )
        {
            g_pHero->HideWing( !NewOptionValue );

            MSG_CG_STATUS_RENDER_OPTION_SYN msg;

            msg.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_WING_OPTION;
            msg.m_byOptionLevel = NewOptionValue ? 0 : 1;

            g_pHero->SendPacket(&msg,sizeof(msg));
        }
        else
        {
            xGetHeroData()->SetHideWing( !NewOptionValue );
        }
    }
#endif //_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    else if( TypeIndex == nOptionType::USE_MOVE_TO_CAMERA )
    {
        g_HeroInput.SetCameraBaseMove( NewOptionValue );
    }
    else if( TypeIndex == nOptionType::USE_BLOCK_WHISPER )
    {
        Send_CG_STATUS_INTERCEPTION_SYN(eINTERCEPTION_WHISPER, NewOptionValue);
    }
    else if( TypeIndex == nOptionType::USE_BLOCK_TRADE )
    {
        Send_CG_STATUS_INTERCEPTION_SYN(eINTERCEPTION_TRADE, NewOptionValue);
    }
    else if( TypeIndex == nOptionType::USE_CHANGE_EYE_MOUSE )
    {
        g_Input.SetMouseChange( NewOptionValue );
    }
    else if( TypeIndex == nOptionType::USE_FIXED_TARGET )
    {
        g_HeroInput.SetFixTarget( NewOptionValue );
    }
    else if( TypeIndex == nOptionType::USE_FORWARD_TARGET )
    {
        g_HeroInput.SetForwardTargeting( NewOptionValue );
    }
    else if( TypeIndex == nOptionType::USE_AUTO_TARGET )
    {
        g_HeroInput.SetAutoTarget( NewOptionValue );
    }
}

BOOL OptionGameDialog::Apply( BOOL bCheckApply ,BOOL bProgramInit)
{
    // 
    for( int i=0;i!=nOptionType::MAX;++i)
    {
        nOptionType::eOptionTypeIndex TypeIndex = (nOptionType::eOptionTypeIndex)i;

        if( bCheckApply == FALSE )
        {
            _OnApplyChangedOption( TypeIndex, m_OptionParam.GetOptionValue(TypeIndex) );
        }
        else if( m_OptionParam.GetOptionValue(TypeIndex) != m_InitOptionParam.GetOptionValue(TypeIndex) )
        {
            _OnApplyChangedOption( TypeIndex, m_OptionParam.GetOptionValue(TypeIndex) );
        }
    }

    //
	g_InterfaceManager.SetDiplayDropItem( FALSE );

    // Copy
	OptionParam tempOption = GAMEOPTION->GetGameOption();

    tempOption.SetOptionValue( nOptionType::USE_AUTO_MOVE,                  m_OptionParam.GetOptionValue( nOptionType::USE_AUTO_MOVE ) );                
    tempOption.SetOptionValue( nOptionType::USE_AUTO_TARGET,                m_OptionParam.GetOptionValue( nOptionType::USE_AUTO_TARGET ) );              
    tempOption.SetOptionValue( nOptionType::USE_CHANGE_EYE_MOUSE,           m_OptionParam.GetOptionValue( nOptionType::USE_CHANGE_EYE_MOUSE ) );         
    tempOption.SetOptionValue( nOptionType::USE_FIXED_TARGET,               m_OptionParam.GetOptionValue( nOptionType::USE_FIXED_TARGET ) );             
    tempOption.SetOptionValue( nOptionType::USE_FIXED_UI_POSITION,          m_OptionParam.GetOptionValue( nOptionType::USE_FIXED_UI_POSITION ) );        
    tempOption.SetOptionValue( nOptionType::USE_FORWARD_TARGET,             m_OptionParam.GetOptionValue( nOptionType::USE_FORWARD_TARGET ) );           
    tempOption.SetOptionValue( nOptionType::USE_HELP,                       m_OptionParam.GetOptionValue( nOptionType::USE_HELP ) );                     
    tempOption.SetOptionValue( nOptionType::USE_MOVE_TO_CAMERA,             m_OptionParam.GetOptionValue( nOptionType::USE_MOVE_TO_CAMERA ) );           
    tempOption.SetOptionValue( nOptionType::SHOW_PLAYER_GRADE,              m_OptionParam.GetOptionValue( nOptionType::SHOW_PLAYER_GRADE ) );            
    tempOption.SetOptionValue( nOptionType::SHOW_PLAYER_GUILD,              m_OptionParam.GetOptionValue( nOptionType::SHOW_PLAYER_GUILD ) );            
    tempOption.SetOptionValue( nOptionType::SHOW_PLAYER_PETNAME,            m_OptionParam.GetOptionValue( nOptionType::SHOW_PLAYER_PETNAME ) );          
#ifdef __CN_ADD_HEAD_EFFECT_FLAG
    tempOption.SetOptionValue( nOptionType::SHOW_HEAD_EFFECT,               m_OptionParam.GetOptionValue( nOptionType::SHOW_HEAD_EFFECT ) );             
#endif // __CN_ADD_HEAD_EFFECT_FLAG
    tempOption.SetOptionValue( nOptionType::SHOW_HELMET,                    m_OptionParam.GetOptionValue( nOptionType::SHOW_HELMET ) );                  
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    tempOption.SetOptionValue( nOptionType::SHOW_WING,                      m_OptionParam.GetOptionValue( nOptionType::SHOW_WING ) );                  
#endif //_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    tempOption.SetOptionValue( nOptionType::SHOW_NPC_NAME,                  m_OptionParam.GetOptionValue( nOptionType::SHOW_NPC_NAME ) );                
    tempOption.SetOptionValue( nOptionType::SHOW_PLAYER_NAME,               m_OptionParam.GetOptionValue( nOptionType::SHOW_PLAYER_NAME ) );             
    tempOption.SetOptionValue( nOptionType::USE_SKILLQUEUEING,              m_OptionParam.GetOptionValue( nOptionType::USE_SKILLQUEUEING ) );            
    tempOption.SetOptionValue( nOptionType::MOUSE_SENS_VALUE,               m_OptionParam.GetOptionValue( nOptionType::MOUSE_SENS_VALUE ) );             
    tempOption.SetOptionValue( nOptionType::MOUSE_VIEW_SENS_VALUE,          m_OptionParam.GetOptionValue( nOptionType::MOUSE_VIEW_SENS_VALUE ) );        
    tempOption.SetOptionValue( nOptionType::MOUSE_VIEW_SPEED_VALUE,         m_OptionParam.GetOptionValue( nOptionType::MOUSE_VIEW_SPEED_VALUE ) );       
    tempOption.SetOptionValue( nOptionType::DAMAGE_TEXT_OUTPUT,             m_OptionParam.GetOptionValue( nOptionType::DAMAGE_TEXT_OUTPUT ) );           
    tempOption.SetOptionValue( nOptionType::SHOW_VENDOR_TITLE,              m_OptionParam.GetOptionValue( nOptionType::SHOW_VENDOR_TITLE ) );            
    tempOption.SetOptionValue( nOptionType::USE_MULTITHREAD,                m_OptionParam.GetOptionValue( nOptionType::USE_MULTITHREAD ) );              
    tempOption.SetOptionValue( nOptionType::USE_PLAY_MOVIE,                 m_OptionParam.GetOptionValue( nOptionType::USE_PLAY_MOVIE ) );               
    tempOption.SetOptionValue( nOptionType::USE_EXTENSION_CAMERA_DISTANCE,  m_OptionParam.GetOptionValue( nOptionType::USE_EXTENSION_CAMERA_DISTANCE ) );
    tempOption.SetOptionValue( nOptionType::USE_SPRING_CAMERA,              m_OptionParam.GetOptionValue( nOptionType::USE_SPRING_CAMERA ) );            
    tempOption.SetOptionValue( nOptionType::USE_BLOCK_WHISPER,              m_OptionParam.GetOptionValue( nOptionType::USE_BLOCK_WHISPER ) );            
    tempOption.SetOptionValue( nOptionType::USE_BLOCK_TRADE,                m_OptionParam.GetOptionValue( nOptionType::USE_BLOCK_TRADE ) );              
    tempOption.SetOptionValue( nOptionType::SHOW_MONSTERINFO,               m_OptionParam.GetOptionValue( nOptionType::SHOW_MONSTERINFO ) );             
    tempOption.SetOptionValue( nOptionType::SHOW_MY_RENDER_INFO,            m_OptionParam.GetOptionValue( nOptionType::SHOW_MY_RENDER_INFO ) );          
#ifdef _YJ_PK_OPTION_001120
    tempOption.SetOptionValue( nOptionType::USE_PK_WITH_CONTROL_KEY,        m_OptionParam.GetOptionValue( nOptionType::USE_PK_WITH_CONTROL_KEY ) );      
#endif //_YJ_PK_OPTION_001120
    tempOption.SetOptionValue( nOptionType::NOTIFY_PICKUP_ITEM,             m_OptionParam.GetOptionValue( nOptionType::NOTIFY_PICKUP_ITEM ) );           
    tempOption.SetOptionValue( nOptionType::NOTIFY_DROP_ITEM,               m_OptionParam.GetOptionValue( nOptionType::NOTIFY_DROP_ITEM ) );             
    tempOption.SetOptionValue( nOptionType::SHOW_VENDERS,                   m_OptionParam.GetOptionValue( nOptionType::SHOW_VENDERS ) );                 
    tempOption.SetOptionValue( nOptionType::SHOW_PLAYERS,                   m_OptionParam.GetOptionValue( nOptionType::SHOW_PLAYERS ) );                 
    tempOption.SetOptionValue( nOptionType::USE_BLOCK_CHANNEL_INVITE,       m_OptionParam.GetOptionValue( nOptionType::USE_BLOCK_CHANNEL_INVITE ) );     
#ifdef _DH_BATTLEZONE2_
    tempOption.SetOptionValue( nOptionType::PICKING_FRIEND,                 m_OptionParam.GetOptionValue( nOptionType::PICKING_FRIEND ) );               
#endif //_DH_BATTLEZONE2_

    tempOption.SetOptionValue( nOptionType::CAMERA_VIEW_HEIGHT_OFFSET,      m_OptionParam.GetOptionValue( nOptionType::CAMERA_VIEW_HEIGHT_OFFSET ) );               
    //m_OptionParam.camera_view_pos = min(m_OptionParam.camera_view_pos, 2);
    //tempOption.camera_view_pos = m_OptionParam.camera_view_pos;
    //g_Camera.MoveUpDown(GetCameraPos(tempOption.camera_view_pos));


    // Save
	m_InitOptionParam = tempOption;
	GAMEOPTION->SetGameOption( tempOption );
    
	if( bProgramInit == FALSE )
	{
		GAMEOPTION->Save();
	}

	GlobalFunc::ApplayOptionShowPlayers();
	return TRUE;
}



void OptionGameDialog::Send_CG_STATUS_INTERCEPTION_SYN(eINTERCEPTION_TYPE eType, BOOL bFlag)
{
	MSG_CG_STATUS_INTERCEPTION_SYN sendPacket;
	sendPacket.m_byInterceptionCode = eType;
	sendPacket.m_bRequestFlag = bFlag;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sendPacket, sizeof (sendPacket));
}

void OptionGameDialog::InitBlockCheck( BOOL bTradeBlock, BOOL bWhisperBlock )
{
	if (m_pCheckBox[OGCB_TRADE_BLOCK])
	{
		m_pCheckBox[OGCB_TRADE_BLOCK]->SetCheckState( bTradeBlock );
        m_InitOptionParam.SetOptionValue( nOptionType::USE_BLOCK_TRADE, bTradeBlock );
        m_OptionParam.SetOptionValue( nOptionType::USE_BLOCK_TRADE, bTradeBlock );

	}

	if (m_pCheckBox[OGCB_WHISPER_BLOCK])
	{
		m_pCheckBox[OGCB_WHISPER_BLOCK]->SetCheckState( bWhisperBlock );
        m_InitOptionParam.SetOptionValue( nOptionType::USE_BLOCK_WHISPER, bWhisperBlock );
        m_OptionParam.SetOptionValue( nOptionType::USE_BLOCK_WHISPER, bWhisperBlock );
	}
}

void OptionGameDialog::SetBlockCheck( eINTERCEPTION_TYPE eType, BOOL bFlag )
{
	if( eType == eINTERCEPTION_TRADE )
	{
		if (m_pCheckBox[OGCB_TRADE_BLOCK])
		{
			m_pCheckBox[OGCB_TRADE_BLOCK]->SetCheckState( bFlag );
            m_OptionParam.SetOptionValue( nOptionType::USE_BLOCK_TRADE, bFlag );
		}
	}
	else
	{
		if (m_pCheckBox[OGCB_WHISPER_BLOCK])
		{
			m_pCheckBox[OGCB_WHISPER_BLOCK]->SetCheckState( bFlag );
            m_OptionParam.SetOptionValue( nOptionType::USE_BLOCK_WHISPER, bFlag );
		}
	}
}

void OptionGameDialog::SetHelmetInfo(BOOL bFlag) 
{
    m_InitOptionParam.SetOptionValue( nOptionType::SHOW_HELMET, bFlag );
    m_OptionParam.SetOptionValue( nOptionType::SHOW_HELMET, bFlag );
}

#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
void OptionGameDialog::SetWingInfo(BOOL bFlag)
{
    m_InitOptionParam.SetOptionValue( nOptionType::SHOW_WING, bFlag );
    m_OptionParam.SetOptionValue( nOptionType::SHOW_WING, bFlag );
}
#endif //_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING


#ifdef _DH_OS_LANGUAGETYPE
/*DWORD OptionGameDialog::GetLanguageStringID()
{
	//OPTION_GAME_TEXT_LANGUAGE,
	//OPTION_GAME_LIST_LANGUAGE,
	
}*/
#endif//_DH_OS_LANGUAGETYPE
#endif

#pragma warning ( pop )
