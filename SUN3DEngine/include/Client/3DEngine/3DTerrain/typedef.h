// typedef.h

#ifndef _3DTERRAIN_TYPEDEF_H_
#define _3DTERRAIN_TYPEDEF_H_

#include <client/3dengine/common/wzcommontypedef.h>
#include <client/3dengine/common/boundingvolume.h>

// TODO: ���� ����
// ���̳� Ŭ���̾�Ʈ���� �� �� �ֱ� ������ �ϴ� �����Ѵ�.
#include "deldefine.h"

#ifdef WZ3DTERRAIN_EXPORTS
#define WZTER_API   __declspec( dllexport )
#else
#define WZTER_API   __declspec( dllimport )
#endif

#define VERSION_WEBZEN_MAP      (179)
#define VERSION_MAPOBJECT       (1)
#define VERSION_MAPOPTION       (2)
#define VERSION_MAPUNIT         (1)
#define VERSION_MAPSOUND        (1)

#define IDSTR_WEBZEN_MAP        ("WMAP" )
#define IDSTR_WEBZEN_UNIT       ("WUNT" )
#define IDSTR_WEBZEN_MAPOPTION  ("MOPT" )

#ifndef _SERVER
#define CLIENT_ONLY
#endif

#define LIMIT_Z_JUMP            (2.5f)          // ������ �ֱ� ������ �׳� ��ã��� �� �� �ִ� �Ѱ�
#define LIMIT_Z_LIMIT_MESH_TILE (3.21f)         // ��ã�� �� Ÿ��z �� �������� �޽�z �� ������� �� ���� ���� ����
#define ANGLE_NO_WALK           (WZ_PI / 3.0f)  // �ٴ� ���Ⱑ �� ���� �̻��̸� �� �� �������� �νĵ�

// #define DISABLE_OCTREE_LOOSE     // ������ ��Ʈ�� ��� ����

//#ifdef LAST_RELEASE
#define MAKE_BLEND_VERTEXBUFFER     // ���� ������ ���ؽ����� ���� ����
//#endif

struct WzSpecialArea
{
    BYTE m_byGroup;
    WzID m_wiIdentity;
    WzBoundingVolume m_wbvRegion;
};

// ��ã�� Event
enum ENUM_JUMP_STEP
{
    EJS_NONE = 0,
    EJS_JUMPSTART,
    EJS_JUMPEND,
};

#define MAX_JUMP_IN_JUMPEVENT   (16)

struct WzPathFindJump
{
    ENUM_JUMP_STEP m_eJumpStep;     // Jump ����/���� ���� ����
    WzVector m_wvJumpFrom;          // ���� ������  ( m_eJumpStep �� EJS_JUMPSTART �϶� ��ȿ )
    WzVector m_wvJumpTo;            // ���� ������  ( m_eJumpStep �� EJS_JUMPSTART �� EJS_JUMPEND �϶� ��ȿ )
};

struct WzPathFindEvent
{
    int m_iCountJump;   // �Ʒ� �迭 �� ��ȿ�� member ����
    WzPathFindJump m_Jump[MAX_JUMP_IN_JUMPEVENT];
};

// SetTerrainMeshGroupOption ���� ����ϴ� �ɼ�
#define TMGO_NO_Z_WRITE         (0x00000001)    // Z ���� write ����
#define TMGO_NO_COLLISION       (0x00000002)    // ���̰� �˻�� üũ �ȵ�
#define TMGO_WATER              (0x00000004)    // ���Ӽ�
#define TMGO_NO_DECAL           (0x00000008)    // ��Į �ȱ׸�
#define TMGO_SPECULAR           (0x00000010)    // ����ŧ��
#define TMGO_DETAIL             (0x00000020)    // ������ ����
#define TMGO_HAZE               (0x00000040)    // ��������
#define TMGO_GLOW               (0x00000080)    // �۷ο�
#define TMGO_NORMALMAP          (0x00000100)    // Normalmap

// MAP ������ ûũ ����
#define CHUNCID_OBJECT              0x1781      // ������Ʈ�� ����� ûũ �ĺ���
#define CHUNKID_SOUND               0x1791      // ���忡 ���� ûũ �ĺ���
#define CHUNKID_MAPOBJECT           0x1792      // �� ������Ʈ ������ ���� ûũ �ĺ���
#define CHUNKID_LIGHTMAP            0X1793      // ����Ʈ�� ������ ���� ûũ �ĺ���
#define CHUNCID_UNIT                0x1794      // ���� ������ ���� ûũ �ĺ���
#define CHUNKID_OCCBOUND            0X1795      // �����޽��� ������ node�� Occlusion ����� ���� ��� ���� ���� ûũ �ĺ���
#define CHUNKID_ENVIRONMENTOBJECT   0X1796      // ȯ�� ������Ʈ�� ���� ûũ �ĺ���

// ���� ������ ûũ ����
#define CHUNKID_SOUNDAREA_HEADER    0x1811      // ���� ���Ͽ� ����Ǵ� ûũ �ĺ���
#define CHUNKID_SOUNDAREA           0x1812
#define CHUNKID_SOUNDDATA_HEADER    0X1821
#define CHUNKID_SOUNDDATA           0X1822
#define CHUNCID_MAPOPTION           0x1784      // �� �ɼ� ���� ����

#define IDX_LIGHTMAP_TERRAIN        99999       // ������ ���� ����Ʈ ���� �ε��� ��ȣ(.wlu ���� importer���� ����)

#endif // _3DTERRAIN_TYPEDEF_H_