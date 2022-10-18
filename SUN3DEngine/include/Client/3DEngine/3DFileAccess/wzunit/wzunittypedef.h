// wzunittypedef.h

#ifndef _3DFILEACCESS_WZUNITTYPEDEF_H_
#define _3DFILEACCESS_WZUNITTYPEDEF_H_

#include "../typedef.h"
#include <programcommon/wzarray.h>
#include <client/3dengine/common/wzcolor.h>
#include <client/3dengine/common/wzquaternion.h>
#include <client/3dengine/common/boundingvolume.h>

// 최대 파일명 길이 (xxx: 기존 호환)
#define MAX_LENGTH_FILENAME             (64)

// 버전별 LOD 거리 수
#define NUM_LOD_DISTANCE_VER113         (4)
#define NUM_LOD_DISTANCE_VER129         (6)
#define NUM_NORMAMAP_DISTANCE_VER126    (2)

// 렌더링 방식
#define EWRT_NORMAL             (0x00000001)
#define EWRT_ALPHA              (0x00000002)    // Alpha (a, 1-a)
#define EWRT_LIGHTING           (0x00000004)    // Lighting (a, 1)
#define EWRT_SHADING            (0x00000008)    // Shading (0, c)
#define EWRT_INV_SHADING        (0x00000010)    // Inverse Shading (0, 1-c)
#define MASK_EWRT_RENDER        (0x0000001F)
#define EWRT_MEMBRANE           (0x00000020)    // membrane효과
#define EWRT_GLOW               (0x00000040)    // Glow 효과
#define EWRT_STREAM             (0x00000100)    // 흐르는 효과
#define EWRT_UVNORMAL           (0x00000200)    // texture u,v 계산할때 normal 참조
#define EWRT_UVLIGHT            (0x00000400)    // texture u,v 계산할때 light 와 normal 참조
#define EWRT_NOSUNLIGHT         (0x00000800)    // Gouraud, Phong 등 쉐이딩 안먹게 하기
#define EWRT_USEFILTER          (0x00001000)    // Filter 텍스처 사용
#define EWRT_FORCESUNLIGHT      (0x00002000)    // Gouraud, Phong 등 쉐이딩 무조건 하기
#define EWRT_REFRACTION         (0x00004000)    // 굴절
#define EWRT_UV1DNORMAL         (0x00008000)    // texture u,v 계산할때 camera dir과 normal 참조
#define EWRT_HAZE               (0x00010000)    // 아지랑이 효과

#define WRIM_ALL    (-1)
#define WRIM_NONE   (-2)

// 그림자 관련
#define EWST_NOSHADOWCAST       (0x0001)    // 그림자 안 주기
#define EWST_NOSHADOWRECEIVE    (0x0002)    // 그림자 안 받기

// 최대 LOD
#define WZ_MAX_LOD              2           // 총 3레벨 (0 ~ 2 레벨)

// 바운딩 볼륨 용도
#define BVT_PICKING             (0x00000001)
#define BVT_PATHBOUNDARY        (0x00000002)
#define BVT_USER1               (0x00010000)
#define BVT_USER2               (0x00020000)
#define BVT_USER3               (0x00040000)
#define BVT_USER4               (0x00080000)

// UnitDraw 렌더링 옵션
#define UDR_ENABLE_SKINNING                 (0x00000001)    // 스키닝 적용 (비활성인 경우, 하나의 본만 참조)
#define UDR_ENABLE_NORMALMAP                (0x00000002)    // 노말맵 적용
#define UDR_ENABLE_NORMALMAP_REFRACTION     (0x00000004)    // 노말맵을 사용해서 픽셀당 굴절효과 적용
#define UDR_ENABLE_SUBTYPE_ALPHABLEND       (0x00000008)    // 서브 타입 알파블랜딩 적용
#define UDR_ENABLE_CULLFACEMODE_IN          (0x00000010)    // 컬링 모드를 내부 텍스처에 따라 바뀌도록???
#define UDR_ENABLE_UPDATE_LIGHT             (0x00000020)    // 사용 안 함
#define UDR_ENABLE_2PASS_LIGHTINGMODE       (0x00000040)    // 사용 안함 (추가 렌더링 정보없이 2-Pass 라이팅모드로 렌더링)
#define UDR_ENABLE_SHADOW_ON_SUNLIGHT       (0x00000080)    // 태양 빛인 경우도 그림자 적용
#define UDR_ENABLE_SUBTYPE_COLOR            (0x00000100)    // 서브 타입 컬러 적용
#define UDR_ENABLE_EXTERNAL_TEXTURE         (0x00000200)    // 외부 텍스처를 디퓨즈로 사용
#define UDR_ENABLE_LASTALPHA_RENDER         (0x00000400)    // 알파모드시 물이나 굴절 후에 그리기
#define UDR_ENABLE_CUSTOM_LIGHT             (0x00000800)    // 커스텀 라이트 사용

