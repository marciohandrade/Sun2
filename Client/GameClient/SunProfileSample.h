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

	/// @name ��� ����
	LONGLONG m_llStartTick;		///< �ѹ� üũ�ϴ� ������ ����
	LONGLONG m_llSumTime;		
	LONGLONG m_llViewSumTime;	
	

	/// @name ȣ�� ȸ��
	int m_iCountInFrame;	///< �� ������ ���� ȣ��� ȸ��
	int m_iLatestCount;		///< �ֱ��� ȣ�� ȸ��
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


