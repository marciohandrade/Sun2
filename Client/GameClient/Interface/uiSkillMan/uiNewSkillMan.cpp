
//------------------------------------------------------------------------------
//  (C) 2010 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "uiNewSkillMan.h"
#include "SkillInventoryStoreParser.h"
#include "SkillSlot.h"
#include "uiSkillTree/uiSkillTree.h"
#include "uiSkillAction/uiNewSkillAction.h"
#include "uiPerkDialog/uiPerkDialog.h"
#include "uiPerkSlot/uiPerkSlot.h"
#include "ItemUnitRenderer.h"
#include "Hero.h"
#include "PlayerAttributes.h"
#include "QuickContainer.h"
#include "QuickSlot.h"
#include "GlobalFunc.h"
#include "SkillInfoParser.h"
#include "SoundEffect.h"
#include "SkillMain.h"
#include "Command.h"


#if (WZENGINEVER >= 310)

////////////////////////////////////////////////////////////////////////////////////////////////
class SkillTreeSound
{
public:
    enum SoundType
    {
        TYPE_BUTTON = 0,

        TYPE_KEYOPEN,       // 좌물쇠 깜빡일때
        TYPE_KEYDOWN,       // 좌물쇠와 철사가 내려갈떄
        TYPE_KEYCLOSE,      // 좌물쇠 깜빡일떄 (사라지기전)

        TYPE_BLUEEFFECT,    // 토네이도 시작시
        TYPE_BLINK_BLOCK,   // 봉인된 스킬창이 풀리며 깜빡일때

        TYPE_BARDOWN1,
    };

private:
    std::vector<int> m_SoundHandleArray;
    std::vector<int> m_SoundArray;
    int tab_level_;

public:
    SkillTreeSound()
    {
        tab_level_ = 0;

        m_SoundArray.push_back( UI_SOUND_SKILLTREE_BUTTON );

        m_SoundArray.push_back( UI_SOUND_SKILLTREE_KEYOPEN );
        m_SoundArray.push_back( UI_SOUND_SKILLTREE_KEYDOWN );
        m_SoundArray.push_back( UI_SOUND_SKILLTREE_KEYCLOSE );

        m_SoundArray.push_back( UI_SOUND_SKILLTREE_BLUEEFFECT );
        m_SoundArray.push_back( UI_SOUND_SKILLTREE_BLINKBLOCK );

        m_SoundArray.push_back( UI_SOUND_SKILLTREE_GAUGEDOWN_01 );
        m_SoundArray.push_back( UI_SOUND_SKILLTREE_GAUGEDOWN_02 );
        m_SoundArray.push_back( UI_SOUND_SKILLTREE_GAUGEDOWN_03 );
        m_SoundArray.push_back( UI_SOUND_SKILLTREE_GAUGEDOWN_04 );
        m_SoundArray.push_back( UI_SOUND_SKILLTREE_GAUGEDOWN_05 );
        m_SoundArray.push_back( UI_SOUND_SKILLTREE_GAUGEDOWN_05 );
    }

    void PlaySkillLevelup()
    {
        g_SoundEffect.PlayUI( m_SoundArray[TYPE_BUTTON] );
    }

    void InitAnimationSound( int tab_level )
    {
        m_SoundHandleArray.clear();
        tab_level_ = tab_level;
    }

    void PlayAnimationSound( SoundType ani_sound_type )
    {
        int sound_handle = -1;

        if( ani_sound_type == TYPE_KEYOPEN )
        {
            sound_handle = g_SoundEffect.PlayUI( m_SoundArray[TYPE_KEYOPEN] );
        }
        else if( ani_sound_type == TYPE_BLINK_BLOCK )
        {
            sound_handle = g_SoundEffect.PlayUI( m_SoundArray[TYPE_BLINK_BLOCK] );
        }
        else if( ani_sound_type == TYPE_KEYDOWN )
        {
            sound_handle = g_SoundEffect.PlayUI( m_SoundArray[TYPE_KEYDOWN] );
        }
        else if( ani_sound_type == TYPE_KEYCLOSE )
        {
            sound_handle = g_SoundEffect.PlayUI( m_SoundArray[TYPE_KEYCLOSE] );
        }
        else if( ani_sound_type == TYPE_BLUEEFFECT )
        {
            sound_handle = g_SoundEffect.PlayUI( m_SoundArray[TYPE_BLUEEFFECT] );
        }
        else if( ani_sound_type == TYPE_BARDOWN1 )
        {
            sound_handle = g_SoundEffect.PlayUI( m_SoundArray[TYPE_BARDOWN1 + tab_level_] );
        }

        m_SoundHandleArray.push_back( sound_handle );
    }

