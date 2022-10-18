#pragma once

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED

#include <Achievement.h>
#include <AchievementSearch.h>

class AchievementManager : public AchievementNotify
{
private:
	enum {OBJECT_TYPE_NONE = 0, 
		  OBJECT_TYPE_CODE,		//������ NPC ���� �ڵ带 ���
		  OBJECT_TYPE_KEY,};	//���� ��ũ��Ʈ�� Ű�� ���
public:
	AchievementManager(void);
	virtual ~AchievementManager(void);

	virtual void Init( Player * player );
	virtual void Release();

	BOOL IncMissionValue( WORD object_code);									//���� ��ġ�� 1 ���� ��Ų��. (�̼� �Ϸ� ����)
	BOOL IncCurseTowerValue( WORD object_key);									//���� ��ġ�� 1 ���� ��Ų��. (������ ž ����)
	BOOL IncConditionValue( BYTE type ,WORD object_key, WORD object_code);		//���� ��ġ�� 1 ���� ��Ų��. 	
	BOOL AddConditionValue( BYTE type ,WORD object_key ,WORD object_value);     //���� ��ġ�� �Է°���ŭ ������Ų��. (GM ��ɾ�θ� ���)
	BOOL SetConditionValue( WORD location ,WORD index ,BYTE object_index, WORD value);	//���� ��ġ�� �Է°� ���� �����Ѵ�.  (GM ��ɾ�θ� ���)

	void RewardAchievement(WORD index_number);				//���� �ޱ⸦ �Ѵ�.

	BOOL ClearAC( WORD location_code );											//�Էµ� ������ ���� �ʱ�ȭ �Ѵ�. (GM ��ɾ�θ� ���)
	
	BOOL CanCollectTreasureBox(const int location_code);//�������ڸ� ǥ���ص� �Ǵ��� üũ �Ѵ�.
	void Serialize( Character_Achievements& achievement_info,				
					ACHIEVEMENT_SERIALIZE eType );											
public:
    void OnCanCollectTreasureBox();                                             //�������� ä�� ���� ���°� �Ǿ���.
	void OnValueChanged(WORD index, WORD location_code, BYTE object_index, 
						WORD previous_value, WORD current_value);				//���� ���� ����Ǿ����� ȣ��ȴ�. (�̺�Ʈ �޼ҵ�)
	void Onreward(WORD index, WORD location_code, 
				  Achievement_ParserInfo_Reward  rewards_[MAX_ACHIEVEMENT_REWARD], 
					bool is_reward, RC::eAC_RESULT rc);							//���� ���ΰ� ����Ǿ����� ȣ��ȴ�. (�̺�Ʈ �޼ҵ�)
#ifdef _NA_001990_ACSYSTEM_ADD
    BOOL ProcessItemUse(SLOTCODE Code, WORD ACCode, BYTE ACReferenceID);
    BOOL ProcessItemObtain(SLOTCODE Code, WORD ACCode, BYTE ACReferenceID, WORD count = 1);
    BOOL ProcessItemEquip(SLOTCODE Code, WORD ACCode, BYTE ACReferenceID);
    BOOL ProcessPerkLevel(WORD id, WORD level);
    BOOL ProcessSkillStepUp(WORD line);
    BOOL ProcessMissionClearTime(WORD missioncode, WORD time);
	BOOL ProcessLevelUp(WORD level);                                            // ����������
    BOOL ProcessACClear(WORD index);                                            // AC�Ϸ����
    BOOL ProcessQuest(QCODE Code, WORD ACCode, BYTE ACReferenceID);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    BOOL ProcessFameRepute(BYTE type, WORD code);
    BOOL ProcessFameReputeCount(BYTE type, WORD grade);
    BOOL ProcessQuestCount(WORD normalQ_count, WORD dailyQ_count);
    BOOL ProcessLotto(int lotto_rank);
    BOOL ProcessDailyQuest(BOOL success);
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_002935_20110704_ITEM_SCORE
    BOOL ProcessItemScore(const int item_score); // ������ �� ���� �޼� ����
#endif // _NA_002935_20110704_ITEM_SCORE
    bool ProcessEnchant(const SLOTCODE item_code, const uint8_t enchant); // ��þƮ �޼�
    
    void SetValueZero_if(ACHIEVEMENT_OBJECT_TYPE type, WORD mapcode);
    void SetValueMax_if(ACHIEVEMENT_OBJECT_TYPE type, WORD mapcode, DWORD value, WORD scale = 1);
    void SetValue_if(ACHIEVEMENT_OBJECT_TYPE type, /*WORD keycode,*/ WORD value);  // for 2 argument
    
    void ProcessBattleEnter(WORD mapcode);
    void ProcessBattleResult(WORD mapcode, DWORD heal, WORD maxDamage, DWORD damageToMonster, DWORD damageByMonster, WORD countKill, WORD countDead);                                        // ��������â
    
    BOOL IncConditionValue(ACHIEVEMENT_OBJECT_TYPE actype, WORD data, WORD count = 1);         // ������ �ش��ϴ� �׸����� Ȯ���� 1 ���� ��Ų��.

    BOOL GetAchievement(Achievement_SearchInfo* achievement_parserInfo, OUT AchievementObjectInfo& object_info);
    BOOL SetAchievementValue(Achievement_SearchInfo* achievement_parserInfo, WORD value, ACHIEVEMENT_VALUE_CHANGE_TYPE type);

#ifdef _NA_008166_20150330_MODIFY_AC_LEVELUP
    //ACHIEVEMENT_OBJECT_LEVELUP, ������AC�� ��ġ����ó���� ���⼭
    BOOL IncConditionValue_LevelUpAC(WORD data, WORD count = 1);

    BOOL GetFlag_InitializeLevelUpAC() { return is_initialize_levelup_ac_;}
#endif //_NA_008166_20150330_MODIFY_AC_LEVELUP
#endif //_NA_001990_ACSYSTEM_ADD

private:
	void Reward(WORD index, WORD location_code, WORD reward_type, WORD reward_code);		//���� ���� �ޱ� ó���� �Ѵ�.
	void SendRewardFail(RC::eAC_RESULT rc);										//���� ���� �޽����� �����Ѵ�.

#ifdef _NA_008166_20150330_MODIFY_AC_LEVELUP
    VOID _SetFlag_InitializeLevelUpAC() {is_initialize_levelup_ac_ = TRUE; }
#endif //_NA_008166_20150330_MODIFY_AC_LEVELUP
private:
	AchievementSearch* achievement_search_;										//Key(�������ڵ�, �����ڵ� ���) �� ������ ã���� �ִ�.
	Achievement	achievement_;													//������ �����Ѵ�.
	Player *player_;															//�÷��̾� ��ü

#ifdef _NA_008166_20150330_MODIFY_AC_LEVELUP
    BOOL is_initialize_levelup_ac_;
#endif //_NA_008166_20150330_MODIFY_AC_LEVELUP
};

#endif
