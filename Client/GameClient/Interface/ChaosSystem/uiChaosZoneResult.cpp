#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiChaosZoneResult.h"
#include "application.h"
#include "interfacemanager.h"
#include "Hero.h"

const DWORD progress_per_time_[] = 
{
    1000, // kAnimaion_Progress_ShowUI, // ����
    700, // kAnimaion_Progress_Open,  // ������ ����
    8000, // kAnimaion_Progress_ShowResult, // ���� ���
    700, // kAnimaion_Progress_Close, // ������ ����
    1000, // kAnimaion_Progress_HideUI, // ��
    1     // kAnimaion_Progress_End
};

//------------------------------------------------------------------------------
/**
*/
uiChaosZoneResult::uiChaosZoneResult(InterfaceManager *ui_man) : uiBase(ui_man),
background_picture_pointer_(NULL),
result_picture_pointer_(NULL)
{
    animation_progress_ = kAnimaion_Progress_ShowUI;
    effect_render_alpha_ = 0.0f;
    ZeroMemory(effect_string, sizeof(effect_string));
}
//------------------------------------------------------------------------------
/**
*/
uiChaosZoneResult::~uiChaosZoneResult()
{
    this->Release();
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControl();
    start_animation_tick_ = 0;

    draw_texture_manager_.Initialize();
    
    TCHAR win_effect[] = "Data\\Interface\\chaos_eff_win.tga";
    TCHAR lose_effect[] = "Data\\Interface\\chaos_eff_lose.tga";
    TCHAR draw_effect[] = "Data\\Interface\\chaos_eff_draw.tga";

    TCHAR temp[256];
    for (int i = 0; i != 4; ++i)
    {
        for (int j = 0; j != 4; ++j)
        {
            Sprintf(temp, _T("win_effect_%01d"), (i * 4) + j);
            draw_texture_manager_.AddImage(temp, win_effect,   j* 64, i * 64, 64, 64);

            Sprintf(temp, _T("lose_effect_%01d"), (i * 4) + j);
            draw_texture_manager_.AddImage(temp, lose_effect,   j* 64, i * 64, 64, 64);

            Sprintf(temp, _T("draw_effect_%01d"), (i * 4) + j);
            draw_texture_manager_.AddImage(temp, draw_effect,   j* 64, i * 64, 64, 64);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::Release()
{
    draw_texture_manager_.Release();
}
//------------------------------------------------------------------------------
/**
*/
bool uiChaosZoneResult::ShowInterface(BOOL val, BOOL show_off)
{	
    return uiBase::ShowInterface(val, show_off);
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::OnShowWindow(BOOL val)
{
    if (val)
    {
        //RECT rect;
        //GetDialogWZ()->GetDlgSize(&rect);

        //int width = rect.right;
        //int height = rect.bottom;
        //int half_width = width >> 1;
        //int half_height = height >> 1;

        //POINT pt = g_pApplication->Get2DModeMapping();
        //long target_width = (pt.x >> 1) - half_width;

        //// â ������ ��ġ
        //// naklu test
        //static float control_height = 0.15f;
        //long target_height = (long)((pt.y * control_height) - half_height);
        ////
        ////long target_height = (pt.y >> 3) - half_height;

        //GetDialogWZ()->MoveWindowWZ((float)target_width, (float)target_height);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::MessageProc(SI_MESSAGE* message)
{
    if (this->IsLock())
    {
        return;
    }

    SolarDialog::MessageProc(message);
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::Process(DWORD tick)
{
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::OnUpdateSolarDialog()
{
    // ����� ƽ ����ϰ� ������ ������Ʈ
    DWORD progress_per_tick = CalculateProgress();

    if (animation_progress() == kAnimaion_Progress_End)
    {
        ShowInterface(FALSE);
    }

    UpdateFadeAnimation(progress_per_tick);
    UpdateScrollAnimation(progress_per_tick);
    UpdateResultDrawAnimation(progress_per_tick);
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::OnRenderSolarDialog()
{
    // OnUpdateSolarDialog() ���� ���� �س��� �� ����� ���⼭
    if ((animation_progress() == kAnimaion_Progress_ShowResult) && (effect_render_alpha_ != 0.0f))
    {
        // 2.���
        if (result_picture_pointer_)
        {
            RECT picture_rect = result_picture_pointer_->GetSizeRect();
            int center_x = (picture_rect.right + picture_rect.left) / 2;
            int center_y = (picture_rect.bottom + picture_rect.top) / 2;
            RECT rect;
            rect.left = center_x - 80;
            rect.top = center_y - 80;
            rect.right = rect.left + 160;
            rect.bottom = rect.top + 160;
            draw_texture_manager_.Draw(effect_string, rect, effect_render_alpha_);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::RegisterControl()
{
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001); // ���� ���
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003); // �Ķ� ���
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004); // ��ο�
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005); // ����
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007); // ��
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002); // �� ��ũ�Ѹ���
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000); // �Ʒ� ��ũ�Ѹ���
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::ShowChaosZoneResult(ChaosZoneResultType result_type)
{
    InitAnimaion(result_type);

    ShowInterface(TRUE);
}

//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::InitAnimaion(ChaosZoneResultType result_type)
{
    result_type_ = result_type;

    CControlWZ* control_pointer = NULL;
    for (DWORD index = kControl_Picture_P001; index <= kControl_Picture_P007; ++index)
    {
        control_pointer = static_cast<CControlWZ*>(GetControlWZ(GetControlID_byPosition(index)));
        if (control_pointer)
        {
            control_pointer->ShowWindowWZ(WZ_HIDE);
            control_pointer->SetDiscardMessage(true);
        }
    }
    control_pointer = GetControlWZ_byPosition(kControl_Picture_P000);
    if (control_pointer)
    {
        control_pointer->SetDiscardMessage(true);
    }
    control_pointer = GetControlWZ_byPosition(kControl_Picture_P002);
    if (control_pointer)
    {
        control_pointer->SetDiscardMessage(true);
    }

    // ��� ����
    if (result_type_ == kResultWin)
    {
        result_picture_pointer_ =
            static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P007)));
    }
    else if (result_type_ == kResultLose)
    {
        result_picture_pointer_ =
            static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P005)));
    }
    else //if (result_type_ == kResultDraw)
    {
        result_picture_pointer_ =
            static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P004)));
    }

    if (result_picture_pointer_)
    {
        result_picture_pointer_->ShowWindowWZ(WZ_SHOW);
        result_picture_pointer_->EnableInnerClipDraw(true);
    }

    // ��� �� ����
    if (g_pHero && g_pHero->GetTeam() == kRed)
    {
        background_picture_pointer_ =
            static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P001)));
    }
    else
    {
        background_picture_pointer_ =
            static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P003)));
    }

    if (background_picture_pointer_)
    {
        background_picture_pointer_->ShowWindowWZ(WZ_SHOW);
        background_picture_pointer_->EnableInnerClipDraw(true);
    }

    set_animation_progress(kAnimaion_Progress_ShowUI);
    start_animation_tick_ = clock_function::GetTickCount();
}
//------------------------------------------------------------------------------
/**
*/
DWORD uiChaosZoneResult::CalculateProgress()
{
    DWORD current_tick = clock_function::GetTickCount() - start_animation_tick_;

    DWORD accumulate_time = 0; // �����ð�
    for (int i = kAnimaion_Progress_ShowUI; i < kAnimaion_Progress_End; ++i)
    {
        if (current_tick < accumulate_time + progress_per_time_[i])
        {
            set_animation_progress(static_cast<AnimationProgress>(i));
            return (current_tick - accumulate_time); // ���� �ܰ� ƽ ����
        }
        accumulate_time += progress_per_time_[i];
    }

    set_animation_progress(kAnimaion_Progress_End);
    return 0;
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::UpdateFadeAnimation(DWORD progerss_tick)
{
    float fade_value = 1.0f;
    if (animation_progress() == kAnimaion_Progress_ShowUI)
    {
        fade_value = progerss_tick / static_cast<float>(progress_per_time_[kAnimaion_Progress_ShowUI]);
    }
    else if (animation_progress() == kAnimaion_Progress_HideUI)
    {
        fade_value = 1.0f - (progerss_tick / static_cast<float>(progress_per_time_[kAnimaion_Progress_HideUI]));
    }

    CControlWZ* down_scroll = GetControlWZ_byPosition(kControl_Picture_P000);
    if (down_scroll)
    {
        down_scroll->SetTransprency(fade_value);
    }
    CControlWZ* up_scroll = GetControlWZ_byPosition(kControl_Picture_P002);
    if (up_scroll)
    {
        up_scroll->SetTransprency(fade_value);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::UpdateScrollAnimation(DWORD progerss_tick)
{
    const int down_scroll_min_y = 20;
    const int down_scroll_max_y = 417;
    const int down_scroll_range = down_scroll_max_y - down_scroll_min_y; // 417 - 20

    float height_ratio = 0.0f;
    if (animation_progress() == kAnimaion_Progress_Open)
    {
        height_ratio = progerss_tick / static_cast<float>(progress_per_time_[kAnimaion_Progress_Open]);
    }
    else if (animation_progress() == kAnimaion_Progress_ShowResult)
    {
        height_ratio = 1.0f;
    }
    else if (animation_progress() == kAnimaion_Progress_Close)
    {
        height_ratio = 1.0f - (progerss_tick / static_cast<float>(progress_per_time_[kAnimaion_Progress_Close]));
    }

    // ��ũ�� ���� ������
    CControlWZ* down_scroll = GetControlWZ_byPosition(kControl_Picture_P000);
    if (down_scroll)
    {
        RECT dialog_rect;
        GetDialogWZ()->GetClientRect(&dialog_rect);
        RECT scroll_rect = down_scroll->GetSizeRect();
        float position_offset = down_scroll_min_y + (down_scroll_range * height_ratio);
        down_scroll->MoveWindowWZ(
            static_cast<float>(scroll_rect.left),
            static_cast<float>(dialog_rect.top + position_offset),
            static_cast<float>(scroll_rect.right - scroll_rect.left),
            static_cast<float>(scroll_rect.bottom - scroll_rect.top));
    }

    // ��� ��� �߶󳻱�
    if (background_picture_pointer_)
    {
        background_picture_pointer_->SetClipRateHeight(height_ratio);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResult::UpdateResultDrawAnimation(DWORD progerss_tick)
{
    float progress_ratio = 0.0f; // ������
    float text_ratio = 0.0f; // ����
    float clip_ratio = 1.0f; // �߷����� ����
    
    if (animation_progress() == kAnimaion_Progress_ShowResult)
    {
        // ������
        progress_ratio = progerss_tick / static_cast<float>(progress_per_time_[kAnimaion_Progress_ShowResult]);

        // ����̵� ���
        if ((0.1f < progress_ratio) && (progress_ratio < 0.6f))  // 10% ~ 60%
        {
            DWORD time_per_frame = 60;   // 1�����ӿ� ����� �ð� 0.06�� �� �ʴ� 1.6666 ������ (1000/60)

            // ����
            if (progress_ratio <= 0.2f)
            {
                effect_render_alpha_ = (progress_ratio - 0.1f) * 10.0f;
            }
            else if ((0.2f < progress_ratio) && (progress_ratio <= 0.5f))
            {
                effect_render_alpha_ = 1.0f;
                time_per_frame = static_cast<DWORD>(60 - ((progress_ratio - 0.2f) * 100));
            }
            else if (0.5f < progress_ratio)
            {
                effect_render_alpha_ = (0.6f - progress_ratio) * 10.0f;
            }


            // ȸ���� ����Ʈ
            // �� 16���� ��������Ʈ�� �̿��ϸ�, ������ �� 4���� �ִϸ��̼� �ǰ�, ���� ������ 12���� ������ ���� �ִϸ��̼��� �ȴ�.
            int frame_index = 0;
            if( progerss_tick <= time_per_frame * 4 )
            {
                // ������ 4������ (0,1,2,3 ������)
                frame_index = progerss_tick / time_per_frame;
            }
            else
            {
                // ���� 12�������� ����(4~15)
                frame_index = (progerss_tick - time_per_frame * 4 ) / time_per_frame;
                frame_index = frame_index % 12;
                frame_index += 4;
            }

            // 2.���
            if (result_type_ == kResultWin)
            {
                Sprintf(effect_string, _T("win_effect_%01d"), frame_index);
            }
            else if (result_type_ == kResultLose)
            {
                Sprintf(effect_string,_T("lose_effect_%01d"), frame_index);
            }
            else //if (result_type_ == kResultDraw)
            {
                Sprintf(effect_string,_T("draw_effect_%01d"), frame_index);
            }
        }
        
        // result text ���� ����
        if (progress_ratio < 0.3f)
        {
            text_ratio = 0.0f;
        }
        else if ((0.3f <= progress_ratio) && (progress_ratio < 0.5f))
        {
            text_ratio = (progress_ratio - 0.3f) * 5.0f;
        }
        else
        {
            text_ratio = 1.0f;
        }

    }
    else if (animation_progress() == kAnimaion_Progress_Close)
    {
        CCtrlPictureWZ* down_scroll = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(kControl_Picture_P000));
        if (result_picture_pointer_ && down_scroll)
        {
            RECT scroll_rect = down_scroll->GetSizeRect();
            RECT result_rect = result_picture_pointer_->GetSizeRect();

            int current_height = (scroll_rect.top + 11) - result_rect.top; // 11�� ��Ʈ�� ����
            clip_ratio = static_cast<float>(current_height) / (result_rect.bottom - result_rect.top);
            clip_ratio = min(max(0.0f, clip_ratio), 1.0f);
        }
      
        text_ratio = 1.0f;
    }

    if (result_picture_pointer_)
    {
        result_picture_pointer_->SetTransprency(text_ratio);
        result_picture_pointer_->SetClipRateHeight(clip_ratio);
    }
}

//------------------------------------------------------------------------------
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND