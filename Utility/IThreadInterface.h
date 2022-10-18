#ifndef __ITHREAD_INTERFACE_H__
#define __ITHREAD_INTERFACE_H__

#pragma once

//=======================================================================================================================
/// �����带 ���ϰ� ����ϱ� ���� �߻� Ŭ����
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 10. 20
	@remarks
			- IThreadInterface�� ��ӹ޾� ������ Ŭ������ �ϳ��� �����尡 �ȴ�.
			- StartThread() ȣ��� �����带 �����ϰ� EndThread() ȣ��� �����带 ���δ�.
	@note
			- �������� Run()�� ���� ������ �ϴ� ������ ó���� EndThread()�� �����ؾ� �Ѵ�.
	@history
			- 2004.11.11 : utility library�� �߰�
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