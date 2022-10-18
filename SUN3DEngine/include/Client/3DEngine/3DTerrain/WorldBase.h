// worldbase.h

#ifndef _3DTERRAIN_WORLDBASE_H_
#define _3DTERRAIN_WORLDBASE_H_

#include "typedef.h"

#include "roommanager.h"
#include "pathfinderbase.h"
#include "skybox.h"
#include "terrainsound.h"
#include "loadhelper.h"
#include "envobject.h"
#include "simpleocclusion.h"

#ifdef CLIENT_ONLY
#include "light.h"
#endif


// 전처리 선언문
class	CDrawBase;
class	CWzArchive;
class	CManagerLightTerrain;


/// @name 지형 render 모드

enum ENUM_RENDER_TERRAIN_MODE
{
	RTM_NORMAL = 0,		///< 일반 모드 ( m_wcColorCalculated 참조)
	RTM_EDGE_WITHALPHA,	///< Edge 출력 - Alpha 값 참조 ( m_byBlendedAlpha 참조)
	RTM_LIGHT,			///< 광원 모드 ( m_wcLightCalculated 참조)
	RTM_COLOR,			///< 입력한 색상 모드 ( m_wcColorInput 참조)
	RTM_ONLY_BLENDED,	///< Blending 된 메쉬만 출력
};


enum ENUM_TERRAIN_INFO_MODE
{
	TIM_ALL,				//. 모든 정보 
	TIM_ONLY_TERRAIN,		//. 오직 Terrain 정보
	TIM_ONLY_PATHFINDER,	//. 오직 길찾기 정보
};


/// World 의 State

#define WS_DISABLE_GOURAUD_FOR_TERRAIN	( 0x00000001)	// 지형 render 시 비 방향에 의한 gouraud 적용 할 것인지 여부 (save 되는 값)
#define WS_GOURAUD_FOR_TERRAIN_OBJECT	( 0x00010000)	// 배경 Object 를 render 시 gouraud 적용할것인지 여부
#define	WS_ENABLE_INDOOR_LIGHT	( 0x00000010)


/// 맵의 라이트 맵 정보를 담는 구조체
typedef struct St_LightmapInfo
{
	char	szFilename[_MAX_PATH];	///< 라이트 맵 정보를 담는 파일 명
	float	fBrightness;			///< 라이트 맵 적용시 사용하는 파라미터값
} LIGHTMAPINFO;


class CWorldBase  
{
public:
	CWorldBase();
	virtual ~CWorldBase();
	virtual void Clear();

	/// @name 생성/제거
	virtual BOOL Create( BOOL bUse = TRUE, BOOL bUseEdit = FALSE );
	virtual void Destroy( CDrawBase *pDraw);
	virtual void Release( CDrawBase *pDraw,ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);

	// ---------------------------------------------------------------------------------------------
	/// @name --- CDiscAccess
protected:
	CDiscAccess*        m_pDiscAccess;
	BOOL                m_bDiscAccessAuto;
public:
	virtual void		SetDiscAccess( CDiscAccess *pDiscAccess, BOOL bAuto = FALSE);
	virtual CDiscAccess *GetDiscAccess();

	CDiscAccess			*CheckDiscAccessExistence();
	BOOL				IsDiscAccessGeneratedAutomatically();

	inline CEnvObjectManager	*GetEnvObjectManager(){ return x_pEnvObjectManager;}
	virtual BOOL		Serialize( CWzArchive *pArchive);

