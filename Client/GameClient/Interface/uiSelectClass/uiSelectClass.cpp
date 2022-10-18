#include "SunClientPrecompiledHeader.h"
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiSelectClass.h"
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "CharacterCreationInfoParser.h"
#include "CharacterScene.h"
#include "CreateCharCustomDialog.h"

#include "GlobalFunc.h"
#include "uiCreateCharacterMan/uiCreateCharacterMan.h"
#include "uiCreateClassInfo/uiCreateClassInfo.h"

#include "uiSkillMan/uiNewSkillMan.h"
#include "uiskilltree/uiSkillTree.h"
#include "uiskilltree/uiSkillTreeEffect.h"

#include "uiLoginBack/uiLoginBackTop.h"

const DWORD kPerFrameTickCount = 30;

namespace
{
    const DWORD kDrawWidth = 18;
    const DWORD kDrawHeight = 18;

    const DWORD kClassNameFont = StrToWzID("mi17");
    const DWORD kClassNameColor = WzColor_RGBA(199, 179, 137, 255);
    const DWORD kClassDescriptionFont = StrToWzID("st10");
    const DWORD kClassDescriptionColor = WzColor_RGBA(255, 255, 255, 255);
}

RECT kani_frames[16] = 
{
    {0,0,128,128},{128,0,128,128},{256,0,128,128},{384,0,128,128},
    {0,128,128,128},{128,128,128,128},{256,128,128,128},{384,128,128,128},
    {0,256,128,128},{128,256,128,128},{256,256,128,128},{384,256,128,128},
    {0,384,128,128},{128,384,128,128},{256,384,128,128},{384,384,128,128},
};

SelectClass::SelectClass(InterfaceManager* ui_manager):
uiBase(ui_manager)
{
    m_SkillEffectHandle = INVALID_HANDLE_VALUE;
}

SelectClass::~SelectClass()
{
    Release();
}

void SelectClass::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    m_CurrentClassType = eCHAR_BERSERKER;
#else
    m_CurrentClassType = eCHAR_NONE;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    m_LastObjectKey = 0;
    m_LastTickCount = 0;

    RegisterControl();

    // 이펙트 이미지 로드
    CManagerTexture* mgr = g_pSunRenderer->x_pManagerTexture;
    m_SkillEffectHandle = mgr->LoadTexture("Data\\Interface\\etc\\skill_eff.tga",
        TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
}

void SelectClass::Release()
{
    if (m_SkillEffectHandle != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_SkillEffectHandle);
        m_SkillEffectHandle = INVALID_HANDLE_VALUE;
    }
}

void SelectClass::DisplayClassDescription(eCHAR_TYPE class_type)
{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    uiCreateClassInfo* createcharinfo_dialog = GET_CAST_DIALOG(uiCreateClassInfo, IM_CREATE_CHAR_MANAGER::CREATECHAR_CLASS_INFO);
    if (createcharinfo_dialog)
    {
        createcharinfo_dialog->AddClassInfo(class_type, GlobalFunc::GetClassNameCode(class_type));
    }
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
}

void SelectClass::OnLButtonClick(SI_MESSAGE* message)
{
    POSTYPE position = static_cast<POSTYPE>(GetControlPosition_byID(message->dwCtrlID));  

    if (position == INVALID_POSTYPE_VALUE)
    {
        return;
    }
    
    CCtrlButtonCheckWZ* control_ptr = NULL;
    control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(position));

    for (int i = kControl_Check_C001; i < kControl_Size; ++i)
    {
        control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));
        if (control_ptr == NULL)
        {
            continue;
        }

        ENUM_DRAWSTATE draw_state = control_ptr->GetDrawState();

        if (i != position)
        {   
            if (draw_state != E_DISABLE)
            {
                control_ptr->SetCheckState(false);
            }
        }
        else
        {
            control_ptr->SetCheckState(true);
        }
    }

    if (m_CurrentClassType == (position+1)) // 선택이 같으면 설명 애니메이션 안한다.
    {
        return;
    }

    m_FrameCount = 0;
    m_CurrentClassType = static_cast<eCHAR_TYPE>(position + 1);
    DisplayClassDescription(m_CurrentClassType);   
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_CLASS_INFO)->ShowDialog(TRUE);
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
    DWORD selected_object_key = g_CharSelectScene.GetSelectedObjectKey();
    if (selected_object_key)
    {
        GET_CAST_DIALOG( CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG )->InsertWeaponParts(m_CurrentClassType);
    }
}

