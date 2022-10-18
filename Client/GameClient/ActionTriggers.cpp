#include "SunClientPrecompiledHeader.h"

#include "actiontriggers.h"
#include "..\\..\\SolarTrigger\\actionInfo.h"
#include "TriggerManEx.h"
#include "Trigger.h"

#include "Application.h"
#include "ObjectManager.h"
#include "MapObject.h"
#include "Monster.h"
#include "EventInfoParser.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "BattleScene.h"
#include "Map.h"
#include "HeroActionInput.h"
#include "Message.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "MissionManager_Concrete.h"
#include "QuestManager_Concrete.h"
#include "uiHelpMan/uiHelpMan.h"
#include "NPCInfoParser.h"
#include "ResourceManager.h"
#include "Character.h"
#include "NPC.h"
#include "uiRadarMan/uiRadarMan.h"

void SearchObjectByType(BYTE object_type, 
                        WzID object_key_value,
                        std::list<Object*>& INOUT object_list)
{
    Object* current_object = NULL;

    switch(object_type)
    {
    case NPCCODE_TYPE:
        {
            // NPC 코드로 검색
            Object_Map_Itr object_manager_itr = g_ObjectManager.GetBegin();
            Object_Map_Itr object_manager_itr_end = g_ObjectManager.GetEnd();
            BASE_NPCINFO* npc_info = NULL;

            for ( ; object_manager_itr != object_manager_itr_end; ++object_manager_itr)
            {
                current_object = (object_manager_itr->second);
                npc_info = NULL;
                if (current_object == NULL)
                {
                    continue;
                }
                if (current_object->IsKindOfObject(MONSTER_OBJECT))
                {
                    Monster* monster_object = static_cast<Monster*>(current_object);
                    npc_info = monster_object->GetMonsterInfo();
                    if (npc_info == NULL)
                    {
                        continue;
                    }
                }
                else if (current_object->IsKindOfObject(NPC_OBJECT))
                {
                    NPC* npc_object = static_cast<NPC*>(current_object);
                    npc_info = npc_object->GetMonsterInfo();
                    if (npc_info == NULL)
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }

                // npc_info 정상
                if (npc_info->m_MonsterCode == object_key_value)
                {
                    object_list.push_back(current_object);
                }
            }
        }
        break;
    case UNITID_TYPE:
        {
            // UNIT_ID로 검색
            Object_Map_Itr object_manager_itr = g_ObjectManager.GetBegin();
            Object_Map_Itr object_manager_itr_end = g_ObjectManager.GetEnd();
            DWORD unitid_for_trigger = 0;

            for ( ; object_manager_itr != object_manager_itr_end; ++object_manager_itr)
            {
                current_object = (object_manager_itr->second);
                unitid_for_trigger = 0;
                if (current_object == NULL)
                {
                    continue;
                }
                if (current_object->IsKindOfObject(MONSTER_OBJECT))
                {
                    Monster* monster_object = static_cast<Monster*>(current_object);
                    unitid_for_trigger = monster_object->unitid_for_trigger();
                    if (unitid_for_trigger == 0)
                    {
                        continue;
                    }
                }
                else if (current_object->IsKindOfObject(NPC_OBJECT))
                {
                    NPC* npc_object = static_cast<NPC*>(current_object);
                    unitid_for_trigger = npc_object->unitid_for_trigger();
                    if (unitid_for_trigger == 0)
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }

                // unitid 체크
                if (unitid_for_trigger == object_key_value)
                {
                    object_list.push_back(current_object);
                    break;
                }
            }
        }
        break;
    case MAPNPC_TYPE:
        {
            // 맵오브젝트(에 연결된 설치형몬스터) 검색
            Object_Map_Itr object_manager_itr = g_ObjectManager.GetBegin();
            Object_Map_Itr object_manager_itr_end = g_ObjectManager.GetEnd();
            DWORD mapobject_key = 0;

            for ( ; object_manager_itr != object_manager_itr_end; ++object_manager_itr)
            {
                current_object = (object_manager_itr->second);
                mapobject_key = 0;
                if (current_object == NULL)
                {
                    continue;
                }
                if (current_object->IsKindOfObject(MONSTER_OBJECT))
                {
                    Monster* monster_object = static_cast<Monster*>(current_object);
                    mapobject_key = monster_object->GetMapObjectKey();
                    if (mapobject_key != object_key_value)
                    {
                        continue;
                    }
                }
                else
                {
                    continue;
                }

                object_list.push_back(current_object);
                break;
            }
        }
        break;
    }
}