	virtual DWORD		GenerateCheckSum();

protected:
	BOOL JumpToJunk( CWzArchive *pArchive, unsigned short nChunkID);
public:
	/// @name 기타..
	virtual	BvAABB	GetTerrainSize()=0;
	virtual BvAABB	GetPathFinderSize()=0;
#ifdef CLIENT_ONLY
	/// 
	BOOL GetTerrainSizeAABB( CWzdBase *pWzd, BvAABB* pBvSize, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);
	/// @name WZM 파일에서 생성
	virtual BOOL GenerateFrom( char *lpszFileName, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);
	virtual BOOL GenerateFromWZM(CWzdBase *pWzd, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL) = 0;
	/// @name 텍스쳐 등 읽기/제거
	virtual BOOL Load3DFiles( CDrawBase *pDraw, char *lpszPath);
	virtual BOOL Load3DFilesForTerrainMesh(CDrawBase *pDraw, char *lpszPath);
	/// @name 지형 경계 Blending
	virtual void SetTerrainEdgeBlending( int iMesh, BOOL bShow) = 0;				// 경계 Blending 켜고 끄기
	virtual void RefreshTerrainEdgeBlending() = 0;								// 경계 Blending 다시 계산
	virtual void SetAllTerrainEdgeBlending( BOOL bSet) = 0;							// 경계 Blending 모두 끄기 SetTerrainEdgeBlending 로 모두 끈것과 같음
	virtual void SetTerrainEdgeBlendingByTexture(int iTexture, BOOL bSet) = 0;		// 경계 Blending iTexture 관련 텍스쳐 끄기

	//_YJ_BLENDING_INFO_SERIALIZE
	virtual BOOL GetTerrainEdgeBlending(int idx) = 0;
	virtual int	 GetTerrainMeshCount()const = 0;
	//_YJ_BLENDING_INFO_SERIALIZE

	/// @name 특정 정보만 덮어씌우기
	virtual BOOL WrapVertexColorInfo( CWzArchive *pArchive, int iStyle = 0) = 0;	// Vertex Color 덮어씌우기 ( iStyle 이 0 이면 기본, 1 이면 Light 에 Color 적용 )

protected:
	virtual BOOL AnalyzeWzd( CWzdBase *pWzd, BvAABB* pBvSize );
	virtual void Destroy3DFiles( CDrawBase *pDraw) = 0;
#endif//CLIENT_ONLY

private:
	void SetWorldEnvParams(CDrawBase *pDraw);

	/// @name DLL 관련
protected:

	/// @name --- 중요한 기능을 분담해줄 class 들
public:
	COctree					*x_pOctree;			///< 옥트리
	COctree					*x_pTerrainOctree;	///< 지형 메쉬를 관리하기 위한 Octree
	CPathFinderBase			*x_pPathFinder;		///< 길찾기 class

	CRoomManager			*x_pRoomManager;	///< Room manager
	int						m_iRoomInfoCnt;		///< 네비 메쉬 변경시 타일 정보 복사를 위한
	WORD*					m_pwRoomInfo;		///< 네비 메쉬 변경시 타일 정보 복사를 위한

	CSkyBox					*x_pSkyBox;			///< Sky Box
	CTerrainSound			*x_pTerrainSound;	///< TerrainSound
	CTerrainSound2			*x_pTerrainSound2;	///< For the new TerrainSound object
	CLoadHelper				*x_pLoadHelper;
	CEnvObjectManager		*x_pEnvObjectManager;

#ifdef CLIENT_ONLY
	CManagerLightTerrain	*x_pManagerLight;
#endif

	/// @name --- 내부 정보
protected:
	DWORD m_dwCheckSum;
public:
	DWORD GetCheckSum()	{ return ( m_dwCheckSum); }

    static int NewHandler(size_t);
    static void PureCallHandler();
    static void BufferOverflowHandler(int code, void* unused);



	/// @name --- 설정 변경
protected:
	DWORD       m_dwState;	            ///< 현재 상태
    HANDLE      m_hWeatherObject;

	void SetState( DWORD dwFlag)	                 { m_dwState |= dwFlag; }
	void UnsetState( DWORD dwFlag)	                 { m_dwState &= ~dwFlag; }
	BOOL CheckState( DWORD dwFlag)	                 { return ( !!( m_dwState & dwFlag)); }
    
public:
#ifdef CLIENT_ONLY
    HANDLE  GetWeatherEffectHandle() const            { return m_hWeatherObject; }
    void    SetWeatherEffectHandle(const HANDLE handle) { m_hWeatherObject = handle; }

	virtual void SetGouraudForTerrainObject( BOOL bSet) = 0;

