// CSunOnline.h : Declaration of the CCSunOnline

#pragma once
#include "resource.h"       // main symbols
#include <SCItemSlot.h>
#include <SCItemSlotContainer.h>
#include <SCSkillSlot.h>
#include <SCSkillSlotContainer.h>

#include <ItemInfoParser.h>
#include <SkillInfoParser.h>
#include <MissionManager.h>
#include <MapInfoParser.h>
#include <RankOptionParser.h>
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include <TitleListParser.h>
#include <ReputeListParser.h>
#include <FameListParser.h>
#endif

//#include "Quest.h"
#include <QuestScriptLoader.h>
#include <QuestInfoManager.h>
#include <QuestManager.h>

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#include <AchievementParser.h>
#endif
//#include <AreaConquestInfoParser.h>
//#include <AreaConquest.h>
#include "AreaConquestManager.h"
#include "rewardinfolist.h"

#include "logic/uiQuestListContent.h"
#include "logic/uiQuestMissionList.h"

#include <ItemTypeList.h>

#include "StringTableParser.h"

#ifdef _NA002217_100728_EP2_SKILL_SYSTEM_DB_
#include <SkillSystem/perkinfoparser.h>
#endif

/*
enum 
{
	SKILL_EXPERTTYPE_A = 0,
	SKILL_EXPERTTYPE_B,
	SKILL_PASSIVE,
	SKILL_STYLE
};
*/

using namespace std;

extern CStringTableParser* g_pStrTblParser[5];

#define STRTBL_PARSER1	(g_pStrTblParser[0])	//��Ʈ�����̺� �ļ�
#define STRTBL_PARSER2	(g_pStrTblParser[1])	//��Ʈ�����̺�	�ļ�
#define STRTBL_PARSER3	(g_pStrTblParser[2])	//��Ʈ�����̺� �ļ�
#define STRTBL_PARSER4	(g_pStrTblParser[3])	//��Ʈ�����̺� �ļ�
#define STRTBL_PARSER5	(g_pStrTblParser[4])	//��Ʈ�����̺� �ļ�

void SetXMLCharSet(CMarkupArchive& xml);

enum {
	INTERNATIONAL_KOREA = 0,	
	INTERNATIONAL_ENGLISH	,
	INTERNATIONAL_JAPAN,	
	INTERNATIONAL_TAIWAN,	
	INTERNATIONAL_CHINA,
    INTERNATIONAL_RUSSIAN,
};

extern WORD wLocaleCode;

