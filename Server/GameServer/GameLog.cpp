#include "StdAfx.h"
#include ".\gamelog.h"
#include <boost/static_assert.hpp>

#include "ItemManager.h"
#include "SkillManager.h"
#include "MissionManager.h"
#include "SCItemSlotContainer.h"
#include "SCSkillSlotContainer.h"
#include "SkillInfoParser.h"
#include "WarehouseSlotContainer.h"
#include "Map.h"
#include <CommonCharacterFormula.h>
#include <ServerStruct.h>
#include <SCItemSlot.h>
#include "GameServerEx.h"
#include "GameZone.h"
#include "RewardManager.h"
#include "SkillManager.h"
#include "Player.h"
#include <QuestDefine.h>
#include "QuestManager_Concrete.h"
#include <AreaConquestCommonHeader.h>
#include "ChaoState.h"
#include "SkillSlotManager.h"
#include "GameGuildManager.h"
#include "GameGuild.h"
#include "GameDominationManager.h"
#include "SystemMemo.h" //_NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
#include "AwakeningInfoParser.h"
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
#include "Skill_Info.h"
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG

GameLog::GameLog()
{
}

//////////////////////////////////////////////////////////////////////////
// 경험치 로그 
// 1. 레벨업 로그
void GameLog::WriteLevelUp( Player* pPlayer, WORD wOldLevel, WORD wNewLevel )
{
	if( !IsRun() || !pPlayer )		return;

	m_ExpData.Init();

	m_ExpData.SetLogType( EXP_LEVELUP );
	m_ExpData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ExpData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ExpData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

//	m_ExpData.SetOldExp( dwOldExp );
//	m_ExpData.SetNewExp( dwNewExp );
	m_ExpData.SetOldLevel( wOldLevel );
	m_ExpData.SetNewLevel( wNewLevel );

//	SetRoomKey( m_ExpData, pPlayer );
//	SetPlayerPosAddInfo( m_ExpData, pPlayer );

#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, _T("Class:%u"),
        (pPlayer->GetCharInfo() != NULL) ? pPlayer->GetCharInfo()->m_byClassCode : eCLASS_NONE);
    m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
    m_ExpData.AddAddInfo(m_pTmpBuffer);
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG

	m_LogMng.Serialize( m_ExpData );
}


// NOTE: f110602.3L
void GameLog::WritePerkLevelUp(Player* player,
    int perk_group, LEVELTYPE old_level, LEVELTYPE new_level)
{
    if (player == NULL || IsRun() == false) {
        return;
    };
    m_ExpData.Init();

    m_ExpData.SetLogType(EXP_PERK_LEVEL);
    m_ExpData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ExpData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ExpData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);

    m_ExpData.SetOldLevel(old_level);
    m_ExpData.SetNewLevel(new_level);

    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Code:%u", perk_group);
    m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
    m_ExpData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize(m_ExpData);
}

// 2. 레벨 10 이상시 경험치 10등분 로그
void GameLog::LogExp( Player* pPlayer, WORD wCurLevel, EXPTYPE OldExp, EXPTYPE NewExp, MAPCODE Mapcode )
{
	if( !IsRun() || !pPlayer  )		return;

	// 레벨이 10이상이 되야 로그를 남긴다.
	if( wCurLevel < MAX_EXP_DIVIDE_LOG_AT_LEVEL )
		return;

	// 이전 레벨의 경험치 누적값
	EXPTYPE CurAccumExp =  pPlayer->GetAccumExp( wCurLevel ) ;
	// 새로운 레벨의 경험치 누적값
	EXPTYPE NextAccumExp = pPlayer->GetAccumExp( wCurLevel +1 );

	BYTE byCurDivide = 0, byNextDivide = 0;
	if( wCurLevel < MAX_EXP_FULL_LOG_AT_LEVEL
#ifdef _NA_000000_20131031_GAMELOG_REDUCE
        || (g_pGameServer->GetGuid() == FIELD_SERVER)
#endif //_NA_000000_20131031_GAMELOG_REDUCE
        ) 
	{
		// 1/10등분의 경험치 값
		EXPTYPE DifExp = (NextAccumExp - CurAccumExp) / 10;
		ASSERT(DifExp > 0);
		if( DifExp <= 0 )	return;

		byCurDivide = (BYTE)( ( OldExp - CurAccumExp ) / DifExp );
		byNextDivide = (BYTE)( ( NewExp - CurAccumExp ) / DifExp );

		//현재 경험치에 의한 등분보다 업된 경험치에 의한 등분이 같다면
		if( byCurDivide == byNextDivide ) return;
	}

	m_ExpData.Init();

	m_ExpData.SetLogType( EXP_DIVIDECNT );
	m_ExpData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ExpData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ExpData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	m_ExpData.SetDivideCount( byNextDivide );
	m_ExpData.SetOldExp( NewExp - OldExp );
	m_ExpData.SetNewExp( NewExp );
	m_ExpData.SetOldLevel( wCurLevel );

	// 레벨업이라면
	if( NewExp >= NextAccumExp )
		m_ExpData.SetNewLevel( wCurLevel + 1 );
	else
		m_ExpData.SetNewLevel( wCurLevel );

	if( Mapcode )
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "MCode:%u", Mapcode );
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		m_ExpData.AddAddInfo(m_pTmpBuffer); // CHANGES: f110602.3L, fixed the invalid data setting.
	}
	m_LogMng.Serialize( m_ExpData );
}


// 3. 경험치 다운
void GameLog::WriteDownExp( Player* pPlayer, EXPTYPE NewExp, EXPTYPE OldExp )
{
	if( !IsRun() || !pPlayer  )		return;

	// 현재의 인터페이스로는 불가능..
	// 현 플레이어의 전체 경험치와 다운된 경험치를 요구하는데, 현재 인자로는 불가능.
	// AddInfo에 삽입할 경우, 다운된 경험치를 넣을것인가?
	m_ExpData.Init();

	m_ExpData.SetLogType( EXP_DOWN );
	m_ExpData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ExpData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ExpData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	m_ExpData.SetNewExp( NewExp );
	m_ExpData.SetOldExp( OldExp );

	m_LogMng.Serialize( m_ExpData );
}

void GameLog::LogExp( int iLogType, Player* pPlayer, EXPTYPE NewExp, EXPTYPE OldExp )
{
	if( !IsRun() || !pPlayer  )		return; 

	// 현재의 인터페이스로는 불가능..
	// 현 플레이어의 전체 경험치와 다운된 경험치를 요구하는데, 현재 인자로는 불가능.
	// AddInfo에 삽입할 경우, 다운된 경험치를 넣을것인가?
	m_ExpData.Init();

	m_ExpData.SetLogType( iLogType );
	m_ExpData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ExpData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ExpData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	m_ExpData.SetNewExp( NewExp );
	m_ExpData.SetOldExp( OldExp );

	m_LogMng.Serialize( m_ExpData );
}

// implemented by _NA_20100423_GROUPSYSTEM_MONEY_AND_EXP_GAMELOG
void GameLog::LogExp (int log_type, Player* player, EXPTYPE new_exp, EXPTYPE old_exp, 
                      const TCHAR* char_name)
{
    if (!IsRun() || !player) return; 
    
    m_ExpData.Init();

    m_ExpData.SetLogType(log_type);
    m_ExpData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ExpData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ExpData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);
    m_ExpData.SetNewExp(new_exp);
    m_ExpData.SetOldExp(old_exp);

    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Low Member:%s", char_name);
    m_pTmpBuffer[_countof(m_pTmpBuffer)-1]='\0';
    m_ExpData.AddAddInfo(m_pTmpBuffer); // CHANGES: f110602.3L, fixed the invalid data setting.

    m_LogMng.Serialize(m_ExpData);
}

void GameLog::LogExpPerk(Player* player, int perk_group, EXPTYPE new_exp, EXPTYPE old_exp)
{
    if (player == NULL || IsRun() == false) {
        return;
    };
    
    m_ExpData.Init();

    m_ExpData.SetLogType(EXP_PERK);
    m_ExpData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ExpData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ExpData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);
    m_ExpData.SetNewExp(new_exp);
    m_ExpData.SetOldExp(old_exp);

    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Code:%u", perk_group);
    m_pTmpBuffer[_countof(m_pTmpBuffer)-1]='\0';
    m_ExpData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize(m_ExpData);
}
//
//class aaab
//{
//public:
//	void CopyOut( ITEMSTREAMEX & aa ) const
//	{
//		aa.OptionPart.Block =1;
//	}
//};
//void aaa(const aaab & rItemSlot)
//{
//	ITEMSTREAMEX ItemStreamEx;
//	rItemSlot.CopyOut(ItemStreamEx);
//}
void GameLog::LogMonsterMoneyDrop( int iLogType, DWORD64 dw64Money, 
								 DWORD dwMonsterCode, DWORD dwMonsterLevel, WORD wMapCode, const WzVector& vecPos, KEYTYPE RoomKey )
{
	// 로그양 줄이기 위해 몬스터 드랍 우선 로그 안남김
	// 20060802 오후 5:30:13
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return;
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#if SUN_CODE_BACKUP
	if( !IsRun() )		return;

	m_MoneyData.Init();
	m_MoneyData.SetLogType( iLogType );
	m_MoneyData.SetMapCode( wMapCode );
	m_MoneyData.SetMapX( (SHORT)vecPos.x );
	m_MoneyData.SetMapY( (SHORT)vecPos.y );
	m_MoneyData.SetMapZ( (SHORT)vecPos.z );
	m_MoneyData.SetMoney( dw64Money );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Room:%u,Mon:%u,MonLV:%u", RoomKey, dwMonsterCode, dwMonsterLevel );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "RmKy:%u,Mon:%u,MonLV:%u", RoomKey, dwMonsterCode, dwMonsterLevel );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_MoneyData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_MoneyData );
#endif
}

void GameLog::LogTotalMoney( DWORD64 dw64Money, DWORD64 dw64ToMoney, WORD PlayerNum, DWORD64 dw64PlayerTotalMoney )
{
	if( !IsRun() )		return;

	m_MoneyData.Init();

	m_MoneyData.SetLogType( MONEY_TOTAL );
	m_MoneyData.SetAccountID( NULL, ID_MAX_LEN );
	m_MoneyData.SetCharName( NULL, MAX_CHARNAME_LENGTH );
	m_MoneyData.SetUserIP( NULL, MAX_IPADDRESS_SIZE );

	m_MoneyData.SetMoney( dw64Money );
	m_MoneyData.SetToMoney( dw64ToMoney );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "PlayerNum:%u,TotMoney:%I64u", PlayerNum, dw64PlayerTotalMoney );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "PlyNo:%u,TotMny:%I64u", PlayerNum, dw64PlayerTotalMoney );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_MoneyData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_MoneyData );
}

void GameLog::LogTotalWarehouseMoney( DWORD64 dw64Money, DWORD64 dw64ToMoney, WORD PlayerNum, DWORD64 dw64PlayerTotalMoney )
{
	if( !IsRun() )		return;

	m_MoneyData.Init();

	m_MoneyData.SetLogType( MONEY_TOTALWAREHOUSE );
	m_MoneyData.SetAccountID( NULL, ID_MAX_LEN );
	m_MoneyData.SetCharName( NULL, MAX_CHARNAME_LENGTH );
	m_MoneyData.SetUserIP( NULL, MAX_IPADDRESS_SIZE );

	m_MoneyData.SetMoney( dw64Money );
	m_MoneyData.SetToMoney( dw64ToMoney );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "PlayerNum:%u,TotMoney:%I64u", PlayerNum, dw64PlayerTotalMoney );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "PlyNo:%u,TotMny:%I64u", PlayerNum, dw64PlayerTotalMoney );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_MoneyData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_MoneyData );
}

// implemented by _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE
void GameLog::LogMemoReward(eSYSTEM_MEMO_TYPE memo_type, Player* player, WORD reward_item_code, 
                            ULONGLONG amount_item, const TCHAR* memo_contents)
{
    if (!IsRun() || !player)		
    {
        return;
    }

    //-- 보상은 아이템 or 돈
    int log_type = (reward_item_code == 0) ? MONEY_MAX : ITEM_MAX;

    switch(memo_type)
    {
    case SYSMEMO_LOTTO_WIN:
    case SYSMEMO_LOTTO_WIN_NEW:
        {
            log_type = MONEY_LOTTO_REWARD; 
        }
        break;
    case SYSMEMO_DOMINATION_RETURN_MONEY:       
    case SYSMEMO_DOMINATION_RETURN_MONEY_NEW:
        {
            log_type = 
                (reward_item_code == 0) ? 
                MONEY_DOMINATION_ACTIONMONEY_RETURN : 
                ITEM_DOMINATION_AUCTION_RETURN_ITEM_BY_SYSTEM_MEMO;
        }
        break;
    case SYSMEMO_DOMINATION_REWARD_MONEY:
    case SYSMEMO_DOMINATION_REWARD_MONEY_NEW:
        {
            log_type = 
                (reward_item_code == 0) ? 
                MONEY_DOMINATION_ACTIONMONEY_REWARD : 
                ITEM_DOMINATION_AUCTION_REWARD_ITEM_BY_SYSTEM_MEMO;
        }
        break;
    default:
        {
            log_type = 
                (reward_item_code == 0) ? 
                MONEY_DEFAULT_SYSTEM_MEMO_REWARD : 
                ITEM_DEFAULT_SYSTEM_MEMO_REWARD;
        }
        break;
    }

    if(reward_item_code == 0)
    {
        LogMemoRewardMoney(log_type, player, amount_item, memo_contents);
    }
    else
    {
        WORD item_num  = static_cast<WORD>(amount_item);
        LogMemoRewardItem(log_type, player, reward_item_code, item_num, memo_contents);
    }

    return;
}

void GameLog::LogMemoRewardMoney(int log_type, Player* player, ULONGLONG amount_Heim, 
                                 const TCHAR* memo_contents)
{
    if (!IsRun() || !player)		
    {
        return;
    }

    ASSERT(log_type >= MONEY_TOTAL || log_type < MONEY_MAX);

    SystemMemo system_memo;

    m_MoneyData.Init();

    m_MoneyData.SetLogType(log_type);
    m_MoneyData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_MoneyData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_MoneyData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);
    m_MoneyData.SetMoney(player->GetMoney());
    m_MoneyData.SetToMoney(amount_Heim);

    GameZone *zone = player->GetGameZonePtr();
    if (zone)
    {
        WzVector vector_pos;
        player->GetPos( &vector_pos );

        m_MoneyData.SetMapCode( (WORD)zone->GetMapCode() );
        m_MoneyData.SetMapX( (short)vector_pos.x );
        m_MoneyData.SetMapY( (short)vector_pos.y );
        m_MoneyData.SetMapZ( (short)vector_pos.z );
    }

    //-- 메모별 AddInfo 작성 : 메모별로 필요한 정보를 얻을때 시스템 
    //메모내에 지정된 포맷의 몇번째 정보인지 미리 알아야 한다.

    if (log_type == MONEY_DOMINATION_ACTIONMONEY_RETURN || 
        log_type == MONEY_DOMINATION_ACTIONMONEY_REWARD)
    {
        GameGuild* guild = g_GameGuildManager.FindGuild(player->GetGuildGuid());
        if(guild != NULL)
        {
            _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Guild:%s, Map:%d, Money:%I64u", 
                       guild->GetGuildName(), 
                       system_memo.GetMemoDataToINT(memo_contents, 2),
                       amount_Heim);//메모 내용중 2번째가 맵코드
            m_pTmpBuffer[_countof(m_pTmpBuffer) - 1] = '\0';
            m_MoneyData.AddAddInfo(m_pTmpBuffer);
        }
    }
    else if(log_type == MONEY_LOTTO_REWARD)
    {
        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Name:%s, Money:%I64u", 
                    player->GetCharName(), amount_Heim);
        m_pTmpBuffer[_countof(m_pTmpBuffer) - 1] = '\0';
        m_MoneyData.AddAddInfo(m_pTmpBuffer);
    }
    m_LogMng.Serialize(m_MoneyData);
}

