// typedef.h

#ifndef _3DRENDERER_TYPEDEF_H_
#define _3DRENDERER_TYPEDEF_H_

#include <client/3dengine/common/wzcommontypedef.h>
#include <client/3dengine/common/wzmatrix.h>
#include <client/3dengine/common/wzuvvert.h>
#include <client/3dengine/common/wzbvtype.h>

#include <client/3dengine/3dfileaccess/wzunit/wzunitsubtype.h>

// TODO: ���� ����
// ���̳� Ŭ���̾�Ʈ���� �� �� �ֱ� ������ �ϴ� �����Ѵ�.
#include "deldefine.h"

#ifdef WZ3DRENDERER_EXPORTS
#define WZRND_API   __declspec( dllexport )
#else
#define WZRND_API   __declspec( dllimport )
#endif

// #define _YJ_PERF_HUD_TEST

// Flag ��ũ��
#define WzInitFlag( v, flag )       ( v = flag )
#define WzSetFlag( v, flag )        ( v |= flag )
#define WzUnsetFlag( v, flag )      ( v &= ~flag )
#define WzEnableFlag( v, flag, is ) ( (is) ? WzSetFlag( v, flag ) : WzUnsetFlag( v, flag ) )
#define WzCheckFlag( v, flag )      ( flag & v )
#define WzCheckFlagB( v, flag )     ( ( flag & v ) ? TRUE : FALSE )

// Mesh �׸��� ���
enum ENUM_MESH_DRAW
{
    MD_POLYGON = 0,
    MD_WIREFRAME,
    MD_POLYGON_WITHEDGE,    // debug ��忡���� ����� �۵�
    MD_KEEP_CURRENT         // ���� ���� ����
};

// Cull ���
enum ENUM_CULL_FACE
{
    CF_DRAW_FRONT = 0,
    CF_DRAW_BACK,
    CF_DRAW_ALL,
    CF_DRAW_KEEP_CURRENT
};

// Z-func
enum ENUM_CMP_FUNC
{
    CF_NEVER = 0,
    CF_LESS,
    CF_EQUAL,
    CF_LESSEQUAL,
    CF_GREATER,
    CF_NOTEQUAL,
    CF_GREATEREQUAL,
    CF_ALWAYS,
    CF_KEEP_CURRENT
};

// ���� Mask
#define COLOR_MASK_RED      (0x01)
#define COLOR_MASK_GREEN    (0x02)
#define COLOR_MASK_BLUE     (0x04)
#define COLOR_MASK_ALPHA    (0x08)

// Stencil Operation
enum ENUM_STENCIL_OP
{
    SO_KEEP = 0,
    SO_ZERO,
    SO_REPLACE,
    SO_INCR,
    SO_DECR,
    SO_INVERT
};



// ȭ�� ��� - �ػ� ���� ����
struct St_ScreenFormat
{
    BOOL m_bFullScreen;
    BOOL m_bUseDialogMode;

    // ������ ũ��
    DWORD m_dwWidth;
    DWORD m_dwHeight;

    // â��� �϶� �ִ�ȭ ����
    // m_bFullScreen == FALSE �϶��� ���
    // m_bMaximize == true �̸� window ũ��(m_dwWidth, m_dwHeight)�� �ػ󵵿� �°� ���� ��ȯ
    //                          (3dsetup.ini ���Ͽ� �ִ� �����찪 ������� ����)
    bool m_bMaximizeScreen;


    // ���� ������ ��Ÿ��(���� â��� �ִ�ȭ ����鼭 ���� ����)
    // �ʱ� ���� ������ ��Ÿ�� ���
    bool m_bUseInitWindowStyle;

    // ȭ�� ���� Ÿ�� (RGBA)
    int m_iRed;
    int m_iGreen;
    int m_iBlue;
    int m_iAlpha;

    // ��Ÿ ���� ����
    int m_iDepthBits;
    int m_iStencilBits;
    int m_iMultiSampling;

