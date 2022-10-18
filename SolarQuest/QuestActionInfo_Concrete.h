#ifndef __SOLARQUEST_QUESTACTIONINFO_CONCRETE_H
#define __SOLARQUEST_QUESTACTIONINFO_CONCRETE_H
#pragma once

#include "QuestActionInfo.h"



BEGIN_DECL_QACTION_INFO( QUEST_ACTION_ITEM )
	enum { _DEF_CHECK_NUM=3, _MAX_CHECK_NUM=5, };		//< �����ൿ ���������Ŵ� 3, �Ϸ��ൿ ���������Ŵ�5
	__BOOL_PROPERTY( Delete )							//< ������ ���� ����(TRUE) : �ο�(FALSE)
	__PROPERTY( BYTE, ItemArrayCount )					//< �����ϴ� ������ ����
	__ARRAY_PROPERTY( WORD, _MAX_CHECK_NUM, ItemCode )
	__ARRAY_PROPERTY( BYTE, _MAX_CHECK_NUM, ItemNum )
END_DECL_QACTION_INFO
//
//BEGIN_DECL_QACTION_INFO( QUEST_ACTION_TIMER )
//	__PROPERTY( DWORD, ExpiredTime );
//END_DECL_QACTION_INFO

BEGIN_DECL_QACTION_INFO( QUEST_ACTION_CHANGEofCLASS )	// Condition�� ���� ĳ���� ����, ��������� ���͸� �Ǿ��� ��...
	PROPERTY<eSPECIAL_QUEST_REWARD>		ChangeOfClassStep;			// x�� ���� (RewardInfo::eSPECIAL_REWARD)
END_DECL_QACTION_INFO

// ����Ʈ ������ ���� ��ȯ.
BEGIN_DECL_QACTION_INFO( QUEST_ACTION_SPAWN_MONSTER )
	__PROPERTY( DWORD, HideNpcCode )								//< ���ܾ��� NPC�ڵ�.
	__PROPERTY( DWORD, SummonMonsterCode )							//< ��ȯ�� ���� �ڵ�
END_DECL_QACTION_INFO

BEGIN_DECL_QACTION_INFO(QUEST_ACTION_SPAWN_MONSTER_ABANDON)
END_DECL_QACTION_INFO

// Create Portal (goto Mission) On Quest Accept
BEGIN_DECL_QACTION_INFO(QUEST_ACTION_CREATE_PORTAL)
    __PROPERTY(MAPCODE, MapPortalCode)  //< �ش� ������ ���� ���� ��Ż ����
END_DECL_QACTION_INFO

BEGIN_DECL_QACTION_INFO(QUEST_ACTION_CLOSE_PORTAL)
END_DECL_QACTION_INFO

// ����Ʈ �Ϸ�,����� ������ ä���� �ʱ�ȭ
BEGIN_DECL_QACTION_INFO(QUEST_ACTION_DELETE_COLLECTIONID)
END_DECL_QACTION_INFO

#ifdef _NA_003027_20111013_HONOR_SYSTEM
BEGIN_DECL_QACTION_INFO(QUEST_ACTION_DAILY_QUEST)
END_DECL_QACTION_INFO

BEGIN_DECL_QACTION_INFO(QUEST_ACTION_DAILY_QUEST_DELETE)
END_DECL_QACTION_INFO
#endif //_NA_003027_20111013_HONOR_SYSTEM

#endif