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


// ��ó�� ����
class	CDrawBase;
class	CWzArchive;
class	CManagerLightTerrain;


/// @name ���� render ���

enum ENUM_RENDER_TERRAIN_MODE
{
	RTM_NORMAL = 0,		///< �Ϲ� ��� ( m_wcColorCalculated ����)
	RTM_EDGE_WITHALPHA,	///< Edge ��� - Alpha �� ���� ( m_byBlendedAlpha ����)
	RTM_LIGHT,			///< ���� ��� ( m_wcLightCalculated ����)
	RTM_COLOR,			///< �Է��� ���� ��� ( m_wcColorInput ����)
	RTM_ONLY_BLENDED,	///< Blending �� �޽��� ���
};


enum ENUM_TERRAIN_INFO_MODE
{
	TIM_ALL,				//. ��� ���� 
	TIM_ONLY_TERRAIN,		//. ���� Terrain ����
	TIM_ONLY_PATHFINDER,	//. ���� ��ã�� ����
};


/// World �� State

#define WS_DISABLE_GOURAUD_FOR_TERRAIN	( 0x00000001)	// ���� render �� �� ���⿡ ���� gouraud ���� �� ������ ���� (save �Ǵ� ��)
#define WS_GOURAUD_FOR_TERRAIN_OBJECT	( 0x00010000)	// ��� Object �� render �� gouraud �����Ұ����� ����
#define	WS_ENABLE_INDOOR_LIGHT	( 0x00000010)


/// ���� ����Ʈ �� ������ ��� ����ü
typedef struct St_LightmapInfo
{
	char	szFilename[_MAX_PATH];	///< ����Ʈ �� ������ ��� ���� ��
	float	fBrightness;			///< ����Ʈ �� ����� ����ϴ� �Ķ���Ͱ�
} LIGHTMAPINFO;


class CWorldBase  
{
public:
	CWorldBase();
	virtual ~CWorldBase();
	virtual void Clear();

	/// @name ����/����
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
	/// @name ��Ÿ..
	virtual	BvAABB	GetTerrainSize()=0;
	virtual BvAABB	GetPathFinderSize()=0;
#ifdef CLIENT_ONLY
	/// 
	BOOL GetTerrainSizeAABB( CWzdBase *pWzd, BvAABB* pBvSize, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);
	/// @name WZM ���Ͽ��� ����
	virtual BOOL GenerateFrom( char *lpszFileName, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL);
	virtual BOOL GenerateFromWZM(CWzdBase *pWzd, ENUM_TERRAIN_INFO_MODE eType = TIM_ALL) = 0;
	/// @name �ؽ��� �� �б�/����
	virtual BOOL Load3DFiles( CDrawBase *pDraw, char *lpszPath);
	virtual BOOL Load3DFilesForTerrainMesh(CDrawBase *pDraw, char *lpszPath);
	/// @name ���� ��� Blending
	virtual void SetTerrainEdgeBlending( int iMesh, BOOL bShow) = 0;				// ��� Blending �Ѱ� ����
	virtual void RefreshTerrainEdgeBlending() = 0;								// ��� Blending �ٽ� ���
	virtual void SetAllTerrainEdgeBlending( BOOL bSet) = 0;							// ��� Blending ��� ���� SetTerrainEdgeBlending �� ��� ���Ͱ� ����
	virtual void SetTerrainEdgeBlendingByTexture(int iTexture, BOOL bSet) = 0;		// ��� Blending iTexture ���� �ؽ��� ����

	//_YJ_BLENDING_INFO_SERIALIZE
	virtual BOOL GetTerrainEdgeBlending(int idx) = 0;
	virtual int	 GetTerrainMeshCount()const = 0;
	//_YJ_BLENDING_INFO_SERIALIZE

	/// @name Ư�� ������ ������
	virtual BOOL WrapVertexColorInfo( CWzArchive *pArchive, int iStyle = 0) = 0;	// Vertex Color ������ ( iStyle �� 0 �̸� �⺻, 1 �̸� Light �� Color ���� )

protected:
	virtual BOOL AnalyzeWzd( CWzdBase *pWzd, BvAABB* pBvSize );
	virtual void Destroy3DFiles( CDrawBase *pDraw) = 0;
#endif//CLIENT_ONLY

private:
	void SetWorldEnvParams(CDrawBase *pDraw);

	/// @name DLL ����
protected:

