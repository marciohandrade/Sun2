// envobject.h

#ifndef _3DTERRAIN_ENVOBJECT_H_
#define _3DTERRAIN_ENVOBJECT_H_

//------------------------------------------------------------------------------
/**
    지형에 대한 특정 구역에 대한 환경 정보를 관리하는 클래스를 정의한다.
    프로그램 작성방식은 기존의 Terrain에 대한 부속 Manager 형식을 따른다.

    부연설명:
    환경 오브젝트란? 
    지형에 특정 구역에 대하여 포그값등 Local적인 변화를 주기 위한 객체이다.

    Date:   2006.7.27
    Rework: 
    Author: Won ShinYeon (3D Engine Team)

    LastModify:

    Copyright 2004 WEBZEN All rights.
*/

#include "distanceobject.h"
#include <vector>

#define OLDEST_MAPVERSION_ABOUT_ENVOBJECT	169 ///< 환경 객체를 지원하는 최소 맵 버전.

/**
 * @brief 환경 오브젝트 정보 구조체
 */
#define ENVOBJ_ELEM_FOG					0x0001	///< 환경 오브젝트의 포그 요소 플래그

typedef struct St_EnvObject
{
	WzID			m_WzID;				///< WzID로 표현되는 식별자	
	WzOctIndex		m_OctIndex;			///< Octree상에 등록시 받는 인덱스값
	float			m_fDistance;		///< 경계컬링의 기준점과 경계간의 교차점까지의 거리
	WzBoundingVolume m_BoundVolume;
	WORD            m_wTileAtt;			///< Todo:타일 속성으로 삽입 할수 있는 요소 추가 
	WORD			m_fElements;		///< 앞으로 환경 객체에 다른 요소가 첨가될때 표시하기 위한 Flag
       
	float			m_fFogBegin;
	float			m_fFogEnd;
	float			m_fFogRate;
	WzColor			m_wcFog;
} ENVIRONMENTOBJECT;

typedef tDistanceObject< St_EnvObject* >	DISTOBJ_ENVOBJ;
typedef std::vector< DISTOBJ_ENVOBJ >		StlVDistObjBuffer_EnvObj;

#define BF_BLEND_FOGCOLOR			0x00000001
#define BF_BLEND_FOGNEAR			0x00000002
#define BF_BLEND_FOGFAR				0x00000004
#define BF_TO_GLOBALFOG				0x00010000

enum eEnvActorType
{
    EAT_BY_CAMERA,          ///< 엔진 내부 카메라에 의한 갱신
    EAT_BY_ACTORPOSITION    ///< 외부에서 입력한 위치를 기준으로한 갱신
};

class CWzArchive;

class CEnvObjectManager
{
	friend class CWorldTerrain;

public:
	virtual void	Clear(void);

protected:
	virtual int		Add(St_EnvObject *pEnvObject);
public:
	virtual int		MakeStEnvObject(St_EnvObject *pEnvObject);//엔진 외부에서 삽입할때

	virtual bool	Remove(const WzOctIndex woi);

	/** 주어진 Octree index와 데이터 값으로 환경 객체를 찾는다. */
	virtual St_EnvObject* Find(const WzOctIndex woi, const DWORD dwObject);
	/** 주어진 WzID를 가진 환경 객체를 찾는다. */
	virtual St_EnvObject* Find(const WzID wID);
    virtual void          SetFogBlendingTime(const float fSecond);
    virtual const float   GetFogBlendingTime();
    inline virtual void   SetActorPos(const WzVector v);
    inline virtual WzVector GetActorPos() const;
	inline virtual void   SetActorTileAtt(const WORD wAtt)  ;

    inline virtual void SetActorType(const eEnvActorType type);
    inline virtual const eEnvActorType GetActorType() const;

	virtual int		      GetCount();
	virtual St_EnvObject* Get(int index);

	float			CalcDistance(WzBoundingVolume* pbvTarget, WzVector &wvFrom, float *pfDistToCenter, float *pfRadius);
	void			Update(CDrawBase* pDraw,  CWzArray<DWORD> *pArray, float fTime, float fRange=30.f);
	
	void			SetGlobalFogParams(WzColor clrFog, float fNear, float fFar, float fFogRate);

	bool			Serialize(CWzArchive *pArchive);
	bool			Load(CWzArchive *pArchive);
	bool			Save(CWzArchive *pArchive);

	// Constructor & Destructor
	CEnvObjectManager();
	~CEnvObjectManager();

private:

	CWzArray< St_EnvObject* > m_EnvTileObjList;
	CWzArray< St_EnvObject* > m_EnvObjList;

	DWORD			m_fBlendFlag;
	float			m_fStartTime;
	float           m_fBlendingTime;    ///< 다른 Fog로 변경되는데 걸리는 시간
    WzVector        m_wvActorPos;       ///< 환경 객체에 영향을 끼치는 기준 위치값
    eEnvActorType   m_ActorType;
	WORD            m_wActorTileAtt;    /// 액터의 현재 밟고 있는 속성 

	WzID			m_CurEnvObjID;
	WzColor			m_clrCurFog;
	float			m_fCurFogBegin;
	float			m_fCurFogEnd;
	float			m_fCurFogRate;

    WzColor			m_clrSrcFog;
	float			m_fSrcFogBegin;
	float			m_fSrcFogEnd;
	float			m_fSrcFogRate;

	WzColor			m_clrDestFog;
	float			m_fDestFogBegin;
	float			m_fDestFogEnd;
	float			m_fDestFogRate;

	WzColor			m_clrGlobalFog;
	float			m_fGlobalFogBegin;
	float			m_fGlobalFogEnd;
	float			m_fGlobalFogRate;
};

//
// Inline methods...
//
inline void CEnvObjectManager::SetActorPos(const WzVector v)           { m_wvActorPos = v; }
inline WzVector CEnvObjectManager::GetActorPos() const                 { return m_wvActorPos; }
inline void CEnvObjectManager::SetActorTileAtt(const WORD wAtt)           { m_wActorTileAtt = wAtt; }

inline void CEnvObjectManager::SetActorType(const eEnvActorType type)  { m_ActorType = type; }
inline const eEnvActorType CEnvObjectManager::GetActorType() const     { return m_ActorType; }

#endif // _3DTERRAIN_ENVOBJECT_H_