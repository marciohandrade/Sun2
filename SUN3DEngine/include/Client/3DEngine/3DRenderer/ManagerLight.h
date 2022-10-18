// managerlight.h

#ifndef _3DRENDERER_MANAGERLIGHT_H_
#define _3DRENDERER_MANAGERLIGHT_H_

#include "managerbase.h"

#define DEFAULT_LIGHT_RANGE         5.0f
#define DEFAULT_LIGHT_INTENSITY     3.0f

// 커스텀 라이트 스타일
#define CLS_NONE                (0x00000000)   
#define CLS_LIGHT_COLOR         (0x00000001)    // 라이트 색
#define CLS_LIGHT_RANGE         (0x00000002)    // 범위
#define CLS_LIGHT_INTENSITY     (0x00000004)    // 강도
#define CLS_LIGHT_DIRECTION     (0x00000008)    // Directional Light의 방향
#define CLS_LIGHT_POSITION      (0x00000008)    // Point Light의 위치
#define CLS_LIGHT_ALL           (0xffffffff)    // 전부 사용
#define CLS_DEFAULT             CLS_LIGHT_ALL

class CWorldBase;

//------------------------------------------------------------------------------
/**
    @class CManagerLight
*/
class CManagerLight : public CManagerBase
{
public:
    // 생성자/소멸자
    CManagerLight();
    virtual ~CManagerLight();

    // 생성/파괴
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // 초기화
    virtual void Clear();

    // 방향 라이트 설정
    virtual BOOL SetDirectionalLight( int iIndex, 
                                      WzColor wcColor, 
                                      const WzVector& wvDirection, 
                                      ENUM_LIGHT_IRRA_TYPE eliType = 0,
                                      float fRange = DEFAULT_LIGHT_RANGE,
                                      float fIntensity = DEFAULT_LIGHT_INTENSITY );

    // 포인트 라이트 설정
    virtual BOOL SetPointLight( int iIndex, 
                                WzColor wcColor, 
                                const WzVector& wvPosition,
                                ENUM_LIGHT_IRRA_TYPE eliType = 0,
                                float fRange = DEFAULT_LIGHT_RANGE,
                                float fIntensity = DEFAULT_LIGHT_INTENSITY );

    // 라이트 정보 얻기
    virtual const St_LightInformation* GetLightInfo( int iIndex ) const;

    // 라이트 강도 설정/얻기
    BOOL SetLightIntensity( int iIndex, float fIntensity );
    float GetLightIntensity( int iIndex ) const;

    // 라이트 켜기/끄기
    virtual BOOL TurnOnLight( int iIndex, BOOL bOn );

    // 활성 라이트 인덱스/정보 얻기 (iActive 번째)
    int GetIndexOfActiveLight( int iActive ) const;
    virtual const St_LightInformation* GetActiveLightInfo( int iActive );

    // iActive 번째 라이트 활성 여부
    BOOL IsActiveLight( int iActive ) const;
    
    // 활성 라이트 수 얻기
    int GetNumberOfActiveLight() const;

    // 커스텀 방향 라이트 설정
    virtual BOOL SetCustomDirectionalLight( WzColor wcColor, 
                                            const WzVector& wvDirection,
                                            ENUM_LIGHT_IRRA_TYPE eliType = 0,
                                            float fRange = DEFAULT_LIGHT_RANGE,
                                            float fIntensity = DEFAULT_LIGHT_INTENSITY );

    // 커스텀 포인트 라이트 설정
    virtual BOOL SetCustomPointLight( WzColor wcColor, 
                                      const WzVector& wvPosition,
                                      ENUM_LIGHT_IRRA_TYPE eliType = 0,
                                      float fRange = DEFAULT_LIGHT_RANGE,
                                      float fIntensity = DEFAULT_LIGHT_INTENSITY );

    // 커스텀 라이트 정보 얻기
    virtual const St_LightInformation* GetCustomLightInfo( int iIndex );

    // 커스텀 라이트 스타일 설정/얻기
    void SetCustomLightStyle( DWORD dwStyle );
    DWORD GetCustomLightStyle() const;

    // 커스텀 라이트 켜기/끄기
    void TurnOnCustomLight( BOOL bOn );

