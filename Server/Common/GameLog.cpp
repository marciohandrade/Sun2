#include "StdAfx.h"
#include ".\gamelog.h"
#using <mscorlib.dll>

CGameLog::CGameLog(void)
{
	m_bWriteText = TRUE;
}

CGameLog::~CGameLog(void)
{

}

BOOL CGameLog::Init( TCHAR* pszDirName, TCHAR* pszFilenamePrefix, TCHAR* pszTextDirName )
{
	BOOL bRet = m_LogMng.Init( pszDirName, pszFilenamePrefix, pszTextDirName );
	return bRet;
}


void CGameLog::WriteBaseData( BaseData* pData, TCHAR* szAccountID, TCHAR* szCharName )
{
	if( szAccountID == NULL )		_tcscpy( pData->szAccountID, "" );
	else							_tcscpy( pData->szAccountID, szAccountID );

	if( szCharName == NULL )		_tcscpy( pData->szCharName, "" );
	else							_tcscpy( pData->szCharName, szCharName );
}

//////////////////////////////////////////////////////////////////////////
// 경험치 로그 
// 1. 레벨업 로그
void CGameLog::WriteLevelUp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwOldExp, DWORD dwNewExp,
				  WORD wOldLevel, WORD wNewLevel )
{
	ExpData data;
	data.iExpCode = EXP_LEVELUP;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iOldExp = dwOldExp;
	data.iNewExp = dwNewExp;
	data.iOldLevel = wOldLevel;
	data.iNewLevel = wNewLevel;
	m_LogMng.WriteExp( data, m_bWriteText, NULL );
}


// 2. 경험치 10등분 로그
void CGameLog::WriteDivideExp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwOldExp, DWORD dwNewExp,
					  WORD wOldLevel, WORD wNewLevel, WORD wDivideCount )
{
	ExpData data;
	data.iExpCode = EXP_DIVIDECNT;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iDivideCount = wDivideCount;
	data.iOldExp = dwOldExp;
	data.iNewExp = dwNewExp;
	data.iOldLevel = wOldLevel;
	data.iNewLevel = wNewLevel;
	m_LogMng.WriteExp( data, m_bWriteText, NULL );
}


// 3. 경험치 추가/다운
void CGameLog::WriteDownExp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwTotalExp, DWORD dwChangeExp,
							WORD wLevel )
{
	// 현재의 인터페이스로는 불가능..
	// 현 플레이어의 전체 경험치와 다운된 경험치를 요구하는데, 현재 인자로는 불가능.
	// AddInfo에 삽입할 경우, 다운된 경험치를 넣을것인가?
	ExpData data;
	data.iExpCode = EXP_DOWN;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iNewExp = dwTotalExp;
	data.iNewLevel = wLevel;
	m_LogMng.WriteExp( data, m_bWriteText, "Change Exp = %d", dwChangeExp );
}


//////////////////////////////////////////////////////////////////////////
// 아이템 로그
// 1. 아이템 드롭 로그(버림과 죽어서 떨구는것과 몬스터 죽여서 떨구는 것의 차이?)
// 1. 아이템 드롭(땅에 떨굼) 과 줍기
void CGameLog::WriteItemOwner( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bOwner, int nItemCode, int nItemSerial
							  ,int nItemCount, int nMapCode, int nPosX, int nPosY, int nPosZ)

{
	ItemData data;
	if( bOwner == TRUE )  	data.iItemCode = ITEM_PICKUP;  // 줍기
	else					data.iItemCode = ITEM_CAST;	   // 버리기
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iItemBaseCode = nItemCode;
	data.iItemSeq = nItemSerial;
	data.iItemCount = nItemCount;
	data.siMapCode = nMapCode;
	data.siMapX = nPosX;
	data.siMapY = nPosY;
	data.siMapZ = nPosZ;
	m_LogMng.WriteItem( data, m_bWriteText, "" );
}

