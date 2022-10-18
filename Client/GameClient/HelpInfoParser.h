//------------------------------------------------------------------------------				    
//  작업자: 이지연
//	2006/03/13
//------------------------------------------------------------------------------

#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>


const int c_iMaxDescCodeNum = 10;


#pragma pack(push,1)

struct BASE_HelpInfo 
{
    DWORD		m_dw_HelpIndex;
	BYTE		m_bHelpMethod;
    DWORD		m_dw_CharStatus;
    DWORD		m_dw_Level;
	DWORD		m_dw_Repeat;
	BYTE		m_bWindow_States;

	DWORD		m_dw_Param;
    DWORD		m_dw_TitleStringCode;
	DWORD		m_dw_DescStringCode[c_iMaxDescCodeNum];

	DWORD		m_dw_ImageCode;
	BYTE		m_bSoundControl;
	DWORD		m_dwSoundValue;
};

struct BASE_HelpIndex
{
     DWORD		m_dw_CharStatus;
     DWORD		m_dw_HelpIndex;
     DWORD		m_dw_Level;
	 DWORD		m_dw_Repeat;
	 BYTE		m_bWindow_States;

	 DWORD		m_dw_Param;

	BYTE		m_bHelpMethod;
	BYTE		m_bSoundControl;
	DWORD		m_dwSoundValue;
};

#pragma pack(pop)


enum eHELP_DESC_INDEX
{
	eHELP_DESC1 = 0,
	eHELP_DESC2,
	eHELP_DESC3,
	eHELP_DESC4,
	eHELP_DESC5,
	eHELP_DESC6,
	eHELP_DESC7,
	eHELP_DESC8,
	eHELP_DESC9,
	eHELP_DESC10,

};


// CharStatus 순으로 정렬
typedef std::multimap<DWORD, BASE_HelpIndex>	HELPINDEX_MAP;
typedef std::pair<DWORD, BASE_HelpIndex>	HELPINDEX_PAIR;
typedef HELPINDEX_MAP::iterator				HELPINDEX_ITER;

class HelpInfoParser	: public Singleton<HelpInfoParser> ,public IParser
{
public:
   HelpInfoParser();
   ~HelpInfoParser();


   VOID							Init(DWORD dwPoolSize , char *szPackfileName);
   VOID							Init(DWORD dwPoolSize);

   VOID							Release();

private:
	VOID						Unload();
	util::SolarHashTable<BASE_HelpInfo *>*      m_pDataTable;

	BOOL						_Load(BOOL bReload);

public:
	virtual	BOOL				LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);
	BASE_HelpInfo*				GetHelpInfo(DWORD dwHelpIndex);
	HELPINDEX_MAP				m_HelpIndex;
	std::vector<DWORD>			m_vecHelpIndex;

};

