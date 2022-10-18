#ifndef __ACHIEVEMENT_ACHIEVEMENT_H
#define __ACHIEVEMENT_ACHIEVEMENT_H
#pragma once


#include "AchievementPart.h"

#if !defined(_SERVER)
//_SCJ_ACSYSTEM_OVERLAP_PARENT
class AchievementInfo;
#endif //!defined(_SERVER)

class Achievement
{
public:
	typedef STLX_HASH_MAP<WORD, AchievementPart*>		 ACHIEVEMENT_PART_MAP;    
	typedef ACHIEVEMENT_PART_MAP::iterator               ACHIEVEMENT_PART_ITER;
	typedef std::pair<ACHIEVEMENT_PART_ITER, bool>       ACHIEVEMENT_PART_RESULT;
    //
    Achievement(void);
    ~Achievement(void);    

    inline ACHIEVEMENT_PART_ITER	Begin()		{ return achievement_part_.begin(); }
    inline ACHIEVEMENT_PART_ITER	End()		{ return achievement_part_.end(); }	

    void    Init(AchievementNotify* notify = NULL);											//초기화	
	bool	InitAchievementPart(WORD location_code);									//입력된 지역의 모든 값을 초기화 한다.

	void	Serialize( Character_Achievements& achievement_info, ACHIEVEMENT_SERIALIZE eType);	//데이타 직렬화

	bool	SetAchievementValue(WORD location_code, WORD index_number,
								BYTE object_index, WORD object_value,
								ACHIEVEMENT_VALUE_CHANGE_TYPE type);
	bool	SetAchievementValue(Achievement_ParserInfo* achievement_parser_info,
								BYTE index, WORD value,
							    ACHIEVEMENT_VALUE_CHANGE_TYPE type);					//업적 값을 변경한다.	
	WORD    AddAccomplishmentRation(WORD location_code, WORD accomplishment_ration);	//달성률을 증가 시킨다.
	bool	RewardAchievement(WORD index_number, WORD location_code,bool is_reward);	//보상 상태를 변경한다.
    bool	Force_RewardAchievement(WORD index_number, WORD location_code,bool is_reward);	//보상 로직 비교없이 강제로 상태를 변경한다.(마이그레이션에 필요)

	bool	IsAccomplishment(WORD location_code, WORD index_number);
	bool	IsAccomplishment(Achievement_ParserInfo* achievement_parser_info);			//해당 업적이 달성되었는지 확인한다.
#ifdef _NA_001990_ACSYSTEM_ADD
    bool    IsAccomplishmentByIndex(WORD index);                                        //인덱스값으로 업적이 완료되었는지 확인한다.
#endif
    void	InitAccomplishmentRatio();													//업적률을 초기화 한다.

    void    AccomplishAchievement();                                                    //달성률이 보상인 업적은 모두 보상 받기상태로 만든다.(마이그레이션에 필요)
#ifdef _SERVER
    bool    CanCollectTreasureBox(WORD location_code);                                  //해당 지역의 보물 상자를 표시해야 할지 결정한다.
#endif

    AchievementPart* GetAchievementPart(WORD location_code);							//입력된 지역의 업적 맵 을 구한다.    

#if !defined(_SERVER)
    //_SCJ_ACSYSTEM_OVERLAP_PARENT
    AchievementInfo* FindAchievementToIndex(WORD index_number); //인덱스로 AchievementInfo를 얻는다.
#endif //!defined(_SERVER)

private:
    // CHANGES: f110526.5L, changes to shared pool each AchievementInfo
    static AchievementPart* AllocPart();
    static BOOL FreePart(AchievementPart* delete_pointer);
#ifdef _SERVER
    void    AddTreasureConditionInfo();                                                 //보물 상자 채집 보이기에 대한 조건 정보를 추가한다.
#endif
	void	AddAllAchievement();														//초기에 스크립트의 모든 업적을 등록한다. (빈값의 업적)	
	bool	SetAchievementValue(Character_Achievement& char_achievement_info);			//업적의 값을 설정한다.	
	bool	AddAchievement(Achievement_ParserInfo* achievement_parser_info);			//업적을 추가한다.	
    //
#if !defined(_SERVER)
    //_SCJ_ACSYSTEM_OVERLAP_PARENT
    void    SetAchievementParentMap(); // 선행코드 리스트를 셋팅한다.
#endif //!defined(_SERVER)

	ACHIEVEMENT_PART_MAP	achievement_part_;        //업적의 지역 맵
    AchievementParser*		achievement_parser_;	  //업적 파서 포인터	
	AchievementNotify*		notify_;				  //업적의 변경사항을 통보할 이벤트 전송 객체

};

#endif //__ACHIEVEMENT_ACHIEVEMENT_H