// 2. 인첸트 
void CGameLog::WriteEnchantResult( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bSuccess, int nItemCode, int nItemSerial, vector<COMPOSITEMATERIAL> vecMatInfo )
{
	ItemData data;
	if( bSuccess == TRUE)
		data.iItemCode = ITEM_ENCHANTSUCCESS;
	else
		data.iItemCode = ITEM_ENCHANTFAIL;

	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iItemBaseCode = nItemCode;
	data.iItemSeq = nItemSerial;
	
	int nSize = (int)vecMatInfo.size();

	string strInfo;

	for(int i = 0; i < nSize; i++)
	{
		COMPOSITEMATERIAL info = vecMatInfo[i];
		char szInfo[128];
		wsprintf( szInfo, "MATCODE = %d, MATNUM = %d  ",  info.nMaterialCode, info.wCntMaterial );
		strInfo += szInfo;
	}

	m_LogMng.WriteItem( data, m_bWriteText, strInfo.data() );
}

// 3. NPC와의 거래
void CGameLog::WriteItemTradeWithNPC( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bSell, int nItemCode,
									 int nItemSerial, int nItemCount, DWORD dwMoney )
{
	ItemData data;
	if( bSell == TRUE )  	data.iItemCode = ITEM_SELLNPC;	// NPC에게 팔기
	else					data.iItemCode = ITEM_BUYNPC;	// NPC에게 사기

	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iItemBaseCode = nItemCode;
	data.iItemSeq = nItemSerial;
	data.iItemCount = nItemCount;
	m_LogMng.WriteItem( data, m_bWriteText, " Money = %d ", dwMoney );
}


void CGameLog::WriteItemTradeWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bGive, 
										ITEMLOGINFO ItemInfo, TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName)
{
	ItemData data;
	if( bGive == TRUE )  	data.iItemCode = ITEM_GIVE;		// 상대 플레이어에게 주기
	else					data.iItemCode = ITEM_RECEIVE;	// 상대에게서 받기
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szPlayer1_ID, szPlayer1_CharName );
	data.iItemBaseCode = ItemInfo.nItemCode;
	data.iItemSeq = ItemInfo.nItemSerial;
	data.iItemCount = ItemInfo.nItemCount;

	if( szPlayer2_ID == NULL )		_tcscpy( data.szToUserID, "" );
	else							_tcscpy( data.szToUserID, szPlayer2_ID );

	if( szPlayer2_CharName == NULL )		_tcscpy(data.szToCharName, "");
	else 									_tcscpy( data.szToCharName, szPlayer2_CharName );

	m_LogMng.WriteItem( data, m_bWriteText, NULL );
}

// 5. 돈 개인거래
void CGameLog::WriteMoneyTradeWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bGive, DWORD dwMoney, 
							   TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName)
{
	ItemData data;
	if( bGive == TRUE )			// 상대 플레이어에게 주기
	{
		data.iItemCode = ITEM_MONEY;		
	}
	else						// 상대에게서 받기
	{
		data.iItemCode = ITEM_MONEY;		
	}

	data.iServerCode = nServerCode;
	WriteBaseData( &data, szPlayer1_ID, szPlayer1_CharName );

	if( szPlayer2_ID == NULL )		_tcscpy( data.szToUserID, "" );
	else							_tcscpy( data.szToUserID, szPlayer2_ID );

	if( szPlayer2_CharName == NULL )		_tcscpy(data.szToCharName, "");
	else 									_tcscpy( data.szToCharName, szPlayer2_CharName );

	if( bGive == TRUE )  //Player1이 Player2에게 돈을 dwMoney만큼 주다.
		m_LogMng.WriteItem( data, m_bWriteText, "Give Money  = %d", dwMoney );
	else				//Player1이 Player2에게서 돈을 dwMoney만큼 받다.
		m_LogMng.WriteItem( data, m_bWriteText, "Receive Money  = %d", dwMoney );
}

// 6. 개인상점 거래
void CGameLog::WriteItemVendorWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bSell, ITEMLOGINFO ItemInfo, 
							   DWORD dwMoney, TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName)
{
	ItemData data;
	if( bSell == TRUE )  	data.iItemCode = ITEM_SELLPC;	// 개인삼정에서 물건 팔기
	else					data.iItemCode = ITEM_BUYPC;	// 개인상점에서 물건 사기
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szPlayer1_ID, szPlayer1_CharName );
	data.iItemBaseCode = ItemInfo.nItemCode;
	data.iItemSeq = ItemInfo.nItemSerial;
	data.iItemCount = ItemInfo.nItemCount;

	if( szPlayer2_ID == NULL )		_tcscpy( data.szToUserID, "" );
	else							_tcscpy( data.szToUserID, szPlayer2_ID );

	if( szPlayer2_CharName == NULL )		_tcscpy(data.szToCharName, "");
	else 									_tcscpy( data.szToCharName, szPlayer2_CharName );

	if( bSell == TRUE )
		m_LogMng.WriteItem( data, m_bWriteText, "RECEIVE MONEY = %d", dwMoney );
	else
		m_LogMng.WriteItem( data, m_bWriteText, "PAY MONEY = %d", dwMoney );

}

