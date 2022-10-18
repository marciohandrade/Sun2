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
/** ���� ����
*/
RC::eSLOT_INSERT_RESULT uiSkillTree::InsertSlot(POSTYPE at_pos, SCSlot& IN slot)
{
    if (ValidPos(at_pos) == false)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

    // ���Կ� ����
    return SCSkillSlotContainer::InsertSlot(at_pos, slot);
}
//------------------------------------------------------------------------------
/** ���� ����
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
/**  SkillInventory.txt ���������� ä���
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


		//���� ����ä��
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

        // ��ų ��� �������� �䱸 ��ų�� �ִ� ��� ��ų�� lock��ġ�� ���
        if( skill_data.GetRequiredSkillClassCode() > INVALID_SKILL_CLASS_CODE )
        {
            skill_tree_effect_.AddLockInfo( real_pos, skill_data.GetSkillClassCode());
        }
	}
}

//------------------------------------------------------------------------------
/**  SkillInventory.txt ���������� ä���
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

        // ��ġ
        int real_pos = (data->bSkillTab2 - 1)*kMaxColumn + idx;

        if( skill_Code == -1 || skill_Code == -2 || skill_Code == -5 || skill_Code == -6 )
        {
            int arrow_index = std::abs(skill_Code) -1;

            // �Ʒ��� ��ų�� ����Ű�� ȭ��ǥ
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

            // 2ĭ �Ʒ��� ��ų�� ����Ű�� ȭ��ǥ
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
/** ���� ���ӽ� ĳ���������� ��ų ���Խ�Ʈ���� ���� ��ų ������ �ʱ�ȭ
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
            // ��ų Ȱ��ȭ
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

    // �̹� ��� ��ų�鿡 ���� ó��
    SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];
        if( skill_data.IsActiveState() == true )
        {
            // �̹� ������ ��ų�̹Ƿ� ���ǥ�� ����
            skill_tree_effect_.HideLock( skill_data.GetSkillClassCode() );

            // �� ��ų�� ���������� Ȱ��ȭ �Ǵ� ȭ��ǥ�� ������ ǥ���ϵ��� ����
            skill_tree_effect_.SetVisibleArrow( skill_data.GetSkillClassCode(), skill_data.GetSkillLevel() );
        }
    }
}

void uiSkillTree::RefreshSkillSlot()
{
    // ��ų������ ����Ȱ��� �ִٸ� ���⿡�� ���ŵ�
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

        // �� ��ų�� ���������� Ȱ��ȭ �Ǵ� ȭ��ǥ�� ������ ǥ���ϵ��� ����
        skill_tree_effect_.SetAnimationArrow( skill_data.GetSkillClassCode() , skill_data.GetSkillLevel() );
    }
}

void uiSkillTree::UpdateLockState()
{
    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();

    // ������Ʈ �Ǵ� �������� ��Ƽ��� ��ų�� lock�� ������� ó���� �� ���ֵ��� ó��
    // (�̹� ����� �ְų� �ִϸ��̼� ���̶�� ���õ�)
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
/** ������ ����(����Ʈ �ִϸ� ���� �����صд�)
*/
void uiSkillTree::RefreshTree(bool data_clear)
{
    // ��ų ������ 0���� ū ��츸 ��� �ջ�
    int total_level = (int)xGetSkillData()->GetSkillDataSetForTree().GetTotalUsedPoint();

    // 
    skill_tree_effect_.SetTotalSkillLevel( total_level, data_clear );


    // ����, ��� ��ư ǥ�ÿ���
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


    // ��� ������ +��ư ���߱�
    for (int i = kPlusBegin; i <= kPlusEnd; ++i)
    {
        if( CControlWZ* control = getControl(i) )
        {
            control->ShowWindowWZ(WZ_HIDE);
        }
    }

    // ������ ���¸� ������ ��
    xGetSkillData()->RefreshLevelupCondition();

    // ������ �����Ѱ͸� +��ư ǥ��
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

    // ��� �̹���ó��
    UpdateLockState();
}

//------------------------------------------------------------------------------
/** �� ������ ��ų ������ �������ΰ� (�ƴϸ� �������� ��ų�����ΰ� -_-)
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
