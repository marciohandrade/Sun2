// managerbase.h

#ifndef _3DRENDERER_MANAGERBASE_H_
#define _3DRENDERER_MANAGERBASE_H_

#include <client/3dengine/3dfileaccess/3dfileaccess.h>
#include <client/3dengine/3dfileaccess/wzdbase/wzdbase.h>

class CDrawBase;

class St_Create_Manager
//typedef struct St_Create_Manager
{
public:
	DWORD m_dwResult[4];
	St_RenderOption m_CurrentRenderOption;
};


class CManagerBase  
{
public:
	CManagerBase();
	virtual ~CManagerBase();
	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

protected:
	CDrawBase *m_pParentDraw;
};

#endif // _3DRENDERER_MANAGERBASE_H_
