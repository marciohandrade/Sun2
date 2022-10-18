#ifndef __ACHIEVEMENT_ACHIEVEMENTPART_H
#define __ACHIEVEMENT_ACHIEVEMENTPART_H
#pragma once

#include "AchievementInfo.h"
#include "CollectTreasureBoxInfo.h"
#include <StructInPacket.h>

//==================================================================================================

struct Collect_Condition_Accomplishment_Info : public Collect_Condition_Info
{
    bool    is_accomplishment_[MAX_COLLECT_CONDITION];

    bool    can_collect_treasurebox_;

    void CalcConditionAccomplish()
    {
        can_collect_treasurebox_ = true;
        for(int j = 0 ; j < Collect_Condition_Accomplishment_Info::MAX_COLLECT_CONDITION ; j++)
        {
            can_collect_treasurebox_ = can_collect_treasurebox_ && is_accomplishment_[j];
        }
    }    
};

//==================================================================================================
//
class AchievementPart
{
public:
	typedef STLX_HASH_MAP<WORD, AchievementInfo*>    ACHIEVEMENT_MAP;    
	typedef ACHIEVEMENT_MAP::iterator               ACHIEVEMENT_ITER;
	typedef std::pair<ACHIEVEMENT_ITER, bool>       ACHIEVEMENT_RESULT;
    //
    AchievementPart(void);
    ~AchievementPart(void);  

	inline ACHIEVEMENT_ITER			Begin()		{ return infos_.begin(); }
	inline ACHIEVEMENT_ITER			End()		{ return infos_.end(); }

	void Init(AchievementNotify* notify);
	void InitAchievement();																//���� ���� �ʱ�ȭ �Ѵ�.
	void InitAccomplishmentRatio();														//���� �޼����� �����Ѵ�.

	void LoadAchievement(WORD& index, Character_Achievements& achievement_info, 
						 ACHIEVEMENT_SERIALIZE eType);	//����Ÿ ����ȭ ��Ű�� ���� ���� ���� �����Ѵ�.
	
	WORD AddAccomplishmentRation(WORD accomplishment_ration);							//���� �޼����� �� �����Ѵ�. (�޼����� ������ �� ����)	

	bool IsAccomplishment(WORD index);													//������ �޼��Ǿ����� Ȯ���Ѵ�.

	AchievementInfo* AddAchievement(WORD index_number);									//������ �߰��Ѵ�.
	AchievementInfo* AddAchievement(Achievement_ParserInfo* achievement_parser_info);	//������ �߰��Ѵ�.
    AchievementInfo* GetAchievement(WORD index_number);									//���� ��ü�� ��´�.
	bool SetAchievementValue(Character_Achievement& char_achievement_info,				
							 ACHIEVEMENT_VALUE_CHANGE_TYPE type);						//���� ���� �����Ѵ�.
	bool SetAchievementValue(WORD index, BYTE object_index, 
							 WORD value , ACHIEVEMENT_VALUE_CHANGE_TYPE type);			//���� ���� �����Ѵ�.

	bool RewardAchievement(WORD index_number, bool is_reward);							//���� ���� ������ �����Ѵ�.
    bool Force_RewardAchievement(WORD index_number, bool is_reward);							//���� ���� ������ �����Ѵ�.
    void AccomplishAchievement();                                                      //�޼����� ������ ������ ��� ���� �ޱ���·� �����.(���̱׷��̼ǿ� �ʿ�)    
#ifdef _SERVER
    void SetTreasureBoxConditionInfo(Collect_Condition_Info* collect_condition_info);      //���� ���� ä�� ���� ������ �����Ѵ�.
#endif
	inline void SetLocationCode(WORD location_code)	{location_code_ = location_code;}
	inline WORD GetLocationCode()					{return location_code_;}
	inline WORD	GetAccomplishmentRatio()			{return ratio_sum_;}
    inline bool show_ratio()                        {return show_ratio_;}
#ifdef _SERVER
    inline bool CanCollectTreasureBox()             {return collect_condition_info_.can_collect_treasurebox_;}
    void    CheckTreasureBoxCondition(AchievementInfo* achievement_info, ACHIEVEMENT_VALUE_CHANGE_TYPE type);
#endif

#if !defined(_SERVER)
    //_SCJ_ACSYSTEM_OVERLAP_PARENT
    void SetAchievementPartParentMap(Achievement* achievement);
    AchievementInfo* FindAchievementInfoToIndex(WORD index_number);
#endif //!defined(_SERVER)
private:
    // CHANGES: f110526.5L, changes to shared pool each AchievementInfo
    static AchievementInfo* AllocInfo();
    static BOOL FreeInfo(AchievementInfo* delete_pointer);
    void InitAccomplishmentAchievement(ACHIEVEMENT_VALUE_CHANGE_TYPE type); //�޼����� ������ �� ���� �Ѵ�.
    //
	WORD					ratio_sum_;		//���� �޼��� ���� ����
    ACHIEVEMENT_MAP			infos_;			//���� ����
	ACHIEVEMENT_MAP 		ratios_;		//�޼����� �����ΰ� ����
    bool                    show_ratio_;    //�޼����� ������ ���� �Ϸ� ������ ������

	WORD					location_code_; //���� �ڵ�

	AchievementParser*		parser_;		//��ũ��Ʈ �ļ� ��ü 
	AchievementNotify*		notify_;		//��������� �뺸���� �̺�Ʈ ��ü    
#ifdef _SERVER
    Collect_Condition_Accomplishment_Info collect_condition_info_;
#endif
};

#endif //__ACHIEVEMENT_ACHIEVEMENTPART_H
