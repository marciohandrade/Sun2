#include "SunClientPrecompiledHeader.h"
#include "ItemUnitCacher.h"
#include "ResourceManager.h"
#include "GraphicResourceList.h"
#include <SCSlotHeader.h>
#include <float.h>
#include "Application.h"
#include "GlobalFunc.h"

const int RESOURCE_RELEASE_TIME = 300000; //5분이상 참조안하면 완전제거

const int MAX_WZUNIT_CACHE = 120;
const int RESOURCE_RELEASE_CHECK = 10000; //10초에 1번 체크
//const int RESOURCE_RELEASE_TIME = 1800000; //30분이상 참조안하면 완전제거
//
//const int MAX_WZUNIT_CACHE = 120;
//const int RESOURCE_RELEASE_CHECK = 120000; //2분에 1번 체크

ResourceCacher::ResourceCacher()	
{
	m_pResourceUnitHashTable = new util::SolarHashTable<ResourceWzUnit *>;
	m_pResourceUnitHashTable->Initialize( 10000 );

	m_pResourceWaitForFree = new util::SolarHashTable<ResourceWzUnit *>;
	m_pResourceWaitForFree->Initialize( MAX_WZUNIT_CACHE+1 );
	
	m_pWzUnitPool = new util::CMemoryPoolFactory<ResourceWzUnit>;
	m_pWzUnitPool->Initialize( 10000, 500 );

	m_dwSkippingTick = 0;
}

ResourceCacher::~ResourceCacher()
{
	ResourceWzUnit * pWzUnit = NULL;

	if (m_pResourceUnitHashTable) 
	{
		m_pResourceUnitHashTable->SetFirst();
		while( pWzUnit = m_pResourceUnitHashTable->GetNext() )
		{
			g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pWzUnit->m_wiID );
			m_pWzUnitPool->Free( pWzUnit );
		}
		m_pResourceUnitHashTable->RemoveAll();
		delete m_pResourceUnitHashTable;
		m_pResourceUnitHashTable = NULL;
	}

	if (m_pResourceWaitForFree)
	{
		m_pResourceWaitForFree->RemoveAll();
		delete m_pResourceWaitForFree;
		m_pResourceWaitForFree = NULL;

	}

	if (m_pWzUnitPool) 
    {
		m_pWzUnitPool->Release();
		delete m_pWzUnitPool;
	}
}


VOID ResourceCacher::Init()
{
}


VOID ResourceCacher::LoadResource( RESOURCECODE resourceCode ,float fRequestRateResizeTexture, bool pre_load, bool is_npc)
{
	assert(resourceCode);
	assert(resourceCode >= ENGINE_RESOURCE_ID_OFFSET);

	RESOURCE_INFO *pInfo = GraphicResourceList::Instance()->GetItemInfo(resourceCode - ENGINE_RESOURCE_ID_OFFSET);
	assert(pInfo && "잘못된 리소스 코드입니다.");

	// check file
	BOOL bExist = FALSE;
	if (pInfo)
	{
        if (is_npc)//현재는 몬스터
        {
            size_t file_length = strlen(pInfo->szFilename);

            if ((file_length + strlen("_NPC")) < 128)//128은 해당 버퍼 사이즈 맥스 
            {
                TCHAR* temp_string = pInfo->szFilename + (file_length - strlen(".wzu"));
                strcat(temp_string, "_NPC.wzu");
            }
        }

		if( GENERALPARAM->IsUsePackFile() )
		{
			DA_FILE hFile = g_pApplication->GetDiscAccess()->OpenFile(ePFN_RESOURCE, pInfo->szFilename, DA_READ, SM_READ, OD_OPEN_EXISTING);
			if (INVALID_DA_FILE	!= hFile ) 
			{
				bExist = TRUE;
				g_pApplication->GetDiscAccess()->CloseFile( hFile );
			}
		}
		else
		{
			FILE *fp = fopen(pInfo->szFilename,"rb");
			if (fp)
			{
				bExist = TRUE;
				fclose(fp);
			}
		}
	}

	// load file
	#define DEFAULT_MESH_NAME	"Data\\Item\\Item1001.wzu"

	char * filename = NULL;
	if (bExist)
	{
		filename = pInfo->szFilename;
	}
	else
	{
		filename = DEFAULT_MESH_NAME;
		
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("[code: %d] 리소스가 존재하지 않아 %s를 대신 로딩함"), (int)(resourceCode - ENGINE_RESOURCE_ID_OFFSET), filename);
	}

    DWORD load_style = ULS_DEFAULT;

    if (pre_load)
    {
       load_style |= ULS_NO_MULTITHREAD;
    }

	if( resourceCode != g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit( resourceCode, filename,TRUE,load_style,fRequestRateResizeTexture ))
	{
#ifdef _DEV_VER
		MessageBoxA(NULL, "리소스 데이터 Error", filename, MB_OK);
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("[%s][code: %d] 리소스를 로딩 할 수 없습니다"), filename, resourceCode - ENGINE_RESOURCE_ID_OFFSET);
#endif
	}			

	// 임시로 효과 서브타입을 만들어준다.
	CWzUnit * pWzUnit = g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit( resourceCode );

	ResourceWzUnit * pUnit	= (ResourceWzUnit *) m_pWzUnitPool->Alloc();
	if(pUnit)
	{
		pUnit->m_pwzUnit	= pWzUnit;
		pUnit->m_wiID		= resourceCode;
		pUnit->m_iRefCount  = 0;
		pUnit->m_iErrorFlag = 0;
		pUnit->m_iReleaseTime = 0;

		assert( NULL == m_pResourceUnitHashTable->GetData( resourceCode ) );
		m_pResourceUnitHashTable->Add( pUnit, resourceCode );
		
	}
}

