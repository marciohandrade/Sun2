// simpleocclusion.h

#ifndef _3DTERRAIN_SIMPLEOCCLUSION_H_
#define _3DTERRAIN_SIMPLEOCCLUSION_H_

// 헤더파일 참조 선언
#include "distanceobject.h"
#include <vector>

#define OFN_NEAR			0x01
#define OFN_FAR				0x02
#define OFN_LEFT			0x04
#define OFN_RIGHT			0x08
#define OFN_TOP				0x10
#define OFN_BOTTOM			0x20

#define NUM_OCCTEST_FACE	4		///< Occlusion 테스트에서 계산되야하는 평면들의 갯수
#define NUM_OCC_VERTICES	4		///< Occlusion plane을 구성하는 정점의 갯수
#define NUM_OCCBOX_VERTICES	8		///< Occlusion Box를 구성하는 정점의 갯수

struct St_OccFrustrum
{
	WzPlane nearplane;	///< 근 평면
	WzPlane plane[4];	///< frustum을 구성하는 좌,우,상,하 평면의 정보를 담는다.
	float   maxdepth;
	float	mindepth;
};

/// Occluder의 형태 열거자.
typedef enum eOT_OCCLUDER
{
	OT_NONE	 = 0,
	OT_PLANE = 1,
	OT_BOX	 = 2,
} OCCLUDERTYPE;

class cBaseOcclusion
{
public:
	cBaseOcclusion(WORD type=0) { m_wOccType = type; }

	WORD Type() { return m_wOccType; }

protected:
	// 1: Plane
	// 2: Box
	WORD m_wOccType;
};

class cOccPlane : public cBaseOcclusion
{
public:
	cOccPlane() { m_wOccType=1; }

	//----------------------------------
	// Member variables
	//
	WzVector m_vPt[4];

	WzVector m_vViewPt[4];
	WzPlane  m_Nearplane;
	WzPlane  m_Plane[4];
	float	 m_fMaxDepth;	///< 가장 시야에서 먼 깊이값(최대 y값). 즉 평면의 최대 Y값이 된다
	float    m_fMinDepth;
	WzPlane  m_OccPlane;
};

class cOccBox : public cBaseOcclusion
{
public:
	cOccBox() { m_wOccType=2; }

	//----------------------------------
	// Member variables
	//
	WzVector	   m_vPt[8];		///< Box를 구성하는 8개 정점
	WzVector	   m_vViewPt[8];	///< View 공간상으로 옮겨진 8개 정점
	WzVector	   m_NorPlane[6];	///< Box의 6개 평면의 Normal vector
	WzPlane		   m_OccPlane[6];	///< Occluder인 Box를 구성하는 6개 평면
	St_OccFrustrum m_OccFrustum[6];
	int			   m_iNumOccFrustum;///< Frustum의 갯수

	float		   m_fRadius;
	WzVector	   m_vCenter;
	WzVector	   m_vViewCenter;
	float		   m_fMinDepth;	///< 가장 시야에서 가까운 깊이값(최대 y값). 즉 평면의 최대 Y값이 된다
	float		   m_fMaxDepth;	///< 가장 시야에서 먼 깊이값(최대 y값). 즉 평면의 최대 Y값이 된다
};

/// 차폐 평면을 이용한 Occlusion Culling을 수행하는 클래스.
///
/// @warnning
///	 1. 두개의 차폐 평면에 걸쳐서 가려지는 경우 컬링이 되지 않는다.
///
/// @date 2006.1.18
/// 
class cSimpleOcclusion
{
private:
	void SetupOccFrsutum(St_OccFrustrum *pOut, WzMatrix *pViewTransform, WzVector *viewpos);

	void SetupOccPlane(const WzVector* pViewer, WzMatrix *pViewTransform);
	void SetupOccBox(const WzVector* pViewer, WzMatrix *pViewTransform, const WzVector* pViewerDir);

	BOOL TestByPlane(WzVector *pViewPos, float fBSphereRadius);
	BOOL TestByBox(WzVector *pViewPos, float fBSphereRadius);

public:
	virtual void SetupOcclusion(const WzVector* pViewer, WzMatrix *pViewTransform, const WzVector* pViewerDir);
	virtual BOOL TestIfOccluded(WzVector *pViewPos, float fBSphereRadius);
	
	BOOL IsInside(cSimpleOcclusion* right);

	// Constructor & Desturctor
	cSimpleOcclusion() { m_pData = NULL; }
	cSimpleOcclusion(WORD type) { if(0==type) {m_pData = NULL;} else if(1==type) {m_pData=new cOccPlane;} else if(2==type) {m_pData=new cOccBox;} }
	~cSimpleOcclusion() { if(m_pData) {delete m_pData;} }

public:
	cBaseOcclusion  *m_pData;

	unsigned long	m_woiIndex;

	WzBoundingVolume m_wbv;			///< Occluder의 Volumetric geometry
	BOOL			m_bOccluded;	///< 다른 Occluder에 의해 가려졌다?
};

typedef std::vector< cSimpleOcclusion* >		StlVSimpOcc;
typedef tDistanceObject< cSimpleOcclusion* >	DISTOBJ_SIMPOCC;
typedef std::vector< DISTOBJ_SIMPOCC >			StlVDistObjBuffer_SimpOcc;

#endif // _3DTERRAIN_SIMPLEOCCLUSION_H_