// worldterrain.h

#ifndef _3DTERRAIN_WORLDTERRAIN_H_
#define _3DTERRAIN_WORLDTERRAIN_H_

#include "worldbase.h"
#include <programcommon/wzqueue.h>

#define PATH_MESH_MAPPING				( "hide.jpg")
#define MAX_NUM_TEXTURE_FOR_TERRAIN		( 1024)
#define NO_TEXTURE_TERRAIN_MESH			( 255)

#pragma pack ( 1)


#define MAX_BLENDED_LAYER	( 3)	// Edge Blending �ִ� �ܰ�

typedef struct St_TerrainVertexInfo
{
	DWORD				m_dwPosIndex;		///< ��ġ���� �ε���
	WzVector			m_wvNormal;			///< �븻
	WzUVVert			m_wuvMap;			///< UV map

	WzColor				m_wcLightInput;		///< ���� ���� �� �ֱ� (���� �Է��� ��)
	WzColor				m_wcColorInput;		///< ���� ���� ���� �ֱ� (���� �Է��� ��)

	// ����ȵǴ� ����
	WzColor				m_wcLightCalculated; ///< �븻, m_wcLightInput �� ������ �� (m_wcMapAmbient �� ������)
	WzColor				m_wcColorCalculated; ///< �븻, m_wcColorInput, m_wcLightInput �� ������ ��	( m_wcLightCalculated �� m_wcColorInput)
	WzColor				m_wcFinalColorForVB;	// VertexBuffer�� ���� ���� Color
} St_TerrainVertexInfo;

typedef struct St_BasisVectorInfo
{
	WzVector			m_wzTangent;
	WzVector			m_wzBinormal;
} St_BasisVectorInfo;

/// ���� ���̴� ���� �޽�

typedef struct St_TerrainMesh
{
	WzOctIndex			m_woi;					///< ��Ʈ�� ���� �ε���
	int					m_iVertexIndex[3];		///< �� ���� �ε���
	int					m_iVertexIndexForLM[3]; ///< ����Ʈ�� �������� �ε���
	WzVector			m_wvNormal;				///< �ﰢ���� Normal ��
	BYTE				m_byTexture;			///< ����� �ؽ��� �ε���
	int					m_iLinkedTile;			///< ����� Ÿ�� (��ã���)

	/// @name ���� ���� - ���� �ٸ� mapping �̸鼭 ������ mesh ���� Blending ��Ų��.
	BOOL				m_bBlendingDisabled;	///< Blending ��
	BYTE				m_byBlendedTexture[MAX_BLENDED_LAYER];	///< ����� �ؽ��� �ε���	( 255 �� ����)
	BYTE				m_byBlendedAlpha[MAX_BLENDED_LAYER][3];	///< ���� �� ������ Alpha ��
#ifdef MAKE_BLEND_VERTEXBUFFER
	int					m_iVertexBlendIndex[MAX_BLENDED_LAYER][3]; ///< ���� ���ؽ� ���� �ε���
#endif
} St_TerrainMesh;

#ifdef MAKE_BLEND_VERTEXBUFFER
typedef struct St_TerrainVertexBlendBufferInfo
{
	HANDLE				m_hVB;
	int					m_iVertexCount;
} St_TerrainVertexBlendBufferInfo;
#endif

#pragma pack ()


/// Edge Blending �� ���� ���� (�ӽ÷� �����ƴ� �����)

#define MAX_CONTAININGMESH	( 128)
#define MAX_NEARVERTEX		( 128)

typedef struct
{
	// �ش� ������ �����ϴ� �޽� ����
	int					m_iNumContainingMesh;
	int					m_iContainingMesh[MAX_CONTAININGMESH];
	// �ش� ������ ���� ���� ����
	int					m_iNumNearVertex;
	int					m_iNearVertex[MAX_NEARVERTEX];
	// ���� ��ü ����
	BOOL				m_bEdge;	// �����
} St_EdgeBlending_Vertex;

typedef struct
{
	int					m_iTexture[MAX_BLENDED_LAYER];		// Blend �� �ؽ��� ��ȣ
} St_EdgeBlending_Mesh;

typedef struct
{
	int					m_iMesh;		// �޽� ��ȣ
	int					m_iLayerIndex;	// ���° Layer �ΰ�
} St_BlendList;