class CSunOnlineItem;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// ICSunOnline
[
	object,
	uuid("14E845B9-0298-4CA1-97AC-6E9536865051"),
	dual,	helpstring("ICSunOnline Interface"),
	pointer_default(unique)
]
__interface ICSunOnline : IDispatch
{	
	[id(1), helpstring("method decodeSkill")] HRESULT decodeSkill([in] BYTE buff[MAX_SKILL_SLOT_NUM*2], [in] BYTE charClassType, [out,retval] BSTR* bstrXml);
	[id(2), helpstring("method decodeMission")] HRESULT decodeMission([in] BYTE buff[MAX_MISSIONSTREAM_SIZE], [out,retval] BSTR* bstrXml);

	[id(3), helpstring("method decodeInventoryItem")] HRESULT decodeInventoryItem([in] BYTE buff[MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], [in] int iCurrentVer, [in] BYTE ExtraInventoryTabCount, [in] BSTR bstrDesc, [out,retval] BSTR* bstrXml);
	[id(4), helpstring("method decodeTempInventoryItem")] HRESULT decodeTempInventoryItem([in] BYTE buff[MAX_TEMPINVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], [in] int iCurrentVer, [out,retval] BSTR* bstrXml);
	[id(5), helpstring("method decodeEquipItem")] HRESULT decodeEquipItem([in] BYTE buff[MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX)], [in] int iCurrentVer, [out,retval] BSTR* bstrXml);
	[id(6), helpstring("method decodeWarehouseItem")] HRESULT decodeWarehouseItem([in] BYTE buff[MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], [in] int iCurrentVer, [in] BSTR bstrDesc, [out,retval] BSTR* bstrXml);

	[id(7), helpstring("method encodeSkill")] HRESULT encodeSkill([in] BSTR bstrXml, [out] BYTE buff[MAX_SKILL_SLOT_NUM*2], [in] BYTE charClassType, [out,retval] int* nResult);
	[id(8), helpstring("method encodeMission")] HRESULT encodeMission([in] BSTR bstrXml, [out] BYTE buff[MAX_MISSIONSTREAM_SIZE], [out,retval] int* nResult);

	[id(9), helpstring("method encodeInventoryItem")] HRESULT encodeInventoryItem([in] BSTR bstrXml, [out] BYTE buff[MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], [out] int* iNewVer, [out,retval] int* nResult);
	[id(10), helpstring("method encodeTempInventoryItem")] HRESULT encodeTempInventoryItem([in] BSTR bstrXml, [out] BYTE buff[MAX_TEMPINVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], [out] int* iNewVer, [out,retval] int* nResult);
	[id(11), helpstring("method encodeEquipItem")] HRESULT encodeEquipItem([in] BSTR bstrXml, [out] BYTE buff[MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX)], [out] int* iNewVer, [out,retval] int* nResult);
	[id(12), helpstring("method encodeWarehouseItem")] HRESULT encodeWarehouseItem([in] BSTR bstrXml, [out] BYTE buff[MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], [out] int* iNewVer, [out,retval] int* nResult);

    [id(13), helpstring("method init")] HRESULT init([in] BSTR bstrLocale, [in] BYTE DBVersion);
	[id(14), helpstring("method release")] HRESULT release(void);
	[id(15), helpstring("method createItem")] HRESULT createItem(void);	
	[id(16), helpstring("method decodeCharStat")] HRESULT decodeCharStat([in] BYTE buff[sizeof(BASE_PLAYERINFO)], [out,retval] BSTR* bstrXml);

	[id(17), helpstring("method decodeQuest")] HRESULT decodeQuest([in] BYTE ProgrQStream[MAX_PROGR_QUESTSTREAM_SIZE], [in] BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], 
																	[out,retval] BSTR* bstrXml);
	[id(18), helpstring("method encodeQuest")] HRESULT encodeQuest([in] BSTR bstrXml, [out] BYTE ProgrQStream[MAX_PROGR_QUESTSTREAM_SIZE], [out] BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], [out,retval] int* nResult);
#ifdef __NA_1288_AC_RENEWAL_FOR_TIME_LINE
	[id(19), helpstring("method decodeACStream")] HRESULT decodeACStream([in] BYTE ACStream[MAX_ACSTREAM_SIZE], [in] int Version , [in] BYTE Mission[MAX_MISSIONSTREAM_SIZE], [in] BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], [out,retval] BSTR* bstrXml);
#else
	[id(19), helpstring("method decodeACStream")] HRESULT decodeACStream([in] BYTE ACStream[MAX_ACSTREAM_SIZE], [in] BYTE Mission[MAX_MISSIONSTREAM_SIZE], [in] BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], [out,retval] BSTR* bstrXml);
#endif
	
	[id(20), helpstring("method encodeACStream")] HRESULT encodeACStream([in] BSTR bstrXml, [out] BYTE ACStream[MAX_ACSTREAM_SIZE], [out,retval] int* nResult);

#ifdef __NA000584_20080201_GUILD_WAREHOUSE
	//2008-05-22 ������ ���â�� ��,���ڵ� �Ҽ� �ִ� �������̽�
	[id(21), helpstring("method encodeGuildWarehouseItem")] HRESULT encodeGuildWarehouseItem([in] BSTR bstrXml, [out] BYTE buff[MAX_GUILD_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], [out] int* iNewVer, [out,retval] int* nResult);
	[id(22), helpstring("method decodeGuildWarehouseItem")] HRESULT decodeGuildWarehouseItem([in] BYTE buff[MAX_GUILD_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], [in] int iCurrentVer, [in] BSTR bstrDesc, [out,retval] BSTR* bstrXml);
