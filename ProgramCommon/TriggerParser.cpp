
#include "StdAfx.h"
#include "TriggerParser.h"
#include <SolarFileReader.h>

TriggerParser::TriggerParser()
{
	Init();
}

TriggerParser::~TriggerParser()
{
	Release();
}

BOOL TriggerParser::Init()
{
	m_TriggerListForMap.Initialize( _MAX_TRIGGERLIST_SIZE );
	return TRUE;
}

void TriggerParser::Release()
{
	sTRIGGER_CONTAINER * pTriggerContainer	= NULL;
	TRIGGER_TYPE * pTRIGGER_TYPE			= NULL;
	sTRIGGERLIST * pTriggerList				= NULL;

	m_TriggerListForMap.SetFirst();
	while( pTriggerList = m_TriggerListForMap.GetNext() )
	{
		TriggerList_iterator	it;
		for( it = pTriggerList->m_TriggerListVector.begin() ; it != pTriggerList->m_TriggerListVector.end() ; ++it )
		{
			//	트리거 한개 꺼낸다
			pTriggerContainer = *it;


			TriggerType_iterator	it_TriggerType;

			//	컨디션 트리거 리스트 삭제
			for( it_TriggerType = pTriggerContainer->m_ConditionTrigger.begin() ; it_TriggerType != pTriggerContainer->m_ConditionTrigger.end() ; ++it_TriggerType )
			{
				pTRIGGER_TYPE = *it_TriggerType;
				SAFE_DELETE( pTRIGGER_TYPE );
			}
			pTriggerContainer->m_ConditionTrigger.clear();


			//	액션 트리거 리스트 삭제
			for( it_TriggerType = pTriggerContainer->m_ActionTrigger.begin() ; it_TriggerType != pTriggerContainer->m_ActionTrigger.end() ; ++it_TriggerType )
			{
				pTRIGGER_TYPE = *it_TriggerType;
				SAFE_DELETE( pTRIGGER_TYPE );
			}
			pTriggerContainer->m_ActionTrigger.clear();


			//	트리거 리스트 중, 한개 삭제
			SAFE_DELETE( pTriggerContainer );
		}

		pTriggerList->m_TriggerListVector.clear();

		SAFE_DELETE( pTriggerList );
	}

	m_TriggerListForMap.RemoveAll();
}