    St_ScreenFormat()
    {
        m_bUseDialogMode = FALSE;
        m_bFullScreen = FALSE;
        m_bMaximizeScreen = false;
        m_bUseInitWindowStyle = false;
        m_dwWidth = 640;
        m_dwHeight = 480;
        m_iRed = 8;
        m_iGreen = 8;
        m_iBlue = 8;
        m_iAlpha = 0;
        m_iDepthBits = 16;
        m_iStencilBits = 0;
        m_iMultiSampling = 0;
    }
};

// RenderCaps - �ִ� �ּ� ���� ���� ����
struct St_RenderCaps
{
    // �ؽ��� �ִ� ũ��
    UINT m_iMaxTextureWidth;
    UINT m_iMaxTextureHeight;

    // ���ÿ� ����� �� �ִ� �ִ� �ؽ��� ��
    int m_iMaxSimultaneousTextures;

    // ���ٽ� ����
    UINT m_iNumStencilBits;
    
    // MultiSampling 
    int m_iDevMultiSampling;
};

// ī�޶� ���� �����͵�
enum ENUM_SCREEN_RATIO
{
    SR_AUTO = 0,
    SR_4_3,
    SR_16_9,
    SR_16_10,
};

struct St_Camera
{
    // �⺻
    WzMatrix m_wmWorldMatrix;

    // pick ���� ���� ��
    WzVector m_wvFrom;
    WzVector m_wvTo;

    // pick ���� - ī�޶� �������� 1.0f �̵��� ����, ���η� �󸶸�ŭ �̵��ϴ��� ����
    float m_fCameraXRate;
    float m_fCameraYRate;

    // Up vector �� Right vector
    WzVector m_wvDir;
    WzVector m_wvUp;
    WzVector m_wvRight;

    // Frustum ��꿡�� ���� ��
    BOOL m_bOrthogonal; // Orthogonal �ΰ�?

    float m_fNear;
    float m_fFar;

    union
    {
        struct
        {
            float m_fFov;
            float m_fAspect;
            ENUM_SCREEN_RATIO m_eScreenRatio;
        };

        struct
        {
            float m_fLeft;
            float m_fRight;
            float m_fBottom;
            float m_fTop;
        };
    };

    WzFrustum m_wfFrustum;
    WzFrustum m_wfFrustum4Shadow;
};

// Fog ���� �Ķ����
struct St_FogParameter
{
    WzColor m_wcFogColor;
    float m_fFogStart;
    float m_fFogEnd;
    float m_fMaxFogRate;    // 0 % - 100%
};

// Light ���� �Ķ����
enum ENUM_LIGHT_TYPE
{
    ELT_DIRECTIONAL = 0,    // �¾��
    ELT_POINT,              // ������
    NUM_ELT
};

// ����Ʈ�� Irradiance Ÿ��
typedef int ENUM_LIGHT_IRRA_TYPE;   // ���� ���� ���ǵǸ� enum���� ����~

enum
{
    NUM_ELIT = 4 
};

// ����Ʈ ����
struct St_LightInformation
{       
    WzColor m_wcColor;      // �� ����
    BOOL m_bValid;          // ��뿩��
    float m_fRange;         // ����
    float m_fIntensity;     // ��(����ŧ��?)

    unsigned long m_OctIndex;       // ��Ʈ�� �ε���    
    ENUM_LIGHT_TYPE m_eltType;      // �� ����  
    ENUM_LIGHT_IRRA_TYPE m_eliType; // Irradiance Ÿ��

    union
    {
        struct
        {   // Directional
            WzVector m_wvDirection; // ����
        };

        struct
        {   // Point
            WzVector m_wvPosition;  // ��ġ
        };
    };
};

struct St_LightApplyInformation
{
    BOOL bValid; 
    CWzArray<DWORD> arIndex;
};

#define MAX_NUM_TERRAIN_LIGHT   (400)   // �ִ� ���� ����Ʈ ��
#define MAX_NUM_ACTIVE_LIGHT    (2)     // �ִ� Ȱ��ȭ ��ų�� �ִ� ����Ʈ ��
#define MAX_DIR_LIGHT           (2)     // �𷺼ų� ����Ʈ ��
#define MAX_NUM_LIGHT           (20)    // �ִ� ����Ʈ ��

// Alpha blending ����

// Blend Factor
enum ENUM_BLEND_FACTOR
{
    BF_ERROR = 0,

