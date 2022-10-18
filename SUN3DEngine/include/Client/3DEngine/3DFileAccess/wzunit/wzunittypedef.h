// wzunittypedef.h

#ifndef _3DFILEACCESS_WZUNITTYPEDEF_H_
#define _3DFILEACCESS_WZUNITTYPEDEF_H_

#include "../typedef.h"
#include <programcommon/wzarray.h>
#include <client/3dengine/common/wzcolor.h>
#include <client/3dengine/common/wzquaternion.h>
#include <client/3dengine/common/boundingvolume.h>

// �ִ� ���ϸ� ���� (xxx: ���� ȣȯ)
#define MAX_LENGTH_FILENAME             (64)

// ������ LOD �Ÿ� ��
#define NUM_LOD_DISTANCE_VER113         (4)
#define NUM_LOD_DISTANCE_VER129         (6)
#define NUM_NORMAMAP_DISTANCE_VER126    (2)

// ������ ���
#define EWRT_NORMAL             (0x00000001)
#define EWRT_ALPHA              (0x00000002)    // Alpha (a, 1-a)
#define EWRT_LIGHTING           (0x00000004)    // Lighting (a, 1)
#define EWRT_SHADING            (0x00000008)    // Shading (0, c)
#define EWRT_INV_SHADING        (0x00000010)    // Inverse Shading (0, 1-c)
#define MASK_EWRT_RENDER        (0x0000001F)
#define EWRT_MEMBRANE           (0x00000020)    // membraneȿ��
#define EWRT_GLOW               (0x00000040)    // Glow ȿ��
#define EWRT_STREAM             (0x00000100)    // �帣�� ȿ��
#define EWRT_UVNORMAL           (0x00000200)    // texture u,v ����Ҷ� normal ����
#define EWRT_UVLIGHT            (0x00000400)    // texture u,v ����Ҷ� light �� normal ����
#define EWRT_NOSUNLIGHT         (0x00000800)    // Gouraud, Phong �� ���̵� �ȸ԰� �ϱ�
#define EWRT_USEFILTER          (0x00001000)    // Filter �ؽ�ó ���
#define EWRT_FORCESUNLIGHT      (0x00002000)    // Gouraud, Phong �� ���̵� ������ �ϱ�
#define EWRT_REFRACTION         (0x00004000)    // ����
#define EWRT_UV1DNORMAL         (0x00008000)    // texture u,v ����Ҷ� camera dir�� normal ����
#define EWRT_HAZE               (0x00010000)    // �������� ȿ��

#define WRIM_ALL    (-1)
#define WRIM_NONE   (-2)

// �׸��� ����
#define EWST_NOSHADOWCAST       (0x0001)    // �׸��� �� �ֱ�
#define EWST_NOSHADOWRECEIVE    (0x0002)    // �׸��� �� �ޱ�

// �ִ� LOD
#define WZ_MAX_LOD              2           // �� 3���� (0 ~ 2 ����)

// �ٿ�� ���� �뵵
#define BVT_PICKING             (0x00000001)
#define BVT_PATHBOUNDARY        (0x00000002)
#define BVT_USER1               (0x00010000)
#define BVT_USER2               (0x00020000)
#define BVT_USER3               (0x00040000)
#define BVT_USER4               (0x00080000)

