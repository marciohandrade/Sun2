//=======================================================================================================================
/** 멥 
	@author
			유재영 < y2jinc@webzen.co.kr >
	@since
			2005. 2. 1
	@remarks
		- 멥관련 리소스를 관리 / 렌더.
		- 오브젝트 컬링
*/


#ifndef MAP_H
#define MAP_H

#pragma once

#include "MapArea.h"
#include "MapObject.h"
#include "TriggerParser.h"
#include "TriggerManEx.h"

class TriggerInfoManager;

#define	MAX_FIELD_IN_MAP_NUM		3

typedef		stdext::hash_map<DWORD, APPEARANCEINFO>		MapObject_Appearance_Map;
typedef		MapObject_Appearance_Map::iterator			MapObject_Appearance_Map_Itr;
typedef		std::pair<DWORD, APPEARANCEINFO>			MapObject_Appearance_Map_Pair;

struct MAP_OBJECT_RENDER_INFO
{
	DWORD	dwObjectKey;
	float	fDistance;	
};

inline bool operator == (const MAP_OBJECT_RENDER_INFO& x, const MAP_OBJECT_RENDER_INFO& y)
{
	return ( x.fDistance == y.fDistance );
}

//  내림차순으로 정렬하기위해 부등호기호를 반대로 정의했습니다!

inline bool operator < (const MAP_OBJECT_RENDER_INFO& x, const MAP_OBJECT_RENDER_INFO& y)
{
	return ( x.fDistance > y.fDistance );
}

inline bool operator > (const MAP_OBJECT_RENDER_INFO& x, const MAP_OBJECT_RENDER_INFO& y)
{
	return ( x.fDistance < y.fDistance );
}


typedef struct _SpeedTreeInfo {
	_SpeedTreeInfo(){
		dwKey = 0;
		hHandle = INVALID_HANDLE_VALUE;
	}

	DWORD	dwKey;
	HANDLE	hHandle;
} SPEED_TREE_INFO;

typedef struct _CAMERA_EXTRA_INFO
{
	WzVector	WzPos;
	WzVector	WzTargetPos;
} CAMERA_EXTRA_INFO;

class CDrawBase;
class CWorldBase;
class MapUnitCacher;
class MapWzUnitDraw;
class Item;
class CArchive;
class CAreaTileHelper;

class Map
{
public:
	enum EN_GRASS_DRAW_ATIRRIBUTE
	{
		EGDA_WAVE = 0x01,
	};

	enum 
	{
		CHUNK_OBJECT		= 0x1781,
		CHUNK_GRASS			= 0x1783,
		CHUNK_CAMERA_INFO	= 0x1784,
	};

	Map(void);
	virtual ~Map(void);

public:
	CWorldBase * GetWorldBase(void) const
	{
		return m_pWorldBase;
	}

public:
	void Create(CDrawBase* pDraw, char* pszRootDir );

	void Destroy(void);	
	void Update( DWORD dwTick );
	float CalcDistance(WzBoundingVolume* pbvTarget, WzVector &wvFrom, float *pfDistToCenter, float fRange);
	void ProcessAmbientSound();
	void Render(DWORD dwTick);
	void RenderUnits(void);	
#ifdef MAP_EDIT_MODE
	void RenderMapObjectInfo(void);	
#endif

	void RenderUnitInfo(void);
	void RenderObjects(void);
	void RenderAlphaUnits(void);
	void RenderAlphaObjects(void);
	void RenderAreaInfo(void);
	void RenderSector();
	BOOL IsOccluded(WzMatrix *pmtViewTransform, const WzBoundingVolume *pBV);

	void ToggleShowAreaInfo(void);
	void CalculateVisibleUnit(void);
	void SetMeshDrawMode(ENUM_MESH_DRAW eMeshDrawMode);

	void ProcessMapObjects( DWORD dwTick );


	void InitializeNodeLists();
	void RemoveNodeLists();