__IMPL_POOL(ACTION_CHANGE_OBJECTANI)
VOID ACTION_CHANGE_OBJECTANI::OnAction()
{
	DWORD ObjectID = Get()->GetObjectID();
	Object * pObject = (Object *)g_ObjectManager.GetObject(MAP_OBJECT_KEY + ObjectID);

	if (pObject)
	{
		if (pObject->IsKindOfObject(MAP_OBJECT))
		{	
            BYTE Direction = Get()->GetDirection();

            BOOL bLoop = false;

            MapObject *pMapObject = (MapObject *)pObject;
            if( Direction == 2 )   // 정방향 루프
            {
                bLoop = true;
            }
            else if( Direction == 3 )   // 역방향 루프
            {
                bLoop = true;
            }
			pMapObject->SetTriggerAnimation(Get()->GetWzAniID(), bLoop, m_bDone);
			pMapObject->SetNextState( STATE::TRIGGER_STATE, g_CurTime);
		}
	}
	else
	{
		assert(0);
	}
}

__IMPL_POOL(ACTION_CHANGE_PATHTILE)
VOID ACTION_CHANGE_PATHTILE::OnAction()
{
	int iAtt    = Get()->GetAttribute();
	int iTileID = Get()->GetTileID();
	if (iAtt)
	{
		g_pMap->GetWorldBase()->x_pPathFinder->SetAttribute(iTileID, PTA_NO_WALK);
	}
	else
	{
		g_pMap->GetWorldBase()->x_pPathFinder->UnsetAttribute(iTileID, PTA_NO_WALK);
	}
}

__IMPL_POOL(ACTION_CHANGE_OBJECTSTATE)
VOID ACTION_CHANGE_OBJECTSTATE::OnAction()
{
	MapObject *pObject = (MapObject *)g_ObjectManager.GetObject( MAP_OBJECT_KEY + Get()->GetObjectID());
	if (pObject)
	{
		//나타남
		if (Get()->GetState() == 0)
		{
			pObject->SetShow(TRUE);
		}
		// 사라짐
		else
		{
			pObject->SetShow(FALSE);
		}							
	}
}

__IMPL_POOL(ACTION_PLAY_EVENT)
VOID ACTION_PLAY_EVENT::OnAction()
{
	int iEventID = Get()->GetEventID();
	if( g_pApplication && g_pApplication->IsFocus() )
	{
		(static_cast<BattleScene*>(GameFramework::GetCurrentScene()))->SetEventScript(iEventID);
	}
}

__IMPL_POOL(ACTION_SET_ISATTACK)
VOID ACTION_SET_ISATTACK::OnAction()
{
}

__IMPL_POOL(ACTION_PORTAL_PLAYER)
VOID ACTION_PORTAL_PLAYER::OnAction()
{
}

__IMPL_POOL(ACTION_SET_OBJECTTHRUST)
VOID ACTION_SET_OBJECTTHRUST::OnAction()
{
}


__IMPL_POOL(ACTION_APPLY_DAMAGE)
VOID ACTION_APPLY_DAMAGE::OnAction()
{
}

__IMPL_POOL(ACTION_REFLECT_DAMAGE)
VOID ACTION_REFLECT_DAMAGE::OnAction()
{
}

__IMPL_POOL(ACTION_CREATE_MONSTER)
VOID ACTION_CREATE_MONSTER::OnInit()
{
	DWORD dwNPCCode = Get()->GetMonsterCode();
	BASE_NPCINFO* pBNI = NPCInfoParser::Instance()->GetNPCInfo(dwNPCCode);
	if(pBNI)
	{
		ResourceManager::Instance()->PreLoadResource(pBNI->m_dwICode + ENGINE_RESOURCE_ID_OFFSET);
	}  
}

VOID ACTION_CREATE_MONSTER::OnAction()
{
}