	/// @name --- �߿��� ����� �д����� class ��
public:
	COctree					*x_pOctree;			///< ��Ʈ��
	COctree					*x_pTerrainOctree;	///< ���� �޽��� �����ϱ� ���� Octree
	CPathFinderBase			*x_pPathFinder;		///< ��ã�� class

	CRoomManager			*x_pRoomManager;	///< Room manager
	int						m_iRoomInfoCnt;		///< �׺� �޽� ����� Ÿ�� ���� ���縦 ����
	WORD*					m_pwRoomInfo;		///< �׺� �޽� ����� Ÿ�� ���� ���縦 ����

	CSkyBox					*x_pSkyBox;			///< Sky Box
	CTerrainSound			*x_pTerrainSound;	///< TerrainSound
	CTerrainSound2			*x_pTerrainSound2;	///< For the new TerrainSound object
	CLoadHelper				*x_pLoadHelper;
	CEnvObjectManager		*x_pEnvObjectManager;

#ifdef CLIENT_ONLY
	CManagerLightTerrain	*x_pManagerLight;
#endif

	/// @name --- ���� ����
protected:
	DWORD m_dwCheckSum;
public:
	DWORD GetCheckSum()	{ return ( m_dwCheckSum); }

    static int NewHandler(size_t);
    static void PureCallHandler();
    static void BufferOverflowHandler(int code, void* unused);



	/// @name --- ���� ����
protected:
	DWORD       m_dwState;	            ///< ���� ����
    HANDLE      m_hWeatherObject;

	void SetState( DWORD dwFlag)	                 { m_dwState |= dwFlag; }
	void UnsetState( DWORD dwFlag)	                 { m_dwState &= ~dwFlag; }
	BOOL CheckState( DWORD dwFlag)	                 { return ( !!( m_dwState & dwFlag)); }
    
public:
#ifdef CLIENT_ONLY
    HANDLE  GetWeatherEffectHandle() const            { return m_hWeatherObject; }
    void    SetWeatherEffectHandle(const HANDLE handle) { m_hWeatherObject = handle; }

	virtual void SetGouraudForTerrainObject( BOOL bSet) = 0;

	/// @name --- ���� �޽� ó��
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
	virtual WzOctIndex GetTerrainMeshOctIndex( int iMesh) = 0;	///< TerrainMesh ���� ���

	virtual void SetBrightnessFunc( float *pfBrightnessFunc) = 0;	///< Brightness ��� ���� float * 3
	virtual void GetBrightnessFunc( float *pfBrightnessFunc) = 0;	///< Brightness ��� ���� float * 3
	virtual void EnableGoraudForTerrain( BOOL bEnable) = 0;	// ���� render �� gouraud ���� ���� ����
	virtual BOOL IsEnabledGoraudForTerrain()	{ return ( !CheckState( WS_DISABLE_GOURAUD_FOR_TERRAIN)); }

	virtual void EnableIndoorLight( BOOL bEnable );
	virtual BOOL IsEnabledIndoorLight()	{ return ( CheckState( WS_ENABLE_INDOOR_LIGHT)); }

#endif

	/// @name --- ���� ������ ����/Picking
#ifdef CLIENT_ONLY
    virtual BOOL RenewalTerrainOcxtree() { return FALSE; }
	virtual int Enumerate_IntersectFrustum( ENUM_DATATYPE_OCTNODE dto, St_Camera *pCameraInfo, CWzArray<DWORD> *parResult) = 0;	///< Frustum �� �����ϴ� �� ���
	virtual int RenderTerrainMesh( CDrawBase *pDraw, 
                                   CWzArray<DWORD> *parOctNodes = NULL,
                                   ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL,
                                   bool bBlend = true,
                                   bool is_far_alpha = true) = 0;	///< ���� ��� //is_far_alpha == ���濡 ���� ���̵带 �����Ұ��� ����
    virtual int RenderTerrainMeshForDebug( CDrawBase *pDraw, 
                                    CWzArray<DWORD> *parOctNodes = NULL,
                                    CWzArray<DWORD> *parOcculedNode = NULL,
                                    ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL,
                                    bool bBlend = true,
                                    bool is_far_alpha = true) = 0;	///< ���� ��� //is_far_alpha == ���濡 ���� ���̵带 �����Ұ��� ����	

