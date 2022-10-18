// flockgroup.h

#ifndef _3DTERRAIN_FLOCKGROUP_H_
#define _3DTERRAIN_FLOCKGROUP_H_

#include <programcommon/wzarray.h>

class CFlockBoid;


class CFlockGroup
{
public:
	CFlockGroup(void);
	virtual ~CFlockGroup(void);
	virtual void Clear();

	virtual BOOL Create( void* pExtraInfo = NULL);
	virtual void Destroy();

	// 멤버 정보
protected:
	CWzArray<CFlockBoid*> m_arMember;
	WzBoundingVolume m_BoundingVolume;

	// 특성치
protected:
	float m_fSpeed;
	float m_fDistanceFromFriend[2];	// min, max

	// 그룹지어 다니기
public:
	virtual WzVector GetDirectionToBeInFlock( CFlockBoid* pFlockBoid);
};

#endif // _3DTERRAIN_FLOCKGROUP_H_