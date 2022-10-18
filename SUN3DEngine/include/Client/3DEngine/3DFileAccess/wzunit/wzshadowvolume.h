// wzshadowvolume.h

#ifndef _3DFILEACCESS_WZSHADOWVOLUME_H_
#define _3DFILEACCESS_WZSHADOWVOLUME_H_

#include "wzsilhouette.h"

#define SOP_ALLOW_FOR_ALPHAOBJECT   (0x00000001)

//------------------------------------------------------------------------------
/**
    @class CWzShadowVolume
*/
class CWzShadowVolume
{
public:
    // ������/�Ҹ���
    CWzShadowVolume();
    ~CWzShadowVolume();

    // ����
    BOOL Create( const WzVector& wvLightDir,
                 int iNumMeshGroup, 
                 WzMeshGroup* pMeshGroups, 
                 const WzVector* pwvTransformedVertices, 
                 DWORD dwOption = 0 );

    // �ı�
    void Destroy();

    // �Ƿ翧 ���μ� ���
    int GetNumSilouetteLine() const;
    // �Ƿ翧 ���� ����Ʈ ���
    St_SilhouetteLine* GetSilhouetteLineList() const;

    // ��� ���ؽ��� ���
    int GetNumVertexShadowVolume() const;
    // ��� ���� ���
    WzVector* GetVertexShadowVolumeBuffer() const;

private:
    // ��� ����
    void GenerateResult( const WzVector* pwvTransformedVertices );

private:
    CWzSilhouette m_silhouette;
    float m_fMaxZ;

    int m_iNumResultVertex;
    int m_iResultBufferSize;
    WzVector* m_pwvResultBuffer;
};

//------------------------------------------------------------------------------
/**
*/
inline int CWzShadowVolume::GetNumSilouetteLine() const
{
    return m_silhouette.m_iNumSilhouetteLine;
}

//------------------------------------------------------------------------------
/**
*/
inline St_SilhouetteLine* CWzShadowVolume::GetSilhouetteLineList() const
{
    return m_silhouette.m_pSilhouetteLineList; 
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzShadowVolume::GetNumVertexShadowVolume() const
{
    return m_iNumResultVertex; 
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector* CWzShadowVolume::GetVertexShadowVolumeBuffer() const
{
    return m_pwvResultBuffer;
}

#endif // _3DFILEACCESS_WZSHADOWVOLUME_H_
