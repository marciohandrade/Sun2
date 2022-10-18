#pragma once

#include "MapDefine.h"

class CPathExplorer;
class Character;

namespace nsAI
{

//--------------------------------------------------------------------------------------------------

struct CacheInfo;
struct CacheCmd;
struct TileArray;
class IPlanEventObject;

enum eWeight
{
    eWeight_None            = 0,
    //eWeight_Level_Light     = 100,    //  ���� ���� ���� ����, �ڿ� ���� ����
    //eWeight_Level_Middle    = 1000,   //  ������ design�� ����, ���� �ڵ�
    eWeight_Level_Heavy     = 200,  //  ���� ���� ��� ����, ���⼭���ʹ� �ڿ����ҵ��� �ʴ´�.
    //  �� ���ĺ��ʹ� ���� ������ ���� ��ŷ�ڵ尡 ���ܵȴ�.
    //  Range: [ eWeight_Level_Heavy, 256)
#ifdef _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE
    eWeight_ClosedSpace     = eWeight_Level_Heavy,  //  'Ÿ���� ���' ������� �������� PVP �����ϵ��� Ÿ�� �Ӽ� ����
#else
    eWeight_ClosedSpace     = eWeight_Level_Heavy + 1, //  'Ÿ���� ���' ������� �����ڵ�
#endif  // _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE
    eWeight_Earthquake_Indi = eWeight_Level_Heavy + 2, //  'Ÿ���� ���' �������� Ż�� ���� �ڵ�

    //
    eWeight_Upperbound      = 256,
};

enum eCharEvent
{
    eCharEvent_None     = 0,
    eCharEvent_ExistNpcTileEvent = 1 << 0,
};

//--------------------------------------------------------------------------------------------------
struct PlanCmd
{
    enum eCmd {
        eCmdTypeMask             = 0xFF000000,
        eTileWeightCmd           = 0x01000000, // 24
        eNpcAreaEvent            = 0x02000000, // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE

        eTileWeight_SetWeight    = eTileWeightCmd | (1),
        eTileWeight_GetTileEvent = eTileWeightCmd | (2),
        eNpcAreaEvent_Regist     = eNpcAreaEvent | (1),
        eNpcAreaEvent_Unregist   = eNpcAreaEvent | (2),
        eNpcAreaEvent_Get        = eNpcAreaEvent | (3),
    };

    const eCmd Cmd;
    PlanCmd(const eCmd _cmd) : Cmd(_cmd) {}

    struct TileWeightArg;
    struct NpcAreaEvent;
    __DISABLE_COPY(PlanCmd);
};

//--------------------------------------------------------------------------------------------------
struct PlanCmd::TileWeightArg : public PlanCmd
{
    TileWeightArg(const PlanCmd::eCmd _cmd, const int _tileNo, const eWeight _weight = eWeight_None)
        : PlanCmd(_cmd)
        , TileNo(_tileNo)
        , Weight(_weight)
    {};

    const int TileNo;
    const eWeight   Weight;
    __DISABLE_COPY(TileWeightArg);
};

//--------------------------------------------------------------------------------------------------
// NOTE: this object is implemented by _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
struct PlanCmd::NpcAreaEvent : public PlanCmd
{
    NpcAreaEvent(const PlanCmd::eCmd _cmd, const int _tileNo, const eCharEvent _char_event)
        : PlanCmd(_cmd)
        , TileNo(_tileNo)
        , CharEvent(static_cast<uint8_t>(_char_event))
    {};

