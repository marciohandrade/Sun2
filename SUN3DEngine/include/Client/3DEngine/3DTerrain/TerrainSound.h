// terrainsound.h

#ifndef _3DTERRAIN_TERRAINSOUND_H_
#define _3DTERRAIN_TERRAINSOUND_H_

//------------------------------------------------------------------------------
/**
    Project: WzMapEditor 

    Date:   ~ 2005.11.25
    Rework: 2005.12.5 ~
    Author: 방원영, Won ShinYeon (3D Engine Team)

    LastModify:
    @2006.05.18
    ; TCHAR형 데이터 변수를 모두 char로 통일함.

    Copyright 2004 WEBZEN All rights.
*/

//----------------------------------------------------------------------------------------
// 헤더 파일 참조 선언
//
#include "octree.h"
#include <programcommon/wzarchive.h>
#include <client/3dengine/common/boundingvolume.h>
#include <vector>
#include <map>

//----------------------------------------------------------------------------------------
// 전처리 선언문
//
class COctree;
class CWorldBase;

//----------------------------------------------------------------------------------------
// 매크로 상수값 정의
//
#define VALID_VERSION_FOR_SOUNDFILE			157
#define VERSION_SOUNDAREA					0x0001
#define VERSION_SOUNDDATA					0x0001

//#define CHUNK_ID_SOUND					0x1791 ///< 맵 파일에 저장되는 청크 식별자

//#define CHUNK_SOUNDAREA_HEADER			0x1811 ///< 사운드 파일에 저장되는 청크 식별자
//#define CHUNK_SOUNDAREA					0x1812
//#define CHUNK_SOUNDDATA_HEADER			0X1821
//#define CHUNK_SOUNDDATA					0X1822

#define MAX_SOUNDFILENAMELEN				128
#define MAX_ACTIVE_SOUND					2

#define DEFAULT_SOUNDFILENAME				"NOSOUNDFILE"
#define EXTENSION_SOUNDFILE					".sdt"



/*---------------------------------------------------------------------------------------------------

											CTerrainSound2

 ----------------------------------------------------------------------------------------------------*/


// 코딩의 편의를 위해 정의한 데이터 타입
typedef std::vector<WzID>					StlVWzIDList;
typedef std::vector<WzID>::iterator			StlVWzIDListIter;
typedef int									SOUND_INDEX;
typedef std::vector<SOUND_INDEX>			StlVSoundGroupList;
typedef std::vector<SOUND_INDEX>::iterator  StlVSoundGroupListIter;

/// 사운드 영역 모양 열거자
enum EN_SOUNDAREATYPE 
{
	SVT_AABB,
	SVT_SPHERE,
	SVT_LAST
};

/// 사운드 데이터 묶음이 적용될 영역에 대한 구조체
struct St_SoundVolume
{
	WzID			m_WzID;			  ///< WzID로 표현되는 식별자	
	WzOctIndex		m_OctIndex;		  ///< Octree상에 등록시 받는 인덱스값
	WORD			m_wIndex;		  ///< 배열상의 인덱스 값
	WORD			m_Reserved;	
	float			m_fDistance;	  ///< 사운드 경계컬링의 기준점과 경계간의 교차점까지의 거리

	WzBoundingVolume m_BoundVolume;	  ///< 사운드 볼륨을 위한 경계 정보

	StlVWzIDList	m_SoundDataList;  ///< 사운드 볼륨과 연관된 사운드 그룹 목록
};

typedef std::map< int, St_SoundVolume* > StlMSoundVolume;


/// 사운드 데이터의 속성을 나타내는 플래그(Sound Data Attribute)
#define	SDA_NONE     0x00
#define	SDA_AMBIENT	 0x01
#define	SDA_3DSOUND	 0x02
#define SDA_EAX		 0x04
#define	SDA_ALL		 (SDA_AMBIENT | SDA_3DSOUND | SDA_EAX)

/// @brief 사운드 파일의 정보를 담을 구조체
///
/// 이전의 환경 사운드와 정보 구성을 동일하게 사용한다.
struct St_SoundFile
{
	// 저장/로딩 되는 멤버
	int			m_iSoundIdx;					///< 사운드 식별 인덱스
	float		m_fMinDist;						///< 3D 사운드시 최소 유효 거리
	float		m_fMaxDist;						///< 3 D사운드시 최대 유효 거리

	// 실행 시에 사용하는 멤버
	int			m_iSoundHandle;					///< 사운드 재생시 리턴받아 저장하고 있어야하는 핸들값

	char		m_szName[MAX_SOUNDFILENAMELEN]; ///< 파일명 저장 공간
};

// 사운드 파일 정보 목록을 위한 데이터 타입 정의
typedef std::vector< St_SoundFile > StlVSoundList;

/// 환경 사운드 정보 구조체
struct St_AmbientSound2
{
	//------------------------------------
	// Methods
	//
	~St_AmbientSound2()
	{
		m_SoundList.clear();
	}
	
	//------------------------------------
	// Data
	//
	StlVSoundList m_SoundList;
};