void GameLog::LogMemoRewardItem(int log_type, Player* player, WORD reward_item_code, 
                                WORD amount_item, const TCHAR* memo_contents)
{
    if (!IsRun() || !player)		
    {
        return;
    }

    ASSERT(log_type < ITEM_MAX);

    SystemMemo system_memo;

    m_ItemData.Init();

    m_ItemData.SetLogType(log_type);
    m_ItemData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ItemData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ItemData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);	

    m_ItemData.SetItemCode(reward_item_code);//코드가 0이면 하임
    m_ItemData.SetItemNum(amount_item);

    GameZone *zone = player->GetGameZonePtr();
    if (zone)
    {
        WzVector vector_pos;
        player->GetPos(&vector_pos);

        m_ItemData.SetMapCode((WORD)zone->GetMapCode());
        m_ItemData.SetMapX((short)vector_pos.x);
        m_ItemData.SetMapY((short)vector_pos.y);
        m_ItemData.SetMapZ((short)vector_pos.z);
    }

    //-- 메모별 AddInfo 작성 : 메모별로 필요한 정보를 얻을때 시스템 
    //메모내에 지정된 포맷의 몇번째 정보인지 미리 알아야 한다.

    if (log_type == ITEM_DOMINATION_AUCTION_RETURN_ITEM_BY_SYSTEM_MEMO || 
        log_type == ITEM_DOMINATION_AUCTION_REWARD_ITEM_BY_SYSTEM_MEMO)
    {
        GameGuild* guild = g_GameGuildManager.FindGuild(player->GetGuildGuid());
        if(guild != NULL)
        {
            _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Guild:%s, Map:%d, ItemNum:%d", 
                       guild->GetGuildName(), 
                       system_memo.GetMemoDataToINT(memo_contents, 2),
                       amount_item);//메모 내용중 2번째가 맵코드
            m_pTmpBuffer[_countof(m_pTmpBuffer) - 1] = '\0';
            m_ItemData.AddAddInfo(m_pTmpBuffer);
        }
    }
    m_LogMng.Serialize(m_ItemData);
}

void GameLog::LogMoney( int iLogType, Player * pPlayer, DWORD64 dw64Money, DWORD64 dw64ToMoney,
						const TCHAR * ptszToUserID, const TCHAR * ptszToCharName )
{
	if( !IsRun() || !pPlayer  )		return;

	ASSERT( iLogType >= MONEY_TOTAL || iLogType < MONEY_MAX );

	m_MoneyData.Init();

	m_MoneyData.SetLogType( iLogType );
	m_MoneyData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_MoneyData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_MoneyData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	m_MoneyData.SetToAccountID( ptszToUserID , ID_MAX_LEN );
	m_MoneyData.SetToCharName( ptszToCharName, MAX_CHARNAME_LENGTH );

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_MoneyData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_MoneyData.SetMapX( (short)vecPos.x );
		m_MoneyData.SetMapY( (short)vecPos.y );
		m_MoneyData.SetMapZ( (short)vecPos.z );
	}

	if( iLogType == MONEY_TRADE )
	{
		//가진 돈
		m_MoneyData.SetMoney( pPlayer->GetMoney() );
		//줄 돈
		m_MoneyData.SetToMoney( dw64Money );
		//받을 돈은 AddInfo에
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",FromMny:%I64d", dw64ToMoney );
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		m_MoneyData.AddAddInfo( m_pTmpBuffer );
	}
	else
	{	
		// 플레이어의 현재 총 머니값
		if( 0 == dw64ToMoney )
		{
			m_MoneyData.SetMoney( pPlayer->GetMoney() );
			// 플레이어의 변화 머니값
			m_MoneyData.SetToMoney( dw64Money );
		}
		else
		{
			m_MoneyData.SetMoney( dw64Money );
			m_MoneyData.SetToMoney( dw64ToMoney );
		}
	}
	

	GameZone * pGameZone = pPlayer->GetGameZonePtr();
	if( pGameZone )
	{
		m_MoneyData.SetRoomKey( pGameZone->GetKey() );
	}

	if( iLogType == MONEY_WAREHOUSE_PUT || iLogType == MONEY_WAREHOUSE_GET )
	{
		if( pPlayer->GetWarehouseContainer() )
		{
			_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",WrHusMny:%I64u", pPlayer->GetWarehouseContainer()->GetCurMoney() );
			m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
			m_MoneyData.AddAddInfo( m_pTmpBuffer );
		}		
	}

	if( iLogType == MONEY_GUILD_WAREHOUSE_PUT || iLogType == MONEY_GUILD_WAREHOUSE_GET )
	{
		if(GameGuild* pGuild = g_GameGuildManager.FindGuild(pPlayer->GetGuildGuid()))
		{
			MONEY curMoney(0);
			if(pPlayer->GetGuildWarehouseContainer()) 
			{
				 curMoney = pPlayer->GetGuildWarehouseContainer()->GetCurMoney();
			}
			_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "GuildName:%s, GuildGuid:%u, CurMoney:%I64u", 
				pGuild->GetGuildName(), pGuild->GetGuildGuid(), curMoney);
			m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
			m_MoneyData.AddAddInfo(m_pTmpBuffer);
		}
	}

    //_KR_001385_20091211_DOMINATION_GAMELOG_MONEY
    if( iLogType == MONEY_DOMINATION_ACTIONMONEY_RETURN || 
        iLogType == MONEY_DOMINATION_ACTIONMONEY_REWARD ||
        iLogType == MONEY_DOMINATION_ACTIONMONEY_APPLY)
    {
        if(GameGuild* pGuild = g_GameGuildManager.FindGuild(pPlayer->GetGuildGuid()))
        {
            _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "%s", pGuild->GetGuildName() );
            m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
            m_MoneyData.AddAddInfo(m_pTmpBuffer);
        }
    }

    //_NA_20100423_GROUPSYSTEM_MONEY_AND_EXP_GAMELOG
    if (iLogType == MONEY_GROUPSYSTEM_GETMONEY)
    {
        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "High Member : %s", ptszToCharName);
        m_pTmpBuffer[_countof(m_pTmpBuffer)-1] = '\0';
        m_MoneyData.AddAddInfo(m_pTmpBuffer);
    }

	m_LogMng.Serialize( m_MoneyData );

	// 큰 하임일 경우 로그 중복 남김, 100만 하임이상일 경우 - 07.11.13 금액 1억 변경
	if( dw64Money >= LOG_TRADE_OVER_MONEY || dw64ToMoney >= LOG_TRADE_OVER_MONEY )
	{
		m_MoneyData.SetLogType( MONEY_GREATE_MONEY );
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",LogType:%u", iLogType );		
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		m_MoneyData.AddAddInfo( m_pTmpBuffer );

		m_LogMng.Serialize( m_MoneyData );
	}
}

void GameLog::LogACItem( int iLogType, Player * pPlayer, const SCItemSlot * pItemSlot, float fPercentage )
{
	if( !IsRun() || !pPlayer  )		return;

	ASSERT( iLogType >= ITEM_MONSTER_DROP || iLogType < ITEM_MAX );

	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	// 아이템 정보
	if( pItemSlot )
	{
		m_ItemData.SetItemCode( pItemSlot->GetItemCode() );

		m_ItemData.SetItemSerial( pItemSlot->GetWorldSerial() );

		m_ItemData.SetItemNum( pItemSlot->GetNum() );

		ITEMSTREAMEX ItemStreamEx;
		pItemSlot->CopyOut(ItemStreamEx);

		m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );
	}

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Percent:%f%%", fPercentage );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",P:%f%%", fPercentage );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ItemData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ItemData );
}
void GameLog::LogItemWarehouse( int iLogType, Player * pPlayer, const SCItemSlot * pItemSlot, BYTE byItemNum, 
							   SLOTIDX f_idx, POSTYPE f_pos, SLOTIDX t_idx, POSTYPE t_pos )
{
	if( !IsRun() || !pPlayer  )		return;
	ASSERT( iLogType >= ITEM_MONSTER_DROP || iLogType < ITEM_MAX );
	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	// 아이템 정보
	if( pItemSlot )
	{
		m_ItemData.SetItemCode( pItemSlot->GetItemCode() );

		m_ItemData.SetItemSerial( pItemSlot->GetWorldSerial() );

		m_ItemData.SetItemNum( byItemNum );

		ITEMSTREAMEX ItemStreamEx;
		pItemSlot->CopyOut(ItemStreamEx);

		m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );
	}

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",FSlotIdx:%u,FPos:%u,TSlotIdx:%u,TPos:%u,Tick:%u", f_idx, f_pos, t_idx, t_pos, GetTickCount() ); //< 아이템를 살때 가격.
//	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
//	m_ItemData.AddAddInfo( m_pTmpBuffer );

	if(iLogType == ITEM_GUILD_WAREHOUSE_GET || iLogType == ITEM_GUILD_WAREHOUSE_PUT)
	{
		if(GameGuild* pGuild = g_GameGuildManager.FindGuild(pPlayer->GetGuildGuid()))
		{
			_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "GuildName:%s, GuildGuid:%u", pGuild->GetGuildName(), pGuild->GetGuildGuid());
			m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
			m_ItemData.AddAddInfo(m_pTmpBuffer);
		}
	}

	m_LogMng.Serialize( m_ItemData );
}
void GameLog::LogItem_Pos( int iLogType, Player * pPlayer, const SCItemSlot * pItemSlot, MONEY ullMoney, BYTE byItemNum, 
							   SLOTIDX f_idx, POSTYPE f_pos, SLOTIDX t_idx, POSTYPE t_pos )
{
	if( !IsRun() || !pPlayer  )		return;
	ASSERT( iLogType >= ITEM_MONSTER_DROP || iLogType < ITEM_MAX );
	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	m_ItemData.SetMoney( ullMoney );

	// 아이템 정보
	if( pItemSlot )
	{
		m_ItemData.SetItemCode( pItemSlot->GetItemCode() );

		m_ItemData.SetItemSerial( pItemSlot->GetWorldSerial() );

		m_ItemData.SetItemNum( byItemNum );

		ITEMSTREAMEX ItemStreamEx;
		pItemSlot->CopyOut(ItemStreamEx);

		m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );
	}

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",FSlotIdx:%u,FPos:%u,TSlotIdx:%u,TPos:%u,Tick:%u", f_idx, f_pos, t_idx, t_pos, GetTickCount() ); //< 아이템를 살때 가격.
//	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
//	m_ItemData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ItemData );
}

void GameLog::LogItem(int log_type, Player* player, uint use_count, const SCItemSlot* use_slot, const SCItemSlot* target_slot)
{
    if (!IsRun() || !player)
    {
        return;
    }

    ASSERT(log_type == ITEM_USE);
    if (log_type != ITEM_USE)
    {
        return;
    }

    m_ItemData.Init();
    m_ItemData.SetLogType(log_type);
    m_ItemData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ItemData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ItemData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);

    GameZone* zone = player->GetGameZonePtr();
    if (zone)
    {
        WzVector vecPos;
        player->GetPos(&vecPos);

        m_ItemData.SetMapCode((WORD)zone->GetMapCode());
        m_ItemData.SetMapX((short)vecPos.x);
        m_ItemData.SetMapY((short)vecPos.y);
        m_ItemData.SetMapZ((short)vecPos.z);
    }

    m_ItemData.SetMoney(0);

    if (use_slot)
    {
        m_ItemData.SetItemCode(use_slot->GetItemCode());
        m_ItemData.SetItemSerial(use_slot->GetWorldSerial());
        m_ItemData.SetItemNum(use_count);

        ITEMSTREAMEX ItemStreamEx;
        use_slot->CopyOut(ItemStreamEx);

        m_ItemData.SetItemStreamEx1((const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx));
    }

    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Target Item, Code : %d, Serial : %d", target_slot->GetItemCode(), target_slot->GetWorldSerial());
    m_pTmpBuffer[_countof(m_pTmpBuffer) - 1]='\0';
    m_ItemData.AddAddInfo( m_pTmpBuffer );
    m_LogMng.Serialize( m_ItemData );
}

#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
void GameLog::LogItem(int log_type, Player * player, const SCItemSlot* item_slot, 
                      BYTE item_num, WORD price_of_goods, SHOPINFO* shop_info)
{
    if (!IsRun() || !player)
    {
        return;
    }

    ASSERT(log_type >= ITEM_MONSTER_DROP || log_type < ITEM_MAX);
    if (log_type < ITEM_MONSTER_DROP || log_type >= ITEM_MAX)
    {
        return;
    }
    
    m_ItemData.Init();
    m_ItemData.SetLogType(log_type);
    m_ItemData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ItemData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ItemData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);

    GameZone* zone = player->GetGameZonePtr();
    if (zone)
    {
        WzVector vecPos;
        player->GetPos(&vecPos);

        m_ItemData.SetMapCode((WORD)zone->GetMapCode());
        m_ItemData.SetMapX((short)vecPos.x);
        m_ItemData.SetMapY((short)vecPos.y);
        m_ItemData.SetMapZ((short)vecPos.z);
    }

    m_ItemData.SetMoney(price_of_goods);

    if (item_slot)
    {
        m_ItemData.SetItemCode(item_slot->GetItemCode());
        m_ItemData.SetItemSerial(item_slot->GetWorldSerial());
        m_ItemData.SetItemNum(item_num);

        ITEMSTREAMEX ItemStreamEx;
        item_slot->CopyOut(ItemStreamEx);

        m_ItemData.SetItemStreamEx1((const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx));
    }

    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, 
               "ShopID:%u, ShopType:%d, NeedItem:%u", 
               shop_info->GetShopListID(), 
               shop_info->GetShopType(), 
               shop_info->GetCurrencyItemCode());

    m_pTmpBuffer[_countof(m_pTmpBuffer) - 1]='\0';
    m_ItemData.AddAddInfo( m_pTmpBuffer );

    m_LogMng.Serialize( m_ItemData );
}

#endif


void GameLog::LogItem( int iLogType, Player * pPlayer, const SCItemSlot * pItemSlot, MONEY ullMoney, BYTE byItemNum,
                      const TCHAR * ptszToUserID, const TCHAR * ptszToCharName, const TCHAR* ptszAddInfo )
{
	if( !IsRun() || !pPlayer  )		return;

	ASSERT( iLogType >= ITEM_MONSTER_DROP || iLogType < ITEM_MAX );
	//switch( iLogType )
	//{
	////case ITEM_MAKE_SERIAL:
	////case ITEM_PICK:
	//	{
	//		if( pItemSlot && eITEMTYPE_WASTE == pItemSlot->GetItemInfo()->m_wType )
	//		{
	//			// 소모성 아이템 경우 로그 안남김 : 로그양 줄이기 위해
	//			return;
	//		}
	//	}
	//	break;	
	//}

	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	m_ItemData.SetToAccountID( ptszToUserID,  ID_MAX_LEN);
	m_ItemData.SetToCharName( ptszToCharName,  MAX_CHARNAME_LENGTH);	

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	m_ItemData.SetMoney( ullMoney );

	// 아이템 정보
	if( pItemSlot )
	{
		m_ItemData.SetItemCode( pItemSlot->GetItemCode() );

		m_ItemData.SetItemSerial( pItemSlot->GetWorldSerial() );

		m_ItemData.SetItemNum( byItemNum );

		ITEMSTREAMEX ItemStreamEx;
		pItemSlot->CopyOut(ItemStreamEx);

		m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );
	}

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

    if (ptszAddInfo != NULL) {
        m_ItemData.AddAddInfo((TCHAR*)ptszAddInfo);
    }

	m_LogMng.Serialize( m_ItemData );
}

