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

    void    Init(AchievementNotify* notify = NULL);											//�ʱ�ȭ	
	bool	InitAchievementPart(WORD location_code);									//�Էµ� ������ ��� ���� �ʱ�ȭ �Ѵ�.

	void	Serialize( Character_Achievements& achievement_info, ACHIEVEMENT_SERIALIZE eType);	//����Ÿ ����ȭ

	bool	SetAchievementValue(WORD location_code, WORD index_number,
								BYTE object_index, WORD object_value,
								ACHIEVEMENT_VALUE_CHANGE_TYPE type);
	bool	SetAchievementValue(Achievement_ParserInfo* achievement_parser_info,
								BYTE index, WORD value,
							    ACHIEVEMENT_VALUE_CHANGE_TYPE type);					//���� ���� �����Ѵ�.	
	WORD    AddAccomplishmentRation(WORD location_code, WORD accomplishment_ration);	//�޼����� ���� ��Ų��.
	bool	RewardAchievement(WORD index_number, WORD location_code,bool is_reward);	//���� ���¸� �����Ѵ�.
    bool	Force_RewardAchievement(WORD index_number, WORD location_code,bool is_reward);	//���� ���� �񱳾��� ������ ���¸� �����Ѵ�.(���̱׷��̼ǿ� �ʿ�)

	bool	IsAccomplishment(WORD location_code, WORD index_number);
	bool	IsAccomplishment(Achievement_ParserInfo* achievement_parser_info);			//�ش� ������ �޼��Ǿ����� Ȯ���Ѵ�.
#ifdef _NA_001990_ACSYSTEM_ADD
    bool    IsAccomplishmentByIndex(WORD index);                                        //�ε��������� ������ �Ϸ�Ǿ����� Ȯ���Ѵ�.
#endif
    void	InitAccomplishmentRatio();													//�������� �ʱ�ȭ �Ѵ�.

    void    AccomplishAchievement();                                                    //�޼����� ������ ������ ��� ���� �ޱ���·� �����.(���̱׷��̼ǿ� �ʿ�)
#ifdef _SERVER
    bool    CanCollectTreasureBox(WORD location_code);                                  //�ش� ������ ���� ���ڸ� ǥ���ؾ� ���� �����Ѵ�.
#endif

    AchievementPart* GetAchievementPart(WORD location_code);							//�Էµ� ������ ���� �� �� ���Ѵ�.    

#if !defined(_SERVER)
    //_SCJ_ACSYSTEM_OVERLAP_PARENT
    AchievementInfo* FindAchievementToIndex(WORD index_number); //�ε����� AchievementInfo�� ��´�.
#endif //!defined(_SERVER)

private:
    // CHANGES: f110526.5L, changes to shared pool each AchievementInfo
    static AchievementPart* AllocPart();
    static BOOL FreePart(AchievementPart* delete_pointer);
#ifdef _SERVER
    void    AddTreasureConditionInfo();                                                 //���� ���� ä�� ���̱⿡ ���� ���� ������ �߰��Ѵ�.
#endif
	void	AddAllAchievement();														//�ʱ⿡ ��ũ��Ʈ�� ��� ������ ����Ѵ�. (���� ����)	
	bool	SetAchievementValue(Character_Achievement& char_achievement_info);			//������ ���� �����Ѵ�.	
	bool	AddAchievement(Achievement_ParserInfo* achievement_parser_info);			//������ �߰��Ѵ�.	
    //
#if !defined(_SERVER)
    //_SCJ_ACSYSTEM_OVERLAP_PARENT
    void    SetAchievementParentMap(); // �����ڵ� ����Ʈ�� �����Ѵ�.
#endif //!defined(_SERVER)

	ACHIEVEMENT_PART_MAP	achievement_part_;        //������ ���� ��
    AchievementParser*		achievement_parser_;	  //���� �ļ� ������	
	AchievementNotify*		notify_;				  //������ ��������� �뺸�� �̺�Ʈ ���� ��ü

};

#endif //__ACHIEVEMENT_ACHIEVEMENT_H