    const int TileNo;
    const uint8_t CharEvent;
    __DISABLE_COPY(NpcAreaEvent);
};

//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  <MovingPlanner>
//  -   CWorldBase + CPathFinderBase ���� ����, �������� ����ϴ� �͵鸸 (�̵�/���� ����) ���� ����
//  -   ���� ���� : GameField ����
//  -   ���� : �̵��� Object�鿡 ���� �̵��� ���� �߰� ó�� ����
class MovingPlanner
{
private:
    SharedFieldInfo* const m_pSharedFieldInfo;
    CWorldBase* const m_pWorldBase;
    CPathFinderBase* const m_pPathFinder;
    const int m_RoomIndex;
    CRoomManager* const m_pRoomManager;
    GameField*              m_pGameField;
    CPathExplorer*          m_pPathExplorer;    // for test
    DWORD                   m_LastUpdate;
    TileArray*              m_pTileArray;
    util::BitSetOperationV        m_TempFlag;
    CPathExplorer*          m_pTempPathPlanAttack;  //_0_20100427_DOMINATION_FINDPATH_ATTACK_BUG
public:
    MovingPlanner()
        : m_pSharedFieldInfo(0)
        , m_pWorldBase(0)
        , m_pPathFinder(0)
        , m_RoomIndex(-1)
        , m_pRoomManager(0)
        , m_pGameField(0)
        , m_pPathExplorer(0)
        , m_LastUpdate(0)
        , m_pTileArray(0)
        , m_pTempPathPlanAttack(0)  //_0_20100427_DOMINATION_FINDPATH_ATTACK_BUG
    {}

    MovingPlanner(SharedFieldInfo* const pSharedFieldInfo, const int room_index);
    ~MovingPlanner();
    void Init(GameField* const pGameField);
    void Release();
private:
    //inline BOOLEAN _CheckTileCache(const int iTile);
    BOOLEAN _TestFunction();

    //////////////////////////////////////////////////////////////////////////
    //  SUN 3D Engine - Wrapped Interfaces
    //////////////////////////////////////////////////////////////////////////
public:
    WzVector GetRandomPosInTile(const int iTile);
    void GetRandomPosInTile(const int iTile, WzVector* OUT pPos); // (WAVERIX) (090708) (ADDIN)
    int GetNumPathTile();

    void SetAttribute(const int iTile, const WORD wAttribute);
    void UnsetAttribute(const int iTile, const WORD wAttribute);
    BOOLEAN CheckAttribute(const int iTile, const WORD wAttribute);
    WORD     GetAttribute(const int iTile);


    //  Wrapper interface x_pPathFinder
    int GetTileToStand(const WzVector& rPos,
                       float* pfHeight = NULL,
                       const float fJumpLimit = -1.0f,
                       const float fAdjustLimit = -1.0f,
                       WORD* const pwAttribute = 0);
    //  Wrapper interface x_pPathFinder
    int GetAllowTileToStand(const WzVector& rPos,
                            float* pfHeight = NULL,
                            const float fJumpLimit = -1.0f,
                            const float fAdjustLimit = -1.0f,
                            WORD* const pwAttribute = 0,
                            const WORD wNotAllowAttribute = PTA_NO_WALK);

    //////////////////////////////////////////////////////////////////////////
    //  SUN 3D Engine - Wrapped Interfaces
    //////////////////////////////////////////////////////////////////////////

    //  Wrapper interface < 3D ��ǥ�� �༭ �ʱ�ȭ (������ ��)
    BOOLEAN PathExp_Reset(Character* const pMovingObject,
                          const WzVector* IN pwvPos, const int iTile = -1);
    //  Wrapper interface < �� ã��
    ENUM_FINDPATH_RESULT    PathExp_FindPath(Character* const pMovingObject,
                                             const WzVector& IN rwvDest, const int iDestTile,
                                             const WORD wAttribute = (PTA_ONLY_JUMP | PTA_NO_WALK),
                                             const int iSeekNode = MAX_NODE_FOR_PATHFIND);
    //  Wrapper interface < ��� ����
    BOOLEAN PathExp_SetPath(Character* const pMovingObject,
                            const WzVector* IN pwvDest,
                            int* const pTileRoute, const int iNumTileRoute);
    //  Wrapper interface < �� ã�� ����
    BOOLEAN PathExp_Progress(Character* const pMovingObject,
                             const float fMove, WzPathFindEvent* pEvent = NULL,
                             ENUM_CHECK_COLLISION eCC = ECC_NO); //< �� ã�� ���� (��ǥ �̵��� �־����� TRUE)
    //  Wrapper interface < Ư�� �������� Ư�� �Ÿ���ŭ �б�
    ENUM_FINDPATH_RESULT    PathExp_Thrust(Character* const pMovingObject,
                                           const WzVector* IN pwvMoveDistance,
                                           const WORD wAttribute = (PTA_ONLY_JUMP | PTA_NO_WALK));