void GameLog::LogItemEx( int iLogType, Player * pPlayer, const SCItemSlot & rItemSlot1, const SCItemSlot & rItemSlot2, 
						MONEY ullMoney, BYTE byItemNum,
						const TCHAR * ptszToUserID, const TCHAR * ptszToCharName )
{
	if( !IsRun() || !pPlayer  )		return;

	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	m_ItemData.SetToAccountID( ptszToUserID,  ID_MAX_LEN);
	m_ItemData.SetToCharName( ptszToCharName,  MAX_CHARNAME_LENGTH);

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	m_ItemData.SetMoney( ullMoney );

	// 아이템 정보
	m_ItemData.SetItemCode( rItemSlot1.GetItemCode() );

	m_ItemData.SetItemSerial( rItemSlot1.GetWorldSerial() );

	m_ItemData.SetItemNum( byItemNum );

	ITEMSTREAMEX ItemStreamEx;

	rItemSlot1.CopyOut(ItemStreamEx);
	m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

	rItemSlot2.CopyOut(ItemStreamEx);
	m_ItemData.SetItemStreamEx2( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );
	if( iLogType == ITEM_GET_BY_LOTTERY )
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ", Lottery Serial = %I64u", rItemSlot2.GetWorldSerial() );
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		m_ItemData.AddAddInfo( m_pTmpBuffer );
	}

	m_LogMng.Serialize( m_ItemData );
}
	
void GameLog::LogItemEx2( int iLogType, Player * pPlayer, const SCItemSlot & rItemSlot1, 
					MONEY ullMoney, BYTE byItemNum,
					DBWLDSERIAL dwItem2WorldSerial, SLOTCODE Item2Code, BYTE Item2Num, 
					const TCHAR * ptszToUserID, const TCHAR * ptszToCharName )
{
	if( !IsRun() || !pPlayer  )		return;

	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	m_ItemData.SetToAccountID( ptszToUserID,  ID_MAX_LEN);
	m_ItemData.SetToCharName( ptszToCharName,  MAX_CHARNAME_LENGTH);

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	m_ItemData.SetMoney( ullMoney );

	// 아이템 정보
	m_ItemData.SetItemCode( rItemSlot1.GetItemCode() );

	m_ItemData.SetItemSerial( rItemSlot1.GetWorldSerial() );

	m_ItemData.SetItemNum( byItemNum );

	ITEMSTREAMEX ItemStreamEx;
	rItemSlot1.CopyOut(ItemStreamEx);

	m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

	ItemStreamEx.Part.dwSerial	= dwItem2WorldSerial;

	ItemStreamEx.Part.wCode		= Item2Code;
	ItemStreamEx.Part.byDura	= Item2Num;

	m_ItemData.SetItemStreamEx2( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

	m_LogMng.Serialize( m_ItemData );
}

void GameLog::LogCrystalizeSuccess( Player * pPlayer, const SCItemSlot & rItemSlot, 
								   BYTE byTotalCount, SLOTCODE * IN pObtainItemCode, BYTE * IN pObtainItemNum )
{
	if( !IsRun() || !pPlayer  )		return;

	m_ItemData.Init();

	m_ItemData.SetLogType( ITEM_CRYSTALIZE_SUCCESS );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	// 아이템 정보
	m_ItemData.SetItemCode( rItemSlot.GetItemCode() );

	m_ItemData.SetItemSerial( rItemSlot.GetWorldSerial() );

	m_ItemData.SetItemNum( rItemSlot.GetNum() );

	ITEMSTREAMEX ItemStreamEx;
	rItemSlot.CopyOut(ItemStreamEx);

	m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

	for( BYTE i = 0 ; i < byTotalCount ; ++i )
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Item:%u,Num:%u", pObtainItemCode[i], pObtainItemNum[i] );
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		m_ItemData.AddAddInfo( m_pTmpBuffer );
	}

	m_LogMng.Serialize( m_ItemData );
}

	void GameLog::LogCompose( BOOL bSuccess, Player * pPlayer, SCItemSlot * pItemSlot1, BYTE byMatCount, 
						sITEM_COMPOSE_LIST::sITEM_FOR_COMPOSE * pMaterials, MONEY ullMoney, DBWLDSERIAL *pSerial, BYTE ComposeNum )
{
	if( !IsRun() || !pPlayer  )		return;


	int iLogType = ITEM_COMPOSE_SUCCESS;
	if( !bSuccess ) iLogType = ITEM_COMPOSE_FAILED;
	
	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}


	if( pItemSlot1 )
	{
		// 아이템 정보
		m_ItemData.SetItemCode( pItemSlot1->GetItemCode() );

		m_ItemData.SetItemSerial( pItemSlot1->GetWorldSerial() );

#ifdef _NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT
        // pItemSlot1->GetNum() 은 인벤토리에 있는 총 개수, ComposeNum에 성공한 개수를 넘기자
        m_ItemData.SetItemNum( ComposeNum );
#else
        m_ItemData.SetItemNum( pItemSlot1->GetNum() );
#endif //_NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT

		ITEMSTREAMEX ItemStreamEx;
		pItemSlot1->CopyOut(ItemStreamEx);

		m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );
	}

	m_ItemData.SetMoney( ullMoney*ComposeNum );

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

	for( BYTE i = 0 ; i < byMatCount ; ++i )
	{
#ifdef _NA_0_20091020_COMPACT_COMPOSITION_LOG_
        if (i > 0) { m_ItemData.AddAddInfo(","); }
        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "%u:%u:%u", \
            pMaterials[i].m_ItemCode, pMaterials[i].m_byItemNum*ComposeNum, pSerial[i]);
#else
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Item:%u,Num:%u,Serial:%u", 
			pMaterials[i].m_ItemCode, pMaterials[i].m_byItemNum*ComposeNum, pSerial[i] );
#endif
        m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		m_ItemData.AddAddInfo( m_pTmpBuffer );
	}

#ifdef _NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT
    if (byMatCount == 0) {
        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "TotalItemNum: %d", pItemSlot1->GetNum());
    }
#endif //_NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT


	m_LogMng.Serialize( m_ItemData );
}

void GameLog::LogFillSocket( Player * pPlayer, const SCItemSlot & rSocketSlot, const SCItemSlot & rTargetSlot, 
							 BYTE bySocketLevel, int iAttrIndex, int iValue )
{
	if( !IsRun() || !pPlayer  )		return;

	m_ItemData.Init();

	m_ItemData.SetLogType( ITEM_SOCKETOPT_FILL );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	// 아이템 정보
	m_ItemData.SetItemCode( rSocketSlot.GetItemCode() );

	m_ItemData.SetItemSerial( rSocketSlot.GetWorldSerial() );

	ITEMSTREAMEX ItemStreamEx;
	rSocketSlot.CopyOut(ItemStreamEx);

	m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

	rTargetSlot.CopyOut(ItemStreamEx);

	m_ItemData.SetItemStreamEx2( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",SockLV:%u,AttrIdx:%d,AttrVal:%u", bySocketLevel, iAttrIndex, iValue );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",SockLV:%u,AttrIdx:%d,AttVal:%u", bySocketLevel, iAttrIndex, iValue );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ItemData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ItemData );
}

void GameLog::LogItemExtrackSocket( Player * pPlayer, const SCItemSlot & rTargetSlot, WORD ItemCode, DURATYPE ItemNum )
{
	if( !IsRun() || !pPlayer  )		return;

	m_ItemData.Init();

	m_ItemData.SetLogType( ITEM_SOCKETOPT_EXTRACT );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}
	// 아이템 정보
	m_ItemData.SetItemCode( rTargetSlot.GetItemCode() );

	m_ItemData.SetItemSerial( rTargetSlot.GetWorldSerial() );

	ITEMSTREAMEX ItemStreamEx;
	rTargetSlot.CopyOut(ItemStreamEx);

	m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",ItemCode:%u,Num:%u", ItemCode, ItemNum );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",ItemCd:%u,Num:%u", ItemCode, ItemNum );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ItemData.AddAddInfo( m_pTmpBuffer );	

	m_LogMng.Serialize( m_ItemData );

}

void GameLog::LogMonsterItemDrop( const SCItemSlot & rItemSlot, DWORD dwMonsterCode, DWORD dwMonsterLevel, 
								 WORD wMapCode, const WzVector& vecPos, KEYTYPE RoomKey )
{
	// 로그양 줄이기 위해 몬스터 드랍 우선 로그 안남김
	// 20060802 오후 5:30:13
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return;
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#if SUN_CODE_BACKUP
	if( !IsRun() )		return;

	m_ItemData.Init();
	m_ItemData.SetLogType( ITEM_MONSTER_DROP );
	m_ItemData.SetMapCode( wMapCode );
	m_ItemData.SetMapX( (SHORT)vecPos.x );
	m_ItemData.SetMapY( (SHORT)vecPos.y );
	m_ItemData.SetMapZ( (SHORT)vecPos.z );


	m_ItemData.SetItemCode( rItemSlot.GetItemCode() );

	ITEMSTREAMEX ItemStreamEx;
	rItemSlot.CopyOut(ItemStreamEx);

	m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Room:%u", RoomKey );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ItemData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ItemData );
#endif
}

void GameLog::LogUseStorageItem(int iLogType, Player* player, SLOTCODE item_code, uint item_count, uint date_time)
{
    if( !IsRun() || !player  )		return;

    m_ItemData.Init();

    m_ItemData.SetLogType( iLogType );
    m_ItemData.SetAccountID( player->GetUserID(), ID_MAX_LEN );
    m_ItemData.SetCharName( player->GetCharName(), MAX_CHARNAME_LENGTH );
    m_ItemData.SetUserIP( player->GetUserIP(), MAX_IPADDRESS_SIZE );	

    GameZone *pZone = player->GetGameZonePtr();
    if( pZone )
    {
        WzVector vecPos;
        player->GetPos( &vecPos );

        m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
        m_ItemData.SetMapX( (short)vecPos.x );
        m_ItemData.SetMapY( (short)vecPos.y );
        m_ItemData.SetMapZ( (short)vecPos.z );
    }

    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Item:%u,Num:%u,Date:%u", item_code, item_count, date_time);
    m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
    m_ItemData.AddAddInfo(m_pTmpBuffer);
    m_LogMng.Serialize( m_ItemData );
}

void GameLog::LogMultiItem( int iLogType, Player * pPlayer, 
						   BYTE byCount, WORD * pItemCode IN, BYTE * pItemNum IN, DWORD * pDateTime IN )
{
	if( !IsRun() || !pPlayer  )		return;

	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

	for( BYTE i = 0 ; i < byCount ; ++i )
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Item:%u,Num:%u", pItemCode[i], pItemNum[i] );
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		m_ItemData.AddAddInfo( m_pTmpBuffer );
		if( pDateTime )
		{
			_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Date:%u", pDateTime[i] );
			m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
			m_ItemData.AddAddInfo( m_pTmpBuffer );
		}
	}

	m_LogMng.Serialize( m_ItemData );
}

void GameLog::LogShopItem( int iLogType, Player * pPlayer, 
						  DWORD dwPriceGuid, DWORD dwTransactionGuid, DWORD dwErrorCode )
{
	if( !IsRun() || !pPlayer  )		return;

	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",PriceGuid:%u,TransactionGuid:%u,ErrorCode:%u", dwPriceGuid, dwTransactionGuid, dwErrorCode );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",PrcGuid:%u,TrnGuid:%u,ErrCd:%u", dwPriceGuid, dwTransactionGuid, dwErrorCode );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ItemData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ItemData );
}

void GameLog::LogShop_RecvItem( int iLogType, Player * pPlayer, 
								DWORD GiftGuid, DWORD dwResult )
{
	if( !IsRun() || !pPlayer  )		return;

	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	// addinfo에 방키
//	SetRoomKey( m_ItemData, pPlayer );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",GiftGuid:%u,Result:%u", GiftGuid, dwResult );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",PakageSequence:%u,Rt:%u", GiftGuid, dwResult );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ItemData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ItemData );
}

//_KR_0_20091021_FIRENDINVITATION_EXTEND
void GameLog::LogItemInvitatedFriendReward( Player * pPlayer, BYTE ItemCount, SLOTCODE RewardItemCode, LEVELTYPE RewardLevel)
{
    if( !IsRun() || !pPlayer  )		return;

    int iLogType = ITEM_REWARD_EVENT;

    m_ItemData.Init();

    m_ItemData.SetLogType( iLogType );
    m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
    m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
    m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

    GameZone *pZone = pPlayer->GetGameZonePtr();
    if( pZone )
    {
        WzVector vecPos;
        pPlayer->GetPos( &vecPos );

        m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
        m_ItemData.SetMapX( (short)vecPos.x );
        m_ItemData.SetMapY( (short)vecPos.y );
        m_ItemData.SetMapZ( (short)vecPos.z );
    }

    // addinfo에 방키
    SetRoomKey( m_ItemData, pPlayer );

    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Item:%u,Num:%u,level:%u", RewardItemCode, ItemCount, RewardLevel );
    m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
    m_ItemData.AddAddInfo( m_pTmpBuffer );

    m_LogMng.Serialize( m_ItemData );
}

void GameLog::LogItemReward( BYTE byRewardType, Player * pPlayer, BYTE totalCount, WORD * IN pRewardItemCode, BYTE * IN pRewardItemNum,
							BYTE * IN pRewardItemEnchant, BYTE * IN pRewardItemSet, BYTE * IN pRewardItemRank, CODETYPE reward_code )
{
	if( !IsRun() || !pPlayer  )		
        return;

	int iLogType = ITEM_MAX;
	switch( byRewardType )
	{
	case RewardManager::eREWARD_QUEST:			iLogType = ITEM_REWARD_QUEST;		break;
	case RewardManager::eREWARD_AC:				iLogType = ITEM_REWARD_AC;			break;
	case RewardManager::eREWARD_MISSION:		iLogType = ITEM_REWARD_MISSION;		break;
	case RewardManager::eREWARD_EVENT:			iLogType = ITEM_REWARD_EVENT;		break;
	case RewardManager::eREWARD_COMPETITION:	iLogType = ITEM_REWARD_COMPETITION; break;
	case RewardManager::eREWARD_MISSIONRANKING:	iLogType = ITEM_REWARD_RANKING;		break;
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    case RewardManager::eREWARD_ATTENDANCE:	    iLogType = ITEM_REWARD_ATTENDANCE;	break;
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    case RewardManager::eREWARD_MINIGAME_BINGO: iLogType = ITEM_REWARD_MINIGAME_BINGO; break;
#endif //_NA_008404_20150712_MINIGAME_SYSTEM
	}	

	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		m_ItemData.SetMapCode( (WORD)pZone->GetMapCode() );
		m_ItemData.SetMapX( (short)vecPos.x );
		m_ItemData.SetMapY( (short)vecPos.y );
		m_ItemData.SetMapZ( (short)vecPos.z );
	}

	// addinfo에 방키
	SetRoomKey( m_ItemData, pPlayer );

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    if (RewardManager::eREWARD_AC == byRewardType)
    {
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Location:%u,Object:%u", 
            m_ACInfoData.location, m_ACInfoData.index_number);
        m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
        m_ItemData.AddAddInfo( m_pTmpBuffer );
    }
#endif //C_NA_0_20100520_ACSYSTEM_CHANGED

	for( BYTE i = 0 ; i < totalCount ; ++i )
	{
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",RewardCode:%d,Item:%u,Num:%u,Encht:%u,Set:%u,Rank:%u", 
                    reward_code, pRewardItemCode[i], pRewardItemNum[i], 
					pRewardItemEnchant[i], pRewardItemSet[i], pRewardItemRank[i] );
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		m_ItemData.AddAddInfo( m_pTmpBuffer );
	}

	m_LogMng.Serialize( m_ItemData );
}

