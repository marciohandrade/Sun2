#include "stdafx.h"

#include "GameWarDistributionChart.h"
#include "GameInstanceDungeon.h"
#include "Monster.h"
#include "StatusManager.h"
#include "TriggerManagerEx.h"
#include "SummonManager.h"

#include "MovingPlanner.h"
#include <World/SSQ/SSQHandlerInGame.h>

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<SSQDistributionChart>
//
ClosedSpaceRangeTiles*
SSQDistributionChart::ms_pClosedSpaceRangeTiles = 0;


SSQDistributionChart::SSQDistributionChart()
	: m_pSSQField(0)
	, m_pGameField(0)
	, ClosedSpaceViewBox( m_ViewBoxForClosedSpace )
	, CenterPosition( m_CenterPosition )
	, ClosedSpaceRangeQ2( SSQClosedSpace_DamageRadius*SSQClosedSpace_DamageRadius )			// 奩唳 24.5m
{
	SetVector( &m_CenterPosition, SSQClosedSpaceCenter_X, SSQClosedSpaceCenter_Y, SSQClosedSpaceCenter_Z );
#ifdef _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE
    const FLOAT closedSpaceDamageRange = 24.5f;	// 匐儀 奩唳 24.5m
#else
    const FLOAT closedSpaceDamageRange = 22.0f;	// 匐儀 奩唳 22m
#endif  // _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE

	
	WzVector radius3D = { closedSpaceDamageRange, closedSpaceDamageRange, closedSpaceDamageRange };
	m_ViewBoxForClosedSpace.SetInit3D( &m_CenterPosition, &radius3D );
}

