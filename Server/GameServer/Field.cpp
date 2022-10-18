// Field.cpp: implementation of the Field class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <float.h>
#include <Global.h>
#include <NPCInfoParser.h>

#include <ServerOptionParser.h>
#include <ServerOptionParserEx.h>

#include "Field.h"
#include "EffectManager.h"
#include "Summoned.h"

#include "Sector.h"
#include "SectorInfo.h"
#include "FieldInfo.h"

#include "MapNPC.h"
#include "MapObject.h"
#include "ObjectFactory.h"
#include "TargetFinder.h"
#include "Item.h"

#include "Player.h"
#include "MovementChecker.h"
#include "Status/StatusManager.h"
#include "GameServerEx.h"
#include "MovingPlanner.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
Field::Field()
	: SectorGridRef( m_SectorGrid )
	, m_pMovingPlanner( new nsAI::MovingPlanner )	//__NA001166_080922_TASK_SSQ_HUMANOID_AI_01__
{
	m_pFieldInfo = NULL;
	m_RoomIndex = -1;
}

Field::~Field()
{
	Release();
	SAFE_DELETE(m_pMovingPlanner);
}


VOID
Field::Release()
{
	if( m_pFieldInfo && m_RoomIndex >= 0 )
	{
		m_pFieldInfo->Release( m_RoomIndex );
	}
	m_pFieldInfo = NULL;
	m_pSharedFieldInfo = NULL;
	m_RoomIndex = -1;

	m_SectorGrid.Destroy();
	//__NA001166_080922_TASK_SSQ_HUMANOID_AI_01__
	m_pMovingPlanner->Release();
}

VOID
Field::BuildSector()
{
	if( !FlowControl::FCAssert( m_pFieldInfo && !m_SectorGrid.SectorCount ) )
		return;

	//	(WAVERIX)(080810)(NOTE)
	//	-	이 함수가 호출되는 시점은 이전까지 PreMapLoading, Player(OnEnter)시에 발생하고 있었다.
	//		이것 생각보다 패킷량이 많이 발생한다. 각종 Object Enter
	//	-	이제 Field::Init 시점에 수행하기로 한다.
	m_SectorGrid.Build( m_pFieldInfo->SectorGridInfoRef );
}

// 임시로 Init에서 pMap을 셋팅하지만 나중에 뺄수 있으면 뺀다.
VOID
Field::Init( FieldInfo* pFieldInfo, bool bPreMapLoading, bool bUsingViewPort, bool bPreRegen )
{
	m_pFieldInfo		= pFieldInfo;
	m_pSharedFieldInfo	= pFieldInfo->GetSharedFieldInfo();
	m_bViewport			= bUsingViewPort;
	m_bPreMapLoading	= bPreMapLoading;
    is_war_castle_ = (m_pSharedFieldInfo->IsWarCastle() != false); // CHANGES: f110308.1L
	m_RoomIndex			= m_pFieldInfo->Init();

	if( m_RoomIndex < 0 )
	{
		SUNLOG( eCRITICAL_LOG, "[Field::Init] InValid RoomIndex[%d]", m_RoomIndex );
	}

	//__NA001166_080922_TASK_SSQ_HUMANOID_AI_01__
    m_pMovingPlanner->~MovingPlanner();
	new (m_pMovingPlanner) nsAI::MovingPlanner( m_pSharedFieldInfo, m_RoomIndex );
	{
		BuildSector();
	}
}


VOID Field::Update( DWORD dwDeltaTick )
{
}


