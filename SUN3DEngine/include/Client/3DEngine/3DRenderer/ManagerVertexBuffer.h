// managervertexbuffer.h

#ifndef _3DRENDERER_MANAGERVERTEXBUFFER_H_
#define _3DRENDERER_MANAGERVERTEXBUFFER_H_

#include "managerbase.h"
#include "managerd3d.h"
#include <programcommon/wzlist.h>

#define NUM_INSTANCING 58	// VS Constant ������ 256���� �������� Instancing ��
#define NUM_VB2D	( 4)

typedef struct St_VertexBufferContainer
{
	int m_iMaxVertices;
	DWORD m_dwFVF;
	DWORD m_dwUsage;
	LPDIRECT3DVERTEXBUFFER9 m_pD3dVb;
	int m_iSerial;
	BOOL m_bLost;
	UINT m_uiFVFSize;
} St_VertexBufferContainer;

#define MAXNUM_IB_SECTOR	( 32)	// Index Buffer �� ��� �������� ���� �� �ִ°�

typedef struct St_IndexBufferContainer
{
	int m_iMaxVertices;
	DWORD m_dwUsage;
	D3DFORMAT m_d3dFormat;
	LPDIRECT3DINDEXBUFFER9 m_pD3dIb;
	WORD m_wNumVertexInIBSector[MAXNUM_IB_SECTOR];
	int m_iSerial;
	BOOL m_bLost;
} St_IndexBufferContainer;


class CManagerVertexBuffer : public CManagerBase, CManagerD3D
{
public:
	CManagerVertexBuffer();
	virtual ~CManagerVertexBuffer();
	virtual void Clear();

	virtual BOOL Create( CDrawBase *pParentDraw, void *pParam);
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	virtual BOOL DrawPrimitive( int iPrimitiveType, int iNumPrimitive, DWORD dwFVF, UINT uiFVFSize, int iStartVertex, LPDIRECT3DVERTEXBUFFER9 pVB);
	virtual BOOL DrawIndexedPrimitive( int iPrimitiveType, int iNumPrimitive, DWORD dwFVF, UINT uiFVFSize, int iStartIndex, int iNumVertex, LPDIRECT3DINDEXBUFFER9 pIB, LPDIRECT3DVERTEXBUFFER9 pVB, int iVertexShader = 0, int iPixelShader = 0);
	virtual BOOL DrawIndexedPrimitive( int iPrimitiveType, int iNumPrimitive, DWORD dwFVF, UINT uiFVFSize, int iStartIndex, int iNumVertex, int iBaseVertexIndex, UINT MinIndex, LPDIRECT3DINDEXBUFFER9 pIB, LPDIRECT3DVERTEXBUFFER9 pVB, int iVertexShader=0, int iPixelShader=0);

	/// @name ���ؽ� ����
protected:
#define NUM_VB_LIST		( 7)
	CWzList<St_VertexBufferContainer> m_lstVbc[NUM_VB_LIST];	///< �ʿ� Vertex ���� ���� 6 �ܰ�� �и��ؼ� ����
	CWzList<St_IndexBufferContainer> m_lstIbc[NUM_VB_LIST];
public:
	virtual LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer( int iNumVertices, DWORD dwUsage, DWORD dwFVF);
	virtual LPDIRECT3DINDEXBUFFER9 GetIndexBuffer( int iNumVertices, DWORD dwUsage, D3DFORMAT d3dFormat);
	UINT GetSizeOfFVF( DWORD dwFVF);


protected:
	int GetMaxVBSizeOfLevel( int iLevel)
	{
		int iStandardBufferSize[NUM_VB_LIST] = { 32, 128, 512, 2048, 4096, 4906*4, 4906*64};
		return ( iStandardBufferSize[iLevel]);
	}
	int GetLevelOfVBSize( int iSize);	///< �ʿ��� m_lstVbc ��ȣ ���
	BOOL CreateVertexBuffer( St_VertexBufferContainer *pvbc);
	void DeleteVertexBuffer( St_VertexBufferContainer *pvbc);
	BOOL CreateIndexBuffer( St_IndexBufferContainer *pibc);
	void DeleteIndexBuffer( St_IndexBufferContainer *pibc);
	int m_iMaxSizeOfVertexBuffer, m_iTotalSizeOfVertexBuffer;

