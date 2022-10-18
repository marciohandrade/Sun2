// wzdmeshconstructor.h

#ifndef _3DFILEACCESS_WZDMESHCONSTRUCTOR_H_
#define _3DFILEACCESS_WZDMESHCONSTRUCTOR_H_

#include "wzdanimation.h"

//------------------------------------------------------------------------------
/**
    @class CWzdMeshConstructor
*/
class CWzdMeshConstructor : public CWzdAnimation
{
public:
    // ������/�Ҹ���
    CWzdMeshConstructor();
    virtual ~CWzdMeshConstructor();

    // �ı�
    virtual void Destroy();

    // �޽� �Ҵ�
    virtual BOOL AllocForMesh();

    // �ﰢ�� �߰�
    virtual void AddTriangle( const char* lpszTexture, 
                              const WzSkinningInfo wsi[3],
                              const WzVector wvPosWorld[3],
                              const WzVector wvNormal[3],
                              const WzUVVert wuvVert[3],
                              const char* lpszSpecularTexture, 
                              const DWORD dwColor[3], 
                              const DWORD dwColor2[3] );

    // ���ؽ� �� ���
    virtual int GetNumberOfVertex() const;
    // ���ؽ� ����Ʈ ���
    virtual WzMeshVertex* GetVertexList() const;

    // ��ü �޽� �׷� �� ���
    virtual int GetNumberOfMeshGroup() const;
    virtual int GetMaxMeshGroup() const;

    // �޽� �׷� ���� ���
    virtual WzMeshGroup* GetMeshGroupInfo( int iMeshGroup ) const;

    // ��ü ���ؽ��� �� ���
    virtual DWORD GetTotalVertPack() const;

    // ���� �� ���
    virtual int GetBlendedCount() const;
    // ��Ű�� ���� ���
    virtual WzSkinningInfo* GetSkinningInfo() const;

protected:
    // ���ؽ� �ε��� ���
    DWORD GetIndexOfVertex( int iBone, const WzVector& wvPosWorld );
    // ���ؽ� ���� Ȯ��
    void MakeSureSizeOfVertices( int iIndex );

    // �޽� �׷� �ε��� ���
    int GetMeshGroupOfTexture( const char* lpszTexture, const char* lpszSpecularTexture );
    // �޽� �׷� �߰�
    void AddMeshGroup( int iMeshGroup, const char* lpszTexture, const char* lpszSpecularTexture );
    // �޽� �׷� ���� Ȯ��
    void MakeSureMeshGroupSize( int iIndex );

    // ���ؽ��� �ε��� ���
    DWORD GetIndexOfVertPack( WzMeshGroup* pCurMeshGroup, const WzMeshVertPack* pwmAddVertPack );
    // ���� ���ؽ����ΰ�?
    BOOL IsSameVertPack( const WzMeshVertPack* pwmVP1, const WzMeshVertPack* pwmVP2 ) const;

    // ���ؽ��� ���� Ȯ��
    void MakeSureSizeOfVertPack( WzMeshGroup* pCurMeshGroup, int iIndex );

    // ���� �ﰢ�� ã��
    void GenerateNearFaceInfo();
    // �ش� ���� ���� �ﰢ�� ����
    void SetNearFaceOfTheDirection( int iMeshGroup, int iFace, int iDir );
    // �޽� �׷쳻 �ﰢ�� ���� Ȯ��
    void MakeSureSizeOfFace( WzMeshGroup* pMeshGroup, int iIndex );

    // ��Ű�� �ε��� ���
    int GetSkinningIndex( const WzSkinningInfo* pwsi );
    // ��Ű�� ���� ���� Ȯ��
    void MakeSureSIListSize( int iIndex );

    // �޽� �׷� ����
    void ReleaseMeshGroup();

private:
    // ���� �ʱ�ȭ (���� �Լ��� �������� �� ��)
    void InitVariables();

    // �޸� ����
    void Release();

protected:
    WzMeshVertex* m_pwmvVertices;
    WzMeshGroup* m_pwmgMeshGroup;
    WzSkinningInfo* m_pwsiList;

    int m_iMaxVertex;
    int m_iNumVertex;

    int m_iMaxMeshGroup;
    int m_iNumMeshGroup;

    DWORD m_dwTotalVertPack;

    int m_iMaxSkinningInfo;
    int m_iNumSkinningInfo;
    int m_iMaxBlended;
};

//------------------------------------------------------------------------------
/**
*/
inline int CWzdMeshConstructor::GetNumberOfVertex() const
{
    return m_iNumVertex;
}

//------------------------------------------------------------------------------
/**
*/
inline WzMeshVertex* CWzdMeshConstructor::GetVertexList() const
{
    return m_pwmvVertices;
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzdMeshConstructor::GetNumberOfMeshGroup() const
{
    return m_iNumMeshGroup;
}
//------------------------------------------------------------------------------ 
inline int CWzdMeshConstructor::GetMaxMeshGroup() const
{
    return m_iMaxMeshGroup;
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD CWzdMeshConstructor::GetTotalVertPack() const
{
    return m_dwTotalVertPack; 
}

//------------------------------------------------------------------------------
/**
*/
inline int CWzdMeshConstructor::GetBlendedCount() const
{
    return m_iMaxBlended; 
}

//------------------------------------------------------------------------------
/**
*/
inline WzSkinningInfo* CWzdMeshConstructor::GetSkinningInfo() const
{
    return m_pwsiList;
}

#endif // _3DFILEACCESS_WZDMESHCONSTRUCTOR_H_