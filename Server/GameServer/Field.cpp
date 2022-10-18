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

//��������������������������������������������������������������������������������������������������������������������������������������������������
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
	//	-	�� �Լ��� ȣ��Ǵ� ������ �������� PreMapLoading, Player(OnEnter)�ÿ� �߻��ϰ� �־���.
	//		�̰� �������� ��Ŷ���� ���� �߻��Ѵ�. ���� Object Enter
	//	-	���� Field::Init ������ �����ϱ�� �Ѵ�.
	m_SectorGrid.Build( m_pFieldInfo->SectorGridInfoRef );
}

// �ӽ÷� Init���� pMap�� ���������� ���߿� ���� ������ ����.
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
    // X,Y��ǥ�󿡼� �ִ밪���� ū ���� Ŭ���̾�Ʈ���� ���� ���
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

    // �ʻ󿡼� �̵� ������ ��ǥ���� �˻�
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

		IfFailThenBreak(IsValidState(pObject) && "[EnterField] �߸��� �����̴�", ERRCODE_DEFAULT);

        // NOTE, below object types can standing on a tile having NO_WALK attribute
        // added by __NA001283_20090225_COLLECTION_USE
        if (0 == pObject->IsEqualObjectKind(COLLECTION_OBJECT) &&
            0 == pObject->IsEqualObjectKind(MAPNPC_OBJECT) )         // _NA001385_20090924_DOMINATION_TRIGGER
        {
            // ������ ���⼭�� Ÿ�ϼӼ� üũ�� �ؾ�������
            // Ʈ���� NPC�� �������� �ʴ� ���������� ������ Ÿ�� ���� �ϴ��� ������ش�.
            // CHANGES: f110321.2L, changes logic using filter options
            FiltersForMovableCoordinates filter = { 0, };
            filter.skip_check_tile_attr = true;
            IfFailThenBreak(IsMovableCoordinates(pwzVec, &filter) &&
                            "[EnterField] �߸��� ��ġ���Ͱ� �Ѿ�Դ�",
                            ERRCODE_INVALID_POS);
        }
        //
		GameField& rGameField = * static_cast<GameField*>( this );
		
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
#else
        rGameField.BuildInstanceInfo();
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION

		pObject->OnEnterField( rGameField, *pwzVec, iAngle );
		//	Post side-effect : sector position�� ��������.

		const SECTOR_IDX coordinates = pObject->GetSectorIndex();
		SectorGrid3x3 OUT grid;
		IfFailThenBreak(m_SectorGrid.GetAroundSectors(coordinates, &grid), ERRCODE_INVALID_SECTOR);

		//	Center�� ���� ó���� ���ΰ� �װ��� �����δ�...
		{	//	Center Processing
			Sector* const pSector = m_SectorGrid.GetSector( coordinates );
			grid.Grid[ SectorGrid3x3::CENTER ] = INVALID_SECTOR_INDEX;		// Center ����

			const RC::eSECTOR_REULT result = pSector->EnterObject( pObject );
			IfFailThenBreak(result == RC::RC_SECTOR_SUCCESS, result);

			pSector->UpdateReferenceCount( pObject, TRUE );
			SpawnSector( coordinates, pObject );
		};

		//	�ֺ� ���͸� ó���Ѵ�. Center�� �� �������� ���ŵǾ���.
		FOREACH_ARRAY( const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray )
		{
			if( INVALID_SECTOR_INDEX == sector_idx )
				continue;

			Sector* const pSector = m_SectorGrid.GetSector( sector_idx );

			//if( IsUsingViewPort() )
			//	���� ������ ���ϸ�, view_port�� ������� �ʴ� ��� ��ü ���͸� ó���ϴ� ���¿���.
			//	�� ��ü ���͸� �� ó���߾������� Ÿ���� �� ��.
			//	�ϴ� ���ɼ��� OldSectoró������ ������ ���� �� �ִ�.

			pSector->OnEnterThisToNeighbor( pObject );	// �ڽ������� �ֺ�����
			pSector->OnEnterNeighborToThis( pObject );	// �ֺ������� �ڽſ���

			pSector->UpdateReferenceCount( pObject, TRUE );
			SpawnSector( sector_idx, pObject );
		}

		//	(NEW)
		//	-	OnEnterField ���� Old�ε����� ������� �ʰ� �ִ�.
		//		���� Player::Update���� ChangeSector�� ���߿� �߻��ϰ� �� ��...
		//	-	���� �ڵ�
		//	(WAVERIX) (NOTE) (081229) �ʱ� ���� ��ȭ�� ������ �ش�. ������� �����Ѵ�.
		//	��������, ���� ������ ������, ��ü���� �÷ο찡 ���� ���ϸ� �����ϰ�,
		//	���ڰ� ���ϸ�, ���� ƥ�� �𸣰� �����ϴ�. �ʱ� ���� ��ü�� ���� ���� �ƴ϶� �����Ǵµ�, ���������
		//	�̷��� �Ǵ� ���� functionality, modularity�� ���� ������� �ʿ伺�� ������ �Ѵ�.
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
		SUNLOG( eCRITICAL_LOG, "[LeaveField] �߸��� �����̴�." );
		return FALSE;
	}

	SECTOR_IDX coordinates = pObject->GetSectorIndex();
	SectorGrid3x3 OUT grid;
	if( !m_SectorGrid.GetAroundSectors( coordinates, &grid ) )
	{
		SUNLOG( eCRITICAL_LOG, "[Field::LeaveField] FieldCode[%d] Invalid SectorIndex[%d] ", GetFieldCode(), coordinates );
		// ������ ������ (��:sector index�� �ʱ�ȭ �Ǵ� ���ձ�?) ã�� �� ���� ���� ���� ���̴�.
		if( coordinates == INVALID_SECTOR_INDEX )
		{
			//	(WAVERIX)(080810)(NOTE)
			//	�������� ������ Ÿ�� ������ ���(ChangeSector), ������ �߱�� �� ���� ��. �Ʒ� �������� �ذ��� �õ��� ����.
			//	�Ʒ��� ������ ������ �ϴ� ���ؼ� ��������� ���ɼ� Ÿ�� ��, ���� ������ �ʿ䰡 �ִ�.
			if( coordinates != pObject->GetOldSectorIndex() )
			{
				if( m_SectorGrid.GetAroundSectors( coordinates, &grid ) )
					coordinates = pObject->GetOldSectorIndex();
			}
		}
	}

	//	Center�� ���� ó���� ���ΰ� �װ��� �����δ�...
	//	Center Processing
	Sector* const pSector = m_SectorGrid.GetSector( coordinates );
	grid.Grid[ SectorGrid3x3::CENTER ] = INVALID_SECTOR_INDEX;		// Center ����

	pObject->OnLeaveField();

	ObjectGameFieldAutoClosure autoclosure( pObject, static_cast<GameField*>(this) );

	// ������ ������Ʈ ����Ʈ���� ����
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


	//	�ֺ� ���͸� ó���Ѵ�. Center�� �� �������� ���ŵǾ���.
	FOREACH_ARRAY( const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray )
	{
		if( INVALID_SECTOR_INDEX == sector_idx )
			continue;

		Sector* const pSector = m_SectorGrid.GetSector( sector_idx );

		//if( IsUsingViewPort() )
		//	���� ������ ���ϸ�, view_port�� ������� �ʴ� ��� ��ü ���͸� ó���ϴ� ���¿���.
		//	�� ��ü ���͸� �� ó���߾������� Ÿ���� �� ��.
		//	�ϴ� ���ɼ��� OldSectoró������ ������ ���� �� �ִ�.

		pSector->OnLeaveThisToNeighbor( pObject );
		pSector->UpdateReferenceCount( pObject, FALSE );
	}

	return TRUE;
}

