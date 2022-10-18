// wzdmeshsorter.h

#ifndef _3DFILEACCESS_WZDMESHSORTER_H_
#define _3DFILEACCESS_WZDMESHSORTER_H_

#include "wzdtypedef.h"

class CWzdMesh;

//------------------------------------------------------------------------------
/**
    @class CWzdMeshSorter
*/
class CWzdMeshSorter
{
public:
    // 생성자/소멸자
    CWzdMeshSorter();
    ~CWzdMeshSorter();

    // 메시 소팅
    void Sort( CWzdMesh* pMesh );

private:
    // 본 연결 정보 초기화
    BOOL InitBoneConnection( St_BoneConnection* pBC );
    // 메시 그룹별 본 연관 정보 설정
    BOOL InitBoneRelation( St_BoneConnection* pBC, int iGroup );
    // 메시 그룹별 본 정렬
    BOOL SortBones( int* pResult, St_BoneConnection* pBC );
    // 메시 그룹내 삼각형 정렬
    void SortFaces( int iGroup, const int* piBoneList );

    // 해당 본 추가시 관련 본들 비용 감소
    void DecreaseCost( St_BoneConnection* pBC, int iBone );
    // 가장 많이 연관된 본 얻기
    int GetMaxRelatedBone( const St_BoneConnection* pBC, const BOOL* pbChecked );

    // 본 추가 가능 여부
    BOOL CanAddBone( int iBone,
                     const St_BoneConnection* pBC,
                     const BOOL* pbChecked, 
                     const BOOL* pbAdded, 
                     int nCurCnt );

    // 해당 본 부모/자식 중 적은 비용 본 얻기
    int GetMinCostBone( const St_BoneConnection* pBC, int iBone );

    // 삼각형 추가 가능 여부
    BOOL CanAddFace( int iGroup, const WzFace* pFace, const BOOL* pBoneSet );

private:
    CWzdMesh* m_pMesh;
};

#endif // _3DFILEACCESS_WZDMESHSORTER_H_
