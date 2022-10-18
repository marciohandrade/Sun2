#include "SunClientPrecompiledHeader.h"

#include "conditiontriggers.h"

#include "TriggerManEx.h"
#include "..\\..\\SolarTrigger\\TriggerMsg.h"

#include "Trigger.h"

#include "MapObject.h"
#include "ObjectManager.h"
#include "Map.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "HeroActionInput.h"

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_ALWAYS)
VOID CONDITION_ALWAYS::OnInit()
{
	setSatisfied( TRUE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_COMPARE_SWITCH)
VOID CONDITION_COMPARE_SWITCH::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_CLICK_OBJECT)
VOID CONDITION_CLICK_OBJECT::OnInit()
{
	setSatisfied( FALSE );

	DWORD dwObjectKey = Get()->GetObjectID();
	MapObject * pMapObject = (MapObject * )g_ObjectManager.GetObject(MAP_OBJECT_KEY + dwObjectKey);

	if (!pMapObject )
	{
		return;
	}

	pMapObject->SetTriggerID(dwObjectKey);
	pMapObject->SetTriggerObj(TRUE);

	pMapObject->PushBackRealTriggerID(this->getParent()->TriggerID());

	sTRIGGER_OBJECT* sTO = new sTRIGGER_OBJECT;
	sTO->m_dwTriggerID = this->getParent()->TriggerID();
	sTO->m_dwObjectKey = dwObjectKey;
	if(FALSE == this->getParent()->IsLoop()) { sTO->m_bLoop = FALSE;} else { sTO->m_bLoop = TRUE; }
	if(FALSE == this->getParent()->IsActive()) { sTO->m_bActive	= FALSE;} else { sTO->m_bActive	= TRUE; }
	this->getMGR()->m_vecTrgObj.push_back(sTO);

}


//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_DAMAGE_OBJECT)
VOID CONDITION_DAMAGE_OBJECT::OnInit()
{
	setSatisfied( FALSE );

	DWORD ObjectID = Get()->GetObjectID();
	MapObject *pMapObject = (MapObject *)g_ObjectManager.GetObject( MAP_OBJECT_KEY +  ObjectID);
	if (!pMapObject)
	{
		return;
	}

	pMapObject->SetShow(FALSE);
	pMapObject->SetTriggerObj(TRUE);
}


//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_COUNT_NPCKILL)
VOID CONDITION_COUNT_NPCKILL::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_PASS_TIME)
VOID CONDITION_PASS_TIME::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_ENTER_AREA)
VOID CONDITION_ENTER_AREA::OnInit()
{
	setSatisfied( FALSE );
}

 VOID CONDITION_ENTER_AREA::onMsg( TRIGGER_MSG * pmsg )
 {
	 if ( !g_pHero )
	 {
		 return;
	 }

	 DWORD AreaID = Get()->GetAreaID();

	 BOOL bVal = CheckHeroInArea(AreaID);
	 if(bVal)
		 setSatisfied( TRUE );
	 else
		 setSatisfied(FALSE);
 }