// UnitDraw 생성 파라미터
#define UDI_NONE                (0x00000000)
#define UDI_UNIT_EFFECT         (0x00000001)
#define UDI_UNIT_GRASS          (0x00000002)
#define UDI_UNIT_NO_LOD         (UDI_UNIT_EFFECT | UDI_UNIT_GRASS)

// UnitDraw 상태 플래그
#define UDS_LIGHT_INIT          (0x00000001)
#define UDS_RECREATE_EFFECT     (0x00000002)
#define UDS_EFFECT_CREATED      (0x00000004)

// 외부 텍스처 수
#define WZU_NUM_EXTERNAL_TEXTURE    3

// xxx: 기존 호환 (제거 예정)
enum ENUM_UNIT_AFFECTED_ROTATION
{
    EUAR_ZYX = 0,   // xyz -> xyz(디폴트)
    EUAR_YZX,       // xyz -> xzy
    EUAR_ZXY,       // xyz -> yxz
    EUAR_XZY,       // xyz -> yzx
    EUAR_YXZ,       // xyz -> zxy
    EUAR_XYZ        // xyz -> zyx
};

// shader lev 확인
// dx9에서 지원되는 최대 쉐이더는 3.0 임.
enum ENUM_SHADER_LEV
{
	eShader_Lev_1_1,
	eShader_Lev_2_0,
	eShader_Lev_2_a,
	eShader_Lve_3_0,
};

// 보간 상태
enum ENUM_IPOL_STATE
{
    IS_IPOL_NONE = 0,
    IS_IPOL_ING,
    IS_IPOL_END
};

// 로딩 상태
enum ENUM_LOAD_STATE
{
    eLOAD_NONE = 0, // 미사용
    eLOAD_LOW,      // LOW 메시 로드
    eLOAD_FULL,     // 풀로드
};

// 색상 타입
enum ENUM_WRIC
{
    WRIC_DIFFUSE = 0,
    WRIC_SPECULAR,
    WRIC_AMBIENT,
    NUM_WRIC
};

// Decoration 타입
enum ENUM_WZ_DECO_TYPE
{
    EWDT_SPRITE = 0,
    EWDT_LIGHT,
    EWDT_EFFECT,
};

enum ENUM_ALTERNATIVE_RENDER
{
    AR_NORMAL = 0,
    AR_UNIT_MOTIONBLUR,
    AR_REFRACTION_GLASS,    // 굴절 - 유리
    AR_REFRACTION_CHAOS,    // 굴절 - 울렁울렁
    AR_REFRACTION_INVERSE,  // 굴절 - 반전
    AR_INFLATION,
    NUM_AR_TYPE
};

// 메시 그룹 이름 정보
struct MeshGroupNameInfo
{
    int nMeshGroupIndex;
    char szMeshGroupName[MAX_PATH];
};

// 본 정보
struct WzBoneInfo
{
    WzID m_wiBone;
    int m_iNode;

    WzBoneInfo()
    : m_wiBone( 0 )
    , m_iNode( 0 )
    {
        // empty
    }

    WzBoneInfo& operator = ( const WzBoneInfo& rhs )
    {
        memcpy( this, &rhs, sizeof( WzBoneInfo ) );

        return *this;
    }
};

// 맵핑 정보
struct WzMappingInfo
{
    char m_lpszFileName[MAX_LENGTH_FILENAME];
    HANDLE m_hTexture;

