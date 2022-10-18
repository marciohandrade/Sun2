#include "SunClientPrecompiledHeader.h"
#include "createcharcustomdialog.h"
#include "CharacterScene.h" 
#include "uiCreateCharacterMan/uiCreateCharacterMan.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "HeroHeightInfoParser.h"
#include "InterfaceManager.h"
#include "ItemInfoParser.h"
#include "EquipmentContainer.h"
#include "CreateCharRotateButton.h"
#include "GameConst_Color.h"
#include "ObjectManager.h"
#include "Player.h"
#include "SCItemSlot.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiSelectClass2/uiSelectClass2.h"
#else
#include "uiSelectClass/uiSelectClass.h"
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
#include "uiCreateCharacterMan/CreateCharRaceDialog.h"
#include "uiCreateCharacterMan/CreateCharClassDialog.h"
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

WzID CreateCharCustomDialog::m_wzId[MAX_CONTROL] = 
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    StrToWzID("E000"),
    StrToWzID("B042"),
    
    StrToWzID("C000"),
    StrToWzID("C001"),
    StrToWzID("C002"),
    StrToWzID("C003"),
    StrToWzID("C004"),
    StrToWzID("C005"),

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    StrToWzID("C014"),
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    StrToWzID("C015"),
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    StrToWzID("C007"),
    StrToWzID("C008"),
    StrToWzID("C009"),
    StrToWzID("C010"),
    StrToWzID("C011"),
    StrToWzID("C012"),

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    StrToWzID("C006"),
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    StrToWzID("C016"),
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    StrToWzID("B000"),
    StrToWzID("B003"),
    StrToWzID("B001"),
    StrToWzID("B004"),
    StrToWzID("B002"),
    StrToWzID("B007"),

    StrToWzID("S006"),
    StrToWzID("S007"),
    StrToWzID("S008"),

    StrToWzID("B009"),
    StrToWzID("B008"),

    StrToWzID("B011"),
    StrToWzID("B010"),

    StrToWzID("S009"),
    StrToWzID("S010"),

    StrToWzID("S003"),
#else
	StrToWzID("E000"),
	StrToWzID("BT00"),
    StrToWzID("C001"), //BERSERKER_CLASS_CHECK
    StrToWzID("C002"), //DRAGON_CLASS_CHECK
    StrToWzID("C003"), //SHADOW_CLASS_CHECK
    StrToWzID("C004"), //VALKYRIE_CLASS_CHECK
    StrToWzID("C005"), //ELEMENTALIST_CLASS_CHECK
    StrToWzID("B000"),	
    StrToWzID("B001"),	
    StrToWzID("B002"),	
    StrToWzID("B003"),	
    StrToWzID("B004"),	
    StrToWzID("B005"),	

    StrToWzID("T021"),
    StrToWzID("T022"),
    StrToWzID("T023"),

    StrToWzID("BT10"),
    StrToWzID("BT04"),

    StrToWzID("BT11"),
    StrToWzID("B009"),

    StrToWzID("ST10"),
    StrToWzID("ST11"),

    StrToWzID("BT01"),
    StrToWzID("BT12"),

    StrToWzID("T024"),
    StrToWzID("T001"),
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
};


CreateCharCustomDialog::CreateCharCustomDialog( InterfaceManager * pUIman)
	:uiBase(pUIman)
	,m_WeaponPos( 0 )
	,m_ArmorPos( 0 )
    ,background_texture_(INVALID_HANDLE_VALUE)
    ,draw_base_(NULL)
#ifdef _NA_000000_20130114_RENEWER_UI
	,render_over_texture_(INVALID_HANDLE_VALUE)
#endif
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    ,render_over_texture2_(INVALID_HANDLE_VALUE)
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
{
    first_setting_reset();

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    is_now_char_Type_ = eCHAR_WITCHBLADE;
#else
    is_now_char_Type_ = eCHAR_HELLROID;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}

CreateCharCustomDialog::~CreateCharCustomDialog(void)
{
}