typedef tDistanceObject< COctNode* >	LPOctNode, LPPOCTNODE;
typedef std::vector< LPPOCTNODE >		LPOctNodeManager, LPOCTNODEMANAGER;

class CWorldTerrain : public CWorldBase  
{
public:
	CWorldTerrain();
	virtual ~CWorldTerrain();
	virtual void		Clear();

	/// @name ����/����
	virtual BOOL		Create( BOOL bUse = TRUE, BOOL bUseEdit = FALSE );
	virtual void		Destroy( CDrawBase *pDraw);
	virtual void		Release( CDrawBase *pDraw, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);

	virtual BOOL		Serialize( CWzArchive *pArchive);

#ifdef CLIENT_ONLY

	/// @name �ؽ��� �� �б�/����
	virtual BOOL		Load3DFiles( CDrawBase *pDraw, char *lpszPath);
	virtual BOOL Load3DFilesForTerrainMesh(CDrawBase *pDraw, char *lpszPath);
protected:
    virtual BOOL        RenewalTerrainOcxtree();
	BOOL				RenewalTerrainMeshToOcxtree();
	virtual BOOL		AnalyzeWzd( CWzdBase *pWzd);
	virtual void		Destroy3DFiles( CDrawBase *pDraw);

	/// @name --- ����ŧ��/Water �ؽ��� ���� ���� - �Ƹ� �����ÿ����� ���
	virtual void		SetSpecularTexture(int iMesh,char* szFileName);
	virtual void		UnSetSpecularTexture(int iMesh);
	virtual void		ReloadSpecularTexture(CDrawBase* pDraw,char* szRoot);
	virtual void		ReloadNormalmap(CDrawBase* pDraw, char* szRoot);

	/// @name --- �����ϸ� �ؽ��� ���� ���� - �Ƹ� �����ÿ����� ���
	virtual void		SetDetailTexture(int iMesh,char* szFileName);
	virtual void		UnSetDetailTexture(int iMesh);
	virtual void		ReloadDetailTexture(CDrawBase* pDraw,char* szRoot);
	virtual void		SetDetailTextureScale(int iMesh,float fScale);
	virtual float		GetDetailTextureScale(int iMesh);
	virtual void		SetDetailFunc(int iMesh,float* pfunc);
	virtual float*		GetDetailFunc(int iMesh);

	virtual const char  *GetNormalTextureFileNameByIndex(int iIndex);
	virtual void		SetNormalTexture(int iMesh, char *szFileName);
	virtual void		UnSetNormalTexture(int iMesh);
	virtual const float	GetHeightFactor(int iMesh) const;
	virtual void		SetHeightFactor(int iMesh, const float fHeightFactor);

#endif//CLIENT_ONLY

	/// @name --- ���� ����
protected:
	float				m_fGouraudForCharacter[3];
public:
#ifdef CLIENT_ONLY
	virtual void		SetGouraudForTerrainObject( BOOL bSet);
#endif


	/// @name --- WZM ���Ͽ��� ���� ����

protected:
	void				ConstructTerrainMesh( CWzdBase *pWzd);
	void				ConstructTangentBinormal();

	void				AddPosVertexList( CWzdBase *pWzd);
	void				AddTerrainMeshFromMeshGroup( CWzdBase *pWzd, int iMeshGroup, int *piMeshCount);
	int					AddTerrainVertex( DWORD dwPosVertIndex, WzVector wvNormal, WzUVVert wuvVert, WzColor wcColor, WzColor wcColor2);
	void				SetTerrainMesh( int iIndex, int *piVertInfoIndex, int iTexture);

	void				CalculateAllVertexLight(BOOL bUsingLightmap);

	/// @name --- ��� �� Blending ó��
protected:
	void				ConstructTerrainEdgeBlending();	///< �������� ���ΰ��� ��� �κ� Blending ��Ű��
	void				GatherMeshToBeBlended( CWzQueue<St_BlendList> *pqMeshToBeBlended, St_EdgeBlending_Mesh *pebmInfo, St_EdgeBlending_Vertex *pebvBuffer);

