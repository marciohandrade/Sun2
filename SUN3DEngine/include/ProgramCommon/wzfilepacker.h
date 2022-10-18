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
	  ret : BOOL (���� ���� ������ TRUE ���н� FALSE
	  param1 : lpPathName (������ Packing���ϳ��� ������)
	  desc : Packing�� ���� ������ Ư�� ������ ������ �� ����Ѵ�.

    /////////////////////////////////////////////////////////////////////////*/
	BOOL RemoveDirectory(LPCTSTR lpPathName);
	
	/*////////////////////////////////////////////////////////////////////////
    
	  Func : AddFile
	  ret : BOOL (���� �߰� ������ TRUE ���н� FALSE
	  param1 : lpSourcePath (������ ������ ���� ���)
	  param2 : lpTargetPath (������ ����� Packing���ϳ��� ���)
	  desc : Packing�� ���� ������ Ư�� ������ ������ ������ �ִ´�.

    /////////////////////////////////////////////////////////////////////////*/
	BOOL AddFile(LPCTSTR lpSourcePath, LPCTSTR lpTargetPath);

//	BOOL AddFile(LPCTSTR lpSourcePath, LPCTSTR lpTargetPath);
	
	CPackFile();
	~CPackFile();

	//////////////////////////////////////////////////////////////////////////
	
	
	/*////////////////////////////////////////////////////////////////////////
    
	  Func : Open
	  ret : HANDLE (Packing�� ������ �ڵ�)
	  param1 : pszPackFilePath (Packing������ ���)
	  param2 : pszEncryptionKey (Ű)
	  param3 : dwOpenModeFlags (Open�� �÷���)
	  desc : Packing�� ���Ͽ� �����ϱ� ���� ����Ѵ�. 

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
	  ret : HANDLE (Packing�� ���ϳ����� Ư�� ������ �ڵ�)
	  param1 : lpFileName (Packing���ϳ��ο��� Ư�� ������ ���)
	  param2 : dwOpenModeFlags (Open�� �÷���)
	  desc : Packing�� ���Ͽ��� Ư�� ���Ͽ� �����ϱ� ���� ���.

    /////////////////////////////////////////////////////////////////////////*/
	HANDLE OpenFile(
		LPCTSTR lpFileName,        // file name
		DWORD dwOpenModeFlags
		);
	
	/*////////////////////////////////////////////////////////////////////////
    
	  Func : DeleteFile
	  ret : BOOL (���� ������ �����ߴ��� ����)
	  param1 : lpFileName (Packing���ϳ��ο��� Ư�� ������ ���)
	  desc : Packing�� ���Ͽ��� Ư�� ������ �����ϱ� ���� ���.

    /////////////////////////////////////////////////////////////////////////*/
	BOOL DeleteFile(
		LPCTSTR lpFileName       // file name		
		);
	
	/*////////////////////////////////////////////////////////////////////////
    
	  Func : CloseFile
	  ret : N/a 
	  param1 : hFile (Packing���ϳ��ο��� Ư�� ������ �ڵ�)
	  desc : Packing�� ���Ͽ��� Ư�� ������ ���� �ٽ� ���� �� ���

    /////////////////////////////////////////////////////////////////////////*/
	void CloseFile(HANDLE hFile);                // handle to file

	/*////////////////////////////////////////////////////////////////////////
    
	  Func : FileExists
	  ret : BOOL ���� Ȥ�� ������ �����ϴ��� ����.
	  param1 : lpFileName (Packing���ϳ��ο��� Ư�� ����Ȥ�� ������)
	  desc : Packing�� ���Ͽ��� Ư�� �����̳� ������ �����ϴ��� �˻�

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
	  ret : BOOL ���� �Ӽ� �������� ��������
	  param1 : hFile (�Ӽ��� ������ ������ �ڵ�)
	  param2 : lpFileProp (������ ������ �Ӽ�)
	  desc : Packing�� ���Ͽ��� Ư�� ������ �Ӽ����� ��������.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL GetFileProperties(HANDLE hFile, 
		FileProperties* lpFileProp);
	
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : ReadFile
	  ret : BOOL ���� �б� ����.
	  param1 : hFile (������ �ڵ�)
	  param2 : lpBuffer (����)
	  param3 : nNUmberofBytesToRead ����ũ��
	  desc : Ư�������� �д´�.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL ReadFile(
		HANDLE hFile,                // handle to file
		LPVOID lpBuffer,             // data buffer
		DWORD nNumberOfBytesToRead // number of bytes to read	      
		);
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : WriteFile
	  ret : BOOL ���� ���� ����.
	  param1 : hFile (������ �ڵ�)
	  param2 : lpBuffer (����)
	  param3 : nNUmberofBytesToRead ����ũ��
	  desc : Ư�����Ͽ� ����.��

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL WriteFile(
		HANDLE hFile,                    // handle to file
		LPCVOID lpBuffer,                // data buffer
		DWORD nNumberOfBytesToWrite     // number of bytes to write  
		);
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : GetFileSize
	  ret : BOOL ���� ũ�� �������� ��������
	  param1 : hFile (������ �ڵ�)
	  param2 : lpFileSize ����ũ��/
	  desc : Ư�������� ũ�⸦ �����´�.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL GetFileSize(HANDLE hFile, ULONGLONG* lpFileSize);
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : GetFileSize
	  ret : BOOL ���� ������ �������� ��������
	  param1 : hFile (������ �ڵ�)
	  param2 : lpFilePosition ���Ͽ�����
	  desc : Ư�������� �������� �����´�.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL GetFilePointer(HANDLE hFile, ULONGLONG* lpFilePosition);
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : SetFilePointer
	  ret : BOOL ���� ������ �����ϱ� ��������
	  param1 : hFile (������ �ڵ�)
	  param2 : lpFilePosition ���Ͽ�����
	  desc : Ư�������� �������� �����Ѵ�.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL SetFilePointer(HANDLE hFile,                // handle to file
		ULONGLONG lDistanceToMove,        // bytes to move pointer
		DWORD dwMoveMethod );          // starting point
	
	/*////////////////////////////////////////////////////////////////////////
	    
	  Func : IsEOF
	  ret : BOOL ������ ���ΰ� Ȯ��.
	  param1 : hFile (������ �ڵ�)
	  desc : Ư�������� �������� ������ ���ΰ� Ȯ��.

    /////////////////////////////////////////////////////////////////////////*/	
	BOOL IsEOF(HANDLE hFile);
	

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
	BOOL FindFile(HANDLE hFile, LPCTSTR FileSpec, FileList& fileList, FolderList& folderList, BOOL bRecursive = FALSE);


	/*////////////////////////////////////////////////////////////////////////

	Func : MurgeFile
	ret : BOOL ���� ���հ��	
	hAddFilePath : ���յ� ���� �ڵ�
	bOverWrite : ���� ����� ���� ó�� true: �����. false: skip �Ѵ�.
	desc : hAddFile �ڵ��� ��ŷ������ �ڽſ��� ���ս�Ų��.

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