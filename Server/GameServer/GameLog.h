#pragma once
//=======================================================================================================================
/// GameLog
/**
	@history
			- 2006.5.15 : [taiyo] ���� ���� �� ������ ���ڿ� ó�� ����
*/
//=======================================================================================================================
#include <BaseLog.h>
#include <ItemData.h>
#include <ExpData.h>
#include <SnapShotData.h>
#include <SessionData.h>
#include <ActionData.h>
#include <MoneyData.h>
#include <InventoryData.h>
#include <WareHouseData.h>
#include <AdminData.h>
#include <AccumulatePointData.h>
#include <ItemComposeListParser.h>
#include <ItemCompositeParser.h>
#include <ShopInfoParser.h>

class NPC;
class Player;
class SCItemSlot;

class SystemMemo;

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
class GameGuild;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
struct DamageArgs;
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG

//��þƮ�� ��� ��������
struct COMPOSITEMATERIAL
{
	SLOTCODE	MaterialCode;
	POSTYPE		MaterialNum;
};

//�����۰ŷ��� ���Ǵ� ������ ����
struct ITEMLOGINFO
{
	int		nItemCode;
	int		nItemSerial;
	int		nItemCount;
};

enum eREGULAR_LOGTYPE
{	
	REGULAR_SKILL = 0,
};

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
struct ACHIEVEMENT_LOGINFO
{
	WORD location;
    WORD index_number;
};
#endif //C_NA_0_20100520_ACSYSTEM_CHANGED

//Thread Argument
//unsigned int __stdcall SkillWorkThread(void* pArg);


class GameLog : public BaseLog
{
	enum { MAX_EXP_FULL_LOG_AT_LEVEL = 50, };
	enum { MAX_EXP_DIVIDE_LOG_AT_LEVEL = 10, };
public:
	GameLog();
    virtual ~GameLog() {}

private:
	ItemData		m_ItemData;
	ExpData			m_ExpData;
	SnapShotData	m_SnapShotData;
	SessionData 	m_SessionData;
	ActionData		m_ActionData;
	InventoryData	m_InventoryData;
	WareHouseData	m_WareHouseData;
	AdminData		m_AdminData;
	MoneyData		m_MoneyData;
	TCHAR			m_pTmpBuffer[MAX_ADDINFO_SIZE+1];
    AccumulatePointData m_AccumulatePointData;
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    ACHIEVEMENT_LOGINFO m_ACInfoData;
#endif //C_NA_0_20100520_ACSYSTEM_CHANGED

private:
	VOID	SetItemInfo( ItemData & rData, const SCItemSlot & rItemSlot );		//������ �⺻������ �����Ѵ�.
	VOID	SetPlayerPosInfo( ItemData & rData, Player* pPlayer );		//�÷��̾��� ��ġ����(���ڵ�, ��ġ)�� �����Ѵ�.
	VOID	SetPlayerPosInfo( ActionData & rData, Player* pPlayer );		//�÷��̾��� ��ġ����(���ڵ�, ��ġ)�� �����Ѵ�.
	VOID	SetPlayerPosInfo( AdminData & rData, Player* pPlayer );		//�÷��̾��� ��ġ����(���ڵ�, ��ġ)�� �����Ѵ�.
	VOID	SetOtherPlayerInfo( ItemData & rData, Player* pPlayer );		//ItemData�� ���� ������ �����Ѵ�.