	void				CalculateUVWith( WzUVVert *puvResult, WzUVVert *puvNewLayer1, WzUVVert *puvNewLayer2, WzUVVert *puvOldLayer0, WzUVVert *puvOldLayer1, WzUVVert *puvOldLayer2);
	void				CalculateBlendedVertexAlpha( St_EdgeBlending_Vertex *pebvBuffer, St_TerrainMesh *pMesh, int iLayer);
	int					AddBlendedLayer( St_EdgeBlending_Mesh *pebmInfo, int iMesh, int iTexture);	///< Blending �� ���� ���� �ϳ� �߰���

public:
#ifdef CLIENT_ONLY
	virtual void		SetTerrainEdgeBlending( int iMesh, BOOL bShow);	///< ��� Blending �Ѱ� ����
	virtual void		RefreshTerrainEdgeBlending();	///< ��� Blending �ٽ� ���
	virtual void		SetAllTerrainEdgeBlending( BOOL bSet);	///< ��� Blending ��� ���� SetTerrainEdgeBlending �� ��� ���Ͱ� ����
	virtual void		SetTerrainEdgeBlendingByTexture(int iTexture, BOOL bSet) ;	///< ��� Blending iTexture ���� �ؽ��� ����
	virtual BOOL		GetTerrainEdgeBlending(int idx);
#endif

#ifdef CLIENT_ONLY
	/// @name Ư�� ������ ������
public:
	virtual BOOL		WrapVertexColorInfo( CWzArchive *pArchive, int iStyle = 0);	///< Vertex Color ������
#endif


	/// @name --- ���� �޽� ó��

protected:
	float				s_fBrightnessFunc[3];	///< ������ Gouraud Shading ��꿡 ���̴� ��
	/// @name Terrain Vertex
	int					m_iMaxTerrainVertices;			///< �ִ� ���� ����
	int					m_iNumTerrainVertices;			///< ���� ����
	WzVector *			m_pwvTerrainVertices;	///< ������ ���� ����

	int					m_iMaxTerrainVertInfo;			///< �ִ� ���� ���� ����
	int					m_iNumTerrainVertInfo;			///< ���� ���� ����

	St_TerrainVertexInfo *m_ptviTerrainVertexInfo;	///< ������ ��Ÿ ����

	BOOL				m_bIsBuildBasisVector;
	St_BasisVectorInfo*	m_pBasisVectorsInfo;

	/// @name Terrain Mesh
	int					m_iNumTerrainMeshes;	///< �޽� ����
	St_TerrainMesh *	m_pTerrainMeshes;	///< �޽��� ���� ����	

	BvAABB				m_bvSize;
public:
	virtual	void		SetTerrainSize( BvAABB bvSize ) { m_bvSize = bvSize; }
	virtual	BvAABB		GetTerrainSize() { return m_bvSize; }


#ifdef CLIENT_ONLY
public:
	virtual WzColor		GetTerrainVertexLight( int iVertex)	{ return ( m_ptviTerrainVertexInfo[iVertex].m_wcLightInput); }
	virtual void		SetTerrainVertexLight( int iVertex, WzColor wcInput);
	virtual void		SetAllTerrainVertexLight( WzColor wcInput);
	virtual WzColor		GetTerrainVertexColor( int iVertex)	{ return ( m_ptviTerrainVertexInfo[iVertex].m_wcColorInput); }
	virtual void		SetTerrainVertexColor( int iVertex, WzColor wcInput);
	virtual void		SetAllTerrainVertexColor(  WzColor wcInput);
	
	virtual void		Calc_TerrainVertexInfo( int iVertex);
	virtual void		Calc_TerrainVertexInfoWithLightmap(int iVertex);
	virtual WzVector	GetTerrainVertexPos( int iVertex)	{ return ( m_pwvTerrainVertices[m_ptviTerrainVertexInfo[iVertex].m_dwPosIndex]); }
	virtual WzOctIndex	GetTerrainMeshOctIndex( int iMesh)	{ return ( m_pTerrainMeshes[iMesh].m_woi); }	///< TerrainMesh ���� ���
	virtual St_TerrainMesh* GetTerrainMesh( int iMesh)		{ return &m_pTerrainMeshes[iMesh]; }
	virtual int			GetTerrainMeshCount() const			{ return m_iNumTerrainMeshes; }