	/// @name --- 지형 메쉬 처리
public:
	virtual WzColor GetTerrainVertexLight( int iVertex) = 0;
	virtual void SetTerrainVertexLight( int iVertex, WzColor wcInput) = 0;
	virtual void SetAllTerrainVertexLight( WzColor wcInput) = 0;
	virtual WzColor GetTerrainVertexColor( int iVertex) = 0;
	virtual void SetTerrainVertexColor( int iVertex, WzColor wcInput) = 0;
	virtual void SetAllTerrainVertexColor(  WzColor wcInput) = 0;
	virtual void Calc_TerrainVertexInfo( int iVertex) = 0;
	virtual void Calc_TerrainVertexInfoWithLightmap( int iVertex) = 0;
	virtual WzVector GetTerrainVertexPos( int iVertex) = 0;
	virtual WzOctIndex GetTerrainMeshOctIndex( int iMesh) = 0;	///< TerrainMesh 정보 얻기

	virtual void SetBrightnessFunc( float *pfBrightnessFunc) = 0;	///< Brightness 계산 공식 float * 3
	virtual void GetBrightnessFunc( float *pfBrightnessFunc) = 0;	///< Brightness 계산 공식 float * 3
	virtual void EnableGoraudForTerrain( BOOL bEnable) = 0;	// 지형 render 시 gouraud 적용 여부 세팅
	virtual BOOL IsEnabledGoraudForTerrain()	{ return ( !CheckState( WS_DISABLE_GOURAUD_FOR_TERRAIN)); }

	virtual void EnableIndoorLight( BOOL bEnable );
	virtual BOOL IsEnabledIndoorLight()	{ return ( CheckState( WS_ENABLE_INDOOR_LIGHT)); }

#endif

	/// @name --- 지형 렌더링 관련/Picking
#ifdef CLIENT_ONLY
    virtual BOOL RenewalTerrainOcxtree() { return FALSE; }
	virtual int Enumerate_IntersectFrustum( ENUM_DATATYPE_OCTNODE dto, St_Camera *pCameraInfo, CWzArray<DWORD> *parResult) = 0;	///< Frustum 과 교차하는 것 얻기
	virtual int RenderTerrainMesh( CDrawBase *pDraw, 
                                   CWzArray<DWORD> *parOctNodes = NULL,
                                   ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL,
                                   bool bBlend = true,
                                   bool is_far_alpha = true) = 0;	///< 지형 출력 //is_far_alpha == 원경에 알파 페이드를 적용할건지 여부
    virtual int RenderTerrainMeshForDebug( CDrawBase *pDraw, 
                                    CWzArray<DWORD> *parOctNodes = NULL,
                                    CWzArray<DWORD> *parOcculedNode = NULL,
                                    ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL,
                                    bool bBlend = true,
                                    bool is_far_alpha = true) = 0;	///< 지형 출력 //is_far_alpha == 원경에 알파 페이드를 적용할건지 여부	