	void CreateWorldBase();
	void DestroyWorldBase();

	//@ loading 관련..
	BOOL Load( MAPID MapID, FIELDID FieldID, BOOL bUseAutoCamera = FALSE );

	//@ Unload 관련..
	void UnLoad(void);

	void EnableBlur( BOOL bFlag );

	BOOL IsLoaded() {return m_bLoaded;};

	void ApplyOverrideEnvironmentSetting();
	void ApplyOptionSetting();

	

public:
	void UpdateVisibleObject();

public:
	//@ 싱글 버전을 위한 함수...
	BOOL GetHeroPositionOnlySingleVersion(WzVector * pWzVec);

	//@ 플레그 관련
public:
	
	inline BOOL IsUsingBlur(void) { return m_bUseBlur; }

	MapWzUnitDraw*  GetPickedMapObject(WzRay* pRay, int iID = -1);
	Character*      GetPickedCharacter(WzRay* pRay, DWORD dwPickingOption);	
	Item*           GetPickedItem(WzRay* pRay);
	MapObject*      GetPickedUnitMapObject(WzRay * pRay, int iID = -1);


    // MapInfo
    MAPID GetMapID() { return m_MapInfo.m_MapID; }
    FIELDID GetFieldID() { return m_MapInfo.m_iFieldID; } 
    const sMAPINFO* GetMapInfo() { return m_MapInfo.m_pMapInfo; }
    const sFIELDINFO* GetFieldInfo() { return m_MapInfo.m_pCurFieldInfo; }
    TCHAR* GetFieldName() { return m_MapInfo.m_szFieldName; }
	int	GetCurrentFieldID(void) const;
	int	GetCurrentWMOCode(void) const;

    int GetMapFileVersion() { return m_MapInfo.m_MapFileVersion; }

	DWORD GetCurrentTriggerID();

	void SetUseAutoCamera(BOOL bUse);
	BOOL GetUseAutoCamera() const;

	CAMERA_EXTRA_INFO* GetExtraCameraInfo()
	{
        if( m_bCreated == TRUE )
            return &m_CameraExtra;
        return NULL;
	}

	//@ 트리거 맵오브젝트 관련 함수
	// TODO 오브젝트로 따로 빠져나갈것이다.


	WzSpecialArea* GetMapArea(DWORD dwID );
	WzSpecialArea* GetMapAreaByIndex(DWORD dwIndex);
	int GetNumberOfSpecialArea();

	BOOL GetStartAreaPosition(WzVector * pwzPos);
	BOOL GetAreaPosition(WzVector * pwzPos, DWORD dwAreaID, BOOL RandomTilePos = TRUE);
	BOOL GetPlayerDirection(WzVector * pwzDir);
	//@ 맵 높이 얻어오기
	BOOL GetTerrainHeight(WzVector *vPos,WzVector *vResult);

    //------------------------------------------------------------------------------
    //! Returns:   int : 섹터 index
    //! Qualifier: 위치에 해당하는 섹터 리턴
    //! Parameter: WzVector * pwzVec : 위치
    //------------------------------------------------------------------------------
    //DWORD GetSectorIndex( WzVector * pwzVec );
    int GetSectorIndex(WzVector * pwzVec);
		    
public:
	void RenderCircleShadow( WzVector *wvPos );
	

	BOOL IntersectMapObject_Ray_Bv(WzRay *pRay,float fLen,float fIgnoreHeight,WzVector & vObjPos);
	BOOL IntersectPlayer_Ray_Bv(WzRay *pRay,float fLen);
	BOOL IntersectTree_Ray_Bv(WzRay *pRay,float fLen);

    BOOL IntersectTree( WzBoundingVolume* pBoundingVolume );
    BOOL IntersectMapObject( WzBoundingVolume* pBoundingVolume );


	// @ 오토 마우스 관련 멤버 Method
public:
	void		AutoCameraPlay( DWORD dwTime );
	WzVector *	GetAutoCameraPos() const;
	void		SetAutoCameraTargetObjectKey(DWORD dwObjectKey);
	DWORD		GetAutoCameraTargetObjectKey() const;

