#pragma once

#include "NonCharacter.h"
#include "StructInPacket.h"

struct CollectionInfo;
class CollectionStateInfo;

class Collection : public NonCharacter
{

public:
	Collection();
	virtual ~Collection() {}

public:
	BOOL InitCollection( MAPOBJECT_INFO* pMapInfo );
	
	void GetRenderInfo( COLLECT_RENDER_INFO & OUT RenderInfo );
	void GetRenderInfo( COLLECT_PLAYER_RENDER_INFO & OUT RenderInfo );

	void Release();

	void SetStateInfo(CollectionStateInfo* pStateInfo) { m_pStateInfo = pStateInfo; }
	CollectionStateInfo* GetStateInfo() { return m_pStateInfo; }

	//.//
	MAPOBJECT_INFO* GetMapObjectInfo() { return m_pMapObjectInfo;  }

    bool is_active() const { return is_active_; }
    void set_active(bool val) { is_active_ = val; }

public :
	virtual VOID OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle=-1 );
	virtual VOID OnLeaveField();
	virtual VOID OnEnterObject( const Sector & IN rSector );
	virtual VOID OnLeaveObject( const Sector & IN rSector ) { __UNUSED(rSector); }

private:
	MAPOBJECT_INFO*			m_pMapObjectInfo;		// 채집정보	(맵툴 정보)	
	CollectionStateInfo*	m_pStateInfo;			// 채집중, 리스폰중 정보
    bool is_active_;
};

