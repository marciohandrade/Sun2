// eelementsprite.h

#ifndef _EFFECT_EELEMENTSPRITE_H_
#define _EFFECT_EELEMENTSPRITE_H_

#include "eelementparticle.h"
#include <client/3dengine/common/wzuvvert.h>
#include <client/3dengine/3drenderer/managertexture.h>

struct UVInterpolation
{
    bool bDrawOverlap;      // OverLap �׸��� ����
    WzUVVert UVVert[4];     // uv��ǥ
    float fOpacity;         // ����
    WORD wInterpolateRate;  // ���� ���� ����
};
//------------------------------------------------------------------------------
/**
    @class CEElementSprite
*/
class CEElementSprite : public CEElementParticle
{
public:
    // ������/�Ҹ���
    CEElementSprite();
    virtual ~CEElementSprite();

    // �ʱ�ȭ
    virtual void Clear();

    // ���� ���� ����
    virtual void SetElementInfo( St_ElementCreate* pElement );

    // ����
    virtual bool Create( bool bFirst );
    // �޸� �ݳ�
    virtual void Free();

    // Move (�ùķ��̼�)
    virtual _ENUM_RESULT_EFFECT_MOVE Move( DWORD dwTick, bool& bSimulate );
    // ������
    virtual void Render();

    // Ÿ�� ���
    virtual ENUM_EELEMENT_TYPE GetType() const;

    // �ؽ��ĸ� �ٽ÷ε��Ѵ�
    virtual void ReloadTexture();

    virtual void GetTextureHandle( std::vector<HANDLE>& HandleArray ) { HandleArray.push_back(m_hTexture); }


protected:
    // ��������Ʈ �̵�
    void MoveSprite(bool is_first_moved = false);

    // UV �ִϸ��̼�
    void UVAnimation( DWORD lnBornCnt );
    // UV �ִϸ��̼� ����
    void UVAniInterpolation( DWORD lnBornCnt );

    // Į�� ����
    void SetSpriteColor( bool bBackImage );

protected:
    St_ElementCreate_Sprite* m_pSprite;

    HANDLE m_hTexture;          // �ؽ�ó �ڵ�

    WzVector m_wvVertex[4];     // ������
    WzUVVert m_UVVert[4];

    float m_fScale;             // ũ��

    long m_lnGaroCnt;           // �ؽ��� Uũ���� ���� ����
    float m_fGaroRate;          // ���� ����
    float m_fSeroRate;          // ���� ����

    long m_lnStartAniNo;        // ���� �ִ� ��ȣ
    long m_lnCurAniNo;          // ���� �ִ� ��ȣ
    long m_lnPrevAniNo;         // ���� �ִ� ��ȣ

    UVInterpolation m_stInterpolate;
};

//------------------------------------------------------------------------------
/**
*/
inline ENUM_EELEMENT_TYPE CEElementSprite::GetType() const
{ 
    return EET_SPRITE;
}

#endif // _EFFECT_EELEMENTSPRITE_H_
