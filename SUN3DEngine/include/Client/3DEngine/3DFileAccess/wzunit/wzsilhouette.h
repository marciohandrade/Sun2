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
    // ������/�Ҹ���
    CWzSilhouette();
    ~CWzSilhouette();

    // ����
    BOOL Create( const WzVector& wvRay, 
                 int iNumMeshGroup, 
                 WzMeshGroup* pMeshGroups, 
                 const WzVector* pwvTransformedVertices );

    // �ı�
    void Destroy();

    // �Ƿ翧 ���μ� ���
    int GetNumSilouetteLine() const;
    // �Ƿ翧 ���� ����Ʈ ���
    St_SilhouetteLine* GetSilhouetteLineList() const;

    // ��� ���ؽ��� ���
    int GetNumResultVertex() const;
    // ��� ���� ���
    WzVector* GetResultBuffer() const;

private:
    // �Ƿ翧 ����
    void DetermineSilhouette( WzMeshGroup* pMeshGroups, 
                              int nMeshGroup, 
                              const WzVector* pwvTransformedVertices,
                              int iNumFace,
                              WzFace* pwfFaces,
                              float* fMaxZ = NULL );

    // ��� ����
    void GenerateResult( const WzVector* pwvTransformedVertices );

    // �Ƿ翧 ������ ���� �޸� Ȯ��
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
