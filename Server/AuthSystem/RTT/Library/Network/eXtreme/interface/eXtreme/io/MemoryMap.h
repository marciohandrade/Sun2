#ifndef  __eXtreme_MemoryMap_H
#define  __eXtreme_MemoryMap_H

namespace eXtreme
{
	class MemoryMap
	{
	public:
			MemoryMap();
			~MemoryMap();

			bool	Open( char* fileName, char* memMapName = 0 );
			void	Close();

			bool	Flush();
			void*	GetBaseAddress();

			int		GetSize();

	private:
			HANDLE	m_handleFile;
			HANDLE	m_handleMemMap;

			void*	m_baseAddr;
			int		m_size;
	};
}

#endif