	int         GetTriggerGroupIndex();

	void		SetRenderFlag(BOOL bFlag);
	BOOL		IsRender();

	void        PlayAmbientSound();
	void        StopAmbientSound();
	void		SetAmbientVolume(float fVolume);
	BOOL		GetSpawnHeroDirVector(const WzVector & rPos, WzVector & OUT rOutVecDir,float fLimit);
	BOOL		GetSpawnMonsterDirVector(const WzVector & rPos, WzVector & OUT rOutVecDir,float fLimit);

    float       GetMapFarClip(){return m_fMapProjectionFar;}
    float       GetMapNearClip(){return m_fMapProjectionNear;}
	BOOL		GetMapObjectAppearinfo(DWORD dwKey,APPEARANCEINFO *pInfo);

	
	BOOL				LoadMapWAT(CAreaTileHelper * pAreaTileLoadHelper);
	char *              GetFullFileName();


public:
	BOOL IsField();
	BOOL IsExsistWAT();
	BOOL IsVill();
	BOOL IsMission();
	BOOL IsTutorialField();
	BOOL IsTutorialMap();
	BOOL IsEsodeMap();
    bool IsAirShipMap();

protected:
    // 맵 로딩 / 언로딩 관련 내부함수. 
	BOOL LoadBaseMapInfo(MAPID MapID, FIELDID FieldID);

    // Load MAP File
	BOOL LoadMapData(CWzArchive* pMapFileArchive);
    BOOL LoadAmbientSound(CWzArchive* pMapFileArchive);
    BOOL LoadOption(CWzArchive* pMapFileArchive);
    BOOL LoadOptionSerialize(CWzArchive* pMapFileArchive);
    BOOL LoadOldCameraExtraInfo(CWzArchive* pMapFileArchive);

    // Load WMO File
    BOOL LoadMapWMO(CWzArchive* pArchive);
    BOOL LoadMapObject(CWzArchive* pArchive);
    BOOL LoadGrass(CWzArchive* pArchive);
    BOOL LoadSpeedTree(CWzArchive* pArchive);
    BOOL LoadPointLight(CWzArchive* pArchive);
    BOOL LoadAutoCamera(CWzArchive* pArchive);
    BOOL _loadMapObjectUnit();
    BOOL _loadMapObjectUnitDraw(CWzArchive* pArchive);

    // WUT File
	BOOL PreLoadCurMapUnitResource();

    // ....
	BOOL LoadTrigger();
	BOOL LoadTriggerGroup();

	void CalculateSectorInfo();
	void UnLoadSpeedTree(void);

	void ApplyLightMap();
	void AttachLightMap(LPSTR lpFileName);	
	void DetachLightMap();	


public:
	BOOL    IsApplyAnotherLight();
	void    SetApplyAnotherLight(BOOL bFlag);
	WzColor GetCharacterAmbientColor();
	WzColor GetCharacterSunColor();
	void    SetCharacterAmbientColor(WzColor color);
	void    SetCharacterSunColor(WzColor color);
	float   GetTerrainColorRate();
	void    SetTerrainColorRate(float fRate);

 	HANDLE	GetTargetingEffect();
 	void	LoadTargetingEffect();
	void	UnLoadTargetingEffect();

	//크리스마스 눈사람관련 이벤트맵
	BOOL	IsSnowManEventMap();	

    WzVector GetGrassWave( const WzVector& wvPos, const WzVector& wvRotOriginal, DWORD dwCurrentTick, int iType);

protected:
    float m_fFieldTable[360];


protected:
	BOOL    m_bApplyAnotherLight;
	WzColor m_CharacterAmbientColor;
	WzColor m_CharacterSunColor;
	WzVector m_ChracterSunDirection;
	float   m_fTerrainColorRate;

public:
	TriggerInfoManager* GetTriggerInfoMan(){return m_pTriggerInfoMan;}
	void GetTriggerGroupName(TCHAR* szVal);

