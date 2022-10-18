// wzdshareinfo.h

#ifndef _3DFILEACCESS_WZDSHAREINFO_H_
#define _3DFILEACCESS_WZDSHAREINFO_H_

#include "wzdtypedef.h"

class CDrawBase;

//------------------------------------------------------------------------------
/**
    @class CWzdShareInfo

    - 메시용 공유 정보
*/
class CWzdShareInfo
{
public:
    // 인스턴스 얻기
    static CWzdShareInfo* Instance();

    // 렌더러 설정/얻기
    void SetRenderer( CDrawBase* pRenderer );
    CDrawBase* GetRenderer();

    // 카메라 방향 변경시
    void OnCameraDirChange();
    // 카메라 방향 얻기 (주의: 카메라가 보는 방향)
    const WzVector& GetCameraDir() const;

    // 빛 방향 설정/얻기 (주의: 빛이 나가는 반대 방향)
    void SetLightDir( const WzVector& dir );
    const WzVector& GetLightDir() const;

    // blinn 벡터 얻기
    void GetBlinnVectors( WzVector pwvBlinnVectors[3] );

    // Brightness 공식 인자 설정/얻기
    void SetBrightnessFunc( const float fFunc[3] );
    void GetBrightnessFunc( float fFunc[3] );

private:
    // 생성자/소멸자
    CWzdShareInfo();
    ~CWzdShareInfo();

    // 초기화
    void Initialize();

    // blinn 벡터 계산
    void CalculateBlinnVector();

private:
    CDrawBase* m_pRenderer;

    float m_fBrightnessFunc[3];

    WzVector m_wvCamDir;    // 카메라가 보는 방향
    WzVector m_wvLightDir;  // 빛이 나가는 반대 방향

    WzVector m_wvBlinnVec;  // Blinn 벡터
    WzVector m_wvBlinnU;    // Blinn 벡터 수직 벡터(u축)
    WzVector m_wvBlinnV;    // Blinn 벡터 수직 벡터(v축)
};

//------------------------------------------------------------------------------
/**
*/
inline CWzdShareInfo* CWzdShareInfo::Instance()
{
    static CWzdShareInfo instance;

    return &instance;
}

//------------------------------------------------------------------------------
/**
    xxx: 본 프로젝트에서 리소스 로딩등 렌더러에 접근할 일이 있어서
    부득이 여기서 렌더러를 설정 했지만, 구조적으로 별로 좋아보이진 않는다.
*/
inline void CWzdShareInfo::SetRenderer( CDrawBase* pRenderer )
{
    m_pRenderer = pRenderer;
}

//------------------------------------------------------------------------------
/**
*/
inline CDrawBase* CWzdShareInfo::GetRenderer()
{
    return m_pRenderer;
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CWzdShareInfo::GetCameraDir() const
{
    return m_wvCamDir;
}

//------------------------------------------------------------------------------
/**
*/
inline const WzVector& CWzdShareInfo::GetLightDir() const
{
    return m_wvLightDir;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzdShareInfo::GetBlinnVectors( WzVector pwvBlinnVectors[3] )
{
    WzAssert( pwvBlinnVectors );

    pwvBlinnVectors[0] = m_wvBlinnVec;
    pwvBlinnVectors[1] = m_wvBlinnU;
    pwvBlinnVectors[2] = m_wvBlinnV;
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzdShareInfo::SetBrightnessFunc( const float fFunc[3] )
{
    WzAssert( fFunc );

    memcpy( m_fBrightnessFunc, fFunc, sizeof( float ) * 3 );
}

//------------------------------------------------------------------------------
/**
*/
inline void CWzdShareInfo::GetBrightnessFunc( float fFunc[3] )
{
    WzAssert( fFunc );

    memcpy( fFunc, m_fBrightnessFunc, sizeof( float ) * 3 );
}

#endif // _3DFILEACCESS_WZDSHAREINFO_H_