	virtual void		SetBrightnessFunc( float *pfBrightnessFunc);	///< Brightness ��� ���� float * 3
	virtual void		GetBrightnessFunc( float *pfBrightnessFunc);	///< Brightness ��� ���� float * 3
	virtual void		EnableGoraudForTerrain( BOOL bEnable);	// ���� render �� gouraud ���� ���� ����

private:
	void				Calc_AllTerrainVertexInfo();
#endif
	void				CalcTerrainSize();


	/// @name --- �ؽ���
public:
#ifdef CLIENT_ONLY
	int					GetTerrainMeshRenderOrder( int nTextureIndex) { return m_iTerrainMeshRenderSequence[nTextureIndex]; }
#endif

protected:
	int					m_iNumTerrainMeshTexture;
	char *				m_lpszTerrainMeshTextureName[MAX_NUM_TEXTURE_FOR_TERRAIN];
	HANDLE				m_hTerrainMeshTexture[MAX_NUM_TEXTURE_FOR_TERRAIN];
	int					m_iTerrainMeshRenderSequence[MAX_NUM_TEXTURE_FOR_TERRAIN];
	WzColor				m_wcTerrainMeshGroupColor[MAX_NUM_TEXTURE_FOR_TERRAIN];
	WzUVVert			m_wuvTerrainMeshStream[MAX_NUM_TEXTURE_FOR_TERRAIN];
	DWORD				m_dwTerrainMeshGroupOption[MAX_NUM_TEXTURE_FOR_TERRAIN];	// ���� �޽��� ���κ� �ɼ�
	DWORD				m_dwTerrainMeshUserOption[MAX_NUM_TEXTURE_FOR_TERRAIN];
	float				m_fTerrainMeshGlow[MAX_NUM_TEXTURE_FOR_TERRAIN][3]; // ���� �޽��� �ؽ��ĺ� Glow �Ķ���Ͱ�( 0: Min, 1: Max, 2: Time)
	
	char *				m_lpszTerrainSpecularTextureName[MAX_NUM_TEXTURE_FOR_TERRAIN];
	HANDLE				m_hTerrainSpecularTexture[MAX_NUM_TEXTURE_FOR_TERRAIN];
#ifdef CLIENT_ONLY
	void				GetMaterialState( CDrawBase* pDraw, int nTextureIndex, HANDLE* phSpecular, HANDLE* phDetail, ENUM_VERTEX_SHADER_TYPE* peVST, ENUM_PIXEL_SHADER_TYPE* pePST);
#endif

	// Normalmap ���� Mesh ����
	char*				m_lpszTerrainNormalmapName[MAX_NUM_TEXTURE_FOR_TERRAIN];
	HANDLE				m_hTerrainNormalmap[MAX_NUM_TEXTURE_FOR_TERRAIN];
	float				m_fHeightFactor[MAX_NUM_TEXTURE_FOR_TERRAIN];

	char*				m_lpszTerrainDetailTextureName[MAX_NUM_TEXTURE_FOR_TERRAIN];
	HANDLE				m_hTerrainDetailTexture[MAX_NUM_TEXTURE_FOR_TERRAIN];
	float				m_fDetailMapScale[MAX_NUM_TEXTURE_FOR_TERRAIN];
	float				m_fDetailFunc[MAX_NUM_TEXTURE_FOR_TERRAIN][3];

#ifdef CLIENT_ONLY
	/// @name --- ���� ������ ����/Picking
public:
	virtual int			Enumerate_IntersectFrustum( ENUM_DATATYPE_OCTNODE dto, St_Camera *pCameraInfo, CWzArray<DWORD> *parResult);	///< Frustum �� �����ϴ� �� ���
	virtual int			RenderTerrainMesh( CDrawBase *pDraw, 
                                           CWzArray<DWORD> *parOctNodes = NULL, 
                                           ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL, 
                                           bool bBlend = true,
                                           bool is_far_alpha = true);	///< ���� ���
    virtual int			RenderTerrainMeshForDebug( CDrawBase *pDraw, 
        CWzArray<DWORD> *parOctNodes = NULL, 
        CWzArray<DWORD> *parOcculedNodes = NULL,
        ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL, 
        bool bBlend = true,
        bool is_far_alpha = true);	///< ���� ���

    virtual void        Enumerate_OccluedTerrainNode(CDrawBase *pDraw, OUT CWzArray<DWORD>& TerrainNodes, OUT CWzArray<DWORD>& OcculedTerrainNodes );


