// loadhelper.h

#ifndef _3DTERRAIN_LOADHELPER_H_
#define _3DTERRAIN_LOADHELPER_H_

//------------------------------------------------------------------------------
/**
    파일로 부터 맵상에 존재하는 여러가지 오브젝트들을 로드하기 위해 사용하는 
    클래스이다. 오브젝트/SpeedTree/Grass/LocalLight등의 로드를 돕는 목적일
    뿐이기 때문에 로딩이후에 이를 접근해서 사용하지 않도록 한다.

    @modify
    @2006.3.7 ~ 4.19 : Script를 이용한 오브젝트 로드 지원.
    @2006.4.24 : Sun/Wiki Studio의 맵 오브젝트 로딩 부분에 LoadHelper 사용.

    @author: 방원영, 원신연(3D Engine Team)

    Copyright 2004 WEBZEN All rights.
*/

#include <programcommon/wzarray.h>
#include <programcommon/wzarchive.h>
#include <programcommon/wzsingleton.h>

#include <client/3dengine/common/wzcolor.h>
#include <client/3dengine/common/wzuvvert.h>
#include <client/3dengine/common/boundingvolume.h>

// 전처리 선언문
#ifdef CLIENT_ONLY
class CDrawBase;
#endif

class CWorldBase;

/// 오브젝트 관련 정보를 담는 구조체.
struct St_LHObjectWZU
{
	char*		m_pszFileName;
	WzID		m_WzID;
	BYTE		m_Attr[4];							///< 0: WZU파일의 인스턴싱 사용유무 1: 화면에서 작게 보일 경우 랜더링 안함
													///< 2: 예약						3: 예약

	// 편집과 관련된 영역
	int			m_iID;								///< 로드될 WZM의 식별 인덱스 값.
};

struct St_LHObjectSpecialTexture
{
	char*		m_pszFileName;
	WzID		m_WzID;

	int			m_iID;								///< 로드될 WZM의 식별 인덱스 값.
};

struct St_LHAtdEffect
{
    WzID    m_wzID;         ///< 오브젝트에 붙어 있는 Loop Effect의 WzID
    short   m_iRotAngle[3]; ///< X,Y,Z 축에 대한 회전값(0 ~ 360도)
    float   m_fScale;       ///< Effect의 전체 Scale에 대한 스칼라값
};

struct St_LHObject
{
	WzID					m_WzID;
	DWORD					m_dwAttribute;
	WzVector				m_wvPos;
	WzVector				m_wvScale;
	WzVector				m_wvRot;
	WzBoundingVolume		m_wzBV;
	WzID					m_wiUser;				///< 유저가 입력한 WZID.
	DWORD					m_dwExtraAttibue[3];	///< ??? 이전 작업자가 해놓은 부분. 용도를 모르고 있다.
	DWORD					m_dwParam[4];			///< 맵툴에서 입력한 추가 정보를 위한 파라미터.
    St_LHAtdEffect          m_AtdEffect[4];
	int						m_iID;					///< 로드될 WZM의 식별 인덱스 값.
	WzColor					m_wcDraw;				//_YJ_EDIT_OBJ_COLOR
    int					    m_iCollision;			//_GS_OBJ_COLLISION
};


/// 풀 오브젝트 구조체
struct St_LHGrass
{
	WzVector		m_wvPos;
	WzVector		m_wvRot;
	WzVector		m_wvScale;
	BYTE			m_byAttribute;

	// 편집을 위한 데이터.
	int				m_iID;
};

/// 풀 오브젝트를 위한 WZU파일 정보 구조체
struct St_LHGrassWZU
{
	WzID			m_WzID;
	int 			m_nNumGrass;
	St_LHGrass*		m_pGrass;

	// 편집을 위한 데이터.
	int				m_iLastIndex;
};


/// 스피드 트리 구조체
struct St_LHSpeedTree
{
	WzID			m_WzID;
	DWORD			m_dwAttribute;
	WzVector		m_wvPos;
	WzVector		m_wvScale;
	WzColor			m_wcDraw;	//_YJ_EDIT_OBJ_COLOR

