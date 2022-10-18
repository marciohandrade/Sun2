// visualinfo.h

#ifndef _EFFECT_VISUALINFO_H_
#define _EFFECT_VISUALINFO_H_

#include "../typedef.h"

// ��������Ʈ
struct St_ElementCreate_Sprite
{
    WzID m_wiTexture;           // �ؽ�ó ID
    float m_fScale;             // ������

    bool m_blUseUVAni;          // �ִϸ��̼� ��� ����
    long m_lnWidth;             // ���� ũ��
    long m_lnHeight;            // ���� ũ��
    long m_lnTotalFrame;        // �� ������ ����
    long m_lnTickAni;           // �ִϸ��̼� �ð� ����
    bool m_blUseLoop;           // ���� ��� ����
    long m_lnStartFrame;        // ���� ������ ��ȣ

    EST_BILLBOARD_TYPE m_eBillboardType;
    WzVector m_wvDirect;        // ��� �� ��
    bool m_blDirApply;          // ���� ���� ����
    WzVector m_wvVertex[4];     // ����(������ Ÿ���� �ƴ� ���)

    float m_fOffset;            // ī�޶� �������� ������
    float m_fMakeOffsetX;       // �߽ɿ��� X�� ������ (0~1)
    float m_fMakeOffsetY;       // �߽ɿ��� Y�� ������

    bool m_blUseStartFrameRand; // ���� ���� ������ ��� ����
    BYTE m_byInterpolateRate;   // �ִϺ��� ����
};

// �޽�
struct St_ElementCreate_Mesh
{
    WzID m_wiMesh;              // �޽� ID
    WzID m_wiAnimation;         // �ִϸ��̼� ��ȣ
    float m_fSize;              // ũ��
    EST_BILLBOARD_TYPE m_eBillboardType; // ������ Ÿ��
};

// ü��
struct St_ElementCreate_Chain
{
    WzID m_wiTexture;           // �ؽ�ó ID
    int m_iNumTail;             // ���� ����
    float m_fThickness;         // �β�
};

// �÷� ü��
struct St_ElementCreate_FlatChain
{
    WzID m_wiTexture;           // �ؽ�ó ID
    WzVector m_wvPos2;          // ��� �� ��
    int m_iNumTail;             // ���� ����
};

// ��Į
struct St_ElementCreate_Decal
{
    WzID m_wiTexture;           // �ؽ�ó ID
    WzVector m_wvNormal;        // Normal ����
    WzVector m_wvUp;            // Up ����
    float m_fSize;              // ���� ����
    float m_fHeight;            // ����
};

// ����
struct St_ElementCreate_Weather
{
    WzID m_wiTexture;           // �ؽ�ó ID
    float m_fScale;             // ��������Ʈ ũ��

    float m_fDistance;          // ���� ũ��
    long m_lnDensityCnt;        // �е�

    EST_BILLBOARD_TYPE m_eBillboardType; // ������ Ÿ��
    bool m_blDirApply;          // ���� ���� ����
};

//------------------------------------------------------------------------------
/**
    @struct St_Visual_Info
*/
struct St_Visual_Info
{
    ENUM_BLEND_OP m_eBlendOP;
    ENUM_EELEMENT_TYPE eType;

    union
    {
        St_ElementCreate_Sprite Sprite;
        St_ElementCreate_Mesh Mesh;
        St_ElementCreate_Chain Chain;
        St_ElementCreate_FlatChain FlatChain;
        St_ElementCreate_Decal Decal;
        St_ElementCreate_Weather WeatherObj;
    };

    St_RandomInfo m_sRandElementSize;

    // ������
    St_Visual_Info()
    {
        Initialize();
    }

    // �ʱ�ȭ
    void Initialize()
    {
        // ��������� memset���� �ʱ�ȭ �ص� �����ϴ�.
        // ����, ���Ӱ� ��� ������ �߰��Ѵٰų� �ϴ� ����
        // memset�� �ص� �Ǵ��� ���� ���� �Ѵ�.

        memset( this, 0, sizeof( St_Visual_Info ) );

        m_eBlendOP = EBOP_NONE;
        eType = EET_NONE;

        // xxx: ����ü ������ �̷��� �ʱ�ȭ �Ѵٴ°� �� ����������
        // �׷��ٰ� �̰� �����ϸ� � ���� ������ �� �� ���� ���⿡
        // �ϴ��� �׳� ����. ������ �����ϸ� �ε� �κп��� ���ε���
        // �ʱ�ȭ �� �ָ� ������, ����Ʈ ����?;;

        // ��������Ʈ �ʱ�ȭ
        SetVector( &Sprite.m_wvVertex[0], -0.5f,  0.5f, 0.0f );
        SetVector( &Sprite.m_wvVertex[1], -0.5f, -0.5f, 0.0f );
        SetVector( &Sprite.m_wvVertex[2],  0.5f,  0.5f, 0.0f );
        SetVector( &Sprite.m_wvVertex[3],  0.5f, -0.5f, 0.0f );

        SetVector( &Sprite.m_wvDirect, 0.0f, 0.0f, 1.0f );

        Sprite.m_fMakeOffsetX = 0.5f;
        Sprite.m_fMakeOffsetY = 0.5f;
        Sprite.m_eBillboardType = EB_ALL;

        // �޽� �ʱ�ȭ
        Mesh.m_fSize = 1.0f;
        Mesh.m_eBillboardType = EB_UNUSE;

        // ���� ������
        m_sRandElementSize.m_fMinValue = 1.0f;
        m_sRandElementSize.m_fMaxValue = 1.0f;
    }

    // operator =
    St_Visual_Info& operator = ( const St_Visual_Info& rhs )
    {
        if( &rhs != this )
        {
            // ��������� memcpy�� ���� �ص� �����ϴ�.
            // ����, ���Ӱ� ��� ������ �߰��Ѵٰų� �ϴ� ����
            // memcpy�� �ص� �Ǵ��� ���� ���� �Ѵ�.

            memcpy( this, &rhs, sizeof( St_Visual_Info ) );
        }

        return *this;
    }
};

#endif // _EFFECT_VISUALINFO_H_