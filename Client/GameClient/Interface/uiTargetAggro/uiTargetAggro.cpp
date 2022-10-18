#include "SunClientPrecompiledHeader.h"

#include "uiTargetAggro/uiTargetAggro.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "Monster.h"
#include "Character.h"
#include "InterfaceManager.h"

void uiTargetAggro::RegisterControl()
{
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);// background top
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);// background middle
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);// background bottom
                                                                      
    // 버서커
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);// aggro 1st
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);// aggro 2nd
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);// aggro 3rd
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);// aggro 4th
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);// aggro 5th
    // 드래곤
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);// aggro 1st
    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);// aggro 2nd
    RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);// aggro 3rd
    RegisterControlPosition(StrToWzID("P011"), kControl_Picture_P011);// aggro 4th
    RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);// aggro 5th
    // 섀도우
    RegisterControlPosition(StrToWzID("P017"), kControl_Picture_P017);// aggro 5th
    RegisterControlPosition(StrToWzID("P016"), kControl_Picture_P016);// aggro 4th
    RegisterControlPosition(StrToWzID("P015"), kControl_Picture_P015);// aggro 3rd
    RegisterControlPosition(StrToWzID("P014"), kControl_Picture_P014);// aggro 2nd
    RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);// aggro 1st
    // 발키리
    RegisterControlPosition(StrToWzID("P018"), kControl_Picture_P018);// aggro 1st
    RegisterControlPosition(StrToWzID("P019"), kControl_Picture_P019);// aggro 2nd
    RegisterControlPosition(StrToWzID("P020"), kControl_Picture_P020);// aggro 3rd
    RegisterControlPosition(StrToWzID("P021"), kControl_Picture_P021);// aggro 4th
    RegisterControlPosition(StrToWzID("P022"), kControl_Picture_P022);// aggro 5th
    // 엘리멘탈
    RegisterControlPosition(StrToWzID("P023"), kControl_Picture_P023);// aggro 1st
    RegisterControlPosition(StrToWzID("P024"), kControl_Picture_P024);// aggro 2nd
    RegisterControlPosition(StrToWzID("P025"), kControl_Picture_P025);// aggro 3rd
    RegisterControlPosition(StrToWzID("P026"), kControl_Picture_P026);// aggro 4th
    RegisterControlPosition(StrToWzID("P027"), kControl_Picture_P027);// aggro 5th
    // 미스틱 //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    RegisterControlPosition(StrToWzID("P028"), kControl_Picture_P028);// aggro 1st
    RegisterControlPosition(StrToWzID("P029"), kControl_Picture_P029);// aggro 2nd
    RegisterControlPosition(StrToWzID("P030"), kControl_Picture_P030);// aggro 3rd
    RegisterControlPosition(StrToWzID("P031"), kControl_Picture_P031);// aggro 4th
    RegisterControlPosition(StrToWzID("P032"), kControl_Picture_P032);// aggro 5th

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    // Hellroid
    RegisterControlPosition(StrToWzID("P028"), kControl_Picture_P033);// aggro 1st
    RegisterControlPosition(StrToWzID("P029"), kControl_Picture_P034);// aggro 2nd
    RegisterControlPosition(StrToWzID("P030"), kControl_Picture_P035);// aggro 3rd
    RegisterControlPosition(StrToWzID("P031"), kControl_Picture_P036);// aggro 4th
    RegisterControlPosition(StrToWzID("P032"), kControl_Picture_P037);// aggro 5th
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    // 위블
    RegisterControlPosition(StrToWzID("P028"), kControl_Picture_P038);// aggro 1st
    RegisterControlPosition(StrToWzID("P029"), kControl_Picture_P039);// aggro 2nd
    RegisterControlPosition(StrToWzID("P030"), kControl_Picture_P040);// aggro 3rd
    RegisterControlPosition(StrToWzID("P031"), kControl_Picture_P041);// aggro 4th
    RegisterControlPosition(StrToWzID("P032"), kControl_Picture_P042);// aggro 5th
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);   // name 1st
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);   // name 2nd
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);   // name 3rd
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);   // name 4th
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);   // name 5th
}

uiTargetAggro::uiTargetAggro(InterfaceManager* ui_manager_ptr)
{

}

uiTargetAggro::~uiTargetAggro(void)
{
}

