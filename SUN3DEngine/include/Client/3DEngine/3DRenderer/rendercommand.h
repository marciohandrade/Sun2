// rendercommand.h

#ifndef _3DRENDERER_RENDERCOMMAND_H_
#define _3DRENDERER_RENDERCOMMAND_H_

#include "typedef.h"
#include <client/3dengine/3dfileaccess/wzdbase/wzdbase.h>
#include <client/3dengine/3dfileaccess/wzunit/wzunitdraw.h>

#define RCF_NONE                    (0x00000000)
#define RCF_SINGLEPASS_PHONG        (0x00000001)    // Single pass Phong 중 ( Shader 에 의함 )
#define RCF_ENABLE_SPECULAR         (0x00000002)    // Specular 사용 유무(default:FALSE)
#define RCF_ALPHATEXTURE_FILTER     (0x00000004)    // 알파만 사용하는 texture filter
#define RCF_WITH_IRRADIANCEMAP      (0x00000008)    // Irradiance를 이용한 Normap Map을 사용할 경우.
#define RCF_MODULATE2X              (0x00000010)    // Modulate2X(default:FALSE)
#define RCF_SHADOW_CAST             (0x00000020)    // Shadow Rendering 중
#define RCF_SHADOW_RECEIVE          (0x00000040)    // Shadow Rendering 중
#define RCF_BRIGHTNESS_FUNC_MATTER  (0x00000080)    // Brightness func 값이 세팅되어있는가
#define RCF_ENABLE_GLOW             (0x00000100)    // GlowMap 사용
#define RCF_ENABLE_HAZE             (0x00000200)    // Haze 사용
#define RCF_ENABLE_REFRACTION_MASK  (0x00000400)    // Refraction Mask 사용
#define RCF_ENABLE_CUSTOM_LIGHT     (0x00000800)    // Custom Light 사용
#define RCF_DEFAULT                 RCF_WITH_IRRADIANCEMAP

// RenderCommand로부터 소트 우선 순위
enum ENUM_SORT_PRIORITY 
{
    ESP_NONE = 0,
    ESP_DEPTH,          // 카메라와의 깊이값에 따라 알파가 아닌 Mesh를 앞에서부터 그리도록
    ESP_BUFFER,         // 같은 버퍼를 사용하는 Mesh끼리 모아서 그리도록
    ESP_TEXTURE,        // 같은 텍스처를 사용하는 Mesh끼리 모아서 그리도록
    ESP_RENDERSTATE,    // 같은 렌더스테이트를 사용하는 Mesh끼리 모아서 그리도록
    NUM_ESP
};

//------------------------------------------------------------------------------
/**
    @struct St_AlterRender
*/
struct St_AlterRender 
{
    ENUM_ALTERNATIVE_RENDER eRenderType;
    union
    {
        // eRenderType이 굴절일 경우에만 유효
        struct
        {
            float fRefractionRate;
            float fRefractionAlpha; 
        };
    };
};

//------------------------------------------------------------------------------
/**
    @struct St_BaseRenderCommand
*/
struct St_BaseRenderCommand
{
    enum ENUM_RENDERCOMMAND_TYPE
    {
        ERCT_UNIT = 0,
        ERCT_TRIANGLEBASED,
        ERCT_SPRITE,
        ERCT_NEW_SPRITE,
        ERCT_EFFECT,
        ERCT_UNIT_INSTANCING,
    };

    ENUM_RENDERCOMMAND_TYPE m_eRCType;          // Unit인지 Triangle-based인지 
    ENUM_ALTERNATIVE_RENDER m_eRenderType;      // Unit일 경우, m_pUnitDraw 내부값으로부터 얻어온다.

    union
    {
        // 비교 대상값
        float m_fSortResult;    // depth 비교시 사용
        DWORD m_dwSortResult;   // buffer, texture, renderstate 비교시 사용
        int m_iCurSortIndex;    // 현재 소트 인덱스( CManagerRenderCommand::GetFirstRender,GetNextRender에서 사용)
    };

    int m_iSortIndex;
};

//------------------------------------------------------------------------------
/**
    @class struct St_TriRenderCommand
*/
struct St_TriRenderCommand : public St_BaseRenderCommand
{
    enum ENUM_TRI_RENDERCOMMAND_TYPE
    {
        ETRCT_TRIINDEXED = 0,
        ETRCT_TRI,      
        ETRCT_TRICOLORINDEXED,
        ETRCT_TRIPACKED,
        ETRCT_VERTEXBUFFER,
        ETRCT_SIMPLEMESH,
        ETRCT_NEW_TRI,
    };

    // 타입
    ENUM_TRI_RENDERCOMMAND_TYPE m_eTriType;

    // 렌더링 상태정보
    St_RenderState m_RenderState;