	// 편집을 위한 데이터.
	int				m_iID;
};

/// 스피드 트리 파일 정보 구조체
struct St_LHSpeedTreeFile
{
	char*		m_pszFileName;
	WzID		m_WzID;

	// 편집을 위한 데이터.
	int				m_iID;
};

/// 스피드 트리 헤더 정보 구조체
struct St_LHSpeedTreeHeader
{
	char		m_szCompositeTexture[256];
	char		m_szCompositeShadowTexture[256];
};


/// 라이트 정보 구조체
struct St_LHLight
{
	WzVector	m_wvPos;
	WzColor		m_wc;
	float		m_fRange;
	float		m_fIntensity;
	BYTE		m_byIrradianceRef;

	// 편집을 위한 데이터.
	int				m_iID;
};


/// Mesh에 대한 라이트맵 정보를 담기 위한 구조체
/// @author 장진만
typedef struct St_LHMeshGroupLM
{
	int			NumObj;					//오브젝트 번호
	char		WZU_Filename[MAX_PATH];	//WZU파일 이름
	int			NumMeshGroup;			//메쉬그룹 번호
	char		LM_Filename[MAX_PATH];	//라이트맵 파일 이름
	int			NumLMVertex;			//라이트맵 버텍스 개수
	int			*FaceTextureNum;		//해당 Face가 쓰는 라이트맵 텍스쳐 번호
	WzUVVert	*ListLMVertex;			//라이트맵 버텍스 리스트

} LMMeshGroup;

typedef struct St_LHTerrainLM
{
	char TerrainLMName[MAX_PATH];		//지형 LM 이름
} TerrainLM;

struct St_LHUnitPath
{
	WzVector wvStartPos;
	WzVector wvPath;
    BYTE     byWalkState;               // 0 걷기, 1 달리기
};
typedef CWzArray<St_LHUnitPath> LHPathList;

// Structure : St_LHUnit
//
// 버전 별 수정 사항:
//  1) 0x0002: 다중 패스 정보 추가.
//  2) 0x0003: 트리거 연동 유닛에 대한 정보 추가.
//  3) 0x0004: 랜덤 리젠 관련 정보 추가.
//  4) 0x0005: 각 패트롤 경로상에서 걷기/달리기 정보 추가
#define VERSION_UNITDATAFORMAT		(0x0005)
#define NUM_REGENUNITTYPE			(5)
struct St_LHUnit
{
	WORD						wVersion;

	// 저장되는 데이터 영역
	char						name[128];			// 이름	
	char						wzufilename[128];	// 유닛이 참조하는 WZU 파일명
	int							category;
	WzID						wID;
	
	float						radian;				// 유닛의 시선방향
	BYTE						unit_type;			// 배치 타입
	BYTE						ai_type;			// AI 종류
    /// 유닛의 리젠타입 (0:일반리젠, 1:랜덤리젠 2:지연생성타입 - 비트체크하여 사용, 현재 0,1 의 경우 사용하지 않음)
	BYTE						ucRegenType;
    /// 유닛의 키값의 갯수(1보다 큰경우 랜덤리젠타입 유닛으로 한다)
	BYTE						ucNumKey;
	bool						bRefTrigger;		// 유닛이 트리거 시스템에 연결되어있다.
	DWORD						regentime;			// 재생 시간
	float						regenProbability;	// 재생 확률
	
	LHPathList					PathList;			// 유닛이 위치할 수 있는 위치값들.
	int							iKeyList[NUM_REGENUNITTYPE]; // 랜덤 리젠 유닛
	float						fRegenRate[NUM_REGENUNITTYPE]; // 랜덤 확률
	DWORD						userdata[2];
};

#define	VERSION_UNITGROUPDATAFORMAT	(0x0001)
struct St_LHUnitGroup
{
	WzID						wID;
	WzID						leader;				// 리더 유닛의 ID
	DWORD						userdata[2];		// 사용유무 미정

	// 유닛 그룹에 속한 유닛들에 대한 포인터 목록.
	CWzArray<WzID>				unitlist;
};


