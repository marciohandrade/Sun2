//----------------------------------------------------------------------------
/**
@desc : 해당 필드의 난이도에 따른 레벨 제한 정보 
@author	: 유재영 (y2jinc@webzen.co.kr)
@remark :
*/


#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>
#include <map>
#include <boost/shared_ptr.hpp>	// boost library 필요


// 미션일때 난이도가 의미가 없다.

#define INVALID_DIFFICULT	99

#define	MIN_GUILD_PVP_INDEX	2000
#define MAX_GUILD_PVP_INDEX	3000


struct BASE_RequireFieldLevelInfo {

     DWORD    m_dwIndex;
//   BYTE     m_byMkind;
     WORD     m_wMcode;
//   BYTE     m_byField;
     BYTE     m_byDiff;
     WORD     m_wMinlv;
     WORD     m_wMaxlv;

};


class RequireFieldLevelInfoParser	: public util::Singleton<RequireFieldLevelInfoParser>, public IParser
{
	typedef boost::shared_ptr<BASE_RequireFieldLevelInfo>									BASE_RequireFieldLevelInfoPtr;
	typedef std::multimap<DWORD, boost::shared_ptr<BASE_RequireFieldLevelInfo> >			RequireFieldLevelInfoType;
	typedef std::multimap<DWORD, boost::shared_ptr<BASE_RequireFieldLevelInfo> >::iterator	RequireFieldLevelInfoItrType;

public:
   RequireFieldLevelInfoParser();
   ~RequireFieldLevelInfoParser();


   VOID				Init(DWORD dwPoolSize);
   VOID				Init(DWORD dwPoolSize, char * pszPackFileName);
   VOID				Release();
   VOID				Reload();

   BASE_RequireFieldLevelInfo*            GetRequireFieldLevelInfo( WORD wMcode, BYTE byDiff, BOOL bGuildPVP = FALSE );

public:
	virtual BOOL	LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

private:
	BOOL			_Load(BOOL bReload);
	VOID			Unload();
	DWORD			MakeKey(WORD wMcode, BYTE byDiff );

	//util::SolarHashTable<BASE_RequireFieldLevelInfo *>*		m_pDataTable;
	RequireFieldLevelInfoType								m_LevelInfoMap;
	char													m_pszFileName[MAX_PATH];
};
