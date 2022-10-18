#ifndef __WZFILEPACKER_H__
#define __WZFILEPACKER_H__
// wzFilePacker.h

#include <Windows.h>

#pragma warning(disable: 4786)
#include <vector>
#include <string>

#define PACKFILE_OPEN_READONLY		0x00000001
#define PACKFILE_OPEN_UPDATE 	0x00000002
#define PACKFILE_OPEN_CREATE	 0x00000004
#define PACKFILE_OPEN_DIRECTORY		0x00000008

#define	PACKFILE_ATTRIBUTE_READONLY	0x00000001
#define	PACKFILE_ATTRIBUTE_DIRECTORY	0x00000002

typedef	struct	FileProperties
{
	FILETIME			Time;
	DWORD				dwAttributeFlags;
	ULONGLONG				dwSize;
	char			    Name[1024];
} FileProperties;



typedef std::vector<std::string> FileList;
typedef std::vector<std::string> FolderList;

#define WM_PROGRESS WM_USER+927

#define PACK_CLASS class CPackFile

#ifdef _DYNAMIC_LIB
 #define PACK_CLASS class __declspec(dllexport) CPackFile
#endif

class CAutoFree
{
public:
	CAutoFree() {};
	~CAutoFree();
};

extern CAutoFree autofree;

class CPackFile
{	
public:
	static BOOL GetFindFileNameInPackFile(LPCTSTR lpcFileName, LPTSTR lpPackFileName);
	void SetHwnd(HWND hWnd);

	/*////////////////////////////////////////////////////////////////////////
    
	  Func : RemoveDirectory
	  ret : BOOL (폴더 삭제 성공시 TRUE 실패시 FALSE
	  param1 : lpPathName (삭제할 Packing파일내의 폴더명)
	  desc : Packing된 파일 내에서 특정 폴더를 삭제할 때 사용한다.

    /////////////////////////////////////////////////////////////////////////*/
	BOOL RemoveDirectory(LPCTSTR lpPathName);
	
	/*////////////////////////////////////////////////////////////////////////
    
	  Func : AddFile
	  ret : BOOL (파일 추가 성공시 TRUE 실패시 FALSE
	  param1 : lpSourcePath (파일을 복사할 실제 경로)
	  param2 : lpTargetPath (파일이 복사될 Packing파일내의 경로)
	  desc : Packing된 파일 내에서 특정 폴더에 파일을 복사해 넣는다.

    /////////////////////////////////////////////////////////////////////////*/
	BOOL AddFile(LPCTSTR lpSourcePath, LPCTSTR lpTargetPath);

//	BOOL AddFile(LPCTSTR lpSourcePath, LPCTSTR lpTargetPath);
	
	CPackFile();
	~CPackFile();

	//////////////////////////////////////////////////////////////////////////
	
	
	/*////////////////////////////////////////////////////////////////////////
    
	  Func : Open
	  ret : HANDLE (Packing된 파일의 핸들)
	  param1 : pszPackFilePath (Packing파일의 경로)
	  param2 : pszEncryptionKey (키)
	  param3 : dwOpenModeFlags (Open시 플래그)
	  desc : Packing된 파일에 접근하기 위해 사용한다. 

    /////////////////////////////////////////////////////////////////////////*/
	HANDLE Open(LPCTSTR pszPackFilePath, 
				LPCTSTR pszEncryptionKey = NULL,
				DWORD dwOpenModeFlags = PACKFILE_OPEN_READONLY | PACKFILE_OPEN_DIRECTORY);

	void Close();

	//////////////////////////////////////////////////////////////////////////
		
	//  implemented
	HANDLE CreateFile(LPCTSTR lpFileName, ULONGLONG ulReservedSize);
	
	/*////////////////////////////////////////////////////////////////////////
    
	  Func : OpenFile
	  ret : HANDLE (Packing된 파일내부의 특정 파일의 핸들)
	  param1 : lpFileName (Packing파일내부에서 특정 파일의 경로)
	  param2 : dwOpenModeFlags (Open시 플래그)
	  desc : Packing된 파일에서 특정 파일에 접근하기 위해 사용.

    /////////////////////////////////////////////////////////////////////////*/
	HANDLE OpenFile(
		LPCTSTR lpFileName,        // file name
		DWORD dwOpenModeFlags
		);
	
	/*////////////////////////////////////////////////////////////////////////
    
	  Func : DeleteFile
	  ret : BOOL (파일 삭제가 성공했는지 여부)
	  param1 : lpFileName (Packing파일내부에서 특정 파일의 경로)
	  desc : Packing된 파일에서 특정 파일을 삭제하기 위해 사용.

    /////////////////////////////////////////////////////////////////////////*/
	BOOL DeleteFile(
		LPCTSTR lpFileName       // file name		
		);
	
	/*////////////////////////////////////////////////////////////////////////
    
	  Func : CloseFile
	  ret : N/a 
	  param1 : hFile (Packing파일내부에서 특정 파일의 핸들)
	  desc : Packing된 파일에서 특정 파일을 열고 다시 닫을 때 사용

    /////////////////////////////////////////////////////////////////////////*/
	void CloseFile(HANDLE hFile);                // handle to file

