// Profiler.h: interface for the CSunProfiler class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


#include "SunProfileSample.h"

#define MAX_SUN_PROFILE_SAMPLE	( 256)


class CSunProfiler
{
public:
	CSunProfiler();
	virtual ~CSunProfiler();
	void Clear( void);

protected:
	BOOL m_bFirstTime;	///< ��Ȯ���� ���� ù �������� �����Ѵ�.
	int m_iNumSample;
	CSunProfileSample m_Sample[MAX_SUN_PROFILE_SAMPLE];

	/// @name ��ü �ð�
	LONGLONG m_llTotalTimeStart;
	LONGLONG m_llTotalTimeEnd;
	LONGLONG m_llTotalTimeSum;

	int m_iCurDepth;

public:
	void Init( void);	///< measure �� ���� �Ŀ� �Ұ�
	void Reset( void);	///< ��� �ʱ�ȭ
	void Begin( TCHAR *lpszName);
	void End( TCHAR *lpszName);
	void GenerateResult( void);	///< ��� ����

	int GetNumSample( void)	{ return ( m_iNumSample); }	
	void GetResultText( int iSample, TCHAR *lpszText);
	int  GetDepth( int iSample);
};

extern LONGLONG g_llFrequency;





