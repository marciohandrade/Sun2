//------------------------------------------------------------------------------				    
//  작업자: 이지연
//	2006/06/14
//------------------------------------------------------------------------------

#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

#pragma pack(push,1)

struct BASE_OptionList 
{
    DWORD mdwIndex;
    DWORD mdwNameCode;
#ifdef _SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED
    BYTE valuetype;
#else
  #ifdef _NA_2088_20100322_ADD_OPTIONLIST_PARSER_RATIOVALUE_FILED
    BYTE m_byRatioValue;
  #endif //_NA_2088_20100322_ADD_OPTIONLIST_PARSER_RATIOVALUE_FILED
#endif //_SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED

#ifdef _SCJ_MODIFY_OPTIONLIST_PARSER_VALUETYPE_FILED_TWO
    BYTE valuetype2;
#endif //_SCJ_MODIFY_OPTIONLIST_PARSER_VALUETYPE_FILED_TWO
};

#pragma pack(pop)

class OptionListParser	: public util::Singleton<OptionListParser> ,public IParser
{
public:
   OptionListParser();
   ~OptionListParser();

   enum eOPTION_VALUE_TYPE //옵션의 접미사 출력 방식
   {
       eOPTION_VALUE_TYPE_NORMAL = 0, 
       eOPTION_VALUE_TYPE_PERCENTAGE = 1, // 항상 "%" 붙임
       eOPTION_VALUE_TYPE_SECOND = 2,     // 항상 "초" 붙임
   };
   enum eOPTION_VALUE_TYPE_TWO //옵션의 접미사 출력 방식
   {
       eOPTION_VALUE_TYPE_TWO_BENEFIT = 0,  // 이득인 옵션
       eOPTION_VALUE_TYPE_TWO_REVERSE_BENEFIT = 1, // 음수가 이득인 옵션
   };

   VOID								Init(DWORD dwPoolSize , char *szPackfileName);
   VOID								Init(DWORD dwPoolSize);

   VOID								Release();

private:
	VOID							Unload();
	util::SolarHashTable<BASE_OptionList*>*     m_pDataTable;
//#ifdef _NA_002935_20110704_ITEM_SCORE
    typedef STLX_HASH_MAP<eATTR_TYPE, BASE_OptionList*> OptionListMap;
    OptionListMap option_list_map_;
//#endif

	BOOL							_Load(BOOL bReload);
public:
	virtual	BOOL					LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	BASE_OptionList*				GetOptionList(DWORD dwIndex);

    BASE_OptionList*                GetOptionListByAttr(const eATTR_TYPE attr_type);
	
};
