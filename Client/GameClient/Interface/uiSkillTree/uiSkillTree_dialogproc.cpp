//------------------------------------------------------------------------------
//  (C) 2010 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#ifndef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
#include "uiSkillTree.h"
#include "SkillSlot.h"
#include "MouseHandler.h"
#include "Mouse.h"
#include "Hero.h"
#include "ItemManager.h"
#include "SlotKeyGenerator.h"
#include "SkillInfoParser.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "SkillMain.h"
#include "Command.h"
#include "uiSystemMan/uiSystemMan.h"
#include "StringTableInfo.h"
#include "SoundEffect.h"

//------------------------------------------------------------------------------
WzID uiSkillTree::wzId_[kDlgMax] = 
{ 
    // 스킬슬롯1
    StrToWzID("B010"), StrToWzID("B000"), StrToWzID("B001"), 
    StrToWzID("B004"), StrToWzID("B003"), StrToWzID("B002"),
    // SKILL_ROW_2
    StrToWzID("B005"), StrToWzID("B006"), StrToWzID("B007"),
    StrToWzID("B008"), StrToWzID("B009"), StrToWzID("B011"),
    // SKILL_ROW_3
    StrToWzID("B018"), StrToWzID("B019"), StrToWzID("B020"), 
    StrToWzID("B021"), StrToWzID("B022"), StrToWzID("B023"),
    // SKILL_ROW_4
    StrToWzID("B012"), StrToWzID("B013"), StrToWzID("B014"),
    StrToWzID("B015"), StrToWzID("B016"), StrToWzID("B017"),
    // SKILL_ROW_5
    StrToWzID("B031"), StrToWzID("B032"), StrToWzID("B033"),
    StrToWzID("B034"), StrToWzID("B035"), StrToWzID("B036"),
    // SKILL_ROW_6
    StrToWzID("B024"), StrToWzID("B025"), StrToWzID("B026"),
    StrToWzID("B027"), StrToWzID("B028"), StrToWzID("B030"),

    // PLUS_BTN_ROW_1
    StrToWzID("B037"), StrToWzID("B038"), StrToWzID("B039"),
    StrToWzID("B040"), StrToWzID("B041"), StrToWzID("B042"),
    // PLUS_BTN_ROW_2
    StrToWzID("B043"), StrToWzID("B044"), StrToWzID("B045"),
    StrToWzID("B046"), StrToWzID("B047"), StrToWzID("B048"),
    // PLUS_BTN_ROW_3
    StrToWzID("B058"), StrToWzID("B059"), StrToWzID("B060"),
    StrToWzID("B049"), StrToWzID("B050"), StrToWzID("B051"),
    // PLUS_BTN_ROW_4
    StrToWzID("B057"), StrToWzID("B056"), StrToWzID("B055"),
    StrToWzID("B052"), StrToWzID("B053"), StrToWzID("B054"),
    // PLUS_BTN_ROW_5
    StrToWzID("B070"), StrToWzID("B071"), StrToWzID("B072"),
    StrToWzID("B061"), StrToWzID("B062"), StrToWzID("B063"),
    // PLUS_BTN_ROW_6
    StrToWzID("B069"), StrToWzID("B068"),StrToWzID("B067"),
    StrToWzID("B064"), StrToWzID("B065"),StrToWzID("B066"),

    //숙련1.2 텍스트
    StrToWzID("S033"), StrToWzID("S000"),
    
    // X버튼.스킬.퍽.액션.이모티콤
    StrToWzID("B999"), StrToWzID("C005"), StrToWzID("C006"), StrToWzID("C000"),
    
    // 버서커.드래곤.섀도우.발키리.앨리
    StrToWzID("P001"), StrToWzID("P002"), StrToWzID("P004"), StrToWzID("P005"), StrToWzID("P003"),

    // 적용, 리셋 버튼
    StrToWzID("B074"), StrToWzID("B073"),

    // 적용 버튼 위의 깜빡임 이미지
    StrToWzID("P000"),

    // 잔여 스킬포인트 영역의 투명 이미지 (툴팁에 사용)
    StrToWzID("P006")


};

