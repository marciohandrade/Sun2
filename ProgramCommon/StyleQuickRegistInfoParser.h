#pragma once


//----------------------------------------------------------------------------
/**
	@author : 유 재 영( y2jinc@webzen.com )
	@desc 
		자동으로 스타일 퀵 컨테이너에 스타일 슬롯을 등록해주기 위해 필요한 스크립트 정보를 관리.
*/

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

const BYTE MAX_SKILL_REGIST_COUNT = 12;
const BYTE MAX_STYLE_REGIST_COUNT = 4;

struct BASE_StyleQuickRegistInfo 
{
	BYTE	m_byType;
	BYTE	m_byClass;
	BYTE	m_byWeapon;
	WORD	m_wStyleClassCode[MAX_SKILL_REGIST_COUNT];	
};

class StyleQuickRegistInfoParser : public util::Singleton<StyleQuickRegistInfoParser>, public IParser
{
public:
   StyleQuickRegistInfoParser();
   virtual ~StyleQuickRegistInfoParser();

   VOID							Init( DWORD dwPoolSize);
   VOID							Init( DWORD dwPoolSize, char * pszPackFileName );
   VOID							Release();
   VOID							Reload();

   VOID							SetFirst() { m_pDataTable->SetFirst();	}
   BASE_StyleQuickRegistInfo *	GetNext() { return m_pDataTable->GetNext(); }


   BASE_StyleQuickRegistInfo *	GetStyleQuickRegistInfo(int iClassType, int iWeaponKind);
public:
	BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL _Load( BOOL bReload );

protected:
	DWORD						MakeKey(int iClassType, int iWeaponKind);

private:
	VOID						Unload();

private:
	util::SolarHashTable<BASE_StyleQuickRegistInfo *>*	m_pDataTable;
	char												m_pszFileName[MAX_PATH];
};



