// 7. 아이템 랭크업
void CGameLog::WriteItemRankUpResult( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, ITEMLOGINFO ItemInfo, 
					 DWORD dwMoney, BOOL bCreateSocket )
{
	ItemData data;
	data.iItemCode = ITEM_RANKUPSUCCESS;	// 랭크업 성공
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iItemBaseCode = ItemInfo.nItemCode;
	data.iItemSeq = ItemInfo.nItemSerial;
	data.iItemCount = ItemInfo.nItemCount;
	if( bCreateSocket )
		m_LogMng.WriteItem( data, m_bWriteText, "소켓 생성 됨" );
	else
		m_LogMng.WriteItem( data, m_bWriteText, "소켓 생성 안됨" );

}

// 8. 창고에 맞기기 / 찾기
// dwMoney : 창고 보관료
void CGameLog::WriteItemMoveInWareHouse( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bKeep, ITEMLOGINFO ItemInfo )
{
	ItemData data;
	if( bKeep == TRUE )  	data.iItemCode = ITEM_PUTSTORAGE;	// 창고에 보관
	else					data.iItemCode = ITEM_GETSTORAGE;	// 창고에서 꺼내기
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iItemBaseCode = ItemInfo.nItemCode;
	data.iItemSeq = ItemInfo.nItemSerial;
	data.iItemCount = ItemInfo.nItemCount;
	m_LogMng.WriteItem( data, m_bWriteText, NULL );
}

// 9. 인첸트 시도
void CGameLog::WriteEnchantStart( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, vector<COMPOSITEMATERIAL> vecMatInfo )
{
	ItemData data;
	data.iItemCode = ITEM_ENCHANTTRY;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	int nSize = (int)vecMatInfo.size();
	string strInfo;
	for(int i = 0; i < nSize; i++)
	{
		COMPOSITEMATERIAL info = vecMatInfo[i];
		char szInfo[128];
		wsprintf( szInfo, "MATCODE = %d, MATNUM = %d  ",  info.nMaterialCode, info.wCntMaterial );
		strInfo += szInfo;
	}

	m_LogMng.WriteItem( data, m_bWriteText, strInfo.data() );
}

// 10. 몬스터 아이템 드랍
void CGameLog::WriteMonsterItemDrop( int nServerCode, ITEMLOGINFO ItemInfo, DWORD dwMonsterCode, DWORD dwMonsterLevel, int nMapCode,
					   int nPosX, int nPosY, int nPosZ )
{
	ItemData data;
	data.iItemCode = ITEM_MONSTER_ITEMDROP;
	data.iServerCode = nServerCode;
	data.iItemBaseCode = ItemInfo.nItemCode;
	data.iItemSeq = ItemInfo.nItemSerial;
	data.iItemCount = ItemInfo.nItemCount;
	data.siMapCode = nMapCode;
	data.siMapX = nPosX;
	data.siMapY = nPosY;
	data.siMapZ = nPosZ;
	m_LogMng.WriteItem( data, m_bWriteText, "MONSTER CODE = %d, LEVEL = %d", dwMonsterCode, dwMonsterLevel );
}

// 11. 아이템 개인거래 요청 / 승인 / 요청거부
// nType (0 : 거래 요청, 1 : 승인, 2 : 거절)
void CGameLog::WriteItemTradeStatus( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, TCHAR* szPlayer2_ID,
						  TCHAR* szPlayer2_CharName, int nType )
{
	ItemData data;
	if( nType == 0 )				// 아이템 거래 요청
		data.iItemCode = ITEM_REQUEST;
	else if( nType == 1 )			// 승인
		data.iItemCode = ITEM_ACCEPT;
	else if( nType == 2 )			// 거절
		data.iItemCode = ITEM_REJECT;
	else
		return;

	data.iServerCode = nServerCode;
	WriteBaseData( &data, szPlayer1_ID, szPlayer1_CharName );
	
	if( szPlayer2_ID == NULL )			_tcscpy( data.szToUserID, "" );
	else								_tcscpy( data.szToUserID, szPlayer2_ID );
	if( szPlayer2_CharName == NULL )	_tcscpy( data.szToCharName, "" );
	else								_tcscpy( data.szToCharName, szPlayer2_CharName );

	m_LogMng.WriteItem( data, m_bWriteText, NULL );

}

