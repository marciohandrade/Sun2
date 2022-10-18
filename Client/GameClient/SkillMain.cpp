#include "SunClientPrecompiledHeader.h"
#include "SkillMain.h"

#include "SkillInfoParser.h"
#include "Player.h"
#include "SkillInventoryStoreParser.h"
#include "BattleScene.h"


#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
static const int s_max_row = 7;
static const int s_point_per_line = 20;
#else
static const int s_max_row = 6;
static const int s_point_per_line = 5;
#endif


SkillMain::SkillMain()
: m_pHero(NULL)
{
}

SkillMain::~SkillMain()
{
}

int SkillMain::GetRemainSkillPoint()
{
    if( m_pHero != NULL )
    {
        return m_pHero->GetCharInfo()->m_iRemainSkill;
    }

    return BattleScene::GetHeroInfo()->m_CharInfo.m_iRemainSkill;
}

int SkillMain::GetHeroLevel()
{
    if( m_pHero != NULL )
    {
        return m_pHero->GetCharInfo()->m_LV;
    }

    return BattleScene::GetHeroInfo()->m_CharInfo.m_LV;
}

void SkillMain::SetRemainSkillPoint( int remain_point )
{
    if( m_pHero != NULL )
        m_pHero->GetCharInfo()->m_iRemainSkill = remain_point;
}

bool SkillMain::IsSkillSimulated()
{
    return GetRemainSkillPoint() != GetRemainUseablePoint();
}

//------------------------------------------------------------------------------
/**  해당 클래스 타입이 아닌 스킬을 블럭시킨다. (변신해서 클래스가 바뀔때 사용)
*/
void SkillMain::BlockSkill(DWORD char_class)
{
    for (size_t idx = 0; idx < GetSkillDataCount(); ++idx)
    {
        SkillData& skill_data = GetSkillData(idx);

		if( skill_data.IsActiveState() == true )
		{
            if( skill_data.IsUseableClass( char_class ) != true )
            {
                skill_data.SetBlockState( true );
            }
		}
    }
}
void SkillMain::UnBlockSkill()
{
    for (size_t i = 0; i < GetSkillDataCount(); ++i)
    {
        SkillData& skill_data = GetSkillData(i);
		if( skill_data.IsBlockedState() == true )
		{
			skill_data.SetBlockState( false );
		}
    }
}

int SkillMain::GetRealSkillCode( int skill_code )
{
    int skill_class_code = SKILLCODE2SKILLCLASSCODE( skill_code );
    if( skill_class_code == INVALID_SKILL_CLASS_CODE )
        return INVALID_SKILL_CODE;

    if( SkillData* skill_data = m_SkillDataSet.GetSkillDataBySkillClassCode( skill_class_code ) )
    {
        return skill_data->GetSkillCode();
    }
    return INVALID_SKILL_CODE;
}

//------------------------------------------------------------------------------
/** skill_code로 업데이트 (새로운 스킬을 배울때 해당 코드로 업데이트한다)
    위치를 리턴한다.
*/
int SkillMain::AddNewSkillCode( CODETYPE skill_code )
{
    if( SkillData* skill_data = GetSkillDataBySkillClassCode( SKILLCODE2SKILLCLASSCODE(skill_code) ) )
    {
        skill_data->SetActiveState();
        skill_data->SetCurSkill( skill_code );
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
        skill_data->SetActive(true);
#endif
        return skill_data->GetSlotPosition();
    }

    return INVALID_SLOT_POS;
}

//------------------------------------------------------------------------------
/** 스킬정보를 통한 슬롯위치찾기
*/
int SkillMain::FindPosBySkillCode(SLOTCODE skill_code )
{
	SkillInfoParser* parser = SkillInfoParser::Instance();
	SkillScriptInfo* skill = parser->GetSkillInfo( skill_code );

	if( skill == NULL ) 
		return INVALID_SLOT_POS;

	return FindPosBySkillClassCode( skill->m_SkillClassCode );
}