#endif
	
#ifdef _NA_008323_20150602_MODIFY_ITEM_SERIALNUMBER_40BITDATA
    [id(23), helpstring("method GetWorldSerial")] HRESULT GetWorldSerial([in] BYTE DBType, [in] INT64 Serial, [out,retval] BSTR* WorldSerial);
#else
    [id(23), helpstring("method GetWorldSerial")] HRESULT GetWorldSerial([in] BYTE DBType, [in] ULONG Serial, [out,retval] BSTR* WorldSerial);
#endif //_NA_008323_20150602_MODIFY_ITEM_SERIALNUMBER_40BITDATA
    [id(24), helpstring("method GetItemStreamVersion")] HRESULT GetItemStreamVersion([out,retval] int* Version);

#ifdef _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_
    [id(25), helpstring("method GetSkillInfo")] HRESULT GetSkillInfo([in] int skill_code, [out] int* skill_level, [out] int* max_skill_level, [out] BSTR* skill_name, [out ,retval] BOOL* result);
#endif
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    [id(26), helpstring("method GetPerkInfo")] HRESULT GetPerkInfo([in] unsigned short group_code, [in] unsigned short leve, [out] BSTR* name, [out] BSTR* request_exp, [out ,retval] BOOL* result);
#endif
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    [id(27), helpstring("method GetACInfo")] HRESULT GetACInfo([in] int index, [out] BSTR* achievement_name, [out] BSTR* location_name, [out] int* location_code, [out] int* valid_count, int achievement_valu[MAX_ACHIEVEMENT_OBJECT], [out ,retval] BOOL* result);    
#endif
#ifdef _NA002217_100728_EP2_SKILL_SYSTEM_DB_
    [id(28), helpstring("method DecodeItemInfo")] HRESULT DecodeItemInfo([in] BYTE position, [in] __int64 serial, [in] int code, [in] BYTE dura, [in] BYTE version, [in] BYTE option[sizeof(OPTIONPART)], [out,retval] BSTR* bstrXml);    
    [id(29), helpstring("method EncodeItemInfo")] HRESULT EncodeItemInfo([in] BSTR bstrXml, [out] __int64* serial, [out] int* code, [out] BYTE* dura, [out] BYTE* version, [out] BYTE option[sizeof(OPTIONPART)], [out ,retval] BOOL* result);    
#endif	
    [id(30), helpstring("method GetMissionName")] HRESULT GetMissionName([in] int mission_code, [out] BSTR* mission_name, [out] BSTR* area_name, [out ,retval] BOOL* result);
};



// CCSunOnline

