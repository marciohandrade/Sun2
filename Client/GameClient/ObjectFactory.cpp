/*!
 * \file	ObjectFactory.cpp
 * \brief	타입에 따라 해당 오브젝트를 생성
 * \bug		
 * \todo	
 * \author	김경표 (lammy@webzen.co.kr)
 * \version	2008/12/30	김경표 - doxygen command 작성
 * \version 2005.06.07 소스 정리(지완)
 */

#include "SunClientPrecompiledHeader.h"
#include "objectfactory.h"
#include "Player.h"
#include "Monster.h"
#include "NPC.h"
#include "Item.h"
#include "MapObject.h"
#include "TerrainObject.h"
#include "CinematicCamera.h"
#include "PlayerMonster.h"
#include "Pet.h"

#include "LottoNPC.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "StatuePlayer.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

//------------------------------------------------------------------------------
/**
*/
ObjectFactory::ObjectFactory(void)
{
}

//------------------------------------------------------------------------------
/**
*/
ObjectFactory::~ObjectFactory(void)
{
}

//------------------------------------------------------------------------------
/**
	 TODO : 이놈들 다 POOL로 대체해야한다.!!
*/
Object* ObjectFactory::MakeNewObject(eOBJECT_TYPE objType,OBJECT_SUB_TYPE subType)
{
	Object* pObj = NULL;

	switch(objType)
	{
	case PLAYER_OBJECT:
		pObj = new Player;
		break;

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	case STATUE_PLAYER_OBJECT:
		pObj = new StatuePlayer;
		break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

	case MONSTER_OBJECT:
		pObj = new Monster;
		break;


	case SSQMONSTER_OBJECT:
		pObj = new PlayerMonster;
		break;

	
	case NPC_OBJECT:
		pObj = new NPC;

		break;
	case LOTTO_NPC_OBJECT:
		pObj = new LottoNPC;
		break;

	case PET_OBJECT:
		pObj = new Pet;
		break;

	case ITEM_OBJECT:
		{
			pObj = new Item;

			Item * pItem = (Item *)pObj;
			if(pItem)
			{
				pItem->SetDisplayTime( GAMEOPTION->GetNotifyDropItem() );
			}
		}
		break;

	case MAP_OBJECT:
		{
			if(subType==OBJECT_SUB_TYPE_TERRAIN)
			{
				pObj = new TerrainObject;
			}
			else
			{
				pObj = new MapObject;
			}
		}
		break;

	case CAMERA_OBJECT:
		pObj = new CinematicCamera;
		break;
	}

	return pObj;
}