void uiTargetAggro::Init(CDrawBase* draw_base)
{
    int i = 0;
    gague_max_size_ = 106;
    display_type_ = kDisplay_Normal;
    last_class_code_ = eCHAR_TYPE_MAX;

    RegisterControl();

    for (i = kControl_Picture_P000; i < kBackground_Max; ++i)
    {
        backgrounds_[i] = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Picture_P000 + i)));
        assert(backgrounds_[i]);
        if (backgrounds_[i] != NULL)
        {
            backgrounds_[i]->SetDiscardMessage(true);
        }        
    }

    int control_position = 0;
    int class_type_max = (eCHAR_TYPE_MAX-1);
    for (i = 0; i < class_type_max; ++i)
    {
        for (int j = 0; j < kGague_Max; ++j)
        {
            control_position = kControl_Picture_P003 + (i*kGague_Max) + j;
            aggro_gagues_[i][j] = static_cast<CCtrlPictureWZ*>(GetControlWZ(GetControlID_byPosition(control_position)));
            // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
            //assert(aggro_gagues_[i][j]);
            if (aggro_gagues_[i][j] != NULL)
            {   
                aggro_gagues_[i][j]->SetDiscardMessage(true);
            }
        }
    }

    if (aggro_gagues_[0][0] != NULL)
    {
        gague_max_size_ = aggro_gagues_[0][0]->GetSize().right;
        aggro_gagues_[0][0]->SetSizeWidth(0);
    }

    for (i = 0; i < kText_Max; ++i)
    {
        aggro_names_[i] = static_cast<CCtrlStaticWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Text_S000 + i)));
        assert(aggro_names_[i]);
        if (aggro_names_[i] != NULL)
        {
            aggro_names_[i]->SetDiscardMessage(true);
        }        
    }

    MSG_CG_ARRGO_DISPLAY_CMD temp;
    temp.aggro_player_count_ = 0;
        
    SetAggroData(&temp);
}

void uiTargetAggro::Release()
{

}

void uiTargetAggro::OnShowWindow(BOOL is_show)
{

}

void uiTargetAggro::Process(DWORD tick_count)
{
    if (g_pHero)
    {
        Character* character_ptr = (Character*)g_ObjectManager.GetObject(g_pHero->m_CurTargetInfo.dwObjectKey);
        if ((character_ptr == NULL) && (IsVisible() == TRUE))
        {
            ShowInterface(FALSE);
        }
    }
}

void uiTargetAggro::MessageProc(SI_MESSAGE* message)
{
}

void uiTargetAggro::NetworkProc(MSG_BASE* message)
{
    if (message->m_byCategory == CG_BATTLE)
    {
        switch (message->m_byProtocol)
        {
        case CG_ARRGO_DISPLAY_CMD:
            {
                MSG_CG_ARRGO_DISPLAY_CMD* aggro_message = static_cast<MSG_CG_ARRGO_DISPLAY_CMD*>(message);
                SetAggroData(aggro_message);
            }
            break;
        }
    }
}

void uiTargetAggro::SetAggroData(MSG_CG_ARRGO_DISPLAY_CMD* aggro_data)
{
    TCHAR name_string[INTERFACE_STRING_LENGTH] = {0,};
    int player_count = 0;
    int class_code = 0;
    if (aggro_data != NULL)
    {
        player_count = aggro_data->aggro_player_count_;
    }

    float aggro_value = 0.0f;  
    int i = 0;

    //모든 게이지 숨기기
    for (int j = 0; j < MSG_CG_ARRGO_DISPLAY_CMD::MAX_AGGRO_DISPLAY; ++j)
    {
        if (aggro_names_[j] != NULL)
        {
            aggro_names_[j]->ShowWindowWZ(WZ_HIDE);
        }

        for (i = 0; i < (eCHAR_TYPE_MAX - 1); ++i)
        {
            if (aggro_gagues_[i][j] != NULL)
            {
                aggro_gagues_[i][j]->ShowWindowWZ(WZ_HIDE);
            }            
        }
    }

    DWORD max_rate_aggro = 0;
    if (player_count > 0)
    {
        max_rate_aggro = aggro_data->aggor_info_[0].aggro_value_;
    }

    if (max_rate_aggro <= 0)
    {
        max_rate_aggro = 1;
    }

    //어그로 데이터는 이미 정렬 되어서 온다.
    int skip_player = 0;
    for (i = 0; i < aggro_data->aggro_player_count_; ++i)
    {
        if (aggro_data->aggor_info_[i].aggro_value_ <= 0.0f)
        {
            --player_count;
            ++skip_player;
            continue;
        }
        
        class_code = aggro_data->aggor_info_[i].class_type_;// 패킷에서 찾아와야 한다.
        if (class_code > 0)
        {
            --class_code;
        }
        
        if (i >= skip_player)
        {
            int index = i - skip_player;
            aggro_value = static_cast<float>(aggro_data->aggor_info_[i].aggro_value_);

            if (aggro_data->aggor_info_[i].name_code_ == 0)
            {
                SetDisplay(aggro_data->aggor_info_[i].character_name_,index, aggro_value, max_rate_aggro);
            }
            else
            {
                g_InterfaceManager.GetInterfaceString(aggro_data->aggor_info_[i].name_code_, name_string, INTERFACE_STRING_LENGTH);
                SetDisplay(name_string, index, aggro_value, max_rate_aggro);
            }
            aggro_value = ((float)aggro_value / max_rate_aggro) * gague_max_size_;
            if (aggro_gagues_[class_code][index] != NULL)
            {
                aggro_gagues_[class_code][index]->ShowWindowWZ(WZ_SHOW);
                aggro_gagues_[class_code][index]->SetSizeWidth((long)aggro_value);
            }            
        }
    }

    if (player_count <= 0)  // 어그로가 모두 0인 경우 들어 온다    
    {
        bool is_target_now = false;
        if (g_pHero && aggro_data->aggro_player_count_ > 0) // 플레이어가 있는데 모두 0인 경우
        {
            Monster* monster_ptr = (Monster*)g_ObjectManager.GetObject(g_pHero->m_CurTargetInfo.dwObjectKey);
            if (monster_ptr && monster_ptr->GetTargetID())
            {
                Character* character_ptr = (Character*)g_ObjectManager.GetObject(monster_ptr->GetTargetID()); // 타겟인 캐릭터를 지정
                if (character_ptr && character_ptr->GetObjectKey() != g_pHero->m_CurTargetInfo.dwObjectKey)
                {
                    SetDisplay((TCHAR*)character_ptr->GetName(), 0, 0.0f, max_rate_aggro);
                    class_code = character_ptr->GetClass();
                    if (class_code > 0)
                    {
                        --class_code;
                    }
                    last_class_code_ = class_code;
                    if (aggro_gagues_[class_code][0] != NULL)
                    {
                        aggro_gagues_[class_code][0]->ShowWindowWZ(WZ_SHOW);
                        aggro_gagues_[class_code][0]->SetSizeWidth((long)gague_max_size_);   
                    }                    
                }
                else // 마지막 값을 사용
                {
                    if (last_class_code_ < (eCHAR_TYPE_MAX-1))//마지막 값이 있다면 표시 하지만 없으면(else)안함
                    {
                        SetDisplay(NULL, 0, 0.0f, max_rate_aggro);
                        if (aggro_gagues_[last_class_code_][0] != NULL)
                        {
                            aggro_gagues_[last_class_code_][0]->ShowWindowWZ(WZ_SHOW);
                            aggro_gagues_[last_class_code_][0]->SetSizeWidth((long)gague_max_size_);
                        }                        
                    }                    
                }
                is_target_now = true;
            }
        }
        
        if (is_target_now == false)    // 플레이어도 없는 경우
        {
            if (display_type_ == kDisplay_Numeric || display_type_ == kDisplay_Percent)
            {
                aggro_names_[0]->SetTextWZ("No Aggro");
            }
            else
            {
                aggro_names_[0]->SetTextWZ("");
            }            
            aggro_names_[0]->ShowWindowWZ(WZ_SHOW);
            aggro_names_[0]->SetStyle(DT_CENTER);
        }
    }

    ResizeTargetAggros(player_count);
}