// 12. 개인상점 개설/폐쇄
void CGameLog::WriteItemVendorStatus( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bOpen, DWORD dwMoney, 
						   int nMapCode, int nPosX, int nPosY, int nPosZ )

{
	ItemData data;
	if( bOpen == TRUE )
		data.iItemCode = ITEM_NEWPC;
	else
		data.iItemCode = ITEM_DELPC;

	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.siMapCode = nMapCode;
	data.siMapX = nPosX;
	data.siMapY = nPosY;
	data.siMapZ = nPosZ;
	m_LogMng.WriteItem( data, m_bWriteText, "Money = %d", dwMoney );
}

// 13. 개인상점 보기
void CGameLog::WriteViewItemVendor( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, TCHAR* szPlayer2_ID,
						 TCHAR* szPlayer2_CharName )
{
	ItemData data;
	data.iItemCode = ITEM_WATCHPC;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szPlayer1_ID, szPlayer1_CharName );
	if( szPlayer2_ID == NULL )			_tcscpy( data.szToUserID, "" );
	else								_tcscpy( data.szToUserID, szPlayer2_ID );
	if( szPlayer2_CharName == NULL )	_tcscpy( data.szToCharName, "" );
	else								_tcscpy( data.szToCharName, szPlayer2_CharName );
	m_LogMng.WriteItem( data, m_bWriteText, NULL );
}


// 14. 창고 정보
void CGameLog::WriteWareHouseInfo( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, vector<ITEMLOGINFO> vecItem, 
								  DWORD dwMoney )
{
	ItemData data;
	data.iItemCode = ITEM_WATCHPC;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	string strInfo;
	int nSize = (int)vecItem.size();
	for(int i = 0; i < nSize ; i++)
	{
		ITEMLOGINFO item = vecItem[i];
		TCHAR szInfo[128];
		wsprintf( szInfo, "ITEM_CODE = %d, ITEM_SERIAL = %d, ITEM_COUNT = %d, ", item.nItemCode, item.nItemSerial,
			item.nItemCount );
		strInfo += szInfo;
	}

	TCHAR szMoney[128];
	wsprintf( szMoney, "MONEY = %d ", dwMoney );
	strInfo += szMoney;

	m_LogMng.WriteItem( data, m_bWriteText, strInfo.data() );
}


// 15. 개인상점 정보(가격설정)
void CGameLog::WriteVendorInfo( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, vector<ITEMLOGINFO> vecItem )
{
	ItemData data;
	data.iItemCode = ITEM_SETPRICE_PC;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	string strInfo;
	int nSize = (int)vecItem.size();
	for(int i = 0; i < nSize ; i++)
	{
		ITEMLOGINFO item = vecItem[i];
		TCHAR szInfo[128];
		wsprintf( szInfo, "ITEM_CODE = %d, ITEM_SERIAL = %d, ITEM_COUNT = %d, ITEM_MONEY = %d", item.nItemCode, item.nItemSerial,
			item.nItemCount, item.dwMoney );
		strInfo += szInfo;
	}

	m_LogMng.WriteItem( data, m_bWriteText, strInfo.data() );
}





//////////////////////////////////////////////////////////////////////////
// 유저액션 로그
//1. 부활 로그
void CGameLog::WritePlayerResurrection( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwExp, int nMapCode, int nPosX, 
					   int nPosY, int nPosZ )
{
	ActionData data;
	data.iActionCode = ACT_REBIRTH_CHAR;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.siMapCode = nMapCode;
	data.siMapX = nPosX;
	data.siMapY = nPosY;
	data.siMapZ = nPosZ;
	m_LogMng.WriteUserAction( data, m_bWriteText, NULL );
}

void CGameLog::WritePlayerDead( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwExp, int nMapCode, int nPosX, 
				   int nPosY, int nPosZ )
{
	ActionData data;
	data.iActionCode = ACT_DEAD_CHAR;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.siMapCode = nMapCode;
	data.siMapX = nPosX;
	data.siMapY = nPosY;
	data.siMapZ = nPosZ;
	m_LogMng.WriteUserAction( data, m_bWriteText, NULL );
}


