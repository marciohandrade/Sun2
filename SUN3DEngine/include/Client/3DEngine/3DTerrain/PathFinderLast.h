// pathfinderlast.h

#ifndef _3DTERRAIN_PATHFINDERLAST_H_
#define _3DTERRAIN_PATHFINDERLAST_H_

#include "pathfinderconvex.h"


class CPathFinderLast : public CPathFinderConvex  
{
public:
	CPathFinderLast();
	virtual ~CPathFinderLast();
	virtual void Clear();

	virtual BOOL Create( COctree *pOctree, CRoomManager *pRoomManager);
	virtual void Destroy();

	virtual BOOL Serialize( CWzArchive *pArchive);
};

#endif // _3DTERRAIN_PATHFINDERLAST_H_
