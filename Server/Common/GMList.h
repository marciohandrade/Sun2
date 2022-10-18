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

// GMList.h/cpp �� �̻� ������� �ʽ��ϴ�. Ȥ�ö� GM üũ�� ������� ���ñ� �ٶ��ϴ�(����â)

enum eGM_GRADE
{
	eGM_GRADE_NORMAL	= 0,	// �Ϲ���
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
	WORD			m_wIndex;							// �ε���
	TCHAR			m_szID[ID_MAX_LEN];					// ���� ID
	TCHAR			m_szPW[PASSWD_MAX_LEN];				// ���� PassWord
	char			m_szIP[IP_MAX_LEN];					// IP
	BYTE			m_byGrade;							// ���� ���
	TCHAR			m_szDepartment[MAX_TITLEID_LENGTH];	// �μ�
	TCHAR			m_szName[MAX_CHARNAME_LENGTH];		// �̸�
	TCHAR			m_szRole[MAX_TITLEID_LENGTH];		// ������
	eDATE_TYPE		m_eExpireDate;						// ���ᳯ¥
};

#endif


typedef std::map< std::string, GM_INFO*>				GM_LIST_MAP;
typedef GM_LIST_MAP::iterator							GM_LIST_MAP_ITER;

// GMList Ŭ���� �� �̻� ������� �ʽ��ϴ�. Ȥ�ö� GM üũ�� ������� ���ñ� �ٶ��ϴ�(����â)
// ���� �� ��Ƽ� ���� ����(�Ǵ� �ּ�ó��)�� �����Դϴ�
// ===> ����� ���� �𸨴ϴ�. �켱 �����մϴ�. ( 2007�� 1�� 10�� ����� )

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














