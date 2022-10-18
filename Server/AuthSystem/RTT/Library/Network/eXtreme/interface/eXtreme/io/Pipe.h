#ifndef  __eXtreme_Pipe_H
#define  __eXtreme_Pipe_H

namespace eXtreme
{
	class Pipe
	{
	public:
			Pipe();
			~Pipe();

			bool		Create( char* name );
			bool		Open( char* name );

			void		Close();


			int			Write( char* buffer, DWORD size );
			int			Read( char* buffer, DWORD size );

	private:
			HANDLE		m_handlePipe;
			char		m_pipeName[127];
	};
}

#endif