    // ���ν��ҳ��� ������� ����� ���Ѵ�
    virtual void Enumerate_OccluedTerrainNode(CDrawBase *pDraw, OUT CWzArray<DWORD>& TerrainNodes, OUT CWzArray<DWORD>& OcculedTerrainNodes ) {}
    virtual void RenderShadowMap( CDrawBase *pDraw); ///< ���� �׸��� �׸��� ȣ��
	virtual void RealizeRenderShadowMap( CDrawBase *pDraw, int iTextureToUse = 0) = 0; ///< ���� ���� �׸��� �׸���
	virtual void RenderTerrainMeshInList( CDrawBase *pDraw,
                                          CWzArray<DWORD> *parMesh,
                                          bool bBlend, 
                                          bool is_far_alpha, //is_far_alpha == ���濡 ���� ���̵带 �����Ұ��� ����
                                          ENUM_RENDER_TERRAIN_MODE rtmMode = RTM_NORMAL) = 0;	///< ���� ���
	virtual void SetColorForDrawingMeshGroup( int iTextureGroup, WzColor wcColor) = 0;	///< ������ ��µɶ��� �޽��׷캰 ���� ����
	virtual void SetUVStreamForTerrainMesh( int iTextureGroup, float fU, float fV) = 0;	///< ���� Mesh �� UV stream �ֱ�
	virtual void GetUVStreamForTerrainMesh( int iTextureGroup, float* pfU, float* pfV) = 0;	///< ���� Mesh �� UV stream ���
	virtual void SetTerrainMeshGroupOption( int iTextureGroup, DWORD dwState) = 0;	// ���� �޽��� ���κ� �ɼ� ����	( TypeDefine.h �� TMGO_ ���� )
	virtual void UnsetTerrainMeshGroupOption( int iTextureGroup, DWORD dwState) = 0;	// ���� �޽��� ���κ� �ɼ� ����
	virtual DWORD GetTerrainMeshGroupOption( int iTextureGroup) = 0;// ���� ���� �޽� ���� �ɼ� ���
	virtual void ReplaceTerrainMeshGroupOption( int iTextureGroup,DWORD dwState) = 0;// ���ð��� dwState ��
	virtual DWORD GetTerrainMeshUserOption( int iTextureGroup)=0;
	virtual void ReplaceTerrainMeshUserOption( int iTextureGroup,DWORD dwState)=0 ;
	virtual void GetTerrainMeshGlow( int iTextureGroup, float *param, int count=3)=0; // ���� �ؽ����� Glow �Ķ���Ͱ� ��������
	virtual void ReplaceTerrainMeshGlow( int iTextureGroup, float *param, int count=3)=0; // ���� �ؽ����� Glow �Ķ���Ͱ� ����
    virtual int terrain_drawcalls() = 0; //���� ������ ���� DrawPrimitive() ȣ�� Ƚ��
#endif
	virtual int PickTerrainMesh( WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE, DWORD dwTerrainGroupOptionCheck = 0, WzVector *pwvNormal = NULL) = 0;
	virtual BOOL CheckPickableTerrain(WzVector* pwvCollision)=0;// �����Ž� ã�� (�߸��� ������ �ִ��� Ȯ��)
	/// @name --- �ؽ��� ���� ���
	virtual int GetTextureIndexOfTerrainMesh( int iMesh) = 0;
	virtual int GetTextureCount()=0;
	virtual const char* GetTextureFileNameByIndex(int iIndex)=0;
	virtual const char* GetSpecularTextureFileNameByIndex(int iIndex)=0;
	virtual const char* GetDetailTextureFileNameByIndex(int iIndex)=0;


#ifdef CLIENT_ONLY
	/// @name --- ����ŧ�� �ؽ��� ���� ���� - �Ƹ� �����ÿ����� ���
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
	/// @name --- �����ϸ� �ؽ��� ���� ���� - �Ƹ� �����ÿ����� ���
	virtual void SetDetailTexture(int iMesh,char* szFileName)=0;
	virtual void UnSetDetailTexture(int iMesh)=0;
	virtual void ReloadDetailTexture(CDrawBase* pDraw,char* szRoot)=0;
	virtual void SetDetailTextureScale(int iMesh,float fScale) = 0;
	virtual float GetDetailTextureScale(int iMesh) = 0;
	virtual void SetDetailFunc(int iMesh,float* pfunc)=0;
	virtual float* GetDetailFunc(int iMesh)=0;
#endif

#ifdef CLIENT_ONLY
	/// @name --- Decal ó��
	virtual void CreateDecal( WzSimpleMesh **ppwsm, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fHeight) = 0;
	virtual void DestroyDecal( WzSimpleMesh *pwsm) = 0;
	virtual void GetPosVertexListInDecal( int *piResultVertex, int *piNumResult, int iMaxVertex, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness) = 0;
	virtual void GetVertexInfoListInDecal( int *piResultVertex, int *piNumResult, int iMaxVertex, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness) = 0;
	virtual void GetMeshListInDecal( int *piResultMesh, int *piNumResult, int iMaxMesh, const WzVector& wvPos, const WzVector& wvNormal, const WzVector& wvUp, float fSize, float fThickness) = 0;
#endif


