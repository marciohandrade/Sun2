// eelementchain.h

#ifndef _EFFECT_EELEMENTCHAIN_H_
#define _EFFECT_EELEMENTCHAIN_H_

#include "eelementparticle.h"
#include <client/3dengine/common/wzuvvert.h>

//------------------------------------------------------------------------------
/**
    @class CEElementChain
*/
class CEElementChain : public CEElementParticle
{
public:
    // 생성자/소멸자
    CEElementChain();
    virtual ~CEElementChain();

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

    // 대상 위치 설정
    virtual void SetTargetPos( const WzVector& wvPos );

    // 타입 얻기
    virtual ENUM_EELEMENT_TYPE GetType() const;

    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray ) { HandleArray.push_back(m_hTexture); }

    virtual void ReloadTexture();

protected:
    // 타겟과 충돌 검사
    bool CheckCollWithTarget();
    // 머리 붙이기
    void GenerateHead();


protected:
    St_ElementCreate_Chain* m_pChain;

    HANDLE m_hTexture;          // 텍스쳐 핸들

    WzVector* m_pwvVertexTB;    // 위/아래 정점 버퍼
    WzVector* m_pwvVertexLR;    // 좌/우 정점 버퍼
    WzUVVert* m_puvVert;        // uv 좌표 버퍼

    WzVector m_wvOrtho[2];      // 수직 벡터
    WzVector m_wvTargetPos;     // 충돌 검사할 타겟위치
    
    int m_iCurNumTail;          // 현재 꼬리 개수
};

//------------------------------------------------------------------------------
/**
*/
inline ENUM_EELEMENT_TYPE CEElementChain::GetType() const
{
    return EET_CHAIN;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementChain::SetTargetPos( const WzVector& wvPos )
{
    m_wvTargetPos = wvPos;
}

#endif // _EFFECT_EELEMENTCHAIN_H_