	VOID	SetRoomKey( BaseLogData & rLogData, Player* pPlayer );
	VOID	SetPlayerPosAddInfo( BaseLogData & rLogData, Player* pPlayer );						//�÷��̾��� ��ġ������ AddInfo�� �����Ѵ�.
	VOID	SetBaseItemPriceInfo( BaseLogData & rLogData, Player* pPlayer, const SCItemSlot & rItemSlot, BOOL bSell );		//���� �������� ��������
	VOID	SetTradeItemPriceInfo( BaseLogData & rLogData, const SCItemSlot & rItemSlot, DWORD dwPrice, BOOL bSell );		//���ΰŷ� �������� ��������
	VOID	SetItemSlotPos( BaseLogData & rLogData, const SCItemSlot & rItemSlot );					//������ ������ġ
	VOID	SetBaseItemAddInfo( BaseLogData & rLogData, const SCItemSlot & rItemSlot );
	VOID	SetFullItemAddInfo( BaseLogData & rLogData, const SCItemSlot & rItemSlot );				// �������� ��� ������ AddInfo�� �ִ´�.
	VOID	SetItemPrice( BaseLogData & rLogData, const SCItemSlot & rItemSlot, BOOL bSell );		// �������� �������� �춧, �ȶ� ���ݸ� �����Ѵ�.
	VOID	SetBasePlayerAddInfo( BaseLogData & rLogData, Player* pPlayer );						// �÷��̾��� �⺻������ AddInfo�� �ִ´�.

public:
	//////////////////////////////////////////////////////////////////////////
	// ����ġ �α� 
	// 1. ������ �α�
	void WriteLevelUp( Player* pPlayer, WORD wOldLevel, WORD wNewLevel );
    // NOTE: f110602.3L
    void WritePerkLevelUp(Player* player, int perk_group, LEVELTYPE old_level, LEVELTYPE new_level);
	// 2. ���� 50�̻�� ����ġ 10��� �α�
	void LogExp( Player* pPlayer, WORD wCurLevel, EXPTYPE OldExp, EXPTYPE NewExp, MAPCODE Mapcode = 0);
	// 3. ����ġ �ٿ�
	void WriteDownExp( Player* pPlayer, EXPTYPE NewExp, EXPTYPE OldExp );
	void LogExp( int iLogType, Player * pPlayer, EXPTYPE NewExp, EXPTYPE OldExp );
    void LogExp (int log_type, Player* player, EXPTYPE new_exp, EXPTYPE old_exp, const TCHAR* char_name);
    void LogExpPerk(Player* player, int perk_group, EXPTYPE new_exp, EXPTYPE old_exp);

	void LogTotalMoney( DWORD64 dw64Money, DWORD64 dw64ToMoney, WORD PlayerNum, DWORD64 dw64PlayerTotalMoney );
	void LogTotalWarehouseMoney( DWORD64 dw64Money, DWORD64 dw64ToMoney, WORD PlayerNum, DWORD64 dw64PlayerTotalMoney );
	void LogMoney( int iLogType, Player * pPlayer, DWORD64 dw64Money, DWORD64 dw64ToMoney=0,
					const TCHAR * ptszToUserID = NULL, const TCHAR * ptszToCharName = NULL );

    // implemented by _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE = {
    void LogMemoReward(eSYSTEM_MEMO_TYPE memo_type, Player* player, WORD reward_item_code, 
                       ULONGLONG amount_item, const TCHAR* memo_contents);
    void LogMemoRewardItem(int log_type, Player* player, WORD reward_item_code, 
                           WORD amount_item, const TCHAR* memo_contents);
    void LogMemoRewardMoney(int log_type, Player* player, ULONGLONG amount_item, 
                            const TCHAR* memo_contents);
    // }
	// ���� ������ �α�
	void LogItemWarehouse( int iLogType, Player * pPlayer, const SCItemSlot * pItemSlot, BYTE byItemNum, 
			SLOTIDX f_idx, POSTYPE f_pos, SLOTIDX t_idx, POSTYPE t_pos );

#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
    void LogItem(int log_type, Player* player, const SCItemSlot* item_slot, 
                 BYTE item_num, WORD price_of_goods, SHOPINFO* shop_info);
#endif

    void LogItem(int log_type, Player* player, uint use_count, const SCItemSlot* use_slot, const SCItemSlot* target_slot);

    void LogItem( int iLogType, Player * pPlayer, const SCItemSlot * pItemSlot, MONEY ullMoney = 0, BYTE ItemNum = 1, 
        const TCHAR * ptszToUserID = NULL, const TCHAR * ptszToCharName = NULL,
        const TCHAR * ptszAddInfo = NULL );
	void LogItemEx( int iLogType, Player * pPlayer, const SCItemSlot & rItemSlot1, const SCItemSlot & rItemSlot2, 
					MONEY ullMoney = 0, BYTE byItemNum = 1,
					const TCHAR * ptszToUserID = NULL, const TCHAR * ptszToCharName = NULL );