BOOL CONDITION_ENTER_AREA::CheckHeroInArea(DWORD areaID)
{ 
    if (g_pMap == NULL)
    {
        return FALSE;
    }

    if (g_pHero == NULL)
    {
        return FALSE;
    }

    eTRIGGER_STATE TrigerState, HeroTrigerState;
    WzSpecialArea* pAreaInfo = g_pMap->GetMapAreaByIndex(areaID);
    if (pAreaInfo == NULL)
    {
        return FALSE;
    }

    TrigerState = TRIGGER_STATE_LEAVE_AREA;
    switch (pAreaInfo->m_wbvRegion.m_eBvType)
    {
    case BVT_AABB:
        {
            if (TRUE == Contain_AABB_Vertex(&pAreaInfo->m_wbvRegion.m_aabb, g_pHero->GetPosition()))			
            {
                TrigerState = TRIGGER_STATE_ENTER_AREA;			
            }
        }
        break;
    case BVT_PATH_TILE_INDEX:
        {
            int iTile = g_pHero->GetPathExplorer()->GetTile();
            for (int i = 0; i < static_cast<int>(pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_nCount); ++i)
            {
                if (iTile == pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData[i])
                {
                    TrigerState = TRIGGER_STATE_ENTER_AREA;
                    break;
                }
            }
        }
        break;
    case BVT_SPHERE:
        {
            if (TRUE == Contain_Sphere_Vertex(&pAreaInfo->m_wbvRegion.m_BvSphere, g_pHero->GetPosition()))
            {
                TrigerState = TRIGGER_STATE_ENTER_AREA;			
            }
        }
        break;
    default:
        {
            return FALSE;
        }
    }

    HeroTrigerState = g_pHero->GetTriggerState(areaID);
    if( HeroTrigerState != TrigerState )
    {
        // 들어갔을땐 언제나 패킷을 보낸다!!!
        if( TrigerState == TRIGGER_STATE_ENTER_AREA )
        {
            //< 플레이어가 들어왔다.
            //	패킷 보내기
            MSG_CG_TRIGGER_ENTER_AREA_SYN	SendPacket;
            SendPacket.m_byCategory		= CG_TRIGGER;
            SendPacket.m_byProtocol		= CG_TRIGGER_ENTER_AREA_SYN;
            SendPacket.m_iAreaID		= areaID;

#ifdef _DEBUG
            TCHAR szMessage[INTERFACE_STRING_LENGTH];
            Sprintf(szMessage, _T("EnterArea( %d ), AreaType( %d )\n"), areaID, pAreaInfo->m_wbvRegion.m_eBvType);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_OUPUT_DEBUG, szMessage);
#endif

            if( FALSE == GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(MSG_CG_TRIGGER_ENTER_AREA_SYN) ) )
            {
                //	MSG_CG_TRIGGER_ENTER_AREA_SYN - BattleScene
                WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
                GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("Failed Send MSG_CG_TRIGGER_ENTER_AREA_SYN - BattleScene"));
            }
        }	
        else if ( TrigerState == TRIGGER_STATE_LEAVE_AREA )
        {
            // 나왔을땐 이전에 들어갔을 경우에만 보낸다.
            if (HeroTrigerState == TRIGGER_STATE_ENTER_AREA)
            {
                MSG_CG_TRIGGER_LEAVE_AREA_SYN	SendPacket;
                SendPacket.m_byCategory		= CG_TRIGGER;
                SendPacket.m_byProtocol		= CG_TRIGGER_LEAVE_AREA_SYN;
                SendPacket.m_iAreaID		= areaID;

#ifdef _DEBUG
                TCHAR szMessage[INTERFACE_STRING_LENGTH];
                Sprintf(szMessage, _T("LeaveArea( %d ), AreaType( %d )\n"), areaID, pAreaInfo->m_wbvRegion.m_eBvType);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_OUPUT_DEBUG, szMessage);
#endif

                if( FALSE == GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(MSG_CG_TRIGGER_LEAVE_AREA_SYN) ) )
                {
                    WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
                    GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("Failed Send MSG_CG_TRIGGER_LEAVE_AREA_SYN - BattleScene"));
                }
            }
        }
        g_pHero->SetTriggerState( areaID,TrigerState );
    }

    if (TrigerState == TRIGGER_STATE_ENTER_AREA)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_HAVE_ITEM)
VOID CONDITION_HAVE_ITEM::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_MULTI_SWITCH)
VOID CONDITION_MULTI_SWITCH::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_MISSION_POINT)
VOID CONDITION_MISSION_POINT::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_COMPARE_VARIABLE)
VOID CONDITION_COMPARE_VARIABLE::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_QUEST_PROGRESS)
VOID CONDITION_QUEST_PROGRESS::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_CHECK_CLASS_ID)
VOID CONDITION_CHECK_CLASS_ID::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_NPCKILL_FOR_UNITID)
VOID CONDITION_NPCKILL_FOR_UNITID::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////

__IMPL_POOL(CONDITION_LIVE_PARTY_MEMBER)
VOID CONDITION_LIVE_PARTY_MEMBER::OnInit()
{
	setSatisfied( FALSE );
}

//////////////////////////////////////////////////////////////////////////
__IMPL_POOL(CONDITION_CHECK_OBJECT_HP)
VOID	CONDITION_CHECK_OBJECT_HP::OnInit()
{
	setSatisfied(FALSE);
}

__IMPL_POOL(CONDITION_DESTROY_OBJECT)
VOID	CONDITION_DESTROY_OBJECT::OnInit()
{
	setSatisfied(FALSE);
}

__IMPL_POOL(CONDITION_CHECK_OBJECTKIND_HP)
VOID	CONDITION_CHECK_OBJECTKIND_HP::OnInit()
{
    setSatisfied(FALSE);
}

__IMPL_POOL(CONDITION_NPC_ENTER_AREA)
VOID	CONDITION_NPC_ENTER_AREA::OnInit()
{
    setSatisfied(FALSE);
}

//------------------------------------------------------------------------------
__IMPL_POOL(CONDITION_COMPLETE_COLLECTION)
VOID CONDITION_COMPLETE_COLLECTION::OnInit()
{
    setSatisfied(FALSE);
}

//------------------------------------------------------------------------------
__IMPL_POOL(CONDITION_SERVER_EVENT)
VOID CONDITION_SERVER_EVENT::OnInit()
{
    setSatisfied(FALSE);
}
//------------------------------------------------------------------------------
__IMPL_POOL(CONDITION_CHECK_TEAM)
VOID CONDITION_CHECK_TEAM::OnInit()
{
    setSatisfied(FALSE);
}
//------------------------------------------------------------------------------
__IMPL_POOL(CONDITION_COMPARE_SWITCH_EX)
VOID CONDITION_COMPARE_SWITCH_EX::OnInit()
{
    setSatisfied(FALSE);
}
//------------------------------------------------------------------------------
__IMPL_POOL(CONDITION_MONSTER_ENTER_AREA)
VOID CONDITION_MONSTER_ENTER_AREA::OnInit()
{
    setSatisfied(FALSE);
}
