#include "stdafx.h"

#include <limits>
#include <3DEngine/3DTerrain/RoomManager.h>
#include <PacketControl/WarMessages_Sync.h>

#include "MovingPlanner.h"
#include "Character.h"
#include "Player.h"
#include "GameServerEx.h"
#include "MoveStateControl.h"
#include "StatusManager.h"
#include <NpcAI/NPCStateManager.h>
#include <TargetManager.h>
#include <World/GameWarControlManager.h>
#include <PublicMath.h>

//==================================================================================================

namespace nsAI
{

//==================================================================================================

static const TileEvent NullTileEvent =
{
    TileEvent::eTile_NotUpdated,    // Ctrl
    USHRT_MAX,                      // Index
    0,                              // Attr
    eWeight_None,                   // Cost
    0,                              // Event
    { USHRT_MAX, USHRT_MAX, USHRT_MAX } // NearNodes
};

//--------------------------------------------------------------------------------------------------
//  <Description>
//  -   각 Field 인스턴스마다 존재하는 타일 속성 기반 컨트롤 정보 배열
struct TileArray
{
public:
    const   WORD        Size;
    TileEvent*          m_pBegin;
    TileEvent*          m_pEnd;

public:
    TileArray()
        : Size(0)
        , m_pBegin(0)
        , m_pEnd(0)
    {}

    TileArray(const WORD count)
        : Size(count)
        , m_pBegin((TileEvent*)TAllocMalloc(count * sizeof(TileEvent)))
        , m_pEnd(m_pBegin+Size)
    {
        Clear();
    }

    ~TileArray()
    {
        if(m_pBegin)
        {
            TAllocFree(m_pBegin);
            m_pBegin = NULL;
            m_pEnd = NULL;
        }
    }

    inline void Clear()
    {
        TileEvent* pIt = m_pBegin;
        TileEvent* pItEnd = m_pBegin + Size;
        for(; pIt != pItEnd; ++pIt)
            *pIt = NullTileEvent;
    }

    inline void GetTileEvent(const int iTileNo, TileEvent& OUT rTileEvent) const;
    inline void SetTileEvent(const int iTileNo, const TileEvent& IN rTileEvent);
    //
    __DISABLE_COPY(TileArray);
};


#define CHECK_TILE_UPPERBOUND(_pArr, _iTile)        \
    ((m_pGameField) && !(m_RoomIndex < 0) && (_pArr) && ((DWORD)(_iTile) < (DWORD)(_pArr)->Size))

#define GET_TILE_NODE(_pArr, _iTile)    (*(((_pArr)->m_pBegin) + (_iTile)))
#define SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(_room_idx)               \
    {   if(m_pSharedFieldInfo->m_LastUsedRoomIndex != m_RoomIndex)  \
        m_pRoomManager->SetActiveRoom(m_pSharedFieldInfo->m_LastUsedRoomIndex = m_RoomIndex);   \
    }

#define SET_TILE_UPDATE_ARGS(_rTileNode, _attr_, _tileno_, _cost_, _event_, _ctrl_)  \
    {   (_rTileNode).Attr   = (_attr_);                    \
        (_rTileNode).Index  = (_tileno_);                  \
        (_rTileNode).Cost   = (_cost_);                    \
        (_rTileNode).Event  = (_event_);                   \
        (_rTileNode).Ctrl   |=(_ctrl_);                    \
        _UpdateNearNodes(_tileno_, (_rTileNode).NearNodes);\
    }


//  no bound check
inline void
TileArray::GetTileEvent(const int iTileNo, TileEvent& OUT rTileEvent) const
{
    if (m_pBegin)
    {
        rTileEvent = m_pBegin[iTileNo];
    }
}

//--------------------------------------------------------------------------------------------------─────

//  no bound check
inline void
TileArray::SetTileEvent(const int iTileNo, const TileEvent& IN rTileEvent)
{
    if (m_pBegin)
    {
        m_pBegin[iTileNo] = rTileEvent;
    }
}


//==================================================================================================
//  <MovingPlanner>
//  -   CWorldBase + CPathFinderBase 통합 형태, 서버에서 사용하는 것들만 (이동/정보 관련) 추출 래퍼
//  -   목적 : 이동형 Object들에 대한 이동에 대한 중간 처리 목적

//--------------------------------------------------------------------------------------------------


MovingPlanner::MovingPlanner(SharedFieldInfo* const pSharedFieldInfo, const int room_index)
    : m_pSharedFieldInfo(pSharedFieldInfo)
    , m_pWorldBase(pSharedFieldInfo->m_pWorldBase)
    , m_pPathFinder(m_pWorldBase->x_pPathFinder)
    , m_RoomIndex(room_index)
    , m_pRoomManager(m_pWorldBase->x_pRoomManager)
    , m_pGameField(0)
    , m_pPathExplorer((CPathExplorer*)g_pGameServer->m_fnContactTerrain(CT_NEW_PATH_EXPLORER, 0))
    , m_LastUpdate(0)
    , m_pTileArray(0)
    // TODO: thinking using method m_pPathExplorer. _0_20100427_DOMINATION_FINDPATH_ATTACK_BUG
    , m_pTempPathPlanAttack((CPathExplorer*)g_pGameServer->m_fnContactTerrain(CT_NEW_PATH_EXPLORER, 0))
{
    m_pTileArray = TAllocNew(TileArray);

    const int max_tiles = m_pPathFinder->GetNumPathTile();
    ASSERT(SAFE_NUMERIC_TYPECAST(int, max_tiles, WORD));

    new (m_pTileArray) TileArray((WORD)max_tiles);

    //  for source and target Divide&Conquer Searching
    m_TempFlag.Create(max_tiles);
}


//--------------------------------------------------------------------------------------------------


MovingPlanner::~MovingPlanner()
{
    Release();
}


//--------------------------------------------------------------------------------------------------


void
MovingPlanner::Init(GameField* const pGameField)
{
    m_pGameField = pGameField;
    //m_pTileArray->Clear();
}


//--------------------------------------------------------------------------------------------------


void
MovingPlanner::Release()
{
    if (m_pPathExplorer) {
        g_pGameServer->m_fnContactTerrain(CT_DELETE_PATH_EXPLORER, m_pPathExplorer);
        m_pPathExplorer = NULL;
    };
    if (m_pTempPathPlanAttack) {
        g_pGameServer->m_fnContactTerrain(CT_DELETE_PATH_EXPLORER, m_pTempPathPlanAttack);
        m_pTempPathPlanAttack = NULL;
    };
    if (m_pTileArray) {
        TAllocDelete(TileArray, m_pTileArray);
        m_pTileArray = NULL;
    };
    m_TempFlag.~BitSetOperationV();
    new (this) MovingPlanner();
}


//--------------------------------------------------------------------------------------------------


static const WzVector NullVector = { .0f, .0f, .0f };

struct CacheCmd
{
    enum eCmd {
        eCmd_TileAttr   = 0,
    };
    BYTE        Cmd;
    int         iTileNo;
};

//--------------------------------------------------------------------------------------------------

inline void
MovingPlanner::_UpdateNearNodes(const WORD tileNo, WORD OUT nearNodes[3])
{
    int query_data[3] = { -1, -1, -1 };
    m_pPathFinder->GetNearNodes(tileNo, query_data, 3);
    nearNodes[0] = (WORD)query_data[0];
    nearNodes[1] = (WORD)query_data[1];
    nearNodes[2] = (WORD)query_data[2];
}


//--------------------------------------------------------------------------------------------------


WzVector
MovingPlanner::GetRandomPosInTile(const int iTile)
{
    if(CHECK_TILE_UPPERBOUND(m_pTileArray, iTile))
    {
        TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, iTile);
        const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

        if(!bUpdated)
        {
            SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
            SET_TILE_UPDATE_ARGS(rNode,
                                 m_pPathFinder->GetAttribute(iTile),
                                 (WORD)iTile,
                                 eWeight_None,
                                 eCharEvent_None,
                                 rNode.eTile_Updated);
        }

        return m_pPathFinder->GetRandomPosInTile(iTile);
    }

