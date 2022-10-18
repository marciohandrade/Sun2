#include "SunClientPrecompiledHeader.h"

#include "QuestCondition_Concrete.h"
#include <Global.h>
#include <QuestManager.h>
#include <Quest.h>
#include "Player.h"
#include "ItemManager.h"
#include "QuestManager_Concrete.h"
#include "SCItemSlot.h"
#include "MissionManager_Concrete.h"
#include "GameFunc.h"
#include "hero.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorSystem/HonorSystem.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM

#define  RETURN_BOOL(exp)	(exp) == RC_QUEST_SUCCESS ? TRUE : FALSE

//------------------------------------------------------------------------------------------------------------
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_REPEAT )
RC::eQUEST_RESULT QUEST_CONDITION_REPEAT::CanAccept( void * pvParam )
{
	// IsSatisfy에서 해주긴 하지만 그래도;;
	if( 0 == GetInfo()->GetMaxRepeatNum() ) 
		return RC_QUEST_SUCCESS;

	return GetInfo()->IsSatisfy( GetQuest()->GetRepeatNum() );
}

//------------------------------------------------------------------------------------------------------------
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_CHARLV )
RC::eQUEST_RESULT QUEST_CONDITION_CHARLV::CanAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;

	if(pPlayer)
	{
		return GetInfo()->IsSatisfy(pPlayer->GetLevel());
	}
	else
	{
		return RC_QUEST_FAILED;
	}
}

RC::eQUEST_RESULT QUEST_CONDITION_CHARLV::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}

BOOL QUEST_CONDITION_CHARLV::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	if( evt == eQUEST_CONDITION_CHARCLASS)
		return RETURN_BOOL(CanAccept(g_pHero));
	else
		return FALSE;	
}

//------------------------------------------------------------------------------------------------------------
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_CHARCLASS )
RC::eQUEST_RESULT QUEST_CONDITION_CHARCLASS::CanAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;
	return GetInfo()->IsSatisfy(CharTypeToClassType(pPlayer->GetClass()));
}

__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_CHAO )
RC::eQUEST_RESULT QUEST_CONDITION_CHAO::CanAccept( void * pvParam )
{
	Player * pPlayer = (Player *)pvParam;

	if ( !pPlayer )
		return RC_QUEST_FAILED;

	return GetInfo()->IsSatisfy( pPlayer->GetChaoState() );
}

RC::eQUEST_RESULT QUEST_CONDITION_CHAO::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}

BOOL QUEST_CONDITION_CHAO::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	if( evt == QUEST_EVENT_CHAO)
		return RETURN_BOOL(CanAccept(g_pHero));
	else
		return FALSE;	
}

//------------------------------------------------------------------------------------------------------------
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
					if( NULL == g_pQMGR_Real->FindQuest( qCode ) ) 
						return RC_QUEST_NOT_EXIST_NECESSARY_QUEST;
				}
				return RC_QUEST_SUCCESS;
			}
			else
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					if( NULL != g_pQMGR_Real->FindQuest( qCode ) ) 
						return RC_QUEST_SUCCESS;
				}
				return RC_QUEST_NOT_EXIST_NECESSARY_QUEST;
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
					QUESTPART_COMPL * pQuest = g_pQMGR_Real->FindComplQuest( qCode );
					if( NULL == pQuest || pQuest->m_Num < GetInfo()->GetQuestNum(i) ) 
						return RC_QUEST_NOT_EXIST_NECESSARY_QUEST;
				}
				return RC_QUEST_SUCCESS;
			}
			else
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					QUESTPART_COMPL * pQuest = g_pQMGR_Real->FindComplQuest( qCode );
					if( NULL != pQuest && pQuest->m_Num >= GetInfo()->GetQuestNum(i) ) 
						return RC_QUEST_SUCCESS;
				}
				return RC_QUEST_NOT_EXIST_NECESSARY_QUEST;
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
					if( NULL != g_pQMGR_Real->FindQuest( qCode ) ) 
						return RC_QUEST_MUST_NOT_EXIST_QUEST;
				}
				return RC_QUEST_SUCCESS;
			}
			else
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					if( NULL == g_pQMGR_Real->FindQuest( qCode ) ) 
						return RC_QUEST_SUCCESS;
				}
				return RC_QUEST_MUST_NOT_EXIST_QUEST;
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
					QUESTPART_COMPL * pQuest = g_pQMGR_Real->FindComplQuest( qCode );
					if( NULL != pQuest || pQuest->m_Num < GetInfo()->GetQuestNum(i) ) 
						return RC_QUEST_MUST_NOT_EXIST_QUEST;
				}
				return RC_QUEST_SUCCESS;
			}
			else
			{
				for( BYTE i = 0 ; i < GetInfo()->GetQuestArrayCount() ; ++i )
				{
					QCODE qCode = GetInfo()->GetQuestCode(i);
					if( 0 == qCode ) continue;
					QUESTPART_COMPL * pQuest = g_pQMGR_Real->FindComplQuest( qCode );
					if( NULL == pQuest && pQuest->m_Num >= GetInfo()->GetQuestNum(i) ) 
						return RC_QUEST_SUCCESS;
				}

				return RC_QUEST_MUST_NOT_EXIST_QUEST;
			}
		}
	}
}

