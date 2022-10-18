// managerspeedtree.h

#ifndef _3DRENDERER_MANAGERSPEEDTREE_H_
#define _3DRENDERER_MANAGERSPEEDTREE_H_

#include "managerbase.h"
#include <programcommon/wzhashtable.h>
#include <list>

#define MAX_SPEED_TREE_DRAW (1024)

enum eST_COLOR_TYPE
{
	eST_COLOR_TYPE_APPLY_TERRAIN_COLOR = 1,
	eST_COLOR_TYPE_MODIFY = 2,
};

//Ʈ�� Access
class CSpeedTreeDrawInfo
{
protected:
	WzBoundingVolume	m_wzBV;				// �ٿ�� ����
	WzColor				m_wcDiffuse;		// ����
	WzVector			m_wvScale;			// ������
	WzVector			m_wvPos;			// ������
	void*				m_pUserData;		// ����� ���� ������
	WzID				m_wID;				// ���� ����

	//int				m_iID;				// ������ ���� �ĺ� �ε���

	DWORD				m_dwAttr; //_YJ_EDIT_OBJ_COLOR
	CSpeedTreeDrawInfo(){ m_dwAttr = eST_COLOR_TYPE_APPLY_TERRAIN_COLOR;}//_YJ_EDIT_OBJ_COLOR



public:
	virtual ~CSpeedTreeDrawInfo(){}

	virtual void	SetPosition(WzVector* pwvPos) { m_wvPos=*pwvPos; }
	virtual void	SetScale(WzVector* pwvScale) { m_wvScale.x=pwvScale->x;m_wvScale.y=pwvScale->x;m_wvScale.z=pwvScale->x; }//x���� ��
	virtual void	SetColor(WzColor wcColor) { m_wcDiffuse=wcColor; }

	virtual void	SetAttr(DWORD dwAttr) {m_dwAttr = dwAttr;}//_YJ_EDIT_OBJ_COLOR


	virtual WzBoundingVolume GetWzBoundingVolume() { return m_wzBV; }
	virtual void	SetUserData(void* pv) { m_pUserData=pv; }
	virtual void	*GetUserData() { return m_pUserData; }
	virtual WzID	GetWzID() { return m_wID; }

	WzColor		GetColor() { return m_wcDiffuse; }
	WzVector	GetPos() { return m_wvPos; }
	WzVector	GetScale() { return m_wvScale; }
	
	DWORD		GetAttr() {return m_dwAttr;}//_YJ_EDIT_OBJ_COLOR


	//CSpeedTreeRT �ø��� ���� ���
	virtual int GetCollisionObjectCount() { return 0; }
	virtual WzBoundingVolume* GetCollisionObject(int iIndex) { return NULL; }
};

struct St_STreeAttribute
{
	float		m_afBrightnessFunc[3];
	float		m_afLod[2];
	float		m_afWindWaveFunc[3];
};


//Manager Ŭ����
typedef std::list<HANDLE> STHANDLE_LIST ;
class CManagerSpeedTree : public CManagerBase
{
protected:
	//�ε��� Ʈ�� ����
	CWzHashTableStr<HANDLE>					m_htsTreeLoaded;		//�����̸� ���� 
	CWzHashTable<HANDLE, WzID>				m_htTreeWzID;			//WzID�� ����
	STHANDLE_LIST							m_lsTreeRef;			//���������� ����

	//��ġ��(�׷���) Ʈ��
	CWzHandleManager<HANDLE>				m_hmTreePlaced;			

	//--------------------------------------------------------------------------
	//���� ��� (Global �Ӽ�)
	//--------------------------------------------------------------------------
	//�ٶ� ����
	float								m_fWindStrength;        //�ٶ����� 0.0 = no wind, 1.0 = full strength
	WzVector							m_wvWindDir;			//�ٶ� ����
	float								m_afWindWaveFunc[3];	//�ٶ� ���� ��ȭ��, ���ʴ�� min,max,frequency
   
	//LOD
  	float								m_fNearLOD;
	float								m_fFarLOD;

	//Brightness
	float								m_afBrightness[3];