//------------------------------------------------------------------------------
/** 
*/
inline static int GetHeroIndex()
{
    int hero_index = g_pHero->GetClass() - 1;
    hero_index = max(hero_index, 0);
    hero_index = min(hero_index, uiSkillTree::kMaxClass - 1);
    return hero_index;
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    // 컨트롤러 유효성 체크
    for (int idx = 0; idx < kDlgMax; ++idx)
    {
        if (this->getControl(idx) == NULL)
        {
            invalid_control_ = true;
            break;
        }
    }

	skill_tree_effect_.Init();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::Release()
{
    FlushUnitDraw();
    SAFE_RELEASENDELETE(item_renderer_);

	skill_tree_effect_.Release();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::Clear()
{
    FlushUnitDraw();

    xGetSkillData()->ClearSkillData();
}

//------------------------------------------------------------------------------
/** 
*/
void uiSkillTree::InitItemRender()
{
    if (GetItemUnitRender() == NULL)
    {
        item_renderer_ = new ItemUnitRenderer;
        item_renderer_->Init( uiBase::GetDrawBase(), GetMaxSlotNum() );
        item_renderer_->ShowCooldown(TRUE);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::FlushUnitDraw()
{
    for (int idx = 0; idx < GetMaxSlotNum(); ++idx)
    {
        if (IsEmpty(idx) == FALSE)
        {
            g_SlotKeyGenerator.Restore(DWORD(GetSlot(idx).GetSerial()));
            DeleteSlot(idx, NULL);
        }
    }
    ClearAll();
}

//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::RefreshUI()
{
    RefreshTree();
    RefreshSkillIcon();
    RefreshBackGround();
    RefreshTab();
    RefreshControlText();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::OnUpdateSolarDialog()
{
    if (invalid_control())
    {
        return;
    }

#if WZENGINEVER >= 320

    if( CControlWZ* control = GetControlWZ( wzId_[kButtonBlendImage] ) )
    {

        static DWORD animation_start_time_  = 0;

        if( xGetSkillData()->IsSkillSimulated() == true )
        {
            if( animation_start_time_ == 0 )
            {
                animation_start_time_ = GetCurrentTime();
            }

            float rate = 1.f;
            DWORD current_time = GetCurrentTime();
            DWORD delta_time = current_time - animation_start_time_;
            float time = (float)delta_time * WZ_PI * 2.f / 1000.f;
            rate = cos( time ) * 0.5f + 0.5f;

            control->SetTransprency( rate );
        }
        else
        {
            animation_start_time_ = 0;
            control->SetTransprency( 1.f );
        }
    }
#endif


    UpdateUnitRender();
    UpdateMouse();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::OnRenderSolarDialog()
{
    if (invalid_control() ||
       (GetItemUnitRender() == NULL))
    {
        return;
    }

    GetItemUnitRender()->Render();
    RenderSlotImage();

	RECT rect;
	GetDialogWZ()->GetClientRect(&rect);
    skill_tree_effect_.RenderOpenEffect(rect);

    RenderToolTip();
}

//------------------------------------------------------------------------------
/**
*/
VOID uiSkillTree::ShowDialog( BOOL val )
{
    if( val == TRUE )
    {
        xGetSkillData()->ResetTreeData();
        RefreshArrowImage();
        RefreshSkillSlot();
    }
    else if( val == FALSE )
    {
        xGetSkillData()->ResetTreeData();
    }


    uiBase::ShowDialog( val );
}

//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::OnShowWindow(BOOL val)
{
    uiSkillMan* ui_skill_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);

    if (invalid_control())
    {
        return;
    }

    ui_skill_man->OpenDialog(uiSkillMan::kSkillTree, val);

    if (val)
    {
        RefreshUI();
    }
    else
    {
#ifdef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2
        skill_tree_effect_.SetPlayAnimation(false);
#else 
        if( skill_tree_effect_.GetOpenLevelEffectState() != OpenLevelEffect::kAniNone )
        {
            skill_tree_effect_.SetState( OpenLevelEffect::kAniNone );
        }
#endif //_NA_20111213_SKILLTREE_VISUAL_RENEWAL2
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::MessageProc(SI_MESSAGE* message)
{
    if (invalid_control())
    {
        return;
    }

    POSTYPE pos = GetControlPos(message->dwCtrlID);

    switch (message->eResultMsg) 
    {
    case RT_MSG_RBUTTONCLICK:
        {
            MouseClickSlot(pos, false);
        }
        break;
    case RT_MSG_MOUSEOVER:
        {
            is_mouse_overed_on_button_ = false;

            if( pos == kButton1 )
            {
                mouse_overed_control_ = pos;
                is_mouse_overed_on_button_ = true;
            }
            else if( pos == kButton2 )
            {
                mouse_overed_control_ = pos;
                is_mouse_overed_on_button_ = true;
            }
            else if( pos == kRemainSkillPoint )
            {
                mouse_overed_control_ = pos;
                is_mouse_overed_on_button_ = true;
            }
            break;
        }
    case RT_MSG_LBUTTONCLICK:
        {
            MouseClickButton(pos);
            MouseClickSlot(pos, true);
            OnClickPlusButton(pos);
            MouseClickTab(pos);

            if (pos == kEscButton)
            {
                ShowInterface(FALSE);
            }
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
        break;
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::MouseClickTab(POSTYPE pos)
{
    if ((pos < kTabBein) ||
        (pos > kTabEnd))
    {
        return;
    }
    
    RefreshTab();
    
    uiSkillMan* ui_skill_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);
    ui_skill_man->OpenTab((pos - kTabBein) + 1);
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::UpdateUnitRender()
{
    if (GetItemUnitRender() == NULL)
    {
        return;
    }

    GetItemUnitRender()->Update();

    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();

    ItemUnitRenderer::RenderUnit* render_unit = NULL;
    for (size_t i = 0; i < skill_set.size(); ++i)
    {
        const SkillData& skill_data = skill_set.at(i);
        SCSlot& slot = GetSlot( skill_data.GetSlotPosition() );

        render_unit = GetItemUnitRender()->GetItemUnit( slot.GetSerial() );
        if (render_unit)
        {
			render_unit->SetState( skill_data.GetBlockState() ? skill_data.GetBlockState() : skill_data.GetState() );
            render_unit->SetShow(TRUE);
        }
    }
}

void uiSkillTree::MouseClickButton(POSTYPE pos)
{
    if ( pos == kButton1 )
    {
        std::vector<int> new_code_array;
        if( xGetSkillData()->MakeNewSkillCodeArray( new_code_array ) == true )
        {
            MSG_CG_SKILL_SELECT_SKILLPOINT_SYN send_packet;
            send_packet.skill_count = new_code_array.size();
            for( size_t i=0;i!=new_code_array.size();++i)
            {
                send_packet.skill_code_array[i] = new_code_array[i];
            }


            // 2803	스킬을 배우시곘습니까?
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString( eST_SKILL_TREE_LEARN_MESSAGEBOX, szMessage, INTERFACE_STRING_LENGTH );
            if( uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM) )
            {
                pSystemMan->Sys_ConfirmLock(szMessage, new SendPacketCommand( CI_GAMESERVERIDX, &send_packet, sizeof(send_packet) ) );
            }
        }

        // 적용
        pos = pos;
        return;
    }
    else if( pos == kButton2 )
    {
        xGetSkillData()->ResetTreeData();
        RefreshArrowImage();
        RefreshSkillSlot();
        RefreshUI();

        // 리셋
        return;
    }
}

//------------------------------------------------------------------------------
/** 마우스클릭처리(우클릭:집기, 좌클릭:사용)
*/
void uiSkillTree::MouseClickSlot(POSTYPE pos, bool pickup)
{
    // 애니상태에서는 막기
#ifdef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2
    if( skill_tree_effect_.IsPlayingAnimation() == true )
    {
        return;
    }
#else
    if ( skill_tree_effect_.IsPlayingEffect() == true )
    {
        return;
    }
#endif

    if ((pos > kSlotEnd) || IsEmpty(pos) )
    {
        return;
    }

    if ( SkillData* skill_data = xGetSkillData()->GetSkillDataSetForTree().GetSkillDataBySlotPosition(pos) )
	{

        if( xGetSkillData()->IsSkillSimulated() )
        {
            // 2804, 스킬 시뮬레이션 중에는 스킬 아이콘을 이동, 사용할 수 없습니다. 
            TCHAR buffer[INTERFACE_STRING_LENGTH];
            g_InterfaceManager.GetInterfaceString(eST_SKILL_TREE_BEGIN_NOTICE, buffer, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_SysMsg(buffer);

            return;
        }


		if ( skill_data->IsActiveState() )
		{
			if (pickup)
			{
				RECT rect = getControl(pos)->GetSizeRect();
				MouseHandler::Instance()->ItemTransaction(SI_SKILL, pos, &rect);
			}
			else
			{
				MouseHandler::Instance()->ItemUseTransaction(SI_SKILL, pos);
			}
		}
	}
}


//------------------------------------------------------------------------------
/** 레벨업 버튼
*/
void uiSkillTree::OnClickPlusButton(POSTYPE pos)
{
    // 애니상태에서는 막기
#ifdef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2
    if( skill_tree_effect_.IsPlayingAnimation() == true )
    {
        return;
    }
#else
    if ( skill_tree_effect_.IsPlayingEffect() == true )
    {
        return;
    }
#endif

    // 버튼 영역체크
    if ((pos < kPlusBegin) ||
        (pos > kPlusEnd))
    {
        return;
    }
  
    if( SkillData* skill_data = xGetSkillData()->GetSkillDataSetForTree().GetSkillDataBySlotPosition(pos - kPlusBegin) )
    {
        g_SoundEffect.PlayUI( UI_SOUND_SKILLTREE_BUTTON );

        if( xGetSkillData()->LevelupSkill(skill_data) == true )
        {
            // 이 스킬을 습득했을때 활성화 되는 화살표가 있으면 표시하도록 설정
            skill_tree_effect_.SetAnimationArrow( skill_data->GetSkillClassCode() , skill_data->GetSkillLevel() );

            RefreshTree(true);
            RefreshSkillSlot();
        }
    }
}
//------------------------------------------------------------------------------
/** 슬롯돌며 마우스 오버 체크
*/
void uiSkillTree::UpdateMouse()
{
    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();
    
    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for (size_t i = 0; i < skill_set.size(); ++i)
    {
        POSTYPE slot_pos = skill_set[i].GetSlotPosition();
        
        RECT rect = getControl(slot_pos)->GetSizeRect();
        if (rect.left <= mouse_x && rect.right >= mouse_x)
        {
            if (rect.top <= mouse_y && rect.bottom >= mouse_y)
            {
                m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SKILL;
                m_MouseOverObject.iSkillIdx = i;
                break;
            }
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::RenderToolTip()
{
    // 버튼의 툴팁
    if (is_mouse_overed_on_button_ && 
        (mouse_overed_control_ != INVALID_POSTYPE_VALUE))
    {
        CControlWZ* control_ptr = getControl(mouse_overed_control_);
        if (control_ptr != NULL)
        {
            if( uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP) )
            {
                int string_index;
                if( mouse_overed_control_ == kButton1 )
                {
                    string_index = eST_SKILL_TREE_LEARN_APPLY;
                }
                else if( mouse_overed_control_ == kButton2 )
                {
                    string_index = eST_SKILL_TREE_LEARN_RESET;
                }
                else if( mouse_overed_control_ == kRemainSkillPoint )
                {
                    string_index = eST_SKILL_TREE_TOOLTIP_REMAIN_POINT;
                }

                static TCHAR buffer[INTERFACE_STRING_LENGTH];

                g_InterfaceManager.GetInterfaceString( string_index, buffer, INTERFACE_STRING_LENGTH );
                tooltip_manager->RegisterTooltip( buffer );
				tooltip_manager->SetTooltipDialogID( GetDialogKey() );

            }
        } 
    }

    if (m_MouseOverObject.eMouseOverType != eMOUSE_OVER_SKILL)
    {
        return;
    }

    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();

    const SkillData& skill_data = skill_set.at((size_t)m_MouseOverObject.iSkillIdx);
    SCSkillSlot& skill_slot = static_cast<SCSkillSlot &>(GetSlot(skill_data.GetSlotPosition()));
      
    if (uiToolTipMan* tooltip = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP) )
    {
       tooltip->RegisterTooltipSkill(skill_data.GetCurSkillInfo(),
                                     skill_data.GetNextSkillInfo(), 
                                     skill_data.IsActiveState());
	   tooltip->SetTooltipDialogID(GetDialogKey());
    }



}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::RenderSlotImage()
{
    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for (size_t i = 0; i < skill_set.size(); ++i)
    {
        const SkillData& skill_data = skill_set[i];
        if (skill_data.GetCurSkillInfo() == NULL)
        {
            continue;
        }
        
        // 다음레벨없으면 Max Level
        if (skill_data.GetNextSkillInfo() == NULL)
        {
			skill_tree_effect_.RenderMaxLevel( getControl(skill_data.GetSlotPosition() + kSlotBegin)->GetSizeRect() );
        }

        // Stat Level
        RECT rect = getControl(skill_data.GetSlotPosition() + kPlusBegin)->GetSizeRect();
        rect.left += 22;
        rect.top += 2;
        int level = skill_data.GetSkillLevel();

        if (skill_data.IsActiveState() == false )
        {
            // 표시안함
            // 0레벨이지만 1레벨로 표시
            //level = 1;
            //g_InterfaceManager.RenderDrawNumber(rect, level, DT_LEFT | DT_TOP);
        }
        else
        {
            if( level > 0 )
            {
                g_InterfaceManager.RenderDrawNumber(rect, level, DT_LEFT | DT_TOP);
            }
        }
    }
}
//------------------------------------------------------------------------------
/** 텍스트(숙련1, 숙련2)
*/
void uiSkillTree::RefreshControlText()
{
    static const int string_list[kMaxClass][2] = 
    {
        {eST_BUTTON_TEXT_MASTERY1_BERSERKER, eST_BUTTON_TEXT_MASTERY2_BERSERKER},
        {eST_BUTTON_TEXT_MASTERY1_DRAGON, eST_BUTTON_TEXT_MASTERY2_DRAGON},
        {eST_BUTTON_TEXT_MASTERY1_SHADOW, eST_BUTTON_TEXT_MASTERY2_SHADOW},
        {eST_BUTTON_TEXT_MASTERY1_VALKIRYE, eST_BUTTON_TEXT_MASTERY2_VALKIRYE},
        {eST_BUTTON_TEXT_MASTERY1_ELEMENTAL, eST_BUTTON_TEXT_MASTERY2_ELEMENTAL},

    };

    // 숙련1
    int hero_index = GetHeroIndex();
    TCHAR text[INTERFACE_STRING_LENGTH];
    int string_code = string_list[hero_index][0];
    g_InterfaceManager.GetInterfaceString(string_code, text, INTERFACE_STRING_LENGTH);
    getControl(kTextExt1)->SetTextWZ(text);

    // 숙련2
    string_code = string_list[hero_index][1];
    g_InterfaceManager.GetInterfaceString(string_code, text, INTERFACE_STRING_LENGTH);
    getControl(kTextExt2)->SetTextWZ(text);
}

//------------------------------------------------------------------------------
/** 탭버튼
*/
void uiSkillTree::RefreshTab()
{
    // Tab
    CCtrlButtonCheckWZ* button = NULL;
    for (int idx = 0; idx < kMaxTab; ++idx)
    {
        button = static_cast<CCtrlButtonCheckWZ*>(getControl(idx + kTabBein));
        button->SetCheckState((idx == 0) ? (true) : (false));
    }
}
//------------------------------------------------------------------------------
/** 뒷배경
*/
void uiSkillTree::RefreshBackGround()
{
    // Back
    for (int idx = 0; idx < kMaxClass; ++idx)
    {
        getControl(idx + kImgBegin)->ShowWindowWZ(WZ_HIDE);
    }
    getControl(GetHeroIndex() + kImgBegin)->ShowWindowWZ(WZ_SHOW);

  
}
//------------------------------------------------------------------------------
/**  아이콘 그리기
*/
void uiSkillTree::RefreshSkillIcon()
{
    GetItemUnitRender()->FlushUnitDraw();

    int max_slot = GetMaxSlotNum();
    for( int idx = 0; idx < max_slot; ++idx)
    {
        if (IsEmpty(idx) == FALSE)
        {
            SCSlot& slot = SCSlotContainer::GetSlot(idx);
            if (GetItemUnitRender()->GetItemUnit(slot.GetSerial()) == false)
            {
                GetItemUnitRender()->AddItemUnit(slot.GetSerial(), slot.GetCode(),
                                                &getControl(idx)->GetSizeRect(), &slot, 
                                                getControl(idx)); 
            }
        }
    }
}

#endif 