bool Field::IsMovableCoordinates(const WzVector* query_position,
                                 FiltersForMovableCoordinates* filter)
{
    // X,Y좌표상에서 최대값보다 큰 값을 클라이언트에서 보낸 경우
    const SectorGridInfo& sector_grid_info = m_pFieldInfo->SectorGridInfoRef;
    const SECTOR_IDX coordinates = sector_grid_info.GetSectorIndex(query_position);
    if (coordinates == INVALID_SECTOR_INDEX)
    {
        const DWORD* const boundary = sector_grid_info.GetBoundaries();
        SUNLOG(eFULL_LOG,
               _T("["__FUNCTION__"] [Xmin, Xmax] = [%d, %d], [Ymin, Ymax] = [%d, %d] - ")
               _T("trying pos {%.f,%.f,%.f}"),
               boundary[SectorGridInfo::SG_X_MIN], boundary[SectorGridInfo::SG_X_MAX],
               boundary[SectorGridInfo::SG_Y_MIN], boundary[SectorGridInfo::SG_Y_MAX],
               query_position->x, query_position->y, query_position->z);
        return false;
    };

    // CHANGES: f110321.2L, changes logic using filter options
    bool check_tile_attr = true;
    float correction_height_workingslot;
    float* correction_height = NULL;
    if (filter)
    {
        if (filter->skip_check_tile_attr) {
            check_tile_attr = false;
        };
        if (filter->modified_position) {
            correction_height = &correction_height_workingslot;
        }
    };

    // 맵상에서 이동 가능한 좌표인지 검사
    const int result_tile = check_tile_attr
        ?   m_pMovingPlanner->GetAllowTileToStand(*query_position, correction_height, -1.0f, 100.0f)
        :   m_pMovingPlanner->GetTileToStand(*query_position, correction_height, -1.0f, 100.0f);

    const bool validated = (result_tile != -1);
    if (validated && filter)
    {
        filter->result_tile_no = result_tile;
        // CHANGES: f110321.2L, changes logic using filter options
        if (correction_height)
        {
            assert(correction_height == &correction_height_workingslot);
            if (WzVector* modified_pos = filter->modified_position)
            {
                *modified_pos = *query_position;
                // NOTE: add value to solve floating precision problem.
                // selecting methodology of adding value is based on top-bottom ray calculation
                correction_height_workingslot +=
                    ((correction_height_workingslot > 0.0f) ? -0.9f : +1.1f);
                // diagram
                // ranges of correction_height_workingslot = (0.0f, 1.1f]
                //                 1.1f (bottom)
                //                          | - safe
                //          z = 0 ---------------------------
                //                          | - refer to the below comments
                // below surface point caused unexpected graphic outputs
                modified_pos->z -= correction_height_workingslot;
            };
        }
    };
    return validated;
}


BOOL Field::EnterField( Object* pObject, const WzVector* pwzVec, int iAngle )
{
	const DWORD ERRCODE_DEFAULT			= 1000;
	const DWORD ERRCODE_INVALID_POS		= ERRCODE_DEFAULT + 1;
	const DWORD ERRCODE_INVALID_SECTOR	= ERRCODE_DEFAULT + 2;
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		if( !FlowControl::FCAssert( pObject ) )
			return FALSE;

		IfFailThenBreak(IsValidState(pObject) && "[EnterField] 잘못된 상태이다", ERRCODE_DEFAULT);

        // NOTE, below object types can standing on a tile having NO_WALK attribute
        // added by __NA001283_20090225_COLLECTION_USE
        if (0 == pObject->IsEqualObjectKind(COLLECTION_OBJECT) &&
            0 == pObject->IsEqualObjectKind(MAPNPC_OBJECT) )         // _NA001385_20090924_DOMINATION_TRIGGER
        {
            // 원래는 여기서도 타일속성 체크를 해야하지만
            // 트리거 NPC가 생성되지 않는 문제때문에 못가는 타일 위라도 일단은 허용해준다.
            // CHANGES: f110321.2L, changes logic using filter options
            FiltersForMovableCoordinates filter = { 0, };
            filter.skip_check_tile_attr = true;
            IfFailThenBreak(IsMovableCoordinates(pwzVec, &filter) &&
                            "[EnterField] 잘못된 위치벡터가 넘어왔다",
                            ERRCODE_INVALID_POS);
        }
        //
		GameField& rGameField = * static_cast<GameField*>( this );
		
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
#else
        rGameField.BuildInstanceInfo();
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION

		pObject->OnEnterField( rGameField, *pwzVec, iAngle );
		//	Post side-effect : sector position이 구해진다.

		const SECTOR_IDX coordinates = pObject->GetSectorIndex();
		SectorGrid3x3 OUT grid;
		IfFailThenBreak(m_SectorGrid.GetAroundSectors(coordinates, &grid), ERRCODE_INVALID_SECTOR);

		//	Center를 먼저 처리할 것인가 그것이 문제로다...
		{	//	Center Processing
			Sector* const pSector = m_SectorGrid.GetSector( coordinates );
			grid.Grid[ SectorGrid3x3::CENTER ] = INVALID_SECTOR_INDEX;		// Center 제거

			const RC::eSECTOR_REULT result = pSector->EnterObject( pObject );
			IfFailThenBreak(result == RC::RC_SECTOR_SUCCESS, result);

			pSector->UpdateReferenceCount( pObject, TRUE );
			SpawnSector( coordinates, pObject );
		};

		//	주변 섹터만 처리한다. Center는 앞 로직에서 제거되었음.
		FOREACH_ARRAY( const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray )
		{
			if( INVALID_SECTOR_INDEX == sector_idx )
				continue;

			Sector* const pSector = m_SectorGrid.GetSector( sector_idx );

			//if( IsUsingViewPort() )
			//	기존 로직에 의하면, view_port를 사용하지 않는 경우 전체 섹터를 처리하는 형태였다.
			//	왜 전체 섹터를 다 처리했었는지를 타전해 볼 것.
			//	일단 가능성은 OldSector처리상의 문제가 있을 수 있다.

			pSector->OnEnterThisToNeighbor( pObject );	// 자신정보를 주변에게
			pSector->OnEnterNeighborToThis( pObject );	// 주변정보를 자신에게

			pSector->UpdateReferenceCount( pObject, TRUE );
			SpawnSector( sector_idx, pObject );
		}

		//	(NEW)
		//	-	OnEnterField 에서 Old인덱스는 변경되지 않고 있다.
		//		따라서 Player::Update에서 ChangeSector가 나중에 발생하게 될 터...
		//	-	실험 코드
		//	(WAVERIX) (NOTE) (081229) 초기 섹터 변화에 영향을 준다. 원래대로 복구한다.
		//	여담으로, 자주 느끼는 거지만, 전체적인 플로우가 좋게 말하면 절묘하게,
		//	나쁘게 말하면, 어디로 튈지 모르게 복잡하다. 초기 설계 자체는 나쁜 것이 아니라 생각되는데, 결과적으로
		//	이렇게 되는 것은 functionality, modularity에 대한 재고찰의 필요성을 느끼게 한다.
		return TRUE;
    }
    FLOWCONTROL_END;
	
	SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"][%3u] %s", flow.GetErrorCode(), flow.GetErrorString() );

	return FALSE;
}

