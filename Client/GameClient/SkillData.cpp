#include "SunClientPrecompiledHeader.h"
#include "SkillData.h"
#include "ItemUnitRenderer.h"
#include "SkillInfoParser.h"


SkillData::SkillData()
{
    m_BlockState = SLOT_RENDER_STATE_NONE;
    m_State = SLOT_RENDER_STATE_DEACTIVATED;

    if (GENERALPARAM->IsNet() == FALSE)
    {
        m_State = SLOT_RENDER_STATE_ACTIVATED;
    }
}

void SkillData::SetSlotPosition( int pos, int tab )
{
    uicSkillInfo::m_SlotIndex = pos;
    uicSkillInfo::m_TabIndex = tab; 
}

bool SkillData::LevelUp()
{

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    if( GetSkillLevel() == 1 && IsActive() == false)
    {
        SetActive(true);
        return true;
    }
#endif

    if( GetCurSkillInfo() == NULL || GetNextSkillInfo() == NULL )
    {
        return false;
    }

    SetCurSkill( GetNextSkillInfo()->m_SkillCode );
    return true;
}

void SkillData::SetCurSkill( int skill_Code )
{
    if( skill_Code == INVALID_SKILL_CODE )
        return;

    SkillInfoParser* parser = SkillInfoParser::Instance();

    if( SkillScriptInfo* cur_skill = parser->GetSkillInfo(skill_Code) )
    {
        m_pCurrentSkillInfo = cur_skill;

        m_pNextSkillInfo = NULL;

        if( SkillScriptInfo* next_skill = SkillInfoParser::Instance()->GetSkillInfo( skill_Code + 1) )
        {
            if( next_skill->m_SkillClassCode == cur_skill->m_SkillClassCode )
            {
                m_pNextSkillInfo = next_skill;
            }
        }
    }
}

int SkillData::GetRequiredSkillClassCode() const
{
    if( m_pCurrentSkillInfo != NULL )
        return m_pCurrentSkillInfo->m_UpperSkillClass;

    return INVALID_SKILL_CLASS_CODE;
}

int SkillData::GetRequiredSkillLevel() const
{
    if( m_pCurrentSkillInfo != NULL )
        return m_pCurrentSkillInfo->m_RequireUpperSkillLevel;
    return INVALID_SKILL_LEVEL;
}

int SkillData::GetSkillCode() const
{
    if( m_pCurrentSkillInfo != NULL )
        return m_pCurrentSkillInfo->m_SkillCode;

    return INVALID_SKILL_CODE;
}


int SkillData::GetSkillClassCode() const
{
    if( m_pCurrentSkillInfo != NULL )
        return m_pCurrentSkillInfo->m_SkillClassCode;

    return INVALID_SKILL_CLASS_CODE;
}

DWORD SkillData::GetSkillNameCode() const
{
    if( m_pCurrentSkillInfo != NULL )
        return m_pCurrentSkillInfo->m_dwSkillNcode;
    return 0;	// INALID_SKILL_NAME_CODE
}

int SkillData::GetSkillLevel() const
{
    if( m_pCurrentSkillInfo != NULL )
        return m_pCurrentSkillInfo->m_wSkillLv;

    return INVALID_SKILL_LEVEL;
}


#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL

bool SkillData::IsLeftSide() const
{
    return (uicSkillInfo::m_SlotIndex % 6) < 3; // x ��ġ�� 0,1,2��
}

void SkillData::SetDeactiveState()
{
    m_State = SLOT_RENDER_STATE_DEACTIVATED;
}
#endif

void SkillData::SetActiveState()
{
    m_State = SLOT_RENDER_STATE_ACTIVATED;
}

bool SkillData::IsActiveState() const
{
    return m_pCurrentSkillInfo != NULL && m_State == SLOT_RENDER_STATE_ACTIVATED;
}

bool SkillData::IsBlockedState() const 
{
    return m_State == SLOT_RENDER_STATE_ACTIVATED && m_BlockState == SLOT_RENDER_STATE_BLOCK;
}

void SkillData::SetBlockState( bool bBlock )
{
    m_BlockState = bBlock ? SLOT_RENDER_STATE_BLOCK : SLOT_RENDER_STATE_NONE;
}

bool SkillData::IsUseableClass(DWORD character_class)
{
    if( m_pCurrentSkillInfo != NULL )
    {
        DWORD skill_class = m_pCurrentSkillInfo->m_dwClassDefine;

        if (skill_class >= 11 && skill_class < 20)
        {
            // ü������ �� �� 10�� ���� �⺻Ŭ������...
            skill_class -= 10;
        }

        // 9���� �峪, �峪(������) �Ѵ� ��밡���� ���̽��� Ư���� ����ó���Ѵ�
        if( skill_class == eDRAGON_COMMON_SKILL_1stTRANS )
        {
            if( character_class == eCHAR_DRAGON || character_class == eTRANSFORM_TYPE_DRAGON1 )
                return true;
        }

        return character_class == skill_class;
    }

    return false;
}