	/// @name �ڽŸ��� VB/IB ���� ó��
	// VB
public:
	HANDLE MakeOwnVertexBuffer( int iNumVertices, DWORD dwUsage, DWORD dwFVF);
	void DeleteOwnVertexBuffer( HANDLE hOwnVertexBuffer);
	LPDIRECT3DVERTEXBUFFER9 GetVBOfOwnVertexBuffer( HANDLE hOwnVertexBuffer, int *piNumVertex = NULL, DWORD *pdwFVF = NULL, UINT *puiFVFSize = NULL);
	virtual BOOL IsLostOwnVertexBuffer( HANDLE hOwnVertexBuffer);	// Own VB �� ������ �սǵƱ� ������, �ٽ� ä����� �ϴ°�?
	virtual void SetOwnVertexBufferRestored( HANDLE hOwnVertexBuffer);	// ���� ������ �˸�
	BOOL DrawOwnVertexBuffer( HANDLE hOwnVertexBuffer, int iNumPrimitive, int iVertexShader = 0, int iPixelShader = 0, int iNumBone = 0, WzMatrix *pwmTransform = NULL);
protected:
	CWzHandleManager<St_VertexBufferContainer*> m_hmOwnVertexBuffer;
	// IB
public:
	void DeleteOwnIndexBuffer( HANDLE hOwnIndexBuffer);
	HANDLE MakeOwnIndexBuffer( int iNumVertices, DWORD dwUsage, D3DFORMAT d3dFormat);
	LPDIRECT3DINDEXBUFFER9 GetIBOfOwnIndexBuffer( HANDLE hOwnIndexBuffer, int *piNumVertex = NULL);
	WORD* GetNumVertexPtrInIB( HANDLE hOwnIndexBuffer);

	virtual BOOL IsLostOwnIndexBuffer( HANDLE hOwnIndexBuffer);	// Own IB �� ������ �սǵƱ� ������, �ٽ� ä����� �ϴ°�?
	virtual void SetOwnIndexBufferRestored( HANDLE hOwnIndexBuffer);	// ���� ������ �˸�
	BOOL DrawOwnIndexBuffer( HANDLE hOwnIndexBuffer, HANDLE hOwnVertexBuffer, int iNumPrimitive, int iVertexShader = 0, int iPixelShader = 0, int iNumBone = 0, WzMatrix *pwmTransform = NULL,void* pvParam=NULL, int iDrawLevel = 0);


	BOOL DrawOwnIndexBufferInstancing( HANDLE hOwnIndexBuffer, HANDLE hOwnVertexBuffer, int iNumPrimitive, CWzArray<St_InstancingInfo*>* parInstancingInfo, int iArraySize, int iPixelShader = 0);

	BOOL DrawOwnLightMapBuffer( HANDLE hOwnIndexBuffer, HANDLE hOwnVertexBuffer, int iNumPrimitive, int iVertexShader = 0, int iPixelShader = 0, int iNumBone = 0, WzMatrix *pwmTransform = NULL,void* pvParam=NULL);

protected:
	CWzHandleManager<St_IndexBufferContainer*> m_hmOwnIndexBuffer;

	/// @name �ø��� ó��
protected:
	int m_iBufferSerial[2];	///< ���۵��� �ø��� ��ȣ	[0] : VB, [1] : IB
#define NUM_LATEST_VB	( 32)
	int m_iLatestBufferSeek[2];
	int m_iLatestBufferIndex[2][NUM_LATEST_VB];
	BOOL IsInLatestVB( int iSerial);
	void UpdateLatestVB( int iSerial);
	BOOL IsInLatestIB( int iSerial);
	void UpdateLatestIB( int iSerial);
public:
	virtual void Refresh();

	/// @name Hyper render ���� ����
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pvbHyperRenderBuffer;
	int m_iSizeHyperRenderBuffer;
	DWORD m_dwFVFForHyperRender;
	int m_iCurDstInHyperRenderMode;
	int m_iNumVerticesInHyperRenderBuffer;
public:
	BOOL StartHyperRenderMode( int iNumVertices, DWORD dwUsage, DWORD dwFVF);
	BOOL EndHyperRenderMode( int iPrimitiveType);
	LPDIRECT3DVERTEXBUFFER9 GetHyperRenderModeLockInfo( int iNumVertices, DWORD dwFVF, int *piOffsetToLock, int *piSizeToLock);

public:
	void InitPrimitive();
	enum { MAX_STREAM = 12 };
	HRESULT SetStreamSource( LPDIRECT3DVERTEXBUFFER9 pVB, UINT uiStride, UINT uiStreamNumber = 0);
	HRESULT SetIndices( LPDIRECT3DINDEXBUFFER9 pIB);
protected:
	LPDIRECT3DVERTEXBUFFER9	m_lpCurVB[MAX_STREAM];
	LPDIRECT3DINDEXBUFFER9 m_lpCurIB;

	// 2D�� VB
protected:
	int m_iVB2DIndex;
	St_VertexBufferContainer m_VB2D[NUM_VB2D];
	void CreateVB2D();
	void DestroyVB2D();
public:
	virtual LPDIRECT3DVERTEXBUFFER9 Get2DVertexBuffer();

protected:
	St_VertexBufferContainer m_VSB2D;
	BOOL CreateVSB2D( int iNumVertices);
	void DestroyVSB2D();
public:
	virtual LPDIRECT3DVERTEXBUFFER9 Get2DVSBuffer() {	return m_VSB2D.m_pD3dVb; }
	virtual const St_VertexBufferContainer* Get2DVSBufferContainer() const { return &m_VSB2D; }
};

#endif // _3DRENDERER_MANAGERVERTEXBUFFER_H_