BOOL TriggerParser::Load( char * pTriggerFileName, sTRIGGERLIST * pTriggerList )
{
	HANDLE hFile = ::CreateFile( pTriggerFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
	if( INVALID_HANDLE_VALUE == hFile )
	{
		return FALSE;
	}

	//-----------------------------------------------------------------------------------
	//		데이터 분석
	//-----------------------------------------------------------------------------------

	DWORD dwNumber	= 0;


	//	방씨!!!! 이 6 바이트는 뭐냐고~~~~~~~   ( 파일 헤더랍니다 )
	const DWORD		dwSIX_BYTE = 6;


	//	건너뛸 파일 헤더
	DWORD	dwFileHeaderSize = dwSIX_BYTE + sizeof( WORD );
	BYTE	FileHeader[8];
	BOOL rt = ::ReadFile( hFile, &FileHeader, dwFileHeaderSize, &dwNumber, NULL );
	if( !rt )
	{
		return FALSE;
	}


	//	건너뛸 청크 사이즈 읽기
	DWORD	dwDummyChunkSize;
	rt = ::ReadFile( hFile, &dwDummyChunkSize, sizeof(DWORD), &dwNumber, NULL );
	if( !rt )
	{
		return FALSE;
	}


	//	필요 없는 청크 건너 뛰기
	dwDummyChunkSize = dwDummyChunkSize + 2 + 4;	//	청크 헤더와 청크 사이즈 정보 건너 뛴다
	BYTE	*pDummyChunk = new BYTE[dwDummyChunkSize + 1];
	rt = ::ReadFile( hFile, pDummyChunk, dwDummyChunkSize, &dwNumber, NULL );
	if( !rt )
	{
		return FALSE;
	}

	SAFE_DELETE_ARRAY( pDummyChunk );



	//-----------------------------------------------------------------------------------
	//		실제 데이터 시작
	//-----------------------------------------------------------------------------------

	DWORD	dwPos = dwSIX_BYTE + dwDummyChunkSize + 6;

	//	총 트리거 갯수
	rt = ::ReadFile( hFile, &(pTriggerList->m_wNumberOfTriggerContainer), sizeof(WORD), &dwNumber, NULL );
	if( !rt )
	{
		return FALSE;
	}
	dwPos += dwNumber;

	//	총 트리거 갯수만큼 돌면서 트리거 읽기
	for( WORD wCount = 0 ; wCount < pTriggerList->m_wNumberOfTriggerContainer ; ++wCount )
	{
		sTRIGGER_CONTAINER * pTriggerContainer = new sTRIGGER_CONTAINER;

		//	트리거 리스트 속성
		rt = ::ReadFile( hFile, &(pTriggerContainer->m_wTriggerListAtt), sizeof(WORD), &dwNumber, NULL );
		if( !rt )
		{
			return FALSE;
		}
		dwPos += dwNumber;

		//	컨디션 트리거 개수
		rt = ::ReadFile( hFile, &(pTriggerContainer->m_wNumConTrigger), sizeof(WORD), &dwNumber, NULL );
		if( !rt )
		{
			return FALSE;
		}
		dwPos += dwNumber;


		WORD			wType;			//	트리거 타입 정보
		DWORD			dwSize;			//	타입에 따른 트리거 데이터 구조체 크기
		TRIGGER_TYPE	*pTRIGGER_TYPE;	//	트리거 리스트에 저장될 트리거 데이터
		BYTE			*pBuffer;

		//	개수 만큼 돌면서 컨디션 트리거 읽기
		for( WORD wNumConTrigger = 0 ; wNumConTrigger < pTriggerContainer->m_wNumConTrigger ; ++wNumConTrigger )
		{
			//	컨디션 트리거 타입 읽기
			rt = ::ReadFile( hFile, &wType, sizeof(WORD), &dwNumber, NULL );
			if( !rt )
			{
				return FALSE;
			}
			dwPos += dwNumber;

			//	타입에 따라 데이터 읽기
			pBuffer = SetTriggerType( wType, &dwSize, eTRIGGER_CONDITION );
			rt = ::ReadFile( hFile, ( pBuffer + sizeof(WORD) ), ( dwSize - sizeof(WORD) ), &dwNumber, NULL );
			if( !rt )
			{
				return FALSE;
			}
			dwPos += dwNumber;
			pTRIGGER_TYPE = (TRIGGER_TYPE *)pBuffer;


			//	읽은 컨디션 트리거 데이터 저장
			pTriggerContainer->m_ConditionTrigger.push_back( pTRIGGER_TYPE );
		}


		//	액션 트리거 개수
		rt = ::ReadFile( hFile, &(pTriggerContainer->m_wNumActTrigger), sizeof(WORD), &dwNumber, NULL );
		if( !rt )
		{
			return FALSE;
		}
		dwPos += dwNumber;

		//	개수 만큼 돌면서 액션 트리거 읽기
		for( WORD wNumActTrigger = 0 ; wNumActTrigger < pTriggerContainer->m_wNumActTrigger ; ++wNumActTrigger )
		{
			//	액션 트리거 타입 읽기
			rt = ::ReadFile( hFile, &wType, sizeof(WORD), &dwNumber, NULL );
			if( !rt )
			{
				return FALSE;
			}
			dwPos += dwNumber;


			//	타입에 따라 데이터 읽기
			pBuffer = SetTriggerType( wType, &dwSize, eTRIGGER_ACTION );
			rt = ::ReadFile( hFile, ( pBuffer + sizeof(WORD) ), ( dwSize - sizeof(WORD) ), &dwNumber, NULL );
			if( !rt )
			{
				return FALSE;
			}
			dwPos += dwNumber;
			pTRIGGER_TYPE = (TRIGGER_TYPE *)pBuffer;

			FASSERT( pTRIGGER_TYPE != NULL );
			//	읽은 액션 트리거 데이터 저장
			pTriggerContainer->m_ActionTrigger.push_back( pTRIGGER_TYPE );
		}

		//	트리거 리스트에 저장
		pTriggerList->m_TriggerListVector.push_back( pTriggerContainer );
	}

	::CloseHandle( hFile );

	return TRUE;
}

BOOL TriggerParser::Load( char * pszRootDir, char *pTriggerScriptFileName )
{
	//-----------------------------------------------------------------------------------------------
	// Trigger.txt파일 파싱
	//-----------------------------------------------------------------------------------------------
	char pszFullPath[MAX_PATH];
	sprintf( pszFullPath, "%s%s", pszRootDir, pTriggerScriptFileName );

	util::SolarFileReader sr;
	if ( !sr.OpenFile( pszFullPath, SEPERATOR_WHITESPACE ) )
	{
		SUNLOG( eCRITICAL_LOG, "Script File Load Fail, File = %s",  pszFullPath );
		SASSERT( NULL, "[에러]:트리거 스크립트 파일을 열 수 없습니다." );
		return FALSE;
	}
	else
		SUNLOG( eDEV_LOG, "Script File Load Start, File = %s",  pszFullPath );

	sTRIGGERLIST * pTriggerList = NULL;
	while( TRUE )
	{
		eTOKEN_TYPE ettType = sr.GetNextTokenType();
		if ( TOKEN_END == ettType )
		{
			break;
		}
		if( ettType != TOKEN_NUMBER )
		{
			FASSERT( !"잘못된 Field 스크립트입니다." );
		}

		pTriggerList = new sTRIGGERLIST;
		pTriggerList->m_TriggerCode = (CODETYPE)sr.GetTokenNumber();

		char pszPath[MAX_PATH];
		sr.GetNextTokenType( TOKEN_STRING );
		sprintf( pszPath, "%s%s", pszRootDir, sr.GetTokenString() );

		if( !Load( pszPath, pTriggerList ) )
		{
			SUNLOG( eCRITICAL_LOG, "[에러]:트리거 파일로 부터 트리거 정보를 읽을 수 없습니다." );
			SASSERT( NULL, "[에러]:트리거 파일로 부터 트리거 정보를 읽을 수 없습니다." );
		}

		FASSERT( NULL == m_TriggerListForMap.GetData( pTriggerList->m_TriggerCode ) );
		m_TriggerListForMap.Add( pTriggerList, pTriggerList->m_TriggerCode );
	}

	sr.CloseFile();
	SUNLOG( eDEV_LOG, "Script File Load Complete, File = %s",  pszFullPath );

	return TRUE;
}

BYTE *TriggerParser::SetTriggerType( WORD Type, DWORD *pdwSize, eTRIGGER_TYPE eCondition )
{
	TRIGGER_TYPE	*pTRIGGER_TYPE = NULL;

	if( eCondition == eTRIGGER_CONDITION )
	{
		//	컨디션 트리거
		switch( Type )
		{
		case eTRIGGER_ALWAYS_CONDITION:	//	0
			{
				TRIGGER_ALWAYS_CONDITION	*pTRIGGER_ALWAYS_CONDITION = new TRIGGER_ALWAYS_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_ALWAYS_CONDITION;
				*pdwSize = sizeof( TRIGGER_ALWAYS_CONDITION );
			}
			break;

		case eTRIGGER_COMPARE_SWITCH_CONDITION:	//	1
			{
				TRIGGER_COMPARE_SWITCH_CONDITION	*pTRIGGER_SWITCH_CONDITION = new TRIGGER_COMPARE_SWITCH_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_SWITCH_CONDITION;
				*pdwSize = sizeof( TRIGGER_COMPARE_SWITCH_CONDITION );
			}
			break;

		case eTRIGGER_CLICK_OBJECT_CONDITION:	//	2
			{
				TRIGGER_CLICK_OBJECT_CONDITION	*pTRIGGER_CLICK_OBJECT_CONDITION = new TRIGGER_CLICK_OBJECT_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_CLICK_OBJECT_CONDITION;
				*pdwSize = sizeof( TRIGGER_CLICK_OBJECT_CONDITION );
			}
			break;

		case eTRIGGER_DAMAGE_OBJECT_CONDITION:	//	3
			{
				TRIGGER_DAMAGE_OBJECT_CONDITION	*pTRIGGER_DAMAGE_OBJECT_CONDITION = new TRIGGER_DAMAGE_OBJECT_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_DAMAGE_OBJECT_CONDITION;
				*pdwSize = sizeof( TRIGGER_DAMAGE_OBJECT_CONDITION );
			}
			break;

		case eTRIGGER_COUNT_NPCKILL_CONDITION:	//	4
			{
				TRIGGER_COUNT_NPCKILL_CONDITION	*pTRIGGER_COUNT_NPCKILL_CONDITION = new TRIGGER_COUNT_NPCKILL_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_COUNT_NPCKILL_CONDITION;
				*pdwSize = sizeof( TRIGGER_COUNT_NPCKILL_CONDITION );
			}
			break;

		case eTRIGGER_PASS_TIME_CONDITION:	//	5
			{
				TRIGGER_PASS_TIME_CONDITION	*pTRIGGER_PASS_TIME_CONDITION = new TRIGGER_PASS_TIME_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_PASS_TIME_CONDITION;
				*pdwSize = sizeof( TRIGGER_PASS_TIME_CONDITION );
			}
			break;

		case eTRIGGER_ENTER_AREA_CONDITION:	//	6
			{
				TRIGGER_ENTER_AREA_CONDITION	*pTRIGGER_ENTER_AREA_CONDITION = new TRIGGER_ENTER_AREA_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_ENTER_AREA_CONDITION;
				*pdwSize = sizeof( TRIGGER_ENTER_AREA_CONDITION );
			}
			break;

		case eTRIGGER_OBTAIN_ITEM_CONDITION:	//	7
			{
				TRIGGER_OBTAIN_ITEM_CONDITION	*pTRIGGER_OBTAIN_ITEM_CONDITION = new TRIGGER_OBTAIN_ITEM_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_OBTAIN_ITEM_CONDITION;
				*pdwSize = sizeof( TRIGGER_OBTAIN_ITEM_CONDITION );
			}
			break;

		case eTRIGGER_MULTI_SWITCH_CONDITION:	//	8
			{
				TRIGGER_MULTI_SWITCH_CONDITION	*pTRIGGER_MULTI_SWITCH_CONDITION = new TRIGGER_MULTI_SWITCH_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_MULTI_SWITCH_CONDITION;
				*pdwSize = sizeof( TRIGGER_MULTI_SWITCH_CONDITION );
			}
			break;

		case eTRIGGER_MISSION_POINT_CONDITION:	//	9
			{
				TRIGGER_MISSION_POINT_CONDITION	*pTRIGGER_MISSION_POINT_CONDITION = new TRIGGER_MISSION_POINT_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_MISSION_POINT_CONDITION;
				*pdwSize = sizeof( TRIGGER_MISSION_POINT_CONDITION );
			}
			break;
		case eTRIGGER_VARIABLE_COMPARE_CONDITION:	//	10
			{
				TRIGGER_VARIABLE_COMPARE_CONDITION	*pTRIGGER = new TRIGGER_VARIABLE_COMPARE_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_VARIABLE_COMPARE_CONDITION );
			}
			break;
		case eTRIGGER_TEST_QUEST_FLAG_CONDITION:	//	11
			{
				TRIGGER_TEST_QUEST_FLAG_CONDITION	*pTRIGGER = new TRIGGER_TEST_QUEST_FLAG_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_TEST_QUEST_FLAG_CONDITION );
			}
			break;
		case eTRIGGER_USER_CONDITION:	//	12
			{
				TRIGGER_USER_CONDITION	*pTRIGGER = new TRIGGER_USER_CONDITION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_USER_CONDITION );
			}
			break;
		case eTRIGGER_MONSTER_KILL:		// 13
			{
				TRIGGER_MONSTER_KILL	*pTRIGGER = new TRIGGER_MONSTER_KILL;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_MONSTER_KILL );
			}
			break;
        // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        case eTRIGGER_NPC_ENTER_AREA_CONDITION: // 14
            {
                pTRIGGER_TYPE = new TRIGGER_NPC_ENTER_AREA_CONDITION;
                *pdwSize = sizeof(TRIGGER_NPC_ENTER_AREA_CONDITION);
            }
            break;
        //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
        case eTRIGGER_COMPLETE_COLLECTION_CONDITION: // 15
            {
                pTRIGGER_TYPE = new TRIGGER_COMPLETE_COLLECTION_CONDITION;
                *pdwSize = sizeof(TRIGGER_COMPLETE_COLLECTION_CONDITION);
            }
            break;
		default:
			{
				FASSERT(!"읽을수 없는 조건 트리거입니다.");
				return NULL;
			}
		}
	}
	else if( eCondition == eTRIGGER_ACTION )
	{
		//	액션 트리거
		switch( Type )
		{
		case eTRIGGER_CHANGE_SWITCH_ACTION:	//	1000
			{
				TRIGGER_CHANGE_SWITCH_ACTION	*pTRIGGER_CHANGE_SWITCH_ACTION = new TRIGGER_CHANGE_SWITCH_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_CHANGE_SWITCH_ACTION;
				*pdwSize = sizeof( TRIGGER_CHANGE_SWITCH_ACTION );
			}
			break;

		case eTRIGGER_CHANGE_OBJECTANI_ACTION:	//	1001
			{
				TRIGGER_CHANGE_OBJECTANI_ACTION	*pTRIGGER_CHANGE_OBJECTANI_ACTION = new TRIGGER_CHANGE_OBJECTANI_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_CHANGE_OBJECTANI_ACTION;
				*pdwSize = sizeof( TRIGGER_CHANGE_OBJECTANI_ACTION );
			}
			break;

		case eTRIGGER_CHANGE_PATHTILE_ACTION:	//	1002
			{
				TRIGGER_CHANGE_PATHTILE_ACTION	*pTRIGGER_CHANGE_PATHTILE_ACTION = new TRIGGER_CHANGE_PATHTILE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_CHANGE_PATHTILE_ACTION;
				*pdwSize = sizeof( TRIGGER_CHANGE_PATHTILE_ACTION );
			}
			break;

		case eTRIGGER_CHANGE_OBJECTSTATE_ACTION:	//	1003
			{
				TRIGGER_CHANGE_OBJECTSTATE_ACTION	*pTRIGGER_CHANGE_OBJECTSTATE_ACTION = new TRIGGER_CHANGE_OBJECTSTATE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_CHANGE_OBJECTSTATE_ACTION;
				*pdwSize = sizeof( TRIGGER_CHANGE_OBJECTSTATE_ACTION );
			}
			break;

		case eTRIGGER_PLAY_EVENT_ACTION:	//	1004
			{
				TRIGGER_PLAY_EVENT_ACTION	*pTRIGGER_PLAY_EVENT_ACTION = new TRIGGER_PLAY_EVENT_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_PLAY_EVENT_ACTION;
				*pdwSize = sizeof( TRIGGER_PLAY_EVENT_ACTION );
			}
			break;

		case eTRIGGER_SET_ISATTACK_ACTION:	//	1005
			{
				TRIGGER_SET_ISATTACK_ACTION	*pTRIGGER_SET_ISATTACK_ACTION = new TRIGGER_SET_ISATTACK_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_SET_ISATTACK_ACTION;
				*pdwSize = sizeof( TRIGGER_SET_ISATTACK_ACTION );
			}
			break;

		case eTRIGGER_PORTAL_PLAYER_ACTION:	//	1006
			{
				TRIGGER_PORTAL_PLAYER_ACTION	*pTRIGGER_PORTAL_PLAYER_ACTION = new TRIGGER_PORTAL_PLAYER_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_PORTAL_PLAYER_ACTION;
				*pdwSize = sizeof( TRIGGER_PORTAL_PLAYER_ACTION );
			}
			break;

		case eTRIGGER_SET_OBJECTTHRUST_ACTION:	//	1007
			{
				TRIGGER_SET_OBJECTTHRUST_ACTION	*pTRIGGER_SET_OBJECTTHRUST_ACTION = new TRIGGER_SET_OBJECTTHRUST_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_SET_OBJECTTHRUST_ACTION;
				*pdwSize = sizeof( TRIGGER_SET_OBJECTTHRUST_ACTION );
			}
			break;

		case eTRIGGER_APPLY_DAMAGE_ACTION:	//	1008
			{
				TRIGGER_APPLY_DAMAGE_ACTION	*pTRIGGER_APPLY_DAMAGE_ACTION = new TRIGGER_APPLY_DAMAGE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_APPLY_DAMAGE_ACTION;
				*pdwSize = sizeof( TRIGGER_APPLY_DAMAGE_ACTION );
			}
			break;

		case eTRIGGER_REFLECT_DAMAGE_ACTION:	//	1009
			{
				TRIGGER_REFLECT_DAMAGE_ACTION	*pTRIGGER_REFLECT_DAMAGE_ACTION = new TRIGGER_REFLECT_DAMAGE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_REFLECT_DAMAGE_ACTION;
				*pdwSize = sizeof( TRIGGER_REFLECT_DAMAGE_ACTION );
			}
			break;

		case eTRIGGER_CREATE_MONSTER_ACTION:	//	1010
			{
				TRIGGER_CREATE_MONSTER_ACTION	*pTRIGGER_CREATE_MONSTER_ACTION = new TRIGGER_CREATE_MONSTER_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_CREATE_MONSTER_ACTION;
				*pdwSize = sizeof( TRIGGER_CREATE_MONSTER_ACTION );
			}
			break;

		case eTRIGGER_DISPLAY_MSGBOX_ACTION:	//	1011
			{
				TRIGGER_DISPLAY_MSGBOX_ACTION	*pTRIGGER_DISPLAY_MSGBOX_ACTION = new TRIGGER_DISPLAY_MSGBOX_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_DISPLAY_MSGBOX_ACTION;
				*pdwSize = sizeof( TRIGGER_DISPLAY_MSGBOX_ACTION );
			}
			break;

		case eTRIGGER_REMOVE_ITEM_ACTION:	//	1012
			{
				TRIGGER_REMOVE_ITEM_ACTION	*pTRIGGER_REMOVE_ITEM_ACTION = new TRIGGER_REMOVE_ITEM_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_REMOVE_ITEM_ACTION;
				*pdwSize = sizeof( TRIGGER_REMOVE_ITEM_ACTION );
			}
			break;

		case eTRIGGER_PRESERVER_ACTION:	//	1013
			{
				TRIGGER_PRESERVER_ACTION	*pTRIGGER_PRESERVER_ACTION = new TRIGGER_PRESERVER_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_PRESERVER_ACTION;
				*pdwSize = sizeof( TRIGGER_PRESERVER_ACTION );
			}
			break;

		case eTRIGGER_REWARD_PLAYER_ACTION:	//	1014
			{
				TRIGGER_REWARD_PLAYER_ACTION	*pTRIGGER_REWARD_PLAYER_ACTION = new TRIGGER_REWARD_PLAYER_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_REWARD_PLAYER_ACTION;
				*pdwSize = sizeof( TRIGGER_REWARD_PLAYER_ACTION );
			}
			break;

		case eTRIGGER_CLEAR_MISSION_ACTION:	//	1015
			{
				TRIGGER_CLEAR_MISSION_ACTION	*pTRIGGER_CLEAR_MISSION_ACTION = new TRIGGER_CLEAR_MISSION_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_CLEAR_MISSION_ACTION;
				*pdwSize = sizeof( TRIGGER_CLEAR_MISSION_ACTION );
			}
			break;

		case eTRIGGER_PORTAL_RANDOM_ACTION:	//	1016
			{
				TRIGGER_PORTAL_RANDOM_ACTION	*pTRIGGER_PORTAL_RANDOM_ACTION = new TRIGGER_PORTAL_RANDOM_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_PORTAL_RANDOM_ACTION;
				*pdwSize = sizeof( TRIGGER_PORTAL_RANDOM_ACTION );
			}
			break;
		case eTRIGGER_ACTIVATE_TRIGGER_ACTION:	//	1017
			{
				TRIGGER_ACTIVATE_TRIGGER_ACTION	*pTRIGGER_ACTIVATE_TRIGGER_ACTION = new TRIGGER_ACTIVATE_TRIGGER_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_ACTIVATE_TRIGGER_ACTION;
				*pdwSize = sizeof( TRIGGER_ACTIVATE_TRIGGER_ACTION );
			}
			break;
		case eTRIGGER_AREA_DAMAGE_ACTION:	//	1018
			{
				TRIGGER_AREA_DAMAGE_ACTION	*pTRIGGER_AREA_DAMAGE_ACTION = new TRIGGER_AREA_DAMAGE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_AREA_DAMAGE_ACTION;
				*pdwSize = sizeof( TRIGGER_AREA_DAMAGE_ACTION );
			}
			break;
		case eTRIGGER_OPERATE_SWITCH_ACTION:	//	1019
			{
				TRIGGER_OPERATE_SWITCH_ACTION	*pTRIGGER_OPERATE_SWITCH_ACTION = new TRIGGER_OPERATE_SWITCH_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER_OPERATE_SWITCH_ACTION;
				*pdwSize = sizeof( TRIGGER_OPERATE_SWITCH_ACTION );
			}
			break;

		case eTRIGGER_OPERATE_VARIABLE_ACTION:	//	1020
			{
				TRIGGER_OPERATE_VARIABLE_ACTION	*pTRIGGER = new TRIGGER_OPERATE_VARIABLE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_OPERATE_VARIABLE_ACTION );
			}
			break;

		case eTRIGGER_AREA_CHANGE_PATHTILE_ACTION: //        = 1021,	// 영역 타일 속성변경 
			{
				TRIGGER_AREA_CHANGE_PATHTILE_ACTION	*pTRIGGER = new TRIGGER_AREA_CHANGE_PATHTILE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_AREA_CHANGE_PATHTILE_ACTION );
			}
			break;

		case eTRIGGER_CREATE_MONSTER_WITH_DIRECTION:  //    = 1022,	// 몬스터 생성 (방향)
			{
				TRIGGER_CREATE_MONSTER_WITH_DIRECTION	*pTRIGGER = new TRIGGER_CREATE_MONSTER_WITH_DIRECTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_CREATE_MONSTER_WITH_DIRECTION );
			}
			break;

		case eTRIGGER_CHANGE_LIGHT:                  //     = 1023,	// 광원 설정 변경
			{
				TRIGGER_CHANGE_LIGHT	*pTRIGGER = new TRIGGER_CHANGE_LIGHT;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_CHANGE_LIGHT );
			}
			break;

		case eTRIGGER_CHANGE_OBJECT_TYPE:              //   = 1024,	// 오브젝트 상태를 바꾼다.
			{
				TRIGGER_CHANGE_OBJECT_TYPE	*pTRIGGER = new TRIGGER_CHANGE_OBJECT_TYPE;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_CHANGE_OBJECT_TYPE );
			}
			break;	

		case eTRIGGER_RANDOMMAP_MOVE:					// = 1025
			{
				TRIGGER_RANDOMMAP_MOVE	*pTRIGGER = new TRIGGER_RANDOMMAP_MOVE;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_RANDOMMAP_MOVE );
			}
			break;

		case eTRIGGER_DROP_TREASURE:					// = 1026
			{
				TRIGGER_DROP_TREASURE	*pTRIGGER = new TRIGGER_DROP_TREASURE;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_DROP_TREASURE );
			}
			break;

		case eTRIGGER_QUEST_CONDITION_COMPLETE_ACTION:					// = 1027
			{
				TRIGGER_QUEST_CONDITION_COMPLETE_ACTION	*pTRIGGER = new TRIGGER_QUEST_CONDITION_COMPLETE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_QUEST_CONDITION_COMPLETE_ACTION );
			}
			break;

		case eTRIGGER_GIVE_QUEST_ACTION :					// = 1028
			{
				TRIGGER_GIVE_QUEST_ACTION	*pTRIGGER = new TRIGGER_GIVE_QUEST_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_GIVE_QUEST_ACTION );
			}
			break;

		case eTRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION :  					// = 1029
			{
				TRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION	*pTRIGGER = new TRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION );

			}
			break;

		case eTRIGGER_USER_ACTION :     					// = 1030                
			{
				TRIGGER_USER_ACTION	*pTRIGGER = new TRIGGER_USER_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_USER_ACTION );
			}
			break;

		case eTRIGGER_USE_CATEGORY_ACTION  :   					// = 1031        
			{
				TRIGGER_USE_CATEGORY_ACTION	*pTRIGGER = new TRIGGER_USE_CATEGORY_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_USE_CATEGORY_ACTION );
			}
			break;

		case eTRIGGER_SET_CANNOT_MOVE_ACTION  :    					// = 1032     
			{
				TRIGGER_SET_CANNOT_MOVE_ACTION	*pTRIGGER = new TRIGGER_SET_CANNOT_MOVE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_SET_CANNOT_MOVE_ACTION );
			}
			break;

		case eTRIGGER_CREATE_MONSTER_GROUP_ACTION  :    					// = 1033
			{
				TRIGGER_CREATE_MONSTER_GROUP_ACTION	*pTRIGGER = new TRIGGER_CREATE_MONSTER_GROUP_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_CREATE_MONSTER_GROUP_ACTION );
			}
			break;

		case eTRIGGER_NOTICE_MESSAGE_BOX_ACTION   :     					// = 1034
			{
				TRIGGER_NOTICE_MESSAGE_BOX_ACTION	*pTRIGGER = new TRIGGER_NOTICE_MESSAGE_BOX_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_NOTICE_MESSAGE_BOX_ACTION );
			}
			break;

		case eTRIGGER_SHOW_MONSTERUNIT_ACTION:					// = 1035
			{
				TRIGGER_SHOW_MONSTERUNIT_ACTION		*pTRIGGER = new TRIGGER_SHOW_MONSTERUNIT_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_SHOW_MONSTERUNIT_ACTION );
			}
			break;

		case eTRIGGER_SHOW_MESSAGE_BOX_ACTION:					// = 1036
			{
				TRIGGER_SHOW_MESSAGE_BOX_ACTION		*pTRIGGER = new TRIGGER_SHOW_MESSAGE_BOX_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_SHOW_MESSAGE_BOX_ACTION );				
			}
			break;

		case eTRIGGER_SKILL_GENERATE_ACTION:					// = 1037
			{
				TRIGGER_SKILL_GENERATE_ACTION		*pTRIGGER = new TRIGGER_SKILL_GENERATE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_SKILL_GENERATE_ACTION );
			}
			break;

		case eTRIGGER_TERRITORY_MOVE_ACTION:					// = 1038
			{
				TRIGGER_TERRITORY_MOVE_ACTION		*pTRIGGER = new TRIGGER_TERRITORY_MOVE_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE *)pTRIGGER;
				*pdwSize = sizeof( TRIGGER_TERRITORY_MOVE_ACTION );
			}
			break;

		case eTRIGGER_MON_RANDOM_SPAWN_ACTION:					// = 1039
			{
				TRIGGER_MON_RANDOM_SPAWN_ACTION*	pTRIGGER = new TRIGGER_MON_RANDOM_SPAWN_ACTION;
				pTRIGGER_TYPE = (TRIGGER_TYPE*)pTRIGGER;
				*pdwSize      = sizeof(TRIGGER_MON_RANDOM_SPAWN_ACTION);
			}
			break;
		case eTRIGGER_SET_LAYER:								// = 1040
			{
				TRIGGER_SET_LAYER*	pTRIGGER = new TRIGGER_SET_LAYER;
				pTRIGGER_TYPE = (TRIGGER_TYPE*)pTRIGGER;
				*pdwSize      = sizeof(TRIGGER_SET_LAYER);
			}
			break;
		default:
			{
				FASSERT(!"읽을수 없는 액션 트리거입니다.");
				return NULL;
		    }		
		}
	}
	else
	{
		return NULL;
	}

	return (BYTE *)pTRIGGER_TYPE;
}

