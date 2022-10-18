#ifndef __GM_LIST_H__
#define __GM_LIST_H__

#pragma once

#include <map>
#include <string>
#include <Singleton.h>
#include <SolarFileReader.h>

#ifdef __NA_20070102_GMIP_CHECK
#include <StructInPacket.h>
#endif

using namespace util;

// GMList.h/cpp 더 이상 사용하지 않습니다. 혹시라도 GM 체크에 사용하지 마시기 바랍니다(성현창)

enum eGM_GRADE
{
	eGM_GRADE_NORMAL	= 0,	// 일반인
	eGM_GRADE_ONE,
	eGM_GRADE_TWO,
	eGM_GRADE_THREE,
	eGM_GRADE_SUPER,
};

#ifndef __NA_20070102_GMIP_CHECK

struct eDATE_TYPE
{
	WORD	m_wYear;
	WORD	m_wMonth;
	WORD	m_wDay;
};

#define MAX_DATE_LENGTH 11

struct GM_INFO
{
	WORD			m_wIndex;							// 인덱스
	TCHAR			m_szID[ID_MAX_LEN];					// 계정 ID
	TCHAR			m_szPW[PASSWD_MAX_LEN];				// 계정 PassWord
	char			m_szIP[IP_MAX_LEN];					// IP
	BYTE			m_byGrade;							// 권한 등급
	TCHAR			m_szDepartment[MAX_TITLEID_LENGTH];	// 부서
	TCHAR			m_szName[MAX_CHARNAME_LENGTH];		// 이름
	TCHAR			m_szRole[MAX_TITLEID_LENGTH];		// 담당업무
	eDATE_TYPE		m_eExpireDate;						// 만료날짜
};

#endif


typedef std::map< std::string, GM_INFO*>				GM_LIST_MAP;
typedef GM_LIST_MAP::iterator							GM_LIST_MAP_ITER;

// GMList 클래스 더 이상 사용하지 않습니다. 혹시라도 GM 체크에 사용하지 마시기 바랍니다(성현창)
// 차후 날 잡아서 전부 제거(또는 주석처리)할 예정입니다
// ===> 사용할 지도 모릅니다. 우선 보류합니다. ( 2007년 1월 10일 조재식 )

class GMList : public Singleton<GMList>, public IParser
{
public:
	enum { MAX_DEFAULT_POOL_SIZE = 50 };
	GMList();	~GMList();

	VOID				Release();
	BOOL				Load( TCHAR* pszFileName, BOOL bReload = FALSE );
	VOID				Reload();
	GM_INFO*			FindGM( const TCHAR *pszUserID );
	BOOL				AddGM( GM_INFO* pInfo );
	GM_LIST_MAP_ITER	Begin(){ return m_mapGMList.begin(); }
	GM_LIST_MAP_ITER	End(){ return m_mapGMList.end(); }
	
private:
	BOOL				_LoadGMList( util::SolarFileReader& sr, BOOL bReload );

#ifdef __NA_000252_20070208_VERSION_GMCMD
	virtual BOOL			ILoad( eSCRIPT_CODE ScriptCode, BYTE* pSTREAM, DWORD dwSize );
	virtual	BOOL			_LoadEx( eSCRIPT_CODE ScriptCode, util::SolarFileReader& sr, BOOL bReload );
#else
	virtual BOOL			ILoad( SCRIPT_CODE_T scriptCode, BYTE* pSTREAM, DWORD dwSize );
#endif

#ifdef __NA_20070102_GMIP_CHECK
	VOID				SerializeGMListStream( GM_TOTAL_INFO& TotalInfo, BOOL bSave );
#endif 

private:
	GM_LIST_MAP							m_mapGMList;
	TCHAR								m_pszGMListName[MAX_PATH];
};

#endif // __GM_LIST_H__