    BF_ZERO = 1,
    BF_ONE,

    BF_SRCCOLOR,
    BF_INVSRCCOLOR,
    BF_SRCALPHA,
    BF_INVSRCALPHA,

    BF_DESTALPHA,
    BF_INVDESTALPHA,
    BF_DESTCOLOR,
    BF_INVDESTCOLOR,

    BF_SRCALPHASAT,

    NUM_BF
};

enum ENUM_ALPHABLEND
{
    AB_NONE = 0,    
    AB_ALPHA,           // �Ϲ� Alpha ( Alpha ä�� ���)
    AB_LIGHTING,        // ��ĥ Alpha ( Alpha ä�� ���, ���� ���� �����ִ� ȿ��)
    AB_ALPHA_COLOR,     // ���� Alpha
    AB_SHADING,         // ���̵�
    AB_INV_SHADING,     // �ι��� ���̵�
    AB_SUBTRACT,        // Src - Dest
    AB_REVSUBTRACT,     // Dest - Src
    NUM_AB,
    AB_KEEP_CURRENT,    // ���簪 ���� (pass �ϱ�)

    // �߰��� ���� (������ Customized - low byte �� src, high byte �� dest)
    AB_NEW_ALPHA        = MAKEWORD( BF_SRCALPHA, BF_INVSRCALPHA ),
    AB_NEW_LIGHTING     = MAKEWORD( BF_SRCALPHA, BF_ONE ),
    AB_NEW_ALPHA_COLOR  = MAKEWORD( BF_SRCCOLOR, BF_INVSRCCOLOR ),
    AB_NEW_SHADING      = MAKEWORD( BF_ZERO, BF_SRCCOLOR ),
    AB_NEW_INV_SHADING  = MAKEWORD( BF_ZERO, BF_INVSRCCOLOR ),
    AB_NEW_SUBTRACT     = MAKEWORD( BF_SRCALPHA, BF_ONE ),
    AB_NEW_REVSUBTRACT  = MAKEWORD( BF_SRCALPHA, BF_ONE ),
};

// MeshGroup �׸��� ����
struct St_RendermeshGroupAnalyzer
{
    HANDLE m_hTexDiffuse;
    int m_iWithAlpha;   // 0 - alpha ����, 1 - �Ϲ� alpha, 2 - �ո��� �̹���
    BOOL m_bStream;
    WzUVVert* m_pwuvCustom;
    BOOL m_bPhong;
    BOOL m_bIntoAlphaList;
    HANDLE m_hTextureFilter;
};

inline void InitRGA( St_RendermeshGroupAnalyzer* prga )
{
    prga->m_hTexDiffuse = INVALID_HANDLE_VALUE;
    prga->m_iWithAlpha = 0;
    prga->m_bStream = FALSE;
    prga->m_pwuvCustom = NULL;
    prga->m_bPhong = FALSE;
    prga->m_bIntoAlphaList = FALSE;
    prga->m_hTextureFilter = INVALID_HANDLE_VALUE;
}

// StateParams - ���� �Ķ���͵�
struct St_RenderState
{
    DWORD m_dwRenderState;          // ���� ���� ( z-buffer ���� ���� ��...)
    WzColor m_wcDraw[NUM_WRIC];     // �׸��� ����(Diffuse, Specular, Ambient)
    ENUM_ALPHABLEND m_abMode;       // Alpha blend ���
    ENUM_MESH_DRAW m_eMeshDrawMode; // mesh draw ���
    ENUM_CULL_FACE m_eCullFaceMode; // cull face ���
    ENUM_CMP_FUNC m_eZFunc;         // Z Func
    DWORD m_dwColorMask;            // Color mask
    DWORD m_dwAlphaTestReference;   // Alpha test �� ���� ��
    float m_fDepthBias;             // Depth Bias
    float m_fSlopeScaleDepthBias;   // SlopeScaleDepthBias;
};

struct St_StateParams
{
    WzColor m_wcClear;              // ȭ�� ����� ��
    ENUM_CMP_FUNC m_eAlphaTestFunc; // Alpha test �� �� �Լ�
    BOOL m_bEnableAlphaTest;
    BOOL m_bEnableSpecular;         // Specular ��� ����(default:FALSE)