		void GameLog::LogItemEx2( int iLogType, Player * pPlayer, const SCItemSlot & rItemSlot1, 
			MONEY ullMoney, BYTE byItemNum,
			DBWLDSERIAL dwItem2WorldSerial, SLOTCODE Item2Code, BYTE Item2Num, 
			const TCHAR * ptszToUserID  = NULL, const TCHAR * ptszToCharName = NULL);	

	void LogItem_Pos( int iLogType, Player * pPlayer, const SCItemSlot * pItemSlot, MONEY ullMoney, BYTE byItemNum, 
					SLOTIDX f_idx, POSTYPE f_pos, SLOTIDX t_idx, POSTYPE t_pos );
	// AC������ �α�
	void LogACItem( int iLogType, Player * pPlayer, const SCItemSlot * pItemSlot, float fPercentage );

	// ��þƮ �α�
	void LogEnchant( int iLogType, Player * pPlayer, const SCItemSlot & rItemSlot, 
					MONEY ullMoney, STLX_VECTOR<COMPOSITEMATERIAL> & vecMatInfo, BYTE byDownLV = 0 );
	// ��ũ�� �� �α�
	//void LogRankUp_Before( Player* pPlayer, const SCItemSlot & rItemSlot1, const SCItemSlot & rItemSlot2 );
	void LogCrystalizeSuccess( Player * pPlayer, const SCItemSlot & rItemSlot, BYTE byTotalCount, SLOTCODE * IN pObtainItemCode, BYTE * IN pObtainItemNum );

	void GameLog::LogCompose( BOOL bSuccess, Player * pPlayer, SCItemSlot * pItemSlot1, BYTE byMatCount, 
		sITEM_COMPOSE_LIST::sITEM_FOR_COMPOSE * pMaterials, MONEY ullMoney, DBWLDSERIAL *pSerial = NULL , BYTE ComposeNum = 1);

	void LogFillSocket( Player * pPlayer, const SCItemSlot & rSocketSlot, const SCItemSlot & rTargetSlot, BYTE bySocketLevel, int iAttrIndex, int iValue );
	void LogItemExtrackSocket( Player * pPlayer, const SCItemSlot & rTargetSlot, WORD ItemCode, DURATYPE ItemNum );


	// ���� ������ ���
	void LogMonsterItemDrop( const SCItemSlot & rItemSlot, 
							DWORD dwMonsterCode, DWORD dwMonsterLevel, WORD wMapCode, const WzVector& vecPos, KEYTYPE RoomKey );
	void LogMonsterMoneyDrop( int iLogType, DWORD64 dw64Money, 
							DWORD dwMonsterCode, DWORD dwMonsterLevel, WORD wMapCode, const WzVector& vecPos, KEYTYPE RoomKey );

    void LogUseStorageItem(int iLogType, Player* player, SLOTCODE item_code, uint item_count, uint date_time);

	void LogMultiItem( int iLogType, Player * pPlayer, 
		BYTE byCount, WORD * pItemCode IN, BYTE * pItemNum IN, DWORD * pDateTime = NULL IN );
	void LogShopItem( int iLogType, Player * pPlayer, 
		DWORD dwPriceGuid, DWORD dwTransactionGuid, DWORD dwErrorCode );
	void LogShop_RecvItem( int iLogType, Player * pPlayer, 
		DWORD GiftGuid, DWORD dwResult );
	void LogItemReward( BYTE byRewardType, Player * pPlayer, BYTE totalCount, WORD * IN pRewardItemCode, BYTE * IN pRewardItemNum,
						BYTE * IN pRewardItemEnchant, BYTE * IN pRewardItemSet, BYTE * IN pRewardItemRank, CODETYPE reward_code );
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    //ģ�� ��õ �ý��ۿ��� ���� �������� �޾����� �α� �ۼ�
    void LogItemInvitatedFriendReward( Player * pPlayer, BYTE ItemCount, SLOTCODE RewardItemCode, LEVELTYPE RewardLevel);
	////////////////////////////////////////////////////////////////////////////
	//// ������
	//// 14. â�� ����
	void WriteWareHouseInfo( Player* pPlayer, BOOL bOpen );
	// ������ �α� 
	void LogSnapShot( Player* pPlayer );
	//LogSnapShot�α׷� ��ü 
	//void LogInvenInfo( int iLogType, Player* pPlayer );
	//////////////////////////////////////////////////////////////////////////

