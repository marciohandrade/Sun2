// simpleocclusion.h

#ifndef _3DTERRAIN_SIMPLEOCCLUSION_H_
#define _3DTERRAIN_SIMPLEOCCLUSION_H_

// ������� ���� ����
#include "distanceobject.h"
#include <vector>

#define OFN_NEAR			0x01
#define OFN_FAR				0x02
#define OFN_LEFT			0x04
#define OFN_RIGHT			0x08
#define OFN_TOP				0x10
#define OFN_BOTTOM			0x20

#define NUM_OCCTEST_FACE	4		///< Occlusion �׽�Ʈ���� ���Ǿ��ϴ� ������ ����
#define NUM_OCC_VERTICES	4		///< Occlusion plane�� �����ϴ� ������ ����
#define NUM_OCCBOX_VERTICES	8		///< Occlusion Box�� �����ϴ� ������ ����

struct St_OccFrustrum
{
	WzPlane nearplane;	///< �� ���
	WzPlane plane[4];	///< frustum�� �����ϴ� ��,��,��,�� ����� ������ ��´�.
	float   maxdepth;
	float	mindepth;
};

/// Occluder�� ���� ������.
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
	float	 m_fMaxDepth;	///< ���� �þ߿��� �� ���̰�(�ִ� y��). �� ����� �ִ� Y���� �ȴ�
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
	WzVector	   m_vPt[8];		///< Box�� �����ϴ� 8�� ����
	WzVector	   m_vViewPt[8];	///< View ���������� �Ű��� 8�� ����
	WzVector	   m_NorPlane[6];	///< Box�� 6�� ����� Normal vector
	WzPlane		   m_OccPlane[6];	///< Occluder�� Box�� �����ϴ� 6�� ���
	St_OccFrustrum m_OccFrustum[6];
	int			   m_iNumOccFrustum;///< Frustum�� ����

	float		   m_fRadius;
	WzVector	   m_vCenter;
	WzVector	   m_vViewCenter;
	float		   m_fMinDepth;	///< ���� �þ߿��� ����� ���̰�(�ִ� y��). �� ����� �ִ� Y���� �ȴ�
	float		   m_fMaxDepth;	///< ���� �þ߿��� �� ���̰�(�ִ� y��). �� ����� �ִ� Y���� �ȴ�
};

/// ���� ����� �̿��� Occlusion Culling�� �����ϴ� Ŭ����.
///
/// @warnning
///	 1. �ΰ��� ���� ��鿡 ���ļ� �������� ��� �ø��� ���� �ʴ´�.
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

	WzBoundingVolume m_wbv;			///< Occluder�� Volumetric geometry
	BOOL			m_bOccluded;	///< �ٸ� Occluder�� ���� ��������?
};

typedef std::vector< cSimpleOcclusion* >		StlVSimpOcc;
typedef tDistanceObject< cSimpleOcclusion* >	DISTOBJ_SIMPOCC;
typedef std::vector< DISTOBJ_SIMPOCC >			StlVDistObjBuffer_SimpOcc;

#endif // _3DTERRAIN_SIMPLEOCCLUSION_H_