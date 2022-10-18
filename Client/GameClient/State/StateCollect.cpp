//------------------------------------------------------------------------------
//  StateSit.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateCollect.h"
#include "hero.h"
#include "player.h"
#include "HeroActionInput.h"
#include "InterfaceManager.h"
#include "npc.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "GlobalFunc.h"
#include "MapObject.h"
#include "InterfaceManager.h"
#include "CastDialog.h"
#include "CollectionParser.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemFunction.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

bool StateCollect::is_send_action_syn_ = false;

//------------------------------------------------------------------------------
/**
*/
StateCollect::StateCollect(void)
{
	m_bSuccess = false;
	m_bMouseCancel = false;
	m_bStampMode = false;
}

//------------------------------------------------------------------------------
/**
*/
StateCollect::~StateCollect(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateCollect::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case STATE::MOVE:
    case STATE::KEYBOARDMOVE:
        {
            return FALSE;
        }
		break;
	}
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateCollect::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));
	
	MapObject* pMapObject = (MapObject*)g_ObjectManager.GetObject(pPlayer->GetCollectObjKey());
	if(NULL == pMapObject)
	{
		assert(!"StateCollect::OnStart(NULL == pMapObject)");
		return STATECHANGE::FAILED;
	}

	if(0 == pPlayer->GetCollectID())
	{
		assert(!"0 == pPlayer->GetCollectID()");
		return STATECHANGE::FAILED;
	}

	const CollectionInfo* pCI = CollectionParser::Instance()->FindCollectionInfo(pPlayer->GetCollectID());
	if(NULL == pCI)
	{
		assert(!"StateCollect::OnStart(NULL == pCI)");
		return STATECHANGE::FAILED;
	}

	for(int i = 0; i < CollectionInfo::MAX_ANI; i++)
	{
		m_wiAniCode[i] = pCI->m_wzidAnimation[i];
	}
	pPlayer->SetCollectMaxTime(pCI->m_dwCollectingTime);
	pPlayer->SetCollectState((BYTE)eCollectState_Start);
	m_wiSucEffectCode = pCI->m_wzidEffect[CollectionInfo::SUCCESS];
	m_wiStartEffectCode = pCI->m_wzidEffect[CollectionInfo::COLLECTING];

	if(pPlayer==g_pHero)
	{
		if ((CollectionInfo::OBJ_COLLECT == pCI->m_Condition[CollectionInfo::STAMP])
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            || (CollectionInfo::OBJ_BATTLE_GROUND_STRONGPOINT == pCI->m_Condition[CollectionInfo::STAMP])
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
            || (CollectionInfo::OBJ_ONEDYSGUILDMISSION_STAMP == pCI->m_Condition[CollectionInfo::STAMP])
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
			)
		{
			CastDialog * pCastDlg =	(CastDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING);

			if( pCastDlg && (FALSE == pCastDlg->IsVisibleWindow()) )
			{
                TCHAR cast_string[INTERFACE_STRING_LENGTH] = {0,};

                if (CollectionInfo::OBJ_COLLECT == pCI->m_Condition[CollectionInfo::STAMP])
                {
                    g_InterfaceManager.GetInterfaceString(5839, cast_string);
                }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                else if (CollectionInfo::OBJ_BATTLE_GROUND_STRONGPOINT == pCI->m_Condition[CollectionInfo::STAMP])
                {
                    g_InterfaceManager.GetInterfaceString(5840, cast_string);
                }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
                else if (CollectionInfo::OBJ_ONEDYSGUILDMISSION_STAMP == pCI->m_Condition[CollectionInfo::STAMP])
                {
                    cast_string[0] = '\0';
                }
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
                
                pCastDlg->SetText(cast_string);
				pCastDlg->SetGage(0.f);
				pCastDlg->ShowWindow(TRUE);

				keyMsg msg;
				ZeroMemory(&msg,sizeof(msg));
				msg.dwType=eDoType_cast_cancel;
				msg.DoSomething=GlobalFunc::DoCastCancel;
				g_KeyQueueManager.PushBack(msg);
			}
		}
	}

	m_bSuccess = false;
	m_bMouseCancel = false;

	m_bStampMode = false;
    const DWORD collect_condition = pCI->m_Condition[CollectionInfo::STAMP];
	if((collect_condition == CollectionInfo::OBJ_COLLECT)
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        || (collect_condition == CollectionInfo::OBJ_BATTLE_GROUND_STRONGPOINT)
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        || (collect_condition == CollectionInfo::OBJ_ONEDYSGUILDMISSION_STAMP)
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        )
    {
		m_bStampMode = false;
    }
	else if((collect_condition == CollectionInfo::OBJ_STAMP) ||
            (collect_condition == CollectionInfo::OBJ_STAMP_USE_GUILD_MASTER)
#ifdef _NA_008334_20150608_SONNENSCHEIN
            || (collect_condition == CollectionInfo::OBJ_SONNENSCHEIN_STAMP)
#endif //_NA_008334_20150608_SONNENSCHEIN
            
            )
    {
		m_bStampMode = true;
        pPlayer->CreateDominationStampEffect();
    }

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ChaosSystem::Function::CreateBattleGroundCollectEffect(pPlayer, pMapObject);
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateCollect::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));

	const CollectionInfo* pCI = CollectionParser::Instance()->FindCollectionInfo(pPlayer->GetCollectID());
	if(NULL == pCI)
	{
		return STATEPROCESS::END;
	}

	if(pPlayer->GetObjectKey() == g_pHero->GetObjectKey() &&
	   true == m_bStampMode &&
	   eCollectState_Max != (eCollectState)pPlayer->GetCollectState() &&
	   false == m_bMouseCancel
	  )
	{
		if((FALSE == g_Input.GetState(MOUSE_LBUTTON, KS_MOUSE_KEY)) ||
		   (g_Input.GetState(MOUSE_LBUTTON, KS_MOUSE_NONE)) ||
		   (g_Input.GetState(MOUSE_LBUTTON, KS_MOUSE_UP))   ||
		   (g_Input.GetState(MOUSE_LBUTTON, KS_MOUSE_DOWN))
		  )
		{
            m_bMouseCancel = true;

            TCHAR* debug_message = NULL;
#ifdef _DEV_VER
            debug_message = _T("MSG_CG_COLLECT_CANCEL_SYN[LBTN UP]");
#endif 
            StateCollect::SendCollectCancelSyn(debug_message);

            return STATEPROCESS::END;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 캐스팅 DLG
	float fRate =  (float)pPlayer->GetCollectStartTime() / (float)pPlayer->GetCollectMaxTime();

	if (fRate < 0.0f)
	{
		fRate = 0.0f;
	}
	else if (fRate > 1.0f)
	{
		fRate = 1.0f;
	}

	if(pPlayer == g_pHero)
	{
		if ((CollectionInfo::OBJ_COLLECT == pCI->m_Condition[CollectionInfo::STAMP])
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        || (CollectionInfo::OBJ_BATTLE_GROUND_STRONGPOINT == pCI->m_Condition[CollectionInfo::STAMP])
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        || (CollectionInfo::OBJ_ONEDYSGUILDMISSION_STAMP == pCI->m_Condition[CollectionInfo::STAMP])
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        )
		{
			CastDialog * pCastDlg =	(CastDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING);
			if(pCastDlg)
				pCastDlg->SetGage(fRate*100.0f);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//	에니메이션 업데이트
	pPlayer->SetCollectStartTime(pPlayer->GetCollectStartTime() + dwTick);

    pPlayer->ProcessInterpolation(dwTick);
	float fRet = pPlayer->ProcessAnimation(dwTick);

	if(eCollectState_Start == (eCollectState)pPlayer->GetCollectState())
	{
		pPlayer->SetAnimation(m_wiAniCode[eCollectState_Start], FALSE);
		pPlayer->SetCollectState((BYTE)eCollectState_Loop);

		if(false == m_bStampMode)
		{
			pPlayer->SetCollectStartEftHndl(GlobalFunc::CreateEffectBeUpToOption(m_wiStartEffectCode, pPlayer->GetVisiblePos()));
		}
		else if(true == m_bStampMode)
		{
			MapObject* pMapObj = (MapObject*)g_ObjectManager.GetObject(pPlayer->GetCollectObjKey());
			if(pMapObj)
			{
				pPlayer->SetCollectStartEftHndl(GlobalFunc::CreateEffectBeUpToOption(m_wiStartEffectCode, pMapObj->GetVisiblePos()));
			}
		}
	}

	if(eCollectState_Loop == (eCollectState)pPlayer->GetCollectState() && 100.f <= fRet)
	{
		pPlayer->SetAnimation(m_wiAniCode[eCollectState_Loop], TRUE);
		pPlayer->SetCollectState((BYTE)eCollectState_End);
	}

	if(eCollectState_End == (eCollectState)pPlayer->GetCollectState() && pPlayer->GetCollectStartTime() > pPlayer->GetCollectMaxTime())
	{
		pPlayer->SetAnimation(m_wiAniCode[eCollectState_End], FALSE);

		MapObject* pMapObj = (MapObject*)g_ObjectManager.GetObject(pPlayer->GetCollectObjKey());
		if(pMapObj)
		{
			pMapObj->RemoveCollectionIDandEffect();
			m_bSuccess = true;
		}

		if(NULL != g_pEffectManager->GetEffectPtrHandle(pPlayer->GetCollectStartEftHndl()))
			g_pEffectManager->RemoveEffectPtr(pPlayer->GetCollectStartEftHndl());

		if(false == m_bStampMode)
		{
			GlobalFunc::CreateEffectBeUpToOption(m_wiSucEffectCode, pPlayer->GetVisiblePos());
		}
		else if(true == m_bStampMode)
		{
			GlobalFunc::CreateEffectBeUpToOption(m_wiSucEffectCode, pMapObj->GetVisiblePos());
		}
		pPlayer->SetCollectState((BYTE)eCollectState_Max);
	}

	if (eCollectState_Max == (eCollectState)pPlayer->GetCollectState() && fRet >= 100.0f)
		return STATEPROCESS::END;

	// 대부분의 State에서 공통적으로 처리하는 상태변화를 처리한다.
	if (pPlayer->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}

	// 다음 동작이 있으면 바로 수행해주세요~
	if (pPlayer->GetActionQueueCount())
	{
		pPlayer->ProcessNextAction();
		return STATEPROCESS::SUSPEND;
	}

	return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateCollect::OnEnd(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));

	if(pPlayer == g_pHero)
	{
		if(TRUE == g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_CASTING))
			g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CASTING,FALSE);

		if(m_bSuccess)
		{
			StateCollect::SendCollectSuccessSyn();
		}
		else
		{
			if(true == m_bStampMode && false == m_bMouseCancel)
			{
                TCHAR* debug_message = NULL;
#ifdef _DEV_VER
                debug_message = _T("MSG_CG_COLLECT_CANCEL_SYN[End]");
#endif 
                StateCollect::SendCollectCancelSyn(debug_message);

				return STATECHANGE::FAILED;
			}
		}
	}

    pPlayer->DestroyDominationStampEffect();

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    MapObject* map_object = (MapObject*)g_ObjectManager.GetObject(pPlayer->GetCollectObjKey());
    if (map_object)
    {
        ChaosSystem::Function::CreateBattleGroundConqeredEffect(pPlayer, map_object);
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	return STATECHANGE::SUCCESS;
}

void StateCollect::SendCollectActionSyn(DWORD object_key, WzVector player_position)
{
    is_send_action_syn_ = true;
    
    // send
    MSG_CG_COLLECT_ACTION_SYN send_packet;
    send_packet.m_ObjKeyCollection = object_key;
    send_packet.m_wvCurPos = player_position;
    if(TRUE == GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet)))
    {
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_COLLECT_ACTION_SYN"));
#endif 
    }
}

void StateCollect::SendCollectSuccessSyn()
{
    if (is_send_action_syn_ == true)
    {
        MSG_CG_COLLECT_SUCCESS_SYN SendPacket;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

        is_send_action_syn_ = false;

#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("MSG_CG_COLLECT_SUCCESS_SYN"));
#endif 
    }
    else
    {
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("StateCollect::SendCollectSuccessSyn() : is_send_action_syn_ == false"));
#endif 
    }
}

void StateCollect::SendCollectCancelSyn(TCHAR* debug_message)
{
    if (is_send_action_syn_ == true)
    {
        //send
        MSG_CG_COLLECT_CANCEL_SYN send_packet;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

        is_send_action_syn_ = false;

#ifdef _DEV_VER
        if (debug_message != NULL)
        {
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, debug_message);
        }        
#endif 
    }
    else
    {
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("StateCollect::SendCollectCancelSyn() : is_send_action_syn_ == false"));
#endif 
    }
}

