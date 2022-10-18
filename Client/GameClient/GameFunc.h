#pragma once
//=======================================================================================================================
/** 
	게임에 쓰이는 전역 함수들 모음 
	@author
	
	@since

	@remarks
	-IsMove 함수 추가: 맵상에서 이동 가능 한지 경로 생성이 가능한지 검사 한다.2005.08.26 지완 
*/
#include "CharacterDef.h"

class Object;
class Character;
class PlayerMonster;
class Item;
class MapObject;
class SolarDialog;
class NPC;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
class StatuePlayer;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
struct PKOptionInfo;

enum eWEAPONSOUNDKIND;

struct RELATIONKIND
{
	enum TYPE
	{
		None,
		Assistant,
		Enemy,
		Friend,
        Vendor,
	};
};

#ifdef _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_
class PKRuleForTile
{
	// 안전지대 추가 및 코드 정리
	// 타일 속성별 룰 테이블
	//  안전지대              경계지역                   분쟁지역            자유분쟁              점령전
	//     x      Ctrl(파티,길드 및 20렙이하 불가)  O(파티,길드원 불가)   O,Ctrl(파티,길드원)   길드관계(공/방)
	//                    길드관계 우선
	// *서로 다른 속성타일에 있을 경우 공격불가
private:
	RELATIONKIND::TYPE PKTile(Player* pPlayer, PKOptionInfo* pPKInfo, bool bForceEnemy);
	RELATIONKIND::TYPE ConflictTile(Player* pPlayer, PKOptionInfo* pPKInfo, bool bForceEnemy);
	RELATIONKIND::TYPE FreeConflictTile(Player* pPlayer, PKOptionInfo* pPKInfo, bool bForceEnemy);
	RELATIONKIND::TYPE DominationTile(Player* pPlayer, PKOptionInfo* pPKInfo, bool bForceEnemy);
public:
	RELATIONKIND::TYPE CheckRelationTileAttr(Player* pPlayer, PKOptionInfo* pPKInfo, bool bForce);
};
#endif //_NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_

const DWORD kNormalItemResultStringCode = 0; // 일반아이템
const DWORD kItemSubTypeElite = 0x10000; // 엘리트의 SubType = 1
const DWORD kItemSubTypeUnique = 0x20000; // 유니크의 SubType = 2
const DWORD kItemSubTypeCorrupt = 0x30000; // 저주셋의 SubType = 3

//const WORD kItemLevelNormal = 1~15
const WORD kItemLevelUltimate = 17;
const WORD kItemLevelElite1 = 21;
const WORD kItemLevelCorrupt1 = 21;
const WORD kItemLevelElite1_5 = 23;
const WORD kItemLevelElite2 = 25;
const WORD kItemLevelUnique1 = 31;
const WORD kItemLevelUnique2 = 35;

class GameFunc
{
public:
	static BOOL					PickTerrainMesh( const WzVector & IN vOrigin,WzVector & OUT vPickPos, BYTE CheckType = TMGO_NO_COLLISION, float height = 3.0f );

	static Character *			GetPickedCharacter(WzRay * pRay,DWORD dwPickingOption);
	static Item *				GetPickedItem(WzRay * pRay);
	static MapObject *			GetPickedMapUnitObject(WzRay * pRay);
    static BOOL					GetClickedPosition(WzVector * OUT pPosition);
    static BOOL					GetVisiblePosition( WzVector * IN OUT pInOutVisiblePos );
	static RELATIONKIND::TYPE	GetRelationKindOfHero(Character* pObject,BOOL bForceEnemy=FALSE);
	static RELATIONKIND::TYPE	GetGuildRelationShip( Player * pPlayer, BOOL bForceEnemy );
    static void GetRealItemName(SCItemSlot* slot_item,
                                const BASE_ITEMINFO* item_info, 
                                TCHAR* name, 
                                int size);
    static void GetCustomizeInfo(SCItemSlot* slot_item, TCHAR* name);
    static void GetItemName(SCItemSlot* slot_item,
                            const BASE_ITEMINFO* item_info, 
                            TCHAR* name, 
                            int size);
    static void GetItemDescription(TCHAR* desc_string, SCItemSlot* item_slot, const BASE_ITEMINFO* item_info);