    return NullVector;
}

void
MovingPlanner::GetRandomPosInTile(const int iTile, WzVector* OUT pPos) // (WAVERIX) (090708) (ADDIN)
{
    if(CHECK_TILE_UPPERBOUND(m_pTileArray, iTile))
    {
        TileEvent& rNode = GET_TILE_NODE(m_pTileArray, iTile);
        const BOOLEAN bUpdated = !!(rNode.Ctrl & rNode.eTile_Updated);

        if(!bUpdated)
        {
            SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
            SET_TILE_UPDATE_ARGS(rNode,
                                 m_pPathFinder->GetAttribute(iTile),
                                 (WORD)iTile,
                                 eWeight_None,
                                 eCharEvent_None,
                                 rNode.eTile_Updated);
        }

        *pPos = m_pPathFinder->GetRandomPosInTile(iTile);
        return;
    }

    *pPos = NullVector;
}


//--------------------------------------------------------------------------------------------------


int
MovingPlanner::GetNumPathTile()
{
    return m_pTileArray->Size;
}


//--------------------------------------------------------------------------------------------------


void
MovingPlanner::SetAttribute(const int iTile, const WORD wAttribute)
{
    if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iTile))
        return;

    TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, iTile);
    const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

    if(!bUpdated)
    {
        SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
        SET_TILE_UPDATE_ARGS(rNode,
                             m_pPathFinder->GetAttribute(iTile),
                             (WORD)iTile,
                             eWeight_None,
                             eCharEvent_None,
                             rNode.eTile_Updated);
    }

    const BOOLEAN bNotChecked = !(wAttribute & rNode.Attr);

    //  bUpdated | bNotChecked      1st             PostAction
    //      0           0           GetAttr         None    None
    //      0           1           GetAttr         Set()   SetAttr()
    //      1           0           None            None    None
    //      1           1           None            Set()   SetAttr()
    if(bNotChecked)
    {
        rNode.Attr |= wAttribute;
        //if(bNotChecked > bUpdated)    <- (WAVERIX) (081120) (BUG-FIX)
        SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
        m_pPathFinder->SetAttribute(iTile, wAttribute);
    }
}


//--------------------------------------------------------------------------------------------------


void
MovingPlanner::UnsetAttribute(const int iTile, const WORD wAttribute)
{
    if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iTile))
        return;

    TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, iTile);
    const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

    if(!bUpdated)
    {
        SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
        SET_TILE_UPDATE_ARGS(rNode,
                             m_pPathFinder->GetAttribute(iTile),
                             (WORD)iTile,
                             eWeight_None,
                             eCharEvent_None,
                             rNode.eTile_Updated);
    }

    const BOOLEAN bChecked = !!(wAttribute & rNode.Attr);

    //  bUpdated | bChecked         1st             PostAction
    //      0           0           GetAttr         None    None
    //      0           1           GetAttr         UnSet() UnSetAttr()
    //      1           0           None            None    None
    //      1           1           None            UnSet() UnSetAttr()
    if(bChecked)
    {
        rNode.Attr &= ~wAttribute;
        //if(bChecked > bUpdated) <- (WAVERIX) (081120) (BUG-FIX)
        SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
        m_pPathFinder->UnsetAttribute(iTile, wAttribute);
    }
}


//--------------------------------------------------------------------------------------------------


BOOLEAN
MovingPlanner::CheckAttribute(const int iTile, const WORD wAttribute)
{
    if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iTile))
        return FALSE;

    TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, iTile);
    const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

    if(!bUpdated)
    {
        SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
        SET_TILE_UPDATE_ARGS(rNode,
                             m_pPathFinder->GetAttribute(iTile),
                             (WORD)iTile,
                             eWeight_None,
                             eCharEvent_None,
                             rNode.eTile_Updated);
    }

    return !!(rNode.Attr & wAttribute);
}


//--------------------------------------------------------------------------------------------------


