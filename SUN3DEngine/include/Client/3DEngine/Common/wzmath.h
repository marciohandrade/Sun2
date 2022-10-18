// wzmath.h

#ifndef _COMMON_WZMATH_H_
#define _COMMON_WZMATH_H_

#ifndef _SERVER
    #include <d3d9.h>
#endif

//#define NO_ASM_MODE // 64bit에는 어쎔블리어가 안되서 64bit 빌드할 때 사용

#include "wzmatrix.h"
#include "wzquaternion.h"

// 함수 포인터 선언
extern WzMatrix* (*g_MatRotXFunc)( WzMatrix*, float );
extern WzMatrix* (*g_MatRotYFunc)( WzMatrix*, float );
extern WzMatrix* (*g_MatRotZFunc)( WzMatrix*, float );

extern WzMatrix* (*g_MatMulFunc)( WzMatrix*, const WzMatrix*, const WzMatrix* );
extern WzMatrix* (*g_MatMul4x3Func)( WzMatrix*, const WzMatrix*, const WzMatrix* );

extern WzQuaternion* (*g_QuatMulFunc)( WzQuaternion*, const WzQuaternion*, const WzQuaternion* );


// Wz <-> D3D 변환 관련 (클라이언트만)
#ifndef _SERVER

WzVector* D3DVECTORToWzVector( WzVector* pwvOut, const D3DVECTOR* pd3vIn );
D3DVECTOR* WzVectorToD3DVECTOR( D3DVECTOR* pd3vOut, const WzVector* pwvIn );

WzMatrix* D3DMATRIXToWzMatrix( WzMatrix* pwmOut, const D3DMATRIX* pd3mIn );
WzMatrix* D3DMATRIXToWzMatrix( WzMatrix* pwmOut, const WzMatrix* pwmIn );

D3DMATRIX* WzMatrixToD3DMATRIX( D3DMATRIX* pd3mOut, const WzMatrix* pwmIn );
D3DMATRIX* WzMatrixToD3DMATRIXTranspose( D3DMATRIX* pd3mOut, const WzMatrix* pwmIn );

#endif // _SERVER


// 벡터 변환 관련
WzVector* VectorTransformCoord( WzVector* pwvOut, const WzVector* pwvIn, const WzMatrix* pwmTransform );
WzVector* VectorTransformNormal( WzVector* pwvOut, const WzVector* pwvIn, const WzMatrix* pwmTransform );


// 매트릭스 관련
WzMatrix* MatrixIdentity( WzMatrix* pwmIn );
WzMatrix* MatrixArrayIdentity( WzMatrix* pwmInAry, int size );
WzMatrix* MatrixSetZero( WzMatrix* pwmIn );

WzMatrix* MatrixInverse( WzMatrix* pwmOut, const WzMatrix* pwmIn );

WzMatrix* MatrixScaling( WzMatrix* pwmOut, float sx, float sy, float sz );
WzMatrix* MatrixTranslation( WzMatrix* pwmOut, float x, float y, float z );

WzMatrix* MatrixRotationX( WzMatrix* pwmOut, float fAngle );
WzMatrix* MatrixRotationX_Scalar( WzMatrix* pwmOut, float fAngle );
WzMatrix* MatrixRotationX_SSE( WzMatrix* pwmOut, float fAngle );
WzMatrix* MatrixRotationX_3DNOW( WzMatrix* pwmOut, float fAngle );

WzMatrix* MatrixRotationY( WzMatrix* pwmOut, float fAngle );
WzMatrix* MatrixRotationY_Scalar( WzMatrix* pwmOut, float fAngle );
WzMatrix* MatrixRotationY_SSE( WzMatrix* pwmOut, float fAngle );
WzMatrix* MatrixRotationY_3DNOW( WzMatrix* pwmOut, float fAngle );

WzMatrix* MatrixRotationZ( WzMatrix* pwmOut, float fAngle );
WzMatrix* MatrixRotationZ_Scalar( WzMatrix* pwmOut, float fAngle );
WzMatrix* MatrixRotationZ_SSE( WzMatrix* pwmOut, float fAngle );
WzMatrix* MatrixRotationZ_3DNOW( WzMatrix* pwmOut, float fAngle );

