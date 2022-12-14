#include "stdafx.h"
#include "QuestCondition_Concrete.h"
#include <Global.h>
#include <QuestManager.h>
#include <Quest.h>
#include <MapInfoParser.h>
#include "Player.h"
#include "ChaoState.h"
#include "ItemManager.h"
#include "MissionManager.h"
#include "WarehouseSlotContainer.h"
#include "GameField.h"
#include "MovingPlanner.h"
#include "ExtraNPCManager.h"
#include "HonorSystem/HonorSystem.h" //_NA_003027_20111013_HONOR_SYSTEM
#include "ResultCode.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_REPEAT )
RC::eQUEST_RESULT QUEST_CONDITION_REPEAT::CanAccept( void * pvParam )
{
	// IsSatisfy에서 해주긴 하지만 그래도;;
	if( 0 == GetInfo()->GetMaxRepeatNum() )
	{
		return RC::RC_QUEST_SUCCESS;
	}

	return GetInfo()->IsSatisfy( GetQuest()->GetRepeatNum() );
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_CHARLV )
RC::eQUEST_RESULT QUEST_CONDITION_CHARLV::CanAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;

	return GetInfo()->IsSatisfy(pPlayer->GetLevel());
}

RC::eQUEST_RESULT QUEST_CONDITION_CHARLV::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_CHARCLASS )

RC::eQUEST_RESULT QUEST_CONDITION_CHARCLASS::CanAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;

	return GetInfo()->IsSatisfy(CharTypeToClassType(pPlayer->GetCharType()));
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_QUEST )
RC::eQUEST_RESULT QUEST_CONDITION_QUEST::CanAccept( void * pvParam )
{	
	
	if( GetInfo()->IsNecessaryQuest() )				//< 퀘스트가 필요한가
	{
		if( GetInfo()->IsProgrQuest() )				//< 진행중인 퀘스트 검사
		{
			if( GetInfo()->GetOperator() == QOP_AND )
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					if( NULL == GetManager()->FindQuest( qCode ) )
					{
						return RC::RC_QUEST_NOT_EXIST_NECESSARY_QUEST;
					}
				}
				return RC::RC_QUEST_SUCCESS;
			}
			else
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;

					if( NULL != GetManager()->FindQuest( qCode ) )
					{
						return RC::RC_QUEST_SUCCESS;
					}
					

				}
				return RC::RC_QUEST_NOT_EXIST_NECESSARY_QUEST;
			}
		}
		else										//< 완료된 퀘스트 검사
		{
			if( GetInfo()->GetOperator() == QOP_AND )
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					QUESTPART_COMPL * pQuest = GetManager()->FindComplQuest( qCode );

					if( NULL == pQuest || pQuest->m_Num < GetInfo()->GetQuestNum(i) )
					{
						return RC::RC_QUEST_NOT_EXIST_NECESSARY_QUEST;
					}
				}
				return RC::RC_QUEST_SUCCESS;
			}
			else
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					QUESTPART_COMPL * pQuest = GetManager()->FindComplQuest( qCode );
					if( NULL != pQuest && pQuest->m_Num >= GetInfo()->GetQuestNum(i) )
					{
						return RC::RC_QUEST_SUCCESS;
					}
				}
				return RC::RC_QUEST_NOT_EXIST_NECESSARY_QUEST;
			}
		}
	}
	else
	{
		// 없어야 한다
		if( GetInfo()->IsProgrQuest() )				//< 진행중인 퀘스트 검사
		{
			if( GetInfo()->GetOperator() == QOP_AND )
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					if( NULL != GetManager()->FindQuest( qCode ) )
					{
						return RC::RC_QUEST_MUST_NOT_EXIST_QUEST;
					}
				}
				return  RC::RC_QUEST_SUCCESS;
			}
			else
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					if( NULL == GetManager()->FindQuest( qCode ) )
					{
						return RC::RC_QUEST_SUCCESS;
					}
				}
				return RC::RC_QUEST_MUST_NOT_EXIST_QUEST;
			}
		}
		else										//< 완료된 퀘스트 검사
		{
			if( GetInfo()->GetOperator() == QOP_AND )
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					QUESTPART_COMPL * pQuest = GetManager()->FindComplQuest( qCode );
					if( NULL != pQuest || pQuest->m_Num < GetInfo()->GetQuestNum(i) )
					{
						return RC::RC_QUEST_MUST_NOT_EXIST_QUEST;						
					}
				}
				return RC::RC_QUEST_SUCCESS;
			}
			else
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					QUESTPART_COMPL * pQuest = GetManager()->FindComplQuest( qCode );
					if( NULL == pQuest && pQuest->m_Num >= GetInfo()->GetQuestNum(i) )
					{
						return RC::RC_QUEST_SUCCESS;
					}
				}
				return RC::RC_QUEST_MUST_NOT_EXIST_QUEST;
			}
		}
	}

	//return GetInfo()->IsSatisfy(pPlayer->GetLevel());
}

