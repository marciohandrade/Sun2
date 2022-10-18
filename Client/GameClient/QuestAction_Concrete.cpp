#include "SunClientPrecompiledHeader.h"

#include "QuestAction_Concrete.h"
#include "Quest.h"
#include "ItemManager.h"

__IMPL_CUSTOMPOOL_PTR( QUEST_ACTION_ITEM )
void QUEST_ACTION_ITEM::DoAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;

	GetInfo()->GetItemCodePtr();
	GetInfo()->GetItemNumPtr();


	if( GetInfo()->IsDelete() )
	{
	}
	else
	{
	}
}
void QUEST_ACTION_ITEM::DoComplete( void * pvParam )
{
	DoAccept(pvParam);
}