VOID CreateCharCustomDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	CCtrlDxEditBox * pEdit = (CCtrlDxEditBox *)getControl( NAME_EDIT );
	if( pEdit )
	{
		pEdit->SetLimitText( MAX_CHARNAME_LENGTH );
		pEdit->ClearText();
	}

	m_pWeaponName	= (CCtrlStaticWZ*)getControl( WEAPON_TEXT ); 
	m_pArmorName	= (CCtrlStaticWZ*)getControl( ARMOR_TEXT );

    // ui로딩중에 텍스처Dlg3_6을 사용하는 컨트롤이 없어 텍스처Dlg3_6이 로드되지 않고, 후에 캐릭터생성화면 중
    // uiLoginBackTop::RenderClassMark() 에서 의도치 않은 타입으로 텍스처Dlg3_6가 로드되어 
    // 텍스처가 비정상적으로 출력되는 현상이 있었습니다. 여기서 미리 로드를 해줍니다.
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    if (pDrawBase && (background_texture_ == INVALID_HANDLE_VALUE))
    {
        draw_base_ = pDrawBase;
        background_texture_ = pDrawBase->x_pManagerTexture->LoadTexture("Data\\Interface\\Dlg3_6.tga",
            TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    }
#endif

#ifdef _NA_000000_20130114_RENEWER_UI

    for (int i = 0 ; i < CREATE_CLASS_COUNT; ++i) // 헬로이드는 좌표가 틀리다...
	{
		render_over_iamge_[i].is_over_mouse = false;

		render_over_iamge_[i].image_source_.left = 374+ (i*40);
		render_over_iamge_[i].image_source_.top = 411 ;
		render_over_iamge_[i].image_source_.right = 40;
		render_over_iamge_[i].image_source_.bottom = 40;
        render_over_iamge_[i].screen_position = CHECK_RACE1 + i;

        render_over_iamge_[CREATE_CLASS_COUNT+i].is_over_mouse = false;

        render_over_iamge_[CREATE_CLASS_COUNT+i].image_source_.left = 614+ ((i-6)*40);
        render_over_iamge_[CREATE_CLASS_COUNT+i].image_source_.top = 411 ;
        render_over_iamge_[CREATE_CLASS_COUNT+i].image_source_.right = 40;
        render_over_iamge_[CREATE_CLASS_COUNT+i].image_source_.bottom = 40;
        render_over_iamge_[CREATE_CLASS_COUNT+i].screen_position = CHECK_CLASS_BERSERKER + i;
	}

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    // 헬로이드는 좌표 수동으로 지정
    render_over_iamge_[6].is_over_mouse = false;
    render_over_iamge_[6].image_source_.left = 947;
    render_over_iamge_[6].image_source_.top = 588;
    render_over_iamge_[6].image_source_.right = 40;
    render_over_iamge_[6].image_source_.bottom = 40;
    render_over_iamge_[6].screen_position = CHECK_RACE7;

    render_over_iamge_[CREATE_CLASS_COUNT + 6].is_over_mouse = false;

    render_over_iamge_[CREATE_CLASS_COUNT + 6].image_source_.left = 947;
    render_over_iamge_[CREATE_CLASS_COUNT + 6].image_source_.top = 748;
    render_over_iamge_[CREATE_CLASS_COUNT + 6].image_source_.right = 40;
    render_over_iamge_[CREATE_CLASS_COUNT + 6].image_source_.bottom = 40;
    render_over_iamge_[CREATE_CLASS_COUNT + 6].screen_position = CHECK_CLASS_HELLROID;
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    // 위치블레이드는 좌표 수동으로 지정
    render_over_iamge_[7].is_over_mouse = false;
    render_over_iamge_[7].image_source_.left = 748;
    render_over_iamge_[7].image_source_.top = 280;
    render_over_iamge_[7].image_source_.right = 40;
    render_over_iamge_[7].image_source_.bottom = 40;
    render_over_iamge_[7].screen_position = CHECK_RACE8;

    render_over_iamge_[CREATE_CLASS_COUNT + 7].is_over_mouse = false;

    render_over_iamge_[CREATE_CLASS_COUNT + 7].image_source_.left = 788;
    render_over_iamge_[CREATE_CLASS_COUNT + 7].image_source_.top = 280;
    render_over_iamge_[CREATE_CLASS_COUNT + 7].image_source_.right = 40;
    render_over_iamge_[CREATE_CLASS_COUNT + 7].image_source_.bottom = 40;
    render_over_iamge_[CREATE_CLASS_COUNT + 7].screen_position = CHECK_CLASS_HELLROID;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	if (render_over_texture_==INVALID_HANDLE_VALUE)
	{
		//너무 긴 라인 줄임
		render_over_texture_ = g_pSunRenderer->x_pManagerTexture->\
			LoadTexture("Data\\Interface\\Common_bg01.tga" 
			,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}

#endif

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (render_over_texture2_ == INVALID_HANDLE_VALUE)
    {
        render_over_texture2_ = g_pSunRenderer->x_pManagerTexture->\
            LoadTexture("Data\\Interface\\Common_bg02.tga" 
            ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}

VOID CreateCharCustomDialog::Release()
{
	m_WeaponItemVec.clear();
	m_ArmorItemVec.clear();

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    if (draw_base_ && (background_texture_ != INVALID_HANDLE_VALUE))
    {
        draw_base_->x_pManagerTexture->DestroyTexture(background_texture_);
        background_texture_ = INVALID_HANDLE_VALUE;
    }
#endif

#ifdef _NA_000000_20130114_RENEWER_UI
	if (render_over_texture_ != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(render_over_texture_);
		render_over_texture_ = INVALID_HANDLE_VALUE;
	}
#endif

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (render_over_texture2_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(render_over_texture2_);
        render_over_texture2_ = INVALID_HANDLE_VALUE;
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

}

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
void CreateCharCustomDialog::OnClickClass(eCHAR_TYPE select_class)
{
    CCtrlButtonCheckWZ* control = NULL;
    for (int i = 0; i < CREATE_CLASS_COUNT; ++i)
    {
        control = static_cast<CCtrlButtonCheckWZ*>(getControl(CHECK_CLASS_BERSERKER+i));
        if (control)
        {
            control->SetCheckState((eCHAR_BERSERKER + CHECK_CLASS_BERSERKER+ i) == (select_class+CHECK_CLASS_BERSERKER));
        }
    }

    CreateCharClassDialog* class_description = static_cast<CreateCharClassDialog*>(GET_DIALOG(IM_CREATE_CHAR_MANAGER::EXPLAIN_CLASS_DIALOG));
    if (class_description)
    {
        class_description->SetClassDescription(select_class);
    }

#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    SelectClass2* select_class_ptr = NULL;
    uiCreateCharacterMan* create_character_manager_ptr = GET_DIALOG_MANAGER(uiCreateCharacterMan , UIMAN_CREATECHARACTER);
    if (create_character_manager_ptr) {
        select_class_ptr = create_character_manager_ptr->get_select_class_ptr();
    }
    if (select_class_ptr) {
        select_class_ptr->set_current_class_code(select_class);
    }
#else
    SelectClass* select_class_ptr = static_cast<SelectClass*>(GET_DIALOG(IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG));
    if (select_class_ptr)
    {
        select_class_ptr->SetCurrentClassCode(select_class);
        select_class_ptr->DisplayClassDescription(select_class);
    }
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
    //GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_CLASS_INFO)->ShowDialog(TRUE);

    DWORD selected_object_key = g_CharSelectScene.GetSelectedObjectKey();
    if (selected_object_key)
    {
        GET_CAST_DIALOG( CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG )->InsertWeaponParts(select_class);
    }

    // Top UI에 클래스명 및 마크 세팅
    uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
    if (login_back_top != NULL)
    {
        DWORD class_name_code = GlobalFunc::GetClassNameCode(select_class);
        login_back_top->SetSceneTitleClassName(select_class, class_name_code);
    }
}

void CreateCharCustomDialog::OnClickRace(eCHAR_TYPE select_race)
{
    CCtrlButtonCheckWZ* control = NULL;
    CCtrlButtonCheckWZ* control_ptr = NULL;
    for (int i = 0; i < CREATE_CLASS_COUNT; ++i)
    {
       control = static_cast<CCtrlButtonCheckWZ*>(getControl(CHECK_RACE1+i));
       if (control)
       {
           control->SetCheckState((eCHAR_BERSERKER+CHECK_RACE1+i) == (select_race+CHECK_RACE1));
       }

       eCHAR_TYPE select_class_type = static_cast<eCHAR_TYPE>(i + eCHAR_BERSERKER );
       control_ptr = static_cast<CCtrlButtonCheckWZ*>(getControl(i + CHECK_CLASS_BERSERKER));
       if (control_ptr)
       {
           if (g_CharSelectScene.CheckEnableSelectClass(select_class_type) == false)
           {   
               control_ptr->EnableWindowWZ(false);
           }
           else
           {
               control_ptr->EnableWindowWZ(true);
           }
       }
    }

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	g_CharSelectScene.ResetCameraPosition( select_race );
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT


 //   g_CharSelectScene.m_camera.x = g_CharSelectScene.GetCameraX(select_race);
 //   g_CharSelectScene.m_target.x = g_CharSelectScene.m_camera.x;
	//g_CharSelectScene.m_zoom = 0.0f;

    CreateCharRaceDialog* race_description = static_cast<CreateCharRaceDialog*>(GET_DIALOG(IM_CREATE_CHAR_MANAGER::EXPLAIN_RACE_DIALOG));
    if (race_description)
    {
        race_description->SetRaceDescription(select_race);
    }

    g_CharSelectScene.GetCreateClassObjectKey(select_race);
    OnClickClass(select_race);
}


#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

VOID CreateCharCustomDialog::MessageProc( SI_MESSAGE * pMessage )
{
    POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

	switch( pMessage->eResultMsg ) 
	{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#ifdef _NA_000000_20130114_RENEWER_UI
		case RT_MSG_MOUSEOVER:
			{
				switch( pos )
				{
				case CHECK_RACE1:
				case CHECK_RACE2:
				case CHECK_RACE3:
				case CHECK_RACE4:
				case CHECK_RACE5:
				case CHECK_RACE6:
				case CHECK_CLASS_BERSERKER:
				case CHECK_CLASS_DRAGON:
				case CHECK_CLASS_SHADOW:
				case CHECK_CLASS_VALKYRIE:
				case CHECK_CLASS_ELEMENTALIST:
				case CHECK_CLASS_MYSTIC:
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
                case CHECK_RACE7:
                case CHECK_CLASS_HELLROID:
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                case CHECK_RACE8:
                case CHECK_CLASS_WITCHBLADE:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
					{
						for (int i = 0 ; i < CREATE_CLASS_COUNT*2 ; ++i)
						{
							if(render_over_iamge_[i].screen_position == pos)
							{
								render_over_iamge_[i].is_over_mouse = true;
							}
							else
							{
								render_over_iamge_[i].is_over_mouse = false;
							}
						}
					}
					break;
				default:
					{
						for (int i = 0 ; i < CREATE_CLASS_COUNT*2 ; ++i)
						{
							render_over_iamge_[i].is_over_mouse = false;
						}
					}
				}
			}
			break;
#endif
#else
        case RT_MSG_MOUSEOVER:
            {
                switch( pos )
                {
                case BERSERKER_CLASS_CHECK:
                case DRAGON_CLASS_CHECK:
                case SHADOW_CLASS_CHECK:
                case VALKYRIE_CLASS_CHECK:
                case ELEMENTALIST_CLASS_CHECK:      
                    {
                        mouse_over_pos_ = pos;
                    }
                break;
				case CHECK_RACE1:
				case CHECK_RACE2:
				case CHECK_RACE3:
				case CHECK_RACE4:
				case CHECK_RACE5:
				case CHECK_RACE6:
					{
						for (int i = 0 ; i < CREATE_CLASS_COUNT ; ++i)
						{
							if(render_over_iamge_[i].screen_position == pos)
							{
								render_over_iamge_[i].is_over_mouse = true;
							}
							else
							{
								render_over_iamge_[i].is_over_mouse = false;
							}
						}
					}
					break;
                default:
                    {
						for (int i = 0 ; i < CREATE_CLASS_COUNT ; ++i)
						{
							render_over_iamge_[i].is_over_mouse = false;
						}

                        pos = INVALID_POSTYPE_VALUE;;
                    }
                }
            }
            break;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	case RT_MSG_LBUTTONCLICK:
		{
			switch (pos)
			{
			case NAMECONFIRM_BTN:
				{
					TCHAR szName[MAX_CHARNAME_LENGTH+MAX_CHARNAME_LENGTH+1] = {0,};	// wchar와 char문제로 max값을 2배로
					ZeroMemory(szName,sizeof(TCHAR));

					GetCharName( szName );
					int iLength =_tcslen(szName);
					TCHAR	szMessage[MAX_MESSAGE_LENGTH + 1];

					if(iLength==0)
					{
						//	캐릭터 이름을 입력하세요
						g_InterfaceManager.GetInterfaceString( eST_INPUT_CHARACTER_NAME, szMessage, MAX_MESSAGE_LENGTH );
						g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
					}
					else if (iLength <4)
					{
						// 이름은 영문 4자 한글 2자 이상 입력해주세요
						g_InterfaceManager.GetInterfaceString(eST_NAME_OVER2_ENG_OVER_4 , szMessage ,INTERFACE_STRING_LENGTH );
						g_InterfaceManager.ShowMessageBox(szMessage , 3000, NULL, 0 );
					}
					else if (iLength > MAX_CHARNAME_LENGTH)
					{
						//	이름은 영문 16자 한글 8자 이내로 입력해 주십시오.
						g_InterfaceManager.GetInterfaceString( eST_CHECK_INPUT_NAME_LENGTH, szMessage, MAX_MESSAGE_LENGTH );
						g_InterfaceManager.ShowMessageBox( szMessage, 3000, NULL, 0 );
					}
					else
					{
						g_CharSelectScene.SendCharacterIdCheckSyn( szName );
					}
				}
				break;
			case HAIR_FORWARD_BTN:
				{
					g_CharSelectScene.OnClickChangeHair( FALSE );
                    first_setting_hair_off();
					UpdateVariationText();
				}
				break;
			case HAIR_BACKWARD_BTN:
				{
					g_CharSelectScene.OnClickChangeHair( TRUE );
                    first_setting_hair_off();
					UpdateVariationText();
				}
				break;
			case FACE_FORWARD_BTN:
				{
					g_CharSelectScene.OnClickChangeFace( FALSE );
                    first_setting_face_off();
					UpdateVariationText();
				}
				break;
			case FACE_BACKWARD_BTN:
				{
					g_CharSelectScene.OnClickChangeFace( TRUE );
                    first_setting_face_off();
					UpdateVariationText();
				}
				break;
			case HEIGHT_FORWARD_BTN:
				{
					g_CharSelectScene.OnClickChangeHeight( FALSE );
                    first_setting_height_off();
					UpdateVariationText();
				}
				break;
			case HEIGHT_BACKWARD_BTN:
				{
					g_CharSelectScene.OnClickChangeHeight( TRUE );
                    first_setting_height_off();
					UpdateVariationText();
				}
				break;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
            case CHECK_RACE1:
            case CHECK_RACE2:
            case CHECK_RACE3:
            case CHECK_RACE4:
            case CHECK_RACE5:
            case CHECK_RACE6:
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            case CHECK_RACE7:
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            case CHECK_RACE8:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                {
					eCHAR_TYPE select_class_code = eCHAR_TYPE(pos - CHECK_RACE1+ eCHAR_BERSERKER);
                    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE                    

					if (is_now_char_Type_ != select_class_code)
					{
						is_now_char_Type_ = select_class_code;

						g_CharSelectScene.OnSelectedCharacter(select_class_code);
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
                        SelectClass2* select_class = NULL;
                        uiCreateCharacterMan* create_character_manager_ptr = GET_DIALOG_MANAGER(uiCreateCharacterMan , UIMAN_CREATECHARACTER);
                        if (create_character_manager_ptr)
                        {
                            select_class = create_character_manager_ptr->get_select_class_ptr();
                        }
#else
						SelectClass* select_class = static_cast<SelectClass*>(GET_DIALOG(IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG));
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
						if (select_class)
						{
							select_class->OnExcute();
							OnClickRace(select_class_code);
						}
					}
                }
                break;
            case CHECK_CLASS_BERSERKER:
            case CHECK_CLASS_DRAGON:
            case CHECK_CLASS_SHADOW:
            case CHECK_CLASS_VALKYRIE:
            case CHECK_CLASS_ELEMENTALIST:
            case CHECK_CLASS_MYSTIC:
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            case CHECK_CLASS_HELLROID:
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            case CHECK_CLASS_WITCHBLADE:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

                {
                    eCHAR_TYPE select_class_code = eCHAR_TYPE(pos - CHECK_CLASS_BERSERKER + eCHAR_BERSERKER);
					first_setting_reset();
					UpdateVariationText();
					OnClickClass(select_class_code);
				
                }
                break;
#else
            case BERSERKER_CLASS_CHECK:
            case DRAGON_CLASS_CHECK:
            case SHADOW_CLASS_CHECK:
            case VALKYRIE_CLASS_CHECK:
            case ELEMENTALIST_CLASS_CHECK:
                {
                    eCHAR_TYPE select_class_type = static_cast<eCHAR_TYPE>(pos - BERSERKER_CLASS_CHECK + 1);
                    // 선택 가능 클래스 확인
                    if (g_CharSelectScene.CheckEnableSelectClass(select_class_type) == true)
                    {
                        // Top UI에 클래스명 및 마크 세팅
                        uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
                        if (login_back_top != NULL)
                        {
                            DWORD class_name_code = GlobalFunc::GetClassNameCode(select_class_type);
                            login_back_top->SetSceneTitleClassName(select_class_type, class_name_code);
                        }

                        // UI 상태 초기화 - 커스터마이즈/장비 관련
                        first_setting_reset();
                        g_CharSelectScene.InitVariation();
                        UpdateVariationText();                        

                        // 선택 가능하면 기존 캐릭터 파츠 해제
                        DeleteParts();

                        // 선택된 클래스 캐릭터로 교체
                        selected_character_object_key_ = g_CharSelectScene.ChangeCreateClass(select_class_type);
                        LoadParts(selected_character_object_key_);
                    }                    
                }
                break;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
			case WEAPON_FORWARD_BTN:
				{
                    first_setting_weapon_off();

					m_WeaponPos++;

					if( m_WeaponPos > (int)(m_WeaponItemVec.size() - 1) )
						m_WeaponPos = 0;

					m_pWeaponName->SetTextWZ( (TCHAR*)m_WeaponItemVec[m_WeaponPos].name.c_str() );
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT                
                    DeleteParts();
                    InsertParts();
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
				}
				break;
			case WEAPON_BACKWARD_BTN:
				{
                    first_setting_weapon_off();

					m_WeaponPos--;

					if( m_WeaponPos < 0 )
						m_WeaponPos = m_WeaponItemVec.size() - 1;

					m_pWeaponName->SetTextWZ( (TCHAR*)m_WeaponItemVec[m_WeaponPos].name.c_str() );
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT                
                    DeleteParts();
                    InsertParts();
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
				}
				break;

			case ARMOR_FORWARD_BTN:
				{
                    first_setting_armor_off();

					m_ArmorPos++;

					if( m_ArmorPos > (int)(m_ArmorItemVec.size() - 1) )
						m_ArmorPos = 0;

					m_pArmorName->SetTextWZ( (TCHAR*)m_ArmorItemVec[m_ArmorPos].name.c_str() );
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT                
                    DeleteParts();
                    InsertParts();
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
                }
				break;
			case ARMOR_BACKWARD_BTN:
				{
                    first_setting_armor_off();

					m_ArmorPos--;

					if( m_ArmorPos < 0 )
						m_ArmorPos = m_ArmorItemVec.size() - 1;

					m_pArmorName->SetTextWZ( (TCHAR*)m_ArmorItemVec[m_ArmorPos].name.c_str() );
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT                
                    DeleteParts();
                    InsertParts();
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
				}
				break;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
			case WEAR_BTN:
				{
					DeleteParts();
					InsertParts();
				}
				break;
			case INIT_BTN:
				{
					DeleteParts();
                    InsertWeaponParts(static_cast<eCHAR_TYPE>(0));
				}
				break;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
			}
		}
		break;
					
	}
}

BOOL CreateCharCustomDialog::InsertWeaponParts(eCHAR_TYPE select_class_type)
{
    static eCHAR_TYPE selected_class_type = eCHAR_NONE;
    if (select_class_type != eCHAR_NONE)
    {
        selected_class_type = select_class_type;
    }
    else if (selected_class_type == eCHAR_NONE)
    {
        return FALSE;
    }

    // 선택된 클래스 캐릭터로 교체
    selected_character_object_key_ = g_CharSelectScene.GetCreateClassObjectKey(selected_class_type);
    LoadParts(selected_character_object_key_);

    g_CharSelectScene.ReleaseAllParts(selected_character_object_key_);
    g_CharSelectScene.ChangeSelectCharacter(selected_character_object_key_);

    Player* customize_character = static_cast<Player*>(g_ObjectManager.GetObject(selected_character_object_key_));
    if (customize_character == NULL)
    {
        return FALSE;
    }

    customize_character->SetHeightVariation(2);
    customize_character->SetPartVariation(PLAYER_VARIATION_FACE,1);
    customize_character->SetPartVariation(PLAYER_VARIATION_HAIR,1);

    SCSlotContainer* equip_container = customize_character->GetEquipmentContainer();
    if (equip_container == NULL)
    {
        return FALSE;
    }

    // 장비 장착
    SCItemSlot weapon_item_slot;
    // 무기장착
    weapon_item_slot.SetCode(static_cast<SLOTCODE>(m_WeaponItemVec[m_WeaponPos].itemcode));
    equip_container->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, weapon_item_slot);

    // 애니매이션 세팅
    char animation_id[5] = {'\0', };
    animation_id[0] = 'N';
    animation_id[1] = customize_character->GetWeaponAnimationCode();
    animation_id[2] = '0';
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    animation_id[3] = '1';
#else
    animation_id[3] = '3';
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    customize_character->SetAnimation(StrToWzID(animation_id), TRUE);

    return TRUE;
}

BOOL CreateCharCustomDialog::InsertParts()
{
	const POSTYPE pos[MAX_ARMORCODE] = { 
		EquipmentContainer::EQUIP_ARMOR,
		EquipmentContainer::EQUIP_PROTECTOR,
		EquipmentContainer::EQUIP_HELMET,
		EquipmentContainer::EQUIP_PANTS,
		EquipmentContainer::EQUIP_BOOTS,
		EquipmentContainer::EQUIP_GLOVE,
		EquipmentContainer::EQUIP_BELT,
		EquipmentContainer::EQUIP_SHIRTS,
	};

    Player* customize_character = static_cast<Player*>(g_ObjectManager.GetObject(selected_character_object_key_));
    if (customize_character == NULL)
    {
        return FALSE;
    }

    SCSlotContainer* equip_container = customize_character->GetEquipmentContainer();
    if (equip_container == NULL)
    {
        return FALSE;
    }

    // 장비 장착
    SCItemSlot weapon_item_slot;
    // 무기장착
    weapon_item_slot.SetCode(static_cast<SLOTCODE>(m_WeaponItemVec[m_WeaponPos].itemcode));
    equip_container->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, weapon_item_slot);
    // 방어구파츠장착
    for (int index = 0; index < MAX_ARMORCODE; ++index)
    {
        SCItemSlot armor_item_slot;
        armor_item_slot.SetCode(static_cast<SLOTCODE>(m_ArmorItemVec[m_ArmorPos].itemcode[index]));
        equip_container->InsertSlot(pos[index], armor_item_slot);
    }

    // 애니매이션 세팅
    char animation_id[5] = {'\0', };
    animation_id[0] = 'N';
    animation_id[1] = customize_character->GetWeaponAnimationCode();
    animation_id[2] = '0';
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    animation_id[3] = '1';
#else
    animation_id[3] = '3';
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    customize_character->SetAnimation(StrToWzID(animation_id), TRUE);

    return TRUE;
}

BOOL CreateCharCustomDialog::DeleteParts()
{
    g_CharSelectScene.ReleaseAllParts(selected_character_object_key_);
	
	return TRUE;
}


VOID CreateCharCustomDialog::OnUpdateSolarDialog()
{
    CCtrlDxEditBox * pEdit = (CCtrlDxEditBox*) getControl( NAME_EDIT );
    if (pEdit != NULL)
    {
        TCHAR szName[MAX_PATH] = {NULL, };
        pEdit->GetTextWZ(szName, MAX_PATH);
        if (_tcslen(szName) > 0)
        {
            first_setting_name_off();
            getControl(NAME_TEXT)->ShowWindowWZ(WZ_HIDE);
        }
    }
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
    if (mouse_over_pos_ != INVALID_POSTYPE_VALUE)
    {
        CControlWZ* control = getControl(mouse_over_pos_);
        if (control != NULL)
        {
            int mouse_pos_x = Mouse::Instance()->GetMouseX();
            int mouse_pos_y = Mouse::Instance()->GetMouseY();
            RECT control_rect = control->GetSizeRect();
            if ((control_rect.left <= mouse_pos_x) && 
                (control_rect.right >= mouse_pos_x) && 
                (control_rect.top <= mouse_pos_y) && 
                (control_rect.bottom >= mouse_pos_y))
            {
                uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
                if (tooltip_manager != NULL)
                {
                    eCHAR_TYPE select_class_type = static_cast<eCHAR_TYPE>(mouse_over_pos_ - BERSERKER_CLASS_CHECK + 1);
                    DWORD class_name_code = GlobalFunc::GetClassNameCode(select_class_type);
                    TCHAR class_name[INTERFACE_STRING_LENGTH] = {'\0', };
                    g_InterfaceManager.GetInterfaceString(class_name_code, class_name);
                    tooltip_manager->RegisterTooltip(class_name);
					tooltip_manager->SetTooltipDialogID( GetDialogKey() );
                }
            }
            else
            {
                mouse_over_pos_ = INVALID_POSTYPE_VALUE;
            }
        }
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

VOID CreateCharCustomDialog::OnShowWindow( BOOL val )
{
	if( val )
	{
		first_setting_reset();

        mouse_over_pos_ = 5;

		UpdateVariationText();
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        is_now_char_Type_ = eCHAR_WITCHBLADE;
        CCtrlButtonCheckWZ* control = NULL;
        control = static_cast<CCtrlButtonCheckWZ*>(getControl(CHECK_CLASS_WITCHBLADE));

        if ( control != NULL )
        {
            control->EnableWindowWZ(TRUE);
            control->SetCheckState(TRUE);
        }
#else
  #ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        is_now_char_Type_ = eCHAR_HELLROID;

        CCtrlButtonCheckWZ* control = NULL;
        control = static_cast<CCtrlButtonCheckWZ*>(getControl(CHECK_CLASS_HELLROID));

        if ( control != NULL )
        {
            control->EnableWindowWZ(TRUE);
            control->SetCheckState(TRUE);
        }
  #endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
		GET_CAST_DIALOG( CreateCharRotateButton, IM_CREATE_CHAR_MANAGER::CREATECHAR_ROTATEBUTTON )->SetZoomButton( TRUE );
#endif//_NA_000000_20130812_NEW_CHARACTER_HELLROID

	}
    else
    {
        first_setting_reset();
    }
}

CControlWZ * CreateCharCustomDialog::getControl( POSTYPE AtPos )
{
	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE CreateCharCustomDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < MAX_CONTROL; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return MAX_CONTROL;
}

void CreateCharCustomDialog::SetClass( BYTE byClass )
{

}

void CreateCharCustomDialog::GetCharName( TCHAR * szName, BOOL bClear )
{
	CCtrlDxEditBox * pEdit = (CCtrlDxEditBox*) getControl( NAME_EDIT );

	if( pEdit )
	{
		pEdit->GetTextWZ( szName, MAX_PATH );
		if( bClear )
			pEdit->ClearText();
	}
}

void CreateCharCustomDialog::SetCharName( TCHAR * szName )
{
	CCtrlDxEditBox * pEdit = (CCtrlDxEditBox*) getControl( NAME_EDIT );

	if( pEdit )
	{
		pEdit->SetTextWZ( szName );
	}
	
}


void CreateCharCustomDialog::UpdateVariationText()
{
	CCtrlStaticWZ *pText = NULL;
	pText = (CCtrlStaticWZ *)getControl(HAIR_TEXT);

	TCHAR szString[INTERFACE_STRING_LENGTH];
	TCHAR szText[128];

	int iVariation;

	if (pText)
	{
        if (is_first_setting_hair())
        {
            //200	헤어스타일
            g_InterfaceManager.GetInterfaceString(200, szText, 128);
        }
        else
        {
            iVariation = g_CharSelectScene.GetCurrentHairVariation();

            //	Type %d
            const int	c_iType = 70426;
            g_InterfaceManager.GetInterfaceString( c_iType, szString, INTERFACE_STRING_LENGTH );
            Sprintf(szText,szString,iVariation);
        }
		GlobalFunc::SetCaption( pText, szText );
	}

	
	pText = (CCtrlStaticWZ *)getControl(FACE_TEXT);

	if (pText)
	{
        if (is_first_setting_face())
        {
            //! 195	얼굴
            g_InterfaceManager.GetInterfaceString(195, szText, 128);
        }
        else
        {
		    iVariation = g_CharSelectScene.GetCurrentFaceVariation();

		    //	Type %d
		    const int	c_iType = 70426;
		    g_InterfaceManager.GetInterfaceString( c_iType, szString, INTERFACE_STRING_LENGTH );
		    Sprintf(szText,szString,iVariation);
        }
		GlobalFunc::SetCaption( pText, szText );

	}


	pText = (CCtrlStaticWZ *)getControl(HEIGHT_TEXT);

	if (pText)
	{
        if (is_first_setting_height())
        {
            //! 196	키
            g_InterfaceManager.GetInterfaceString(196, szText, 128);
        }
        else
        {
		    iVariation = g_CharSelectScene.GetCurrentHeightVariation();

		    DWORD dwClass = g_CharSelectScene.GetSelectedClass();

		    float fHeight = 0.0f;

		    BASE_HeroHeightInfo *pInfo = HeroHeightInfoParser::Instance()->GetHeroHeightInfo(dwClass);

		    if (pInfo)
		    {
			    fHeight = pInfo->m_fBase * pInfo->m_fHeight[iVariation];
		    }

		    Sprintf(szText,_T("%.2fm"),fHeight);
        }
		GlobalFunc::SetCaption( pText, szText );
	}
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
    pText = (CCtrlStaticWZ *)getControl(COSTUME_TEXT);

    if (pText)
    {
        ZeroMemory(szText, sizeof(szText));
        if (is_first_setting_costume())
        {
            g_InterfaceManager.GetInterfaceString(576, szText, 128);
        }
        else
        {
        }
        GlobalFunc::SetCaption( pText, szText );
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    if (is_first_setting_name())
    {
        getControl(NAME_TEXT)->ShowWindowWZ(WZ_SHOW);
    }
    else
    {
        getControl(NAME_TEXT)->ShowWindowWZ(WZ_HIDE);
    }
}

BOOL CreateCharCustomDialog::LoadParts(DWORD object_key)
{
	m_WeaponItemVec.clear();
	m_ArmorItemVec.clear();

	m_ArmorPos  = 0;
	m_WeaponPos = 0;

    selected_character_object_key_ = object_key;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
    RefreshClassTabState();
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    
    Player* selected_character = static_cast<Player*>(g_ObjectManager.GetObject(object_key));
    if (selected_character == NULL)
    {
        return FALSE;
    }

    BYTE class_code = selected_character->GetClass();
	HERO_VARI_INFO_MAP_TYPE ArmorItemMap;
	HeroCustomInfoParser::Instance()->GetItemList(class_code, ArmorItemMap);

	HERO_VARI_INFO_MAP_ITR_TYPE itr = ArmorItemMap.begin();
	HERO_VARI_INFO_MAP_ITR_TYPE end = ArmorItemMap.end();

	TCHAR szTemp[MAX_PATH];
	TCHAR szName1[MAX_PATH];
	TCHAR szName2[MAX_PATH];

	BASE_ITEMINFO * pItem = NULL;

	for( ; itr != end; ++itr )
	{
		HERO_VARIATION_INFO info( *(itr->second) );

		for( int i = 0; i < MAX_WEAPONCODE; ++i )
		{
			WEAPON_DISPLAY_TYPE weapon;
			weapon.itemcode = info.weaponcode[i];

			pItem = ItemInfoParser::Instance()->GetItemInfo( weapon.itemcode );

            GameFunc::GetItemName(NULL, pItem, szTemp, MAX_PATH);

			weapon.name = szTemp;

			m_WeaponItemVec.push_back( weapon );
		}

		ARMOR_DISPLAY_TYPE armor;

		for( int j = 0; j < MAX_ARMORCODE; j++ )
		{			
			armor.itemcode[j] = info.armorcode[j];	
		}

		pItem = ItemInfoParser::Instance()->GetItemInfo( armor.itemcode[0] );

		if( pItem != NULL )
		{
			g_InterfaceManager.GetInterfaceString( 1800, szName1, MAX_PATH );
			g_InterfaceManager.GetInterfaceString( pItem->m_NCode, szName2, MAX_PATH );
			
			Sprintf( szTemp, szName1, szName2 );
			armor.name = szTemp;

			m_ArmorItemVec.push_back( armor );
		}
	}

    if (is_first_setting_weapon())
    {
        //! 155417	무기
        g_InterfaceManager.GetInterfaceString( 155417, szName1, MAX_PATH );
        m_pWeaponName->SetTextWZ( szName1 );
    }
    else
    {
        m_pWeaponName->SetTextWZ( (TCHAR*)m_WeaponItemVec[0].name.c_str() );
    }

    if (is_first_setting_armor())
    {
        //! 155418	방어구
        g_InterfaceManager.GetInterfaceString( 155418, szName1, MAX_PATH );
        m_pArmorName->SetTextWZ( szName1 );
    }
    else
    {
        m_pArmorName->SetTextWZ( (TCHAR*)m_ArmorItemVec[0].name.c_str() );
    }
	
	return TRUE;
}

void CreateCharCustomDialog::InitText()
{
	CCtrlDxEditBox * pE = (CCtrlDxEditBox*) getControl( NAME_EDIT );
	if (pE) 
	{
		pE->ClearText();
		pE->SetLimitText( MAX_CHARNAME_LENGTH );
		pE->SetColorCaret(c_InterfaceCaretColor);
	}
}
//------------------------------------------------------------------------------ 
void CreateCharCustomDialog::first_setting_reset()
{
    bool reset_value = true;

    is_first_setting_name_ = reset_value;
    is_first_setting_hair_ = reset_value;
    is_first_setting_face_ = reset_value;
    is_first_setting_height_ = reset_value;
    is_first_setting_costume_ = reset_value;

    is_first_setting_weapon_ = reset_value;
    is_first_setting_armor_ = reset_value;

	
}

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
void CreateCharCustomDialog::RefreshClassTabState()
{
    CCtrlButtonCheckWZ* class_check_button = NULL;
    for (POSTYPE index = BERSERKER_CLASS_CHECK; index < (ELEMENTALIST_CLASS_CHECK + 1); ++index)
    {
        class_check_button = static_cast<CCtrlButtonCheckWZ*>(getControl(index));
        if (class_check_button == NULL)
        {
            continue;
        }

        eCHAR_TYPE class_type = static_cast<eCHAR_TYPE>((index - BERSERKER_CLASS_CHECK) + 1);
        bool enable_class = g_CharSelectScene.CheckEnableSelectClass(class_type);
        bool check_state = (class_type == GetSelectedCharacterClass()) ? true : false;
        if (check_state == true)
        {
            class_check_button->EnableWindowWZ(enable_class);
            class_check_button->SetCheckState(check_state);
        }
        else
        {
            class_check_button->SetCheckState(check_state);
            class_check_button->EnableWindowWZ(enable_class);
        }
    }
}
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

eCHAR_TYPE CreateCharCustomDialog::GetSelectedCharacterClass()
{
    eCHAR_TYPE class_code = eCHAR_NONE;
    Player* selected_character = static_cast<Player*>(g_ObjectManager.GetObject(selected_character_object_key_));
    if (selected_character == NULL)
    {
        return class_code;
    }

    class_code = selected_character->GetClass();
    return class_code;
}

GenderType::Value CreateCharCustomDialog::GetSelectedCharacterGender()
{
    GenderType::Value gender = GenderType::kAll;
    Player* selected_character = static_cast<Player*>(g_ObjectManager.GetObject(selected_character_object_key_));
    if (selected_character == NULL)
    {
        return gender;
    }

    gender = selected_character->GetGender();
    return gender;
}
#ifdef _NA_000000_20130114_RENEWER_UI
void CreateCharCustomDialog::RenderMouseOver()
{
	CCtrlButtonCheckWZ   * pCheckControl = NULL;
	CCtrlButtonCheckWZ   * pCheckControl2 = NULL;

	pCheckControl = (CCtrlButtonCheckWZ  *) getControl(CHECK_RACE1);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    pCheckControl2 = (CCtrlButtonCheckWZ  *) getControl(CHECK_RACE8);
#else
  #ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    pCheckControl2 = (CCtrlButtonCheckWZ  *) getControl(CHECK_RACE7);
  #endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	//첫 컨트롤 버튼과 마지막 컨트롤 버튼의 길이값을 구한다.
	if(pCheckControl && pCheckControl2)
	{
		isFillRectPos.left = pCheckControl->GetSizeRect().left;
		isFillRectPos.top = pCheckControl->GetSizeRect().top;

		isFillRectPos.right = pCheckControl2->GetSizeRect().right;
		isFillRectPos.bottom = pCheckControl2->GetSizeRect().bottom;
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	if ((isFillRectPos.left <= iMouseX) && (isFillRectPos.right >= iMouseX))
	{
		if ((isFillRectPos.top <= iMouseY) && (isFillRectPos.bottom >= iMouseY))
		{
            for (int i = 0; i < CREATE_CLASS_COUNT; ++i)
			{
				CCtrlButtonCheckWZ   * pCheckControl = (CCtrlButtonCheckWZ  *) getControl(render_over_iamge_[i].screen_position);

				if(pCheckControl && pCheckControl->GetCheckState() == false && pCheckControl->GetDrawState() != E_DISABLE)
				{
					if(render_over_iamge_[i].is_over_mouse)
					{
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                        if (i == eCHAR_WITCHBLADE - 1) // 위치블레이드는 Common_bg02.tga에 있어서 마우스 오버 이미지 텍스쳐를 따로 지정 해줘야 한다.
                        {
                            g_pSunRenderer->RenderTexture(render_over_texture2_,
                                (float)pCheckControl->GetSizeRect().left,
                                (float)pCheckControl->GetSizeRect().top,
                                40.0f,
                                40.0f,
                                (float)render_over_iamge_[i].image_source_.left,
                                (float)render_over_iamge_[i].image_source_.top,
                                40.0f,
                                40.0f);
                        }
                        else
                        {
                            g_pSunRenderer->RenderTexture(render_over_texture_,
                                (float)pCheckControl->GetSizeRect().left,
                                (float)pCheckControl->GetSizeRect().top,
                                40.0f,
                                40.0f,
                                (float)render_over_iamge_[i].image_source_.left,
                                (float)render_over_iamge_[i].image_source_.top,
                                40.0f,
                                40.0f);
                        }
#else
						g_pSunRenderer->RenderTexture(render_over_texture_,
							(float)pCheckControl->GetSizeRect().left,
							(float)pCheckControl->GetSizeRect().top,
							40.0f,
							40.0f,
							(float)render_over_iamge_[i].image_source_.left,
							(float)render_over_iamge_[i].image_source_.top,
							40.0f,
							40.0f);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
					}
				}
			}
		}
	}

	pCheckControl = (CCtrlButtonCheckWZ  *) getControl(CHECK_CLASS_BERSERKER);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    pCheckControl2 = (CCtrlButtonCheckWZ  *) getControl(CHECK_CLASS_WITCHBLADE);
#else
  #ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    pCheckControl2 = (CCtrlButtonCheckWZ  *) getControl(CHECK_CLASS_HELLROID);
  #endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	//첫 컨트롤 버튼과 마지막 컨트롤 버튼의 길이값을 구한다.
	if(pCheckControl && pCheckControl2)
	{
		isFillRectPos.left = pCheckControl->GetSizeRect().left;
		isFillRectPos.top = pCheckControl->GetSizeRect().top;

		isFillRectPos.right = pCheckControl2->GetSizeRect().right;
		isFillRectPos.bottom = pCheckControl2->GetSizeRect().bottom;
	}

	if ((isFillRectPos.left <= iMouseX) && (isFillRectPos.right >= iMouseX))
	{
		if ((isFillRectPos.top <= iMouseY) && (isFillRectPos.bottom >= iMouseY))
		{
            for (int i = CREATE_CLASS_COUNT ; i < CREATE_CLASS_COUNT * 2 ; ++i)
			{
				CCtrlButtonCheckWZ   * pCheckControl = (CCtrlButtonCheckWZ  *) getControl(render_over_iamge_[i].screen_position);

				if(pCheckControl && pCheckControl->GetCheckState() == false && pCheckControl->GetDrawState() != E_DISABLE)
				{
					if(render_over_iamge_[i].is_over_mouse)
					{
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                        if (i == (eCHAR_WITCHBLADE - 1) * 2) // // 위치블레이드는 Common_bg02.tga에 있어서 마우스 오버 이미지 텍스쳐를 따로 지정 해줘야 한다.
                        {
                            g_pSunRenderer->RenderTexture(render_over_texture2_,
                                (float)pCheckControl->GetSizeRect().left,
                                (float)pCheckControl->GetSizeRect().top,
                                40.0f,
                                40.0f,
                                (float)render_over_iamge_[i].image_source_.left,
                                (float)render_over_iamge_[i].image_source_.top,
                                40.0f,
                                40.0f);
                        }
                        else
                        {
                            g_pSunRenderer->RenderTexture(render_over_texture_,
                                (float)pCheckControl->GetSizeRect().left,
                                (float)pCheckControl->GetSizeRect().top,
                                40.0f,
                                40.0f,
                                (float)render_over_iamge_[i].image_source_.left,
                                (float)render_over_iamge_[i].image_source_.top,
                                40.0f,
                                40.0f);
                        }
#else
						g_pSunRenderer->RenderTexture(render_over_texture_,
							(float)pCheckControl->GetSizeRect().left,
							(float)pCheckControl->GetSizeRect().top,
							40.0f,
							40.0f,
							(float)render_over_iamge_[i].image_source_.left,
							(float)render_over_iamge_[i].image_source_.top,
							40.0f,
							40.0f);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
					}
				}
			}
		}
	}

}

VOID CreateCharCustomDialog::OnRenderSolarDialog()
{	
	RenderMouseOver();
}
#endif