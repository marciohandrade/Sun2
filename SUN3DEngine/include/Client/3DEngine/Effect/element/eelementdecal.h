// eelementdecal.h

#ifndef _EFFECT_EELEMENTDECAL_H_
#define _EFFECT_EELEMENTDECAL_H_

#include "eelementbase.h"
#include <client/3dengine/common/wzsimplemesh.h>

//------------------------------------------------------------------------------
/**
    @class CEElementDecal
*/
class CEElementDecal : public CEElementBase
{
public:
    // 생성자/소멸자
    CEElementDecal();
    virtual ~CEElementDecal();

    // 초기화
    virtual void Clear();

    // 생성 정보 설정
    virtual void SetElementInfo( St_ElementCreate* pElement );

    // 생성/파괴
    virtual bool Create( bool bFirst );
    virtual void Destroy();
    // 메모리 반납
    virtual void Free();

    // Move (시뮬레이션)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );
    // 렌더링
    virtual void Render();

    // 월드 값 설정
    virtual void SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale );
    virtual void SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot = NULL, const WzMatrix* pwmScale = NULL );

    // 타입 얻기
    virtual ENUM_EELEMENT_TYPE GetType() const;

    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray );

    virtual void ReloadTexture();

protected:
    // 태어날 때 처리
    virtual void OnBeingBorn();

    // 전부 계산
    void CalculateAll( DWORD dwTime );

    // 메시 생성
    bool GenerateMesh();
    // 메시 제거
    void DestroyMesh();

protected:
    St_ElementCreate_Decal* m_pDecal;

    WzSimpleMesh* m_pMesh;

    float m_fScale;
    float m_fPrevScale;

    float m_fRotZ;
    float m_fPrevRotZ;

    WzVector m_wvRealPos;
    WzVector m_wvPrevRealPos;

    WzVector m_wvLocalPos;
    WzVector m_wvPrevParentPos;

    float m_fScaleW;
    WzMatrix m_wmRotW;
    WzVector m_wvPosW;

    bool m_bSetResultMat;
};

//------------------------------------------------------------------------------
/**
*/
inline ENUM_EELEMENT_TYPE CEElementDecal::GetType() const
{
    return EET_DECAL;
}

#endif // _EFFECT_EELEMENTDECAL_H_
