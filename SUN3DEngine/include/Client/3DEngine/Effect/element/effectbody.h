// effectbody.h

#ifndef _EFFECT_EFFECTBODY_H_
#define _EFFECT_EFFECTBODY_H_

#include "eelementparticle.h"
#include <client/3dengine/3dfileaccess/wzunit/wzunitdraw.h>
#include <list>
#include <vector>
#include <algorithm>

class CEElementFlatChain;

//------------------------------------------------------------------------------
/**
    @class CEffectBody
*/
class CEffectBody : public CEElementParticle
{
public:
#ifdef _DEBUG
    int m_CreateFrom;

    virtual int GetAllocatedMemorySize();
#endif

public:
    // 생성자/소멸자
    CEffectBody();
    virtual ~CEffectBody();

    // 초기화
    virtual void Clear();

    // 생성 정보 설정
    virtual void SetElementInfo( St_ElementCreate* pElement );

    // 생성
    virtual bool Create( bool bFirst );
    // 파괴
    virtual void Destroy();
    // 메모리 반납
    virtual void Free();

    // 자식 생성
    bool CreateElement( St_ElementCreate* pec, int iRate = 100 );

    // Move (시뮬레이션)
    _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate, int iRate );
    // 랜더링
    virtual void Render();

    // 월드 위치/회전/크기 설정
    virtual void SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale );
    virtual void SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot = NULL, const WzMatrix* pwmScale = NULL);

    // 타입 얻기
    virtual ENUM_EELEMENT_TYPE GetType() const;

    // 수명 정보 (외부에서 강제 설정하는 경우)
    void SetLifeInfo( DWORD dwMakeTime, DWORD dwLife );

    // 타겟 위치 설정
    WZEFF_API void SetAllElementTargetPos( const WzVector& wvPos );

    // BodyID 설정/얻기
    void SetBodyID( WzID wzID );
    WzID GetBodyID() const;

    // 랜더링 타입 설정
    void SetRenderType( ENUM_ALTERNATIVE_RENDER eRenderType );
    ENUM_ALTERNATIVE_RENDER GetRenderType() const;

    // 스피드 설정
    void SetSpeedOfSecond( float fSS );
    // 크기 비율 설정
    void SetAccmulateScale( float fScale );
    float GetAccmulateScale();

    // 페이드 아웃 효과 적용 설정/얻기
    void SetFadeOutEffect( bool bl );
    bool IsUseFade() const;

    // 초당 페이드 아웃 값 설정
    void SetFadeOfSec( float fValue );
    // 페이드 아웃 시작
    void FadeOut();

    // 굴절율 설정/얻기
    void SetRefractionRate( float fRefractionRate );
    float GetRefractionRate() const;

    // 굴절 알파 설정/얻기
    void SetRefractionAlpha( float fRefractionAlpha );
    float GetRefractionAlpha() const;

    // 컬링 적용 설정/얻기
    void SetCulling( bool bCull );
    bool UseCulling() const;

    // 보이기/감추기
    void SetShow( bool b );
    bool IsShow() const;

    // 외부에서 강제로 죽임
    void KillSelf();

    // FlatChain 얻기
    CEElementFlatChain* GetFlatChainPtr();

    // 기본 바운딩 박스 설정
    void SetBaseBBox( const BvAABB& bbox );

    // 바운딩 박스 얻기
    const BvAABB& GetBBox();

    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray );

    virtual void ReloadTexture();

protected:
    // 자식 해제
    void FreeElement( CEElementBase* pElem );
    // 모든 자식들 제거
    void DestroyAllElements();

    // Move (자식들)
    void MoveElement( DWORD dwTick, int iRate );

    // 바운딩 박스 갱신
    void UpdateBBox();

    // 사운드 정보 추가
    void AddSoundInfo( const St_ElementCreate* pElement, DWORD dwMakeTime );
    // 모든 사운드 정보 초기화
    void InitAllSoundInfos();
    // 모든 사운드 정보 해제
    void FreeAllSoundInfos();

    // 사운드 처리
    void ProcessSound();

