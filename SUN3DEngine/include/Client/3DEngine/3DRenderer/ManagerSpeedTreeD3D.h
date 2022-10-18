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
	WzBoundingVolume*			m_pwbvCollision;		//CSpeedTree CAD ���� ������ �浹 ������Ʈ
	int							m_nCollision;			//�浹 ������Ʈ ����

	//CManagerSpeedTreeD3D Ŭ������ 
	friend class CManagerSpeedTreeD3D;
	CSpeedTreeDrawInfoDX():m_pTreeWrapper(NULL),m_pwbvCollision(NULL),m_nCollision(0){}
public:
	//base Ŭ���� 
	virtual ~CSpeedTreeDrawInfoDX(){SAFE_DELETE(m_pTreeWrapper);SAFE_DELETE_ARRAY(m_pwbvCollision)}
	void SetPosition(WzVector* pwvPos);
	void SetScale(WzVector* pwvScale);
	void SetColor(WzColor wcColor){CSpeedTreeDrawInfo::SetColor(wcColor);}
	void SetAttr(DWORD dwAttr){CSpeedTreeDrawInfo::SetAttr(dwAttr);} //_YJ_EDIT_OBJ_COLOR

	//���⼭�� ���
	//Ʈ������
	CSpeedTreeWrapper* GetTree(){return m_pTreeWrapper;}
	void SetTree(CSpeedTreeWrapper* pTree);
	void SetTreeWzID(WzID wID){m_wID=wID;}
	//�ٿ�� ������Ʈ
	void UpdateBoundingVolume();

	//CSpeedTreeRT �ø��� ���� ���
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

	//�ؽ��� �ʱ�ȭ
	virtual BOOL LoadTexture(char* szComositeMap,char* szCompositeShadow);

	//Ʈ�� ���۷���  �ε�,����
	virtual WzID CManagerSpeedTreeD3D::LoadTreeRef(WzID wID, const char* szTree);
	virtual void DestroyTreeRef(WzID wID);
	

	//Ʈ�� �ν��Ͻ�  �߰� ����
	virtual HANDLE AddTree(WzID wID,WzVector* pPos,WzColor wcColor=WzColor_RGB(255,255,255), WzVector* pScale=NULL, DWORD dwAttr = eST_COLOR_TYPE_APPLY_TERRAIN_COLOR);
	virtual void RemoveTree(HANDLE hTree);
	virtual void RemoveAllTree();

	//���۷��� ,�ν��Ͻ� ���� ����  (���۷����� ���� �ȵ�)
	virtual void UnloadAllSpeedTrees();

	//�Ӽ�
	virtual void SetWindStrength(float fStrength);
	virtual void SetLOD(float fNear,float fFar);

	//������ & etc....
	virtual void Render(HANDLE *pHandle,int iDrawCount,bool bSortRender = 1); //pHandle�� �������� �������� ä�� �ش�.(��Ƽ� ������ �ؾ� ����)
	virtual void Render(WzID wID,WzVector* pWvPos,WzVector *pScale); //ID�� ������
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
	 //�ٶ�
	CSpeedWind							m_cSpeedWind;			
	HANDLE								m_hComopsiteTexture;
	bool								m_bSupporHarWare;

#ifdef WRAPPER_RENDER_SELF_SHADOWS
	HANDLE							m_hShadowTexture;
#endif
	char							m_szShadowTexutre[MAX_PATH];
	char							m_szCompositeTexutre[MAX_PATH];

	//�� (���̴� ��� ���� ������ 4*float) 
	float							m_afLightInfo[20];
};

#endif // _3DRENDERER_MANAGERSPEEDTREED3D_H_