// visualinfo.h

#ifndef _EFFECT_VISUALINFO_H_
#define _EFFECT_VISUALINFO_H_

#include "../typedef.h"

// 스프라이트
struct St_ElementCreate_Sprite
{
    WzID m_wiTexture;           // 텍스처 ID
    float m_fScale;             // 스케일

    bool m_blUseUVAni;          // 애니메이션 사용 여부
    long m_lnWidth;             // 가로 크기
    long m_lnHeight;            // 세로 크기
    long m_lnTotalFrame;        // 총 프레임 개수
    long m_lnTickAni;           // 애니메이션 시간 간격
    bool m_blUseLoop;           // 루프 사용 여부
    long m_lnStartFrame;        // 시작 프레임 번호

    EST_BILLBOARD_TYPE m_eBillboardType;
    WzVector m_wvDirect;        // 사용 안 함
    bool m_blDirApply;          // 방향 적용 여부
    WzVector m_wvVertex[4];     // 점들(빌보드 타입이 아닌 경우)

    float m_fOffset;            // 카메라 방향으로 오프셋
    float m_fMakeOffsetX;       // 중심에서 X축 오프셋 (0~1)
    float m_fMakeOffsetY;       // 중심에서 Y축 오프셋

    bool m_blUseStartFrameRand; // 랜덤 시작 프레임 사용 여부
    BYTE m_byInterpolateRate;   // 애니보간 비율
};

// 메시
struct St_ElementCreate_Mesh
{
    WzID m_wiMesh;              // 메시 ID
    WzID m_wiAnimation;         // 애니메이션 번호
    float m_fSize;              // 크기
    EST_BILLBOARD_TYPE m_eBillboardType; // 빌보드 타입
};

// 체인
struct St_ElementCreate_Chain
{
    WzID m_wiTexture;           // 텍스처 ID
    int m_iNumTail;             // 꼬리 개수
    float m_fThickness;         // 두께
};

// 플랫 체인
struct St_ElementCreate_FlatChain
{
    WzID m_wiTexture;           // 텍스처 ID
    WzVector m_wvPos2;          // 사용 안 함
    int m_iNumTail;             // 꼬리 개수
};

// 데칼
struct St_ElementCreate_Decal
{
    WzID m_wiTexture;           // 텍스처 ID
    WzVector m_wvNormal;        // Normal 벡터
    WzVector m_wvUp;            // Up 벡터
    float m_fSize;              // 수평 넓이
    float m_fHeight;            // 높이
};

// 날씨
struct St_ElementCreate_Weather
{
    WzID m_wiTexture;           // 텍스처 ID
    float m_fScale;             // 스프라이트 크기

    float m_fDistance;          // 공간 크기
    long m_lnDensityCnt;        // 밀도

    EST_BILLBOARD_TYPE m_eBillboardType; // 빌보드 타입
    bool m_blDirApply;          // 방향 적용 여부
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

    // 생성자
    St_Visual_Info()
    {
        Initialize();
    }

    // 초기화
    void Initialize()
    {
        // 현재까지는 memset으로 초기화 해도 안전하다.
        // 이후, 새롭게 멤버 변수를 추가한다거나 하는 경우는
        // memset을 해도 되는지 살펴 봐야 한다.

        memset( this, 0, sizeof( St_Visual_Info ) );

        m_eBlendOP = EBOP_NONE;
        eType = EET_NONE;

        // xxx: 공용체 변수를 이렇게 초기화 한다는게 영 찝찝하지만
        // 그렇다고 이걸 제거하면 어떤 일이 벌어질 지 알 수가 없기에
        // 일단은 그냥 간다. 엔진만 생각하면 로딩 부분에서 따로따로
        // 초기화 해 주면 되지만, 이펙트 툴은?;;

        // 스프라이트 초기화
        SetVector( &Sprite.m_wvVertex[0], -0.5f,  0.5f, 0.0f );
        SetVector( &Sprite.m_wvVertex[1], -0.5f, -0.5f, 0.0f );
        SetVector( &Sprite.m_wvVertex[2],  0.5f,  0.5f, 0.0f );
        SetVector( &Sprite.m_wvVertex[3],  0.5f, -0.5f, 0.0f );

        SetVector( &Sprite.m_wvDirect, 0.0f, 0.0f, 1.0f );

        Sprite.m_fMakeOffsetX = 0.5f;
        Sprite.m_fMakeOffsetY = 0.5f;
        Sprite.m_eBillboardType = EB_ALL;

        // 메시 초기화
        Mesh.m_fSize = 1.0f;
        Mesh.m_eBillboardType = EB_UNUSE;

        // 랜덤 사이즈
        m_sRandElementSize.m_fMinValue = 1.0f;
        m_sRandElementSize.m_fMaxValue = 1.0f;
    }

    // operator =
    St_Visual_Info& operator = ( const St_Visual_Info& rhs )
    {
        if( &rhs != this )
        {
            // 현재까지는 memcpy로 복사 해도 안전하다.
            // 이후, 새롭게 멤버 변수를 추가한다거나 하는 경우는
            // memcpy을 해도 되는지 살펴 봐야 한다.

            memcpy( this, &rhs, sizeof( St_Visual_Info ) );
        }

        return *this;
    }
};

#endif // _EFFECT_VISUALINFO_H_