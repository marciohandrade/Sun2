#ifndef __ITEM_UNIT_CACHER__
#define __ITEM_UNIT_CACHER__

//=============================================================================================================================
///  CWzUnit class를 캐쉬하여 공유하기 위한 ItemUnitCacher class
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 1. 12
	@remarks
		- CWzUnitDraw를 구성하여 ItemUnitRenderer에서 Rendering할 수 있게 구성하는 역할을 한다.
		- CWzUnit을 공유하여 메모리를 절약한다.
	@todo
		- CWzUnitDraw의 pool역할도 담당할 예정이다.
	@note
		- ItemUnitRenderer의 멤버로 사용된다.
*/
//=============================================================================================================================


#pragma once

#include <MemoryPoolFactory.h>
#include <SolarHashTable.h>

class CWzUnitDraw;
class CWzUnit;
class CDrawBase;

class ResourceCacher
{
public:
	class ResourceWzUnit 
	{
	public:

        ResourceWzUnit() :
          m_bForceRemove(false),  m_bFreeMost(true)
        {
        }

		CWzUnit *	m_pwzUnit;
		WzID		m_wiID;
		int         m_iRefCount;
		int         m_iErrorFlag;
		int         m_iReleaseTime;
		DWORD       m_dwTotalRefCount;
		bool		m_bFreeMost;

#ifdef _DEBUG
        char        m_FilePath[1024];

#endif //_DEBUG

        inline
        void
        SetForceRemove()
        {
            this->m_bForceRemove = true;
        };

        inline
        bool
        IsForceRemove()
        {
            return this->m_bForceRemove;
        };

    private:
        bool        m_bForceRemove;
	};

public:
	ResourceCacher();
	~ResourceCacher();

	VOID								Init();

	VOID								LoadResource(RESOURCECODE resourceCode, float fRequestRateResizeTexture = -1.0f, bool pre_load = false, bool is_npc = false);	
	CWzUnit *							GetResourceWzUnit( RESOURCECODE resourceCode 
														  ,float fRequestRateResizeTexture = -1.0f
														  , bool bPreLoad = false
														  , bool bFreeMost = true
                                                          , bool is_npc = false);
	void                                ReleaseResourceWzUnit( RESOURCECODE resourceCode);

	void                                ReleaseResourceCount( RESOURCECODE resourceCode);

	int							        SetErrorFlag( RESOURCECODE resourceCode ,int iErrorFlag);
	int  							    GetErrorFlag( RESOURCECODE resourceCode );

	void                                AddToFreeList( ResourceWzUnit *pUnit,RESOURCECODE resourceCode);

    void                                SetUnitForceRemove( RESOURCECODE resourceCode );
    bool                                IsUnitForceRemove( RESOURCECODE resourceCode );

	void                                Process(DWORD dwTick);

	void                                ReleaseAllCachedUnit();
	void								ReleaseMostUnit();

	int                                 GetLoadedResourceCount();
	int                                 GetWaitForFreeResourceCount();
	

    void GetUnitArray(std::vector<ResourceWzUnit*>& array)
    {
        array.clear();

        m_pResourceUnitHashTable->SetFirst();
        ResourceWzUnit* pUnit = m_pResourceUnitHashTable->GetHeadData();
        while( pUnit )
        {
            array.push_back(pUnit);
            pUnit = m_pResourceUnitHashTable->GetNext();
        }
    }

    void GetFreeUnitArray(std::vector<ResourceWzUnit*>& array)
    {
        array.clear();

        m_pResourceWaitForFree->SetFirst();
        ResourceWzUnit* pUnit = m_pResourceWaitForFree->GetHeadData();
        while( pUnit )
        {
            array.push_back(pUnit);
            pUnit = m_pResourceWaitForFree->GetNext();
        }
    }


    int GetUnitCount() const 
    {
        return m_pResourceUnitHashTable->GetDataNum();
    }

    int GetFreeUnitCount() const
    {
        return m_pResourceWaitForFree->GetDataNum();
    }

    int GetPoolBasicSize() const
    {
        return m_pWzUnitPool->GetPoolBasicSize();
    }
    int GetPoolExtendSize() const
    {
        return m_pWzUnitPool->GetPoolExtendSize();
    }
    int GetNumberOfBands() const
    {
        return m_pWzUnitPool->GetNumberOfBands();
    }
    int GetAvailableNumberOfTypes() const
    {
        return m_pWzUnitPool->GetAvailableNumberOfTypes();
    }

    long GetUnitPoolMemorySize()
    {
        long total_size = 0;

        total_size += m_pWzUnitPool->GetNumberOfBands() * m_pWzUnitPool->GetPoolExtendSize() * sizeof(ResourceWzUnit);
        return total_size;
    }


private:


	util::SolarHashTable<ResourceWzUnit *> *	m_pResourceUnitHashTable;	
	util::SolarHashTable<ResourceWzUnit *> *	m_pResourceWaitForFree;	
	util::CMemoryPoolFactory<ResourceWzUnit> *	m_pWzUnitPool;

	DWORD m_dwSkippingTick;
};

#endif // __ITEM_UNIT_CACHER__
