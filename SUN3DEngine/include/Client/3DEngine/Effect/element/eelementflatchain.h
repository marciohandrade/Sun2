// eelementflatchain.h

#ifndef _EFFECT_EELEMENTFLATCHAIN_H_
#define _EFFECT_EELEMENTFLATCHAIN_H_

#include "eelementbase.h"
#include <client/3dengine/common/wzuvvert.h>

// 플랫 체인 버텍스 구조체
struct FCVertex
{
    WzVector v;
    float t;
};

//------------------------------------------------------------------------------
/**
*/
class CEElementFlatChain : public CEElementBase  
{
public:
    // 생성자/소멸자
    CEElementFlatChain();
    virtual ~CEElementFlatChain();

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

    // 타입 얻기
    virtual ENUM_EELEMENT_TYPE GetType() const;

    // 수명 정보 설정
    WZEFF_API void SetLifeInfo( DWORD dwMakeTime, DWORD dwLife );
    // 꼬리 개수 설정
    WZEFF_API void SetTailNum( int nTail );
    // 위치 설정
    void SetPosition( const WzVector& wvPos1, const WzVector& wvPos2 );
    // 스플라인 보간 길이
    void SetSplineLength( float fLen );


    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray ) { HandleArray.push_back(m_hTexture); }


    virtual void ReloadTexture();

protected:
    // 버퍼 파괴
    void DestroyBuffers();

    // 머리 붙이기
    void GenerateHead();

protected:
    St_ElementCreate_FlatChain* m_pFlatChain;

    HANDLE m_hTexture;      // 텍스쳐 핸들

    int m_iNumTail;         // 최대 꼬리 개수
    int m_iCurNumTail;      // 현재 꼬리 개수

    FCVertex* m_pwvVertex;  // 정점 버퍼
    WzUVVert* m_puvVert;    // UV좌표 버퍼

    bool m_bInitBuf;

    WzVector m_wvCPos1;
    WzVector m_wvOldCPos1;

    WzVector m_wvCPos2;
    WzVector m_wvOldCPos2;

    float m_fMaxLen;
};

//------------------------------------------------------------------------------
/**
*/
inline ENUM_EELEMENT_TYPE CEElementFlatChain::GetType() const
{
    return EET_FLATCHAIN;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementFlatChain::SetSplineLength( float fLen )
{
    WzAssert( fLen > 0.0f );

    m_fMaxLen = fLen;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementFlatChain::SetPosition( const WzVector& wvPos1, const WzVector& wvPos2 )
{
    m_wvCPos1 = wvPos1;
    m_wvCPos2 = wvPos2;
}

#endif // _EFFECT_EELEMENTFLATCHAIN_H_
