// rendercommand.h

#ifndef _3DRENDERER_RENDERCOMMAND_H_
#define _3DRENDERER_RENDERCOMMAND_H_

#include "typedef.h"
#include <client/3dengine/3dfileaccess/wzdbase/wzdbase.h>
#include <client/3dengine/3dfileaccess/wzunit/wzunitdraw.h>

#define RCF_NONE                    (0x00000000)
#define RCF_SINGLEPASS_PHONG        (0x00000001)    // Single pass Phong �� ( Shader �� ���� )
#define RCF_ENABLE_SPECULAR         (0x00000002)    // Specular ��� ����(default:FALSE)
#define RCF_ALPHATEXTURE_FILTER     (0x00000004)    // ���ĸ� ����ϴ� texture filter
#define RCF_WITH_IRRADIANCEMAP      (0x00000008)    // Irradiance�� �̿��� Normap Map�� ����� ���.
#define RCF_MODULATE2X              (0x00000010)    // Modulate2X(default:FALSE)
#define RCF_SHADOW_CAST             (0x00000020)    // Shadow Rendering ��
#define RCF_SHADOW_RECEIVE          (0x00000040)    // Shadow Rendering ��
#define RCF_BRIGHTNESS_FUNC_MATTER  (0x00000080)    // Brightness func ���� ���õǾ��ִ°�
#define RCF_ENABLE_GLOW             (0x00000100)    // GlowMap ���
#define RCF_ENABLE_HAZE             (0x00000200)    // Haze ���
#define RCF_ENABLE_REFRACTION_MASK  (0x00000400)    // Refraction Mask ���
#define RCF_ENABLE_CUSTOM_LIGHT     (0x00000800)    // Custom Light ���
#define RCF_DEFAULT                 RCF_WITH_IRRADIANCEMAP

// RenderCommand�κ��� ��Ʈ �켱 ����
enum ENUM_SORT_PRIORITY 
{
    ESP_NONE = 0,
    ESP_DEPTH,          // ī�޶���� ���̰��� ���� ���İ� �ƴ� Mesh�� �տ������� �׸�����
    ESP_BUFFER,         // ���� ���۸� ����ϴ� Mesh���� ��Ƽ� �׸�����
    ESP_TEXTURE,        // ���� �ؽ�ó�� ����ϴ� Mesh���� ��Ƽ� �׸�����
    ESP_RENDERSTATE,    // ���� ����������Ʈ�� ����ϴ� Mesh���� ��Ƽ� �׸�����
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
        // eRenderType�� ������ ��쿡�� ��ȿ
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

    ENUM_RENDERCOMMAND_TYPE m_eRCType;          // Unit���� Triangle-based���� 
    ENUM_ALTERNATIVE_RENDER m_eRenderType;      // Unit�� ���, m_pUnitDraw ���ΰ����κ��� ���´�.

    union
    {
        // �� ���
        float m_fSortResult;    // depth �񱳽� ���
        DWORD m_dwSortResult;   // buffer, texture, renderstate �񱳽� ���
        int m_iCurSortIndex;    // ���� ��Ʈ �ε���( CManagerRenderCommand::GetFirstRender,GetNextRender���� ���)
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

    // Ÿ��
    ENUM_TRI_RENDERCOMMAND_TYPE m_eTriType;

    // ������ ��������
    St_RenderState m_RenderState;

    // ��ġ ����
    WzVector m_wvPos;

    // ���� ����
    float m_fRefractionRate;    // �ܰ� �� 0~1���� ��
    float m_fRefractionAlpha;   // �ܰ� ���ĵ� ( 0 - Diffuse 100%, 1 - �ܰ� 100% )

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

    - �ϳ��� Sprite ������ render �Ϸ��� ����� ����
*/
struct St_SpriteRenderCommand : public St_BaseRenderCommand
{
    WzColor m_wcDraw;

    ///< ������ ��������
    St_RenderState m_RenderState;

    union 
    {
        ///< RenderSprite's ���ڰ�(St_AlphaRender ����)
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

    float m_fRefractionRate;      // �ܰ� �� 0~1���� ��
    float m_fRefractionAlpha;     // �ܰ� ���ĵ� ( 0 - Diffuse 100%, 1 - �ܰ� 100% )

    // Shader
    int m_iVertexShader;
    int m_iPixelShader;

    // ������ Ÿ��
    ENUM_BILLBOARD_TYPE m_eBillboardType;

    // ��� ��ġ ����
    WzVector* m_pResultPos;
};

//------------------------------------------------------------------------------
/**
    @struct St_NewSpriteRenderCommand
*/
struct St_NewSpriteRenderCommand : public St_BaseRenderCommand
{
    WzColor m_wcDraw;

    // ������ ��������
    St_RenderState m_RenderState;

    union 
    {
        // RenderSprite's ���ڰ�(St_AlphaRender ����)
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

    float m_fRefractionRate;      // �ܰ� �� 0~1���� ��
    float m_fRefractionAlpha;     // �ܰ� ���ĵ� ( 0 - Diffuse 100%, 1 - �ܰ� 100% )

    // Shader
    int m_iVertexShader;
    int m_iPixelShader;

    // ������ Ÿ��
    ENUM_BILLBOARD_TYPE m_eBillboardType;

    // ��� ��ġ ����
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

    // render �� ���
    CWzUnitDraw* m_pUnitDraw;               // render �� CWzUnitDraw
    CWzdBase* m_pWzdMesh;                   // render �� mesh
    int m_iMeshGroup;                       // render �� mesh group ��ȣ
    WzMatrix* m_pwmTransform;               // vertex, normal ��ȯ�� matrix - InverseAndTransformBuffer
    HANDLE m_hVertexTB;                     // Transform �� ����� ��� ���� (Vertex ��)
    HANDLE m_hNormalTB;                     // Transform �� ����� ��� ���� (Normal ��)
    HANDLE m_hShadingBuffer;                // Shading ����� ��� ���� ( Gouraud, Phong Shading )
    HANDLE m_hCustomUVBuffer;               // Custom UV �� ����ϴ� ��� ( UVNormal, UVLight �� ���)

    // texture ����
    HANDLE m_ahTexture[NUM_TXC];            // ����ϴ� texture �� ( diffuse, specular, normalmap )
    HANDLE m_hTextureFilter;                // ����ϴ� texture filter
    BOOL m_bUVShift;                        // UV shift �� ���°�
    WzUVVert m_wuvShift;                    // uv shift ��

    St_RenderState m_RenderState;           // render �� ���·�
    int m_iWithAlpha;                       // Alpha �� ������ render �ϴ� ���ΰ�

    // ������
    float m_fBrightnessFunc[3];             // Brightness func

    // on/off
    int m_iUVNormal;                        // Normal �� �������� uv ���(0:false, 1:UVNormal, 2:UV1DNormal)
    WzVector m_wvFactorForUVNormal[2];      // m_bUVNormal �� TRUE �� ����� factor
    BOOL m_bPhong;                          // Phong shading ��
    BOOL m_bUVLight;                        // Normal �� Light �� �������� uv ���
    int m_iApplySunLight;                   // Sun Light �� ���� ���� ( 0 : �������, 1 : ���� ���� ����, 2 : ������ ����)
    float m_fSpecularIntensity;             // Specular ������

    St_OcclusionInfo* m_pOcclusion;

    float m_fGlowMapFactor;                 // GlowMap ����
    float m_fHazeDist;                      // Haze �Ÿ�
    float m_fHazeScale;                     // Haze ������
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