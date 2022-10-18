#include "stdafx.h"
#include "battle.h"


bool IsPositionInSector( WzVector * pwvCheckPos, 
						 WzVector * pwvSrcPos, 
						 WzVector * pwvSrcDir,
						 float fRadian )
{
	WzVector v3 = *pwvCheckPos - *pwvSrcPos;

	float fRad = atan2(v3.y, v3.x) - atan2( pwvSrcDir->y, pwvSrcDir->x);

	return bool(fRad < fRadian && fRad > -fRadian );
}


bool IsPositionInCircleCone(WzVector * pwvSrcPos, 
							WzVector * pwvSrcDir, 
							WzVector * pwvCheckPos, 
							float fRadian )
{
	WzVector v3 = *pwvCheckPos - *pwvSrcPos;

	// 내적계산으로 각을 정확히 알아낼려면 벡터를 노말라이즈 하여야한다.
	VectorNormalize( &v3, &v3 );	

	float fRad = VectorDotProduct(pwvSrcDir, &v3);

	return bool(fRad <= 1.f && fRad >= cos(fRadian) );
}
