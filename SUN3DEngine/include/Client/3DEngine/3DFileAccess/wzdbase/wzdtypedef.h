// wzdtypedef.h

#ifndef _3DFILEACCESS_WZDTYPEDEF_H_
#define _3DFILEACCESS_WZDTYPEDEF_H_

#include <client/3dengine/common/wzuvvert.h>
#include <client/3dengine/common/wzcolor.h>
#include <client/3dengine/common/wzmath.h>

#include <map>

// 주의 : 노드(Node)하고 본(Bone)하고 같은 의미

// xxx: 각종 flag (원래는 옵션으로 쓰였음)
// 기존꺼랑 새로 추가한 거랑 잘 어울리지 않는다.
// 새로 추가하는 경우 충돌 안나게 조심할 것
#define WZDOP_USE_QUATERNION_FOR_BONE   (0x00000001)
#define WZDOP_SKINNING_SAVE_OFFSET      (0x00000002)    // 안 쓰임
#define WZDOP_ENABLE_SOFTWARESKINNING   (0x00000004)
#define WZDOP_USE_KEYFRAME              (0x00000008)

#define WZDF_LOADED                     (0x00000010)    // 로딩 됐나?
#define WZDF_NEARFACEINFO_GENERATED     (0x00000020)    // 인접 삼각형 정보가 생성 됐나?

#define WZDOP_UNIT_LIGHTMAP             (0x00001000)    // 유닛 라이트맵
#define WZDOP_NO_MULTITHREAD            (0x00002000)    // 멀티쓰레드 로딩 안함
#define WZDOP_QUICK_LOAD                (0x00004000)    // 퀵 로드

#define WZDOP_USE_BRIGHTNESS            (0x00010000)    // Render 할때 명암 넣기
#define WZDOP_HALFSIZE_TEXTURE          (0x00040000)    // HalfSize Texture
#define WZDOP_INSTANCING                (0x00080000)    // 인스턴싱

// xxx: 기존 호환, 저장시는 그대로, 로딩시는 1/100로 로딩
#define WZD_SIZEFACTOR_SAVE     1.0f
#define WZD_SIZEFACTOR_LOAD     0.01f

//------------------------------------------------------------------------------
/**
    렌더링 옵션
*/

// 셰이딩 타입
enum ENUM_SHADING_TYPE
{
    ST_DISABLED = 0,            // 셰이딩 끔
    ST_SOFTWARE_GOURAUD1,       // 소프트웨어 계산 Gouraud 셰이딩(only Diffuse)
    ST_SOFTWARE_GOURAUD2,       // Diffuse + Specular
    ST_SOFTWARE_GOURAUD3,       // Diffuse + Specular (or SpecularMap) + NormalMap
    ST_SOFTWARE_PHONG,          // 소프트웨어 계산 Phong 셰이딩
    ST_SOFTWARE_CARTOON,        // 소프트웨어 계산 카툰렌더링
};

// 그림자 타입
enum ENUM_SHADOW_TYPE
{
    SWT_DISABLED = 0,               // 셰도우 끔
    SWT_CIRCLESHADOW,               // 원 (엔진에서 그리지는 않고, 게임 상에서 직접 구현 해야 함)
    SWT_SHADOWBUFFER,               // 그림자 맵 (깊이 버퍼 사용 안함)
    SWT_SHADOWMAP_NO_SELFSHADOW,    // 그림자 맵 (깊이 버퍼 사용, 자기 그림자 없음)
    SWT_SHADOWMAP,                  // 그림자 맵 (깊이 버퍼 사용, 자기 그림자)
    SWT_SHADOWVOLUME,               // 셰도우 볼륨
};

// 텍스처 포맷
enum ENUM_TEXTURE_FORMAT
{
    TF_BEST = 0,        // 원래 포맷 그대로 (최고 화질)
    TF_16BIT_CONVERT    // 16비트로 컨버트 해서 사용 (속도 향상)
};

// 렌더링 옵션
struct St_RenderOption
{
    ENUM_SHADING_TYPE m_eShadingType;
    ENUM_SHADOW_TYPE m_eShadowType;

    // 0 : 32bit 텍스처, 1 : 16bit 텍스처
    ENUM_TEXTURE_FORMAT m_eTextureFormat;

    BOOL m_bShaderUse;      // VS & PS 사용여부
    BOOL m_bUseSpecularMap; // Specular map 사용여부
    BOOL m_bFogOn;          // Fog 사용 여부 (Shader 사용 시에만 가능)

    // Mipmap Bias (default : -2.0, MipmapBias와 카메라와의 거리에 따라 Mipmap LOD를 결정)
    float m_fMipmapBias;

    int m_iTextureLarge;    // 텍스처 최대 크기
    int m_iTextureSmall;    // 텍스처 최소 크기
    float m_fTextureRate;   // 텍스처 축소 비율

    // 파일 확장자가 tga일 경우, DDS로 읽음(0 : 사용안함, 1 : DDS > TGA, 2 : only DDS)
    int m_iDDS;

    // 압축 노말맵 (0 : 사용안함(디폴트), 1 : A8L8 with Generated Z(PS 2.0이상), 2 : DXT5 with Generated Z(PS 2.0이상))
    int m_iCompressedNormalMap;

    int m_iTerrainWater;    // 지형 물 효과 (0 : 없음, 1 : specular없음, 2 : 모두 있음)
    int m_iTerrainSpecular; // 지형 specular (0 : 없음, 1 : 버텍스, 2 : 픽셀)
    int m_iTerrainDetail;   // 지형 detail map (0 : 없음, 1 : 사용)

    int m_iSkinningBone;    // 스키닝 본수 (현재 2 ~ 3 지원)
    int m_iLightmap;        // 라이트맵 사용 여부

    BOOL m_bRenderShadow;
};


//------------------------------------------------------------------------------
/**
    애니메이션 정보
*/

// 버텍스당 최대/최소 블랜딩 본 수
#define MAX_BONE_FOR_SKINNING   (4)
#define MIN_BONE_FOR_SKINNING   (2)

// 버텍스당 지원가능한 블렌딩 본 수
#define SUPPORT_BONE_FOR_SKINNING   (3)

// Transform 정보 (위치, 회전)
struct WzTransform
{
    WzVector m_wvPos;
    WzQuaternion m_wqQuat;

    WzTransform()
    {
        Init();
    }

    void Init()
    {
        m_wvPos.x = m_wvPos.y = m_wvPos.z = 0.0f;
        m_wqQuat.x = m_wqQuat.y = m_wqQuat.z = 0.0f;
        m_wqQuat.w = 1.0f;
    }
};

// KeyTransform 정보
struct WzKeyTransform
{
    WzTransform m_wtTransform;
    WORD m_wKey;

    WzKeyTransform()
    : m_wKey( 0 )
    {
        // empty
    }
};

// KeyFrame 정보
struct WzKeyFrame
{
    WzKeyTransform* m_pKeyTransforms;
    WORD* m_pwKeyTable;
    int m_wNumKey;

    WzKeyFrame()
    : m_pKeyTransforms( NULL )
    , m_pwKeyTable( NULL )
    , m_wNumKey( 0 )
    {
        // empty
    }

    ~WzKeyFrame()
    {
        SAFE_DELETE_ARRAY( m_pwKeyTable );
        SAFE_DELETE_ARRAY( m_pKeyTransforms );
    }
};

// Node(본) 정보
struct WzNode
{
    int m_iParent;  // 부모 본 번호

    WzNode()
    : m_iParent( -1 )
    {
        // empty
    }
};

// Node 추가 정보
struct WzNodeExtra
{
    char m_szName[64];

    WzNodeExtra()
    {
        memset( m_szName, 0, sizeof( m_szName ) );
    }
};

// 스키닝 정보
struct WzSkinningInfo
{
    BYTE m_byNumBlended;
    int m_iNode[MAX_BONE_FOR_SKINNING];
    float m_fWeight[MAX_BONE_FOR_SKINNING];
};

