//------------------------------------------------------------------------------
/**

맵 오브젝트의 리소스를 관리해주는 메니저.
@author
	유재영
@since
	[6/14/2005]
@remarks
- Application Create / Destory 할때 한번씩만 Create / Destory  
	[6/22/2005]
- RemoveUnitAll() 추가
	[6/22/2005]
*/

#ifndef MAPRESMGR_H
#define MAPRESMGR_H

#include <Singleton.h>
#include <MemoryPoolFactory.h>

#define MAX_MAP_OBJECT 20000

class MapResWzUnit  
{
public:
	CWzUnit *	m_pwzUnit;
	WzID		m_wiID;
	int         m_iRefCount;

#ifdef _DEBUG
    char m_Path[1024];
#endif //_DEBUG

};

typedef stdext::hash_map<DWORD, MapResWzUnit *>	MAP_RES_HASH;
typedef MAP_RES_HASH::iterator					MAP_RES_ITR;
typedef std::pair<DWORD, MapResWzUnit  *>		MAP_RES_PAIR;

typedef stdext::hash_map<DWORD, CWzUnitDraw *>	MAP_DRAW_HASH;
typedef MAP_DRAW_HASH::iterator					MAP_DRAW_ITR;
typedef std::pair<DWORD, CWzUnitDraw *>			MAP_DRAW_PAIR;

class MapResMgr : public util::Singleton< MapResMgr >
{
public:
	MapResMgr(void);
	virtual ~MapResMgr(void);

	void			Create(int iMaxMapObject);
	void			Destory();

public:
	BOOL			AddUnit(DWORD dwID, char * pszResName,BYTE *byAttr = NULL);
	CWzUnitDraw	*	AddDraw(DWORD dwUnitID);
	
	BOOL			RemoveUnit(DWORD dwID);
	void			RemoveUnitAll();
	BOOL			RemoveUnitDraw(CWzUnitDraw * pWzUnitDraw);


    MAP_RES_HASH&	GetResourceMap() { return m_MapResHashmap; }


protected:
	MapResWzUnit *	GetUnit(DWORD dwID);

private:
	MAP_RES_HASH	m_MapResHashmap;

	util::CMemoryPoolFactory<MapResWzUnit> *	m_pMapUnitPool;
};


#define MAPRESMGR()		MapResMgr::Instance()

#endif MAPRESMGR_H
// MAPRESMGR_H