	void LogAction( int iLogType, Player * pPlayer, Player* pOtherPlayer=NULL, DWORD64 dwReservedData1=0, DWORD64 dwReservedData2=0 );
    void LogSimpleAction( int iLogType, Player* pPlayer, const TCHAR* tszAddInfo = NULL );
	void LogAction_Ranking( int iLogType, Player * pPlayer, RANKINGNO MissionNo, BYTE Ranking, DWORD ClearTime, BYTE Error );

#ifdef _NA_008119_20150310_CHECK_ONESHOT_ONEKILL_SITUATION
    //�÷��̾ ų/������ ���� �α׾����Լ�
    void LogAction_PK(int iLogType, Player * pPlayer, Player* pOtherPlayer=NULL, DAMAGETYPE _damage_value = 0);
#endif //_NA_008119_20150310_CHECK_ONESHOT_ONEKILL_SITUATION

	void LogKillSpecialMonster( Player * pPlayer, MONSTERCODE MonCode, BYTE MonsterType );

	void LogFatigue( Player* pPlayer );

	void LogRankingPoint( Player *pPlayer, DWORD GetPoint, DWORD SumPoint );

    //gamelulu_dev_note : ������ ������ ���� �α�
    void LogDomination(int iLogType, Player * player, SLOTCODE item_code, BYTE item_num, 
                       MAPCODE map_code);
    void LogDomination(int iLogType, MAPCODE mapCode);
    void LogDomination(int iLogType, Player* pPlayer, NPC* pNpc, BYTE gate1=0, BYTE gate2=0, 
                       BYTE gate3=0);
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    // ���� ����Ʈ �α�
    void LogAccumulatePoint( int iLogType, Player* pPlayer, WORD IncreasePoint, SLOTCODE RewardItemCode = 0 );
	void LogAccumulatePoint_GM( int iLogType, Player* pPlayer, WORD IncreasePoint, SLOTCODE RewardItemCode = 0 );
#endif

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    void SetAchievementInfo(WORD location, WORD index_number);
#endif //C_NA_0_20100520_ACSYSTEM_CHANGED
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    void LogPointWallet( Player* pPlayer, eLOG_ACTION action_type, DWORD point_code, \
                         DWORD previous_point, DWORD point_diffrence, DWORD current_point );
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    void LogChaosZone(eLOG_ACTION log_type, Player* player,
        const TCHAR* target_character_name = NULL, const TCHAR * add_info = NULL);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
    void LogGoldRush(eLOG_ACTION log_type, Player* player, WORD changed_point, WORD total_point, 
                     MONSTERCODE monster_code = 0, const TCHAR * add_info = NULL);
#endif //_JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT

#ifdef _NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN
    void LogGuild(eLOG_ACTION log_type, Player* player, const WORD donated_point, const TCHAR* add_info = NULL);
#endif //_NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void LogGuildFacility(eLOG_ACTION log_type, Player* player, GameGuild* const guild, FCODE code, FLEVEL level);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007086_20140318_MONSTERBOOK
    void LogMonsterBook(eLOG_ACTION log_type, Player* player, MONSTERCODE monster_code);
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    void LogWzDat(eLOG_ACTION log_type, const TCHAR* add_info = NULL);
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    // ���� ��� �α�
    void LogAwakening( int iLogType, Player * pPlayer, SCItemSlot& equip_item,
        const CODETYPE material_item_code, const BYTE material_awakening, 
        const BYTE result_awakening, const BYTE prev_awakening, const CODETYPE additive_item_code);
    void LogEvolution(int iLogType, Player * pPlayer, const CODETYPE result_item_code, 
        const CODETYPE prev_item_code, const CODETYPE additive_item_code);
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
    // �ִ� ������ �α� Damaged���� ȣ��Ǿ� attacker�� �������� �����
    void LogMaxDamage(eLOG_ACTION log_type, Character* attacked_character, Character* attacker, DamageArgs* damage_arg);
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG

#ifdef _NA_008334_20150608_SONNENSCHEIN
    VOID LogSonnenSchein(eLOG_ACTION log_type, GUILDGUID guild_guid, Player* player);
#endif //_NA_008334_20150608_SONNENSCHEIN