    WzMappingInfo()
    : m_hTexture( INVALID_HANDLE_VALUE )
    {
        memset( m_lpszFileName, 0, sizeof( m_lpszFileName ) );
    }

    WzMappingInfo& operator = ( const WzMappingInfo& rhs )
    {
        strcpy( m_lpszFileName, rhs.m_lpszFileName );
        m_hTexture = rhs.m_hTexture;

        return *this;
    }
};

// 텍스쳐 애니메이션용 맵핑 정보
struct WzTexAniMappingInfo
{
    WzID m_wiID;
    WzMappingInfo m_MappingInfo;

    WzTexAniMappingInfo()
    : m_wiID( 0 )
    {
        // empty
    }

    WzTexAniMappingInfo& operator = ( const WzTexAniMappingInfo& rhs )
    {
        m_wiID = rhs.m_wiID;
        m_MappingInfo = rhs.m_MappingInfo;

        return *this;
    }
};

// 텍스쳐 애니메이션 정보
struct WzTexAniSetInfo
{
    WzID m_wiID;
    float m_fLoopTime;
    CWzArray<WzID> m_arTexID;

    WzTexAniSetInfo()
    : m_wiID( 0 )
    , m_fLoopTime( 0.0f )
    {
        m_arTexID.Clear();
    }

    void AddTexID( WzID wiID )
    {
        m_arTexID.Add( wiID ); 
    }

    WzTexAniSetInfo& operator = ( const WzTexAniSetInfo& rhs )
    {
        m_wiID = rhs.m_wiID;
        m_fLoopTime = rhs.m_fLoopTime;
        m_arTexID = rhs.m_arTexID;

        return *this;
    }
};

// 추가 바운딩 볼륨 정보
struct WzBVExtraInfo
{
    DWORD m_dwType;         // 사용 용도
    WzBoundingVolume m_BV;  // 바운딩 볼륨
    int m_iBoneIndex;       // 본 번호 - 구일때 사용
    WzVector m_wvOffset;    // 본에 대한 오프셋 - 구일때 사용

    WzBVExtraInfo()
    : m_dwType( 0 )
    , m_iBoneIndex( -1 )
    {
        SetVector( &m_wvOffset, 0.0f, 0.0f, 0.0f );
    }

    WzBVExtraInfo& operator = ( const WzBVExtraInfo& rhs )
    {
        memcpy( this, &rhs, sizeof( WzBVExtraInfo ) );

        return *this;
    }
};

// 본 확대
struct WzBoneScaling
{
    int m_iNode;        // 본
    WzVector m_wvScale; // 스케일 (상대값)

    WzBoneScaling()
    : m_iNode( 0 )
    {
        SetVector( &m_wvScale, 1.0f, 1.0f, 1.0f );
    }

    WzBoneScaling& operator = ( const WzBoneScaling& rhs )
    {
        memcpy( this, &rhs, sizeof( WzBoneScaling ) );

        return *this;
    }
};

// Decoration 구조체
struct WzDecoration
{
    ENUM_WZ_DECO_TYPE m_eDecoType;
    int m_iNode;

    union
    {
        // sprite, light 에서 사용
        struct
        {
            WzID m_wiTextureToUse;
            float m_fScale;
        };

        // effect 에서 사용
        struct
        {
            WzID m_wiEffect;
            BYTE m_bySubType;
            WzVector m_wvPosition;      // 이펙트 위치
            WzQuaternion m_wqRotation;  // 이펙트 방향
            WzVector m_wvScale;         // 이펙트 크기
            BOOL m_bRotation;           // 회전 적용 플래그
            WzVector m_wvRotation;      // 이펙트 방향 (에디트 용)
        };
    };

    WzDecoration()
    {
        memset( this, 0, sizeof( WzDecoration ) );
    }

    WzDecoration& operator = ( const WzDecoration& rhs )
    {
        memcpy( this, &rhs, sizeof( WzDecoration ) );

        return *this;
    }
};

// 렌더링 정보
struct WzRenderInfo
{
    int m_iMesh;                    // 메시 그룹
    DWORD m_dwRenderType;           // 렌더링 방식

    WzColor m_wcRender[NUM_WRIC];   // 그리는 색상(Diffuse, Specular, Ambient)
    WzID m_wiTextureToUse;          // 별도의 텍스처로 그릴 경우