CWzUnit * ResourceCacher::GetResourceWzUnit( RESOURCECODE resourceCode ,float fRequestRateResizeTexture, bool bPreLoad /*=false*/, bool bFreeMost /*=true*/, bool is_npc /*=false*/)
{
	if (resourceCode == 0)
	{
		return NULL;
	}

	ResourceWzUnit* pUnit = m_pResourceUnitHashTable->GetData( resourceCode );

	if( pUnit == NULL )
	{
		LoadResource( resourceCode ,fRequestRateResizeTexture, is_npc);
		pUnit = m_pResourceUnitHashTable->GetData( resourceCode );
		pUnit->m_iRefCount = 1;
		pUnit->m_iReleaseTime = 0;		
		pUnit->m_bFreeMost = bFreeMost;
	}
	else 
    {
		if( bPreLoad == false )
			pUnit->m_iRefCount++;
	
		pUnit->m_iReleaseTime = 0;
		

		// Free 리스트에 있으면 다시 없애준다.
		if( ResourceWzUnit* pFree = m_pResourceWaitForFree->GetData(resourceCode) )
		{
			m_pResourceWaitForFree->Remove(resourceCode);
		}
	}

	return pUnit->m_pwzUnit;
}

//------------------------------------------------------------------------------
/**
*/
void
ResourceCacher::SetUnitForceRemove( RESOURCECODE resourceCode )
{
    ResourceWzUnit * pUnit = m_pResourceUnitHashTable->GetData( resourceCode );
    if( !pUnit )
    {
        assert(!"ResourceCacher::SetUnitForceRemove 로드되지 않은 리소스입니다.");		
    }
    else
    {
        pUnit->SetForceRemove();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
ResourceCacher::IsUnitForceRemove( RESOURCECODE resourceCode )
{
    ResourceWzUnit * pUnit = m_pResourceUnitHashTable->GetData( resourceCode );
    if( !pUnit )
    {
        assert(!"ResourceCacher::IsUnitForceRemove 로드되지 않은 리소스입니다.");		
    }
    else
    {
        return pUnit->IsForceRemove();
    }

    return false;
}

void ResourceCacher::AddToFreeList( ResourceWzUnit *pCurUnit,RESOURCECODE resourceCode)
{
	if (!m_pResourceWaitForFree) 
    {
		return;
	}

	int count = m_pResourceWaitForFree->GetDataNum();

	// 아직 풀이 꽉차지 않았으면 넣고끝
	if (count < MAX_WZUNIT_CACHE) 
    {
		pCurUnit->m_iReleaseTime = RESOURCE_RELEASE_TIME;
		m_pResourceWaitForFree->Add(pCurUnit,resourceCode);
		return;
	}
	else
	{
		DWORD bestcount = ULONG_MAX;
		ResourceWzUnit *pBest = NULL;

		// 풀안에서 가장 참조카운트가 딸린놈을 찾아낸다.
		m_pResourceWaitForFree->SetFirst();

		ResourceWzUnit *pWzUnit = m_pResourceWaitForFree->GetNext();
		pBest = pWzUnit;

		RESOURCE_INFO *pInfo = GraphicResourceList::Instance()->GetItemInfo(pWzUnit->m_wiID - ENGINE_RESOURCE_ID_OFFSET);
		// 혹시라도 ResourceInfoList에 없는 놈이 Assign되었다면 최우선적으로 unload한다
		if(pInfo)
		{
			if (bestcount > (DWORD)pInfo->dwTotalRefCount)
			{
				bestcount = (DWORD)pInfo->dwTotalRefCount;				
			}
		}
		else
		{
			bestcount = 0;			
		}

		pWzUnit = m_pResourceWaitForFree->GetNext();

		while( pWzUnit )
		{
			RESOURCE_INFO *pInfo = GraphicResourceList::Instance()->GetItemInfo(pWzUnit->m_wiID - ENGINE_RESOURCE_ID_OFFSET);
			if(pInfo)
			{
				if (bestcount > (DWORD)pInfo->dwTotalRefCount)
				{
					bestcount = pInfo->dwTotalRefCount;
					pBest = pWzUnit;
				}
			}
			else
			{
				// 혹시라도 ResourceInfoList에 없는 놈이 Assign되었다면 최우선적으로 unload한다
				bestcount = 0;
				pBest = pWzUnit;
			}

			pWzUnit = m_pResourceWaitForFree->GetNext();					
        }	

		// 최종적으로 지울놈과 현재 인자로 들어온놈을 비교해
		// RefCount가 가장 낮은놈을 지운다.
		if (pBest)
		{
			DWORD curcount = 0;		

			RESOURCE_INFO *pInfo = GraphicResourceList::Instance()->GetItemInfo(resourceCode - ENGINE_RESOURCE_ID_OFFSET);
			if(pInfo)
			{
				curcount = pInfo->dwTotalRefCount;
			}

			// 같아도 인자로 들어온놈을 지운다. 왠만하면 리스트를 갱신하지 않는쪽으로간다.
			if (curcount <= bestcount)
			{
				g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pCurUnit->m_wiID );
				m_pResourceUnitHashTable->Remove(pCurUnit->m_wiID);
				m_pWzUnitPool->Free(pCurUnit);
			}
			else
			{
				// 기존거는 지우고 새것을 넣는다~
				g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pBest->m_wiID );
				m_pResourceUnitHashTable->Remove(pBest->m_wiID);
				m_pResourceWaitForFree->Remove(pBest->m_wiID);
				m_pWzUnitPool->Free(pBest);

				pCurUnit->m_iReleaseTime = RESOURCE_RELEASE_TIME;
				m_pResourceWaitForFree->Add(pCurUnit,resourceCode);
			}
		}
	}
}