    static bool IsCheckItem(SCSlot& slot);
    static CODETYPE GetImagecode(SCItemSlot* item_slot, CODETYPE base_code);
    static BOOL IsLimitedItem(SCItemSlot* item_slot, CODETYPE base_code);
	static RELATIONKIND::TYPE	GetRelationKindOfPlayerInField( Character* pObject,BOOL bForceEnemy = FALSE );
	static RELATIONKIND::TYPE	GetRelationKindOfPlayerInVillage( Character* pObject,BOOL bForceEnemy = FALSE );
	static RELATIONKIND::TYPE	GetRelationKindOfPlayerInPVPZone( Character* pObject,BOOL bForceEnemy = FALSE );
	static RELATIONKIND::TYPE	GetRelationKindOfPlayerInSSQZone( Character* pObject,BOOL bForceEnemy = FALSE );
	static RELATIONKIND::TYPE	GetRelationKindOfPlayerInHuntingZone( Character* pObject,BOOL bForceEnemy = FALSE );
	static RELATIONKIND::TYPE	GetRelationKindOfPlayerInMissionZone( Character* pObject,BOOL bForceEnemy = FALSE );
	static RELATIONKIND::TYPE	GetRelationKindOfPlayerInInstanceDungeon( Character* pObject,BOOL bForceEnemy = FALSE );
	static RELATIONKIND::TYPE	GetRelationKindOfPlayerInDominationWar( Character* pObjcet, BOOL bForceEnemy = FALSE );
    static RELATIONKIND::TYPE	GetRelationKindOfPlayerInBattleGround( Character* pObjcet, BOOL bForceEnemy = FALSE );
    static RELATIONKIND::TYPE	GetRelationKindOfPlayerInFreePVP( Character* pObjcet, BOOL bForceEnemy = FALSE );
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    static RELATIONKIND::TYPE	GetRelationKindOfPlayerInGoldRush( Character* pObjcet, BOOL bForceEnemy = FALSE );
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    static RELATIONKIND::TYPE	GetRelationKindOfPlayerInSpa( Character* pObjcet, BOOL bForceEnemy = FALSE );
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    static RELATIONKIND::TYPE	GetRelationKindOfPlayerInGuildMission( Character* pObjcet, BOOL bForceEnemy = FALSE );
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    

	static RELATIONKIND::TYPE	GetRelationOfPlayer( Player *pPlayer, BOOL bForceEnemy );
	static RELATIONKIND::TYPE	GetRelationOfMonster( Monster *pMonster, BOOL bForceEnemy );

	static BOOL					GetCheckPlayerOnTile( DWORD dwPlayerTile, DWORD dwHeroTile, DWORD dwTileAttr );

	static BOOL                 IsEnablePk(DWORD targetKey);
	static WzVector             FindStandPosition(WzVector vSrc,WzVector vDest);

	static BOOL						IsFriendShip( Player * pPlayer );

	static DWORD				GetAttackTargetWithinRange(
                                    WzVector vPos, 
                                    float fRange,
                                    DWORD *pObjectKeys,
                                    DWORD exception);

	static DWORD				GetAttackTargetWithinLine(
                                    WzVector vPos, 
                                    WzVector vDest,
                                    float fRange,
                                    DWORD * pObjectKeys,
                                    DWORD exception);
	static BOOL					IsMove(
                                    WzVector vPos,
                                    WzVector vDest,
                                    BOOL bCheckMakePath = TRUE,
									DWORD dwMaxNode=MAX_NODE_FOR_PATHFIND);	

    static bool IsActiveCharCustomize(POSTYPE pos);

