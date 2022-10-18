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
    // 생성자/소멸자
    CWzdMeshConstructor();
    virtual ~CWzdMeshConstructor();

    // 파괴
    virtual void Destroy();

    // 메시 할당
    virtual BOOL AllocForMesh();

    // 삼각형 추가
    virtual void AddTriangle( const char* lpszTexture, 
                              const WzSkinningInfo wsi[3],
                              const WzVector wvPosWorld[3],
                              const WzVector wvNormal[3],
                              const WzUVVert wuvVert[3],
                              const char* lpszSpecularTexture, 
                              const DWORD dwColor[3], 
                              const DWORD dwColor2[3] );

    // 버텍스 수 얻기
    virtual int GetNumberOfVertex() const;
    // 버텍스 리스트 얻기
    virtual WzMeshVertex* GetVertexList() const;

    // 전체 메시 그룹 수 얻기
    virtual int GetNumberOfMeshGroup() const;
    virtual int GetMaxMeshGroup() const;

    // 메시 그룹 정보 얻기
    virtual WzMeshGroup* GetMeshGroupInfo( int iMeshGroup ) const;

    // 전체 버텍스팩 수 얻기
    virtual DWORD GetTotalVertPack() const;

    // 블랜딩 수 얻기
    virtual int GetBlendedCount() const;
    // 스키닝 정보 얻기
    virtual WzSkinningInfo* GetSkinningInfo() const;

protected:
    // 버텍스 인덱스 얻기
    DWORD GetIndexOfVertex( int iBone, const WzVector& wvPosWorld );
    // 버텍스 버퍼 확보
    void MakeSureSizeOfVertices( int iIndex );

    // 메시 그룹 인덱스 얻기
    int GetMeshGroupOfTexture( const char* lpszTexture, const char* lpszSpecularTexture );
    // 메시 그룹 추가
    void AddMeshGroup( int iMeshGroup, const char* lpszTexture, const char* lpszSpecularTexture );
    // 메시 그룹 버퍼 확보
    void MakeSureMeshGroupSize( int iIndex );

    // 버텍스팩 인덱스 얻기
    DWORD GetIndexOfVertPack( WzMeshGroup* pCurMeshGroup, const WzMeshVertPack* pwmAddVertPack );
    // 같은 버텍스팩인가?
    BOOL IsSameVertPack( const WzMeshVertPack* pwmVP1, const WzMeshVertPack* pwmVP2 ) const;

    // 버텍스팩 버퍼 확보
    void MakeSureSizeOfVertPack( WzMeshGroup* pCurMeshGroup, int iIndex );

    // 인접 삼각형 찾기
    void GenerateNearFaceInfo();
    // 해당 방향 인접 삼각형 설정
    void SetNearFaceOfTheDirection( int iMeshGroup, int iFace, int iDir );
    // 메시 그룹내 삼각형 버퍼 확보
    void MakeSureSizeOfFace( WzMeshGroup* pMeshGroup, int iIndex );

    // 스키닝 인덱스 얻기
    int GetSkinningIndex( const WzSkinningInfo* pwsi );
    // 스키닝 정보 버퍼 확보
    void MakeSureSIListSize( int iIndex );

    // 메시 그룹 해제
    void ReleaseMeshGroup();

private:
    // 변수 초기화 (가상 함수로 선언하지 말 것)
    void InitVariables();

    // 메모리 해제
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