WORD
MovingPlanner::GetAttribute(const int iTile)
{
    if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iTile))
        return 0;

    TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, iTile);
    const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

    if(!bUpdated)
    {
        SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
        SET_TILE_UPDATE_ARGS(rNode,
                             m_pPathFinder->GetAttribute(iTile),
                             (WORD)iTile,
                             eWeight_None,
                             eCharEvent_None,
                             rNode.eTile_Updated);
    }

    return rNode.Attr;
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface x_pPathFinder
int
MovingPlanner::GetTileToStand(const WzVector& rPos,
                              float* pfHeight, const float fJumpLimit, const float fAdjustLimit,
                              WORD* const pwAttribute)
{
    SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);

    WORD intercept_attr = 0;    // pwAttribute가 null일 수 있으므로...

    const int iTile = m_pPathFinder->GetTileToStand(rPos, pfHeight, fJumpLimit, fAdjustLimit, &intercept_attr);
    if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iTile))
        return -1;

    TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, iTile);
    const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

    if(!bUpdated)
    {
        SET_TILE_UPDATE_ARGS(rNode,
                             intercept_attr,
                             (WORD)iTile,
                             eWeight_None,
                             eCharEvent_None,
                             rNode.eTile_Updated);
    }
    if(pwAttribute)
        *pwAttribute = rNode.Attr;

    return iTile;
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface x_pPathFinder
int
MovingPlanner::GetAllowTileToStand(const WzVector& rPos,
                                   float* pfHeight, const float fJumpLimit, const float fAdjustLimit,
                                   WORD* const pwAttribute,
                                   const WORD wNotAllowAttribute)
{
    SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);

    WORD intercept_attr = 0;    // pwAttribute가 null일 수 있으므로...
    const int iTile = m_pPathFinder->GetTileToStand(rPos, pfHeight, fJumpLimit, fAdjustLimit,
                                                    &intercept_attr);
    if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iTile))
        return -1;

    TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, iTile);
    const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

    if(!bUpdated)
    {
        SET_TILE_UPDATE_ARGS(rNode,
                             intercept_attr,
                             (WORD)iTile,
                             eWeight_None,
                             eCharEvent_None,
                             rNode.eTile_Updated);
    }

    if(wNotAllowAttribute & rNode.Attr)
        return -1;

    if(pwAttribute)
        *pwAttribute = rNode.Attr;

    return iTile;
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface < 3D 좌표를 줘서 초기화 (포인터 형)
BOOLEAN
MovingPlanner::PathExp_Reset(Character* const pMovingObject,
                             const WzVector* IN pwvPos, const int iTile)
{
    SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);

    WORD intercept_attr = 0;
    const int iCurTile = m_pPathFinder->GetTileToStand(*pwvPos, 0, 0.0f, -1.0f, &intercept_attr);
    if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iCurTile))
        return FALSE;

    TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, iCurTile);
    const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

    if(!bUpdated)
    {
        SET_TILE_UPDATE_ARGS(rNode,
                             intercept_attr,
                             (WORD)iTile,
                             eWeight_None,
                             eCharEvent_None,
                             rNode.eTile_Updated);
    }

    CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();

    WzVector pos = *pwvPos;
    m_pWorldBase->PathExp_Reset(pPathExplorer, &pos, iCurTile);

    return TRUE;
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface < 길 찾기
ENUM_FINDPATH_RESULT
MovingPlanner::PathExp_FindPath(Character* const pMovingObject,
                                const WzVector& IN rwvDest,
                                const int iDestTile, const WORD wAttribute, const int iSeekNode)
{
    SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);

    //WORD intercept_attr = 0;
    //const int iCurTile = m_pPathFinder->GetTileToStand(*pwvPos, 0, 0.0f, -1.0f, &intercept_attr);
    //if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iCurTile))
    //  return FALSE;

    //TileEvent& rNode = GET_TILE_NODE(m_pTileArray, iCurTile);

    //const BOOLEAN bUpdated = !!(rNode.Ctrl & rNode.eTile_Updated);
    //if(!bUpdated)
    //{
    //  rNode.Attr = intercept_attr;
    //  rNode.Ctrl |= rNode.eTile_Updated;
    //}


    CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();
    return m_pWorldBase->PathExp_FindPath(pPathExplorer, rwvDest, iDestTile, wAttribute, iSeekNode);
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface < 경로 지정
BOOLEAN
MovingPlanner::PathExp_SetPath(Character* const pMovingObject,
                               const WzVector* IN pwvDest,
                               int* const pTileRoute, const int iNumTileRoute)
{
    SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);

    WzVector pos = *pwvDest;

    CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();
    return (m_pWorldBase->PathExp_SetPath(pPathExplorer, &pos, pTileRoute, iNumTileRoute) != false);
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface < 길 찾기 진행
BOOLEAN
MovingPlanner::PathExp_Progress(Character* const pMovingObject,
                                const float fMove,
                                WzPathFindEvent* pEvent,
                                ENUM_CHECK_COLLISION eCC)  //< 길 찾기 진행 (좌표 이동이 있었으면 TRUE)
{
    SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);

    CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();
    return (m_pWorldBase->PathExp_Progress(pPathExplorer, fMove, pEvent, eCC) != false);
    //PathExp_Progress(CPathExplorer *ppe, float fMove, WzPathFindEvent *pEvent = NULL, ENUM_CHECK_COLLISION eCC = ECC_NO) = 0; ///< 길 찾기 진행 (좌표 이동이 있었으면 TRUE)
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface < 특정 방향으로 특정 거리만큼 밀기
ENUM_FINDPATH_RESULT
MovingPlanner::PathExp_Thrust(Character* const pMovingObject,
                              const WzVector* IN pwvMoveDistance,
                              const WORD wAttribute)
{
    WzVector pos = *pwvMoveDistance;

    CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();
    return m_pWorldBase->PathExp_Thrust(pPathExplorer, &pos, wAttribute);
    //PathExp_Thrust(CPathExplorer *ppe, WzVector *pwvMoveDistance, WORD wAttribute = (PTA_ONLY_JUMP | PTA_NO_WALK)) = 0;   ///< 특정 방향으로 특정 거리만큼 밀기 
}


//--------------------------------------------------------------------------------------------------─────
//--------------------------------------------------------------------------------------------------─────
//--------------------------------------------------------------------------------------------------─────


BOOLEAN
MovingPlanner::PathPlan_Command(const PlanCmd& rCmd, const TileEvent** OUT ppTileEvent)
{
    switch(rCmd.Cmd & PlanCmd::eCmdTypeMask)
    {
    case PlanCmd::eTileWeightCmd:
        return _PathPlan_TileWeight(static_cast<const PlanCmd::TileWeightArg&>(rCmd), ppTileEvent);
    case PlanCmd::eNpcAreaEvent:
        return _PathPlan_NpcAreaEvent(static_cast<const PlanCmd::NpcAreaEvent&>(rCmd));
    }

    return FALSE;
}

//--------------------------------------------------------------------------------------------------─────