[
	coclass,
	threading("apartment"),
	vi_progid("gameLogic_Sun.CSunOnline"),
	progid("gameLogic_Sun.CSunOnline.1"),
	version(1.0),
	uuid("28094A5A-19C9-4ED9-AC75-AC8369AB1E11"),
	helpstring("CSunOnline Class")
]
class ATL_NO_VTABLE CCSunOnline : 
	public ICSunOnline
{
public:
	CCSunOnline()
	{
		OutputDebugString("=== Game Logic Start ===");
		//_LOG("=== Game Logic Start ===");
		m_bInitialized = FALSE;		
	}
	~CCSunOnline()
	{	
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

public:
	//////////////////////////////////////////////////////////////////////////////////////
	// func : decodeSkill
	// param1 : ���ڵ��� ��ų ���̳ʸ� ������
	// param2 : ���ϰ����� ���� XML��Ʈ��������
	// ret : S_OK, S_FAIL
	// desc : ��ų ���̳ʸ� �����͸� XML�������� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////
	STDMETHOD(decodeSkill)(BYTE buff[MAX_SKILL_SLOT_NUM*2], BYTE charClassType, BSTR* bstrXml);
	//////////////////////////////////////////////////////////////////////////////////////
	// func : decodeMission
	// param1 : ���ڵ��� �̼� ���̳ʸ� ������
	// param2 : ���ϰ����� ���� XML��Ʈ��������
	// ret : S_OK, S_FAIL
	// desc : �̼� ���̳ʸ� �����͸� XML�������� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////
	STDMETHOD(decodeMission)(BYTE buff[MAX_MISSIONSTREAM_SIZE], BSTR* bstrXml);

	//2008-05-22 ������ ���â�� ��,���ڵ� �Ҽ� �ִ� �������̽�
#ifdef __NA000584_20080201_GUILD_WAREHOUSE
	STDMETHOD(decodeGuildWarehouseItem)(BYTE buff[MAX_GUILD_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], int iCurrentVer, BSTR bstrDesc, BSTR* bstrXml);
	STDMETHOD(encodeGuildWarehouseItem)(BSTR bstrXml, BYTE buff[MAX_GUILD_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], int* iNewVer, int* nResult);
#endif 

#ifdef _NA_008323_20150602_MODIFY_ITEM_SERIALNUMBER_40BITDATA
    STDMETHOD(GetWorldSerial)(BYTE DBType, INT64 Serial, BSTR* WorldSerial);
#else
    STDMETHOD(GetWorldSerial)(BYTE DBType, ULONG Serial, BSTR* WorldSerial);
#endif //_NA_008323_20150602_MODIFY_ITEM_SERIALNUMBER_40BITDATA
    STDMETHOD(GetItemStreamVersion)(int* Version);    

	STDMETHOD(decodeInventoryItem)(BYTE buff[MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], int iCurrentVer, BYTE ExtraInventoryTabCount, BSTR bstrDesc, BSTR* bstrXml);
	STDMETHOD(decodeTempInventoryItem)(BYTE buff[MAX_TEMPINVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], int iCurrentVer, BSTR* bstrXml);
	STDMETHOD(decodeEquipItem)(BYTE buff[MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX)], int iCurrentVer, BSTR* bstrXml);
	STDMETHOD(decodeWarehouseItem)(BYTE buff[MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], int iCurrentVer, BSTR bstrDesc, BSTR* bstrXml);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : encodeSkill
	// param1 : ���ڵ��� ��ų XML������
	// param2 : ���ڵ��� ���̳ʸ������Ͱ� ������ ����
	// ret : S_OK, S_FAIL
	// desc : ��ų XML�����͸� ���̳ʸ� �����ͷ� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////
	STDMETHOD(encodeSkill)(BSTR bstrXml, BYTE buff[MAX_SKILL_SLOT_NUM*2], BYTE charClassType, int* nResult);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : encodeMission
	// param1 : ���ڵ��� �̼� XML������
	// param2 : ���ڵ��� ���̳ʸ������Ͱ� ������ ����
	// ret : S_OK, S_FAIL
	// desc : �̼� XML�����͸� ���̳ʸ� �����ͷ� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////
	STDMETHOD(encodeMission)(BSTR bstrXml, BYTE buff[MAX_MISSIONSTREAM_SIZE], int* nResult);

	STDMETHOD(encodeInventoryItem)(BSTR bstrXml, BYTE buff[MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], int* iNewVer, int* nResult);
	STDMETHOD(encodeTempInventoryItem)(BSTR bstrXml, BYTE buff[MAX_TEMPINVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], int* iNewVer, int* nResult);
	STDMETHOD(encodeEquipItem)(BSTR bstrXml, BYTE buff[MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX)], int* iNewVer, int* nResult);
	STDMETHOD(encodeWarehouseItem)(BSTR bstrXml, BYTE buff[MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], int* iNewVer, int* nResult);
	//////////////////////////////////////////////////////////////////////////
	
	STDMETHOD(decodeQuest)( BYTE ProgrQStream[MAX_PROGR_QUESTSTREAM_SIZE], 
							    BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], 
							    //BYTE Mission[MAX_MISSIONSTREAM_SIZE],
							    BSTR* bstrXml);

	STDMETHOD(encodeQuest)( BSTR bstrXml, BYTE ProgrQStream[MAX_PROGR_QUESTSTREAM_SIZE], BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], int* nResult);
#ifdef __NA_1288_AC_RENEWAL_FOR_TIME_LINE
	STDMETHOD(decodeACStream)( BYTE ACStream[MAX_ACSTREAM_SIZE], int Version,BYTE Mission[MAX_MISSIONSTREAM_SIZE], BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], BSTR* bstrXml);