class ObjectGameFieldAutoClosure
{
public:		ObjectGameFieldAutoClosure( Object* const pObject, GameField* const pGameField )
				: m_pObject( pObject )
			{
				m_pObject->SetField( pGameField );
			};

			~ObjectGameFieldAutoClosure()
			{
				m_pObject->SetField( 0 );
			}
private:	Object*		m_pObject;
};

BOOL Field::LeaveField( Object* pObject )
{
	if( !IsValidState(pObject) )
	{
		SUNLOG( eCRITICAL_LOG, "[LeaveField] 잘못된 상태이다." );
		return FALSE;
	}

	SECTOR_IDX coordinates = pObject->GetSectorIndex();
	SectorGrid3x3 OUT grid;
	if( !m_SectorGrid.GetAroundSectors( coordinates, &grid ) )
	{
		SUNLOG( eCRITICAL_LOG, "[Field::LeaveField] FieldCode[%d] Invalid SectorIndex[%d] ", GetFieldCode(), coordinates );
		// 뭔가의 이유로 (예:sector index의 초기화 또는 벽뚫기?) 찾을 수 없을 수도 있을 것이다.
		if( coordinates == INVALID_SECTOR_INDEX )
		{
			//	(WAVERIX)(080810)(NOTE)
			//	정상적인 로직을 타지 못했을 경우(ChangeSector), 문제가 야기될 수 있을 터. 아래 로직으로 해결을 시도해 보자.
			//	아래의 로직의 검증은 일단 이준석 팀장님한테 가능성 타전 후, 정밀 검증할 필요가 있다.
			if( coordinates != pObject->GetOldSectorIndex() )
			{
				if( m_SectorGrid.GetAroundSectors( coordinates, &grid ) )
					coordinates = pObject->GetOldSectorIndex();
			}
		}
	}

	//	Center를 먼저 처리할 것인가 그것이 문제로다...
	//	Center Processing
	Sector* const pSector = m_SectorGrid.GetSector( coordinates );
	grid.Grid[ SectorGrid3x3::CENTER ] = INVALID_SECTOR_INDEX;		// Center 제거

	pObject->OnLeaveField();

	ObjectGameFieldAutoClosure autoclosure( pObject, static_cast<GameField*>(this) );

	// 섹터의 오브젝트 리스트에서 삭제
	if( pSector )
	{
		RC::eSECTOR_REULT result = pSector->LeaveObject( pObject );
		if( result != RC::RC_SECTOR_SUCCESS )
		{
			SUNLOG( eCRITICAL_LOG,  "[Field::LeaveField] RemoveObject Fail![%d] ", result );
			return FALSE;
		}
		pSector->UpdateReferenceCount( pObject, FALSE );
	}


	//	주변 섹터만 처리한다. Center는 앞 로직에서 제거되었음.
	FOREACH_ARRAY( const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray )
	{
		if( INVALID_SECTOR_INDEX == sector_idx )
			continue;

		Sector* const pSector = m_SectorGrid.GetSector( sector_idx );

		//if( IsUsingViewPort() )
		//	기존 로직에 의하면, view_port를 사용하지 않는 경우 전체 섹터를 처리하는 형태였다.
		//	왜 전체 섹터를 다 처리했었는지를 타전해 볼 것.
		//	일단 가능성은 OldSector처리상의 문제가 있을 수 있다.

		pSector->OnLeaveThisToNeighbor( pObject );
		pSector->UpdateReferenceCount( pObject, FALSE );
	}

	return TRUE;
}