typedef struct St_LHMapOption
{
	WzVector	vPosition;
	WzVector	vLookAt;
				
	// 공식 
	float		fBrightArg[3];          // Brightness Function에 대한 Arguments(현재 사용안함)
	WzColor		wcShadow;

	// Projection params...
	float		fFov;
	float		fNear;
	float		fFar;

	// Sight params...
	float		fVisibleRange;	        // 가시 범위
	float		fAlphaRange;	        // Alpha값이 적용되어 랜더링이 되는 범위
	float		fVisibleRate;	        // 대상의 불투명 비율

	// Render option
	float		fBrightness;	        // 지형 랜더링시 밝기 조절을 위한 값.

	// Params for bloom...
	int			iBlurCount;				// 블러 횟수
	float		fMinLuminance;			// 적용할 최소 희도 0.0~1.0
	float		fSelectHighLight;		// 선택된 색깔 HighLight량 
	WzColor		wcSelectColor;			// 적용할 색상
	float		fFinalColorBlend;		// 연산 마직막에 얼마만큼 적용 할껀지.
	float		fOriginalColorBlend;	// 원본이미지 적용 비율

    WzID        WeatherEffect;          // 날씨효과 이펙트의 WzID

	float		fDOF_Rate;
	float		fDOF_Range;
} LHMapOption;

typedef struct St_LHFileHeader
{
	char  cIdentify[4];
	WORD  wFirstVersion;
	WORD  wSecondVersion;
	DWORD dwCheckSum;

} LHFileHeader;

enum Enum_FileType
{
	FILETYPE_MAP,
	FILETYPE_MAPOBJECT,
	FILETYPE_MAPSOUND,
	FILETYPE_MAPUNIT,
	FILETYPE_MAPOPTION,
};


class CLoadHelper : public CWzSingleton<CLoadHelper>  
{

public:
	CLoadHelper();
	virtual ~CLoadHelper();
	/** 로드된 정보를 삭제하고 할당된 모든 메모리를 해제한다. */
	virtual void DeleteAll();

	/**	@name Object
	 *	
	 *	SpeedTree에 관련된 함수들.
	 *	@{
	 */

	virtual BOOL						LoadObj(CWzArchive* pArchive);
	virtual int							GetObjWzuCount();
	virtual St_LHObjectWZU				*GetObjWzu();
	virtual int							GetSpecialTextureCount();
	virtual St_LHObjectSpecialTexture*  GetSpecialTexture();
	virtual int							GetObjectCount();
	virtual St_LHObject					*GetObject();
	/** 오브젝트에 대한 정보를 삭제하고 메모리를 반환한다. */
	virtual void						DeleteObject();

	virtual int							GetLastIndexObjWzu() { return m_iLastIndexObjWZU; }
	virtual int							GetLastIndexSpecTex() { return m_iLastIndexSpecTex; }
	virtual int							GetLastIndexObj() { return m_iLastIndexObj; }

	/** 오브젝트 인스턴스들을 위한 메모리 공간을 할당한다 */
	virtual void						AllocateArrayForWzu(int size);
	virtual void						AllocateArrayForSpecailTexture(int size);
	virtual void						AllocateArrayForObject(int size);
	virtual void						ExpandArrayForObject(int size);
	/** 오브젝트를 로드한다. */
	virtual void						LoadWzu(int index, LPSTR lpFilename, WzID wID);	
	virtual void						LoadSpecailTexture(int index, LPSTR lpFilename, WzID wID);
	
	virtual int							ReallocateArrayForWzu(int size);

	/**
	 * 주어진 이름과 WzID의 WZU object가 있는지 찾는다.
	 * @return : 0이상인 경우 현재 목록에서 Wzu object의 인덱스이다.
	 */
	virtual int							FindObjectWzu(LPSTR lpFilename, WzID wID);
	/**
	 * 주어진 이름과 WzID의 SpecialTexture가 있는지 찾는다.
	 * @return : 0이상인 경우 현재 목록에서 SpecialTexture의 인덱스이다.
	 */
	virtual int							FindSpecailTexture(LPSTR lpFilename, WzID wID);

	/**	@}	*/ // end of Object


	/**	@name Grass
	 *	
	 *	Grass에 관련된 함수들.
	 *	@{
	 */

