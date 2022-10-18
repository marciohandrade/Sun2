// managerunitlod.h

#ifndef _3DRENDERER_MANAGERUNITLOD_H_
#define _3DRENDERER_MANAGERUNITLOD_H_

#include "managerbase.h"
#include <programcommon/wzflag.h>

// ���� LOD �ɼ�
#define ULO_ENABLE_UNITLOD          (0x00000001)    // ���� LOD ����
#define ULO_USE_UNITSUBTYPE_LOD     (0x00000002)    // ������ ���� Ÿ�� LOD ����
#define ULO_ENABLE_LODSKINNING      (0x00000004)    // �Ÿ��� ���� ��Ű�� ����
#define ULO_ENABLE_LODNORMALMAP     (0x00000008)    // �Ÿ��� ���� �븻�� ����
#define ULO_ENABLE_LODUNITFADE      (0x00000010)    // �Ÿ��� ���� ���̵� ����
#define ULO_AFFECTED_BY_UNITSIZE    (0x00000020)    // �Ÿ� ���� ���� ũ�� �ݿ�

//------------------------------------------------------------------------------
/**
    @class CManagerUnitLOD

    * LOD �Ÿ� ����
    - ������ ī�޶�� ������� ���� �־��� �� �Ÿ��� �ٸ��� ����(���� ����)
    - ������ ���� ����(ī�޶�� ����� ����) �� ����Ƽ

                 0����        1����      2����      3����
                         [1]         [3]        [5]
                 --->     |           |          |
        ī�޶� ---------|-|---------|-|--------|-|---- ����
                 <---   |           |          |
                       [0]         [2]        [4]

        * ���ְ� ī�޶� �־��� ���� �� �Ÿ�(1, 3, 5)���� �־����� ������ �ٲ��
          �ݴ�� ������� ���� �Ʒ� �Ÿ�(0, 2, 4)���� ��������� ������ �ٲ�
*/
class CManagerUnitLOD : public CManagerBase
{
public:
    // ������/�Ҹ���
    CManagerUnitLOD();
    virtual ~CManagerUnitLOD();

    // �ı�
    virtual void Destroy();

    // xxx: ���� ȣȯ (���� ����)
    WZRND_API void UpdateUnitLOD( CWzUnitDraw* pUnitDraw );

    // �Ÿ� ����/���
    WZRND_API void SetDistanceOfLOD( float* apfDist, int iNumDist );
    float* GetDistanceOfLOD();
    // �Ÿ� �� ���
    int GetNumberOfDistance() const;

    // �Ÿ��� ���� ��Ű�� ���� ���� ����
    void AutoInvalidSkinning( BOOL bUse, float* apfDist = NULL );
    float* GetSkinningDistance();
    BOOL IsSkinningLOD() const;

    // �Ÿ��� ���� �븻�� ���� ���� ����
    void AutoInvalidNormalMap( BOOL bUse, float* apfDist = NULL );
    float* GetNormalMapDistance();
    BOOL IsNormalMapLOD() const;

    // �Ÿ��� ���� ���̵� ȿ�� ���� ���� ����
    void AutoUnitFade( BOOL bUse, float* apfDist = NULL );
    float* GetFadeDistance();
    BOOL IsEnableUnitFade() const;

    // ���� LOD Ȱ�� ���� ����/���
    void EnableUnitLOD( BOOL bEnable );
    BOOL IsUnitLOD() const;

    // ������ ���� Ÿ�� LOD ���� ���� ����/���
    void EnableUnitSubTypeLOD( BOOL bEnable );
    BOOL IsUnitSubTypeLOD() const;

    // ���� ũ�⿡ ���� �Ÿ� ��� ���� ����/���
    void EnableAffectedByUnitSize( BOOL bEnable );
    BOOL IsAffectedByUnitSize() const;