MapTrigger *TriggerParser::Load(CWzArchive *pArchive)
{
	St_ChunkInfo ChunkInfo;
	if( pArchive->GetVersion() >= 145 )
	{
		BOOL bTriggerChunk = FALSE;
		while( TRUE )
		{
			ChunkInfo = pArchive->ReadChunk();
			if( ChunkInfo.m_nID == 0x1786 )
			{
				bTriggerChunk = TRUE;
				break;
			}
			pArchive->SkipCurrentChunk(ChunkInfo);
		}
		FASSERT( TRUE == bTriggerChunk );

		MapTrigger * pMapTrigger = new MapTrigger;		
		WORD wNumberOfTriggerGroup	= 0;
		
		pMapTrigger->Load(pArchive);
		
		return pMapTrigger;
	}
	else
	{
		return NULL;
	}
}

MapTrigger::MapTrigger()
{
	m_wGroupNum = 0;;
	m_TriggerGroupList.clear();
}

MapTrigger::~MapTrigger()
{
	Destroy();
}

void MapTrigger::Destroy()
{
	TriggerGroup_iterator iter = m_TriggerGroupList.begin();

	while(iter != m_TriggerGroupList.end()) 
    {
		sTRIGGERGROUP *pGroup = *iter;
		pGroup->Destroy();
		delete pGroup;
		iter++;
	}

	m_TriggerGroupList.clear();
	m_wGroupNum = 0;
}

