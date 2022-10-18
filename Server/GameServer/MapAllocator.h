//=============================================================================================================================
/// MapAllocator class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2004. 10. 24
	@purpose
		- �ʰ��� Ǯ �Ҵ� ������ �ϴ� Ŭ����
	@history
		- 2006.2.22 : Ŀ���� Ǯ�� ���� 
*/
//=============================================================================================================================

#pragma once

#if(0)

#include <MemoryPoolFactory.h>

class Map;
class MapInfoLoader;
class GameField;
class Sector;
class Zone;
//class SectorGroup;


class MapAllocator
{
public:
	MapAllocator();
	~MapAllocator();

	VOID							Init( DWORD MapPoolSize, DWORD SectorPoolSize, WORD wVillageSectorSize, WORD wRoomSectorSize );
	VOID							Release();

	Map *							AllocMap( MAPCODE MapCode, BOOL bUsingViewPort, BOOL bUsingPreLoading );
	VOID							FreeMap( Map * pMap );

private:
	MapInfoLoader *						m_pMapInfoLoader;
	CMemoryPoolFactory<Map> *			m_pMapPool;
	CMemoryPoolFactory<Sector> *		m_pSectorPool;
//	CMemoryPoolFactory<SectorGroup> *	m_pSectorGroupPool;

	CMemoryPoolFactory<GameField> *		m_pFieldPool;
};

extern MapAllocator g_MapAllocator;






#endif