//  Tile Weight Cost Operation  < 특정 타일에 접근 꺼려지는 가중치 처리
BOOLEAN
MovingPlanner::_PathPlan_TileWeight(const PlanCmd::TileWeightArg& IN rArg,
                                    const TileEvent** OUT ppTileEvent)
{
    if(!CHECK_TILE_UPPERBOUND(m_pTileArray, rArg.TileNo))
        return FALSE;

    TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, rArg.TileNo);
    const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

    if(!bUpdated)
    {
        SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
        SET_TILE_UPDATE_ARGS(rNode,
                             m_pPathFinder->GetAttribute(rArg.TileNo),
                             (WORD)rArg.TileNo,
                             eWeight_None,
                             eCharEvent_None,
                             rNode.eTile_Updated);
    }

    switch(rArg.Cmd)
    {
    case PlanCmd::eTileWeight_SetWeight:
        {
            const BOOLEAN bCurHeavy = eWeight_Level_Heavy < rNode.Cost;
            if(bCurHeavy)
            {
                if((eWeight_None == rArg.Weight) || (rNode.Cost < rArg.Weight))
                    rNode.Cost = rArg.Weight;
            }
            else
            {
                rNode.Cost = rArg.Weight;
            }
        }
        break;
    case PlanCmd::eTileWeight_GetTileEvent:
        {
            if(ppTileEvent)
                *ppTileEvent = &rNode;
        }
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

bool MovingPlanner::_PathPlan_NpcAreaEvent(const PlanCmd::NpcAreaEvent& IN arg)
{
    if (!CHECK_TILE_UPPERBOUND(m_pTileArray, arg.TileNo)) {
        return false;
    };
    TileEvent& tile_event = GET_TILE_NODE(m_pTileArray, arg.TileNo);
    const bool updated = !!(tile_event.Ctrl & tile_event.eTile_Updated);

    if (updated == false)
    {
        SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
        SET_TILE_UPDATE_ARGS(tile_event,
                             m_pPathFinder->GetAttribute(arg.TileNo),
                             static_cast<uint16_t>(arg.TileNo),
                             eWeight_None,
                             eCharEvent_None,
                             tile_event.eTile_Updated);
    }

    switch (arg.Cmd)
    {
    case PlanCmd::eNpcAreaEvent_Regist:
        tile_event.Event |= arg.CharEvent;
        break;
    case PlanCmd::eNpcAreaEvent_Unregist:
        tile_event.Event &= ~arg.CharEvent;
        break;
    case PlanCmd::eNpcAreaEvent_Get:
        assert(!"currently not supported");
        break;
    default:
        return false;
    }

    return true;
}

//  Wrapper interface < 3D 좌표를 줘서 초기화 (포인터 형)
BOOLEAN
MovingPlanner::PathPlan_Reset(Character* const pMovingObject,
                              const WzVector* IN pwvPos, const int iTile)
{
    __TOUCHED(iTile);
    SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);

    WORD intercept_attr = 0;
    // (BUGFIX) (WAVERIX) (090218) SetPos의 부적절한 사용 문제 해결을 위해 두 번째 인자 수정
    float fHeight;
    const int iCurTile = m_pPathFinder->GetTileToStand(*pwvPos, &fHeight,
                                                       -1.0f, 100.0f, &intercept_attr);
    if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iCurTile))
        return FALSE;

    TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, iCurTile);
    const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

    if(!bUpdated)
    {
        SET_TILE_UPDATE_ARGS(rNode,
                             intercept_attr,
                             (WORD)iCurTile,
                             eWeight_None,
                             eCharEvent_None,
                             rNode.eTile_Updated);
    }

    IPlanEventObject& rPlanEvent = pMovingObject->PlanEvent;
    if(rPlanEvent.EventField & rPlanEvent.MOVE_HOLDING)
        rPlanEvent.RemoveField(rPlanEvent.MOVE_HOLDING);

    CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();

    WzVector pos = *pwvPos;
    // (BUGFIX) (090218) (WAVERIX) SetPos의 부적절한 사용 문제 해결
    if(fabs(fHeight) > 0.01f)
        pos.z = pos.z - fHeight;
    m_pWorldBase->PathExp_Reset(pPathExplorer, &pos, iCurTile);

    return TRUE;
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface < 길 찾기
ENUM_FINDPATH_RESULT
MovingPlanner::PathPlan_FindPath(Character* const pMovingObject,
                                 const WzVector& IN rwvDest,
                                 const int iDestTile,
                                 const WORD wAttribute,
                                 const int iSeekNode)
{
    SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);

    //WORD intercept_attr = 0;
    //const int iCurTile = m_pPathFinder->GetTileToStand(*pwvPos, 0, 0.0f, -1.0f, &intercept_attr);
    //if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iCurTile))
    //  return FALSE;

    //TileEvent& rNode = GET_TILE_NODE(m_pTileArray, iCurTile);

    //const BOOLEAN bUpdated = !!(rNode.Ctrl & rNode.eTile_Updated);
    //if(!bUpdated)
    //{
    //  rNode.Attr = intercept_attr;
    //  rNode.Ctrl |= rNode.eTile_Updated;
    //}


    CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();
    return m_pWorldBase->PathExp_FindPath(pPathExplorer, rwvDest, iDestTile, wAttribute, iSeekNode);
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface < 경로 지정
BOOLEAN
MovingPlanner::PathPlan_SetPath(Character* const pMovingObject,
                                const WzVector* IN pwvDest,
                                int* const pTileRoute, const int iNumTileRoute)
{
    SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);

    WzVector pos = *pwvDest;

    CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();
    return (m_pWorldBase->PathExp_SetPath(pPathExplorer, &pos, pTileRoute, iNumTileRoute) != false);
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface < 길 찾기 진행
BOOLEAN
MovingPlanner::PathPlan_Progress(Character* const pMovingObject,
                                 const float fMove,
                                 WzPathFindEvent* pEvent,
                                 ENUM_CHECK_COLLISION eCC)  //< 길 찾기 진행 (좌표 이동이 있었으면 TRUE)
{
    SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);

    CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();
    return (m_pWorldBase->PathExp_Progress(pPathExplorer, fMove, pEvent, eCC) != false);
    //PathExp_Progress(CPathExplorer *ppe, float fMove, WzPathFindEvent *pEvent = NULL, ENUM_CHECK_COLLISION eCC = ECC_NO) = 0; ///< 길 찾기 진행 (좌표 이동이 있었으면 TRUE)
}


//--------------------------------------------------------------------------------------------------

//  Wrapper interface < 특정 방향으로 특정 거리만큼 밀기
ENUM_FINDPATH_RESULT
MovingPlanner::PathPlan_Thrust(Character* const pMovingObject,
                               const WzVector* IN pwvMoveDistance,
                               const WORD wAttribute)
{
    WzVector pos = *pwvMoveDistance;

    CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();
    return m_pWorldBase->PathExp_Thrust(pPathExplorer, &pos, wAttribute);
    //PathExp_Thrust(CPathExplorer *ppe, WzVector *pwvMoveDistance,
    //               WORD wAttribute = (PTA_ONLY_JUMP | PTA_NO_WALK)) = 0;
    ///< 특정 방향으로 특정 거리만큼 밀기 
}