RC::eQUEST_RESULT QUEST_CONDITION_QUEST::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_ITEM )
RC::eQUEST_RESULT QUEST_CONDITION_ITEM::CanAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;
	if( GetInfo()->IsNecessaryItem() )				//< 아이템이 있어야하는가
	{
		// 다른 종류의 아이템이어야 한다!
		// 같은 종류가 다른 배열에 있는 가정은 하지 않는다 ; 당연 체크도 하지 않는다.
		for( BYTE i = 0 ; i < GetInfo()->GetItemArrayCount() ; ++i )
		{
            int cur_exist_item_num = pPlayer->GetItemManager()->ExistItem( SI_INVENTORY, GetInfo()->GetItemCode(i), GetInfo()->GetItemNum(i) );
			if( GetInfo()->GetItemNum(i) != cur_exist_item_num )
			{
                SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Item QuestComplete Failed! [QCode:%d][ItemCode:%d][Cur:%d][Goal:%d]",
                    GetQuest()->GetQuestCode(), GetInfo()->GetItemCode(i), cur_exist_item_num, GetInfo()->GetItemNum(i));

				return RC::RC_QUEST_NOT_EXIST_NECESSARY_ITEM;
			}
		}
		return RC::RC_QUEST_SUCCESS;
	}
	else
	{
		for( BYTE i = 0 ; i < GetInfo()->GetItemArrayCount() ; ++i )
		{
			// 아이템이 없어야 한다
			if( GetInfo()->GetItemNum(i) != pPlayer->GetItemManager()->ExistItem( SI_INVENTORY, GetInfo()->GetItemCode(i), GetInfo()->GetItemNum(i) ) )
			{
				return RC::RC_QUEST_MUST_NOT_EXIST_ITEM;
			}
		}
		return RC::RC_QUEST_SUCCESS;
	}
}

RC::eQUEST_RESULT QUEST_CONDITION_ITEM::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_CLASSITEM )
RC::eQUEST_RESULT QUEST_CONDITION_CLASSITEM::CanAccept( void * pvParam )
{
	// 제한된 클래스의 아이템을 가지면 안됨
	// 아직은 사용되지 않음
	SUNLOG( eCRITICAL_LOG, "[QUEST_CONDITION_CLASSITEM]사용되지 않는 퀘스트 조건이 사용되었다.\n" );
	/*
	Player * pPlayer = (Player *)pvParam;
	SCItemSlotContainer * pContainer = pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
	for( POSTYPE pos = 0 ; pos < pContainer->GetMaxSlotNum() ; ++pos )
	{
		if( !pContainer->IsEmpty(pos) )
		{
			SCItemSlot & slot = (SCItemSlot &)pContainer->GetSlot(pos);
			if( 0 != slot.GetItemInfo()->m_wEqClass1 )
			{
				if( m_pPlayer->GetCharType() != eCHAR_BERSERKER ) return FALSE;
			}
			if( 0 != slot.GetItemInfo()->m_wEqClass2 )
			{
				if( m_pPlayer->GetCharType() != eCHAR_DRAGON ) return FALSE;
			}
			if( 0 != slot.GetItemInfo()->m_wEqClass3 )
			{
				if( m_pPlayer->GetCharType() != eCHAR_SHADOW ) return FALSE;
			}
			if( 0 != slot.GetItemInfo()->m_wEqClass4 )
			{
				if( m_pPlayer->GetCharType() != eCHAR_VALKYRIE ) return FALSE;
			}
			if( 0 != slot.GetItemInfo()->m_wEqClass5 )
			{
				if( m_pPlayer->GetCharType() != eCHAR_ELEMENTALIST ) return FALSE;
			}
		}
	}
	*/

	return RC::RC_QUEST_FAILED;
}