WzMatrix* MatrixRotationZYX( WzMatrix* pwmOut, const WzVector* pwvAngle );
WzMatrix* MatrixRotationZXY( WzMatrix* pwmOut, const WzVector* pwvAngle );

WzMatrix* MatrixRotation( WzMatrix* pwmOut, const WzVector* pwvAxis, float fAngle );

WzMatrix* MatrixMultiply( WzMatrix* pwmOut, const WzMatrix* pwm1, const WzMatrix* pwm2 );
WzMatrix* MatrixMultiply_Scalar( WzMatrix*, const WzMatrix*, const WzMatrix* );
WzMatrix* MatrixMultiply_SSE( WzMatrix* pwmOut, const WzMatrix* pwm1, const WzMatrix* pwm2 );
WzMatrix* MatrixMultiply_3DNOW( WzMatrix* pwmOut, const WzMatrix* pwm1, const WzMatrix* pwm2 );

WzMatrix* MatrixMultiply4x3( WzMatrix* pwmOut, const WzMatrix* pwm1, const WzMatrix* pwm2 );
WzMatrix* MatrixMultiply4x3_Scalar( WzMatrix*, const WzMatrix*, const WzMatrix* );

WzMatrix* MatrixTranspose( WzMatrix* pwmOut, const WzMatrix* pwmIn );

WzMatrix* MatrixSRT( WzMatrix* pwmOut, const WzVector& wvScale, const WzVector& wvRot, const WzVector& wvPos );
WzMatrix* MatrixSRT( WzMatrix* pwmOut, const WzVector& wvScale, const WzMatrix* pwmRot, const WzVector& wvPos );


// 쿼터니언 관련
WzQuaternion* QuaternionInit( WzQuaternion* pwqIn );
WzQuaternion* QuaternionConjugate( WzQuaternion* pwqOut, const WzQuaternion* pwqIn );

WzQuaternion* QuaternionRotation( WzQuaternion* pwqOut, const WzVector* pwvAxis, float fAngle );
WzQuaternion* RotationToQuaternion( WzQuaternion* pqtRotate, const WzVector* pwvRotate );

WzQuaternion* GetQuaternionToConvertVector( WzQuaternion* pwqOut, const WzVector* pwvFrom, const WzVector* pwvTo );

WzMatrix* QuaternionToMatrix( WzMatrix* pwmOut, const WzQuaternion* pwqIn );
WzQuaternion* MatrixToQuaternion( WzQuaternion* pwqOut, const WzMatrix* pwmIn );

void QuaternionToAxisAngle( WzVector* pwvOutAxis, float* pfOutAngle, const WzQuaternion* pwqIn );

WzQuaternion* QuaternionSlerp( WzQuaternion* pwqOut, const WzQuaternion* pwq1, const WzQuaternion* pwq2, float t );

WzQuaternion* QuaternionMultiply( WzQuaternion* pwqOut, const WzQuaternion* pwq1, const WzQuaternion* pwq2 );
WzQuaternion* QuaternionMultiply_Scalar( WzQuaternion* pwqOut, const WzQuaternion* pwq1, const WzQuaternion* pwq2 );
WzQuaternion* QuaternionMultiply_SSE( WzQuaternion* pwqOut, const WzQuaternion* pwq1, const WzQuaternion* pwq2 );
WzQuaternion* QuaternionMultiply_3DNOWEXT( WzQuaternion* pwqOut, const WzQuaternion* pwq1, const WzQuaternion* pwq2 );


#ifndef _SERVER

//------------------------------------------------------------------------------
/**
*/
inline WzVector* D3DVECTORToWzVector( WzVector* pwvOut, const D3DVECTOR* pd3vIn )
{
    WzAssert( pwvOut );
    WzAssert( pd3vIn );

    pwvOut->x = pd3vIn->x;
    pwvOut->y = pd3vIn->z;
    pwvOut->z = pd3vIn->y;

    return pwvOut;
}