    static BodyPart             EquipToBodyPart(eEQUIPCONTAINER_POS equip_pos/*item_info->m_wEqPos*/);
	static eWEAPONSOUNDKIND     GetWeaponSoundKind(eWEAPONTYPE eType);
	static BOOL					GetBonePos(
                                    CWzUnitDraw *pSkeleton, 
                                    int index, 
                                    WzVector * OUT pVector );
	static BOOL					GetBoneMatrix(
									CWzUnitDraw *pSkeleton,
									int index,
									WzMatrix * pMatrix );
	static BOOL					UpdateCursorByPickedObject(Object * pObject);
    // NPC 만났을때 포워딩
	static SolarDialog *		NPCMeetWithForward(
                                    MAPID MapID, 
                                    FIELDID FieldID, 
                                    CODETYPE NpcCode, 
                                    DWORD dwNPCKey = 0 );

    // NPC 만나서 특정 창 열때    
	static SolarDialog *		OpenNPCMeetDialog(
                                    MAPID MapID, 
                                    FIELDID FieldID, 
                                    CODETYPE NpcCode );

	static BOOL					CheckHeroInAreaForQuest(DWORD areaID);
	static BOOL					IsHeroInAreaForQuest(DWORD areaID);
    static Monster	*           SpawnMonster( CODETYPE MonsterCode, size_t MonsterCount = 1, WzVector* wzVec = NULL, DWORD dwMaster = 0 );
	static Player *             SpawnPlayer(BYTE bCharType);
	static DWORD				GetRandomStyle(DWORD dwWeaponStyle,BYTE bClassCode);
    static void                 SetPlayerMonsterAttackStyle(PlayerMonster* player_monster);
	

	static void					SpawnFullsetPlayer(BYTE bCharType , BYTE bySetType);

	static BOOL                 GetValidPosForRange(WzVector & IN rwzPos, WzVector & OUT rOutwzReturn, float fRangeLength = 10.f);
	static BOOL                 GetValidPosForRange(WzVector & IN rwzPos, float IN fAngle, WzVector & OUT rOutwzReturn, float IN fRangeLength = 10.f);

	static void					GetPlayerSkill(int charType,Character *pChar,Character * pTarget,PLAYER_ACTION &action);

	static eZONETYPE            ConvertServerZoneType(SCENE_TYPE eSceneType);
    
    static int                  CheckLODLevelByPlayerCount(int player_count, int option_lod_level);
    static void                 ApplyLODDistance(int lod_level);
    static void                 ApplyEffectLODDistance(int lod_level);

    static int GetItemGradeStringCode(TCHAR *item_name, WORD item_level, ItemSubTypeDetail item_subtype);

    static int GetNearestTile(const WzVector& pos);
    //------------------------------------------------------------------------------
    // Returns :int : 타일 번호
    // Qualifier : 임의 점에서 가장 가까운 타일을 찾는다.
    // Parameter : WzVector & pos : 임의 점
    // Parameter : DWORD restrict_tile_attribute : 제약을 걸 속성
    //------------------------------------------------------------------------------
    static int GetNearestTile(const WzVector& pos, WORD restrict_tile_attribute);
    static bool FindMoveableNearestTile( const WzVector& StartPosition, IN OUT WzVector& DestPosition, OUT int& tile_number );

    static bool IsPossibleStand(WzVector& pos, float height, WORD restrict_tile_attribute);
    static bool IsIntersect(WzVector& scr, WzVector& dst);

    //------------------------------------------------------------------------------
    // Qualifier : 현재 ObjectManager 에 있는 waypoint를 얻어옴
    //------------------------------------------------------------------------------
    static MapObject* GetWaypoint();

    //------------------------------------------------------------------------------
    // Qualifier : 해당 아이템을 파는 npc를 찾음
    // Parameter : eITEMTYPE sell_item_type : 판매하는 아이템 타입
    //------------------------------------------------------------------------------
    static NPC* GetNearestShopNpc(eITEMTYPE sell_item_type);
    static NPC* GetNearestShopNpcByItem(SLOTCODE item_code);

