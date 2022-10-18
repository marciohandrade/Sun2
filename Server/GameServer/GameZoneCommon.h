
#pragma once

#define __DECL_GAMEZONEPOOL(_ClassType)
#define __IMPL_GAMEZONEPOOL(_ClassType)


#define WzVector_FiniteCheck( _wzVectorPtr )	\
	( _finite((_wzVectorPtr)->x) && _finite((_wzVectorPtr)->y) && _finite((_wzVectorPtr)->z) )


class sPOLICY_ZONE
{
public:	typedef DWORD							POLICY_T;

		enum eFLAG {
			DISABLE_ALL							= 0,
			//////////////////////////////////////////////////////////////////////////
			//ENABLE_SECTOR_VAR_RESOLUTION		= 1<< 0,	// f110531.4L, set to default, Sector Variable Resolution 적용 여부
			//ENABLE_FIELD_HIGH_LOAD_BALANCING	= 1<< 1,	// f110531.4L, set to default, 동기화 패킷이 많이 발생하는 필드에 대한 패킷 로드벨런싱 처리 여부
			//////////////////////////////////////////////////////////////////////////
			ENABLE_TRIGGER						= 1<< 3,	// 트리거 처리 적용 여부
			ENABLE_SAVE_LOCATION				= 1<< 4,	// 해당 필드에서의 플레이어 위치 저장 적용 여부
			ENABLE_CHANGE_MOB_ABILITY			= 1<< 5,	// 입장 플레이어 수에 따른 몬스터 능력치 재계산 적용 여부
			ENABLE_SUPERROOM					= 1<< 6,	// SuperRoom으로 설정된... PC방일 경우 처리 적용 여부
			ENABLE_PARTY						= 1<< 7,	// 파티 관련 처리 적용 여부
#ifdef _NA_008012_20150130_RANKING_SYSTEM
			ENABLE_RANKING						= 1<< 8,	// RankingSystemCtl 적용 여부
#endif // _NA_008012_20150130_RANKING_SYSTEM
            //ENABLE_NvN_SYNC                     = 1<< 9,    // synchronization control flag based on room related to PvP, GvG, and NvN|added by (WAVERIX) (f100329.2L)
            ENABLE_ITEM_OBTAIN_ALL_PARTY_MEMBER = 1 << 10,  // 파티원 전원이 아이템을 획득하는 지역 - arycoat.2012.02.03
			//////////////////////////////////////////////////////////////////////////
            ENABLE_DOMINATION_CONTROL           = 1<<19,    // 지역점령전 룰 설정 added by waverix
			ENABLE_NPC_UPDATE_REF_SKIP			= 1<<20,	// Sector의 ReferenceCount에 의한 UpdateSkip 무시 여부 (부하 무시 못한다. 특수한 경우에만 사용)
			ENABLE_SSQ_CONTROL					= 1<<21,	// 타락한 사원만의 특수 컨트롤
			//////////////////////////////////////////////////////////////////////////
			ENABLE_GROUP_VILLAGE
				= ENABLE_TRIGGER
				| ENABLE_SAVE_LOCATION
				| ENABLE_PARTY
				,
			ENABLE_GROUP_HUNTING_FIELD
				= ENABLE_TRIGGER
				| ENABLE_PARTY
				,
			ENABLE_GROUP_ROOM_BASE						// 일반 Room 상태에서 공통적으로 가지는 기능 (경우에 따라 빠질 수도 있다.), 로비는 별도 처리
				= ENABLE_TRIGGER
				| ENABLE_CHANGE_MOB_ABILITY
				| ENABLE_SUPERROOM
#ifdef _NA_008012_20150130_RANKING_SYSTEM
				| ENABLE_RANKING
#endif // _NA_008012_20150130_RANKING_SYSTEM
				| ENABLE_PARTY
				,
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
            ENABLE_GROUP_SPAFIELD
                = ENABLE_TRIGGER
                | ENABLE_SAVE_LOCATION
                | ENABLE_PARTY
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
			//////////////////////////////////////////////////////////////////////////
		};

public:
	const	POLICY_T&		FIELD;

private:	POLICY_T		m_Field;		// FIELD

public:		sPOLICY_ZONE() : FIELD(m_Field), m_Field( DISABLE_ALL ) {}
			inline	VOID			Clear() { m_Field = DISABLE_ALL; }
			inline	VOID			SetField( const POLICY_T flags ) { m_Field = (POLICY_T)flags; }
			inline	POLICY_T		GetField() const { return m_Field; }
			inline	BOOLEAN			IsChecked( const eFLAG flag ) const { return !!(m_Field & flag); }
    __DISABLE_COPY(sPOLICY_ZONE);
};

//==================================================================================================

struct IObjectFinder
{
    virtual bool operator()(Object* object) const = 0;
};

#if SUN_CODE_SAMPLE // Sample to use IObjectFinder
/*
void Sample_for_IObjectFinder_usage()
{
    struct ObjectFinder : public IObjectFinder {
        ObjectFinder(DWORD item_key) : item_key_(item_key) {}
        virtual bool operator()(Object* object) const {
            if (object->GetObjectKey() == item_key_) {
                if (object->IsDeleteReserved()) {
                    return false;
                }
                return true;
            }
            return false;
        }
        DWORD item_key_;
    } object_finder(dwFieldItemObjectKey);
    Item* pItem = pField->SearchNeighborItem(pPlayer->GetSectorIndex(), &object_finder);
};
*/
#endif

//==================================================================================================