//리소스는 남겨 둔체로 카운트만줄인다.
void  ResourceCacher::ReleaseResourceCount( RESOURCECODE resourceCode)
{
	ResourceWzUnit * pUnit = m_pResourceUnitHashTable->GetData( resourceCode );
	if(pUnit)
	{
		if (pUnit->m_iRefCount > 0)
		{
			pUnit->m_iRefCount--;
		}
	}
}


void ResourceCacher::ReleaseResourceWzUnit( RESOURCECODE resourceCode )
{
	ResourceWzUnit * pUnit = m_pResourceUnitHashTable->GetData( resourceCode );
	if( !pUnit )
	{
		assert(!"ResourceCacher::ReleaseItemUnit 로드되지 않은 리소스입니다.");		
	}
	else 
    {
		if (pUnit->m_iRefCount > 0)
		{
			pUnit->m_iRefCount--;
			// ref count가 0이되면 없애준다.

			if (pUnit->m_iRefCount == 0)
			{	
                // 강제제거가 켜있으면 즐.
				if (pUnit->IsForceRemove())
				{
					g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pUnit->m_wiID );
					m_pResourceUnitHashTable->Remove(resourceCode);
					m_pWzUnitPool->Free(pUnit);
				}
				// 그렇지 않으면 삭제 우선 리스트에 넣어버린다.
				else 
                {		
					AddToFreeList(pUnit,resourceCode);									
				}
			}
		}
		else
		{
			assert(!"ResourceCacher::ReleaseItemUnit 잘못된 참조 카운트");
		}		
	}	
}

int	ResourceCacher::SetErrorFlag( RESOURCECODE resourceCode ,int iErrorFlag)
{
	ResourceWzUnit * pUnit = m_pResourceUnitHashTable->GetData( resourceCode );
	if( !pUnit )
	{
		assert(!"ResourceCacher::SetErrorFlag 로드되지 않은 리소스입니다.");		
	}
	else
	{
		pUnit->m_iErrorFlag = iErrorFlag;
	}

	return 0;
}