int MapTrigger::GetGroupNum()
{
	return m_wGroupNum;
}

void MapTrigger::Load(CWzArchive *pArchive)
{	
	*pArchive >> m_wGroupNum;

	// Last Index는 필요없으니 건너뛴다.
	if (pArchive->GetVersion() >= 160)
	{
		int lastindex;
		*pArchive >> lastindex;
	}

	m_TriggerGroupList.clear();
		
	for( DWORD idx = 0 ; idx < m_wGroupNum ; ++idx )
	{
		sTRIGGERGROUP *pTriggerGroup = new sTRIGGERGROUP;

		pTriggerGroup->Load(pArchive);
		
		m_TriggerGroupList.push_back(pTriggerGroup);
	}		
}

sTRIGGERGROUP * MapTrigger::GetGroup(int iGroupIndex)
{
	TriggerGroup_vector::iterator iter;
	int index = 0;

	iter = m_TriggerGroupList.begin();
	while (iter != m_TriggerGroupList.end())
	{
		if (index == iGroupIndex)
		{
			return (*iter);
		}
		index++;
		iter++;
	}

	// 모든 경로에서 값을 반환하지 않는다! taiyo
	return NULL;
}

void sTRIGGERGROUP::Load(CWzArchive *pArchive)
{
	m_TriggerCategoryList.clear();

	// 그룹 이름 읽기.
	BYTE bNameLength;
	*pArchive >> bNameLength;

	ZeroMemory(m_szName,MAX_TRIGGER_GROUPNAME_LENGTH);
	pArchive->Read(m_szName,bNameLength);

	// 카테고리 갯수읽기.
	*pArchive >> m_wNumberOfTriggerCategory;

	// Last Index는 필요없으니 건너뛴다.
	if (pArchive->GetVersion() >= 160)
	{
		int lastindex;
		*pArchive >> lastindex;
	}

	for( DWORD idx = 0 ; idx < m_wNumberOfTriggerCategory ; ++idx )
	{
		sTRIGGERCATEGORY *pTriggerCategory = new sTRIGGERCATEGORY;

		pTriggerCategory->Load(pArchive);
		
		m_TriggerCategoryList.push_back(pTriggerCategory);
	}
}

