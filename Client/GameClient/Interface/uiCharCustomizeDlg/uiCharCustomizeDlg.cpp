#include "SunClientPrecompiledHeader.h"
#include "uiCharCustomizeMan/uiCharCustomizeMan.h"
#include "uiCharCustomizeDlg.h"
#include "HeroVariationInfoParser.h"
#include "HeroHeightInfoParser.h"
#include "interfacemanager.h"
#include "ObjectManager.h"
#include "uiAssistMan/uiAssistMan.h"
#include "GameFunc.h"
#include "Mouse.h"
#include "Player.h"
#include "Hero.h"
#include "Camera.h"
#include "GameConst_Color.h"
#include "SCItemSlot.h"
#include "InventoryDialog.h"

//--------------------------------------------------------------------------------------------------
/**
*/
WzID uiCharCustomizeDlg::control_id[kCustomDlgMax] = 
{
    StrToWzID("ST00"), // 타이틀(ST00)                                          
    StrToWzID("P004"), // 캐릭터화면(P004)
    StrToWzID("P002"), // 2번째PIC(P002))
    StrToWzID("P003"), // 3번째PIC(P003)
    StrToWzID("BT01"), // x버튼(BT01)
    StrToWzID("BT02"), // +버튼(BT02)
    StrToWzID("BT03"), // -버튼(BT03)
    StrToWzID("BT04"), // 좌회전버튼(BT04)
    StrToWzID("BT05"), // 우회전버튼(BT05)
    StrToWzID("BT06"), // 좌헤어버튼(BT06)
    StrToWzID("BT08"), // 좌얼굴버튼(BT08)
    StrToWzID("BT10"), // 좌키버튼(BT10)
    StrToWzID("BT07"), // 우헤어버튼(BT07)
    StrToWzID("BT09"), // 우얼굴버튼(BT09)
    StrToWzID("BT11"), // 우키버튼(BT11)
    StrToWzID("BT12"), // 변경버튼(BT12)
    StrToWzID("BT13"), // 취소(BT13)
    StrToWzID("BT14"), // 원래대로(BT14)
    StrToWzID("S000"), // 머리텍스트(S000)
    StrToWzID("S001"), // 헤어텍스트(S001)
    StrToWzID("S002"), // 키텍스트(S002)
    StrToWzID("S005"), // 머리(S005)
    StrToWzID("S006"), // 헤어(S006)
    StrToWzID("S003"), // 키(S003)
    StrToWzID("L000"), // 설명(L000)
};