RC::eQUEST_RESULT QUEST_CONDITION_CLASSITEM::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_MONEY )
RC::eQUEST_RESULT QUEST_CONDITION_MONEY::CanAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;

	return GetInfo()->IsSatisfy( pPlayer->GetMoney() );
}

RC::eQUEST_RESULT QUEST_CONDITION_MONEY::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_KILLMONSTER )
void QUEST_CONDITION_KILLMONSTER::Init( const QUESTPART_PROGR * pStream )
{
	Clear();

	if( pStream )
	{
		for( BYTE i=0;i<QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM;++i)
		{
			SetCurMonsterNum(i, pStream->m_KCount[i]);
		}
	}
}
void QUEST_CONDITION_KILLMONSTER::Clear()
{
	for( BYTE i=0;i<QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM;++i)
	{
        SetCurMonsterNum(i,0);
	}
}
void QUEST_CONDITION_KILLMONSTER::CopyOut( QUESTPART_PROGR & OUT rStream )
{
	for( BYTE i=0;i<QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM;++i)
	{
		rStream.m_KCount[i] = GetCurMonsterNum(i);
	}
}

RC::eQUEST_RESULT QUEST_CONDITION_KILLMONSTER::CanComplete( void * pvParam )
{

	for( BYTE i=0;i<QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM;++i)
	{
		if( GetCurMonsterNum(i) < GetInfo()->GetMonsterNum(i) )
		{
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] KillMonster QuestComplete Failed! [Cur:%d][Goal:%d",
                   GetCurMonsterNum(i), GetInfo()->GetMonsterNum(i));

			return RC::RC_QUEST_NOT_KILL_NECESSARY_MONSTER;
		}
	}

	return RC::RC_QUEST_SUCCESS;
}

BOOL QUEST_CONDITION_KILLMONSTER::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	if( evt == QUEST_EVENT_MONSTERKILL )
	{
		// QUEST_CONDITION_KILLMONSTER_INFO 에 동일한 몬스터 코드가 다른 배열인덱스에 존재하지 않는다는 가정
		QUEST_MONSTER_KILL_MSG * pmsg = (QUEST_MONSTER_KILL_MSG *)pMsg;
		for( BYTE i=0;i<QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM;++i)
		{
			if( GetInfo()->GetMonsterCode(i) == pmsg->m_MonsterCode &&
				GetCurMonsterNum(i) < GetInfo()->GetMonsterNum(i) )
			{
				SetCurMonsterNum( i, GetCurMonsterNum(i)+1 ); //pmsg->m_MonsterNum );
				return TRUE;
			}
		}
	}
    else if(evt == QUEST_EVENT_MONSTERKILL_GRADE)
    {
        QUEST_MONSTER_KILL_GRADE_MSG * pmsg = (QUEST_MONSTER_KILL_GRADE_MSG *)pMsg;
        QUEST_CONDITION_KILLMONSTER_INFO* pInfo = GetInfo();

        for( BYTE i=0;i<QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM;++i)
        {
            const BYTE QuestMonsterGrade = pInfo->GetMonsterGrade(i);
            const WORD QuestMonsterLevel = pInfo->GetMonsterLevel(i);
            const BYTE QuestMonsterNum   = pInfo->GetMonsterNum(i);

            if( QuestMonsterGrade == pmsg->m_MonsterGrade &&
                QuestMonsterLevel <= pmsg->m_MonsterLevel &&
                QuestMonsterNum   >  m_pCurMonsterNum[i]  )
            {
                ++m_pCurMonsterNum[i];

#ifdef _NA_006644_20130419_BUG_QUEST_KILLMONSTER
#else
                // 클라에서 필요한 퀘스트정보를 준다.
                pmsg->m_MonsterLevel = QuestMonsterLevel;
#endif //_NA_006644_20130419_BUG_QUEST_KILLMONSTER
                return TRUE;
            }
        }
    }

    return FALSE;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_MISSION )