void CGameLog::WriteInvenInfoOnLogin( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode )
{
	ActionData data;
	data.iActionCode = ACT_INVENLOGIN;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteUserAction( data, m_bWriteText, NULL );
	// 인벤정보를 어떻게 남길 것인가? 
}

void CGameLog::WriteUseStat( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, byte byStatType )
{
	ActionData data;
	data.iActionCode = ACT_USE_STAT;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteUserAction( data, m_bWriteText, " STATCODE = %d", byStatType );
}

void CGameLog::WriteCharacter( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bCreate )
{
	ActionData data;
	if( bCreate == TRUE )
		data.iActionCode = ACT_CREATE_CHAR;
	else
		data.iActionCode = ACT_DELETE_CHAR;

	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteUserAction( data, m_bWriteText, NULL );
}

// 6. 마을로 이동
void CGameLog::WriteMoveVillage( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, int nMapCode )
{
	ActionData data;
	data.iActionCode = ACT_MOVE_TOWN;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.siMapCode = nMapCode;
	m_LogMng.WriteUserAction( data, m_bWriteText, NULL );
}


// 7. 스킬레벨업
void CGameLog::WriteSkillLevelUp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, int nSkillCode, BOOL bLevelUp )
{
	ActionData data;
	if( bLevelUp == FALSE )  //레벨업이 아니면, 처음 스킬을 배우는 거다.
		data.iActionCode = ACT_ACQUIRE_SKILL;
	else
		data.iActionCode = ACT_SKILLLV_UP;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );

	m_LogMng.WriteUserAction( data, m_bWriteText, "SKILL CODE = %d", nSkillCode );
}



//////////////////////////////////////////////////////////////////////////
// 세션 로그 
//1. 플레이어 로그인 ]
void CGameLog::WritePlayerLogin( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, TCHAR* szIP,
								WORD wClass, WORD wLevel )
{
	SessionData data;
	data.iConnectCode = CONN_LOGIN;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	if( szIP == NULL )		_tcscpy( data.szClientIP, "" );
	else					_tcscpy( data.szClientIP, szIP );
	m_Time.GetTime( 3, data.szLoginTime, 16 );
	data.byClassCode = (BYTE)wClass;
	data.siLevel = wLevel;
	m_LogMng.WriteSession( data, m_bWriteText, NULL );
}


//2. 플레이어 로그인 실패
void CGameLog::WritePlayerLoginFail( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, TCHAR* szIP )
{
	SessionData data;
	data.iConnectCode = CONN_LOGIN_FAIL;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	if( szIP == NULL )		_tcscpy( data.szClientIP, "" );
	else					_tcscpy( data.szClientIP, szIP );
	m_Time.GetTime( 3, data.szLoginTime, 16 );
	m_LogMng.WriteSession( data, m_bWriteText, NULL );
}

// 3. 로그아웃
void CGameLog::WritePlayerLogOut( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, TCHAR* szIP, TCHAR* szLoginTime,
					   WORD wClass, WORD wLevel )
{
	SessionData data;
	data.iConnectCode = CONN_LOGOUT;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );

	if( szIP == NULL )		_tcscpy( data.szClientIP, "" );
	else					_tcscpy( data.szClientIP, szIP );

	if( szLoginTime == NULL )		_tcscpy( data.szLoginTime, "" );
	else							_tcscpy( data.szLoginTime, szLoginTime );

	m_Time.GetTime( 3, data.szLogoutTime, 16 );

	//로그인 타임과 로그아웃 타임을 계산하고, 사용시간을 남긴다.
	//다음 함수는 타입 3, 4를 모두 사용할수 있다.
	if( data.szLogoutTime != NULL )
	{
		int nUsedTime = WZTime::GetDiffTimeType4( data.szLoginTime, data.szLogoutTime );
		data.iUseTime = nUsedTime;
	}

	data.byClassCode = (BYTE)wClass;
	data.siLevel = wLevel;
	m_LogMng.WriteSession( data, m_bWriteText, NULL );
}


// 4. 캐릭터 접속 / 종료
void CGameLog::WriteCharConnect( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bConnect )
{
	SessionData data;
	if( bConnect == TRUE ) //접속
		data.iConnectCode = CONN_CHAR_CONNECT;
	else
		data.iConnectCode = CONN_CHAR_DISCONNECT;

	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteSession( data, m_bWriteText, NULL );
}

