//=============================================================================================================================
/// SectorInfo class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 2. 22
	@purpose
		- 섹터의 주변 섹터의 정보를 가지기 위해(뷰포트)
	@note
		- 
	@history 
		- 2006.2.20 ~ 2006.2.24 : 리펙토링 FieldInfo.h.cpp에서 분리
*/
//=============================================================================================================================
#pragma once

#include <list>

class SectorInfo;
typedef STLX_VECTOR<SectorInfo*>	SECTORINFO_LIST;
typedef SECTORINFO_LIST::iterator	SECTORINFO_LIST_IT;

class SectorInfo
{
	enum { MAX_NEIGHBOR_SECTOR_NUM = 8, };
public:
	SectorInfo( DWORD index );
	~SectorInfo();	

	inline DWORD				GetSectorIndex()	{ return m_dwSectorIndex;	}

	inline VOID					AddNeighbor( SectorInfo * pSectorInfo ) { ASSERT(pSectorInfo); m_NeighborSectorInfo.push_back( pSectorInfo ); }
	inline SECTORINFO_LIST &	GetNeighborList() { return m_NeighborSectorInfo;	}

private:
	DWORD						m_dwSectorIndex;
	SECTORINFO_LIST				m_NeighborSectorInfo;
};

