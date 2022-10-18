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
    // ������/�Ҹ���
    CWzdMeshSorter();
    ~CWzdMeshSorter();

    // �޽� ����
    void Sort( CWzdMesh* pMesh );

private:
    // �� ���� ���� �ʱ�ȭ
    BOOL InitBoneConnection( St_BoneConnection* pBC );
    // �޽� �׷캰 �� ���� ���� ����
    BOOL InitBoneRelation( St_BoneConnection* pBC, int iGroup );
    // �޽� �׷캰 �� ����
    BOOL SortBones( int* pResult, St_BoneConnection* pBC );
    // �޽� �׷쳻 �ﰢ�� ����
    void SortFaces( int iGroup, const int* piBoneList );

    // �ش� �� �߰��� ���� ���� ��� ����
    void DecreaseCost( St_BoneConnection* pBC, int iBone );
    // ���� ���� ������ �� ���
    int GetMaxRelatedBone( const St_BoneConnection* pBC, const BOOL* pbChecked );

    // �� �߰� ���� ����
    BOOL CanAddBone( int iBone,
                     const St_BoneConnection* pBC,
                     const BOOL* pbChecked, 
                     const BOOL* pbAdded, 
                     int nCurCnt );

    // �ش� �� �θ�/�ڽ� �� ���� ��� �� ���
    int GetMinCostBone( const St_BoneConnection* pBC, int iBone );

    // �ﰢ�� �߰� ���� ����
    BOOL CanAddFace( int iGroup, const WzFace* pFace, const BOOL* pBoneSet );

private:
    CWzdMesh* m_pMesh;
};

#endif // _3DFILEACCESS_WZDMESHSORTER_H_