__IMPL_POOL(ACTION_DISPLAY_MSGBOX)
VOID ACTION_DISPLAY_MSGBOX::OnAction()
{
}

__IMPL_POOL(ACTION_REMOVE_ITEM)
VOID ACTION_REMOVE_ITEM::OnAction()
{
}

__IMPL_POOL(ACTION_REWARD_PLAYER)
VOID ACTION_REWARD_PLAYER::OnAction()
{
}

__IMPL_POOL(ACTION_CLEAR_MISSION)
VOID ACTION_CLEAR_MISSION::OnAction()
{
	g_pQMGR_Real->Event( QUEST_EVENT_MISSION, NULL);
}

__IMPL_POOL(ACTION_PORTAL_RANDOM)
VOID ACTION_PORTAL_RANDOM::OnAction()
{
}

__IMPL_POOL(ACTION_ACTIVATE_TRIGGER)
VOID ACTION_ACTIVATE_TRIGGER::OnAction()
{	
	WzID wiTrgID  = this->Get()->GetWzTriggerID();
	BYTE btFlag = this->Get()->GetFlag();

	if(NULL == g_pMap)
		return;
	if(NULL == g_pMap->GetTriggerMan())
		return;

	int iCnt = g_pMap->GetTriggerMan()->m_vecTrgObj.size();
	for(int i = 0; i < iCnt; i++)
	{
		sTRIGGER_OBJECT* pTO = g_pMap->GetTriggerMan()->m_vecTrgObj[i];

		if(pTO && pTO->m_dwTriggerID == wiTrgID)
		{
			pTO->m_bActive = (!btFlag);
		}
	}
}

__IMPL_POOL(ACTION_AREA_DAMAGE)
VOID ACTION_AREA_DAMAGE::OnAction()
{
}

__IMPL_POOL(ACTION_OPERATE_SWITCH)
VOID ACTION_OPERATE_SWITCH::OnAction()
{
}

__IMPL_POOL( ACTION_ROAD_CONNECT )		// 길 연결을 설정/해제 한다
VOID ACTION_ROAD_CONNECT::OnAction()
{
    if (g_pMap == NULL)
    {
        return;
    }

    int area_index = Get()->GetAreaIndex();
    BYTE area_atribute = Get()->GetCanGo();      //갈 수 있다=0, 없다=1
    WzSpecialArea* special_area = g_pMap->GetWorldBase()->GetSpecialAreaByIndex(area_index);
    
    if (special_area->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX)
    {
        return;
    }

    int tile_index;
    int tile_count = special_area->m_wbvRegion.m_BvPathTileIndex.m_nCount;
    for (int data_index = 0; data_index < tile_count; ++data_index)
    {
        tile_index = special_area->m_wbvRegion.m_BvPathTileIndex.m_pdwData[data_index];
        if (area_atribute)
        {
            g_pMap->GetWorldBase()->x_pPathFinder->SetAttribute(tile_index, PTA_NO_WALK);
        }
        else
        {
            g_pMap->GetWorldBase()->x_pPathFinder->UnsetAttribute(tile_index, PTA_NO_WALK);
        }
    }
}

__IMPL_POOL(ACTION_CREATE_DIR_MONSTER)
VOID ACTION_CREATE_DIR_MONSTER::OnInit()
{
	DWORD dwNPCCode = Get()->GetMonsterCode();
	BASE_NPCINFO* pBNI = NPCInfoParser::Instance()->GetNPCInfo(dwNPCCode);
	if(pBNI)
	{
		ResourceManager::Instance()->PreLoadResource(pBNI->m_dwICode + ENGINE_RESOURCE_ID_OFFSET);
	}  
}

VOID ACTION_CREATE_DIR_MONSTER::OnAction()
{
}

