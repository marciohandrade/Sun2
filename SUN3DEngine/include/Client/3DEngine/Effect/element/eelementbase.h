// eelementbase.h

#ifndef _EFFECT_EELEMENTBASE_H_
#define _EFFECT_EELEMENTBASE_H_

// xxx: 외부에서 본 파일을 직접 include하는 경우가 있어
// typedef.h를 include했다.
#include "../typedef.h"

#include <client/3dengine/common/wzcolor.h>
#include <client/3dengine/common/boundingvolume.h>

#include "../createinfo/bodycreate.h"

class CEffectBody;

//------------------------------------------------------------------------------
/**
    @class CEElementBase
*/
class CEElementBase
{
#ifdef _DEBUG
public:
    int m_CreateFrom;
    int m_MemorySize;
#endif 

public:
    // 생성자/소멸자
    CEElementBase();
    virtual ~CEElementBase();

    // 초기화
    virtual void Clear();

    // 생성 정보 설정
    virtual void SetElementInfo( St_ElementCreate* pElement );
    // 부모 설정
    virtual void SetParentPtr( CEffectBody* pPtr );

    // 생성/반복
    virtual bool Create( bool bFirst );
    virtual void Loop();
    // 파괴
    virtual void Destroy();
    // 메모리 반납
    virtual void Free();

    // Move (시뮬레이션)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );
    // 렌더링
    virtual void Render();

    // 월드 값(위치, 회전, 크기) 설정 - 외부에서 호출
    virtual void SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale );
    virtual void SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot = NULL, const WzMatrix* pwmScale = NULL );

    // 대상 위치 설정
    virtual void SetTargetPos( const WzVector& wvPos );

    // 타입 얻기
    virtual ENUM_EELEMENT_TYPE GetType() const;

    // 태어났나?
    bool IsBorn() const;
    // 반복 타입?
    bool IsLoop() const;

    // 불투명도 설정
    void SetOpacity( float fOpacity );

    // 색상 설정/얻기
    void SetColor( WzColor c );
    WzColor GetColor() const;

    // 렌더링 여부
    void SetRender( bool b );
    bool GetRender() const;

    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray ) {}
    virtual void ReloadTexture() {}

protected:
    // 재 초기화
    virtual void Reset();
    // 태어날 때 처리
    virtual void OnBeingBorn();

protected:
    St_ElementCreate* m_ec;         // element 생성 정보
    CEffectBody* m_pParentPtr;      // 부모

    bool m_bBorn;                   // 태어났나?
    bool m_bReset;                  // 재 초기화? (반복 시작)

    DWORD m_dwMakeTime;             // 태어날 시간
    DWORD m_dwLife;                 // 수명 (태어난 후 살아갈 시간)
    DWORD m_dwRemainLife;           // 남은 수명
    DWORD m_dwExLife;               // 더 산 수명

    DWORD m_dwCurLife;              // 생성 후 시간
    DWORD m_dwBornCnt;              // 태어난 후 시간

    WzColor m_wcColor;              // 색상
    float m_fOpacity;               // 불 투명도

    bool m_bRender;                 // 렌더링 여부
};

//------------------------------------------------------------------------------
/**
*/
inline ENUM_EELEMENT_TYPE CEElementBase::GetType() const
{
    return EET_NONE;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::SetElementInfo( St_ElementCreate* pElement )
{
    WzAssert( pElement );

    // 생성 정보 세팅
    m_ec = pElement;

    // 예전에는 본 함수에서 본 element에 필요한 정보를 전부 복사 했었는데
    // 그 비용이 적지 않고 성능에도 크게 영향을 미치기 때문에 단순히 포인터
    // 대입으로 바꿨다.

    // 이로 인해, 본 element가 제거 되기 전까지 ElementCreate 정보가
    // 사라지면 안 된다. 주의!!
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::SetParentPtr( CEffectBody* pPtr )
{
    m_pParentPtr = pPtr;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::Loop()
{
    m_bReset = true;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::Reset()
{
    Create( false );
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::OnBeingBorn()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::Destroy()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::Free()
{
#ifdef _DEBUG
    m_CreateFrom = 0;
#endif
    Destroy();
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::Render()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot, const WzMatrix* pwmScale )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::SetTargetPos( const WzVector& wvPos )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline bool CEElementBase::IsBorn() const
{
    return m_bBorn;
}

//------------------------------------------------------------------------------
/**
*/
inline bool CEElementBase::IsLoop() const
{
    WzAssert( m_ec );

    return ( m_ec->m_pCreate->m_byLoopType != CV_LOOP_NO );
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::SetOpacity( float fOpacity )
{
    m_fOpacity = max( 0.0f, fOpacity );
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::SetColor( WzColor c )
{
    m_wcColor = c;
}

//------------------------------------------------------------------------------
/**
*/
inline WzColor CEElementBase::GetColor() const
{
    return m_wcColor;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEElementBase::SetRender( bool b )
{
    m_bRender = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool CEElementBase::GetRender() const
{
    return m_bRender;
}

#endif // _EFFECT_EELEMENTBASE_H_
