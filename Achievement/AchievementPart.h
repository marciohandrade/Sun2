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
	void InitAchievement();																//업적 값을 초기화 한다.
	void InitAccomplishmentRatio();														//업적 달성률을 설정한다.

	void LoadAchievement(WORD& index, Character_Achievements& achievement_info, 
						 ACHIEVEMENT_SERIALIZE eType);	//데이타 직렬화 시키기 위해 업적 값을 세팅한다.
	
	WORD AddAccomplishmentRation(WORD accomplishment_ration);							//업적 달성률을 재 구성한다. (달성률이 업적인 값 세팅)	

	bool IsAccomplishment(WORD index);													//업적이 달성되었는지 확인한다.

	AchievementInfo* AddAchievement(WORD index_number);									//업적을 추가한다.
	AchievementInfo* AddAchievement(Achievement_ParserInfo* achievement_parser_info);	//업적을 추가한다.
    AchievementInfo* GetAchievement(WORD index_number);									//업적 객체를 얻는다.
	bool SetAchievementValue(Character_Achievement& char_achievement_info,				
							 ACHIEVEMENT_VALUE_CHANGE_TYPE type);						//업적 값을 세팅한다.
	bool SetAchievementValue(WORD index, BYTE object_index, 
							 WORD value , ACHIEVEMENT_VALUE_CHANGE_TYPE type);			//업적 값을 세팅한다.

	bool RewardAchievement(WORD index_number, bool is_reward);							//보상 받은 설정을 세팅한다.
    bool Force_RewardAchievement(WORD index_number, bool is_reward);							//보상 받은 설정을 세팅한다.
    void AccomplishAchievement();                                                      //달성률이 보상인 업적은 모두 보상 받기상태로 만든다.(마이그레이션에 필요)    
#ifdef _SERVER
    void SetTreasureBoxConditionInfo(Collect_Condition_Info* collect_condition_info);      //보물 상자 채집 가능 조건을 설정한다.
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
    void InitAccomplishmentAchievement(ACHIEVEMENT_VALUE_CHANGE_TYPE type); //달성률이 업적인 값 세팅 한다.
    //
	WORD					ratio_sum_;		//현재 달성된 업적 비율
    ACHIEVEMENT_MAP			infos_;			//업적 보관
	ACHIEVEMENT_MAP 		ratios_;		//달성률이 업적인것 보관
    bool                    show_ratio_;    //달성률로 보일지 업적 완료 개수로 보일지

	WORD					location_code_; //지역 코드

	AchievementParser*		parser_;		//스크립트 파서 객체 
	AchievementNotify*		notify_;		//변경사항을 통보해줄 이벤트 객체    
#ifdef _SERVER
    Collect_Condition_Accomplishment_Info collect_condition_info_;
#endif
};

#endif //__ACHIEVEMENT_ACHIEVEMENTPART_H