    //////////////////////////////////////////////////////////////////////////
    //  SUN Server PathPlan Interfaces for Moving Planning
    //////////////////////////////////////////////////////////////////////////

    //  Wrapper interface < 3D ��ǥ�� �༭ �ʱ�ȭ (������ ��)
    BOOLEAN PathPlan_Reset(Character* const pMovingObject,
                           const WzVector* IN pwvPos, const int iTile = -1);
    //  Wrapper interface < �� ã��
    ENUM_FINDPATH_RESULT    PathPlan_FindPath(Character* const pMovingObject,
                                              const WzVector& IN rwvDest,
                                              const int iDestTile,
                                              const WORD wAttribute = (PTA_ONLY_JUMP | PTA_NO_WALK),
                                              const int iSeekNode = MAX_NODE_FOR_PATHFIND);
    //  Wrapper interface < ��� ����
    BOOLEAN PathPlan_SetPath(Character* const pMovingObject,
                             const WzVector* IN pwvDest,
                             int* const pTileRoute, const int iNumTileRoute);
    //  Wrapper interface < �� ã�� ����
    BOOLEAN PathPlan_Progress(Character* const pMovingObject,
                              const float fMove, WzPathFindEvent* pEvent = NULL,
                              ENUM_CHECK_COLLISION eCC = ECC_NO); //< �� ã�� ���� (��ǥ �̵��� �־����� TRUE)
    //  Wrapper interface < Ư�� �������� Ư�� �Ÿ���ŭ �б�
    ENUM_FINDPATH_RESULT    PathPlan_Thrust(Character* const pMovingObject,
                                            const WzVector* IN pwvMoveDistance,
                                            const WORD wAttribute = (PTA_ONLY_JUMP | PTA_NO_WALK));

    //_NA001385_20090924_DOMINATION_FINDPATH_ATTACK 
    ENUM_FINDPATH_RESULT    PathPlan_MoveCheck(Character* const pMovingObject,
                                               const WzVector& IN rwvDest,
                                               const int iDestTile,
                                               const WORD wAttribute,
                                               const int iSeekNode);
    // CHANGES: f110823.3L, added to support fast skill moving action.
    bool PathPlan_FastMoved(Character* const moving_object,
        const WzVector& IN dest_pos, const WORD tile_attribute,
        WzVector* const result_moved_pos);

public:
    BOOLEAN PathPlan_Command(const PlanCmd& rCmd, const TileEvent** OUT ppTileEvent = 0);

private:
    ////    no-check bound
    ////    <returns>
    ////    BOOLEAN --> pre updated status --> action:updated_status
    //inline BOOLEAN _TileNodeUpdate(const int tileNo, TileEvent& OUT rNode);
    //  no-check bound
    inline void _UpdateNearNodes(const WORD tileNo, WORD OUT nearNodes[3]);
    inline void _UpdateCharacterInfo(Character* const pCharacter, const WORD tileNo);
    //  Tile Weight Cost Operation  < Ư�� Ÿ�Ͽ� ���� �������� ����ġ ó��
private:
    BOOLEAN _PathPlan_TileWeight(const PlanCmd::TileWeightArg& IN rArg,
                                 const TileEvent** OUT ppTileEvent);     // ���� �򰡰� �䱸�� ��...
    bool _PathPlan_NpcAreaEvent(const PlanCmd::NpcAreaEvent& IN arg);

    inline const TileEvent& _GetTileEvent(const int iTile);
    // depth-value 3(�� ���� �˻�),2,1,0
    WORD/*TileNo*/ _SearchValidNearNode(const TileEvent& rTileEvent,
                                        const DWORD depth, const WORD cost);

    enum eFindSafezoneReason {
        eFSR_CurTileDanger,
    };
    BOOLEAN _PathPlan_FindPathToSafeZone(NPC* const pNPC, PlanEventObject& rPlanEvent,
                                         const eFindSafezoneReason reason);
    inline void _PathPlan_NearNodeRandomSort(TileEvent& OUT rOutTileEvent,
                                             const TileEvent& rInSrcTileEvent);
    friend class PlanEventObject;
};

//--------------------------------------------------------------------------------------------------



}; // end namespace nsAI



