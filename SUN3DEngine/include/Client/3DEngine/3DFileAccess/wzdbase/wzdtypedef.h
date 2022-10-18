// wzdtypedef.h

#ifndef _3DFILEACCESS_WZDTYPEDEF_H_
#define _3DFILEACCESS_WZDTYPEDEF_H_

#include <client/3dengine/common/wzuvvert.h>
#include <client/3dengine/common/wzcolor.h>
#include <client/3dengine/common/wzmath.h>

#include <map>

// ���� : ���(Node)�ϰ� ��(Bone)�ϰ� ���� �ǹ�

// xxx: ���� flag (������ �ɼ����� ������)
// �������� ���� �߰��� �Ŷ� �� ��︮�� �ʴ´�.
// ���� �߰��ϴ� ��� �浹 �ȳ��� ������ ��
#define WZDOP_USE_QUATERNION_FOR_BONE   (0x00000001)
#define WZDOP_SKINNING_SAVE_OFFSET      (0x00000002)    // �� ����
#define WZDOP_ENABLE_SOFTWARESKINNING   (0x00000004)
#define WZDOP_USE_KEYFRAME              (0x00000008)

#define WZDF_LOADED                     (0x00000010)    // �ε� �Ƴ�?
#define WZDF_NEARFACEINFO_GENERATED     (0x00000020)    // ���� �ﰢ�� ������ ���� �Ƴ�?

#define WZDOP_UNIT_LIGHTMAP             (0x00001000)    // ���� ����Ʈ��
#define WZDOP_NO_MULTITHREAD            (0x00002000)    // ��Ƽ������ �ε� ����
#define WZDOP_QUICK_LOAD                (0x00004000)    // �� �ε�

#define WZDOP_USE_BRIGHTNESS            (0x00010000)    // Render �Ҷ� ��� �ֱ�
#define WZDOP_HALFSIZE_TEXTURE          (0x00040000)    // HalfSize Texture
#define WZDOP_INSTANCING                (0x00080000)    // �ν��Ͻ�

// xxx: ���� ȣȯ, ����ô� �״��, �ε��ô� 1/100�� �ε�
#define WZD_SIZEFACTOR_SAVE     1.0f
#define WZD_SIZEFACTOR_LOAD     0.01f

//------------------------------------------------------------------------------
/**
    ������ �ɼ�
*/

// ���̵� Ÿ��
enum ENUM_SHADING_TYPE
{
    ST_DISABLED = 0,            // ���̵� ��
    ST_SOFTWARE_GOURAUD1,       // ����Ʈ���� ��� Gouraud ���̵�(only Diffuse)
    ST_SOFTWARE_GOURAUD2,       // Diffuse + Specular
    ST_SOFTWARE_GOURAUD3,       // Diffuse + Specular (or SpecularMap) + NormalMap
    ST_SOFTWARE_PHONG,          // ����Ʈ���� ��� Phong ���̵�
    ST_SOFTWARE_CARTOON,        // ����Ʈ���� ��� ī��������
};

// �׸��� Ÿ��
enum ENUM_SHADOW_TYPE
{
    SWT_DISABLED = 0,               // �ε��� ��
    SWT_CIRCLESHADOW,               // �� (�������� �׸����� �ʰ�, ���� �󿡼� ���� ���� �ؾ� ��)
    SWT_SHADOWBUFFER,               // �׸��� �� (���� ���� ��� ����)
    SWT_SHADOWMAP_NO_SELFSHADOW,    // �׸��� �� (���� ���� ���, �ڱ� �׸��� ����)
    SWT_SHADOWMAP,                  // �׸��� �� (���� ���� ���, �ڱ� �׸���)
    SWT_SHADOWVOLUME,               // �ε��� ����
};

// �ؽ�ó ����
enum ENUM_TEXTURE_FORMAT
{
    TF_BEST = 0,        // ���� ���� �״�� (�ְ� ȭ��)
    TF_16BIT_CONVERT    // 16��Ʈ�� ����Ʈ �ؼ� ��� (�ӵ� ���)
};

// ������ �ɼ�
struct St_RenderOption
{
    ENUM_SHADING_TYPE m_eShadingType;
    ENUM_SHADOW_TYPE m_eShadowType;

    // 0 : 32bit �ؽ�ó, 1 : 16bit �ؽ�ó
    ENUM_TEXTURE_FORMAT m_eTextureFormat;

    BOOL m_bShaderUse;      // VS & PS ��뿩��
    BOOL m_bUseSpecularMap; // Specular map ��뿩��
    BOOL m_bFogOn;          // Fog ��� ���� (Shader ��� �ÿ��� ����)

    // Mipmap Bias (default : -2.0, MipmapBias�� ī�޶���� �Ÿ��� ���� Mipmap LOD�� ����)
    float m_fMipmapBias;

    int m_iTextureLarge;    // �ؽ�ó �ִ� ũ��
    int m_iTextureSmall;    // �ؽ�ó �ּ� ũ��
    float m_fTextureRate;   // �ؽ�ó ��� ����

    // ���� Ȯ���ڰ� tga�� ���, DDS�� ����(0 : ������, 1 : DDS > TGA, 2 : only DDS)
    int m_iDDS;

    // ���� �븻�� (0 : ������(����Ʈ), 1 : A8L8 with Generated Z(PS 2.0�̻�), 2 : DXT5 with Generated Z(PS 2.0�̻�))
    int m_iCompressedNormalMap;

    int m_iTerrainWater;    // ���� �� ȿ�� (0 : ����, 1 : specular����, 2 : ��� ����)
    int m_iTerrainSpecular; // ���� specular (0 : ����, 1 : ���ؽ�, 2 : �ȼ�)
    int m_iTerrainDetail;   // ���� detail map (0 : ����, 1 : ���)

    int m_iSkinningBone;    // ��Ű�� ���� (���� 2 ~ 3 ����)
    int m_iLightmap;        // ����Ʈ�� ��� ����

    BOOL m_bRenderShadow;
};


//------------------------------------------------------------------------------
/**
    �ִϸ��̼� ����
*/

// ���ؽ��� �ִ�/�ּ� ���� �� ��
#define MAX_BONE_FOR_SKINNING   (4)
#define MIN_BONE_FOR_SKINNING   (2)

// ���ؽ��� ���������� ���� �� ��
#define SUPPORT_BONE_FOR_SKINNING   (3)

// Transform ���� (��ġ, ȸ��)
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

// KeyTransform ����
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

// KeyFrame ����
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

// Node(��) ����
struct WzNode
{
    int m_iParent;  // �θ� �� ��ȣ

    WzNode()
    : m_iParent( -1 )
    {
        // empty
    }
};

// Node �߰� ����
struct WzNodeExtra
{
    char m_szName[64];

    WzNodeExtra()
    {
        memset( m_szName, 0, sizeof( m_szName ) );
    }
};

// ��Ű�� ����
struct WzSkinningInfo
{
    BYTE m_byNumBlended;
    int m_iNode[MAX_BONE_FOR_SKINNING];
    float m_fWeight[MAX_BONE_FOR_SKINNING];
};

// ��ȯ �Ķ����
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
    �޽� ����
*/

#define MGA_WITHALPHA   (0x01)

// �޽� ���ؽ� ����
struct WzMeshVertex
{
    int m_iBone;            // �� ��ȣ
    WzVector m_wvPos;       // ��ġ
};

// �޽� �븻 ����
struct WzMeshNormal
{
    int m_iBone;            // �� ��ȣ
    WzVector m_wvNormal;    // �븻 ����
};

// �޽� ���ؽ� ��
struct WzMeshVertPack
{
    int m_iBone;
    DWORD m_dwVertexIndex;  // ������ Index
    WzVector m_wvNormal;    // ����(Normal) ����
    DWORD m_dwColor;        // Vertex Color
    DWORD m_dwColor2;       // Vertex Color For Light
    WzVector m_wvTangent;   // ����(Tangent) ����
    BYTE m_byBinormalFactor;// (�������̸� 1, �ƴϸ� 0)
    WzUVVert m_wuvVert;     // UV ��
};

// �ﰢ�� ����
struct WzFace
{
    DWORD m_dwVertPack[3];  // ���� Pack  ( WzMeshVertPack �迭�� �ε��� )
    int m_nNearFace[3];     // ���� Face
    BYTE m_bySwitch;        // ���� �뵵�� Switch
};

// ���ؽ� ���̴� ��� ����
struct WzVertexShaderConstantInfo
{
    int m_iSplit;           // ���� ��
    int** m_ppiBoneIndex;   // ���Һ� �� �ε��� ����Ʈ
    int* m_piNumBoneIndex;  // ���Һ� �� ����Ʈ ��

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

// �ؽ�ó Ŭ����
enum ENUM_TEXTURE_CLASS
{
    TXC_DIFFUSE = 0,
    TXC_SPECULAR,
    TXC_NORMALMAP,
    TXC_GLOW,
    NUM_TXC
};

// �޽� �׷��� ���� Ÿ�� (Irradiance ����)
enum ENUM_GROUP_MATERIAL_TYPE
{
    EGMT_NORMAL = 0,
    EGMT_METAL,
    NUM_EGMT
};

// �޽� �׷�
struct WzMeshGroup
{
    char m_szMeshGroupName[MAX_PATH];

    char m_alpszTexture[NUM_TXC][MAX_PATH];
    HANDLE m_ahTexture[NUM_TXC];

    BYTE m_byAttribute;
    DWORD m_dwMaterialType;
    ENUM_GROUP_MATERIAL_TYPE m_eGMType; // ���� Ÿ��

    WzMeshVertPack* m_pwMVPs;
    WzFace* m_pwfFaces;

    int m_iMaxVP;
    int m_iNumVP;
    int m_iStartIndexVP;    // 1���� �����κ��� �����ε�����(�ε�� ��ȿ)

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

// TransformMeshParam ����
struct WzTransformMeshParam
{
    BOOL m_bSkinningIgnore;     // ��Ű�� �����ϱ�
};

#define MAX_MATRIX_PALETTE_SIZE     (25)    // ���̴��� ������ �� �ִ� �ִ� ��� ��
#define MAX_NUM_CHILD_BONE          (32)    // �ִ� �ڽ� �� ��
#define MAX_BONE_SPLIT              (32)    // �ִ� �� ���� �׷� ��

// �� ���� ����
struct St_BoneConnection
{
    int iParent;                            // �θ� ��
    int nChild;                             // �ڽ� ��
    int aiChild[MAX_NUM_CHILD_BONE];        // �ڽĵ� �ε���
    int nRel;                               // ���� ������ �� ��
    int* piRelBone;                         // ���� ������ ���� �ε���
    int iParentCost;                        // ���� �� ��� �ڽ�(leaf����)�� �����ϰ� ���� �߰� �ȵ� �� ��
    int aiChildCost[MAX_NUM_CHILD_BONE];    // �ش� �ڽ� ����(leaf����) ���� �߰� �ȵ� �� ��
    int aiChildCostCal[MAX_NUM_CHILD_BONE]; // ����
};

// �� ���� ����
struct St_BoneSplitInfo 
{
    int* m_apiBone;         // ��� ��ȣ�� �����
    int m_iBone_idx;        // ����� ����
    BOOL* m_apbInserted;    // ��尡 ������ ��� ������(�ߺ� üũ��)
    int* m_apiInsertIndex;  // ����� ���� �ε��� --> Shader�� �� �ε���
};

// �ν��Ͻ� ����
class CWzUnitDraw;

struct St_InstancingInfo
{
    CWzUnitDraw* m_pUnitDraw;
    WzColor m_wcColor;
};

// �� ���� ��ȯ��
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
    // ���� �� ��
    if( lhs.m_byNumBlended != rhs.m_byNumBlended )
    {
        return FALSE;
    }

    WzAssert( lhs.m_byNumBlended <= MAX_BONE_FOR_SKINNING );

    // ��Ű�� ���� �� (��, ����ġ)
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