VOID Field::ChangeSector( Object* pObject )
{
	if( !IsValidState(pObject) )
	{
		SUNLOG( eCRITICAL_LOG, "[EnterField] 잘못된 상태이다." );
		return;
	}

	const SECTOR_IDX old_index = pObject->GetOldSectorIndex();
	const SECTOR_IDX new_index = pObject->GetSectorIndex();

	SectorGrid3x3_ChangeSector grids;
	if( FALSE == m_SectorGrid.ChangeSector( old_index, new_index, grids ) )
	{
		SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"] - Irony State!! [OI:CI] = [%4d:%4d]", old_index, new_index );
		return;
	}

	Sector* const pOldSector =	GetSector( old_index );
	if( pOldSector )	// 이전 섹터에서 캐릭터 제거
		pOldSector->Remove(pObject);
	//ASSERT( new_index == grids.CurrentSectors[SectorGrid3x3::CENTER] );
	Sector* const pNewSector =	GetSector( new_index );
	if( pNewSector )	// 새로운 섹터에 캐릭터 추가
		pNewSector->AddObject( pObject );


	//	완전히 떠나는 섹터 처리
	SectorGrid3x3& rLeave = grids.LeaveSectors;
	FOREACH_ARRAY( const SECTOR_IDX sector_idx, rLeave.Grid, SectorGrid3x3::GridArray )
	{
		if( sector_idx == INVALID_SECTOR_INDEX )
			continue;

		Sector* const pSector = GetSector( sector_idx );
		//	if( IsUsingViewPort() )
		//	- 이번에도 마찬가지다. Enter, Leave시 몽땅 처리하고 있다... 왠지 불안해진다. 왜 이렇게 했었지?

		pSector->OnLeaveThisToNeighbor( pObject );		//	캐릭터가 떠났음을 섹터에 알려준다.
		pSector->OnLeaveNeighborToThis( pObject );		//	섹터의 정보를 떠나는 캐릭터로부터 제거한다.
		pSector->UpdateReferenceCount( pObject, FALSE );
	}

	//	신규로 등장하는 섹터 처리
	SectorGrid3x3& rEnter = grids.EnterSectors;
	FOREACH_ARRAY( const SECTOR_IDX sector_idx, rEnter.Grid, SectorGrid3x3::GridArray )
	{
		if( sector_idx == INVALID_SECTOR_INDEX )
			continue;

		Sector* const pSector = GetSector( sector_idx );
		//	if( IsUsingViewPort() )
		//	- 이번에도 마찬가지다. Enter, Leave시 몽땅 처리하고 있다... 왠지 불안해진다. 왜 이렇게 했었지?

		pSector->OnEnterThisToNeighbor( pObject );		// 새로운 캐릭터가 등장했음을 섹터에 알려준다.
		pSector->OnEnterNeighborToThis( pObject );		// 섹터의 정보를 등장 캐릭터에게 알려준다.
		pSector->UpdateReferenceCount( pObject, TRUE );
		SpawnSector( sector_idx, pObject );
	}
}