__IMPL_POOL(ACTION_CHANGE_LIGHT)
VOID ACTION_CHANGE_LIGHT::OnAction()
{
	int  iLightIndex = Get()->GetLightIndex();
	int  iDelayTime  = Get()->GetDelayTime();
	BYTE iStatus     = Get()->GetStatus();

	if (g_pSunTerrain)
	{
		switch(iStatus)
		{
			// 끄기
		case 0:
			g_pSunRenderer->x_pManagerLight->TurnOnLight(iLightIndex,FALSE);
			break;
			// 켜기
		case 1:
			g_pSunRenderer->x_pManagerLight->TurnOnLight(iLightIndex,TRUE);	
			break;
			// 토글
		case 2:
			const St_LightInformation *pInfo = g_pSunRenderer->GetLightInfo(iLightIndex);

			if (pInfo) {
				if (pInfo->m_bValid)
				{
					g_pSunRenderer->x_pManagerLight->TurnOnLight( iLightIndex,FALSE);	
				}
				else
				{
					g_pSunRenderer->x_pManagerLight->TurnOnLight( iLightIndex,TRUE);	

				}
			}
			break;
		}
	}
}

__IMPL_POOL(ACTION_CHANGE_OBJECT_TYPE)
VOID ACTION_CHANGE_OBJECT_TYPE::OnAction()
{
	int iObjectID = Get()->GetObjectID();
	int iTime	  = Get()->GetDelayTime();
	int iType     = Get()->GetObjectType();

	Object *pObject = (Object *)g_ObjectManager.GetObject(iObjectID);
	if (pObject)			
	{
		if (pObject->IsKindOfObject(MAP_OBJECT))
		{
			MapObject *pMapObject = (MapObject *)pObject;

			pMapObject->SetDelayStateChange(iType, iTime);
		}
	}
}


__IMPL_POOL( ACTION_RANDOM_MAP_MOVE )
VOID ACTION_RANDOM_MAP_MOVE::OnAction()
{
}



__IMPL_POOL( ACTION_QUEST_SATISFACTION )
VOID ACTION_QUEST_SATISFACTION::OnAction()
{
}

__IMPL_POOL( ACTION_QUEST_GRANT )
VOID ACTION_QUEST_GRANT::OnAction()
{
}

__IMPL_POOL( ACTION_QUEST_SETUP )
VOID ACTION_QUEST_SETUP::OnAction()
{
}

__IMPL_POOL( ACTION_USE_CATEGORY )
VOID ACTION_USE_CATEGORY::OnAction()
{
}

// 기존에 존재하는 액션 트리거
//__IMPL_POOL( ACTION_CREATE_MONSTER )

__IMPL_POOL( ACTION_CREATE_MONSTER_GROUP )
VOID ACTION_CREATE_MONSTER_GROUP::OnAction()
{
}

__IMPL_POOL( ACTION_SHOW_OR_HIDE_UNIT )
VOID ACTION_SHOW_OR_HIDE_UNIT::OnAction()
{
}


__IMPL_POOL( ACTION_SHOW_GUIDE_MSGBOX )
VOID ACTION_SHOW_GUIDE_MSGBOX::OnAction()
{
	if (m_bDone)
	{
		return;
	}

	int iHelpIndex = Get()->GetHelpIndex();

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
	uiHelpMan* helpMan =
		static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

	if (helpMan)
	{
		helpMan->ShowHelpAnnounce(iHelpIndex,TRUE);
	}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW
}

__IMPL_POOL( ACTION_SHOW_OR_HIDE_MSGBOX )
VOID ACTION_SHOW_OR_HIDE_MSGBOX::OnAction()
{
	int  TextID    = Get()->GetTextID();
	BYTE byIsParty = Get()->GetIsParty();
	int  iDisplayWindow = Get()->GetDisplayWindow();
	int  iIsShow = Get()->GetIsShow();

	GlobalFunc::DisplayMessageForTrigger(m_bDone, 
	                                     TextID, 
	                                     byIsParty, 
	                                     iDisplayWindow, 
	                                     iIsShow, 
                                         0);
}

__IMPL_POOL( ACTION_RANDOM_AREA_MOVE )
VOID ACTION_RANDOM_AREA_MOVE::OnAction()
{
}