    BOOL GetCollisionBV(WzRay *pRay,float fDist,float *pfT);

	void			SetTriggerMan(TriggerManEx* pTman){m_pTriggerMan = pTman;}
	TriggerManEx*	GetTriggerMan(){return m_pTriggerMan;}

protected:
	TriggerInfoManager* m_pTriggerInfoMan;
	TriggerManEx*		m_pTriggerMan;

protected:
	// library 관련 멤버
	CDrawBase *				m_pDraw;
	CWorldBase *			m_pWorldBase;
	CLoadHelper *			m_pLoadHelper;
	
	//////////////////////////////////////////////////////////////////////////
		
	float                   m_fLightmapBrightness;
    //char					m_szWMOFileName[MAX_PATH];
	//TCHAR                   m_szFilePath[MAX_PATH];
	char					m_szRootDir[MAX_PATH];
	char					m_szMapObjectDir[MAX_PATH];

	MapUnitCacher*			m_pMapUnitCacher;

    struct sBaseMapInfo
    {
        MAPID				m_MapID;
        FIELDID				m_iFieldID;
        int                 m_iFieldIndex;

        const sMAPINFO*     m_pMapInfo;
        sFIELDINFO*         m_pCurFieldInfo;
        sWMOINFO*			m_pCurWMOInfo;
        sWUTINFO*			m_pCurWUTInfo;
        eZONE_RULESET		m_eRuleset;

        TCHAR               m_szTriggerGroupName[MAX_TRIGGER_GROUPNAME_LENGTH];
        TCHAR               m_szFieldName[INTERFACE_STRING_LENGTH];

        char				m_szFullFileName[MAX_PATH];
        char                m_szFilePath[MAX_PATH];

        char                m_szWMOFileName[MAX_PATH];
        int                 m_MapFileVersion;
    };
    sBaseMapInfo m_MapInfo;

	BOOL                    m_bExistWAT;

	// @ 스피드 트리 관련
	typedef std::map<DWORD, SPEED_TREE_INFO*>		SPEED_TREE_MAP;
	typedef SPEED_TREE_MAP::iterator				SPEED_TREE_ITR;
	typedef std::pair<DWORD, SPEED_TREE_INFO*>		SPEED_TREE_PAIR;

	vector<HANDLE>			m_RenderSpeedTreeVec;
	SPEED_TREE_MAP			m_SpeedTreeMap;
	
	DWORD					m_dwSpeedTreeIndex;

	// Frustum 관련 리스트
	CWzArray<DWORD>*		m_parPossibleVisibleUnitList;
	CWzArray<DWORD>*		m_parPossibleVisibleObjectList;	
	CWzArray<DWORD>*		m_parAlphaVisibleUnitList;
	CWzArray<DWORD>*		m_parAlphaVisibleObjectList;
	
	CWzArray<DWORD>*		m_parVisibleUnitList;
	CWzArray<DWORD>*		m_parVisibleObjectList;
	CWzArray<DWORD>*		m_parVisibleNodeList;	

	// Picking 관련 리스트
	CWzArray<DWORD>*		m_pIntersecBoundUnitArray;	
	CWzArray<DWORD>*		m_pIntersecBoundMapObjectArray;	

	vector<MAP_OBJECT_RENDER_INFO> m_vecRenderInfoList;
	vector<WzVector> m_vecDummyObjectPos;
	vector<WzVector> m_vecDummyMonsterPos;

    vector<DWORD>           m_vecCollisionObj;

	//////////////////////////////////////////////////////////////////////////
	// 나중에 멥안으로 들어갈 것 들
//	SkyBox					m_SkyBox[2];
	
	St_PE_BLUR				m_sBlur;
	BOOL					m_bUseBlur;
	//////////////////////////////////////////////////////////////////////////