	/** Map파일의 Grass 정보를 로드한다. */
	virtual BOOL						LoadGrass(CWzArchive* pArchive);
	virtual int							GetGrassWZUCount();
	virtual int							GetGrassCount(int iWZU);
	virtual St_LHGrassWZU				*GetGrassWZU();
	virtual int							GetTotalGrassCount();
	/** Grass에 대한 정보를 삭제하고 메모리를 반환한다. */
	virtual void						DeleteGrass();

	virtual BOOL CreateGrassWzuList(int nGrassTotal, int nGrassWZU);
	virtual BOOL CreateGrassList(int iWZU, WzID wID, int nNumGrass, int iLastGrass);
	/**
	 * 해당 인덱스의 인스턴스에 대한 정보를 설정한다.
	 * @param index : 배열의 인덱스
	 * @param iWzu : Grass 객체가 사용하는 Wzu 파일의 인덱스값
	 * @param iID : 식별 인덱스
	 * @param vPos :
	 * @param sRot : 
	 * @param sScale :
	 * @param byAttrubte :
	 */
	virtual void SetGrass(int index, int iWZU, int iID, WzVector *vPos, short sRot, short sScale, BYTE byAttribute);
	/**
	 * 현재 로드된 Grass 목록의 해당 인스턴스에 대한 정보를 수정한다.
	 * @param index : 배열의 인덱스
	 * @param iWzu : Grass 객체가 사용하는 Wzu 파일의 인덱스값
	 * @param vPos :
	 * @param sRot : 
	 * @param sScale :
	 * @param byAttrubte :
	 */
	virtual void ModifyGrass(int index, int iWZU, WzVector *vPos, short sRot, short sScale, BYTE byAttribute);
	virtual int FindGrassWzu(WzID wID);
	virtual int FindGrass(int iGrassWZU, int iID, WzVector *vPos, WzVector *vRot, WzVector *vScale);
	/** 주어진 크기로 Grass에 사용되는 Wzu 목록에 대한 메모리를 재할당한다. */
	virtual int ReallocGrassWzu(int size);
	/** 주어진 크기로 Grass에 대한 메모리를 재할당한다. */
	virtual int ReallocGrass(int iGrassWZU, int size);
	
	/**	@}	*/ // end of Grass


	/**	@name SpeedTree
	 *	
	 *	SpeedTree에 관련된 함수들.
	 *	@{
	 */

	/** Map파일로부터 SpeedTree 정보를 읽는다. */
	virtual BOOL LoadSpeedTree(CWzArchive* pArchive);
	virtual St_LHSpeedTreeHeader* GetSpeedTreeHeader();
	virtual int GetSpeedTreeCount();
	virtual St_LHSpeedTree* GetSpeedTree();
	virtual int GetSpeedTreeFileCount();	
	virtual St_LHSpeedTreeFile* GetSpeedTreeFile();
	/** SpeedTree에 대한 정보를 삭제하고 메모리를 반환한다. */
	virtual void DeleteSpeedTree();

	virtual BOOL CreateSpeedTreeHeader(LPSTR lpCompositeTexture, LPSTR lpCompositeShadowTexture);
	virtual BOOL CreateSpeedTreeFileList(int nSpeedTreeFile, int iLastSpeedTreeFile);
	virtual  void SetSpeedTreeFile(int index, int iID, LPSTR filename, WzID wID);
	virtual BOOL CreateSpeedTreeList(int nSpeedTree, int iLastSpeedTree);
	virtual void SetSpeedTree(int index, int iID, WzID wID, DWORD dwAttr, WzVector *vPos, WzVector *vScale);
	virtual void ModifySpeedTree(int index, DWORD dwAttr, WzVector *vPos, WzVector *vScale);

	virtual int	GetLastIndexSpeedTreeFile() { return m_iLastSpeedTreeFile; }
	virtual int	GetLastIndexSpeedTree() { return m_iLastSpeedTree; }

