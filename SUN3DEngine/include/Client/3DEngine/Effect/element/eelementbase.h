// eelementbase.h

#ifndef _EFFECT_EELEMENTBASE_H_
#define _EFFECT_EELEMENTBASE_H_

// xxx: �ܺο��� �� ������ ���� include�ϴ� ��찡 �־�
// typedef.h�� include�ߴ�.
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
    // ������/�Ҹ���
    CEElementBase();
    virtual ~CEElementBase();

    // �ʱ�ȭ
    virtual void Clear();

    // ���� ���� ����
    virtual void SetElementInfo( St_ElementCreate* pElement );
    // �θ� ����
    virtual void SetParentPtr( CEffectBody* pPtr );

    // ����/�ݺ�
    virtual bool Create( bool bFirst );
    virtual void Loop();
    // �ı�
    virtual void Destroy();
    // �޸� �ݳ�
    virtual void Free();

    // Move (�ùķ��̼�)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );
    // ������
    virtual void Render();

    // ���� ��(��ġ, ȸ��, ũ��) ���� - �ܺο��� ȣ��
    virtual void SetResultMatrix( const WzVector& wvPos, const WzVector& wvRot, const WzVector& wvScale );
    virtual void SetResultMatrix( const WzMatrix* pwmPos, const WzMatrix* pwmRot = NULL, const WzMatrix* pwmScale = NULL );

    // ��� ��ġ ����
    virtual void SetTargetPos( const WzVector& wvPos );

    // Ÿ�� ���
    virtual ENUM_EELEMENT_TYPE GetType() const;

    // �¾��?
    bool IsBorn() const;
    // �ݺ� Ÿ��?
    bool IsLoop() const;

    // ������ ����
    void SetOpacity( float fOpacity );

    // ���� ����/���
    void SetColor( WzColor c );
    WzColor GetColor() const;

    // ������ ����
    void SetRender( bool b );
    bool GetRender() const;

    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray ) {}
    virtual void ReloadTexture() {}

protected:
    // �� �ʱ�ȭ
    virtual void Reset();
    // �¾ �� ó��
    virtual void OnBeingBorn();

protected:
    St_ElementCreate* m_ec;         // element ���� ����
    CEffectBody* m_pParentPtr;      // �θ�

    bool m_bBorn;                   // �¾��?
    bool m_bReset;                  // �� �ʱ�ȭ? (�ݺ� ����)

    DWORD m_dwMakeTime;             // �¾ �ð�
    DWORD m_dwLife;                 // ���� (�¾ �� ��ư� �ð�)
    DWORD m_dwRemainLife;           // ���� ����
    DWORD m_dwExLife;               // �� �� ����

    DWORD m_dwCurLife;              // ���� �� �ð�
    DWORD m_dwBornCnt;              // �¾ �� �ð�

    WzColor m_wcColor;              // ����
    float m_fOpacity;               // �� ����

    bool m_bRender;                 // ������ ����
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

    // ���� ���� ����
    m_ec = pElement;

    // �������� �� �Լ����� �� element�� �ʿ��� ������ ���� ���� �߾��µ�
    // �� ����� ���� �ʰ� ���ɿ��� ũ�� ������ ��ġ�� ������ �ܼ��� ������
    // �������� �ٲ��.

    // �̷� ����, �� element�� ���� �Ǳ� ������ ElementCreate ������
    // ������� �� �ȴ�. ����!!
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
