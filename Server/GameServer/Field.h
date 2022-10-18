#pragma once
#ifndef GAMESERVER_FIELD_H
#define GAMESERVER_FIELD_H
//==================================================================================================
/// Field class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2004. 8. 11~
	@history
			- 2006/2/17 : 리팩토링 시작
			NPC 관련 처리 제거
			Pool처리방식변경
			Init()시에 인자로 확장성있는 Field 구성하게 작성
			- 2006/2/20 :
			TriggerManager는 우선 Field에 유지 : Init과 Update만하기 때문에 굳이 외부로 빼서 TriggerMGR를 Find하는 오버헤드를 줄 필요 없음
			- 2006/2/21 : 섹터 그룹 리펙토링 (섹터그룹의 단순화)
			- 2006/2/22 : object의 grid처리 : XXX_Grid.cpp 추가
							
*/
//==================================================================================================


//==================================================================================================
//
#include <misc.h>
#include "SectorGrid.h"
#include "FieldInfo.h"
#include "MapNpc.h"


//==================================================================================================
//
class FieldInfo;		class Sector;
class TriggerManager;	class Map;
typedef DWORD							TRIGGER_MAPNPC_KEY;
typedef STLX_LIST<MapNpc*>				TRIGGER_MAPNPC_LIST;
typedef TRIGGER_MAPNPC_LIST::iterator	TRIGGER_MAPNPC_LIST_ITER;

// CHANGES: f110321.2L, added filter to support extra control in IsMovableCoordinates
struct FiltersForMovableCoordinates
{
    bool skip_check_tile_attr;
    int result_tile_no;
    WzVector* modified_position; // if the field not null, align correct position
};

//==================================================================================================
//

class Field
{
public:		Field();
	virtual	~Field();

	virtual	VOID			Init( FieldInfo* pFieldInfo, bool bPreMapLoading, bool bUsingViewPort, bool bPreRegen );
	virtual	VOID			Release();
	virtual	VOID			Update( DWORD dwDeltaTick );

	virtual	BOOL			EnterField( Object* pObject, const WzVector* pwzVec, int iAngle=-1 );
	virtual	BOOL			LeaveField( Object* pObject );

            // TODO: need research related to __NA_0_20090720_GUILD_RENDER_INFO_MODIFY
            VOID            FirstGuildMemberEnter( Player* pPlayer );
	// 길찾기
			BOOL			FindPath( Character* pChar, WzVector* pwvDest, eCHAR_STATE_TYPE eException = eCHAR_STATE_INVALID );

			BOOL			FindPathThrust( Character *pChar, WzVector *pwvMoveDistance, BOOL bForcedMove=FALSE );
            //_NA001385_20090924_DOMINATION_FINDPATH_ATTACK
            BOOL            CorrectMovableDestPath(
                Character* pCharacter, const WzVector* pwvDest, WzVector* corrected_dest);
    //----------------------------------------------------------------------------------------------
	// 단지 포워드 기능을 하는 함수 
    // (f100623.1L) change an interface to support a multi-checksum checker
    bool CheckMapVersion(EngineFileChecksum* stored_checksums,
                         const EngineFileChecksum& inputed_checksums) const;
	//inline	BOOL			CheckMapVersion( DWORD dwCheckSum, DWORD* OUT pCurCheckSum = NULL ) { return m_pFieldInfo->CheckMapVersion( dwCheckSum, pCurCheckSum ); }

	// 기타
    bool GetRandomPosInAround(const WzVector* query_position,
                              int iDist, // TODO: f110321.2L, changes to more valuable usage
                              WzVector& result_position,
                              int number_of_max_retry = 10);
            //_NA001385_20090924_DOMINATION_~~
            BOOL            GetRandomPosInAroundCheckPath( Character* pCharacter, WORD wMaxNodeCount, const WzVector* pOrgPos,
                                                           INT iDist, WzVector & vResultPos, int iLoop = 10 );
			BOOL			GetAreaRandomStartingVector( WzID id, WzVector& wzVec );
			WzVector		GetRandomStartingVector(BYTE byClass=0);
			BOOL			GetRandomPos( WzVector& wzVec, int iLoop = 1000 );
	FIELDCODE GetFieldCode() const { return m_pFieldInfo->GetFieldCode(); }
	inline	FieldInfo*		GetFieldInfo()			{ return m_pFieldInfo; }
			BOOL			RandomAreaPos( WzVector& vRandomPos, DWORD dwAreaID );
			BOOL			IsWarCastle()			{ return m_pFieldInfo->GetSharedFieldInfo()->IsWarCastle(); }
    // CHANGES: f110321.2L, support more accurate coordinates including z-axis.
    // NOTE: this method is used on run-time service unlike it of FieldInfo.
    bool IsMovableCoordinates(const WzVector* query_position,
                              FiltersForMovableCoordinates* filter = NULL);