	bool FindIndexSpeedTreeFile(int iID);
	/** 주어진 정보를 가진 트리 파일이 있는지 확인한다. */
	virtual bool FindSpeedTreeFile(LPSTR filename, WzID wID);
	bool FindIndexSpeedTree(int iID);
	/** 주어진 정보를 가진 트리가 있는지 확인한다. */
	virtual bool FindSpeedTree(WzID wID, WzVector *vPos, WzVector *vScale);
	/** 스피드 트리 파일 목록을 재할당한다. */
	virtual int ReallocSpeedTreeFileList(int size);
	/** 스피드 트리 목록을 재할당한다. */
	virtual int ReallocSpeedTreeList(int size);

	/**	@}	*/ // end of SpeedTree


	/**	@name Light
	 *	
	 *	라이트에 관련된 함수들.
	 *	@{
	 */

	/** Map파일로부터 Light 정보를 읽는다. */
	virtual BOOL LoadLight(CWzArchive* pArchive);
	virtual int  GetLightCount();
	virtual St_LHLight* GetLight();
	/** Light정보를 삭제하고 메모리를 해제한다. */
	virtual void DeleteLight();

	virtual bool CreateLocalLightList(int nLight);
	/** 
	 * 지역광의 배열에 지역광에 대한 정보를 설정한다. 
	 * @param index : 배열의 인덱스
	 * @param iID : 식별 인덱스
	 * @param wc : 색상값
	 * @param vPos : 위치값
	 * @param fRange : 
	 * @paranm byIrradianceRef : 
	 * @param fIntensity :
	 */
	virtual void SetLocalLight(int index, int iID, WzColor wc, WzVector *vPos, float fRange, BYTE byIrradianceRef, float fIntensity);
	/** 
	 * 해당 지역광의 정보를 수정한다. 
	 * @param index : 대상 지역광의 배열 인덱스
	 * @param wc : 색상값
	 * @param vPos : 위치값
	 * @param fRange : 
	 * @paranm byIrradianceRef : 
	 * @param fIntensity :
	 */
	virtual void ModifyLocalLight(int index, WzColor wc, WzVector *vPos, float fRange, BYTE byIrradianceRef, float fIntensity);
	virtual int GetLastLight() { return m_iLastLight; }

	virtual bool FindLocalLight(const int iID, WzVector *vPos, const float fRange);
	/** 주어진 크기로 지역광에 대한 메모리를 재할당한다. */
	virtual int ReallocLocalLight(int size);

	/**	@}	*/ // end of Light


	/**	@name Light
	 *	
	 *	라이트에 관련된 함수들.
	 *	@{
	 */

	/** 
	 * WLU 파일 정보를 읽고 Mesh의 LIghtMap정보들에 대한 목록을 생성한다.
	 *
	 * @param Filename : Lightmap 정보 파일이름(*.wlu) - I
	 * @param ppLMUV : 메쉬의 라이트맵 정보를 담은 버퍼에 대한 포인터를 얻는다. - O
	 * @param NumLMMeshGroup : 총 메쉬 그룹의 갯수 - O
	 * @param pTLM : 지형의 라이트맵 정보를 담은 버퍼에 대한 포인터를 얻는다. - O
	 * @param NumTLM : 지형 라이트맵 갯수 - O
	 */
	#ifdef CLIENT_ONLY
	virtual BOOL ParseWLU(char *FileName, St_LHMeshGroupLM **ppLMUV, DWORD &NumLMMeshGroup, TerrainLM** ppTLM, DWORD &NumTLM, CDrawBase *pDrawBase );
	#endif
	virtual void ReleaseTerrainWLU();
	virtual void ReleaseUnitWLU();

	virtual St_LHMeshGroupLM *GetMeshGroupLM() { return	m_LMUV;	}
	virtual DWORD GetNumMeshGroupLM() { return m_NumLMMeshGroup; }

	/**	@}	*/ // end of Light


	/**	@name unit
	 *	
	 *	유닛 정보를 읽는다.
	 *	@{
	 */
	
private:
	void ReadUnitData(CWzArchive *pArchive, St_LHUnit *pUnit);
	void ReadUnitGroupData(CWzArchive *pArchive, St_LHUnitGroup *pUnitGroup);

