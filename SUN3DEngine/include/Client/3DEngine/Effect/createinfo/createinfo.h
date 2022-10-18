// createinfo.h

#ifndef _EFFECT_CREATEINFO_H_
#define _EFFECT_CREATEINFO_H_

#include "../typedef.h"

//------------------------------------------------------------------------------
/**
    @struct St_Create_Info
*/
struct St_Create_Info
{
    DWORD m_dwLife;                 // 수명
    DWORD m_dwMakeTime;             // 태어날 시간

    E_POSITION_TYPE m_ePosType;     // 생성 위치 타입
    
    // 포인트에 생성
    WzVector m_wvPos;               // 생성 위치

    // 박스내 생성
    WzVector m_wvMinRange;          // 최소점
    WzVector m_wvMaxRange;          // 최대점

    // 구내 생성
    float m_fRadious;               // 반지름
    WzVector m_wvCenter;            // 중심

    // 점 리스트 (custom line 생성시)
    std_CourseMap m_stdMapVector;

    // 생성 시간 리스트
    std_SequenceList m_stdSequence;

    // 루프 타입(0:사용 안 함, 1: 지연 루프, 2: 지연 없는 루프)
    BYTE m_byLoopType;

    St_RandomInfo m_sRandMakeTime;  // 랜덤 태어날 시간
    St_RandomInfo m_sRandSequenceCnt; // 사용 안 함

    // 각 축별 초기 랜덤 회전 정보
    St_RandomInfo m_sRandRotX;
    St_RandomInfo m_sRandRotY;
    St_RandomInfo m_sRandRotZ;

    // 생성자
    St_Create_Info()
    {
        Initialize();
    }

    // 초기화
    void Initialize()
    {
        // memset으로 초기화 하면 안 됨

        m_dwLife = 0;
        m_dwMakeTime = 0;

        m_ePosType = EPOS_POINT;

        SetVector( &m_wvPos, 0.0f, 0.0f, 0.0f );

        SetVector( &m_wvMinRange, 0.0f, 0.0f, 0.0f );
        SetVector( &m_wvMaxRange, 0.0f, 0.0f, 0.0f );

        m_fRadious = 0.0f;
        SetVector( &m_wvCenter, 0.0f, 0.0f, 0.0f );

        m_stdMapVector.clear();
        m_stdSequence.clear();

        m_byLoopType = CV_LOOP_NO;

        m_sRandMakeTime.Clear();
        m_sRandSequenceCnt.Clear();

        m_sRandRotX.Clear();
        m_sRandRotY.Clear();
        m_sRandRotZ.Clear();
    }

    // 데이타 제거
    void Release()
    {
        m_stdMapVector.clear();
        m_stdSequence.clear();
    }

    // operator =
    St_Create_Info& operator = ( const St_Create_Info& rhs )
    {
        if( &rhs == this )
        {
            return *this;
        }

        // memcpy로 복사 하면 안 됨

        m_dwLife = rhs.m_dwLife;
        m_dwMakeTime = rhs.m_dwMakeTime;

        m_ePosType = rhs.m_ePosType;

        m_wvPos = rhs.m_wvPos;

        m_wvMinRange = rhs.m_wvMinRange;
        m_wvMaxRange = rhs.m_wvMaxRange;

        m_fRadious = rhs.m_fRadious;
        m_wvCenter = rhs.m_wvCenter;

        m_stdMapVector = rhs.m_stdMapVector;
        m_stdSequence = rhs.m_stdSequence;

        m_byLoopType = rhs.m_byLoopType;

        m_sRandMakeTime = rhs.m_sRandMakeTime;
        m_sRandSequenceCnt = rhs.m_sRandSequenceCnt;

        m_sRandRotX = rhs.m_sRandRotX;
        m_sRandRotY = rhs.m_sRandRotY;
        m_sRandRotZ = rhs.m_sRandRotZ;

        return *this;
    }
};

#endif // _EFFECT_CREATEINFO_H_