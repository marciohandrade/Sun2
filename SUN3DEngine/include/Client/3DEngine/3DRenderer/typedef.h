// typedef.h

#ifndef _3DRENDERER_TYPEDEF_H_
#define _3DRENDERER_TYPEDEF_H_

#include <client/3dengine/common/wzcommontypedef.h>
#include <client/3dengine/common/wzmatrix.h>
#include <client/3dengine/common/wzuvvert.h>
#include <client/3dengine/common/wzbvtype.h>

#include <client/3dengine/3dfileaccess/wzunit/wzunitsubtype.h>

// TODO: 제거 예정
// 툴이나 클라이언트에서 쓸 수 있기 때문에 일단 포함한다.
#include "deldefine.h"

#ifdef WZ3DRENDERER_EXPORTS
#define WZRND_API   __declspec( dllexport )
#else
#define WZRND_API   __declspec( dllimport )
#endif

// #define _YJ_PERF_HUD_TEST

// Flag 매크로
#define WzInitFlag( v, flag )       ( v = flag )
#define WzSetFlag( v, flag )        ( v |= flag )
#define WzUnsetFlag( v, flag )      ( v &= ~flag )
#define WzEnableFlag( v, flag, is ) ( (is) ? WzSetFlag( v, flag ) : WzUnsetFlag( v, flag ) )
#define WzCheckFlag( v, flag )      ( flag & v )
#define WzCheckFlagB( v, flag )     ( ( flag & v ) ? TRUE : FALSE )

// Mesh 그리는 방식
enum ENUM_MESH_DRAW
{
    MD_POLYGON = 0,
    MD_WIREFRAME,
    MD_POLYGON_WITHEDGE,    // debug 모드에서만 제대로 작동
    MD_KEEP_CURRENT         // 현재 상태 유지
};

// Cull 방식
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

// 색상 Mask
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



// 화면 모드 - 해상도 색상 포맷
struct St_ScreenFormat
{
    BOOL m_bFullScreen;
    BOOL m_bUseDialogMode;

    // 윈도우 크기
    DWORD m_dwWidth;
    DWORD m_dwHeight;

    // 창모드 일때 최대화 여부
    // m_bFullScreen == FALSE 일때만 사용
    // m_bMaximize == true 이면 window 크기(m_dwWidth, m_dwHeight)를 해상도에 맞게 강제 변환
    //                          (3dsetup.ini 파일에 있는 윈도우값 사용하지 않음)
    bool m_bMaximizeScreen;


    // 툴용 윈도우 스타일(게임 창모드 최대화 생기면서 잡은 변수)
    // 초기 생성 윈도우 스타일 사용
    bool m_bUseInitWindowStyle;

    // 화면 색상 타입 (RGBA)
    int m_iRed;
    int m_iGreen;
    int m_iBlue;
    int m_iAlpha;

    // 기타 버퍼 세팅
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

// RenderCaps - 최대 최소 값들 등의 정보
struct St_RenderCaps
{
    // 텍스쳐 최대 크기
    UINT m_iMaxTextureWidth;
    UINT m_iMaxTextureHeight;

    // 동시에 사용할 수 있는 최대 텍스쳐 수
    int m_iMaxSimultaneousTextures;

    // 스텐실 버퍼
    UINT m_iNumStencilBits;
    
    // MultiSampling 
    int m_iDevMultiSampling;
};

// 카메라 관련 데이터들
enum ENUM_SCREEN_RATIO
{
    SR_AUTO = 0,
    SR_4_3,
    SR_16_9,
    SR_16_10,
};

struct St_Camera
{
    // 기본
    WzMatrix m_wmWorldMatrix;

    // pick 에서 쓰는 것
    WzVector m_wvFrom;
    WzVector m_wvTo;

    // pick 관련 - 카메라 방향으로 1.0f 이동시 가로, 세로로 얼마만큼 이동하는지 비율
    float m_fCameraXRate;
    float m_fCameraYRate;

    // Up vector 와 Right vector
    WzVector m_wvDir;
    WzVector m_wvUp;
    WzVector m_wvRight;

    // Frustum 계산에서 쓰는 것
    BOOL m_bOrthogonal; // Orthogonal 인가?

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

// Fog 관련 파라미터
struct St_FogParameter
{
    WzColor m_wcFogColor;
    float m_fFogStart;
    float m_fFogEnd;
    float m_fMaxFogRate;    // 0 % - 100%
};

// Light 관련 파라미터
enum ENUM_LIGHT_TYPE
{
    ELT_DIRECTIONAL = 0,    // 태양빛
    ELT_POINT,              // 점광원
    NUM_ELT
};

// 라이트의 Irradiance 타입
typedef int ENUM_LIGHT_IRRA_TYPE;   // 차후 형이 정의되면 enum으로 변경~

enum
{
    NUM_ELIT = 4 
};

// 라이트 정보
struct St_LightInformation
{       
    WzColor m_wcColor;      // 빛 색상
    BOOL m_bValid;          // 사용여부
    float m_fRange;         // 범위
    float m_fIntensity;     // 힘(스펙큘라?)

    unsigned long m_OctIndex;       // 옥트린 인덱스    
    ENUM_LIGHT_TYPE m_eltType;      // 빛 종류  
    ENUM_LIGHT_IRRA_TYPE m_eliType; // Irradiance 타입

