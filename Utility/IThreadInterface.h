#ifndef __ITHREAD_INTERFACE_H__
#define __ITHREAD_INTERFACE_H__

#pragma once

//=======================================================================================================================
/// 스레드를 편리하게 사용하기 위한 추상 클래스
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 10. 20
	@remarks
			- IThreadInterface를 상속받아 구현한 클래스는 하나의 스레드가 된다.
			- StartThread() 호출로 스레드를 생성하고 EndThread() 호출로 스레드를 죽인다.
	@note
			- 스레드의 Run()를 빠져 나오게 하는 적절한 처리를 EndThread()에 구현해야 한다.
	@history
			- 2004.11.11 : utility library에 추가
*/
//=======================================================================================================================
namespace util 
{

class IThreadInterface
{
public:
	IThreadInterface(void);
	virtual ~IThreadInterface(void);

	VOID							SuspendThread();
	VOID							ResumeThread();

	VOID							StartThread( BOOL bSuspend = FALSE );
	virtual VOID					EndThread() = 0;

	inline HANDLE					GetHandle()	{ return m_hThread; }

protected:
	virtual DWORD					run() = 0;
	static unsigned int __stdcall	m_fnThread(void * pt);

private:
	HANDLE							m_hThread;
};

}


#endif // __ITHREAD_INTERFACE_H__