void uiTargetAggro::ResizeTargetAggros(BYTE player_count)
{
    BYTE player_aggro_index = 0;
    if (player_count > 0)
    {
        player_aggro_index = player_count - 1;
    }

    RECT positions = aggro_gagues_[0][player_aggro_index]->GetSizeRect();
    RECT middle_position = backgrounds_[kControl_Picture_P001]->GetSizeRect();
    middle_position.right = middle_position.right - middle_position.left;
    middle_position.bottom = middle_position.bottom - middle_position.top;
    RECT bottom_position = backgrounds_[kControl_Picture_P002]->GetSizeRect();
    bottom_position.right = bottom_position.right - bottom_position.left;
    bottom_position.bottom = bottom_position.bottom - bottom_position.top;

    backgrounds_[kControl_Picture_P001]->MoveWindowWZ((float)middle_position.left,
        (float)middle_position.top, (float)middle_position.right, (float)((positions.bottom - middle_position.top) + 1));
    backgrounds_[kControl_Picture_P002]->MoveWindowWZ((float)bottom_position.left,
        (float)positions.bottom + 1, (float)bottom_position.right, (float)bottom_position.bottom);

    RECT dialog_rect;
    GetDialogWZ()->GetClientRect(&dialog_rect);
    RECT disable_rect = {dialog_rect.left, dialog_rect.top, dialog_rect.right, dialog_rect.bottom};	// 무효화 영역
    disable_rect.top = backgrounds_[kControl_Picture_P002]->GetSizeRect().bottom;
    DisableInterSectRect(disable_rect);
}

void uiTargetAggro::SetDisplay(TCHAR* character_name, int index, float aggro_value, DWORD max_rate_aggro)
{
    TCHAR final_string[INTERFACE_STRING_LENGTH] = {0,};

    if (character_name != NULL)
    {
        switch (display_type_)
        {
        case kDisplay_Numeric:
            {
                int aggro_value_number = static_cast<int>(aggro_value);
                sprintf(final_string, "%d:%s", aggro_value_number, character_name);
                aggro_names_[index]->SetStyle(DT_LEFT); 
            }
            break;
        case kDisplay_Percent:
            {
                aggro_value = (aggro_value / max_rate_aggro) * 100.0f;
                sprintf(final_string, "%.1f%%:%s", aggro_value, character_name);
                aggro_names_[index]->SetStyle(DT_LEFT);
            }
            break;
            //case kDisplay_Normal:
        default:
            {
                strcpy(final_string, character_name);
                aggro_names_[index]->SetStyle(DT_CENTER);
            }
            break;
        }
        aggro_names_[index]->SetTextWZ(final_string);
    }
    aggro_names_[index]->ShowWindowWZ(WZ_SHOW);
    aggro_names_[index]->SetTextKind(ETK_OUTLINE);
}
