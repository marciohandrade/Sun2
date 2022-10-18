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

	// ��� ����
protected:
	CWzArray<CFlockBoid*> m_arMember;
	WzBoundingVolume m_BoundingVolume;

	// Ư��ġ
protected:
	float m_fSpeed;
	float m_fDistanceFromFriend[2];	// min, max

	// �׷����� �ٴϱ�
public:
	virtual WzVector GetDirectionToBeInFlock( CFlockBoid* pFlockBoid);
};

#endif // _3DTERRAIN_FLOCKGROUP_H_