#else
	STDMETHOD(decodeACStream)( BYTE ACStream[MAX_ACSTREAM_SIZE], BYTE Mission[MAX_MISSIONSTREAM_SIZE], BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], BSTR* bstrXml);
#endif
	STDMETHOD(encodeACStream)( BSTR bstrXml, BYTE ACStream[MAX_ACSTREAM_SIZE], int* nResult);

#ifdef _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_
    STDMETHOD(GetSkillInfo)( int skill_code, int* skill_level, int* max_skill_level, BSTR* skill_name, BOOL* result);    
#endif
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    STDMETHOD(GetPerkInfo)(unsigned short group_code, unsigned short leve, BSTR* name, BSTR* request_exp, BOOL* result);
#endif
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    STDMETHOD(GetACInfo)(int index, BSTR* achievement_name, BSTR* location_name, int* location_code, int* valid_count, int achievement_valu[MAX_ACHIEVEMENT_OBJECT], BOOL* result);    
#endif
#ifdef _NA002217_100728_EP2_SKILL_SYSTEM_DB_
    STDMETHOD(DecodeItemInfo)(BYTE position, __int64 serial, int code, BYTE dura, BYTE version, BYTE option[sizeof(OPTIONPART)], BSTR* bstrXml);    
    STDMETHOD(EncodeItemInfo)(BSTR bstrXml, __int64* serial, int* code, BYTE* dura, BYTE* version, BYTE option[sizeof(OPTIONPART)], BOOL* result);    
#endif	
    STDMETHOD(GetMissionName)( int mission_code, BSTR* mission_name, BSTR* area_name, BOOL* result);        

	//////////////////////////////////////////////////////////////////////////////////////
	// func : init
	// param1 : n/a
	// ret : n/a
	// desc : �ʿ��� ��ũ��Ʈ ���ϵ��� �ε��Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////
	STDMETHOD(init)(BSTR bstrLocale, BYTE DBVersion);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : release
	// param1 : n/a
	// ret : n/a
	// desc : ��ũ��Ʈ ������ �б� ���� �� �ļ����� release�Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////
	STDMETHOD(release)(void);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : createItem
	// param1 : n/a
	// ret : n/a
	// desc : �������� �����ϱ� ���� �ʿ��� �������� XML���Ϸ� �������ش�.
	//////////////////////////////////////////////////////////////////////////////////////
	STDMETHOD(createItem)(void);

	STDMETHODIMP decodeCharStat(BYTE buff[sizeof(BASE_PLAYERINFO)], BSTR* bstrXml);

private:

	//////////////////////////////////////////////////////////////////////////////////////
	// func : LoadParser	
	// char *pszFileName : ��ũ��Ʈ ���ϸ�
	// eSCRIPT_CODE eCode : ��ũ��Ʈ���Ͽ� ���� ���� �ش�.
	// IParser* pParser : �ļ� ��ä ������
	//////////////////////////////////////////////////////////////////////////////////////	
#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
	BOOL LoadParser( const char *pszFileName, eSCRIPT_CODE eCode, IParser* pParser, BOOL bBigData=FALSE ) const;
#else	// __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
	BOOL LoadParser(const char *pszFileName, eSCRIPT_CODE eCode, IParser* pParser) const;
