#ifndef __MAP_UNIT_CACHER_H__
#define __MAP_UNIT_CACHER_H__

//=============================================================================================================================
///  MapUnitCacher
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 1. 28
	@remarks
		- 맵 유닛 ( 오브젝트, 풀 등)을 로드하고 객체 관리하는 작업
	@note
		- 풀만 처리.
		- 
*/
//=============================================================================================================================

#define MAP_GRASS_KEY_START			100000
#define MAP_SPEED_TREE_KEY_START	200000


#pragma once

#include <MemoryPoolFactory.h>
#include <SolarHashTable.h>


class CWzUnitDraw;
class CWzUnit;
class CDrawBase;
class CWzArchive;
class CWorldBase;

class MapWzUnitDraw
{
public:
	enum eUNIT_TYPE
	{
		UT_OBJECT,
		UT_GRASS,
	};

	MapWzUnitDraw() : pUnitDraw( NULL ), dwAttribute(0), wzId( 0 )
	{

	}
	~MapWzUnitDraw() {}
	DWORD				dwUniqueID;
	
	WzVector			wzRot;
	
	WzID				wzId;
	DWORD				dwAttribute;	
	CWzUnitDraw *		pUnitDraw;
	BYTE				byAttr;				//< 풀에서만 사용	
	WzColor				wzColor;			//< 이동되었을 때에는 color를 업데이트 해줘야 한다.
	
	int                 iPhysicsType;

};

class MapUnitCacher
{
	enum 
	{
		CHUNK_OBJECT	= 0x1781,
		CHUNK_GRASS		= 0x1783,
	};
	class MapWzUnit 
	{
	public:
		CWzUnit *	m_pwzUnit;
		WzID		m_wiID;
		int         m_iPhysicsType;
	};

public:
	MapUnitCacher();
	~MapUnitCacher();

	VOID									Init( CDrawBase * pDraw);

	VOID									LoadMapGrassUnit( VOID );		///< 풀은 스크립트 파일이 전역적임 한번만 로딩
	VOID									LoadMapGrassUnitDraw( CWorldBase * pWorldBase );

	VOID									UnloadAll();

	CWzUnit *								GetMapUnit( WzID dwMapUnitID );
	MapWzUnitDraw *							GetMapUnitDraw( DWORD dwID );

	
	VOID									SetHide(BOOL bHide);

	VOID									SetFirst();
	MapWzUnitDraw *							GetNext();


private:
	WzColor									_getTerrainColor( CWorldBase * pWorldBase, WzVector * pWvPos );

	DWORD									m_dwUniqueID;
	util::SolarHashTable<MapWzUnitDraw *> *	m_pMapUnitDrawHashTable;				///< 맵상에 존재하는 풀 객체 ( 랜더링 대상 객체 hash )

	util::SolarHashTable<MapWzUnit *> *		m_pMapUnitHashTable;
	util::CMemoryPoolFactory<MapWzUnit> *	m_pWzUnitPool;

	CDrawBase *								m_pDraw;
};

#endif // __MAP_UNIT_CACHER_H__