	/// @name 원 형태 그림자
	HANDLE					s_hTextureCircleShadow;
	WzSimpleMesh *			m_pwsmCircleShadow;
	HANDLE					m_hTextureLight;

	HANDLE					m_hTargetEffect;
	
	CAMERA_EXTRA_INFO		m_CameraExtra;
	CAutoCameraPlay			*m_pAutoCameraPlay;
    DWORD                   m_dwCurrentTrigger;
	St_ChunkInfo			m_CurChunkInfo;

    BOOL                    m_bCreated;
	BOOL					m_bLoaded;
	BOOL					m_bUseAutoCamera;

	// @오토 카메라 관련 변수
	WzVector				m_wvAutoCameraFrom;
	WzVector				m_wvAutoCameraTo;
	WzVector				m_wvAutoCameraTargetPos;
	DWORD					m_dwAutoCameraTargetObjectKey;

	// @AreaInfo 렌더 관련
	BOOL                    m_bRenderAreaInfo;

	// 유닛 별도 라이트 관련
	WzColor                 m_wzCharacterLightColor;
	WzColor                 m_wzCharacterLightDirection;
	WzColor                 m_wzOriginalLightColor;
	WzColor                 m_wzOriginalLightDirection;

	BOOL					m_bRenderFlag;
	int                     m_iAmbientSoundHandle;

	// 서버에 SectorIndex보내기 관련

	DWORD                   m_dwSectorSize;
	int                     m_iShiftMinX;
	int                     m_iShiftMinY;
	DWORD                   m_dwSectorXNum;
	DWORD                   m_dwSectorYNum;
	DWORD                   m_dwTotalSectorNum;


	float                   m_fMapProjectionFar;
	float                   m_fMapProjectionNear;
	float                   m_fMapProjectionFOV;

	BOOL                    m_bEnableEAX;

	// 서버서 보내주는 Object 생성을 위한 AppearanceMap

	MapObject_Appearance_Map m_hashmapAppearance;

	//

	list<DWORD>             m_listMapObjectKey;

//////////////////////////////////////////////////////////////////////////
//MapObj Visible Range 관련.
	float m_fVisibleRange;
	float m_fAlphaRange;
	float m_fVisibleRate;
	BOOL m_bAlphaVisible;
public:
	BOOL GetAlphaVisible(){return m_bAlphaVisible;}
	void SetAlphaVisible(BOOL bVal){m_bAlphaVisible = bVal;}

	void SetVisibleRange(float fRange){m_fVisibleRange = fRange;}
	void SetAlphaRange(float fRange){m_fAlphaRange = fRange;}

	float GetVisibleRange(){return m_fVisibleRange;}
	float GetAlphaRange(){return m_fAlphaRange;}

//////////////////////////////////////////////////////////////////////////
//WeatherEffect관련.
protected:
	HANDLE m_hWeatherEft;
	WzID   m_wiWeatherEft;
	BOOL   m_bOriginWeather;
public:
	void SetRenderWeather(BOOL bWeather);
	BOOL GetRenderWeather();
	BOOL GetOriginWeather(){return m_bOriginWeather;}
//////////////////////////////////////////////////////////////////////////

public:
	DWORD GetCurMMCode();

	int m_iVisMonCnt;
	int m_iVisMapObjCnt;
	int m_iVisPlayerCnt;
	int m_iNPCCnt;
    int grass_cnt_;
    int rendering_terrain_cnt_;

protected:
	bool m_bRenderTile;
public:
	void SetbRenderTile(bool bVal){m_bRenderTile = bVal;}
	bool GetbRenderTile(){return m_bRenderTile;}


	DWORD m_dwMultiThreadTime;
	bool m_bMultiThreadWhenLoad;

public:
    bool m_bRenderWireTerrain;

private:
    DWORD checksum_wmo_;
public:
    DWORD checksum_wmo(){return checksum_wmo_;}
};

extern Map *	g_pMap;

#endif
// end of MAP_H define