#ifndef __PUBLICMATH_H__
#define __PUBLICMATH_H__


#include "SinTable.h"
#include <time.h>

// 반올림 함수
#define round(dTemp)	( long( dTemp + (dTemp > 0 ? 0.5 : -0.5 ) ) )

inline int Math_GetAngle(const WzVector* pvNormal)
{
	int iAngle =  g_SinTable.FACOS( pvNormal->x );

	if( pvNormal->y < 0.0f )
	{
		iAngle = 360 - iAngle;
	}
	return iAngle;
}

inline void Math_ZRotateVector(const WzVector* pwzVector, WzVector& wvRotate, int iAngle)
{
	wvRotate.x = pwzVector->x * g_SinTable.FCOS(iAngle) + pwzVector->y * g_SinTable.FSIN(iAngle);
	wvRotate.y = -pwzVector->x * g_SinTable.FSIN(iAngle) + pwzVector->y * g_SinTable.FCOS(iAngle);
	wvRotate.z = pwzVector->z;
}

// x축의 양의 방향을 0도로 시작하는 벡터의 방향각을 리턴해준다. 메모리에 담아놓은 FACOS() 값을 사용함. (0도 ~ 360도)
inline int Math_GetAngleFromVector2(const WzVector* pwvDir)
{
	WzVector vNormal;
	VectorNormalize(&vNormal, pwvDir);

	return Math_GetAngle( &vNormal );
}

inline float Math_VectorLength( const WzVector& vec1, const WzVector& vec2)
{
	WzVector tmp = vec1 - vec2;
	return VectorLength(&tmp);
}

inline float Math_VectorLength2( const WzVector& vec1, const WzVector& vec2)
{
	WzVector tmp = vec1 - vec2;
	return VectorLength2(&tmp);
}

// x, y 평면상의 거리를 구해준다.
inline float Math_Vector2DLength(const WzVector* pwzVector)
{
	return (float)( sqrt(pwzVector->x * pwzVector->x + pwzVector->y * pwzVector->y) );
}

inline float Math_Vector2DLength2(const WzVector* pwzVector)
{
	return pwzVector->x * pwzVector->x + pwzVector->y * pwzVector->y;
}
			 
inline float Math_Vector2DLength(const WzVector& vec1, const WzVector& vec2)
{
	WzVector tmp = vec1 - vec2;
	return Math_Vector2DLength(&tmp);
}

inline float Math_Vector3DLength(const WzVector *pwzVector)
{
	return (float)( sqrt(pwzVector->x * pwzVector->x + pwzVector->y * pwzVector->y + pwzVector->z * pwzVector->z) );
}

inline float Math_Vector3DLength(const WzVector& vec1, const WzVector& vec2)
{
	WzVector tmp = vec1 - vec2;
	return Math_Vector3DLength(&tmp);
}
//-------------------------------------------------------------------------------------------------
//                        - d
//                    --
//                 --
//              --  
//           --
//        --
//    a ---------------------> b
//        --
//            --         c(검사대상)
//                --
//                    --
//                        - e
//
// 좌표 a, b, c, d, e가 있을 때
// pwvTargetPos: 좌표 c
// pwvAttackerPos: 좌표 a
// pwvMainTargetDir: 벡터 a->b
// iSrcAngle: 벡터 a->d와 벡터 a->e가 이루는 각
//-------------------------------------------------------------------------------------------------
// 부채꼴 모양의 영역에 들어가는지 체크해준다.
inline bool Math_IsPositionInFanshapedArea(const WzVector* pwvTargetPos,
    const WzVector* pwvAttackerPos, const WzVector* pwvMainTargetDir, int iSrcAngle)
{
	WzVector vTargetDirNormal, vMainTargetDirNormal;
	WzVector vTargetVec = *pwvTargetPos - *pwvAttackerPos;
	WzVector vMainTargetDir = *pwvMainTargetDir;
	vTargetVec.z = 0.0f;
	vMainTargetDir.z = 0.0f;

	VectorNormalize( &vTargetDirNormal, &vTargetVec );
	VectorNormalize( &vMainTargetDirNormal, &vMainTargetDir );

    float fInner = vTargetDirNormal.x * vMainTargetDirNormal.x + vTargetDirNormal.y * vMainTargetDirNormal.y;

	bool bRet = ( fInner >= g_SinTable.FCOS(iSrcAngle / 2) ) || !fInner;

    return bRet;
}

//inline VOID LogToFile(char *FileName, const char *Content,...)
//{
//	char	Buf[1024];
//	char	TimeBuf[128];
//	int		Count;
//	FILE *fp;
//	va_list	arglist;
//	va_start(arglist,Content);
//	Count = vsprintf(Buf, Content, arglist);
//	fp	=	fopen(FileName, "at");
//	_strtime( TimeBuf );
//	fprintf(fp,"[%s] %s\n", TimeBuf, Buf);
//	fclose(fp);
//	va_end(arglist);
//}

inline bool Math_IsPositionInOBB(const WzVector* pvStart, const WzVector* pvEnd,
    float fWidth, const WzVector* pvTarget)
{
	WzVector vDiff = *pvStart - *pvEnd;
	WzVector vCenter = (*pvStart + *pvEnd) * 0.5f;

	vDiff.z = 0.0f;
	vCenter.z = 0.0f;

	// 두좌표가 차이가없으면 타겟이 없음을 리턴

	if (vDiff.x == 0 && vDiff.y == 0) 
	{
#ifdef _NA_006622_20130408_BUG_DASH_AND_PIERCE_SKILL
    // 2013.04.09 민경승 / 이부분에 의해서 이동형+관통형 스킬이 루프를 한번만 돌아서 다수의 적을 공격하지 못한다.
#else
		return FALSE;
#endif //_NA_006622_20130408_BUG_DASH_AND_PIERCE_SKILL
	}

#ifdef _NA_006622_20130408_BUG_DASH_AND_PIERCE_SKILL
    if (vDiff.x <= 0.01f && vDiff.y <= 0.01f)
    {
        vDiff.x = 0.0f; vDiff.y = 0.0f;
    }
#endif //_NA_006622_20130408_BUG_DASH_AND_PIERCE_SKILL

	float fLength = VectorLength(&vDiff);

	VectorNormalize(&vDiff,&vDiff);
	

	// 축 계산~
	float fAxis1X = vDiff.x;
	float fAxis1Y = vDiff.y;

	float fAxis2X = vDiff.y;
	float fAxis2Y = -vDiff.x;

	// 목표의 좌표를 축 변환~

	WzVector vOffset = *pvTarget - vCenter;			

	float fNewAxisX = -(vOffset.x * fAxis2Y + vOffset.y * -fAxis1Y);
	float fNewAxisY = -(vOffset.x * -fAxis2X + vOffset.y * fAxis1X);

	// DEBUG: float fDet = fAxis1X * fAxis2Y - fAxis1Y * fAxis2X;

	// 변환된 축좌표가 사각형안에 들어가는지 체크
	if (fabs(fNewAxisX) <= fLength * 0.5f)
	{
		if (fabs(fNewAxisY) <= fWidth * 0.5f)
		{
			return TRUE;
		}
	}

	return FALSE;
}






#endif