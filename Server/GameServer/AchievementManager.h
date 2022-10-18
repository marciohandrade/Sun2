#pragma once

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED

#include <Achievement.h>
#include <AchievementSearch.h>

class AchievementManager : public AchievementNotify
{
private:
	enum {OBJECT_TYPE_NONE = 0, 
		  OBJECT_TYPE_CODE,		//아이템 NPC 등의 코드를 사용
		  OBJECT_TYPE_KEY,};	//업적 스크립트에 키를 사용
public:
	AchievementManager(void);
	virtual ~AchievementManager(void);

	virtual void Init( Player * player );
	virtual void Release();

	BOOL IncMissionValue( WORD object_code);									//업적 수치를 1 증가 시킨다. (미션 완료 전용)
	BOOL IncCurseTowerValue( WORD object_key);									//업적 수치를 1 증가 시킨다. (저주의 탑 전용)
	BOOL IncConditionValue( BYTE type ,WORD object_key, WORD object_code);		//업적 수치를 1 증가 시킨다. 	
	BOOL AddConditionValue( BYTE type ,WORD object_key ,WORD object_value);     //업적 수치를 입력값만큼 증가시킨다. (GM 명령어로만 사용)
	BOOL SetConditionValue( WORD location ,WORD index ,BYTE object_index, WORD value);	//업적 수치를 입력값 으로 설정한다.  (GM 명령어로만 사용)

	void RewardAchievement(WORD index_number);				//보상 받기를 한다.

	BOOL ClearAC( WORD location_code );											//입력된 지역의 값을 초기화 한다. (GM 명령어로만 사용)
	
	BOOL CanCollectTreasureBox(const int location_code);//보물상자를 표시해도 되는지 체크 한다.
	void Serialize( Character_Achievements& achievement_info,				
					ACHIEVEMENT_SERIALIZE eType );											
public:
    void OnCanCollectTreasureBox();                                             //보물상자 채집 가능 상태가 되었다.
	void OnValueChanged(WORD index, WORD location_code, BYTE object_index, 
						WORD previous_value, WORD current_value);				//업적 값이 변경되었을때 호출된다. (이벤트 메소드)
	void Onreward(WORD index, WORD location_code, 
				  Achievement_ParserInfo_Reward  rewards_[MAX_ACHIEVEMENT_REWARD], 
					bool is_reward, RC::eAC_RESULT rc);							//보상 여부가 변경되었을때 호출된다. (이벤트 메소드)
#ifdef _NA_001990_ACSYSTEM_ADD
    BOOL ProcessItemUse(SLOTCODE Code, WORD ACCode, BYTE ACReferenceID);
    BOOL ProcessItemObtain(SLOTCODE Code, WORD ACCode, BYTE ACReferenceID, WORD count = 1);
    BOOL ProcessItemEquip(SLOTCODE Code, WORD ACCode, BYTE ACReferenceID);
    BOOL ProcessPerkLevel(WORD id, WORD level);
    BOOL ProcessSkillStepUp(WORD line);
    BOOL ProcessMissionClearTime(WORD missioncode, WORD time);
	BOOL ProcessLevelUp(WORD level);                                            // 레벨업과제
    BOOL ProcessACClear(WORD index);                                            // AC완료과제
    BOOL ProcessQuest(QCODE Code, WORD ACCode, BYTE ACReferenceID);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    BOOL ProcessFameRepute(BYTE type, WORD code);
    BOOL ProcessFameReputeCount(BYTE type, WORD grade);
    BOOL ProcessQuestCount(WORD normalQ_count, WORD dailyQ_count);
    BOOL ProcessLotto(int lotto_rank);
    BOOL ProcessDailyQuest(BOOL success);
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_002935_20110704_ITEM_SCORE
    BOOL ProcessItemScore(const int item_score); // 아이템 평가 점수 달성 과제
#endif // _NA_002935_20110704_ITEM_SCORE
    bool ProcessEnchant(const SLOTCODE item_code, const uint8_t enchant); // 인첸트 달성
    
    void SetValueZero_if(ACHIEVEMENT_OBJECT_TYPE type, WORD mapcode);
    void SetValueMax_if(ACHIEVEMENT_OBJECT_TYPE type, WORD mapcode, DWORD value, WORD scale = 1);
    void SetValue_if(ACHIEVEMENT_OBJECT_TYPE type, /*WORD keycode,*/ WORD value);  // for 2 argument
    
    void ProcessBattleEnter(WORD mapcode);
    void ProcessBattleResult(WORD mapcode, DWORD heal, WORD maxDamage, DWORD damageToMonster, DWORD damageByMonster, WORD countKill, WORD countDead);                                        // 전투정보창
    
    BOOL IncConditionValue(ACHIEVEMENT_OBJECT_TYPE actype, WORD data, WORD count = 1);         // 업적에 해당하는 항목인지 확인후 1 증가 시킨다.

    BOOL GetAchievement(Achievement_SearchInfo* achievement_parserInfo, OUT AchievementObjectInfo& object_info);
    BOOL SetAchievementValue(Achievement_SearchInfo* achievement_parserInfo, WORD value, ACHIEVEMENT_VALUE_CHANGE_TYPE type);

#ifdef _NA_008166_20150330_MODIFY_AC_LEVELUP
    //ACHIEVEMENT_OBJECT_LEVELUP, 레벨업AC의 수치증가처리는 여기서
    BOOL IncConditionValue_LevelUpAC(WORD data, WORD count = 1);

    BOOL GetFlag_InitializeLevelUpAC() { return is_initialize_levelup_ac_;}
#endif //_NA_008166_20150330_MODIFY_AC_LEVELUP
#endif //_NA_001990_ACSYSTEM_ADD

private:
	void Reward(WORD index, WORD location_code, WORD reward_type, WORD reward_code);		//실제 보상 받기 처리를 한다.
	void SendRewardFail(RC::eAC_RESULT rc);										//보상 실패 메시지를 전송한다.

#ifdef _NA_008166_20150330_MODIFY_AC_LEVELUP
    VOID _SetFlag_InitializeLevelUpAC() {is_initialize_levelup_ac_ = TRUE; }
#endif //_NA_008166_20150330_MODIFY_AC_LEVELUP
private:
	AchievementSearch* achievement_search_;										//Key(아이템코드, 몬스터코드 등등) 로 업적을 찾을수 있다.
	Achievement	achievement_;													//업적을 관리한다.
	Player *player_;															//플레이어 객체

#ifdef _NA_008166_20150330_MODIFY_AC_LEVELUP
    BOOL is_initialize_levelup_ac_;
#endif //_NA_008166_20150330_MODIFY_AC_LEVELUP
};

#endif
