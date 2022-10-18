// eelementflatchain.h

#ifndef _EFFECT_EELEMENTFLATCHAIN_H_
#define _EFFECT_EELEMENTFLATCHAIN_H_

#include "eelementbase.h"
#include <client/3dengine/common/wzuvvert.h>

// �÷� ü�� ���ؽ� ����ü
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
    // ������/�Ҹ���
    CEElementFlatChain();
    virtual ~CEElementFlatChain();

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

    // Ÿ�� ���
    virtual ENUM_EELEMENT_TYPE GetType() const;

    // ���� ���� ����
    WZEFF_API void SetLifeInfo( DWORD dwMakeTime, DWORD dwLife );
    // ���� ���� ����
    WZEFF_API void SetTailNum( int nTail );
    // ��ġ ����
    void SetPosition( const WzVector& wvPos1, const WzVector& wvPos2 );
    // ���ö��� ���� ����
    void SetSplineLength( float fLen );


    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray ) { HandleArray.push_back(m_hTexture); }


    virtual void ReloadTexture();

protected:
    // ���� �ı�
    void DestroyBuffers();

    // �Ӹ� ���̱�
    void GenerateHead();

protected:
    St_ElementCreate_FlatChain* m_pFlatChain;

    HANDLE m_hTexture;      // �ؽ��� �ڵ�

    int m_iNumTail;         // �ִ� ���� ����
    int m_iCurNumTail;      // ���� ���� ����

    FCVertex* m_pwvVertex;  // ���� ����
    WzUVVert* m_puvVert;    // UV��ǥ ����

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