int ResourceCacher::GetErrorFlag( RESOURCECODE resourceCode )
{
	ResourceWzUnit * pUnit = m_pResourceUnitHashTable->GetData( resourceCode );
	if( !pUnit )
	{
		assert(!"ResourceCacher::GetErrorFlag 로드되지 않은 리소스입니다.");		
		return 0;
	}
	else
	{
		return pUnit->m_iErrorFlag;
	}
}

void ResourceCacher::Process(DWORD dwTick)
{
	ResourceWzUnit * pWzUnit = NULL;
	
	if (m_pResourceWaitForFree) 
	{
		//2분에 1번만 검사
		if(m_dwSkippingTick > RESOURCE_RELEASE_CHECK)
		{
			if(m_pResourceWaitForFree->GetDataNum())
			{
				m_pResourceWaitForFree->SetFirst();

				pWzUnit = m_pResourceWaitForFree->GetNext();
				while( pWzUnit )
				{
					//pWzUnit->m_iReleaseTime -= dwTick;
					pWzUnit->m_iReleaseTime -= RESOURCE_RELEASE_CHECK;

					if ((pWzUnit->m_iReleaseTime < 0))
					{
						g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pWzUnit->m_wiID );
						m_pResourceUnitHashTable->Remove(pWzUnit->m_wiID);
						m_pResourceWaitForFree->Remove(pWzUnit->m_wiID);
						m_pWzUnitPool->Free( pWzUnit );

						pWzUnit = m_pResourceWaitForFree->GetNext();
					}
					else
					{
						pWzUnit = m_pResourceWaitForFree->GetNext();					
					}				
				}
			}		

			m_dwSkippingTick = 0;
		}
		else
		{
			m_dwSkippingTick += dwTick;
		}

	}

}

void ResourceCacher::ReleaseAllCachedUnit()
{
	ResourceWzUnit * pWzUnit = NULL;
	ResourceWzUnit * pNextWzUnit = NULL;

	if (m_pResourceWaitForFree) 
	{				
		m_pResourceWaitForFree->SetFirst();

		pWzUnit = m_pResourceWaitForFree->GetNext();
		while( pWzUnit )
		{
			g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pWzUnit->m_wiID );
			m_pResourceUnitHashTable->Remove(pWzUnit->m_wiID);			
			m_pWzUnitPool->Free( pWzUnit );

			pWzUnit = m_pResourceWaitForFree->GetNext();					
		}

		m_pResourceWaitForFree->RemoveAll();
	}
}

void ResourceCacher::ReleaseMostUnit()
{
	ResourceWzUnit * pWzUnit = NULL;
	ResourceWzUnit * pNextWzUnit = NULL;

	if (m_pResourceWaitForFree) 
    {				
		m_pResourceWaitForFree->SetFirst();

		pWzUnit = m_pResourceWaitForFree->GetNext();
		while( pWzUnit )
		{
			if(true == pWzUnit->m_bFreeMost)
			{
				pNextWzUnit = m_pResourceWaitForFree->GetNext();

				g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pWzUnit->m_wiID );
				m_pResourceUnitHashTable->Remove(pWzUnit->m_wiID);		
				m_pResourceWaitForFree->Remove(pWzUnit->m_wiID);
				m_pWzUnitPool->Free( pWzUnit );

				pWzUnit = pNextWzUnit;
			}
			else
			{	
				pWzUnit = m_pResourceWaitForFree->GetNext();					
			}
		}
	}

	pWzUnit = NULL;
	if (m_pResourceUnitHashTable) 
	{
		m_pResourceUnitHashTable->SetFirst();
		pWzUnit = m_pResourceUnitHashTable->GetNext();
		while( pWzUnit )
		{
			if(pWzUnit->m_bFreeMost)
			{
				pNextWzUnit = m_pResourceUnitHashTable->GetNext();

				g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pWzUnit->m_wiID );
				m_pResourceUnitHashTable->Remove(pWzUnit->m_wiID);	
				m_pWzUnitPool->Free( pWzUnit );

				pWzUnit = pNextWzUnit;
			}
			else
			{
				pWzUnit = m_pResourceUnitHashTable->GetNext();
			}
		}
	}
}

int ResourceCacher::GetLoadedResourceCount()
{
	if (m_pResourceUnitHashTable) 
    {
		return m_pResourceUnitHashTable->GetDataNum();
	}

	return 0;
}

int ResourceCacher::GetWaitForFreeResourceCount()
{
	if (m_pResourceWaitForFree) 
	{
		return m_pResourceWaitForFree->GetDataNum();
	}

	return 0;
}

