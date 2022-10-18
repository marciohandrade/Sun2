// worldextrainfo.h

#ifndef _3DTERRAIN_WORLDEXTRAINFO_H_
#define _3DTERRAIN_WORLDEXTRAINFO_H_

#include "worldpathfinder.h"

class CWorldExtraInfo :
	public CWorldPathFinder
{
public:
	CWorldExtraInfo(void);
	virtual ~CWorldExtraInfo(void);
	virtual void Clear();

	/// @name ����/����
	virtual BOOL Create( BOOL bUse = TRUE, BOOL bUseEdit = FALSE );
	virtual void Destroy( CDrawBase *pDraw);
	virtual void Release( CDrawBase *pDraw, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);
	virtual BOOL Serialize( CWzArchive *pArchive);

	/// @name Ư�� ����
protected:
	CWzArray<WzSpecialArea*> m_arSpecialArea;
public:
	virtual BOOL AddSpecialArea( WzSpecialArea *pSpecialArea);
	virtual BOOL RemoveSpecialArea( WzID wiIdentity);
	virtual BOOL RemoveSpecialAreaByIndex( int iIndex);
	virtual void RemoveAllSpecialArea();
	virtual int GetNumberOfSpecialArea();
	virtual WzSpecialArea* GetSpecialArea( WzID wiIdentity);
	virtual WzSpecialArea* GetSpecialAreaByIndex( int iIndex);
protected:
	int GetIndexOfSpecialArea( WzID wiIdentity);

	/// @name ��ã�� ���� ���� ���/�Ҵ�.
public:
	virtual BOOL ReloadPathInfo( CWzArchive *pArchive);
	
	// MapEditor���� �� �Ӽ� �� ���� ���� Import ��� ���� ����ϴ� �Լ���...
	virtual BOOL ReloadPathInfoFromMapFile(CWzArchive *pArchive);	
	virtual BOOL ReadOnlyMapFileHeader( CWzArchive *pArchive);
	virtual BOOL ReloadPathInfo2_MapFile( CWzArchive *pArchive);
	virtual BOOL ReloadPathInfo2_WMOFileFrom162To166(CWzArchive *pArchive, bool bLoadAll);
	virtual BOOL ReloadPathInfo2_WMOFile( CWzArchive *pArchive, bool bLoadAll);

protected:
	BOOL SerializeLightmapInfo(CWzArchive *pArchive);
	BOOL SerializeAreaInfo(CWzArchive *pArchive);

protected:
	BOOL LoadPathInfo(CWzArchive *pArchive);
	BOOL MatchPathInfo();
	BOOL ReassignPathTileArea();
	BOOL ReassignPathTileAttr();// Ÿ�� �Ӽ�, ���� �Ӽ� ���߱�...
	void GenSamplingPathInfo(CWzArray<WzRay>& waRay,WzVector* pwv,CPathTile* pPathTile,WzVector* pmvNor =NULL);
	void AddSamplingPathInfo(CWzArray<WzRay>& waRay,WzVector* pwv,WzVector* pmvNor);
	void AddSamplingPathInfoEx(CWzArray<WzRay>& waRay,WzVector* pwvStart,WzVector* pwvEnd,WzVector* pmvNor);
	WzVector ResizeTri(WzVector* pwvOut,WzVector* wvIn,float fSize);
	void RemoveTemporaryPathInfo();

	virtual BOOL SerializeExtraAreaInfo( CWzArchive *pArchive, bool bSerializeAll);

	WzVector*			m_pwvVertex;
	DWORD				m_dwNumVertex;
	CPathTile*			m_pOldPathTiles;
	WORD*				m_wPathTileAttr;
	DWORD				m_dwNumPathTiles;
	CWzArray<int*>		m_arNewPathIndex;
};

#endif // _3DTERRAIN_WORLDEXTRAINFO_H_