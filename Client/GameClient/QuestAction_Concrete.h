#pragma once

#include <QuestAction.h>
#include <QuestActionInfo_Concrete.h>

BEGIN_DECL_QACTION( QUEST_ACTION_ITEM )
__DECL_CUSTOMPOOL_PTR( QUEST_ACTION_ITEM )
	void DoAccept( void * pvParam );
	void DoComplete( void * pvParam );
	__BOOL_PROPERTY( Delete )					//< ������ ���� ����(TRUE) : �ο�(FALSE)
	__ARRAY_PROPERTY( WORD, 3, ItemCode )
	__ARRAY_PROPERTY( BYTE, 3, ItemNum )
END_DECL_QACTION
