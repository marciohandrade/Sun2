#include "SunClientPrecompiledHeader.h"
#include "mapresmgr.h"
#include "Application.h"
#include "Map.h"

MapResMgr::MapResMgr(void)
{
}

MapResMgr::~MapResMgr(void)
{
	Destory();
}


void MapResMgr::Create(int iMaxMapObject)
{
	m_pMapUnitPool = new util::CMemoryPoolFactory<MapResWzUnit>;
	m_pMapUnitPool->Initialize( iMaxMapObject, iMaxMapObject/2+1 );
}

void MapResMgr::Destory()
{
	if (NULL == m_pMapUnitPool)
		return;

	RemoveUnitAll();

	m_pMapUnitPool->Release();
	delete m_pMapUnitPool;

	m_pMapUnitPool = NULL;
}


void MapResMgr::RemoveUnitAll()
{
	MAP_RES_ITR itr = m_MapResHashmap.begin();

	while( itr != m_MapResHashmap.end() )
	{
		MapResWzUnit * pUnit = static_cast<MapResWzUnit * >(itr->second);
		assert( pUnit );
		g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pUnit->m_wiID );
		m_pMapUnitPool->Free( pUnit );
		itr++;
	}

	m_MapResHashmap.clear();
}


BOOL MapResMgr::AddUnit(DWORD dwID, char * pszResName,BYTE *byAttr)
{
	MAP_RES_ITR itr = m_MapResHashmap.find(dwID);

	MapResWzUnit * pMapUnit = NULL;

	if (itr == m_MapResHashmap.end())
	{
		pMapUnit = (MapResWzUnit *)  m_pMapUnitPool->Alloc();

		DWORD dwLoadStyle = ULS_DEFAULT;

		if (byAttr)
		{
			if (byAttr[0])
			{
#ifdef MAPOBJECT_INSTANCING
				dwLoadStyle |= ULS_INSTANCING;
#endif
			}

		}
        
        // 맵오브젝트는 유닛로드시 퀵로드안하게끔
        dwLoadStyle |= ULS_NO_MULTITHREAD;
        
        if (g_pMap && g_pMap->GetWorldBase()->GetLightmapFileName())
            dwLoadStyle |= ULS_UNIT_LIGHTMAP;
		
		if( dwID != g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit( dwID, pszResName ,TRUE, dwLoadStyle) )
		{
			assert( !"맵 유닛 데이터를 로딩 할 수 없습니다" );
			MessageBoxA(NULL, pszResName, "파일이 엄서요!!", MB_OK);
			return FALSE;
		}

		pMapUnit->m_pwzUnit = g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit( dwID );
		pMapUnit->m_wiID	= dwID;
		pMapUnit->m_iRefCount = 1;

#ifdef _DEBUG
        strcpy(pMapUnit->m_Path, pszResName);
#endif //_DEBUG

		m_MapResHashmap.insert( MAP_RES_PAIR(dwID, pMapUnit) );
	}
	else
	{
		pMapUnit = (MapResWzUnit *)itr->second;
		++pMapUnit->m_iRefCount;
	}

	return TRUE;
}


CWzUnitDraw	* MapResMgr::AddDraw(DWORD dwUnitID)
{
	MAP_RES_ITR itr = m_MapResHashmap.find(dwUnitID);

	if (itr != m_MapResHashmap.end())
	{
		MapResWzUnit* pUnit = static_cast<MapResWzUnit *>(itr->second);
		if (pUnit) 
		{	
			pUnit->m_iRefCount++;

			CWzUnitDraw* pUnitDraw = (CWzUnitDraw *)Func_ContactWzd( CW_NEW_WZUNITDRAW, 0 );
			pUnitDraw->SetWzUnit( pUnit->m_pwzUnit , FALSE);

			return pUnitDraw;
		}
		else
		{
			assert(!"멥오브젝트를 확인해봐라!!");
		}
	}
	else
	{
		assert(!"멥파일을 확인해보기바람!!");
	}

	return NULL;
}


MapResWzUnit * MapResMgr::GetUnit(DWORD dwID)
{
	MAP_RES_ITR itr = m_MapResHashmap.find(dwID);
	if (itr == m_MapResHashmap.end())
	{
		return NULL;
	}
	else
	{
		return (MapResWzUnit *)itr->second;
	}
}


BOOL MapResMgr::RemoveUnit(DWORD dwID)
{
	MAP_RES_ITR itr = m_MapResHashmap.find(dwID);


	if (itr == m_MapResHashmap.end())
	{
		return FALSE;
	}

	MapResWzUnit * pUnit = static_cast<MapResWzUnit *>(itr->second);

	assert(pUnit);

	if (pUnit && pUnit->m_iRefCount > 0)
	{
		pUnit->m_iRefCount--;
		// ref count가 0이되면 없애준다.
		if (pUnit->m_iRefCount == 0)
		{
			g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pUnit->m_wiID );
			m_MapResHashmap.erase(itr);
			m_pMapUnitPool->Free(pUnit);
		}
	}
	else
	{
		assert(!"ResourceCacher::ReleaseItemUnit 잘못된 참조 카운트");
		return FALSE;
	}

	return TRUE;
}

// TODO: 없애자..
BOOL MapResMgr::RemoveUnitDraw(CWzUnitDraw * pWzUnitDraw)
{
	if (!pWzUnitDraw || pWzUnitDraw->GetWzUnit() == NULL)
	{
		return FALSE;
	}
	else
	{
		RemoveUnit( pWzUnitDraw->GetWzUnit()->GetType() );

		g_pSunRenderer->DestroyResourceForWzUnitDraw(pWzUnitDraw );
        Func_ContactWzd( CW_DELETE_WZUNITDRAW, pWzUnitDraw );		
	}

	return TRUE;
}