void SelectClass::OnLButtonDoubleClick(SI_MESSAGE* message)
{
    POSTYPE position = static_cast<POSTYPE>(GetControlPosition_byID(message->dwCtrlID));  

    if (position == INVALID_POSTYPE_VALUE)
    {
        return;
    }
    m_CurrentClassType = static_cast<eCHAR_TYPE>(position + 1);
    DWORD selected_object_key = g_CharSelectScene.GetSelectedObjectKey();
    if (selected_object_key)
    {
        GET_CAST_DIALOG( CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG )->InsertWeaponParts(m_CurrentClassType);
    }
    OnExcute();
}

void SelectClass::MessageProc( SI_MESSAGE * message )
{
    if (IsLock())
        return;

    static bool is_double_click = false;
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONDCLICK:
        {
            OnLButtonDoubleClick(message);
            is_double_click = true;
        }
        break;
    case RT_MSG_LBUTTONCLICK:
        {
            if (is_double_click == true)
            {
                is_double_click = false;
            }
            else
            {
                OnLButtonClick(message);
            }
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

void SelectClass::HideClass(BYTE class_type)
{
    CCtrlButtonCheckWZ* control_ptr = NULL;
    int position = 0;

    RECT rect;
    GetDialogWZ()->GetClientRect(&rect);

    for (int i = kControl_Check_C001; i < kControl_Size; ++i)
    {
        eCHAR_TYPE select_class_type = static_cast<eCHAR_TYPE>(i + 1);
        control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));

        if (control_ptr == NULL)
        {
            continue;
        }

        // 선택 가능 클래스 확인
        if (g_CharSelectScene.CheckEnableSelectClass(select_class_type) == false)
        {   
            //control_ptr->ShowWindowWZ(WZ_HIDE);
            control_ptr->EnableWindowWZ(false);
        }
        else
        {
            control_ptr->EnableWindowWZ(true);
            if (select_class_type == class_type)
            {
                control_ptr->SetCheckState(true);

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
                GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_CLASS_INFO)->ShowDialog(TRUE);
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES 
                m_CurrentClassType = select_class_type;
                DisplayClassDescription(select_class_type);

                DWORD selected_object_key = g_CharSelectScene.GetSelectedObjectKey();
                if (selected_object_key != g_CharSelectScene.create_selected_character_object_key())
                {
                    g_CharSelectScene.ShowBaseCinematicCharacter();
                    GET_CAST_DIALOG( CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG )->InsertWeaponParts(m_CurrentClassType);
                }
            }

            /*if (position < kControl_Size)
            {
                control_ptr->MoveWindowWZ(static_cast<float>(control_positions_[position].left + rect.left), 
                    static_cast<float>(control_positions_[position].top + rect.top),
                    control_ptr->GetSizeWidth(), 
                    control_ptr->GetSizeHeight());
                ++position;
            }*/
        }
    }
}

void SelectClass::ShowClass()
{
    RECT rect;
    CCtrlButtonCheckWZ* control_ptr = NULL;
    for (int i = kControl_Check_C001; i < kControl_Size; ++i)
    {
        eCHAR_TYPE select_class_type = static_cast<eCHAR_TYPE>(i + 1);
        control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));
        if (control_ptr)
        {
            control_ptr->ShowWindowWZ(WZ_SHOW);
            control_ptr->SetCheckState(false);
            rect = control_ptr->GetSizeRect();
            control_ptr->MoveWindowWZ(static_cast<float>(-rect.left)-50, 
                static_cast<float>(rect.top), 
                control_ptr->GetSizeWidth(),
                control_ptr->GetSizeHeight());
			m_PositionAmount = -rect.right -50;
        }
    }
    m_LastTickCount = 0;
	m_FrameCount = 0;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    m_CurrentClassType = eCHAR_BERSERKER;
    SetCurrentClassCode(eCHAR_BERSERKER);
#else
    m_CurrentClassType = eCHAR_NONE;
    SetCurrentClassCode(eCHAR_NONE);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

void SelectClass::OnShowWindow( BOOL val )
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    if (val)
    {
        ShowInterface(FALSE);
    }
#else
    SolarDialog::OnShowWindow(val);

    if (val)
    {
        HideClass(GetSelectedClassCode());
    }
    else
    {
        ShowClass();
    }
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

void SelectClass::OnRenderSolarDialog()
{
    DWORD tick_count = clock_function::GetTickCount();
    
    if (m_LastTickCount == 0)
    {
        m_LastTickCount = tick_count;
    }

    DWORD current_tick_count = (tick_count - m_LastTickCount);
    if (current_tick_count > kPerFrameTickCount)
    {
        m_LastTickCount = tick_count;
    }
    AnimateSlide(current_tick_count);
    RenderCircle(current_tick_count);
}

