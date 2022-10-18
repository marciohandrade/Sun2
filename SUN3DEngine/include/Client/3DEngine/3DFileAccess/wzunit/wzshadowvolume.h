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
    // 생성자/소멸자
    CWzShadowVolume();
    ~CWzShadowVolume();

    // 생성
    BOOL Create( const WzVector& wvLightDir,
                 int iNumMeshGroup, 
                 WzMeshGroup* pMeshGroups, 
                 const WzVector* pwvTransformedVertices, 
                 DWORD dwOption = 0 );

    // 파괴
    void Destroy();

    // 실루엣 라인수 얻기
    int GetNumSilouetteLine() const;
    // 실루엣 라인 리스트 얻기
    St_SilhouetteLine* GetSilhouetteLineList() const;

    // 결과 버텍스수 얻기
    int GetNumVertexShadowVolume() const;
    // 결과 버퍼 얻기
    WzVector* GetVertexShadowVolumeBuffer() const;

private:
    // 결과 생성
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