uiCharCustomizeDlg::uiCharCustomizeDlg(InterfaceManager *pUIMan)
:uiBase(pUIMan),
invalid_dlg_control_(true)
{
    DataClear();
}
//--------------------------------------------------------------------------------------------------
/**
*/
uiCharCustomizeDlg::~uiCharCustomizeDlg(void)
{
	Release();
}
//--------------------------------------------------------------------------------------------------
/**
*/
VOID uiCharCustomizeDlg::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
	InitControls();
}
//--------------------------------------------------------------------------------------------------
/** 창이열릴때 컨트롤러 들이 정상적의로 붙어있는지 체크함(널처리횟수 줄이기위한변수)
*/
void uiCharCustomizeDlg::InitControls()
{
    for (int index = 0; index < kCustomDlgMax; ++index)
    {
         CControlWZ* control_ptr = getControl(index);

         if (control_ptr == NULL)
         {
            set_invalid_dlg_control(true);
            return;
         }
    }
    
    set_invalid_dlg_control(false);
}
//--------------------------------------------------------------------------------------------------
/**
*/
void uiCharCustomizeDlg::OnShowWindow(BOOL val)
{
    if (invalid_dlg_control())
    {
        return;
    }

	if (val)
	{
		//uiBase::MoveCenter();
        g_KeyQueueManager.DeleteMsg(eDoType_Char_Customize);
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_Char_Customize;
        msg.wParam = IM_CHAR_CUSTOMIZE_MGR::CHAR_CUSTOMIZE_DLG;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
        InitCustomizeData();
	}
	else
	{
        uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
        if (inventory_manager != NULL)
        {
            inventory_manager->UnlockInventoryUI();
        }
		g_KeyQueueManager.DeleteMsg(eDoType_Char_Customize);
        RelesePlayer();
	}
}
//--------------------------------------------------------------------------------------------------
/** 메시지 처리
*/
VOID uiCharCustomizeDlg::MessageProc( SI_MESSAGE * pMessage )
{
	//패킷대기상태에서는 정지한다.
	switch (pMessage->eResultMsg)
	{
    case RT_MSG_LBUTTONUP:
    case RT_MSG_LBUTTONCLICK:
        {
            set_face_view(kEventNone);
        }
        break;
    
	case RT_MSG_LBUTTONDOWN:
		{
            BYTE index = getCtrlIDToPos(pMessage->dwCtrlID);
			switch(index)
			{
            case kCancleBtn:     //취소
			case kEscBtn:        //x버튼
			{
			    this->ShowInterface(FALSE);
			}
			break;
            case kZoonInBtn:       // +버튼(BT02)
                {
                    set_face_view(kCameraZoomIn);
                }
                break;
            case kZoomOutBtn:      // -버튼(BT03)
                {
                    set_face_view(kCameraZoomOut);
                }
                break;
            case kLeftRotateBtn: // 좌회전버튼(BT04)
                {
                    set_face_view(kRotateLeft);
                }
                break;
            case kRightRotateBtn:// 우회전버튼(BT05)
                {
                    set_face_view(kRotateRight);
                }
                break;

            case kLeftHairBtn:   // 좌헤어버튼(BT06)
            case kLeftFaceBtn:   // 좌얼굴버튼(BT08)
            case kLeftHeightBtn: // 좌키버튼(BT10)
                {
                    OnClickChangeButton(index - kLeftHairBtn, false);
                }
                break;
            
            case kRigthHairBtn:  // 우헤어버튼(BT07)
            case kRightFaceBtn:  // 우얼굴버튼(BT09)
            case kRightHeightBtn:// 우키버튼(BT11)
                {
                    OnClickChangeButton(index - kRigthHairBtn, true);
                }
                break;
            case kChangeBtn:     // 변경버튼(BT12)
                {
                    uiAssistMan* assist_man = static_cast<uiAssistMan*>(
                                        g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST));  

                    if (assist_man)
                    {
                        assist_man->ShowAskCharCustomize(customize_type());
                    }
                }
                break;
            case kRollBackBtn:   // 원래대로(BT14)
                {
                    set_cur_pos(kFace, save_pos(kFace));
                    set_cur_pos(kHair, save_pos(kHair));
                    set_cur_pos(kHeight, save_pos(kHeight));
                    RefreshData();
                }
                break;
            }
		}
		break;
	}
}
//--------------------------------------------------------------------------------------------------
/** 최초 커스터마이징 창이 열릴때마다 타입에 맞게(상단 타이틀을 바꿔준다)
*/
bool uiCharCustomizeDlg::InitTitleCaption(BYTE type, CCtrlStaticWZ* control)
{
    BASE_HeroVariationInfo* info  = get_custom_info(type, cur_pos(type));
    
    if ((info == NULL) ||
        (control == NULL))
    {
        if (control != NULL)
        {
            // 타입이 없을 때 타입1을 넣어준다.
            //70426: Type %d
            TCHAR result_string[INTERFACE_STRING_LENGTH]={0,};
            TCHAR temp_string[INTERFACE_STRING_LENGTH]={0,};
            g_InterfaceManager.GetInterfaceString(70426, temp_string);
            Sprintf(result_string, temp_string, 1);
            GlobalFunc::SetCaption(control, result_string);
            control->SetTextColor(RGBA(255,0,0,255));
        }
        return false;
    }

    TCHAR result_string[INTERFACE_STRING_LENGTH]={0,};
    
    //기본타입일때    
    if (info->resource_code == 0)
    {
        //70426: Type %d
        TCHAR temp_string[INTERFACE_STRING_LENGTH]={0,};
        g_InterfaceManager.GetInterfaceString(70426, temp_string);
        Sprintf(result_string, temp_string, info->partID.wPartID);
        GlobalFunc::SetCaption(control, result_string);
    }
    else
    {
        g_InterfaceManager.GetInterfaceString(info->name_code, result_string);
        GlobalFunc::SetCaption(control, result_string);
    }

    if (save_pos(type) != cur_pos(type))
    {
        control->SetTextColor(RGBA(255,255,255,255));
        return true;
    }
    else
    {
        control->SetTextColor(RGBA(255,0,0,255));
    }
    
    return false;
}
//--------------------------------------------------------------------------------------------------
/** 이벤트발생시마다 선택되어진 타입을 그려준다
*/
void uiCharCustomizeDlg::RefreshSelectText()
{
    //컨트롤러 널체크(이하구문은 체크할필요없음 모두유효함)
    if (invalid_dlg_control())
    {
        return;
    }
    bool show_change_button = false;
    switch (customize_type())
    {
    case kBase:
        {
            if (InitTitleCaption(kHair, (CCtrlStaticWZ*)getControl(kCurHairText)))
            {
                show_change_button = true;
            }
            
            if (InitTitleCaption(kFace, (CCtrlStaticWZ*)getControl(kCurFaceText)))
            {
                show_change_button = true;
            }

            CCtrlStaticWZ* height_ctrl = (CCtrlStaticWZ*)getControl(kCurHeightText);
            
            if (height_info() == NULL)
            {
                return;
            }

            TCHAR text[INTERFACE_STRING_LENGTH]={0,};
            float height = height_info()->m_fBase * height_info()->m_fHeight[cur_pos(kHeight)];
            Sprintf(text, _T("%.2fm"), height);
            
            GlobalFunc::SetCaption(height_ctrl, text);

            if (save_pos(kHeight) != cur_pos(kHeight))
            {
                show_change_button = true;
                height_ctrl->SetTextColor(RGBA(255,255,255,255));
            }
            else
            {
                height_ctrl->SetTextColor(RGBA(255,0,0,255));
            }
        }
        break;
    case kHair:
        {
            show_change_button = InitTitleCaption(kHair, (CCtrlStaticWZ*)getControl(kCurHairText));
        }
        break;
    case kFace:
        {
            show_change_button = InitTitleCaption(kFace, (CCtrlStaticWZ*)getControl(kCurHairText));
        }
        break;
    }

    ENUM_STATEWZ state = show_change_button ? WZ_SHOW : WZ_HIDE;
    getControl(kChangeBtn)->ShowWindowWZ(state);
}
//--------------------------------------------------------------------------------------------------
/**  이벤트발생시마다 선택된 외형설명을 기입해준다(캐쉬만 해주고있음)
*/
void uiCharCustomizeDlg::RefreshDescText()
{
    // 컨트롤러 널체크함수
    if (invalid_dlg_control())
    {
        return;
    }

    if (customize_type() == kBase)
    {
        return;
    }
    
    BASE_HeroVariationInfo* info = get_custom_info(customize_type(), cur_pos(customize_type()));

    if (info == NULL)
    {
        return;
    }

    int desc_string = info->desc_code;

    CCtrlListWZ* list_control = static_cast<CCtrlListWZ *>(getControl(kOnDescList));
    list_control->ShowWindowWZ((desc_string == 0) ? (WZ_HIDE):(WZ_SHOW));
    list_control->DeleteItemAll();
    list_control->SetBeginDrawIndex(0);
    list_control->SetUnuseSelBar(TRUE);
    list_control->OnFocusOut();
    std::vector<util::_tstring> text_vector;
    TCHAR text[INTERFACE_STRING_LENGTH]={0,};
    g_InterfaceManager.GetInterfaceString(desc_string, text, INTERFACE_STRING_LENGTH-1);
    GlobalFunc::ProcessWordWrapByFontSize(&text_vector, text, list_control->GetFontID(),
                                          _tcslen(text),(list_control->GetSize().right - 15));
    int max_size = text_vector.size();
    
    for (int idx = 0; idx < max_size; ++idx )
    {
        int index = list_control->InsertItemBack();
        GlobalFunc::SetItem(list_control, index , 0, (TCHAR *)text_vector[idx].c_str() ,
                            list_control->GetFontID(),DT_VCENTER | DT_LEFT, c_InterfaceBKColor,
                            c_InterfaceTextColor , c_InterfaceSelColor);
    }
}
//--------------------------------------------------------------------------------------------------
/** 데이터 갱신
*/
void uiCharCustomizeDlg::RefreshData()
{
    RefreshSelectText();
    RefreshDescText();
    RefreshAppearance();
}
//--------------------------------------------------------------------------------------------------
/**
*/
void uiCharCustomizeDlg::OnUpdateSolarDialog()
{
    static float fRange = 0.12f;

    if (face_view() == kEventNone)
    {
        return;
    }

    switch (face_view())
    {
    case kCameraZoomIn:
        {
            SetDiplayCamera(true);
        }
        break;
    case kCameraZoomOut:
        {
            SetDiplayCamera(false);
        }
        break;
    case kRotateLeft:
        {
            SetRotateAngle(fRange);
        }
        break;
    case kRotateRight:
        {
            SetRotateAngle(-fRange);
        }
        break;
    }

    // 마우스 포인터 넘어갔을땐 자동 상태해지
    RECT rc;
    GetDialogWZ()->GetClientRect(&rc);
    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();
    if ((rc.left <= mouse_x) && 
        (rc.right >= mouse_x) &&
        (rc.top <= mouse_y) &&
        (rc.bottom >= mouse_y))
    {
        return;
    }

    set_face_view(kEventNone);
}
//--------------------------------------------------------------------------------------------------
/** 카메라 줌인아웃(OthonomarlMatrix라 꼼수로 스케일로 카메로 위치로 조절한다)
*/
void uiCharCustomizeDlg::SetDiplayCamera(bool is_zoomin)
{
    float class_offset = 0.f;
    
    static float offset_array[eCHAR_TYPE_MAX] =
    {
        {0.f},   // eCHAR_NONE				
        {0.1f},  // eCHAR_BERSERKER	
        {0.07f}, // eCHAR_DRAGON		
        {0.04f}, // eCHAR_SHADOW		
        {0.03f}, // eCHAR_VALKYRIE		
        {0.03f}, // eCHAR_ELEMENTALIST	
        {-0.08f}, // eCHAR_MYSTIC	
    };

    //고정된 증가값
    float next_scale = 0.18f;
    float next_camera_pos = offset_array[g_pHero->GetClass()] + 0.32f;

    //제한 값
    static float limit_scale = 2.8f;
    static float limit_camera_pos = 5.3f;

    if (is_zoomin)
    {
        next_scale = unit_scale() + next_scale;
        next_camera_pos = camera_pos() + next_camera_pos;

        if ((next_scale > limit_scale) ||
            (next_camera_pos > limit_camera_pos))
        {
            next_scale = unit_scale();
            next_camera_pos = camera_pos();
        }
    }
    else
    {
        next_scale = unit_scale() - next_scale;
        next_camera_pos = camera_pos()- next_camera_pos;

        if ((next_scale < 0.f) ||
            (next_camera_pos < 0.f))
        {
            next_camera_pos = 0.f;
            next_scale = 0.f;
        }
    }
    set_unit_scale(next_scale);
    set_camera_pos(next_camera_pos);
}
//--------------------------------------------------------------------------------------------------
/** 회전
*/
void uiCharCustomizeDlg::SetRotateAngle(float angle)
{
    Player* display = GetCustomizePlayer();
    if (display == NULL)
    {
        return;
    }
    set_roate_angel(roate_angel() + angle);
    display->SetAngle(roate_angel(), TRUE);
}
//--------------------------------------------------------------------------------------------------
/**
*/
void uiCharCustomizeDlg::OnRenderSolarDialog()
{
    if (invalid_dlg_control())
    {
        return;
    }
    RECT rect = g_InterfaceManager.CalcuateViewPortWithRect(getControl(kMainView)->GetSizeRect());

    RenderPlayer(rect);
}
//--------------------------------------------------------------------------------------------------
/**  좌우 변경 버튼클릭시
*/
void uiCharCustomizeDlg::OnClickChangeButton(WORD index, bool is_next)
{
    // 얼굴,헤어경우때는 첫번째 버튼를 사용하기때문에 보정해줌
    if (customize_type() != kBase)
    {
        index = customize_type();
    }
    int next_pos = cur_pos(index);
    int max_limit = max_pos(index) - 1;

    if (is_next)
    {
        if (++next_pos > max_limit)
        {
            next_pos = 0;
        }
    }
    else
    {
        if (--next_pos < 0)
        {
            next_pos =  max_limit;
        }
    }

    set_cur_pos(index, next_pos);
    RefreshData();
}
//--------------------------------------------------------------------------------------------------
/**  
*/
DWORD uiCharCustomizeDlg::GetCompositeID(WORD parts, WORD id)
{
    HERO_VARIATION_PART part_id;
    part_id.byClass = g_pHero->GetClass();
    part_id.byPart = (BYTE)(parts + 1);
    part_id.wPartID = id;
    return part_id.dwID;
}
//--------------------------------------------------------------------------------------------------
/** 단일변경아이템 리스트채우기(아이템정보에서 HeroVariationInfo파서에서 resource_code == 2)이고
    더블어 BaseItemInfo타입에서 m_byWasteType != 0 다르다면 캐쉬아이템이긴한데 캐쉬리스트가아니라
    해당 리스트만 보여주는타입니다.
*/
bool uiCharCustomizeDlg::SetSingleListData(WORD type, WORD hero_part_id)
{
    InventoryDialog* inven = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);

    if (inven == NULL)
    {
        return true;
    }
    //싱글턴은 널체크하지말자
    HeroVariationInfoParser* parser = HeroVariationInfoParser::Instance();

    SCItemSlot& item_slot = static_cast<SCItemSlot &>(inven->GetSlot(inven_pos()));
    WORD parser_id = item_slot.GetItemInfo()->m_byWasteType;

    // 단일아이템일경우 현재 주인공의 외형정보에 아이템에 물려있는 ID두개만 들어가야된다
    if (parser_id == 0)
    {
        return false;
    }
    //주인공 데이터는 첫번째로 넣는다
    DWORD real_data_id = GetCompositeID(type, hero_part_id);
    BASE_HeroVariationInfo* info = parser->GetHeroVariationInfo(real_data_id);

    if (info == NULL)
    {
        return true;
    }
    custom_list_insert(type, info);
    set_max_pos(type, 1);

    // 주인공ID와 아이템 ID 다르다면 아이템캐쉬데이터 두번째로삽입
    if (hero_part_id != parser_id)
    {
        real_data_id = GetCompositeID(type, parser_id);
        info = parser->GetHeroVariationInfo(real_data_id);
        if (info == NULL)
        {
            return true;
        }
        custom_list_insert(type, info);
        set_max_pos(type, 2);
        set_cur_pos(type, 1);
    }
    return true;
}
//--------------------------------------------------------------------------------------------------
/**  HeroVariation에 있는 내용을 해당클래스,파츠에 맞게 얻어와서
     다이얼로그에서 보여줄 리스트를 만들어낸다
*/
void uiCharCustomizeDlg::SetListData(WORD type, WORD parts, bool is_cash)
{
    // 싱글턴은 널체크하지말자
    HeroVariationInfoParser* parser = HeroVariationInfoParser::Instance();
    
    // 현재 주인공 외형정보
    int hero_part_id = g_pHero->GetPartVariation(parts);
    
    // 단일 변경아이템인지
    if (is_cash &&
        SetSingleListData(type, hero_part_id))
    {
        return;
    }

    // 파서 리스트 얻기(캐쉬.기본)
    const std::vector<DWORD>& list_info = parser->GetList(g_pHero->GetClass(), parts, is_cash);
    if (list_info.empty())
    {
        return;
    }
    int max_size = list_info.size();
    bool exgist_id = false;
    BASE_HeroVariationInfo* info = NULL;
    // 캐시리스트 내용 채우기
    for (int index = 0; index < max_size; ++index)
    {
        info =  parser->GetHeroVariationInfo(list_info[index]);
        if (info == NULL)
        {
            continue;
        }
        //리스트에 이미 현재 주인공(Hero) 정보가 존재할때 저장해준다
        if (info->partID.wPartID == hero_part_id)
        {
            set_save_pos(type, index);
            set_cur_pos(type, index);
            exgist_id = true;
        }
        custom_list_insert(type, info);
    }
    
    //마지막에 주인공현재 외형도 삽입해준다    
    if (exgist_id == false)
    {
        DWORD real_data_id = GetCompositeID(type, hero_part_id);
        info =  parser->GetHeroVariationInfo(real_data_id);
        if (info == NULL)
        {
            return;
        }
        custom_list_insert(type, info);
        set_save_pos(type, max_size);
        set_cur_pos(type, max_size);
        set_max_pos(type, max_size + 1);
    }
    else
    {
        set_max_pos(type, max_size);
    }
}
//--------------------------------------------------------------------------------------------------
/**  아이템클릭시 (인벤슬롯위치와 아이템타입(hair.face.all)인지 정보셋팅해줌)
*/
void uiCharCustomizeDlg::GenerateCustomInfo(WORD custom_type, POSTYPE slot_pos)
{
    DataClear();
    // 인벤위치 저장
    set_inven_pos(slot_pos);
    if (custom_type == eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL)
    {
        set_customize_type(kBase);
        SetListData(kHair, PLAYER_VARIATION_HAIR, false);
        SetListData(kFace, PLAYER_VARIATION_FACE, false);
        set_height_info(HeroHeightInfoParser::Instance()->GetHeroHeightInfo(g_pHero->GetClass()));
        if (height_info())
        {
            set_max_pos(kHeight, MAX_HEIGHT_VARIATION);
            set_cur_pos(kHeight, g_pHero->GetHeightVariation());
            set_save_pos(kHeight, g_pHero->GetHeightVariation());
        }

    }
    else if (custom_type == eITEMTYPE_CHANGE_CHARACTER_HAIR)
    {
        set_customize_type(kHair);
        SetListData(kHair, PLAYER_VARIATION_HAIR, true);
    }
    else if (custom_type == eITEMTYPE_CHANGE_CHARACTER_FACE)
    {
        set_customize_type(kFace);
        SetListData(kFace, PLAYER_VARIATION_FACE, true);
    }
}
//--------------------------------------------------------------------------------------------------
/**  
*/
VOID uiCharCustomizeDlg::DataClear()
{
    reset_customize_data();
    reset_face_view();
    set_height_info(NULL);
    custom_list_clear(kHair);
    custom_list_clear(kFace);
}
//--------------------------------------------------------------------------------------------------
/** 컨트롤러및 고정 텍스트들 셋팅
*/
void uiCharCustomizeDlg::InitCustomizeData()
{
    if (invalid_dlg_control())
    {
        return;
    }
    
    uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
    if (inventory_manager != NULL)
    {
        inventory_manager->LockInventoryUI();
    }    

    //1. 다이얼로그 컨트롤 셋팅
    bool is_basic = customize_type() == kBase;
    ENUM_STATEWZ state = is_basic ? WZ_HIDE : WZ_SHOW;
    getControl(kOnDescList)->ShowWindowWZ(state);
    state = is_basic ? WZ_SHOW : WZ_HIDE;
    getControl(kFaceTypeText)->ShowWindowWZ(state);
    getControl(kHeightTypeText)->ShowWindowWZ(state);
    getControl(kCurFaceText)->ShowWindowWZ(state);
    getControl(kCurHeightText)->ShowWindowWZ(state);
    getControl(kSecondPic)->ShowWindowWZ(state);
    getControl(kThirdPic)->ShowWindowWZ(state);
    getControl(kLeftFaceBtn)->ShowWindowWZ(state);
    getControl(kRightFaceBtn)->ShowWindowWZ(state);
    getControl(kLeftHeightBtn)->ShowWindowWZ(state);
    getControl(kRightHeightBtn)->ShowWindowWZ(state);

    //2. 다이얼로그 기본고정텍스트들 셋팅
    TCHAR text[INTERFACE_STRING_LENGTH]={0,};
    if (is_basic)
    {
        //타이틀(2132).헤어(200).얼굴(195).키(196)
        g_InterfaceManager.GetInterfaceString(2132, text);
        GlobalFunc::SetCaption((CCtrlStaticWZ*)getControl(kDlgTitle), text);
        g_InterfaceManager.GetInterfaceString(200, text);
        GlobalFunc::SetCaption((CCtrlStaticWZ*)getControl(kHairTypeText), text);
        g_InterfaceManager.GetInterfaceString(195, text);
        GlobalFunc::SetCaption((CCtrlStaticWZ*)getControl(kFaceTypeText), text);
        g_InterfaceManager.GetInterfaceString(196, text);
        GlobalFunc::SetCaption((CCtrlStaticWZ*)getControl(kHeightTypeText), text);
    }
    else
    {
        //타이틀 및(헤어or얼굴)
        static int name_code[kBase-1][2] = {{2131, 200}, {2130, 195}};
        g_InterfaceManager.GetInterfaceString(name_code[customize_type()][0], text);
        GlobalFunc::SetCaption((CCtrlStaticWZ*)getControl(kDlgTitle), text);
        g_InterfaceManager.GetInterfaceString(name_code[customize_type()][1], text);
        GlobalFunc::SetCaption((CCtrlStaticWZ*)getControl(kHairTypeText), text);
    }

    //유동 정보
    RefreshData();
}
//--------------------------------------------------------------------------------------------------
/** 활성화 슬롯인지 체크
*/
bool uiCharCustomizeDlg::IsActiveSlot(POSTYPE slot_pos)
{
    if ((slot_pos != inven_pos()) ||
        (IsVisibleWindow() == FALSE))
    {
        return false;
    }
    
    InventoryDialog* inventory = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
    
    if ((inventory == NULL) ||
        inventory->IsEmpty(slot_pos)) 
    {
        return false;
    }

    SCItemSlot& item_slot = static_cast<SCItemSlot&>(inventory->GetSlot(slot_pos));
    
    if (item_slot.IsContainValidItem() == FALSE)
    {
        return false;
    }
         
    return true;
}
//--------------------------------------------------------------------------------------------------
/** 변경하기 버튼클릭시(C2S 서버로 패킷보내기)
*/
void uiCharCustomizeDlg::SendCharCustomizePacket()
{
    MSG_CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN packet;
    BASE_HeroVariationInfo* info = NULL;
    switch (customize_type())
    {
    case kFace:
        {
            info = get_custom_info(kFace,cur_pos(kFace));
            if (info != NULL)
            {
                packet.character_appearance_.face_ = (BYTE)info->partID.wPartID;
            }
            packet.character_appearance_.appearance_type_ = ChangeCharacterAppearanceTypeConst::kFace;
        }
        break;
    case kHair:
        {
            info = get_custom_info(kHair,cur_pos(kHair));
            if (info != NULL)
            {
                packet.character_appearance_.hair_ = (BYTE)info->partID.wPartID;
            }
            packet.character_appearance_.appearance_type_ = ChangeCharacterAppearanceTypeConst::kHair;
        }
        break;
    case kBase:
        {
            info = get_custom_info(kFace,cur_pos(kFace));
            if (info != NULL)
            {
                packet.character_appearance_.face_ = (BYTE)info->partID.wPartID;
            }
            info = get_custom_info(kHair,cur_pos(kHair));
            if (info != NULL)
            {
                packet.character_appearance_.hair_ = (BYTE)info->partID.wPartID;
            }
            packet.character_appearance_.height_ = (BYTE)cur_pos(kHeight);
            packet.character_appearance_.gender = (BYTE)GetCustomizePlayer()->GetGender();
            packet.character_appearance_.appearance_type_ = ChangeCharacterAppearanceTypeConst::kAll;
        }
        break;
    }

    packet.item_position_ = inven_pos();
    
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &packet, sizeof(packet));
}
//--------------------------------------------------------------------------------------------------
/** 커스텀 캐릭터 얻어오기
*/
Player* uiCharCustomizeDlg::GetCustomizePlayer()
{
    Player* display = static_cast<Player*>(g_ObjectManager.GetObject(CUSTOMIZE_CHARACTER_KEY));

    if (display == NULL)
    {
        DWORD class_code = g_pHero->GetClass();
        ClassCodeEx class_code_ex;
        class_code_ex.class_type = g_pHero->GetClass();
        class_code_ex.gender = g_pHero->GetGender();
        class_code = class_code_ex.class_code;

        display = static_cast< Player* >(g_ObjectManager.Add(CUSTOMIZE_CHARACTER_KEY, PLAYER_OBJECT,
                                                             class_code, FALSE));
        CLIENT_CHARACTER_PART part;
        ZeroMemory(&part,sizeof(part));
        part.m_byClass = g_pHero->GetClass();
        part.m_bySlot =0;
        display->SetClientChar(part);
        display->SetAnimation("N001", TRUE);
        display->SetColor(WzColor_RGBA(255,255,255,255));
        display->SetFreeMove(TRUE);
        display->ProcessAnimation(0);
        display->HideHelmet(TRUE, FALSE);
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        display->HideWing(TRUE, FALSE);
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        EQUIP_ITEM_INFO out;
        g_pHero->GetEquipItemTotalInfo(out);
        display->SetEquipItemTotalInfo(out);
        int variation = g_pHero->GetPartVariation(PLAYER_VARIATION_HAIR);
        display->SetPartVariation(PLAYER_VARIATION_HAIR, variation);
        variation = g_pHero->GetPartVariation(PLAYER_VARIATION_FACE);
        display->SetPartVariation(PLAYER_VARIATION_FACE, variation);
        display->SetHeightVariation(g_pHero->GetHeightVariation());
        display->SetShadowDraw(FALSE);   
        display->SetShow(FALSE);
    }
    return display;
}
//--------------------------------------------------------------------------------------------------
/** 커스텀 캐릭터 외형 적용
*/
void uiCharCustomizeDlg::RefreshAppearance()
{
    Player* display = GetCustomizePlayer();

    if (display == NULL)
    {
        return;
    }
    
    BASE_HeroVariationInfo* info  = NULL;

    switch (customize_type())
    {
    case kFace:
        {
            info = get_custom_info(kFace, cur_pos(kFace));
            if (info)
            {
                display->SetPartVariation(PLAYER_VARIATION_FACE, info->partID.wPartID);
            }
        }
        break;
    case kHair:
        {
            info = get_custom_info(kHair, cur_pos(kHair));
            if (info)
            {
                display->SetPartVariation(PLAYER_VARIATION_HAIR, info->partID.wPartID);
            }
        }
        break;
    case kBase:
        {
            info = get_custom_info(kFace, cur_pos(kFace));
            if (info)
            {
                display->SetPartVariation(PLAYER_VARIATION_FACE, info->partID.wPartID);
            }
            info = get_custom_info(kHair, cur_pos(kHair));
            if (info)
            {
                display->SetPartVariation(PLAYER_VARIATION_HAIR, info->partID.wPartID);
            }
            display->SetHeightVariation(cur_pos(kHeight));
        }
        break;
    }
    display->RefreshAppearRender();

    // 모든 무기& 악세사리 제거한다 
    int max_attachment = display->GetAttachmentCount();
    for (int idx = 0; idx < max_attachment; ++idx)
    {
        display->DeleteAttachment(idx);
    }
}
//--------------------------------------------------------------------------------------------------
/** 
*/
void uiCharCustomizeDlg::RenderPlayer(RECT viewport)
{
    Player* display = GetCustomizePlayer();

    if (display == NULL)
    {
        return;
    }
    WzVector camera_position ={0.f, 0.f, camera_pos()};
    static WzVector look_at ={0.0, 100.0, 0.0};
    static WzVector upvector ={0.0, 0.0, 1.0};
    WzMatrix mat;
    MatrixIdentity(&mat);
    BOOL prev_mode,prev_size;
    g_pSunRenderer->Check2DMode(&prev_mode,&prev_size);
    g_pSunRenderer->Set2DMode( FALSE );
    g_pSunRenderer->SetWorldMatrix(&mat);
    g_pSunRenderer->SetCameraMatrix(&camera_position, &look_at, &upvector);
    g_pSunRenderer->SetViewport(&viewport);
    g_pSunRenderer->ClearRender(CR_DEPTH, NULL, NULL, NULL, &viewport);  
 
    static float offset_array[eCHAR_TYPE_MAX][2] =
    {
        {0.f, 0.f},    // eCHAR_NONE				
        {0.08f, 2.1f}, // eCHAR_BERSERKER	
        {0.f, 2.0f},   // eCHAR_DRAGON		
        {0.f, 1.9f},   // eCHAR_SHADOW		
        {-0.03f, 1.8f},// eCHAR_VALKYRIE		
        {0.f, 1.9f},   // eCHAR_ELEMENTALIST	
        {0.f, 1.9f},   // eCHAR_MYSTIC
        {0.f, 2.2f},   // eCHAR_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        {-0.03f, 1.8f},// eCHAR_WITCHBLADE
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };

    float pos_offset = unit_scale()*offset_array[display->GetClass()][0];
    float view_size  = offset_array[display->GetClass()][1];
   
    g_pSunRenderer->SetOrthogonalMatrix(0.0, view_size, 0, view_size, 1, 220);
    WzVector unit_pos = {(view_size/2.f) + pos_offset, 10.f, 0.f};
    display->GetBasePart()->SetPosition(unit_pos);

    float cur_height = 1.f;
    if (height_info())
    {
        cur_height = height_info()->m_fHeight[cur_pos(kHeight)];
    }
    cur_height += unit_scale();
    WzVector base_scale = {cur_height, cur_height, cur_height};
    display->SetScale(base_scale);

    BOOL is_zwrite = g_pSunRenderer->IsEnableZWrite();
    BOOL is_zread = g_pSunRenderer->IsEnableZTest();
    g_pSunRenderer->SetZBufferWrite(TRUE);
    g_pSunRenderer->SetZBufferTest(TRUE);
    display->SetStandAnimationDelay();
    display->SetShow(TRUE);
    display->Render();
    display->SetShow(FALSE);
    g_pSunRenderer->FlushReservedDrawCall();
    g_pSunRenderer->SetViewport(NULL);
    g_Camera.RestoreProjection();
    g_pSunRenderer->SetZBufferWrite(is_zwrite);
    g_pSunRenderer->SetZBufferTest(is_zread);
    g_pSunRenderer->Set2DMode(prev_mode, prev_size);		
    
}
void uiCharCustomizeDlg::RelesePlayer()
{
    g_ObjectManager.Delete(CUSTOMIZE_CHARACTER_KEY);
}