//
////////////////////////////////////////////////////////////////////////////
//// 아이템 로그
//// 1. 아이템 드롭 로그(버림과 죽어서 떨구는것과 몬스터 죽여서 떨구는 것의 차이?)
//// 1. 아이템 드롭(땅에 떨굼) 과 줍기
//void GameLog::WriteItemOwner( Player* pPlayer, BOOL bPickUp, const SCItemSlot & rItemSlot )
//{
//	if( !IsRun() || !pPlayer  )		return;
//
//	m_ItemData.Init();
//
//	if( bPickUp == TRUE )		m_ItemData.SetBaseData( ITEM_PICK, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//	else						m_ItemData.SetBaseData( ITEM_DROP, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//
//	SetItemInfo( m_ItemData, rItemSlot );
//	SetPlayerPosInfo( m_ItemData, pPlayer );
//
//	SetRoomKey( m_ItemData, pPlayer );
//	SetBaseItemAddInfo( m_ItemData, rItemSlot );
//
//	m_LogMng.Serialize( m_ItemData );
//}
//
//// 2. 인첸트 성공 / 실패
//void GameLog::WriteEnchantResult( Player* pPlayer, BOOL bSuccess, const SCItemSlot & rItemSlot )
//{
//	if( !IsRun() || !pPlayer  )		return;
//
//	m_ItemData.Init();
//
//	if( bSuccess == TRUE)		m_ItemData.SetBaseData( ITEM_ENCHANTSUCCESS, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//	else						m_ItemData.SetBaseData( ITEM_ENCHANTFAIL, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//
//	SetItemInfo( m_ItemData, rItemSlot );
//	SetPlayerPosInfo( m_ItemData, pPlayer );
//
//	SetRoomKey( m_ItemData, pPlayer );
//	SetBaseItemAddInfo( m_ItemData, rItemSlot );
//	SetItemSlotPos( m_ItemData, rItemSlot );
//
//
//	m_LogMng.Serialize( m_ItemData );
//}
//
//// 3. NPC와의 거래
//void GameLog::WriteItemTradeWithNPC( Player* pPlayer, BOOL bSell, const SCItemSlot & rItemSlot )
//{
//	if( !IsRun() || !pPlayer  )		return;
//
//	m_ItemData.Init();
//	if( bSell == TRUE )  	m_ItemData.SetBaseData( ITEM_SELLNPC, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() ); 
//	else					m_ItemData.SetBaseData( ITEM_BUYNPC, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() ); 
//
//	SetItemInfo( m_ItemData, rItemSlot );
//	SetPlayerPosInfo( m_ItemData, pPlayer );
//
//	SetRoomKey( m_ItemData, pPlayer );
//	SetBaseItemAddInfo( m_ItemData, rItemSlot );
//	SetBaseItemPriceInfo( m_ItemData, pPlayer, rItemSlot, bSell );
//	
//	m_LogMng.Serialize( m_ItemData );
//}
//
////개인거래
//void GameLog::WriteItemTradeWithPlayer( Player* pPlayer1, BOOL bGive, const SCItemSlot & rItemSlot, Player* pPlayer2 )
//{
//	if( !IsRun() || !pPlayer1 || !pPlayer2  )		return;
//
//	m_ItemData.Init();
//	if( bGive == TRUE )  	m_ItemData.SetBaseData( ITEM_TRADE_GIVE, pPlayer1->GetUserID(), pPlayer1->GetCharName(), pPlayer1->GetUserIP() );	// 상대 플레이어에게 주기
//	else					m_ItemData.SetBaseData( ITEM_TRADE_RECEIVE, pPlayer1->GetUserID(), pPlayer1->GetCharName(), pPlayer1->GetUserIP() );	// 상대에게서 받기
//
//	SetItemInfo( m_ItemData, rItemSlot );
//	SetOtherPlayerInfo( m_ItemData, pPlayer2 );
//	SetPlayerPosInfo( m_ItemData, pPlayer1 );
//
//	SetRoomKey( m_ItemData, pPlayer1 );
//	SetBaseItemAddInfo( m_ItemData, rItemSlot );
//	SetItemSlotPos( m_ItemData, rItemSlot );
//
//	m_LogMng.Serialize( m_ItemData );
//}
//
//// 5. 돈 개인거래
//void GameLog::WriteMoneyTradeWithPlayer( Player* pPlayer1, BOOL bGive, DWORD dwMoney, Player* pPlayer2 )
//{
//	if( !IsRun() || !pPlayer1 || !pPlayer2 )		return;
//
//	m_ItemData.Init();
//
//	//아직 돈 주기 받기 구분이 안 되어 있다. 추후 고려해서 나눈다.
//	if( bGive == TRUE )			m_ItemData.SetBaseData( ITEM_MONEY, pPlayer1->GetUserID(), pPlayer1->GetCharName(), pPlayer1->GetUserIP() ); // 상대 플레이어에게 주기
//	else						m_ItemData.SetBaseData( ITEM_MONEY, pPlayer1->GetUserID(), pPlayer1->GetCharName(), pPlayer1->GetUserIP() ); // 상대 플레이어에게 받기
//
//	SetOtherPlayerInfo( m_ItemData, pPlayer2 );
//	SetPlayerPosInfo( m_ItemData, pPlayer1 );
//
//	SetRoomKey( m_ItemData, pPlayer1 );
//
//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",%u,%I64u", dwMoney, pPlayer1->GetMoney() ); //< 거래한 돈, 소지 하임값
//	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
//	m_ItemData.AddAddInfo( m_pTmpBuffer );
//
//
//	m_LogMng.Serialize( m_ItemData );
//}
//
//// 6. 개인상점 거래
//void GameLog::WriteItemVendorWithPlayer( Player* pPlayer1, BOOL bSell, const SCItemSlot & rItemSlot,  DWORD dwMoney, Player* pPlayer2 )
//{
//	if( !IsRun() || !pPlayer1 || !pPlayer2 )		return;
//
//	m_ItemData.Init();
//	if( bSell == TRUE )  	m_ItemData.SetBaseData( ITEM_SELLPC, pPlayer1->GetUserID(), pPlayer1->GetCharName(), pPlayer1->GetUserIP() ); // 개인삼정에서 물건 팔기
//	else					m_ItemData.SetBaseData( ITEM_BUYPC, pPlayer1->GetUserID(), pPlayer1->GetCharName(), pPlayer1->GetUserIP() );  // 개인상점에서 물건 사기
//
//	SetItemInfo( m_ItemData, rItemSlot );
//	SetOtherPlayerInfo( m_ItemData, pPlayer2 ); 
//	SetPlayerPosInfo( m_ItemData, pPlayer1 );
//
//	SetRoomKey( m_ItemData, pPlayer1 );
//
//	SetBaseItemAddInfo( m_ItemData, rItemSlot );
//	SetTradeItemPriceInfo( m_ItemData, rItemSlot, dwMoney, bSell );
//
//
//	m_LogMng.Serialize( m_ItemData );
//}
//
//void GameLog::LogRankUp_Before( Player * pPlayer, const SCItemSlot & rItemSlot1, const SCItemSlot & rItemSlot2 )
//{
//	if( !IsRun() || !pPlayer  )		return;
//
//	m_ItemData.Init();
//	m_ItemData.SetBaseData( ITEM_RANKUP_BEFORE, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//
//	ITEMSTREAMEX ItemStreamEx;
//	rItemSlot1.CopyOut(ItemStreamEx);
//	m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx );
//
//	rItemSlot2.CopyOut(ItemStreamEx);
//	m_ItemData.SetItemStreamEx2( (const BYTE*)&ItemStreamEx );
//
//	m_LogMng.Serialize( m_ItemData );
//}

//// 7. 아이템 랭크업
//void GameLog::WriteItemRankUpResult( Player* pPlayer, const SCItemSlot & rItemSlot, DWORD dwMoney )
//{
//	if( !IsRun() || !pPlayer )		return;
//
//	m_ItemData.Init();
//	m_ItemData.SetBaseData( ITEM_RANKUPSUCCESS, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//	SetItemInfo( m_ItemData, rItemSlot );
//
//	SetRoomKey( m_ItemData, pPlayer );
//	SetBaseItemAddInfo( m_ItemData, rItemSlot );
//
//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",%u", dwMoney ); //< 사용 하임 값
//	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
//	m_ItemData.AddAddInfo( m_pTmpBuffer );
//	SetItemSlotPos( m_ItemData, rItemSlot );					//아이템 슬롯 위치
//
//	m_LogMng.Serialize( m_ItemData );
//}
//
//// 8. 창고에 맞기기 / 찾기
//// dwMoney : 창고 보관료
//void GameLog::WriteItemMoveInWareHouse( Player* pPlayer, BOOL bKeep, const SCItemSlot & rItemSlot )
//{
//	if( !IsRun() || !pPlayer )		return;
//
//	m_ItemData.Init();
//
//	if( bKeep == TRUE )  	m_ItemData.SetBaseData( ITEM_PUTSTORAGE, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() ); // 창고에 보관
//	else					m_ItemData.SetBaseData( ITEM_GETSTORAGE, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() ); // 창고에서 꺼내기
//	SetItemInfo( m_ItemData, rItemSlot );
//	SetPlayerPosInfo( m_ItemData, pPlayer );
//
//	SetRoomKey( m_ItemData, pPlayer );
//	SetBaseItemAddInfo( m_ItemData, rItemSlot );
//
//	if( bKeep == TRUE )	//보관
//	{
//
//	}
//	else //< 찾을때
//	{
//		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",%I64u", rItemSlot.GetPriceForBuy() ); //< 아이템를 살때 가격.
//		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
//		m_ItemData.AddAddInfo( m_pTmpBuffer );
//	}
//	
//
//
//	m_LogMng.Serialize( m_ItemData );
//}
//

void GameLog::LogEnchant( int iLogType, Player* pPlayer, const SCItemSlot & rItemSlot, 
						 MONEY ullMoney, STLX_VECTOR<COMPOSITEMATERIAL> & vecMatInfo, BYTE byDownLV  )
{
	if( !IsRun() || !pPlayer  )		return;

	if( ITEM_ENCHANT_SUCCESS	 != iLogType &&
		ITEM_ENCHANT_FAILED		 != iLogType &&
		ITEM_ENCHANT_FAILED_DOWN != iLogType 
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        && ITEM_ENCHANT_FAILED_DESTROY != iLogType
        && ITEM_ENCHANT_FAILED_PREVENT != iLogType
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        )
	{
		return;
	}

	m_ItemData.Init();

	m_ItemData.SetLogType( iLogType );
	m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	// 아이템 정보
	m_ItemData.SetItemCode( rItemSlot.GetItemCode() );

	m_ItemData.SetItemSerial( rItemSlot.GetWorldSerial() );

	m_ItemData.SetItemNum( rItemSlot.GetNum() );
	m_ItemData.SetMoney( ullMoney );

	ITEMSTREAMEX ItemStreamEx;
	rItemSlot.CopyOut(ItemStreamEx);

	m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

	//인첸트 재료 
	for( int i = 0 ; i < (int)vecMatInfo.size() ; ++i )
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Item:%u,Num:%u", vecMatInfo[i].MaterialCode, vecMatInfo[i].MaterialNum ); //< 아이템를 살때 가격.
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		m_ItemData.AddAddInfo( m_pTmpBuffer );
	}

	if( byDownLV )
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "ItemDownLV:%d", byDownLV ); //< 아이템다운값이 있을시 추가해준다.
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		m_ItemData.AddAddInfo( m_pTmpBuffer );
	}
	m_LogMng.Serialize( m_ItemData );
}
//// 9. 인첸트 시도
//void GameLog::WriteEnchantStart( Player* pPlayer, const SCItemSlot & rItemSlot, STLX_VECTOR<COMPOSITEMATERIAL> vecMatInfo )
//{
//	if( !IsRun() || !pPlayer )		return;
//
//	m_ItemData.Init();
//	m_ItemData.SetBaseData( ITEM_ENCHANTTRY, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//
//	SetItemInfo( m_ItemData, rItemSlot );
//	SetPlayerPosInfo( m_ItemData, pPlayer );
//
//	SetRoomKey( m_ItemData, pPlayer );
//
//	SetBaseItemAddInfo( m_ItemData, rItemSlot );
//	SetItemPrice( m_ItemData, rItemSlot, FALSE );		//아이템 살때 가격 
//	SetItemSlotPos( m_ItemData, rItemSlot );			//아이템슬롯 위치
//
//	//인첸트 재료 
//	for(int i = 0; i < (int)vecMatInfo.size(); i++)
//	{
//		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",%u,%u", vecMatInfo[i].MaterialCode, vecMatInfo[i].MaterialNum ); //< 아이템를 살때 가격.
//		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
//		m_ItemData.AddAddInfo( m_pTmpBuffer );
//	}
//
//
//	m_LogMng.Serialize( m_ItemData );
//}
//
//// 10. 몬스터 아이템 드랍
//void GameLog::WriteMonsterItemDrop( const SCItemSlot & rItemSlot, DWORD dwMonsterCode, DWORD dwMonsterLevel, int nMapCode, WzVector& vecPos, KEYTYPE RoomKey )
//{
//	if( !IsRun() )		return;
//
//	m_ItemData.Init();
//	m_ItemData.SetLogType( ITEM_MONSTER_ITEMDROP );
//	SetItemInfo( m_ItemData, rItemSlot );
//	m_ItemData.SetMapCode( nMapCode );
//	m_ItemData.SetMapX( (short)vecPos.x );
//	m_ItemData.SetMapY( (short)vecPos.y );
//	m_ItemData.SetMapZ( (short)vecPos.z );
//
//	//1. RoomKey
//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",%u", RoomKey ); //< 아이템를 살때 가격.
//	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
//	m_ItemData.AddAddInfo( m_pTmpBuffer );
//
//	//2. Item
//	SetBaseItemAddInfo( m_ItemData, rItemSlot );
//
//
//	m_LogMng.Serialize( m_ItemData );
//}
//
//// 11. 아이템 개인거래 요청 / 승인 / 요청거부
//// nType (0 : 거래 요청, 1 : 승인, 2 : 거절)
//void GameLog::WriteItemTradeStatus( Player * pPlayer1, Player * pPlayer2, int nType )
//{
//	if( !IsRun() || !pPlayer1 || !pPlayer2 )		return;
//
//	m_ItemData.Init();
//
//	switch( nType )				// 아이템 거래 요청
//	{
//	case 0:
//		m_ItemData.SetBaseData( ITEM_REQUEST, pPlayer1->GetUserID(), pPlayer1->GetCharName(), pPlayer1->GetUserIP() );
//		break;
//	case 1:			// 승인
//		m_ItemData.SetBaseData( ITEM_ACCEPT, pPlayer1->GetUserID(), pPlayer1->GetCharName(), pPlayer1->GetUserIP() );
//		break;
//	case 2:			// 거절
//		m_ItemData.SetBaseData( ITEM_REJECT, pPlayer1->GetUserID(), pPlayer1->GetCharName(), pPlayer1->GetUserIP() );
//		break;
//	default:
//		return;
//	}
//
//	SetOtherPlayerInfo( m_ItemData, pPlayer2 );
//	SetRoomKey( m_ItemData, pPlayer1 );
//
//	m_LogMng.Serialize( m_ItemData );
//}
//
//// 12. 개인상점 개설/폐쇄
//void GameLog::WriteItemVendorStatus( Player* pPlayer, BOOL bOpen )
//{
//	if( !IsRun() || !pPlayer )		return;
//
//	m_ItemData.Init();
//
//	if( bOpen == TRUE )		m_ItemData.SetBaseData( ITEM_NEWPC, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//	else					m_ItemData.SetBaseData( ITEM_DELPC, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//
//	SetPlayerPosInfo( m_ItemData, pPlayer );
//
//	SetRoomKey( m_ItemData, pPlayer );
//
//	m_LogMng.Serialize( m_ItemData );
//}
//
//// 13. 개인상점 보기
//void GameLog::WriteViewItemVendor( Player* pPlayer1, Player* pPlayer2 )
//{
//	if( !IsRun() || !pPlayer1 || !pPlayer2 )		return;
//
//	m_ItemData.Init();
//
//	m_ItemData.SetBaseData( ITEM_WATCHPC, pPlayer1->GetUserID(), pPlayer1->GetCharName(), pPlayer1->GetUserIP() );
//	SetOtherPlayerInfo( m_ItemData, pPlayer2 );
//
//	SetRoomKey( m_ItemData, pPlayer1);
//
//	m_LogMng.Serialize( m_ItemData );
//}
//