    // 프로스텀내의 지형노드 목록을 구한다
    virtual void Enumerate_OccluedTerrainNode(CDrawBase *pDraw, OUT CWzArray<DWORD>& TerrainNodes, OUT CWzArray<DWORD>& OcculedTerrainNodes ) {}
    virtual void RenderShadowMap( CDrawBase *pDraw); ///< 지형 그림자 그리기 호출
	virtual void RealizeRenderShadowMap( CDrawBase *pDraw, int iTextureToUse = 0) = 0; ///< 실제 지형 그림자 그리기
	virtual void RenderTerrainMeshInList( CDrawBase *pDraw,
                                          CWzArray<DWORD> *parMesh,
                                          bool bBlend, 
                                          bool is_far_alpha, //is_far_alpha == 원경에 알파 페이드를 적용할건지 여부
                                          ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL) = 0;	///< 지형 출력
	virtual void SetColorForDrawingMeshGroup( int iTextureGroup, WzColor wcColor) = 0;	///< 지형이 출력될때의 메쉬그룹별 색상 지정
	virtual void SetUVStreamForTerrainMesh( int iTextureGroup, float fU, float fV) = 0;	///< 지형 Mesh 에 UV stream 주기
	virtual void GetUVStreamForTerrainMesh( int iTextureGroup, float* pfU, float* pfV) = 0;	///< 지형 Mesh 에 UV stream 얻기
	virtual void SetTerrainMeshGroupOption( int iTextureGroup, DWORD dwState) = 0;	// 지형 메쉬의 매핑별 옵션 세팅	( TypeDefine.h 의 TMGO_ 참조 )
	virtual void UnsetTerrainMeshGroupOption( int iTextureGroup, DWORD dwState) = 0;	// 지형 메쉬의 매핑별 옵션 끄기
	virtual DWORD GetTerrainMeshGroupOption( int iTextureGroup) = 0;// 현재 지형 메쉬 매핑 옵션 얻기
	virtual void ReplaceTerrainMeshGroupOption( int iTextureGroup,DWORD dwState) = 0;// 세팅값을 dwState 로
	virtual DWORD GetTerrainMeshUserOption( int iTextureGroup)=0;
	virtual void ReplaceTerrainMeshUserOption( int iTextureGroup,DWORD dwState)=0 ;
	virtual void GetTerrainMeshGlow( int iTextureGroup, float *param, int count=3)=0; // 지형 텍스쳐의 Glow 파라미터값 가져오기
	virtual void ReplaceTerrainMeshGlow( int iTextureGroup, float *param, int count=3)=0; // 지형 텍스쳐의 Glow 파라미터값 설정
    virtual int terrain_drawcalls() = 0; //지형 렌더링 시의 DrawPrimitive() 호출 횟수
#endif
	virtual int PickTerrainMesh( WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE, DWORD dwTerrainGroupOptionCheck = 0, WzVector *pwvNormal = NULL) = 0;
	virtual BOOL CheckPickableTerrain(WzVector* pwvCollision)=0;// 직선매쉬 찾기 (잘못된 정보가 있는지 확인)
	/// @name --- 텍스쳐 정보 얻기
	virtual int GetTextureIndexOfTerrainMesh( int iMesh) = 0;
	virtual int GetTextureCount()=0;
	virtual const char* GetTextureFileNameByIndex(int iIndex)=0;
	virtual const char* GetSpecularTextureFileNameByIndex(int iIndex)=0;
	virtual const char* GetDetailTextureFileNameByIndex(int iIndex)=0;


#ifdef CLIENT_ONLY
	/// @name --- 스펙큘라 텍스쳐 정보 세팅 - 아마 에디팅에서만 사용
	virtual void SetSpecularTexture(int iMesh,char* szFileName)=0;
	virtual void UnSetSpecularTexture(int iMesh)=0;
	virtual void ReloadSpecularTexture(CDrawBase* pDraw,char* szRoot)=0;

	virtual const char *GetNormalTextureFileNameByIndex(int iIndex) = 0;
	virtual void SetNormalTexture(int iMesh,char *szFileName) = 0;
	virtual void UnSetNormalTexture(int iMesh) = 0;
	virtual void ReloadNormalmap(CDrawBase* pDraw, char* szRoot)=0;
	virtual const float GetHeightFactor(int iMesh) const = 0;
	virtual void SetHeightFactor(int iMesh, const float fHeightFactor) = 0;	
#endif

#ifdef CLIENT_ONLY
	/// @name --- 디테일맵 텍스쳐 정보 세팅 - 아마 에디팅에서만 사용
	virtual void SetDetailTexture(int iMesh,char* szFileName)=0;
	virtual void UnSetDetailTexture(int iMesh)=0;
	virtual void ReloadDetailTexture(CDrawBase* pDraw,char* szRoot)=0;
	virtual void SetDetailTextureScale(int iMesh,float fScale) = 0;
	virtual float GetDetailTextureScale(int iMesh) = 0;
	virtual void SetDetailFunc(int iMesh,float* pfunc)=0;
	virtual float* GetDetailFunc(int iMesh)=0;
#endif

#ifdef CLIENT_ONLY
	/// @name --- Decal 처리
	virtual void CreateDecal( WzSimpleMesh **ppwsm, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fHeight) = 0;
	virtual void DestroyDecal( WzSimpleMesh *pwsm) = 0;
	virtual void GetPosVertexListInDecal( int *piResultVertex, int *piNumResult, int iMaxVertex, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness) = 0;
	virtual void GetVertexInfoListInDecal( int *piResultVertex, int *piNumResult, int iMaxVertex, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness) = 0;
	virtual void GetMeshListInDecal( int *piResultMesh, int *piNumResult, int iMaxMesh, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness) = 0;
#endif