    //_NA_008506_20150923_LOG_LOGOUT_IN_GAMESERVER
    VOID LogLogout(Player* player);

	// �����׼� �α�
	// 1. ��Ȱ �α�
	void WritePlayerResurrection( Player* pPlayer, Player *pHealer );
	// 2. ��� �α�
	void WritePlayerDead( Player* pPlayer, MAPCODE Mapcode = 0 );
	// 4. ���ݻ�� �α�
	void WriteUseStat( Player* pPlayer, BYTE byStatType );
	// 7. ��ų������
	void WriteSkillLevelUp( Player* pPlayer, SLOTCODE SkillCode, BOOL bLevelUp );
	// 8. PVP( bKill�� TRUE�� ���� ���ΰŰ�, FALSE�� ���濡�� ���� ���ΰŴ�. )
	void WritePVPKillUser( Player* pPlayer, Player* pOtherPlayer, BOOL bKill );
	// 9. PVP ����
	void WritePVPStart( Player* pPlayer );
	void WriteSkillLevelDown( Player* pPlayer, SLOTCODE SkillCode, BOOL bLevelDown ); //< bLevelDown�� FALSE �� UNLEARN
	void WriteDecreaseStat( Player* pPlayer, BYTE byStatType, SHORT Decrease );
	void WriteInitStatAll( Player* pPlayer, eLOG_ACTION eLogAction );
	void CharacterChangeUpLog( Player* pPlayer, BYTE byClassType, BYTE byChangeUpCode );
	//////////////////////////////////////////////////////////////////////////
	// ���� �α� 
	// 4. ĳ���� ����
	void WriteCharConnect( Player* pPlayer );


	//////////////////////////////////////////////////////////////////////////
	// GM�α�
	void	WriteAccount( Player* pPlayer );							// ��ī��Ʈ
	void	WriteEntrance( Player* pPlayer );							// ����
	void	WriteTransparent( Player* pPlayer );						// ����
	void	WriteInvincibility( Player* pPlayer );						// ����

	void	WriteRoomInfo( Player* pPlayer, DWORD dwRoomKey );			// ������
	void	WriteSpeed( Player* pPlayer );								// �ӵ�
	void	WriteRebirth( Player* pPlayer );							// ��Ȱ

	void	WriteRemovalItem( Player* pPlayer );						// ����������
	void	WriteRemovalMonster( Player* pPlayer );						// ��������
	void	WriteRecovery( Player* pPlayer );							// ȸ��
	void	WriteLevelUp( Player* pPlayer, WORD wOldLevel );			// ������
	void	WriteStatPointUp( Player* pPlayer, WORD wPlusPoint );		// ���ݾ�
	void	WriteSkillPointUp( Player* pPlayer, WORD wSkillPoint );		// ��ų����Ʈ��	
	void	WriteCreateMonster( Player* pPlayer, DWORD dwMonsterCode, WORD wMonsterCount );
	void	WriteRemoval( Player* pPlayer, const TCHAR* pszMonsterName );		// ����

	void	WriteCreateItem( Player* pPlayer, WORD wItemCode, WORD wItemCount );	// ����
	void	WriteCreateHeim( Player* pPlayer, MONEY amount );						// ����
	void	WriteKill( Player* pPlayer, const TCHAR* pszMonsterName );				// ų
	void	LogGM( int nLogType, Player* pPlayer );
	//////////////////////////////////////////////////////////////////////////
	// �������� ���ӷα�
	void StartRegularLog( int nLogType );


	//////////////////////////////////////////////////////////////////////////
	// �������� ���ӷα� ������
	friend unsigned int __stdcall SkillWorkThread(void* pArg);
	
	/////////////////////////////////////////////////////////////////////////
	//��ó��
	void PreProcess();
};
