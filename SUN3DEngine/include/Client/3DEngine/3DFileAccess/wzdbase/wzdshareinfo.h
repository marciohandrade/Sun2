// wzdshareinfo.h

#ifndef _3DFILEACCESS_WZDSHAREINFO_H_
#define _3DFILEACCESS_WZDSHAREINFO_H_

#include "wzdtypedef.h"

class CDrawBase;

//------------------------------------------------------------------------------
/**
    @class CWzdShareInfo

    - �޽ÿ� ���� ����
*/
class CWzdShareInfo
{
public:
    // �ν��Ͻ� ���
    static CWzdShareInfo* Instance();

    // ������ ����/���
    void SetRenderer( CDrawBase* pRenderer );
    CDrawBase* GetRenderer();

    // ī�޶� ���� �����
    void OnCameraDirChange();
    // ī�޶� ���� ��� (����: ī�޶� ���� ����)
    const WzVector& GetCameraDir() const;

    // �� ���� ����/��� (����: ���� ������ �ݴ� ����)
    void SetLightDir( const WzVector& dir );
    const WzVector& GetLightDir() const;

    // blinn ���� ���
    void GetBlinnVectors( WzVector pwvBlinnVectors[3] );

    // Brightness ���� ���� ����/���
    void SetBrightnessFunc( const float fFunc[3] );
    void GetBrightnessFunc( float fFunc[3] );

private:
    // ������/�Ҹ���
    CWzdShareInfo();
    ~CWzdShareInfo();

    // �ʱ�ȭ
    void Initialize();

    // blinn ���� ���
    void CalculateBlinnVector();

private:
    CDrawBase* m_pRenderer;

    float m_fBrightnessFunc[3];

    WzVector m_wvCamDir;    // ī�޶� ���� ����
    WzVector m_wvLightDir;  // ���� ������ �ݴ� ����

    WzVector m_wvBlinnVec;  // Blinn ����
    WzVector m_wvBlinnU;    // Blinn ���� ���� ����(u��)
    WzVector m_wvBlinnV;    // Blinn ���� ���� ����(v��)
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
    xxx: �� ������Ʈ���� ���ҽ� �ε��� �������� ������ ���� �־
    �ε��� ���⼭ �������� ���� ������, ���������� ���� ���ƺ����� �ʴ´�.
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