	/*////////////////////////////////////////////////////////////////////////
    
	  Func : FileExists
	  ret : BOOL 파일 혹은 폴더가 존재하는지 여부.
	  param1 : lpFileName (Packing파일내부에서 특정 파일혹은 폴더명)
	  desc : Packing된 파일에서 특정 파일이나 폴더가 존재하는지 검사

    /////////////////////////////////////////////////////////////////////////*/
	BOOL FileExists(LPCTSTR lpFileName);
	
	
	// not yet implemented
	BOOL RenameFile(
		LPCTSTR lpFileName, 
		LPCTSTR lpNewFileName);
	
	// not yet implemented
	BOOL CreateDirectory(LPCTSTR lpPathName);
	
	
	//////////////////////////////////////////////////////////////////////////
		
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : GetFileProperties
	  ret : BOOL 파일 속성 가져오기 성공여부
	  param1 : hFile (속성을 가져올 파일의 핸들)
	  param2 : lpFileProp (가져올 파일의 속성)
	  desc : Packing된 파일에서 특정 파일의 속성정보 가져오기.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL GetFileProperties(HANDLE hFile, 
		FileProperties* lpFileProp);
	
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : ReadFile
	  ret : BOOL 파일 읽기 여부.
	  param1 : hFile (파일의 핸들)
	  param2 : lpBuffer (버퍼)
	  param3 : nNUmberofBytesToRead 버퍼크기
	  desc : 특정파일을 읽는다.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL ReadFile(
		HANDLE hFile,                // handle to file
		LPVOID lpBuffer,             // data buffer
		DWORD nNumberOfBytesToRead // number of bytes to read	      
		);
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : WriteFile
	  ret : BOOL 파일 쓰기 여부.
	  param1 : hFile (파일의 핸들)
	  param2 : lpBuffer (버퍼)
	  param3 : nNUmberofBytesToRead 버퍼크기
	  desc : 특정파일에 쓴다.ㄴ

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL WriteFile(
		HANDLE hFile,                    // handle to file
		LPCVOID lpBuffer,                // data buffer
		DWORD nNumberOfBytesToWrite     // number of bytes to write  
		);
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : GetFileSize
	  ret : BOOL 파일 크기 가져오기 성공여부
	  param1 : hFile (파일의 핸들)
	  param2 : lpFileSize 파일크기/
	  desc : 특정파일의 크기를 가져온다.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL GetFileSize(HANDLE hFile, ULONGLONG* lpFileSize);
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : GetFileSize
	  ret : BOOL 파일 오프셋 가져오기 성공여부
	  param1 : hFile (파일의 핸들)
	  param2 : lpFilePosition 파일오프셋
	  desc : 특정파일의 오프셋을 가져온다.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL GetFilePointer(HANDLE hFile, ULONGLONG* lpFilePosition);
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : SetFilePointer
	  ret : BOOL 파일 오프셋 설정하기 성공여부
	  param1 : hFile (파일의 핸들)
	  param2 : lpFilePosition 파일오프셋
	  desc : 특정파일의 오프셋을 셋팅한다.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL SetFilePointer(HANDLE hFile,                // handle to file
		ULONGLONG lDistanceToMove,        // bytes to move pointer
		DWORD dwMoveMethod );          // starting point
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : IsEOF
	  ret : BOOL 파일의 끝인가 확인.
	  param1 : hFile (파일의 핸들)
	  desc : 특정파일의 오프셋이 파일의 끝인가 확인.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL IsEOF(HANDLE hFile);
	

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
	BOOL FindFile(HANDLE hFile, LPCTSTR FileSpec, FileList& fileList, FolderList& folderList, BOOL bRecursive = FALSE);


	/*////////////////////////////////////////////////////////////////////////

	Func : MurgeFile
	ret : BOOL 파일 병합결과	
	hAddFilePath : 병합될 파일 핸들
	bOverWrite : 같은 경로의 파일 처리 true: 덮어쓴다. false: skip 한다.
	desc : hAddFile 핸들의 패킹파일을 자신에게 병합시킨다.

	/////////////////////////////////////////////////////////////////////////*/	
	BOOL MurgeFile(HANDLE hAddFile, BOOL bOverWrite);

	
	//Extract File
	BOOL ExtractFile(HANDLE hFile, LPCTSTR lpSrcFile, LPCTSTR lpTarFile);

	//Extract all
	BOOL ExtractAll(HANDLE hFile, LPCTSTR lpTarFile);

	//HeaderEncrypt.
	static BOOL EncryptHdr(LPTSTR lpOldWpkPath);

	HANDLE IsFileOpen() { return m_hFile; }

	TCHAR* GetCurrentOpenFile() {	return lstrlen(m_szFilePath) > 0 ? m_szFilePath : NULL; }


	void WriteHeader(TCHAR *lpOldWpkPath,int iSignature, int iVersion);


private:

	inline void UpdateVfsInfo();

	static inline void CONVERT_PATHCHAR(LPTSTR lpStr);

	HANDLE  CreateFinder(HANDLE hFile, LPCTSTR FileSpec);
	
	void DestroyFinder(HANDLE hFinder);
	
	BOOL FinderGetNextFile(HANDLE hFinder);
	
	BOOL FinderGetProperties(HANDLE hFinder, FileProperties* lpFileProp);	
	
	
	HANDLE m_hFile;

	HWND g_Hwnd;

	TCHAR m_szFilePath[1024];
};

//////////////////////////////////////////////////////////////////////////

//};

#endif //__WZFILEPACKER_H__