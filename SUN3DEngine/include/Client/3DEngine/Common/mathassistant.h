// mathassistant.h

#ifndef _COMMON_MATHASSISTANT_H_
#define _COMMON_MATHASSISTANT_H_

#include "boundingvolume.h"

#define INT2FLOAT( n )  (float( n ))

#ifdef _DEBUG
#define FLOAT2INT( f )  (int( f ))
#else
  #ifdef _WIN64
    #define FLOAT2INT( f )  (int( f ))
  #else
    inline int FLOAT2INT( float f )
	{
		__asm cvttss2si eax,[f]
	}
  #endif //_WIN64
#endif //_DEBUG


// 두 float 사이의 랜덤 값 얻기
float Math_GetRandomFloatBetween( float fMin, float fMax );

// 데이터 용량 줄이기 (float을 100배 한 short로)
short Math_FloatToShort( float fVal );
// 데이터 용량 줄이기 (100배 한 short에서 float 얻기)
float Math_FloatFromShort( short nVal );

// fParamN일 때 fResultN인 경우, fParamToGet일 때 fResult 얻기
float Math_GetProportionalValueBetween( float fParam1, 
                                        float fParam2, 
                                        float fParamToGet,
                                        float fResult1,
                                        float fResult2 );

// 주어진 벡터와 수직인 두 벡터 얻기
void Math_GetOrthogonalVectors( WzVector* pwvOut1,
                                WzVector* pwvOut2,
                                const WzVector* pwvIn );

// 주어진 벡터를 회전 시켜 랜덤 벡터 얻기
WzVector* Math_GetRandomDirection( WzVector* pwvOut, const WzVector* pwvBase, float fAngle );

// BV 안의 random 위치 구하기
void Math_GetRandomPosInBoundingVolume( WzVector* pwvOut, const WzBoundingVolume* pwbv );

// 방향각 얻기 - 라디안 (-y축 : 0도, +x축 : 90도)
float Math_GetAngleFromVector( const WzVector* pwvDir );

//------------------------------------------------------------------------------
/**
*/
inline float Math_GetRandomFloatBetween( float fMin, float fMax )
{
    return ( fMin + ( fMax - fMin ) * ( ( (float)( rand() % 10001 ) ) * 0.0001f ) );
}

//------------------------------------------------------------------------------
/**
*/
inline short Math_FloatToShort( float fVal )
{
    WzAssert( -32000.0f <= fVal && fVal <= 32000.0f );

    return ( (short)( fVal * 100.0f ) );
}

//------------------------------------------------------------------------------
/**
*/
inline float Math_FloatFromShort( short nVal )
{
    return ( (float)nVal * 0.01f );
}

//------------------------------------------------------------------------------
/**
    단순히 직선의 방정식이다.

    y = (r2 - r1) / (p2 - p1) * (get - p1) + r1
*/
inline float Math_GetProportionalValueBetween( float fParam1, 
                                               float fParam2, 
                                               float fParamToGet, 
                                               float fResult1, 
                                               float fResult2 )
{
    float fParamDiv = fParam2 - fParam1;
    float fParamMul = fParamToGet - fParam1;

    if( fParamDiv == 0.0f )
    {
        return fResult1;
    }

    return ( fResult1 + ( fResult2 - fResult1 ) * fParamMul / fParamDiv );
}

//------------------------------------------------------------------------------
/**
    오른손 좌표계

     0도 : -y축 (0.0f, -1.0f, 0.0f)
    90도 : +x축 (1.0f, 0.0f, 0.0f)
*/
inline float Math_GetAngleFromVector( const WzVector* pwvDir )
{
    WzAssert( pwvDir );

    WzVector wvDirNormal;
    SetVector( &wvDirNormal, pwvDir->x, pwvDir->y, 0.0f );

    VectorNormalize( &wvDirNormal, &wvDirNormal );

    float fAngle = acosf( -wvDirNormal.y );
    if( wvDirNormal.x < 0.0f )
    {
        // fAngle = ( 2 * WZ_PI ) - fAngle;
        fAngle = 6.28318530717958647692f - fAngle;
    }

    return fAngle;
}

#endif // _COMMON_MATHASSISTANT_H_
