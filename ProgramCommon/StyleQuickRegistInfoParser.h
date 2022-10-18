#pragma once


//----------------------------------------------------------------------------
/**
	@author : �� �� ��( y2jinc@webzen.com )
	@desc 
		�ڵ����� ��Ÿ�� �� �����̳ʿ� ��Ÿ�� ������ ������ֱ� ���� �ʿ��� ��ũ��Ʈ ������ ����.
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



