RC::eQUEST_RESULT QUEST_CONDITION_QUEST::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}

BOOL QUEST_CONDITION_QUEST::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	if( evt == eQUEST_CONDITION_QUEST )
		return RETURN_BOOL(CanAccept(g_pHero));
	else
		return FALSE;
}

//------------------------------------------------------------------------------------------------------------
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
			if ( 0 == GetInfo()->GetItemCode(i) )
				break;

			if( FALSE == ItemManager::Instance()->IsExistItem( SI_INVENTORY, GetInfo()->GetItemCode(i), GetInfo()->GetItemNum(i) ) )
				return RC_QUEST_NOT_EXIST_NECESSARY_ITEM;
		}
		return RC_QUEST_SUCCESS;
	}
	else
	{
		for( BYTE i = 0 ; i < GetInfo()->GetItemArrayCount() ; ++i )
		{
			if ( 0 == GetInfo()->GetItemCode(i) )
				break;

			// 아이템이 없어야 한다
			if( TRUE == ItemManager::Instance()->IsExistItem( SI_INVENTORY, GetInfo()->GetItemCode(i), GetInfo()->GetItemNum(i) ) )
				return RC_QUEST_MUST_NOT_EXIST_ITEM;
		}
		return RC_QUEST_SUCCESS;
	}
}

RC::eQUEST_RESULT QUEST_CONDITION_ITEM::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}

BOOL QUEST_CONDITION_ITEM::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	if( evt == QUEST_EVENT_ITEM )
		return RETURN_BOOL(CanAccept(g_pHero));
	else
		return FALSE;
}

//------------------------------------------------------------------------------------------------------------
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_CLASSITEM )
RC::eQUEST_RESULT QUEST_CONDITION_CLASSITEM::CanAccept( void * pvParam )
{
	// 제한된 클래스의 아이템을 가지면 안됨
	// 아직은 사용되지 않음
	DISPMSG( "[QUEST_CONDITION_CLASSITEM]사용되지 않는 퀘스트 조건이 사용되었다.\n" );
	
	return RC_QUEST_SUCCESS;
	
}

RC::eQUEST_RESULT QUEST_CONDITION_CLASSITEM::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}

//------------------------------------------------------------------------------------------------------------

__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_MONEY )
RC::eQUEST_RESULT QUEST_CONDITION_MONEY::CanAccept( void * pvParam )
{
	Hero * pHero = (Hero *)pvParam;
	return GetInfo()->IsSatisfy( pHero->GetMoney() );
}

RC::eQUEST_RESULT QUEST_CONDITION_MONEY::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}

BOOL QUEST_CONDITION_MONEY::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	if( evt == QUEST_EVENT_MONEY )
	{
		return RETURN_BOOL(CanAccept(g_pHero));
	}
	else
	{
		return FALSE;	
	}
}

//------------------------------------------------------------------------------------------------------------
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_KILLMONSTER )
void QUEST_CONDITION_KILLMONSTER::Init( const QUESTPART_PROGR * pStream )
{
	Clear();

	if( pStream )
	{
		for( BYTE i=0;i<5;++i)
		{
			SetCurMonsterNum(i, pStream->m_KCount[i]);
		}
	}
}
void QUEST_CONDITION_KILLMONSTER::Clear()
{
	for( BYTE i=0;i<5;++i)
	{
		SetCurMonsterCode(i,0);
		SetCurMonsterNum(i,0);
	}
}
void QUEST_CONDITION_KILLMONSTER::CopyOut( QUESTPART_PROGR & OUT rStream )
{
	for( BYTE i=0;i<5;++i)
	{
		rStream.m_KCount[i] = GetCurMonsterNum(i);
	}
}

RC::eQUEST_RESULT QUEST_CONDITION_KILLMONSTER::CanComplete( void * pvParam )
{
	for( BYTE i=0;i<5;++i)
	{
		if( GetCurMonsterNum(i) < GetInfo()->GetMonsterNum(i) ) 
			return RC_QUEST_NOT_KILL_NECESSARY_MONSTER;
	}
	return RC_QUEST_SUCCESS;
}

