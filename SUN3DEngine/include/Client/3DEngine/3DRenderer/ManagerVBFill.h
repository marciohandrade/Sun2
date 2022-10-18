// managervbfill.h

#ifndef _3DRENDERER_MANAGERVBFILL_H_
#define _3DRENDERER_MANAGERVBFILL_H_

#include "managerbase.h"
#include "managerd3d.h"
#include "managerwzdloader.h"

#define INSTANCING_START_INDEX 21.0f

enum ENUM_OWN_VB_TYPE
{
	EOVT_TERRAIN = 0,
	EOVT_UNIT,
	EOVT_LIGHTMAPUV = 3,
	EOVT_TERRAIN_TANGENT_BINORMAL = 4,
};

enum ENUM_OWN_IB_TYPE
{
	EOIT_DWORD = 0,
	EOIT_WORD,
};

class CManagerVBFill : public CManagerBase, CManagerD3D
{
public:
	CManagerVBFill(void);
	virtual ~CManagerVBFill(void);
	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();


	// Own Vertex / Index Buffer 관련
public:
	virtual HANDLE MakeOwnVertexBuffer( int iNumVertices, ENUM_OWN_VB_TYPE eOvt);
	virtual void DeleteOwnVertexBuffer( HANDLE hOwnVertexBuffer);
	virtual HANDLE MakeOwnIndexBuffer( int iNumVertices, ENUM_OWN_IB_TYPE eOit);
	virtual void DeleteOwnIndexBuffer( HANDLE hOwnIndexBuffer);
	virtual BOOL FillOwnVertexBuffer( HANDLE hOwnVertexBuffer, WzVector *pwvPosVert, int iNumVertex, int *piPosVertMap, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNorVert, int iPosVertDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert), int iNorVertDataSize = sizeof(WzVector));
	virtual BOOL FillOwnVertexBufferPartially( HANDLE hOwnVertexBuffer, int *piVertexIndex, WzVector *pwvPosVert, int iNumVertex, int *piPosVertMap, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNorVert, int iPosVertDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert), int iNorVertDataSize = sizeof(WzVector));		// 전체가 아닌 특정 Triangle 만 갱신하기
	virtual BOOL FillOwnVertexBufferUsingRefIndex( HANDLE hOwnVertexBuffer, WzVector *pwvPosVert, int iNumVertex, int *piPosVertMap, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNorVert, int* piRefIndex, int iPosVertDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert), int iNorVertDataSize = sizeof(WzVector), int iRefIndexDataSize = sizeof(int));
	virtual BOOL FillOwnVertexBufferPartiallyUsingRefIndex( HANDLE hOwnVertexBuffer, int *piVertexIndex, WzVector *pwvPosVert, int iNumVertex, int *piPosVertMap, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNorVert, CWzArray<int>* parRef, CWzArray<int>* parRefSeq, int iPosVertDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert), int iNorVertDataSize = sizeof(WzVector));		// 전체가 아닌 특정 Triangle 만 갱신하기

	virtual BOOL FillOwnVertexBufferTerrain( HANDLE hOwnVertexBuffer, WzVector *pwvPosVert, int iNumVertex, int *piPosVertMap, WzColor *pwcColor, WzUVVert *pwUVs, WzColor *pwcAlphaLayer, int iPosVertDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert));
	virtual BOOL FillOwnVertexBufferTerrainPartially( HANDLE hOwnVertexBuffer, int *piVertexIndex, WzVector *pwvPosVert, int iNumVertex, int *piPosVertMap, WzColor *pwcColor, WzUVVert *pwUVs, WzColor *pwcAlphaLayer, int iPosVertDataSize = sizeof ( WzVector), int iPosVertMapDataSize = sizeof ( int), int iColorDataSize = sizeof ( WzColor), int iUVDataSize = sizeof ( WzUVVert));		// 전체가 아닌 특정 Triangle 만 갱신하기
	virtual BOOL FillOwnVertexBufferbyTriangle( HANDLE hOwnVertexBuffer, int iNumTriangle, WzVector *pwvVertex, WzUVVert *pwUVs, int *piVertexMap, int *piUVMap, WzColor *pwcColor, int iVertDataSize = sizeof ( WzVector), int iUVVertSize = sizeof ( WzUVVert), int iIndexSize = 3 * sizeof ( int), int iColorDataSize = sizeof ( WzColor));
	virtual BOOL FillOwnVertexBufferbySelectedTriangle( HANDLE hOwnVertexBuffer, int *piTriangleIndex, int iNumTriangle, WzVector *pwvVertex, WzUVVert *pwUVs, int *piVertexMap, int *piUVMap, WzColor *pwcColor, int iVertDataSize = sizeof ( WzVector), int iUVVertSize = sizeof ( WzUVVert), int iIndexSize = 3 * sizeof ( int), int iColorDataSize = sizeof ( WzColor));	// 전체가 아닌 특정 Triangle 만 갱신하기
	virtual BOOL FillOwnVertexBufferBasisVector(HANDLE hOwnVertexBuffer, int nVertexCount, WzVector* pwvBasisVector, int* pnFaceIndices, int nBasisVectorStride, int nIndexStride );

	// 블렌딩버퍼
	virtual BOOL FillOwnVertexBufferBlend( HANDLE hOwnVertexBuffer, int iMaxLayer, CWzArray<int>* pBlendIndex, CWzArray<DWORD>* pBlendColor, int iBlendColorSize, float* fColor, WzVector *pwvPosVert, int iNumVertex, int *piPosVertMap, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNorVert, int iPosVertDataSize = sizeof(WzVector), int iPosVertMapDataSize = sizeof(int), int iColorDataSize = sizeof(WzColor), int iUVDataSize = sizeof(WzUVVert), int iNorVertDataSize = sizeof(WzVector));
	virtual BOOL FillOwnVertexBufferBlendPartially( HANDLE hOwnVertexBuffer, int iMaxLayer, CWzArray<int>* pBlendIndex, CWzArray<DWORD>* pBlendColor, int iBlendColorSize, float* fColor, int iMaxVertex, int *piVertexIndex, WzVector *pwvPosVert, int iNumVertex, int *piPosVertMap, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNorVert, int iPosVertDataSize = sizeof(WzVector), int iPosVertMapDataSize = sizeof(int), int iColorDataSize = sizeof(WzColor), int iUVDataSize = sizeof(WzUVVert), int iNorVertDataSize = sizeof(WzVector));		// 전체가 아닌 특정 Triangle 만 갱신하기
	virtual BOOL FillOwnVertexBufferBlendPartiallyUsingRefIndex( HANDLE hOwnVertexBuffer, int iMaxLayer, CWzArray<int>* pBlendIndex, CWzArray<DWORD>* pBlendColor, int iBlendColorSize, float* fColor, int iMaxVertex, int *piVertexIndex, WzVector *pwvPosVert, int iNumVertex, int *piPosVertMap, WzColor *pwcColor, WzUVVert *pwUVs, WzVector *pwvNorVert, CWzArray<int>* parRef, CWzArray<int>* parRefSeq, int iPosVertDataSize = sizeof(WzVector), int iPosVertMapDataSize = sizeof(int), int iColorDataSize = sizeof(WzColor), int iUVDataSize = sizeof(WzUVVert), int iNorVertDataSize = sizeof(WzVector));		// 전체가 아닌 특정 Triangle 만 갱신하기

	// Wzd rendering 시 가속 관련
