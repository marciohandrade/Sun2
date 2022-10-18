//  (C) 2010 Webzen Sun Studio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifndef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
#include "ItemUnitRenderer.h"
#include "uiSkillTree.h"
#include "SkillInventoryStoreParser.h"
#include "SkillSlot.h"
#include "SlotKeyGenerator.h"
#include "SkillInfoParser.h"
#include "QuickContainer.h"
#include "ItemManager.h"
#include "Hero.h"
#include "SkillMain.h"



//------------------------------------------------------------------------------
/**
*/
uiSkillTree::uiSkillTree(InterfaceManager* ui_man) :
uiBase(ui_man),
invalid_control_(false),
item_renderer_(NULL)
{
    SkillSlotContainer::Init(kMaxSlot);

    this->InitItemRender();

    is_mouse_overed_on_button_ = false;
    mouse_overed_control_ = 0;

}
//------------------------------------------------------------------------------
/**
*/
uiSkillTree::~uiSkillTree()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
SCSlotContainer* uiSkillTree::GetSlotContainer()
{
    return static_cast<SCSlotContainer *>(this);
}

//------------------------------------------------------------------------------
/** 슬롯 삽입
*/
RC::eSLOT_INSERT_RESULT uiSkillTree::InsertSlot(POSTYPE at_pos, SCSlot& IN slot)
{
    if (ValidPos(at_pos) == false)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

    // 슬롯에 삽입
    return SCSkillSlotContainer::InsertSlot(at_pos, slot);
}
//------------------------------------------------------------------------------
/** 슬롯 삭제
*/
BOOL uiSkillTree::DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete)
{
    if (IsEmpty(at_pos))
    {
        return FALSE;
    }

    if (GetItemUnitRender())
    {
        GetItemUnitRender()->RemoveItemUnit(GetSlot(at_pos).GetSerial());
    }

    return SCSkillSlotContainer::DeleteSlot(at_pos, slot_clone_before_delete); 
}
//------------------------------------------------------------------------------
/**  SkillInventory.txt 정보내용을 채운다
*/
void uiSkillTree::LoadSkillTreeSlot(const SkillInven_s* data)
{
    for (int idx = 0; idx < kMaxColumn; ++idx)
	{
		int skill_Code = data->iSkillID[idx];
		if (skill_Code <= 0)
		{
			continue;
		}


		//슬롯 내용채움
		SkillSlot skill_slot;
		skill_slot.SetCode(skill_Code);
		skill_slot.SetSerial(g_SlotKeyGenerator.GetKey());
        int real_pos = (data->bSkillTab2 - 1)*kMaxColumn + idx;
        InsertSlot(real_pos, skill_slot);



		SkillData skill_data;
		skill_data.SetSlotPosition( real_pos, data->bSkillTab2 - 1 );
		skill_data.SetCurSkill( skill_Code );

        xGetSkillData()->AddSkillData( skill_data );

		//curSkillInfo->SetFirst();
		//BASE_ABILITYINFO* ablity_info = NULL;
		//while (ablity_info = curSkillInfo->GetNext())
		//{
		//	info.currStateCodes.push_back(ablity_info->m_wAbilityID);
		//}

        // 스킬 등록 과정에서 요구 스킬이 있는 모든 스킬의 lock위치를 등록
        if( skill_data.GetRequiredSkillClassCode() > INVALID_SKILL_CLASS_CODE )
        {
            skill_tree_effect_.AddLockInfo( real_pos, skill_data.GetSkillClassCode());
        }
	}
}