BOOL
Field::CorrectMovableDestPath(Character* pCharacter,
    const WzVector* pwvDest, WzVector* corrected_dest)
{
    // implemented by _NA001385_20090924_DOMINATION_FINDPATH_ATTACK
    const DWORD ERRCODE_INVALID_USAGE = 100;
    // CHANGES: f110303.1L, added new argument to control result pos.
    WzVector working_vector;
    WzVector* result_dest = &working_vector;
    if (corrected_dest == NULL) {
        working_vector = *pwvDest;
    };
    if (corrected_dest != NULL) {
        if (corrected_dest != pwvDest) {
            *corrected_dest = *pwvDest;
        }
        result_dest = corrected_dest;
    };
    //////////////////////////////////////////////////////////////////////////
    //	<LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        //IfFailThenBreak(pwvDest, ERRCODE_INVALID_USAGE);

        static const SERVER_COMMON_OPTION& stServerOptionInfo = 
            ServerOptionParserEx::Instance()->GetServerOption();

        static const WORD maxDistance = stServerOptionInfo.m_wFINDPATH_MAX_DISTANCE;
        static const FLOAT fAcceptalble_lenQ2 = (FLOAT)(maxDistance * maxDistance);

        WORD cur_tile_attr = 0;
        FLOAT fZdiff = 0;
        //static const WORD jumpLimit = stServerOptionInfo.m_fFINDPATH_JUMPLIMIT;
        const INT iTileNo = m_pMovingPlanner->GetTileToStand(*result_dest, \
            &fZdiff, stServerOptionInfo.m_fFINDPATH_JUMPLIMIT, 100.0f, &cur_tile_attr);

        if( iTileNo < 0 )
            return FALSE;

        result_dest->z -= fZdiff;

        const WORD filtered_attr = (PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE);

        const ENUM_FINDPATH_RESULT path_result =
            m_pMovingPlanner->PathPlan_MoveCheck(pCharacter, *result_dest, iTileNo, 
                                                 filtered_attr, MAX_TILE_NODE_CNT_IN_DOMINATION);

        if (path_result == EFR_FAILURE ||
            path_result == EFR_SAME_POSITION)
            return FALSE;

        return TRUE;
    }
    FLOWCONTROL_END;

    SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"Unexpected Status Check-it now! [Err:%u]", flow.GetErrorCode() );

    return FALSE;
}

BOOL
Field::FindPath(Character* pCharacter, WzVector* pwvDest, eCHAR_STATE_TYPE eException)
{
    // (CHANGES) (091229) (WAVERIX) code-arrangement for NPC moving control
    // (NOTE) (080925) (WAVERIX)
    //  -   두 번째로 고려 사항이 많은 로직이 되어 버렸다. 일단 기존 로직 재정리 후 추가 적용하는 방식을 취하기로 한다.
    //  Normal status는 그냥 리턴
    const DWORD ERRCODE_INVALID_USAGE = 100;
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak(pCharacter && pwvDest, ERRCODE_INVALID_USAGE);

        // 캐릭터가 이동가능한 상태인지 체크
        //  can_move    can_move_e
        //      0           0       0
        //      0           1       1       
        //      1           0(X)    1(not_process)
        //      1           1(X)    1(not_process)

        BOOLEAN can_move   = !!pCharacter->CanMove();
        BOOLEAN can_move_e = can_move ? can_move : !!pCharacter->CanMove(eException);

        //  서버 강제 이동일 경우도 포함
        if ((can_move | can_move_e) == 0)
            return false;

        // 이동가능한 좌표인지 체크
        if (m_pFieldInfo->IsValidCoordinates(pwvDest) == false)
            return false;

        const WORD disable_attr = PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE;
        // (CHANGES) add current tile check
        if (nsAI::PlanEventObject::AcceptableTarget(pCharacter, 0, 0) == false) // no check safety
            return false;
        //const int cur_tile = pCharacter->PlanEvent->GetTile();
        //if (cur_tile < 0)
        //    return false;

        //const WORD cur_tile_attr = m_pMovingPlanner->GetAttribute(cur_tile);
        //// (NOTE) 이동 불가 타일 위에 존재하고 있을 수 있다.
        //if (disable_attr & cur_tile_attr)
        //    return false;

        static const SERVER_COMMON_OPTION& stServerOptionInfo =
            ServerOptionParserEx::Instance()->GetServerOption();
        static const float fAcceptalble_lenQ2 = (float)(stServerOptionInfo.m_wFINDPATH_MAX_DISTANCE *
                                                        stServerOptionInfo.m_wFINDPATH_MAX_DISTANCE);

        //////////////////////////////////////////////////////////////////////////
        //  다음 로직부터는 time slicing등등을 고려해야 한다.
        // TODO(WAVERIX)

        WORD dest_tile_attr = 0;
        float fZdiff = 0;

        const INT dest_tile = m_pMovingPlanner->GetTileToStand(*pwvDest,
            &fZdiff, stServerOptionInfo.m_fFINDPATH_JUMPLIMIT, 100.0f, &dest_tile_attr);
        if (dest_tile < 0)
            return false;

        //  갈 수 없는 곳 또는 가서는 안되는 것 스킵 처리

        // TODO(WAVERIX) 고려사항이 많다.
        //  현 시점(080925) fear상태라면, 낙사타일로도 도망치도록 하자.
        const WORD filtered_attr = (can_move_e > can_move)
                                 ?  PTA_NO_WALK
                                 :  disable_attr;            

        if (filtered_attr & dest_tile_attr)
            return false;

        pwvDest->z -= fZdiff;

        //  기존 로직... 처리상 (PTA_ONLY_JUMP) 이상한 부분이 있다.
        //  일단 변환 작업하는 목적에 맞게 변경하기전 이 부분을 해결하기로 한다.
        const BOOLEAN isNPC = pCharacter->IsEqualObjectKind(NPC_OBJECT);

        //  (Old Comment) 현재 길찾기 엔진에 문제 때문에 너무 먼거리는 길찾기 시도를 하지 않는다.
        if (isNPC) {
            const float fLengthQ2 = Math_VectorLength2(*pwvDest, *pCharacter->GetPosPtr());
            if (fLengthQ2 > fAcceptalble_lenQ2)
                return false;
        }

        const ENUM_FINDPATH_RESULT path_result = m_pMovingPlanner->PathPlan_FindPath(pCharacter,
            *pwvDest, dest_tile, filtered_attr, stServerOptionInfo.m_wFINDPATH_MAX_NODE_COUNT);

        if (path_result == EFR_FAILURE ||
            path_result == EFR_SAME_POSITION)
        {
            return false;
        }

        return true;
    }
    FLOWCONTROL_END;

    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"Unexpected Status Check-it now! [Err:%u]",
           flow.GetErrorCode());

    return false;
}

