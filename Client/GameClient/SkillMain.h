#pragma once

#include "ClientSingleton.h"
#include "SkillData.h"


class Player;

class SkillMain
    : public Client::Singleton<SkillMain>
{
private:

    // Ŭ���̾�Ʈ(Hero) ������ ��ų������
    SkillDataSet m_SkillDataSet;


    // ��ųâ�� ���� ������ �纻
    SkillDataSet m_SkillDataSetForTree;
    // ��ų ����Ʈ (Ʈ����)
    int m_RemainSkillPointForTree;

    // �����
    Player* m_pHero;

public:
    SkillMain();
    ~SkillMain();

    void SetHero( Player* hero ) { m_pHero = hero; }


    void ClearSkillData() { m_SkillDataSet.clear(); }
    void AddSkillData( const SkillData& skill_data ) { m_SkillDataSet.push_back(skill_data); }

    int AddNewSkillCode( CODETYPE skill_code );    // ���ο� ��ų�� ��


    // basic

    
    size_t GetSkillDataCount() { return m_SkillDataSet.size(); }
    SkillData& GetSkillData( size_t index ) { return m_SkillDataSet[index]; }
    SkillDataSet& GetSkillDataSet() { return m_SkillDataSet; } // ���� �÷��̾ ������ �ִ� ��ų�� ���
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


    // ��ų�� ���濩�θ� �˻�
    bool CheckSkillLevel(DWORD skill_class, int skill_level, DWORD* skill_name_code);

    // ��ų�� ���������ɿ��θ� ����
    void RefreshLevelupCondition();

    // ���� �������� ��ų������ �ش��ϴ� �ڵ带 �����´�
    int GetRealSkillCode( int skill_code );


    // Ʈ���� ������ ����
    void ResetTreeData();

    // ��밡���� ����Ʈ
    int GetRemainUseablePoint() { return m_RemainSkillPointForTree; }

    SkillDataSet& GetSkillDataSetForTree() { return m_SkillDataSetForTree; }  // ��ų Ʈ���� ǥ�õǰ� �ִ� ��ų�� ��� (�ùķ��������ϰ�� �÷��̾��� ��ų�� �ٸ�)
    bool LevelupSkill( SkillData* skill_data );
    void RefreshActiveSkill( SkillDataSet& skill_set );

    // ��ų �ùķ���Ʈ���ΰ�
    bool IsSkillSimulated();

    // Ʈ���� �����Ϳ� ������ �����͸� ���� ���Ӱ� ������ ��ų�� �ڵ����� ���Ѵ�.
    bool MakeNewSkillCodeArray( std::vector<int>& code_array );

	// ��� �� �ִ� �ִ� ��ų������ ��ų����� �����
	bool MakeMaxSkillCodeArray( std::vector<int>& code_array );


public:
    static int SkillCode2SkillClassCode( int skill_code );
};

#define xGetSkillData()                     SkillMain::Instance()
#define SKILLCODE2SKILLCLASSCODE(_code)     SkillMain::SkillCode2SkillClassCode(_code)
