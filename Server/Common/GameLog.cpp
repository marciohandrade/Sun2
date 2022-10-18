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
// ����ġ �α� 
// 1. ������ �α�
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


// 2. ����ġ 10��� �α�
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


// 3. ����ġ �߰�/�ٿ�
void CGameLog::WriteDownExp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, DWORD dwTotalExp, DWORD dwChangeExp,
							WORD wLevel )
{
	// ������ �������̽��δ� �Ұ���..
	// �� �÷��̾��� ��ü ����ġ�� �ٿ�� ����ġ�� �䱸�ϴµ�, ���� ���ڷδ� �Ұ���.
	// AddInfo�� ������ ���, �ٿ�� ����ġ�� �������ΰ�?
	ExpData data;
	data.iExpCode = EXP_DOWN;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iNewExp = dwTotalExp;
	data.iNewLevel = wLevel;
	m_LogMng.WriteExp( data, m_bWriteText, "Change Exp = %d", dwChangeExp );
}


//////////////////////////////////////////////////////////////////////////
// ������ �α�
// 1. ������ ��� �α�(������ �׾ �����°Ͱ� ���� �׿��� ������ ���� ����?)
// 1. ������ ���(���� ����) �� �ݱ�
void CGameLog::WriteItemOwner( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bOwner, int nItemCode, int nItemSerial
							  ,int nItemCount, int nMapCode, int nPosX, int nPosY, int nPosZ)