//------------------------------------------------------------------------------
/** 스킬 검사 (습득 여부를 위한)
*/
bool SkillMain::CheckSkillLevel(DWORD skill_class, int skill_level, DWORD* skill_name_code)
{
    SkillData* skill_data = GetSkillDataBySkillClassCode(skill_class);

    // 스킬이 없으면 실패
    if( skill_data == NULL )
    {
        return false;
    }

    // 일단 이름은 복사
    if (skill_name_code)
    {
        *skill_name_code = skill_data->GetSkillNameCode();
    }

    // 레벨이 부족하면 실패
    if( skill_level > skill_data->GetSkillLevel() )
    {
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/** 총사용 포인트의 합계
*/
void SkillMain::RefreshLevelupCondition()
{
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL

    SkillDataSet& skill_set = m_SkillDataSetForTree;
    int remain_point = m_RemainSkillPointForTree;

    // 
    for( size_t i=0;i!=skill_set.size();++i)
    {
        skill_set[i].SetEnableLevelup(false);
        skill_set[i].SetMaxLevel(false);
    }

    if( remain_point == 0 )
    {
        return;
    }

    // 포인트 계산
    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];  

        int total_used_point_per_side = skill_set.GetTotalUsedPoint(skill_data.IsLeftSide());
        int enable_line = total_used_point_per_side / s_point_per_line;
        if( enable_line >= s_max_row )
            enable_line = s_max_row -1;



        //skill_data.SetEnableLevelup( skill_data.GetTabIndex() > 0 ? false : true );

        //// 탭으로 일단 검사
        //if ( skill_data.GetTabIndex() > skill_tree_effect_.GetCurTab() || skill_data.GetNextSkillInfo() == NULL )
        //{
        //    skill_data.SetEnableLevelup( false );
        //    continue;
        //}

        if ( skill_data.GetCurSkillInfo() == NULL )
        {
            continue;
        }

        // 아직 활성화 되지 않은 라인은 패스
        if( skill_data.GetLineIndex() > enable_line )
        {
            continue;
        }


        //if( skill_data.GetSkillLevel() == 1 )
        //{
        //    // 처음 습득하는경우
        //    if( skill_data.GetCurSkillInfo()->m_wRequireLv > GetHeroLevel() )
        //    {
        //        continue;
        //    }

        //}
        //else if( skill_data.GetSkillLevel() >= 1 )
        //{
        //    // 요구레벨 검사
        //    if ( skill_data.GetNextSkillInfo()->m_wRequireLv > GetHeroLevel() )
        //    {
        //        continue;
        //    }
        //}

        // 최대레벨 검사
        if ( skill_data.GetCurSkillInfo()->m_wSkillLv >= skill_data.GetCurSkillInfo()->m_wMaxLv )
        {
            skill_data.SetMaxLevel( true );
            continue;
        }

        //// 요구스킬 검사
        //if( skill_data.GetRequiredSkillClassCode() != INVALID_SKILL_CLASS_CODE && 
        //    skill_data.GetRequiredSkillLevel() > 0 )
        //{
        //    if( skill_set.CheckSkillLevel( skill_data.GetRequiredSkillClassCode(), skill_data.GetRequiredSkillLevel(), NULL ) == false )
        //    {
        //        continue;
        //    }
        //}

        skill_data.SetEnableLevelup( true );
    }
#else
    SkillDataSet& skill_set = m_SkillDataSetForTree;
    int remain_point = m_RemainSkillPointForTree;

    static const int max_row = 6;
    static const int point_per_line = 5;

    int total_used_point = skill_set.GetTotalUsedPoint();
    int enable_line = total_used_point / point_per_line;
    if( enable_line >= max_row )
        enable_line = max_row -1;

    // 
    for( size_t i=0;i!=skill_set.size();++i)
    {
        skill_set[i].SetEnableLevelup(false);
    }

    if( remain_point == 0 )
    {
        return;
    }

    // 포인트 계산
    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];  

        //skill_data.SetEnableLevelup( skill_data.GetTabIndex() > 0 ? false : true );

        //// 탭으로 일단 검사
        //if ( skill_data.GetTabIndex() > skill_tree_effect_.GetCurTab() || skill_data.GetNextSkillInfo() == NULL )
        //{
        //    skill_data.SetEnableLevelup( false );
        //    continue;
        //}

        if ( skill_data.GetCurSkillInfo() == NULL )
        {
            continue;
        }

        // 아직 활성화 되지 않은 라인은 패스
        if( skill_data.GetLineIndex() > enable_line )
        {
            continue;
        }


        // 요구레벨 검사
        if ( skill_data.GetCurSkillInfo()->m_wRequireLv > GetHeroLevel() )
        {
            continue;
        }

        // 최대레벨 검사
        if ( skill_data.GetCurSkillInfo()->m_wSkillLv >= skill_data.GetCurSkillInfo()->m_wMaxLv )
        {
            continue;
        }

        // 요구스킬 검사
        if( skill_data.GetRequiredSkillClassCode() != INVALID_SKILL_CLASS_CODE && 
            skill_data.GetRequiredSkillLevel() > INVALID_SKILL_LEVEL )
        {
            if( skill_set.CheckSkillLevel( skill_data.GetRequiredSkillClassCode(), skill_data.GetRequiredSkillLevel(), NULL ) == false )
            {
                continue;
            }
        }

        skill_data.SetEnableLevelup( true );
    }
#endif

}

void SkillMain::ResetTreeData()
{
    if( m_pHero == NULL )
    {
        m_RemainSkillPointForTree = BattleScene::GetHeroInfo()->m_CharInfo.m_iRemainSkill;
    }
    else
    {
        m_RemainSkillPointForTree = m_pHero->GetCharInfo()->m_iRemainSkill;
    }

    m_SkillDataSetForTree = m_SkillDataSet;

    RefreshLevelupCondition();
}

bool SkillMain::LevelupSkill( SkillData* skill_data )
{
    if( m_RemainSkillPointForTree == 0 )
    {
        return false;
    }

    if( skill_data->LevelUp() )
    {
        --m_RemainSkillPointForTree;

        RefreshActiveSkill( m_SkillDataSetForTree );
        RefreshLevelupCondition();
    }

    return true;
}

