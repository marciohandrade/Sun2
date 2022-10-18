#pragma  once

#include <SolarHashTable.h>
#include "ItemOptionHeader.h"
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>
#include <hash_map>

#define _MAX_LIMITED_ITEM_OPTION_NUM		(128)			//< 소켓 옵션 개수 128
#define MAX_LIMITED_OPTIONNAME_LENGTH		(32)

struct sLIMITED_OPTION
{
	INT				m_AttrIndex;				// 키	
	char			m_pszAttrName[MAX_LIMITED_OPTIONNAME_LENGTH];
	eVALUE_TYPE		m_NumericType;
	INT				m_iValue[LIMITED_MAX];	
};

struct sLIMITED_ITEM_OPTION
{
	WORD			m_LimitedOptionIndex;
	char			m_pszName[MAX_LIMITED_OPTIONNAME_LENGTH];
	BYTE			m_ItemType;				// 0: 무기, 1: 방어구, 2: 악세서리
	WORD			m_OptionKind;
	eVALUE_TYPE		m_NumericType;
    WORD			m_MinValue;
	WORD			m_MaxValue;
    int				m_Probability[5];
	int				m_ProbabilitySum;
};

typedef stdext::hash_map<WORD, sLIMITED_OPTION *>		LIMITEDOPT_HASH;
typedef stdext::hash_map<WORD, sLIMITED_ITEM_OPTION *>	LIMITED_ITEM_OPT_HASH;

class LimitedOptionParser : public util::Singleton<LimitedOptionParser>, public IParser
{
public:
	LimitedOptionParser(void);
	virtual ~LimitedOptionParser(void);

	VOID				Init( char * pszPackFileName );
	VOID				Release();


	VOID				Unload();

	sLIMITED_OPTION * GetLimitedOptionByKind( WORD OptionIndex );
	
	virtual	BOOL		LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

protected:
	WORD				ValidCheck(sLIMITED_ITEM_OPTION *pInfo);
private:
	BOOL				_Load( BOOL bReload = FALSE );

	LIMITEDOPT_HASH									m_LimitedOptionKindHash;  //옵션 종류별
	LIMITED_ITEM_OPT_HASH							m_LimitedOptionListHash;  //겜블 스크립트의 정보

	char											m_pszFileName[MAX_PATH];
};