// UnitDraw ������ �ɼ�
#define UDR_ENABLE_SKINNING                 (0x00000001)    // ��Ű�� ���� (��Ȱ���� ���, �ϳ��� ���� ����)
#define UDR_ENABLE_NORMALMAP                (0x00000002)    // �븻�� ����
#define UDR_ENABLE_NORMALMAP_REFRACTION     (0x00000004)    // �븻���� ����ؼ� �ȼ��� ����ȿ�� ����
#define UDR_ENABLE_SUBTYPE_ALPHABLEND       (0x00000008)    // ���� Ÿ�� ���ĺ��� ����
#define UDR_ENABLE_CULLFACEMODE_IN          (0x00000010)    // �ø� ��带 ���� �ؽ�ó�� ���� �ٲ��???
#define UDR_ENABLE_UPDATE_LIGHT             (0x00000020)    // ��� �� ��
#define UDR_ENABLE_2PASS_LIGHTINGMODE       (0x00000040)    // ��� ���� (�߰� ������ �������� 2-Pass �����ø��� ������)
#define UDR_ENABLE_SHADOW_ON_SUNLIGHT       (0x00000080)    // �¾� ���� ��쵵 �׸��� ����
#define UDR_ENABLE_SUBTYPE_COLOR            (0x00000100)    // ���� Ÿ�� �÷� ����
#define UDR_ENABLE_EXTERNAL_TEXTURE         (0x00000200)    // �ܺ� �ؽ�ó�� ��ǻ��� ���
#define UDR_ENABLE_LASTALPHA_RENDER         (0x00000400)    // ���ĸ��� ���̳� ���� �Ŀ� �׸���
#define UDR_ENABLE_CUSTOM_LIGHT             (0x00000800)    // Ŀ���� ����Ʈ ���

// UnitDraw ���� �Ķ����
#define UDI_NONE                (0x00000000)
#define UDI_UNIT_EFFECT         (0x00000001)
#define UDI_UNIT_GRASS          (0x00000002)
#define UDI_UNIT_NO_LOD         (UDI_UNIT_EFFECT | UDI_UNIT_GRASS)

// UnitDraw ���� �÷���
#define UDS_LIGHT_INIT          (0x00000001)
#define UDS_RECREATE_EFFECT     (0x00000002)
#define UDS_EFFECT_CREATED      (0x00000004)

// �ܺ� �ؽ�ó ��
#define WZU_NUM_EXTERNAL_TEXTURE    3

// xxx: ���� ȣȯ (���� ����)
enum ENUM_UNIT_AFFECTED_ROTATION
{
    EUAR_ZYX = 0,   // xyz -> xyz(����Ʈ)
    EUAR_YZX,       // xyz -> xzy
    EUAR_ZXY,       // xyz -> yxz
    EUAR_XZY,       // xyz -> yzx
    EUAR_YXZ,       // xyz -> zxy
    EUAR_XYZ        // xyz -> zyx
};

// shader lev Ȯ��
// dx9���� �����Ǵ� �ִ� ���̴��� 3.0 ��.
enum ENUM_SHADER_LEV
{
	eShader_Lev_1_1,
	eShader_Lev_2_0,
	eShader_Lev_2_a,
	eShader_Lve_3_0,
};

// ���� ����
enum ENUM_IPOL_STATE
{
    IS_IPOL_NONE = 0,
    IS_IPOL_ING,
    IS_IPOL_END
};

// �ε� ����
enum ENUM_LOAD_STATE
{
    eLOAD_NONE = 0, // �̻��
    eLOAD_LOW,      // LOW �޽� �ε�
    eLOAD_FULL,     // Ǯ�ε�
};

// ���� Ÿ��
enum ENUM_WRIC
{
    WRIC_DIFFUSE = 0,
    WRIC_SPECULAR,
    WRIC_AMBIENT,
    NUM_WRIC
};

// Decoration Ÿ��
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
    AR_REFRACTION_GLASS,    // ���� - ����
    AR_REFRACTION_CHAOS,    // ���� - �ﷷ�ﷷ
    AR_REFRACTION_INVERSE,  // ���� - ����
    AR_INFLATION,
    NUM_AR_TYPE
};

// �޽� �׷� �̸� ����
struct MeshGroupNameInfo
{
    int nMeshGroupIndex;
    char szMeshGroupName[MAX_PATH];
};

// �� ����
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

// ���� ����
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

// �ؽ��� �ִϸ��̼ǿ� ���� ����
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

// �ؽ��� �ִϸ��̼� ����
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