VOID Field::ChangeSector( Object* pObject )
{
	if( !IsValidState(pObject) )
	{
		SUNLOG( eCRITICAL_LOG, "[EnterField] �߸��� �����̴�." );
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
	if( pOldSector )	// ���� ���Ϳ��� ĳ���� ����
		pOldSector->Remove(pObject);
	//ASSERT( new_index == grids.CurrentSectors[SectorGrid3x3::CENTER] );
	Sector* const pNewSector =	GetSector( new_index );
	if( pNewSector )	// ���ο� ���Ϳ� ĳ���� �߰�
		pNewSector->AddObject( pObject );


	//	������ ������ ���� ó��
	SectorGrid3x3& rLeave = grids.LeaveSectors;
	FOREACH_ARRAY( const SECTOR_IDX sector_idx, rLeave.Grid, SectorGrid3x3::GridArray )
	{
		if( sector_idx == INVALID_SECTOR_INDEX )
			continue;

		Sector* const pSector = GetSector( sector_idx );
		//	if( IsUsingViewPort() )
		//	- �̹����� ����������. Enter, Leave�� ���� ó���ϰ� �ִ�... ���� �Ҿ�������. �� �̷��� �߾���?

		pSector->OnLeaveThisToNeighbor( pObject );		//	ĳ���Ͱ� �������� ���Ϳ� �˷��ش�.
		pSector->OnLeaveNeighborToThis( pObject );		//	������ ������ ������ ĳ���ͷκ��� �����Ѵ�.
		pSector->UpdateReferenceCount( pObject, FALSE );
	}

	//	�űԷ� �����ϴ� ���� ó��
	SectorGrid3x3& rEnter = grids.EnterSectors;
	FOREACH_ARRAY( const SECTOR_IDX sector_idx, rEnter.Grid, SectorGrid3x3::GridArray )
	{
		if( sector_idx == INVALID_SECTOR_INDEX )
			continue;

		Sector* const pSector = GetSector( sector_idx );
		//	if( IsUsingViewPort() )
		//	- �̹����� ����������. Enter, Leave�� ���� ó���ϰ� �ִ�... ���� �Ҿ�������. �� �̷��� �߾���?

		pSector->OnEnterThisToNeighbor( pObject );		// ���ο� ĳ���Ͱ� ���������� ���Ϳ� �˷��ش�.
		pSector->OnEnterNeighborToThis( pObject );		// ������ ������ ���� ĳ���Ϳ��� �˷��ش�.
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
    //  -   �� ��°�� ��� ������ ���� ������ �Ǿ� ���ȴ�. �ϴ� ���� ���� ������ �� �߰� �����ϴ� ����� ���ϱ�� �Ѵ�.
    //  Normal status�� �׳� ����
    const DWORD ERRCODE_INVALID_USAGE = 100;
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak(pCharacter && pwvDest, ERRCODE_INVALID_USAGE);

        // ĳ���Ͱ� �̵������� �������� üũ
        //  can_move    can_move_e
        //      0           0       0
        //      0           1       1       
        //      1           0(X)    1(not_process)
        //      1           1(X)    1(not_process)

        BOOLEAN can_move   = !!pCharacter->CanMove();
        BOOLEAN can_move_e = can_move ? can_move : !!pCharacter->CanMove(eException);

        //  ���� ���� �̵��� ��쵵 ����
        if ((can_move | can_move_e) == 0)
            return false;

        // �̵������� ��ǥ���� üũ
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
        //// (NOTE) �̵� �Ұ� Ÿ�� ���� �����ϰ� ���� �� �ִ�.
        //if (disable_attr & cur_tile_attr)
        //    return false;

        static const SERVER_COMMON_OPTION& stServerOptionInfo =
            ServerOptionParserEx::Instance()->GetServerOption();
        static const float fAcceptalble_lenQ2 = (float)(stServerOptionInfo.m_wFINDPATH_MAX_DISTANCE *
                                                        stServerOptionInfo.m_wFINDPATH_MAX_DISTANCE);

        //////////////////////////////////////////////////////////////////////////
        //  ���� �������ʹ� time slicing����� ����ؾ� �Ѵ�.
        // TODO(WAVERIX)

        WORD dest_tile_attr = 0;
        float fZdiff = 0;

        const INT dest_tile = m_pMovingPlanner->GetTileToStand(*pwvDest,
            &fZdiff, stServerOptionInfo.m_fFINDPATH_JUMPLIMIT, 100.0f, &dest_tile_attr);
        if (dest_tile < 0)
            return false;

        //  �� �� ���� �� �Ǵ� ������ �ȵǴ� �� ��ŵ ó��

        // TODO(WAVERIX) ��������� ����.
        //  �� ����(080925) fear���¶��, ����Ÿ�Ϸε� ����ġ���� ����.
        const WORD filtered_attr = (can_move_e > can_move)
                                 ?  PTA_NO_WALK
                                 :  disable_attr;            

        if (filtered_attr & dest_tile_attr)
            return false;

        pwvDest->z -= fZdiff;

        //  ���� ����... ó���� (PTA_ONLY_JUMP) �̻��� �κ��� �ִ�.
        //  �ϴ� ��ȯ �۾��ϴ� ������ �°� �����ϱ��� �� �κ��� �ذ��ϱ�� �Ѵ�.
        const BOOLEAN isNPC = pCharacter->IsEqualObjectKind(NPC_OBJECT);

        //  (Old Comment) ���� ��ã�� ������ ���� ������ �ʹ� �հŸ��� ��ã�� �õ��� ���� �ʴ´�.
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
    //  -   ���� ��� ������ ���� ������ �Ǿ� ���ȴ�. �ϴ� ���� ���� ������ �� �߰� �����ϴ� ����� ���ϱ�� �Ѵ�.
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        //  ĳ���Ͱ� �̵������� �������� üũ
        if ((bForcedMove || pCharacter->CanMove()) == false)
            return false;

        // (CHANGES) add current tile check
        if (nsAI::PlanEventObject::AcceptableTarget(pCharacter, 0, 0) == false) // no check safety
            return false;
        //const int cur_tile = pCharacter->PlanEvent->GetTile();
        //if (cur_tile < 0)
        //    return false;

        //const WORD cur_tile_attr = m_pMovingPlanner->GetAttribute(cur_tile);
        //// (NOTE) �̵� �Ұ� Ÿ�� ���� �����ϰ� ���� �� �ִ�.
        //if ((PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE) & cur_tile_attr)
        //    return false;

        //  �̵������� ��ǥ���� üũ
        const WzVector& rCurPos = *pCharacter->GetPosPtr();
        WzVector wvDestPos = rCurPos;
        wvDestPos += *pwvMoveDistance;

        // TODO(WAVERIX) ��������� ����.
        if (!IsMovableCoordinates(&wvDestPos))
            return false;

        //  �� �� ���� �� �Ǵ� ������ �ȵǴ� �� ��ŵ ó��
        const WORD disable_attr = (PTA_NO_WALK);//| PTA_PLAYER_FALLDOWN_TILE);

        //  �� ����(080925) fear���¶��, ����Ÿ�Ϸε� ����ġ���� ����.
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

				//	30�� �õ���... �ȵǴ� ���� �ȵǴ� ����������...
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
				//	30�� �õ���... �ȵǴ� ���� �ȵǴ� ����������...
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

		//	���⿡ �����ϴ� ���� ���� �߻��� ���
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
				, "[Field::GetAreaRandomStartingVector][FIELDCODE:%u][AreaID:%s]������ �����ϴ�."
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

		// 10�������� �õ��Ѵ�.
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
        SUNLOG( eCRITICAL_LOG, "[EnterField] �߸��� �����̴�." );
        return;
    }

    const SECTOR_IDX idxSector = pPlayer->GetSectorIndex();
    SectorGrid3x3 OUT grid;
    if( FALSE == m_SectorGrid.GetAroundSectors(idxSector, &grid) )
        return;

    //.// Center

    const Sector* const pSector = m_SectorGrid.GetSector( idxSector );
    grid.Grid[ SectorGrid3x3::CENTER ] = INVALID_SECTOR_INDEX;		// Center ����

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