	/// @name --- �� ó��
protected:
	WzVector m_wvSunLightDir;	///< ���� ����
	WzColor m_wcSunLightColor;	///< ���� ����
	WzColor m_wcMapAmbient;	///< ���� �ֺ���
	WzColor	m_wcAmbientDrawBase;		///< DrawBase �ֺ���
	WzColor	m_wcShadowColor;		///< �׸��� ����
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
	virtual WzColor GetTerrainLight( const WzVector& wvPos, int iMesh = -1) = 0;	///< �������� �� ���
	virtual void SetAmbientColor(CDrawBase *pDraw,WzColor wc) ;
	virtual void SetShodowColor(CDrawBase *pDraw,WzColor wv) ;
#endif


	/// @name --- ��ã�� ����

public:
	virtual WzOctIndex				GetTileOctIndex( int iTile) = 0;	///< Tile ���� ���
	virtual int						GetTileForTerrainMesh( int iTerrainMesh) = 0;	///< �޽��� ����� Ÿ�� ã��
	virtual BOOL					PathExp_Reset( CPathExplorer *ppe, WzVector wvPos, int iTile = -1) = 0;	///< 3D ��ǥ�� �༭ �ʱ�ȭ
	virtual BOOL					PathExp_Reset( CPathExplorer *ppe, WzVector *pwvPos, int iTile = -1) = 0;	///< 3D ��ǥ�� �༭ �ʱ�ȭ (������ ��)
	virtual ENUM_FINDPATH_RESULT	PathExp_FindPath( CPathExplorer *ppe, WzVector wvDest, int iDestTile, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND) = 0;	///< �� ã��
	virtual void					PathExp_FindPath_Profiler( CPathExplorer *ppe, WzVector wvDest, int iDestTile, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK), int iSeekNode = MAX_NODE_FOR_PATHFIND, DWORD* pdwTick1 = 0,DWORD* pdwTick2 = 0){};	/// ���� �׽�Ʈ �ϱ� ���� �Լ�.
	virtual BOOL					PathExp_SetPath( CPathExplorer *ppe, WzVector *pwvDest, int *pTileRoute, int iNumTileRoute) = 0;	///< ��� ����
	virtual BOOL					PathExp_Progress( CPathExplorer *ppe, float fMove, WzPathFindEvent *pEvent = NULL, ENUM_CHECK_COLLISION eCC = ECC_NO) = 0;	///< �� ã�� ���� (��ǥ �̵��� �־����� TRUE)
	virtual BOOL					PathExp_Progress_Profiler( CPathExplorer *ppe, float fMove, WzPathFindEvent *pEvent = NULL, DWORD* pdwTick1 = 0,DWORD* pdwTick2 = 0) {return true;}	
	virtual int						PathExp_GetMeshToStand( WzVector wvPos, float *pfT, float fMeshTileDist = -1.0f, DWORD dwTerrainGroupOptionCheck = 0, WzVector *pwvNormal = NULL, float wvHeight = 1.0f) = 0;	///< 3D ��ǥ �� �� ������ �� ���� �޽� ���
	virtual ENUM_FINDPATH_RESULT	PathExp_Thrust( CPathExplorer *ppe, WzVector *pwvMoveDistance, WORD wAttribute = ( PTA_ONLY_JUMP | PTA_NO_WALK)) = 0;	///< Ư�� �������� Ư�� �Ÿ���ŭ �б�	

//	virtual BOOL					LoadStaticCollision( const char* pszFileName) const ;
	virtual BOOL					SaveStaticCollision( const char* pszFileName) const ;
	/// @name --- Tile ������ ����/Picking