    void StopAnimationSound()
    {
        for( size_t i=0;i!=m_SoundHandleArray.size();++i)
        {
            g_SoundEffect.StopUI( m_SoundHandleArray[i] );
        }
        g_SoundEffect.Clear();
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////
SkillTreeSound local_skill_sound;

////////////////////////////////////////////////////////////////////////////////////////////////
#endif // (WZENGINEVER >= 310) 

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
const DWORD IM_SKILL_MANAGER::SKILL_TREE_DLG = StrToWzID("4211");     // 스킬
const DWORD IM_SKILL_MANAGER::SKILL_PERK_DLG = StrToWzID("4212");     // 퍽
const DWORD IM_SKILL_MANAGER::SKILL_ACTION_DLG = StrToWzID("4213");   // 액션.감정
#else
const DWORD IM_SKILL_MANAGER::SKILL_TREE_DLG = StrToWzID("4260");     // 스킬
const DWORD IM_SKILL_MANAGER::SKILL_PERK_DLG = StrToWzID("4270");     // 퍽
const DWORD IM_SKILL_MANAGER::SKILL_ACTION_DLG = StrToWzID("4290");   // 액션.감정
#endif
const DWORD IM_SKILL_MANAGER::SKILL_PERK_SLOT_DLG = StrToWzID("4280");// 퍽등록슬롯UI

//------------------------------------------------------------------------------
/**
*/
uiSkillMan::uiSkillMan(InterfaceManager *pUIMan) : 
uiBaseMan(pUIMan),
init_data_(false),
current_dialogkey_(IM_SKILL_MANAGER::SKILL_TREE_DLG)
{
    ZeroMemory(&ui_position_, sizeof(ui_position_));
    ZeroMemory(&skill_dlg_, sizeof(skill_dlg_));
   
}

//------------------------------------------------------------------------------
/**
*/
void uiSkillMan::OnInitialize()
{

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    const char* ui_skill_tree_name = "Data\\Interface\\42_11_Skill_Window.iwz";
    const char* ui_perk_filename = "Data\\Interface\\42_12_Perk_Window.iwz";
    const char* ui_action_name = "Data\\Interface\\42_13_Action_Emote.iwz";
#else
    // 스킬트리
    const char* ui_skill_tree_name = "Data\\Interface\\42_6_Skill_Window.iwz";
    const char* ui_perk_filename = "Data\\Interface\\42_7_Perk_Window.iwz";
    const char* ui_action_name = "Data\\Interface\\42_9_Action_Emote.iwz";
#endif

    uiSkillTree* tree =  CreateUIDialog<uiSkillTree>(IM_SKILL_MANAGER::SKILL_TREE_DLG, ui_skill_tree_name, this, TRUE);
    uiPerkDialog* perk =  CreateUIDialog<uiPerkDialog>(IM_SKILL_MANAGER::SKILL_PERK_DLG, ui_perk_filename, this, TRUE);
    uiSkillAction* action = CreateUIDialog<uiSkillAction>(IM_SKILL_MANAGER::SKILL_ACTION_DLG, ui_action_name, this);

#ifdef _NA_000000_20130114_RENEWER_UI
#else
    const char* ui_perkslot_filename = "Data\\Interface\\42_8_Perkslot.iwz";
    CreateUIDialog<uiPerkSlot>(IM_SKILL_MANAGER::SKILL_PERK_SLOT_DLG,  ui_perkslot_filename, this, TRUE);
#endif

    set_skill_dlg(kSkillTree, tree);
    set_skill_dlg(kSkillPerk, perk);
    set_skill_dlg(kSkillAction, action);

    SetCurrentDialogKey(IM_SKILL_MANAGER::SKILL_TREE_DLG);


#ifdef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2
#else
    uiSkillTreeEffect& skill_tree_effect = tree->GetSkillTreeEffect();
    skill_tree_effect.SetAnimationChangedCommand( new FunctionCommand( &OnAnimationChanged ) );
#endif //_NA_20111213_SKILLTREE_VISUAL_RENEWAL2

}
//------------------------------------------------------------------------------
/**
*/
uiSkillTree* uiSkillMan::skill_tree()
{
    if (get_skill_dlg(kSkillTree) == NULL)
    {
        return NULL;
    }

    return static_cast<uiSkillTree*>(get_skill_dlg(kSkillTree)); 
}
//------------------------------------------------------------------------------
/**
*/
uiSkillAction* uiSkillMan::skill_action()
{
    if (get_skill_dlg(kSkillAction) == NULL)
    {
        return NULL;
    }

    return static_cast<uiSkillAction*>(get_skill_dlg(kSkillAction)); 
}
//------------------------------------------------------------------------------
/**
*/
SCSkillSlot* uiSkillMan::GetSkillSlotByCode(CODETYPE code)
{
    SCSkillSlot* skill_slot = NULL;
    if (skill_tree())
    {
        int skill_class_code = SKILLCODE2SKILLCLASSCODE( code );
        if( skill_class_code != INVALID_SKILL_CLASS_CODE )
        {
            if( SkillData* skill_data = xGetSkillData()->GetSkillDataBySkillClassCode( skill_class_code ) )
            {
                int slot_position = skill_data->GetSlotPosition();

                return skill_tree()->GetSkillSlot( slot_position );
            }
        }
    }


    //SCSkillSlot* skill_slot = NULL;
    //if (skill_tree())
    //{
    //    //skill_tree()->IsEmpty(

    //    skill_slot = skill_tree()->FindSlotBySkillCode(code);
    //}

    //if (skill_slot)
    //{
    //    return skill_slot;
    //}
   
    if (skill_action())
    {
        skill_slot = skill_action()->FindSlotByCode(code);
    }
    
    return skill_slot;
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillMan::RefreshValidSkillFromQuick()
{
	if (g_pHero == NULL)
	{
		return;
	}

	QuickContainer* quick_container = g_pHero->GetQuickContainer();
	if (quick_container == NULL)
	{
		return;
	}

	for (int idx = 0; idx < quick_container->GetMaxSlotNum(); ++idx)
	{
		if (quick_container->IsEmpty(idx))
        {
			continue;
        }

		QuickSlot& quick_slot = static_cast<QuickSlot&>(quick_container->GetSlot(idx));
		if ((quick_slot.GetOrgSlotType() != ST_SKILL) ||
            (quick_slot.GetOrgSlot() == NULL))
        {
			continue;
        }

        int skill_code = quick_slot.GetOrgSlot()->GetCode();

        if( SkillData* skill_data = xGetSkillData()->GetSkillDataSet().GetSkillDataBySkillCode( skill_code ) )
        {
            if( skill_data->IsActiveState() == false )
            {
                // 초기화나 기타 이유에 따라 현재는 사용 될수 없는 스킬이당.
                quick_container->DeleteSlot(idx, NULL);
            }
        }
	}
}
//------------------------------------------------------------------------------
/**  
*/
int uiSkillMan::getRenderState(SLOTCODE skill_code)
{
    if( SkillData* skill_data = xGetSkillData()->GetSkillDataSetForTree().GetSkillDataBySkillCode( skill_code ) )
    {
        return skill_data->GetState();
    }
    else if( SkillData* skill_data = xGetSkillData()->GetSkillDataSet().GetSkillDataBySkillCode( skill_code ) )
    {
        return skill_data->GetState();
    }

    return SLOT_RENDER_STATE_NONE;
}

//------------------------------------------------------------------------------
/**  SkillInventory내용을 채워넣는다
*/
void uiSkillMan::PreloadSkillInvtoryStoreDatas()
{
    uiSkillTree* tree = skill_tree();
    uiSkillAction* action = skill_action();

    if (init_data())
    {
        return;
    }

    if (tree == NULL)
    {
        return;
    }
    if (action == NULL)
    {
        return;
    }
    
    tree->Clear();
    action->Clear();

	if (g_pHero == NULL)
	{
		return;
	}

    SkillInventoryStoreParser* parser = SkillInventoryStoreParser::Instance();

    int max_data = parser->GetSkillInventoryStoreNum();

    for (int idx = 0; idx < max_data; ++idx)
    {
        const SkillInven_s* inven_info = parser->GetSkillInventoryStore(idx);

		if (inven_info == NULL)
		{
			continue;
		}
          
        if( inven_info->bSkillTab1 == kSkillTree && 
            inven_info->bClassID == g_pHero->GetClass() )
        {
            tree->LoadSkillTreeSlot(inven_info);
        }
        else if( inven_info->bSkillTab1 == kSkillAction )
        {
            action->LoadSkillActionSlot(inven_info);
        }
    }

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
#else
    // 화살표정보 생성을 위해 한번더 사용
    for (int idx = 0; idx < max_data; ++idx)
    {
        const SkillInven_s* inven_info = parser->GetSkillInventoryStore(idx);

		if (inven_info == NULL)
		{
			continue;
		}

        if( inven_info->bSkillTab1 == kSkillTree && 
            inven_info->bClassID == g_pHero->GetClass() )
        {
            tree->LoadSkillTreeSlotArrow(inven_info);
        }
    }
#endif
}

//------------------------------------------------------------------------------
/** 서버로부터 모든 스킬슬롯정보를 갱신
*/
void uiSkillMan::SetSkillInfos(const SKILL_TOTAL_INFO& skillInfos)
{
    if (init_data())
    {
        return;
    }
    set_init_data(true);

    if (skill_tree())
    {
        skill_tree()->SetSkillTotalInfo(skillInfos);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiSkillMan::OpenDialog(int type, BOOL is_open)
{
    static const int dlg_key[kSkillMax] = {0,
                                       IM_SKILL_MANAGER::SKILL_TREE_DLG,
                                       IM_SKILL_MANAGER::SKILL_PERK_DLG,
                                       IM_SKILL_MANAGER::SKILL_ACTION_DLG};

    if (get_skill_dlg(type) == NULL)
    {
        return;
    }
    
    g_KeyQueueManager.DeleteMsg(eDoType_dialog_Skill);

    if (is_open)
    {
        // 키큐등록
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_dialog_Skill;
        msg.wParam = dlg_key[type];
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

        // 탭다이얼로그 등록
        SetCurrentDialogKey(dlg_key[type]);

        if (ui_position().is_set)
        {
            get_skill_dlg(type)->GetDialogWZ()->MoveWindowWZ(ui_position().x, ui_position().y);
        }
        else
        {
            get_skill_dlg(type)->MoveWindowWZ(-1.0f, 120.0f); 
        }

        ui_position().is_set = true;
    }
    else
    {
        if (dlg_key[type] != GetCurrentDialogKey())
        {
            return;
        }

        if (ui_position().is_set)
        {
            RECT rect;
            get_skill_dlg(type)->GetDialogWZ()->GetClientRect(&rect);
            ui_position().x = (float)rect.left;
            ui_position().y = (float)rect.top;
        }    
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillMan::OpenTab(int open_tab)
{
    uiBase* ui_base = NULL;
    for (int idx = kSkillTree; idx < kSkillMax; ++idx)
    {
        ui_base = get_skill_dlg(idx);

        if (ui_base)
        {
            ui_base->ShowInterface(FALSE);
        }
    }
    
    ui_base = get_skill_dlg(open_tab);
    if (ui_base)
    {
        ui_base->ShowInterface(TRUE);
    }
}

void uiSkillMan::OnAnimationChanged()
{
#ifdef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2

#else

    uiSkillMan* ui_skill_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);
    OpenLevelEffect::AnimationState state = ui_skill_man->skill_tree()->GetSkillTreeEffect().GetOpenLevelEffectState();
    int move_count = ui_skill_man->skill_tree()->GetSkillTreeEffect().GetGaugeSlideHeight();

    static bool ani = false;

    switch( state )
    {
    case OpenLevelEffect::kAniStart:
        ani = true;
        break;

    case OpenLevelEffect::kAniNone:
        if( ani == true )
        {
            xGetSkillData()->RefreshActiveSkill( xGetSkillData()->GetSkillDataSet() );
            xGetSkillData()->ResetTreeData();
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
#else
            ui_skill_man->skill_tree()->UpdateArrowState();
#endif
            ui_skill_man->skill_tree()->RefreshSkillSlot();
            ui_skill_man->skill_tree()->RefreshTree(true);

            ani = false;
        }

        break;
    }


#if (WZENGINEVER >= 310) 
    

    switch( state )
    {
    case OpenLevelEffect::kAniStart:
        local_skill_sound.InitAnimationSound( move_count -1);
        local_skill_sound.PlayAnimationSound( SkillTreeSound::TYPE_BARDOWN1);
        break;

    case OpenLevelEffect::kAniNone:
        local_skill_sound.StopAnimationSound();
        break;

    case OpenLevelEffect::kBlinkChain:
        local_skill_sound.PlayAnimationSound( SkillTreeSound::TYPE_KEYOPEN );
        break;

    case OpenLevelEffect::kTornadoAppear:
        local_skill_sound.PlayAnimationSound( SkillTreeSound::TYPE_BLUEEFFECT );
        break;

    case OpenLevelEffect::kChainMove:
        local_skill_sound.PlayAnimationSound( SkillTreeSound::TYPE_KEYDOWN );
        break;

    case OpenLevelEffect::kBlockBlink:
        local_skill_sound.PlayAnimationSound( SkillTreeSound::TYPE_BLINK_BLOCK );
        break;

    case OpenLevelEffect::kBlinkChainFast:
        local_skill_sound.PlayAnimationSound( SkillTreeSound::TYPE_KEYCLOSE );
        break;
    }
#endif // (WZENGINEVER >= 310)
#endif // _NA_20111213_SKILLTREE_VISUAL_RENEWAL2

}

//------------------------------------------------------------------------------
/**
*/  
void uiSkillMan::NetworkProc(MSG_BASE* msg)
{
    if (msg->m_byCategory != CG_SKILL)
    {
        return;
    }

    switch (msg->m_byProtocol)
    {
    case CG_SKILL_SELECT_SKILLPOINT_ACK:
        {
            MSG_CG_SKILL_SELECT_SKILLPOINT_ACK* recv_msg = (MSG_CG_SKILL_SELECT_SKILLPOINT_ACK*)msg;
            if(g_pHero)
            {
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
                int old_left_total_level = (int)xGetSkillData()->GetSkillDataSet().GetTotalUsedPoint(true) / 20;
                int old_right_total_level = (int)xGetSkillData()->GetSkillDataSet().GetTotalUsedPoint(false) / 20;
#endif

                // 1. 트리 리셋
                xGetSkillData()->ResetTreeData();

                // 2. 리셋된 데이터로 UI를 갱신 (화살표, 잠김을 위해)
                if( uiSkillTree* tree = skill_tree() )
                {
                    //tree->RefreshArrowImage();     // 화살표, 잠김 리셋
                    tree->RefreshTree(true);
                }

                // 3. 스킬 포인트 설정
                xGetSkillData()->SetRemainSkillPoint( recv_msg->remain_skill_point );

                // 4. 스킬 추가
                for( int i=0;i<recv_msg->skill_total_info.m_Count; ++i)
                {
                    SkillSlot slot( recv_msg->skill_total_info.m_Slot[i].m_Stream );
                    xGetSkillData()->AddNewSkillCode( slot.GetCode() );
                }


#ifdef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2
                // 5. 리셋, 사용가능한 스킬 활성화
                xGetSkillData()->RefreshActiveSkill( xGetSkillData()->GetSkillDataSet() );
                xGetSkillData()->ResetTreeData();

                // 6. 트리 갱신
                if( uiSkillTree* tree = skill_tree() )
                {
                    tree->RefreshSkillSlot();
                    tree->RefreshTree(false);   // 이 과정에서 애니메이션 시작
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL

                    int left_total_level = (int)xGetSkillData()->GetSkillDataSet().GetTotalUsedPoint(true) / 20;
                    int right_total_level = (int)xGetSkillData()->GetSkillDataSet().GetTotalUsedPoint(false) / 20;

                    tree->StartAnimation( old_left_total_level, left_total_level, old_right_total_level, right_total_level );
#else
                    tree->UpdateArrowState();
#endif
                }
#else 
                // 5. 리셋, 사용가능한 스킬 활성화
                xGetSkillData()->ResetTreeData();
                xGetSkillData()->RefreshActiveSkill( xGetSkillData()->GetSkillDataSet() );

                // 6. 트리 갱신
                if( uiSkillTree* tree = skill_tree() )
                {
                    tree->RefreshSkillSlot();
                    tree->RefreshTree(false);   // 이 과정에서 애니메이션 시작
                }
#endif

            }
        }
        break;

    case CG_SKILL_SELECT_SKILLPOINT_NAK:
        {
            MSG_CG_SKILL_SELECT_SKILLPOINT_NAK* recv_msg = (MSG_CG_SKILL_SELECT_SKILLPOINT_NAK*)msg;

            WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_CONSOLE | c_wOUTPUT_LOG_FILE, 
                     _T("CG_SKILL_SELECT_SKILLPOINT_NAK[%d]"), recv_msg->error_code);

            // 1. 트리 리셋
            xGetSkillData()->ResetTreeData();

            // 2. 리셋된 데이터로 UI를 갱신 (화살표, 잠김을 위해)
            if( uiSkillTree* tree = skill_tree() )
            {
                //tree->RefreshArrowImage();     // 화살표, 잠김 리셋
                tree->RefreshSkillSlot();
                tree->RefreshTree(true);
            }

            GlobalFunc::OutputMsg_SysMsg( "recived CG_SKILL_SELECT_SKILL_POINT_NAK [CODE:%d]", recv_msg->error_code );

        }
        break;

    case CG_SKILL_REFRESH_CMD:
        {
            MSG_CG_SKILL_REFRESH_CMD* recv_msg = (MSG_CG_SKILL_REFRESH_CMD *)msg;

            //1. 스킬 정보를 전부 재셋팅한다
            ResetOnceInitSkillInfos();

            PreloadSkillInvtoryStoreDatas();				

            //2. 스킬 정보를 새팅
            SetSkillInfos(recv_msg->m_SkillInfo);

            xGetSkillData()->ResetTreeData();

            if( uiSkillTree* tree = skill_tree() )
            {
                //tree->RefreshArrowImage();
                tree->RefreshSkillSlot();
            }

            //3. 리셋팅후 퀵슬롯에 등록되지않은 스킬들은없애버린다
            RefreshValidSkillFromQuick();

            if (g_pHero)
            {
               g_pHero->UpdateSkillAttr();
            }

            if( uiSkillTree* tree = skill_tree() )
            {
                if( tree->IsVisibleWindow() == TRUE )
                {
                    tree->RefreshUI();
                }
            }
         }
        break;

    case CG_SKILL_POINT_REMAIN_CMD:
        {
            MSG_CG_SKILL_POINT_REMAIN_CMD * recv_msg = (MSG_CG_SKILL_POINT_REMAIN_CMD *)msg;
            
            if (g_pHero) 
            {
                BASE_PLAYERINFO * pPlayerInfo = g_pHero->GetCharInfo();
                g_pHero->GetCharInfo()->m_iRemainSkill = recv_msg->m_iRemainSkill;

                xGetSkillData()->ResetTreeData();

                if( uiSkillTree* tree = skill_tree() )
                {
                    if( tree->IsVisibleWindow() == TRUE )
                    {
                        tree->RefreshUI();
                    }
                }


            }
        }
        break;
    }
}

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
void uiSkillMan::SEND_Perk_List_SYN()
{
    if (skill_dlg_[kSkillPerk])
    {        
        MSG_CG_ETC_GAMEOPTION_LOAD_SYN send_packet;
        static_cast<uiPerkDialog*>(skill_dlg_[kSkillPerk])->SEND_CG_ETC_GAMEOPTION_LOAD_SYN_PERK_LIST(&send_packet);        
    }
}

void uiSkillMan::RECV_Perk_List_ACK( GameOption& game_option )
{
    if (game_option.option_type_ == GameOption::kPerkListChar)
    {
        if (skill_dlg_[kSkillPerk])
        {
            static_cast<uiPerkDialog*>(skill_dlg_[kSkillPerk])->RecvPerkGroupList(game_option);
        }
    }
}
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM