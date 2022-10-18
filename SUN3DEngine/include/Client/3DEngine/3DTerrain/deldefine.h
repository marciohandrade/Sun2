// deldefine.h

#ifndef _3DTERRAIN_DELDEFINE_H_
#define _3DTERRAIN_DELDEFINE_H_

#define _YJ_SKYBOX_FILE_PATH_MODIFY //07.07.31

#define MULTI_TILE_ATTRIB       // 다수 속성 적용

#define FOR_MAPTOOL_EXTRAINFO   // 맵툴을 위한 추가 처리

#define DISABLE_MOVE_TO_VISIBLE_POINT       //  보이는 곳 직선으로 이동

#define ADD_MAP_ATTR        // 실외-실내속성 추가

// 지형 블럭화 렌더링 작업 : 2007/05/31
//#define TERRAIN_RENDER_SECTION
#ifdef TERRAIN_RENDER_SECTION
#   ifndef MAKE_BLEND_VERTEXBUFFER
#   define MAKE_BLEND_VERTEXBUFFER
#   endif
#endif

#define OPTIMIZE_WORLDTERRAINBLENDING
#define OPTIMIZE_ARRAY
#define OPTIMIZE_OCTNODEOBJECTS

// 길찾기에서 거리 검사할 때,
// 현재위치와 도착점을 적용하여 실시간으로 계산하는 기능 끔
// #define DISABLE_REAL_DISTANCE_CALCULATE

#define _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS // 라이트맵 빌드 시 오브젝트 관련 제외(순수 배경만)

#endif