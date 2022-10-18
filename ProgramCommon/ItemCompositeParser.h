#ifndef ITEMCOMPOSITEPARSER_H
#define ITEMCOMPOSITEPARSER_H

#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <vector>
#include <ScriptCode.IParser.h>

struct sITEMCOMPOSITEMATERIAL
{
	SLOTCODE	m_dwCompositeMaterialCode;
	BYTE		m_bySpendMaterialNum;
};


struct sITEMCOMPOSITE
{
	enum { MAX_COMPOSITE_MATERIAL = 5, };

	SLOTCODE				m_Code;			// eITEMCOMPOSITECODE 참조 
	BYTE					m_byResult;		// ItemCompositeResult.txt 참조.
	BYTE					m_byMatCount;
	sITEMCOMPOSITEMATERIAL	m_sCompositeMaterials[MAX_COMPOSITE_MATERIAL ];	// 필요 메터리얼 / 개수
	MONEY					m_Money;
};

class ItemCompositeParser :	public util::Singleton<ItemCompositeParser>, public IParser
{
public:		ItemCompositeParser(void);
	virtual	~ItemCompositeParser(void);

public:		VOID					Init( DWORD dwCompositeInfoSize );
			VOID					Init( DWORD dwCompositeInfoSize, char* pszPackFileName );
			VOID					Release();
			VOID					Reload();

			sITEMCOMPOSITE*			GetCompositeInfo( eITEMCOMPOSITECODE CompositeCode ) { return m_pCompositeInfoHashTable->GetData( CompositeCode ); }

public:
	virtual	BOOL					LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload );
private:	BOOL					_Load( BOOL bReload );
protected:
	VOID							Unload();

private:
	util::SolarHashTable<sITEMCOMPOSITE *>*			m_pCompositeInfoHashTable;
	char											m_pszFileName[MAX_PATH];
};

#endif
//ITEMCOMPOSITEPARSER_H