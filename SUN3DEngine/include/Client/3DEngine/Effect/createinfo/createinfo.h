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
    DWORD m_dwLife;                 // ����
    DWORD m_dwMakeTime;             // �¾ �ð�

    E_POSITION_TYPE m_ePosType;     // ���� ��ġ Ÿ��
    
    // ����Ʈ�� ����
    WzVector m_wvPos;               // ���� ��ġ

    // �ڽ��� ����
    WzVector m_wvMinRange;          // �ּ���
    WzVector m_wvMaxRange;          // �ִ���

    // ���� ����
    float m_fRadious;               // ������
    WzVector m_wvCenter;            // �߽�

    // �� ����Ʈ (custom line ������)
    std_CourseMap m_stdMapVector;

    // ���� �ð� ����Ʈ
    std_SequenceList m_stdSequence;

    // ���� Ÿ��(0:��� �� ��, 1: ���� ����, 2: ���� ���� ����)
    BYTE m_byLoopType;

    St_RandomInfo m_sRandMakeTime;  // ���� �¾ �ð�
    St_RandomInfo m_sRandSequenceCnt; // ��� �� ��

    // �� �ະ �ʱ� ���� ȸ�� ����
    St_RandomInfo m_sRandRotX;
    St_RandomInfo m_sRandRotY;
    St_RandomInfo m_sRandRotZ;

    // ������
    St_Create_Info()
    {
        Initialize();
    }

    // �ʱ�ȭ
    void Initialize()
    {
        // memset���� �ʱ�ȭ �ϸ� �� ��

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

    // ����Ÿ ����
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

        // memcpy�� ���� �ϸ� �� ��

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