namespace nsAI
{

//	for easy access
class CPathExplorerD : public CPathExplorer
{
public:
    struct PathExpD
    {
        PathExpD(const int& iTileCur,
                 const WzVector& wvPosCur,
                 const int& iTileTarget,
                 const WzVector& wvPosTarget,
                 const DWORD& movingStatus)
            : riTileCur(iTileCur)
            , rwvPosCur(wvPosCur)
            , riTileTarget(iTileTarget)
            , rwvPosTarget(wvPosTarget)
            , rMovingStatus(movingStatus)
        {}
        const int& riTileCur;
        const WzVector& rwvPosCur;
        const int& riTileTarget;
        const WzVector& rwvPosTarget;
        const DWORD& rMovingStatus;
        __DISABLE_COPY(PathExpD);
    };
    static const int& P_GetCurTile(CPathExplorer* const pPathExp) {
        BOOST_STATIC_ASSERT(sizeof(static_cast<CPathExplorerD*>(pPathExp)->m_iTile) ==
                            sizeof(int));
        return static_cast<CPathExplorerD*>(pPathExp)->m_iTile;
    }
    static const WzVector& P_GetPosPtr(CPathExplorer* const pPathExp) {
        BOOST_STATIC_ASSERT(sizeof(static_cast<CPathExplorerD*>(pPathExp)->m_wvPos) ==
                            sizeof(WzVector));
        return static_cast<CPathExplorerD*>(pPathExp)->m_wvPos;
    }
    static const int& P_GetTargetTile(CPathExplorer* const pPathExp) {
        BOOST_STATIC_ASSERT(sizeof(static_cast<CPathExplorerD*>(pPathExp)->m_iTargetTile) ==
                            sizeof(int));
        return static_cast<CPathExplorerD*>(pPathExp)->m_iTargetTile;
    }
    static const WzVector& P_GetTargetPos(CPathExplorer* const pPathExp) {
        BOOST_STATIC_ASSERT(sizeof(static_cast<CPathExplorerD*>(pPathExp)->m_wvTarget) ==
                            sizeof(WzVector));
        return static_cast<CPathExplorerD*>(pPathExp)->m_wvTarget;
    }
    static const DWORD& P_GetMovingStatus(CPathExplorer* const pPathExp) {
        BOOST_STATIC_ASSERT(sizeof(static_cast<CPathExplorerD*>(pPathExp)->m_dwMovingStatus) ==
                            sizeof(DWORD));
        return static_cast<CPathExplorerD*>(pPathExp)->m_dwMovingStatus;
    }
    // isMoving(T) == m_dwMovingStatus != PEMT_STANDING
};

}; // end namespace nsAI


namespace nsAI
{

//--------------------------------------------------------------------------------------------------
class PlanEventObject
{
    friend class MovingPlanner;
    friend class IPlanEventObject;
    typedef IPlanEventObject        IPEO;

public:
    typedef IPlanEventObject::EventField_t  EventField_t;

    PlanEventObject(EventField_t& rEventField,
                    CPathExplorer*& pPathExplorer,
                    SkillManager*& pSkillManager,
                    StatusManager*& pStatusManager);

    BOOLEAN EventCheck();
    BOOLEAN Update(const DWORD delta_tick, const BOOLEAN need_AI_proc_state);

    // default can't accept tile attributes = { PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE }
    static BOOLEAN AcceptableTarget(Character* const pTarget,
                                    WORD add_cant_accept_tile_attr, // add can't accept tile attributes
                                    WORD sub_cant_accept_tile_attr);// sub can't accept tile attributes
    // (CHANGES) (091213) add easy access interface
    BOOLEAN GetTileEvent(const TileEvent** OUT ppTileEvent) const;
    // (CHANGES) (091213) add corresponding interface which is SUN3DEngine::CPathExplorer::GetTile
    int GetTile() const;

private:
    void Init(Character* const pCharacter);
    void Release();