    union
    {
        struct
        {   // Directional
            WzVector m_wvDirection; // 방향
        };

        struct
        {   // Point
            WzVector m_wvPosition;  // 위치
        };
    };
};

struct St_LightApplyInformation
{
    BOOL bValid; 
    CWzArray<DWORD> arIndex;
};

#define MAX_NUM_TERRAIN_LIGHT   (400)   // 최대 지역 라이트 수
#define MAX_NUM_ACTIVE_LIGHT    (2)     // 최대 활성화 시킬수 있는 라이트 수
#define MAX_DIR_LIGHT           (2)     // 디렉셔널 라이트 수
#define MAX_NUM_LIGHT           (20)    // 최대 라이트 수

// Alpha blending 관련

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
    AB_ALPHA,           // 일반 Alpha ( Alpha 채널 사용)
    AB_LIGHTING,        // 덧칠 Alpha ( Alpha 채널 사용, 기존 색에 더해주는 효과)
    AB_ALPHA_COLOR,     // 색상별 Alpha
    AB_SHADING,         // 쉐이딩
    AB_INV_SHADING,     // 인버스 쉐이딩
    AB_SUBTRACT,        // Src - Dest
    AB_REVSUBTRACT,     // Dest - Src
    NUM_AB,
    AB_KEEP_CURRENT,    // 현재값 유지 (pass 하기)

    // 추가된 내용 (일종의 Customized - low byte 가 src, high byte 가 dest)
    AB_NEW_ALPHA        = MAKEWORD( BF_SRCALPHA, BF_INVSRCALPHA ),
    AB_NEW_LIGHTING     = MAKEWORD( BF_SRCALPHA, BF_ONE ),
    AB_NEW_ALPHA_COLOR  = MAKEWORD( BF_SRCCOLOR, BF_INVSRCCOLOR ),
    AB_NEW_SHADING      = MAKEWORD( BF_ZERO, BF_SRCCOLOR ),
    AB_NEW_INV_SHADING  = MAKEWORD( BF_ZERO, BF_INVSRCCOLOR ),
    AB_NEW_SUBTRACT     = MAKEWORD( BF_SRCALPHA, BF_ONE ),
    AB_NEW_REVSUBTRACT  = MAKEWORD( BF_SRCALPHA, BF_ONE ),
};

// MeshGroup 그리기 정보
struct St_RendermeshGroupAnalyzer
{
    HANDLE m_hTexDiffuse;
    int m_iWithAlpha;   // 0 - alpha 없음, 1 - 일반 alpha, 2 - 뚫리는 이미지
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

// StateParams - 상태 파라미터들
struct St_RenderState
{
    DWORD m_dwRenderState;          // 현재 상태 ( z-buffer 관련 정보 등...)
    WzColor m_wcDraw[NUM_WRIC];     // 그리는 색상(Diffuse, Specular, Ambient)
    ENUM_ALPHABLEND m_abMode;       // Alpha blend 모드
    ENUM_MESH_DRAW m_eMeshDrawMode; // mesh draw 모드
    ENUM_CULL_FACE m_eCullFaceMode; // cull face 모드
    ENUM_CMP_FUNC m_eZFunc;         // Z Func
    DWORD m_dwColorMask;            // Color mask
    DWORD m_dwAlphaTestReference;   // Alpha test 시 비교할 값
    float m_fDepthBias;             // Depth Bias
    float m_fSlopeScaleDepthBias;   // SlopeScaleDepthBias;
};

struct St_StateParams
{
    WzColor m_wcClear;              // 화면 지우는 색
    ENUM_CMP_FUNC m_eAlphaTestFunc; // Alpha test 시 비교 함수
    BOOL m_bEnableAlphaTest;
    BOOL m_bEnableSpecular;         // Specular 사용 유무(default:FALSE)

    // 사용 아직 안함
    int m_iTextureStageCurrent;     // 현재 일반 텍스쳐가 쓰는 TextureStage
    int m_iTextureStageLatest;      // 최근에 세팅한 TextureStage
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

// 하나의 Sprite 정보를 render 하려는 명령을 저장
struct SpriteVerts
{
    WzVector m_vVerts;
    DWORD m_dwColor;
    float m_fUV[2];
};

// Alpha 로 출력할 객체들
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
        // 메쉬 일 경우
        struct
        {
            int m_iNumFace;
            int m_iSizeForVertex;
            DWORD m_dwFVF;
            BYTE* m_pbyVB;      // vb 에 들어갈 내용
            int m_iBufferSize;  // 버퍼의 크기
            int m_iUsedSize;    // 사용된 크기

            St_RendermeshGroupAnalyzer m_rga;

            // 그릴때의 상태
            ENUM_MESH_DRAW m_eMeshDrawMode;
            ENUM_CULL_FACE m_eCullFaceMode;
        };

        // 스프라이트 일 경우
        struct
        {
            HANDLE m_hTexture;  // 사용하는 텍스쳐
            float m_fSize[2];
            float m_fRotate;
            BOOL m_bUVRegion;   // m_wuvRegion 의 사용 여부
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
#define RTO_FLIP_HORIZONTAL     (0x00000001)    // 좌우 Flip
#define RTO_FLIP_VERTICAL       (0x00000002)    // 상하 Flip
#define RTO_NO_STRIP            (0x10000000)    // Hyper용!

#endif // _3DRENDERER_TYPEDEF_H_
