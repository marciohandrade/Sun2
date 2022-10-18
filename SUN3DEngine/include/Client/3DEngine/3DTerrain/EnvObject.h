// envobject.h

#ifndef _3DTERRAIN_ENVOBJECT_H_
#define _3DTERRAIN_ENVOBJECT_H_

//------------------------------------------------------------------------------
/**
    ������ ���� Ư�� ������ ���� ȯ�� ������ �����ϴ� Ŭ������ �����Ѵ�.
    ���α׷� �ۼ������ ������ Terrain�� ���� �μ� Manager ������ ������.

    �ο�����:
    ȯ�� ������Ʈ��? 
    ������ Ư�� ������ ���Ͽ� ���װ��� Local���� ��ȭ�� �ֱ� ���� ��ü�̴�.

    Date:   2006.7.27
    Rework: 
    Author: Won ShinYeon (3D Engine Team)

    LastModify:

    Copyright 2004 WEBZEN All rights.
*/

#include "distanceobject.h"
#include <vector>

#define OLDEST_MAPVERSION_ABOUT_ENVOBJECT	169 ///< ȯ�� ��ü�� �����ϴ� �ּ� �� ����.

/**
 * @brief ȯ�� ������Ʈ ���� ����ü
 */
#define ENVOBJ_ELEM_FOG					0x0001	///< ȯ�� ������Ʈ�� ���� ��� �÷���

typedef struct St_EnvObject
{
	WzID			m_WzID;				///< WzID�� ǥ���Ǵ� �ĺ���	
	WzOctIndex		m_OctIndex;			///< Octree�� ��Ͻ� �޴� �ε�����
	float			m_fDistance;		///< ����ø��� �������� ��谣�� ������������ �Ÿ�
	WzBoundingVolume m_BoundVolume;
	WORD            m_wTileAtt;			///< Todo:Ÿ�� �Ӽ����� ���� �Ҽ� �ִ� ��� �߰� 
	WORD			m_fElements;		///< ������ ȯ�� ��ü�� �ٸ� ��Ұ� ÷���ɶ� ǥ���ϱ� ���� Flag
       
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
    EAT_BY_CAMERA,          ///< ���� ���� ī�޶� ���� ����
    EAT_BY_ACTORPOSITION    ///< �ܺο��� �Է��� ��ġ�� ���������� ����
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
	virtual int		MakeStEnvObject(St_EnvObject *pEnvObject);//���� �ܺο��� �����Ҷ�

	virtual bool	Remove(const WzOctIndex woi);

	/** �־��� Octree index�� ������ ������ ȯ�� ��ü�� ã�´�. */
	virtual St_EnvObject* Find(const WzOctIndex woi, const DWORD dwObject);
	/** �־��� WzID�� ���� ȯ�� ��ü�� ã�´�. */
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
	float           m_fBlendingTime;    ///< �ٸ� Fog�� ����Ǵµ� �ɸ��� �ð�
    WzVector        m_wvActorPos;       ///< ȯ�� ��ü�� ������ ��ġ�� ���� ��ġ��
    eEnvActorType   m_ActorType;
	WORD            m_wActorTileAtt;    /// ������ ���� ��� �ִ� �Ӽ� 

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