BOOL
Field::FindPathThrust(Character* pCharacter, WzVector* pwvMoveDistance, BOOL bForcedMove)
{
    // (CHANGES) (091229) (WAVERIX) code-arrangement for NPC moving control
    // (NOTE) (080925) (WAVERIX)
    //  -   제일 고려 사항이 많은 로직이 되어 버렸다. 일단 기존 로직 재정리 후 추가 적용하는 방식을 취하기로 한다.
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        //  캐릭터가 이동가능한 상태인지 체크
        if ((bForcedMove || pCharacter->CanMove()) == false)
            return false;

        // (CHANGES) add current tile check
        if (nsAI::PlanEventObject::AcceptableTarget(pCharacter, 0, 0) == false) // no check safety
            return false;
        //const int cur_tile = pCharacter->PlanEvent->GetTile();
        //if (cur_tile < 0)
        //    return false;

        //const WORD cur_tile_attr = m_pMovingPlanner->GetAttribute(cur_tile);
        //// (NOTE) 이동 불가 타일 위에 존재하고 있을 수 있다.
        //if ((PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE) & cur_tile_attr)
        //    return false;

        //  이동가능한 좌표인지 체크
        const WzVector& rCurPos = *pCharacter->GetPosPtr();
        WzVector wvDestPos = rCurPos;
        wvDestPos += *pwvMoveDistance;

        // TODO(WAVERIX) 고려사항이 많다.
        if (!IsMovableCoordinates(&wvDestPos))
            return false;

        //  갈 수 없는 곳 또는 가서는 안되는 것 스킵 처리
        const WORD disable_attr = (PTA_NO_WALK);//| PTA_PLAYER_FALLDOWN_TILE);

        //  현 시점(080925) fear상태라면, 낙사타일로도 도망치도록 하자.
        const WORD filtered_attr = (!bForcedMove)
                                 ?  PTA_NO_WALK
                                 :  disable_attr;

        //if (filtered_attr & cur_tile_attr)
        //  return false;

        const ENUM_FINDPATH_RESULT path_result = \
            m_pMovingPlanner->PathPlan_Thrust(pCharacter, pwvMoveDistance, filtered_attr);

        if (path_result == EFR_FAILURE ||
            path_result == EFR_SAME_POSITION)
        {
            return false;
        }

        return true;
    }
    FLOWCONTROL_END;

    return false;
}


bool Field::GetRandomPosInAround(const WzVector* query_position,
                                 int iDist, // TODO: f110321.2L, changes to more valuable usage
                                 WzVector& result_position,
                                 int number_of_max_retry)
{
    // CHANGES: f080925.x, temporary changes
    // CHANGES: f110321.2L, the renewal logic has more correct z-axis value than the custom logic result
    FieldInfo* field_info = this->GetFieldInfo();
    // NOTE: first, check whether the position stand on valid grid sector
    if (field_info->IsValidCoordinates(query_position) == false) {
        return false;
    };
    //
    WzVector working_position = *query_position;
    FiltersForMovableCoordinates coord_check_filter = { 0, };
    coord_check_filter.modified_position = &working_position;
    //
    for (int loop = number_of_max_retry; loop >= 0; --loop)
    {
        RandomRangeVector(&working_position, iDist);
        if (this->IsMovableCoordinates(&working_position, &coord_check_filter)) {
            result_position = working_position; // CHANGES: f110322.1L, fixed
            return true;
        }
    }
    return false;
}