// �߰� �ٿ�� ���� ����
struct WzBVExtraInfo
{
    DWORD m_dwType;         // ��� �뵵
    WzBoundingVolume m_BV;  // �ٿ�� ����
    int m_iBoneIndex;       // �� ��ȣ - ���϶� ���
    WzVector m_wvOffset;    // ���� ���� ������ - ���϶� ���

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

// �� Ȯ��
struct WzBoneScaling
{
    int m_iNode;        // ��
    WzVector m_wvScale; // ������ (��밪)

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

// Decoration ����ü
struct WzDecoration
{
    ENUM_WZ_DECO_TYPE m_eDecoType;
    int m_iNode;

    union
    {
        // sprite, light ���� ���
        struct
        {
            WzID m_wiTextureToUse;
            float m_fScale;
        };

        // effect ���� ���
        struct
        {
            WzID m_wiEffect;
            BYTE m_bySubType;
            WzVector m_wvPosition;      // ����Ʈ ��ġ
            WzQuaternion m_wqRotation;  // ����Ʈ ����
            WzVector m_wvScale;         // ����Ʈ ũ��
            BOOL m_bRotation;           // ȸ�� ���� �÷���
            WzVector m_wvRotation;      // ����Ʈ ���� (����Ʈ ��)
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

// ������ ����
struct WzRenderInfo
{
    int m_iMesh;                    // �޽� �׷�
    DWORD m_dwRenderType;           // ������ ���

    WzColor m_wcRender[NUM_WRIC];   // �׸��� ����(Diffuse, Specular, Ambient)
    WzID m_wiTextureToUse;          // ������ �ؽ�ó�� �׸� ���

    BYTE m_bySet;                   // ��Ʈ�� ó��
    WORD m_wShadowType;             // �׸��� �ְ� �ޱ� Ÿ��
    WzID m_wiTexAniSet;             // �ؽ�ó �ִϸ��̼� ID

    float m_fGlowFactor;            // �۷ο� �� ����
    BOOL m_bChangeGlowFactor;       // �۷ο� �� ������ ��ȭ ��ų����
    float m_fMinGlowFactor;         // �۷ο� �� ���� �ּҰ�
    float m_fMaxGlowFactor;         // �۷ο� �� ���� �ִ밪
    float m_fTimeToChangeGlowFactor; // �۷ο� �� ���� ��ȭ �ð�

    float m_fHazeDist;              // �������� �Ÿ�
    float m_fHazeScale;             // �������� ������

    float m_fRefractionRate;        // ������
    float m_fRefractionAlpha;       // ����

    float m_fSpecularIntensity;     // ����ŧ������

    float m_fStreamFactor[2];       // �帣�� ȿ���� �帣�� �ӵ� ����
    WzVector m_wvUVNormalFactor[2]; // texture u,v ����Ҷ� normal �����ϴ� ������

    WzID m_wiTextureFilter;         // ���ͷ� ����ϴ� �ؽ�ó (�ո� �̹���), EWRT_USEFILTER �� ���� �� ���
    BOOL m_bUseAlphaFilter;         // ���� �ؽ�ó�� ���� ä�θ� �� ������

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

// ���� ����Ʈ ����
struct WzUnitEffectInfo
{
    HANDLE m_hEffect;       // ����Ʈ �ڵ�
    int m_iNode;            // ����Ʈ�� �ٴ� �� ��ȣ
    WzVector m_wvPosition;  // ����Ʈ ��ġ
    WzQuaternion m_wqRotation; // ����Ʈ ����
    WzVector m_wvScale;
    BOOL m_bRotation;       // ȸ�� ���� �÷���
};

// �߰� �ؽ�ó ����
struct St_ExternalTextureInfo
{
    int m_iMeshGroup;   
    HANDLE m_hTexture;  
};

// �ִϸ��̼� ���� ����
struct WzAniBlendInfo
{
    int m_iStartFrame;
    int m_iEndFrame;
};

// ���� Transform Offset
struct WzUnitTransformOffset
{
    BOOL m_bPosUse;
    BOOL m_bRotUse;
    WzVector m_wvPos;
    WzVector m_wvRot;
};

#endif // _3DFILEACCESS_WZUNITTYPEDEF_H_