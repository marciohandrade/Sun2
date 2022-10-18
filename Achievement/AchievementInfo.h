#ifndef __ACHIEVEMENT_ACHIEVEMENTINFO_H
#define __ACHIEVEMENT_ACHIEVEMENTINFO_H
#pragma once


#include "AchievementParser.h"
#include "AchievementNotify.h"

#if !defined(_SERVER)
//_SCJ_ACSYSTEM_OVERLAP_PARENT
class Achievement;
class AchievementInfo;
typedef STLX_MAP<DWORD, AchievementInfo*>   ACHIEVEMENT_INFO_MAP;
typedef ACHIEVEMENT_INFO_MAP::iterator      ACHIEVEMENT_INFO_MAP_ITER;
#ifdef _NA_008226_AC_PARENTMAP_LIST_SKIP
const int PARENT_LIMIT_DEPTH = 100;
#else
const int PARENT_LIMIT_DEPTH = 30;
#endif //_NA_008226_AC_PARENTMAP_LIST_SKIP
#endif //!defined(_SERVER)

enum ACHIEVEMENT_VALUE_CHANGE_TYPE {
	ACHIEVEMENT_VALUE_SET,		//값 변경
	ACHIEVEMENT_VALUE_APPEND,	//값 추가
	ACHIEVEMENT_VALUE_INIT,		//초기값 설정
};

struct AchievementObjectInfo
{
	WORD    Achievement_;	//달성 목표 값 (이 값 이상이면 달성)
	WORD    value_;			//수행한 현재 값
};	

class AchievementInfo
{
public:    
    AchievementInfo();
	~AchievementInfo(void);

private:
	enum Achievement_Status {EMPTY = 0,			//업적을 하나도 달성하지 않은 상태
		NORMAL,			//업적수치가 존재 하지만 게임 플레이중 변경되지는 않은상태
		MODIFICATION};		//추가로 업적 수치가 변경된상태

public:
	inline bool	IsEmpty()						{return status_ == EMPTY;}
	inline bool	IsNormal()						{return status_ == NORMAL;}
	inline bool	IsModification()				{return status_ == MODIFICATION;}


public:
	void Init(Achievement_ParserInfo* achievement_parser_info, AchievementNotify* notify);
	void InitValue();									//업적 관련된 값만 초기화 한다.
	bool IsAccomplishment(BYTE object_index);			//해당 object 가 달성되었는지 확인한다.
	bool IsAccomplishment() const;						//업적이 달성되었는지 여부														
	bool IsAccomplishmentReward();						//보상중에 업적률 보상이 있는지 확인한다.

	bool SetValue(BYTE object_index, WORD object_value, ACHIEVEMENT_VALUE_CHANGE_TYPE type);
	void SetValue(WORD object_value[MAX_ACHIEVEMENT_OBJECT], ACHIEVEMENT_VALUE_CHANGE_TYPE type);
	void GetValue(WORD object_value[MAX_ACHIEVEMENT_OBJECT]);
	void SetReward(bool reward_value, ACHIEVEMENT_VALUE_CHANGE_TYPE type);//보상 여부 변경

	WORD GetAccomplishmentRatio();						//보상이 달성률이면 달성률 수치를 리턴한다.	

	void SetNotifyObject(AchievementNotify* notify) {notify = notify_;}

	const AchievementObjectInfo* const GetAchievementObjectFormIndex(BYTE object_index);

public:    
    inline WORD GetLocationCode()   {return location_code_;}
	inline WORD GetIndex() const    {return index_number_;}
	inline bool IsReward()		{return is_reward_;}
    inline WORD GetObjectType()  {return object_type_;}
	inline AchievementObjectInfo GetObjectInfo(BYTE Index) {return obejct_value_[Index];}
	inline Achievement_ParserInfo_Reward GetRewardInfo(BYTE Index) {return rewards_[Index];}
#if !defined(_SERVER)
	inline DWORD GetStringCode() {return object_string_code_;}
	inline DWORD GetToolTipCode(){return tooltip_string_code_;}
	inline DWORD GetIconCode()	 {return icon_code_;}

    //_SCJ_ACSYSTEM_OVERLAP_PARENT
    void SetAchievementInfoParentMap(Achievement* achievement);
    inline ACHIEVEMENT_INFO_MAP& GetParentMap() {return parent_ac_map_;}
    inline ACHIEVEMENT_INFO_MAP& GetChildMap() {return child_ac_map_;}
    bool IsShowRewardToParentMap();    // 선행과제 포함해서 보상버튼 보일지
    AchievementInfo* GetRewardToParentMap();    // 선행과제 포함해서 보상받을 과제
#endif
#ifdef _NA_001990_ACSYSTEM_ADD
    inline DWORD GetClassType()  {return class_type_;}
    inline WORD  GetParentCode(BYTE index){return parent_ac_code_[index];}
#endif //_NA_001990_ACSYSTEM_ADD

private:
    void NotifyChangedValue(BYTE object_index, WORD previous_value, WORD current_value);	//업적의 값이 변경되었을때 이벤트를 날린다.
	void Notifyreward(bool is_reward, RC::eAC_RESULT rc);	//업적의 보상 여부 이벤트를 날린다.

    static bool _Sort_Index(AchievementInfo*& info_left, AchievementInfo*& info_right);
#if !defined(_SERVER)
    //defined(_SCJ_ACSYSTEM_OVERLAP_PARENT)
    void SetParentMap(Achievement* achievement);
    void MakeParentMap(Achievement* achievement, ACHIEVEMENT_INFO_MAP* map_pointer, int depth);
    void SetChildMap(Achievement* achievement);
    void AddChildMap(AchievementInfo* achievement_info);
#endif //!defined(_SERVER)
private:	
	Achievement_Status		status_;				  //업적 값의 상태를 가르킨다.
	WORD	index_number_;			//업적 index
	bool	is_reward_;				//보상 여부		
	WORD	location_code_;			//지역정보
    WORD    object_type_;           //과제타입
#if !defined(_SERVER)
	DWORD    object_string_code_;	//과제 명     
	DWORD    tooltip_string_code_;	//툴팁
	DWORD    icon_code_;				//아이콘 번호

    //_SCJ_ACSYSTEM_OVERLAP_PARENT
    ACHIEVEMENT_INFO_MAP parent_ac_map_; // 선행 AC과제 리스트
    ACHIEVEMENT_INFO_MAP child_ac_map_; // 피선행(후행)AC목록
#endif 
#ifdef _NA_001990_ACSYSTEM_ADD
    DWORD    class_type_;            //클래스구분
    WORD     parent_ac_code_[MAX_ACHIEVEMENT_PARENT_CODE]; //선행AC코드
#endif //_NA_001990_ACSYSTEM_ADD

	Achievement_ParserInfo_Reward	rewards_[MAX_ACHIEVEMENT_REWARD];			//보상 정보
	AchievementObjectInfo			obejct_value_[MAX_ACHIEVEMENT_OBJECT];		//과제 정보

	AchievementNotify* notify_;													//변경 사항을 통보할 이벤트 객체

};

#endif //__ACHIEVEMENT_ACHIEVEMENTINFO_H
