// wzsilhouette.h

#ifndef _3DFILEACCESS_WZSILHOUETTE_H_
#define _3DFILEACCESS_WZSILHOUETTE_H_

struct WzMeshGroup;
struct WzFace;

struct St_SilhouetteLine
{
    DWORD m_dwVertex[2];
    short m_nMeshGroup;
};

//------------------------------------------------------------------------------
/**
    @class CWzSilhouette
*/
class CWzSilhouette
{
public:
    // 생성자/소멸자
    CWzSilhouette();
    ~CWzSilhouette();

    // 생성
    BOOL Create( const WzVector& wvRay, 
                 int iNumMeshGroup, 
                 WzMeshGroup* pMeshGroups, 
                 const WzVector* pwvTransformedVertices );

    // 파괴
    void Destroy();

    // 실루엣 라인수 얻기
    int GetNumSilouetteLine() const;
    // 실루엣 라인 리스트 얻기
    St_SilhouetteLine* GetSilhouetteLineList() const;

    // 결과 버텍스수 얻기
    int GetNumResultVertex() const;
    // 결과 버퍼 얻기
    WzVector* GetResultBuffer() const;

private:
    // 실루엣 따기
    void DetermineSilhouette( WzMeshGroup* pMeshGroups, 
                              int nMeshGroup, 
                              const WzVector* pwvTransformedVertices,
                              int iNumFace,
                              WzFace* pwfFaces,
                              float* fMaxZ = NULL );

    // 결과 생성
    void GenerateResult( const WzVector* pwvTransformedVertices );

    // 실루엣 라인을 위한 메모리 확보
    void MakeSureSilhouetteLineBuffer( int iSize );

private:
    friend class CWzShadowVolume;

    WzVector m_wvRay;

    int m_iNumSilhouetteLine;
    int m_iSilhouetteLineListSize;
    St_SilhouetteLine* m_pSilhouetteLineList;

    int m_iNumResultVertex;
    int m_iResultBufferSize;
    WzVector* m_pwvResultBuffer;
};

//------------------------------------------------------------------------------
/**
*/
inline int CWzSilhouette::GetNumSilouetteLine() const
{
    return m_iNumSilhouetteLine;
}

//------------------------------------------------------------------------------
/**
*/
inline St_SilhouetteLine* CWzSilhouette::GetSilhouetteLineList() const
{
    return m_pSilhouetteLineList; 
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzSilhouette::GetNumResultVertex() const
{
    return m_iNumResultVertex; 
}

//------------------------------------------------------------------------------
/**
*/
inline WzVector* CWzSilhouette::GetResultBuffer() const
{
    return m_pwvResultBuffer;
}

#endif // _3DFILEACCESS_WZSILHOUETTE_H_
