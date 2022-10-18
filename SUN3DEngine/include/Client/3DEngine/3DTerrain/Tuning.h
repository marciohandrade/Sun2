// Tuning.h:
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUNIN_H__FFD2E825_0C88_400A_A02F_243543992BDC__INCLUDED_)
#define AFX_TUNIN_H__FFD2E825_0C88_400A_A02F_243543992BDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "IncludeAll.h"


/// @name --- 튜닝할 수치

#define LIMIT_Z_JUMP				( 2.5f)		///< 점프가 있기 때문에 그냥 길찾기로 갈 수 있는 한계
#define LIMIT_Z_LIMIT_MESH_TILE		( 3.21f)		///< 길찾기 중 타일z 를 기준으로 메쉬z 를 얻었을때 둘 간의 차이 범위
#define ANGLE_NO_WALK				( WZ_PI / 3.0f)	///< 바닥 기울기가 이 각도 이상이면 갈 수 없음으로 인식됨


#endif // !defined(AFX_TUNIN_H__FFD2E825_0C88_400A_A02F_243543992BDC__INCLUDED_)
