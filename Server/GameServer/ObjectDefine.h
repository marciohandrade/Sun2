#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<OBJECTs>
class Object;	class Player;		class Character;
class NPC;		class MapObject;	class Item;
class Summoned; class GameGuildMember;
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<ETCs>
class GameField;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SHARED>
#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
//	-	소환체 코드별 숫자관리
typedef STLX_HASH_MAP<DWORD, DWORD>						NUMBER_OF_NPCS_MAP;				// <MonsterCode, Count>
typedef NUMBER_OF_NPCS_MAP::iterator					NUMBER_OF_NPCS_MAP_ITER;
typedef std::pair<DWORD, DWORD>							NUMBER_OF_NPCS_MAP_PAIR;
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//

typedef STLX_MAP<DWORD, NPC*>							SUMMONED_MAP;
typedef SUMMONED_MAP::iterator							SUMMONED_MAP_IT;
typedef std::pair<DWORD, NPC*>							SUMMONED_MAP_PAIR;

typedef STLX_MAP<DWORD, Player*>						PLAYER_HASH;
typedef PLAYER_HASH::iterator							PLAYER_HASH_ITR;
typedef std::pair<DWORD, Player*>						PLAYER_HASH_PAIR;

typedef PLAYER_HASH										_PLAYER_HASH;	// playerkey, userkey혼용 사용중...-.-;
typedef _PLAYER_HASH::iterator							_PLAYER_HASH_ITR;

// f100827.4L
typedef STLX_HASH_MAP<DWORD, util::Timer*> COOL_TIME_TABLE;

typedef STLX_HASH_MAP<CHARGUID, GameGuildMember*> GUILDMEMBER_HASH;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SHARED>
//
typedef STLX_HASH_MAP<FIELDCODE, GameField *>			_GAMEFIELD_HASH;