    // internal functions
    BOOLEAN _UpdatePlayer_Move(const DWORD delta_tick, MovingPlanner* const pMovingPlanner);
    BOOLEAN _UpdateMonster_Move(const DWORD delta_tick, MovingPlanner* const pMovingPlanner);
    BOOLEAN _UpdateMonster_State(const DWORD delta_tick, MovingPlanner* const pMovingPlanner);

public:
    enum eEventRet { eEventRet_SkipProgressFail, eEventRet_SkipProgress, eEventRet_NeedProgress, };
private:
    eEventRet _UpdateEvent(const DWORD delta_tick,
                           MovingPlanner* const pMovingPlanner,
                           const BOOLEAN bCheckChangeEvent,
                           const int iTile,
                           const BOOLEAN bSameTile); // local == cur
    // Fields...
    EventField_t&   m_rEventField;
    GameField*      m_pGameFieldSync;
    Character*      m_pCharacter;
    SkillManager*&      m_pSkillManager;
    StatusManager*&     m_pStatusManager;
    TargetManager*      m_pTargetManager;   // for NPC, no Ref;
    NPCStateManager*    m_pStateManager;    // for NPC, no Ref;
    CPathExplorer*&     m_pPathExplorer;
    CPathExplorerD::PathExpD m_PathExpWrap;
    const TileEvent*    m_pTileEventInField; // (NOTE) latest updated after progress event
    TileEvent   m_TileEventInfo;
    ITimeout    m_DangerTimer;
    BYTE        m_DangerSkipLevel;  // 3,2,1,0

    __DISABLE_COPY(PlanEventObject);
};

//--------------------------------------------------------------------------------------------------

inline BOOLEAN
PlanEventObject::AcceptableTarget(Character* const pTarget,
                                  WORD add_cant_accept_tile_attr,
                                  WORD sub_cant_accept_tile_attr)
{
    static DWORD shared_idx = 0;

    PlanEventObject* const pPlanEventObject = pTarget->PlanEvent.m_pHandler;
    const nsAI::TileEvent* pTileEvent = pPlanEventObject->m_pTileEventInField;
    // (BUGFIX) (091211) (WAVERIX) fix up mismatched type
    pTileEvent = (pTileEvent->Index == pTileEvent->InvalidTileIndex) ? 0 : pTileEvent;

    const WORD FilterSet = PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE;
    add_cant_accept_tile_attr |= FilterSet;
    // (CHANGES) (100122) (WAVERIX) fix up conflicted rules that MapNpc can attacked in domination field
    add_cant_accept_tile_attr = add_cant_accept_tile_attr & ~sub_cant_accept_tile_attr;
    const BOOLEAN isOkTile = pTileEvent && !(pTileEvent->Attr & add_cant_accept_tile_attr);
    const BOOLEAN isSafe   = pTileEvent && !(pTileEvent->Cost > nsAI::eWeight_Level_Heavy);
    //  0   0   -- no
    //  0   1   -- x
    //  1   0   -- ratio
    //  1   1   -- yes
    if(!(isOkTile & isSafe))
    {
        const BOOLEAN need_ratio = isOkTile > isSafe;
        if(!need_ratio)
            return FALSE;

        shared_idx = (shared_idx + 1) & (32 - 1);
        // to support minor randomizing (for AI)
        if((shared_idx & 5) != 5)
            return FALSE;
    }

    return TRUE;
}

// (CHANGES) (091213) add easy access interface
inline BOOLEAN
PlanEventObject::GetTileEvent(const TileEvent** OUT ppTileEvent) const
{
    // (WARNING) need check pCharacter->GetField
    // (NOTE) m_pTileEventInField�� ���� progressing�� ����� ��, PathPlan_Command�� ���� ���ŵȴ�.
    // Character::GetTileAttr�� �����ϱ� ���� ó���̴�.
    const nsAI::TileEvent* pTileEvent = m_pTileEventInField;
    pTileEvent = (pTileEvent->Index == pTileEvent->InvalidTileIndex) ? 0 : pTileEvent;
    *ppTileEvent = pTileEvent;

    return !!pTileEvent;
}

// (CHANGES) (091213) add corresponding interface which is SUN3DEngine::CPathExplorer::GetTile
inline int
PlanEventObject::GetTile() const
{
    return this->m_PathExpWrap.riTileCur;
}


}; // end namespace nsAI

