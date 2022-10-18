//------------------------------------------------------------------------------
/** Icon 를 관리해주는 메니저

@author
	유재영
@since
	[6/14/2005]
@remarks
- 
*/

#ifndef ICONMGR_H 
#define ICONMGR_H

#include "Singleton.h"
#include <MemoryPoolFactory.h>
#include "ObjKeyGenerator.h"

class IconImage;

typedef stdext::hash_map<DWORD, IconImage *>		ICON_HASH_MAP;
typedef ICON_HASH_MAP::iterator						ICON_MAP_ITR;
typedef std::pair<DWORD, IconImage *>				ICON_MAP_PAIR;

class IconMgr : public util::Singleton<IconMgr>
{
public:
	IconMgr(void);
	~IconMgr(void);

public:
	void Create(int iMaxSize, CDrawBase * pDrawBase);
	void Destroy();

	void AddIconCount() { m_dwIconCount++; }
	void RemoveIconCount() { m_dwIconCount--; }

	DWORD GetIconCount() { return m_dwIconCount; }

	IconImage * Add(DWORD dwIconID, char * pszFileName = NULL);
	BOOL		RemoveIcon( IconImage * pIcon );
	BOOL		RemoveAll();

private:
	CDrawBase * m_pDrawBase;
	util::CMemoryPoolFactory<IconImage> *	m_pIconTextureHandlePool;
	ICON_HASH_MAP							m_hashMapIconImage;

	util::CObjKeyGenerator<DWORD> *			m_pIconKeyGenerator;
	DWORD									m_dwIconCount;
};

#define ICONMGR() IconMgr::Instance()

#endif

// ICONMGR_H