    // 위치 정보
    WzVector m_wvPos;

    // 굴절 정보
    float m_fRefractionRate;    // 외곡 율 0~1사이 값
    float m_fRefractionAlpha;   // 외곡 알파도 ( 0 - Diffuse 100%, 1 - 외곡 100% )

    // Shader
    int m_iVertexShader;
    int m_iPixelShader;

    // Diffuse Texture
    HANDLE m_hTexture;

    struct St_TriParam 
    { 
        // HANDLE hTexture;
        int iNumFace;
        WzVector* pwvVertex;
        WzVector* pwvNormal;
        WzUVVert* pwUVs;
        int iDataSize; 
    };

    struct St_TriIndexedParam
    { 
        // HANDLE hTexture;
        int iNumFace;
        WzVector* pwvVertex;
        int* piVertexMap;
        WzVector* pwvNormal;
        int* piNormalMap;
        WzUVVert* pwUVs;
        int* piUVMap;
        int iDataSize; 
    };

    struct St_ColorTriIndexedParam
    {
        // HANDLE hTexture;
        int iNumFace;
        WzVector* pwvVertex;
        int* piVertexMap;
        WzColor* pwcColor;
        int* piColorMap;
        WzUVVert* pwUVs;
        int* piUVMap;
        int iVertDataSize;
        int iColorDataSize; 
    };

    struct St_TriPackedParam
    {
        // HANDLE hTexture;
        int iNumFace;
        int* piVertexMap;
        WzVector* pwvPosVert;
        int* piPosVertIndex;
        WzColor* pwcColor;
        WzUVVert* pwUVs;
        WzVector* pwvNormal;
        int iVertexDataSize;
        int iPosVertMapDataSize;
        int iColorDataSize;
        int iUVDataSize;
        int iNormalDataSize;
    };

    struct St_VertexBufferParam 
    {
        // HANDLE hTexture;
        HANDLE hVertexBuffer;
        int iNumFace;
        int* piIndex;
        WzUVVert* pwuvStream;
        // int iVertexShader;
        // int iPixelShader;
    };

    struct St_SimpleMeshParam
    {
        // WzSimpleMesh *pwsmSimpleMesh;
        int iNumMesh;
        WzVector* pVertices;
        WzUVVert* pUVVerts;
    };

    struct St_NewTriParam
    {
        // HANDLE hTexture;
        int iNumFace;
        WzVector* pwvVertex;
        WzColor* pwcColor;
        WzUVVert* pwUVs;
        int* piVertexIndex;
        int* piColorIndex;
        int* piUVIndex;
        int iVertexSize;
        int iColorSize; 
        int iUVSize;
        DWORD dwFlags;
    };

    union
    {
        St_TriParam m_Tri;
        St_TriIndexedParam m_TriIndexed;
        St_ColorTriIndexedParam m_ColorTriIndexed;
        St_TriPackedParam m_TriPacked;
        St_VertexBufferParam m_VertBuffer;
        St_SimpleMeshParam m_SimpMesh;
        St_NewTriParam m_NewTri;
    };
};

//------------------------------------------------------------------------------
/**
    @struct St_SpriteRenderCommand

    - 하나의 Sprite 정보를 render 하려는 명령을 저장
*/
struct St_SpriteRenderCommand : public St_BaseRenderCommand
{
    WzColor m_wcDraw;

    ///< 렌더링 상태정보
    St_RenderState m_RenderState;

    union 
    {
        ///< RenderSprite's 인자값(St_AlphaRender 참조)
        struct 
        {
            HANDLE m_hTexture;
            WzVector m_wvPos;
            // ENUM_ALPHABLEND m_eabMode;
            float m_fSize[2]; 
            float m_fRotate;
            float m_fDepthOffset;
            BOOL m_bUVRegion; 
            WzUVVert m_wuvRegion[2]; 
        };
    };

    float m_fRefractionRate;      // 외곡 율 0~1사이 값
    float m_fRefractionAlpha;     // 외곡 알파도 ( 0 - Diffuse 100%, 1 - 외곡 100% )

    // Shader
    int m_iVertexShader;
    int m_iPixelShader;

    // 빌보드 타입
    ENUM_BILLBOARD_TYPE m_eBillboardType;

    // 결과 위치 리턴
    WzVector* m_pResultPos;
};

//------------------------------------------------------------------------------
/**
    @struct St_NewSpriteRenderCommand
*/
struct St_NewSpriteRenderCommand : public St_BaseRenderCommand
{
    WzColor m_wcDraw;

    // 렌더링 상태정보
    St_RenderState m_RenderState;

