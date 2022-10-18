// wzFilePacker.h
//#include "UtilityCommon.h"
//#define PACK_CLASS class CPackFile

CAutoFree autofree;
CAutoFree::~CAutoFree()
{
}

//class CPackFile
BOOL
CPackFile::GetFindFileNameInPackFile(LPCTSTR lpcFileName, LPTSTR lpPackFileName)
{
    __UNUSED(lpcFileName);
    __UNUSED(lpPackFileName);
    return false;
}

void
CPackFile::SetHwnd(HWND hWnd)
{
    __UNUSED(hWnd);
}

/*////////////////////////////////////////////////////////////////////////

Func : RemoveDirectory
ret : BOOL (���� ���� ������ TRUE ���н� FALSE
param1 : lpPathName (������ Packing���ϳ��� ������)
desc : Packing�� ���� ������ Ư�� ������ ������ �� ����Ѵ�.

/////////////////////////////////////////////////////////////////////////*/
BOOL
CPackFile::RemoveDirectory(LPCTSTR lpPathName)
{
    __UNUSED(lpPathName);
    return false;
}

/*////////////////////////////////////////////////////////////////////////

Func : AddFile
ret : BOOL (���� �߰� ������ TRUE ���н� FALSE
param1 : lpSourcePath (������ ������ ���� ���)
param2 : lpTargetPath (������ ����� Packing���ϳ��� ���)
desc : Packing�� ���� ������ Ư�� ������ ������ ������ �ִ´�.

/////////////////////////////////////////////////////////////////////////*/
BOOL
CPackFile::AddFile(LPCTSTR lpSourcePath, LPCTSTR lpTargetPath)
{
    __UNUSED(lpSourcePath);
    __UNUSED(lpTargetPath);
    return false;
}

//  BOOL AddFile(LPCTSTR lpSourcePath, LPCTSTR lpTargetPath);

CPackFile::CPackFile()
{
}

CPackFile::~CPackFile()
{
}

//////////////////////////////////////////////////////////////////////////


/*////////////////////////////////////////////////////////////////////////

Func : Open
ret : HANDLE (Packing�� ������ �ڵ�)
param1 : pszPackFilePath (Packing������ ���)
param2 : pszEncryptionKey (Ű)
param3 : dwOpenModeFlags (Open�� �÷���)
desc : Packing�� ���Ͽ� �����ϱ� ���� ����Ѵ�. 

/////////////////////////////////////////////////////////////////////////*/
HANDLE
CPackFile::Open(LPCTSTR pszPackFilePath, 
                LPCTSTR pszEncryptionKey,
                DWORD dwOpenModeFlags)
{
    __UNUSED(pszPackFilePath);
    __UNUSED(pszEncryptionKey);
    __UNUSED(dwOpenModeFlags);
    return INVALID_HANDLE_VALUE;
}

void
CPackFile::Close()
{
}

//////////////////////////////////////////////////////////////////////////

//  implemented
HANDLE
CPackFile::CreateFile(LPCTSTR lpFileName, ULONGLONG ulReservedSize)
{
    __UNUSED(lpFileName);
    __UNUSED(ulReservedSize);
    return INVALID_HANDLE_VALUE;
}

/*////////////////////////////////////////////////////////////////////////

Func : OpenFile
ret : HANDLE (Packing�� ���ϳ����� Ư�� ������ �ڵ�)
param1 : lpFileName (Packing���ϳ��ο��� Ư�� ������ ���)
param2 : dwOpenModeFlags (Open�� �÷���)
desc : Packing�� ���Ͽ��� Ư�� ���Ͽ� �����ϱ� ���� ���.

/////////////////////////////////////////////////////////////////////////*/
HANDLE
CPackFile::OpenFile(LPCTSTR lpFileName,        // file name
                    DWORD dwOpenModeFlags)
{
    __UNUSED(lpFileName);
    __UNUSED(dwOpenModeFlags);
    return INVALID_HANDLE_VALUE;
}

/*////////////////////////////////////////////////////////////////////////

Func : DeleteFile
ret : BOOL (���� ������ �����ߴ��� ����)
param1 : lpFileName (Packing���ϳ��ο��� Ư�� ������ ���)
desc : Packing�� ���Ͽ��� Ư�� ������ �����ϱ� ���� ���.

/////////////////////////////////////////////////////////////////////////*/
BOOL
CPackFile::DeleteFile(LPCTSTR lpFileName)       // file name     
{
    __UNUSED(lpFileName);
    return false;
}

