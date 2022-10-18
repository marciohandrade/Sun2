// worldlast.h

#ifndef _3DTERRAIN_WORLDLAST_H_
#define _3DTERRAIN_WORLDLAST_H_

#include "WorldExtraInfo.h"


class CWorldLast : public CWorldExtraInfo  
{
public:
	CWorldLast();
	virtual ~CWorldLast();
	virtual void Clear();

	/// @name 생성/제거
	virtual BOOL Create( BOOL bUse = TRUE, BOOL bUseEdit = FALSE );
	virtual void Destroy( CDrawBase *pDraw);
};

#endif // _3DTERRAIN_WORLDLAST_H_