	//��Ÿ
	float								m_fAccumulateTime;
	CSpeedTreeDrawInfo*					m_apDrawInfo[MAX_SPEED_TREE_DRAW];
	int									m_iDrawCount;
public:
	CManagerSpeedTree(void);
	virtual ~CManagerSpeedTree(void);
	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	//�ؽ��� �ʱ�ȭ
	virtual BOOL LoadTexture(char* szComositeMap,char* szCompositeShadow){return true;}

	//Ʈ�� ���۷���  �ε�,����
	virtual WzID LoadTreeRef(WzID wID, const char* szTree){return true;}
	virtual void DestroyTreeRef(WzID wID){return;}

	//Ʈ�� �ν��Ͻ�  �߰� ����
	virtual HANDLE AddTree(WzID wID,WzVector* pPos,WzColor wcColor=WzColor_RGB(255,255,255), WzVector* pScale=NULL, DWORD dwAttr = eST_COLOR_TYPE_APPLY_TERRAIN_COLOR){return INVALID_HANDLE_VALUE;}
	virtual void RemoveTree(HANDLE hTree){return;}
	virtual void RemoveAllTree(){}

	//���۷��� ,�ν��Ͻ� ���� ���� 
	virtual void UnloadAllSpeedTrees(){}

	//Ʈ�� ����
	virtual CSpeedTreeDrawInfo* GetTreeByHandle(HANDLE hTree){return (CSpeedTreeDrawInfo*)m_hmTreePlaced.GetData(hTree);}

	//�Ӽ�
	virtual void SetAllAttribute(St_STreeAttribute* pSTA);
	virtual void SetWindStrength(float fStrength){m_fWindStrength = fStrength;}
	virtual void SetWindDirection(WzVector* pWV){VectorNormalize(&m_wvWindDir,pWV);}
	virtual void SetWindWaveFunc(float* param3f){memcpy(m_afWindWaveFunc,param3f,sizeof(m_afWindWaveFunc));}
	virtual void SetLOD(float fNear,float fFar){m_fNearLOD =fNear; m_fFarLOD=fFar;}
	virtual void SetBrightness(float* param3f){memcpy(m_afBrightness,param3f,sizeof(m_afBrightness));}
	
	virtual void GetAllAttribute(St_STreeAttribute* pSTA);
	virtual float GetWindStrength(){return m_fWindStrength;}
	virtual WzVector GetWindDirection(){return m_wvWindDir;}
	virtual void GetWindWaveFunc(float* param3f){memcpy(param3f,m_afWindWaveFunc,sizeof(m_afWindWaveFunc));}
	virtual void GetLOD(float* pfNear,float* pfFar){*pfNear =m_fNearLOD; *pfFar=m_fFarLOD;}
	virtual void GetBrightness(float* param3f){memcpy(param3f,m_afBrightness,sizeof(m_afBrightness));}
	//������ & etc....
	virtual void Render(HANDLE *pHandle,int iDrawCount,bool bSortRender = 1){} //pHandle�� �������� �������� ä�� �ش�.(��Ƽ� ������ �ؾ� ����)
	virtual void Render(WzID wID,WzVector* pWvPos,WzVector *pScale){} //ID�� ������
	virtual void Render4ShadowMap(CWzArray<CSpeedTreeDrawInfo*>& arDraw){} 
	virtual void ProgressFrame(float fDeletaTime, bool bApplyWindWave =false);
	virtual BOOL IsUseSpeedTree();//����Ʈ Ʈ�� ��� ����
	virtual int GetRenderCount(){return m_iDrawCount;}
	virtual const CSpeedTreeDrawInfo* GetRenderTree(int iAt){return m_apDrawInfo[iAt];}


protected:
	virtual	HANDLE hCreateTree( const char* lpszFileName,WzID wID){return NULL;}
	HANDLE	hFindHANDLE( const char *lpszFilePath);
	HANDLE	hGetHandleByWzID(WzID wID);
	
	void	vAddTreeLoad( const char* lpszKey, HANDLE hHandle,WzID wID);
	void	vRemoveTreeLoad(char* lpszKey,WzID widKey);

	HANDLE	hAddTreePlace(HANDLE hHandle){ return m_hmTreePlaced.Add(hHandle);}
	void	vRemoveTreePlace(HANDLE hHandle){ m_hmTreePlaced.Release(hHandle);m_iDrawCount=0;}



};

#endif // _3DRENDERER_MANAGERSPEEDTREE_H_