// 14. 창고 정보
void GameLog::WriteWareHouseInfo( Player* pPlayer, BOOL bOpen )
{
	if( !IsRun() || !pPlayer )		return;

	m_WareHouseData.Init();

	if( bOpen == TRUE )
	{
		m_WareHouseData.SetLogType( SNAPSHOT_WAREHOUSE_OEPN );
		m_WareHouseData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
		m_WareHouseData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
		m_WareHouseData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	}
	else
	{
		m_WareHouseData.SetLogType( SNAPSHOT_WAREHOUSE_CLOSE );
		m_WareHouseData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
		m_WareHouseData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
		m_WareHouseData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	}

	// 창고 시작 상태로 바꿈
	if( m_WareHouseData.GetWareHouseSize() >= MAX_WAREHOUSESTREAM_SIZE )
	{
		CWarehouseSlotContainer * pContainer = pPlayer->GetWarehouseContainer();
		if( !pContainer )	return;

		pContainer->SerializeItemInfo( m_WareHouseData.GetWareHousePtr(), m_WareHouseData.GetWareHouseSize(), SERIALIZE_LOAD );
		m_WareHouseData.SetMoney( pContainer->GetCurMoney() );

		m_LogMng.Serialize( m_WareHouseData );
	}
}

//
//// 15. 개인상점 정보(가격설정)
//void GameLog::WriteVendorInfo( Player* pPlayer, const SCItemSlot & rItemSlot, DWORD dwMoney )
//{
//	if( !IsRun() || !pPlayer )		return;
//
//	m_ItemData.Init();
//
//	m_ItemData.SetBaseData( ITEM_SETPRICE_PC, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );	
//	SetItemInfo( m_ItemData, rItemSlot );
//	SetPlayerPosInfo( m_ItemData, pPlayer );
//
//	SetRoomKey( m_ItemData, pPlayer );
//	SetBaseItemAddInfo( m_ItemData, rItemSlot );
//	SetTradeItemPriceInfo( m_ItemData, rItemSlot, dwMoney, FALSE );
//	
//	m_LogMng.Serialize( m_ItemData );
//}
//
//// 16. 랭크업 시도
//void GameLog::WriteItemRankUpStart( Player* pPlayer, POSTYPE pos1, POSTYPE pos2 )
//{
//	if( !IsRun() || !pPlayer )		return;
//
//	SCSlotContainer* pInventory = (SCSlotContainer*)pPlayer->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
//	if( !pInventory )	return;
//
//	SCItemSlot& rTargetSlot1 = (SCItemSlot &)pInventory->GetSlot(pos1);
//	SCItemSlot& rTargetSlot2 = (SCItemSlot &)pInventory->GetSlot(pos2);
//
//	m_ItemData.Init();
//	m_ItemData.SetBaseData( ITEM_RANKUPTRY, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//
//	//1번째 사용된 아이템
//	SetItemInfo( m_ItemData, rTargetSlot1 );
//
//	SetRoomKey( m_ItemData, pPlayer );
//	SetBaseItemAddInfo( m_ItemData, rTargetSlot1 );		//1번재 사용된 아이템 추가정보.
//	SetFullItemAddInfo( m_ItemData, rTargetSlot2 );		//2번째 사용된 아이템.
//
//	m_LogMng.Serialize( m_ItemData );
//}

void GameLog::LogAction( int iLogType, Player * pPlayer, Player* pOtherPlayer, DWORD64 dwReservedData1, DWORD64 dwReservedData2 )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	m_ActionData.SetLogType( iLogType );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	SetPlayerPosInfo( m_ActionData, pPlayer );
//	SetRoomKey( m_ActionData, pPlayer );

	if( 0 != dwReservedData1 )
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",%I64u,%I64u", dwReservedData1, dwReservedData2 );
		m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
		m_ActionData.AddAddInfo(m_pTmpBuffer);
	}

	if( pOtherPlayer )
	{
		SetBasePlayerAddInfo( m_ActionData, pOtherPlayer );
	}
	
	if( iLogType == ACT_JOIN_MAP )
	{
		GameZone *pZone = pPlayer->GetGameZonePtr();
        if( pZone ) {
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
            _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Class:%u",
                (pPlayer->GetCharInfo() != NULL) ? pPlayer->GetCharInfo()->m_byClassCode : eCLASS_NONE);
#else  // _NA_007750_20141113_WZDAT_MODIFY_LOG
            _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "MapCode:%u", pZone->GetMapCode() );
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
            m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
            m_ActionData.AddAddInfo(m_pTmpBuffer);
        }
	}
	else if( iLogType == ACT_QUEST_REWARD || iLogType == ACT_QUEST_ACCEPT )
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "QuestCode:%u", dwReservedData1 );
        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
        m_ActionData.AddAddInfo(m_pTmpBuffer);
	}
    else if (iLogType == ACT_DOMINATION_KILL ||
             iLogType == ACT_DOMINATION_DIE)
    {
        if (pOtherPlayer)
        {
            GameZone* pZone = pPlayer->GetGameZonePtr();
            const GUILDGUID guildGuid = pOtherPlayer->GetGuildGuid();
            GameGuild* const pGuild = g_GameGuildManager.FindGuild(guildGuid);

            if (NULL != pZone) 
            {
                const MAPCODE mapCode = pZone->GetMapCode();
                _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "MapCode:%u", mapCode);
                m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
                m_ActionData.AddAddInfo(m_pTmpBuffer);

            }
            if (NULL != pGuild)
            {
                const TCHAR* guildName = pGuild->GetGuildName();

                _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, ", GuildName:%s", guildName);
                m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';

                m_ActionData.AddAddInfo(m_pTmpBuffer);
            }
        }
    }
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    else if(iLogType == ACT_FRIENDINVITATION_RECALL)
    {   
        if( pOtherPlayer )
        {
            GameZone *pZone = pOtherPlayer->GetGameZonePtr();
            if( pZone )
            {
                _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ", Map:%u", pZone->GetMapCode());
                m_ActionData.AddAddInfo(m_pTmpBuffer);
            }
        }
    }

	m_LogMng.Serialize( m_ActionData );
}

void GameLog::LogSimpleAction( int iLogType, Player* pPlayer, const TCHAR* tszAddInfo )
{
    if( !IsRun() || !pPlayer ) {
        return;
    }

    m_ActionData.Init();

    m_ActionData.SetLogType( iLogType );
    m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
    m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
    m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

    SetPlayerPosInfo( m_ActionData, pPlayer );

    if (tszAddInfo != NULL) {
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "%s", tszAddInfo );
        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
        m_ActionData.AddAddInfo(m_pTmpBuffer);
    }
    m_LogMng.Serialize( m_ActionData );
}

void GameLog::LogAction_Ranking( int iLogType, Player * pPlayer, RANKINGNO MissionNo, BYTE Ranking, DWORD ClearTime, BYTE Error )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	m_ActionData.SetLogType( iLogType );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	SetPlayerPosInfo( m_ActionData, pPlayer );
//	SetRoomKey( m_ActionData, pPlayer );

	/*
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, 
		",MissionNo:%u,Ranking:%u,Time:%u,Error:%u", MissionNo, Ranking, ClearTime, Error );
		*/
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, 
		",MisnNo:%u,Rnkg:%u,Time:%u,Err:%u", MissionNo, Ranking, ClearTime, Error );
	m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
	m_ActionData.AddAddInfo(m_pTmpBuffer);

	m_LogMng.Serialize( m_ActionData );
}

void GameLog::LogKillSpecialMonster( Player * pPlayer, MONSTERCODE MonCode, BYTE MonsterType )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	m_ActionData.SetLogType( ACT_KILL_SPECIAL_MONSTER );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_ActionData, pPlayer );
	//	SetRoomKey( m_ActionData, pPlayer );
	
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "MonCode:%u, MonType:%u", MonCode, MonsterType );
	m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
	m_ActionData.AddAddInfo(m_pTmpBuffer);	

	m_LogMng.Serialize( m_ActionData );
}

void GameLog::LogFatigue( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	// 일정 주기로 로그를 남기기 위해
	m_ActionData.Init();

	SetPlayerPosInfo( m_ActionData, pPlayer );

	m_ActionData.SetLogType( ACT_FATIGUE_PLAY_TIME );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Time:%d", pPlayer->GetFatigueManager()->GetPlayTime() );
	m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
	m_ActionData.AddAddInfo(m_pTmpBuffer);

	m_LogMng.Serialize( m_ActionData );
}

void GameLog:: LogRankingPoint( Player *pPlayer, DWORD GetPoint, DWORD SumPoint )
{
	if( !IsRun() || !pPlayer )		return;	

	m_ActionData.Init();

	SetPlayerPosInfo( m_ActionData, pPlayer );

	m_ActionData.SetLogType( ACT__RANKING_POINT );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Get:%u, Sum:%u", GetPoint, SumPoint );
	m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
	m_ActionData.AddAddInfo(m_pTmpBuffer);

	m_LogMng.Serialize( m_ActionData );
}

//지역 점령전 관련 아이템 사용시 로그
void GameLog::LogDomination(int log_type, Player * player, SLOTCODE item_code, 
                            BYTE item_num, MAPCODE map_code)
{
    if (!IsRun() || !player)
    {
        return;
    }

    assert(log_type >= ITEM_MONSTER_DROP || log_type < ITEM_MAX );

    m_ItemData.Init();
    m_ItemData.SetLogType(log_type);
    m_ItemData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ItemData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ItemData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);

    GameZone *pZone = player->GetGameZonePtr();
    if (pZone)
    {
        WzVector vecPos;
        player->GetPos(&vecPos);

        m_ItemData.SetMapCode((WORD)pZone->GetMapCode());
        m_ItemData.SetMapX((short)vecPos.x);
        m_ItemData.SetMapY((short)vecPos.y);
        m_ItemData.SetMapZ((short)vecPos.z);
    }

    m_ItemData.SetItemCode(item_code);
    m_ItemData.SetItemNum(item_num);

    if (log_type == ITEM_DOMINATION_AUCTION_APPLY || log_type == ITEM_DOMINATION_AUCTION_DUP_APPLY)
    {
        GameGuild* guild = g_GameGuildManager.FindGuild(player->GetGuildGuid());
        if (guild == NULL)
        {
            return;
        }
        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Domination Map:%d, Guild Name:%s", 
                   map_code, guild->GetGuildName());
        m_pTmpBuffer[_countof(m_pTmpBuffer) - 1] = '\0';
        m_ItemData.AddAddInfo(m_pTmpBuffer);
    }
    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    else if (log_type == ACT_DOMINATION_ADD_DEFFENSEWALL_POINT)
    {
        GameGuild* guild = g_GameGuildManager.FindGuild(player->GetGuildGuid());
        if(guild == NULL)
        {
            return;
        }

        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "Domination Map:%d, Guild Name : %s", 
                   map_code, guild->GetGuildName());
        m_pTmpBuffer[_countof(m_pTmpBuffer) - 1] = '\0';
        m_ItemData.AddAddInfo(m_pTmpBuffer);
    }
    m_LogMng.Serialize(m_ItemData);
}

void GameLog::LogDomination(int iLogType, MAPCODE map_code)
{
    if (FALSE == IsRun())
    {
        return;
    }
    
    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    m_ActionData.Init();
    m_ActionData.SetLogType(iLogType);

    const DOMINATION_INFO* domination_info = domination_manager->GetDominationInfo(map_code);
    if (NULL == domination_info)
    {
        return;
    }

    TCHAR* guildName = NULL;

    if (iLogType == ACT_DOMINATION_SUCCESS_DEFENCE)
    {
        const TCHAR* guildName = domination_info->m_inGuildInfo.m_tszGuildName;

        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "MapCode:%u, GuildName:%s", map_code, guildName);
        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';

        m_ActionData.AddAddInfo(m_pTmpBuffer);
    }
    else if (iLogType == ACT_DOMINATION_SUCCESS_OFFENCE)
    {
        const TCHAR* guildName = domination_info->m_disGuildInfo.m_tszGuildName;

        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, "MapCode:%u, GuildName:%s", map_code, guildName);
        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';

        m_ActionData.AddAddInfo(m_pTmpBuffer);
    }
    else if (iLogType == ACT_DOMINATION_REWORD_EVENT_START)
    {
        const TCHAR* guild_name = (domination_info->m_inGuildInfo.m_tszGuildName[0] != '\0') ?
            domination_info->m_inGuildInfo.m_tszGuildName : _T("NULL");
        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, _T("MapCode:%u, GuildName:%s"), map_code, guild_name);
        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
        m_ActionData.AddAddInfo(m_pTmpBuffer);
    }
    else if (iLogType == ACT_DOMINATION_SET_ATTACK_GUILD)
    {
        const TCHAR* guild_name = (domination_info->m_disGuildInfo.m_tszGuildName[0] != '\0') ? 
            domination_info->m_disGuildInfo.m_tszGuildName : _T("NULL");

        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, _T("Map_code:%u, AttackGuildName:%s"), 
            domination_info->m_FieldCode, guild_name);

        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
        m_ActionData.AddAddInfo(m_pTmpBuffer);
    }

    m_LogMng.Serialize( m_ActionData );
}

void GameLog::LogDomination(int iLogType, Player* pPlayer, NPC* pNpc, 
                            BYTE gate1, BYTE gate2, BYTE gate3)
{
    if (FALSE == IsRun() ||
        NULL  == pPlayer)
        return;

    m_ActionData.Init();

    m_ActionData.SetLogType(iLogType);
    m_ActionData.SetAccountID(pPlayer->GetUserID(), ID_MAX_LEN);
    m_ActionData.SetCharName(pPlayer->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ActionData.SetUserIP(pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE);	

    SetPlayerPosInfo( m_ActionData, pPlayer );


    MAPCODE mapCode = 0;
    const GameZone* pZone = pPlayer->GetGameZonePtr();

    _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, ".");
    m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';

    m_ActionData.AddAddInfo(m_pTmpBuffer);


    const GUILDGUID guildGuid = pPlayer->GetGuildGuid();
    GameGuild* const pGuild = g_GameGuildManager.FindGuild(guildGuid);
    if (NULL != pGuild)
    {
        const TCHAR* guildName = pGuild->GetGuildName();

        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, ", GuildName:%s", guildName);
        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';

        m_ActionData.AddAddInfo(m_pTmpBuffer);
    }

    if (NULL != pNpc)
    {
        GameZone* pZone = pNpc->GetGameZonePtr();
        if (NULL != pZone)
        {
            WzVector vecPos;
            pNpc->GetPos(&vecPos);

            _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, 
                ", X:%0.3f,Y:%0.3f,Z:%0.3f", vecPos.x, vecPos.y, vecPos.z);
            m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';

            m_ActionData.AddAddInfo(m_pTmpBuffer);
        }
    }


    switch(iLogType)
    {
    case ACT_DOMINATION_DESTROY_OBJECT:
        if (NULL != pNpc)
        {
            const TCHAR* charName = pNpc->GetCharName();

            _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, ", ObjectName:%s", charName);
            m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';

            m_ActionData.AddAddInfo(m_pTmpBuffer);
        }
        break;

    case ACT_DOMINATION_SET_GATE:
        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, ", L:%u,C:%u,R:%u", gate1,gate2,gate3);
        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';

        m_ActionData.AddAddInfo(m_pTmpBuffer);
        break;
    }

    m_LogMng.Serialize( m_ActionData );
}

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
void GameLog::LogAccumulatePoint( int iLogType, Player* pPlayer, WORD IncreasePoint, SLOTCODE RewardItemCode )
{
    if( !IsRun() || !pPlayer ) return;

    BASE_PLAYERINFO* pInfo = pPlayer->GetCharInfo();
    ASSERT(pInfo);
    if ( !pInfo )
        return;

    WORD PreviousPoint = pInfo->m_wAccumulatePoint;
    switch(iLogType)
    {
    case ACCUM_ACQUISITION_TIME:
    case ACCUM_ACQUISITION_QUEST:
        PreviousPoint = max((pInfo->m_wAccumulatePoint - IncreasePoint), 0);
        break;
    case ACCUM_CONSUMPTION_ROULETTE:
        PreviousPoint = min((pInfo->m_wAccumulatePoint + IncreasePoint), 65000);
        break;
    }

    m_AccumulatePointData.Init();
    m_AccumulatePointData.SetLogType( iLogType );
    m_AccumulatePointData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
    m_AccumulatePointData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
    m_AccumulatePointData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

    m_AccumulatePointData.SetAccumulateYear(pInfo->m_wAccumulateYear);
    m_AccumulatePointData.SetAccumulateTime(pInfo->m_dwAccumulateTime);
    m_AccumulatePointData.SetOldAccumulatePoint(PreviousPoint);
    m_AccumulatePointData.SetNewAccumulatePoint(pInfo->m_wAccumulatePoint);
    m_AccumulatePointData.SetRewardItemCode(RewardItemCode);

    m_LogMng.Serialize( m_AccumulatePointData );
}