	/// @name --- 빛 처리
protected:
	WzVector m_wvSunLightDir;	///< 빛의 방향
	WzColor m_wcSunLightColor;	///< 빛의 색깔
	WzColor m_wcMapAmbient;	///< 맵의 주변광
	WzColor	m_wcAmbientDrawBase;		///< DrawBase 주변광
	WzColor	m_wcShadowColor;		///< 그림자 색깔
public:
	virtual void SetSunLightDir( const WzVector& wvSunLightDir, BOOL bUpdate = TRUE);
	virtual const WzVector& GetSunLightDir() const		{ return ( m_wvSunLightDir); }
	virtual void SetSunLightColor( WzColor wcSunLightColor);
	virtual WzColor GetSunLightColor()		{ return ( m_wcSunLightColor); }
	virtual void SetMapAmbientColor( WzColor wcMapAmbient)	{ m_wcMapAmbient = wcMapAmbient; }
	virtual WzColor GetMapAmbientColor()	{ return ( m_wcMapAmbient); }
	virtual WzColor GetAmbientColor(void) { return m_wcAmbientDrawBase;}
	virtual WzColor GetShadowColor(void) { return m_wcShadowColor;}
#ifdef CLIENT_ONLY
	virtual WzColor GetTerrainLight( const WzVector& wvPos, int iMesh = -1) = 0;	///< 지형에서 빛 얻기
	virtual void SetAmbientColor(CDrawBase *pDraw,WzColor wc) ;
	virtual void SetShodowColor(CDrawBase *pDraw,WzColor wv) ;
#endif


	/// @name --- 길찾기 관련

public:
	virtual WzOctIndex				GetTileOctIndex( int iTile) = 0;	///< Tile 정보 얻기
	virtual int						GetTileForTerrainMesh( int iTerrainMesh) = 0;	///< 메쉬와 연결된 타일 찾기
	virtual BOOL					PathExp_Reset( CPathExplorer *ppe, WzVector wvPos, int iTile = -1) = 0;	///< 3D 좌표를 줘서 초기화
	virtual BOOL					PathExp_Reset( CPathExplorer *ppe, WzVector *pwvPos, int iTile = -1) = 0;	///< 3D 좌표를 줘서 초기화 (포인터 형)
	virtual ENUM_FINDPATH_RESULT	PathExp_FindPath( CPathExplorer *ppe, WzVector wvDest, int iDestTile, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND) = 0;	///< 길 찾기
	virtual void					PathExp_FindPath_Profiler( CPathExplorer *ppe, WzVector wvDest, int iDestTile, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND, DWORD* pdwTick1 = 0,DWORD* pdwTick2 = 0){};	/// 성능 테스트 하기 위한 함수.
	virtual BOOL					PathExp_SetPath( CPathExplorer *ppe, WzVector *pwvDest, int *pTileRoute, int iNumTileRoute) = 0;	///< 경로 지정
	virtual BOOL					PathExp_Progress( CPathExplorer *ppe, float fMove, WzPathFindEvent *pEvent = NULL, ENUM_CHECK_COLLISION eCC = ECC_NO) = 0;	///< 길 찾기 진행 (좌표 이동이 있었으면 TRUE)
	virtual BOOL					PathExp_Progress_Profiler( CPathExplorer *ppe, float fMove, WzPathFindEvent *pEvent = NULL, DWORD* pdwTick1 = 0,DWORD* pdwTick2 = 0) {return true;}	
	virtual int						PathExp_GetMeshToStand( WzVector wvPos, float *pfT, float fMeshTileDist = -1.0f, DWORD dwTerrainGroupOptionCheck = 0, WzVector *pwvNormal = NULL, float wvHeight = 1.0f) = 0;	///< 3D 좌표 상에 서 있을때 발 딛을 메쉬 얻기
	virtual ENUM_FINDPATH_RESULT	PathExp_Thrust( CPathExplorer *ppe, WzVector *pwvMoveDistance, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK)) = 0;	///< 특정 방향으로 특정 거리만큼 밀기	