{
	ItemData data;
	if( bOwner == TRUE )  	data.iItemCode = ITEM_PICKUP;  // �ݱ�
	else					data.iItemCode = ITEM_CAST;	   // ������
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

// 2. ��þƮ 
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

// 3. NPC���� �ŷ�
void CGameLog::WriteItemTradeWithNPC( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bSell, int nItemCode,
									 int nItemSerial, int nItemCount, DWORD dwMoney )
{
	ItemData data;
	if( bSell == TRUE )  	data.iItemCode = ITEM_SELLNPC;	// NPC���� �ȱ�
	else					data.iItemCode = ITEM_BUYNPC;	// NPC���� ���

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
	if( bGive == TRUE )  	data.iItemCode = ITEM_GIVE;		// ��� �÷��̾�� �ֱ�
	else					data.iItemCode = ITEM_RECEIVE;	// ��뿡�Լ� �ޱ�
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

// 5. �� ���ΰŷ�
void CGameLog::WriteMoneyTradeWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bGive, DWORD dwMoney, 
							   TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName)
{
	ItemData data;
	if( bGive == TRUE )			// ��� �÷��̾�� �ֱ�
	{
		data.iItemCode = ITEM_MONEY;		
	}
	else						// ��뿡�Լ� �ޱ�
	{
		data.iItemCode = ITEM_MONEY;		
	}

	data.iServerCode = nServerCode;
	WriteBaseData( &data, szPlayer1_ID, szPlayer1_CharName );

	if( szPlayer2_ID == NULL )		_tcscpy( data.szToUserID, "" );
	else							_tcscpy( data.szToUserID, szPlayer2_ID );

	if( szPlayer2_CharName == NULL )		_tcscpy(data.szToCharName, "");
	else 									_tcscpy( data.szToCharName, szPlayer2_CharName );

	if( bGive == TRUE )  //Player1�� Player2���� ���� dwMoney��ŭ �ִ�.
		m_LogMng.WriteItem( data, m_bWriteText, "Give Money  = %d", dwMoney );
	else				//Player1�� Player2���Լ� ���� dwMoney��ŭ �޴�.
		m_LogMng.WriteItem( data, m_bWriteText, "Receive Money  = %d", dwMoney );
}

// 6. ���λ��� �ŷ�
void CGameLog::WriteItemVendorWithPlayer( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, BOOL bSell, ITEMLOGINFO ItemInfo, 
							   DWORD dwMoney, TCHAR* szPlayer2_ID, TCHAR* szPlayer2_CharName)
{
	ItemData data;
	if( bSell == TRUE )  	data.iItemCode = ITEM_SELLPC;	// ���λ������� ���� �ȱ�
	else					data.iItemCode = ITEM_BUYPC;	// ���λ������� ���� ���
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

// 7. ������ ��ũ��
void CGameLog::WriteItemRankUpResult( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, ITEMLOGINFO ItemInfo, 
					 DWORD dwMoney, BOOL bCreateSocket )
{
	ItemData data;
	data.iItemCode = ITEM_RANKUPSUCCESS;	// ��ũ�� ����
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iItemBaseCode = ItemInfo.nItemCode;
	data.iItemSeq = ItemInfo.nItemSerial;
	data.iItemCount = ItemInfo.nItemCount;
	if( bCreateSocket )
		m_LogMng.WriteItem( data, m_bWriteText, "���� ���� ��" );
	else
		m_LogMng.WriteItem( data, m_bWriteText, "���� ���� �ȵ�" );

}

// 8. â�� �±�� / ã��
// dwMoney : â�� ������
void CGameLog::WriteItemMoveInWareHouse( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bKeep, ITEMLOGINFO ItemInfo )
{
	ItemData data;
	if( bKeep == TRUE )  	data.iItemCode = ITEM_PUTSTORAGE;	// â�� ����
	else					data.iItemCode = ITEM_GETSTORAGE;	// â���� ������
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.iItemBaseCode = ItemInfo.nItemCode;
	data.iItemSeq = ItemInfo.nItemSerial;
	data.iItemCount = ItemInfo.nItemCount;
	m_LogMng.WriteItem( data, m_bWriteText, NULL );
}

// 9. ��þƮ �õ�
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

// 10. ���� ������ ���
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

// 11. ������ ���ΰŷ� ��û / ���� / ��û�ź�
// nType (0 : �ŷ� ��û, 1 : ����, 2 : ����)
void CGameLog::WriteItemTradeStatus( TCHAR* szPlayer1_ID, TCHAR* szPlayer1_CharName, int nServerCode, TCHAR* szPlayer2_ID,
						  TCHAR* szPlayer2_CharName, int nType )
{
	ItemData data;
	if( nType == 0 )				// ������ �ŷ� ��û
		data.iItemCode = ITEM_REQUEST;
	else if( nType == 1 )			// ����
		data.iItemCode = ITEM_ACCEPT;
	else if( nType == 2 )			// ����
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

// 12. ���λ��� ����/���
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

// 13. ���λ��� ����
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


// 14. â�� ����
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


// 15. ���λ��� ����(���ݼ���)
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
// �����׼� �α�
//1. ��Ȱ �α�
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
	// �κ������� ��� ���� ���ΰ�? 
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

// 6. ������ �̵�
void CGameLog::WriteMoveVillage( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, int nMapCode )
{
	ActionData data;
	data.iActionCode = ACT_MOVE_TOWN;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	data.siMapCode = nMapCode;
	m_LogMng.WriteUserAction( data, m_bWriteText, NULL );
}


// 7. ��ų������
void CGameLog::WriteSkillLevelUp( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, int nSkillCode, BOOL bLevelUp )
{
	ActionData data;
	if( bLevelUp == FALSE )  //�������� �ƴϸ�, ó�� ��ų�� ���� �Ŵ�.
		data.iActionCode = ACT_ACQUIRE_SKILL;
	else
		data.iActionCode = ACT_SKILLLV_UP;
	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );

	m_LogMng.WriteUserAction( data, m_bWriteText, "SKILL CODE = %d", nSkillCode );
}



//////////////////////////////////////////////////////////////////////////
// ���� �α� 
//1. �÷��̾� �α��� ]
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


//2. �÷��̾� �α��� ����
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

// 3. �α׾ƿ�
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

	//�α��� Ÿ�Ӱ� �α׾ƿ� Ÿ���� ����ϰ�, ���ð��� �����.
	//���� �Լ��� Ÿ�� 3, 4�� ��� ����Ҽ� �ִ�.
	if( data.szLogoutTime != NULL )
	{
		int nUsedTime = WZTime::GetDiffTimeType4( data.szLoginTime, data.szLogoutTime );
		data.iUseTime = nUsedTime;
	}

	data.byClassCode = (BYTE)wClass;
	data.siLevel = wLevel;
	m_LogMng.WriteSession( data, m_bWriteText, NULL );
}


// 4. ĳ���� ���� / ����
void CGameLog::WriteCharConnect( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, BOOL bConnect )
{
	SessionData data;
	if( bConnect == TRUE ) //����
		data.iConnectCode = CONN_CHAR_CONNECT;
	else
		data.iConnectCode = CONN_CHAR_DISCONNECT;

	data.iServerCode = nServerCode;
	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteSession( data, m_bWriteText, NULL );
}

//////////////////////////////////////////////////////////////////////////
// ä�� �α� 
// 1. ���� ä��
void CGameLog::WriteVillageChat( TCHAR* szAccountID, TCHAR* szCharName, int nServerCode, short sMapCode, TCHAR* szChatMsg )
{
	ChatData data;
	data.iChannel = nServerCode;		// ä�θ�
	data.byChatCode = CHAT_EYERANGE;	// �þ� ���� ä���̴�
	data.siMapCode = sMapCode;
	memcpy( data.szContext, szChatMsg, sizeof(TCHAR) * 100 );

	WriteBaseData( &data, szAccountID, szCharName );
	m_LogMng.WriteChat( data, m_bWriteText, NULL );
}

// 2. ��ġ��
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

// 3. ��Ʋ�� ä��
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

// 4. ��� ä��
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

// 5. �ӼӸ�
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

// 6. ģ�� ä��
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

// ���������� ���� �α׸� �����.
// ���⼭ ������� ������, �α׵����͸� �� Ŭ������ �Ѱܵ�, ��ϵ��� �ʴ´�.
void CGameLog::StartRegularLog( int nLogType )
{
	if( nLogType == REGULAR_SKILL )  //��ų�α׸� ���۽�Ų�ٸ�..
	{
		// ���⼭ �����带 �����Ű��,
		// ������ ������ Timer�� �����Ų��.
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