//------------------------------------------------------------------------------
/**  SkillInventory.txt 정보내용을 채운다
*/
void uiSkillTree::LoadSkillTreeSlotArrow(const SkillInven_s* data)
{
	for (int idx = 0; idx < kMaxColumn; ++idx)
	{
		int skill_Code = data->iSkillID[idx];
		//if (skill_Code >= 0)
		//{
		//	continue;
		//}

        // 위치
        int real_pos = (data->bSkillTab2 - 1)*kMaxColumn + idx;

        if( skill_Code == -1 || skill_Code == -2 || skill_Code == -5 || skill_Code == -6 )
        {
            int arrow_index = std::abs(skill_Code) -1;

            // 아래쪽 스킬을 가리키는 화살표
            if( SkillData* skill_data = xGetSkillData()->GetSkillDataBySlotPosition( real_pos + kMaxColumn ) )
            {
                if( skill_data->GetRequiredSkillClassCode() > INVALID_SKILL_CLASS_CODE && skill_data->GetRequiredSkillLevel() > INVALID_SKILL_LEVEL )
                {
                    skill_tree_effect_.AddArrowInfo( real_pos, arrow_index, skill_data->GetRequiredSkillClassCode(), skill_data->GetRequiredSkillLevel() );
                }
            }
        }
        else if( skill_Code == -3 || skill_Code == -4 )
        {
            int arrow_index = std::abs(skill_Code) -1;

            // 2칸 아래쪽 스킬을 가리키는 화살표
            if( SkillData* skill_data = xGetSkillData()->GetSkillDataBySlotPosition( real_pos + kMaxColumn + kMaxColumn ) )
            {
                if( skill_data->GetRequiredSkillClassCode() > INVALID_SKILL_CLASS_CODE && skill_data->GetRequiredSkillLevel() > INVALID_SKILL_LEVEL )
                {
                    skill_tree_effect_.AddArrowInfo( real_pos, arrow_index, skill_data->GetRequiredSkillClassCode(), skill_data->GetRequiredSkillLevel() );
                }
            }
        }
	}
}

//------------------------------------------------------------------------------
/** 게임 접속시 캐릭터정보의 스킬 슬롯스트림을 통해 스킬 정보를 초기화
*/
void uiSkillTree::SetSkillTotalInfo(const SKILL_TOTAL_INFO& skill_total_info)
{
    const SKILLSLOT* total_slot = skill_total_info.m_Slot;

    SkillInfoParser* parser = SkillInfoParser::Instance();
    for (POSTYPE idx = 0; idx < skill_total_info.m_Count; ++idx)
    {
        SkillSlot post_slot(total_slot[idx].m_Stream);
        if( SkillScriptInfo* skill_info = parser->GetSkillInfo(post_slot.GetCode()) )
        {
            // 스킬 활성화
            xGetSkillData()->AddNewSkillCode( post_slot.GetCode() );
            //SetSkillActive(post_slot.GetCode());
        }
    }

    xGetSkillData()->RefreshActiveSkill( xGetSkillData()->GetSkillDataSet() );

    RefreshArrowImage();
    RefreshSkillSlot();
}

SkillSlot* uiSkillTree::GetSkillSlot( int skill_slot_position )
{
    if( IsEmpty( skill_slot_position ) == FALSE )
    {
        return static_cast<SkillSlot*>(&GetSlot(skill_slot_position));
    }
    return NULL;
}

void uiSkillTree::RefreshArrowImage()
{
    skill_tree_effect_.ResetLock();
    skill_tree_effect_.ResetArrow();

    // 이미 배운 스킬들에 대한 처리
    SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];
        if( skill_data.IsActiveState() == true )
        {
            // 이미 습득한 스킬이므로 잠김표시 숨김
            skill_tree_effect_.HideLock( skill_data.GetSkillClassCode() );

            // 이 스킬을 습득했을때 활성화 되는 화살표가 있으면 표시하도록 설정
            skill_tree_effect_.SetVisibleArrow( skill_data.GetSkillClassCode(), skill_data.GetSkillLevel() );
        }
    }
}