#endif	// __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
    
    template <typename PARSER_T>
    void LoadScriptParser(
        const eSCRIPT_CODE script_code, 
        const std::string& app_path, 
        const std::string& script_file_name) const
    {
        std::string start_msg("Start to load script.|");
        start_msg += script_file_name;
        _LOG(start_msg.c_str());

        std::string path(app_path);
        path += "\\";
        path += script_file_name;
        
        if (LoadParser(path.c_str(), script_code, PARSER_T::Instance()) == TRUE)
        {
            std::string success_msg("Success to load script.|");
            success_msg += script_file_name;
            _LOG(success_msg.c_str());
        }
        else
        {
            std::string fail_msg("Fail to load script.|");
            fail_msg += script_file_name;
            _LOG(fail_msg.c_str());
        }
    }

	//////////////////////////////////////////////////////////////////////////////////////
	// func : ConvertSocketType
	// param1 : SOCKET�� enumŸ�԰�
	// ret : enum���� ��Ʈ�������� ������ ��
	// desc : enum���� ��Ʈ������ �����Ѵ�. ex> SOCKET_1  --> "SOCKET_1"
	//////////////////////////////////////////////////////////////////////////////////////
	std::string ConvertSocketType(eSOCKET sock);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : ConvertSocketType
	// param1 : SOCKET�� ��Ʈ��Ÿ�԰�
	// ret : ��Ʈ������ enum������ ������ ��
	// desc : ��Ʈ������ enum������ �����Ѵ�. ex> "SOCKET_1"  --> SOCKET_1
	//////////////////////////////////////////////////////////////////////////////////////
	eSOCKET ConvertSocketType(std::string sSock);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : ConvertRankType
	// param1 : RANK�� enumŸ�԰�
	// ret : enum���� ��Ʈ�������� ������ ��
	// desc : enum���� ��Ʈ������ �����Ѵ�. ex> RANK_B  --> "RANK_B"
	//////////////////////////////////////////////////////////////////////////////////////
	std::string ConvertRankType(eRANK rank);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : ConvertSocketType
	// param1 : RANK�� ��Ʈ��Ÿ�԰�
	// ret : ��Ʈ������ enum������ ������ ��
	// desc : ��Ʈ������ enum������ �����Ѵ�. ex> "RANK_B"  --> RANK_B
	//////////////////////////////////////////////////////////////////////////////////////
	eRANK ConvertRankType(std::string sRank);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : ConvertItemType
	// param1 : ITEMTYPE�� ��Ʈ��Ÿ�԰�
	// ret : ��Ʈ������ enum������ ������ ��
	// desc : ��Ʈ������ enum������ �����Ѵ�. ex> "��յ���"  --> eITEMTYPE_TWOHANDAXE
	//////////////////////////////////////////////////////////////////////////////////////
//	eITEMTYPE ConvertItemType(std::string sItem);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : ConvertItemType
	// param1 : ITEMTYPE�� enumŸ�԰�
	// ret : enum���� ��Ʈ�������� ������ ��
	// desc : enum���� ��Ʈ������ �����Ѵ�. ex> eITEMTYPE_TWOHANDAXE  --> "��յ���"
	//////////////////////////////////////////////////////////////////////////////////////
//	std::string ConvertItemType(eITEMTYPE Item);
//	std::string ConvertRankOptKind(eOPTION_KIND kind);
//	eOPTION_KIND ConvertRankOptKind(std::string sKind);
	
	BOOL isInitialized();
	void GetItemChargeExpireTime(IN BYTE byChargeSubType, IN SCItemSlot& slot, CMarkupArchive& xml, OUT char* pszTime);
	void SetItemChargeExpireTime(IN BYTE byChargeSubType, IN SCItemSlot& slot, CMarkupArchive& xml, IN const char* pszTime);
	

private:

//	AreaConquestManager m_ACManager;
	
	BOOL m_bInitialized;