	virtual void		RenderTerrainMeshInList( CDrawBase *pDraw, 
                                                 CWzArray<DWORD> *parMesh, 
                                                 bool bBlend,
                                                 bool is_far_alpha,
                                                 ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL);	///< ���� ���
	virtual void		SetColorForDrawingMeshGroup( int iTextureGroup, WzColor wcColor);
	virtual void		SetUVStreamForTerrainMesh( int iTextureGroup, float fU, float fV);
	virtual void		GetUVStreamForTerrainMesh( int iTextureGroup, float* pfU, float* pfV);
	virtual void		SetTerrainMeshGroupOption( int iTextureGroup, DWORD dwState);	// ���� �޽��� ���κ� �ɼ� ����
	virtual void		UnsetTerrainMeshGroupOption( int iTextureGroup, DWORD dwState);	// ���� �޽��� ���κ� �ɼ� ����
	virtual DWORD		GetTerrainMeshGroupOption( int iTextureGroup);// ���� ���� �޽� ���� �ɼ� ���
	virtual void		ReplaceTerrainMeshGroupOption( int iTextureGroup,DWORD dwState) ;// ���� �޽��� ���� �ɼ��� dwState ��
	virtual DWORD		GetTerrainMeshUserOption( int iTextureGroup);	
	virtual void		ReplaceTerrainMeshUserOption( int iTextureGroup,DWORD dwState);
	virtual void		GetTerrainMeshGlow( int iTextureGroup, float *param, int count=3);
	virtual void		ReplaceTerrainMeshGlow( int iTextureGroup, float *param, int count=3);
    virtual int         terrain_drawcalls(){return terrain_drawcalls_;} //���� ������ ���� DrawPrimitive() ȣ�� Ƚ��
#endif
	virtual int			PickTerrainMesh( WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE, DWORD dwTerrainGroupOptionCheck = 0, WzVector *pwvNormal = NULL);
	virtual BOOL		CheckPickableTerrain(WzVector* pwvCollision);
	virtual int			GetTextureIndexOfTerrainMesh( int iMesh);
	virtual int			GetTextureCount();
	virtual const		char* GetTextureFileNameByIndex(int iIndex);
	virtual const		char* GetSpecularTextureFileNameByIndex(int iIndex);
	virtual const		char* GetDetailTextureFileNameByIndex(int iIndex);
	virtual const		char* GetNormalmapFileNameByIndex(int iIndex);



#ifdef CLIENT_ONLY
protected:
	void				RenderTerrainMeshBySorting( CDrawBase *pDraw, 
                                                    CWzArray<DWORD> *parMesh,
                                                    ENUM_RENDER_TERRAIN_MODE rtmMode, 
                                                    bool is_far_alpha,
                                                    int iTextureToUse = 0);
	void				RenderTerrainMeshBySortingFast( CDrawBase *pDraw, 
                                                        CWzArray<DWORD> *parMesh, 
                                                        bool is_far_alpha, 
                                                        int iTextureToUse = 0);
	void				RenderTerrainMeshLightmapBySortingFast( CDrawBase *pDraw, CWzArray<DWORD> *parMesh);
	void				RealizeRenderShadowMap( CDrawBase *pDraw, int iTextureToUse = 0);

	virtual void		RealizeRenderWater( CDrawBase *pDraw, HANDLE hTexRefraction, BOOL bBump = TRUE );
	virtual BOOL		IsExistingWater();

	virtual void		RealizeRenderGlow( CDrawBase *pDraw );
	virtual BOOL		IsExistingGlow();

	CWzArray<DWORD>		m_arMeshMap;
	CWzArray<int>		m_arVertexMap[MAX_NUM_TEXTURE_FOR_TERRAIN];	///< ������ �� �ؽ��ĺ� Sorting �� ���� ����ϴ� ����

	//CWzArray<int>		m_arVertexMapLightmapUV[MAX_NUM_TEXTURE_FOR_TERRAIN];	///< ������ �� �ؽ��ĺ� Sorting �� ���� ����ϴ� ����Ʈ�� uv ����
	CWzArray<int>		m_arVertexMapLightmap[MAX_NUM_TEXTURE_FOR_TERRAIN];
	CWzArray<int>*		m_parRefVertexToMesh;
	CWzArray<int>*		m_parRefVertexToMeshSeq;

