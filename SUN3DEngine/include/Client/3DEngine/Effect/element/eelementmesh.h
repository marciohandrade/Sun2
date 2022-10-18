// eelementmesh.h

#ifndef _EFFECT_EELEMENTMESH_H_
#define _EFFECT_EELEMENTMESH_H_

#include "eelementparticle.h"

class CWzUnitDraw;

//------------------------------------------------------------------------------
/**
    @class CEElementMesh
*/
class CEElementMesh : public CEElementParticle
{
public:
    // ������/�Ҹ���
    CEElementMesh();
    virtual ~CEElementMesh();

    // �ʱ�ȭ
    virtual void Clear();

    // ���� ���� ����
    virtual void SetElementInfo( St_ElementCreate* pElement );

    // ����/�ı�
    virtual bool Create( bool bFirst );
    virtual void Destroy();
    // �޸� �ݳ�
    virtual void Free();
    
    // ���� ��(��ġ, ȸ��, ũ��) ���� - �ܺο��� ȣ��
    virtual void SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale );
    virtual void SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot, const WzMatrix* pwmScale );

    // Move (�ùķ��̼�)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );
    // ������
    virtual void Render();

    // Ÿ�� ���
    virtual ENUM_EELEMENT_TYPE GetType() const;

protected:
    // ���� �̵�
    void MoveMesh( DWORD dwTick );

protected:
    St_ElementCreate_Mesh* m_pMesh;

    CWzUnitDraw* m_pUnitDraw;
    float m_fScale;
};

//------------------------------------------------------------------------------
/**
*/
inline ENUM_EELEMENT_TYPE CEElementMesh::GetType() const
{ 
    return EET_MESH;
}

#endif // _EFFECT_EELEMENTMESH_H_