public:
	BOOL MakeWzdVertexBuffer( CWzdBase *pWzd);
	void DeleteWzdVertexBuffer( CWzdBase *pWzd);
	BOOL UpdateWzdVertexBuffer( CWzdBase *pWzd, int iMeshGroup, HANDLE hVB, HANDLE hIB);
protected:
	void FillWzdVertexBuffer( CWzdBase *pWzd, int iMeshGroup, BYTE *pbyBuffer, CWzArray<WORD> *parTriListInGroup, int *piNumGroup);
	void FillWzdIndexBuffer( CWzdBase *pWzd, int iMeshGroup, BYTE *pbyBuffer, CWzArray<WORD> *parTriListInGroup, int iNumGroup);

protected:
	// 새 포맷용(IB사용) 테스트(차후 기존함수로 통합) - 현재 Shader만 적용!
	void New_FillWzdVertexBuffer( CWzdBase *pWzd, int iMeshGroup, BYTE *pbyBuffer, CWzArray<WORD> *parTriListInGroup, int *piNumGroup, UINT uiFVFSize);
	void New_FillWzdIndexBuffer( CWzdBase *pWzd, int iMeshGroup, BYTE *pbyBuffer, CWzArray<WORD> *parTriListInGroup, int iNumGroup, ENUM_OWN_IB_TYPE eOwnIBType);

	// 메쉬 분할
protected:
	int bsfs_GenerateBoneSplitInfo( St_BoneSplitInfo *apBSI, CWzArray<WORD> *parTriListInGroup, CWzArray<int> *parGroupIndexOfTri, CWzdBase *pWzd, int iMeshGroup);
	void bsfs_RestoreBoneSplitInfo( St_BoneSplitInfo *apBSI, int iCountGroup, CWzdBase *pWzd, int iMeshGroup);
	int bsfs_ComputeBoneIndex(int* piOut, int nPallete, St_BoneSplitInfo* pBSI, WzFace *pFace,WzMeshVertex *pwmvVertices,WzSkinningInfo *pwsiList,WzMeshGroup *pwmg);
	void bsfs_ReAlignment(St_BoneSplitInfo *apBSI ,int iGoupCount, int* piNeedRelocationBone ,int iNumNode);

public:
	void FillLightMapVertexBuffer( CWzdBase *pWzd, int iMeshGroup, BYTE *pbyBuffer, CWzArray<WORD> *parTriListInGroup, int *piNumGroup);
	BOOL UpdateLightMapVertexBuffer( CWzdBase *pWzd, int iMeshGroup, HANDLE hVB, HANDLE hIB);

public:
	virtual BOOL LockIndexBuffer( HANDLE hIB, void** ppIndex);
	virtual void UnlockIndexBuffer( HANDLE hIB);
};

#endif // _3DRENDERER_MANAGERVBFILL_H_