	CWzArray<DWORD>		m_arTempTerrainNode;
	CWzArray<DWORD>		m_arMeshBlended[MAX_BLENDED_LAYER];
	CWzArray<int>		m_arMesh4Pick;

	CWzArray<DWORD>		m_arMesh;
	CWzArray<DWORD>		m_arMesh4Shadow;
	BOOL				m_bExistWater;
	BOOL				m_bExistGlow;

    int                 terrain_drawcalls_;

	St_TextureContainer* m_pTextureInfo[MAX_NUM_TEXTURE_FOR_TERRAIN];				// ���� �ؽ��� ����
	CWzArray<WzColor>	m_arColorBuffer[MAX_NUM_TEXTURE_FOR_TERRAIN];

	void				InitInstantBufferToRender();
	BOOL				RestoreTerrainMesh( CDrawBase *pDraw);

	/// @name --- Decal ó��
public:
	virtual void		CreateDecal( WzSimpleMesh **ppwsm, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness);
	virtual void		DestroyDecal( WzSimpleMesh *pwsm);
	virtual void		GetPosVertexListInDecal( int *piResultVertex, int *piNumResult, int iMaxVertex, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness);
	virtual void		GetVertexInfoListInDecal( int *piResultVertex, int *piNumResult, int iMaxVertex, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness);
	virtual void		GetMeshListInDecal( int *piResultMesh, int *piNumResult, int iMaxMesh, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness);
protected:
	void				GetMeshListForDecal( CWzList<DWORD> *plstResultMesh, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness, WzFrustum *pwfRange = NULL);
	int					CutMeshListForDecal( CWzList<DWORD> *plstMesh, WzFrustum *pwfRange, WzVector ( *pwvTemp)[3 + 6], int **ppiNumVert, WzVector wvNormal);


	/// @name --- �� ó��

	virtual void		SetSunLightDir( const WzVector& wvSunLightDir,BOOL bUpdate);	// ���� ���� ����
	virtual void		SetSunLightColor( WzColor wcSunLightColor );			// ���� ��������
	virtual void		SetMapAmbientColor( WzColor wcMapAmbient);				// ���� Ambient ���� ����
	virtual WzColor		GetTerrainLight( const WzVector& wvPos, int iMesh = -1);		// �������� �� ���


	/// @name --- D3D ���� ( Own Vertex Buffer)
protected:
	HANDLE				m_hOwnVertexBuffer;

	BOOL				m_bUsingNormalMap;
	HANDLE				m_hOwnTangentBinormalVB;

	HANDLE				m_hOwnVBLightmapUV;
	CWzArray< St_TerrainLightMap* > m_arTerrainLightmap;
	WzUVVert*			m_pwuLightmapUV;
	int*				m_piLightmapIndex;
	CWzArray< HANDLE > m_arTerrainLightmapHandle;
	CWzArray< St_UnitLightMap* > m_arUnitLightMap;


#ifdef MAKE_BLEND_VERTEXBUFFER
	St_TerrainVertexBlendBufferInfo m_tvbbVertexBlendInfo[MAX_BLENDED_LAYER][254];	// ���� ������ ���ؽ� ���� (���̾�*�ƽ��ؽ��İ���)
	void				CreateOwnVertexBlendBuffer();
	void				FillOwnVertexBlendBuffer();
	void				RemoveOwnVertexBlendBuffer();
#endif

	CDrawBase *			m_pDrawRef;
	void				CreateOwnVertexBuffer( CDrawBase *pDraw);
	void				RemoveOwnVertexBuffer();

	void				FillOwnVertexBuffer( int *piVertexList = NULL, int iNumVertex = 0);


	void				CreateOwnTangentBinormalVB();
	void				RemoveOwnTangentBinormalVB();
	void				FillOwnTangentBinormalVB(int* piTriangle = NULL, int iNumTriangle = 0 );

public:
	virtual void		UpdateOwnVertexBuffer( int* piVertexList = NULL, int iNumVertex = 0); // ���� ���� ���� update �ϱ�

	virtual void		AttachLightmap( CWzArray< St_TerrainLightMap* >* parLightmapInfo, CWzArray< HANDLE >* parTerrainLightmapHandle );
	virtual void		DetachLightmap();
	void				CreateOwnVBForLightmap();
	void				FillOwnVBForLightmap();
	void				RemoveOwnVBForLightmap();