//------------------------------------------------------------------------------
/**
*/
inline D3DVECTOR* WzVectorToD3DVECTOR( D3DVECTOR* pd3vOut, const WzVector* pwvIn )
{
    WzAssert( pd3vOut );
    WzAssert( pwvIn );

    pd3vOut->x = pwvIn->x;
    pd3vOut->y = pwvIn->z;
    pd3vOut->z = pwvIn->y;

    return pd3vOut;
}

//------------------------------------------------------------------------------
/**
    2, 3열 <-> 3, 2열
    2, 3행 <-> 3, 2행
*/
inline WzMatrix* D3DMATRIXToWzMatrix( WzMatrix* pwmOut, const D3DMATRIX* pd3mIn )
{
    WzAssert( pwmOut );
    WzAssert( pd3mIn );

    pwmOut->_11 = pd3mIn->_11; pwmOut->_12 = pd3mIn->_13; pwmOut->_13 = pd3mIn->_12; pwmOut->_14 = pd3mIn->_14;
    pwmOut->_21 = pd3mIn->_31; pwmOut->_22 = pd3mIn->_33; pwmOut->_23 = pd3mIn->_32; pwmOut->_24 = pd3mIn->_34;
    pwmOut->_31 = pd3mIn->_21; pwmOut->_32 = pd3mIn->_23; pwmOut->_33 = pd3mIn->_22; pwmOut->_34 = pd3mIn->_24;
    pwmOut->_41 = pd3mIn->_41; pwmOut->_42 = pd3mIn->_43; pwmOut->_43 = pd3mIn->_42; pwmOut->_44 = pd3mIn->_44;

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
    pwmIn은 D3D형식으로 저장된 WzMatrix임
*/
inline WzMatrix* D3DMATRIXToWzMatrix( WzMatrix* pwmOut, const WzMatrix* pwmIn )
{
    WzAssert( pwmOut );
    WzAssert( pwmIn );
    WzAssert( pwmOut != pwmIn );

    pwmOut->_11 = pwmIn->_11; pwmOut->_12 = pwmIn->_13; pwmOut->_13 = pwmIn->_12; pwmOut->_14 = pwmIn->_14;
    pwmOut->_21 = pwmIn->_31; pwmOut->_22 = pwmIn->_33; pwmOut->_23 = pwmIn->_32; pwmOut->_24 = pwmIn->_34;
    pwmOut->_31 = pwmIn->_21; pwmOut->_32 = pwmIn->_23; pwmOut->_33 = pwmIn->_22; pwmOut->_34 = pwmIn->_24;
    pwmOut->_41 = pwmIn->_41; pwmOut->_42 = pwmIn->_43; pwmOut->_43 = pwmIn->_42; pwmOut->_44 = pwmIn->_44;

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
    2, 3열 <-> 3, 2열
    2, 3행 <-> 3, 2행
*/
inline D3DMATRIX* WzMatrixToD3DMATRIX( D3DMATRIX* pd3mOut, const WzMatrix* pwmIn )
{
    WzAssert( pd3mOut );
    WzAssert( pwmIn );

    pd3mOut->_11 = pwmIn->_11; pd3mOut->_12 = pwmIn->_13; pd3mOut->_13 = pwmIn->_12; pd3mOut->_14 = pwmIn->_14;
    pd3mOut->_21 = pwmIn->_31; pd3mOut->_22 = pwmIn->_33; pd3mOut->_23 = pwmIn->_32; pd3mOut->_24 = pwmIn->_34;
    pd3mOut->_31 = pwmIn->_21; pd3mOut->_32 = pwmIn->_23; pd3mOut->_33 = pwmIn->_22; pd3mOut->_34 = pwmIn->_24;
    pd3mOut->_41 = pwmIn->_41; pd3mOut->_42 = pwmIn->_43; pd3mOut->_43 = pwmIn->_42; pd3mOut->_44 = pwmIn->_44;

    return pd3mOut;
}

//------------------------------------------------------------------------------
/**
*/
inline D3DMATRIX* WzMatrixToD3DMATRIXTranspose( D3DMATRIX* pd3mOut, const WzMatrix* pwmIn )
{
    WzAssert( pd3mOut );
    WzAssert( pwmIn );

    pd3mOut->_11 = pwmIn->_11; pd3mOut->_12 = pwmIn->_31; pd3mOut->_13 = pwmIn->_21; pd3mOut->_14 = pwmIn->_41;
    pd3mOut->_21 = pwmIn->_13; pd3mOut->_22 = pwmIn->_33; pd3mOut->_23 = pwmIn->_23; pd3mOut->_24 = pwmIn->_43;
    pd3mOut->_31 = pwmIn->_12; pd3mOut->_32 = pwmIn->_32; pd3mOut->_33 = pwmIn->_22; pd3mOut->_34 = pwmIn->_42;
    pd3mOut->_41 = pwmIn->_14; pd3mOut->_42 = pwmIn->_34; pd3mOut->_43 = pwmIn->_24; pd3mOut->_44 = pwmIn->_44;

    return pd3mOut;
}

#endif // _SERVER

//------------------------------------------------------------------------------
/**
*/
inline WzVector* VectorTransformCoord( WzVector* pwvOut, const WzVector* pwvIn, const WzMatrix* pwmTransform )
{
    WzAssert( pwvOut );
    WzAssert( pwvIn );
    WzAssert( pwmTransform );
    WzAssert( pwvOut != pwvIn );

    pwvOut->x = pwvIn->x * pwmTransform->_11 + pwvIn->y * pwmTransform->_21 + pwvIn->z * pwmTransform->_31 + pwmTransform->_41;
    pwvOut->y = pwvIn->x * pwmTransform->_12 + pwvIn->y * pwmTransform->_22 + pwvIn->z * pwmTransform->_32 + pwmTransform->_42;
    pwvOut->z = pwvIn->x * pwmTransform->_13 + pwvIn->y * pwmTransform->_23 + pwvIn->z * pwmTransform->_33 + pwmTransform->_43;

    return pwvOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector* VectorTransformNormal( WzVector* pwvOut, const WzVector* pwvIn, const WzMatrix* pwmTransform )
{
    WzAssert( pwvOut );
    WzAssert( pwvIn );
    WzAssert( pwmTransform );
    WzAssert( pwvOut != pwvIn );

    pwvOut->x = pwvIn->x * pwmTransform->_11 + pwvIn->y * pwmTransform->_21 + pwvIn->z * pwmTransform->_31;
    pwvOut->y = pwvIn->x * pwmTransform->_12 + pwvIn->y * pwmTransform->_22 + pwvIn->z * pwmTransform->_32;
    pwvOut->z = pwvIn->x * pwmTransform->_13 + pwvIn->y * pwmTransform->_23 + pwvIn->z * pwmTransform->_33;

    return pwvOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixIdentity( WzMatrix* pwmIn )
{
    WzAssert( pwmIn );

                 pwmIn->_12 = pwmIn->_13 = pwmIn->_14 =
    pwmIn->_21 =              pwmIn->_23 = pwmIn->_24 =
    pwmIn->_31 = pwmIn->_32 =              pwmIn->_34 =
    pwmIn->_41 = pwmIn->_42 = pwmIn->_43 = 0.0f;
    pwmIn->_11 = pwmIn->_22 = pwmIn->_33 = pwmIn->_44 = 1.0f;

    return pwmIn;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixArrayIdentity( WzMatrix* pwmInAry, int size )
{
    WzAssert( pwmInAry );
    
    for( int i = 0; i < size; ++i )
    {
        MatrixIdentity( &pwmInAry[i] );
    }

    return pwmInAry;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixSetZero( WzMatrix* pwmIn )
{
    WzAssert( pwmIn );

    pwmIn->_11 = pwmIn->_12 = pwmIn->_13 = pwmIn->_14 =
    pwmIn->_21 = pwmIn->_22 = pwmIn->_23 = pwmIn->_24 =
    pwmIn->_31 = pwmIn->_32 = pwmIn->_33 = pwmIn->_34 =
    pwmIn->_41 = pwmIn->_42 = pwmIn->_43 = pwmIn->_44 = 0.0f;

    return pwmIn;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixScaling( WzMatrix* pwmOut, float sx, float sy, float sz )
{
    WzAssert( pwmOut );

    // 대각 원소 제외 전부 0.0f
                  pwmOut->_12 = pwmOut->_13 = pwmOut->_14 =
    pwmOut->_21 =               pwmOut->_23 = pwmOut->_24 =
    pwmOut->_31 = pwmOut->_32 =               pwmOut->_34 =
    pwmOut->_41 = pwmOut->_42 = pwmOut->_43 = 0.0f;

    // 대각 원소 설정
    pwmOut->_11 = sx;
    pwmOut->_22 = sy;
    pwmOut->_33 = sz;
    pwmOut->_44 = 1.0f;

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixTranslation( WzMatrix* pwmOut, float x, float y, float z )
{
    WzAssert( pwmOut );

    // 대각 원소 제외 전부 0.0f
                  pwmOut->_12 = pwmOut->_13 = pwmOut->_14 =
    pwmOut->_21 =               pwmOut->_23 = pwmOut->_24 =
    pwmOut->_31 = pwmOut->_32 =               pwmOut->_34 = 0.0f;

    // 대각 원소 전부 1.0f
    pwmOut->_11 = pwmOut->_22 = pwmOut->_33 = pwmOut->_44 = 1.0f;

    // 4행 원소 설정
    pwmOut->_41 = x;
    pwmOut->_42 = y;
    pwmOut->_43 = z;

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixRotationX( WzMatrix* pwmOut, float fAngle )
{
    WzAssert( pwmOut );

    return g_MatRotXFunc( pwmOut, fAngle );
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixRotationY( WzMatrix* pwmOut, float fAngle )
{
    WzAssert( pwmOut );

    return g_MatRotYFunc( pwmOut, fAngle );
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixRotationZ( WzMatrix* pwmOut, float fAngle )
{
    WzAssert( pwmOut );

    return g_MatRotZFunc( pwmOut, fAngle );
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixRotation( WzMatrix* pwmOut, const WzVector* pwvAxis, float fAngle )
{
    WzAssert( pwmOut );
    WzAssert( pwvAxis );

    WzQuaternion wqRotate;

    QuaternionRotation( &wqRotate, pwvAxis, fAngle );

    return QuaternionToMatrix( pwmOut, &wqRotate );
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixMultiply( WzMatrix* pwmOut, const WzMatrix* pwm1, const WzMatrix* pwm2 )
{
    WzAssert( pwmOut );
    WzAssert( pwm1 );
    WzAssert( pwm2 );

    return g_MatMulFunc( pwmOut, pwm1, pwm2 );
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixMultiply4x3( WzMatrix* pwmOut, const WzMatrix* pwm1, const WzMatrix* pwm2 )
{
    WzAssert( pwmOut );
    WzAssert( pwm1 );
    WzAssert( pwm2 );

    return g_MatMul4x3Func( pwmOut, pwm1, pwm2 );
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixTranspose( WzMatrix* pwmOut, const WzMatrix* pwmIn )
{
    WzAssert( pwmOut );
    WzAssert( pwmIn );
    WzAssert( pwmOut != pwmIn );

    pwmOut->_11 = pwmIn->_11; pwmOut->_12 = pwmIn->_21; pwmOut->_13 = pwmIn->_31; pwmOut->_14 = pwmIn->_41;
    pwmOut->_21 = pwmIn->_12; pwmOut->_22 = pwmIn->_22; pwmOut->_23 = pwmIn->_32; pwmOut->_24 = pwmIn->_42;
    pwmOut->_31 = pwmIn->_13; pwmOut->_32 = pwmIn->_23; pwmOut->_33 = pwmIn->_33; pwmOut->_34 = pwmIn->_43;
    pwmOut->_41 = pwmIn->_14; pwmOut->_42 = pwmIn->_24; pwmOut->_43 = pwmIn->_34; pwmOut->_44 = pwmIn->_44;

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixSRT( WzMatrix* pwmOut, const WzVector& wvScale, const WzVector& wvRot, const WzVector& wvPos )
{
    WzAssert( pwmOut );

    // 파라미터 행렬(크기 * 회전 * 위치) 구하기
    // 각각에 대해 행렬을 구하고 곱해서 최종 행렬을 만들어야 하나
    // 다음과 같이 간단하게 구할 수 있다.

    // 회전 행렬로 초기화
    MatrixRotationZYX( pwmOut, &wvRot );

    // 스케일 적용 (스케일 행렬 * 회전 행렬 순)
    // 주의: 회전 행렬에 스케일을 곱하는 것 같지만, 결과적으로는
    // (스케일행렬 * 회전행렬)이다. 각 행렬을 따로 구해서 곱해볼 것
    // 엄연히 (크기 * 회전) != (회전 * 크기) 이다.

    // ex) 1행 각 원소 = 1행 각 원소 * x스케일값
    for( int i = 0; i < 3; ++i )
    {
        pwmOut->m_Element[i][0] *= wvScale.m_Element[i];
        pwmOut->m_Element[i][1] *= wvScale.m_Element[i];
        pwmOut->m_Element[i][2] *= wvScale.m_Element[i];
    }

    // 이동 적용
    pwmOut->_41 = wvPos.x;
    pwmOut->_42 = wvPos.y;
    pwmOut->_43 = wvPos.z;

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixSRT( WzMatrix* pwmOut, const WzVector& wvScale, const WzMatrix* pwmRot, const WzVector& wvPos )
{
    WzAssert( pwmOut );

    // 파라미터 행렬(크기 * 회전 * 위치) 구하기
    // 각각에 대해 행렬을 구하고 곱해서 최종 행렬을 만들어야 하나
    // 다음과 같이 간단하게 구할 수 있다.

    // 회전 행렬로 초기화
    if( pwmRot )
    {
        *pwmOut = *pwmRot;
    }
    else
    {
        MatrixIdentity( pwmOut );
    }

    // 스케일 적용 (스케일 행렬 * 회전 행렬 순)
    // 주의: 회전 행렬에 스케일을 곱하는 것 같지만, 결과적으로는
    // (스케일행렬 * 회전행렬)이다. 각 행렬을 따로 구해서 곱해볼 것
    // 엄연히 (크기 * 회전) != (회전 * 크기) 이다.

    // ex) 1행 각 원소 = 1행 각 원소 * x스케일값
    for( int i = 0; i < 3; ++i )
    {
        pwmOut->m_Element[i][0] *= wvScale.m_Element[i];
        pwmOut->m_Element[i][1] *= wvScale.m_Element[i];
        pwmOut->m_Element[i][2] *= wvScale.m_Element[i];
    }

    // 이동 적용
    pwmOut->_41 = wvPos.x;
    pwmOut->_42 = wvPos.y;
    pwmOut->_43 = wvPos.z;

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixRotationX_Scalar( WzMatrix* pwmOut, float fAngle )
{
    WzAssert( pwmOut );

                  pwmOut->_12 = pwmOut->_13 = pwmOut->_14 =
    pwmOut->_21 =                             pwmOut->_24 = 
    pwmOut->_31 =                             pwmOut->_34 =
    pwmOut->_41 = pwmOut->_42 = pwmOut->_43 = 0.0f;

    pwmOut->_11 = pwmOut->_44 = 1.0f;

    pwmOut->_22 = pwmOut->_33 = cosf( fAngle );
    pwmOut->_23 = sinf( fAngle );
    pwmOut->_32 = -pwmOut->_23;

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixRotationY_Scalar( WzMatrix* pwmOut, float fAngle )
{
    WzAssert( pwmOut );

                  pwmOut->_12 =               pwmOut->_14 =
    pwmOut->_21 =               pwmOut->_23 = pwmOut->_24 =
                  pwmOut->_32 =               pwmOut->_34 =
    pwmOut->_41 = pwmOut->_42 = pwmOut->_43 = 0.0f;

    pwmOut->_22 = pwmOut->_44 = 1.0f;

    pwmOut->_33 = pwmOut->_11 = cosf( fAngle );
    pwmOut->_31 = sinf( fAngle );
    pwmOut->_13 = -pwmOut->_31;

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* MatrixRotationZ_Scalar( WzMatrix* pwmOut, float fAngle )
{
    WzAssert( pwmOut );

                                pwmOut->_13 = pwmOut->_14 =
                                pwmOut->_23 = pwmOut->_24 =
    pwmOut->_31 = pwmOut->_32 =               pwmOut->_34 =
    pwmOut->_41 = pwmOut->_42 = pwmOut->_43 = 0.0f;

    pwmOut->_33 = pwmOut->_44 = 1.0f;

    pwmOut->_11 = pwmOut->_22 = cosf( fAngle );
    pwmOut->_12 = sinf( fAngle );
    pwmOut->_21 = -pwmOut->_12;

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzQuaternion* QuaternionInit( WzQuaternion* pwqIn )
{
    WzAssert( pwqIn );

    pwqIn->x = pwqIn->y = pwqIn->z = 0.0f;
    pwqIn->w = 1.0f;

    return pwqIn;
}

//------------------------------------------------------------------------------
/**
*/
inline WzQuaternion* QuaternionConjugate( WzQuaternion* pwqOut, const WzQuaternion* pwqIn )
{
    WzAssert( pwqOut );
    WzAssert( pwqIn );

    pwqOut->x = -pwqIn->x;
    pwqOut->y = -pwqIn->y;
    pwqOut->z = -pwqIn->z;
    pwqOut->w =  pwqIn->w;

    return pwqOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzQuaternion* QuaternionRotation( WzQuaternion* pwqOut, const WzVector* pwvAxis, float fAngle )
{
    WzAssert( pwqOut );
    WzAssert( pwvAxis );

    WzVector v;
    VectorNormalize( &v, pwvAxis );

    float fSin = sinf( fAngle * 0.5f );
    float fCos = cosf( fAngle * 0.5f );

    pwqOut->x = fSin * v.x;
    pwqOut->y = fSin * v.y;
    pwqOut->z = fSin * v.z;
    pwqOut->w = fCos;

    return pwqOut;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMatrix* QuaternionToMatrix( WzMatrix* pwmOut, const WzQuaternion* pwqIn )
{
    WzAssert( pwmOut );
    WzAssert( pwqIn );

    static float fInXX, fInYY, fInZZ, fInXY, fInXZ, fInYZ, fInWZ, fInWY, fInWX;

    fInXX = (pwqIn->x * pwqIn->x) * 2.0f;
    fInYY = (pwqIn->y * pwqIn->y) * 2.0f;
    fInZZ = (pwqIn->z * pwqIn->z) * 2.0f;
    fInXY = (pwqIn->x * pwqIn->y) * 2.0f;
    fInXZ = (pwqIn->x * pwqIn->z) * 2.0f;
    fInYZ = (pwqIn->y * pwqIn->z) * 2.0f;
    fInWZ = (pwqIn->w * pwqIn->z) * 2.0f;
    fInWY = (pwqIn->w * pwqIn->y) * 2.0f;
    fInWX = (pwqIn->w * pwqIn->x) * 2.0f;

    pwmOut->_11 = 1.0f - fInYY - fInZZ;
    pwmOut->_12 = fInXY + fInWZ;
    pwmOut->_13 = fInXZ - fInWY;

    pwmOut->_21 = fInXY - fInWZ;
    pwmOut->_22 = 1.0f - fInXX - fInZZ;
    pwmOut->_23 = fInYZ + fInWX;

    pwmOut->_31 = fInXZ + fInWY;
    pwmOut->_32 = fInYZ - fInWX;
    pwmOut->_33 = 1.0f - fInXX - fInYY;

    pwmOut->_14 = pwmOut->_24 = pwmOut->_34 = pwmOut->_41 = pwmOut->_42 = pwmOut->_43 = 0.0f;
    pwmOut->_44 = 1.0f;

/*
    // 기존
    pwmOut->_11 = 1.0f - 2.0f * ( pwqIn->y * pwqIn->y + pwqIn->z * pwqIn->z);
    pwmOut->_12 = 2.0f * ( pwqIn->x * pwqIn->y + pwqIn->w * pwqIn->z);
    pwmOut->_13 = 2.0f * ( pwqIn->x * pwqIn->z - pwqIn->w * pwqIn->y);

    pwmOut->_21 = 2.0f * ( pwqIn->x * pwqIn->y - pwqIn->w * pwqIn->z);
    pwmOut->_22 = 1.0f - 2.0f * ( pwqIn->x * pwqIn->x + pwqIn->z * pwqIn->z);
    pwmOut->_23 = 2.0f * ( pwqIn->y * pwqIn->z + pwqIn->w * pwqIn->x);

    pwmOut->_31 = 2.0f * ( pwqIn->x * pwqIn->z + pwqIn->w * pwqIn->y);
    pwmOut->_32 = 2.0f * ( pwqIn->y * pwqIn->z - pwqIn->w * pwqIn->x);
    pwmOut->_33 = 1.0f - 2.0f * ( pwqIn->x * pwqIn->x + pwqIn->y * pwqIn->y);

    pwmOut->_14 = pwmOut->_24 = pwmOut->_34 = pwmOut->_41 = pwmOut->_42 = pwmOut->_43 = 0.0f;
    pwmOut->_44 = 1.0f;
*/

/*
    // http://www.gamasutra.com/features/19980703/quaternions_01.htm
    static float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2; 

    // calculate coefficients
    x2 = pwqIn->x + pwqIn->x; y2 = pwqIn->y + pwqIn->y; 
    z2 = pwqIn->z + pwqIn->z;
    xx = pwqIn->x * x2; xy = pwqIn->x * y2; xz = pwqIn->x * z2;
    yy = pwqIn->y * y2; yz = pwqIn->y * z2; zz = pwqIn->z * z2;
    wx = pwqIn->w * x2; wy = pwqIn->w * y2; wz = pwqIn->w * z2;

    pwmOut->m_Element[0][0] = 1.0f - (yy + zz);
    pwmOut->m_Element[1][0] = xy - wz;
    pwmOut->m_Element[2][0] = xz + wy;
    pwmOut->m_Element[3][0] = 0.0;

    pwmOut->m_Element[0][1] = xy + wz;
    pwmOut->m_Element[1][1] = 1.0f - (xx + zz);
    pwmOut->m_Element[2][1] = yz - wx;
    pwmOut->m_Element[3][1] = 0.0f;

    pwmOut->m_Element[0][2] = xz - wy;
    pwmOut->m_Element[1][2] = yz + wx;
    pwmOut->m_Element[2][2] = 1.0f - (xx + yy);
    pwmOut->m_Element[3][2] = 0.0f;

    pwmOut->m_Element[0][3] = pwmOut->m_Element[1][3] = pwmOut->m_Element[2][3] = 0.0f;
    pwmOut->m_Element[3][3] = 1.0f;
*/

    return pwmOut;
}

//------------------------------------------------------------------------------
/**
    pwqOut = pwq1 * pwq2

    cf) D3DXQuaternionMultiply()와 입력 인자 순서가 틀림
*/
inline WzQuaternion* QuaternionMultiply( WzQuaternion* pwqOut, const WzQuaternion* pwq1, const WzQuaternion* pwq2 )
{
    WzAssert( pwqOut );
    WzAssert( pwq1 );
    WzAssert( pwq2 );

    return g_QuatMulFunc( pwqOut, pwq1, pwq2 );
}

//------------------------------------------------------------------------------
/**
*/
inline WzQuaternion* QuaternionMultiply_Scalar( WzQuaternion* pwqOut, const WzQuaternion* pwq1, const WzQuaternion* pwq2 )
{
    WzAssert( pwqOut );
    WzAssert( pwq1 );
    WzAssert( pwq2 );

    pwqOut->x = pwq1->y * pwq2->z - pwq1->z * pwq2->y + pwq1->w * pwq2->x + pwq1->x * pwq2->w;
    pwqOut->y = pwq1->z * pwq2->x - pwq1->x * pwq2->z + pwq1->w * pwq2->y + pwq1->y * pwq2->w;
    pwqOut->z = pwq1->x * pwq2->y - pwq1->y * pwq2->x + pwq1->w * pwq2->z + pwq1->z * pwq2->w;
    pwqOut->w = pwq1->w * pwq2->w - pwq1->x * pwq2->x - pwq1->y * pwq2->y - pwq1->z * pwq2->z;

    return pwqOut;
}

#endif // _COMMON_WZMATH_H_