void GameLog::LogAccumulatePoint_GM( int iLogType, Player* pPlayer, WORD IncreasePoint, SLOTCODE RewardItemCode )
{
	if( !IsRun() || !pPlayer ) return;

	BASE_PLAYERINFO* pInfo = pPlayer->GetCharInfo();
	ASSERT(pInfo);
	if ( !pInfo )
		return;

	WORD PreviousPoint = pInfo->m_wAccumulatePoint;
	switch(iLogType)
	{
	case GM_ACQUISITION_ACCMULATE_POINT_BY_QUEST:
	case GM_ACQUISITION_ACCMULATE_POINT:
		PreviousPoint = max((pInfo->m_wAccumulatePoint - IncreasePoint), 0);
		break;
	case GM_CONSUMPTION_ACCMULATE_POINT:
		PreviousPoint = min((pInfo->m_wAccumulatePoint + IncreasePoint), 65000);
		break;
	}

	m_AdminData.Init();	
	m_AdminData.SetLogType( iLogType );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Year:%u,Time:%u,PreviousPoint:%u,CurrentPoint:%u,RewardItem:%u",
				pInfo->m_wAccumulateYear, pInfo->m_dwAccumulateTime, PreviousPoint, pInfo->m_wAccumulatePoint, RewardItemCode );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_AdminData.AddAddInfo( m_pTmpBuffer );


	m_LogMng.Serialize( m_AdminData );
}

#endif

//////////////////////////////////////////////////////////////////////////
// 유저액션 로그
//1. 부활 로그
void GameLog::WritePlayerResurrection( Player* pPlayer, Player *pHealer )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	m_ActionData.SetLogType( ACT_REBIRTH_CHAR );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_ActionData, pPlayer );

//	SetRoomKey( m_ActionData, pPlayer );

	// 부활시켜준 사람의 정보
	if( pHealer )
	{
		SetBasePlayerAddInfo( m_ActionData, pHealer );
	}

	m_LogMng.Serialize( m_ActionData );

}

//캐릭터 사망
void GameLog::WritePlayerDead( Player* pPlayer, MAPCODE Mapcode )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	m_ActionData.SetLogType( ACT_DEAD_CHAR );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_ActionData, pPlayer );

//	SetRoomKey( m_ActionData, pPlayer );
	if( Mapcode )
    {
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
        _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE,_T("MCode:%u,Class:%u"), Mapcode,
            (pPlayer->GetCharInfo() != NULL) ? pPlayer->GetCharInfo()->m_byClassCode : eCLASS_NONE);
#else  // _NA_007750_20141113_WZDAT_MODIFY_LOG
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "MCode:%u", Mapcode );
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
        m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
        m_ActionData.AddAddInfo(m_pTmpBuffer); // CHANGES: f110602.3L, fixed the invalid data setting.
	}

	m_LogMng.Serialize( m_ActionData );
}

//스텟사용
void GameLog::WriteUseStat( Player* pPlayer, BYTE byStatType )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	m_ActionData.SetLogType( ACT_USE_STAT );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "StatPt:%u,StatType:%u", pPlayer->GetRemainStatPoint(), byStatType );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ActionData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ActionData );
}

//스텟 감소
void GameLog::WriteDecreaseStat( Player* pPlayer, BYTE byStatType, SHORT Decrease )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	m_ActionData.SetLogType( ACT_DECREASE_STAT );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	/*
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "RemainStatPt:%u,StatType:%u,Decrease:%u",
		pPlayer->GetRemainStatPoint(), byStatType, Decrease );
		*/
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "RmnStatPt:%u,StatType:%u,Dec:%u",
		pPlayer->GetRemainStatPoint(), byStatType, Decrease );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ActionData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ActionData );
}

//스텟 초기화
void GameLog::WriteInitStatAll( Player* pPlayer, eLOG_ACTION eLogAction )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	m_ActionData.SetLogType( eLogAction );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	
//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "RemainStatPt:%u", pPlayer->GetRemainStatPoint() );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "RmnStatPt:%u", pPlayer->GetRemainStatPoint() );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ActionData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_ActionData );
}

// 7. 스킬레벨업
void GameLog::WriteSkillLevelUp( Player* pPlayer, SLOTCODE SkillCode, BOOL bLevelUp )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	//레벨업이 아니면, 처음 스킬을 배우는 거다.
	if( bLevelUp == FALSE )
	{
		m_ActionData.SetLogType( ACT_ACQUIRE_SKILL );
		m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
		m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
		m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	}
	else
	{
		m_ActionData.SetLogType( ACT_SKILLLV_UP );
		m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
		m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
		m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	}

	ROOT_SKILLINFO *pRootInfo = SkillInfoParser::Instance()->GetInfo( SkillCode );
	if( !pRootInfo )			return;

	if( pRootInfo->IsSkill() )
	{
		SkillScriptInfo *pBaseSkillInfo = (SkillScriptInfo*)pRootInfo;

		/*
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "SkillPt:%d,Skill:%u,ReqSKillPt:%u", 
			pPlayer->GetRemainSkillPoint(), SkillCode, pBaseSkillInfo->m_byRequireSkillPoint );
			*/
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "SklPt:%d,Skill:%u,ReqSklPt:%u", 
			pPlayer->GetRemainSkillPoint(), SkillCode, pBaseSkillInfo->m_byRequireSkillPoint );
	}
	else
	{
		BASE_STYLEINFO *pBaseStyleInfo = (BASE_STYLEINFO*)pRootInfo;
		/*
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "SkillPt:%d,Skill:%u,ReqSKillPt:%u", 
			pPlayer->GetRemainSkillPoint(), SkillCode, pBaseStyleInfo->m_byRequireSkillPoint );
			*/
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "SklPt:%d,Skill:%u,ReqSklPt:%u", 
			pPlayer->GetRemainSkillPoint(), SkillCode, pBaseStyleInfo->m_byRequireSkillPoint );
	}

	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ActionData.AddAddInfo( m_pTmpBuffer );


	m_LogMng.Serialize( m_ActionData );
}

//8. PVP 상대방 죽이기 로그
void GameLog::WritePVPKillUser( Player* pPlayer, Player* pOtherPlayer, BOOL bKill )
{
	if( !IsRun() || !pPlayer || !pOtherPlayer )		return;

	m_ActionData.Init();

	if( bKill == TRUE )
	{
		m_ActionData.SetLogType( ACT_PVPKILL );
		m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
		m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
		m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	}
	else
	{
		m_ActionData.SetLogType( ACT_PVPDIE );
		m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
		m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
		m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	}

	SetRoomKey( m_ActionData, pPlayer );
	SetBasePlayerAddInfo( m_ActionData, pOtherPlayer );


	m_LogMng.Serialize( m_ActionData );
}

//9. PVP 시작
void GameLog::WritePVPStart( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	m_ActionData.SetLogType( ACT_PVP_START );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetRoomKey( m_ActionData, pPlayer );
	m_LogMng.Serialize( m_ActionData );
}

//스킬 레벨 다운
void GameLog::WriteSkillLevelDown( Player* pPlayer, SLOTCODE SkillCode, BOOL bLevelDown )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	//레벨다운이 아니면, 스킬을 잊는거다.
	if( bLevelDown == FALSE )
	{
		m_ActionData.SetLogType( ACT_SKILL_UNLEARN );
		m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
		m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
		m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	}
	else
	{
		m_ActionData.SetLogType( ACT_SKILLLV_DOWN );
		m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
		m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
		m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
	}

	ROOT_SKILLINFO *pRootInfo = SkillInfoParser::Instance()->GetInfo( SkillCode );
	if( !pRootInfo )			return;

	if( pRootInfo->IsSkill() )
	{
		SkillScriptInfo *pBaseSkillInfo = (SkillScriptInfo*)pRootInfo;

		/*
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "SkillPt:%d,Skill:%u,ReqSKillPt:%u", 
			pPlayer->GetRemainSkillPoint(), SkillCode, pBaseSkillInfo->m_byRequireSkillPoint );
			*/
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "SklPt:%d,Skill:%u,ReqSklPt:%u", 
			pPlayer->GetRemainSkillPoint(), SkillCode, pBaseSkillInfo->m_byRequireSkillPoint );
	}
	else
	{
		BASE_STYLEINFO *pBaseStyleInfo = (BASE_STYLEINFO*)pRootInfo;
		/*
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "SkillPt:%d,Skill:%u,ReqSKillPt:%u", 
			pPlayer->GetRemainSkillPoint(), SkillCode, pBaseStyleInfo->m_byRequireSkillPoint );
			*/
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "SklPt:%d,Skill:%u,ReqSklPt:%u", 
			pPlayer->GetRemainSkillPoint(), SkillCode, pBaseStyleInfo->m_byRequireSkillPoint );
	}

	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_ActionData.AddAddInfo( m_pTmpBuffer );


	m_LogMng.Serialize( m_ActionData );
}

//체인지업
void GameLog::CharacterChangeUpLog( Player* pPlayer, BYTE byClassType, BYTE byChangeUpCode )
{
	if( !IsRun() || !pPlayer )		return;

	m_ActionData.Init();

	m_ActionData.SetLogType( ACT_CHARACTER_CHANGE_UP );
	m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_ActionData, pPlayer );

	m_LogMng.Serialize( m_ActionData );
}

// 4. 캐릭터 접속
void GameLog::WriteCharConnect( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_SessionData.Init();

	m_SessionData.SetLogType( SESSION_CHAR_CONNECT );
	m_SessionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_SessionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_SessionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	m_LogMng.Serialize( m_SessionData );
}

//
////로그인/아웃시 인벤토리 정보(스냅샷 로그로 대체)
//void GameLog::LogInvenInfo( int iLogType, Player * pPlayer )
//{
//	if( !IsRun() || !pPlayer )		return;
//
//	m_InventoryData.Init();
//
//
//	m_InventoryData.SetBaseData( iLogType, pPlayer->GetUserID(), pPlayer->GetCharName(), pPlayer->GetUserIP() );
//
//	m_InventoryData.SetMoney( pPlayer->GetMoney() );
//
//	/************************************************************************/
//	/* 인벤토리		                                                        */
//	/************************************************************************/
//	// 인벤토리 정보
//	//Inventory
//	if( m_InventoryData.GetInventorySize() == MAX_INVENTORYITEMSTREAM_SIZE )
//	{
//		ITEMSTREAMEX* pItemStrem = (ITEMSTREAMEX*)m_InventoryData.GetInventoryPtr();
//
//		SCItemSlotContainer* pItemContainer = (SCItemSlotContainer*)pPlayer->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
//		if( !pItemContainer )	return;
//
//		pItemContainer->SerializeItemStreamAll( pItemStrem, MAX_INVENTORYITEM_SIZE, SERIALIZE_LOAD );
//	}
//
//	if( m_InventoryData.GetTempInventorySize() == MAX_TEMPINVENTORYITEMSTREAM_SIZE )
//	{
//		// 템프인벤토리
//		ITEMSTREAMEX* pTempItemStrem = (ITEMSTREAMEX*)m_InventoryData.GetTempInventoryPtr();
//
//		SCItemSlotContainer* pTempItemContainer = (SCItemSlotContainer*)pPlayer->GetItemManager()->GetItemSlotContainer( SI_TEMPINVENTORY );
//		if( !pTempItemContainer )	return;
//
//		pTempItemContainer->SerializeItemStreamAll( pTempItemStrem, MAX_TEMPINVENTORYITEM_SIZE, SERIALIZE_LOAD );
//	}
//
//
//	m_LogMng.Serialize( m_InventoryData );
//}

// 스냅샷 로그
void GameLog::LogSnapShot( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	// 일정 주기로 로그를 남기기 위해
	pPlayer->GetSnapShotLogTimer().Reset( SNAPSHOT_LOG_CYCLE );
		
	m_SnapShotData.Init();

	// 기본정보
	m_SnapShotData.SetLogType( SNAPSHOT_REGULAR );
	m_SnapShotData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_SnapShotData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_SnapShotData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	int nSize = 0;
	/************************************************************************/
	/* 캐릭터 정보                                                          */
	/************************************************************************/
	BASE_PLAYERINFO* pCharInfo = NULL;
	pCharInfo = pPlayer->GetCharInfo();
	nSize = sizeof(BASE_PLAYERINFO);
	//ASSERT( nSize <= MAX_CHAR_STAT_SIZE );
//	if( nSize <= MAX_CHAR_STAT_SIZE )

	m_SnapShotData.SetCharStat( (BYTE*)pCharInfo, pCharInfo->GetLogSize() );

	/************************************************************************/
	/* 미션정보                                                             */
	/************************************************************************/
	if( m_SnapShotData.GetMissionSize() == MAX_MISSIONSTREAM_SIZE )
	{
		pPlayer->GetMissionManager()->SerializeStream( m_SnapShotData.GetMissionPtr(), m_SnapShotData.GetMissionSize(), SERIALIZE_LOAD );
	}
	else
	{
		ASSERT(FALSE);
	}

//#ifdef _DEV_TAIYO_ADD_SNAPSHOT_AC
	/************************************************************************/
	/* 퀘스트                                                               */
	/************************************************************************/
	// 퀘스트는 추후 추가..
	if( m_SnapShotData.GetProgrQuestSize() == MAX_PROGR_QUESTSTREAM_SIZE )
	{
		pPlayer->GetQuestManager()->SerializeProgrStream( m_SnapShotData.GetProgrQuestPtr(), m_SnapShotData.GetProgrQuestSize(), QSERIALIZE_LOAD );
		
	}
	else
	{
		ASSERT(FALSE);
	}

	if( m_SnapShotData.GetComplQuestSize() == MAX_COMPL_QUESTSTREAM_SIZE )
	{
		pPlayer->GetQuestManager()->SerializeComplStream( m_SnapShotData.GetComplQuestPtr(), m_SnapShotData.GetComplQuestSize(), QSERIALIZE_LOAD );
	}
	else
	{
		ASSERT(FALSE);
	}


	/************************************************************************/
	/* AC스트림																*/
	/************************************************************************/
	if( m_SnapShotData.GetACSize() == MAX_ACSTREAM_SIZE )
	{
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
		//새로운 업적에는 스넵샷에서 AC 정보를 뺀다. (용량이 너무 커서)
		//pPlayer->GetACManager()->Serialize( m_SnapShotData.GetACPtr(), SERIALIZE_LOAD );
#else
		pPlayer->GetACManager()->Serialize( m_SnapShotData.GetACPtr(), m_SnapShotData.GetACSize(), ACSERIALIZE_LOAD );
#endif
	}
	else
	{
		ASSERT(FALSE);
	}
//#endif

	/************************************************************************/
	/* 장비                                                                 */
	/************************************************************************/
	//Inventory
	if( m_SnapShotData.GetEquipItemSize() >= MAX_EQUIPITEMSTREAM_SIZE )
	{
		ITEMSTREAMEX* pEquipItemStrem = (ITEMSTREAMEX*)m_SnapShotData.GetEquipItemPtr();

		SCItemSlotContainer* pEquipContainer = (SCItemSlotContainer*)pPlayer->GetItemManager()->GetItemSlotContainer( SI_EQUIPMENT );
		if( !pEquipItemStrem )	return;

		pEquipContainer->SerializeItemStreamAll( pEquipItemStrem, m_SnapShotData.GetEquipItemSize(), SERIALIZE_LOAD );
	}
	else
	{
		ASSERT(FALSE);
	}

	/************************************************************************/
	/* 스킬                                                                 */
	/************************************************************************/
	if( m_SnapShotData.GetSkillSize() == MAX_SKILLSTREAM_SIZE )
	{
		SKILLSTREAM* pSkillStrem = (SKILLSTREAM*)m_SnapShotData.GetSkillPtr();

		SCSkillSlotContainer* pSkillContainer = (SCSkillSlotContainer*)pPlayer->GetSkillSlotManager()->GetSkillSlotContainer();
		if( !pSkillContainer )	return;

		pSkillContainer->SerializeSkillStreamAll( pSkillStrem, m_SnapShotData.GetSkillSize(), SERIALIZE_LOAD );
	}
	else
	{
		ASSERT(FALSE);
	}

	/************************************************************************/
	/* 인벤토리		                                                        */
	/************************************************************************/
	// 인벤토리 정보
	//Inventory
	if( m_SnapShotData.GetInventorySize() >= MAX_INVENTORYITEMSTREAM_SIZE )
	{
		ITEMSTREAMEX * pItemStrem = (ITEMSTREAMEX*)m_SnapShotData.GetInventoryPtr();

		SCItemSlotContainer* pItemContainer = (SCItemSlotContainer*)pPlayer->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
		if( !pItemContainer )	return;

		int Size = m_SnapShotData.GetInventorySize();
		pItemContainer->SerializeItemStreamAll( pItemStrem, m_SnapShotData.GetInventorySize(), SERIALIZE_LOAD );
	}
	else
	{
		ASSERT(FALSE);
	}

	if( m_SnapShotData.GetTempInventorySize() >= MAX_TEMPINVENTORYITEMSTREAM_SIZE )
	{
		// 템프인벤토리
		ITEMSTREAMEX* pTempItemStrem = (ITEMSTREAMEX*)m_SnapShotData.GetTempInventoryPtr();

		SCItemSlotContainer* pTempItemContainer = (SCItemSlotContainer*)pPlayer->GetItemManager()->GetItemSlotContainer( SI_TEMPINVENTORY );
        //youngmoon 템프인벤토리가 없어졌으므로 NULL 이어도 스넵샷은 찍어야 한다.
		if( pTempItemContainer )
        {
            pTempItemContainer->SerializeItemStreamAll( pTempItemStrem, m_SnapShotData.GetTempInventorySize(), SERIALIZE_LOAD );
        }		
	}
	else
	{
		ASSERT(FALSE);
	}

	m_LogMng.Serialize( m_SnapShotData );
}

