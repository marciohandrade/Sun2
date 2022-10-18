#pragma once

#include "Singleton.h"
#include "SolarFileReader.h"

class CPackFile;

enum ePACKOBJECT_TYPE
{
	ePACKOBJECT_SYSTEM	= 0,
	ePACKOBJECT_RESOURCE,
};	

namespace util 
{

	class FilePackerManager : public util::Singleton<FilePackerManager>
	{
	public:
		FilePackerManager();
		~FilePackerManager();

		VOID	Release();
		BOOL	OpenPackFile( char * pszPackFileName );
		BOOL	OpenPackFile( wchar_t * pszPackFileName);

		CPackFile*	GetPackFile( char * pszPackFileName, const char * pszFileName);

		CPackFile*	GetSystemPackObj() { return  m_pobPackFileSystem;	}
		CPackFile*	GetResourcePackObj() { return  m_pobPackFileResource;	}

		CPackFile*	GetPackObj( char * pszPackFileName );
		CPackFile*	GetPackObj( wchar_t * pszPackFileName);

		int			GetPackObjType( char * pszPackFileName );
		int			GetPackObjType( wchar_t * pszPackFileName);

		void		testPackFile();

	private:
		BOOL	_openPackFile(int iType, char * pszPackFileName);

	private :
		CPackFile			*m_pobPackFileSystem;
		CPackFile			*m_pobPackFileResource;

	};
}

#define PACKER_MANAGER()		util::FilePackerManager::Instance()