protected:
    std::list<CEElementBase*> m_lstElement;
    std::vector<St_SoundInfo*> m_lstSndInfo;

    CEElementFlatChain* m_pFlatChain;

    ENUM_ALTERNATIVE_RENDER m_eRenderType;

    WzID m_wzBodyID;            // BodyID

    float m_fSpeedToSecond;     // 초당 스피드
    float m_fAccmulateScale;    // 크기 비율

    bool m_bUseFade;            // 죽을때 페이드 효과
    float m_fFadeOfSec;         // 투명도 지속 시간
    bool m_bFadeOut;            // 페이드 아웃 시작
    long m_lnCurFadeRemainTick; // 현재 남은 페이드 지속 Tick  

    float m_fRefractionRate;    // 외곡 율 0~1사이 값
    float m_fRefractionAlpha;   // 외곡 알파도 ( 0 - Diffuse 100%, 1 - 외곡 100% )

    bool m_bUseCull;            // 컬링 여부
    bool m_bShow;               // 보이기 여부
    bool m_bKillSelf;           // 자살 여부

    BvAABB m_baseBBox;          // 기본 바운딩 박스
    BvAABB m_BBox;              // 현재 바운딩 박스
};

//------------------------------------------------------------------------------
/**
*/
inline ENUM_EELEMENT_TYPE CEffectBody::GetType() const
{
    return EET_BODY;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::SetBodyID( WzID wzID )
{
    m_wzBodyID = wzID;
}

//------------------------------------------------------------------------------
/**
*/
inline WzID CEffectBody::GetBodyID() const
{
    return m_wzBodyID;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::SetRenderType( ENUM_ALTERNATIVE_RENDER eRenderType )
{
    m_eRenderType = eRenderType;
}

//------------------------------------------------------------------------------
/**
*/
inline ENUM_ALTERNATIVE_RENDER CEffectBody::GetRenderType() const
{
    return m_eRenderType;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::SetSpeedOfSecond( float fSS )
{
    m_fSpeedToSecond = fSS;
}

//------------------------------------------------------------------------------
/**
*/
inline float CEffectBody::GetAccmulateScale()
{ 
    return m_fAccmulateScale;
}
//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::SetAccmulateScale( float fScale )
{ 
    m_fAccmulateScale = fScale;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::SetFadeOutEffect( bool bl )
{
    m_bUseFade = bl;
}

//------------------------------------------------------------------------------
/**
*/
inline bool CEffectBody::IsUseFade() const
{
    return m_bUseFade;
}

//------------------------------------------------------------------------------
/**.
*/
inline void CEffectBody::SetFadeOfSec( float fValue )
{
    m_fFadeOfSec = fValue;

    m_lnCurFadeRemainTick = (long)( 1000.0f * m_fFadeOfSec );
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::FadeOut()
{
    m_bFadeOut = true;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::SetRefractionRate( float fRefractionRate )
{
    m_fRefractionRate = fRefractionRate;
}

//------------------------------------------------------------------------------
/**
*/
inline float CEffectBody::GetRefractionRate() const
{
    return m_fRefractionRate;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::SetRefractionAlpha( float fRefractionAlpha )
{
    m_fRefractionAlpha = fRefractionAlpha;
}

//------------------------------------------------------------------------------
/**
*/
inline float CEffectBody::GetRefractionAlpha() const
{
    return m_fRefractionAlpha;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::SetCulling( bool bCull )
{ 
    m_bUseCull = bCull;
}

//------------------------------------------------------------------------------
/**
*/
inline bool CEffectBody::UseCulling() const
{
    return m_bUseCull;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::SetShow( bool b )
{
    m_bShow = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool CEffectBody::IsShow() const
{
    return m_bShow;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::KillSelf()
{
    m_bKillSelf = true;
}

//------------------------------------------------------------------------------
/**
*/
inline CEElementFlatChain* CEffectBody::GetFlatChainPtr()
{
    return m_pFlatChain;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::SetBaseBBox( const BvAABB& bbox )
{
    m_baseBBox = bbox;
}

//------------------------------------------------------------------------------
/**
*/
inline void CEffectBody::UpdateBBox()
{
    m_BBox.m_wvMin = m_wvRealPos + m_baseBBox.m_wvMin;
    m_BBox.m_wvMax = m_wvRealPos + m_baseBBox.m_wvMax;
}

//------------------------------------------------------------------------------
/**
*/
inline const BvAABB& CEffectBody::GetBBox()
{
    return m_BBox;
}

#endif // _EFFECT_EFFECTBODY_H_