//////////////////////////////////////////////////////////////////////////
// 채팅 로그 
// 1. 마을 채팅
void CGameLog::WriteVillageChat( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, short sMapCode, TCHAR* szChatMsg )
{
	ChatData data;
	data.iChannel = nServerCode;		// 채널명
	data.byChatCode = CHAT_EYERANGE;	// 시야 범위 채팅이다
	data.siMapCode = sMapCode;
	memcpy( data.szContext, szChatMsg, sizeof(TCHAR) * 100 );

	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteChat( data, m_bWriteText, NULL );
}

// 2. 외치기
void CGameLog::WriteShoutChat( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, short sMapCode, TCHAR* szChatMsg )
{
	ChatData data;
	data.iChannel = nServerCode;
	data.byChatCode = CHAT_SHOUT;
	data.siMapCode = sMapCode;
	memcpy( data.szContext, szChatMsg, sizeof(TCHAR) * 100 );

	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteChat( data, m_bWriteText, NULL );
}

// 3. 배틀존 채팅
void CGameLog::WriteBattleZoneChat( TCHAR* szAccountID, TCHAR* szCharName, int nRoomNo, int nServerCode, short sMapCode, TCHAR* szChatMsg )
{
	ChatData data;
	data.iChannel = nServerCode;
	data.byChatCode = CHAT_SHOUT;
	data.siMapCode = sMapCode;
	memcpy( data.szContext, szChatMsg, sizeof(TCHAR) * 100 );
	data.iRoomNo = nRoomNo;

	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteChat( data, m_bWriteText, NULL );
}

// 4. 길드 채팅
void CGameLog::WriteGuildChat( TCHAR* szAccountID, TCHAR* szCharName, TCHAR* szGuildName, int nServerCode, short sMapCode, TCHAR* szChatMsg )
{
	ChatData data;
	data.iChannel = nServerCode;
	data.byChatCode = CHAT_SHOUT;
	memcpy( data.szContext, szChatMsg, sizeof(TCHAR) * 100 );
	memcpy( data.szListenCharName, szGuildName, sizeof(TCHAR) * MAX_CHAR_NAME_STR_SIZE );

	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteChat( data, m_bWriteText, NULL );
}

// 5. 귓속말
void CGameLog::WriteWhisperChat( TCHAR* szAccountID, TCHAR* szCharName, TCHAR* szRecvCharName, int nServerCode, TCHAR* szChatMsg )
{
	ChatData data;
	data.iChannel = nServerCode;
	data.byChatCode = CHAT_SHOUT;
	memcpy( data.szContext, szChatMsg, sizeof(TCHAR) * 100 );
	memcpy( data.szListenCharName, szRecvCharName, sizeof(TCHAR) * MAX_CHAR_NAME_STR_SIZE );

	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteChat( data, m_bWriteText, NULL );
}

// 6. 친구 채팅
void CGameLog::WriteFriendChat( TCHAR* szAccountID, TCHAR* szCharName, TCHAR* szRecvCharName, int nServerCode, TCHAR* szChatMsg )
{
	ChatData data;
	data.iChannel = nServerCode;
	data.byChatCode = CHAT_SHOUT;
	memcpy( data.szContext, szChatMsg, sizeof(TCHAR) * 100 );
	memcpy( data.szListenCharName, szRecvCharName, sizeof(TCHAR) * MAX_CHAR_NAME_STR_SIZE );

	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteChat( data, m_bWriteText, NULL );
}

// 정기적으로 게임 로그를 남긴다.
// 여기서 실행되지 않으면, 로그데이터를 이 클래스로 넘겨도, 기록되지 않는다.
void CGameLog::StartRegularLog( int nLogType )
{
	if( nLogType == REGULAR_SKILL )  //스킬로그를 시작시킨다면..
	{
		// 여기서 쓰레드를 실행시키고,
		// 쓰레드 내에서 Timer를 실행시킨다.
		unsigned int nThreadID = 1;
		HANDLE hSkillThread = (HANDLE)_beginthreadex( NULL, 0, SkillWorkThread, NULL, 0, &nThreadID );
	}
}


//Thread Argument
unsigned int __stdcall SkillWorkThread(void* pArg)
{
	CGameLog* pLog = (CGameLog*)pArg;

	while(TRUE)
	{
		
	}
}