//	virtual BOOL					LoadStaticCollision( const char* pszFileName) const ;
	virtual BOOL					SaveStaticCollision( const char* pszFileName) const ;
	/// @name --- Tile 렌더링 관련/Picking

public:
	virtual void RenderPathTile( CDrawBase *pDraw, WORD wAttribute = 0, int *piTiles = NULL, int iNumTile = 0,BOOL bViewCollision = FALSE) = 0;	///< 길찾기 지형 출력
    virtual int PickPathTile( WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE) = 0;	///< 길찾기 지형 pick
    virtual int PickPathTileForDebug( WzRay *pwr, WzVector* VertexArray, int MaxVertexArrayCount, int* count, COctNode** pNode, float *pfT, BOOL bCulling ) = 0;	///< 길찾기 지형 pick
	virtual WzVector GetRandomPosInTile( int iTile) = 0;	///< Tile 내 random 위치 얻기
	virtual BOOL CorrectHeightByTile( int iTile, WzVector *pwvPos) = 0;
	virtual void RenderConvex( CDrawBase *pDraw, int *piConvexes, int iNumConvex, WzColor wcColor) = 0;	///< 컨벡스 출력


#ifdef CLIENT_ONLY
	/// @name --- D3D 가속 ( Own Vertex Buffer)
public:
	virtual void UpdateOwnVertexBuffer( int *piTriangle = NULL, int iNumTriangle = 0) = 0;	// 가속 관련 버퍼 update 하기
#endif


	/// @name 특수 영역
public:
	virtual BOOL AddSpecialArea( WzSpecialArea *pSpecialArea) = 0;
	virtual BOOL RemoveSpecialArea( WzID wiIdentity) = 0;
	virtual BOOL RemoveSpecialAreaByIndex( int iIndex) = 0;
	virtual void RemoveAllSpecialArea() = 0;
	virtual int GetNumberOfSpecialArea() = 0;
	virtual WzSpecialArea* GetSpecialArea( WzID wiIdentity) = 0;
	virtual WzSpecialArea* GetSpecialAreaByIndex( int iIndex) = 0;


	/// @name 테스트용
public:
	virtual void DoIt();

#ifdef CLIENT_ONLY
	virtual BOOL ReloadPathInfo( CWzArchive *pArchive)=0;
	
	// MapEditor에서 길 속성 및 영역 정보 Import 기능 사용시 사용하는 함수들...
	virtual BOOL ReloadPathInfoFromMapFile(CWzArchive *pArchive) = 0;
	virtual BOOL ReadOnlyMapFileHeader( CWzArchive *pArchive) = 0;
	virtual BOOL ReloadPathInfo2_MapFile( CWzArchive *pArchive) = 0;
	virtual BOOL ReloadPathInfo2_WMOFileFrom162To166(CWzArchive *pArchive, bool bLoadAll) = 0;
	virtual BOOL ReloadPathInfo2_WMOFile( CWzArchive *pArchive, bool bLoadAll) = 0;

	//Bloom 관련
	St_PE_BLUR				m_Pe_Blur;
	// --- Fog관련{
protected:
	St_FogParameter			m_FogParameter;
	St_STreeAttribute		m_SpeedTreeAttribute;
	
public:
	virtual void				SetFogParameter(CDrawBase *pDraw,St_FogParameter fogNew);
	virtual St_FogParameter		GetFogParameter(){return m_FogParameter;}
	virtual void				SetBlurAttribute(CDrawBase *pDraw,St_PE_BLUR* pnewBlurAttr);
	virtual St_PE_BLUR			GetBlurAttribute(){return m_Pe_Blur;}

	virtual void				SetSpeedTreeAttribute(CDrawBase *pDraw,St_STreeAttribute* pAttribute);
	virtual St_STreeAttribute	GetSpeedTreeAttribute(){return m_SpeedTreeAttribute;}

	virtual void				SetBloomParameters(CDrawBase *pDraw, St_PE_BLUR bloom);
	virtual void				GetBloomParameters(St_PE_BLUR &bloom);
	// --- Fog관련}

#endif


