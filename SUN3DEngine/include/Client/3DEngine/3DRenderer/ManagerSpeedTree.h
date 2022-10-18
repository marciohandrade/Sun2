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

//트리 Access
class CSpeedTreeDrawInfo
{
protected:
	WzBoundingVolume	m_wzBV;				// 바운딩 볼륨
	WzColor				m_wcDiffuse;		// 색상
	WzVector			m_wvScale;			// 스케일
	WzVector			m_wvPos;			// 포지션
	void*				m_pUserData;		// 사용자 지정 데이터
	WzID				m_wID;				// 나무 종류

	//int				m_iID;				// 편집을 위한 식별 인덱스

	DWORD				m_dwAttr; //_YJ_EDIT_OBJ_COLOR
	CSpeedTreeDrawInfo(){ m_dwAttr = eST_COLOR_TYPE_APPLY_TERRAIN_COLOR;}//_YJ_EDIT_OBJ_COLOR



public:
	virtual ~CSpeedTreeDrawInfo(){}

	virtual void	SetPosition(WzVector* pwvPos) { m_wvPos=*pwvPos; }
	virtual void	SetScale(WzVector* pwvScale) { m_wvScale.x=pwvScale->x;m_wvScale.y=pwvScale->x;m_wvScale.z=pwvScale->x; }//x값만 씀
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


	//CSpeedTreeRT 컬리젼 정보 얻기
	virtual int GetCollisionObjectCount() { return 0; }
	virtual WzBoundingVolume* GetCollisionObject(int iIndex) { return NULL; }
};

struct St_STreeAttribute
{
	float		m_afBrightnessFunc[3];
	float		m_afLod[2];
	float		m_afWindWaveFunc[3];
};


//Manager 클래스
typedef std::list<HANDLE> STHANDLE_LIST ;
class CManagerSpeedTree : public CManagerBase
{
protected:
	//로딩된 트리 저장
	CWzHashTableStr<HANDLE>					m_htsTreeLoaded;		//파일이름 으로 
	CWzHashTable<HANDLE, WzID>				m_htTreeWzID;			//WzID로 접근
	STHANDLE_LIST							m_lsTreeRef;			//순차적으로 접근

	//배치된(그려진) 트리
	CWzHandleManager<HANDLE>				m_hmTreePlaced;			

	//--------------------------------------------------------------------------
	//공통 멤버 (Global 속성)
	//--------------------------------------------------------------------------
	//바람 관련
	float								m_fWindStrength;        //바람세기 0.0 = no wind, 1.0 = full strength
	WzVector							m_wvWindDir;			//바람 방향
	float								m_afWindWaveFunc[3];	//바람 세기 변화량, 차례대로 min,max,frequency
   
	//LOD
  	float								m_fNearLOD;
	float								m_fFarLOD;

	//Brightness
	float								m_afBrightness[3];

	//기타
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

	//텍스쳐 초기화
	virtual BOOL LoadTexture(char* szComositeMap,char* szCompositeShadow){return true;}

	//트리 레퍼런스  로드,해제
	virtual WzID LoadTreeRef(WzID wID, const char* szTree){return true;}
	virtual void DestroyTreeRef(WzID wID){return;}

	//트리 인스턴스  추가 삭제
	virtual HANDLE AddTree(WzID wID,WzVector* pPos,WzColor wcColor=WzColor_RGB(255,255,255), WzVector* pScale=NULL, DWORD dwAttr = eST_COLOR_TYPE_APPLY_TERRAIN_COLOR){return INVALID_HANDLE_VALUE;}
	virtual void RemoveTree(HANDLE hTree){return;}
	virtual void RemoveAllTree(){}

	//레퍼런스 ,인스턴스 같이 해제 
	virtual void UnloadAllSpeedTrees(){}

	//트리 접근
	virtual CSpeedTreeDrawInfo* GetTreeByHandle(HANDLE hTree){return (CSpeedTreeDrawInfo*)m_hmTreePlaced.GetData(hTree);}

	//속성
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
	//렌더링 & etc....
	virtual void Render(HANDLE *pHandle,int iDrawCount,bool bSortRender = 1){} //pHandle에 렌더링할 나무들을 채워 준다.(모아서 렌더링 해야 빠름)
	virtual void Render(WzID wID,WzVector* pWvPos,WzVector *pScale){} //ID로 렌더링
	virtual void Render4ShadowMap(CWzArray<CSpeedTreeDrawInfo*>& arDraw){} 
	virtual void ProgressFrame(float fDeletaTime, bool bApplyWindWave =false);
	virtual BOOL IsUseSpeedTree();//스피트 트리 사용 여부
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