BOOL QUEST_CONDITION_KILLMONSTER::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	if( evt == QUEST_EVENT_MONSTERKILL )
	{
		// QUEST_CONDITION_KILLMONSTER_INFO 에 동일한 몬스터 코드가 다른 배열인덱스에 존재하지 않는다는 가정
		QUEST_MONSTER_KILL_MSG * pmsg = (QUEST_MONSTER_KILL_MSG *)pMsg;
		for( BYTE i=0;i<QUEST_CONDITION_KILLMONSTER::MAX_MONSTER_KIND_NUM;++i)
		{
			if( GetInfo()->GetMonsterCode(i) == pmsg->m_MonsterCode )
			{
				SetCurMonsterNum( i, GetCurMonsterNum(i)+1 ); 
				return TRUE;
			}
		}
	}
	else if ( evt == QUEST_EVENT_MONSTERKILL_GRADE )
	{
		QUEST_MONSTER_KILL_GRADE_MSG * pmsg = (QUEST_MONSTER_KILL_GRADE_MSG *)pMsg;
		for( BYTE i=0;i<QUEST_CONDITION_KILLMONSTER::MAX_MONSTER_KIND_NUM;++i)
		{
			if( GetInfo()->GetMonsterGrade(i) == pmsg->m_MonsterGrade
				&& GetInfo()->GetMonsterLevel(i) <= pmsg->m_MonsterLevel )
			{
				SetCurMonsterNum( i, GetCurMonsterNum(i)+1 ); 
				return TRUE;
			}
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------------------------------------

__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_MISSION )
RC::eQUEST_RESULT QUEST_CONDITION_MISSION::CanAccept( void * pvParam )
{
	Hero * pPlayer = (Hero *)pvParam;

	for( BYTE i = 0 ; i < QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM ; ++i )
	{
		if( 0 == GetInfo()->GetMissionCode(i) ) continue;
		assert(SAFE_NUMERIC_TYPECAST( WORD, GetInfo()->GetMissionCode(i), BYTE ));
		WORD point = 
			g_pMission_Concrete->GetMissionPoint((BYTE)GetInfo()->GetMissionCode(i));
		if( point == 0 || point < GetInfo()->GetMissionNum(i) ) 
			return RC_QUEST_NOT_LESS_MISSION_POINT;
	}
	return RC_QUEST_SUCCESS;
}

RC::eQUEST_RESULT QUEST_CONDITION_MISSION::CanComplete( void * pvParam )
{
	return CanAccept(pvParam);
}

BOOL QUEST_CONDITION_MISSION::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
	if( evt == QUEST_EVENT_MISSION )
	{
		return RETURN_BOOL(CanAccept(g_pHero));
	}
	else
	{
		return FALSE;	
	}
}

//------------------------------------------------------------------------------------------------------------