// 변환 파라미터
struct WzTransformParam
{
    std::map<int, WzVector> m_scaleBoneMap;

    WzVector m_wvScale;
    WzMatrix m_wmRot;
    WzVector m_wvPos;

    WzMatrix* m_pwmParent;
};

//------------------------------------------------------------------------------
/**
    메시 정보
*/

#define MGA_WITHALPHA   (0x01)

// 메시 버텍스 정보
struct WzMeshVertex
{
    int m_iBone;            // 본 번호
    WzVector m_wvPos;       // 위치
};

// 메시 노말 정보
struct WzMeshNormal
{
    int m_iBone;            // 본 번호
    WzVector m_wvNormal;    // 노말 벡터
};

// 메시 버텍스 팩
struct WzMeshVertPack
{
    int m_iBone;
    DWORD m_dwVertexIndex;  // 정점의 Index
    WzVector m_wvNormal;    // 법선(Normal) 벡터
    DWORD m_dwColor;        // Vertex Color
    DWORD m_dwColor2;       // Vertex Color For Light
    WzVector m_wvTangent;   // 접선(Tangent) 벡터
    BYTE m_byBinormalFactor;// (정방향이면 1, 아니면 0)
    WzUVVert m_wuvVert;     // UV 값
};

// 삼각형 정보
struct WzFace
{
    DWORD m_dwVertPack[3];  // 정점 Pack  ( WzMeshVertPack 배열의 인덱스 )
    int m_nNearFace[3];     // 인접 Face
    BYTE m_bySwitch;        // 여러 용도의 Switch
};

// 버텍스 셰이더 상수 정보
struct WzVertexShaderConstantInfo
{
    int m_iSplit;           // 분할 수
    int** m_ppiBoneIndex;   // 분할별 본 인덱스 리스트
    int* m_piNumBoneIndex;  // 분할별 본 리스트 수

    WzVertexShaderConstantInfo()
    : m_iSplit( 0 )
    , m_ppiBoneIndex( NULL )
    , m_piNumBoneIndex( NULL )
    {
        // empty
    }

    ~WzVertexShaderConstantInfo()
    {
        if( m_ppiBoneIndex )
        {
            for( int i = 0; i < m_iSplit; ++i )
            {
                SAFE_DELETE_ARRAY( m_ppiBoneIndex[i] );
            }

            delete [] m_ppiBoneIndex;
            m_ppiBoneIndex = NULL;
        }

        SAFE_DELETE_ARRAY( m_piNumBoneIndex );
            
        m_iSplit = 0;
    }
};

// 텍스처 클래스
enum ENUM_TEXTURE_CLASS
{
    TXC_DIFFUSE = 0,
    TXC_SPECULAR,
    TXC_NORMALMAP,
    TXC_GLOW,
    NUM_TXC
};

// 메시 그룹의 재질 타입 (Irradiance 관련)
enum ENUM_GROUP_MATERIAL_TYPE
{
    EGMT_NORMAL = 0,
    EGMT_METAL,
    NUM_EGMT
};

// 메시 그룹
struct WzMeshGroup
{
    char m_szMeshGroupName[MAX_PATH];

    char m_alpszTexture[NUM_TXC][MAX_PATH];
    HANDLE m_ahTexture[NUM_TXC];

    BYTE m_byAttribute;
    DWORD m_dwMaterialType;
    ENUM_GROUP_MATERIAL_TYPE m_eGMType; // 재질 타입

    WzMeshVertPack* m_pwMVPs;
    WzFace* m_pwfFaces;

    int m_iMaxVP;
    int m_iNumVP;
    int m_iStartIndexVP;    // 1차원 정보로부터 시작인덱스값(로드시 유효)

    int m_iMaxFace;
    int m_iNumFace;

    HANDLE m_hOwnVertexBuffer;
    HANDLE m_hOwnIndexBuffer;

    WzVertexShaderConstantInfo* m_pVSCI;

