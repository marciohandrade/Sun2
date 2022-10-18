//------------------------------------------------------------------------------
//	
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "iconmgr.h"
#include "IconImage.h"
#include "GraphicResourceList.h"

#ifdef _DEV_VER
#include "GlobalFunc.h"
#endif //_DEV_VER

//----------------------------------------------------------------------------
/**
*/
IconMgr::IconMgr(void) : m_pDrawBase(0), 
						 m_pIconTextureHandlePool(0), 
						 m_pIconKeyGenerator(0), 
						 m_dwIconCount(0)
{

}


//----------------------------------------------------------------------------
/**
*/
IconMgr::~IconMgr(void)
{

	assert(0 <= GetIconCount() && "제대로 아이콘 이미지를 지우지 않았다. 확인바람!!(재영)");

}


//----------------------------------------------------------------------------
/**
*/
void 
IconMgr::Create(int iMaxSize, CDrawBase * pDrawBase) 
{
	assert(!m_pDrawBase);
	assert(!m_pIconTextureHandlePool);
	assert(pDrawBase);
	assert(iMaxSize > 0);

	m_pDrawBase = pDrawBase;

	m_pIconTextureHandlePool = new util::CMemoryPoolFactory<IconImage>;
	m_pIconTextureHandlePool->Initialize( iMaxSize, iMaxSize/2+1 );

	this->m_pIconKeyGenerator = new util::CObjKeyGenerator<DWORD>(1, MAX_ICON_IMAGE);
}


//----------------------------------------------------------------------------
/**
*/
void 
IconMgr::Destroy()
{
	if (m_pIconTextureHandlePool)
	{
		RemoveAll();

		m_pIconTextureHandlePool->Release();
		delete m_pIconTextureHandlePool;
		m_pIconTextureHandlePool = NULL;
	}

	SAFE_DELETE(this->m_pIconKeyGenerator);

	m_pDrawBase = NULL;	// 외부에서 해제한다.
}


//----------------------------------------------------------------------------
/**
*/
IconImage * 
IconMgr::Add(DWORD dwIconID, char * pszFileName /*= NULL*/)
{
    if (dwIconID <= 0)
    {
        return NULL;
    }

	DWORD dwKey = m_pIconKeyGenerator->GetKey();

	HANDLE hTexture = INVALID_HANDLE_VALUE;

	IconImage* pIcon = (IconImage *) m_pIconTextureHandlePool->Alloc();
	
	if (!pIcon)
	{
#ifdef _DEV_VER
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("실패::conTextureHandlePool->Alloc()"));
#endif //_DEV_VER
		assert(!"텍스쳐 풀이 꽉찼다.!!");

        m_pIconKeyGenerator->RestoreKey(dwKey);
		return NULL;
	}


	pIcon->m_dwKey = dwKey;
	pIcon->m_dwResourceID = dwIconID;

	RESOURCE_INFO * pInfo = GraphicResourceList::Instance()->GetItemInfo(dwIconID);

	if ( pInfo )
	{
		hTexture = m_pDrawBase->x_pManagerTexture->LoadTexture(pInfo->szFilename ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}

	if (INVALID_HANDLE_VALUE == hTexture || NULL == hTexture)
	{
		hTexture = m_pDrawBase->x_pManagerTexture->LoadTexture("Data\\Item\\defaultslotImage.dds",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP);

		if (INVALID_HANDLE_VALUE == hTexture || NULL == hTexture)
		{
#ifdef _DEV_VER
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("아이콘 생성 실패::%d"), dwIconID);
#endif //_DEV_VER
			assert(!"텍스쳐 핸들을 만들지 못했다!!");

            m_pIconKeyGenerator->RestoreKey(dwKey);
            m_pIconTextureHandlePool->Free(pIcon);
            pIcon = NULL;
			return NULL;
		}
	}

	St_TextureContainer * pTextureInfo =  m_pDrawBase->x_pManagerTexture->GetTextureInfo(hTexture);

	if ( NULL == pTextureInfo ) 
	{
#ifdef _DEV_VER
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("NULL == pTextureInfo"));
#endif //_DEV_VER

        m_pIconKeyGenerator->RestoreKey(dwKey);
        m_pIconTextureHandlePool->Free(pIcon);
        pIcon = NULL;
		return NULL;
	}

	pIcon->Create( hTexture, pTextureInfo->m_nWidth, pTextureInfo->m_nHeight);

	m_hashMapIconImage.insert(ICON_MAP_PAIR(dwKey, pIcon));

	return pIcon;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
IconMgr::RemoveIcon( IconImage * pIcon )
{
	if (pIcon == NULL)
	{
		assert(pIcon);
		return FALSE;
	}

	this->m_pIconKeyGenerator->RestoreKey(pIcon->m_dwKey);
	ICON_MAP_ITR itr = m_hashMapIconImage.find(pIcon->m_dwKey);
    if (itr != m_hashMapIconImage.end())
    {
        m_hashMapIconImage.erase(itr);
    }

	pIcon->Destroy();
	m_pIconTextureHandlePool->Free(pIcon);
	pIcon = NULL;

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
IconMgr::RemoveAll()
{
	IconImage * pIcon;

	ICON_MAP_ITR itr = m_hashMapIconImage.begin();

	while (itr != m_hashMapIconImage.end())
	{
		pIcon = static_cast<IconImage *>(itr->second);
		if(pIcon)
		{
			this->m_pIconKeyGenerator->RestoreKey(pIcon->m_dwKey);
			pIcon->Destroy();
			m_pIconTextureHandlePool->Free(pIcon);
			pIcon = NULL;
		}

		itr++;
	}

	m_hashMapIconImage.clear();

	return TRUE;
}