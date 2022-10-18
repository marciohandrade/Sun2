#pragma once

#include "ClientSingleton.h"
#include "SkillData.h"


class Player;

class SkillMain
    : public Client::Singleton<SkillMain>
{
private:

    // 클라이언트(Hero) 고유의 스킬데이터
    SkillDataSet m_SkillDataSet;


    // 스킬창을 위한 데이터 사본
    SkillDataSet m_SkillDataSetForTree;
    // 스킬 포인트 (트리용)
    int m_RemainSkillPointForTree;

    // 히어로
    Player* m_pHero;

public:
    SkillMain();
    ~SkillMain();

    void SetHero( Player* hero ) { m_pHero = hero; }


    void ClearSkillData() { m_SkillDataSet.clear(); }
    void AddSkillData( const SkillData& skill_data ) { m_SkillDataSet.push_back(skill_data); }

    int AddNewSkillCode( CODETYPE skill_code );    // 새로운 스킬을 배울떄


    // basic

    
    size_t GetSkillDataCount() { return m_SkillDataSet.size(); }
    SkillData& GetSkillData( size_t index ) { return m_SkillDataSet[index]; }
    SkillDataSet& GetSkillDataSet() { return m_SkillDataSet; } // 실제 플레이어가 가지고 있는 스킬의 목록
    const SkillDataSet& GetSkillDataSet() const { return m_SkillDataSet; }

    void SetRemainSkillPoint( int remain_point );
    int GetRemainSkillPoint();
    int GetHeroLevel();

    // find
    int FindPosBySkillClassCode(int skill_class_code) { return m_SkillDataSet.FindPosBySkillClassCode( skill_class_code ); }
    int FindPosBySkillCode(SLOTCODE skill_code );

    SkillData* GetSkillDataBySkillClassCode(CODETYPE skill_class) { return m_SkillDataSet.GetSkillDataBySkillClassCode( skill_class ); }
    SkillData* GetSkillDataBySkillCode(CODETYPE skill_code) { return m_SkillDataSet.GetSkillDataBySkillCode( skill_code ); }
    SkillData* GetSkillDataBySlotPosition(int position) { return m_SkillDataSet.GetSkillDataBySlotPosition( position ); }


    void BlockSkill(DWORD char_class);
    void UnBlockSkill();


    // 스킬의 습득여부를 검사
    bool CheckSkillLevel(DWORD skill_class, int skill_level, DWORD* skill_name_code);

    // 스킬이 레벨업가능여부를 갱신
    void RefreshLevelupCondition();

    // 실제 보유중인 스킬레벨에 해당하는 코드를 가져온다
    int GetRealSkillCode( int skill_code );


    // 트리용 데이터 리셋
    void ResetTreeData();

    // 사용가능한 포인트
    int GetRemainUseablePoint() { return m_RemainSkillPointForTree; }

    SkillDataSet& GetSkillDataSetForTree() { return m_SkillDataSetForTree; }  // 스킬 트리에 표시되고 있는 스킬의 목록 (시뮬레이터중일경우 플레이어의 스킬과 다름)
    bool LevelupSkill( SkillData* skill_data );
    void RefreshActiveSkill( SkillDataSet& skill_set );

    // 스킬 시뮬레이트중인가
    bool IsSkillSimulated();

    // 트리의 데이터와 원본의 데이터를 비교해 새롭게 습득한 스킬의 코드목록을 구한다.
    bool MakeNewSkillCodeArray( std::vector<int>& code_array );

	// 배울 수 있는 최대 스킬레벨의 스킬목록을 만든다
	bool MakeMaxSkillCodeArray( std::vector<int>& code_array );


public:
    static int SkillCode2SkillClassCode( int skill_code );
};

#define xGetSkillData()                     SkillMain::Instance()
#define SKILLCODE2SKILLCLASSCODE(_code)     SkillMain::SkillCode2SkillClassCode(_code)
