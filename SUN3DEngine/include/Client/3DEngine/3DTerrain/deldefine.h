// deldefine.h

#ifndef _3DTERRAIN_DELDEFINE_H_
#define _3DTERRAIN_DELDEFINE_H_

#define _YJ_SKYBOX_FILE_PATH_MODIFY //07.07.31

#define MULTI_TILE_ATTRIB       // �ټ� �Ӽ� ����

#define FOR_MAPTOOL_EXTRAINFO   // ������ ���� �߰� ó��

#define DISABLE_MOVE_TO_VISIBLE_POINT       //  ���̴� �� �������� �̵�

#define ADD_MAP_ATTR        // �ǿ�-�ǳ��Ӽ� �߰�

// ���� ��ȭ ������ �۾� : 2007/05/31
//#define TERRAIN_RENDER_SECTION
#ifdef TERRAIN_RENDER_SECTION
#   ifndef MAKE_BLEND_VERTEXBUFFER
#   define MAKE_BLEND_VERTEXBUFFER
#   endif
#endif

#define OPTIMIZE_WORLDTERRAINBLENDING
#define OPTIMIZE_ARRAY
#define OPTIMIZE_OCTNODEOBJECTS

// ��ã�⿡�� �Ÿ� �˻��� ��,
// ������ġ�� �������� �����Ͽ� �ǽð����� ����ϴ� ��� ��
// #define DISABLE_REAL_DISTANCE_CALCULATE

#define _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS // ����Ʈ�� ���� �� ������Ʈ ���� ����(���� ��游)

#endif