// ProfileSample.cpp: implementation of the CSunProfileSample class.
//
//////////////////////////////////////////////////////////////////////
#include "SunClientPrecompiledHeader.h"
#include "SunProfileSample.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSunProfileSample::CSunProfileSample()
{
	Clear();
}

CSunProfileSample::~CSunProfileSample()
{

}

void CSunProfileSample::Clear( void)
{
	ZeroMemory( m_lpszName, sizeof(m_lpszName));
	m_llSumTime = 0;
	m_llViewSumTime = 0;

	m_iCountInFrame = 0;
	m_iLatestCount = 0;
}

void CSunProfileSample::Init( TCHAR *lpszName)
{
	Clear();
	StrnCopy( m_lpszName, lpszName, MAX_SAMPLENAME - 1);
}

BOOL CSunProfileSample::CheckName( TCHAR *lpszName)
{
	return ( 0 == Strcmp( m_lpszName, lpszName));
}

void CSunProfileSample::Begin( LONGLONG llTick,int iDepth)
{
	m_llStartTick = llTick;
	m_iDepth = iDepth;
}

void CSunProfileSample::End( LONGLONG llTick)
{
	m_llSumTime += ( llTick - m_llStartTick);
	++m_iCountInFrame;
}

void CSunProfileSample::GenerateResult( LONGLONG llTotalTime, LONGLONG llTotalTimeSum)
{
	if ( m_llSumTime <= 0)
	{	// 한번도 호출 안한경우 그대로 유지
		return;
	}

	// 시간 계산
	m_llViewSumTime = m_llSumTime;

	
	m_llSumTime = 0;

	// 호출 회수
	m_iLatestCount = m_iCountInFrame;
	m_iCountInFrame = 0;
}

void CSunProfileSample::GetResultText( TCHAR *lpszText)
{	
	double elapsedmillsecond = (double)(m_llViewSumTime * 1000) / (double)g_llFrequency;

	_stprintf( lpszText, _T("%.2fms (%d time), "), elapsedmillsecond,m_iLatestCount);
}

int CSunProfileSample::GetDepth()
{
	return m_iDepth;
}
