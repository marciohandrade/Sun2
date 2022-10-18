// flockmanager.h

#ifndef _3DTERRAIN_FLOCKMANAGER_H_
#define _3DTERRAIN_FLOCKMANAGER_H_

#include <programcommon/wzarray.h>

class CFlockBoid;
class CFlockGroup;


class CFlockManager
{
public:
	CFlockManager(void);
	virtual ~CFlockManager(void);

	// 리스트 관리
protected:
	CWzArray<CFlockBoid*> m_arBoid;
	CWzArray<CFlockGroup*> m_arGroup;
};

#endif // _3DTERRAIN_FLOCKMANAGER_H_