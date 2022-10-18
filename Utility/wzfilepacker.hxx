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
ret : BOOL (폴더 삭제 성공시 TRUE 실패시 FALSE
param1 : lpPathName (삭제할 Packing파일내의 폴더명)
desc : Packing된 파일 내에서 특정 폴더를 삭제할 때 사용한다.

/////////////////////////////////////////////////////////////////////////*/
BOOL
CPackFile::RemoveDirectory(LPCTSTR lpPathName)
{
    __UNUSED(lpPathName);
    return false;
}

/*////////////////////////////////////////////////////////////////////////

Func : AddFile
ret : BOOL (파일 추가 성공시 TRUE 실패시 FALSE
param1 : lpSourcePath (파일을 복사할 실제 경로)
param2 : lpTargetPath (파일이 복사될 Packing파일내의 경로)
desc : Packing된 파일 내에서 특정 폴더에 파일을 복사해 넣는다.

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
ret : HANDLE (Packing된 파일의 핸들)
param1 : pszPackFilePath (Packing파일의 경로)
param2 : pszEncryptionKey (키)
param3 : dwOpenModeFlags (Open시 플래그)
desc : Packing된 파일에 접근하기 위해 사용한다. 

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
ret : HANDLE (Packing된 파일내부의 특정 파일의 핸들)
param1 : lpFileName (Packing파일내부에서 특정 파일의 경로)
param2 : dwOpenModeFlags (Open시 플래그)
desc : Packing된 파일에서 특정 파일에 접근하기 위해 사용.

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
ret : BOOL (파일 삭제가 성공했는지 여부)
param1 : lpFileName (Packing파일내부에서 특정 파일의 경로)
desc : Packing된 파일에서 특정 파일을 삭제하기 위해 사용.

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
param1 : hFile (Packing파일내부에서 특정 파일의 핸들)
desc : Packing된 파일에서 특정 파일을 열고 다시 닫을 때 사용

/////////////////////////////////////////////////////////////////////////*/
void
CPackFile::CloseFile(HANDLE hFile)                // handle to file
{
    __UNUSED(hFile);
    return;
}

/*////////////////////////////////////////////////////////////////////////

Func : FileExists
ret : BOOL 파일 혹은 폴더가 존재하는지 여부.
param1 : lpFileName (Packing파일내부에서 특정 파일혹은 폴더명)
desc : Packing된 파일에서 특정 파일이나 폴더가 존재하는지 검사

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
ret : BOOL 파일 속성 가져오기 성공여부
param1 : hFile (속성을 가져올 파일의 핸들)
param2 : lpFileProp (가져올 파일의 속성)
desc : Packing된 파일에서 특정 파일의 속성정보 가져오기.

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
ret : BOOL 파일 읽기 여부.
param1 : hFile (파일의 핸들)
param2 : lpBuffer (버퍼)
param3 : nNUmberofBytesToRead 버퍼크기
desc : 특정파일을 읽는다.

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
ret : BOOL 파일 쓰기 여부.
param1 : hFile (파일의 핸들)
param2 : lpBuffer (버퍼)
param3 : nNUmberofBytesToRead 버퍼크기
desc : 특정파일에 쓴다.ㄴ

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
ret : BOOL 파일 크기 가져오기 성공여부
param1 : hFile (파일의 핸들)
param2 : lpFileSize 파일크기/
desc : 특정파일의 크기를 가져온다.

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
ret : BOOL 파일 오프셋 가져오기 성공여부
param1 : hFile (파일의 핸들)
param2 : lpFilePosition 파일오프셋
desc : 특정파일의 오프셋을 가져온다.

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
ret : BOOL 파일 오프셋 설정하기 성공여부
param1 : hFile (파일의 핸들)
param2 : lpFilePosition 파일오프셋
desc : 특정파일의 오프셋을 셋팅한다.

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
ret : BOOL 파일의 끝인가 확인.
param1 : hFile (파일의 핸들)
desc : 특정파일의 오프셋이 파일의 끝인가 확인.

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
ret : BOOL 파일의 끝인가 확인.
param1 : hFile (파일의 핸들)
param2 : FileSpec
param3 : fileList
param4 bRecursive
desc : 특정파일에서 파일리스트 가져오기.

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
ret : BOOL 파일 병합결과    
hAddFilePath : 병합될 파일 핸들
bOverWrite : 같은 경로의 파일 처리 true: 덮어쓴다. false: skip 한다.
desc : hAddFile 핸들의 패킹파일을 자신에게 병합시킨다.

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