RC::eQUEST_RESULT QUEST_CONDITION_MISSION::CanAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;

	for( BYTE i = 0 ; i < QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM ; ++i )
	{
		if( 0 == GetInfo()->GetMissionCode(i) ) continue;
		ASSERT(SAFE_NUMERIC_TYPECAST( WORD, GetInfo()->GetMissionCode(i), BYTE ));
		WORD point = pPlayer->GetMissionManager()->GetMissionPoint((BYTE)GetInfo()->GetMissionCode(i));
		if( point == 0 || point < GetInfo()->GetMissionNum(i) )
		{
			return RC::RC_QUEST_NOT_LESS_MISSION_POINT;
		}
	}
	return RC::RC_QUEST_SUCCESS;
}

RC::eQUEST_RESULT QUEST_CONDITION_MISSION::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_AREA )
void QUEST_CONDITION_AREA::Init( const QUESTPART_PROGR * pStream )
{
    Clear();

    if( NULL == pStream )
        return;

    SetCurSearchedArea(Area1, pStream->m_SearchedArea1);
    SetCurSearchedArea(Area2, pStream->m_SearchedArea2);
    SetCurSearchedArea(Area3, pStream->m_SearchedArea3);
}

void QUEST_CONDITION_AREA::Clear()
{
    for( BYTE i=0;i<QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM;++i)
        SetCurSearchedArea(i,UNKNOWN_AREA);
}

void QUEST_CONDITION_AREA::CopyOut( QUESTPART_PROGR & OUT rStream )
{
    rStream.m_SearchedArea1 = GetCurSearchedArea(Area1);
    rStream.m_SearchedArea2 = GetCurSearchedArea(Area2);
    rStream.m_SearchedArea3 = GetCurSearchedArea(Area3);
}


BOOL QUEST_CONDITION_AREA::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
    if( evt == QUEST_EVENT_AREA )
    {
        QUEST_AREA_MSG * pmsg = (QUEST_AREA_MSG *)pMsg;

        for( BYTE i=0;i<QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM;++i)
        {
            if( GetInfo()->GetAreaID(i) == pmsg->m_AreaID )
            {
                SetCurSearchedArea( i, SEARCHED_AREA );
                return TRUE;
            }
        }
    }

    return FALSE;
}

RC::eQUEST_RESULT QUEST_CONDITION_AREA::CanComplete( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;

    GameField* pField = pPlayer->GetField();
	if( !pField )
	{
    	return RC::RC_QUEST_NOT_FIELD;
    }

    for( BYTE i=0;i<QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM;++i)
    {
        if( 0 != GetInfo()->GetAreaID(i) && 
            UNKNOWN_AREA == GetCurSearchedArea(i) )
        {        
            return RC::RC_QUEST_NOT_CRASH_AREA;        
        }
    }
   
    return RC::RC_QUEST_SUCCESS;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_ITEM_SPACE )
RC::eQUEST_RESULT QUEST_CONDITION_ITEM_SPACE::CanAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;

	static JUST_INVENTORY_TOTAL_INFO s_JustTotalInfo;

	/*
	if( GetInfo()->GetItemArrayCount() == 0 )
		return TRUE;
	
	RC::eITEM_RESULT rt = pPlayer->GetItemManager()->ObtainAllKindOfItem( 
		GetInfo()->GetItemArrayCount(), GetInfo()->GetItemCodePtr(), GetInfo()->GetItemNumPtr(), &s_JustTotalInfo );	

	if( rt != RC::RC_ITEM_SUCCESS )
	{
		return FALSE;
	}

	// 성공하면 삭제
	pPlayer->GetItemManager()->DeleteFromTotalInfo( &s_JustTotalInfo );

	return TRUE;
	*/
	
	SCItemSlotContainer * pInven = pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
    if(pInven->GetSlotNum() + GetInfo()->GetItemArrayCount() > pInven->GetMaxSlotNum())
	{
		return RC::RC_QUEST_NOT_EMPTY_ITEM_SPACE;
	}
	else
	{
		return RC::RC_QUEST_SUCCESS;
	}
}

