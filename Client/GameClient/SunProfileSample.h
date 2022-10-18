// ProfileSample.h: interface for the CSunProfileSample class.
//
//////////////////////////////////////////////////////////////////////



#pragma once



#define MAX_SAMPLENAME	( 64)

class CSunProfileSample  
{
public:
	CSunProfileSample();
	virtual ~CSunProfileSample();
	void Clear( void);

protected:
	TCHAR m_lpszName[MAX_SAMPLENAME];
	DWORD m_dwID;

	/// @name 계산 정보
	LONGLONG m_llStartTick;		///< 한번 체크하는 동안의 시작
	LONGLONG m_llSumTime;		
	LONGLONG m_llViewSumTime;	
	

	/// @name 호출 회수
	int m_iCountInFrame;	///< 한 프레임 동안 호출된 회수
	int m_iLatestCount;		///< 최근의 호출 회수
	int m_iDepth;

public:
	void Init(TCHAR *lpszName);
	BOOL CheckName( TCHAR *lpszName);
	BOOL CheckID(DWORD dwID);
	TCHAR* GetName( void)	{ return ( m_lpszName); }
	void Begin( LONGLONG llTick,int iDepth);
	void End( LONGLONG llTick);

	void GenerateResult( LONGLONG llTotalTime, LONGLONG llTotalTimeSum);
	void GetResult( float *pfLatestPercentage, float *pfMaxPercentage, float *pfMinPercentage, float *pfTotalPercentage);
	void GetResultText( TCHAR *lpszText);
	int GetDepth();
};