// 몬스터 랜덤 스폰
__IMPL_POOL( ACTION_RANDOM_MONSTER_SPAWN )
VOID ACTION_RANDOM_MONSTER_SPAWN::OnInit()
{
	DWORD dwNPCCode = Get()->GetMonsterID_1();
	BASE_NPCINFO* pBNI = NPCInfoParser::Instance()->GetNPCInfo(dwNPCCode);
	if(pBNI)
	{
		ResourceManager::Instance()->PreLoadResource(pBNI->m_dwICode + ENGINE_RESOURCE_ID_OFFSET);
	}

	dwNPCCode = Get()->GetMonsterID_2();
	pBNI = NPCInfoParser::Instance()->GetNPCInfo(dwNPCCode);
	if(pBNI)
	{
		ResourceManager::Instance()->PreLoadResource(pBNI->m_dwICode + ENGINE_RESOURCE_ID_OFFSET);
	}

	dwNPCCode = Get()->GetMonsterID_3();
	pBNI = NPCInfoParser::Instance()->GetNPCInfo(dwNPCCode);
	if(pBNI)
	{
		ResourceManager::Instance()->PreLoadResource(pBNI->m_dwICode + ENGINE_RESOURCE_ID_OFFSET);
	}

	dwNPCCode = Get()->GetMonsterID_4();
	pBNI = NPCInfoParser::Instance()->GetNPCInfo(dwNPCCode);
	if(pBNI)
	{
		ResourceManager::Instance()->PreLoadResource(pBNI->m_dwICode + ENGINE_RESOURCE_ID_OFFSET);
	}

	dwNPCCode = Get()->GetMonsterID_5();
	pBNI = NPCInfoParser::Instance()->GetNPCInfo(dwNPCCode);
	if(pBNI)
	{
		ResourceManager::Instance()->PreLoadResource(pBNI->m_dwICode + ENGINE_RESOURCE_ID_OFFSET);
	}
}

VOID ACTION_RANDOM_MONSTER_SPAWN::OnAction()
{
}


__IMPL_POOL( ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR )
VOID ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR::OnAction()
{
}
//////////////////////////////////////////////////////////////////////////
	__IMPL_POOL( ACTION_REMOVE_MONSTER )
	VOID ACTION_REMOVE_MONSTER::OnAction()
	{
	}

	__IMPL_POOL( ACTION_ADD_EXP )
	VOID ACTION_ADD_EXP::OnAction()
	{
	}
//////////////////////////////////////////////////////////////////////////
	__IMPL_POOL( ACTION_MONSTER_STAT_UP_OR_DOWN )
		VOID ACTION_MONSTER_STAT_UP_OR_DOWN::OnAction()
	{
	}

	__IMPL_POOL( ACTION_RANDUM_TRIGER_ON_OR_OFF )
		VOID ACTION_RANDUM_TRIGER_ON_OR_OFF::OnAction()
	{
	}

//////////////////////////////////////////////////////////////////////////

	__IMPL_POOL( ACTION_PLAY_MUSIC )
		VOID ACTION_PLAY_MUSIC::OnAction()
	{
	}

//////////////////////////////////////////////////////////////////////////
	__IMPL_POOL( ACTION_DOWN_OBJETC_HP )
		VOID ACTION_DOWN_OBJETC_HP::OnAction()
	{
	}

	__IMPL_POOL( ACTION_CHANGE_ATTACK_KIND )
		VOID ACTION_CHANGE_ATTACK_KIND::OnAction()
	{
		DWORD ObjectKey = Get()->GetdwObjKey();
		Object* pObj = g_ObjectManager.GetObject(MAP_OBJECT_KEY + ObjectKey);
		if(pObj && pObj->IsKindOfObject(MAP_OBJECT))
		{
			MapObject* pMapObj = (MapObject*)pObj;
			bool bVal = true;
			if(0 == Get()->GetbtPossibility())
				bVal = false;
			else if(1 == Get()->GetbtPossibility())
				bVal = true;

			pMapObj->SetbCanHit(bVal);
		}
	}

	__IMPL_POOL( ACTION_ACTIVE_STAMP )
		VOID ACTION_ACTIVE_STAMP::OnAction()
	{
	}

__IMPL_POOL(ACTION_ATTACK_OBJECTKIND)
VOID	ACTION_ATTACK_OBJECTKIND::OnAction()
{
}

__IMPL_POOL(ACTION_ACTIVE_SKILL_OBJECTKIND)
VOID	ACTION_ACTIVE_SKILL_OBJECTKIND::OnAction()
{
}

