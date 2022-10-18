// typedef.h

#ifndef _EFFECT_TYPEDEF_H_
#define _EFFECT_TYPEDEF_H_

#include <client/3dengine/common/wzcommontypedef.h>

// 이펙트 버전 파일
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

// 타입 정의
typedef std::map<long, float> std_KeyMap; 
typedef std::map<long, WzVector> std_CourseMap; 
typedef std::list<DWORD> std_SequenceList;
typedef std::vector<void*> std_list;

// 이펙트 옵션
#define EOPT_RENDERWEATHER      0x00000001
#define EOPT_AUTOEFFLVLCTL      0x00000002
#define EOPT_AUTOEFFRATECTL     0x00000004
#define EOPT_APPLYEFFLOD        0x00000008  // LOD 적용
#define EOPT_APPLYLOD_LOOPONLY  0x00000010  // 반복 타입만 LOD 적용


// 이펙트 리스트 파일 로딩 타입
enum EENUM_LOAD_TYPE
{
    EELT_NEW,   // 기존 정보 제거 후 새로 로딩
    EELT_ADD,   // 기존 정보에 추가
};

// Element 타입
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

// 생성 위치 타입
enum E_POSITION_TYPE
{
    EPOS_POINT,
    EPOS_BOX,
    EPOS_SPHERE,
    EPOS_CUSTOM,
};

// 루프 타입
enum EFF_LOOP_TYPE
{
    CV_LOOP_NO = 0,
    CV_LOOP_YES,
    CV_LOOP_YES_DELAY,
};

// Move(시뮬레이션) 후 결과
enum _ENUM_RESULT_EFFECT_MOVE
{
	EREM_ALIVE,
	EREM_LOOP,
	EREM_DIE_NOW,
};

// 코스 종류
enum ENUM_MOVE_KIND
{
    EM_POS1,    // 이동 코스
    EM_POS2,    // 사용 안 함
};

// 지형 또는 타겟 충돌시 처리 타입
enum ENUM_MOVE_TARRAIN
{   
    EMT_NONE,
    EMT_TERRAIN_STOP,       // 사용 안 함
    EMT_TERRAIN_ELASTIC,    // 사용 안 함
    EMT_TARGET,             // 타겟 충돌시 제거
};

// 방향 타입
enum ENUM_DIR_TYPE
{
    EDT_DIR_NO,     // 방향성 없음
    EDT_DIR_IN,     // 중심 방향으로 이동
    EDT_DIR_OUT,    // 중심 반대 방향으로 이동
};

// 그래프 타입
enum ENUM_GRAPH_TYPE
{
    EGT_SPLINE,
    EGT_LINE,
};

// 빌보드 타입 설정
enum EST_BILLBOARD_TYPE
{
    EB_ALL,     // 기본 빌보드
    EB_X,       // X축 고정
    EB_Y,       // Y축 고정
    EB_Z,       // Z축 고정
    EB_UNUSE,   // 사용자 지정
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

// 사운드 이벤트
enum ENUM_SOUND_EVENT
{
    ENUM_UNUSE_EVENT,
    ENUM_START_EVENT,       // 사운드 시작
    ENUM_STOP_EVENT,        // 사운드 멈춤
    ENUM_UPDATE_POS_EVENT,  // 위치 갱신
};

// 랜덤 정보 구조체
struct St_RandomInfo
{
    bool m_blUse;           // 사용 여부
    float m_fMinValue;      // 최소값
    float m_fMaxValue;      // 최대값

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

// 자동 조절 파라미터
struct St_AutoCtlParam
{
    DWORD dwTime;   // 렌더링 시간
    int iParam;     // 파라미터 (ex: 레벨, 비율)
};

// LOD 파라미터
struct St_EffLODParam
{
    float fDist;    // 거리
    int iParam;     // 파라미터 (ex: 시뮬레이션 비율)
};

#endif // _EFFECT_TYPEDEF_H_