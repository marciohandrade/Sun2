
#pragma once

#include "QuestConditionInfo.h"
#include <ResultCode.h>
#include "Quest.h"

//������������������������������������������������������������������������������������������
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_REPEAT )
	RC::eQUEST_RESULT IsSatisfy( QUESTCOUNT CurRepeatNum );
	__PROPERTY( QUESTCOUNT, MaxRepeatNum )				//< �ִ� ����Ʈ �ݺ� �÷��� �� �� �ִ� Ƚ��
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_CHARLV )
	RC::eQUEST_RESULT IsSatisfy( WORD CurCharLV );
	__PROPERTY( WORD, MinCharLV )
	__PROPERTY( WORD, MaxCharLV )
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_CHARCLASS )
	RC::eQUEST_RESULT IsSatisfy( BYTE byCharClass );
	__PROPERTY( BYTE, CharClassBit )
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_QUEST )
	enum { _DEF_CHECK_NUM=3, _MAX_CHECK_NUM=5, };
	__PROPERTY( BYTE, QuestArrayCount )			//< �� 3��, 5�� : ����ȭ�� ���Ѱ�
	__ARRAY_PROPERTY( QCODE, _MAX_CHECK_NUM, QuestCode )
	__ARRAY_PROPERTY( QCODE, _MAX_CHECK_NUM, QuestNum )		//< �Ϸ� ����Ʈ�� ���
	__PROPERTY( BYTE, Operator )				//< eQUEST_OPERATOR���� => 0 : AND , 1 : OR
	__BOOL_PROPERTY( ProgrQuest )				//< ����Ʈ�� �������ΰ� or �Ϸ�Ȱǰ�
	__BOOL_PROPERTY( NecessaryQuest )			//< �� �迭�� ����Ʈ�� �־���ϴ°�(TRUE) or ����� �ϴ°�(FALSE)
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_ITEM )
	enum { _DEF_CHECK_NUM=3, _MAX_CHECK_NUM=5, };
	__BOOL_PROPERTY( NecessaryItem )			//< �� �������� �־�� �Ѵ�(TRUE) or ����� �Ѵ�(FALSE)
	__PROPERTY( BYTE, ItemArrayCount )			//< �� 3��, 5�� : ����ȭ�� ���Ѱ�
	__ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, ItemCode )
	__ARRAY_PROPERTY( BYTE, _MAX_CHECK_NUM, ItemNum )
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_CLASSITEM )
	__PROPERTY( BYTE, CharClassLimit )			//< ���ѵ� Ŭ������ �������� ������ �ȵ�
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_CHAO )
	RC::eQUEST_RESULT IsSatisfy( BYTE byChaoState );
	__PROPERTY( BYTE, ChaoState )				//< ī�� ����( �Ϲ�, ����ī��, ī�� )
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
BEGIN_DECL_QCONDITION_INFO(QUEST_CONDITION_CREATE_PORTAL)
    __PROPERTY(MAPCODE, MapPortalCode)      //< �ش� ������ ���� ���� ��Ż ���� �˻�
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
BEGIN_DECL_QCONDITION_INFO(QUEST_CONDITION_HONOR_RATING)
__PROPERTY(BYTE, HonorType)
__PROPERTY(WORD, HonorCode)
__PROPERTY(BYTE, HonorRating)
END_DECL_QCONDITION_INFO

BEGIN_DECL_QCONDITION_INFO(QUEST_CONDITION_DAILY_QUEST)
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
// �Ϸ� ���ǿ��� ����
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_MONEY )
	RC::eQUEST_RESULT IsSatisfy( DWORD64 CurMoney );
	__PROPERTY( DWORD64, NeedMoney )
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
// �Ϸ� ���ǿ��� ����
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_KILLMONSTER )
	enum { _MAX_CHECK_NUM=5, };
	__ARRAY_PROPERTY( DWORD, _MAX_CHECK_NUM, MonsterCode )
    __ARRAY_PROPERTY( BYTE, _MAX_CHECK_NUM, MonsterGrade )
    __ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, MonsterLevel )
	__ARRAY_PROPERTY( BYTE, _MAX_CHECK_NUM, MonsterNum )
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_MISSION )
	enum { _MAX_CHECK_NUM=2, };
	__ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, MissionCode )
	__ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, MissionNum )
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
// �Ϸ� ���ǿ��� ����
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_AREA )
    enum { _MAX_CHECK_NUM=3, };
    __ARRAY_PROPERTY( DWORD, _MAX_CHECK_NUM, AreaID )
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
// 20060711_PM12:19:10
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_ITEM_SPACE )
	enum { _MAX_CHECK_NUM=5, };
	__PROPERTY( BYTE, ItemArrayCount )					//< �� 3��, 5�� : ����ȭ�� ���Ѱ�
	__ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, ItemCode )
	__ARRAY_PROPERTY( BYTE, _MAX_CHECK_NUM, ItemNum )
END_DECL_QCONDITION_INFO
//������������������������������������������������������������������������������������������
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_MISSION_CLEAR)
    enum { _MAX_CHECK_NUM=2, };
    __ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, MissionCode )
    __ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, MissionNum )
END_DECL_QCONDITION_INFO
#endif //_NA_006607_20130402_ADD_QUESTTYPE
//������������������������������������������������������������������������������������������
//������������������������������������������������������������������������������������������
#ifdef _DEVTAIYO_QUEST_TRIGGER_WORK

// �Ϸ� ���ǿ��� ����
BEGIN_DECL_QCONDITION_INFO( QUEST_CONDITION_TRIGGER )
	enum { _MAX_CHECK_NUM=4, };
	__ARRAY_PROPERTY( DWORD, _MAX_CHECK_NUM, TriggerNCode )
END_DECL_QCONDITION_INFO
#endif 
//������������������������������������������������������������������������������������������
