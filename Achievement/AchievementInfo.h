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
	ACHIEVEMENT_VALUE_SET,		//�� ����
	ACHIEVEMENT_VALUE_APPEND,	//�� �߰�
	ACHIEVEMENT_VALUE_INIT,		//�ʱⰪ ����
};

struct AchievementObjectInfo
{
	WORD    Achievement_;	//�޼� ��ǥ �� (�� �� �̻��̸� �޼�)
	WORD    value_;			//������ ���� ��
};	

class AchievementInfo
{
public:    
    AchievementInfo();
	~AchievementInfo(void);

private:
	enum Achievement_Status {EMPTY = 0,			//������ �ϳ��� �޼����� ���� ����
		NORMAL,			//������ġ�� ���� ������ ���� �÷����� ��������� ��������
		MODIFICATION};		//�߰��� ���� ��ġ�� ����Ȼ���

public:
	inline bool	IsEmpty()						{return status_ == EMPTY;}
	inline bool	IsNormal()						{return status_ == NORMAL;}
	inline bool	IsModification()				{return status_ == MODIFICATION;}


public:
	void Init(Achievement_ParserInfo* achievement_parser_info, AchievementNotify* notify);
	void InitValue();									//���� ���õ� ���� �ʱ�ȭ �Ѵ�.
	bool IsAccomplishment(BYTE object_index);			//�ش� object �� �޼��Ǿ����� Ȯ���Ѵ�.
	bool IsAccomplishment() const;						//������ �޼��Ǿ����� ����														
	bool IsAccomplishmentReward();						//�����߿� ������ ������ �ִ��� Ȯ���Ѵ�.

	bool SetValue(BYTE object_index, WORD object_value, ACHIEVEMENT_VALUE_CHANGE_TYPE type);
	void SetValue(WORD object_value[MAX_ACHIEVEMENT_OBJECT], ACHIEVEMENT_VALUE_CHANGE_TYPE type);
	void GetValue(WORD object_value[MAX_ACHIEVEMENT_OBJECT]);
	void SetReward(bool reward_value, ACHIEVEMENT_VALUE_CHANGE_TYPE type);//���� ���� ����

	WORD GetAccomplishmentRatio();						//������ �޼����̸� �޼��� ��ġ�� �����Ѵ�.	

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
    bool IsShowRewardToParentMap();    // ������� �����ؼ� �����ư ������
    AchievementInfo* GetRewardToParentMap();    // ������� �����ؼ� ������� ����
#endif
#ifdef _NA_001990_ACSYSTEM_ADD
    inline DWORD GetClassType()  {return class_type_;}
    inline WORD  GetParentCode(BYTE index){return parent_ac_code_[index];}
#endif //_NA_001990_ACSYSTEM_ADD

private:
    void NotifyChangedValue(BYTE object_index, WORD previous_value, WORD current_value);	//������ ���� ����Ǿ����� �̺�Ʈ�� ������.
	void Notifyreward(bool is_reward, RC::eAC_RESULT rc);	//������ ���� ���� �̺�Ʈ�� ������.

    static bool _Sort_Index(AchievementInfo*& info_left, AchievementInfo*& info_right);
#if !defined(_SERVER)
    //defined(_SCJ_ACSYSTEM_OVERLAP_PARENT)
    void SetParentMap(Achievement* achievement);
    void MakeParentMap(Achievement* achievement, ACHIEVEMENT_INFO_MAP* map_pointer, int depth);
    void SetChildMap(Achievement* achievement);
    void AddChildMap(AchievementInfo* achievement_info);
#endif //!defined(_SERVER)
private:	
	Achievement_Status		status_;				  //���� ���� ���¸� ����Ų��.
	WORD	index_number_;			//���� index
	bool	is_reward_;				//���� ����		
	WORD	location_code_;			//��������
    WORD    object_type_;           //����Ÿ��
#if !defined(_SERVER)
	DWORD    object_string_code_;	//���� ��     
	DWORD    tooltip_string_code_;	//����
	DWORD    icon_code_;				//������ ��ȣ

    //_SCJ_ACSYSTEM_OVERLAP_PARENT
    ACHIEVEMENT_INFO_MAP parent_ac_map_; // ���� AC���� ����Ʈ
    ACHIEVEMENT_INFO_MAP child_ac_map_; // �Ǽ���(����)AC���
#endif 
#ifdef _NA_001990_ACSYSTEM_ADD
    DWORD    class_type_;            //Ŭ��������
    WORD     parent_ac_code_[MAX_ACHIEVEMENT_PARENT_CODE]; //����AC�ڵ�
#endif //_NA_001990_ACSYSTEM_ADD

	Achievement_ParserInfo_Reward	rewards_[MAX_ACHIEVEMENT_REWARD];			//���� ����
	AchievementObjectInfo			obejct_value_[MAX_ACHIEVEMENT_OBJECT];		//���� ����

	AchievementNotify* notify_;													//���� ������ �뺸�� �̺�Ʈ ��ü

};

#endif //__ACHIEVEMENT_ACHIEVEMENTINFO_H