__IMPL_POOL(ACTION_SHOW_MESSAGEBOX_OBJECTKIND)
VOID	ACTION_SHOW_MESSAGEBOX_OBJECTKIND::OnAction()
{
}

__IMPL_POOL(ACTION_SET_ATTACK_STATE_OBJECTKIND)
VOID	ACTION_SET_ATTACK_STATE_OBJECTKIND::OnAction()
{
}

__IMPL_POOL(ACTION_DESTROY_OBJECTKIND)
VOID	ACTION_DESTROY_OBJECTKIND::OnAction()
{
}

//  [5/5/2010 iforall]
__IMPL_POOL(ACTION_CREATE_MONSTER_LINKED_MAPOBJECT)
VOID ACTION_CREATE_MONSTER_LINKED_MAPOBJECT::OnAction()
{
}

__IMPL_POOL(ACTION_CONTROL_TIMER)
VOID ACTION_CONTROL_TIMER::OnAction()
{
}

__IMPL_POOL( ACTION_DO_COMMAND )
VOID ACTION_DO_COMMAND::OnAction()
{
}

__IMPL_POOL( ACTION_MOVE_CONTROL )
VOID ACTION_MOVE_CONTROL::OnAction()
{
}

__IMPL_POOL( ACTION_ANIMATION_CONTROL )
VOID ACTION_ANIMATION_CONTROL::OnAction()
{
    // 오브젝트 타입에 따른 객체 찾기
    std::list<Object*> object_list;         // 검색한 Character객체의 포인터를 담아줄 컨테이너

    BYTE object_type = Get()->GetbyObjectType();
    WzID object_key_value = Get()->GetdwObjectKey();

    SearchObjectByType(object_type, object_key_value, object_list);
    
    if (object_list.empty() == true)
    {
        return;
    }

    // 오브젝트 리스트에 트리거 애니 제어 정보 세팅 및 애니메이션 플레이(액션/상태전환)
    std::list<Object*>::iterator object_list_itr = object_list.begin();
    std::list<Object*>::iterator object_list_itr_end = object_list.end();
    Character* character_object = NULL;
    PLAYER_ACTION action;
    action.ActionID = ACTION_TRIGGER_ANIMATION_CONTROL;
    action.TRIGGERANIMATIONCONTROL.start_animation_id = Get()->GetdwAnimationCode1();
    action.TRIGGERANIMATIONCONTROL.start_animation_loop = Get()->GetbyLoop1();
    action.TRIGGERANIMATIONCONTROL.next_animation_id = Get()->GetdwAnimationCode2();
    action.TRIGGERANIMATIONCONTROL.next_animation_loop = Get()->GetbyLoop2();

    for ( ; object_list_itr != object_list_itr_end; ++object_list_itr)
    {
        character_object = static_cast<Character*>(*object_list_itr);
        if (character_object == NULL)
        {
            continue;
        }

        character_object->PutAction(&action);
        character_object->DoNextAction();
    }
}

__IMPL_POOL( ACTION_NPC_APPLY_DAMAGE )
VOID ACTION_NPC_APPLY_DAMAGE::OnAction()
{
}

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
//------------------------------------------------------------------------------
__IMPL_POOL( ACTION_COMPLETE_COLLECTION )
VOID ACTION_COMPLETE_COLLECTION::OnAction()
{

}

__IMPL_POOL( ACTION_APPLY_HPMPSD )
VOID ACTION_APPLY_HPMPSD::OnAction()
{

}

__IMPL_POOL( ACTION_CHANGE_OBJECT_HP )
VOID ACTION_CHANGE_OBJECT_HP::OnAction()
{

}


__IMPL_POOL( ACTION_CHANGE_RADAR_MAP )
VOID ACTION_CHANGE_RADAR_MAP::OnAction()
{
}


__IMPL_POOL( ACTION_ATTACH_STATE_RANGE )
VOID ACTION_ATTACH_STATE_RANGE::OnAction()
{
}

__IMPL_POOL( ACTION_CHANGE_PLAYER_HP )
VOID ACTION_CHANGE_PLAYER_HP::OnAction()
{

}
