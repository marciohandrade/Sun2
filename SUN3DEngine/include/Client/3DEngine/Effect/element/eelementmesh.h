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
    // 생성자/소멸자
    CEElementMesh();
    virtual ~CEElementMesh();

    // 초기화
    virtual void Clear();

    // 생성 정보 설정
    virtual void SetElementInfo( St_ElementCreate* pElement );

    // 생성/파괴
    virtual bool Create( bool bFirst );
    virtual void Destroy();
    // 메모리 반납
    virtual void Free();
    
    // 월드 값(위치, 회전, 크기) 설정 - 외부에서 호출
    virtual void SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale );
    virtual void SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot, const WzMatrix* pwmScale );

    // Move (시뮬레이션)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );
    // 렌더링
    virtual void Render();

    // 타입 얻기
    virtual ENUM_EELEMENT_TYPE GetType() const;

protected:
    // 유닛 이동
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