#ifdef CLIENT_ONLY
	/// @name --- Water 관련 멤버
public:
	virtual void RenderWater( CDrawBase *pDraw);
	virtual void RealizeRenderWater( CDrawBase *pDraw, HANDLE hTexRefraction, BOOL bBump = TRUE ) = 0;
	virtual BOOL IsExistingWater() = 0;

	virtual void RenderGlow( CDrawBase *pDraw);
	virtual void RealizeRenderGlow( CDrawBase *pDraw ) = 0;
	virtual BOOL IsExistingGlow() = 0;
#endif

	/// @name --- RenderState 관련 {
protected:
	float m_fFOV;
	float m_fNear;
	float m_fFar;
    bool m_bDepthApply;
public:
    virtual	void SetDepthApplyOff(bool val) { m_bDepthApply = val; }
	void SetProjectParams(float fFOV, float fNear, float fFar) { m_fFOV=fFOV; m_fNear=fNear; m_fFar=fFar; }
	void GetProjectParams(float *pfFOV, float *pfNear, float *pfFar) { *pfFOV=m_fFOV; *pfNear=m_fNear; *pfFar=m_fFar; }
	/// @name --- RenderState 관련 }

public:
	virtual void OutputMemoryInfo();
    virtual void GetTileVertexInfo(WzVector* pvOut, const int iTile);

	/** @name the occlusion culling group
	 *
	 *	polygon을 활용한 occlusion culling 기능 코드
	 *	@{
	 */

public:
	/// Occlusion culling의 유효범위를 얻는다.
	virtual float		GetRange_Occlusion(void) = 0;
	/// Occlusion culling에 사용되는 occluder의 유효한 범위를 지정한다.
	virtual void		SetRange_Occlusion(const float range) = 0;
	/// 거리값에 따라 정렬된 SimpOccluder 목록.
	virtual StlVDistObjBuffer_SimpOcc &GetSortedSimpOcc() = 0;

	virtual WzOctIndex	AddOccluder(cBaseOcclusion *occluder, DWORD *data) = 0;
	virtual void		RemoveOcclusion(WzOctIndex woi, DWORD data) = 0;

	virtual WzOctIndex	UpdateOcclusion(WzOctIndex woi, DWORD data, WzVector &p1, WzVector &p2) = 0;
    virtual WzOctIndex  UpdateOcclusion(const WzOctIndex old_oct_index,
                                        const DWORD data, 
                                        const WzVector &p1, 
                                        const WzVector &p2, 
                                        const WzVector &p3, 
                                        const WzVector &p4) = 0;
	virtual WzOctIndex	UpdateOccBox(WzOctIndex woi, DWORD data, WzVector *point) = 0;
	virtual WzOctIndex	RenewalOcclusionToOctree(DWORD data) = 0;

#ifdef CLIENT_ONLY
	virtual void		RefineTerrainMeshNode(CDrawBase *pDraw, WzMatrix *pmtViewTransform, WzFrustum *pFrustum, WzVector *pEye) = 0;
	virtual void		RenderOcclusion( CDrawBase *pDraw) = 0;
	virtual void		RenderOcclusionByEnum( CDrawBase *pDraw) = 0;
    virtual CWzArray<DWORD>& GetOcclusionListByEnum() = 0;
#endif//CLIENT_ONLY

	virtual WzOctIndex	PickingOcclusion(WzRay &ray, DWORD *data) = 0;
	virtual WzOctIndex	PickingOcclusionByEnum(WzRay &ray, DWORD *data) = 0;
    //픽킹된 오클루젼을 max_count만큼 가져온다.
    virtual int         PickingOcclusionList(DWORD* OUT arry_picked_data,  
                                             const int max_count, 
                                             const WzRay& ray) = 0;
	virtual int			GetNumOcclusion( ) = 0;
	virtual void		GetOcclusion( int index, WzOctIndex *woi, DWORD *data) = 0;

	virtual BOOL		SerializeOcclusion( CWzArchive *pArchive) = 0;
	virtual BOOL		SerializeExtraAreaInfo( CWzArchive *pArchive, bool bSerializeAll) { return TRUE; }

	/**	@}	*/ // end of the occlusion culling group



	/**	@name the drawing group
	 *	
	 *	맵의 추가적인 정보들을 표시하기 위해서 사용되는 공통된 그리기 지원 코드
	 *	@{
	 */

