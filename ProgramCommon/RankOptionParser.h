#pragma once

//=======================================================================================================================
/// RankOptionParser class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 1. 5
	@remarks
			- 랭크업시 필요한 옵션 정보 스크립트
	@note
			- sRANK_OPTION : 스크립트에서 읽은 원 데이터
			- sRANK_OPTION_SELECTOR : sRANK_OPTION를 가공해서 옵션 선택을 위한 데이터
	@history
		
*/
//=======================================================================================================================

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ItemOptionHeader.h>
#include <hash_map>
#include <ScriptCode.IParser.h>


//enum eRANK_OPTION_ITEM_TYPE
//{
//	eRANK_OPTION_ITEM_WEAPON					= 0,
//	eRANK_OPTION_ITEM_ARMOR						= 1,
//	eRANK_OPTION_ITEM_RING						= 2,
//	eRANK_OPTION_ITEM_NECKLACE					= 3,
//	eRANK_OPTION_ITEM_SACCESSORY_BERSERKER		= 4,
//	eRANK_OPTION_ITEM_SACCESSORY_DRAGON			= 5,
//	eRANK_OPTION_ITEM_SACCESSORY_SHADOW			= 6,
//	eRANK_OPTION_ITEM_SACCESSORY_VALKYRIE		= 7,
//	eRANK_OPTION_ITEM_SACCESSORY_ELEMENTALIST	= 8,
//	eRANK_OPTION_ITEM_TYPE_MAX,
//};

#define MAX_OPTIONNAME_LENGTH		32

#define MAX_ITEM_RANK_OPTION_NUM	100

struct sRANK_OPTION
{
	INT		m_iOptionIndex;
	char	m_tszOptionName[MAX_OPTIONNAME_LENGTH];
	DWORD	m_OptionNCode;
	DWORD	m_NumericType;
	DWORD	m_Value[RANK_MAX];
};

struct sPTION_SELECTOR_FOR_RANK
{
	DWORD	m_OptionCount;
	DWORD	m_OptionIndex[MAX_ITEM_RANK_OPTION_NUM];
};


typedef stdext::hash_map<WORD, sRANK_OPTION *>	RANKOPT_HASH;
typedef std::pair<WORD, sRANK_OPTION *>			RANKOPT_HASH_PAIR;

typedef stdext::hash_map<WORD, sPTION_SELECTOR_FOR_RANK *>	RANKOPT_SEL_HASH;
typedef std::pair<WORD, sPTION_SELECTOR_FOR_RANK *>			RANKOPT_SEL_HASH_PAIR;

class RankOptionParser : public util::Singleton<RankOptionParser>, public IParser
{
public:
	RankOptionParser(void);
	virtual ~RankOptionParser(void);

	VOID				Init( char * pszPackFileName );
	VOID				Release(){}

	VOID				Unload();
	VOID				UnloadSelector();

	VOID				Reload();

	inline sRANK_OPTION *				GetRankOption( WORD wItemType, DWORD OptionIndex );
	inline sPTION_SELECTOR_FOR_RANK *	GetRankOptionSelector( WORD wItemType, eRANK Rank );

public:
	virtual BOOL			LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);
private:
	BOOL					_Load(BOOL bReload );
	VOID					CheckValidity( sRANK_OPTION* pInfo, WORD wItemType );

protected:
	VOID					CalculateOptionSelector();

private:
	RANKOPT_HASH			m_ItemRankOptionHash;
	RANKOPT_SEL_HASH		m_RankOptionSelectorHash;
	char					m_tszFileName[MAX_PATH];
};

sRANK_OPTION * RankOptionParser::GetRankOption( WORD wItemType, DWORD OptionIndex )
{
	RANKOPT_HASH::iterator itr = m_ItemRankOptionHash.find( wItemType );
	if ( itr != m_ItemRankOptionHash.end() )
	{
		sRANK_OPTION * pOptionArray = itr->second;

		if( OptionIndex < MAX_ITEM_OPTION_KIND_NUM )
			return &pOptionArray[OptionIndex];
	}
	return NULL;
}

sPTION_SELECTOR_FOR_RANK * RankOptionParser::GetRankOptionSelector( WORD wItemType, eRANK Rank )
{
	RANKOPT_SEL_HASH::iterator itr = m_RankOptionSelectorHash.find( wItemType );
	if ( itr != m_RankOptionSelectorHash.end() )
	{
		sPTION_SELECTOR_FOR_RANK * pOptionArray = itr->second;
		if( Rank < RANK_MAX )
			return &pOptionArray[Rank];
	}

	return NULL;
}