void SkillMain::RefreshActiveSkill( SkillDataSet& skill_set )
{
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];

        // 이미 활성화 된 스킬은 패스
        if( skill_data.IsActiveState() == true )
        {
            continue;
        }


        int total_used_point_per_side = skill_set.GetTotalUsedPoint( skill_data.IsLeftSide() );
        int enable_line = total_used_point_per_side / s_point_per_line;
        if( enable_line >= s_max_row )
            enable_line = s_max_row -1;

        // 아직 활성화 되지않은 라인은 패스
        if( skill_data.GetLineIndex() > enable_line )
        {
            continue;
        }

        if( skill_data.GetSkillLevel() == 1 )
        {
            if( skill_data.IsActive() == true )
            {
                skill_data.SetActiveState();
            }
        }
        else
        {
            skill_data.SetActiveState();
        }
    }
#else
    static const int max_row = 6;
    static const int point_per_line = 5;

    int total_used_point = skill_set.GetTotalUsedPoint();
    int enable_line = total_used_point / point_per_line;
    if( enable_line >= max_row )
        enable_line = max_row -1;

    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];

        // 이미 활성화 된 스킬은 패스
        if( skill_data.IsActiveState() == true )
        {
            continue;
        }

        // 아직 활성화 되지않은 라인은 패스
        if( skill_data.GetLineIndex() > enable_line )
        {
            continue;
        }

        if( skill_data.GetRequiredSkillClassCode() != INVALID_SKILL_CLASS_CODE && 
            skill_data.GetRequiredSkillLevel() > INVALID_SKILL_LEVEL )
        {
            if( skill_set.CheckSkillLevel( skill_data.GetRequiredSkillClassCode(), skill_data.GetRequiredSkillLevel(), NULL ) == true )
            {
                skill_data.SetActiveState();
            }
        }
        else
        {
            skill_data.SetActiveState();
        }
    }
#endif

}

// 배울 수 있는 최대 스킬레벨의 스킬목록을 만든다
bool SkillMain::MakeMaxSkillCodeArray( std::vector<int>& code_array )
{
	code_array.clear();

	for( size_t i=0;i!=m_SkillDataSet.size();++i)
	{
		SkillData& skill_data = m_SkillDataSet[i];
		if( skill_data.GetCurSkillInfo() == NULL )
			continue;

		int cur_skill_class_code = skill_data.GetCurSkillInfo()->m_SkillClassCode;
		int cur_skill_code = skill_data.GetCurSkillInfo()->m_SkillCode;
		int new_skill_code = cur_skill_code;
		while( true )
		{
			SkillScriptInfo* next_skill_info = SkillInfoParser::Instance()->GetSkillInfo( cur_skill_code + 1);
			if( next_skill_info == NULL )
				break;

			if( cur_skill_class_code != next_skill_info->m_SkillClassCode )
				break;

			cur_skill_code = next_skill_info->m_SkillCode;
			new_skill_code = cur_skill_code;
		}

		if( new_skill_code != skill_data.GetCurSkillInfo()->m_SkillCode )
		{
			code_array.push_back( new_skill_code );
		}
	}

	return code_array.empty() == false;
}

bool SkillMain::MakeNewSkillCodeArray( std::vector<int>& code_array )
{
    code_array.clear();

    for( size_t i=0;i!=m_SkillDataSetForTree.size();++i)
    {
        SkillData& copy_data = m_SkillDataSetForTree[i];
        
        if( copy_data.IsActiveState() == false )
            continue;

        bool new_skill = true;
        for( size_t j=0;j!=m_SkillDataSet.size();++j)
        {
            // 같은 스킬코드라면 같은 레벨이므로 배운게 아님 (레벨이 변경된 경우는 코드 자체가 다르기때문에.. )
            if( copy_data.GetSkillCode() == m_SkillDataSet[j].GetSkillCode() )
            {
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
                // 다만 활성화 여부가 변경된 경우에는 습득으로 판단 (레벨1 비활성에서, 레벨1 활성화로 간 경우)
                if( m_SkillDataSet[j].IsActive() == copy_data.IsActive() )
                {
                    new_skill = false;
                    break;
                }
#else
                new_skill = false;
                break;
#endif
            }
        }

        if( new_skill == false )
            continue;

        code_array.push_back( copy_data.GetSkillCode() );
    }

    return code_array.empty() ? false : true;
}


int SkillMain::SkillCode2SkillClassCode( int skill_code )
{
    if( skill_code == INVALID_SKILL_CODE )
    {
        return INVALID_SKILL_CLASS_CODE;
    }

    SkillInfoParser* parser = SkillInfoParser::Instance();
    if( parser == NULL )
    {
        return INVALID_SKILL_CLASS_CODE;
    }

    SkillScriptInfo* skill_info = parser->GetSkillInfo( skill_code );
    if( skill_info == NULL ) 
    {
        return INVALID_SKILL_CLASS_CODE;
    }

    return skill_info->m_SkillClassCode;
}