    // ��� ���� ����
    int m_iTextureStageCurrent;     // ���� �Ϲ� �ؽ��İ� ���� TextureStage
    int m_iTextureStageLatest;      // �ֱٿ� ������ TextureStage
};

// St_OcclusionInfo
struct St_OcclusionInfo
{
    BOOL m_bChecked;
    BOOL m_pOccluded;
    void* m_pQuery;
};

// St_ShaderInfo
struct St_ShaderInfo
{
    BOOL m_bShader;
    BOOL m_bEffect;

    union
    {
        // m_bEffect:1
        struct
        {
            int m_iEffect;
            int m_iTechnique;
            int m_iEffectVS;
            int m_iEffectPS;
        };

        // m_bEffect:0
        struct
        {
            int m_iVst;
            int m_iPst;
        };
    };
};

enum ENUM_BILLBOARD_TYPE
{
    EBBT_SPRITE = 0,
    EBBT_CAMERA_UP,
    EBBT_CAMERA_UP_Y,
    EBBT_CAMERA_Z,
    EBBT_NONE,
};

// �ϳ��� Sprite ������ render �Ϸ��� ����� ����
struct SpriteVerts
{
    WzVector m_vVerts;
    DWORD m_dwColor;
    float m_fUV[2];
};

// Alpha �� ����� ��ü��
enum ENUM_ALPHARENDERTYPE
{
    ENUM_ART_MESH = 0,
    ENUM_ART_SPRITE
};

struct St_AlphaRender
{
    ENUM_ALPHARENDERTYPE m_eType;

    ENUM_ALPHABLEND m_abMode;
    WzColor m_wcDraw;

    WzVector m_wvPos;
    float m_fDist;

    union
    {
        // �޽� �� ���
        struct
        {
            int m_iNumFace;
            int m_iSizeForVertex;
            DWORD m_dwFVF;
            BYTE* m_pbyVB;      // vb �� �� ����
            int m_iBufferSize;  // ������ ũ��
            int m_iUsedSize;    // ���� ũ��

            St_RendermeshGroupAnalyzer m_rga;

            // �׸����� ����
            ENUM_MESH_DRAW m_eMeshDrawMode;
            ENUM_CULL_FACE m_eCullFaceMode;
        };

        // ��������Ʈ �� ���
        struct
        {
            HANDLE m_hTexture;  // ����ϴ� �ؽ���
            float m_fSize[2];
            float m_fRotate;
            BOOL m_bUVRegion;   // m_wuvRegion �� ��� ����
            WzUVVert m_wuvRegion[2];
        };
    };
};

// Screen Setting
struct St_ScreenSetting
{
    int m_iWidth;
    int m_iHeight;
    int m_iBpp;     // Bit per pixel
};

struct St_FRect
{
    float fTopX;        // LeftTopX
    float fTopY;        // LeftTopY
    float fWidth;       // Width
    float fHeight;      // Height

    St_FRect()
    {
        fTopX = 0.0f;
        fTopY = 0.0f;
        fWidth = 0.0f;
        fHeight = 0.0f;
    };

    St_FRect( const RECT& Right )
    {
        fTopX = (float)Right.left;
        fTopY = (float)Right.top;
        fWidth = (float)Right.right;
        fHeight = (float)Right.bottom;
    };
};

// New_RenderTriangle's Option
#define NRTO_NONE               (0x00000000)
#define NRTO_ANALYZE_TEXTURE    (0x00000001)
#define NRTO_REFRACTION         (0x00000002)
#define NRTO_STRIP              (0x00000004)
#define NRTO_DEFAULT            NRTO_ANALYZE_TEXTURE

// RenderTexture Option
#define RTO_DEFAULT             (0x00000000)
#define RTO_FLIP_HORIZONTAL     (0x00000001)    // �¿� Flip
#define RTO_FLIP_VERTICAL       (0x00000002)    // ���� Flip
#define RTO_NO_STRIP            (0x10000000)    // Hyper��!

#endif // _3DRENDERER_TYPEDEF_H_
