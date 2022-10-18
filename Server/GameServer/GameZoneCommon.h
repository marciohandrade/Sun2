
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
			//ENABLE_SECTOR_VAR_RESOLUTION		= 1<< 0,	// f110531.4L, set to default, Sector Variable Resolution ���� ����
			//ENABLE_FIELD_HIGH_LOAD_BALANCING	= 1<< 1,	// f110531.4L, set to default, ����ȭ ��Ŷ�� ���� �߻��ϴ� �ʵ忡 ���� ��Ŷ �ε座���� ó�� ����
			//////////////////////////////////////////////////////////////////////////
			ENABLE_TRIGGER						= 1<< 3,	// Ʈ���� ó�� ���� ����
			ENABLE_SAVE_LOCATION				= 1<< 4,	// �ش� �ʵ忡���� �÷��̾� ��ġ ���� ���� ����
			ENABLE_CHANGE_MOB_ABILITY			= 1<< 5,	// ���� �÷��̾� ���� ���� ���� �ɷ�ġ ���� ���� ����
			ENABLE_SUPERROOM					= 1<< 6,	// SuperRoom���� ������... PC���� ��� ó�� ���� ����
			ENABLE_PARTY						= 1<< 7,	// ��Ƽ ���� ó�� ���� ����
#ifdef _NA_008012_20150130_RANKING_SYSTEM
			ENABLE_RANKING						= 1<< 8,	// RankingSystemCtl ���� ����
#endif // _NA_008012_20150130_RANKING_SYSTEM
            //ENABLE_NvN_SYNC                     = 1<< 9,    // synchronization control flag based on room related to PvP, GvG, and NvN|added by (WAVERIX) (f100329.2L)
            ENABLE_ITEM_OBTAIN_ALL_PARTY_MEMBER = 1 << 10,  // ��Ƽ�� ������ �������� ȹ���ϴ� ���� - arycoat.2012.02.03
			//////////////////////////////////////////////////////////////////////////
            ENABLE_DOMINATION_CONTROL           = 1<<19,    // ���������� �� ���� added by waverix
			ENABLE_NPC_UPDATE_REF_SKIP			= 1<<20,	// Sector�� ReferenceCount�� ���� UpdateSkip ���� ���� (���� ���� ���Ѵ�. Ư���� ��쿡�� ���)
			ENABLE_SSQ_CONTROL					= 1<<21,	// Ÿ���� ������� Ư�� ��Ʈ��
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
			ENABLE_GROUP_ROOM_BASE						// �Ϲ� Room ���¿��� ���������� ������ ��� (��쿡 ���� ���� ���� �ִ�.), �κ�� ���� ó��
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