int SkillData::GetClassDefine() const
{
    if( m_pCurrentSkillInfo != NULL )
    {
        return m_pCurrentSkillInfo->m_dwClassDefine > 10 ? m_pCurrentSkillInfo->m_dwClassDefine - 10 : m_pCurrentSkillInfo->m_dwClassDefine;
    }

    return INVALID_CLASS_CODE;
}

WORD SkillData::GetSkillRange() const
{
    if( m_pCurrentSkillInfo != NULL )
    {
        return m_pCurrentSkillInfo->m_wSkillRange;
    }
    return 0;
}

bool SkillData::IsPassiveSkill() const
{
    if( m_pCurrentSkillInfo != NULL )
    {
        return m_pCurrentSkillInfo->m_bySkillType == SKILL_TYPE_PASSIVE;
    }
    return false;
}


#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL

size_t SkillDataSet::GetTotalUsedPoint(bool left) const
{
    int line_index = 0;
    size_t total_cost = 0;
    for (int i=0;i!=size();++i)
    {
        const SkillData& skill_data = Get(i);
        if( left == skill_data.IsLeftSide() )
        {
            int cost = 0;
            if( skill_data.GetSkillLevel() == 1 && skill_data.IsActive() == true )
            {
                cost = skill_data.GetSkillLevel();
            }
            else if( skill_data.GetSkillLevel() > 1 )
            {
                cost = skill_data.GetSkillLevel();
            }

            if( cost > 0 )
            {
                // �� ���� �����̸� �� ������ �������� �ٽ� ī�����Ѵ�
                if( skill_data.GetLineIndex() > line_index )
                {
                    total_cost = cost;
                    line_index = skill_data.GetLineIndex();
                }
                else if( skill_data.GetLineIndex() == line_index )
                {
                    total_cost += cost;
                }
            }
        }
    }

    // �� ������ �ִ� 20����Ʈ
    if( total_cost > 20 )
        total_cost = 20;

    return line_index * 20 + total_cost;
}

#else

size_t SkillDataSet::GetTotalUsedPoint() const
{
    size_t total_cost = 0;
    for (int i=0;i!=size();++i)
    {
        const SkillData& skill_data = Get(i);
        if( skill_data.GetSkillLevel() > 0 )
        {
            total_cost += skill_data.GetSkillLevel();
        }
    }
    return total_cost;
}
#endif



//------------------------------------------------------------------------------
/** ������ġ�� ���� ��ũ��Ʈ �������
*/
SkillData* SkillDataSet::GetSkillDataBySlotPosition(int position)
{
    if( position == INVALID_SLOT_POS )
    {
        return NULL;
    }

    for (size_t i = 0; i < size(); ++i)
    {
        if (Get(i).GetSlotPosition() == position)
        {
            return &(Get(i));
        }
    }
    return NULL;
}

SkillData* SkillDataSet::GetSkillDataBySkillClassCode(CODETYPE skill_class_code)
{
    if( skill_class_code == INVALID_SKILL_CLASS_CODE )
    {
        return NULL;
    }

    for (size_t i = 0; i < size(); ++i)
    {
        if (Get(i).GetSkillClassCode() == skill_class_code )
        {
            return &(Get(i));
        }
    }
    return NULL;
}

SkillData* SkillDataSet::GetSkillDataBySkillCode(CODETYPE skill_code)
{
    if( skill_code == INVALID_SKILL_CODE )
    {
        return NULL;
    }

    for (size_t i = 0; i < size(); ++i)
    {
        if (Get(i).GetSkillCode() == skill_code )
        {
            return &(Get(i));
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
/** ��ų������ ���� ������ġã��
*/
int SkillDataSet::FindPosBySkillClassCode(int skill_class_code)
{
    if( skill_class_code == INVALID_SKILL_CLASS_CODE )
    {
        return INVALID_SLOT_POS;
    }

    for (size_t i = 0; i < size(); ++i)
    {
		if( Get(i).GetSkillClassCode() == skill_class_code )
		{
			return i;
		}
    }
    return INVALID_SLOT_POS;
}

bool SkillDataSet::CheckSkillLevel(DWORD skill_class, int skill_level, DWORD* skill_name_code)
{
    SkillData* skill_data = GetSkillDataBySkillClassCode(skill_class);

    // ��ų�� ������ ����
    if( skill_data == NULL )
    {
        return false;
    }

    // �ϴ� �̸��� ����
    if (skill_name_code)
    {
        *skill_name_code = skill_data->GetSkillNameCode();
    }

    // ������ �����ϸ� ����
    if( skill_level > skill_data->GetSkillLevel() )
    {
        return false;
    }

    return true;
}