struct St_3DSound
{
	StlVSoundList m_SoundList;

	~St_3DSound()
	{
		m_SoundList.clear();
	}
};

struct St_EAX
{
	unsigned long	m_ulID;			/// sound environment ID
	float			m_fVolume;
	float			m_fDamping;
	float			m_fDecay;
};

struct St_Link
{
	WzID			m_idArea;		///< 사운드 영역 ID
	WzID			m_idSndData;	///< 연결하고자 하는 사운드 데이터 ID
};


/// 재생할 사운드 정보를 묶는 구조체
struct St_SoundData
{
	//------------------------------------
	// Methods
	//

	St_SoundData()
		: m_iIndex(-1)
		, m_WzID(INVALID_WZID)
		, m_Flags(SDA_NONE)
		, m_pAmbient(NULL)
		, m_p3DSound(NULL)
		, m_pEAX(NULL)
	{}
	~St_SoundData()
	{
		if(m_pAmbient) delete m_pAmbient;
		if(m_p3DSound) delete m_p3DSound;
		if(m_pEAX)	   delete m_pEAX;
	}


	//------------------------------------
	// Data
	//

	int					m_iIndex;		///< 사운드 그룹의 인덱스
	WzID				m_WzID;			///< 사운드 데이터의 WzID
	BYTE				m_Flags;		///< 사운드 그룹이 가지고 있는 사운드 정보의 종류 플래그
	char				m_Reserved[3];	///< 4byte 정렬을 위해서

	St_AmbientSound2*	m_pAmbient;		///< 환경 사운드 정보 구조체에 대한 포인터
	St_3DSound*			m_p3DSound;		///< 3D 사운드에 대한 정보 구조체에 대한 포인터
	St_EAX*				m_pEAX;			///< EAX 사운드 정보 구조체에 대한 포인터
};

struct St_GlobalSoundData
{
	St_SoundFile		m_GlobalAmbient; ///< 전역 환경 사운드 파일 정보
};

// 전처리 선언.
struct St_LHFileHeader;

/// 지형의 사운드 설정 정보를 관리하는 클래스
/// 이전의 CTerrainSound는 더이상 사용하지 않는다.
///
/// @modify
//	 2005.12.14
//		- 명칭변경: SoundVolume -> SoundArea
//		- 명칭변경: SoundGroup  -> SoundData
/// @wranning
///
class CTerrainSound2
{
private:
	BOOL Serialize_SoundArea(CWzArchive *pArchive);
	BOOL Serialize_SoundData(CWzArchive *pArchive);

	int FindEmptySlot_SoundArea();
	int FindEmptySlot_SoundData();

	/// WzID를 이용하여 사운드 영역 인스턴스를 찾는다.
	St_SoundVolume* FindSoundArea(WzID id);
	float CalcDistance(WzBoundingVolume* pbvTarget, WzVector &wvFrom, float *pfDistToCenter);

	void RemoveAllSoundArea();
	void RemoveAllSoundData();

	BOOL LoadSoundFile(LPCSTR lpFilename, St_LHFileHeader &fhInfo);
	BOOL SaveSoundFile(LPCSTR lpFilename, St_LHFileHeader &fhInfo);

public:
	//--------------------------------------------
	// 외부로 공개되는 함수들
	//

	/// 사운드 관리자 초기화
	virtual void Initialize(CWorldBase* pWorld, COctree* pOctree);
	/// 사운드 관리자의 모든 인스턴스 목록을 비운다
	virtual void Clear();
	/// 파일에 내용을 저장하거나/읽어온다.
	/// @param bSkip : 사운드 파일을 로딩/세이브 하지 않고 건너띈다.
	virtual BOOL Serialize( CWzArchive *pArchive, St_LHFileHeader &fhInfo, bool bSkip=false);
	
	/// 프레임별 장면에 연관되어 활성화/비활성화될 사운드 목록을 만든다
	virtual void BuildSoundList(CDrawBase* pDraw, WzFrustum &wfFrustum, WzVector &wvFrom, float fRange=30.f);

	/// 사운드 볼륨을 추가한다.
	/// @param index: 리스트상에서의 인덱스값(출력)
	/// @return return a handle of sound area instance.
	virtual int AddSoundArea(WzID id, WzBoundingVolume& bv, int *handle);
	/// 사운드 데이터 인스턴스를 추가한다.
	virtual int AddSoundData(WzID id);

	virtual void DelSoundData(WzID id);

	/// index에 해당하는 사운드 영역 객체의 WzID를 변경한다.
	virtual void SetWzID_SoundArea(const int index, const WzID id);
	///
	virtual void SetWzID_SoundData(const int index, const WzID id);

	/// index에 해당하는 사운드 영역 객체를 수정한다.
	virtual void UpdateSoundArea(const int index, const WzBoundingVolume& bv);
	///
	virtual void UpdateSoundArea_SoundGroupList(const int index, StlVWzIDList& list);
	///
	virtual void UpdateSoundData_AmbientSoundList(const int index, StlVSoundList &list);
	virtual void UpdateSoundData_3DSoundList(const int index, StlVSoundList &list);
	virtual void UpdateSoundData_EAX(const int index, unsigned long ulID, float fVolume,  float fDamping, float fDecay);