    BYTE m_bySet;                   // 세트로 처리
    WORD m_wShadowType;             // 그림자 주고 받기 타입
    WzID m_wiTexAniSet;             // 텍스처 애니메이션 ID

    float m_fGlowFactor;            // 글로우 맵 강도
    BOOL m_bChangeGlowFactor;       // 글로우 맵 강도를 변화 시킬건지
    float m_fMinGlowFactor;         // 글로우 맵 강도 최소값
    float m_fMaxGlowFactor;         // 글로우 맵 강도 최대값
    float m_fTimeToChangeGlowFactor; // 글로우 맵 강도 변화 시간

    float m_fHazeDist;              // 아지랑이 거리
    float m_fHazeScale;             // 아지랑이 스케일

    float m_fRefractionRate;        // 굴절률
    float m_fRefractionAlpha;       // 투명도

    float m_fSpecularIntensity;     // 스펙큘러강도

    float m_fStreamFactor[2];       // 흐르는 효과의 흐르는 속도 지정
    WzVector m_wvUVNormalFactor[2]; // texture u,v 계산할때 normal 참조하는 정도값

    WzID m_wiTextureFilter;         // 필터로 사용하는 텍스처 (뚫린 이미지), EWRT_USEFILTER 가 있을 때 사용
    BOOL m_bUseAlphaFilter;         // 필터 텍스처에 알파 채널만 쓸 것인지

    WzRenderInfo()
    : m_iMesh( WRIM_ALL )
    , m_dwRenderType( EWRT_NORMAL )
    , m_wiTextureToUse( 0 )
    , m_bySet( 0 )
    , m_wShadowType( 0 )
    , m_wiTexAniSet( 0 )
    , m_fGlowFactor( 1.0f )
    , m_bChangeGlowFactor( FALSE )
    , m_fMinGlowFactor( 0.0f )
    , m_fMaxGlowFactor( 1.0f )
    , m_fTimeToChangeGlowFactor( 1.0f )
    , m_fHazeDist( 500.0f )
    , m_fHazeScale( 0.5f )
    , m_fRefractionRate( 1.0f )
    , m_fRefractionAlpha( 0.5f )
    , m_fSpecularIntensity( 100.0f )
    , m_wiTextureFilter( 0 )
    , m_bUseAlphaFilter( FALSE )
    {
        m_wcRender[WRIC_DIFFUSE] = WzColor_RGB( 255, 255, 255 );
        m_wcRender[WRIC_SPECULAR] = WzColor_RGB( 255, 255, 255 );
        m_wcRender[WRIC_AMBIENT] = WzColor_RGB( 128, 128, 128 );

        m_fStreamFactor[0] = 0.0f;
        m_fStreamFactor[1] = 0.0f;

        SetVector( &m_wvUVNormalFactor[0], 0.0f, 0.0f, 0.0f );
        SetVector( &m_wvUVNormalFactor[1], 0.0f, 0.0f, 0.0f );
    }

    WzRenderInfo& operator = ( const WzRenderInfo& rhs )
    {
        memcpy( this, &rhs, sizeof( WzRenderInfo ) );

        return *this;
    }
};

// 유닛 이펙트 정보
struct WzUnitEffectInfo
{
    HANDLE m_hEffect;       // 이펙트 핸들
    int m_iNode;            // 이펙트가 붙는 본 번호
    WzVector m_wvPosition;  // 이펙트 위치
    WzQuaternion m_wqRotation; // 이펙트 방향
    WzVector m_wvScale;
    BOOL m_bRotation;       // 회전 적용 플래그
};

// 추가 텍스처 정보
struct St_ExternalTextureInfo
{
    int m_iMeshGroup;   
    HANDLE m_hTexture;  
};

// 애니메이션 블랜딩 정보
struct WzAniBlendInfo
{
    int m_iStartFrame;
    int m_iEndFrame;
};

// 유닛 Transform Offset
struct WzUnitTransformOffset
{
    BOOL m_bPosUse;
    BOOL m_bRotUse;
    WzVector m_wvPos;
    WzVector m_wvRot;
};

#endif // _3DFILEACCESS_WZUNITTYPEDEF_H_