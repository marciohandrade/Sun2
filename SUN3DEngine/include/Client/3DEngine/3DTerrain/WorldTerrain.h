// worldterrain.h

#ifndef _3DTERRAIN_WORLDTERRAIN_H_
#define _3DTERRAIN_WORLDTERRAIN_H_

#include "worldbase.h"
#include <programcommon/wzqueue.h>

#define PATH_MESH_MAPPING				( "hide.jpg")
#define MAX_NUM_TEXTURE_FOR_TERRAIN		( 1024)
#define NO_TEXTURE_TERRAIN_MESH			( 255)

#pragma pack ( 1)


#define MAX_BLENDED_LAYER	( 3)	// Edge Blending 최대 단계

typedef struct St_TerrainVertexInfo
{
	DWORD				m_dwPosIndex;		///< 위치정보 인덱스
	WzVector			m_wvNormal;			///< 노말
	WzUVVert			m_wuvMap;			///< UV map

	WzColor				m_wcLightInput;		///< 정점 단위 명도 넣기 (툴로 입력할 값)
	WzColor				m_wcColorInput;		///< 정점 단위 색상 넣기 (툴로 입력할 값)

	// 저장안되는 값들
	WzColor				m_wcLightCalculated; ///< 노말, m_wcLightInput 을 참조한 값 (m_wcMapAmbient 도 참조됨)
	WzColor				m_wcColorCalculated; ///< 노말, m_wcColorInput, m_wcLightInput 을 참조한 값	( m_wcLightCalculated 과 m_wcColorInput)
	WzColor				m_wcFinalColorForVB;	// VertexBuffer에 들어가는 최종 Color
} St_TerrainVertexInfo;

typedef struct St_BasisVectorInfo
{
	WzVector			m_wzTangent;
	WzVector			m_wzBinormal;
} St_BasisVectorInfo;

/// 눈에 보이는 지형 메쉬

