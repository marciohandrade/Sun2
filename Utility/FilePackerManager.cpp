#include "UtilityCommon.h"
#include "FilePackerManager.h"

#include <io.h>

#include <ProgramCommon/wzFilePacker.h>
#include "GeneralFunction.h"

#ifdef _SERVER
    #include "./wzfilepacker.hxx"
    #pragma comment(linker, "/NODEFAULTLIB:WzFilePacker_Static")    // for 스크립트 로드
#endif

namespace util
{

FilePackerManager::FilePackerManager()
{
	m_pobPackFileSystem		= new CPackFile();
	m_pobPackFileResource	= new CPackFile();

	m_pobPackFileSystem->Close();
	m_pobPackFileResource->Close();
}

FilePackerManager::~FilePackerManager()
{
	SAFE_DELETE( m_pobPackFileSystem );
	SAFE_DELETE( m_pobPackFileResource );
}

VOID FilePackerManager::Release()
{
	m_pobPackFileSystem->Close();
	m_pobPackFileResource->Close();

	SAFE_DELETE( m_pobPackFileSystem );
	SAFE_DELETE( m_pobPackFileResource );
}	

BOOL FilePackerManager::OpenPackFile( char * pszPackFileName )
{
	int iType = GetPackObjType(pszPackFileName);
	return _openPackFile(iType, pszPackFileName);
}

BOOL FilePackerManager::OpenPackFile( wchar_t * pszPackFileName)
{
	char szPackFileName[MAX_PATH +1] = {0,};
	WideCharToMultiByte( CP_ACP, MB_PRECOMPOSED, pszPackFileName, -1, szPackFileName, -1 , NULL ,NULL);

	int iType = GetPackObjType(szPackFileName);
	return _openPackFile(iType, szPackFileName);
}

int FilePackerManager::GetPackObjType( char * pszPackFileName )
{
	if(pszPackFileName)
	{
		if(strstr(pszPackFileName, "Resource.wpk"))
		{
			return ePACKOBJECT_RESOURCE;
		}
		else if(strstr(pszPackFileName, "System.wpk"))
		{
			return ePACKOBJECT_SYSTEM;
		}

	}

	return -1;
}

int FilePackerManager::GetPackObjType( wchar_t * pszPackFileName )
{
	char szPackFileName[MAX_PATH +1] = {0,};
	WideCharToMultiByte( CP_ACP, MB_PRECOMPOSED, pszPackFileName, -1, szPackFileName, -1 , NULL ,NULL);

	if(szPackFileName)
	{
		if(strstr(szPackFileName, "Resource.wpk"))
		{
			return ePACKOBJECT_RESOURCE;
		}
		else if(strstr(szPackFileName, "System.wpk"))
		{
			return ePACKOBJECT_SYSTEM;
		}

	}
	return -1;
}

CPackFile*	FilePackerManager::GetPackObj( char * pszPackFileName )
{
	int iType = GetPackObjType(pszPackFileName);

	if(iType == ePACKOBJECT_RESOURCE)
	{
		return m_pobPackFileResource;
	}
	else if(iType == ePACKOBJECT_SYSTEM)
	{
		return m_pobPackFileSystem;
	}

	return NULL;
}

CPackFile*	FilePackerManager::GetPackObj( wchar_t * pszPackFileName)
{
	int iType = GetPackObjType(pszPackFileName);

	if(iType == ePACKOBJECT_RESOURCE)
	{
		return m_pobPackFileResource;
	}
	else if(iType == ePACKOBJECT_SYSTEM)
	{
		return m_pobPackFileSystem;
	}

	return NULL;
}

BOOL FilePackerManager::_openPackFile(int iType, char * pszPackFileName)
{
	if(_access(pszPackFileName, 0) != 0)
	{
		//파일 있는지 확인해보고 없으면  FALSE 리턴
		return FALSE;
	}

	if(iType == ePACKOBJECT_RESOURCE)
	{
		if(!m_pobPackFileResource->IsFileOpen())
		{
			if(m_pobPackFileResource->Open(pszPackFileName))
			{
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else if(iType == ePACKOBJECT_SYSTEM)
	{
		if(!m_pobPackFileSystem->IsFileOpen())
		{
			if(m_pobPackFileSystem->Open(pszPackFileName))
			{
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}


CPackFile* FilePackerManager::GetPackFile( char * pszPackFileName, const char * pszFileName)
{
	__UNUSED( pszFileName );
	CPackFile* pPackFile = GetPackObj(pszPackFileName);
	if( pPackFile )
	{
		if(!pPackFile->IsFileOpen())
			OpenPackFile(pszPackFileName);

		return pPackFile;
	}

	return pPackFile;
}


} //namespace util