public:
	virtual void RenderPathTile( CDrawBase *pDraw, WORD wAttribute = 0, int *piTiles = NULL, int iNumTile = 0,BOOL bViewCollision = FALSE) = 0;	///< ��ã�� ���� ���
    virtual int PickPathTile( WzRay *pwr, float *pfT = NULL, BOOL bCulling = TRUE) = 0;	///< ��ã�� ���� pick
    virtual int PickPathTileForDebug( WzRay *pwr, WzVector* VertexArray, int MaxVertexArrayCount, int* count, COctNode** pNode, float *pfT, BOOL bCulling ) = 0;	///< ��ã�� ���� pick
	virtual WzVector GetRandomPosInTile( int iTile) = 0;	///< Tile �� random ��ġ ���
	virtual BOOL CorrectHeightByTile( int iTile, WzVector *pwvPos) = 0;
	virtual void RenderConvex( CDrawBase *pDraw, int *piConvexes, int iNumConvex, WzColor wcColor) = 0;	///< ������ ���


#ifdef CLIENT_ONLY
	/// @name --- D3D ���� ( Own Vertex Buffer)
public:
	virtual void UpdateOwnVertexBuffer( int *piTriangle = NULL, int iNumTriangle = 0) = 0;	// ���� ���� ���� update �ϱ�
#endif


	/// @name Ư�� ����
public:
	virtual BOOL AddSpecialArea( WzSpecialArea *pSpecialArea) = 0;
	virtual BOOL RemoveSpecialArea( WzID wiIdentity) = 0;
	virtual BOOL RemoveSpecialAreaByIndex( int iIndex) = 0;
	virtual void RemoveAllSpecialArea() = 0;
	virtual int GetNumberOfSpecialArea() = 0;
	virtual WzSpecialArea* GetSpecialArea( WzID wiIdentity) = 0;
	virtual WzSpecialArea* GetSpecialAreaByIndex( int iIndex) = 0;


	/// @name �׽�Ʈ��
public:
	virtual void DoIt();

#ifdef CLIENT_ONLY
	virtual BOOL ReloadPathInfo( CWzArchive *pArchive)=0;
	
	// MapEditor���� �� �Ӽ� �� ���� ���� Import ��� ���� ����ϴ� �Լ���...
	virtual BOOL ReloadPathInfoFromMapFile(CWzArchive *pArchive) = 0;
	virtual BOOL ReadOnlyMapFileHeader( CWzArchive *pArchive) = 0;
	virtual BOOL ReloadPathInfo2_MapFile( CWzArchive *pArchive) = 0;
	virtual BOOL ReloadPathInfo2_WMOFileFrom162To166(CWzArchive *pArchive, bool bLoadAll) = 0;
	virtual BOOL ReloadPathInfo2_WMOFile( CWzArchive *pArchive, bool bLoadAll) = 0;

	//Bloom ����
	St_PE_BLUR				m_Pe_Blur;
	// --- Fog����{
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
	// --- Fog����}

#endif


#ifdef CLIENT_ONLY
	/// @name --- Water ���� ���
public:
	virtual void RenderWater( CDrawBase *pDraw);
	virtual void RealizeRenderWater( CDrawBase *pDraw, HANDLE hTexRefraction, BOOL bBump = TRUE ) = 0;
	virtual BOOL IsExistingWater() = 0;

	virtual void RenderGlow( CDrawBase *pDraw);
	virtual void RealizeRenderGlow( CDrawBase *pDraw ) = 0;
	virtual BOOL IsExistingGlow() = 0;
#endif

	/// @name --- RenderState ���� {
protected:
	float m_fFOV;
	float m_fNear;
	float m_fFar;
    bool m_bDepthApply;
public:
    virtual	void SetDepthApplyOff(bool val) { m_bDepthApply = val; }
	void SetProjectParams(float fFOV, float fNear, float fFar) { m_fFOV=fFOV; m_fNear=fNear; m_fFar=fFar; }
	void GetProjectParams(float *pfFOV, float *pfNear, float *pfFar) { *pfFOV=m_fFOV; *pfNear=m_fNear; *pfFar=m_fFar; }
	/// @name --- RenderState ���� }

public:
	virtual void OutputMemoryInfo();
    virtual void GetTileVertexInfo(WzVector* pvOut, const int iTile);

	/** @name the occlusion culling group
	 *
	 *	polygon�� Ȱ���� occlusion culling ��� �ڵ�
	 *	@{
	 */

public:
	/// Occlusion culling�� ��ȿ������ ��´�.
	virtual float		GetRange_Occlusion(void) = 0;
	/// Occlusion culling�� ���Ǵ� occluder�� ��ȿ�� ������ �����Ѵ�.
	virtual void		SetRange_Occlusion(const float range) = 0;
	/// �Ÿ����� ���� ���ĵ� SimpOccluder ���.
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
    //��ŷ�� ��Ŭ������ max_count��ŭ �����´�.
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
	 *	���� �߰����� �������� ǥ���ϱ� ���ؼ� ���Ǵ� ����� �׸��� ���� �ڵ�
	 *	@{
	 */

#ifdef CLIENT_ONLY
	/// Octree�� Ư�� Node�� �ѷ��� ��� ���ڸ� �׸���.
	virtual void		RenderBoundVolume( CDrawBase *pDraw, DWORD dwOption) = 0;
	/// �־��� �������� ���ڸ� �׸���.
	virtual void		RenderBox( CDrawBase *pDraw, WzVector *pVertex, WzColor color=WzColor_RGB(255,255,255)) = 0;
	/// �־��� AABB�� ������ ���ڸ� �׸���.
	virtual void		RenderBox( CDrawBase *pDraw, BvAABB *pBV, WzColor color=WzColor_RGB(255,255,255)) = 0;
	/// �־��� �������� ����� �׸���.
	virtual void		RenderPlane( CDrawBase *pDraw, WzVector *pVertex, WzColor color=WzColor_RGB(255,255,255)) = 0;
	/// �־��� �������� ��ġ�� ���� ��´�.
	virtual void		RenderDot( CDrawBase *pDraw, WzVector *pVertex, int vertices, WzColor color=WzColor_RGB(255,255,255), float fDotSize = 1.0f) = 0;
#endif//CLIENT_ONLY


	/**	@}	*/ // end of the drawing group


	/**	@name the lightmap group
	 *	
	 *	���� lightmap ������ ���� �ڵ�.
	 *	@{
	 */

#ifdef CLIENT_ONLY
	virtual void		AttachLightmap( CWzArray< St_TerrainLightMap* >* parLightmapInfo, CWzArray< HANDLE >* parTerrainLightmapHandle ) = 0;
	virtual void		DetachLightmap() = 0;
	virtual void		AttachUnitLightmap(St_UnitLightMap *parUnitLightMap, DWORD dwNumMeshGroup) = 0;
	virtual void		DetachUnitLightmap() = 0;
#endif//CLIENT_ONLY

	/// ����Ʈ�� ���ϸ��� ��´�.
	virtual LPCSTR		GetLightmapFileName();
	/// ����Ʈ�� ���ϸ��� �����Ѵ�. lpFilename�� NULL�� ��� ���ϸ��� �����.
	virtual void		SetLightmapFilename(LPCSTR lpFilename);
	/// ����Ʈ ���� ���� �Ķ���͸� ��´�.
	virtual float		GetBrightness();
	/// ����Ʈ ���� ���� �Ķ���͸� �����Ѵ�.
	virtual void		SetBrightness(float val);

protected:
	St_LightmapInfo		m_LightmapInfo; ///< ����Ʈ�� ������ ��� ����ü

	/**	@}	*/ // end of the lightmap group


	/**	@name environment object
	 *	
	 *	���� �߰����� �������� ǥ���ϱ� ���ؼ� ���Ǵ� ����� �׸��� ���� �ڵ�
	 *	@{
	 */

public:
	/** MapEditor���� ȯ�� ��ü�� ����� �� ȣ���Ѵ�. */
	virtual WzOctIndex		AddNewEnvObject(WzID wId, WzBoundingVolume wbv, WzColor wcFog, float fFogNear, float fFogFar, float fFogRate, DWORD *pdwObject) = 0;
	/** MapEditor���� ȯ�� ��ü�� Map-Octree�κ��� ������ �� ȣ���Ѵ�. */
	virtual bool			RemoveEnvObject(const WzOctIndex woi, const DWORD dwObject) = 0;
	virtual void			ClearAllEnvObject() = 0;
	virtual WzOctIndex		UpdateEnvObjectBoundingVolume(WzOctIndex woi, WzBoundingVolume wbv, DWORD dwObject) = 0;	
	virtual void			RenewalEnvObjectToOctree() = 0;
	/**	@}	*/ // end of environment object


public:
	/**	@Texcell force
	*	
	*	������ ��� �ؼ��� 0~1 �� ��ǥ�� �����Ѵ�.
	*	@{
	*   �ؼ��� ���� �ϱ� ������ ��� ����� �ٽ� ���ߵ�. ���� ����!
	*/
	virtual void ForceTexCellIdx(bool isForceBlender) = 0;
};


#endif // _3DTERRAIN_WORLDBASE_H_
