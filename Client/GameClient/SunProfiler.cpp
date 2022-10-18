// Profiler.cpp: implementation of the CSunProfiler class.
//
//////////////////////////////////////////////////////////////////////

#include "SunClientPrecompiledHeader.h"
#include "SunProfileSample.h"
#include "SunProfiler.h"

LONGLONG g_llFrequency;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSunProfiler::CSunProfiler()
{
	Clear();
}

CSunProfiler::~CSunProfiler()
{

}

void CSunProfiler::Clear( void)
{
	m_bFirstTime = TRUE;
	m_iNumSample = 0;
	m_llTotalTimeSum = 0;
	m_iCurDepth = 0;
}

void CSunProfiler::Init( void)
{
	LARGE_INTEGER Freq;

	QueryPerformanceFrequency(&Freq);

	g_llFrequency = Freq.QuadPart;
	
	Reset();
}

void CSunProfiler::Reset( void)
{
	Clear();

	LARGE_INTEGER Tick;
	QueryPerformanceCounter(&Tick);
	for ( int i = 0; i < MAX_SUN_PROFILE_SAMPLE; ++i)
	{
		m_Sample[i].Clear();
	}
	m_llTotalTimeStart = Tick.QuadPart;
}

void CSunProfiler::Begin( TCHAR *lpszName)
{
	if (GENERALPARAM->GetSpecialMode() >= eSPECIAL_MODE_STEP2)
	{
		if ( m_bFirstTime)
		{
			return;
		}

		LARGE_INTEGER Tick;
		QueryPerformanceCounter(&Tick);


		LONGLONG llTick = Tick.QuadPart;
		for ( int i = 0; i < m_iNumSample; ++i)
		{
			if ( m_Sample[i].CheckName( lpszName))
			{
				m_Sample[i].Begin( llTick ,m_iCurDepth);
				m_iCurDepth++;
				return;
			}
		}

		// 새로 추가
		if ( m_iNumSample < MAX_SUN_PROFILE_SAMPLE)
		{
			m_Sample[m_iNumSample].Init( lpszName);
			m_Sample[m_iNumSample++].Begin( llTick,m_iCurDepth);
			m_iCurDepth++;
		}
	}

}

void CSunProfiler::End( TCHAR *lpszName)
{
	if (GENERALPARAM->GetSpecialMode() >= eSPECIAL_MODE_STEP2)
	{
		if ( m_bFirstTime)
		{
			return;
		}

		LARGE_INTEGER Tick;
		QueryPerformanceCounter(&Tick);


		
		for ( int i = 0; i < m_iNumSample; ++i)
		{
			if ( m_Sample[i].CheckName( lpszName))
			{
				m_Sample[i].End( Tick.QuadPart);
				m_iCurDepth--;
				return;
			}
		}
	}
}

void CSunProfiler::GenerateResult( void)
{

	LARGE_INTEGER Tick;
	QueryPerformanceCounter(&Tick);
	m_llTotalTimeEnd = Tick.QuadPart;

	LONGLONG llTotalTime = m_llTotalTimeEnd - m_llTotalTimeStart;
	if ( !m_bFirstTime)
	{
		m_llTotalTimeSum += llTotalTime;
		for ( int i = 0; i < m_iNumSample; ++i)
		{
			m_Sample[i].GenerateResult( llTotalTime, m_llTotalTimeSum);
		}
	}

	m_bFirstTime = FALSE;

	QueryPerformanceCounter(&Tick);
	
	m_llTotalTimeStart = Tick.QuadPart;
}



void CSunProfiler::GetResultText( int iSample, TCHAR *lpszText)
{
	assert( iSample < m_iNumSample);

	TCHAR lpszTemp[1024];
	m_Sample[iSample].GetResultText( lpszTemp);
	_stprintf( lpszText, _T("%s - %s"), m_Sample[iSample].GetName(), lpszTemp);
}

int CSunProfiler::GetDepth( int iSample)
{
	assert( iSample < m_iNumSample);


	return m_Sample[iSample].GetDepth();

}