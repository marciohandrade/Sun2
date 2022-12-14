#pragma once

#include <QuestAction.h>
#include <QuestActionInfo_Concrete.h>


BEGIN_DECL_QACTION( QUEST_ACTION_ITEM )
__DECL_CUSTOMPOOL_PTR( QUEST_ACTION_ITEM )
	VOID DoAccept( VOID * pvParam );
	VOID DoComplete( VOID * pvParam );
	__BOOL_PROPERTY( Delete )					//< 아이템 삭제 여부(TRUE) : 부여(FALSE)
	__ARRAY_PROPERTY( WORD, 3, ItemCode )
	__ARRAY_PROPERTY( BYTE, 3, ItemNum )
END_DECL_QACTION

BEGIN_DECL_QACTION( QUEST_ACTION_CHANGEofCLASS )
	VOID	DoAccept( VOID* pvParam );
	VOID	DoComplete( VOID* pvParam );
END_DECL_QACTION

// 퀘스트 수락시 몬스터 소환.
BEGIN_DECL_QACTION( QUEST_ACTION_SPAWN_MONSTER )
	VOID	DoAccept( VOID* pvParam );
	VOID	DoComplete( VOID* pvParam );
END_DECL_QACTION

BEGIN_DECL_QACTION(QUEST_ACTION_SPAWN_MONSTER_ABANDON)
    VOID    DoComplete(VOID* pvParam);
END_DECL_QACTION
//
//__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
BEGIN_DECL_QACTION(QUEST_ACTION_CREATE_PORTAL)
    VOID    DoAccept(VOID* pvParam);
    VOID    DoComplete(VOID* pvParam);
END_DECL_QACTION
BEGIN_DECL_QACTION(QUEST_ACTION_CLOSE_PORTAL)
    VOID    DoComplete(VOID* pvParam);
END_DECL_QACTION

// 퀘스트 완료,포기시 진행중 채집물 초기화
BEGIN_DECL_QACTION(QUEST_ACTION_DELETE_COLLECTIONID)
VOID    DoComplete(VOID* pvParam);
END_DECL_QACTION

#ifdef _NA_003027_20111013_HONOR_SYSTEM
BEGIN_DECL_QACTION(QUEST_ACTION_DAILY_QUEST)
    VOID    DoAccept(VOID* pvParam);
    VOID    DoComplete(VOID* pvParam);
END_DECL_QACTION
BEGIN_DECL_QACTION(QUEST_ACTION_DAILY_QUEST_DELETE)
    VOID    DoComplete(VOID* pvParam);
END_DECL_QACTION
#endif //_NA_003027_20111013_HONOR_SYSTEM
