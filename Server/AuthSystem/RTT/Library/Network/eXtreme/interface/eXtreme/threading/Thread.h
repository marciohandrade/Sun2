#ifndef  __eXtreme_Thread_H
#define  __eXtreme_Thread_H

namespace eXtreme
{
	class Thread
	{
	public:
				Thread();
		virtual ~Thread();

				bool		Start();
				bool		Terminate( DWORD exitCode );


				bool		WaitForTerminate( DWORD timeout = INFINITE );

	protected:
		virtual	void		Run()			= 0;

				HANDLE		m_handleThread;
				DWORD		m_threadID;

	private:
		static unsigned int __stdcall	Runner( LPVOID parameter );
	};
}

#endif