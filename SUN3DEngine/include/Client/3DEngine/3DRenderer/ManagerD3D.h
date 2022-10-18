// managerd3d.h

#ifndef _3DRENDERER_MANAGERD3D_H_
#define _3DRENDERER_MANAGERD3D_H_

#include <d3dx9.h>
#include "managerbase.h"

struct St_PE_SetShaderParam // Post Effect 용 쉐이더 세팅
{
	BOOL	m_bSetShader;					//쉐이더 설정 여부
	float*	m_pfData;						//데이터	
	int		m_iStratIndex;					//시작 인덱스
	int		m_iVec4Count;					//Vector4 Count
	
};

typedef struct St_Create_ManagerD3D : public St_Create_Manager
{
	LPDIRECT3DDEVICE9 m_lpD3dDevice;
	D3DCAPS9 *m_pD3dCaps;
	char *m_lpszShaderPath;

} St_Create_ManagerD3D;


class CManagerD3D  
{
public:
	CManagerD3D();
	virtual ~CManagerD3D();
	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();

protected:
	LPDIRECT3DDEVICE9 m_lpD3dDevice;
};

#endif // _3DRENDERER_MANAGERD3D_H_