    WzMeshGroup()
    : m_byAttribute( 0 )
    , m_dwMaterialType( 0 )
    , m_eGMType( EGMT_NORMAL )
    , m_pwMVPs( NULL )
    , m_pwfFaces( NULL )
    , m_iMaxVP( 0 )
    , m_iNumVP( 0 )
    , m_iStartIndexVP( 0 )
    , m_iMaxFace( 0 )
    , m_iNumFace( 0 )
    , m_hOwnVertexBuffer( INVALID_HANDLE_VALUE )
    , m_hOwnIndexBuffer( INVALID_HANDLE_VALUE )
    , m_pVSCI( NULL )
    {
        strcpy( m_szMeshGroupName, "No Group Name" );

        memset( m_alpszTexture, 0, sizeof( m_alpszTexture ) );
        memset( m_ahTexture, (int)INVALID_HANDLE_VALUE, sizeof( m_ahTexture ) );
    }
};

// TransformMeshParam 정보
struct WzTransformMeshParam
{
    BOOL m_bSkinningIgnore;     // 스키닝 무시하기
};

#define MAX_MATRIX_PALETTE_SIZE     (25)    // 셰이더에 세팅할 수 있는 최대 행렬 수
#define MAX_NUM_CHILD_BONE          (32)    // 최대 자식 본 수
#define MAX_BONE_SPLIT              (32)    // 최대 본 분할 그룹 수

// 본 연결 정보
struct St_BoneConnection
{
    int iParent;                            // 부모 본
    int nChild;                             // 자식 수
    int aiChild[MAX_NUM_CHILD_BONE];        // 자식들 인덱스
    int nRel;                               // 나와 연관된 본 수
    int* piRelBone;                         // 나와 연관된 본들 인덱스
    int iParentCost;                        // 나와 내 모든 자식(leaf까지)을 제외하고 아직 추가 안된 본 수
    int aiChildCost[MAX_NUM_CHILD_BONE];    // 해당 자식 이하(leaf까지) 아직 추가 안된 본 수
    int aiChildCostCal[MAX_NUM_CHILD_BONE]; // 계산용
};

// 본 분할 정보
struct St_BoneSplitInfo 
{
    int* m_apiBone;         // 노드 번호가 저장됨
    int m_iBone_idx;        // 저장된 개수
    BOOL* m_apbInserted;    // 노드가 이전에 들어 갔는지(중복 체크용)
    int* m_apiInsertIndex;  // 저장될 때의 인덱스 --> Shader에 들어갈 인덱스
};

// 인스턴스 정보
class CWzUnitDraw;

struct St_InstancingInfo
{
    CWzUnitDraw* m_pUnitDraw;
    WzColor m_wcColor;
};

// 구 버전 변환용
struct WzFace_Conv
{
    WORD m_wVertex[3];
    WORD m_wNormal[3];
    WORD m_wUV[3];
};

struct WzMeshGroup_Conv
{
    char m_szDiffuseTexName[MAX_PATH];

    WzUVVert* m_pwUVs;
    WzFace_Conv* m_pwfFaces;

    int m_iNumUV;
    int m_iNumFace;
};

//------------------------------------------------------------------------------
/**
*/
inline BOOL operator == ( const WzSkinningInfo& lhs, const WzSkinningInfo& rhs )
{
    // 블랜딩 수 비교
    if( lhs.m_byNumBlended != rhs.m_byNumBlended )
    {
        return FALSE;
    }

    WzAssert( lhs.m_byNumBlended <= MAX_BONE_FOR_SKINNING );

    // 스키닝 정보 비교 (본, 가중치)
    for( int i = 0; i < lhs.m_byNumBlended; ++i )
    {
        if( lhs.m_iNode[i] != rhs.m_iNode[i] ||
            lhs.m_fWeight[i] != rhs.m_fWeight[i] )
        {
            return FALSE;
        }
    }

    return TRUE;
}

#endif // _3DFILEACCESS_WZDTYPEDEF_H_