ENUM_FINDPATH_RESULT
MovingPlanner::PathPlan_MoveCheck(Character* const pMovingObject,
                                  const WzVector& IN rwvDest,
                                  const int iDestTile,
                                  const WORD wAttribute,
                                  const int iSeekNode)
{
//     스킬 성공시 목적지로 이동 방지.. (pTemp) 부하가 많이 걸리므로 주석처리함.
//     목적지로 가지 않는 버전 엔진에서 지원 필요 [12/17/2009 lst1024]
//     점령전 에서만 사용.
//     단순히 이동 가능인지의 체크를 필요로 하나, PathExp_FindPath의 첫번째 파라메터를 변경.
//     케릭터의 CPathExplorer를 변경하여 이미 위치로 이동 되어 버린다.
//     일단은 PathExp_FindPath 의 부하와 memcpy의 부하만 가지고 간다.

    //_0_20100427_DOMINATION_FINDPATH_ATTACK_BUG
    memcpy(m_pTempPathPlanAttack, pMovingObject->GetPathExplorer(), sizeof(CPathExplorer));

    ENUM_FINDPATH_RESULT retVal = m_pWorldBase->PathExp_FindPath(m_pTempPathPlanAttack, rwvDest, 
                                                                 iDestTile, wAttribute, iSeekNode);
    return retVal;
}

}; // end namespace nsAI


// CHANGES: f110823.3L, added to support fast skill moving action.
bool nsAI::MovingPlanner::PathPlan_FastMoved(
    Character* const moving_object, const WzVector& IN dest_pos, const WORD tile_attribute,
    WzVector* const result_moved_pos)
{
    CopyMemory(m_pTempPathPlanAttack, moving_object->GetPathExplorer(),
        sizeof(*m_pTempPathPlanAttack));

    WzVector calc_dest_pos = dest_pos;
    const WzVector* explorer_pos = m_pTempPathPlanAttack->GetPosPtr();
    float distance = Math_VectorLength(*explorer_pos, calc_dest_pos);
    calc_dest_pos -= (*explorer_pos); // CAUTION!!!
    
    ENUM_FINDPATH_RESULT find_path_result = \
        m_pWorldBase->PathExp_Thrust(m_pTempPathPlanAttack, &calc_dest_pos, tile_attribute);
    if (find_path_result == EFR_FAILURE ||
        find_path_result == EFR_SAME_POSITION)
    {
        if (result_moved_pos) {
            *result_moved_pos = *explorer_pos;
        };
        return false;
    };
    bool result = (m_pWorldBase->PathExp_Progress(m_pTempPathPlanAttack, \
        distance, NULL, ECC_NO) != false);
    if (result_moved_pos) {
        *result_moved_pos = *explorer_pos;
    };
    return result;
};
//==================================================================================================