    //------------------------------------------------------------------------------
    // Returns :int : 몬스트 갯수
    // Qualifier : 해당 object_key(플레이어)를 공격중인 몬스트 갯수
    // Parameter : OUT vector<Monster * > & monster_vector : 몬스트 리스트
    // Parameter : DWORD object_key : 플레이어 키
    //------------------------------------------------------------------------------
    static int GetMonstetListTargetPlayer(OUT vector<DWORD>& monster_vector, DWORD object_key);

    //------------------------------------------------------------------------------
    //! Returns:   float 보간값
    //! Qualifier: 2값사이의 보간값
    //------------------------------------------------------------------------------
    static int GetInterpolationValue(int small_value, int big_value, float ratio);
    static float GetInterpolationValue(float small_value, float big_value, float ratio);
    static RECT GetInterpolationValue(RECT& small_value, RECT& big_value, float ratio);


    //------------------------------------------------------------------------------ 
    //! 남과장님 요청한 예제 함수
    //! GetPlayerNextDirection 예제
    static bool Tile_CrossCheck(WzVector *pwvFrom, 
                                WzVector *pwvTo, 
                                WzVector *pwvEdge1, 
                                WzVector *pwvEdge2, 
                                float *pfDist, 
                                WzVector* pwvCross);
    static bool GetTangentDirection(WzVector* triangle_vector_list, 
                                WzVector& from, 
                                WzVector& to,
                                OUT WzVector& out_dir, 
                                OUT float& dot_product_value);
    static void ExamplePlayerNextDirection(Player* player, DWORD elapse_tick);


    //! 플레이어 생성
    //------------------------------------------------------------------------------
    //! Returns:   Player* : 
    //! Qualifier: ObjectManager에 Player 생성
    //! Parameter: const DWORD object_Key : object key(고유)
    //! Parameter: Player * base_player
    //! Parameter: bool is_with_equip : base_player 에서 파츠 정보까지 복사 여부
    //------------------------------------------------------------------------------
    static Player*  MakePlayer(const DWORD object_key, Player* base_player, bool is_with_equip);

    static Player*  MakeBattleZonePlayer(PLAYER_RENDER_INFO_EX& render_info_ex, 
                                        ETC_STATE_TOTAL_INFO* state_total_info,
                                        VENDOR_RENDER_INFO* vendor_render_info,
                                        EQUIP_ITEM_BASEINFO* equip_item_info,
                                        sMSG_RENDER_EFFECT_ITEM_LIST* effect_item_info);
    static bool     IsTransform(PLAYER_RENDER_INFO_EX& render_info_ex,
                                OUT RESOURCECODE& resource_code,
                                OUT DWORD& skill_code, 
                                OUT eTRANSFORM_TYPE& transform_type);
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	//! 동상 생성
	//---------------------------------------------------------------------------------
	//! Qualifier: ObjectManager에 BronzePlayer 생성
	//---------------------------------------------------------------------------------
	static StatuePlayer* MakeStatuePlayer(const WzVector& position, 
		                                  const WzVector& direction, 
										  const float& scale_value, 
										  StatueInfo statue_info);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
    

    // 요일 구하기
    static int     GetDayOfTheWeek(int year, int month, int day);
    // 마지막 일 구하기
    static int     GetLastDayOfTheMonth(int year, int month);
};

void	ClampAngle(float *pAngle); //혼자 쌩뚱 맞다!
void	RenderOutLinedText(TCHAR *pText,RECT *prc,WzColor color, BYTE alpha = 255);
void	RenderShadowedText(TCHAR *pText,RECT *prc,WzColor color);
void	RenderBlockedText(TCHAR *pText,RECT *prc,WzColor color,WzColor BackColor);
void	RenderBlockedTextWithDepthTest( LPCTSTR pText,RECT *prc,WzColor color,WzColor BackColor,float fDepth, bool bPlayer = false);
float	LerfFloat(float fNumber,float fNumber2,float fRate);
void	SetGamma(DWORD dwRamp);
DWORD	GetGamma();