typedef struct St_TerrainMesh
{
	WzOctIndex			m_woi;					///< 옥트리 상의 인덱스
	int					m_iVertexIndex[3];		///< 세 점의 인덱스
	int					m_iVertexIndexForLM[3]; ///< 라이트맵 렌더링용 인덱스
	WzVector			m_wvNormal;				///< 삼각형의 Normal 값
	BYTE				m_byTexture;			///< 사용한 텍스쳐 인덱스
	int					m_iLinkedTile;			///< 연결된 타일 (길찾기용)

	/// @name 블렌딩 정보 - 서로 다른 mapping 이면서 인접한 mesh 들은 Blending 시킨다.
	BOOL				m_bBlendingDisabled;	///< Blending 끔
	BYTE				m_byBlendedTexture[MAX_BLENDED_LAYER];	///< 사용한 텍스쳐 인덱스	( 255 면 없음)
	BYTE				m_byBlendedAlpha[MAX_BLENDED_LAYER][3];	///< 블렌딩 된 정점의 Alpha 값
#ifdef MAKE_BLEND_VERTEXBUFFER
	int					m_iVertexBlendIndex[MAX_BLENDED_LAYER][3]; ///< 블렌딩 버텍스 버퍼 인덱스
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


/// Edge Blending 을 위한 정보 (임시로 생성됐다 사라짐)

#define MAX_CONTAININGMESH	( 128)
#define MAX_NEARVERTEX		( 128)

typedef struct
{
	// 해당 정점을 포함하는 메쉬 정보
	int					m_iNumContainingMesh;
	int					m_iContainingMesh[MAX_CONTAININGMESH];
	// 해당 정점과 붙은 정점 정보
	int					m_iNumNearVertex;
	int					m_iNearVertex[MAX_NEARVERTEX];
	// 정점 자체 정보
	BOOL				m_bEdge;	// 경계임
} St_EdgeBlending_Vertex;

typedef struct
{
	int					m_iTexture[MAX_BLENDED_LAYER];		// Blend 될 텍스쳐 번호
} St_EdgeBlending_Mesh;

typedef struct
{
	int					m_iMesh;		// 메쉬 번호
	int					m_iLayerIndex;	// 몇번째 Layer 인가
} St_BlendList;

typedef tDistanceObject< COctNode* >	LPOctNode, LPPOCTNODE;
typedef std::vector< LPPOCTNODE >		LPOctNodeManager, LPOCTNODEMANAGER;

class CWorldTerrain : public CWorldBase  
{
public:
	CWorldTerrain();
	virtual ~CWorldTerrain();
	virtual void		Clear();

	/// @name 생성/제거
	virtual BOOL		Create( BOOL bUse = TRUE, BOOL bUseEdit = FALSE );
	virtual void		Destroy( CDrawBase *pDraw);
	virtual void		Release( CDrawBase *pDraw, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);

	virtual BOOL		Serialize( CWzArchive *pArchive);

#ifdef CLIENT_ONLY

	/// @name 텍스쳐 등 읽기/제거
	virtual BOOL		Load3DFiles( CDrawBase *pDraw, char *lpszPath);
	virtual BOOL Load3DFilesForTerrainMesh(CDrawBase *pDraw, char *lpszPath);
protected:
    virtual BOOL        RenewalTerrainOcxtree();
	BOOL				RenewalTerrainMeshToOcxtree();
	virtual BOOL		AnalyzeWzd( CWzdBase *pWzd);
	virtual void		Destroy3DFiles( CDrawBase *pDraw);

	/// @name --- 스펙큘라/Water 텍스쳐 정보 세팅 - 아마 에디팅에서만 사용
	virtual void		SetSpecularTexture(int iMesh,char* szFileName);
	virtual void		UnSetSpecularTexture(int iMesh);
	virtual void		ReloadSpecularTexture(CDrawBase* pDraw,char* szRoot);
	virtual void		ReloadNormalmap(CDrawBase* pDraw, char* szRoot);

	/// @name --- 디테일맵 텍스쳐 정보 세팅 - 아마 에디팅에서만 사용
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

	/// @name --- 설정 변경
protected:
	float				m_fGouraudForCharacter[3];
public:
#ifdef CLIENT_ONLY
	virtual void		SetGouraudForTerrainObject( BOOL bSet);
#endif


	/// @name --- WZM 파일에서 지형 생성

protected:
	void				ConstructTerrainMesh( CWzdBase *pWzd);
	void				ConstructTangentBinormal();

	void				AddPosVertexList( CWzdBase *pWzd);
	void				AddTerrainMeshFromMeshGroup( CWzdBase *pWzd, int iMeshGroup, int *piMeshCount);
	int					AddTerrainVertex( DWORD dwPosVertIndex, WzVector wvNormal, WzUVVert wuvVert, WzColor wcColor, WzColor wcColor2);
	void				SetTerrainMesh( int iIndex, int *piVertInfoIndex, int iTexture);

	void				CalculateAllVertexLight(BOOL bUsingLightmap);

	/// @name --- 경계 간 Blending 처리
protected:
	void				ConstructTerrainEdgeBlending();	///< 지형에서 맵핑간의 경계 부분 Blending 시키기
	void				GatherMeshToBeBlended( CWzQueue<St_BlendList> *pqMeshToBeBlended, St_EdgeBlending_Mesh *pebmInfo, St_EdgeBlending_Vertex *pebvBuffer);

	void				CalculateUVWith( WzUVVert *puvResult, WzUVVert *puvNewLayer1, WzUVVert *puvNewLayer2, WzUVVert *puvOldLayer0, WzUVVert *puvOldLayer1, WzUVVert *puvOldLayer2);
	void				CalculateBlendedVertexAlpha( St_EdgeBlending_Vertex *pebvBuffer, St_TerrainMesh *pMesh, int iLayer);
	int					AddBlendedLayer( St_EdgeBlending_Mesh *pebmInfo, int iMesh, int iTexture);	///< Blending 을 위한 층이 하나 추가됨

public:
#ifdef CLIENT_ONLY
	virtual void		SetTerrainEdgeBlending( int iMesh, BOOL bShow);	///< 경계 Blending 켜고 끄기
	virtual void		RefreshTerrainEdgeBlending();	///< 경계 Blending 다시 계산
	virtual void		SetAllTerrainEdgeBlending( BOOL bSet);	///< 경계 Blending 모두 끄기 SetTerrainEdgeBlending 로 모두 끈것과 같음
	virtual void		SetTerrainEdgeBlendingByTexture(int iTexture, BOOL bSet) ;	///< 경계 Blending iTexture 관련 텍스쳐 끄기
	virtual BOOL		GetTerrainEdgeBlending(int idx);
#endif

#ifdef CLIENT_ONLY
	/// @name 특정 정보만 덮어씌우기
public:
	virtual BOOL		WrapVertexColorInfo( CWzArchive *pArchive, int iStyle = 0);	///< Vertex Color 덮어씌우기
#endif


	/// @name --- 지형 메쉬 처리

protected:
	float				s_fBrightnessFunc[3];	///< 지형의 Gouraud Shading 계산에 쓰이는 값
	/// @name Terrain Vertex
	int					m_iMaxTerrainVertices;			///< 최대 정점 개수
	int					m_iNumTerrainVertices;			///< 정점 개수
	WzVector *			m_pwvTerrainVertices;	///< 정점을 담은 버퍼

	int					m_iMaxTerrainVertInfo;			///< 최대 정점 정보 개수
	int					m_iNumTerrainVertInfo;			///< 정점 정보 개수

	St_TerrainVertexInfo *m_ptviTerrainVertexInfo;	///< 정점의 기타 정보

	BOOL				m_bIsBuildBasisVector;
	St_BasisVectorInfo*	m_pBasisVectorsInfo;

	/// @name Terrain Mesh
	int					m_iNumTerrainMeshes;	///< 메쉬 개수
	St_TerrainMesh *	m_pTerrainMeshes;	///< 메쉬를 담은 버퍼	

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
	virtual WzOctIndex	GetTerrainMeshOctIndex( int iMesh)	{ return ( m_pTerrainMeshes[iMesh].m_woi); }	///< TerrainMesh 정보 얻기
	virtual St_TerrainMesh* GetTerrainMesh( int iMesh)		{ return &m_pTerrainMeshes[iMesh]; }
	virtual int			GetTerrainMeshCount() const			{ return m_iNumTerrainMeshes; }

	virtual void		SetBrightnessFunc( float *pfBrightnessFunc);	///< Brightness 계산 공식 float * 3
	virtual void		GetBrightnessFunc( float *pfBrightnessFunc);	///< Brightness 계산 공식 float * 3
	virtual void		EnableGoraudForTerrain( BOOL bEnable);	// 지형 render 시 gouraud 적용 여부 세팅

private:
	void				Calc_AllTerrainVertexInfo();
#endif
	void				CalcTerrainSize();


	/// @name --- 텍스쳐
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
	DWORD				m_dwTerrainMeshGroupOption[MAX_NUM_TEXTURE_FOR_TERRAIN];	// 지형 메쉬의 매핑별 옵션
	DWORD				m_dwTerrainMeshUserOption[MAX_NUM_TEXTURE_FOR_TERRAIN];
	float				m_fTerrainMeshGlow[MAX_NUM_TEXTURE_FOR_TERRAIN][3]; // 지형 메쉬의 텍스쳐별 Glow 파라미터값( 0: Min, 1: Max, 2: Time)
	
	char *				m_lpszTerrainSpecularTextureName[MAX_NUM_TEXTURE_FOR_TERRAIN];
	HANDLE				m_hTerrainSpecularTexture[MAX_NUM_TEXTURE_FOR_TERRAIN];
#ifdef CLIENT_ONLY
	void				GetMaterialState( CDrawBase* pDraw, int nTextureIndex, HANDLE* phSpecular, HANDLE* phDetail, ENUM_VERTEX_SHADER_TYPE* peVST, ENUM_PIXEL_SHADER_TYPE* pePST);
#endif

	// Normalmap 적용 Mesh 정보
	char*				m_lpszTerrainNormalmapName[MAX_NUM_TEXTURE_FOR_TERRAIN];
	HANDLE				m_hTerrainNormalmap[MAX_NUM_TEXTURE_FOR_TERRAIN];
	float				m_fHeightFactor[MAX_NUM_TEXTURE_FOR_TERRAIN];

	char*				m_lpszTerrainDetailTextureName[MAX_NUM_TEXTURE_FOR_TERRAIN];
	HANDLE				m_hTerrainDetailTexture[MAX_NUM_TEXTURE_FOR_TERRAIN];
	float				m_fDetailMapScale[MAX_NUM_TEXTURE_FOR_TERRAIN];
	float				m_fDetailFunc[MAX_NUM_TEXTURE_FOR_TERRAIN][3];

#ifdef CLIENT_ONLY
	/// @name --- 지형 렌더링 관련/Picking
public:
	virtual int			Enumerate_IntersectFrustum( ENUM_DATATYPE_OCTNODE dto, St_Camera *pCameraInfo, CWzArray<DWORD> *parResult);	///< Frustum 과 교차하는 것 얻기
	virtual int			RenderTerrainMesh( CDrawBase *pDraw, 
                                           CWzArray<DWORD> *parOctNodes = NULL, 
                                           ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL, 
                                           bool bBlend = true,
                                           bool is_far_alpha = true);	///< 지형 출력
    virtual int			RenderTerrainMeshForDebug( CDrawBase *pDraw, 
        CWzArray<DWORD> *parOctNodes = NULL, 
        CWzArray<DWORD> *parOcculedNodes = NULL,
        ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL, 
        bool bBlend = true,
        bool is_far_alpha = true);	///< 지형 출력

    virtual void        Enumerate_OccluedTerrainNode(CDrawBase *pDraw, OUT CWzArray<DWORD>& TerrainNodes, OUT CWzArray<DWORD>& OcculedTerrainNodes );


	virtual void		RenderTerrainMeshInList( CDrawBase *pDraw, 
                                                 CWzArray<DWORD> *parMesh, 
                                                 bool bBlend,
                                                 bool is_far_alpha,
                                                 ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL);	///< 지형 출력
	virtual void		SetColorForDrawingMeshGroup( int iTextureGroup, WzColor wcColor);
	virtual void		SetUVStreamForTerrainMesh( int iTextureGroup, float fU, float fV);
	virtual void		GetUVStreamForTerrainMesh( int iTextureGroup, float* pfU, float* pfV);
	virtual void		SetTerrainMeshGroupOption( int iTextureGroup, DWORD dwState);	// 지형 메쉬의 매핑별 옵션 세팅
	virtual void		UnsetTerrainMeshGroupOption( int iTextureGroup, DWORD dwState);	// 지형 메쉬의 매핑별 옵션 끄기
	virtual DWORD		GetTerrainMeshGroupOption( int iTextureGroup);// 현재 지형 메쉬 매핑 옵션 얻기
	virtual void		ReplaceTerrainMeshGroupOption( int iTextureGroup,DWORD dwState) ;// 지형 메쉬의 매핑 옵션을 dwState 로
	virtual DWORD		GetTerrainMeshUserOption( int iTextureGroup);	
	virtual void		ReplaceTerrainMeshUserOption( int iTextureGroup,DWORD dwState);
	virtual void		GetTerrainMeshGlow( int iTextureGroup, float *param, int count=3);
	virtual void		ReplaceTerrainMeshGlow( int iTextureGroup, float *param, int count=3);
    virtual int         terrain_drawcalls(){return terrain_drawcalls_;} //지형 렌더링 시의 DrawPrimitive() 호출 횟수
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
	CWzArray<int>		m_arVertexMap[MAX_NUM_TEXTURE_FOR_TERRAIN];	///< 렌더링 시 텍스쳐별 Sorting 을 위해 사용하는 버퍼

	//CWzArray<int>		m_arVertexMapLightmapUV[MAX_NUM_TEXTURE_FOR_TERRAIN];	///< 렌더링 시 텍스쳐별 Sorting 을 위해 사용하는 라이트맵 uv 버퍼
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

	St_TextureContainer* m_pTextureInfo[MAX_NUM_TEXTURE_FOR_TERRAIN];				// 지형 텍스쳐 정보
	CWzArray<WzColor>	m_arColorBuffer[MAX_NUM_TEXTURE_FOR_TERRAIN];

	void				InitInstantBufferToRender();
	BOOL				RestoreTerrainMesh( CDrawBase *pDraw);

	/// @name --- Decal 처리
public:
	virtual void		CreateDecal( WzSimpleMesh **ppwsm, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness);
	virtual void		DestroyDecal( WzSimpleMesh *pwsm);
	virtual void		GetPosVertexListInDecal( int *piResultVertex, int *piNumResult, int iMaxVertex, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness);
	virtual void		GetVertexInfoListInDecal( int *piResultVertex, int *piNumResult, int iMaxVertex, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness);
	virtual void		GetMeshListInDecal( int *piResultMesh, int *piNumResult, int iMaxMesh, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness);
protected:
	void				GetMeshListForDecal( CWzList<DWORD> *plstResultMesh, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness, WzFrustum *pwfRange = NULL);
	int					CutMeshListForDecal( CWzList<DWORD> *plstMesh, WzFrustum *pwfRange, WzVector ( *pwvTemp)[3 + 6], int **ppiNumVert, WzVector wvNormal);


	/// @name --- 빛 처리

	virtual void		SetSunLightDir( const WzVector& wvSunLightDir,BOOL bUpdate);	// 빛의 방향 지정
	virtual void		SetSunLightColor( WzColor wcSunLightColor );			// 빙의 색깔지정
	virtual void		SetMapAmbientColor( WzColor wcMapAmbient);				// 맵의 Ambient 색상 지정
	virtual WzColor		GetTerrainLight( const WzVector& wvPos, int iMesh = -1);		// 지형에서 빛 얻기


	/// @name --- D3D 가속 ( Own Vertex Buffer)
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
	St_TerrainVertexBlendBufferInfo m_tvbbVertexBlendInfo[MAX_BLENDED_LAYER][254];	// 블렌딩 지형용 버텍스 버퍼 (레이어*맥스텍스쳐갯수)
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
	virtual void		UpdateOwnVertexBuffer( int* piVertexList = NULL, int iNumVertex = 0); // 가속 관련 버퍼 update 하기

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
	 *	polygon을 활용한 occlusion culling 기능 코드
	 *	@{
	 */

protected:
	StlVSimpOcc			m_Occlusions;			// 현재 프레임에서 유효한 Occluder들의 목록을 담은 배열
	CWzArray<DWORD>		m_arOcclusion;			// SimpOccluder를 담는 배열
	StlVDistObjBuffer_SimpOcc m_DistBufSimpOcc; // 거리값에 따라 정렬된 SimpOccluder 목록

protected:
	/** 4각형 평면의 형태인 Occluder를 맵에 추가한다. */
	WzOctIndex			AddOccPlane(const WzVector *point, DWORD *data);
	/** Box를 구성하는 8개의 정점을 이용하여 Occluder를 맵에 추가한다. */
	WzOctIndex			AddOccBox(const WzVector *point, DWORD *data);
	void				ClearOcclusionAll();
	/** 
	 * Occlusion culling을 사용하는 지형 랜더링 함수의 사용을 위한 지형 Mesh에
	 * 대한 추가 정보를 Octree의 해당 Node에 설정한다.
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
	 * Occlusion 계산을 위한 지형 메쉬를 포함하는 노드의 경계값을 사전에
	 * 계산 후 저장하여 맵의 로딩 속도를 향상시키기 위한 함수.
	 */
	virtual bool		SerializeOccBoundInfo(CWzArchive *pArchive);
    virtual void        RenewalOcclusionToOctree();
	virtual WzOctIndex	RenewalOcclusionToOctree(DWORD data);

#ifdef CLIENT_ONLY
	/**
	 * 그려질 지형 메쉬 목록을 얻는다.	 
	 *
	 * @param pDraw : Draw 인터페이스 - I
	 * @param arResult : 결과 목록 - O
	 * @param parOctNodes : 외부에서 전처리하여 대상이 되는 OctNode 목록을 생성한 경우. 해당 목록으로부터 목록을 생성한다. - I
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
    //픽킹된 오클루젼을 max_count만큼 가져온다.
    virtual int         PickingOcclusionList(DWORD* OUT arry_picked_data, 
                                             const int max_count, 
                                             const WzRay& ray);
	virtual int			GetNumOcclusion( );
	virtual void		GetOcclusion( int index, WzOctIndex *woi, DWORD *data);
	
	BOOL				SerializeOcclusion( CWzArchive *pArchive);
	/**	@}	*/ // end of the occlusion culling group

	/**	@name environment object
	 *	
	 *	맵의 추가적인 정보들을 표시하기 위해서 사용되는 공통된 그리기 지원 코드
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
	 *	맵의 추가적인 정보들을 표시하기 위해서 사용되는 공통된 그리기 지원 코드
	 *	@{
	 */

#ifdef CLIENT_ONLY
	/** 주어진 8개의 정점으로 Box를 그려주는 함수. */
	virtual void		RenderBox( CDrawBase *pDraw, WzVector *pVertex, WzColor color=WzColor_RGB(255,255,255));
	/** 주어진 경계 정보를 이용하여 Box를 그려주는 함수. */
	virtual void		RenderBox( CDrawBase *pDraw, BvAABB *pBV, WzColor color=WzColor_RGB(255,255,255));
	/** 주어진 4개의 정점을 이용하여 4각형 평면을 그려주는 함수. */
	virtual void		RenderPlane( CDrawBase *pDraw, WzVector *pVertex, WzColor color=WzColor_RGB(255,255,255));
	/** 주어진 정점들을 화면에 찍어주는 함수 */
	virtual void		RenderDot( CDrawBase *pDraw, WzVector *pVertex, int vertices, WzColor color=WzColor_RGB(255,255,255), float fDotSize = 1.0f);
	/** 맵의 노드들 중 보이는 노드들의 경계를 그려주는 함수. */
	virtual void		RenderBoundVolume( CDrawBase *pDraw, DWORD dwOption);
#endif//CLIENT_ONLY

	/**	@}	*/ // end of the drawing group
public:
	/**	@Texcell force
	*	
	*	강제로 모든 텍셀을 0~1 의 좌표로 강제한다.
	*	@{
	*   텍셀을 강제 하기 때문에 모든 계산이 다시 들어가야됨. 성능 느림!
	*	해당함수는 업데이트 될수 없음 Vertex buffer가 생성되기전 셋팅되어야된다.
	*/
	virtual void ForceTexCellIdx(bool isForceBlender);
};

#endif // _3DTERRAIN_WORLDTERRAIN_H_
