// discaccess.h

#ifndef _PROGRAMCOMMON_WZDISCACCESS_H_
#define _PROGRAMCOMMON_WZDISCACCESS_H_

#include "wzcriticalsection.h"
#include "wzhandlemanager.h"

//------------------------------------------------------------------------------
// Disc 읽기/쓰기 처리 관련

// 파일 핸들
typedef void* DA_FILE;

#define INVALID_DA_FILE ( (void*)0xFFFFFFFF )

// Access 방식
#define DA_READ         ( 0x00000001 )
#define DA_WRITE        ( 0x00000002 )

// Share 모드
#define SM_READ         ( 0x00000001 )
#define SM_WRITE        ( 0x00000002 )

// 파일 속성
#define FA_READONLY     ( 0x00000001 )
#define FA_DIRECTORY    ( 0x00000002 )

// Open 성향
enum ENUM_OPEN_DISPOSITION
{
    OD_CREATE_ALWAYS = 0,   // 새 파일로 무조건 생성, 기존 파일 존재시 새로 생성
    OD_CREATE_NEW,          // 새 파일 생성, 기존 파일 존재시 실패
    OD_OPEN_ALWAYS,         // 무조건 연다, 파일 존재 안하면 새로 생성
    OD_OPEN_EXISTING,       // 파일 존재시 연다, 파일 존재 안하면 실패
    OD_TRUNCATE_EXISTING,   // 기존 파일을 비우고 연다, 파일 존재 안하면 실패
};

// Seek File 기준점
enum ENUM_SEEK_ORIGIN
{
    SO_BEGIN = 0,   // 파일 시작 위치
    SO_CURRENT,     // 현재 위치
    SO_END,         // 파일 끝 위치
};

// 파일 특성
struct St_FileInformation
{
    DWORD m_dwFileAttributes;   // 속성
    FILETIME m_ftLastWriteTime; // 수정 날짜
    DWORD m_dwFileSizeLow;      // Size Low
    DWORD m_dwFileSizeHigh;     // Size High
};

// 기타
struct St_DAFileInfo
{
    DWORD m_dwDisc;
    HANDLE m_handle;
};

//------------------------------------------------------------------------------
// Hard disc & Packer 열기 관련

// Disc Index - OpenFile 에서 dwDisc 로 넣어줄 값

#define EDI_ACTIVE              ( 0 )   // 현재 Active 로 지정된 Disc 를 사용
#define EDI_HDD_CURRENT_PATH    ( 1 )   // HDD 상의 현재 경로를 읽도록 기본적으로 생성된 Disc
#define EDI_USER                ( 2 )   // 여기서 부터 User 가 세팅한 Disc
#define MAX_EDI                 ( 31 )  // 31 까지 가능

// OpenPacker 타입
enum ENUM_OPEN_PACKER_TYPE
{
    OPT_READ = 0,       // 읽기
    OPT_WRITE,          // 쓰기
    OPT_READ_AND_WRITE  // 읽기 쓰기 모두
};

// Disc 종류
enum ENUM_DISC_TYPE
{
    DT_NONE = 0,
    DT_HARDDISK,
    DT_WZFILEPACKER,
};

// Disc 정보
struct St_DADiscInfo
{
    ENUM_DISC_TYPE m_eDiscType; // Disc 종류
    void* m_pPackerPtr;         // Packer 객체
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
    // 생성자/소멸자
    CDiscAccess( void );
    ~CDiscAccess( void );

    // 생성/제거
    BOOL Create( void );
    void Destroy( void );

    // Hard Disc 열기
    BOOL OpenDisc_Hard( DWORD dwDisc, const char* lpszPath = NULL );
    // Packer 열기
    BOOL OpenDisc_Packer( DWORD dwDisc, 
                          const char* lpszPackName, 
                          const char* lpszPassword, 
                          ENUM_OPEN_PACKER_TYPE eOpt = OPT_READ );

    // Disc 닫기
    void CloseDisc( DWORD dwDisc );

    // 활성 Disc 설정
    BOOL SetActiveDisc( DWORD dwDisc );
    // 활성 Disc 얻기
    DWORD GetActiveDisc( void );

    // 파일 열기
    DA_FILE OpenFile( DWORD dwDisc, 
                      const char* lpszFileName,
                      DWORD dwDesiredAccess,
                      DWORD dwShareMode, 
                      ENUM_OPEN_DISPOSITION eOpenDisposition );
    // 파일 닫기
    BOOL CloseFile( DA_FILE dfFile );

    // 파일 포인터 이동
    DWORD SeekFile( DA_FILE dfFile, LONG lOffset, ENUM_SEEK_ORIGIN eSeekOrigin );

    // 파일 읽기
    DWORD ReadFile( DA_FILE dfFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead );
    // 파일에 쓰기
    DWORD WriteFile( DA_FILE dfFile, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite );

    // 파일이 존재하는가?
    BOOL FileExists( DWORD dwDisc, const char* lpFileName );

    // 파일 크기 얻기
    DWORD GetFileSize( DA_FILE dfFile );
    // 파일 정보 얻기
    BOOL GetFileInformationByHandle( DA_FILE dfFile, St_FileInformation* lpFileInformation );

    // 파일 복사
    BOOL CopyFile( DWORD dwDisc, const char* lpTargetPath, const char* lpSourcePathInHDD );
    // 파일 지우기
    BOOL DeleteFile( DWORD dwDisc, const char* lpFileName );

    // 디렉토리 생성
    BOOL CreateDirectory( DWORD dwDisc, const char* lpPathName );
    // 디렉토리 제거
    BOOL RemoveDirectory( DWORD dwDisc, const char* lpPathName );

private:
    // 파일명 교정
    void FixFileName( char* lpszResult, const char* lpszSource );

private:
    St_DADiscInfo m_DADiscInfo[MAX_EDI + 1];
    DWORD m_dwActiveDisc;

    CWzHandleManager<St_DAFileInfo> m_hmFileInfo;

    WzDeclare_Sync( DiscAccess );
};

#endif // _PROGRAMCOMMON_WZDISCACCESS_H_