void uiSkillTree::RefreshSkillSlot()
{
    // 스킬정보가 변경된것이 있다면 여기에서 갱신됨
    SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];

        int slot_position = skill_data.GetSlotPosition();
        if( IsEmpty( slot_position ) == FALSE )
        {
            SCSkillSlot& skill_slot = static_cast<SCSkillSlot&>(GetSlot( skill_data.GetSlotPosition() ) );
            skill_slot.SetSlotType( ST_SKILL );
            skill_slot.SetCode( skill_data.GetSkillCode() );
        }
    }
}

void uiSkillTree::UpdateArrowState()
{
    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();

    for( size_t i=0;i!=skill_set.size();++i)
    {
        const SkillData& skill_data = skill_set[i];

        // 이 스킬을 습득했을때 활성화 되는 화살표가 있으면 표시하도록 설정
        skill_tree_effect_.SetAnimationArrow( skill_data.GetSkillClassCode() , skill_data.GetSkillLevel() );
    }
}

void uiSkillTree::UpdateLockState()
{
    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();

    // 업데이트 되는 시점에서 엑티브된 스킬로 lock을 사라지는 처리를 할 수있도록 처리
    // (이미 사라져 있거나 애니메이션 중이라면 무시됨)
    for( size_t i=0;i!=skill_set.size();++i)
    {
        const SkillData& skill_data = skill_set[i];
        if( skill_data.IsActiveState() == true )
        {
            skill_tree_effect_.BegionFadeout( skill_data.GetSkillClassCode() );
        }
    }
}

//------------------------------------------------------------------------------
/** 탭정보 갱신(이펙트 애니를 위해 저장해둔다)
*/
void uiSkillTree::RefreshTree(bool data_clear)
{
    // 스킬 레벨이 0보다 큰 경우만 모두 합산
    int total_level = (int)xGetSkillData()->GetSkillDataSetForTree().GetTotalUsedPoint();

    // 
    skill_tree_effect_.SetTotalSkillLevel( total_level, data_clear );


    // 적용, 취소 버튼 표시여부
    if( xGetSkillData()->IsSkillSimulated() )
    {

        GetControlWZ( wzId_[kButtonBlendImage] )->ShowWindowWZ(WZ_SHOW);
        GetControlWZ( wzId_[kButton1] )->EnableWindowWZ(WZS_ENABLE);
        GetControlWZ( wzId_[kButton2] )->EnableWindowWZ(WZS_ENABLE);
    }
    else
    {
        GetControlWZ( wzId_[kButtonBlendImage] )->ShowWindowWZ(WZ_HIDE);
        GetControlWZ( wzId_[kButton1] )->EnableWindowWZ(WZS_DISABLE);
        GetControlWZ( wzId_[kButton2] )->EnableWindowWZ(WZS_DISABLE);
    }


    // 모든 슬롯의 +버튼 감추기
    for (int i = kPlusBegin; i <= kPlusEnd; ++i)
    {
        if( CControlWZ* control = getControl(i) )
        {
            control->ShowWindowWZ(WZ_HIDE);
        }
    }

    // 레벨업 상태를 갱신한 후
    xGetSkillData()->RefreshLevelupCondition();

    // 레벨업 가능한것만 +버튼 표시
    SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for (size_t i = 0; i < skill_set.size(); ++i)
    {
        if (skill_set[i].IsEnableLevelup())
        {
            if( CControlWZ* control = getControl(skill_set[i].GetSlotPosition() + kPlusBegin) )
            {
                control->ShowWindowWZ(WZ_SHOW);
            }
        }
    }

    // 잠김 이미지처리
    UpdateLockState();
}

//------------------------------------------------------------------------------
/** 이 슬롯이 스킬 슬롯의 아이템인가 (아니면 퀵슬롯의 스킬슬롯인가 -_-)
*/
BOOL uiSkillTree::IsSkillSlot(SCSkillSlot* slot)
{
    SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];

        SCSkillSlot& skill_slot = static_cast<SCSkillSlot&>(GetSlot( skill_data.GetSlotPosition() ) );
        if( &skill_slot == slot )
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

#endif//'ndef' _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
