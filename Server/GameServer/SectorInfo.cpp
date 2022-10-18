#include "StdAfx.h"
#include ".\sectorinfo.h"


SectorInfo::SectorInfo( DWORD index )
:	m_dwSectorIndex ( index )
,	m_NeighborSectorInfo ( MAX_NEIGHBOR_SECTOR_NUM )
{
	m_NeighborSectorInfo.clear();
}

SectorInfo::~SectorInfo()
{}