    // irradiance 얻기
    virtual HANDLE GetIrradiance( ENUM_LIGHT_IRRA_TYPE iLit, ENUM_GROUP_MATERIAL_TYPE iGmt ) const;
    virtual HANDLE GetActiveIrradiance( int iActive, ENUM_GROUP_MATERIAL_TYPE iGmt );

    // irradiance 재로드
    virtual BOOL ReloadIrradiance();

    // 지역 라이트 사용 여부 설정/얻기
    void EnableAreaLight( BOOL bEnable );
    BOOL IsAreaLight() const;

    // 가까운 지역 라이트 얻기
    virtual void FindAreaLight( WzBoundingVolume* bvLight, int* iIndex );  

    // 라이트 적용
    virtual void ApplyAreaLight( const int* iIndex );

    // 월드 설정
    void SetWorld( CWorldBase* pWorld );

    // 파일 로드 경로 설정
    static void SetFilePath( const char* lpszPath );

protected:
    // 기본 라이트 설정
    virtual void SetDefaultLight();

    // software shading 을 위한 빛 정보 세팅
    virtual void UpdateWzdLightInfo();

    // 활성 라이트 초기화
    void InitActiveLight();

protected:
    static char m_szFilePath[MAX_PATH];

    St_LightInformation m_LightInfo[MAX_NUM_LIGHT];
    int m_iIndexOfActiveLight[MAX_NUM_ACTIVE_LIGHT];
    
    St_LightInformation m_CustomLightInfo;
    DWORD m_dwCustomLightStyle;
    BOOL m_bUseCustomLight;

    CWorldBase* x_pWorld;
    BOOL m_bEnableAreaLight;
};

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerLight::SetLightIntensity( int iIndex, float fIntensity )
{
    WzAssert( iIndex >= 0 && iIndex < MAX_NUM_LIGHT );

    if( iIndex >= 0 && iIndex < MAX_NUM_LIGHT )
    {
        m_LightInfo[iIndex].m_fIntensity = fIntensity;
        return TRUE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
inline float CManagerLight::GetLightIntensity( int iIndex ) const
{
    WzAssert( iIndex >= 0 && iIndex < MAX_NUM_LIGHT );

    if( iIndex >= 0 && iIndex < MAX_NUM_LIGHT )
    {
        return m_LightInfo[iIndex].m_fIntensity;
    }

    return 0.0f;
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerLight::InitActiveLight()
{
    memset( m_iIndexOfActiveLight, -1, sizeof( m_iIndexOfActiveLight ) );
}

//------------------------------------------------------------------------------
/**
*/
inline int CManagerLight::GetIndexOfActiveLight( int iActive ) const
{
    WzAssert( iActive >= 0 && iActive < MAX_NUM_ACTIVE_LIGHT );

    if( iActive >= 0 && iActive < MAX_NUM_ACTIVE_LIGHT )
    {
        return m_iIndexOfActiveLight[iActive];
    }

    return -1;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerLight::IsActiveLight( int iActive ) const
{
    return ( GetIndexOfActiveLight( iActive ) >= 0 );
}

//------------------------------------------------------------------------------
/**
*/
inline int CManagerLight::GetNumberOfActiveLight() const
{
    // xxx: 기존 호환
    int iNum = 0;
    for( int i = 0; i < MAX_NUM_ACTIVE_LIGHT; ++i )
    {
        if( m_iIndexOfActiveLight[i] < 0 )
        {
            break;
        }

        ++iNum;
    }

    return iNum;
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerLight::SetCustomLightStyle( DWORD dwStyle )
{
    m_dwCustomLightStyle = dwStyle;
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD CManagerLight::GetCustomLightStyle() const
{
    return m_dwCustomLightStyle; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerLight::TurnOnCustomLight( BOOL bOn )
{
    m_bUseCustomLight = bOn; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerLight::EnableAreaLight( BOOL bEnable )
{
    m_bEnableAreaLight = bEnable;
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerLight::IsAreaLight() const 
{
    return m_bEnableAreaLight;
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerLight::SetWorld( CWorldBase* pWorld )
{
    x_pWorld = pWorld;
}

#endif // _3DRENDERER_MANAGERLIGHT_H_