	inline	CWorldBase*		GetWorldBase()		{ return m_pFieldInfo->GetWorldBase();	}
	inline	nsAI::MovingPlanner*	GetMovingPlanner()	{ return m_pMovingPlanner; }
	virtual	int				GetPlayerNum()		{ return 0; }

			VOID			ChangeSector( Object* pObject );

public:
	inline	VOID			CheckAndApplyChangeSector( Object* const pObject, const SECTOR_IDX newSectorIndex );
	//inline	VOID			SetActiveRoom();
	inline	Sector*			GetSector( const SECTOR_IDX coordinates ) const	{ return m_SectorGrid.GetSector( coordinates ); };
	inline	SECTOR_IDX		GetSectorIndex( const WzVector* const pPosition ) const;
	inline	DWORD			GetSectorSize() const							{ return m_SectorGrid.SectorCount; }

	inline	bool			IsUsingViewPort() const							{ return m_bViewport; }
	inline	BOOLEAN			IsReady() const									{ return m_pFieldInfo && m_SectorGrid.SectorCount; }

    MAPOBJECT_INFO*         FindMapObjectInfo( DWORD dwObjectID )           { return m_pSharedFieldInfo->FindMapObjectInfo(dwObjectID); }

protected:	VOID			BuildSector();		// GameField의 BuildSector는 제거키로 한다. (Sector생성,제거시 발생하는 패킷이 많다.)
			VOID			DestroySector();
	inline	BOOL			IsValidState( Object* const pObject ) const { __UNUSED(pObject); return !!m_pFieldInfo; }

protected:
	virtual	VOID			SpawnSector( DWORD dwSectorIndex, Object* pObject ) { __UNUSED((dwSectorIndex, pObject)); }

protected:	FieldInfo*				m_pFieldInfo;
			SharedFieldInfo*		m_pSharedFieldInfo;
			INT						m_RoomIndex;			//< 엔진에서 사용하는 Index
			bool					m_bViewport;
			bool					m_bPreMapLoading;
            bool is_war_castle_; // CHANGES: f110308.1L

public:		nsAI::MovingPlanner*	m_pMovingPlanner;		// __NA001166_080922_TASK_SSQ_HUMANOID_AI_01__
	const	SectorGrid&				SectorGridRef;
protected:	SectorGrid				m_SectorGrid;			//	Sector*를 관리하는 정규 섹터 그리드
};


//==================================================================================================
//

inline	VOID
Field::CheckAndApplyChangeSector( Object* const pObject, const SECTOR_IDX newSectorIndex )
{
	const SECTOR_IDX curSectorIndex = pObject->GetSectorIndex();
	if( newSectorIndex == curSectorIndex )
		return;

	pObject->SetOldSectorIndex( curSectorIndex );
	pObject->SetSectorIndex( newSectorIndex );
	ChangeSector( pObject );
}

inline	SECTOR_IDX
Field::GetSectorIndex( const WzVector* const pPosition ) const
{
	if( !FlowControl::FCAssert( m_pFieldInfo ) )
		return INVALID_SECTOR_INDEX;

	return m_SectorGrid.GetSectorIndex( pPosition );
};

// (f100623.1L) change an interface to support a multi-checksum checker
inline bool Field::CheckMapVersion(EngineFileChecksum* stored_checksums,
                                   const EngineFileChecksum& inputed_checksums) const
{
    return this->m_pFieldInfo->CheckMapVersion(stored_checksums, inputed_checksums);
}

#endif //GAMESERVER_FIELD_H