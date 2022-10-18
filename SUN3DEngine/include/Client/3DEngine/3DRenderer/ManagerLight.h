// managerlight.h

#ifndef _3DRENDERER_MANAGERLIGHT_H_
#define _3DRENDERER_MANAGERLIGHT_H_

#include "managerbase.h"

#define DEFAULT_LIGHT_RANGE         5.0f
#define DEFAULT_LIGHT_INTENSITY     3.0f

// Ŀ���� ����Ʈ ��Ÿ��
#define CLS_NONE                (0x00000000)   
#define CLS_LIGHT_COLOR         (0x00000001)    // ����Ʈ ��
#define CLS_LIGHT_RANGE         (0x00000002)    // ����
#define CLS_LIGHT_INTENSITY     (0x00000004)    // ����
#define CLS_LIGHT_DIRECTION     (0x00000008)    // Directional Light�� ����
#define CLS_LIGHT_POSITION      (0x00000008)    // Point Light�� ��ġ
#define CLS_LIGHT_ALL           (0xffffffff)    // ���� ���
#define CLS_DEFAULT             CLS_LIGHT_ALL

class CWorldBase;

//------------------------------------------------------------------------------
/**
    @class CManagerLight
*/
class CManagerLight : public CManagerBase
{
public:
    // ������/�Ҹ���
    CManagerLight();
    virtual ~CManagerLight();

    // ����/�ı�
    virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );
    virtual void Destroy();

    // �ʱ�ȭ
    virtual void Clear();

    // ���� ����Ʈ ����
    virtual BOOL SetDirectionalLight( int iIndex, 
                                      WzColor wcColor, 
                                      const WzVector& wvDirection, 
                                      ENUM_LIGHT_IRRA_TYPE eliType = 0,
                                      float fRange = DEFAULT_LIGHT_RANGE,
                                      float fIntensity = DEFAULT_LIGHT_INTENSITY );

    // ����Ʈ ����Ʈ ����
    virtual BOOL SetPointLight( int iIndex, 
                                WzColor wcColor, 
                                const WzVector& wvPosition,
                                ENUM_LIGHT_IRRA_TYPE eliType = 0,
                                float fRange = DEFAULT_LIGHT_RANGE,
                                float fIntensity = DEFAULT_LIGHT_INTENSITY );

    // ����Ʈ ���� ���
    virtual const St_LightInformation* GetLightInfo( int iIndex ) const;

    // ����Ʈ ���� ����/���
    BOOL SetLightIntensity( int iIndex, float fIntensity );
    float GetLightIntensity( int iIndex ) const;

    // ����Ʈ �ѱ�/����
    virtual BOOL TurnOnLight( int iIndex, BOOL bOn );

    // Ȱ�� ����Ʈ �ε���/���� ��� (iActive ��°)
    int GetIndexOfActiveLight( int iActive ) const;
    virtual const St_LightInformation* GetActiveLightInfo( int iActive );

    // iActive ��° ����Ʈ Ȱ�� ����
    BOOL IsActiveLight( int iActive ) const;
    
    // Ȱ�� ����Ʈ �� ���
    int GetNumberOfActiveLight() const;

    // Ŀ���� ���� ����Ʈ ����
    virtual BOOL SetCustomDirectionalLight( WzColor wcColor, 
                                            const WzVector& wvDirection,
                                            ENUM_LIGHT_IRRA_TYPE eliType = 0,
                                            float fRange = DEFAULT_LIGHT_RANGE,
                                            float fIntensity = DEFAULT_LIGHT_INTENSITY );

    // Ŀ���� ����Ʈ ����Ʈ ����
    virtual BOOL SetCustomPointLight( WzColor wcColor, 
                                      const WzVector& wvPosition,
                                      ENUM_LIGHT_IRRA_TYPE eliType = 0,
                                      float fRange = DEFAULT_LIGHT_RANGE,
                                      float fIntensity = DEFAULT_LIGHT_INTENSITY );

    // Ŀ���� ����Ʈ ���� ���
    virtual const St_LightInformation* GetCustomLightInfo( int iIndex );

    // Ŀ���� ����Ʈ ��Ÿ�� ����/���
    void SetCustomLightStyle( DWORD dwStyle );
    DWORD GetCustomLightStyle() const;

    // Ŀ���� ����Ʈ �ѱ�/����
    void TurnOnCustomLight( BOOL bOn );

    // irradiance ���
    virtual HANDLE GetIrradiance( ENUM_LIGHT_IRRA_TYPE iLit, ENUM_GROUP_MATERIAL_TYPE iGmt ) const;
    virtual HANDLE GetActiveIrradiance( int iActive, ENUM_GROUP_MATERIAL_TYPE iGmt );

    // irradiance ��ε�
    virtual BOOL ReloadIrradiance();

    // ���� ����Ʈ ��� ���� ����/���
    void EnableAreaLight( BOOL bEnable );
    BOOL IsAreaLight() const;

    // ����� ���� ����Ʈ ���
    virtual void FindAreaLight( WzBoundingVolume* bvLight, int* iIndex );  

    // ����Ʈ ����
    virtual void ApplyAreaLight( const int* iIndex );

    // ���� ����
    void SetWorld( CWorldBase* pWorld );

    // ���� �ε� ��� ����
    static void SetFilePath( const char* lpszPath );

protected:
    // �⺻ ����Ʈ ����
    virtual void SetDefaultLight();

    // software shading �� ���� �� ���� ����
    virtual void UpdateWzdLightInfo();

    // Ȱ�� ����Ʈ �ʱ�ȭ
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
    // xxx: ���� ȣȯ
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