SSQDistributionChart::~SSQDistributionChart()
{
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


GameField*
SSQDistributionChart::SetAndGetSSQField( GameInstanceDungeon* const pSSQField )
{
	GameField* const pField = pSSQField->GetEntryField();

	m_pSSQField			= pSSQField;
	m_pGameField		= pField;
	m_TotalTileNumber	= pField->GetMovingPlanner()->GetNumPathTile();

	return pField;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

class ClosedSpaceRangeTiles
{
	typedef SSQDistributionChart	SSQDC;
public:
	struct ArrayInfo
	{
		DWORD		Size;
		WORD*		pArray;
	};

	const	ArrayInfo&	ClosedSpaceTiles;

	inline	INT			GetUnusefulTiles() { return m_UnusefulTiles; }

private:	ArrayInfo	m_csTiles;
			INT			m_UnusefulTiles;

public:		ClosedSpaceRangeTiles(
				const INT max_tiles
				, GameField* pGameField
				, const SSQDC::ViewBoxForClosedSpace& ClosedSpaceViewBox
				, const WzVector& CenterPosition
				, const FLOAT ClosedSpaceRangeQ2
				)
				: ClosedSpaceTiles( m_csTiles )
			{
				m_csTiles.Size					= (DWORD)max_tiles;
				const SIZE_T sizeof_TileWeight	= sizeof(WORD)*m_csTiles.Size;
				m_csTiles.pArray				= (WORD*)TAllocMalloc( sizeof_TileWeight );
				ZeroMemory( m_csTiles.pArray, sizeof_TileWeight );

				using namespace nsAI;
				MovingPlanner* const pMovingPlanner = pGameField->GetMovingPlanner();

				for( DWORD loop=2 ; loop ; --loop )
				{
					for( INT iTile=0 ; iTile<max_tiles ; ++iTile )
					{
						if( SSQDC::WEIGHT_CLOSEDSPACE == m_csTiles.pArray[iTile] )
							continue;
						WzVector pos = pMovingPlanner->GetRandomPosInTile( iTile );

						const BOOLEAN isFind1 = !(VectorLength2( &(CenterPosition-pos) ) > ClosedSpaceRangeQ2);
						if( isFind1 )
							m_csTiles.pArray[iTile] = SSQDC::WEIGHT_CLOSEDSPACE;
					}
				}
			}

			~ClosedSpaceRangeTiles()
			{
				if( m_csTiles.pArray )
					TAllocFree( m_csTiles.pArray );
				m_csTiles.Size = 0;
				m_csTiles.pArray = 0;
			}

//private:	VOID		_Calulation(
//							GameField* pGameField
//							, const SSQDC::ViewBoxForClosedSpace& rView
//							, const WzVector& CenterPosition
//							, const FLOAT ClosedSpaceRangeQ2
//							)
};

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


INT
SSQDistributionChart::RandomDistribution( const SSQInfo* const pSSQInfo, const MatchCheckCodeFilters* const pCheckFilter )
{
	if( !(m_pSSQField && m_SectorCount) )
		return 0;

	nsAI::MovingPlanner* const pMovingPlanner = m_pGameField->GetMovingPlanner();

	ClosedSpaceRangeTiles::ArrayInfo weight_for_sector;
	{
		weight_for_sector.Size			= m_SectorCount;
		const SIZE_T sizeof_SectorWeight= sizeof(WORD)*weight_for_sector.Size;
		weight_for_sector.pArray		= (WORD*)TAllocMalloc( sizeof_SectorWeight );
		ZeroMemory( weight_for_sector.pArray, sizeof_SectorWeight );
	}

	ClosedSpaceRangeTiles::ArrayInfo weight_for_tile;
	{
		weight_for_tile.Size			= (DWORD)m_TotalTileNumber;
		const SIZE_T sizeof_TileWeight	= sizeof(WORD)*weight_for_tile.Size;
		weight_for_tile.pArray			= (WORD*)TAllocMalloc( sizeof_TileWeight );
		ZeroMemory( weight_for_tile.pArray, sizeof_TileWeight );
	}

	typedef GameField::_CHARACTER_HASH	_CHARACTER_HASH;
	const _CHARACTER_HASH& rCharacters	= m_pGameField->GetRefCharacterList();
	const DWORD character_num			= (DWORD)rCharacters.size();	// ASSERT( character_num );
		  DWORD _player_num				= (DWORD)m_pGameField->GetRefPlayerList().size();
		  ASSERT( _player_num < SSQ_MAX_CHARACTERS );
	const DWORD player_num				= min( _player_num, SSQ_MAX_CHARACTERS );

	DWORD weight_sum_of_tile	= 0;

    // (CHANGES) (100107) (WAVERIX) add player random warp
    // (NOTE) ж晦 WEIGHT_PLAYER(5)->2朝 譆模и 錶Щ陛 楠渾ж啪 檜瘀雖朝 匙籀歲 嶸紫ж晦 嬪л.
	FOREACH_CONTAINER( const _CHARACTER_HASH::value_type& rCharacter, rCharacters, _CHARACTER_HASH )
	{
		Character* const pCharacter	= rCharacter.second;
		const INT iTileOfChar		= pCharacter->GetPathExplorer()->GetTile();
		if( !( (DWORD)iTileOfChar < weight_for_tile.Size ) )
			continue;
		const DWORD sector_index	= GetSectorIndex( pCharacter->GetPosPtr() );

        const WORD weight = pCharacter->IsEqualObjectKind(PLAYER_OBJECT) ? 2 //WEIGHT_PLAYER
                                                                         : WEIGHT_MONSTER;

		++weight_sum_of_tile;
		weight_for_tile.pArray[iTileOfChar]		+= weight;
		weight_for_sector.pArray[sector_index]	+= weight;
	}

	const SSQInfo::SUMMONDED_NPC_INFO& rNpcInfo = pSSQInfo->m_SummonedNpcInfo;
	const DWORD npc_list_count	= rNpcInfo.Count;

	{	//	(For Test)	擁蟾 嶸盪陛 橈朝 鼻�窒□� 檜 瑞ず檜 褒ч腎賊 寰腆 匙檜棻
		weight_sum_of_tile = weight_sum_of_tile ? weight_sum_of_tile : 1;
	};

	//	Numeric Formula
	#define WEITGHT_AVG_CAL( numerator, denominator )	\
		(DWORD)( ( ( (2*(numerator))+(denominator) )/(denominator) ) + 1 )

	const DWORD	weight_sum_of_sector	= (DWORD)(weight_sum_of_tile/60);
	DWORD		available_tile_num		= weight_for_tile.Size;
	DWORD		weight_avg_of_tile		= WEITGHT_AVG_CAL(weight_sum_of_tile, available_tile_num );


	static ClosedSpaceRangeTiles CSRT(
									weight_for_tile.Size
									, m_pGameField
									, ClosedSpaceViewBox
									, CenterPosition
									, ClosedSpaceRangeQ2
									);

	ms_pClosedSpaceRangeTiles = &CSRT;
	available_tile_num -= (DWORD)CSRT.GetUnusefulTiles();
	CopyMemory( weight_for_tile.pArray, CSRT.ClosedSpaceTiles.pArray, sizeof(WORD)*weight_for_tile.Size );

	RandomNumberGenerator::AddRandomNumberEntropy( character_num );

    _CHARACTER_HASH::const_iterator character_it = rCharacters.begin();
    _CHARACTER_HASH::const_iterator character_itend = rCharacters.end();

	const INT	loop_limit	= SSQ_MAX_CHARACTERS - player_num;
	DWORD		loop		= 300;
	INT			mob_num		= 0;
    BOOLEAN skip_player_warp = false;

	for( ; loop && mob_num<loop_limit ; --loop )
	{
		DWORD		tile_no			= -1;
		WORD*		pTileWeight		= 0;
		WORD*		pSectorWeight	= 0;
		WzVector	regen_pos;

		for( BOOLEAN bAccept = FALSE ; !bAccept ; )
		{
			tile_no		= RandomNumberGenerator::GetRandomNumberRange( 0, weight_for_tile.Size );
			pTileWeight	= &weight_for_tile.pArray[tile_no];

			if( (DWORD)*pTileWeight > weight_avg_of_tile )
				continue;

			regen_pos = pMovingPlanner->GetRandomPosInTile( tile_no );
			const DWORD sector_index = GetSectorIndex( &regen_pos );
			pSectorWeight = &weight_for_sector.pArray[sector_index];

			if( (DWORD)*pSectorWeight > weight_sum_of_sector )
				continue;

			bAccept = TRUE;
		}

		const INT tile_attr	= pMovingPlanner->GetAttribute( tile_no );
		if( DISABLE_TILE_ATTR & tile_attr )
		{
			*pTileWeight	= WEIGHT_DISABLE;
			*pSectorWeight	= WEIGHT_DISABLE;
			continue;
		}

		// caching脹 薑爾煎 檜嘐 п唸腎歷棻.
		////if( VectorLength2( &(CenterPosition-regen_pos) ) < ClosedSpaceRangeQ2 )
		//if( ClosedSpaceViewBox.IsValidCoordinateXYZ( &regen_pos ) )
		//{
		//	*pTileWeight	= WEIGHT_CLOSEDSPACE;
		//	*pSectorWeight	= WEIGHT_CLOSEDSPACE;
		//	--available_tile_num;
		//	continue;
		//}

        // (CHANGES) (100107) (WAVERIX) add player random warp
        if (skip_player_warp == false && (skip_player_warp = character_it != character_itend))
        { 
            Character* character = character_it->second;
            BOOLEAN is_player = !!character->IsEqualObjectKind(PLAYER_OBJECT);
            m_pGameField->TeleportObject(character, &regen_pos, true);
            ++character_it;
            *pTileWeight += is_player ? WEIGHT_PLAYER : WEIGHT_MONSTER;
            *pSectorWeight += is_player ? WEIGHT_PLAYER : WEIGHT_MONSTER;
            continue;
        }

		*pTileWeight		+= WEIGHT_MONSTER;
		*pSectorWeight		+= WEIGHT_MONSTER;

		++weight_sum_of_tile;
		weight_avg_of_tile	= WEITGHT_AVG_CAL(weight_sum_of_tile, available_tile_num);

		const MONSTERCODE selected_npc = pSSQInfo->GetRandomRegenMonster();
		//const DWORD selected_npc = RandomNumberGenerator::GetRandomNumberRange( 0, npc_list_count );
		//// <- 澀 й 匙... (�捕�煎 啗骯脹棻.
		// TODO(WAVERIX)
		//const MONSTERCODE npcCode	= rNpcInfo.List[selected_npc].m_NPCCode;
		Monster* const pMonster		= m_pGameField->CreateMonster( selected_npc, &regen_pos );
		// <- SSQMonster 橾 匙檜棻.
		if( !pMonster )
		{
			SUNLOG( eCRITICAL_LOG
				, "Can't Create Monster { MC:%u FC:%u MOB:%u } X:Y:Z = { %.2f : %.2f : %.2f }"
				, m_pSSQField->GetMapCode(), m_pGameField->GetFieldCode(), selected_npc
				, regen_pos.x, regen_pos.y, regen_pos.z );
			continue;	// 鼠и 瑞Щ 陛棟撩擎 膩舒п 舒濠.
		}

		StatusManager* const pStatusManager = pMonster->GetStatusManager();
		pStatusManager->Status.On_FilterControl( TRUE, TRUE, pCheckFilter );
		pStatusManager->ChangeInterState( eCHAR_STATE_SSQ_CTRL_BLOCK_ATTACK, TRUE );
		// last
		++mob_num;
	}

	{	// apply closed space tile weight
		const WORD*			pIt		= weight_for_tile.pArray;
		const WORD* const	pItEnd	= weight_for_tile.pArray + weight_for_tile.Size;

		typedef nsAI::PlanCmd	PlanCmd;
		for( INT iTileNo=0 ; pIt!=pItEnd ; ++pIt, ++iTileNo )
		{
			if( WEIGHT_CLOSEDSPACE != *pIt )
				continue;

			const PlanCmd::TileWeightArg args(
				PlanCmd::eTileWeight_SetWeight
				, iTileNo
				, nsAI::eWeight_ClosedSpace
				);
			pMovingPlanner->PathPlan_Command( args );
		}
	};

	TAllocFree( weight_for_sector.pArray );
	TAllocFree( weight_for_tile.pArray );

	return mob_num;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


BOOLEAN
SSQDistributionChart::Earthquake_Indication(
							const SSQPattern* const pSSQPattern, const BYTE step_array_index
							)
{
	if( !(pSSQPattern->m_BanLiftStep > step_array_index) )
		return FALSE;

	typedef nsAI::PlanCmd	PlanCmd;
	nsAI::MovingPlanner* const pMovingPlanner = m_pGameField->GetMovingPlanner();

	typedef SSQInfoParser::SSQTILEINFO_TABLE	SSQTILEINFO_TABLE;
	const SSQTILEINFO_TABLE& rPatternInfos		= SSQInfoParser::Instance()->GetRefSSQPatternInfo();

	const SSQPattern::sSINGLE_STEP& rTileInfo	= pSSQPattern->m_StepGroup[step_array_index];

	const SSQPattern::OBJECT_ID_T* pIt			= &rTileInfo.m_List[0];
	const SSQPattern::OBJECT_ID_T* const pEnd	= &rTileInfo.m_List[rTileInfo.m_ListSize];
	for( ; pIt!=pEnd ; ++pIt )
	{
		const SSQPattern::OBJECT_ID_T obj_id = *pIt;
		SSQTILEINFO_TABLE::const_iterator itFind = rPatternInfos.find( obj_id );
		if( itFind == rPatternInfos.end() )
			continue;

		const _AREA_TILE_INFO& rTiles = itFind->second;

		const INT* pIt			= &rTiles.piTileIndex[0];
		const INT* const pEnd	= &rTiles.piTileIndex[rTiles.wNumberOfTiles];
		for( ; pIt!=pEnd ; ++pIt )
		{
			const INT tileID = *pIt;
			ASSERT( m_TotalTileNumber > (DWORD)tileID );

			const PlanCmd::TileWeightArg args(
				PlanCmd::eTileWeight_SetWeight
				, tileID
				, nsAI::eWeight_Earthquake_Indi
				);
			pMovingPlanner->PathPlan_Command( args );
		}
	}
	return TRUE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

BOOLEAN
SSQDistributionChart::Earthquake(
							const SSQPattern* const pSSQPattern, const BYTE step_array_index
							, nsSSQ::MSGSUB_SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION& INOUT rMsgFPC
							)
{
	if( !(pSSQPattern->m_BanLiftStep > step_array_index) )
		return FALSE;

	nsAI::MovingPlanner* const pMovingPlanner = m_pGameField->GetMovingPlanner();

	typedef SSQInfoParser::SSQTILEINFO_TABLE	SSQTILEINFO_TABLE;
	const SSQTILEINFO_TABLE& rPatternInfos		= SSQInfoParser::Instance()->GetRefSSQPatternInfo();

	const SSQPattern::sSINGLE_STEP& rTileInfo	= pSSQPattern->m_StepGroup[step_array_index];

	const SSQPattern::OBJECT_ID_T* pIt			= &rTileInfo.m_List[0];
	const SSQPattern::OBJECT_ID_T* const pEnd	= &rTileInfo.m_List[rTileInfo.m_ListSize];
	for( ; pIt!=pEnd ; ++pIt )
	{
		const SSQPattern::OBJECT_ID_T obj_id = *pIt;
		SSQTILEINFO_TABLE::const_iterator itFind = rPatternInfos.find( obj_id );
		if( itFind == rPatternInfos.end() )
			continue;

		const _AREA_TILE_INFO& rTiles = itFind->second;

		const INT* pIt			= &rTiles.piTileIndex[0];
		const INT* const pEnd	= &rTiles.piTileIndex[rTiles.wNumberOfTiles];
		for( ; pIt!=pEnd ; ++pIt )
		{
			const INT tileID = *pIt;
			ASSERT( m_TotalTileNumber > (DWORD)tileID );
			pMovingPlanner->SetAttribute( tileID, PTA_PLAYER_FALLDOWN_TILE );
		}
	}

	{
		rMsgFPC.Count = 0;

		typedef nsSSQ::MSGSUB_SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION::NODE	FPC_NODE;
		FPC_NODE* pIt		= &rMsgFPC.PositionInfo[0];
		FPC_NODE* const pEnd= &rMsgFPC.PositionInfo[_countof(rMsgFPC.PositionInfo)];

		//	Character Control
		typedef GameField::_CHARACTER_HASH	_CHARACTER_HASH;
		const _CHARACTER_HASH& rCharacters	= m_pGameField->GetRefCharacterList();
		FOREACH_CONTAINER( const _CHARACTER_HASH::value_type& rCharacter, rCharacters, _CHARACTER_HASH )
		{
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
            Object* const object = rCharacter.second;
            if (object != NULL && object->IsDeleted())
            {
                const Object::Key key = object->GetObjectKey();
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    key
                );
                continue;
            }
//#endif
			Character* const pCharacter	= rCharacter.second;

			const DWORD curHP = pCharacter->GetHP();
			StatusField& rStatus = pCharacter->GetStatusManager()->Status;
			const BOOLEAN bApplyDead =
				curHP && !( (rStatus.FIELD&rStatus.ENABLE_GM_UNDEAD) || (rStatus.FIELD&rStatus.ENABLE_OBSERVER_MODE) );

			const INT iTileOfChar		= pCharacter->GetPathExplorer()->GetTile();
			if( !( (DWORD)iTileOfChar < m_TotalTileNumber ) )
			{	// abnormal status , dead process
				if( bApplyDead )
				{
					pCharacter->ClearKillerInfo();
					pCharacter->SetHP(0);
					pCharacter->OnDead();
				}
				continue;
			}

			if( pIt != pEnd )
			{
				pIt->ObjectKey	= pCharacter->GetObjectKey();
				pIt->Position	= *pCharacter->GetPosPtr();
				++pIt;
				++rMsgFPC.Count;
			}

			const WORD tile_attr = pMovingPlanner->GetAttribute( iTileOfChar );
			if( tile_attr & PTA_PLAYER_FALLDOWN_TILE )
			{	// dead process
				if( bApplyDead )
				{
					pCharacter->ClearKillerInfo();
					pCharacter->SetHP(0);
					pCharacter->OnDead();
				}
			}
		}
	};

	return TRUE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


BOOLEAN
SSQDistributionChart::Thunderbolt(
							const SSQInfo* const pSSQInfo
							, const BOOLEAN bShowmanship
							, const WzVector* const pRange
							, const BOOLEAN bProcOutRangeClosedSpace, const DWORD max_value
							, nsSSQ::MSGSUB_SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE& INOUT rMsgTTE
							, nsSSQ::MSGSUB_SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS& INOUT rMsgCDP
							)
{
	const DWORD rangeX		= m_Boundaries[SG_X_MAX];
	const DWORD rangeY		= m_Boundaries[SG_Y_MAX];

	const DWORD upperbound	= (min( rMsgTTE.MAX_SIZE, max_value )) + 1;	// ubound-1梱雖 random value檜嘎煎...
	const DWORD minVal		= (min( rMsgTTE.MAX_SIZE, max_value )) >>1;
	ASSERT( (minVal+1) < upperbound );

	typedef RandomNumberGenerator	RNG;
	const DWORD loop		= RNG::GetRandomNumberRange( minVal, upperbound );
	rMsgTTE.Count			= (SSQInfo::COUNT_T)loop;
	WzVector* pPosVector	= &rMsgTTE.Position[0];

	const DWORD minCoord	= bProcOutRangeClosedSpace ? 12500 : 17800 ;
	const DWORD maxCoord	= bProcOutRangeClosedSpace ? 27000 : 22200 ;

	for( DWORD idx=0 ; idx<loop ; )
	{
		//	[127, 270] : x==y
		const FLOAT coordX	= (FLOAT)RNG::GetRandomNumberRange( minCoord, maxCoord /*rangeX*/ )/100.f;
		const FLOAT coordY	= (FLOAT)RNG::GetRandomNumberRange( minCoord, maxCoord /*rangeY*/ )/100.f;

		SetVector( pPosVector, coordX, coordY, 9.0f );

		const BOOLEAN inRangeClosedSpace = ClosedSpaceViewBox.IsValidCoordinateXYZ( pPosVector );
		const BOOLEAN need_skip = bProcOutRangeClosedSpace ? inRangeClosedSpace : !inRangeClosedSpace ;

		if( need_skip )
			continue;

		++idx;
		++pPosVector;
	}

	//////////////////////////////////////////////////////////////////////////
	rMsgCDP.Count = 0;

	if( bShowmanship )
		return TRUE;

	pPosVector = &rMsgTTE.Position[0];

	ViewBoxCubic	viewbox[rMsgTTE.MAX_SIZE];
	ViewBoxCubic*	pAlignedViewBox[rMsgTTE.MAX_SIZE] = { 0, };
	for( DWORD idx=0 ; idx<loop ; ++idx, ++pPosVector )
	{
		ViewBoxCubic* const pView	= &viewbox[idx];
		pAlignedViewBox[idx]		= pView;
		pView->SetInit2D( pPosVector, pRange );
	}

	const DAMAGETYPE damageNPC	= pSSQInfo->m_ThunderboltInfo.m_NpcDamage;
	const DAMAGETYPE damagePC	= pSSQInfo->m_ThunderboltInfo.m_PlayerDamage;

	typedef GameField::_CHARACTER_HASH	_CHARACTER_HASH;
	const _CHARACTER_HASH& rCharacters	= m_pGameField->GetRefCharacterList();
	FOREACH_CONTAINER( const _CHARACTER_HASH::value_type& rCharacter, rCharacters, _CHARACTER_HASH )
	{
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
        Object* const object = rCharacter.second;
        if (object != NULL && object->IsDeleted())
        {
            const Object::Key key = object->GetObjectKey();
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                key
            );
            continue;
        }
//#endif
		Character* const pCharacter	= rCharacter.second;

		const WzVector* const pPosition = pCharacter->GetPosPtr();

		//	Damage Range Processing
		ViewBoxCubic** ppView = &pAlignedViewBox[0];
		for( DWORD count=loop ; count ; --count, ++ppView )
		{
			ViewBoxCubic* const pView	= *ppView;
			if( !( pView && pView->IsValidCoordinateXY( pPosition ) ) )
				continue;

			// by script data
			const sPOLICY_CHARACTER& Policyment = pCharacter->Policyment;
			const BOOLEAN isPlayer = !!(Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD_PLAYER);
			const DAMAGETYPE damage = isPlayer ? damagePC : damageNPC;

			const DWORD curHP = pCharacter->GetHP();
			const DWORD nextHP
				= ( (DWORD)damage < curHP )
				?	curHP - damage
				:	0
				;

			StatusField& rStatus = pCharacter->GetStatusManager()->Status;
			const BOOLEAN isObserver = !!(rStatus.FIELD&rStatus.ENABLE_OBSERVER_MODE);
			const BOOLEAN bApplyDead =
				curHP && !( (rStatus.FIELD&rStatus.ENABLE_GM_UNDEAD) || isObserver );

			if( nextHP )
			{
				pCharacter->SetHP( nextHP );
			}
			else
			{
				if( bApplyDead )
				{
					pCharacter->ClearKillerInfo();
					pCharacter->SetHP(0);
					pCharacter->OnDead();
				}
			}

			if( !isObserver && (rMsgCDP.Count < rMsgCDP.MAX_SIZE) )
			{
				DAMAGE_INFO& rDamage = rMsgCDP.Damages[rMsgCDP.Count];
				rDamage.m_dwTargetKey	= pCharacter->GetObjectKey();
				rDamage.m_wDamage		= damage;
				rDamage.m_dwTargetHP	= pCharacter->GetHP();
				++rMsgCDP.Count;
			}
		}
	}

	return TRUE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


BOOLEAN
SSQDistributionChart::ClosedSpace_RemoveDanger(
								   const SSQInfo* const pSSQInfo
								   )
{
	if( !ms_pClosedSpaceRangeTiles )
		return FALSE;

	nsAI::MovingPlanner* const pMovingPlanner = m_pGameField->GetMovingPlanner();

	// release closed space tile weight
	const ClosedSpaceRangeTiles::ArrayInfo& rArrayInfo = ms_pClosedSpaceRangeTiles->ClosedSpaceTiles;
	const WORD*			pIt		= rArrayInfo.pArray;
	const WORD* const	pItEnd	= rArrayInfo.pArray + rArrayInfo.Size;

	typedef nsAI::PlanCmd	PlanCmd;
	for( INT iTileNo=0 ; pIt!=pItEnd ; ++pIt, ++iTileNo )
	{
		if( WEIGHT_CLOSEDSPACE != *pIt )
			continue;

		const PlanCmd::TileWeightArg args(
			PlanCmd::eTileWeight_SetWeight
			, iTileNo
			, nsAI::eWeight_None
			);
		pMovingPlanner->PathPlan_Command( args );
	}

	return TRUE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


BOOLEAN
SSQDistributionChart::ClosedSpace(
							const SSQInfo* const pSSQInfo
							, const BYTE earthquake_step
							, nsSSQ::MSGSUB_SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE& INOUT rMsgTCE
							)
{
	rMsgTCE.Count = 0;

	const BYTE earthquake_step_rev = min((SSQInfo::MI_EARTHQUAKE_SECTION_MAX-1), earthquake_step);
	// damage = maxHP*(ratio/100)
	const FLOAT damageNPC_ratio	= pSSQInfo->m_ClosedSpaceInfo.m_NpcDamages[earthquake_step_rev]/100.0f;
	const FLOAT damagePC_ratio	= pSSQInfo->m_ClosedSpaceInfo.m_PlayerDamages[earthquake_step_rev]/100.0f;
	//printf( "closed space spec : eq_step %u, ratio_base [%.2f:%.2f], calc_ratio [%.2f:%.2f]\n"
	//	, earthquake_step_rev
	//	, pSSQInfo->m_ClosedSpaceInfo.m_NpcDamages[earthquake_step_rev]
	//	, pSSQInfo->m_ClosedSpaceInfo.m_PlayerDamages[earthquake_step_rev]
	//	, damageNPC_ratio, damagePC_ratio
	//		);
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    // NOTE: f110308.1L,                                    fake, sector 0 is exist
    const SectorGridVR& rSectorGridVR = *m_pGameField->GetSectorGridVR(0, SSQClosedSpace_DamageRadius);

	SectorGrid3x3	grid;
	ViewBoxCubic	viewboxVR;
	rSectorGridVR.GetAroundSectors( &CenterPosition, &grid, &viewboxVR );

	//	橾婪 等嘐雖 輿朝 剪塭 掩檜 撫攪 試盪 籀葬й в蹂 橈棻.
	//GRID3x3_CENTER_PRIOR_SWAP( grid );
	FOREACH_ARRAY( const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray )
	{
		if( INVALID_SECTOR_INDEX == sector_idx )
			continue;

		Sector* const pSector = m_pGameField->GetSector( sector_idx );
        ns_object::SearchNodeConstIterator it = pSector->GetCharacterRoot();
        const ns_object::SearchNodeConstIterator end = pSector->GetCharacterEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            Character* const pCharacter	= link_node->select.character_node.character;
            if (obj_util::IsValid(pCharacter) == false)
            {
                continue;
            }
			const WzVector* const pPosition = pCharacter->GetPosPtr();

            if( !m_ViewBoxForClosedSpace.IsValidCoordinateXY( pPosition )
#ifdef _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE
                || !m_ViewBoxForClosedSpace.IsValidCoordinateXY( pPosition, &CenterPosition )
#endif
                )
                continue;

			// by script data
			const sPOLICY_CHARACTER& Policyment = pCharacter->Policyment;
			const BOOLEAN isPlayer = !!(Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD_PLAYER);

			const FLOAT damageRatio = isPlayer ? damagePC_ratio : damageNPC_ratio;
			const DWORD maxHP = pCharacter->GetMaxHP();
			const DAMAGETYPE damage = DAMAGETYPE(maxHP*damageRatio);

			const DWORD curHP = pCharacter->GetHP();
			const DWORD nextHP
				= ( (DWORD)damage < curHP )
				?	curHP - damage
				:	0
				;

			StatusField& rStatus = pCharacter->GetStatusManager()->Status;
			const BOOLEAN isObserver = !!(rStatus.FIELD&rStatus.ENABLE_OBSERVER_MODE);
			const BOOLEAN bApplyDead =
				curHP && !( (rStatus.FIELD&rStatus.ENABLE_GM_UNDEAD) || isObserver );

			if( nextHP )
			{
				pCharacter->SetHP( nextHP );
			}
			else
			{
				if( bApplyDead )
				{
					pCharacter->ClearKillerInfo();
					pCharacter->SetHP(0);
					pCharacter->OnDead();
				}
			}

			if( !isObserver && (rMsgTCE.Count < rMsgTCE.MAX_SIZE) )
			{
				DAMAGE_INFO& rDamage	= rMsgTCE.Damages[rMsgTCE.Count];
				rDamage.m_dwTargetKey	= pCharacter->GetObjectKey();
                rDamage.m_wDamage		= damage;
				rDamage.m_dwTargetHP	= pCharacter->GetHP();
				++rMsgTCE.Count;
			}
		} // sector loop
	}

	return TRUE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


typedef GameWarSSQItemDistribution	SIDN;

//////////////////////////////////////////////////////////////////////////
//	Ratio Table
const DWORD SSQItemRatioTableCols = 4;
const DWORD SSQItemRatioTableRows = 5;
typedef WORD	SSQItemRatioRow[SSQItemRatioTableCols];

#define _SSQIRU(percent)		((percent)*100)

//////////////////////////////////////////////////////////////////////////
//	歜瞪ж朝 Ы溯檜橫 熱 : �捕� Col 摹鷗晦
static const WORD
cSSQItemRatioSelector[SSQItemRatioTableCols] =
{
	10, 30, 50, 70
};

//////////////////////////////////////////////////////////////////////////
//	歜瞪 Ы溯檜橫熱(fix) + 雖霞 欽啗
static const SSQItemRatioRow
cSSQItemRatioTable[SSQItemRatioTableRows] =
{
	{ _SSQIRU(10)+_SSQIRU( 5), _SSQIRU(10)+_SSQIRU(10), _SSQIRU(10)+_SSQIRU(15), _SSQIRU(10)+_SSQIRU(25) },//Begin	FixRatio 10%
	{ _SSQIRU(10)+_SSQIRU( 5), _SSQIRU(10)+_SSQIRU(10), _SSQIRU(10)+_SSQIRU(15), _SSQIRU(10)+_SSQIRU(25) },//Begin	FixRatio 10%
	{ _SSQIRU(20)+_SSQIRU( 5), _SSQIRU(20)+_SSQIRU(10), _SSQIRU(20)+_SSQIRU(15), _SSQIRU(20)+_SSQIRU(25) },//WarmUp	FixRatio 20%
	{ _SSQIRU(20)+_SSQIRU( 5), _SSQIRU(20)+_SSQIRU(10), _SSQIRU(20)+_SSQIRU(15), _SSQIRU(20)+_SSQIRU(25) },//WarmUp	FixRatio 20%
	{ _SSQIRU(35)+_SSQIRU( 5), _SSQIRU(35)+_SSQIRU(10), _SSQIRU(35)+_SSQIRU(15), _SSQIRU(35)+_SSQIRU(25) },//Climax	FixRatio 35%
};
//////////////////////////////////////////////////////////////////////////


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


GameWarSSQItemDistribution::GameWarSSQItemDistribution()
	: m_pSSQInfo(0)
	, m_InitPlayers(0)
{
	ZeroMemory( &m_DropInfoSpace, sizeof(m_DropInfoSpace) );
}


GameWarSSQItemDistribution::GameWarSSQItemDistribution( const SSQInfo* const pSSQInfo, const DWORD init_players )
	: m_pSSQInfo( pSSQInfo )
	, m_InitPlayers( init_players )
{
	//	�捕� 摹鷗晦
	{
		DWORD idx = 0;
		for( ; idx<SSQItemRatioTableCols ; ++idx )
			if( m_InitPlayers <= cSSQItemRatioSelector[idx] )
				break;

		ASSERT( SSQItemRatioTableCols!=idx );
		m_SelectedCol = ( SSQItemRatioTableCols!=idx ) ? (BYTE)idx : 0;
	}
	m_DropInfoSpace = m_pSSQInfo->m_RewardDropInfo;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


SSQInfo::REWARD_NODE
GameWarSSQItemDistribution::Drop( const BYTE earthquake_step )
{
	const BYTE row_no = min( eRatio_MaxStep, earthquake_step );
	//const eRatio row_no
	//	= value < 2 ?	eRatio_Begin
	//	: value < 4 ?	eRatio_WarmUp
	//	:				eRatio_Climax
	//	;

	const DWORD var_ratio = cSSQItemRatioTable[row_no][m_SelectedCol];
	const DWORD probability = RandomNumberGenerator::GetRandomNumberRange( 0, eRatio_MaxDegree );

	SSQInfo::REWARD_NODE drops = { 0, 0 };
	if( probability > var_ratio )
		return drops;

	const DWORD upperbound = m_DropInfoSpace.Count;
	const DWORD selected_idx = RandomNumberGenerator::GetRandomNumberRange( 0, upperbound );
	SSQInfo::REWARD_NODE& rNode = m_DropInfoSpace.List[selected_idx];
	if( !rNode.m_RewardLimit )
		return drops;

	--rNode.m_RewardLimit;
	drops.m_RewardCode	= rNode.m_RewardCode;
	drops.m_RewardLimit	= 1;

	return drops;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


GameWarObserverMode::GameWarObserverMode( GameInstanceDungeon* const pIndun )
	: m_pField( pIndun->GetEntryField() )
{
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


BOOLEAN
GameWarObserverMode::Enter( const nsSSQ::Rules::eC2G_ReqRule rules, Player* const pPlayer )
{
	using namespace nsGrid;
	using namespace nsSSQ;
	typedef nsSSQ::Observer::Info	Info;
	if( !m_pField )
		return FALSE;

	const DWORD object_key = pPlayer->GetObjectKey();
	//////////////////////////////////////////////////////////////////////////
	//	#1	濠褐檜 集陬棻堅 樓檣棻. -.-
	SECTOR_IDX coordinates = pPlayer->GetSectorIndex();
	SectorGrid3x3 OUT grid;

	const SectorGrid& rSectorGrid = m_pField->SectorGridRef;
	if( !rSectorGrid.GetAroundSectors( coordinates, &grid ) )
		return FALSE;

	//	Fake - Player::OnLeaveField
	{
		TriggerManagerEx* const pTriggerManager = m_pField->GetTriggerManager();
        // (CHANGES) (f100614.4L) add a slot type for a trigger area linker to support a multi-area link
        pPlayer->GetTriggerLinks()->Disconnect(pTriggerManager);
        //pTriggerManager->Disconnected(pPlayer->GetTriggerLink());
	};
	//	Fake - Character::OnLeaveField
	{
		// 濠褐擊 んли 檣蕾播攪曖 NPC菟縑啪 pObject陛 в萄蒂 集陬擠擊 憲溥遽棻.
		AI_MSG_LEAVE_FIELD msgAI;
		msgAI.dwObjectKey = object_key;
		pPlayer->SendToObservers( &msgAI, sizeof(msgAI) );

		SummonManager::Instance()->LeaveSummonNPCs( pPlayer );
	};
	//	Fake - Object::OnLeaveField
	{
		// Sector婦溼 籀葬夤縑 橈戲嘎煎 蝶霾
	};
	//	Fake - Notification
	{
		MSG_CG_SYNC_PLAYER_LEAVE_BRD msgBRD;
		msgBRD.m_byCount = 1;
		msgBRD.m_PlayerKey[0] = (WORD)object_key;

		m_pField->SendPacketAround( coordinates, &msgBRD, msgBRD.GetSize(), object_key );
	};

	//////////////////////////////////////////////////////////////////////////
	//	#2	п渡 Ы溯檜橫蒂 褫盪幗 賅萄煎 撲薑и棻.
	StatusManager* const pStatusManager = pPlayer->GetStatusManager();
	StatusField& rStatus = pStatusManager->Status;
	ISSQHandlerInGame& rHandler = pPlayer->SSQHandler;
	{
		pStatusManager->ChangeInterState( eCHAR_STATE_WAR_CTRL_OBSERVER_MODE, TRUE );

		const Info::eCoordinates first_index =
			(Info::eCoordinates)RandomNumberGenerator::GetRandomNumberRange( Info::eCoord_0, Info::eCoord_Slots );
		rHandler->SetObserverCoordinateIndex( (BYTE)first_index );
		// 嬴, 斜楝堅 爾棲... 避擎 議葛檜橫撿 и渡.
		// 輿滲檜 濠褐擊 跤爾紫煙... npc紫 葆雙陛雖... 摹鷗紫 寰腎紫煙...
		// TODO(WAVERIX)

		const WzVector* const pPos = Observer::Controller::GetPosByIndex( first_index );
		m_pField->TeleportObject( pPlayer, pPos, TRUE );

		return TRUE;
	};
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


BOOLEAN
GameWarObserverMode::ChangeCoordinates(
								const nsSSQ::Rules::eC2G_ReqRule rules
								, Player* const pPlayer
								, const nsSSQ::Observer::Info::eKeyCmd key_cmd )
{
	using namespace nsGrid;
	using namespace nsSSQ;
	typedef nsSSQ::Observer::Info	Info;
	if( !m_pField )
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	//	#2	褫盪幗 賅萄曖 п渡 Ы溯檜橫蒂 棻艇 謝ル煎 營撲薑и棻.
	StatusManager* const pStatusManager = pPlayer->GetStatusManager();
	StatusField& rStatus = pStatusManager->Status;
	ISSQHandlerInGame& rHandler = pPlayer->SSQHandler;
	{
		//if( !(rStatus.FIELD & rStatus.ENABLE_OBSERVER_MODE) )
		//	return;

		const Info::eCoordinates current_index = (Info::eCoordinates)rHandler->GetObserverCoordinateIndex();
		if( !((DWORD)current_index < (DWORD)Info::eCoord_Slots) )
			return FALSE;
		if( !((DWORD)key_cmd < (DWORD)Info::eKeyCmd_Max) )
			return FALSE;

		const Info::eCoordinates next_index = Observer::Controller::GetNextIndex( current_index, key_cmd );
		rHandler->SetObserverCoordinateIndex( (BYTE)next_index );

		const WzVector* const pPos = Observer::Controller::GetPosByIndex( next_index );
		//printf( "set %.2f:%.2f:%.2f\n", pPos->x, pPos->y, pPos->z );
		m_pField->TeleportObject( pPlayer, pPos, TRUE );
	};

	return TRUE;
}