RC::eQUEST_RESULT QUEST_CONDITION_ITEM_SPACE::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_CHAO )
RC::eQUEST_RESULT QUEST_CONDITION_CHAO::CanAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;
	
	return GetInfo()->IsSatisfy(pPlayer->GetChaoState().GetChaoState());
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
__IMPL_CUSTOMPOOL_PTR(QUEST_CONDITION_CREATE_PORTAL)
// 은파씨 저 일욜 휴무라 빌드 시 필요 한 부분입니다.
// 출근 하신다면 꼭 좀 작업 부탁드려요.
// 전화 하니까 안받다가 끊어버리시네 -_- 09.03.21(SAT)
// <- waverix왈... 전화기 외투 속에 있고, 외투는 다른 방 옷장에 있습니다. (보통...-.- 오해는 마시길.)

#define	ERROR_RETURN_FOR_QUEST_CONDITION(return_type)	(return_type)
RC::eQUEST_RESULT QUEST_CONDITION_CREATE_PORTAL::CanAccept(void* pvParam)
{
	RC::eQUEST_RESULT ret = RC::RC_QUEST_SUCCESS;
	Player* pPlayer = (Player*)pvParam;
	if(pPlayer == 0)
		return ERROR_RETURN_FOR_QUEST_CONDITION(RC::RC_QUEST_NOT_PLAYER);

	GameField* pField = pPlayer->GetField();
	if(pField == 0)
		return ERROR_RETURN_FOR_QUEST_CONDITION(RC::RC_QUEST_NOT_FIELD);

	//일단 아래 제약 조건은 다른 곳에서 가져오긴 했는데... NULL이어도 되나? 오히려 위험 코드.
	Quest* pQuest = GetQuest();
	if(pQuest == 0)
		return ERROR_RETURN_FOR_QUEST_CONDITION(RC::RC_QUEST_EMPTY);

	QuestInfo* pQuestInfo = pQuest->GetQuestInfo();
	DWORD startNpcCode = pQuestInfo->GetNPCCode_Start();
	if(!g_ExtraNPCManager.IsLengthCanUseNPC(startNpcCode, pPlayer))
		return ERROR_RETURN_FOR_QUEST_CONDITION(RC::RC_QUEST_DISTANCE_DIFF_TOO_LONG);

	QUEST_CONDITION_CREATE_PORTAL_INFO* pInfo = GetInfo();
	MAPCODE mapCode = pInfo->GetMapPortalCode();
	if(mapCode == 0)
		return ERROR_RETURN_FOR_QUEST_CONDITION(RC::RC_QUEST_NOT_CRASH_AREA);

	const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);
	if(pMapInfo == 0)
	{
		SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Can't Find MapPortalCode %u\n", mapCode);
		return ERROR_RETURN_FOR_QUEST_CONDITION(RC::RC_QUEST_NOT_CRASH_AREA);
	}

	const eZONETYPE zoneType = (eZONETYPE)pMapInfo->byMKind;
	const BOOLEAN acceptableMap = zoneType == eZONETYPE_MISSION ||
		zoneType == eZONETYPE_INSTANCE;
	if(acceptableMap == FALSE)
		return ERROR_RETURN_FOR_QUEST_CONDITION(RC::RC_QUEST_NOT_CRASH_AREA);

	// Register Portal Constraints { NPC 기준 범위... 제약, 기본 타임아웃도 존재하는데... }
	// A<->G 계열 Zone Control이 요구된다.

	nsAI::MovingPlanner* pMovingPlanner = pField->GetMovingPlanner();
	const WzVector* pCurPos = pPlayer->GetPosPtr();
	WORD attributes;
	INT iTile = pMovingPlanner->GetAllowTileToStand(*pCurPos, NULL, -1.0f, 100.0f, &attributes,
		PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE);
	if(iTile < 0)
		return ERROR_RETURN_FOR_QUEST_CONDITION(RC::RC_QUEST_NOT_CRASH_AREA);

	return ERROR_RETURN_FOR_QUEST_CONDITION(RC::RC_QUEST_SUCCESS);
}
//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifdef _NA_003027_20111013_HONOR_SYSTEM