/*////////////////////////////////////////////////////////////////////////

Func : CloseFile
ret : N/a 
param1 : hFile (Packing���ϳ��ο��� Ư�� ������ �ڵ�)
desc : Packing�� ���Ͽ��� Ư�� ������ ���� �ٽ� ���� �� ���

/////////////////////////////////////////////////////////////////////////*/
void
CPackFile::CloseFile(HANDLE hFile)                // handle to file
{
    __UNUSED(hFile);
    return;
}

/*////////////////////////////////////////////////////////////////////////

Func : FileExists
ret : BOOL ���� Ȥ�� ������ �����ϴ��� ����.
param1 : lpFileName (Packing���ϳ��ο��� Ư�� ����Ȥ�� ������)
desc : Packing�� ���Ͽ��� Ư�� �����̳� ������ �����ϴ��� �˻�

/////////////////////////////////////////////////////////////////////////*/
BOOL
CPackFile::FileExists(LPCTSTR lpFileName)
{
    __UNUSED(lpFileName);
    return false;
}


// not yet implemented
BOOL
CPackFile::RenameFile(LPCTSTR lpFileName, 
                      LPCTSTR lpNewFileName)
{
    __UNUSED(lpFileName);
    __UNUSED(lpNewFileName);
    return false;
}

// not yet implemented
BOOL
CPackFile::CreateDirectory(LPCTSTR lpPathName)
{
    __UNUSED(lpPathName);
    return false;
}


//////////////////////////////////////////////////////////////////////////

/*////////////////////////////////////////////////////////////////////////

Func : GetFileProperties
ret : BOOL ���� �Ӽ� �������� ��������
param1 : hFile (�Ӽ��� ������ ������ �ڵ�)
param2 : lpFileProp (������ ������ �Ӽ�)
desc : Packing�� ���Ͽ��� Ư�� ������ �Ӽ����� ��������.

/////////////////////////////////////////////////////////////////////////*/ 
BOOL
CPackFile::GetFileProperties(HANDLE hFile, 
                             FileProperties* lpFileProp)
{
    __UNUSED(hFile);
    __UNUSED(lpFileProp);
    return false;
}


/*////////////////////////////////////////////////////////////////////////

Func : ReadFile
ret : BOOL ���� �б� ����.
param1 : hFile (������ �ڵ�)
param2 : lpBuffer (����)
param3 : nNUmberofBytesToRead ����ũ��
desc : Ư�������� �д´�.

/////////////////////////////////////////////////////////////////////////*/ 
BOOL
CPackFile::ReadFile(HANDLE hFile,                // handle to file
                    LPVOID lpBuffer,             // data buffer
                    DWORD nNumberOfBytesToRead) // number of bytes to read         
{
    __UNUSED(hFile);
    __UNUSED(lpBuffer);
    __UNUSED(nNumberOfBytesToRead);
    return false;
}

/*////////////////////////////////////////////////////////////////////////

Func : WriteFile
ret : BOOL ���� ���� ����.
param1 : hFile (������ �ڵ�)
param2 : lpBuffer (����)
param3 : nNUmberofBytesToRead ����ũ��
desc : Ư�����Ͽ� ����.��

/////////////////////////////////////////////////////////////////////////*/ 
BOOL
CPackFile::WriteFile(HANDLE hFile,                    // handle to file
                     LPCVOID lpBuffer,                // data buffer
                     DWORD nNumberOfBytesToWrite)     // number of bytes to write  
{
    __UNUSED(hFile);
    __UNUSED(lpBuffer);
    __UNUSED(nNumberOfBytesToWrite);
    return false;
}
/*////////////////////////////////////////////////////////////////////////

Func : GetFileSize
ret : BOOL ���� ũ�� �������� ��������
param1 : hFile (������ �ڵ�)
param2 : lpFileSize ����ũ��/
desc : Ư�������� ũ�⸦ �����´�.

/////////////////////////////////////////////////////////////////////////*/ 
BOOL
CPackFile::GetFileSize(HANDLE hFile, ULONGLONG* lpFileSize)
{
    __UNUSED(hFile);
    __UNUSED(lpFileSize);
    return false;
}

/*////////////////////////////////////////////////////////////////////////

Func : GetFileSize
ret : BOOL ���� ������ �������� ��������
param1 : hFile (������ �ڵ�)
param2 : lpFilePosition ���Ͽ�����
desc : Ư�������� �������� �����´�.

/////////////////////////////////////////////////////////////////////////*/ 
BOOL
CPackFile::GetFilePointer(HANDLE hFile, ULONGLONG* lpFilePosition)
{
    __UNUSED(hFile);
    __UNUSED(lpFilePosition);
    return false;
}