	virtual void		AttachUnitLightmap(St_UnitLightMap *parUnitLightMap, DWORD dwNumMeshGroup);
	virtual void		DetachUnitLightmap();

#endif

	/** @name the occlusion culling group
	 *
	 *	polygon�� Ȱ���� occlusion culling ��� �ڵ�
	 *	@{
	 */

protected:
	StlVSimpOcc			m_Occlusions;			// ���� �����ӿ��� ��ȿ�� Occluder���� ����� ���� �迭
	CWzArray<DWORD>		m_arOcclusion;			// SimpOccluder�� ��� �迭
	StlVDistObjBuffer_SimpOcc m_DistBufSimpOcc; // �Ÿ����� ���� ���ĵ� SimpOccluder ���

protected:
	/** 4���� ����� ������ Occluder�� �ʿ� �߰��Ѵ�. */
	WzOctIndex			AddOccPlane(const WzVector *point, DWORD *data);
	/** Box�� �����ϴ� 8���� ������ �̿��Ͽ� Occluder�� �ʿ� �߰��Ѵ�. */
	WzOctIndex			AddOccBox(const WzVector *point, DWORD *data);
	void				ClearOcclusionAll();
	/** 
	 * Occlusion culling�� ����ϴ� ���� ������ �Լ��� ����� ���� ���� Mesh��
	 * ���� �߰� ������ Octree�� �ش� Node�� �����Ѵ�.
	 * @param a index of face.
	 * @param a WzOctIndex of the node of the octree.
	 * @param Indices of information of vertices.
	 * @return NULL.
	 */
	void				SetTerrainMeshInfoIntoOctree(const int *MeshIndex, const int nMesh, const WzID woi, const int *iVertInfoIndex, const int vertices, const bool bSetOccBound=true);
	
public:
	virtual float		GetRange_Occlusion(void) { return x_pOctree->GetRangeForOcclusion(); };
	virtual void		SetRange_Occlusion(const float range) { x_pOctree->SetRangeForOcclusion(range); };
	virtual StlVDistObjBuffer_SimpOcc &GetSortedSimpOcc() { return m_DistBufSimpOcc; }

	virtual WzOctIndex	AddOccluder(cBaseOcclusion *occluder, DWORD *data);
	virtual void		RemoveOcclusion(WzOctIndex woi, DWORD data);

	virtual WzOctIndex	UpdateOcclusion(WzOctIndex woi, DWORD data, WzVector &p1, WzVector &p2);
    virtual WzOctIndex  UpdateOcclusion(const WzOctIndex old_oct_index, 
                                        const DWORD data, 
                                        const WzVector &p1, 
                                        const WzVector &p2,
                                        const WzVector &p3, 
                                        const WzVector &p4);
	virtual WzOctIndex	UpdateOccBox(WzOctIndex woi, DWORD data, WzVector *point);

	/**
	 * Occlusion ����� ���� ���� �޽��� �����ϴ� ����� ��谪�� ������
	 * ��� �� �����Ͽ� ���� �ε� �ӵ��� ����Ű�� ���� �Լ�.
	 */
	virtual bool		SerializeOccBoundInfo(CWzArchive *pArchive);
    virtual void        RenewalOcclusionToOctree();
	virtual WzOctIndex	RenewalOcclusionToOctree(DWORD data);

#ifdef CLIENT_ONLY
	/**
	 * �׷��� ���� �޽� ����� ��´�.	 
	 *
	 * @param pDraw : Draw �������̽� - I
	 * @param arResult : ��� ��� - O
	 * @param parOctNodes : �ܺο��� ��ó���Ͽ� ����� �Ǵ� OctNode ����� ������ ���. �ش� ������κ��� ����� �����Ѵ�. - I
	 */
	void				RefineTerrainMesh(CDrawBase *pDraw, CWzArray<DWORD> &arResult, CWzArray<DWORD> *parOctNodes);
    void				RefineTerrainMeshForDebug(CDrawBase *pDraw, CWzArray<DWORD> &arResult, CWzArray<DWORD> *parOctNodes, CWzArray<DWORD> *parOcculedNodeResult);