void sTRIGGERGROUP::Destroy()
{
	TriggerCategory_iterator iter = m_TriggerCategoryList.begin();

	while(iter != m_TriggerCategoryList.end()) 
    {
		sTRIGGERCATEGORY * pCategory = *iter;
		pCategory->Destroy();
		delete pCategory;
		iter++;
	}

	m_TriggerCategoryList.clear();	
}

void sTRIGGERCATEGORY::Load(CWzArchive *pArchive)
{
	m_TriggerListVector.clear();

	// 카테고리 이름 읽기.
	BYTE bNameLength;
	*pArchive >> bNameLength;

	ZeroMemory(m_szName,MAX_TRIGGER_CATEGORYNAME_LENGTH);
	pArchive->Read(m_szName,bNameLength);

	// 리스트 갯수읽기.
	*pArchive >> m_wNumberOfTriggerList;

	// Last Index는 필요없으니 건너뛴다.
	if (pArchive->GetVersion() >= 160)
	{
		int lastindex;
		*pArchive >> lastindex;
	}

	for( DWORD idx = 0 ; idx < m_wNumberOfTriggerList ; ++idx )
	{
		sTRIGGER_CONTAINER *pTriggerList = new sTRIGGER_CONTAINER;

		pTriggerList->Load(pArchive);
		
		m_TriggerListVector.push_back(pTriggerList);
	}
}