namespace nsAI
{


inline const TileEvent&
MovingPlanner::_GetTileEvent(const int iTile)
{
    if(!CHECK_TILE_UPPERBOUND(m_pTileArray, iTile))
        return NullTileEvent;

    TileEvent& rNode        = GET_TILE_NODE(m_pTileArray, iTile);
    const BOOLEAN bUpdated  = !!(rNode.Ctrl & rNode.eTile_Updated);

    if(!bUpdated)
    {
        SET_ACTIVE_ROOM_SHAREDFIELD_UPDATE(m_RoomIndex);
        SET_TILE_UPDATE_ARGS(rNode,
                             m_pPathFinder->GetAttribute(iTile),
                             (WORD)iTile,
                             eWeight_None,
                             eCharEvent_None,
                             rNode.eTile_Updated);
    }
    return rNode;
}

const DWORD PTA_SIZE = 3 * 1 + 3 * 2 + 3 * 4 + 3 * 8;
static DWORD s_aligned_no = 0;
static WORD  s_pFTA[PTA_SIZE];


inline void
MovingPlanner::_PathPlan_NearNodeRandomSort(TileEvent& OUT rOutTileEvent,
                                            const TileEvent& rInSrcTileEvent)
{
    static DWORD round_v = 4;
    rOutTileEvent = rInSrcTileEvent;

    //switch(round_v)
    //{
    //case 0:       break;
    //case 1:
    //  {
    //      const WORD backup = rOutTileEvent.NearNodes[0];
    //      rOutTileEvent.NearNodes[0] = rOutTileEvent.NearNodes[1];
    //      rOutTileEvent.NearNodes[1] = rOutTileEvent.NearNodes[2];
    //      rOutTileEvent.NearNodes[2] = backup;
    //  }
    //  break;
    //case 2:
    //  {
    //      const WORD backup = rOutTileEvent.NearNodes[2];
    //      rOutTileEvent.NearNodes[2] = rOutTileEvent.NearNodes[1];
    //      rOutTileEvent.NearNodes[1] = rOutTileEvent.NearNodes[0];
    //      rOutTileEvent.NearNodes[0] = backup;
    //  }
    //  break;
    //case 3:
    //  {
    //      const WORD backup = rOutTileEvent.NearNodes[1];
    //      rOutTileEvent.NearNodes[1] = rOutTileEvent.NearNodes[0];
    //      rOutTileEvent.NearNodes[0] = rOutTileEvent.NearNodes[2];
    //      rOutTileEvent.NearNodes[2] = backup;
    //  }
    //  break;
    //case 4:
    //  {
    //      const WORD backup = rOutTileEvent.NearNodes[2];
    //      rOutTileEvent.NearNodes[2] = rOutTileEvent.NearNodes[0];
    //      rOutTileEvent.NearNodes[0] = rOutTileEvent.NearNodes[1];
    //      rOutTileEvent.NearNodes[1] = backup;
    //  }
    //  break;
    //case 5:
    //  {
    //      const WORD backup = rOutTileEvent.NearNodes[2];
    //      rOutTileEvent.NearNodes[2] = rOutTileEvent.NearNodes[0];
    //      rOutTileEvent.NearNodes[0] = rOutTileEvent.NearNodes[1];
    //      rOutTileEvent.NearNodes[1] = backup;
    //  }
    //  break;
    //}

    std::random_shuffle(&rOutTileEvent.NearNodes[0], &rOutTileEvent.NearNodes[3]);

    (round_v)
        ?   --round_v
        :   round_v = 4
        ;
}

WORD/*TileNo*/
MovingPlanner::_SearchValidNearNode(const TileEvent& rTileEvent,
                                    const DWORD depth, const WORD cost)
{
    const WORD* pIt     = &rTileEvent.NearNodes[0];
    const WORD* pItEnd  = &rTileEvent.NearNodes[3];

    const TileEvent* pTileEvents[3] = { 0, };
    const TileEvent** pTileEventIt = pTileEvents;

    for(; pIt != pItEnd ; ++pIt)
    {
        const WORD nearTile = *pIt;
        if(nearTile == rTileEvent.InvalidTileIndex)
            continue;

        const BOOLEAN cant_enter = m_TempFlag[nearTile];
        if(cant_enter)
            continue;

        const TileEvent& rTileEvent = _GetTileEvent(nearTile);
        if(rTileEvent.Index == rTileEvent.InvalidTileIndex)
            continue;

        //  valid index
        if((rTileEvent.Attr & (PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE)))
        {
            m_TempFlag.Set(nearTile);
            continue;
        }

        if(rTileEvent.Cost < cost)
            return nearTile;

        *pTileEventIt++ = &rTileEvent;
    }

    if(depth < 3)
    {
        pTileEventIt = pTileEvents;
        for(; pTileEventIt != &pTileEvents[3]; ++pTileEventIt)
        {
            if(!(*pTileEventIt))
                break;
            const TileEvent& rTileEvent = **pTileEventIt;
            if(rTileEvent.Index == rTileEvent.InvalidTileIndex)
                continue;

            TileEvent cloneEvent;
            _PathPlan_NearNodeRandomSort(cloneEvent, rTileEvent);

            const WORD finded_tile = _SearchValidNearNode(cloneEvent, depth + 1, rTileEvent.Cost);
            if(finded_tile != rTileEvent.InvalidTileIndex)
                return finded_tile;
        }
    }

    return rTileEvent.InvalidTileIndex;
}


BOOLEAN
MovingPlanner::_PathPlan_FindPathToSafeZone(NPC* const pNPC,
                                            PlanEventObject& rPlanEvent,
                                            const eFindSafezoneReason reason)
{
    __TOUCHED((pNPC, reason));
    PlanEventObject::EventField_t& rEventField = rPlanEvent.m_rEventField;

    const BOOLEAN raiseEscapeEvent = (rEventField & IPlanEventObject::AITYPE_NPC) &&
                                    !(rEventField & IPlanEventObject::AIACT_ESCAPE);

    if(!raiseEscapeEvent)
        return FALSE;

    BYTE& rDangerLevel = rPlanEvent.m_DangerSkipLevel;
    const TileEvent& rRootTileEvent = *rPlanEvent.m_pTileEventInField;

    s_aligned_no = 0;
    memset(s_pFTA, -1, sizeof(s_pFTA));
    m_TempFlag.ClearAll();

    TileEvent cloneEvent;
    _PathPlan_NearNodeRandomSort(cloneEvent, rRootTileEvent);

    const WORD finded_tile = _SearchValidNearNode(cloneEvent, rDangerLevel, rRootTileEvent.Cost);
    if(finded_tile == rRootTileEvent.InvalidTileIndex)
    {
        // 도망치기 포기할 것. 그저 절망 속에 괴로워하거라.
        rDangerLevel = (rDangerLevel + 1) % 3;  // 계속 증가시켜버리자.[0,256)
        return FALSE;
    }

    rDangerLevel = 0;
    WzVector escape_pos = GetRandomPosInTile(finded_tile);

    //const WzVector& rCurPos = *m_pCharacter->GetPosPtr();
    //const float fdiff_len = VectorLength(&(escape_pos - rCurPos));
    //if(fdiff_len > 50.f)
    //  printf("Length %.2f\n", fdiff_len);

    CmdAI::Escape cmd(6 * 1000, finded_tile, &escape_pos);
    rPlanEvent.m_pStateManager->RequestCommand_(cmd);

    rPlanEvent.m_rEventField |= IPlanEventObject::AIACT_ESCAPE;

    return TRUE;
}


}; //end of namespace 'nsAI'


//==================================================================================================


