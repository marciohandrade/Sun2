// flockboid.h

#ifndef _3DTERRAIN_FLOCKBOID_H_
#define _3DTERRAIN_FLOCKBOID_H_

class CWzUnitDraw;


class CFlockBoid
{
public:
	CFlockBoid(void);
	virtual ~CFlockBoid(void);

	virtual BOOL Create( WzVector wvPos, void* pExtraInfo = NULL);
	virtual void Destroy();

	// 기본 정보
protected:
	WzVector m_wvPosition;
	WzVector m_wvDirection;
	int m_iGroup;
	CWzUnitDraw *m_UnitDraw;
public:
	WzVector GetPosition()	{ return ( m_wvPosition); }
	WzVector GetDirection()	{ return ( m_wvDirection); }
};

#endif // _3DTERRAIN_FLOCKBOID_H_