void sTRIGGERCATEGORY::Destroy()
{
	TriggerList_iterator iter = m_TriggerListVector.begin();

	while(iter != m_TriggerListVector.end()) 
    {
		sTRIGGER_CONTAINER *pContainer = *iter;
		pContainer->Destroy();
		delete pContainer;
		iter++;
	}

	m_TriggerListVector.clear();	
}


void sTRIGGER_CONTAINER::Load(CWzArchive *pArchive)
{
	m_ConditionTrigger.clear();
	m_ActionTrigger.clear();

	if (pArchive->GetVersion() >= 160)
	{
		int index;
		*pArchive >> index;
	}

	// ID
	*pArchive >> m_TriggerListID;

	// 속성
	*pArchive >> m_wTriggerListAtt;

	//	컨디션 트리거 개수
	*pArchive >> m_wNumConTrigger;

	// Last Index는 필요없으니 건너뛴다.
	if (pArchive->GetVersion() >= 160)
	{
		int lastindex;
		*pArchive >> lastindex;
	}

	WORD			wType;			//	트리거 타입 정보
	DWORD			dwSize;			//	타입에 따른 트리거 데이터 구조체 크기
	TRIGGER_TYPE *  pTRIGGER_TYPE;	//	트리거 리스트에 저장될 트리거 데이터
	BYTE *          pBuffer;
	int             iIDIndex;
	WORD            wOldTrigger = 0;

	//	개수 만큼 돌면서 컨디션 트리거 읽기
    WORD wNumConTrigger = 0;
	for( ; wNumConTrigger < m_wNumConTrigger ; ++wNumConTrigger )
	{
		//	컨디션 트리거 타입 읽기
		*pArchive >> wType;

		//	타입에 따라 데이터 읽기
		if (pArchive->GetVersion() >= 160)
		{
			*pArchive >> iIDIndex;
		}
		else
		{
			iIDIndex = wNumConTrigger;
		}

		pBuffer = TriggerParser::SetTriggerType( wType, &dwSize, eTRIGGER_CONDITION );			
		pArchive->Read(pBuffer + sizeof(WORD) + sizeof(int) ,dwSize - sizeof(WORD) - sizeof(int));

		pTRIGGER_TYPE = (TRIGGER_TYPE *)pBuffer;
		pTRIGGER_TYPE->m_iIDIndex = iIDIndex;

		//	읽은 컨디션 트리거 데이터 저장
		m_ConditionTrigger.push_back( pTRIGGER_TYPE );

		wOldTrigger = wType;
	}

	//	액션 트리거 개수
	*pArchive >> m_wNumActTrigger;

	// Last Index는 필요없으니 건너뛴다.
	if (pArchive->GetVersion() >= 160)
	{
		int lastindex;
		*pArchive >> lastindex;
	}	

	//	개수 만큼 돌면서 액션 트리거 읽기
	for( WORD wNumActTrigger = 0 ; wNumActTrigger < m_wNumActTrigger ; ++wNumActTrigger )
	{
		//	액션 트리거 타입 읽기
		*pArchive >> wType;

		if (pArchive->GetVersion() >= 160)
		{
			*pArchive >> iIDIndex;
		}
		else
		{
			iIDIndex = wNumConTrigger;
		}

		//	타입에 따라 데이터 읽기
		pBuffer = TriggerParser::SetTriggerType( wType, &dwSize, eTRIGGER_ACTION );
		pArchive->Read(pBuffer + sizeof(WORD) + sizeof(int), dwSize - sizeof(WORD) - sizeof(int));
		pTRIGGER_TYPE = (TRIGGER_TYPE *)pBuffer;

		//	읽은 컨디션 트리거 데이터 저장
		m_ActionTrigger.push_back( pTRIGGER_TYPE );
    #if defined(_DEBUG)
        // 에러체크용 코드 
		if (wType == 1016)
		{
			TRIGGER_PORTAL_RANDOM_ACTION * pTRIGGER_PORTAL_RANDOM_ACTION = (TRIGGER_PORTAL_RANDOM_ACTION *)pTRIGGER_TYPE;
		}
    #endif
        wOldTrigger = wType;
	}
}

void sTRIGGER_CONTAINER::Destroy()
{
	TriggerType_iterator iter = m_ConditionTrigger.begin();

	while(iter != m_ConditionTrigger.end()) 
    {
		TRIGGER_TYPE *pTrigger = *iter;		
		delete pTrigger;
		iter++;
	}

	m_ConditionTrigger.clear();

	iter = m_ActionTrigger.begin();

	while (iter != m_ActionTrigger.end()) 
    {
		TRIGGER_TYPE *pTrigger = *iter;		
		delete pTrigger;
		iter++;
	}

	m_ActionTrigger.clear();		
}