BOOL Field::GetRandomPosInAroundCheckPath( Character* pCharacter, WORD wMaxNodeCount, const WzVector* pOrgPos,
                                          INT iDist, WzVector & vResultPos, int iLoop )
{
    // TODO: do changes related to f110321.2L
    // implemented _NA001385_20090924_DOMINATION_~~
    vResultPos = *pOrgPos;
    for( int i = 0; i < iLoop; i++ )
    {
        RandomRangeVector( &vResultPos, iDist );

        if (IsMovableCoordinates(&vResultPos) &&
            CorrectMovableDestPath(pCharacter, &vResultPos, &vResultPos))
            return TRUE;
    }

    return FALSE;
}

BOOL
Field::GetRandomPos( WzVector& wzVec, int iLoop )
{
	const WORD cant_move_tile_attr = PTA_PICK|PTA_ONLY_JUMP|PTA_NO_WALK;
	const INT max_tile_count = m_pMovingPlanner->GetNumPathTile();

	for( INT loop=iLoop ; loop ; --loop )
	{
		const INT iTileNo = rand()%max_tile_count;
		const WORD tile_attr = m_pMovingPlanner->GetAttribute( iTileNo );

		if( tile_attr & cant_move_tile_attr )
			continue;

		wzVec = m_pMovingPlanner->GetRandomPosInTile( iTileNo );
		return TRUE;
	}

	SUNLOG( eCRITICAL_LOG, "[Field::GetRandomPos] Can't Find RandomPoint(Too Many Loof)!!" );
	return FALSE;
}


WzVector
Field::GetRandomStartingVector( BYTE byClass )
{
	WzVector wzVec;
	DWORD TileIndex;

	nsAI::MovingPlanner* const pMovingPlanner = GetMovingPlanner();

	for( int i = 0; i < 50; i++ )
	{
		TileIndex = m_pFieldInfo->GetRandomStartingTileIndex( byClass );
		wzVec = pMovingPlanner->GetRandomPosInTile( TileIndex );

		if( IsMovableCoordinates( &wzVec ) )
			return wzVec;
	}

	SUNLOG( eCRITICAL_LOG, "[Field::GetRandomStartingVector] Can't Find RandomStartingPoint!!" );

	SetVector( &wzVec, 0, 0, 0 );
	return wzVec;
}



BOOL
Field::GetAreaRandomStartingVector( WzID id, WzVector& wzVec )
{
	const DWORD ERRCODE_CANT_FIND_AREA		= 100;
	const DWORD ERRCODE_NOT_DEFINED_BVTYPE	= 110;
	const DWORD ERRCODE_OTHER				= 120;

	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		WzSpecialArea* const pArea = m_pFieldInfo->GetAreaInfo( id );
		IfFailThenBreak(pArea, ERRCODE_CANT_FIND_AREA);

		const ENUM_BVTYPE bv_type = pArea->m_wbvRegion.m_eBvType;
		switch( bv_type )
		{
		case BVT_AABB:
			{
				//const BvAABB& rAABB = pArea->m_wbvRegion.m_aabb;
				const WzVector& rMinPos = pArea->m_wbvRegion.m_aabb.m_wvMin;
				const WzVector& rMaxPos = pArea->m_wbvRegion.m_aabb.m_wvMax;
				wzVec.z = rMaxPos.z;

				//	30번 시도라... 안되는 것은 안되는 거지만서두...
				for( INT loop=30 ; loop ; --loop )
				{
					wzVec.x = (float)dRandom( rMinPos.x, rMaxPos.x );
					wzVec.y = (float)dRandom( rMinPos.y, rMaxPos.y );

					if( IsMovableCoordinates( &wzVec ) )
						return TRUE;
				}
			}
			break;
		case BVT_PATH_TILE_INDEX:
			{
				const DWORD area_tile_count = pArea->m_wbvRegion.m_BvPathTileIndex.m_nCount;
				const DWORD* const area_tiles = pArea->m_wbvRegion.m_BvPathTileIndex.m_pdwData;
				IfFailThenBreak(area_tile_count, ERRCODE_OTHER);

				nsAI::MovingPlanner* const pMovingPlanner = GetMovingPlanner();
				//	30번 시도라... 안되는 것은 안되는 거지만서두...
				for( INT loop=30 ; loop ; --loop )
				{
					const DWORD selected_tile = (rand() % area_tile_count);
					wzVec = pMovingPlanner->GetRandomPosInTile( area_tiles[selected_tile] );

					if( IsMovableCoordinates( &wzVec ) )
						return TRUE;
				}

				SetFailFc( "loop limit", ERRCODE_OTHER );
			}
			break;
		}

		//	여기에 도달하는 것은 에러 발생한 경우
		//	return FALSE;
    }
    FLOWCONTROL_END;
	
	//////////////////////////////////////////////////////////////////////////
	//	<ERROR CONTROL>
	switch( flow.GetErrorCode() )
	{
	case ERRCODE_CANT_FIND_AREA:
		{
			TCHAR pszAreaID[8] = { 0, };
			WzIDToString( pszAreaID, id );
			pszAreaID[4] = '\0'; 
			SUNLOG( eCRITICAL_LOG
				, "[Field::GetAreaRandomStartingVector][FIELDCODE:%u][AreaID:%s]영역이 없습니다."
				, m_pFieldInfo->GetFieldCode(), pszAreaID );

			wzVec = GetRandomStartingVector();
		}
		return FALSE;
	default:
		SetVector( &wzVec, 0, 0, 0 );
		break;
	}
	
	return FALSE;
}


