//=============================================================================================================================
///  CAreaTileHelper
/**
@author	Kim Ji Wan < kjws78@webzen.co.kr >
@since	2007. 5. 23
@remarks
- �������� Ÿ���� �ε�,���̺� 
@note
- Ÿ���� ����� ���� ������ ��� ������ �ٿ�� �ڽ��� ���� Ÿ���� ������Ʈ �ε����� �Բ� �ε�,���̺� �Ѵ�.
- 
*/
//=============================================================================================================================
#pragma once
#include <SolarHashTable.h>

struct _AREA_TILE_INFO
{
	INT		iAreaID;
	WORD	wNumberOfTiles;	
	INT*	piTileIndex;

	_AREA_TILE_INFO() : iAreaID(0), wNumberOfTiles(0), piTileIndex(NULL) {}
};

class AREA_TILE_INFO : public _AREA_TILE_INFO
{
public:
	AREA_TILE_INFO() {}
	virtual ~AREA_TILE_INFO()
	{ 
		if( piTileIndex ) 
			delete [] piTileIndex; 
	}
};

typedef util::SolarHashTable<AREA_TILE_INFO *>::iterator	AREATILEINFO_ITERATOR;


class CDiscAccess;

class CAreaTileHelper 
{
public:
	CAreaTileHelper();
	~CAreaTileHelper();	

	BOOL LoadAreaTile( TCHAR* szFileName, CDiscAccess* pDiscAccess = NULL );
	BOOL SaveAreaTile( TCHAR* szFileName );

	inline AREATILEINFO_ITERATOR Begin()
	{
		return m_pAreaTileInfoHashTable->begin(); 
	}
	
	inline AREATILEINFO_ITERATOR End()	
	{
		return m_pAreaTileInfoHashTable->end(); 
	}
	
	inline void	SetFirst()	
	{	
		m_pAreaTileInfoHashTable->SetFirst();
	}
	
	inline AREA_TILE_INFO* GetNext()
	{
		return m_pAreaTileInfoHashTable->GetNext();
	}

	inline AREA_TILE_INFO*	GetAreaTileInfo( int iAreaID )
	{
		return m_pAreaTileInfoHashTable->GetData( iAreaID );
	}

	inline void AddAreaTileInfo(AREA_TILE_INFO*	pInfo,DWORD dwKey)
	{
		m_pAreaTileInfoHashTable->Add( pInfo, dwKey);
	}

	inline DWORD	GetTotalTileCount() { return m_TotalTileCount; }

protected:
	util::SolarHashTable<AREA_TILE_INFO *>*	m_pAreaTileInfoHashTable;			
	DWORD									m_TotalTileCount;
};