//	typedef struct _SKILLINFO
//	{
//		char  			szSkillName[MAX_SKILLNAME_LENGTH];        // ��ų �̸�
//		CODETYPE		dwClassDefine;					// Ŭ���� ����
//		LEVELTYPE		wMaxLV;								// �ְ� ����
//	}SKILLINFO;
//	
//	typedef std::map<SLOTCODE, SKILLINFO> SKILL_BOOK;
//
//	SKILL_BOOK m_SkillBook;
 
	/*	
	Class Type

	1 ����Ŀ
	2 �巡��
	3 ������
	4 ��Ű��
	5 ������
	7 �巡��(����1��)
	8 �巡��(����2��)
	*/
	typedef struct _SKILL_INFO
	{
		BYTE bSkillTab1;   // 0 �нú�/��Ÿ�� 1 ���õ�1 2 ���õ�2
		BYTE bSkillTab2;   // 0-1 �нú꽺ų 0-2 ��Ÿ�� 1-1 ��ų 2-1 ��ų
		LEVELTYPE wLevel;
		BYTE isActive; // 0 Inactive, 1 Active
	} SKILL_INFO;

	typedef std::map<SLOTCODE, SKILL_INFO> SKILL_PRESET;
	SKILL_PRESET m_SkillPreset[eCHAR_TYPE_MAX];
	SKILL_PRESET m_BasicSkillPreset[eCHAR_TYPE_MAX];

    typedef struct _TILE_ACCODE_INFO
    {
        WORD ac_code_;
        DWORD name_code_;
    } TILE_ACCODE_INFO;

    
    typedef std::map<WORD, TILE_ACCODE_INFO> TILTLE_ACCODE_MAP;
    TILTLE_ACCODE_MAP title_accode;
	
protected:

    BOOL IsVertyPetName(TCHAR* PetName);
    BOOL IsVertyPetName(BYTE PetNameByte);

    void encodeRideItem(SCItemSlot& slot, CMarkupArchive& xml);
    void decodeRideItem(SCItemSlot& slot, CMarkupArchive& xml);

	void encodeEtherItemInfo(SCItemSlot& slot, CMarkupArchive& xml);
	void decodeEtherItemInfo(SCItemSlot& slot, CMarkupArchive& xml);

	void encodePetItemInfo(SCItemSlot& slot, CMarkupArchive& xml);
	void decodePetItemInfo(SCItemSlot& slot, CMarkupArchive& xml);

	// ���̳ʸ��� �ִ� Limited �Ӽ��� XML �� ���Ѵ�.
	void WriteLimited(SCItemSlot& slot, CMarkupArchive& xml, int iCurrentVer);
	// ����Ƽ�� �Ӽ��� �о ���̳ʸ��� �ִ´�.
	void ReadLimited(SCItemSlot& slot, CMarkupArchive& xml);
	// ���̳ʸ��� �ִ� Fate �Ӽ��� XML �� �߰��Ѵ�.
	void WriteFate(SCItemSlot& slot, CMarkupArchive& xml, OUT char* pszTime);
	// Fate �Ӽ��� �о ���̳ʸ��� �ִ´�.
	void ReadFate(SCItemSlot& slot, CMarkupArchive& xml, IN const char* pszTime);
	// xml ������ �����Ѵ�.


	STDMETHODIMP decodeWareHouse(BYTE* buff, int nBuffSize, int nSoltSize, int nContainSize, int iCurrentVer, BSTR bstrDesc, BSTR* bstrXml, const char* szMethodName, const char* szTagName);
	// â��� ��� â�� ���ڵ��Ѵ�.
	STDMETHODIMP encodeWareHouse(BSTR bstrXml, BYTE* buff, int nBufSize, int nSoltSize, int nContainSize, int* iNewVer, int* nResult, const char* szMethodName, const char* szTagName);
	// â��� ��� â�� ���ڵ��Ѵ�.

	//��ųŸ���� �����Ѵ�.
	int GetSkillType(BYTE bSkillTab1, BYTE bSkillTab2);


	/*
	pszFileName: ������ ���ϸ�
	bRealName: true ������ �� ���鶧 1���̸�
			   false ������ �� ���鶧 1���̸�+2���̸�
	*/
	bool CreateXMLFile(const char* pszFileName, bool bRealName);
    bool CreateQuestfoFile();
    bool CreateACInfoFile();
    bool CreateSkillInfoFile();
    bool CreateHonorSystemInfoFile();
    bool CreateCurrencyInfo() const;
    bool CreateRandomKey() const;
    bool CreateRandomItemInfo() const;
    bool CreateGuildFacilityInfo() const;

	void DecodeItem(SCItemSlot& slot, CMarkupArchive& xml, POSTYPE pos, int iCurrentVer);
	STDMETHODIMP EncodeItem(SCItemSlot& slot, CMarkupArchive& xml, POSTYPE& SlotIdx);

    friend class CSunOnlineItem;
};






