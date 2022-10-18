// managerspeedtreed3d.h

#ifndef _3DRENDERER_MANAGERSPEEDTREED3D_H_
#define _3DRENDERER_MANAGERSPEEDTREED3D_H_

#include <d3dx9.h>
#include "managerspeedtree.h"
#include "managerd3d.h"
#include "speedtree/speedtreewrapper.h"
#include "speedtree/speedwind.h"

class CSpeedTreeDrawInfoDX : public CSpeedTreeDrawInfo
{
	CSpeedTreeWrapper*			m_pTreeWrapper;
	WzBoundingVolume*			m_pwbvCollision;		//CSpeedTree CAD 에서 가져온 충돌 오브젝트
	int							m_nCollision;			//충돌 오브젝트 갯수

	//CManagerSpeedTreeD3D 클래스만 
	friend class CManagerSpeedTreeD3D;
	CSpeedTreeDrawInfoDX():m_pTreeWrapper(NULL),m_pwbvCollision(NULL),m_nCollision(0){}
public:
	//base 클래스 
	virtual ~CSpeedTreeDrawInfoDX(){SAFE_DELETE(m_pTreeWrapper);SAFE_DELETE_ARRAY(m_pwbvCollision)}
	void SetPosition(WzVector* pwvPos);
	void SetScale(WzVector* pwvScale);
	void SetColor(WzColor wcColor){CSpeedTreeDrawInfo::SetColor(wcColor);}
	void SetAttr(DWORD dwAttr){CSpeedTreeDrawInfo::SetAttr(dwAttr);} //_YJ_EDIT_OBJ_COLOR

	//여기서만 사용
	//트리세팅
	CSpeedTreeWrapper* GetTree(){return m_pTreeWrapper;}
	void SetTree(CSpeedTreeWrapper* pTree);
	void SetTreeWzID(WzID wID){m_wID=wID;}
	//바운딩 업데이트
	void UpdateBoundingVolume();

	//CSpeedTreeRT 컬리젼 정보 얻기
	virtual int GetCollisionObjectCount();
	virtual WzBoundingVolume* GetCollisionObject(int iIndex);
};

class CManagerSpeedTreeD3D : public CManagerSpeedTree, CManagerD3D
{
public:
	CManagerSpeedTreeD3D(void);
	~CManagerSpeedTreeD3D(void);
	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	//텍스쳐 초기화
	virtual BOOL LoadTexture(char* szComositeMap,char* szCompositeShadow);

	//트리 레퍼런스  로드,해제
	virtual WzID CManagerSpeedTreeD3D::LoadTreeRef(WzID wID, const char* szTree);
	virtual void DestroyTreeRef(WzID wID);
	

	//트리 인스턴스  추가 삭제
	virtual HANDLE AddTree(WzID wID,WzVector* pPos,WzColor wcColor=WzColor_RGB(255,255,255), WzVector* pScale=NULL, DWORD dwAttr = eST_COLOR_TYPE_APPLY_TERRAIN_COLOR);
	virtual void RemoveTree(HANDLE hTree);
	virtual void RemoveAllTree();

	//레퍼런스 ,인스턴스 같이 해제  (레퍼런스만 해제 안됨)
	virtual void UnloadAllSpeedTrees();

	//속성
	virtual void SetWindStrength(float fStrength);
	virtual void SetLOD(float fNear,float fFar);

	//렌더링 & etc....
	virtual void Render(HANDLE *pHandle,int iDrawCount,bool bSortRender = 1); //pHandle에 렌더링할 나무들을 채워 준다.(모아서 렌더링 해야 빠름)
	virtual void Render(WzID wID,WzVector* pWvPos,WzVector *pScale); //ID로 렌더링
	virtual void Render4ShadowMap(CWzArray<CSpeedTreeDrawInfo*>& arDraw);


private:
		void	vSetupWindMatrices(float fTimeInSecs);
		void	vChangeRS();
		void	vRecoverRS(); 
		void	vReleaseDeviceObjebject();
		void	vReleaseTexture();
		void	vSetSpeedTreeRTLight();
		void	vSetLightInfo();
		BOOL	bRestoreDeviceObj();
		void	vLoadSpeedTreeTexture(CSpeedTreeWrapper* pSpeedTreeWrapper);
		HANDLE	hCreateTree( const char* lpszFileName,WzID wID);
		void	vSetVSConst();
		void	vCheckHardWare();
static	bool bSortFunc(CSpeedTreeDrawInfoDX* pST1,CSpeedTreeDrawInfoDX* pST2);

 private:
	 //바람
	CSpeedWind							m_cSpeedWind;			
	HANDLE								m_hComopsiteTexture;
	bool								m_bSupporHarWare;

#ifdef WRAPPER_RENDER_SELF_SHADOWS
	HANDLE							m_hShadowTexture;
#endif
	char							m_szShadowTexutre[MAX_PATH];
	char							m_szCompositeTexutre[MAX_PATH];

	//빛 (쉐이더 상수 설정 때문에 4*float) 
	float							m_afLightInfo[20];
};

#endif // _3DRENDERER_MANAGERSPEEDTREED3D_H_