// 정기적으로 게임 로그를 남긴다.
// 여기서 실행되지 않으면, 로그데이터를 이 클래스로 넘겨도, 기록되지 않는다.
void GameLog::StartRegularLog( int nLogType )
{
	if( nLogType == REGULAR_SKILL )  //스킬로그를 시작시킨다면..
	{
		// 여기서 쓰레드를 실행시키고,
		// 쓰레드 내에서 Timer를 실행시킨다.
		/*unsigned int nThreadID = 1;
		HANDLE hSkillThread = (HANDLE)_beginthreadex( NULL, 0, SkillWorkThread, NULL, 0, &nThreadID );*/
	}
}

/*
//아이템 기본정보를 얻는다.
VOID GameLog::SetItemInfo( ItemData & rData, const SCItemSlot & rItemSlot )  
{
	rData.SetItemCode( rItemSlot.GetItemCode() );
	rData.SetItemSerial( rItemSlot.GetSerial() );
	rData.SetItemNum( rItemSlot.GetNum() );
}
*/

//플레이어의 위치정보(맵코드, 위치)를 얻는다.
VOID GameLog::SetPlayerPosInfo( ItemData & rData, Player* pPlayer)
{
	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		rData.SetMapCode( (WORD)pZone->GetMapCode() );
		rData.SetMapX( (short)vecPos.x );
		rData.SetMapY( (short)vecPos.y );
		rData.SetMapZ( (short)vecPos.z );
	}
}

//플레이어의 위치정보(맵코드, 위치)를 저장한다.
VOID	GameLog::SetPlayerPosInfo( ActionData & rData, Player* pPlayer )
{
	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		rData.SetMapCode( pZone->GetMapCode() );
		rData.SetMapX( (short)vecPos.x );
		rData.SetMapY( (short)vecPos.y );
		rData.SetMapZ( (short)vecPos.z );
	}
}

//플레이어의 위치정보(맵코드, 위치)를 저장한다.
VOID	GameLog::SetPlayerPosInfo( AdminData & rData, Player * pPlayer )
{
	GameZone *pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );

		rData.SetMapCode( pZone->GetMapCode() );
		rData.SetMapX( (short)vecPos.x );
		rData.SetMapY( (short)vecPos.y );
		rData.SetMapZ( (short)vecPos.z );
	}
}

//플레이어의 위치정보를 AddInfo에 저장한다.
VOID	GameLog::SetPlayerPosAddInfo( BaseLogData & rLogData, Player* pPlayer )
{
	GameZone * pZone = pPlayer->GetGameZonePtr();
	if( pZone )
	{
		WzVector vecPos;
		pPlayer->GetPos( &vecPos );
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Map:%u,X:%0.2f,Y:%0.2f", pZone->GetMapCode(), vecPos.x, vecPos.y );
	}
	else
	{
		m_pTmpBuffer[0]=',';
		m_pTmpBuffer[1]='\0';
	}
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	rLogData.AddAddInfo( m_pTmpBuffer );
}

/*
//pPlayer : 상대방 플레이어 정보
void GameLog::SetOtherPlayerInfo( ItemData & rData, Player* pPlayer )
{

	rData.SetToAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	rData.SetToCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
}
*/

VOID GameLog::SetRoomKey( BaseLogData & rLogData, Player* pPlayer )
{
	//방번호
	GameZone * pGameZone = pPlayer->GetGameZonePtr();
	if( pGameZone )
	{
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",MapCode:%d,Room:%u", pGameZone->GetMapCode(), 
                    pGameZone->GetKey() );
	}
	else
	{
		m_pTmpBuffer[0] = ',';
		m_pTmpBuffer[1] = '\0';
	}

	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	rLogData.AddAddInfo( m_pTmpBuffer );
}

/*
VOID	GameLog::SetBaseItemPriceInfo( BaseLogData & rLogData, Player * pPlayer, const SCItemSlot & rItemSlot, BOOL bSell )
{
	DWORD dwPrice = 0;
	if( bSell )				dwPrice = (DWORD)rItemSlot.GetPriceForSell();
	else					dwPrice = (DWORD)rItemSlot.GetPriceForBuy();

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Price:%u,Money:%I64u", dwPrice, pPlayer->GetMoney() );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	rLogData.AddAddInfo( m_pTmpBuffer );

}


//개인거래 아이템의 가격정보
VOID	GameLog::SetTradeItemPriceInfo( BaseLogData & rLogData, const SCItemSlot & rItemSlot, DWORD dwPrice, BOOL bSell )
{
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Price:%u", dwPrice );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	rLogData.AddAddInfo( m_pTmpBuffer );

}

VOID	GameLog::SetItemSlotPos( BaseLogData & rLogData, const SCItemSlot & rItemSlot )
{
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Pos:%u", rItemSlot.GetPos() );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	rLogData.AddAddInfo( m_pTmpBuffer );

}

VOID	GameLog::SetFullItemAddInfo( BaseLogData & rLogData, const SCItemSlot & rItemSlot )				// 아이템의 모든 정보를 AddInfo에 넣는다.
{
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Code:%u,Serial:%u,Num:%u", 
												rItemSlot.GetItemCode(), rItemSlot.GetSerial(), rItemSlot.GetNum() );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	rLogData.AddAddInfo( m_pTmpBuffer );

	SetBaseItemAddInfo( rLogData, rItemSlot );

}


VOID	GameLog::SetBaseItemAddInfo( BaseLogData & rLogData, const SCItemSlot & rItemSlot )
{
	eRANK rank = rItemSlot.GetRank();

	if( rank > RANK_E )
	{
		// 현재랭크,랭크옵션인덱스,랭크옵션값
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Rank:%u,AttrIdx:%u,AttrValue:%u", 
												rank, rItemSlot.GetRankAttrIndex(rank), rItemSlot.GetRankAttrValue(rank) );
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		rLogData.AddAddInfo( m_pTmpBuffer );
	}
	else
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Rank:%u,,", rank );
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		rLogData.AddAddInfo( m_pTmpBuffer );
	}

	if(rItemSlot.WithDura())
	{
		_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Dura:%u", (BYTE)rItemSlot.GetDura() );
		m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
		rLogData.AddAddInfo( m_pTmpBuffer );
	}
	else
	{
		m_pTmpBuffer[0]=',';
		m_pTmpBuffer[1]='\0';
		rLogData.AddAddInfo( m_pTmpBuffer );

	}
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",SockNum:%u", rItemSlot.GetSocketNum() );	//소켓의 개수
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	rLogData.AddAddInfo( m_pTmpBuffer );

	//소켓 옵션은 최대 3개이다. 
	for( int i = 0; i < SOCKET_MAX ; i++ )
	{
		if( 0 != rItemSlot.GetSocketAttrIndex((eSOCKET)i) )
		{
			_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",AttrIdx:%d,AttrValue:%d",
				rItemSlot.GetSocketAttrIndex((eSOCKET)i), rItemSlot.GetSocketAttrValue((eSOCKET)i) );	//< 소켓의 속성(불, 물.... )
			m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
			rLogData.AddAddInfo( m_pTmpBuffer );
		}
		else
		{
			m_pTmpBuffer[0]=',';
			m_pTmpBuffer[1]=',';
			m_pTmpBuffer[2]='\0';
			rLogData.AddAddInfo( m_pTmpBuffer );
		}
	}
}

// 아이템의 상점에서 살때, 팔때 가격를 설정한다.
VOID	GameLog::SetItemPrice( BaseLogData & rLogData, const SCItemSlot & rItemSlot, BOOL bSell )
{
	MONEY Price = 0;
	if( bSell )
		Price = rItemSlot.GetPriceForSell();
	else
		Price = rItemSlot.GetPriceForBuy();

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Price:%u", Price );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	rLogData.AddAddInfo( m_pTmpBuffer );
}
*/

// 플레이어의 기본정보를 AddInfo에 넣는다.
VOID	GameLog::SetBasePlayerAddInfo( BaseLogData & rLogData, Player * pPlayer )
{
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",UsrID:%s,ChrName:%s", pPlayer->GetUserID(), pPlayer->GetCharName() );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	rLogData.AddAddInfo( m_pTmpBuffer );
}

//
////Thread Argument
//unsigned int __stdcall SkillWorkThread(void* pArg)
//{
//	GameLog* pLog = (GameLog*)pArg;
//
//	while(TRUE)
//	{
//
//	}
//}

// 입장
void	GameLog::WriteEntrance( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_ENTRANCE );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	SetPlayerPosInfo( m_AdminData, pPlayer );

	SetRoomKey( m_AdminData, pPlayer );

	m_LogMng.Serialize( m_AdminData );
}

// 투명
void	GameLog::WriteTransparent( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_TRANSPARENT );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );

	m_LogMng.Serialize( m_AdminData );
}

// 무적
void	GameLog::WriteInvincibility( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_INVINCIBILITY );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );

	m_LogMng.Serialize( m_AdminData );

}


// 방정보
void	GameLog::WriteRoomInfo( Player* pPlayer, DWORD dwRoomKey )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_ROOMINFO );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

	SetRoomKey( m_AdminData, pPlayer );

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Room:%u", dwRoomKey ); //< 찾는 방번호
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_AdminData.AddAddInfo( m_pTmpBuffer );


	m_LogMng.Serialize( m_AdminData );

}

// 부활
void	GameLog::WriteRebirth( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_REBIRTH );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );


	m_LogMng.Serialize( m_AdminData );

}

// 아이템제거
void	GameLog::WriteRemovalItem( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_REMOVALITEM );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );


	m_LogMng.Serialize( m_AdminData );

}

// 몬스터제거
void	GameLog::WriteRemovalMonster( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_REMOVALMONSTER );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );


	m_LogMng.Serialize( m_AdminData );

}

// 회복
void	GameLog::WriteRecovery( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_RECOVERY );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );


	m_LogMng.Serialize( m_AdminData );

}

// 레벨업
void	GameLog::WriteLevelUp( Player* pPlayer, WORD wOldLevel )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_LEVELUP );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",LV:%u,OldLV:%u", pPlayer->GetLevel(), wOldLevel ); //< 현재레벨,이전레벨
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_AdminData.AddAddInfo( m_pTmpBuffer );


	m_LogMng.Serialize( m_AdminData );

}

// 스텟업
void	GameLog::WriteStatPointUp( Player* pPlayer, WORD wPlusPoint )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_STATPOINTUP );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",PlusPt:%u,StatPt:%d", wPlusPoint, pPlayer->GetStatPoint() );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_AdminData.AddAddInfo( m_pTmpBuffer );


	m_LogMng.Serialize( m_AdminData );

}

// 스킬포인트업	
void	GameLog::WriteSkillPointUp( Player* pPlayer, WORD wPlusPoint )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_SKILLPOINTUP );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",PlusPt:%u,SkillRemainPt:%d", wPlusPoint, pPlayer->GetRemainSkillPoint() );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_AdminData.AddAddInfo( m_pTmpBuffer );


	m_LogMng.Serialize( m_AdminData );

}

// 몬스터소환
void	GameLog::WriteCreateMonster( Player* pPlayer, DWORD dwMonsterCode, WORD wMonsterCount  )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_CREATEMONSTER );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );

//	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",MonCode:%u,MonNum%u", dwMonsterCode, wMonsterCount );
	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",MonCd:%u,MonNum%u", dwMonsterCode, wMonsterCount );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_AdminData.AddAddInfo( m_pTmpBuffer );


	m_LogMng.Serialize( m_AdminData );
}

// 제거
void	GameLog::WriteRemoval( Player* pPlayer, const TCHAR* pszMonsterName )		// 제거
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_REMOVAL );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

	m_AdminData.SetToCharName( pszMonsterName, MAX_MONSTERNAME_LENGTH );

//	SetRoomKey( m_AdminData, pPlayer );
	
	m_LogMng.Serialize( m_AdminData );
}

// 생성
void	GameLog::WriteCreateItem( Player* pPlayer, WORD wItemCode, WORD wItemCount )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_CREATEITEM );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );

	_sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",Item:%u,Num:%u", wItemCode, wItemCount );
	m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
	m_AdminData.AddAddInfo( m_pTmpBuffer );


	m_LogMng.Serialize( m_AdminData );

}

// 하임
void	GameLog::WriteCreateHeim( Player* pPlayer, MONEY amount )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_CREATEHEIM );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Money : %I64u", amount );
    m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
    m_AdminData.AddAddInfo( m_pTmpBuffer );

	m_LogMng.Serialize( m_AdminData );

}

// 킬
void	GameLog::WriteKill( Player* pPlayer, const TCHAR* pszMonsterName )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_KILL );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

	m_AdminData.SetToCharName( pszMonsterName, MAX_MONSTERNAME_LENGTH );

//	SetRoomKey( m_AdminData, pPlayer );


	m_LogMng.Serialize( m_AdminData );
}

//속도
void	GameLog::WriteSpeed( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_SPEED );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

//	SetRoomKey( m_AdminData, pPlayer );


	m_LogMng.Serialize( m_AdminData );
}


//어카운트
void	GameLog::WriteAccount( Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( GM_KILL );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

	m_LogMng.Serialize( m_AdminData );

}
void GameLog::LogGM( int nLogType, Player* pPlayer )
{
	if( !IsRun() || !pPlayer )		return;

	m_AdminData.Init();

	m_AdminData.SetLogType( nLogType );
	m_AdminData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
	m_AdminData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
	m_AdminData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );

	SetPlayerPosInfo( m_AdminData, pPlayer );

	m_LogMng.Serialize( m_AdminData );
}