void SelectClass::RenderCircle(DWORD tick_count)
{
    int ani_tick = (tick_count / kPerFrameTickCount);
    // 위치
    CCtrlButtonCheckWZ* control_ptr = NULL;
    control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(m_CurrentClassType-1));
    
    RECT position_rect = {0, 0, 64, 64};
    if (control_ptr)
    {
        position_rect = control_ptr->GetSizeRect();
    }

    m_FrameCount += ani_tick;
    //ani_tick = (frame_count_/10);

    if (m_FrameCount > 15)
    {
        m_FrameCount = 5;//반복 회전은 5번째 프레임 부터
    }

    g_pSunRenderer->RenderTexture(m_SkillEffectHandle, position_rect.left - 10.5f, position_rect.top - 10.5f, 64,64, 
        kani_frames[m_FrameCount].left + 0.5f, kani_frames[m_FrameCount].top + 0.5f, kani_frames[m_FrameCount].right + 0.5f, kani_frames[m_FrameCount].bottom + 0.5f);
}


void SelectClass::RegisterControl()
{
    RegisterControlPosition(StrToWzID("C001"), kControl_Check_C001);
    RegisterControlPosition(StrToWzID("C003"), kControl_Check_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_Check_C004);
    RegisterControlPosition(StrToWzID("C005"), kControl_Check_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_Check_C006);

    CCtrlButtonCheckWZ* control_ptr = NULL;
    for (int i = kControl_Check_C001; i < kControl_Size; ++i)
    {
        control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));
        if (control_ptr)
        {
            m_ControlPositionRect[i] = control_ptr->GetSize();
           /* control_ptr->MoveWindowWZ(
                static_cast<float>(-500),
                static_cast<float>(control_positions_[i].top),
                static_cast<float>(control_positions_[i].right),
                static_cast<float>(control_positions_[i].bottom));*/
        }
    }
    m_PositionAmount = -m_ControlPositionRect[0].right - 50;
}

void SelectClass::OnExcute()
{
    eCHAR_TYPE select_class_type = static_cast<eCHAR_TYPE>(m_CurrentClassType);
    if (select_class_type == eCHAR_NONE)
    {
        return;
    }
    else
    {
        SelectCreateClass(select_class_type, GetSelectedClassCode());
    }
}

void SelectClass::SelectCreateClass(eCHAR_TYPE select_class_type, BYTE select_class_code)
{
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
        CreateCharCustomDialog* createcharcustomdialog_ptr = GET_CAST_DIALOG(CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG);
        if (createcharcustomdialog_ptr != NULL)
        {
            createcharcustomdialog_ptr->first_setting_reset();
            g_CharSelectScene.InitVariation();
            createcharcustomdialog_ptr->UpdateVariationText();


            // 선택 가능하면 기존 캐릭터 파츠 해제
            //createcharcustomdialog_ptr->DeleteParts();

            // 선택된 클래스 캐릭터로 교체
            DWORD change_create_class = g_CharSelectScene.ChangeCreateClass(select_class_type);
			createcharcustomdialog_ptr->set_selected_character_object_key(change_create_class);

			//createcharcustomdialog_ptr->InsertWeaponParts(change_create_class);
			//createcharcustomdialog_ptr->LoadParts(change_create_class);
			createcharcustomdialog_ptr->InsertWeaponParts(select_class_type);
        }
        g_CharSelectScene.OnClickedChar(select_class_type, select_class_code);
    }
}

void SelectClass::AnimateSlide(DWORD current_tick_count)
{
    CCtrlButtonCheckWZ* control_ptr = NULL;
    int position = 0;
    float x_pos = 0.0f;

    RECT rect;
    GetDialogWZ()->GetClientRect(&rect);

    for (int i = kControl_Check_C001; i < kControl_Size; ++i)
    {
        control_ptr = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));

        if (control_ptr)
        {
            m_PositionAmount += static_cast<int>(current_tick_count/kPerFrameTickCount) * 5;
            x_pos = static_cast<float>(m_PositionAmount + -(i * 40.f) + rect.left);

            if (m_ControlPositionRect[i].left < x_pos)
            {
                x_pos = 0;
            }
            control_ptr->MoveWindowWZ(x_pos + rect.left, 
                static_cast<float>(m_ControlPositionRect[i].top + rect.top),
                control_ptr->GetSizeWidth(), 
                control_ptr->GetSizeHeight());
        }
    }
}
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
