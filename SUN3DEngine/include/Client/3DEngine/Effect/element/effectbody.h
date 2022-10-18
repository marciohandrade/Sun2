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
    // ������/�Ҹ���
    CEffectBody();
    virtual ~CEffectBody();

    // �ʱ�ȭ
    virtual void Clear();

    // ���� ���� ����
    virtual void SetElementInfo( St_ElementCreate* pElement );

    // ����
    virtual bool Create( bool bFirst );
    // �ı�
    virtual void Destroy();
    // �޸� �ݳ�
    virtual void Free();

    // �ڽ� ����
    bool CreateElement( St_ElementCreate* pec, int iRate = 100 );

    // Move (�ùķ��̼�)
    _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate, int iRate );
    // ������
    virtual void Render();

    // ���� ��ġ/ȸ��/ũ�� ����
    virtual void SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale );
    virtual void SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot = NULL, const WzMatrix* pwmScale = NULL);

    // Ÿ�� ���
    virtual ENUM_EELEMENT_TYPE GetType() const;

    // ���� ���� (�ܺο��� ���� �����ϴ� ���)
    void SetLifeInfo( DWORD dwMakeTime, DWORD dwLife );

    // Ÿ�� ��ġ ����
    WZEFF_API void SetAllElementTargetPos( const WzVector& wvPos );

    // BodyID ����/���
    void SetBodyID( WzID wzID );
    WzID GetBodyID() const;

    // ������ Ÿ�� ����
    void SetRenderType( ENUM_ALTERNATIVE_RENDER eRenderType );
    ENUM_ALTERNATIVE_RENDER GetRenderType() const;

    // ���ǵ� ����
    void SetSpeedOfSecond( float fSS );
    // ũ�� ���� ����
    void SetAccmulateScale( float fScale );
    float GetAccmulateScale();

    // ���̵� �ƿ� ȿ�� ���� ����/���
    void SetFadeOutEffect( bool bl );
    bool IsUseFade() const;

    // �ʴ� ���̵� �ƿ� �� ����
    void SetFadeOfSec( float fValue );
    // ���̵� �ƿ� ����
    void FadeOut();

    // ������ ����/���
    void SetRefractionRate( float fRefractionRate );
    float GetRefractionRate() const;

    // ���� ���� ����/���
    void SetRefractionAlpha( float fRefractionAlpha );
    float GetRefractionAlpha() const;

    // �ø� ���� ����/���
    void SetCulling( bool bCull );
    bool UseCulling() const;

    // ���̱�/���߱�
    void SetShow( bool b );
    bool IsShow() const;

    // �ܺο��� ������ ����
    void KillSelf();

    // FlatChain ���
    CEElementFlatChain* GetFlatChainPtr();

    // �⺻ �ٿ�� �ڽ� ����
    void SetBaseBBox( const BvAABB& bbox );

    // �ٿ�� �ڽ� ���
    const BvAABB& GetBBox();

    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray );

    virtual void ReloadTexture();

protected:
    // �ڽ� ����
    void FreeElement( CEElementBase* pElem );
    // ��� �ڽĵ� ����
    void DestroyAllElements();

    // Move (�ڽĵ�)
    void MoveElement( DWORD dwTick, int iRate );

    // �ٿ�� �ڽ� ����
    void UpdateBBox();

    // ���� ���� �߰�
    void AddSoundInfo( const St_ElementCreate* pElement, DWORD dwMakeTime );
    // ��� ���� ���� �ʱ�ȭ
    void InitAllSoundInfos();
    // ��� ���� ���� ����
    void FreeAllSoundInfos();

    // ���� ó��
    void ProcessSound();

protected:
    std::list<CEElementBase*> m_lstElement;
    std::vector<St_SoundInfo*> m_lstSndInfo;

    CEElementFlatChain* m_pFlatChain;

    ENUM_ALTERNATIVE_RENDER m_eRenderType;

    WzID m_wzBodyID;            // BodyID

    float m_fSpeedToSecond;     // �ʴ� ���ǵ�
    float m_fAccmulateScale;    // ũ�� ����

    bool m_bUseFade;            // ������ ���̵� ȿ��
    float m_fFadeOfSec;         // ���� ���� �ð�
    bool m_bFadeOut;            // ���̵� �ƿ� ����
    long m_lnCurFadeRemainTick; // ���� ���� ���̵� ���� Tick  

    float m_fRefractionRate;    // �ܰ� �� 0~1���� ��
    float m_fRefractionAlpha;   // �ܰ� ���ĵ� ( 0 - Diffuse 100%, 1 - �ܰ� 100% )

    bool m_bUseCull;            // �ø� ����
    bool m_bShow;               // ���̱� ����
    bool m_bKillSelf;           // �ڻ� ����

    BvAABB m_baseBBox;          // �⺻ �ٿ�� �ڽ�
    BvAABB m_BBox;              // ���� �ٿ�� �ڽ�
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