/*-----------------------------------------------------------------------------
 * CArmorKit.h 
 *-----------------------------------------------------------------------------
 * ArmorKit Ŭ���� 
 *-----------------------------------------------------------------------------
 * Copyright (c) , 2005, WEBZEN Inc.
 * All rights reserved.
 *
 * Author : Noh Yong Hwan, 2005, WEBZEN Inc. Research & Development Dept.
 *          fixbrain@gmail.com , somma@webzen.co.kr
**---------------------------------------------------------------------------*/

#ifndef _C_ARMOR_KIT_H_
#define _C_ARMOR_KIT_H_


#include "ArmorKit.h"
#include <MMSystem.h>
#include <vector>

#include "IGhostUtil.h"			// GhostApi �� ���� �������̽�

// ��ü Ž�� �� ����ó���� ���� ���� �ݹ��Լ� Ÿ��
//
typedef INT (*LPFN_USERCALLBACK )(PHACK_DETECT_INFO pHDI);


/*-----------------------------------------------------------------------------
 * CArmorKit Ŭ���� ����
**---------------------------------------------------------------------------*/
class CArmorKit 
{
private:
	unsigned int	dwActionFlags;		// �׼� �÷���
	DWORD	dwPEAuthType;				// PE���� ���
	DWORD	dwActionMode;				// �׼� ���
	DWORD	dwFrequency;				// �׼� �ֱ�	
	MMRESULT	uTimerId;				// Ÿ�̸� ���̵�
	DWORD	dwDebugLevel;				// ����� ���� 		
	TCHAR	szLauncherName[MAX_PATH+1];	// ����� üũ�� ���� �θ����μ��� �̸�

	CRITICAL_SECTION m_Critical;

	/// CArmorKit �� ����������.. -> CArmorKit.Start() �� ȣ��Ǿ��ٸ� True..
	//	WARNING !!!
	//		�� ����� thread-safe ���� ����.
	//
	BOOL m_bStarted;
	BOOL m_useFilelog;

	// 2006.09.20 ���ȯ �߰� 
	//	GhostApi ���
	// 
	RSCM_IGhost m_rscm_ghost;
	IGhost* m_ghost;

	// 2007.01.22 ������ �߰�
	// AKSupport������ ���� ������� �߰�
	// m_Timer�� m_DetectThread�� ��� ������Ϸ� ����
	// LPVOID�� ���� (���� ���� ĳ���� ����)
	HANDLE	m_hEvent;
	LPVOID	m_Timer;
	LPVOID	m_DetectThread;
	int		ResumeDetectThread();
	IGhost* GetGhostPtr() { return m_ghost; }

public:
	DWORD	m_os_version ;							// Ŭ���̾�Ʈ�� �������� ���� 

	LPFN_USERCALLBACK	lpfnUserCallBack;		// ���� �ݹ� �Լ� ������	
	BOOL IsThreadRunning(void);		

	////
	// ȯ�漳�� �� �������̽� �Լ�
	////
	VOID	SetActionFlags( unsigned int dwSetActionFlags );	// �׼� �÷��׸� �����ϴ� �Լ�
	DWORD	GetActionFlags( VOID );						// �׼� �÷��׸� �������� �Լ�
    VOID	SetLauncherName( TCHAR * lptszLauncherName);// �����Ž���� ���� ������ ��ó���ڿ��� �����ϴ� �Լ�
	TCHAR*	GetLauncherName(VOID);						// ��ó���ڿ��� �������� �Լ� 
	
	VOID	SetActionMode ( DWORD dwSetActionMode, DWORD dwFrequency );	// �׼Ǹ�带 �����ϴ� �Լ�
	VOID	SetDebugLevel ( DWORD dwSetDebugLevel ) ;					// ����� ������ �����ϴ� �Լ�
	VOID	SetWindowVersion() ;										// ������ ������ �����ϴ� �Լ�	

	BOOL GetUseFileLog(void);							// ���Ϸα� ���� �޼ҵ� 
	void SetUseFileLog(const BOOL Value);

	////
	// ������ �ʿ��� �Լ���
	////
	INT		Init(LPFN_USERCALLBACK lpfnUserCallBack, TCHAR	*lpVersion);	// Armorkit�� �ʱ�ȭ�ϴ� �Լ�, ���� ���� ��� �߰�
	BOOL	Start() ;									// Armorkit�� ������Ű�� �Լ�
	VOID	Release() ;									// Armorkit�� �ڿ��� ������Ű�� �Լ�
	
	INT		SetPatternList(TCHAR *list, int length);
	INT		GenerateSid( int seed, char *sid );         // Server���� �ﾲüũ ���� �Լ�
	
	// �� ��忡 ���� �����Լ���
	BOOL	DoByTimer() ;
	
	// AKTimer callback
	static void __stdcall OnAKTimer(unsigned int timer_id, DWORD_PTR user_tag);

	CArmorKit();
	~CArmorKit() ;

};




#endif