BOOL Field::RandomAreaPos( WzVector & vRandomPos, DWORD dwAreaID )
{
	WzSpecialArea *pBaseAreaInfo = GetFieldInfo()->GetAreaInfo( dwAreaID );
	if( !pBaseAreaInfo )
	{
		SUNLOG( eFULL_LOG, "[RandomAreaPos] MoveArea(%u)Info doesn't exist!", dwAreaID );
		return FALSE;
	}

	if( pBaseAreaInfo ->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX )
	{
		DWORD dwAreaTileCnt = pBaseAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_nCount;
		if( dwAreaTileCnt )
		{
			DWORD *pdwTileIndex = pBaseAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData;
			DWORD dwSelectedTile = rand() % dwAreaTileCnt;
			vRandomPos = m_pMovingPlanner->GetRandomPosInTile( pdwTileIndex[dwSelectedTile] );
			return TRUE;
		}
	}
	else if( pBaseAreaInfo ->m_wbvRegion.m_eBvType == BVT_AABB )
	{
		WzVector vMinPos = pBaseAreaInfo->m_wbvRegion.m_aabb.m_wvMin;
		WzVector vMaxPos = pBaseAreaInfo->m_wbvRegion.m_aabb.m_wvMax;
		vRandomPos.z = vMaxPos.z;

		// 10번까지만 시도한다.
		for( int i = 0; i < 10; ++i )
		{
			vRandomPos.x = (float)dRandom( vMinPos.x, vMaxPos.x );
			vRandomPos.y = (float)dRandom( vMinPos.y, vMaxPos.y );

			if( IsMovableCoordinates( &vRandomPos ) )
			{
				return TRUE;
			}
		}
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "[Field::RandomAreaPos] AreaType is Invalid!!" );
	}

	return FALSE;
}


//{__NA_0_20090720_GUILD_RENDER_INFO_MODIFY
//GetSectorGrid3x3
VOID Field::FirstGuildMemberEnter( Player* pPlayer )
{
    if( !IsValidState(pPlayer) )
    {
        SUNLOG( eCRITICAL_LOG, "[EnterField] 잘못된 상태이다." );
        return;
    }

    const SECTOR_IDX idxSector = pPlayer->GetSectorIndex();
    SectorGrid3x3 OUT grid;
    if( FALSE == m_SectorGrid.GetAroundSectors(idxSector, &grid) )
        return;

    //.// Center

    const Sector* const pSector = m_SectorGrid.GetSector( idxSector );
    grid.Grid[ SectorGrid3x3::CENTER ] = INVALID_SECTOR_INDEX;		// Center 제거

    pPlayer->OnSendAllPlayersGuildRenderInfoToNeighbor(*pSector);
    pPlayer->OnSendAllPlayersGuildRenderInfo(*pSector);

    //.// Around

    FOREACH_ARRAY( const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray )
    {
        if( INVALID_SECTOR_INDEX == sector_idx )
            continue;

        const Sector* const pSector = m_SectorGrid.GetSector( sector_idx );

        pPlayer->OnSendAllPlayersGuildRenderInfoToNeighbor(*pSector);
        pPlayer->OnSendAllPlayersGuildRenderInfo(*pSector);
    }
}
//}