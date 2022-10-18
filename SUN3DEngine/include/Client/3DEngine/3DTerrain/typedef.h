// typedef.h

#ifndef _3DTERRAIN_TYPEDEF_H_
#define _3DTERRAIN_TYPEDEF_H_

#include <client/3dengine/common/wzcommontypedef.h>
#include <client/3dengine/common/boundingvolume.h>

// TODO: 제거 예정
// 툴이나 클라이언트에서 쓸 수 있기 때문에 일단 포함한다.
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

#define LIMIT_Z_JUMP            (2.5f)          // 점프가 있기 때문에 그냥 길찾기로 갈 수 있는 한계
#define LIMIT_Z_LIMIT_MESH_TILE (3.21f)         // 길찾기 중 타일z 를 기준으로 메쉬z 를 얻었을때 둘 간의 차이 범위
#define ANGLE_NO_WALK           (WZ_PI / 3.0f)  // 바닥 기울기가 이 각도 이상이면 갈 수 없음으로 인식됨

// #define DISABLE_OCTREE_LOOSE     // 느슨한 옥트리 사용 안함

//#ifdef LAST_RELEASE
#define MAKE_BLEND_VERTEXBUFFER     // 블렌딩 지형용 버텍스버퍼 따로 만듬
//#endif

struct WzSpecialArea
{
    BYTE m_byGroup;
    WzID m_wiIdentity;
    WzBoundingVolume m_wbvRegion;
};

// 길찾기 Event
enum ENUM_JUMP_STEP
{
    EJS_NONE = 0,
    EJS_JUMPSTART,
    EJS_JUMPEND,
};

#define MAX_JUMP_IN_JUMPEVENT   (16)

struct WzPathFindJump
{
    ENUM_JUMP_STEP m_eJumpStep;     // Jump 시작/끝에 대한 정보
    WzVector m_wvJumpFrom;          // 점프 시작점  ( m_eJumpStep 가 EJS_JUMPSTART 일때 유효 )
    WzVector m_wvJumpTo;            // 점프 착지점  ( m_eJumpStep 가 EJS_JUMPSTART 나 EJS_JUMPEND 일때 유효 )
};

struct WzPathFindEvent
{
    int m_iCountJump;   // 아래 배열 중 유효한 member 개수
    WzPathFindJump m_Jump[MAX_JUMP_IN_JUMPEVENT];
};

// SetTerrainMeshGroupOption 에서 사용하는 옵션
#define TMGO_NO_Z_WRITE         (0x00000001)    // Z 버퍼 write 안함
#define TMGO_NO_COLLISION       (0x00000002)    // 높이값 검사시 체크 안됨
#define TMGO_WATER              (0x00000004)    // 물속성
#define TMGO_NO_DECAL           (0x00000008)    // 데칼 안그림
#define TMGO_SPECULAR           (0x00000010)    // 스펙큘라
#define TMGO_DETAIL             (0x00000020)    // 디테일 맵핑
#define TMGO_HAZE               (0x00000040)    // 아지랑이
#define TMGO_GLOW               (0x00000080)    // 글로우
#define TMGO_NORMALMAP          (0x00000100)    // Normalmap

// MAP 파일의 청크 정의
#define CHUNCID_OBJECT              0x1781      // 오브젝트가 저장된 청크 식별자
#define CHUNKID_SOUND               0x1791      // 사운드에 대한 청크 식별자
#define CHUNKID_MAPOBJECT           0x1792      // 맵 오브젝트 정보에 대한 청크 식별자
#define CHUNKID_LIGHTMAP            0X1793      // 라이트맵 정보에 대한 청크 식별자
#define CHUNCID_UNIT                0x1794      // 유닛 정보에 대한 청크 식별자
#define CHUNKID_OCCBOUND            0X1795      // 지형메쉬를 포함한 node의 Occlusion 계산을 위한 경계 정보 구역 청크 식별자
#define CHUNKID_ENVIRONMENTOBJECT   0X1796      // 환경 오브젝트에 대한 청크 식별자

// 사운드 파일의 청크 정의
#define CHUNKID_SOUNDAREA_HEADER    0x1811      // 사운드 파일에 저장되는 청크 식별자
#define CHUNKID_SOUNDAREA           0x1812
#define CHUNKID_SOUNDDATA_HEADER    0X1821
#define CHUNKID_SOUNDDATA           0X1822
#define CHUNCID_MAPOPTION           0x1784      // 맵 옵션 정보 영역

#define IDX_LIGHTMAP_TERRAIN        99999       // 지형을 위한 라이트 맵의 인덱스 번호(.wlu 파일 importer에서 정의)

#endif // _3DTERRAIN_TYPEDEF_H_