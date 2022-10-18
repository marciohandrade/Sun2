// discaccess.h

#ifndef _PROGRAMCOMMON_WZDISCACCESS_H_
#define _PROGRAMCOMMON_WZDISCACCESS_H_

#include "wzcriticalsection.h"
#include "wzhandlemanager.h"

//------------------------------------------------------------------------------
// Disc �б�/���� ó�� ����

// ���� �ڵ�
typedef void* DA_FILE;

#define INVALID_DA_FILE ( (void*)0xFFFFFFFF )

// Access ���
#define DA_READ         ( 0x00000001 )
#define DA_WRITE        ( 0x00000002 )

// Share ���
#define SM_READ         ( 0x00000001 )
#define SM_WRITE        ( 0x00000002 )

// ���� �Ӽ�
#define FA_READONLY     ( 0x00000001 )
#define FA_DIRECTORY    ( 0x00000002 )

// Open ����
enum ENUM_OPEN_DISPOSITION
{
    OD_CREATE_ALWAYS = 0,   // �� ���Ϸ� ������ ����, ���� ���� ����� ���� ����
    OD_CREATE_NEW,          // �� ���� ����, ���� ���� ����� ����
    OD_OPEN_ALWAYS,         // ������ ����, ���� ���� ���ϸ� ���� ����
    OD_OPEN_EXISTING,       // ���� ����� ����, ���� ���� ���ϸ� ����
    OD_TRUNCATE_EXISTING,   // ���� ������ ���� ����, ���� ���� ���ϸ� ����
};

// Seek File ������
enum ENUM_SEEK_ORIGIN
{
    SO_BEGIN = 0,   // ���� ���� ��ġ
    SO_CURRENT,     // ���� ��ġ
    SO_END,         // ���� �� ��ġ
};

// ���� Ư��
struct St_FileInformation
{
    DWORD m_dwFileAttributes;   // �Ӽ�
    FILETIME m_ftLastWriteTime; // ���� ��¥
    DWORD m_dwFileSizeLow;      // Size Low
    DWORD m_dwFileSizeHigh;     // Size High
};

// ��Ÿ
struct St_DAFileInfo
{
    DWORD m_dwDisc;
    HANDLE m_handle;
};

//------------------------------------------------------------------------------
// Hard disc & Packer ���� ����

// Disc Index - OpenFile ���� dwDisc �� �־��� ��

#define EDI_ACTIVE              ( 0 )   // ���� Active �� ������ Disc �� ���
#define EDI_HDD_CURRENT_PATH    ( 1 )   // HDD ���� ���� ��θ� �е��� �⺻������ ������ Disc
#define EDI_USER                ( 2 )   // ���⼭ ���� User �� ������ Disc
#define MAX_EDI                 ( 31 )  // 31 ���� ����

// OpenPacker Ÿ��
enum ENUM_OPEN_PACKER_TYPE
{
    OPT_READ = 0,       // �б�
    OPT_WRITE,          // ����
    OPT_READ_AND_WRITE  // �б� ���� ���
};

// Disc ����
enum ENUM_DISC_TYPE
{
    DT_NONE = 0,
    DT_HARDDISK,
    DT_WZFILEPACKER,
};

// Disc ����
struct St_DADiscInfo
{
    ENUM_DISC_TYPE m_eDiscType; // Disc ����
    void* m_pPackerPtr;         // Packer ��ü
    ENUM_OPEN_PACKER_TYPE m_eOpenPackerType;
    char m_lpszPath[MAX_PATH];
};

//------------------------------------------------------------------------------
/**
    @class CDiscAccess
*/
class CDiscAccess
{
public:
    // ������/�Ҹ���
    CDiscAccess( void );
    ~CDiscAccess( void );

    // ����/����
    BOOL Create( void );
    void Destroy( void );

    // Hard Disc ����
    BOOL OpenDisc_Hard( DWORD dwDisc, const char* lpszPath = NULL );
    // Packer ����
    BOOL OpenDisc_Packer( DWORD dwDisc, 
                          const char* lpszPackName, 
                          const char* lpszPassword, 
                          ENUM_OPEN_PACKER_TYPE eOpt = OPT_READ );

    // Disc �ݱ�
    void CloseDisc( DWORD dwDisc );

    // Ȱ�� Disc ����
    BOOL SetActiveDisc( DWORD dwDisc );
    // Ȱ�� Disc ���
    DWORD GetActiveDisc( void );

    // ���� ����
    DA_FILE OpenFile( DWORD dwDisc, 
                      const char* lpszFileName,
                      DWORD dwDesiredAccess,
                      DWORD dwShareMode, 
                      ENUM_OPEN_DISPOSITION eOpenDisposition );
    // ���� �ݱ�
    BOOL CloseFile( DA_FILE dfFile );

    // ���� ������ �̵�
    DWORD SeekFile( DA_FILE dfFile, LONG lOffset, ENUM_SEEK_ORIGIN eSeekOrigin );

    // ���� �б�
    DWORD ReadFile( DA_FILE dfFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead );
    // ���Ͽ� ����
    DWORD WriteFile( DA_FILE dfFile, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite );

    // ������ �����ϴ°�?
    BOOL FileExists( DWORD dwDisc, const char* lpFileName );

    // ���� ũ�� ���
    DWORD GetFileSize( DA_FILE dfFile );
    // ���� ���� ���
    BOOL GetFileInformationByHandle( DA_FILE dfFile, St_FileInformation* lpFileInformation );

    // ���� ����
    BOOL CopyFile( DWORD dwDisc, const char* lpTargetPath, const char* lpSourcePathInHDD );
    // ���� �����
    BOOL DeleteFile( DWORD dwDisc, const char* lpFileName );

    // ���丮 ����
    BOOL CreateDirectory( DWORD dwDisc, const char* lpPathName );
    // ���丮 ����
    BOOL RemoveDirectory( DWORD dwDisc, const char* lpPathName );

private:
    // ���ϸ� ����
    void FixFileName( char* lpszResult, const char* lpszSource );

private:
    St_DADiscInfo m_DADiscInfo[MAX_EDI + 1];
    DWORD m_dwActiveDisc;

    CWzHandleManager<St_DAFileInfo> m_hmFileInfo;

    WzDeclare_Sync( DiscAccess );
};

#endif // _PROGRAMCOMMON_WZDISCACCESS_H_