	BOOL ReadMapSubFileHeader(Enum_FileType type, CWzArchive *pArchive, St_LHFileHeader &fhInfo);

public:
	/** 유닛 정보를 읽는다. */
	virtual BOOL LoadUnit(CWzArchive *pArchive, St_LHFileHeader &fhInfo);
	/** 유닛 정보를 삭제한다. */
	virtual void DestroyUnit();

	CWzArray< St_LHUnit* >			*GetUnits() { return &m_Units; }
	CWzArray< St_LHUnitGroup* >		*GetUnitGroups() { return &m_UnitGroups; }

	/** @} */ // end of unit


	/**	@name MapOption
	 *	
	 *	맵파일의 옵션 부분에 해당하는 정보를 읽는다.
	 *	@{
	 */

	virtual BOOL LoadMapOption(CWzArchive *pArchive, St_LHFileHeader &fhInfo);
	
	virtual St_LHMapOption *GetMapOption();

	/**	@}	*/ // end of MapOption

	virtual BOOL LoadMapObjectInfoFromMem(CWorldBase *pWorld, CWzArchive *pArchive);

	int		GetReference() { return m_iRef; }
	void	AddReference() { ++m_iRef; }
	void	SubReference() { --m_iRef; }

	//------------------------------------------------------
	// Data.
	//
private:
	static int						m_iRef;

protected:
	St_LHObjectWZU*					m_pObjectWZU;					// 오브젝트를 위한 WZU 파일 목록
	St_LHObjectSpecialTexture*		m_pObjectSpecialTexture;		// 오브젝트 텍스쳐에 대한 목록
	St_LHObject*					m_pObject;						// 오브젝트 파일에 대한 목록
	St_LHGrassWZU*					m_pGrassWZU;					// 풀을 위한 WZU 파일 목록
	St_LHSpeedTreeHeader*			m_pSpeedTreeHeader;				// SpeedTree에 대한 헤더 정보
	St_LHSpeedTree*					m_pSpeedTree;					// SpeedTree 파일 목록
	St_LHSpeedTreeFile*				m_pSpeedTreeFile;				// SpeedTree 파일 목록
	St_LHLight*						m_pLight;						// 라이트 정보 목록

	St_LHTerrainLM*					m_TLM;							// 지형 라이트 맵 정보 목록
	St_LHMeshGroupLM*				m_LMUV;							// 메쉬의 라이트맵 정보

	CWzArray< St_LHUnit* >			m_Units;						// 유닛의 목록에 대한 배열
	CWzArray< St_LHUnitGroup* >		m_UnitGroups;					// 유닛 그룹에 대한 배열

	St_LHMapOption					m_MapOption;					// 맵 옵션 정보 구조체

	int								m_nObjectWZU;					// WZU 파일 갯수
	int								m_nObjectSpecialTexture;		// 오브젝트를 위한 텍스쳐 갯수
	int								m_nObject;						// 오브젝트 갯수
	int								m_nGrassWZU;					// 풀에 대한 WZU 파일 갯수
	int								m_nGrassTotal;					// 풀의 총 인스턴스 갯수
	int								m_nSpeedTree;					// SpeedTree 갯수
	int								m_nSpeedTreeFile;				// SpeedTree 파일 갯수
	int								m_nLight;						// 라이트 갯수
	DWORD							m_NumLMMeshGroup;				// 메쉬 그룹에 대한 라이트 맵정보 갯수.

	int								m_iLastIndexObjWZU;				// WZU에 할당된 마지막 인덱스
	int								m_iLastIndexSpecTex;			// 텍스쳐에 할당된 마지막 인덱스
	int								m_iLastIndexObj;				// 오브젝트에 할당된 마지막 인덱스
	int								m_iLastSpeedTreeFile;			// 스피드 트리 파일에 할당된 마지막 인덱스
	int								m_iLastSpeedTree;				// 스피드 트리에 할당된 마지막 인덱스
	int								m_iLastLight;					// 라이트에 대한 마지막 인덱스
};

// 매크로
#define WZMAPLOADER					CLoadHelper::GetSingleton()

#endif // _3DTERRAIN_LOADHELPER_H_