/*////////////////////////////////////////////////////////////////////////

Func : SetFilePointer
ret : BOOL ���� ������ �����ϱ� ��������
param1 : hFile (������ �ڵ�)
param2 : lpFilePosition ���Ͽ�����
desc : Ư�������� �������� �����Ѵ�.

/////////////////////////////////////////////////////////////////////////*/ 
BOOL
CPackFile::SetFilePointer(HANDLE hFile,                // handle to file
                          ULONGLONG lDistanceToMove,        // bytes to move pointer
                          DWORD dwMoveMethod )          // starting point
{
    __UNUSED(hFile);
    __UNUSED(lDistanceToMove);
    __UNUSED(dwMoveMethod);
    return false;
}

/*////////////////////////////////////////////////////////////////////////

Func : IsEOF
ret : BOOL ������ ���ΰ� Ȯ��.
param1 : hFile (������ �ڵ�)
desc : Ư�������� �������� ������ ���ΰ� Ȯ��.

/////////////////////////////////////////////////////////////////////////*/ 
BOOL
CPackFile::IsEOF(HANDLE hFile)
{
    __UNUSED(hFile);
    return false;
}


//////////////////////////////////////////////////////////////////////////
// File Finder

/*////////////////////////////////////////////////////////////////////////

Func : FindFile
ret : BOOL ������ ���ΰ� Ȯ��.
param1 : hFile (������ �ڵ�)
param2 : FileSpec
param3 : fileList
param4 bRecursive
desc : Ư�����Ͽ��� ���ϸ���Ʈ ��������.

/////////////////////////////////////////////////////////////////////////*/ 
BOOL
CPackFile::FindFile(HANDLE hFile, LPCTSTR FileSpec,
                    FileList& fileList, FolderList& folderList,
                    BOOL bRecursive)
{
    __UNUSED(hFile);
    __UNUSED(FileSpec);
    __UNUSED(fileList);
    __UNUSED(folderList);
    __UNUSED(bRecursive);
    return false;
}


/*////////////////////////////////////////////////////////////////////////

Func : MurgeFile
ret : BOOL ���� ���հ��    
hAddFilePath : ���յ� ���� �ڵ�
bOverWrite : ���� ����� ���� ó�� true: �����. false: skip �Ѵ�.
desc : hAddFile �ڵ��� ��ŷ������ �ڽſ��� ���ս�Ų��.

/////////////////////////////////////////////////////////////////////////*/ 
BOOL
CPackFile::MurgeFile(HANDLE hAddFile, BOOL bOverWrite)
{
    __UNUSED(hAddFile);
    __UNUSED(bOverWrite);
    return false;
}


//Extract File
BOOL
CPackFile::ExtractFile(HANDLE hFile, LPCTSTR lpSrcFile, LPCTSTR lpTarFile)
{
    __UNUSED(hFile);
    __UNUSED(lpSrcFile);
    __UNUSED(lpTarFile);
    return false;
}

//Extract all
BOOL
CPackFile::ExtractAll(HANDLE hFile, LPCTSTR lpTarFile)
{
    __UNUSED(hFile);
    __UNUSED(lpTarFile);
    return false;
}

//HeaderEncrypt.
BOOL
CPackFile::EncryptHdr(LPTSTR lpOldWpkPath)
{
    __UNUSED(lpOldWpkPath);
    return false;
}

//HANDLE
//CPackFile::IsFileOpen() { return m_hFile; }
//
//TCHAR*
//CPackFile::GetCurrentOpenFile() {   return lstrlen(m_szFilePath) > 0 ? m_szFilePath : NULL; }


void
CPackFile::WriteHeader(TCHAR *lpOldWpkPath,int iSignature, int iVersion)
{
    __UNUSED(lpOldWpkPath);
    __UNUSED(iSignature);
    __UNUSED(iVersion);
}


//inline void
//CPackFile::UpdateVfsInfo();

//inline void
//CPackFile::CONVERT_PATHCHAR(LPTSTR lpStr);

HANDLE
CPackFile::CreateFinder(HANDLE hFile, LPCTSTR FileSpec)
{
    __UNUSED(hFile);
    __UNUSED(FileSpec);
    return INVALID_HANDLE_VALUE;
}

void
CPackFile::DestroyFinder(HANDLE hFinder)
{
    __UNUSED(hFinder);
    return;
}

BOOL
CPackFile::FinderGetNextFile(HANDLE hFinder)
{
    __UNUSED(hFinder);
    return false;
}

BOOL
CPackFile::FinderGetProperties(HANDLE hFinder, FileProperties* lpFileProp)
{
    __UNUSED(hFinder);
    __UNUSED(lpFileProp);
    return false;
}

//////////////////////////////////////////////////////////////////////////