namespace nsAI
{

BOOLEAN
MovingPlanner::_TestFunction()
{
    //Character* const pMovingObject = 0;
    //CPathExplorer* const pPathExplorer = pMovingObject->GetPathExplorer();
    return FALSE;
}


//--------------------------------------------------------------------------------------------------
#define CPATHEXPW_IsMoving(rMovingField)    \
    ((rMovingField) != PEMT_STANDING)

PlanEventObject::PlanEventObject(EventField_t& IN rEventField,
                                 CPathExplorer*& pPathExplorer,
                                 SkillManager*& pSkillManager,
                                 StatusManager*& pStatusManager)
    : m_rEventField(rEventField)
    , m_pGameFieldSync(0)
    , m_pCharacter(0)
    , m_pSkillManager(pSkillManager)
    , m_pStatusManager(pStatusManager)
    , m_pTargetManager(0)
    , m_pStateManager(0)
    , m_pPathExplorer(pPathExplorer = (CPathExplorer*)g_pGameServer->m_fnContactTerrain(CT_NEW_PATH_EXPLORER, 0))
    , m_PathExpWrap(CPathExplorerD::P_GetCurTile(m_pPathExplorer),
                    CPathExplorerD::P_GetPosPtr(m_pPathExplorer),
                    CPathExplorerD::P_GetTargetTile(m_pPathExplorer),
                    CPathExplorerD::P_GetTargetPos(m_pPathExplorer),
                    CPathExplorerD::P_GetMovingStatus(m_pPathExplorer))
    , m_pTileEventInField(&NullTileEvent)
    , m_DangerSkipLevel(0)
{
    m_TileEventInfo = NullTileEvent;
    m_DangerTimer.Instance();
}


//--------------------------------------------------------------------------------------------------


void
PlanEventObject::Init(Character* const pCharacter)
{
    //  해당 Character의 활성화
    const eOBJECT_TYPE object_type = pCharacter->GetObjectType();
    if(!FlowControl::FCAssert((object_type & CHARACTER_OBJECT) == CHARACTER_OBJECT))
        return;

    typedef IPlanEventObject    IPEO;
    switch(object_type)
    {
    case PLAYER_OBJECT:
        {
            m_rEventField   |= IPEO::AITYPE_PC;
            m_pGameFieldSync= pCharacter->GetField();
            m_pCharacter    = pCharacter;
        }
        break;
    case MONSTER_OBJECT:
    case SUMMON_OBJECT:
    case MERCHANT_OBJECT:
    case MAPNPC_OBJECT:
    case LUCKYMON_OBJECT:
    case TOTEMNPC_OBJECT:
    case SSQMONSTER_OBJECT:
    case CRYSTALWARP_OBJECT:
    case FRIEND_MONSTER_OBJECT:
    case SYNC_MERCHANT_OBJECT:
        {
            m_rEventField   |= IPEO::AITYPE_NPC;
            m_pGameFieldSync= pCharacter->GetField();
            m_pCharacter    = pCharacter;

            NPC* const pNPC = static_cast<NPC*>(pCharacter);
            m_pTargetManager= pNPC->GetTargetManager();
            m_pStateManager = pNPC->GetStateManager();
        }
        break;
    default:
        ASSERT(!"Invalid ObjectType");
        return;
    }
}


//--------------------------------------------------------------------------------------------------


void
PlanEventObject::Release()
{
    //  해당 Character의 비활성화
    m_pGameFieldSync    = 0;
    m_pTileEventInField = &NullTileEvent;
}


//--------------------------------------------------------------------------------------------------


PlanEventObject::eEventRet
PlanEventObject::_UpdateEvent(const DWORD delta_tick,
                              MovingPlanner* const pMovingPlanner,
                              const BOOLEAN bCheckChangeEvent,
                              const int     iTile,
                              const BOOLEAN bSameTile)
{
    __TOUCHED(delta_tick);
    //  check event
    if(!bSameTile)
    {
        PlanCmd::TileWeightArg cmd(PlanCmd::eTileWeight_GetTileEvent, iTile);
        if(!pMovingPlanner->PathPlan_Command(cmd, &m_pTileEventInField))
            return eEventRet_SkipProgressFail;// FALSE;
    }

    const TileEvent& rLastest   = *m_pTileEventInField;
    BOOLEAN bRuleAbsoluteCur    = FALSE;
    BOOLEAN bRuleAbsoluteAct    = FALSE;
    //BOOLEAN bRuleEscape           = FALSE;

    //if(bCheckChangeEvent)
    {
        TileEvent stk_event = !bSameTile                            ?   NullTileEvent
                            : (WORD)iTile == m_TileEventInfo.Index  ?   m_TileEventInfo
                            : NullTileEvent;

        //  Check Changes
        stk_event.Ctrl = stk_event.Ctrl != rLastest.Ctrl
                       ?   (~stk_event.Ctrl) & (rLastest.Ctrl)
                       :   NullTileEvent.Ctrl;

        stk_event.Index = stk_event.Index != rLastest.Index
                        ?   rLastest.Index
                        :   NullTileEvent.Index;

        stk_event.Attr = stk_event.Attr != rLastest.Attr
                       ?   (~stk_event.Attr) & rLastest.Attr
                       :   NullTileEvent.Attr;

        stk_event.Cost = stk_event.Cost != rLastest.Cost
                       ?   rLastest.Cost
                       :   NullTileEvent.Cost;
        // CHANGES: f110314.1L
        stk_event.Event = stk_event.Event != rLastest.Event
                        ?   rLastest.Event
                        :   NullTileEvent.Event;

        bRuleAbsoluteCur = !!(rLastest.Attr & PTA_PLAYER_FALLDOWN_TILE);
        bRuleAbsoluteAct = !!(stk_event.Attr & PTA_PLAYER_FALLDOWN_TILE);
        //bRuleEscape           = (stk_event.Cost > eWeight_Level_Heavy);

        m_TileEventInfo = rLastest;
    }

    const BOOLEAN isMoving = CPATHEXPW_IsMoving(m_PathExpWrap.rMovingStatus);
                             // !!m_pPathExplorer->IsMoving();
    const DWORD curHP = m_pCharacter->GetHP();
    if(!curHP)
    {
        if(isMoving)
            m_pPathExplorer->Stop();
        return eEventRet_SkipProgress;// TRUE;
    }

    if(IPEO::MOVE_HOLDING & m_rEventField)
        return eEventRet_SkipProgress;// TRUE;

    if(bRuleAbsoluteCur & bRuleAbsoluteAct)
    {
        m_pPathExplorer->Stop();

        MSGSUB_SYNC_STOP_BRD msg;
        msg.m_dwObjectKey   = m_pCharacter->GetObjectKey();
        msg.m_wvCurPos      = m_PathExpWrap.rwvPosCur;
        m_pCharacter->SendPacketAroundForSync(&msg);

        StatusField& rStatus = m_pCharacter->GetStatusManager()->Status;
        if(!(rStatus.FIELD & rStatus.ENABLE_GM_UNDEAD))
        {
            m_pCharacter->ClearKillerInfo();
            m_pCharacter->SetHP(0);
            m_pCharacter->OnDead();
        }

        m_rEventField |= IPEO::MOVE_HOLDING;

        return eEventRet_SkipProgress;// TRUE;
    }

    // (WAVERIX) (090711) braceket
    if((m_rEventField & IPEO::AIACT_ESCAPE) && isMoving)
        return eEventRet_NeedProgress;

    if((m_rEventField & IPEO::AITYPE_NPC) && bCheckChangeEvent)
    {
        if(m_DangerTimer.IsExpired())
        {
            if(eWeight_Level_Heavy < m_TileEventInfo.Cost)
            {   //  NPCState가 재구성되면 그때 옮기도록 하자.
                m_DangerTimer.SetNextTimeoutByCurTickBased(500);
                //printf("Raise event\n");
                NPC* const pNPC = static_cast<NPC*>(m_pCharacter);
                pMovingPlanner->_PathPlan_FindPathToSafeZone(pNPC, *this,
                                                             MovingPlanner::eFSR_CurTileDanger);
            }
        }
    }

    if(!isMoving)
        return eEventRet_SkipProgress;// TRUE;

    return eEventRet_NeedProgress;
}


//--------------------------------------------------------------------------------------------------
const float fTimeSlicingToken = 1.0f; // 1meter
//const float   floatMax  = (std::numeric_limits<float>::max)();
//const float   floatMin  = (std::numeric_limits<float>::min)();
//
//inline BOOLEAN    IsZero(float val)
//{
//  return ((-floatMin < val) && (val < floatMin));
//}

#define COND_PROGRESS_SMALL_DIST(fval)  ((fval) < 0.001f)

BOOLEAN
PlanEventObject::_UpdatePlayer_Move(const DWORD delta_tick, MovingPlanner* const pMovingPlanner)
{
    Player* const pPlayer = static_cast<Player*>(m_pCharacter);
    const WzVector* const pLatestPos = &m_PathExpWrap.rwvPosCur;
    const float move_speed = pPlayer->GetMoveStateControl()->GetMoveSpeed();
    const float move_length = (float)(delta_tick > 1000 ? 1000 : delta_tick) * move_speed;

    static ViewBoxCubic inRange;
    static const WzVector micro_box = { .003f, .003f, .0f };
    inRange.SetInit2D(&m_PathExpWrap.rwvPosTarget, &micro_box);

    float   fremain     = move_length;
    BOOLEAN lastresult  = FALSE;

    do
    {
        float ftoken = fremain > fTimeSlicingToken ? fTimeSlicingToken : fremain;

        const int iTile = m_PathExpWrap.riTileCur;
        const BOOLEAN bSameTile = ((WORD)iTile == m_pTileEventInField->Index);
        // side-effect : m_rEventField
        const eEventRet eeRet = _UpdateEvent(delta_tick, pMovingPlanner,
                                             /*!bSameTile*/FALSE, iTile, bSameTile);
        if(eeRet != eEventRet_NeedProgress)
        {
            break;
        }

        lastresult = pMovingPlanner->PathPlan_Progress(pPlayer, ftoken, 0, ECC_NO);
        if(!lastresult)
            break;

        // TODO(WAVERIX) 임시 처리, 좀더 개선 가능하다.

        fremain -= ftoken;
        if(COND_PROGRESS_SMALL_DIST(fremain))
            break;

    } while(!inRange.IsValidCoordinateXY(pLatestPos));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_traceroute", this->m_pCharacter)));

    return TRUE;
}


//--------------------------------------------------------------------------------------------------


BOOLEAN
PlanEventObject::_UpdateMonster_Move(const DWORD delta_tick, MovingPlanner* const pMovingPlanner)
{
    NPC* const pNPC = static_cast<NPC*>(m_pCharacter);
    const WzVector* const pLatestPos = &m_PathExpWrap.rwvPosCur;
    const float move_speed = pNPC->GetMoveStateControl()->GetMoveSpeed();
    const float move_length = (float)(delta_tick > 1000 ? 1000 : delta_tick) * move_speed;

    const sPOLICY_CHARACTER& CharPolicy = pNPC->Policyment;
    const bool contained_area_event = (CharPolicy.FIELD & CharPolicy.PCC_EXIST_AREA_EVENT_NPC) != 0;

    static ViewBoxCubic inRange;
    static const WzVector micro_box = { .003f, .003f, .0f };
    inRange.SetInit2D(&m_PathExpWrap.rwvPosTarget, &micro_box);

    float   fremain     = move_length;
    BOOLEAN lastresult  = FALSE;
    //printf("move + length %f\n", move_length);

    do
    {
        float ftoken = fremain > fTimeSlicingToken ? fTimeSlicingToken : fremain;

        const int iTile = m_PathExpWrap.riTileCur;
        //printf("progress\n");
        lastresult = pMovingPlanner->PathPlan_Progress(pNPC, ftoken, 0, ECC_NO);
        if(!lastresult)
            break;
        const int post_tile = m_PathExpWrap.riTileCur;
        const BOOLEAN bSameTile = (iTile == post_tile);
        // side-effect : m_rEventField
        const eEventRet eeRet = _UpdateEvent(delta_tick, pMovingPlanner,
                                             bSameTile == false, iTile, bSameTile);
        if(eeRet != eEventRet_NeedProgress)
        {
            //printf("UpdateEvent fail\n");
            break;
        }
    #ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
        // NOTE: f110314.1L, temporary event notifier
        if (contained_area_event && bSameTile == false) {
            if (GameField* game_field = pNPC->GetField()) {
                if (ns_trigger::TriggerNpcLinkMatchTable* table =
                        game_field->GetNpcTriggerEvents())
                {
                    table->OnChangeTile(iTile, post_tile, pNPC);
                }
            }
        };
    #endif
        // TODO(WAVERIX) 임시 처리, 좀더 개선 가능하다.

        fremain -= ftoken;
        if(COND_PROGRESS_SMALL_DIST(fremain))
            break;

    } while(!inRange.IsValidCoordinateXY(pLatestPos));

    return TRUE;
}


//--------------------------------------------------------------------------------------------------


BOOLEAN
PlanEventObject::_UpdateMonster_State(const DWORD delta_tick, MovingPlanner* const pMovingPlanner)
{
    __TOUCHED(pMovingPlanner);
    NPC* const pNPC = static_cast<NPC*>(m_pCharacter);
    __TOUCHED(pNPC); // for debug
    m_pTargetManager->Update();
    return (m_pStateManager->Update(delta_tick) != false);
}


//--------------------------------------------------------------------------------------------------


BOOLEAN
PlanEventObject::Update(const DWORD delta_tick, const BOOLEAN need_AI_proc_state)
{
    const int iTile = m_PathExpWrap.riTileCur;
    if((iTile < 0) || !(m_rEventField & IPlanEventObject::ACTIVATED_OBJECT))
    {
        m_pTileEventInField = &NullTileEvent;
        return FALSE;
    }

    const BOOLEAN isPlayer = !!(m_rEventField & IPlanEventObject::AITYPE_PC);

    //  Assert == (m_pCharacter->GetField() != 0)
    GameField* const pField = m_pCharacter->GetField();
    if (pField == NULL) {
        return FALSE;
    }
    const BOOLEAN bSynced   = m_pGameFieldSync && (m_pGameFieldSync == pField);
    if(!bSynced)
        m_pGameFieldSync = pField;

    const BOOLEAN bSameTile = bSynced && m_pTileEventInField &&
                              ((WORD)iTile == m_pTileEventInField->Index);

    MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();

    // side-effect : m_rEventField
    const eEventRet eeRet = (IPEO::AIACT_ESCAPE & m_rEventField)
                          ?   eEventRet_NeedProgress
                          :   _UpdateEvent(delta_tick, pMovingPlanner, TRUE, iTile, bSameTile);

    if(need_AI_proc_state && !isPlayer) // Monster
        if(FALSE == _UpdateMonster_State(delta_tick, pMovingPlanner))
            return FALSE;

    if(eeRet != eEventRet_NeedProgress)
        return TRUE;
    //return eeRet == eEventRet_SkipProgress;

    //  Moving Status
    if(isPlayer)    _UpdatePlayer_Move(delta_tick, pMovingPlanner);
    else            _UpdateMonster_Move(delta_tick, pMovingPlanner);

    return TRUE;
}

}; //end of namespace 'nsAI'