RC::eQUEST_RESULT QUEST_CONDITION_DAILY_QUEST::CanAccept( void * pvParam )
{
    Player* player = (Player*)pvParam;
    if(player == 0)
        return RC::RC_QUEST_NOT_PLAYER;

    Quest* quest = GetQuest();
    
    if( !quest->GetQuestInfo()->IsDailyQuest() )
        return RC::RC_QUEST_NOT_DAILY;

    QCODE code = quest->GetQuestCode();
    WORD group = quest->GetQuestInfo()->GetSquadCode();
    QuestManager* quest_manager = quest->GetQuestManager();
    
    if( group <= 0 ) {
        return RC::RC_QUEST_NOT_TODAY;
    }

    if( !HonorSystem::Instance()->IsTodayQuest(group, code) )
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Daily Quest Error! [Group:%d][ClientQuestCode:%d]", group, code);

        return RC::RC_QUEST_NOT_TODAY;
    }

    if (quest->GetQuestInfo()->GetDailyQuest() == DAILYQUEST_GUILD &&
        player->GetGuildGuid() == INVALID_GUILDGUID)
        return RC::RC_QUEST_FAILED;

    return  quest_manager->CanDoAcceptDailyQuest(code, player);
}
#endif //_NA_003027_20111013_HONOR_SYSTEM
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
void QUEST_CONDITION_MISSION_CLEAR::Init(const QUESTPART_PROGR *pStream)
{
    Clear();

    if(pStream)
    {
        for( BYTE i=0;i<QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM; ++i)
        {
            SetCurClearNum(i, pStream->mission_number[i]);
        }
    }
}
void QUEST_CONDITION_MISSION_CLEAR::Clear()
{
    for( BYTE i=0;i<QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM;++i)
    {
        SetCurClearNum(i,0);
    }
}
void QUEST_CONDITION_MISSION_CLEAR::CopyOut( QUESTPART_PROGR & OUT rStream )
{
    for( BYTE i=0;i<QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM;++i)
    {
        rStream.mission_number[i] = GetCurClearNum(i);
    }
}
RC::eQUEST_RESULT QUEST_CONDITION_MISSION_CLEAR::CanComplete(void *pvParam)
{
    for( BYTE i=0;i<QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM;++i)
    {
        if( GetCurClearNum(i) < GetInfo()->GetMissionNum(i) )
        {
            return RC::RC_QUEST_NOT_ENOUGH_MISSION_CLEAR_COUNT;
        }
    }

    return RC::RC_QUEST_SUCCESS;
}
BOOL QUEST_CONDITION_MISSION_CLEAR::Event(eQUEST_EVENT evt, QUEST_MSG *pMsg)
{
    if( evt == QUEST_EVENT_MISSION_CLEAR)
    {
        QUEST_MISSION_CLEAR_MSG* pmsg = (QUEST_MISSION_CLEAR_MSG *)pMsg;
        for(BYTE i=0; i<QUEST_CONDITION_MISSION_CLEAR_INFO::_MAX_CHECK_NUM; ++i)
        {
            if(GetInfo()->GetMissionCode(i) == pmsg->mission_code &&
               GetCurClearNum(i) < GetInfo()->GetMissionNum(i))
            {
                SetCurClearNum(i, GetCurClearNum(i)+1);
                return true;
            }
        }
    }

    return FALSE;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#endif //_NA_006607_20130402_ADD_QUESTTYPE