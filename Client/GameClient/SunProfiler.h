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
	BOOL m_bFirstTime;	///< 정확도를 위해 첫 프레임은 제외한다.
	int m_iNumSample;
	CSunProfileSample m_Sample[MAX_SUN_PROFILE_SAMPLE];

	/// @name 전체 시간
	LONGLONG m_llTotalTimeStart;
	LONGLONG m_llTotalTimeEnd;
	LONGLONG m_llTotalTimeSum;

	int m_iCurDepth;

public:
	void Init( void);	///< measure 가 끝난 후에 할것
	void Reset( void);	///< 결과 초기화
	void Begin( TCHAR *lpszName);
	void End( TCHAR *lpszName);
	void GenerateResult( void);	///< 결과 생성

	int GetNumSample( void)	{ return ( m_iNumSample); }	
	void GetResultText( int iSample, TCHAR *lpszText);
	int  GetDepth( int iSample);
};

extern LONGLONG g_llFrequency;





