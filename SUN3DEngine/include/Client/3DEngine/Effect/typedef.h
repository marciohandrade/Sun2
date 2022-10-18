// typedef.h

#ifndef _EFFECT_TYPEDEF_H_
#define _EFFECT_TYPEDEF_H_

#include <client/3dengine/common/wzcommontypedef.h>

// ����Ʈ ���� ����
#include "effectver.h"

#include <map>
#include <list>
#include <vector>
#include <algorithm>

#ifdef WZEFFECT_EXPORTS
#define WZEFF_API   __declspec( dllexport )
#else
#define WZEFF_API   __declspec( dllimport )
#endif

// Ÿ�� ����
typedef std::map<long, float> std_KeyMap; 
typedef std::map<long, WzVector> std_CourseMap; 
typedef std::list<DWORD> std_SequenceList;
typedef std::vector<void*> std_list;

// ����Ʈ �ɼ�
#define EOPT_RENDERWEATHER      0x00000001
#define EOPT_AUTOEFFLVLCTL      0x00000002
#define EOPT_AUTOEFFRATECTL     0x00000004
#define EOPT_APPLYEFFLOD        0x00000008  // LOD ����
#define EOPT_APPLYLOD_LOOPONLY  0x00000010  // �ݺ� Ÿ�Ը� LOD ����


// ����Ʈ ����Ʈ ���� �ε� Ÿ��
enum EENUM_LOAD_TYPE
{
    EELT_NEW,   // ���� ���� ���� �� ���� �ε�
    EELT_ADD,   // ���� ������ �߰�
};

// Element Ÿ��
enum ENUM_EELEMENT_TYPE
{
    EET_NONE = 0,
    EET_SPRITE,
    EET_CHAIN,
    EET_DECAL,
    EET_FLATCHAIN,
    EET_MESH,   
    EET_BODY,
    EET_WEATHER,
};

// ���� ��ġ Ÿ��
enum E_POSITION_TYPE
{
    EPOS_POINT,
    EPOS_BOX,
    EPOS_SPHERE,
    EPOS_CUSTOM,
};

// ���� Ÿ��
enum EFF_LOOP_TYPE
{
    CV_LOOP_NO = 0,
    CV_LOOP_YES,
    CV_LOOP_YES_DELAY,
};

// Move(�ùķ��̼�) �� ���
enum _ENUM_RESULT_EFFECT_MOVE
{
	EREM_ALIVE,
	EREM_LOOP,
	EREM_DIE_NOW,
};

// �ڽ� ����
enum ENUM_MOVE_KIND
{
    EM_POS1,    // �̵� �ڽ�
    EM_POS2,    // ��� �� ��
};

// ���� �Ǵ� Ÿ�� �浹�� ó�� Ÿ��
enum ENUM_MOVE_TARRAIN
{   
    EMT_NONE,
    EMT_TERRAIN_STOP,       // ��� �� ��
    EMT_TERRAIN_ELASTIC,    // ��� �� ��
    EMT_TARGET,             // Ÿ�� �浹�� ����
};

// ���� Ÿ��
enum ENUM_DIR_TYPE
{
    EDT_DIR_NO,     // ���⼺ ����
    EDT_DIR_IN,     // �߽� �������� �̵�
    EDT_DIR_OUT,    // �߽� �ݴ� �������� �̵�
};

// �׷��� Ÿ��
enum ENUM_GRAPH_TYPE
{
    EGT_SPLINE,
    EGT_LINE,
};

// ������ Ÿ�� ����
enum EST_BILLBOARD_TYPE
{
    EB_ALL,     // �⺻ ������
    EB_X,       // X�� ����
    EB_Y,       // Y�� ����
    EB_Z,       // Z�� ����
    EB_UNUSE,   // ����� ����
};

// Texture Blend 
enum ENUM_BLEND_OP
{
    EBOP_NONE,
    EBOP_ALPHA,
    EBOP_LIGHTING,
    EBOP_ALPHA_COLOR,
    EBOP_SHADING,
    EBOP_INV_SHADING,
    EBOP_SUBTRACT,      // Src - Dest
    EBOP_REVSUBTRACT,   // Dest - Src
    EBOP_NUM, 
};

// ���� �̺�Ʈ
enum ENUM_SOUND_EVENT
{
    ENUM_UNUSE_EVENT,
    ENUM_START_EVENT,       // ���� ����
    ENUM_STOP_EVENT,        // ���� ����
    ENUM_UPDATE_POS_EVENT,  // ��ġ ����
};

// ���� ���� ����ü
struct St_RandomInfo
{
    bool m_blUse;           // ��� ����
    float m_fMinValue;      // �ּҰ�
    float m_fMaxValue;      // �ִ밪

    St_RandomInfo()
    {
        Clear();
    }

    void Clear()
    {
        memset( this, 0, sizeof( St_RandomInfo ) );
    }

    St_RandomInfo& operator = ( const St_RandomInfo& rhs )
    {
        if( &rhs != this )
        {
            memcpy( this, &rhs, sizeof( St_RandomInfo ) );
        }

        return *this;
    }
};

// �ڵ� ���� �Ķ����
struct St_AutoCtlParam
{
    DWORD dwTime;   // ������ �ð�
    int iParam;     // �Ķ���� (ex: ����, ����)
};

// LOD �Ķ����
struct St_EffLODParam
{
    float fDist;    // �Ÿ�
    int iParam;     // �Ķ���� (ex: �ùķ��̼� ����)
};

#endif // _EFFECT_TYPEDEF_H_