void GameLog::	PreProcess()
{
	BOOST_STATIC_ASSERT( MAX_ITEMPARTEX_SIZE				== sizeof(ITEMSTREAMEX) );
	BOOST_STATIC_ASSERT( MAX_BASE_PLAYER_INFO_SIZE			== sizeof(BASE_PLAYERINFO_LOG) );
}

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
void GameLog::SetAchievementInfo( WORD location, WORD index_number )
{
    m_ACInfoData.location = location;
    m_ACInfoData.index_number = index_number;
}
#endif //C_NA_0_20100520_ACSYSTEM_CHANGED

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
void GameLog::LogPointWallet( Player* pPlayer, eLOG_ACTION action_type, DWORD point_code, \
                              DWORD previous_point, DWORD point_diffrence, DWORD current_point )
{
    if( !IsRun() || !pPlayer )
        return;

    m_ActionData.Init();
    m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
    m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
    m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );
    m_ActionData.SetLogType( action_type );
    SetPlayerPosInfo( m_ActionData, pPlayer );

    memset(m_pTmpBuffer, 0, sizeof(m_pTmpBuffer));
    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, 
        "MoneyCode:%d,Before:%d,Use:%d,Total:%d", \
        point_code, previous_point, point_diffrence, current_point);

    m_ActionData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize( m_ActionData );
}
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
void GameLog::LogChaosZone(eLOG_ACTION log_type, Player* player, 
                           const TCHAR* target_character_name, const TCHAR * add_info)
{
    if( !IsRun() || !player )
        return;

    m_ActionData.Init();
    m_ActionData.SetLogType(log_type);
    m_ActionData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ActionData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ActionData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);

    SetPlayerPosInfo(m_ActionData, player);
    SetRoomKey(m_ActionData, player);

    if (target_character_name != NULL)
    {
        m_ActionData.SetCharName(target_character_name, MAX_CHARNAME_LENGTH);
    }    

    if (add_info != NULL) 
    {
        m_ActionData.AddAddInfo((TCHAR*)add_info);
    }

    m_LogMng.Serialize(m_ActionData);
}

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN
void GameLog::LogGuild( eLOG_ACTION log_type, Player* player, const WORD donated_point, const TCHAR* add_info /*= NULL*/ )
{
    if( !IsRun() || !player )
        return;

    m_ActionData.Init();
    m_ActionData.SetLogType(log_type);
    m_ActionData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ActionData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ActionData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);
    SetPlayerPosInfo(m_ActionData, player);

    memset(m_pTmpBuffer, 0, sizeof(m_pTmpBuffer));
    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, 
        "DonatedPoint: %d", donated_point);

    m_ActionData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize( m_ActionData );
}

#endif //_NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN

#ifdef _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
// 골드러시를 위한 DB로그
// changed_point, total_point -- 골드포인트관련 로그를 남길 경우 사용
// monster_code -- 몬스터킬 로그를 남길 경우 사용
void GameLog::LogGoldRush( eLOG_ACTION log_type, Player* player, WORD changed_point, WORD total_point, 
                           MONSTERCODE monster_code /*= 0*/, const TCHAR* add_info /* = NULL */)
{
    if( !IsRun() || !player )
        return;

    m_ActionData.Init();
    m_ActionData.SetLogType(log_type);
    m_ActionData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ActionData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ActionData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);

    SetPlayerPosInfo(m_ActionData, player);

    GameZone * game_zone = player->GetGameZonePtr();
    ZONEKEY room_key = 0;
    if (game_zone) {
        room_key = game_zone->GetKey();
    }

    memset(m_pTmpBuffer, 0, sizeof(m_pTmpBuffer));
    switch (log_type)
    {
    case ACT_GOLDRUSH_MONSTERKILL:
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",MonsterCode:%d Room:%u", 
            monster_code ,room_key );
        break;
    case ACT_GOLDRUSH_GOLDPOINT_GET:
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",ChangedPoint: %d, TotalPoint: %d, Room:%u", 
            changed_point, total_point, room_key );
        break;
    case ACT_GOLDRUSH_GOLDPOINT_LOSE: //골드포인트 손실
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",ChangedPoint: %d, TotalPoint: %d, Room:%u", 
            changed_point * -1, total_point, room_key );
        break;
    case ACT_GOLDRUSH_BATTLERESULT:
        if (add_info != NULL) {
            _tcscpy(m_pTmpBuffer, add_info);
        }
        break;
    }


    m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
    m_ActionData.AddAddInfo((TCHAR*)m_pTmpBuffer);

    m_LogMng.Serialize(m_ActionData);
}
#endif //_JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
void GameLog::LogGuildFacility( eLOG_ACTION log_type, Player* player, GameGuild* const guild, FCODE code, FLEVEL level )
{
    if( !IsRun() || !player )
        return;

    m_ActionData.Init();
    m_ActionData.SetLogType(log_type);
    m_ActionData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ActionData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ActionData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);
    SetPlayerPosInfo(m_ActionData, player);

    if (guild != NULL)
    {
        memset(m_pTmpBuffer, 0, sizeof(m_pTmpBuffer));
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Guild Guid : %d, Guild Name : %s", 
            guild->GetGuildGuid(), guild->GetGuildName());
        m_ActionData.AddAddInfo(m_pTmpBuffer);
    }

    memset(m_pTmpBuffer, 0, sizeof(m_pTmpBuffer));
    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Facility Code : %d, Facility Level : %d", 
        code, level);
    m_ActionData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize( m_ActionData );
}

#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_007086_20140318_MONSTERBOOK
void GameLog::LogMonsterBook( eLOG_ACTION log_type, Player* player, MONSTERCODE monster_code )
{
    if( !IsRun() || !player )
        return;

    m_ActionData.Init();
    m_ActionData.SetLogType(log_type);
    m_ActionData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
    m_ActionData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
    m_ActionData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);
    SetPlayerPosInfo(m_ActionData, player);

    memset(m_pTmpBuffer, 0, sizeof(m_pTmpBuffer));
    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, 
        "MonsterCode: %d", monster_code);

    m_ActionData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize( m_ActionData );
}
#endif //_NA_007086_20140318_MONSTERBOOK

#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
void GameLog::LogWzDat( eLOG_ACTION log_type, const TCHAR* add_info /*= NULL*/ )
{
    if( !IsRun())
    {
        return;
    }

    m_ActionData.Init();
    m_ActionData.SetLogType(log_type);
    m_ActionData.SetAccountID("WzDat", ID_MAX_LEN);
    m_ActionData.SetCharName("WzDat", MAX_CHARNAME_LENGTH);

    if (add_info != NULL) 
    {
        m_ActionData.AddAddInfo((TCHAR*)add_info);
    }

    m_LogMng.Serialize(m_ActionData);
}

#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG

#ifdef _NA_008119_20150310_CHECK_ONESHOT_ONEKILL_SITUATION
void GameLog::LogAction_PK( int iLogType, Player * pPlayer, Player* pOtherPlayer/*=NULL*/, 
                            DAMAGETYPE _damage_value /*= 0*/)
{
    if( !IsRun() || !pPlayer )		return;

    m_ActionData.Init();

    m_ActionData.SetLogType( iLogType );
    m_ActionData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
    m_ActionData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
    m_ActionData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

    SetPlayerPosInfo( m_ActionData, pPlayer );

    if( pOtherPlayer ) {
        SetBasePlayerAddInfo( m_ActionData, pOtherPlayer );
    }

    // 처치 시의 데미지 기록
    if (_damage_value != 0)
    {
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ", LastHitDmg: %d", _damage_value);
        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
        m_ActionData.AddAddInfo(m_pTmpBuffer);
    }

    if (iLogType == ACT_PK_KILL)
    {
        time_t cur_time;	util::TimeSync::time( &cur_time );
        DWORD dwLeavedTime = max( 0, pPlayer->GetChaoState().GetChaoEndTime() - cur_time );

        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ",%I64u", dwLeavedTime);
        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
        m_ActionData.AddAddInfo(m_pTmpBuffer);
    }

    // PK 킬 로그면 카오상태 표시
    if( pOtherPlayer != NULL && (iLogType == ACT_PK_KILL || iLogType == ACT_PK_DIE) )
    {
        _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, ", CaoStat:%u/%u,OtherCaoStat:%u/%u", 
            pPlayer->GetChaoState().GetChaoState(), pPlayer->GetChaoState().IsBoradori(),
            pOtherPlayer->GetChaoState().GetChaoState(), pOtherPlayer->GetChaoState().IsBoradori() );
        m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
        m_ActionData.AddAddInfo(m_pTmpBuffer);
    }
    else if (pOtherPlayer != NULL && (iLogType == ACT_DOMINATION_KILL ||iLogType == ACT_DOMINATION_DIE))
    {
        GameZone* pZone = pPlayer->GetGameZonePtr();
        const GUILDGUID guildGuid = pOtherPlayer->GetGuildGuid();
        GameGuild* const pGuild = g_GameGuildManager.FindGuild(guildGuid);

        if (NULL != pZone) 
        {
            _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, ".");
            m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';
            m_ActionData.AddAddInfo(m_pTmpBuffer);

        }
        if (NULL != pGuild)
        {
            const TCHAR* guildName = pGuild->GetGuildName();

            _sntprintf(m_pTmpBuffer, MAX_ADDINFO_SIZE, ", GuildName:%s", guildName);
            m_pTmpBuffer[MAX_ADDINFO_SIZE] = '\0';

            m_ActionData.AddAddInfo(m_pTmpBuffer);
        }
    }

    m_LogMng.Serialize( m_ActionData );
}
#endif //_NA_008119_20150310_CHECK_ONESHOT_ONEKILL_SITUATION

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
void GameLog::LogAwakening( int iLogType, Player * pPlayer, SCItemSlot& equip_item, 
                            const CODETYPE material_item_code, const BYTE material_awakening, 
                            const BYTE result_awakening, const BYTE prev_awakening,
                            const CODETYPE additive_item_code )
{
    if( !IsRun() || !pPlayer  )		return;

    if( ITEM_AWAKENING_SUCCESS      != iLogType &&
        ITEM_AWAKENING_FAILED       != iLogType
        )
    {
        return;
    }

    m_ItemData.Init();
    m_ItemData.SetLogType( iLogType );
    m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
    m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
    m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	
    m_ItemData.SetItemCode( equip_item.GetItemCode() );
    m_ItemData.SetItemSerial( equip_item.GetWorldSerial() );
    m_ItemData.SetItemNum( equip_item.GetNum() );

    ITEMSTREAMEX ItemStreamEx;
    equip_item.CopyOut(ItemStreamEx);
    m_ItemData.SetItemStreamEx1( (const BYTE*)&ItemStreamEx, sizeof(ItemStreamEx) );

    int need_additive_number = 0;

    const AwakeningInfo* awakening_info = 
        AwakeningInfoParser::Instance()->FindData(equip_item.GetCode());
    if ( awakening_info != NULL )
    {
        need_additive_number = result_awakening - prev_awakening;
        need_additive_number *= awakening_info->additive_count;

    }

    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE,
        "Material:%d,ResultAwakening:%d,PrevAwakening:%d,Additive:%d,NeedAdditive:%d", 
        material_item_code, result_awakening, prev_awakening, additive_item_code, need_additive_number);

    m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
    m_ItemData.AddAddInfo( m_pTmpBuffer );

    m_LogMng.Serialize( m_ItemData );
}

void GameLog::LogEvolution( int iLogType, Player * pPlayer, const CODETYPE result_item_code, 
                           const CODETYPE prev_item_code,const CODETYPE additive_item_code )
{
    if( !IsRun() || !pPlayer  )		return;

    if( ITEM_EVOLUTION_SUCCESS      != iLogType &&
        ITEM_EVOLUTION_FAILED       != iLogType
        )
    {
        return;
    }

    m_ItemData.Init();
    m_ItemData.SetLogType( iLogType );
    m_ItemData.SetAccountID( pPlayer->GetUserID(), ID_MAX_LEN );
    m_ItemData.SetCharName( pPlayer->GetCharName(), MAX_CHARNAME_LENGTH );
    m_ItemData.SetUserIP( pPlayer->GetUserIP(), MAX_IPADDRESS_SIZE );	

    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE,
        "ResultItemCode:%d,PrevItemCode:%d,Additive:%d", 
        result_item_code, prev_item_code, additive_item_code);

    m_pTmpBuffer[MAX_ADDINFO_SIZE]='\0';
    m_ItemData.AddAddInfo( m_pTmpBuffer );

    m_LogMng.Serialize( m_ItemData );
}

#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
void GameLog::LogMaxDamage( eLOG_ACTION log_type, Character* attacked_character, Character* attacker, DamageArgs* damage_arg )
{
    if( !IsRun() || !attacked_character || !attacker || !damage_arg  )		return;

    m_ActionData.Init();
    m_ActionData.SetLogType(log_type);

    if (attacker->IsEqualObjectKind(PLAYER_OBJECT))
    {
        Player* attacker_player = static_cast<Player*>(attacker);
        if (attacker_player != NULL)
        {
            m_ActionData.SetAccountID(attacker_player->GetUserID(), ID_MAX_LEN);
            m_ActionData.SetCharName(attacker_player->GetCharName(), MAX_CHARNAME_LENGTH);
            m_ActionData.SetUserIP(attacker_player->GetUserIP(), MAX_IPADDRESS_SIZE);
            SetPlayerPosInfo(m_ActionData, attacker_player);
        }
    }

    int npc_code = 0;
    if (attacked_character->IsEqualObjectKind(NPC_OBJECT))
    {
        NPC* npc = static_cast<NPC*>(attacked_character);
        if (npc != NULL)
        {
            const BASE_NPCINFO* pInfo = npc->GetBaseInfo();
            if (pInfo != NULL)
            {
                npc_code = pInfo->m_MonsterCode;
            }
        }
    }

    MAPCODE map_code = 0;
    GameZone *pZone = attacked_character->GetGameZonePtr();
    if (pZone != NULL)
    {
        map_code = pZone->GetMapCode();
    }

    memset(m_pTmpBuffer, 0, sizeof(m_pTmpBuffer));
    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "NpcCode:%d, MapCode:%d, Damage:%d, SkillCode:%d, AttackType:%d", 
        npc_code, map_code, damage_arg->damage_, damage_arg->skill_code_, damage_arg->attack_type_);
    m_ActionData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize( m_ActionData );
}

#endif // _NA_008299_20150519_MAX_DAMAGE_LOG

#ifdef _NA_008334_20150608_SONNENSCHEIN
VOID GameLog::LogSonnenSchein( eLOG_ACTION log_type, GUILDGUID guild_guid, Player* player )
{
    if( !IsRun() ) {
        return;
    }

    m_ActionData.Init();
    m_ActionData.SetLogType(log_type);

    CHARGUID stamper_guid = 0;
    if (player != NULL)
    {
        m_ActionData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
        m_ActionData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
        m_ActionData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);
        SetPlayerPosInfo(m_ActionData, player);

        CHARGUID stamper_guid = player->GetCharGuid();
    }

    memset(m_pTmpBuffer, 0, sizeof(m_pTmpBuffer));
    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "GuildGuid:%d, StamperGuid:%d", 
                guild_guid, stamper_guid);

    m_ActionData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize( m_ActionData );
}

#endif //_NA_008334_20150608_SONNENSCHEIN

//_NA_008506_20150923_LOG_LOGOUT_IN_GAMESERVER
VOID GameLog::LogLogout( Player* player )
{
    if( !IsRun() ) {
        return;
    }

    m_ActionData.Init();
    m_ActionData.SetLogType(SESSION_LOGOUT_GAMESERVER);

    MAPCODE map_code = 0;
    if (player != NULL)
    {
        m_ActionData.SetAccountID(player->GetUserID(), ID_MAX_LEN);
        m_ActionData.SetCharName(player->GetCharName(), MAX_CHARNAME_LENGTH);
        m_ActionData.SetUserIP(player->GetUserIP(), MAX_IPADDRESS_SIZE);
        SetPlayerPosInfo(m_ActionData, player);

        GameZone* zone = player->GetGameZonePtr();
        if (zone != NULL)
        {
            map_code = zone->GetMapCode();
        }
    }

    memset(m_pTmpBuffer, 0, sizeof(m_pTmpBuffer));
    _sntprintf( m_pTmpBuffer, MAX_ADDINFO_SIZE, "Last Location:%d", map_code);

    m_ActionData.AddAddInfo(m_pTmpBuffer);

    m_LogMng.Serialize( m_ActionData );
}