    union 
    {
        // RenderSprite's 인자값(St_AlphaRender 참조)
        struct 
        { 
            HANDLE m_hTexture;
            SpriteVerts m_wvPos[6];
            // ENUM_ALPHABLEND m_eabMode;
            float m_fRotate; 
            float m_fDepthOffset;
            BOOL m_bUVRegion; 
            WzUVVert m_wuvRegion[2];
            WzVector m_vCenterPos;
        };
    };

    float m_fRefractionRate;      // 외곡 율 0~1사이 값
    float m_fRefractionAlpha;     // 외곡 알파도 ( 0 - Diffuse 100%, 1 - 외곡 100% )

    // Shader
    int m_iVertexShader;
    int m_iPixelShader;

    // 빌보드 타입
    ENUM_BILLBOARD_TYPE m_eBillboardType;

    // 결과 위치 리턴
    WzVector* m_pResultPos;
};

//------------------------------------------------------------------------------
/**
    @class struct St_RenderCommand
*/
struct St_RenderCommand : public St_BaseRenderCommand
{
    // Render Flags
    DWORD m_dwFlags;

    // render 할 대상
    CWzUnitDraw* m_pUnitDraw;               // render 할 CWzUnitDraw
    CWzdBase* m_pWzdMesh;                   // render 할 mesh
    int m_iMeshGroup;                       // render 할 mesh group 번호
    WzMatrix* m_pwmTransform;               // vertex, normal 변환용 matrix - InverseAndTransformBuffer
    HANDLE m_hVertexTB;                     // Transform 한 결과를 담는 버퍼 (Vertex 용)
    HANDLE m_hNormalTB;                     // Transform 한 결과를 담는 버퍼 (Normal 용)
    HANDLE m_hShadingBuffer;                // Shading 결과를 담는 버퍼 ( Gouraud, Phong Shading )
    HANDLE m_hCustomUVBuffer;               // Custom UV 를 사용하는 경우 ( UVNormal, UVLight 에 사용)

    // texture 관련
    HANDLE m_ahTexture[NUM_TXC];            // 사용하는 texture 들 ( diffuse, specular, normalmap )
    HANDLE m_hTextureFilter;                // 사용하는 texture filter
    BOOL m_bUVShift;                        // UV shift 를 쓰는가
    WzUVVert m_wuvShift;                    // uv shift 값

    St_RenderState m_RenderState;           // render 시 상태로
    int m_iWithAlpha;                       // Alpha 를 가지고 render 하는 것인가

    // 조절값
    float m_fBrightnessFunc[3];             // Brightness func

    // on/off
    int m_iUVNormal;                        // Normal 을 기준으로 uv 계산(0:false, 1:UVNormal, 2:UV1DNormal)
    WzVector m_wvFactorForUVNormal[2];      // m_bUVNormal 가 TRUE 인 경우의 factor
    BOOL m_bPhong;                          // Phong shading 중
    BOOL m_bUVLight;                        // Normal 과 Light 를 기준으로 uv 계산
    int m_iApplySunLight;                   // Sun Light 의 적용 여부 ( 0 : 적용안함, 1 : 현재 상태 적용, 2 : 무조건 적용)
    float m_fSpecularIntensity;             // Specular 강도값

    St_OcclusionInfo* m_pOcclusion;

    float m_fGlowMapFactor;                 // GlowMap 강도
    float m_fHazeDist;                      // Haze 거리
    float m_fHazeScale;                     // Haze 스케일
};

//------------------------------------------------------------------------------
/**
    @struct St_InstancingRenderCommand
*/
struct St_InstancingRenderCommand : public St_RenderCommand
{
    int m_iAllocInstancing;
    CWzArray<St_InstancingInfo*> m_arInstancing;
};

//------------------------------------------------------------------------------
/**
*/
inline void InitRenderCommand( St_RenderCommand* prc )
{
    ZeroMemory( prc, sizeof ( St_RenderCommand ) );

    prc->m_hVertexTB = INVALID_HANDLE_VALUE;
    prc->m_hNormalTB = INVALID_HANDLE_VALUE;
    prc->m_hShadingBuffer = INVALID_HANDLE_VALUE;
    prc->m_hCustomUVBuffer = INVALID_HANDLE_VALUE;

    memset( prc->m_ahTexture, (int)INVALID_HANDLE_VALUE, sizeof( prc->m_ahTexture ) );

    prc->m_hTextureFilter = INVALID_HANDLE_VALUE;
    prc->m_iApplySunLight = 1;

    WzInitFlag( prc->m_dwFlags, RCF_DEFAULT );
}

//------------------------------------------------------------------------------
/**
*/
inline void InitInstancingRenderCommand( St_InstancingRenderCommand* pirc )
{
    InitRenderCommand( (St_RenderCommand*)pirc );
    pirc->m_iAllocInstancing = 0;
}

#endif // _3DRENDERER_RENDERCOMMAND_H_