    // �⺻ ���� ũ�� ����/��� (���� ũ�⿡ ���� �Ÿ� ���� ���)
    void SetDefaultUnitSize( float fSize );
    float GetDefaultUnitSize() const;

private:
    // �ʱ�ȭ
    void Initialize();
    // ����
    void Release();

protected:
    CWzFlag<DWORD> m_opt;

    float* m_apfLODDist;
    int m_iNumLODDist;

    float m_fSkinningDist[2];
    float m_fNormalMapDist[2];
    float m_fFadeDist[2];

    float m_fDefUnitSize;
};

//------------------------------------------------------------------------------
/**
*/
inline float* CManagerUnitLOD::GetDistanceOfLOD()
{
    return m_apfLODDist;
}

//------------------------------------------------------------------------------
/**
*/
inline int CManagerUnitLOD::GetNumberOfDistance() const
{
    return m_iNumLODDist;
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::AutoInvalidSkinning( BOOL bUse, float* apfDist )
{
    if( bUse )
    {
        m_opt.Set( ULO_ENABLE_LODSKINNING );

        if( apfDist )
        {
            memcpy( m_fSkinningDist, apfDist, sizeof( float ) * 2 );
        }
    }
    else
    {
        m_opt.Unset( ULO_ENABLE_LODSKINNING );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline float* CManagerUnitLOD::GetSkinningDistance()
{
    return m_fSkinningDist; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsSkinningLOD() const
{
    return m_opt.Check( ULO_ENABLE_LODSKINNING ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::AutoInvalidNormalMap( BOOL bUse, float* apfDist )
{
    if( bUse )
    {
        m_opt.Set( ULO_ENABLE_LODNORMALMAP );

        if( apfDist )
        {
            memcpy( m_fNormalMapDist, apfDist, sizeof( float ) * 2 );
        }
    }
    else
    {
        m_opt.Unset( ULO_ENABLE_LODNORMALMAP );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline float* CManagerUnitLOD::GetNormalMapDistance()
{
    return m_fNormalMapDist; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsNormalMapLOD() const
{
    return m_opt.Check( ULO_ENABLE_LODNORMALMAP ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::AutoUnitFade( BOOL bUse, float* apfDist )
{
    if( bUse )
    {
        m_opt.Set( ULO_ENABLE_LODUNITFADE );

        if( apfDist )
        {
            memcpy( m_fFadeDist, apfDist, sizeof( float ) * 2 );
        }
    }
    else
    {
        m_opt.Unset( ULO_ENABLE_LODUNITFADE );
    }
}

//------------------------------------------------------------------------------
/**
*/
inline float* CManagerUnitLOD::GetFadeDistance()
{
    return m_fFadeDist; 
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsEnableUnitFade() const
{
    return m_opt.Check( ULO_ENABLE_LODUNITFADE );; 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::EnableUnitLOD( BOOL bEnable )
{
    m_opt.SetB( ULO_ENABLE_UNITLOD, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsUnitLOD() const
{
    return m_opt.Check( ULO_ENABLE_UNITLOD );
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::EnableUnitSubTypeLOD( BOOL bEnable )
{
    m_opt.SetB( ULO_USE_UNITSUBTYPE_LOD, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsUnitSubTypeLOD() const
{
    return m_opt.Check( ULO_USE_UNITSUBTYPE_LOD ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::EnableAffectedByUnitSize( BOOL bEnable )
{
    m_opt.SetB( ULO_AFFECTED_BY_UNITSIZE, bEnable );
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CManagerUnitLOD::IsAffectedByUnitSize() const
{
    return m_opt.Check( ULO_AFFECTED_BY_UNITSIZE ); 
}

//------------------------------------------------------------------------------
/**
*/
inline void CManagerUnitLOD::SetDefaultUnitSize( float fSize )
{
    WzAssert( fSize > 0.0f );

    m_fDefUnitSize = max( fSize, 0.00001f ); 
}

//------------------------------------------------------------------------------
/**
*/
inline float CManagerUnitLOD::GetDefaultUnitSize() const
{
    return m_fDefUnitSize; 
}

#endif // _3DRENDERER_MANAGERUNITLOD_H_