	/// 현재 Frustum안에 있는 Octree 노드에 연결된 사운드 영역들에 대한 목록을 담은 배열을 리턴받는다.
	CWzArray<DWORD>			*GetSoundObjectList_InFrustum() { return m_parSoundObject; }
	/// 최근 활성화된 사운드 영역의 목록
	StlMSoundVolume			*Enum_ActiveSoundArea() { return &m_LastSndAreas; }
	/// Frustum으로부터 벗어나서 더이상 활성화 상태가 되지 않는 사운드 영역 목록
	StlMSoundVolume			*Enum_DeactiveSoundArea() { return &m_DeactiveSndAreas; }

	// 주어진 파일명으로 사운드 정보 파일명을 만든다.
	virtual void			SetFilename(LPCSTR pFilename);
	/// 주어진 파일명으로 사운드 정보 파일명을 만든다. (== SetFilename())
	virtual void			MakeFilename(LPCSTR pFilename);
	/// 전역 환경 사운드를 설정한다.
	virtual void			SetGlobalAmbientSound(int index, LPCSTR lpFilename);
	virtual void			GetGlobalAmbientSound(int &index, LPSTR lpFilename);

	virtual int				SoundAreaCount() { return m_arSoundArea.GetCount(); }
	virtual int				SoundDataCount() { return m_arSoundData.GetCount(); }
	virtual St_SoundVolume	*GetSoundArea(int index) { return m_arSoundArea.Get(index); }
	virtual St_SoundData	*GetSoundData(int index) { return m_arSoundData.Get(index); }
	virtual void			Get_AmbientSoundList(const int index, StlVSoundList **ppList);
	virtual void			Get_3DSoundList(const int index, StlVSoundList **ppList);
	virtual St_SoundData	*FindSoundData(WzID id);
	virtual St_EAX			*GetCurEAX() { return m_pCurEAX; }
	virtual BOOL			RemoveSoundArea(const int index);
	virtual BOOL			RemoveSoundData(const int index);
	virtual void			RenewalSoundArea(const int handle);

	// 사운드 데이터(SD)를 사운드 영역(SA)에 연결한다.
	virtual BOOL LinkSDToSV(WzID sd, WzID sa);
	virtual BOOL UnlinkSDFromSV(WzID sd, WzID sa);
	/// 주어진 파일명의 파일이 존재하는지 확인한다.
	/// @return true이면 파일이 존재한다.
	virtual bool FileExists(LPCSTR lpFilename);

	CTerrainSound2(void);
	virtual ~CTerrainSound2(void);


	//--------------------------------------------
	// Data
	//
private:
	COctree							*m_pOctree;					///< 맵의 Octree 객체에 대한 참조자
	CWorldBase						*m_pWorld;					///< World 객체에 대한 참조자
	
	StlMSoundVolume					m_LastSndAreas;				///< 최근 활성화된 사운드 영역 목록
	StlMSoundVolume					m_DeactiveSndAreas;			///< 비활성화될 사운드 영역 목록
	char							m_szFilename[_MAX_PATH];	///< 사운드 정보가 저장된 파일 이름

	CWzArray<DWORD>					*m_parSoundObject;

public:
	CWzArray<St_SoundVolume*>		m_arSoundArea;
	CWzArray<St_SoundData*>			m_arSoundData;
	St_EAX							*m_pCurEAX;
	St_GlobalSoundData				m_GlobalSound;
};






/*---------------------------------------------------------------------------------------------------

											CTerrainSound

 ----------------------------------------------------------------------------------------------------*/




//----------------------------------------------------------------------------------------
// 매크로 상수값 정의
//
#define MAX_TERRAIN_SOUND_EVENT_PARAM  (4)

enum ENTERRAIN_SOUNDMSG
{
	//일반 2D용 사운드
	eTSM_PLAYSOUND,

	eTSM_DEACTIVE_AS_GROUP,
	eTSM_ACTIVE_AS_GROUP,

	//루프 사운드.
	eTSM_LOOP_SOUND_PLAY,
	eTSM_LOOP_SOUND_STOP,
	eTSM_LOOP_SOUND_SET_VOLUME,// Fade out/in  으로 사용
};

struct ST_TerrainSoundEvent
{
	ENTERRAIN_SOUNDMSG		m_EnventType;
	void*					m_avParam[MAX_TERRAIN_SOUND_EVENT_PARAM];
	int						m_aiParam[MAX_TERRAIN_SOUND_EVENT_PARAM];
	float					m_afParam[MAX_TERRAIN_SOUND_EVENT_PARAM];
};


// 구버전 사운드 관리 객체. 단순히 하위 버전에 대한 호환성 차원에서
// 지원한다.
class CTerrainSound
{
public:
	CTerrainSound(void){}
	virtual ~CTerrainSound(void) {}
	
	virtual BOOL Serialize( CWzArchive *pArchive);
};

#endif // _3DTERRAIN_TERRAINSOUND_H_