#include "SunClientPrecompiledHeader.h"
#include "ItemUnitCacher.h"
#include "ResourceManager.h"
#include "GraphicResourceList.h"
#include <SCSlotHeader.h>
#include <float.h>
#include "Application.h"
#include "GlobalFunc.h"

const int RESOURCE_RELEASE_TIME = 300000; //5���̻� �������ϸ� ��������

const int MAX_WZUNIT_CACHE = 120;
const int RESOURCE_RELEASE_CHECK = 10000; //10�ʿ� 1�� üũ
//const int RESOURCE_RELEASE_TIME = 1800000; //30���̻� �������ϸ� ��������
//
//const int MAX_WZUNIT_CACHE = 120;
//const int RESOURCE_RELEASE_CHECK = 120000; //2�п� 1�� üũ

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
	assert(pInfo && "�߸��� ���ҽ� �ڵ��Դϴ�.");

	// check file
	BOOL bExist = FALSE;
	if (pInfo)
	{
        if (is_npc)//����� ����
        {
            size_t file_length = strlen(pInfo->szFilename);

            if ((file_length + strlen("_NPC")) < 128)//128�� �ش� ���� ������ �ƽ� 
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
		
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("[code: %d] ���ҽ��� �������� �ʾ� %s�� ��� �ε���"), (int)(resourceCode - ENGINE_RESOURCE_ID_OFFSET), filename);
	}

    DWORD load_style = ULS_DEFAULT;

    if (pre_load)
    {
       load_style |= ULS_NO_MULTITHREAD;
    }

	if( resourceCode != g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit( resourceCode, filename,TRUE,load_style,fRequestRateResizeTexture ))
	{
#ifdef _DEV_VER
		MessageBoxA(NULL, "���ҽ� ������ Error", filename, MB_OK);
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("[%s][code: %d] ���ҽ��� �ε� �� �� �����ϴ�"), filename, resourceCode - ENGINE_RESOURCE_ID_OFFSET);
#endif
	}			

	// �ӽ÷� ȿ�� ����Ÿ���� ������ش�.
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
		

		// Free ����Ʈ�� ������ �ٽ� �����ش�.
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
        assert(!"ResourceCacher::SetUnitForceRemove �ε���� ���� ���ҽ��Դϴ�.");		
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
        assert(!"ResourceCacher::IsUnitForceRemove �ε���� ���� ���ҽ��Դϴ�.");		
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

	// ���� Ǯ�� ������ �ʾ����� �ְ�
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

		// Ǯ�ȿ��� ���� ����ī��Ʈ�� �������� ã�Ƴ���.
		m_pResourceWaitForFree->SetFirst();

		ResourceWzUnit *pWzUnit = m_pResourceWaitForFree->GetNext();
		pBest = pWzUnit;

		RESOURCE_INFO *pInfo = GraphicResourceList::Instance()->GetItemInfo(pWzUnit->m_wiID - ENGINE_RESOURCE_ID_OFFSET);
		// Ȥ�ö� ResourceInfoList�� ���� ���� Assign�Ǿ��ٸ� �ֿ켱������ unload�Ѵ�
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
				// Ȥ�ö� ResourceInfoList�� ���� ���� Assign�Ǿ��ٸ� �ֿ켱������ unload�Ѵ�
				bestcount = 0;
				pBest = pWzUnit;
			}

			pWzUnit = m_pResourceWaitForFree->GetNext();					
        }	

		// ���������� ������ ���� ���ڷ� ���³��� ����
		// RefCount�� ���� �������� �����.
		if (pBest)
		{
			DWORD curcount = 0;		

			RESOURCE_INFO *pInfo = GraphicResourceList::Instance()->GetItemInfo(resourceCode - ENGINE_RESOURCE_ID_OFFSET);
			if(pInfo)
			{
				curcount = pInfo->dwTotalRefCount;
			}

			// ���Ƶ� ���ڷ� ���³��� �����. �ظ��ϸ� ����Ʈ�� �������� �ʴ������ΰ���.
			if (curcount <= bestcount)
			{
				g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pCurUnit->m_wiID );
				m_pResourceUnitHashTable->Remove(pCurUnit->m_wiID);
				m_pWzUnitPool->Free(pCurUnit);
			}
			else
			{
				// �����Ŵ� ����� ������ �ִ´�~
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

//���ҽ��� ���� ��ü�� ī��Ʈ�����δ�.
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
		assert(!"ResourceCacher::ReleaseItemUnit �ε���� ���� ���ҽ��Դϴ�.");		
	}
	else 
    {
		if (pUnit->m_iRefCount > 0)
		{
			pUnit->m_iRefCount--;
			// ref count�� 0�̵Ǹ� �����ش�.

			if (pUnit->m_iRefCount == 0)
			{	
                // �������Ű� �������� ��.
				if (pUnit->IsForceRemove())
				{
					g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pUnit->m_wiID );
					m_pResourceUnitHashTable->Remove(resourceCode);
					m_pWzUnitPool->Free(pUnit);
				}
				// �׷��� ������ ���� �켱 ����Ʈ�� �־������.
				else 
                {		
					AddToFreeList(pUnit,resourceCode);									
				}
			}
		}
		else
		{
			assert(!"ResourceCacher::ReleaseItemUnit �߸��� ���� ī��Ʈ");
		}		
	}	
}

int	ResourceCacher::SetErrorFlag( RESOURCECODE resourceCode ,int iErrorFlag)
{
	ResourceWzUnit * pUnit = m_pResourceUnitHashTable->GetData( resourceCode );
	if( !pUnit )
	{
		assert(!"ResourceCacher::SetErrorFlag �ε���� ���� ���ҽ��Դϴ�.");		
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
		assert(!"ResourceCacher::GetErrorFlag �ε���� ���� ���ҽ��Դϴ�.");		
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
		//2�п� 1���� �˻�
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

