#ifndef     __uiQuestListContent_H__
#define     __uiQuestListContent_H__
//------------------------------------------------------------------------------
/**
    @class uiQuestListContent

    퀘스트 리스트의 내용

    (C) 2005 y2jinc 
*/

#include "SCItemSlotContainer.h"
#include "Quest.h"


// typedefs 
typedef DWORD							RESOURCECODE;       ///< 그래픽 리소스
typedef WORD							KEYSETCODE;			///< DInput CODE 
typedef util::_tstring					SUN_STRING;			///< 
typedef basic_string<TCHAR>::size_type	STR_INDEX;			/// 
typedef std::list<Quest *>		QUEST_LIST;

struct SortQuestListForGroupOps
{
	bool operator()( CODETYPE a, CODETYPE b ) const
	{
		return a < b;
	}
};

//////////////////////////////////////////////////////////////////////////

class uiQuestListContent
{
public:
	typedef std::map< CODETYPE, QUEST_LIST, SortQuestListForGroupOps >	QUEST_GROUP_MAP;
	typedef QUEST_GROUP_MAP::iterator									QUEST_GROUP_ITR;
	typedef std::pair<CODETYPE, QUEST_LIST>							QUEST_GROUP_PAIR;

    uiQuestListContent();
    virtual ~uiQuestListContent();

   //--------------------------------------------------------------------------
    // functions
    //
public:
	void ClearInfo();
	
	DWORD CompileQuestStream(BSTR bstrXml, BYTE* ProgrQStream, BYTE* ComplQStream);

	static void DecompileQuestInfo(BYTE * pProgrStream, BYTE * pComplStream, BSTR* bstrXml);
	
    //--------------------------------------------------------------------------
    // member vars
    //
	QUEST_GROUP_MAP			m_QuestGroup;	
};

//------------------------------------------------------------------------------
#endif  //  __uiQuestListContent_H__