#ifdef CLIENT_ONLY
	/// Octree의 특정 Node를 둘러싼 경계 상자를 그린다.
	virtual void		RenderBoundVolume( CDrawBase *pDraw, DWORD dwOption) = 0;
	/// 주어진 정점으로 상자를 그린다.
	virtual void		RenderBox( CDrawBase *pDraw, WzVector *pVertex, WzColor color=WzColor_RGB(255,255,255)) = 0;
	/// 주어진 AABB를 가지고 상자를 그린다.
	virtual void		RenderBox( CDrawBase *pDraw, BvAABB *pBV, WzColor color=WzColor_RGB(255,255,255)) = 0;
	/// 주어진 정점으로 평면을 그린다.
	virtual void		RenderPlane( CDrawBase *pDraw, WzVector *pVertex, WzColor color=WzColor_RGB(255,255,255)) = 0;
	/// 주어진 정점들의 위치에 점을 찍는다.
	virtual void		RenderDot( CDrawBase *pDraw, WzVector *pVertex, int vertices, WzColor color=WzColor_RGB(255,255,255), float fDotSize = 1.0f) = 0;
#endif//CLIENT_ONLY


	/**	@}	*/ // end of the drawing group


	/**	@name the lightmap group
	 *	
	 *	맵의 lightmap 정보에 대한 코드.
	 *	@{
	 */

#ifdef CLIENT_ONLY
	virtual void		AttachLightmap( CWzArray< St_TerrainLightMap* >* parLightmapInfo, CWzArray< HANDLE >* parTerrainLightmapHandle ) = 0;
	virtual void		DetachLightmap() = 0;
	virtual void		AttachUnitLightmap(St_UnitLightMap *parUnitLightMap, DWORD dwNumMeshGroup) = 0;
	virtual void		DetachUnitLightmap() = 0;
#endif//CLIENT_ONLY

	/// 라이트맵 파일명을 얻는다.
	virtual LPCSTR		GetLightmapFileName();
	/// 라이트맵 파일명을 설정한다. lpFilename이 NULL일 경우 파일명을 지운다.
	virtual void		SetLightmapFilename(LPCSTR lpFilename);
	/// 라이트 맵을 위한 파라미터를 얻는다.
	virtual float		GetBrightness();
	/// 라이트 맵을 위한 파라미터를 설정한다.
	virtual void		SetBrightness(float val);

protected:
	St_LightmapInfo		m_LightmapInfo; ///< 라이트맵 정보를 담는 구조체

	/**	@}	*/ // end of the lightmap group


	/**	@name environment object
	 *	
	 *	맵의 추가적인 정보들을 표시하기 위해서 사용되는 공통된 그리기 지원 코드
	 *	@{
	 */

public:
	/** MapEditor에서 환경 객체를 등록할 때 호출한다. */
	virtual WzOctIndex		AddNewEnvObject(WzID wId, WzBoundingVolume wbv, WzColor wcFog, float fFogNear, float fFogFar, float fFogRate, DWORD *pdwObject) = 0;
	/** MapEditor에서 환경 객체를 Map-Octree로부터 제거할 때 호출한다. */
	virtual bool			RemoveEnvObject(const WzOctIndex woi, const DWORD dwObject) = 0;
	virtual void			ClearAllEnvObject() = 0;
	virtual WzOctIndex		UpdateEnvObjectBoundingVolume(WzOctIndex woi, WzBoundingVolume wbv, DWORD dwObject) = 0;	
	virtual void			RenewalEnvObjectToOctree() = 0;
	/**	@}	*/ // end of environment object


public:
	/**	@Texcell force
	*	
	*	강제로 모든 텍셀을 0~1 의 좌표로 강제한다.
	*	@{
	*   텍셀을 강제 하기 때문에 모든 계산이 다시 들어가야됨. 성능 느림!
	*/
	virtual void ForceTexCellIdx(bool isForceBlender) = 0;
};


#endif // _3DTERRAIN_WORLDBASE_H_
