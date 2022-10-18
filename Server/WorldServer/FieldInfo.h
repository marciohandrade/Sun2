
#ifndef __FILED_INFO_H__
#define __FILED_INFO_H__

#pragma once


//#include "MapDefine.h"
#include <Linkedlist.h>
#include <SolarHashTable.h>
#include <Struct.h>
#include <vector>
//#include <ProgramCommon/WzArchive.h>
//#include <3DEngine/Common/BoundingVolume.h>
//#include <TypeDefine.h>
#pragma warning(push)
#pragma warning(disable : 4100 4201)
#include <3DEngine/3DTerrain/WorldBase.h>
#pragma warning(pop)


class CWorldBase;
struct WzVector;
class ViewPortManager;

class MAPOBJECT_INFO
{
public:
	MAPOBJECT_INFO(){}
	~MAPOBJECT_INFO(){}
	DWORD				dwMapObjectID;
	WzID				wzId;
	DWORD				dwAttribute;
	WzVector			wvPos;
	WzVector			wzRot;
	WzVector			wzScale;
	WzBoundingVolume	wzBV;
};

class SectorInfo
{
	enum 
	{
		MAX_NEIGHBOR_SECTOR_NUM = 8,
	};

public:
	SectorInfo( DWORD index );
	~SectorInfo();	

	inline DWORD						GetSectorIndex()	{ return m_dwSectorIndex;	}

	VOID								AddNeighbor( SectorInfo * pSectorInfo );
	inline util::CLinkedlist<SectorInfo *> &	GetNeighborList() { return m_NeighborSectorInfo;	}

private:
	DWORD								m_dwSectorIndex;
	util::CLinkedlist<SectorInfo *>			m_NeighborSectorInfo;
};


class FieldInfo
{
	enum 
	{ 
		_MAX_MAPOBJECT_INFO_NUM = 100,
	};

public:
	FieldInfo();
	~FieldInfo();
	
	// �� Field���� ȣ��Ǿ�� �ϴ� �Լ�
	INT										Init();
	VOID									Release( INT Index );

	VOID									Create( const sFIELDINFO * pInfoIn );	//, ViewPortManager *pViewPortManager );
	VOID									Destroy();
	BOOL									Load();
	VOID									EstablishSectorInfo( WORD wSectorSize, BOOL bViewport );

	//get,set
	FIELDCODE								GetFieldCode() { return m_pFieldInfo->FieldCode;	}
	DWORD									GetSectorIndexFor( WzVector* pwzVec ); 
	VOID									GetSectorIndexFor( WzVector * pwzVec, DWORD & dwXIndexOut, DWORD & dwYIndexOut );
	inline  DWORD							GetSectorSize()	{ return m_dwSectorSize;	}
	inline	SectorInfo*						GetSectorInfo( DWORD dwIndex )	{ return m_ppSectorInfo[dwIndex];	}
	inline	CWorldBase*						GetWorldBase() { return m_pWorldBase; }
	inline	DWORD							GetTotalSectorNum() { return m_dwTotalSectorNum;	}
	inline	DWORD							GetSectorXNum() { return m_dwSectorXNum;	}
	inline	DWORD							GetSectorYNum() { return m_dwSectorYNum;	}

	inline	VOID							SetFieldInfo( const sFIELDINFO * pInfoIn ){ m_pFieldInfo = pInfoIn;	}
	inline	const sFIELDINFO *				GetFieldInfo() { return m_pFieldInfo;	}
	BOOL									CheckMapVersion( DWORD dwCheckSum );

	VOID									SetStartingTileIndex();
//	DWORD									GetRandomStartingTileIndex();
	WzSpecialArea *							GetAreaInfo( WzID AreaID ) { return m_pWorldBase->GetSpecialArea( AreaID ); }
	WzSpecialArea *							GetAreaInfoByIndex( INT index  ) { return m_pWorldBase->GetSpecialAreaByIndex(index); }
	util::SolarHashTable<MAPOBJECT_INFO *> *GetMapObjectInfoHashTable() { return m_pMapObjectInfoHashTable;	}
	BOOL									GetViewPort() { return m_bViewport; }
		
	template <class AreaOperator>
	VOID									ForeachArea( AreaOperator & op )
	{
		int num = m_pWorldBase->GetNumberOfSpecialArea();
		for( int i = 0 ; i < num ; ++i )
		{
			WzSpecialArea * pArea = m_pWorldBase->GetSpecialAreaByIndex(i);
			op(pArea);
		}
	}

	BOOL					IsValidCoordinates( WzVector* pwzVec );
private:
	//VOID					_loadAreaInfo( CWzArchive & archive );
	VOID					_loadMapObjectInfo( CWzArchive & archive );
	//VOID					_unloadAreaInfo();
	VOID					_unloadMapObjectInfo();
	//WORD					m_wFieldID;

	BOOL					m_bViewport;			// ����Ʈ ó���� �ϴ� ������

	WzVector				m_wvStartingVector;
	const sFIELDINFO	*	m_pFieldInfo;
	DWORD					m_dwSectorSize;			// ���� ������ ũ��(�������� : ���簢��)
	DWORD					m_dwSectorXNum;			// �ʵ带 �����ϴ� X��ǥ���� ���� ����
	DWORD					m_dwSectorYNum;			// �ʵ带 �����ϴ� Y��ǥ���� ���� ����
	DWORD					m_dwTotalSectorNum;		// �� ���� ����(m_dwSectorXNum * m_dwSectorYNum)

	// ���� �ε����� ����ϱ� ���� ����
	int						m_iShiftMinX;			// �ʻ��� �ּ� X��ǥ�� ���밪
	int						m_iShiftMinY;			// �ʻ��� �ּ� Y��ǥ�� ���밪
	int						m_iShiftMaxX;			// �ʻ��� �ִ� X��ǥ�� ���밪
	int						m_iShiftMaxY;			// �ʻ��� �ִ� Y��ǥ�� ���밪

	//char					m_szFileName[MAX_PATH];
	CWorldBase *			m_pWorldBase;
	SectorInfo **			m_ppSectorInfo;
	// MAP������ ���� ����
	//util::SolarHashTable<AREA_INFO *> * m_pAreaInfoHashTable;
	util::SolarHashTable<MAPOBJECT_INFO *> * m_pMapObjectInfoHashTable;
	STLX_VECTOR<DWORD>		m_StartTiles;
	ViewPortManager			*m_pViewPortManager;
};



class MapInfo
{
	friend class Map;
	friend class MapInfoLoader;
	enum { _MAX_FIELD_IN_MAP_NUM = 10, };
public:
	MapInfo();
	~MapInfo();

	CODETYPE			GetMapCode() { return m_pMapInfo->MapCode; }
	inline	VOID		SetMapInfo( const sMAPINFO * pInfoIn ) { m_pMapInfo = pInfoIn; }
	const sMAPINFO *	GetMapInfo() { return m_pMapInfo;	}

	VOID				AddFieldInfo( FieldInfo * pFieldInfo );
	FieldInfo*			GetFieldInfo( CODETYPE FieldCode )		{ return m_FieldInfoHashTable.GetData( FieldCode ); }

	util::SolarHashTable<FieldInfo *> * GetFieldInfoHashTable() { return &m_FieldInfoHashTable;	}
protected:
	VOID				setFirst() { m_FieldInfoHashTable.SetFirst();	}
	FieldInfo *			getNext() { return m_FieldInfoHashTable.GetNext();	}

private:

	const sMAPINFO *	m_pMapInfo;	
	util::SolarHashTable<FieldInfo *> m_FieldInfoHashTable;
};



#endif // __FILED_INFO_H__
