__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_AREA )
BOOL QUEST_CONDITION_AREA::Event(eQUEST_EVENT evt, QUEST_MSG* pMsg)
{
	if (evt == QUEST_EVENT_AREA)
	{
		QUEST_AREA_MSG* pmsg = (QUEST_AREA_MSG*)pMsg;
		for (int Index = 0; Index < QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM; ++Index)
		{
			if( GetInfo()->GetAreaID(Index) == pmsg->m_AreaID && FALSE == GetAreaCondition(Index) )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

RC::eQUEST_RESULT QUEST_CONDITION_AREA::CanComplete( void * pvParam )
{
	for (int Index = 0; Index < QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM; ++Index)
	{
		if ( FALSE == GetAreaCondition(Index) && GetInfo()->GetAreaID(Index) != 0 )
		{
			return RC_QUEST_NOT_CRASH_AREA;
		}
	}
		return RC_QUEST_SUCCESS;
}

void QUEST_CONDITION_AREA::Init( const QUESTPART_PROGR * pStream )
{
	if (pStream)
	{
		SetAreaCondition(0, BOOL(pStream->m_SearchedArea1));
		SetAreaCondition(1, BOOL(pStream->m_SearchedArea2));
		SetAreaCondition(2, BOOL(pStream->m_SearchedArea3));
	}	
}

int QUEST_CONDITION_AREA::CompleteArea(DWORD dwAreaID)
{
	for (int Index = 0; Index < QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM; ++Index)
	{
		if( dwAreaID == GetInfo()->GetAreaID(Index) )
		{
			SetAreaCondition(Index, TRUE);
			return Index;
		}
	}
	return -1;
}

void QUEST_CONDITION_AREA::Clear()
{
	for (int Index = 0; Index < QUEST_CONDITION_AREA_INFO::_MAX_CHECK_NUM; ++Index)
	{
		SetAreaCondition(Index, FALSE);
	}
}

__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_HONOR_RATING )
void QUEST_CONDITION_HONOR_RATING::Init( const QUESTPART_PROGR * pStream )
{
    if (pStream == NULL)
    {
        return;
    }


}

RC::eQUEST_RESULT QUEST_CONDITION_HONOR_RATING::CanAccept( void * pvParam )
{
    RC::eQUEST_RESULT result_code = RC_QUEST_LESS_HONOR;

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    BYTE condition_honor_type = GetInfo()->GetHonorType();
    WORD condition_honor_code = GetInfo()->GetHonorCode();
    BYTE condition_honor_rating = GetInfo()->GetHonorRating();
    BYTE current_honor_rating = 0;
    DWORD current_point_value = 0;

    if (condition_honor_type == 1)
    {
        HonorSystem::FamePointInfo* fame_point_info = HonorSystem::GetHonorSystemData().GetFamePointInfo(condition_honor_code);
        if (fame_point_info != NULL)
        {
            current_point_value = fame_point_info->fame_point_value;
            current_honor_rating = FameListParser::GetFameRating(fame_point_info->fame_info, current_point_value);
        }
    }
    else if (condition_honor_type == 2)
    {
        HonorSystem::ReputePointInfo* repute_point_info = HonorSystem::GetHonorSystemData().GetReputePointInfo(condition_honor_code);
        if (repute_point_info != NULL)
        {
            current_point_value = repute_point_info->repute_point_value;
            current_honor_rating = ReputeListParser::GetReputeRating(repute_point_info->repute_info, current_point_value);
        }
    }

    if (current_honor_rating >= condition_honor_rating)
    {
        result_code = RC_QUEST_SUCCESS;
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM

    return result_code;
}

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
__IMPL_CUSTOMPOOL_PTR( QUEST_CONDITION_MISSION_CLEAR )
void QUEST_CONDITION_MISSION_CLEAR::Init( const QUESTPART_PROGR * pStream )
{
    if (pStream)
    {       
        for(int i = 0; i < MAX_MISSION_CLEAR_CHECK_NUM; ++i)
        {
            SetCurMissionClearNum(i, 0);
            SetMissionCode(i, 0);
        }

        for(int i = 0; i < MAX_MISSION_CLEAR_CHECK_NUM; ++i)
        {
            SetCurMissionClearNum(i, pStream->mission_number[i]);            
        }
    }    
}

BOOL QUEST_CONDITION_MISSION_CLEAR::Event( eQUEST_EVENT evt, QUEST_MSG * pMsg )
{
    QUEST_MISSION_CLEAR_MSG* pMessage = reinterpret_cast<QUEST_MISSION_CLEAR_MSG*> (pMsg);

    if( evt == QUEST_EVENT_MISSION_CLEAR )
    {
        if (g_pHero)
        {
            for(DWORD i = 0; i < MAX_MISSION_CLEAR_CHECK_NUM; ++i)
            {
                if(GetInfo()->GetMissionCode(i) == pMessage->mission_code)
                {
                    DWORD current_mission_clear_num = GetCurMissionClearNum(i);

                    SetCurMissionClearNum(i, current_mission_clear_num + 1);
                }
            }            

            return RETURN_BOOL(CanAccept(g_pHero));
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

RC::eQUEST_RESULT QUEST_CONDITION_MISSION_CLEAR::CanComplete( void* pvParam )
{
    int result_count = 0;
    RC::eQUEST_RESULT result = RC_QUEST_FAILED;

    for(DWORD i = 0; i < MAX_MISSION_CLEAR_CHECK_NUM; ++i)
    {
        if (GetCurMissionClearNum(i) >= GetInfo()->GetMissionNum(i))
        {
            result = RC_QUEST_SUCCESS;
        }
        else
        {
            result = RC_QUEST_FAILED;

            return result;
        }
    }

    return result;
}

void QUEST_CONDITION_MISSION_CLEAR::CopyOut( QUESTPART_PROGR & OUT rStream )
{
    for ( DWORD i=0;i < MAX_MISSION_CLEAR_CHECK_NUM; ++i)
    {
        rStream.m_KCount[i] = static_cast<BYTE>(GetCurMissionClearNum(i));
    }
}

RC::eQUEST_RESULT QUEST_CONDITION_MISSION_CLEAR::CanAccept( void* pvParam )
{
    Hero * pPlayer = static_cast<Hero*> (pvParam);

    return RC_QUEST_SUCCESS;
}
#endif // _NA_006607_20130402_ADD_QUESTTYPE