	virtual void		RefineTerrainMeshNode(CDrawBase *pDraw, WzMatrix *pmtViewTransform, WzFrustum *pFrustum, WzVector *pEye);
	virtual void		RenderOcclusion( CDrawBase *pDraw);
	virtual void		RenderOcclusionByEnum( CDrawBase *pDraw);
    virtual CWzArray<DWORD>& GetOcclusionListByEnum();
#endif//CLIENT_ONLY

	virtual WzOctIndex	PickingOcclusion(WzRay &ray, DWORD *data);
	virtual WzOctIndex	PickingOcclusionByEnum(WzRay &ray, DWORD *data);
    //��ŷ�� ��Ŭ������ max_count��ŭ �����´�.
    virtual int         PickingOcclusionList(DWORD* OUT arry_picked_data, 
                                             const int max_count, 
                                             const WzRay& ray);
	virtual int			GetNumOcclusion( );
	virtual void		GetOcclusion( int index, WzOctIndex *woi, DWORD *data);
	
	BOOL				SerializeOcclusion( CWzArchive *pArchive);
	/**	@}	*/ // end of the occlusion culling group

	/**	@name environment object
	 *	
	 *	���� �߰����� �������� ǥ���ϱ� ���ؼ� ���Ǵ� ����� �׸��� ���� �ڵ�
	 *	@{
	 */

private:
	CWzArray<DWORD>			m_arEnvObj;

public:
	#ifdef CLIENT_ONLY
	void UpdateEnvObject(CDrawBase *pDraw);
	#endif//CLIENT_ONLY
	bool SerializeEnvObject(CWzArchive *pArchive);

	virtual WzOctIndex		AddNewEnvObject( WzID wId, WzBoundingVolume wbv, WzColor wcFog, float fFogNear, float fFogFar, float fFogRate, DWORD *pdwObject);
	virtual bool			RemoveEnvObject(const WzOctIndex woi, const DWORD dwObject);
	virtual void			ClearAllEnvObject();	
	virtual WzOctIndex		UpdateEnvObjectBoundingVolume(WzOctIndex woi, WzBoundingVolume wbv, DWORD dwObject);	
	virtual void			RenewalEnvObjectToOctree();

	/**	@}	*/ // end of environment object


	/**	@name the drawing group
	 *	
	 *	���� �߰����� �������� ǥ���ϱ� ���ؼ� ���Ǵ� ����� �׸��� ���� �ڵ�
	 *	@{
	 */

#ifdef CLIENT_ONLY
	/** �־��� 8���� �������� Box�� �׷��ִ� �Լ�. */
	virtual void		RenderBox( CDrawBase *pDraw, WzVector *pVertex, WzColor color=WzColor_RGB(255,255,255));
	/** �־��� ��� ������ �̿��Ͽ� Box�� �׷��ִ� �Լ�. */
	virtual void		RenderBox( CDrawBase *pDraw, BvAABB *pBV, WzColor color=WzColor_RGB(255,255,255));
	/** �־��� 4���� ������ �̿��Ͽ� 4���� ����� �׷��ִ� �Լ�. */
	virtual void		RenderPlane( CDrawBase *pDraw, WzVector *pVertex, WzColor color=WzColor_RGB(255,255,255));
	/** �־��� �������� ȭ�鿡 ����ִ� �Լ� */
	virtual void		RenderDot( CDrawBase *pDraw, WzVector *pVertex, int vertices, WzColor color=WzColor_RGB(255,255,255), float fDotSize = 1.0f);
	/** ���� ���� �� ���̴� ������ ��踦 �׷��ִ� �Լ�. */
	virtual void		RenderBoundVolume( CDrawBase *pDraw, DWORD dwOption);
#endif//CLIENT_ONLY

	/**	@}	*/ // end of the drawing group
public:
	/**	@Texcell force
	*	
	*	������ ��� �ؼ��� 0~1 �� ��ǥ�� �����Ѵ�.
	*	@{
	*   �ؼ��� ���� �ϱ� ������ ��� ����� �ٽ� ���ߵ�. ���� ����!
	*	�ش��Լ��� ������Ʈ �ɼ� ���� Vertex buffer�� �����Ǳ��� ���õǾ�ߵȴ�.
	*/
	virtual void ForceTexCellIdx(bool isForceBlender);
};

#endif // _3DTERRAIN_WORLDTERRAIN_H_
