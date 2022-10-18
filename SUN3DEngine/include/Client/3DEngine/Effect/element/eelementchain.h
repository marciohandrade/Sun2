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
    // ������/�Ҹ���
    CEElementChain();
    virtual ~CEElementChain();

    // �ʱ�ȭ
    virtual void Clear();

    // ���� ���� ����
    virtual void SetElementInfo( St_ElementCreate* pElement );

    // ����/�ı�
    virtual bool Create( bool bFirst );
    virtual void Destroy();
    // �޸� �ݳ�
    virtual void Free();

    // Move (�ùķ��̼�)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );
    // ������
    virtual void Render();

    // ��� ��ġ ����
    virtual void SetTargetPos( const WzVector& wvPos );

    // Ÿ�� ���
    virtual ENUM_EELEMENT_TYPE GetType() const;

    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray ) { HandleArray.push_back(m_hTexture); }

    virtual void ReloadTexture();

protected:
    // Ÿ�ٰ� �浹 �˻�
    bool CheckCollWithTarget();
    // �Ӹ� ���̱�
    void GenerateHead();


protected:
    St_ElementCreate_Chain* m_pChain;

    HANDLE m_hTexture;          // �ؽ��� �ڵ�

    WzVector* m_pwvVertexTB;    // ��/�Ʒ� ���� ����
    WzVector* m_pwvVertexLR;    // ��/�� ���� ����
    WzUVVert* m_puvVert;        // uv ��ǥ ����

    WzVector m_wvOrtho[2];      // ���� ����
    WzVector m_wvTargetPos;     // �浹 �˻��� Ÿ����ġ
    
    int m_iCurNumTail;          // ���� ���� ����
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
