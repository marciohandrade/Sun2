#pragma once


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
class BattleRecord;	class Player;			
class Character;	class NPC;
class GameField;	class Item;
struct DistributeExpOprArgs;
struct DistributeExpOprResult;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
class ItemExpDistributor
{
public:
	ItemExpDistributor() {}
	~ItemExpDistributor() {}

	// 소유권 결정
	Character*		DecideExpOwnership( NPC *pNPC );
	Player*			DecideItemOwnership( NPC *pNPC );

	// 경험치 분배
	VOID			DistributeExp( NPC *pNPC, Character *pExpOwner );
		BOOLEAN		GetCalcedExpValue(
								Player* const						pPlayer
								, const DistributeExpOprArgs* const	pArgs
								, DistributeExpOprResult* OUT		pResult
								);

	// 아이템, 돈 분배
	VOID			DistributeItem( Item * pItem, Player * pItemOwner );
    VOID			DistributeMoney( MONEY Money, DWORD dwMonsterKey, Player * pItemOwner );
};


extern ItemExpDistributor g_ItemExpDistributor;



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
struct DistributeExpOprArgs
{
	GameZone* const			pGameZone;
	GameField* const		pGameField;
	const eZONETYPE			zoneType;
	const BYTE				roomBonusType;
	const BYTE				radius;
	const WORD				memberNum;
	const LEVELTYPE			totalLevel;
	const FLOAT				fRadius_Q2;		// radius*radius
	const DWORD				npc_key;
	const WzVector* const	pwvNpcPos;
	const FLOAT				fNpcExpRatio;
	const DWORD				npcMaxHP;
	const LEVELTYPE			npcLevel;

	DistributeExpOprArgs(
		GameZone* const			_pGameZone
		, GameField* const		_pGameField
		, const eZONETYPE		_zoneType
		, const BYTE			_roomBonusType
		, const BYTE			_radius
		, const WORD			_memberNum
		, const LEVELTYPE		_totalLevel
		, const FLOAT			_fRadius_Q2
		, const DWORD			_npc_key
		, const WzVector* const	_pwvNpcPos
		, const FLOAT			_fNpcExpRatio
		, const DWORD			_npcMaxHP
		, const LEVELTYPE		_npcLevel
		)
		: pGameZone		( _pGameZone		)
		, pGameField	( _pGameField		)
		, zoneType		( _zoneType			)
		, roomBonusType	( _roomBonusType	)
		, radius		( _radius			)
		, memberNum		( _memberNum		)
		, totalLevel	( _totalLevel		)
		, fRadius_Q2	( _fRadius_Q2		)
		, npc_key		( _npc_key			)
		, pwvNpcPos		( _pwvNpcPos		)
		, fNpcExpRatio	( _fNpcExpRatio		)
		, npcMaxHP		( _npcMaxHP			)
		, npcLevel		( _npcLevel			)
	{}
    __DISABLE_COPY(DistributeExpOprArgs);
};

struct DistributeExpOprResult
{
	EXPTYPE			TotalExp;	// 전체 경험치
	INT				BonusExp;	// TotalExp = calced_value + BonusExp
};

