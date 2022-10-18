// CSunOnline.cpp : Implementation of CCSunOnline

#include "stdafx.h"
#include "CSunOnline.h"

#include <strsafe.h>

#pragma warning (disable:4786 4244 4101)

#include <map>
#include <string>
#include <iterator>
using namespace std;

#include "../lib/XMLParser/MarkupArchive.h"
using namespace markup;

#include "util/util.h"
#include "util/FileLogger.h"

#include <time.h>

#include "QuestEx.h"

#ifdef _NA_001699_20110104_SOCKET_OPTION_POINT    
#include <OptionListParser.h>
#endif

#include "logic/MissionRewardParser.h"

#include "VersionControl.h"
#include "SolarDateTime.h"
//#endif
//
//#ifdef BUILD_KOREA_061211

#include <SkillInventoryStoreParser.h>
//#include "SkillInventoryStoreParser.h"

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "CurrencyInfoParser.h"
#endif // _NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA_003740_20111122_RANDOM_ITEM
#include "RandomItemParser.h"
#endif // _NA_003740_20111122_RANDOM_ITEM
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildFacilityInfoParser.h"
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#include "logic/logic.h"
#include ".\csunonline.h"
#include "./CSunOnlineItem.h"
//#include "Afxtempl.h"

//#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/log4cxx.lib")
#pragma comment(lib, "ws2_32.lib")

QuestInfoManager2* g_pQuestInfoManager = NULL;

WORD wLocaleCode = INTERNATIONAL_KOREA;

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
#include "AccumulatePointParser.h"
#endif


//////////////////////////////////////////////////////////////////////////

enum { ENCODE_SUCCESS = 0 }; // 인코딩 성공
enum { INVALID_XML_FORMAT = -400 }; // XML 파싱 에러

#ifdef CHECK
    #undef CHECK
#endif
//#define CHECK(x) {if(!x) {return S_FALSE;} }
#define CHECK(x, err) {if(!x) {_LOG("Not Found Tag Name: %s", err);return E_FAIL;} }



//////////////////////////////////////////////////////////////////////////
// CCSunOnline

CStringTableParser* g_pStrTblParser[5] = {0};


LPCTSTR GenerateDmpName()
{
	static	TCHAR		fileName[127];
	//	static	int			count = 0;
	SYSTEMTIME	time;

	//	count++;
	GetLocalTime( &time );
	wsprintf( fileName, "C:\\WOPSConfig\\SUNGMS\\gameLogic_SUN_%.4d.%.2d.%.2d_%.2d%.2d.dmp", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
	return fileName;
}

void CrashResult( LPCTSTR stackWalkLog, LPCTSTR dumpFileName )
{
	_LOG("CrashFile is generated : %s ", dumpFileName);
}

void SetXMLCharSet(CMarkupArchive& xml)
{
    if(INTERNATIONAL_TAIWAN == wLocaleCode)
        xml.SetEncodingString("big5");
    else if(INTERNATIONAL_KOREA ==wLocaleCode )
        xml.SetEncodingString("euc-kr");
    else if(INTERNATIONAL_CHINA ==wLocaleCode )
        xml.SetEncodingString("gb2312");
    else if(INTERNATIONAL_JAPAN == wLocaleCode)
        xml.SetEncodingString("shift-JIS");
    else if(INTERNATIONAL_RUSSIAN == wLocaleCode)
        xml.SetEncodingString("Windows-1251");
}


HRESULT CCSunOnline::FinalConstruct()
{

	return S_OK;
}

void CCSunOnline::FinalRelease() 
{
//	CrashHandler::Uninstall();
	release();
}


BOOL CCSunOnline::isInitialized()
{
	if(!m_bInitialized)
	{
		_LOG("[ERROR] gameLogic is not initialized. ");
		return FALSE;
	}

	return TRUE;
}

STDMETHODIMP CCSunOnline::decodeSkill(BYTE buff[MAX_SKILL_SLOT_NUM*2], BYTE charClassType, BSTR* bstrXml)
{

	_LOG(">> CCSunOnline::decodeSkill()");
	if(!isInitialized())
		return S_FALSE;
	
		
	if( (charClassType > eCHAR_TYPE_MAX ) || 
		(charClassType <= 0 ) )
	{
		_LOG("@Error - invalid charClassType %d", charClassType);
		return S_FALSE;
	}
	

	USES_CONVERSION;

	SKILLSTREAM* pStream = reinterpret_cast<SKILLSTREAM*>(buff);

    CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate |
                       CMarkupArchive::ArchiveModeStoring |
                        CMarkupArchive::ArchiveModeNotZipped);

    SetXMLCharSet(xml);
	
	xml.Open("C:\\WOPSConfig\\SUNGMS\\decodeSkill.xml");

	//스킬컨테이너 초기화
	SCSkillSlotContainer SkillContainer;
	SkillContainer.Init(MAX_SKILL_SLOT_NUM, SI_SKILL);

	//스킬컨테이너에 바이너리 스킬 데이터를 삽입한다.
	for(POSTYPE pos = 0;pos < SkillContainer.GetMaxSlotNum();++pos)
	{
		//	if(pStream[pos].Part.wCode != 0)		//0값이면 빈 슬롯임을 의미한다.
		if( pStream[pos].Part.IsValid() )
		{	
						
			if(SkillInfoParser::Instance()->GetStyleInfo(pStream[pos].Part.wCode) ||
				SkillInfoParser::Instance()->GetSkillInfo(pStream[pos].Part.wCode) )
			{
				SkillContainer.InsertSlot(pos, pStream[pos]);
			}
		}
	}

	SKILL_PRESET skillPreset;
	std::copy(m_SkillPreset[charClassType].begin(), m_SkillPreset[charClassType].end(), std::inserter(skillPreset, skillPreset.begin()) );

	SKILL_PRESET basicSkillPreset;
	std::copy(m_BasicSkillPreset[charClassType].begin(), m_BasicSkillPreset[charClassType].end(), std::inserter(basicSkillPreset, basicSkillPreset.begin()) );
	
	ROOT_SKILLINFO* pInfo = NULL;
	SkillScriptInfo* pSkillInfo = NULL;
	BASE_STYLEINFO* pStyleInfo = NULL;

	//////////////////////////////////////////
	int nSkillContainer = SkillContainer.GetMaxSlotNum();

	for(POSTYPE pos = 0; pos < SkillContainer.GetMaxSlotNum(); pos++)
	{
		if(SkillContainer.IsEmpty(pos) )
			continue;

		//슬롯의 코드를 얻어와서 파서로부터 해당 정보를 얻어온다.
		SCSkillSlot& slot = (SCSkillSlot&)SkillContainer.GetSlot(pos);
		pInfo = SkillInfoParser::Instance()->GetInfo(slot.GetCode());				

		if(pInfo->IsSkill())	//스킬일경우
		{
			pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(slot.GetCode());
			if(pSkillInfo == NULL)	
				continue;

			SLOTCODE wBaseSkillCode = 0;
			if(0 < pSkillInfo->m_wSkillLv)
			{
				wBaseSkillCode = slot.GetCode() - pSkillInfo->m_wSkillLv;
				SKILL_PRESET::iterator iter = skillPreset.find(wBaseSkillCode);
				if(iter !=  skillPreset.end() )
				{
					iter->second.wLevel = pSkillInfo->m_wSkillLv;
					iter->second.isActive = 1;
				}
			}
			else
			{
				wBaseSkillCode = slot.GetCode();
				SKILL_PRESET::iterator iter = basicSkillPreset.find(wBaseSkillCode);
				if(iter !=  basicSkillPreset.end() )
				{
				//	iter->second.wLevel = pSkillInfo->m_wSkillLv;
					iter->second.isActive = 1;
				}
			}
		}
		else if(pInfo->IsStyle() )	//스타일일경우
		{
			pStyleInfo = SkillInfoParser::Instance()->GetStyleInfo(slot.GetCode());
			if(pStyleInfo == NULL)	{
				_LOG("Error Style Skill");
				continue;
			}

			SLOTCODE wBaseSkillCode = 0;
			if(0 < pStyleInfo->m_wStyleLV)
			{
				wBaseSkillCode = slot.GetCode() - pStyleInfo->m_wStyleLV;
				SKILL_PRESET::iterator iter = skillPreset.find(wBaseSkillCode);
				if(iter !=  skillPreset.end() )
				{
					iter->second.wLevel = pStyleInfo->m_wStyleLV;
					iter->second.isActive = 1;
				}
			}
			else
			{
				wBaseSkillCode = slot.GetCode();
				SKILL_PRESET::iterator iter = basicSkillPreset.find(wBaseSkillCode);
				if(iter !=  basicSkillPreset.end() )
				{					
					iter->second.isActive = 1;
				}
			}
			
		}
	}
        
	pInfo = NULL;
	pSkillInfo = NULL;
	pStyleInfo = NULL;

	xml.AddElem("SkillList");				//Skill XML시작
	xml.AddChildElem("SkillInfo");			//SkillInfo의 시작
	xml.IntoElem();

	int nSkillPreset = skillPreset.size();

	for(SKILL_PRESET::iterator iter1  = skillPreset.begin(); iter1 !=  skillPreset.end(); iter1++)
	{
		SKILL_INFO&  _SKILL = iter1->second;
		SLOTCODE wSkillCode = iter1->first;
		if(_SKILL.wLevel > 0 )
			wSkillCode += _SKILL.wLevel;

		//1281 65

		
		//슬롯의 코드를 얻어와서 파서로부터 해당 정보를 얻어온다.
		pInfo = SkillInfoParser::Instance()->GetInfo( wSkillCode );
		if(!pInfo)
			continue;

		
		if(pInfo->IsSkill())	//스킬일경우
		{
			pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( wSkillCode);
			if(pSkillInfo == NULL)	
				continue;

			xml.AddChildElem("SKILL");
			xml.IntoElem();

			//스킬명을 스트링테이블에서 얻어온다.
			TCHAR pName[256];
			GetStringByCode(pSkillInfo->m_dwSkillNcode, pName, 256);			
			if(0 != strcmp("(NULL)", pName) )
				xml.AddChildElem("SkillName", pName);
			else
				xml.AddChildElem("SkillName", pSkillInfo->m_szSkillName);

			//스킬코드
			xml.AddChildElemEx("SkillCode", wSkillCode);

			//스타일이 아니므로 0을 넣는다.
			//xml.AddChildElemEx("Type", _SKILL.byType);
			xml.AddChildElemEx("Type", GetSkillType(_SKILL.bSkillTab1,_SKILL.bSkillTab2));

			//스킬레벨
			xml.AddChildElemEx("Level", _SKILL.isActive?  pSkillInfo->m_wSkillLv : 0);

			CString str = "0";
			for(int n = 1; n <= pSkillInfo->m_wMaxLv; n++)
			{
				char chBuf[8]= {0};
				itoa(n, chBuf, 10);
				str += " ";
				str += chBuf;				
			}
			str.TrimRight();
			xml.AddChildElem("LevelOptions", (LPCTSTR)str);

			xml.OutOfElem();
		}
		else if(pInfo->IsStyle() )	//스타일일경우
		{
			pStyleInfo = SkillInfoParser::Instance()->GetStyleInfo(wSkillCode);
			if(pStyleInfo == NULL)	continue;

			xml.AddChildElem("SKILL");
			xml.IntoElem();

			//마찬가지이고 
			TCHAR pName[256];
			GetStringByCode(pStyleInfo->m_dwStyleNCode, pName, 256);
			if(0 != strcmp("(NULL)", pName) )
				xml.AddChildElem("SkillName", pName);
			else
				xml.AddChildElem("SkillName", pStyleInfo->m_szStyleName);

			xml.AddChildElemEx("SkillCode", wSkillCode );

			//여기만 1이다. 스타일이므로
			//xml.AddChildElemEx("Type", _SKILL.byType);
			xml.AddChildElemEx("Type", GetSkillType(_SKILL.bSkillTab1,_SKILL.bSkillTab2));

			xml.AddChildElemEx("Level", _SKILL.isActive? pStyleInfo->m_wStyleLV : 0);

			CString str = "0";
			for(int n = 1; n <= pStyleInfo->m_wMaxLV; n++)
			{
				char chBuf[8]= {0};
				itoa(n, chBuf, 10);
				str += " ";
				str += chBuf;				
			}
			str.TrimRight();
			xml.AddChildElem("LevelOptions", (LPCTSTR)str);

			xml.OutOfElem();

		}
	}

	int nBasicSkillPreset = basicSkillPreset.size();

	for(SKILL_PRESET::iterator iter2  = basicSkillPreset.begin(); iter2 !=  basicSkillPreset.end(); iter2++)
	{
		SKILL_INFO&  _SKILL = iter2->second;
		SLOTCODE wSkillCode = iter2->first;
		if(_SKILL.wLevel > 0 )
			wSkillCode += _SKILL.wLevel;

		

		//슬롯의 코드를 얻어와서 파서로부터 해당 정보를 얻어온다.
		pInfo = SkillInfoParser::Instance()->GetInfo( wSkillCode );
		if(!pInfo)
			continue;

		if(pInfo->IsSkill())	//스킬일경우
		{
			pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( wSkillCode);
			if(pSkillInfo == NULL)	
				continue;

			xml.AddChildElem("SKILL");
			xml.IntoElem();

			//스킬명을 스트링테이블에서 얻어온다.
			TCHAR pName[256];
			GetStringByCode(pSkillInfo->m_dwSkillNcode, pName, 256);
			if(0 != strcmp("(NULL)", pName) )
				xml.AddChildElem("SkillName", pName);
			else
				xml.AddChildElem("SkillName", pSkillInfo->m_szSkillName);

			//스킬코드
			xml.AddChildElemEx("SkillCode", wSkillCode);

			//스타일이 아니므로 0을 넣는다.
			xml.AddChildElemEx("Type", GetSkillType(_SKILL.bSkillTab1,_SKILL.bSkillTab2));
			//xml.AddChildElemEx("Type", 0);

			//스킬레벨
			xml.AddChildElemEx("Level", _SKILL.isActive?  pSkillInfo->m_wSkillLv : 0);

			CString str = "0";
			for(int n = 1; n <= pSkillInfo->m_wMaxLv; n++)
			{
				char chBuf[8]= {0};
				itoa(n, chBuf, 10);
				str += " ";
				str += chBuf;				
			}
			str.TrimRight();
			xml.AddChildElem("LevelOptions", (LPCTSTR)str);

			xml.OutOfElem();
		}
		else if(pInfo->IsStyle() )	//스타일일경우
		{
			pStyleInfo = SkillInfoParser::Instance()->GetStyleInfo(wSkillCode);
			if(pStyleInfo == NULL)	continue;

			xml.AddChildElem("SKILL");
			xml.IntoElem();

			//마찬가지이고 
			TCHAR pName[256];
			GetStringByCode(pStyleInfo->m_dwStyleNCode, pName, 256);
			if(0 != strcmp("(NULL)", pName) )			
				xml.AddChildElem("SkillName", pName);
			else
				xml.AddChildElem("SkillName", pStyleInfo->m_szStyleName);

			xml.AddChildElemEx("SkillCode", wSkillCode );

			//여기만 1이다. 스타일이므로
			xml.AddChildElemEx("Type", GetSkillType(_SKILL.bSkillTab1,_SKILL.bSkillTab2));
			//xml.AddChildElemEx("Type", 1);

			xml.AddChildElemEx("Level", _SKILL.isActive? pStyleInfo->m_wStyleLV : 0);

			CString str = "0";
			for(int n = 1; n <= pStyleInfo->m_wMaxLV; n++)
			{
				char chBuf[8]= {0};
				itoa(n, chBuf, 10);
				str += " ";
				str += chBuf;				
			}
			str.TrimRight();
			xml.AddChildElem("LevelOptions", (LPCTSTR)str);

			xml.OutOfElem();

		}
	}

	xml.OutOfElem();

	SkillContainer.Release();

	*bstrXml = _bstr_t(xml.GetDoc().c_str() ).copy();

	xml.Close();

	_LOG("<< CCSunOnline::decodeSkill()");
	return S_OK;
}

STDMETHODIMP CCSunOnline::decodeMission(BYTE buff[MAX_MISSIONSTREAM_SIZE], BSTR* bstrXml)
{
    
	_LOG(">> CCSunOnline::decodeMission()");
	if(!isInitialized())
		return S_FALSE;
	USES_CONVERSION;

	try
	{
	
	MissionManager missionMgr;
	//미션매니저에 바이너리 데이터 삽입
	missionMgr.SerializeStream(buff, MAX_MISSIONSTREAM_SIZE, SERIALIZE_STORE);

    CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate |
                       CMarkupArchive::ArchiveModeStoring |
                       CMarkupArchive::ArchiveModeNotZipped);
	
	SetXMLCharSet(xml);

	xml.Open("C:\\WOPSConfig\\SUNGMS\\decodeMission.xml");

	xml.AddElem("Mission");				//미션XML의 시작
	xml.AddChildElem("MissionInfo");		//미션INFO의 시작
	xml.IntoElem();

	for(int i=0;i<MAX_MISSION_NUM;i++)		//0번부터 최대 미션번호까지 루프 
	{

		//i는 미션넘버를 뜻한다. i미션번호에 해당하는 맵정보를 가져온다.
		const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(i);
		if(!pMapInfo)	
		{
			continue;		//해당 미션번호의 맵정보가 없으면 통과.
		}
		
		//스트링테이블에서 맵이름 가져오고.
		TCHAR* pszMapName = STRTBL_PARSER2->GetStringByCode(pMapInfo->dwNCode);
		if(!pszMapName)	
		{
			_LOG("@@@@@@@@@ can't get MapName %d", pMapInfo->dwNCode);
			continue;
		}

		//미션번호 가져온다.
		BYTE byMissionNum = missionMgr.ReservedMissionNum();
		

		xml.AddChildElem("Info");		//정보시작
		xml.IntoElem();
		
		xml.AddChildElemEx("MissionNum", i);								//미션 번호
		xml.AddChildElem("MissionMapName", pszMapName);						//미션 맵 이름
		xml.AddChildElemEx("MissionPoint", missionMgr.GetMissionPoint(i));		//미션포인트
		xml.AddChildElemEx("MissionState", missionMgr.GetMissionState(i));		//미션 클리어 상태

		xml.OutOfElem();
	}

	xml.OutOfElem();
	*bstrXml = _bstr_t(xml.GetDoc().c_str() ).copy();		//리턴될 XML이다.
	xml.Close();

	}
	catch(...)
	{
		_LOG("<< [ERROR] CCSunOnline::decodeMission()");

        enum { kBufSize = 1024 };
		char psz[kBufSize] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		_snprintf(psz, _countof(psz), "C:\\WOPSConfig\\SUNGMS\\decodeMission_error[%04d-%02d-%02d %02d.%02d.%02d.%03d].buff", 
			systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

		_LOG("	-- writing Binary dump: %s", psz);

		WriteToFile( psz, buff, MAX_MISSIONSTREAM_SIZE);
		return S_FALSE;
	}

	_LOG("<< CCSunOnline::decodeMission()");
	
	return S_OK;
}

STDMETHODIMP CCSunOnline::decodeInventoryItem( BYTE buff[MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], int iCurrentVer, BYTE ExtraInventoryTabCount, BSTR bstrDesc, BSTR* bstrXml)
{

	USES_CONVERSION;

	_LOG(">> CCSunOnline::decodeInventoryItem(),  %s", OLE2CT(bstrDesc));

	if(!isInitialized())
		return S_FALSE;
		
	try
	{

		CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate |
			CMarkupArchive::ArchiveModeStoring |
			CMarkupArchive::ArchiveModeNotZipped);

	// 버전 관리-----------------------------------------------------------------------------------------------
	// 임시

		_LOG("DBVersion : %d, ExtraInventoryTabCount: %d, ", iCurrentVer, ExtraInventoryTabCount);
		if(iCurrentVer <= 0 )
		{
			_LOG("<< CCSunOnline::decodeInventoryItem() : cannot handle DBVersion %d", iCurrentVer);
			return S_OK;
		}
		BYTE NewStream[MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)];
		int NewVersion = 0;
		BOOL rt;

#ifdef __NA_0_ITEM_STREAM_STRUCT_SEPARATION
		rt = VersionControl::ItemConverter(SI_INVENTORY, 
			(MAX_INVENTORY_TAB_NUM+ExtraInventoryTabCount) * MAX_INVENTORY_SLOT_PER_TAB, 
			iCurrentVer, buff, 
			NewVersion, NewStream );

#else
		rt = VersionControl::InventoryConverter( 
			MAX_INVENTORY_TAB_NUM+ExtraInventoryTabCount, 
			iCurrentVer, buff, 
			NewVersion, NewStream );
#endif
	
	
	if( ( NewVersion == -1) || (0 == rt ) )
	{
		_LOG("<< CCSunOnline::decodeInventoryItem() : VersionControl::InventoryConverter internal error (rt:%d)", rt);
		return S_OK;
	}
	//*iNewVer = NewVersion;

	BYTE* byteStream = NewStream;
	// 버전 관리-----------------------------------------------------------------------------------------------

	
	ITEMSTREAMEX* pStream = reinterpret_cast<ITEMSTREAMEX*>(byteStream);
	

	//인벤토리 컨테이너를 초기화한다.
	SCItemSlotContainer conInventory;
	conInventory.Init(MAX_INVENTORY_SLOT_NUM, SI_INVENTORY);

	conInventory.SetMaxSlotNum( MAX_INVENTORY_SLOT_PER_TAB*(MAX_INVENTORY_TAB_NUM+ExtraInventoryTabCount) );

#if 0//defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__)
    CSunOnlineItem item_serializer(this);

    const POSTYPE number_of_slots = conInventory.GetMaxSlotNum();
    for(POSTYPE pos = 0; pos < number_of_slots; ++pos) {
        if(conInventory.IsEmpty(pos))
            continue;

        //컨테이너에서 슬롯을 가져온다.
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(conInventory.GetSlot(pos));
        item_serializer.DecodeItem(xml, item_slot, pos);
    }
#else //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
	for( POSTYPE pos = 0 ; pos < conInventory.GetMaxSlotNum() ; pos++ )
	{
	//	if(pStream[pos].Part.wCode != 0)		//시리얼 번호가 0이면 빈슬롯
		if(pStream[pos].Part.IsValid() && ItemInfoParser::Instance()->GetItemInfo(pStream[pos].Part.wCode) )
		{
#ifdef _NA002217_100728_EP2_SKILL_SYSTEM_DB_
            SCItemSlot slot;
            slot.Copy(pStream[pos]);
            conInventory.InsertSlot(pos,slot);
#else
			conInventory.InsertSlot(pos, pStream[pos]);
#endif
		}	
	}
#endif //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

	SetXMLCharSet(xml);

	xml.Open("C:\\WOPSConfig\\SUNGMS\\decodeInventoryItem.xml");

//	WriteToFile("C:\\WOPSConfig\\SUNGMS\\decodeInventoryItem.dat", buff, MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX));
	
	//SCItemSlot slot;
		
	xml.AddElem("Inventory");					//인벤토리 XML시작
	xml.AddChildElem("InventoryItems");			//인벤토리 ITEM
	xml.IntoElem();	

#if 0//defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__)
    CSunOnlineItem item_serializer(this);

    const POSTYPE number_of_slots = conInventory.GetMaxSlotNum();
    for(POSTYPE pos = 0; pos < number_of_slots; ++pos) {
        if(conInventory.IsEmpty(pos))
            continue;

        //컨테이너에서 슬롯을 가져온다.
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(conInventory.GetSlot(pos));
        item_serializer.DecodeItem(xml, item_slot, pos);
    }
#else //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
	for( POSTYPE pos = 0 ; pos < conInventory.GetMaxSlotNum() ; pos++ )
	{
		if( conInventory.IsEmpty(pos) ) 
			continue;		

        

		//컨테이너에서 슬롯을 가져온다.
		SCItemSlot & slot = (SCItemSlot &)conInventory.GetSlot(pos);        
		DecodeItem(slot, xml, pos, iCurrentVer);
		//기본 아이템정보를 가져온다.		
	}
#endif //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

	xml.OutOfElem();
	
	conInventory.ClearAll();
	conInventory.Release();

	*bstrXml = _bstr_t(xml.GetDoc().c_str() ).copy();
	xml.Close();

	}
	catch(...)
	{
		_LOG("<< [ERROR] CCSunOnline::decodeInventoryItem()");

        enum { kBufSize = 1024 };
		char psz[kBufSize] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		_snprintf(psz, _countof(psz), "C:\\WOPSConfig\\SUNGMS\\decodeInventoryItem_error[%04d-%02d-%02d %02d.%02d.%02d.%03d].buff", 
			systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

		_LOG("	-- writing Binary dump: %s", psz);

		WriteToFile( psz, buff, MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX) );

		return S_FALSE;
	}
	
	_LOG("<< CCSunOnline::decodeInventoryItem()");

	return S_OK;
}

STDMETHODIMP CCSunOnline::decodeTempInventoryItem( BYTE buff[MAX_TEMPINVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], int iCurrentVer, BSTR* bstrXml)
{	

	_LOG(">> CCSunOnline::decodeTempInventoryItem()");
	if(!isInitialized())
		return S_FALSE;
	USES_CONVERSION;

	try
	{
	
	// 버전 관리-----------------------------------------------------------------------------------------------
	if(iCurrentVer <= 0 )
	{
		_LOG("<< CCSunOnline::decodeTempInventoryItem() : cannot handle DBVersion %d", iCurrentVer);
		return S_OK;
	}

	BYTE NewStream[MAX_TEMPINVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)];
	int NewVersion = 0;
	BOOL rt;

#ifdef __NA_0_ITEM_STREAM_STRUCT_SEPARATION
	rt = VersionControl::ItemConverter(SI_TEMPINVENTORY,   MAX_TEMPINVENTORY_SLOT_NUM,
		iCurrentVer, buff, 
		NewVersion, NewStream );
#else
	rt = VersionControl::CommonConverter(  MAX_TEMPINVENTORY_SLOT_NUM,
		iCurrentVer, buff, 
		NewVersion, NewStream );
#endif


	if( ( NewVersion == -1) || (0 == rt ) )
	{
		_LOG("<< CCSunOnline::decodeTempInventoryItem() : VersionControl::CommonConverter internal error (rt:%d)", rt);
		return S_OK;
	}
	// 버전 관리-----------------------------------------------------------------------------------------------
	//*iNewVer = NewVersion;

	BYTE* byteStream = NewStream;

	ITEMSTREAMEX* pStream = reinterpret_cast<ITEMSTREAMEX*>(byteStream);

	//임시인벤토리 컨테이너 초기화
	SCItemSlotContainer conInventory;
	conInventory.Init(MAX_TEMPINVENTORY_SLOT_NUM, SI_TEMPINVENTORY);

#if 0//defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__)
    CSunOnlineItem item_serializer(this);

    const POSTYPE number_of_slots = conInventory.GetMaxSlotNum();
    for(POSTYPE pos = 0; pos < number_of_slots; ++pos) {
        if(conInventory.IsEmpty(pos))
            continue;

        //컨테이너에서 슬롯을 가져온다.
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(conInventory.GetSlot(pos));
        item_serializer.DecodeItem(xml, item_slot, pos);
    }
#else //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
	for( POSTYPE pos = 0 ; pos < conInventory.GetMaxSlotNum() ; pos++ )
	{
	//	if(pStream[pos].Part.wCode != 0)		//dwSerial이 0이면 빈 슬롯
		if(pStream[pos].Part.IsValid() &&
			ItemInfoParser::Instance()->GetItemInfo(pStream[pos].Part.wCode) )
        {
#ifdef _NA002217_100728_EP2_SKILL_SYSTEM_DB_
            SCItemSlot slot;
            slot.Copy(pStream[pos]);
            conInventory.InsertSlot(pos,slot);
#else
            conInventory.InsertSlot(pos, pStream[pos]);
#endif
        }
		else
        {
			//WriteToFile("C:\\WOPSConfig\\decodeTempInventoryItem_error.buff", buff, MAX_TEMPINVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX) );
		}
	}
#endif //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

	CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate |
		CMarkupArchive::ArchiveModeStoring |
		CMarkupArchive::ArchiveModeNotZipped);
	
	SetXMLCharSet(xml);

	xml.Open("C:\\WOPSConfig\\SUNGMS\\decodeTempInventoryItem.xml");


	//SCItemSlot slot;	

	xml.AddElem("TmpInventory");
	xml.AddChildElem("TmpInventoryItems");
	xml.IntoElem();

	for( POSTYPE pos = 0 ; pos < conInventory.GetMaxSlotNum() ; pos++ )
	{
		if( conInventory.IsEmpty(pos) ) continue;		

		//슬롯을 가져온다.
		SCItemSlot & slot = (SCItemSlot &)conInventory.GetSlot(pos);

		DecodeItem(slot, xml, pos, iCurrentVer);
	
	}

	xml.OutOfElem();

	conInventory.Release();
	*bstrXml = _bstr_t(xml.GetDoc().c_str() ).copy();
	xml.Close();
	}
	catch(...)
	{
		_LOG("<< [ERROR] CCSunOnline::decodeTempInventoryItem()");

		char psz[1024] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		sprintf(psz, "C:\\WOPSConfig\\SUNGMS\\decodeTempInventoryItem_error[%04d-%02d-%02d %02d.%02d.%02d.%03d].buff", 
			systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

		_LOG("	-- writing Binary dump: %s", psz);

		WriteToFile( psz, buff, MAX_TEMPINVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX) );

		return S_FALSE;
	}

	_LOG("<< CCSunOnline::decodeTempInventoryItem()");

	return S_OK;

}

STDMETHODIMP CCSunOnline::decodeEquipItem( BYTE buff[MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX)], int iCurrentVer, BSTR* bstrXml)
{

	_LOG(">> CCSunOnline::decodeEquipItem()");
	if(!isInitialized())
		return S_FALSE;
	USES_CONVERSION;

	try
	{
	
	// 버전 관리-----------------------------------------------------------------------------------------------
	// 임시
	if(iCurrentVer <= 0 )
	{
		_LOG("<< CCSunOnline::decodeEquipItem() : cannot handle DBVersion %d", iCurrentVer);
		return S_OK;
	}

	BYTE NewStream[MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX)];
	int NewVersion = 0;
	BOOL rt;

#ifdef __NA_0_ITEM_STREAM_STRUCT_SEPARATION
	rt = VersionControl::ItemConverter( SI_EQUIPMENT,MAX_EQUIPMENT_SLOT_NUM,
		iCurrentVer, buff, 
		NewVersion, NewStream );
#else
	rt = VersionControl::EquipConverter( 
		iCurrentVer, buff, 
		NewVersion, NewStream );
#endif

	if( ( NewVersion == -1) || (0 == rt ) )
	{
		_LOG("<< CCSunOnline::decodeEquipItem() : VersionControl::EquipConverter internal error (rt:%d)", rt);
		return S_OK;
	}


	//*iNewVer = NewVersion;
	// 버전 관리-----------------------------------------------------------------------------------------------
	BYTE* byteStream = NewStream;

	ITEMSTREAMEX* pStream = reinterpret_cast<ITEMSTREAMEX*>(byteStream);

	//컨테이너 초기화.
	SCItemSlotContainer conInventory;
	conInventory.Init(MAX_EQUIPMENT_SLOT_NUM, SI_EQUIPMENT);
	for( POSTYPE pos = 0 ; pos < conInventory.GetMaxSlotNum() ; pos++ )
	{
	//	if(pStream[pos].Part.wCode != 0)
		if(pStream[pos].Part.IsValid() && ItemInfoParser::Instance()->GetItemInfo(pStream[pos].Part.wCode) )
		{
#ifdef _NA002217_100728_EP2_SKILL_SYSTEM_DB_
            SCItemSlot slot;
            slot.Copy(pStream[pos]);
            conInventory.InsertSlot(pos,slot);
#else
            conInventory.InsertSlot(pos, pStream[pos]);
#endif	
		}
		else
		{
			//WriteToFile("C:\\WOPSConfig\\decodeEquipItem_error.buff", buff, MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX) );
		}
	}


	CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate |
		CMarkupArchive::ArchiveModeStoring |
		CMarkupArchive::ArchiveModeNotZipped);
	
	SetXMLCharSet(xml);

	xml.Open("C:\\WOPSConfig\\SUNGMS\\decodeEquipItem.xml");


	//SCItemSlot slot;
	

	xml.AddElem("Equipment");
	xml.AddChildElem("EquipmentItems");
	xml.IntoElem();

#if 0//defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__)
    CSunOnlineItem item_serializer(this);

    const POSTYPE number_of_slots = conInventory.GetMaxSlotNum();
    for(POSTYPE pos = 0; pos < number_of_slots; ++pos) {
        if(conInventory.IsEmpty(pos))
            continue;

        //컨테이너에서 슬롯을 가져온다.
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(conInventory.GetSlot(pos));
        item_serializer.DecodeItem(xml, item_slot, pos);
    }
#else //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
	for( POSTYPE pos = 0 ; pos < conInventory.GetMaxSlotNum() ; pos++ )
	{
		if( conInventory.IsEmpty(pos) ) continue;

		//슬롯을 가져온다.
		SCItemSlot & slot = (SCItemSlot &)conInventory.GetSlot(pos);

		DecodeItem(slot, xml, pos, iCurrentVer);
	}
#endif //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

	xml.OutOfElem();
	conInventory.Release();
	*bstrXml = _bstr_t(xml.GetDoc().c_str() ).copy();
	xml.Close();

	}
	catch(...)
	{
		_LOG("<< [ERROR] CCSunOnline::decodeEquipItem()");

		char psz[1024] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		sprintf(psz, "C:\\WOPSConfig\\SUNGMS\\decodeEquipItem_error[%04d-%02d-%02d %02d.%02d.%02d.%03d].buff", 
			systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

		_LOG("	-- writing Binary dump: %s", psz);

		WriteToFile( psz, buff, MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX) );

		return S_FALSE;
	}


	_LOG("<< CCSunOnline::decodeEquipItem()");
	return S_OK;
}

STDMETHODIMP CCSunOnline::decodeWareHouse(BYTE* buff, int nBuffSize, int nSoltSize, int nContainSize, int iCurrentVer, BSTR bstrDesc, BSTR* bstrXml, const char* szMethodName, const char* szTagName)
{

	USES_CONVERSION;

	_LOG(">> CCSunOnline::%s(), %s", szMethodName, OLE2CT(bstrDesc));

	if(!isInitialized())
		return S_FALSE;

	try
	{

		// 버전 관리-----------------------------------------------------------------------------------------------
		if(iCurrentVer <= 0 )
		{
			_LOG("<< CCSunOnline::%s() : cannot handle DBVersion %d", szMethodName, iCurrentVer);
			return S_OK;
		}

		//BYTE NewStream[MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)];
		BYTE* NewStream = new BYTE[nBuffSize];
		int NewVersion = 0;
		BOOL rt;

#ifdef __NA_0_ITEM_STREAM_STRUCT_SEPARATION
		rt = VersionControl::ItemConverter(nContainSize,  nSoltSize,
			iCurrentVer, buff, 
			NewVersion, NewStream );
#else
		rt = VersionControl::CommonConverter(  nSoltSize,
			iCurrentVer, buff, 
			NewVersion, NewStream );
#endif
		if( ( NewVersion == -1) || (0 == rt ) )
		{
			_LOG("<< CCSunOnline::%s() : VersionControl::CommonConverter internal error (rt:%d)", szMethodName, rt);
			return S_OK;
		}

		// 버전 관리-----------------------------------------------------------------------------------------------
		//*iNewVer = NewVersion;

		BYTE* byteStream = NewStream;


		ITEMSTREAMEX* pStream = reinterpret_cast<ITEMSTREAMEX*>(byteStream);

		SCItemSlotContainer conInventory;
		//conInventory.Init(MAX_WAREHOUSE_SLOT_NUM, SI_WAREHOUSE);
		conInventory.Init(nSoltSize, nContainSize);
		for( POSTYPE pos = 0 ; pos < conInventory.GetMaxSlotNum() ; pos++ )
		{            
			if(pStream[pos].Part.IsValid() )
			{
				if( ItemInfoParser::Instance()->GetItemInfo(pStream[pos].Part.wCode) )
                {
#ifdef _NA002217_100728_EP2_SKILL_SYSTEM_DB_
                    SCItemSlot slot;
                    slot.Copy(pStream[pos]);
                    conInventory.InsertSlot(pos,slot);
#else
                    conInventory.InsertSlot(pos, pStream[pos]);
#endif
                }
				else
				{
			
				}
			}
		}

		delete[] NewStream;

		CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate |
			CMarkupArchive::ArchiveModeStoring |
			CMarkupArchive::ArchiveModeNotZipped);

		SetXMLCharSet(xml);

		TCHAR szXmlFileName[MAX_PATH];
		wsprintf(szXmlFileName, "C:\\WOPSConfig\\SUNGMS\\%s.xml",szMethodName);	

		xml.Open(szXmlFileName);


		//SCItemSlot slot;
		BASE_ITEMINFO * pItemInfo = NULL;
		TCHAR szChildTagName[MAX_PATH];
		wsprintf(szChildTagName,"%sItems",szTagName);

		xml.AddElem(szTagName);
		xml.AddChildElem(szChildTagName);
		xml.IntoElem();

#if 0//defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__)
        CSunOnlineItem item_serializer(this);

        const POSTYPE number_of_slots = conInventory.GetMaxSlotNum();
        for(POSTYPE pos = 0; pos < number_of_slots; ++pos) {
            if(conInventory.IsEmpty(pos))
                continue;

            //컨테이너에서 슬롯을 가져온다.
            SCItemSlot& item_slot = static_cast<SCItemSlot&>(conInventory.GetSlot(pos));
            item_serializer.DecodeItem(xml, item_slot, pos);
        }
#else //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
		for( POSTYPE pos = 0 ; pos < conInventory.GetMaxSlotNum() ; pos++ )
		{
			if( conInventory.IsEmpty(pos) ) continue;

			
			//슬롯을 가져온다.
			SCItemSlot & slot = (SCItemSlot &)conInventory.GetSlot(pos);

			DecodeItem(slot, xml, pos, iCurrentVer);
		}
#endif //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

		xml.OutOfElem();
		conInventory.Release();

		*bstrXml = _bstr_t(xml.GetDoc().c_str() ).copy();
		xml.Close();
		

	}
	catch(...)
	{
		_LOG("<< [ERROR] CCSunOnline::%s()",szMethodName);

		char psz[1024] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		sprintf(psz, "C:\\WOPSConfig\\SUNGMS\\%s_error[%04d-%02d-%02d %02d.%02d.%02d.%03d].buff", 
			"decodeWarehouseItem", systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

		_LOG("	-- writing Binary dump: %s", psz);

		WriteToFile( psz, buff, nBuffSize);

		return S_FALSE;
	}


	_LOG("<< CCSunOnline::%s()",szMethodName);
	return S_OK;

}


STDMETHODIMP CCSunOnline::decodeWarehouseItem( BYTE buff[MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], int iCurrentVer, BSTR bstrDesc, BSTR* bstrXml)
{	
	return decodeWareHouse(buff, MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX), MAX_WAREHOUSE_SLOT_NUM,SI_WAREHOUSE, iCurrentVer, bstrDesc, bstrXml, "decodeWarehouseItem", "Warehouse");
}

STDMETHODIMP CCSunOnline::encodeSkill(BSTR bstrXml, BYTE buff[MAX_SKILL_SLOT_NUM*2], BYTE charClassType, int* nResult)
{
	_LOG(">> CCSunOnline::encodeSkill(),  charClassType %d", charClassType);
	if(!isInitialized())
		return S_FALSE;

	if( (charClassType > eCHAR_TYPE_MAX ) || 
		(charClassType <= 0 ) )
	{
		_LOG("@Error - invalid charClassType %d", charClassType);
		return S_FALSE;
	}

	USES_CONVERSION;
//	WriteToFile("C:\\WOPSConfig\\SUNGMS\\encodeSkill.xml", (BYTE*)OLE2CT(bstrXml), strlen(OLE2CT(bstrXml)));

	std::string strBstrTmp;
	ConvertBSTRToString(bstrXml,strBstrTmp);	

	*nResult = INVALID_XML_FORMAT;

	try
	{

		CMarkupArchive xml(CMarkupArchive::ArchiveModeNotZipped | CMarkupArchive::ArchiveModeString);
		xml.OpenString(strBstrTmp.c_str());


		CHECK(xml.FindElem("SkillList"),"SkillList");		//SkillList를 XML스트링에서 찾는다. 없으면 종료
		CHECK(xml.FindChildElem("SkillInfo"),"SkillInfo");		//SKillInfo도 찾는다.
		xml.IntoElem();

		SKILLSTREAM stBin[MAX_SKILL_SLOT_NUM*2];
		ZeroMemory(stBin, MAX_SKILL_SLOT_NUM*2);

		SCSkillSlotContainer container;
		container.Init(MAX_SKILL_SLOT_NUM, SI_SKILL);		//컨테이너를 세팅하고.


		ROOT_SKILLINFO* pInfo = NULL;
		SkillScriptInfo* pSkillInfo = NULL;
		BASE_STYLEINFO* pStyleInfo = NULL;

		for(POSTYPE pos = 0;pos<MAX_SKILL_SLOT_NUM;pos++)
		{
			if(!xml.FindChildElem(_T("SKILL")))			//스킬항목을 찾는다.
				break;

			xml.IntoElem();

			SCSkillSlot slot;

			string sTemp, strName;
			CHECK(xml.FindGetChildData("SkillName", strName),"SkillName");			//스킬명을 찾고


			CHECK(xml.FindGetChildData("SkillCode", sTemp),"SkillCode");			//스킬코드를 찾는다.

			SLOTCODE code = (SLOTCODE)_ttoi(sTemp.c_str());

			int nType = FALSE;
			CHECK(xml.FindGetChildData("Type", nType),"Type");				//스타일여부 읽어온다.

			int iLevel = 0;
			CHECK(xml.FindGetChildData("Level", iLevel), "Level");				//레벨을 읽어온다.
//          edited 2007.06.29

			/*
			0: 숙련도1
			1: 숙련도2
			2: 패시브
			3: 스타일
			*/

			int iMaxLevel = 0;
			int iLevelDiff = 0;
			if(3 == nType)		//스타일이면...
			{
				pStyleInfo = SkillInfoParser::Instance()->GetStyleInfo(code);
				if(!pStyleInfo)
				{
					_LOG("[Type %d] can't get StyleInfo for %d", nType, code);
					return S_FALSE;
				}

				if(0 == iLevel && pStyleInfo->m_wMaxLV != 0)
				{
					xml.OutOfElem();
					continue;
				}
				
				iMaxLevel = pStyleInfo->m_wMaxLV;
				iLevelDiff = iLevel - pStyleInfo->m_wStyleLV;		

				//	CHECK(xml.FindGetChildData("Level", iLevel));				//레벨을 읽어온다.
			}		
			else
			{
				if(0 == iLevel)
				{
					xml.OutOfElem();
					continue;
				}
				pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(code);
				if(!pSkillInfo)
				{
					_LOG("[Type %d] can't get SkillInfo for %d", nType, code);
					return S_FALSE;
				}
				iMaxLevel = pSkillInfo->m_wMaxLv;
				iLevelDiff = iLevel - pSkillInfo->m_wSkillLv;			
			}
//          edited 2007.06.29
			
			if( (0 == iLevel) && 
				(0 < iMaxLevel ) )	//Default 스킬		
			{
				xml.OutOfElem();
				continue;
			}			

			

			code += iLevelDiff;
			slot.SetCode(code);											//스킬코드값을 넣는다.

			slot.CopyOut(stBin[pos]);			//슬롯에 저장된 값을 컨테이너로 복사한다.

			xml.OutOfElem();
		}

		//컨테이너에서 버퍼로 복사한다.
		memcpy(buff, stBin, 2*MAX_SKILL_SLOT_NUM);

		xml.OutOfElem();
		xml.Close();

		*nResult = ENCODE_SUCCESS;

		_LOG("<< CCSunOnline::encodeSkill()");
		return S_OK;

	}
	catch(...)
	{
		*nResult = INVALID_XML_FORMAT;
		_LOG("<< [ERROR] CCSunOnline::encodeSkill()");

		char psz[1024] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		sprintf(psz, "C:\\WOPSConfig\\SUNGMS\\encodeSkill_dump[%04d-%02d-%02d %02d.%02d.%02d.%03d].xml", 
			systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
		_LOG("	-- writing XML dump: %s", psz);

		WriteToFile(psz, (BYTE*)strBstrTmp.c_str(), strBstrTmp.size() ) ;
		return S_FALSE;
	}
}

STDMETHODIMP CCSunOnline::encodeMission(BSTR bstrXml, BYTE buff[MAX_MISSIONSTREAM_SIZE], int* nResult)
{
    
	_LOG(">> CCSunOnline::encodeMission()");
	if(!isInitialized())
		return S_FALSE;
	USES_CONVERSION;

	*nResult = INVALID_XML_FORMAT;
	std::string strBstrTmp;
	ConvertBSTRToString(bstrXml,strBstrTmp);	
	
	try
	{
		CMarkupArchive xml(CMarkupArchive::ArchiveModeNotZipped | CMarkupArchive::ArchiveModeString);
		xml.OpenString(strBstrTmp.c_str());


		CHECK(xml.FindElem("Mission"), "Mission");			//Mission을 XML스트링에서 찾는다.

		CHECK(xml.FindChildElem("MissionInfo"), "MissionInfo");
		xml.IntoElem();

		MissionManager missionMgr;

		for(int k=0;k<MAX_MISSION_NUM;k++)
		{
			missionMgr.SetMissionPoint(k, 0);					//Mission매니저의 내부 값을 초기화한다.
			missionMgr.SetMissionState(k, (eMISSION_STATE)0);
		}
		

		for(int i=0;i<MAX_MISSION_NUM;i++)
		{
			if(!xml.FindChildElem("Info"))
				continue;

			xml.IntoElem();
			
			DWORD dwMission = 0;
			DWORD dwTemp = 0;
			xml.FindGetChildData("MissionNum", dwMission);			//미션번호를 찾는다.
			xml.FindGetChildData("MissionPoint", dwTemp);			//미션 포인터를 찾는다.
			missionMgr.SetMissionPoint((BYTE)dwMission, dwTemp);		//미션 포인트 셋팅

			xml.FindGetChildData("MissionState", dwTemp);			//미션클리어 상태를 찾는다.
			missionMgr.SetMissionState((BYTE)dwMission, (eMISSION_STATE)dwTemp);		//상태 셋팅

			xml.OutOfElem();
		}
		//미션매니저에서 버퍼로 바이너리 데이터 복사
		missionMgr.SerializeStream(buff, MAX_MISSION_NUM*4, SERIALIZE_LOAD);

		xml.OutOfElem();
	}
	catch(...)
	{
		*nResult = INVALID_XML_FORMAT;
		_LOG("<< [ERROR] CCSunOnline::encodeMission()");

		char psz[1024] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		sprintf(psz, "C:\\WOPSConfig\\SUNGMS\\encodeMission_dump[%04d-%02d-%02d %02d.%02d.%02d.%03d].xml", 
			systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
		_LOG("	-- writing XML dump: %s", psz);

		WriteToFile(psz, (BYTE*)strBstrTmp.c_str(), strBstrTmp.size() );
		return S_FALSE;
	}

	*nResult = ENCODE_SUCCESS;
	_LOG("<< CCSunOnline::encodeMission()");
    
	return S_OK;
}

STDMETHODIMP CCSunOnline::encodeInventoryItem(BSTR bstrXml, BYTE buff[MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)], 
					      int* iNewVer, 
					      int* nResult)
{	
	_LOG(">> CCSunOnline::encodeInventoryItem()");
	if(!isInitialized())
		return S_FALSE;

	USES_CONVERSION;

	*nResult = INVALID_XML_FORMAT;
	std::string strBstrTmp;
	ConvertBSTRToString(bstrXml,strBstrTmp);	
	const char* pszXml = strBstrTmp.c_str();


	try
	{
	
	CMarkupArchive xml(CMarkupArchive::ArchiveModeNotZipped | CMarkupArchive::ArchiveModeString);
	xml.OpenString((const char*)pszXml);

	//WriteToFile("C:\\WOPSConfig\\SUNGMS\\encodeInventoryItem.xml", (BYTE*)OLE2CT(bstrXml), strlen(OLE2CT(bstrXml)));

	CHECK(xml.FindElem(_T("Inventory")), "Inventory");				//Inventory를 XML스트링에서 찾는다.
	CHECK(xml.FindChildElem(_T("InventoryItems")), "InventoryItems");
	xml.IntoElem();

	ITEMSTREAMEX stBin[MAX_INVENTORY_SLOT_NUM];
	ZeroMemory(stBin, sizeof(ITEMSTREAMEX)*MAX_INVENTORY_SLOT_NUM);

#if 0//defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__)
    CSunOnlineItem item_serializer(this);

    const POSTYPE number_of_slots = conInventory.GetMaxSlotNum();
    for(POSTYPE pos = 0; pos < number_of_slots; ++pos) {
        if(conInventory.IsEmpty(pos))
            continue;

        //컨테이너에서 슬롯을 가져온다.
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(conInventory.GetSlot(pos));
        item_serializer.DecodeItem(xml, item_slot, pos);
    }
#else //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
	for(POSTYPE pos = 0; pos < MAX_INVENTORY_SLOT_NUM; pos++)
	{

		if(!xml.FindChildElem(_T("ITEM"))) continue;
		
		SCItemSlot slot;

		POSTYPE SlotIdx = 0;

		if(S_OK != EncodeItem(slot,xml, SlotIdx)) return S_FALSE;
		//바이너리값 복사
     
		slot.CopyOut(stBin[SlotIdx]);
        		
	}
#endif //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

	xml.OutOfElem();


	xml.Close();
	
	ZeroMemory(buff, MAX_INVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX));
	memcpy(buff, stBin, sizeof(ITEMSTREAMEX)*MAX_INVENTORY_SLOT_NUM);

	}
	catch(...)
	{
		*nResult = INVALID_XML_FORMAT;
		_LOG("<< [ERROR] CCSunOnline::encodeInventoryItem()");

		char psz[1024] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		sprintf(psz, "C:\\WOPSConfig\\SUNGMS\\encodeInventoryItem_dump[%04d-%02d-%02d %02d.%02d.%02d.%03d].xml", 
			systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
		_LOG("	-- writing XML dump: %s", psz);

		WriteToFile(psz, (BYTE*)pszXml, strlen(pszXml ) );
		return S_FALSE;
	}

	*iNewVer = 2;
	*nResult = ENCODE_SUCCESS;
	_LOG("<<  CCSunOnline::encodeInventoryItem()");
	return S_OK;
}

STDMETHODIMP CCSunOnline::encodeTempInventoryItem(BSTR bstrXml, BYTE buff[MAX_TEMPINVENTORY_SLOT_NUM*sizeof(ITEMSTREAMEX)],
						  int* iNewVer, 
						  int* nResult)
{
	_LOG(">> CCSunOnline::encodeTempInventoryItem()");
	if(!isInitialized())
		return S_FALSE;

	USES_CONVERSION;

	*nResult = INVALID_XML_FORMAT;
	std::string strBstrTmp;
	ConvertBSTRToString(bstrXml,strBstrTmp);	
	const char* pszXml = strBstrTmp.c_str();


//	char* pszOut = ConvertBSTRToString(bstrXml);
//	static char pszXml[1024*100] = {0};
//	strcpy(pszXml, pszOut);
//	delete [] pszOut;

	try
	{

	CMarkupArchive xml(CMarkupArchive::ArchiveModeNotZipped | CMarkupArchive::ArchiveModeString);
	xml.OpenString((const char*)pszXml);

	CHECK(xml.FindElem(_T("TmpInventory")), "TmpInventory");
	CHECK(xml.FindChildElem(_T("TmpInventoryItems")), "TmpInventoryItems");
	xml.IntoElem();

	ITEMSTREAMEX stBin[MAX_TEMPINVENTORY_SLOT_NUM];
	ZeroMemory(stBin, sizeof(ITEMSTREAMEX)*MAX_TEMPINVENTORY_SLOT_NUM);

	//임시인벤토리용 컨테이너 초기화
	SCItemSlotContainer container;
	container.Init(MAX_TEMPINVENTORY_SLOT_NUM, SI_TEMPINVENTORY);


#if 0//defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__)
    CSunOnlineItem item_serializer(this);

    const POSTYPE number_of_slots = conInventory.GetMaxSlotNum();
    for(POSTYPE pos = 0; pos < number_of_slots; ++pos) {
        if(conInventory.IsEmpty(pos))
            continue;

        //컨테이너에서 슬롯을 가져온다.
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(conInventory.GetSlot(pos));
        item_serializer.DecodeItem(xml, item_slot, pos);
    }
#else //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
	for(POSTYPE pos = 0; pos < MAX_TEMPINVENTORY_SLOT_NUM; pos++)
	{

		if(!xml.FindChildElem(_T("ITEM"))) continue;

		SCItemSlot slot;

		POSTYPE SlotIdx = 0;

		if(S_OK != EncodeItem(slot,xml, SlotIdx)) return S_FALSE;
		//바이너리값 복사
		slot.CopyOut(stBin[SlotIdx]);		
	}
#endif //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

	xml.OutOfElem();


	xml.Close();

	memcpy(buff, stBin, sizeof(ITEMSTREAMEX)*MAX_TEMPINVENTORY_SLOT_NUM);

	container.Release();

	}
	catch(...)
	{

		*nResult = INVALID_XML_FORMAT;
		_LOG("<< [ERROR] CCSunOnline::encodeTempInventoryItem()");

		char psz[1024] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		sprintf(psz, "C:\\WOPSConfig\\SUNGMS\\encodeTempInventoryItem_dump[%04d-%02d-%02d %02d.%02d.%02d.%03d].xml", 
			systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
		_LOG("	-- writing XML dump: %s", psz);

		WriteToFile(psz, (BYTE*)pszXml, strlen(pszXml ) );
		return S_FALSE;
	}

	*iNewVer = 2;

	*nResult = ENCODE_SUCCESS;
	_LOG("<< CCSunOnline::encodeTempInventoryItem()");
	return S_OK;
}


STDMETHODIMP CCSunOnline::encodeEquipItem(BSTR bstrXml, BYTE buff[MAX_EQUIPMENT_SLOT_NUM*sizeof(ITEMSTREAMEX)], 
					  int* iNewVer, 
					  int* nResult)
{
	
	_LOG(">> CCSunOnline::encodeEquipItem()");
	if(!isInitialized())
		return S_FALSE;

	USES_CONVERSION;

	*nResult = INVALID_XML_FORMAT;
	std::string strBstrTmp;
	ConvertBSTRToString(bstrXml,strBstrTmp);	
	const char* pszXml = strBstrTmp.c_str();	

	try
	{
	CMarkupArchive xml(CMarkupArchive::ArchiveModeNotZipped | CMarkupArchive::ArchiveModeString);
	xml.OpenString((const char*)pszXml);

	CHECK(xml.FindElem(_T("Equipment")), "Equipment");
	CHECK(xml.FindChildElem(_T("EquipmentItems")), "EquipmentItems");
	xml.IntoElem();

	ITEMSTREAMEX stBin[MAX_EQUIPMENT_SLOT_NUM];
	ZeroMemory(stBin, sizeof(ITEMSTREAMEX)*MAX_EQUIPMENT_SLOT_NUM);

	//장착창용 컨테이너 초기화
	SCItemSlotContainer container;
	container.Init(MAX_EQUIPMENT_SLOT_NUM, SI_EQUIPMENT);

#if 0//defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__)
    CSunOnlineItem item_serializer(this);

    const POSTYPE number_of_slots = conInventory.GetMaxSlotNum();
    for(POSTYPE pos = 0; pos < number_of_slots; ++pos) {
        if(conInventory.IsEmpty(pos))
            continue;

        //컨테이너에서 슬롯을 가져온다.
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(conInventory.GetSlot(pos));
        item_serializer.DecodeItem(xml, item_slot, pos);
    }
#else //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
	for(POSTYPE pos = 0; pos < MAX_EQUIPMENT_SLOT_NUM; pos++)
	{
		if(!xml.FindChildElem(_T("ITEM"))) continue;

		SCItemSlot slot;

		POSTYPE SlotIdx = 0;

		if(S_OK != EncodeItem(slot,xml, SlotIdx)) return S_FALSE;
		//바이너리값 복사
		slot.CopyOut(stBin[SlotIdx]);
	}
#endif //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

	xml.OutOfElem();


	xml.Close();

	memcpy(buff, stBin, sizeof(ITEMSTREAMEX)*MAX_EQUIPMENT_SLOT_NUM);

	container.Release();
	}
	catch(...)
	{

		*nResult = INVALID_XML_FORMAT;
		_LOG("<< [ERROR] CCSunOnline::encodeEquipItem()");
		
		char psz[1024] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		sprintf(psz, "C:\\WOPSConfig\\SUNGMS\\encodeEquipItem_dump[%04d-%02d-%02d %02d.%02d.%02d.%03d].xml", 
			systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
		_LOG("	-- writing XML dump: %s", psz);

		WriteToFile(psz, (BYTE*)pszXml, strlen(pszXml ) );
		return S_FALSE;
	}

	*iNewVer = 2;
	*nResult = ENCODE_SUCCESS;
	_LOG("<< CCSunOnline::encodeEquipItem()");
	return S_OK;
}

STDMETHODIMP CCSunOnline::encodeWareHouse(BSTR bstrXml, BYTE* buff, int nBufSize, int nSoltSize, int nContainSize, int* iNewVer, int* nResult, const char* szMethodName, const char* szTagName)
{
	_LOG(">> CCSunOnline::%s()", szMethodName);

	if(!isInitialized())
		return S_FALSE;

	USES_CONVERSION;

	*nResult = INVALID_XML_FORMAT;

	
	
	std::string strBstrTmp;
	ConvertBSTRToString(bstrXml,strBstrTmp);	
	const char* pszXml = strBstrTmp.c_str();
		
	try
	{

		CMarkupArchive xml(CMarkupArchive::ArchiveModeNotZipped | CMarkupArchive::ArchiveModeString);
		xml.OpenString((const char*)pszXml);
	
		CHECK(xml.FindElem(szTagName), szTagName);
		strBstrTmp = szTagName;
		strBstrTmp += "Items";
		CHECK(xml.FindChildElem(strBstrTmp.c_str()), strBstrTmp.c_str());
		xml.IntoElem();

		//ITEMSTREAMEX stBin[MAX_WAREHOUSE_SLOT_NUM];
		ITEMSTREAMEX* stBin = new ITEMSTREAMEX[nSoltSize];
		ZeroMemory(stBin, nBufSize);

		//창고용 컨테이너 초기화
		SCItemSlotContainer container;
		container.Init(nSoltSize, nContainSize);


#if 0//defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__)
        CSunOnlineItem item_serializer(this);

        const POSTYPE number_of_slots = conInventory.GetMaxSlotNum();
        for(POSTYPE pos = 0; pos < number_of_slots; ++pos) {
            if(conInventory.IsEmpty(pos))
                continue;

            //컨테이너에서 슬롯을 가져온다.
            SCItemSlot& item_slot = static_cast<SCItemSlot&>(conInventory.GetSlot(pos));
            item_serializer.DecodeItem(xml, item_slot, pos);
        }
#else //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
		for(POSTYPE pos = 0; pos < nSoltSize; pos++)
		{
			if(!xml.FindChildElem(_T("ITEM"))) continue;

			SCItemSlot slot;

			POSTYPE SlotIdx = 0;

			if(S_OK != EncodeItem(slot,xml, SlotIdx)) return S_FALSE;
			//바이너리값 복사
			slot.CopyOut(stBin[SlotIdx]);

		}
#endif //!__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

		xml.OutOfElem();


		xml.Close();

		memcpy(buff, stBin, nBufSize);

		delete[] stBin;

		container.Release();
	}
	catch(...)
	{
		*nResult = INVALID_XML_FORMAT;
		_LOG("<< [ERROR] CCSunOnline::%s()",szMethodName);

		char psz[1024] = {0};
		SYSTEMTIME systemTime;
		GetLocalTime(&systemTime);

		sprintf(psz, "C:\\WOPSConfig\\SUNGMS\\%s_dump[%04d-%02d-%02d %02d.%02d.%02d.%03d].xml", 
			szMethodName, systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
		_LOG("	-- writing XML dump: %s", psz);

		WriteToFile(psz, (BYTE*)pszXml, strlen(pszXml ) );
		return S_FALSE;
	}

	*iNewVer = 2;
	*nResult = ENCODE_SUCCESS;
	_LOG("<< CCSunOnline::%s()", szMethodName);
	return S_OK;
}

STDMETHODIMP CCSunOnline::encodeWarehouseItem(BSTR bstrXml, BYTE buff[MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], 
					      int* iNewVer, 
					      int* nResult)
{
	return encodeWareHouse(bstrXml, buff, MAX_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX), MAX_WAREHOUSE_SLOT_NUM, SI_WAREHOUSE, iNewVer, nResult, "encodeWarehouseItem", "Warehouse");	
}

eRANK CCSunOnline::ConvertRankType(string sRank)
{
	eRANK RankType;
	if(_tcscmp(sRank.c_str(), "RANK_F") == 0)
	{
		RankType = RANK_F;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_E") == 0)
	{
		RankType = RANK_E;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_D") == 0)
	{
		RankType = RANK_D;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_C") == 0)
	{
		RankType = RANK_C;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_B") == 0)
	{
		RankType = RANK_B;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_A") == 0)
	{
		RankType = RANK_A;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_MA") == 0)
	{
		RankType = RANK_MA;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_PA") == 0)
	{
		RankType = RANK_PA;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_S") == 0)
	{
		RankType = RANK_S;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_PS") == 0)
	{
		RankType = RANK_PS;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_MS") == 0)
	{
		RankType = RANK_MS;
	}
	else if(_tcscmp(sRank.c_str(), "RANK_MAX") == 0)
	{
		RankType = RANK_MAX;
	}
	return RankType;
}

string CCSunOnline::ConvertRankType(eRANK rank)
{
	switch(rank)
	{
	case RANK_F:
		return "RANK_F";
	case RANK_E:
		return"RANK_E";
	case RANK_D:
		return "RANK_D";
	case RANK_C:
		return "RANK_C";
	case RANK_B:
		return "RANK_B";
	case RANK_MA:
		return "RANK_MA";
	case RANK_A:
		return "RANK_A";
	case RANK_PA:
		return "RANK_PA";
	case RANK_MS:
		return "RANK_MS";
	case RANK_S:
		return "RANK_S";
	case RANK_PS:
		return "RANK_PS";
	case RANK_MAX:
		return "RANK_MAX";
	}	

	return "UnKnow RANK";
}

#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM

BOOL CCSunOnline::LoadParser( const char *pszFileName, eSCRIPT_CODE eCode, IParser* pParser, BOOL bBigData) const
{
	return pParser->Load( eCode, pszFileName, bBigData, SEPERATOR_WHITESPACE, NULL, FALSE );
}

#else	// __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM

BOOL CCSunOnline::LoadParser(const char *pszFileName, eSCRIPT_CODE eCode, IParser* pParser) const
{
	util::SolarFileReader sr;
	if( !sr.OpenFile( pszFileName, SEPERATOR_WHITESPACE ) )
		return FALSE;	

	pParser->_LoadEx( eCode, sr, FALSE );

	sr.CloseFile();

	return TRUE;
}

#endif	// __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM



string CCSunOnline::ConvertSocketType(eSOCKET sock)
{
	switch(sock)
	{
	case SOCKET_1:
		return "SOCKET_1";
	case SOCKET_2:
		return "SOCKET_2";
	case SOCKET_3:
		return "SOCKET_3";
#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    case SOCKET_4:
        return "SOCKET_4";
    case SOCKET_5:
        return "SOCKET_5";
#endif		
	}
	return "SOCKET_MAX";
}
eSOCKET CCSunOnline::ConvertSocketType(string sSock)
{
	eSOCKET SockType;
	if(_tcscmp(sSock.c_str(), "SOCKET_1") == 0)
	{
		SockType = SOCKET_1;
	}
	else if(_tcscmp(sSock.c_str(), "SOCKET_2") == 0)
	{
		SockType = SOCKET_2;
	}
	else if(_tcscmp(sSock.c_str(), "SOCKET_3") == 0)
	{
		SockType = SOCKET_3;
	}
#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    else if(_tcscmp(sSock.c_str(), "SOCKET_4") == 0)
    {
        SockType = SOCKET_4;
    }
    else if(_tcscmp(sSock.c_str(), "SOCKET_5") == 0)
    {
        SockType = SOCKET_5;
    }
#endif
	
	return SockType;
}


STDMETHODIMP CCSunOnline::init(BSTR bstrLocale, BYTE DBVersion)
{  

	USES_CONVERSION;

	if(m_bInitialized)
	{
		_LOG("<<  (INVALID) CCSunOnline::init(void)");
		return S_OK;
	}

	std::string app_path = "C:\\WOPSConfig\\SUNGMS\\";

	char* pszLocale = (char*)OLE2CT(bstrLocale);
	app_path += pszLocale;
	g_pFileLogger = new CFileLogger(app_path.c_str() );

	if( 0 == strnicmp(pszLocale, "ko", 2))
        wLocaleCode = INTERNATIONAL_KOREA;
	else if(0 == strnicmp(pszLocale, "en", 2) )
		wLocaleCode = INTERNATIONAL_ENGLISH;
	else if(0 == strnicmp(pszLocale, "jp", 2) )
		wLocaleCode = INTERNATIONAL_JAPAN;
	else if(0 == strnicmp(pszLocale, "tw", 2) )
		wLocaleCode = INTERNATIONAL_TAIWAN;
	else if(0 == strnicmp(pszLocale, "cn", 2) )
		wLocaleCode = INTERNATIONAL_CHINA;
    else if(0 == strnicmp(pszLocale, "ru", 2) )
        wLocaleCode = INTERNATIONAL_RUSSIAN;
	else
	{
		_LOG(">> Not defined Langage Info");
	}
	
	_LOG(">> CCSunOnline::init() v 1.0.9.21 (Locale - %s)", pszLocale );

	BOOL bLoad = FALSE;

	/////////////////////////////////////////////////////////////////////////

	for(int n = 0; n < 5; n++)
		g_pStrTblParser[n] = new CStringTableParser;


	//스트링테이블 파서로딩
	bLoad = STRTBL_PARSER1->Init((char*)(app_path + "\\StringTable.txt").c_str());
	if(!bLoad) _LOG("Open fail StringTable.txt");
	
	bLoad = STRTBL_PARSER2->Init((char*)(app_path + "\\Stringtableforname.txt").c_str());
	if(!bLoad) _LOG("Open fail Stringtableforname.txt");

	bLoad = STRTBL_PARSER3->Init((char*)(app_path + "\\StringTableForProgram.txt").c_str());
	if(!bLoad) _LOG("Open fail StringTableForProgram.txt");

	bLoad = STRTBL_PARSER4->Init((char*)(app_path + "\\ItemTypes.txt").c_str());
	if(!bLoad) _LOG("Open fail ItemTypes.txt");

	bLoad = STRTBL_PARSER5->Init((char*)(app_path + "\\StringTable_GMS.txt").c_str());
	if(!bLoad) _LOG("Open fail StringTable_GMS.txt");

	_LOG("STRTBL_PARSER OK");	

#if NATION_BUILD_VERSION >= 904
	TableData::CreateEnvironment();
#endif

	////////////////////////////////////
	//Item인포 스크립트 로딩
	ItemInfoParser::Instance()->Init(1000, "");
	_LOG("ItemInfoParser::Instance()->Init ");

	//bLoad = ItemInfoParser::Instance()->Load( (char*)(app_path + "\\AccessoryItemInfo.txt").c_str() );
	_LOG("AccessoryItemInfo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\AccessoryItemInfo.txt").c_str(), SCRIPT_ACCESSORYITEMINFO, ItemInfoParser::Instance());
	if(bLoad) _LOG("AccessoryItemInfo.txt Load Complete");
	else _LOG("AccessoryItemInfo.txt Load fail");

	//bLoad = ItemInfoParser::Instance()->Load( (char*)(app_path + "\\WeaponItemInfo.txt").c_str() );
	_LOG("WeaponItemInfo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\WeaponItemInfo.txt").c_str(), SCRIPT_WEAPONITEMINFO, ItemInfoParser::Instance());
	//_LOG("ItemInfoParser::Instance()->WeaponItemInfo ");
	if(bLoad) _LOG("WeaponItemInfo.txt Load Complete");
	else _LOG("WeaponItemInfo.txt Load fail");

	//bLoad = ItemInfoParser::Instance()->Load( (char*)(app_path + "\\ArmorItemInfo.txt").c_str() );
	_LOG("ArmorItemInfo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\ArmorItemInfo.txt").c_str(), SCRIPT_ARMORITEMINFO, ItemInfoParser::Instance());
	//_LOG("ItemInfoParser::Instance()->ArmorItemInfo ");
	if(bLoad) _LOG("ArmorItemInfo.txt Load Complete");
	else _LOG("ArmorItemInfo.txt Load fail");

	//bLoad = ItemInfoParser::Instance()->Load( (char*)(app_path + "\\WasteItemInfo.txt").c_str() );
	_LOG("WasteItemInfo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\WasteItemInfo.txt").c_str(), SCRIPT_WASTEITEMINFO, ItemInfoParser::Instance());
	//_LOG("ItemInfoParser::Instance()->WasteItemInfo ");
	if(bLoad) _LOG("WasteItemInfo.txt Load Complete");
	else _LOG("WasteItemInfo.txt Load fail");

	//bLoad = ItemInfoParser::Instance()->Load( (char*)(app_path + "\\ChargeItemInfo.txt").c_str() );
	_LOG("ChargeItemInfo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\ChargeItemInfo.txt").c_str(), SCRIPT_CHARGEITEMINFO, ItemInfoParser::Instance());
	//_LOG("ItemInfoParser::Instance()->ChargeItemInfo ");
	if(bLoad) _LOG("ChargeItemInfo.txt Load Complete");
	else _LOG("ChargeItemInfo.txt Load fail");

	SkillInfoParser::Instance()->Init(1000);
	_LOG("SkillInfoParser::Instance()->Init ");


	SkillInventoryStoreParser::Instance()->Init(1000);	
	_LOG("SkillInventoryStoreParser::Instance()->Init ");

	_LOG("SkillInventoryStore.txt Load Start");

	bLoad =LoadParser((char*)(app_path + "\\SkillInventoryStore.txt").c_str(), SCRIPT_SKILL_INVEN_STORE,  SkillInventoryStoreParser::Instance());


	if(bLoad) _LOG("SkillInventoryStore.txt Load Complete");
	else _LOG("SkillInventoryStore.txt Load fail");
	//_LOG("SkillInventoryStoreParser::Instance()->SkillInventoryStore");

	_LOG("SkillInfo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\SkillInfo.txt").c_str(), SCRIPT_SKILLINFO, SkillInfoParser::Instance());
	if(bLoad) _LOG("SkillInfo.txt Load Complete");
	else _LOG("SkillInfo.txt Load fail");
	//_LOG("SkillInfoParser::Instance()->SkillInfo ");

	_LOG("StyleInfo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\StyleInfo.txt").c_str(), SCRIPT_STYLEINFO, SkillInfoParser::Instance());
	if(bLoad) _LOG("StyleInfo.txt Load Complete");
	else _LOG("StyleInfo.txt Load fail");
	
	MapInfoParser::Instance()->Init(100);
	_LOG("MapInfoParser::Instance()->Init(100)");

	

#if NATION_BUILD_VERSION >= 810
	_LOG("Worldsubinfo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\Worldsubinfo.txt").c_str(), SCRIPT_WORLD_SUB_INFO, MapInfoParser::Instance());
	if(bLoad) _LOG("Worldsubinfo.txt Load Complete");
	else _LOG("Worldsubinfo.txt Load fail");
	//_LOG("MapInfoParser::Instance()->world + Worldsubinfo");
#endif

	_LOG("World.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\World.txt").c_str(), SCRIPT_WORLD, MapInfoParser::Instance());
	if(bLoad) _LOG("World.txt Load Complete");
	else _LOG("World.txt Load fail");
	//_LOG("MapInfoParser::Instance()->world + World");


#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
	_LOG("Field.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\Field.txt").c_str(), SCRIPT_FIELD, MapInfoParser::Instance(), TRUE);
	if(bLoad) _LOG("Field.txt Load Complete");
	else _LOG("Field.txt Load fail");
	//_LOG("MapInfoParser::Instance()->world + Field");

	_LOG("Fieldwmo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\Fieldwmo.txt").c_str(), SCRIPT_FIELDWMO, MapInfoParser::Instance(), TRUE);
	if(bLoad) _LOG("Fieldwmo.txt Load Complete");
	else _LOG("Fieldwmo.txt Load fail");
	//_LOG("MapInfoParser::Instance()->world + Fieldwmo");

	_LOG("Fieldwut.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\Fieldwut.txt").c_str(), SCRIPT_FIELDWUT, MapInfoParser::Instance(), TRUE);
	if(bLoad) _LOG("Fieldwut.txt Load Complete");
	else _LOG("Fieldwut.txt Load fail");
	//_LOG("MapInfoParser::Instance()->world + Fieldwut");
#else
	_LOG("Field.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\Field.txt").c_str(), SCRIPT_FIELD, MapInfoParser::Instance());
	if(bLoad) _LOG("Field.txt Load Complete");
	else _LOG("Field.txt Load fail");
	//_LOG("MapInfoParser::Instance()->world + Field");

	_LOG("Fieldwmo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\Fieldwmo.txt").c_str(), SCRIPT_FIELDWMO, MapInfoParser::Instance());
	if(bLoad) _LOG("Fieldwmo.txt Load Complete");
	else _LOG("Fieldwmo.txt Load fail");
	//_LOG("MapInfoParser::Instance()->world + Fieldwmo");

	_LOG("Fieldwut.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\Fieldwut.txt").c_str(), SCRIPT_FIELDWUT, MapInfoParser::Instance());
	if(bLoad) _LOG("Fieldwut.txt Load Complete");
	else _LOG("Fieldwut.txt Load fail");
	//_LOG("MapInfoParser::Instance()->world + Fieldwut");
#endif
	

	_LOG("MapEnterancePortal.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\MapEnterancePortal.txt").c_str(), SCRIPT_MAPENTERANCEPORTAL, MapInfoParser::Instance());
	if(bLoad) _LOG("MapEnterancePortal.txt Load Complete");
	else _LOG("MapEnterancePortal.txt Load fail");
	//_LOG("MapInfoParser::Instance()->world + MapEnterancePortal");

	_LOG("RankOptionInfo.txt Load Start");
	//bLoad = RankOptionParser::Instance()->Load((char*)(app_path + "\\RankOptionInfo.txt").c_str());
	bLoad = LoadParser( (char*)(app_path + "\\RankOptionInfo.txt").c_str(), SCRIPT_RANKOPTIONINFO, RankOptionParser::Instance());
	if(bLoad) _LOG("RankOptionInfo.txt Load Complete");
	else _LOG("RankOptionInfo.txt Load fail");
	//_LOG("MapInfoParser::Instance()->RankOptionInfo");
	
	_LOG("SocketOptionInfo.txt Load Start");
	//bLoad = SocketOptionParser::Instance()->Load((char*)(app_path + "\\SocketOptionInfo.txt").c_str() );
#ifdef _NA_001699_20110104_SOCKET_OPTION_POINT    
    bLoad = LoadParser( (char*)(app_path + "\\ZardOptionInfo.txt").c_str(), SCRIPT_ZARD_OPTION_INFO, SocketOptionParser::Instance());
    if(bLoad) _LOG("ZardOptionInfo.txt Load Complete");
    else _LOG("ZardOptionInfo.txt Load fail");
    bLoad = LoadParser( (char*)(app_path + "\\ZardPointInfo.txt").c_str(), SCRIPT_ZARD_POINT_INFO, SocketOptionParser::Instance());
    if(bLoad) _LOG("ZardPointInfo.txt Load Complete");
    else _LOG("ZardPointInfo.txt Load fail");
    OptionListParser::Instance()->Init(100);
    bLoad = LoadParser( (char*)(app_path + "\\OptionList.txt").c_str(), SCRIPT_OPTION_LIST_INFO, OptionListParser::Instance());
    if(bLoad) _LOG("OptionList.txt Load Complete");
    else _LOG("OptionList.txt Load fail");
#else
    bLoad = LoadParser( (char*)(app_path + "\\SocketOptionInfo.txt").c_str(), SCRIPT_SOCKETOPTIONINFO, SocketOptionParser::Instance());
    if(bLoad) _LOG("SocketOptionInfo.txt Load Complete");
    else _LOG("SocketOptionInfo.txt Load fail");
#endif // _NA_001699_20110104_SOCKET_OPTION_POINT	
	//_LOG("MapInfoParser::Instance()->SocketOptionInfo");



	ExtraStoneOptionInfoParser::Instance()->Init(100);
	_LOG("ExtraStoneOptionInfoParser::Instance()->Init");

	_LOG("ExtraStoneOptionInfo.txt Load Start");
	//bLoad = ExtraStoneOptionInfoParser::Instance()->Load( (char*)(app_path + "\\ExtraStoneOptionInfo.txt").c_str());
	bLoad = LoadParser( (char*)(app_path + "\\ExtraStoneOptionInfo.txt").c_str(), SCRIPT_EXTRASTONEOPTIONINFO, ExtraStoneOptionInfoParser::Instance());
	if(bLoad) _LOG("ExtraStoneOptionInfo.txt Load Complete");
	else _LOG("ExtraStoneOptionInfo.txt Load fail");
	//_LOG("ExtraStoneOptionInfoParser::Instance()->ExtraStoneOptionInfo");

	if(NULL == g_pQuestInfoManager)
	{
		g_pQuestInfoManager = new QuestInfoManager2;
		_LOG("QuestInfo.txt Load Start");
		bLoad = g_pQuestInfoManager->Load((char*)(app_path + "\\QuestInfo.txt").c_str());	
		if(bLoad) _LOG("QuestInfo.txt Load Complete");
		else _LOG("QuestInfo.txt Load fail");
		//_LOG("QuestInfoManager2->QuestInfo");
	}       
	
	RewardInfoList::Instance()->Init("");
	_LOG("RewardInfoList::Instance()->Init");	

	//bLoad = RewardInfoList::Instance()->Load( (char*)(app_path + "\\RewardInfoList.txt").c_str());
	_LOG("RewardInfoList.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\RewardInfoList.txt").c_str(), SCRIPT_REWARDINFOLIST, RewardInfoList::Instance());
	if(bLoad) _LOG("RewardInfoList.txt Load Complete");
	else _LOG("RewardInfoList.txt Load fail");
	//_LOG("RewardInfoList::Instance()->RewardInfoList");

	_LOG("ACScript.txt Load Start");
	//AreaConquestInfoParser::Instance()->Load((char*)(app_path + "\\ACScript.txt").c_str());
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    AchievementParser::Instance()->Init( 1000 );
    _LOG("AchievementParser::Instance()->Init");
    bLoad = LoadParser( (char*)(app_path + "\\ACScript.txt").c_str(), SCRIPT_ACHIEVEMENT_INFO, AchievementParser::Instance());
#else
    AreaConquestInfoParser::Instance()->Init( 1000 );
    _LOG("AreaConquestInfoParser::Instance()->Init");
    bLoad = LoadParser( (char*)(app_path + "\\ACScript.txt").c_str(), SCRIPT_ACSCRIPT, AreaConquestInfoParser::Instance());
#endif	
	if(bLoad) _LOG("ACScript.txt Load Complete");
	else _LOG("ACScript.txt Load fail");
	//_LOG("AreaConquestInfoParser::Instance()->ACScript");

    MissionRewardParser::Instance()->Init(100);
	_LOG("MissionRewardParser::Instance()->Init");
	_LOG("MissionRewardInfo.txt Load Start");
#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
	LoadParser((char*)(app_path + "\\MissionRewardInfo.txt").c_str(), SCRIPT_MISSIONREWARDINFO, MissionRewardParser::Instance());
#else
	MissionRewardParser::Instance()->Load((char*)(app_path + "\\MissionRewardInfo.txt").c_str());
#endif	
	if(bLoad) _LOG("MissionRewardInfo.txt Load Complete");
	else _LOG("MissionRewardInfo.txt Load fail");
	//_LOG("MissionRewardParser::Instance()->MissionRewardInfo");


#if NATION_BUILD_VERSION >= 810
	//리미티드 아이템 추가
	//bLoad = LoadParser( (char*)(app_path + "\\Gambleoptionlist.txt").c_str(), SCRIPT_GAMBLE_OPTION, LimitedOptionParser::Instance());
	//_LOG("LimitedOptionParser::Instance()->Gambleoptionlist");
	_LOG("Limitediteminfo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\Limitediteminfo.txt").c_str(), SCRIPT_LIMITED_ITEM_INFO, LimitedItemInfoParser::Instance());
	if(bLoad) _LOG("Limitediteminfo.txt Load Complete");
	else _LOG("Limitediteminfo.txt Load fail");
	//_LOG("LimitedItemInfoParser::Instance()->Limitediteminfo");	
#endif

#if NATION_BUILD_VERSION >= 811
	_LOG("Luckyiteminfo.txt Load Start");
	bLoad = LoadParser( (char*)(app_path + "\\Luckyiteminfo.txt").c_str(), SCRIPT_FATE_ITEM_INFO, FateItemInfoParser::Instance());
	if(bLoad) _LOG("Luckyiteminfo.txt Load Complete");
	else _LOG("Luckyiteminfo.txt Load fail");
	//_LOG("FateItemInfoParser::Instance()->Luckyiteminfo");
#endif

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    _LOG("AccumulatePoint.txt Load Start");
    bLoad = LoadParser( (char*)(app_path + "\\AccumulatePoint.txt").c_str(), 
                        SCRIPT_ACCUMULATE_POINT, AccumulatePointParser::Instance());
    if(bLoad) _LOG("AccumulatePoint.txt Load Complete");
    else _LOG("AccumulatePoint.txt Load fail");
#endif
#ifdef _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    _LOG("ItemTypeList.txt Load Start");
    bLoad = LoadParser( (char*)(app_path + "\\ItemTypeList.txt").c_str(), 
                        SCRIPT_ITEMTYPELIST, ItemTypeList::Instance());
    if(bLoad) _LOG("ItemTypeList.txt Load Complete");
    else _LOG("ItemTypeList.txt Load fail");
#endif

#ifdef _NA002217_100728_EP2_SKILL_SYSTEM_DB_
    _LOG("PerkInfo.txt Load Start");
    bLoad = LoadParser( (char*)(app_path + "\\PerkInfo.txt").c_str(), 
        SCRIPT_PERKINFO, PerkInfoParser::Instance());
    if(bLoad) _LOG("PerkInfo.txt Load Complete");
    else _LOG("PerkInfo.txt Load fail");    
#endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    _LOG("TitleList.txt Load Start");
    TitleListParser::Instance()->Init();
    bLoad = LoadParser( (char*)(app_path + "\\TitleList.txt").c_str(), 
        SCRIPT_TITLE_LIST, TitleListParser::Instance());
    if(bLoad) _LOG("TitleList.txt Load Complete");
    else _LOG("TitleList.txt Load fail");    

    _LOG("FameList.txt Load Start");
    FameListParser::Instance()->Init();
    bLoad = LoadParser( (char*)(app_path + "\\FameList.txt").c_str(), 
        SCRIPT_FAME_LIST, FameListParser::Instance());
    if(bLoad) _LOG("FameList.txt Load Complete");
    else _LOG("FameList.txt Load fail");  

    _LOG("ReputeList.txt Load Start");
    ReputeListParser::Instance()->Init();
    bLoad = LoadParser( (char*)(app_path + "\\ReputeList.txt").c_str(), 
        SCRIPT_REPUTE_LIST, ReputeListParser::Instance());
    if(bLoad) _LOG("ReputeList.txt Load Complete");
    else _LOG("ReputeList.txt Load fail");  
#endif

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    LoadScriptParser<CurrencyInfoParser>(SCRIPT_CURRENCY_INFO, app_path, "CurrencyInfo.txt");
#endif
#ifdef _NA_003740_20111122_RANDOM_ITEM
    LoadScriptParser<RandomItemParser>(SCRIPT_RANDOM_ITEM_VALUE, app_path, _T("RandomItemValue.txt"));
    LoadScriptParser<RandomItemParser>(SCRIPT_RANDOM_ITEM_OPTION_TYPE, app_path, _T("RandomItemOptionType.txt"));
    LoadScriptParser<RandomItemParser>(SCRIPT_RANDOM_ITEM_INFO, app_path, _T("RandomItemInfo.txt"));
    LoadScriptParser<RandomItemParser>(SCRIPT_RANDOM_ITEM_KEY, app_path, _T("RandomItemKey.txt"));
#endif

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    LoadScriptParser<GuildFacilityInfoParser>(SCRIPT_GUILD_FACILITY_INFO, app_path, _T("GuildFacilityInfo.txt"));
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
		
#if NATION_BUILD_VERSION >= 904
	TableData::DestroyEnvironment();
#endif

	m_bInitialized = TRUE;

	_LOG("<< CCSunOnline::init(void)");
	return S_OK;
}

STDMETHODIMP CCSunOnline::release(void)
{
//	_LOG(">> CCSunOnline::release(void)");
	if(!m_bInitialized)
	{
//		_LOG("<< (INVALID) CCSunOnline::release(void)");
		return S_OK;
	}

	ItemInfoParser::Instance()->Release();
	SkillInfoParser::Instance()->Release();	
	MapInfoParser::Instance()->Release();
	RewardInfoList::Instance()->Release();	
#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED
	AreaConquestInfoParser::Instance()->Release();
#endif
	RankOptionParser::Instance()->Release();
	SocketOptionParser::Instance()->Release();
	ExtraStoneOptionInfoParser::Instance()->Release();
	MissionRewardParser::Instance()->Release();

	if(NULL != g_pQuestInfoManager)
	{
		g_pQuestInfoManager->ReleaseEx();
		delete g_pQuestInfoManager;
		g_pQuestInfoManager = NULL;
	}
	SkillInventoryStoreParser::Instance()->Release();	
	LimitedItemInfoParser::Instance()->Release();
	FateItemInfoParser::Instance()->Release();
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    TitleListParser::Instance()->Release();
    FameListParser::Instance()->Release();
    ReputeListParser::Instance()->Release();
#endif
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    CurrencyInfoParser::Instance()->Release();
#endif
#ifdef _NA_003740_20111122_RANDOM_ITEM
    RandomItemParser::Instance()->Release();
#endif
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildFacilityInfoParser::Instance()->Release();
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

	//////////////////////////////
	for(int n = 0; n < 5; n++)
	{
		if(g_pStrTblParser[n])
                        delete g_pStrTblParser[n];
		g_pStrTblParser[n]= 0;
	}

	if(g_pQuestInfoManager)
	{
		g_pQuestInfoManager->Unload();
		delete g_pQuestInfoManager;
		g_pQuestInfoManager = NULL;
	}

	m_bInitialized = FALSE;
//	_LOG("<< CCSunOnline::release(void)");

	if(g_pFileLogger)
		delete g_pFileLogger;
	g_pFileLogger = 0;

	return S_OK;
}

bool CCSunOnline::CreateQuestfoFile()
{    

     CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate | 
        CMarkupArchive::ArchiveModeStoring | 
        CMarkupArchive::ArchiveModeNotZipped);

    SetXMLCharSet(xml);    

    TCHAR path[MAX_PATH] = {0};
    sprintf(path, "%s", "C:\\WOPSConfig\\SUNGMS\\getQuest.xml");
    xml.Open(path);

    xml.AddElem(_T("QUEST"));


    for(QUEST_PRESET::iterator iter = g_pQuestInfoManager->m_QuestPreset.begin(); iter != g_pQuestInfoManager->m_QuestPreset.end();
        iter++)
    {
        QCODE questCODE = iter->first;
        QUEST_INFO& questINFO = iter->second;  

        xml.AddChildElem(_T("QUEST_INFO"));
        xml.IntoElem();

        xml.AddChildElem(_T("Name"), questINFO.pszName);	
        xml.AddChildElem(_T("Category"), questINFO.pszCategoryName);
        xml.AddChildElemEx(_T("Code"), questCODE );   
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        xml.AddChildElemEx(_T("IsDailyQuest"), questINFO.is_daily ? 1 : 0);
        xml.AddChildElemEx(_T("GroupCode"), questINFO.group_code);
#endif

        xml.OutOfElem();
    }

    xml.Close();
    return true;
}

bool CCSunOnline::CreateHonorSystemInfoFile()
{
#ifdef _NA_003027_20111013_HONOR_SYSTEM

    {
        CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate | 
            CMarkupArchive::ArchiveModeStoring | 
            CMarkupArchive::ArchiveModeNotZipped);

        SetXMLCharSet(xml);

        TCHAR path[MAX_PATH] = {0};
        sprintf(path, "%s", "C:\\WOPSConfig\\SUNGMS\\getTitle.xml");
        xml.Open(path);

        xml.AddElem(_T("TITLE"));

        TitleListParser* const title_parser = TitleListParser::Instance();
        TitleListParser::TitleInfoTable& title_table = title_parser->GetTitleInfoTable();

        TitleListParser::TitleInfo* title_info = NULL;
        title_table.SetFirst();    

        TCHAR name[256];

        while(title_info = title_table.GetNext())
        {
            xml.AddChildElem(_T("TITLE_INFO"));
            xml.IntoElem();            

            xml.AddChildElemEx(_T("title_code"), title_info->title_code);	

            if(title_accode.end() == title_accode.find(title_info->title_code))
            {
                xml.AddChildElemEx(_T("ac_code"), 0);                
                xml.AddChildElemEx(_T("ac_name"), "");
            }
            else
            {                
                xml.AddChildElemEx(_T("ac_code"), title_accode[title_info->title_code].ac_code_);
                GetStringByCode(title_accode[title_info->title_code].name_code_, name, 256);	
                xml.AddChildElemEx(_T("ac_name"), name);
            }

            GetStringByCode(title_info->title_name_code, name, 256);	
            xml.AddChildElem(_T("title_name"), name );        

            xml.OutOfElem();
        }
        xml.Close();
    }

    {
        CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate | 
            CMarkupArchive::ArchiveModeStoring | 
            CMarkupArchive::ArchiveModeNotZipped);

        SetXMLCharSet(xml);

        TCHAR path[MAX_PATH] = {0};
        sprintf(path, "%s", "C:\\WOPSConfig\\SUNGMS\\getFame.xml");
        xml.Open(path);

        xml.AddElem(_T("FAME"));

        FameListParser* const fame_parser = FameListParser::Instance();
        FameListParser::FameInfoMap& fame_map = fame_parser->GetFameInfoContainer();

        FameListParser::FameInfo* fame_info = NULL;

        FameListParser::FameInfoMap::iterator fame_iter = fame_map.begin();

        TCHAR name[256];

        for(; fame_iter != fame_map.end() ; fame_iter++)
        {
            xml.AddChildElem(_T("FAME_INFO"));
            xml.IntoElem(); 

            fame_info = fame_iter->second;

            xml.AddChildElemEx(_T("code"), fame_info->fame_code);	
            GetStringByCode(fame_info->fame_name_code, name, 256);	
            xml.AddChildElem(_T("name"), name);	
            GetStringByCode(fame_info->fame_description_code, name, 256);	
            xml.AddChildElem(_T("description"), name);	

            xml.AddChildElemEx(_T("step_count"), FameListParser::kMaxFameRating);	

            for(int i = 0 ; i < FameListParser::kMaxFameRating ; i++)
            {
                sprintf(name, "step_point_%d", i+1);
                xml.AddChildElemEx(name, fame_info->fame_rating_point[i]);
            }

            xml.OutOfElem();
            
        }
     
        xml.Close();
    }
    
    {
        CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate | 
            CMarkupArchive::ArchiveModeStoring | 
            CMarkupArchive::ArchiveModeNotZipped);

        SetXMLCharSet(xml);

        TCHAR path[MAX_PATH] = {0};
        sprintf(path, "%s", "C:\\WOPSConfig\\SUNGMS\\getRepute.xml");
        xml.Open(path);

        xml.AddElem(_T("REPUTE"));

        ReputeListParser* const repute_parser = ReputeListParser::Instance();
        ReputeListParser::ReputeInfoTable& repute_table = repute_parser->GetReputeInfoTable();

        ReputeListParser::ReputeInfo* repute_info = NULL;
        repute_table.SetFirst();    

        const int name_count = 1024;

        TCHAR name[name_count];

        while(repute_info = repute_table.GetNext())
        {
            xml.AddChildElem(_T("REPUTE_INFO"));
            xml.IntoElem();     

            xml.AddChildElemEx(_T("code"), repute_info->repute_code);	
            xml.AddChildElemEx(_T("group_code"), repute_info->repute_group_code);	
            GetStringByCode(repute_info->repute_name_code, name, name_count);	
            xml.AddChildElemEx(_T("name"), name);	
            GetStringByCode(repute_info->repute_group_name_code, name, name_count);	
            xml.AddChildElemEx(_T("group_name"), name);	
            GetStringByCode(repute_info->repute_description_code, name, name_count);	
            xml.AddChildElemEx(_T("description"), name);	

            xml.AddChildElemEx(_T("step_count"), ReputeListParser::kMaxReputeRating);	

            for(int i = 0 ; i < ReputeListParser::kMaxReputeRating ; i++)
            {
                sprintf(name, "step_point_%d", i+1);
                xml.AddChildElemEx(name, repute_info->repute_rating_point[i]);
            }

            xml.OutOfElem();
        }
        xml.Close();
    }

#endif



    return true;
}

bool CCSunOnline::CreateACInfoFile()
{
    CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate | 
        CMarkupArchive::ArchiveModeStoring | 
        CMarkupArchive::ArchiveModeNotZipped);

    SetXMLCharSet(xml);

    TCHAR path[MAX_PATH] = {0};
    sprintf(path, "%s", "C:\\WOPSConfig\\SUNGMS\\getAC.xml");
    xml.Open(path);

    title_accode.clear();

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    ::std::map<WORD, WORD> location_code;

    AchievementParser* achievement_parser = AchievementParser::Instance();

    ACHIEVEMENTINFO_ITERATOR achievement_iter = achievement_parser->Begin();

    TCHAR name[256];

    xml.AddElem(_T("AC_INFO"));

    for(;achievement_iter != achievement_parser->End() ; achievement_iter++)
    {
        Achievement_ParserInfo* parserinfo = *achievement_iter;

        location_code[parserinfo->location_code_] = parserinfo->location_code_;
        xml.AddChildElem(_T("AC_System_INFO"));
        xml.IntoElem();     
        xml.AddChildElemEx(_T("Code"), parserinfo->index_);	
        GetStringByCode(parserinfo->object_string_code_, name, 256);	
        xml.AddChildElemEx(_T("AchName"), name);	
        GetStringByCode(parserinfo->location_code_, name, 256);	
        xml.AddChildElemEx(_T("LocName"), name);	
        xml.AddChildElemEx(_T("LocCode"), parserinfo->location_code_);	

        xml.AddChildElem(_T("VALUE"));
        xml.IntoElem();     
        int value_count = 0;
        for( ; value_count < MAX_ACHIEVEMENT_OBJECT ; ++value_count)
        {
            if(parserinfo->obejcts_[value_count].object_code_ > 0)
            {
                xml.AddChildElemEx(_T("AchVal"),parserinfo->obejcts_[value_count].object_number_);
            }
            else
            {
                break;
            }
        }
        xml.OutOfElem();

        xml.AddChildElemEx(_T("ValCount"), value_count);	            


#ifdef _NA_003027_20111013_HONOR_SYSTEM	//AC 보상이 칭호이기 때문에 AC 정보와 칭호 정보르의 매칭값을 찾는다.
        value_count = 0;
        RewardInfoList* const reward_infolist = RewardInfoList::Instance();
        for( ; value_count < MAX_ACHIEVEMENT_REWARD ; ++value_count)
        {
            if(parserinfo->rewards_[value_count].reward_type_ == ACHIEVEMENT_REWARD_CODE)
            {   
                WORD reward_code = parserinfo->rewards_[value_count].reward_code_;
                RewardInfo* const reward_info = reward_infolist->FindRewardInfo(reward_code);
                if(reward_info)
                {
                    WORD title_code = reward_info->GetTitle();
                    if(title_code > 0)
                    {
                        TILE_ACCODE_INFO title_accode_info;
                        title_accode_info.ac_code_ = parserinfo->index_;
                        title_accode_info.name_code_ = parserinfo->object_string_code_;
                        
                        title_accode[title_code] = title_accode_info;
                    }
                }
            }
        }
        
#endif


        xml.OutOfElem();

    }
#endif

    xml.Close();

    return true;
}
bool CCSunOnline::CreateSkillInfoFile()
{
    CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate | 
        CMarkupArchive::ArchiveModeStoring | 
        CMarkupArchive::ArchiveModeNotZipped);

    SetXMLCharSet(xml);

    TCHAR path[MAX_PATH] = {0};
    sprintf(path, "%s", "C:\\WOPSConfig\\SUNGMS\\getSkill.xml");
    xml.Open(path);

    xml.AddElem(_T("SKILL"));

    SkillInfoParser* skill_parser = SkillInfoParser::Instance();    

    TCHAR pName[256];

    skill_parser->SetFirst();

    SkillScriptInfo* skill_info = NULL;

    while(skill_info = skill_parser->GetNextSkill())
    {
        xml.AddChildElem(_T("SKILL_INFO"));
        xml.IntoElem();    

        GetStringByCode(skill_info->m_dwSkillNcode, pName, 256);	

        xml.AddChildElemEx(_T("CLASS"), skill_info->m_dwClassDefine);
        xml.AddChildElemEx(_T("CODE"), skill_info->m_SkillCode);
        xml.AddChildElemEx(_T("LEVEL"), skill_info->m_wSkillLv);
        xml.AddChildElemEx(_T("NAME"), pName);
        xml.AddChildElemEx(_T("MAXLEVEL"), skill_info->m_wMaxLv);

        xml.OutOfElem();      
    }

    xml.OutOfElem();


    xml.AddElem(_T("PERK"));


    PerkInfoParser* perk_parser = PerkInfoParser::Instance();

    PerkInfoParser::PerkTable perk_table = perk_parser->GetPerkTable();
    PerkInfoParser::PerkTable::iterator perk_iter = perk_table.begin();

    for(; perk_iter != perk_table.end(); perk_iter++)
    {
        const PerkInfo* perk_info = perk_iter->second;

        xml.AddChildElem(_T("PERK_INFO"));
        xml.IntoElem();    

        GetStringByCode(perk_info->name_code_, pName, 256);	        
        xml.AddChildElemEx(_T("NAME"), pName);

        wsprintf(pName, "%I64d", perk_info->exp_value_);
        xml.AddChildElemEx(_T("REQ_EXP"), pName);

        xml.AddChildElemEx(_T("GROUP_CODE"), perk_info->perk_id_.group_);
        xml.AddChildElemEx(_T("LEVEL"), perk_info->perk_id_.level_);   

        xml.OutOfElem(); 
    }

    xml.OutOfElem();
    xml.Close();

    return true;
}

bool CCSunOnline::CreateCurrencyInfo() const
{
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM    
    CMarkupArchive xml(
        CMarkupArchive::ArchiveModeCreate|
        CMarkupArchive::ArchiveModeStoring| 
        CMarkupArchive::ArchiveModeNotZipped);

    SetXMLCharSet(xml);

    TCHAR path[MAX_PATH] = { 0 };
    _snprintf(path, _countof(path), "%s", "C:\\WOPSConfig\\SUNGMS\\getCurrencyInfo.xml");
    xml.Open(path);

    xml.AddElem(_T("CURRENCY"));

    const CurrencyInfoParser* const parser = CurrencyInfoParser::Instance();
    const CurrencyInfoParser::CurrencyInfoMap& currency_infos = parser->GetCurrencyInfoMap();
    for (CurrencyInfoParser::CurrencyInfoMap::const_iterator it = currency_infos.begin(); 
        it != currency_infos.end(); ++it)
    {
        const CurrencyInfoParser::CurrencyInfo* const currency_info = it->second;
        if (currency_info == NULL)
        {
            continue;
        }
        
        xml.AddChildElem(_T("CURRENCY_INFO"));
        xml.IntoElem();

        xml.AddChildElemEx(_T("currency_code"), currency_info->currency_code);
        enum { kStrBufSize = 256 };
        TCHAR name_str[kStrBufSize] = { 0 };
        GetStringByCode(currency_info->currency_name_code, name_str, _countof(name_str));
        xml.AddChildElemEx(_T("currency_name"), name_str);
        xml.AddChildElemEx(_T("max_coin"), currency_info->currency_max_point);
        
        xml.OutOfElem();
    }
    xml.Close();
#endif // _NA004034_20120102_POINT_WALLET_SYSTEM
    return true;
}

bool CCSunOnline::CreateRandomKey() const
{
#ifdef _NA_003740_20111122_RANDOM_ITEM    
    CMarkupArchive xml(
        CMarkupArchive::ArchiveModeCreate|
        CMarkupArchive::ArchiveModeStoring| 
        CMarkupArchive::ArchiveModeNotZipped);

    SetXMLCharSet(xml);

    TCHAR path[MAX_PATH] = { 0 };
    _snprintf(path, _countof(path), "%s", "C:\\WOPSConfig\\SUNGMS\\getRandomKey.xml");
    xml.Open(path);

    xml.AddElem(_T("RANDOM_KEY"));

    typedef RandomItemParser::RandomKeyManager::RandomKeys RandomKeys;
    const RandomKeys& random_keys = (RandomItemParser::Instance()->key_manager_).random_keys_;
    for (RandomKeys::const_iterator it = random_keys.begin(); it != random_keys.end(); ++it)
    {
        const RandomItemParser::RandomKey& random_key = it->second;
        const RandomItemParser::RandomOptionPart* const option_part = random_key.option_part;
        if (option_part == NULL)
        {
            continue;
        }
        if ((option_part->is_empty == false) && (random_key.value_part == NULL))
        {
            continue;
        }
        xml.AddChildElem(_T("RANDOM_INFO"));
        xml.IntoElem();
        {
            xml.AddChildElemEx(_T("option_key"), random_key.key);
            if (option_part->is_empty == true)
            {
                xml.AddChildElemEx(_T("option_is_empty"), 1);
                xml.AddChildElemEx(_T("option_code"), 0);
                xml.AddChildElemEx(_T("option_name"), _T(""));
                xml.AddChildElemEx(_T("option_value_grade"), 0);
                xml.AddChildElemEx(_T("option_value"), 0);
            }
            else
            {
                xml.AddChildElemEx(_T("option_is_empty"), 0);
                xml.AddChildElemEx(_T("option_code"), option_part->kind);
                const BASE_OptionList* const option_list = 
                    OptionListParser::Instance()->GetOptionList(option_part->kind);
                if (option_list != NULL)
                {
                    enum { kStrBufSize = 256 };
                    TCHAR option_name[kStrBufSize] = { 0 };
                    GetStringByCode(option_list->mdwNameCode, option_name, _countof(option_name));
                    xml.AddChildElemEx(_T("option_name"), option_name);
                }
                else
                {
                    xml.AddChildElemEx(_T("option_name"), "");
                }
                const RandomItemParser::RandomValuePart* const value_part = random_key.value_part;
                xml.AddChildElemEx(_T("option_value_grade"), value_part->grade);
                xml.AddChildElemEx(_T("option_value"), value_part->my_value);
            }
        }
        xml.OutOfElem();
    }
    xml.Close();
#endif // _NA_003740_20111122_RANDOM_ITEM
    return true;
}

bool CCSunOnline::CreateRandomItemInfo() const
{
#ifdef _NA_003740_20111122_RANDOM_ITEM
    CMarkupArchive xml(
        CMarkupArchive::ArchiveModeCreate|
        CMarkupArchive::ArchiveModeStoring| 
        CMarkupArchive::ArchiveModeNotZipped);

    SetXMLCharSet(xml);

    TCHAR path[MAX_PATH] = { 0 };
    _snprintf(path, _countof(path), "%s", "C:\\WOPSConfig\\SUNGMS\\getRandomItemInfo.xml");
    xml.Open(path);

    xml.AddElem(_T("RANDOM_ITEM"));

    typedef RandomItemParser::RandomItemManager::RandomItems RandomItems;
    const RandomItems random_items = (RandomItemParser::Instance()->item_manager_).random_items_;
    for (RandomItems::const_iterator it = random_items.begin(); it != random_items.end(); ++it)
    {
        xml.AddChildElem(_T("RANDOM_ITEM_INFO"));
        xml.IntoElem();
        {
            const RandomItemParser::RandomItem& random_item = it->second;
            xml.AddChildElemEx(_T("item_code"), random_item.item_code);

            typedef RandomItemParser::RandomItem::Options RandomOptions;
            const RandomOptions& random_options = random_item.options;
            for (size_t i = 0; i < random_options.size(); ++i)
            {
                const RandomItemParser::RandomOption* const random_option = random_options[i];
                if (random_option == NULL)
                {
                    continue;
                }
                typedef RandomItemParser::RandomOption::Keys RandomKeys;
                const RandomKeys& random_keys = random_option->keys;
                if (random_keys.empty())
                {
                    continue;
                }

                xml.AddChildElem(_T("RANDOM_OPTION"));
                xml.IntoElem();
                {
                    xml.AddChildElemEx(_T("number"), i+1);

                    typedef RandomItemParser::RandomOption::Keys RandomKeys;
                    const RandomKeys& random_keys = random_option->keys;
                    for (RandomKeys::const_iterator it2 = random_keys.begin(); 
                        it2 != random_keys.end(); ++it2)
                    {
                        const RandomItemParser::RandomKey* const random_key = it2->second;
                        if (random_key == NULL)
                        {
                            continue;
                        }
                        const RandomItemParser::RandomOptionPart* const option_part = 
                            random_key->option_part;
                        if (option_part == NULL)
                        {
                            continue;
                        }
                        if ((option_part->is_empty == false) && (random_key->value_part == NULL))
                        {
                            continue;
                        }
                        xml.AddChildElem(_T("RANDOM_INFO"));
                        xml.IntoElem();
                        {
                            xml.AddChildElemEx(_T("option_key"), random_key->key);
                            if (option_part->is_empty == true)
                            {
                                xml.AddChildElemEx(_T("option_is_empty"), 1);
                                xml.AddChildElemEx(_T("option_code"), 0);
                                xml.AddChildElemEx(_T("option_name"), _T(""));
                                xml.AddChildElemEx(_T("option_value_grade"), 0);
                                xml.AddChildElemEx(_T("option_value"), 0);
                            }
                            else
                            {
                                xml.AddChildElemEx(_T("option_is_empty"), 0);
                                xml.AddChildElemEx(_T("option_code"), option_part->kind);
                                const BASE_OptionList* const option_list = 
                                    OptionListParser::Instance()->GetOptionList(option_part->kind);
                                if (option_list != NULL)
                                {
                                    enum { kStrBufSize = 256 };
                                    TCHAR option_name[kStrBufSize] = { 0 };
                                    GetStringByCode(option_list->mdwNameCode, option_name, _countof(option_name));
                                    xml.AddChildElemEx(_T("option_name"), option_name);
                                }
                                else
                                {
                                    xml.AddChildElemEx(_T("option_name"), "");
                                }
                                const RandomItemParser::RandomValuePart* const value_part = 
                                    random_key->value_part;
                                xml.AddChildElemEx(_T("option_value_grade"), value_part->grade);
                                xml.AddChildElemEx(_T("option_value"), value_part->my_value);
                            }
                        }
                        xml.OutOfElem();
                    }
                }
                xml.OutOfElem();
            }
        }
        xml.OutOfElem();
    }
    xml.Close();
#endif // _NA_003740_20111122_RANDOM_ITEM
    return true;
}

bool CCSunOnline::CreateXMLFile(const char* pszFileName, bool bRealName)
{
	_LOG( ">> CCSunOnline::createItem(void)");
	if(!isInitialized())
		return false;

	CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate | 
		CMarkupArchive::ArchiveModeStoring | 
		CMarkupArchive::ArchiveModeNotZipped);

	SetXMLCharSet(xml);

	TCHAR path[MAX_PATH] = {0};
	sprintf(path, "%s\\%s", "C:\\WOPSConfig\\SUNGMS", pszFileName);
	xml.Open(path);

	xml.AddElem(_T("CREATEITEM_INFO"));	

	TCHAR* szCharName[] = {"BERSERKER", "DRAGON", "SHADOW", "VALKYRIE","MAGICIAN"};

	//캐릭터정보 생성
	for(int i=0;i<5;i++)
	{
		xml.AddChildElem(_T("CHARACTER_INFO"));
		xml.IntoElem();

		std::string str = szCharName[i];
		xml.AddChildElem(_T("Name"), str.c_str());
		xml.AddChildElemEx(_T("Code"), i+1);
		xml.OutOfElem();
	}

	STRTBL_PARSER4->SetFirst();
	char* pszItemCategory = NULL;	
	std::map<WORD,WORD> maptypecode;
	
	while( pszItemCategory = STRTBL_PARSER4->GetNext() )
	{
		xml.AddChildElem(_T("ITEM_TYPE"));
		xml.IntoElem();

		eITEMTYPE type = (eITEMTYPE)STRTBL_PARSER4->GetCodeByString( pszItemCategory );
		xml.AddChildElemEx(_T("TYPE_CODE"), type);		
		xml.AddChildElem(_T("TYPE_NAME"), pszItemCategory );
		maptypecode[type] = type;

		xml.OutOfElem();
	}

	//아이템정보
	ItemInfoParser::Instance()->SetFirst();

	BASE_ITEMINFO* pItemInfo = NULL;
	while( (pItemInfo = ItemInfoParser::Instance()->GetNext()) != NULL )
	{
        char* psz = STRTBL_PARSER1->GetStringByCode(pItemInfo->m_NCode);

		if(!psz)
		{
			_LOG("Not Find Name Code (NameCode: %d, ItemCode: %d)", pItemInfo->m_NCode, pItemInfo->m_Code);
			continue;
		}

		//2008-05-22 문상현 Itemtype.txt 에 없는 타입의 아이템이 있다면 ItemType 를 추가한다.
		if(maptypecode.end() == maptypecode.find(pItemInfo->m_wType))
		{
			char szTypeName[256];
			xml.AddChildElem(_T("ITEM_TYPE"));
			xml.IntoElem();

			xml.AddChildElemEx(_T("TYPE_CODE"), pItemInfo->m_wType);	
			wsprintf(szTypeName, "unknown(%d)", pItemInfo->m_wType);
			xml.AddChildElem(_T("TYPE_NAME"), szTypeName );
			xml.OutOfElem();

			maptypecode[pItemInfo->m_wType] = pItemInfo->m_wType;
		}
		//////

		xml.AddChildElem(_T("ITEM_INFO"));
		xml.IntoElem();	

#ifdef __NA_1247_POSSESSION_ITEM		
		xml.AddChildElemEx(_T("IsPossessionType"), pItemInfo->m_bPossessionItem ? 1 : 0);		//귀속 여부
#endif
#ifdef __NA_0_POSSESSION_ITEM_DROP	
		xml.AddChildElemEx("IsDestroyItem", 0);		
#endif
		xml.AddChildElemEx(_T("Code"), pItemInfo->m_Code);		//코드.
		xml.AddChildElemEx(_T("Type"), pItemInfo->m_wType);		//타입
		xml.AddChildElemEx(_T("SubType"), pItemInfo->m_wSubType);		//서브타입
		xml.AddChildElemEx(_T("ChargeType"), pItemInfo->m_ChargeType);		//유료화타입
		xml.AddChildElemEx(_T("ChargeSubType"), pItemInfo->m_ChargeSubType);		//유료화서브타입

        std::string sItemName = psz;
		//	char pszItemName[256] = {0};
		//	strcpy(pszItemName, psz);
		if((pItemInfo->m_SCNCode != 0) && (!bRealName))	//두번째 네임코드
		{
			TCHAR* pszSecItemName = STRTBL_PARSER1->GetStringByCode(pItemInfo->m_SCNCode);
			if(pszSecItemName) sItemName += pszSecItemName;
			//	strcat(pszItemName, pszSecItemName);
		}

		xml.AddChildElem(_T("Name"), sItemName.c_str());		//아이템명

		if( (eITEM_MATERIAL_TYPE_CAN_BOTH == pItemInfo->m_byMaterialType ) || 
			( eITEM_MATERIAL_TYPE_CAN_EQUIP == pItemInfo->m_byMaterialType))
			xml.AddChildElemEx(_T("EqPos"), pItemInfo->m_wEqPos);		//장비 위치
		else
			xml.AddChildElemEx(_T("EqPos"), -1);		//장비 위치

		if( !pItemInfo->IsWeapon() && !pItemInfo->IsArmor() )
		{
			xml.AddChildElemEx(_T("IsDura"), 0);
		}
		else
		{
			//	(pItemInfo->m_Dura)
			xml.AddChildElemEx(_T("IsDura"), 1);
		}

		//내구성or개수
		xml.AddChildElemEx(_T("Dura"), pItemInfo->m_Dura);
		xml.OutOfElem();		
	}

	/*

	enum eITEMTYPE
{

};

	*/

	////////////////////////////////////////////////	
	std::list<WORD> listItemType;
	{
	listItemType.push_back(eITEMTYPE_INVALID);//			= 0,	
	// 무기
	listItemType.push_back(eITEMTYPE_TWOHANDAXE);//		= 1,	//	양손도끼1
	listItemType.push_back(eITEMTYPE_TWOHANDSWORD);//		= 2,	//	양손검1
	listItemType.push_back(eITEMTYPE_ONEHANDSWORD);//		= 3,	//	한손검1
	listItemType.push_back(eITEMTYPE_SPEAR);//				= 4,	//	창1
#if NATION_BUILD_VERSION >= 816
	listItemType.push_back(eITEMTYPE_DAGGER);//			= 5,	//	단검1
#endif
	listItemType.push_back(eITEMTYPE_WHIP);//				= 6,	//	채찍1
	listItemType.push_back(eITEMTYPE_ONEHANDCROSSBOW);//	= 7,	//	한손석궁1
	listItemType.push_back(eITEMTYPE_ETHERWEAPON);//		= 8,	//	양손석궁1,에테르웨폰
	listItemType.push_back(eITEMTYPE_SCIMITAR);//			= 9,	//	곡도1
	listItemType.push_back(eITEMTYPE_STAFF);//				= 10,	//	지팡이1
	listItemType.push_back(eITEMTYPE_ORB);//				= 11,	//	오브1
	listItemType.push_back(eITEMTYPE_FISHING_POLE);//		= 12,	//  낚시대
	listItemType.push_back(eITEMTYPE_PUNCH);//				= 13,	//	맨주먹

	// 방어구
	listItemType.push_back(eITEMTYPE_ARMOR);//				= 101,	//	갑옷
	listItemType.push_back(eITEMTYPE_PROTECTOR);//			= 201,	//	프로텍터
	listItemType.push_back(eITEMTYPE_HELMET);//			= 301,	//	헬멧
	listItemType.push_back(eITEMTYPE_PANTS);//				= 401,	//	바지
	listItemType.push_back(eITEMTYPE_BOOTS);//				= 501,	//	부츠
	listItemType.push_back(eITEMTYPE_GLOVE);//				= 601,	//	장갑
	listItemType.push_back(eITEMTYPE_BELT);//				= 701,	//	벨트
	listItemType.push_back(eITEMTYPE_SHIRTS);//			= 801,	//	셔츠

	// 악세서리
	listItemType.push_back(eITEMTYPE_RING);//				= 811,	//	반지 
	listItemType.push_back(eITEMTYPE_NECKLACE);//			= 821,	//	목걸이

	
	// 클래스별 특수 악세서리로 변경
	listItemType.push_back(eITEMTYPE_BERSERKER_SACCESSORY);//	= 831,	//	망토
	listItemType.push_back(eITEMTYPE_DRAGON_SACCESSORY);//		= 841,	//	날개
	listItemType.push_back(eITEMTYPE_VALKYRIE_SACCESSORY);//	= 851,	//	속성석
	listItemType.push_back(eITEMTYPE_SHADOW_SACCESSORY);//		= 861,	//	수호정령, 제네레이터
	listItemType.push_back(eITEMTYPE_ELEMENTALIST_SACCESSORY);// = 871,	//	문장

	listItemType.push_back(eITEMTYPE_BERSERKER_KALBUS_ACCESSORY);//	= 881,
	listItemType.push_back(eITEMTYPE_DRAGON__KALBUS_ACCESSORY);//		= 882,
	listItemType.push_back(eITEMTYPE_VALKYRIE__KALBU_ACCESSORY);//		= 883,
#if NATION_BUILD_VERSION >= 816
	listItemType.push_back(eITEMTYPE_ELEMENTALIST_KALBUS_ACCESSORY);// = 884,
	listItemType.push_back(eITEMTYPE_FISHING_ROD);//			= 885,
	listItemType.push_back(eITEMTYPE_FISHING_BAIT);//			= 886,
	listItemType.push_back(eITEMTYPE_FISH);//					= 887,
#endif

	listItemType.push_back(eITEMTYPE_WASTE);//					= 911,	//	소모성아이템
	listItemType.push_back(eITEMTYPE_ENCHANT);//				= 921,	//	인첸트보석1
	listItemType.push_back(eITEMTYPE_CRYSTAL);//				= 931,	//	결정체A
	listItemType.push_back(eITEMTYPE_SOCKET);//				= 941,	//	소켓아이템
	listItemType.push_back(eITEMTYPE_ARROW);//					= 951,	//	석궁용 화살

	listItemType.push_back(eITEMTYPE_SOCKETMATERIAL);//		= 961,	//	소켓재료
	listItemType.push_back(eITEMTYPE_ACCESSORYMATERIAL);//		= 971,	//	악세서리재료
	listItemType.push_back(eITEMTYPE_SACCESSORYMATERIAL);//	= 981,	//	특수악세서리재료

	listItemType.push_back(eITEMTYPE_STORY);//					= 982,
	listItemType.push_back(eITEMTYPE_EXPECT);//				= 983,
	listItemType.push_back(eITEMTYPE_NORMAL);//				= 984,
	listItemType.push_back(eITEMTYPE_GAMBLE);//				= 985,
	listItemType.push_back(eITEMTYPE_SPECIAL);//				= 986,
	listItemType.push_back(eITEMTYPE_GUILD);// 				= 987,
	listItemType.push_back(eITEMTYPE_AC);//					= 988,
	listItemType.push_back(eITEMTYPE_EVENT);//					= 989,
	listItemType.push_back(eITEMTYPE_JUNK);//					= 990,
	listItemType.push_back(eITEMTYPE_QUEST);//					= 991,
	listItemType.push_back(eITEMTYPE_RANDOM);//				= 992,	// 클라이언트에서만 사용할 렌덤 아이템	
	listItemType.push_back(eITEMTYPE_TOTEM);//					= 993,	// 토템아이템
	listItemType.push_back(eITEMTYPE_TOP_OF_TRIAL);//			= 994,	// 저주의탑 입장 아이템
	listItemType.push_back(eITEMTYPE_TOP_OF_TRIAL_CHARGE);//	= 995,	// 저주의탑 입장 아이템(캐쉬아이템)

#ifdef __NA000844_080324_100LEVEL_CHAGNEUP_REBUILDING__
	listItemType.push_back(eITEMTYPE_PORTAL_QUEST_ITEM);//		= 996,	// 일단 100레벨 용도이고 (성격:맵이동+퀘스트아이템)이고, 이후 확장할 수 있도록 하자.
#endif
	listItemType.push_back(eITEMTYPE_ETHER_BULLET);//			= 997,	// 에테르 탄환  __NA_000968_ETHER_EMISSION_DEVICE
	listItemType.push_back(eITEMTYPE_ETHER_DEVICE);//			= 998,	// 에테르 장치	__NA_000968_ETHER_EMISSION_DEVICE

#ifdef __NA001051_080526_EVENT_EFFECT_RENDER_BY_SCRIPT_2ndPROCESS__
	listItemType.push_back(eITEMTYPE_RENDER_EFFECT_ITEM);//	= 999,	// Inventory에 존재하지만 외부로 보여지는 Effect가 존재할 경우
#endif

	listItemType.push_back(eITEMTYPE_BILL);//					= 1000,	//	화폐

	// 유료화 관련 추가 아이템
	listItemType.push_back(eITEMTYPE_SUPERMASTER);//			= 20000,	// 슈퍼방장
	listItemType.push_back(eITEMTYPE_PORTAL);//				= 20001,	// 포탈용아이템
	listItemType.push_back(eITEMTYPE_SAVE_COORD);//			= 20002,	// 좌표 저장 아이템
	listItemType.push_back(eITEMTYPE_SOCKETEXTRACT);//			= 20003,	// 소켓제거아이템
	listItemType.push_back(eITEMTYPE_CHAT_SHOUT);//			= 20004,	// 외치기아이템(기간제)
	listItemType.push_back(eITEMTYPE_RECALL);//				= 20005,	// 몬스터소환아이템
	listItemType.push_back(eITEMTYPE_REPAIR);//				= 20006,	// 내구도수리아이템
	listItemType.push_back(eITEMTYPE_HOLDDROP);//				= 20007,	// 드랍방지용아이템
	listItemType.push_back(eITEMTYPE_STAT_RESET);//			= 20008,	// 스텟초기화아이템(힘,민첩,체력,지력,정신력,숙련도1,2,스킬포인트)
	listItemType.push_back(eITEMTYPE_EXTRATAB);//				= 20009,	// 탭추가아이템(사용시 1탭씩 추가)
	listItemType.push_back(eITEMTYPE_CLEAR_COORD);//			= 20010,	// 좌표 초기화 아이템
	listItemType.push_back(eITEMTYPE_RECALL_ALLIANCE);//		= 20011,	// 여신의 은혜 아이템
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	listItemType.push_back(eITEMTYPE_GUILD_PVP_ENTRANCE);//	= 20012,	// 길드 PVP 입장권
#endif
	listItemType.push_back(eITEMTYPE_EXTEND_CASH_SLOT);//		= 20013,	// 캐쉬 슬롯 2개 확장
	listItemType.push_back(eITEMTYPE_EXTEND_DATE_WEAPON);//	= 20014,	// 기간연장 아이템(무기)	__NA_000493_20070712_SHOP_WEAPON_ARMOR_ITEM
	listItemType.push_back(eITEMTYPE_RANK_DOWN);//				= 20015,	// 랭크하향 망치
	listItemType.push_back(eITEMTYPE_EXTEND_DATE_ARMOR);//		= 20016,	// 기간연장 아이템(방어구)	__NA_000493_20070712_SHOP_WEAPON_ARMOR_ITEM
	listItemType.push_back(eITEMTYPE_HAMMER_OF_ALCHEMIST);//	= 20017,	// 연금술사의 망치(ALCHEMIST's hammer) 리미티드 아이템 수리

#ifdef __NA_000569_20080117_GUILD_MARK
	listItemType.push_back(eITEMTYPE_GUILDMARK_SEAL);// = 20018,
#endif
	listItemType.push_back(eITEMTYPE_HOLDDROP2);//				= 20019,	// 종속의 고리2  __NA_825_HOLDDROP2
	listItemType.push_back(eITEMTYPE_PET);//					= 20020,	// 펫 __NA_0_PET_SYSTEM
	listItemType.push_back(eITEMTYPE_PET_FEED);//				= 20021,	// 펫 __NA_0_PET_FEED

//_YJ_FISHING_SYSTEM_000933
	listItemType.push_back(eITEMTYPE_FISH_HYBRID);//			= 20022,	//잡종   물고기
	listItemType.push_back(eITEMTYPE_FISH_WASTE);//			= 20023,	//소모성 물고기
	listItemType.push_back(eITEMTYPE_FISH_HIGH_CLASS);//		= 20024,	//고급   물고기
	listItemType.push_back(eITEMTYPE_FISH_RARE);//				= 20025,	//희귀   물고기
	listItemType.push_back(eITEMTYPE_FISH_AC);//				= 20026,	//AC     물고기
	listItemType.push_back(eITEMTYPE_FISH_CASH);//				= 20027,	//캐시   물고기
//_YJ_FISHING_SYSTEM_000933

#ifdef __NA000584_20080201_GUILD_WAREHOUSE
	listItemType.push_back(eITEMTYPE_GUILDWAREHOUSE_SLOTCASH);// = 20028,	//길드 창고 슬롯 확장
#endif

#ifdef __NA_000946_20080415_LOTTERY_SYSTEM
	listItemType.push_back(eITEMTYPE_LOTTO);//					= 20029,	//복권 아이템
#endif

#ifdef __NA_001043_20080517_WORLD_SHOUT_CONSUME_MODIFY	// 외치기아이템(소모성) 디파인
	listItemType.push_back(eITEMTYPE_CHAT_SHOUT_CONSUME);//	= 20030,	
#endif

#ifdef __NA_001092_20080717_ADD_SKILL_ITEM
	listItemType.push_back(eITEMTYPE_FIRECRACKER);//			= 20031,	
	listItemType.push_back(eITEMTYPE_POLLEN);//				= 20032,	
	listItemType.push_back(eITEMTYPE_TORCH);//					= 20033,	
	listItemType.push_back(eITEMTYPE_MINE);//					= 20034,	
#endif
#ifdef __NA001094_080707_SURVIVAL_SOLOING_QUEST_SYSTEM__
	listItemType.push_back(eITEMTYPE_SSQ);//					= 20035,
#endif

#ifdef __NA001124_ITEM_CHANGE_SYSTEM
	listItemType.push_back(eITEMTYPE_STAT_CHANGE);//			= 20036,	//아이템 스텟 교환
#endif

#ifdef __NA_1131_PET_NAME_SETTING
	listItemType.push_back(eITEMTYPE_PET_NAME_CARD);//			= 20037,	//< 펫 이름 설정 아이템
#endif
	listItemType.push_back(eITEMTYPE_MAX);//,
		
	}	

	

	////////////////////////////////////////////////

	//소켓옵션
	//	const RANKOPTION& sockOpt = ItemOptionParser::Instance()->GetItemRankOption(OPTION_KIND_SOCKET);
#ifdef _NA_0_20100727_SOCKET_SYSTEM_PARSER
    const SocketOptionParser::SocketOptionMap& socket_option_map = SocketOptionParser::Instance()->GetSocketOptionMap();

    OptionListParser* option_list_parser = OptionListParser::Instance();
    BASE_OptionList* base_option_list = NULL;

    FOREACH_CONTAINER(const SocketOptionParser::SocketOptionMap::value_type& socket_option_pair, socket_option_map, SocketOptionParser::SocketOptionMap)
    {
        const sSOCKET_OPTION* const socket_option = socket_option_pair.second;

        if(!socket_option) 
            continue;

        xml.AddChildElem(_T("ZARD_INFO"));
        xml.IntoElem();

        TCHAR* pszName = STRTBL_PARSER1->GetStringByCode(socket_option->name_code);
        if(pszName == NULL)
        {
            pszName = "unknow_zard";
        }

        xml.AddChildElemEx("name"                     , pszName);
        xml.AddChildElemEx("index"                    , socket_option->index);
        xml.AddChildElemEx("Item_code"                    , socket_option->item_code);
            
        int option_index = 0;
        

        while(true)        
        {
            const sSOCKET_OPTION::OptionPointInfo* option_point_info = socket_option->GetPointInfo(option_index++);
            if(option_point_info == NULL) 
                break;

            
            base_option_list = option_list_parser->GetOptionList(option_point_info->item_option_code);            
            if(base_option_list == NULL)
            {
                pszName = "unknow_option";
            }
            else
            {
                pszName = STRTBL_PARSER1->GetStringByCode(base_option_list->mdwNameCode);
                if(pszName == NULL)
                {
                    pszName = "unknow_option";
                }
            }
            
            xml.AddChildElem(_T("OPTION_INFO"));
            xml.IntoElem();           
            xml.AddChildElemEx("index"                    , socket_option->index);
            xml.AddChildElemEx("name"                     , pszName);            
            xml.AddChildElemEx("point"                    , option_point_info->option_point);            
            xml.OutOfElem();
        }

        xml.OutOfElem();    
    }
   
#else //_NA_0_20100727_SOCKET_SYSTEM_PARSER
    
    //랭크옵션
    //	for(BYTE kind = (BYTE)eRANK_OPTION_ITEM_WEAPON;kind < eRANK_OPTION_ITEM_TYPE_MAX;kind++)

    for(std::list<WORD>::iterator iter = listItemType.begin(); iter != listItemType.end(); iter++)
    {
        //	const RANKOPTION& rankOpt = ItemOptionParser::Instance()->GetItemRankOption((eOPTION_KIND)kind);
        WORD wType = *iter;

        for(int i=0;i<MAX_ITEM_RANK_OPTION_NUM;i++)
        {
            const sRANK_OPTION* rankOpt = RankOptionParser::Instance()->GetRankOption( wType, i);
            if(!rankOpt)
                continue;

            //텍스트 얻어오고..
            TCHAR* pszName = STRTBL_PARSER1->GetStringByCode(rankOpt->m_OptionNCode);

            if(pszName == NULL)	continue;

            for(BYTE rank = (BYTE)RANK_E;rank<RANK_MAX;rank++)
            {

                if(rankOpt->m_Value[rank] == RANK_E)
                    continue;

                xml.AddChildElem(_T("RANK_INFO"));
                xml.IntoElem();

                TCHAR szRankValue[128];

                //랭크옵션종류
                //	xml.AddChildElemEx("RankKind", kind);
                xml.AddChildElemEx("ItemType", wType);

                //랭크명
                string sRank = ConvertRankType((eRANK)rank);

                //밸류값
                BYTE byValue = rankOpt->m_Value[rank];

                //인덱스

                char *pdest = strrchr(pszName, ':');
                if(pdest)
                {
                    int ret = pdest - pszName;
                    pszName[ret] = '\0';
                }               
                //랭크 테그 변경 
                //2009-11-06 문상현                
                /*wsprintf(szRankValue, "%s:%d:%d", pszName, byValue, i);				
                xml.AddChildElem("RankOption", szRankValue);*/
                xml.AddChildElem("RankName", pszName);
                xml.AddChildElemEx("RankValue", byValue);
                xml.AddChildElemEx("RankOptionIndex", i);
                xml.AddChildElemEx("NameCode", rankOpt->m_OptionNCode);

                //랭크명
                xml.AddChildElem("Rank", sRank.c_str());

                //랭크인덱스
                xml.AddChildElemEx("RankIndex", (int)rank);

                xml.OutOfElem();
            }			
        }
    }

	for(int i = 0; i < MAX_ITEM_OPTION_KIND_NUM ;i++)
	{        
		const sSOCKET_OPTION* sockOpt = SocketOptionParser::Instance()->GetSocketOption(i);
		if(!sockOpt)
		{
			_LOG("Invaild SocketOption: Index(%d)", i);
			continue;
		}

		//텍스트 얻어오고...
		TCHAR* pszName = STRTBL_PARSER1->GetStringByCode(sockOpt->m_NCode);
		if(pszName == NULL)
		{
			_LOG("Invaild String(stringtable.txt): NameCode(%d)", sockOpt->m_NCode);
			continue;
		}

		//텍스트, 벨류, 인덱스, 레벨
		for(BYTE sock=(BYTE)SOCKET_1;sock<SOCKET_MAX;sock++)
		{
			for(BYTE lv=(BYTE)SOCKETLV_LOW;lv<(BYTE)SOCKETLV_MAX;lv++)
			{
				xml.AddChildElem(_T("SOCKET_INFO"));
				xml.IntoElem();

				TCHAR szSockValue[128];

				//소켓명
				string sSock = ConvertSocketType((eSOCKET)sock);

				//밸류값<저레벨, 고레벨>
				//	BYTE byValue = sockOpt.m_ROption[i].m_Attribute[lv];
				BYTE byValue = sockOpt->m_iValue[lv];
                xml.AddChildElem("SocketName", pszName);
                xml.AddChildElemEx("SocketValue", byValue);
                xml.AddChildElemEx("SocketOptionIndex", i);
                xml.AddChildElemEx("SocketLevel", lv);
                xml.AddChildElemEx("NameCode", sockOpt->m_NCode);
				xml.AddChildElem("Socket", sSock.c_str());

				xml.AddChildElemEx("SocketIndex", (int)sock);

				xml.OutOfElem();
			}
		}
    }
#endif //_NA_0_20100727_SOCKET_SYSTEM_PARSER
	

	//맵정보	
	std::map<DWORD64, std::string> m_mapOverlap;
	std::map<DWORD64, std::string>::iterator OverlapIter;
	sMAPINFO_HASH& stMapInfo = MapInfoParser::Instance()->GetMapInfoHash();
	for(sMAPINFO_HASH::const_iterator pIter = stMapInfo.begin();pIter!=stMapInfo.end();++pIter)
	{
		sMAPINFO* pMapInfo = (sMAPINFO*)(pIter->second);
		if(pMapInfo)
		{	
			//맵 이름
			TCHAR* pszName = STRTBL_PARSER2->GetStringByCode(pMapInfo->dwNCode);
			if(pszName == NULL)	
				continue;

			DWORD64 map_key = ((DWORD64)(((DWORD)((pMapInfo->MapCode) & 0xffffffff)) | 
				((DWORD64)((DWORD)((pMapInfo->dwNCode) & 0xffffffff))) << 32));

			if( (OverlapIter = m_mapOverlap.find(map_key)) != m_mapOverlap.end())
			{	
				continue;
			}

			std::pair<DWORD64, std::string> MapValue;

			MapValue.first = map_key;
			MapValue.second = pszName;

			m_mapOverlap.insert(MapValue);

			xml.AddChildElem(_T("MAP_INFO"));
			xml.IntoElem();

			xml.AddChildElemEx("MapCode", pMapInfo->MapCode);		//맵코드와 
			xml.AddChildElem("MapName", pszName);					//맵 이름 추가.

			xml.OutOfElem();
		}
	}

	m_mapOverlap.clear();

	////////////////////////////////
	// Skill Preset을 생성 "SkillInventoryStore.txt" 스크립트를 파싱함.
	// 썬 스튜디오 클라이언트팀 이지연씨 또는 유재영씨로부터 협조받음.
	/*	
	Class Type

	1 버서커
	2 드래곤
	3 섀도우
	4 발키리
	5 매지션
	7 드래곤(변신1차)
	8 드래곤(변신2차)

	*/
	// 미리 읽어들일 스킬 총 갯수
	int numInfos = SkillInventoryStoreParser::Instance()->GetSkillInventoryStoreNum();
	for (int i=0; i<numInfos; ++i)
	{
		const SkillInven_s * pSkillInven = SkillInventoryStoreParser::Instance()->GetSkillInventoryStore(i);

		
		if((0 == pSkillInven->bSkillTab1 && (pSkillInven->bSkillTab2 == 1 || pSkillInven->bSkillTab2 == 2)) 
			|| 1 == pSkillInven->bSkillTab1 || 2 == pSkillInven->bSkillTab1)
		{
			for (int i=0; i<pSkillInven->wSkillNum; ++i)
			{
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
                SLOTCODE skillCode = (SLOTCODE)pSkillInven->iSkillID[i];
#else
                SLOTCODE skillCode = (SLOTCODE)pSkillInven->dwSkillID[i];
#endif                
				LEVELTYPE wLevel;

				if(0 == pSkillInven->bSkillTab1 && pSkillInven->bSkillTab2 == 2)
				{					
					BASE_STYLEINFO* currStyleInfo = SkillInfoParser::Instance()->GetStyleInfo(skillCode);
					if(!currStyleInfo) continue;
					wLevel = currStyleInfo->m_wStyleLV;
				}
				else
				{
					SkillScriptInfo* currSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(skillCode);
					if(!currSkillInfo) continue;
					wLevel = currSkillInfo->m_wSkillLv;
				}
				


				SKILL_INFO _INFO = { pSkillInven->bSkillTab1, pSkillInven->bSkillTab2, wLevel, 0};
				if(wLevel > 0 )
				{
					skillCode = skillCode - wLevel;
					m_SkillPreset[pSkillInven->bClassID][skillCode] = _INFO;
				}
				else
				{
					m_BasicSkillPreset[pSkillInven->bClassID][skillCode] = _INFO;
				}	
			}
		}
		
	}
#if NATION_BUILD_VERSION < 1101
	{
		for(QUEST_PRESET::iterator iter = g_pQuestInfoManager->m_QuestPreset.begin(); iter != g_pQuestInfoManager->m_QuestPreset.end();
			iter++)
		{
			QCODE questCODE = iter->first;
			QUEST_INFO& questINFO = iter->second;
			//_LOG("QUEST [%d] %s", questCODE, questINFO.pszName);

			xml.AddChildElem(_T("QUEST_INFO"));
			xml.IntoElem();

			xml.AddChildElem(_T("Name"), questINFO.pszName);	
			xml.AddChildElem(_T("Category"), questINFO.pszCategoryName);

			xml.AddChildElemEx(_T("Code"), questCODE );
			//	xml.AddChildElemEx(_T("ExpireTime"), questINFO.m_ExpireTime );

			xml.OutOfElem();
		}
	}


#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    AccumulatePointParser* AccParser = AccumulatePointParser::Instance();
    for (int i = AccParser->DayFlag::Min_Day ; i < AccParser->DayFlag::Max_Day ; i++)
    {
        xml.AddChildElem(_T("ACCUMULATEPOINT_INFO"));
        xml.IntoElem();
        AccumulatePointParser::RewardGroup reward_group = AccParser->get_reward_group(i);

        xml.AddChildElemEx(_T("Day"), reward_group.day_flag);	
        xml.AddChildElemEx(_T("Request_Coin"), reward_group.require_point);	       
        xml.AddChildElemEx(_T("MileageCoinSet"), reward_group.generate_point);	
        xml.AddChildElemEx(_T("MileageTimeSet"), reward_group.generate_time);	

        for (int j = 0 ; j < AccumulatePointParser::RewardGroup::Max_Reward ; j++)
        {
            xml.AddChildElem(_T("REWARD_GROUP"));
            xml.IntoElem();
            xml.AddChildElemEx(_T("GroupCode"), reward_group.reward_array[j].reward_code);	
            xml.AddChildElemEx(_T("RewardRate"), reward_group.reward_array[j].reward_ratio);	
            xml.OutOfElem();
        }
        xml.OutOfElem();
    }    
#endif

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    STLX_MAP<WORD, WORD> location_code;

    AchievementParser* achievement_parser = AchievementParser::Instance();

    ACHIEVEMENTINFO_ITERATOR achievement_iter = achievement_parser->Begin();
    
    TCHAR locationname[256];

    for(;achievement_iter != achievement_parser->End() ; achievement_iter++)
    {
        
        Achievement_ParserInfo* parserinfo = *achievement_iter;
        if(location_code.find(parserinfo->location_code_) == location_code.end())
        {
            location_code[parserinfo->location_code_] = parserinfo->location_code_;
            xml.AddChildElem(_T("AC_LOCATION_INFO"));
            xml.IntoElem();
            xml.AddChildElemEx(_T("code"), parserinfo->location_code_);	
            GetStringByCode(parserinfo->location_code_, locationname, 256);	
            xml.AddChildElemEx(_T("name"), locationname);	
            xml.OutOfElem();
        }        
    }
#endif
#endif//#if NATION_BUILD_VERSION < 1101
	xml.Close();
	_LOG("<< CCSunOnline::createItem(void)");

	return true;
}

STDMETHODIMP CCSunOnline::createItem(void)
{
	if(!CreateXMLFile("getItemList.xml",false)) 
	{
		_LOG("getItemList.xml Createfile Fail !!!");
	}

	if(!CreateXMLFile("getRNItemList.xml",true)) 
	{
		_LOG("getRNItemList.xml Createfile Fail !!!");
	}

#if NATION_BUILD_VERSION >= 1101
    if(!CreateQuestfoFile())
    {
        _LOG("QuestfoFile Create Fail !!!");
    }
    if(!CreateACInfoFile())
    {
        _LOG("ACInfoFile Create Fail !!!");
    }
    if(!CreateSkillInfoFile())
    {
        _LOG("SkillInfoFile Create Fail !!!");
    }
#endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    if(!CreateHonorSystemInfoFile())
    {
        _LOG("TitleInfoFile Create Fail !!!");
    }
#endif
    if (CreateCurrencyInfo() == false)
    {
        _LOG("Fail to create currency info.");
    }
    if (CreateRandomKey() == false)
    {
        _LOG("Fail to create radnom key.");
    }
    if (CreateRandomItemInfo() == false)
    {
        _LOG("Fail to create random item info.");
    }
    if (CreateGuildFacilityInfo() == false) {
        _LOG("Fail to create Guild Facility info.");
    }
	return S_OK;
}


//STDMETHODIMP CCSunOnline::decodeCharStat(BYTE buff[BASE_PLAYERINFO_SIZE], BSTR* bstrXml)
STDMETHODIMP CCSunOnline::decodeCharStat(BYTE buff[sizeof(BASE_PLAYERINFO)], BSTR* bstrXml)

{
	
	_LOG(">> CCSunOnline::decodeCharStat()");

	if(!isInitialized())
		return S_FALSE;

	BASE_PLAYERINFO* pPlayer = (BASE_PLAYERINFO*)buff;

	if(!pPlayer)
	{
		_LOG("decodeCharStat fail !!!");
		return S_FALSE;
	}

	CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate |
		CMarkupArchive::ArchiveModeStoring |
		CMarkupArchive::ArchiveModeNotZipped);
	

	SetXMLCharSet(xml);

	xml.Open("C:\\WOPSConfig\\SUNGMS\\decodeCharStat.xml");
	
	xml.AddElem(_T("BASE_PLAYERINFO"));
	
	xml.AddChildElemEx("m_UserGuid",pPlayer->m_UserGuid);
	xml.AddChildElemEx("m_bySlot",pPlayer->m_bySlot);
	xml.AddChildElemEx("m_byClassCode",pPlayer->m_byClassCode);
	xml.AddChildElemEx("m_CharGuid",pPlayer->m_CharGuid);
	xml.AddChildElem("m_tszCharName",pPlayer->m_tszCharName);
	xml.AddChildElemEx("m_byHeight",pPlayer->m_byHeight);
	xml.AddChildElemEx("m_byFace",pPlayer->m_byFace);
	xml.AddChildElemEx("m_byHair",pPlayer->m_byHair);
	xml.AddChildElemEx("m_LV",pPlayer->m_LV);
	xml.AddChildElemEx("m_sStrength",pPlayer->m_sStrength);
	xml.AddChildElemEx("m_sDexterity",pPlayer->m_sDexterity);
	xml.AddChildElemEx("m_sVitality",pPlayer->m_sVitality);
	xml.AddChildElemEx("m_sInteligence",pPlayer->m_sInteligence);
	xml.AddChildElemEx("m_sSpirit",pPlayer->m_sSpirit);
	xml.AddChildElemEx("m_sSkillStat1",pPlayer->m_sSkillStat1);
	xml.AddChildElemEx("m_sSkillStat2",pPlayer->m_sSkillStat2);
	xml.AddChildElemEx("m_UserPoint",pPlayer->m_UserPoint);

	xml.AddChildElemEx("m_iExpForLog",pPlayer->m_iExpForLog);				// 로그 관리를 위해 어쩔 수 없이 남김

	
	xml.AddChildElemEx("m_iMaxHP",pPlayer->m_iMaxHP);
	xml.AddChildElemEx("m_iHP",pPlayer->m_iHP);
	xml.AddChildElemEx("m_iMaxMP",pPlayer->m_iMaxMP);
	xml.AddChildElemEx("m_iMP",pPlayer->m_iMP);

	xml.AddChildElemEx("m_Money", pPlayer->m_Money);
	xml.AddChildElemEx("m_iRemainStat",pPlayer->m_iRemainStat);			
	xml.AddChildElemEx("m_iRemainSkill",pPlayer->m_iRemainSkill);
	xml.AddChildElemEx("m_iSelectStyleCode",pPlayer->m_iSelectStyleCode);
	xml.AddChildElemEx("m_byPKState",pPlayer->m_byPKState);
	xml.AddChildElemEx("m_byCharState",pPlayer->m_byCharState);

	xml.AddChildElemEx("m_StateTime",pPlayer->m_StateTime);
	xml.AddChildElemEx("m_iRegion",pPlayer->m_iRegion);
	xml.AddChildElemEx("m_sLocationX",pPlayer->m_sLocationX);
	xml.AddChildElemEx("m_sLocationY",pPlayer->m_sLocationY);
	xml.AddChildElemEx("m_sLocationZ",pPlayer->m_sLocationZ);
	xml.AddChildElemEx("m_iPreRegion",pPlayer->m_iPreRegion);	
	xml.AddChildElem("m_tszTitleID",pPlayer->m_tszTitleID);

	
	xml.AddChildElemEx("m_TitleTime",pPlayer->m_TitleTime);
	xml.AddChildElemEx("m_iInventoryLock",pPlayer->m_iInventoryLock);
//	xml.AddChildElemEx("m_bInventoryLock",pPlayer->m_bInventoryLock); // 20070709

//	xml.AddChildElemEx("m_sCHuntingDisCount",pPlayer->	m_sCHuntingDisCount	);
//	xml.AddChildElemEx("m_sCHuntingMaxSeries",pPlayer->	m_sCHuntingMaxSeries	);
	xml.AddChildElemEx("m_ChaoState", pPlayer->m_ChaoState);	
	xml.AddChildElemEx("m_ChaoEndTime", pPlayer->m_ChaoEndTime);

	xml.AddChildElemEx("m_byChaoGrade", 0);
	xml.AddChildElemEx("m_ChaoTime", 0);
	
	xml.AddChildElemEx("m_PlayLimitedTime",pPlayer->m_PlayLimitedTime);

	xml.AddChildElemEx("m_iPVPPoint",pPlayer->m_iPVPPoint);
	xml.AddChildElemEx("m_iPVPScore",pPlayer->m_iPVPScore);
	xml.AddChildElemEx("m_byPVPGrade",pPlayer->m_byPVPGrade);
	xml.AddChildElemEx("m_iPVPMaxSeries",pPlayer->m_iPVPMaxSeries);
	xml.AddChildElemEx("m_iPVPMaxKill",pPlayer->m_iPVPMaxKill);
	xml.AddChildElemEx("m_iPVPMaxDie",pPlayer->m_iPVPMaxDie);
	xml.AddChildElemEx("m_iPVPTotalKill",pPlayer->m_iPVPTotalKill);
	xml.AddChildElemEx("m_iPVPTotalDie",pPlayer->m_iPVPTotalDie);
	xml.AddChildElemEx("m_iPVPTotalDraw",pPlayer->m_iPVPTotalDraw);
	xml.AddChildElemEx("m_byInvisibleOptFlag",pPlayer->m_byInvisibleOptFlag);
	xml.AddChildElemEx("m_GuildGuid",pPlayer->m_GuildGuid);
	xml.AddChildElemEx("m_GuildMarkIdx",pPlayer->m_GuildMarkIdx);
	xml.AddChildElem("m_tszGuildName",pPlayer->m_tszGuildName);
	xml.AddChildElemEx("m_GuildPosition",pPlayer->m_GuildPosition);
	xml.AddChildElem("m_tszGuildNickName",pPlayer->m_tszGuildNickName);

	xml.AddChildElemEx("m_iCHuntingDisCount",pPlayer->m_iCHuntingDisCount);
	xml.AddChildElemEx("m_iCHuntingVCount",pPlayer->m_iCHuntingVCount);
	xml.AddChildElemEx("m_iCHuntingLCount",pPlayer->m_iCHuntingLCount);
	xml.AddChildElemEx("m_iCHuntingTotCount",pPlayer->m_iCHuntingTotCount);
	xml.AddChildElemEx("m_Version",pPlayer->m_Version);

#ifdef _NA_002050_20110216_ADD_GENDER
    xml.AddChildElemEx("m_gender",pPlayer->gender);
#endif


	struct tm *newtime;
	__int64 ltime;
	char sTime[64];

	ltime = pPlayer->m_CreateTime;
	newtime = _gmtime64( &ltime );
	
	sprintf(sTime, "%04d-%02d-%02d %02d:%02d:%02d", newtime->tm_year + 1900, newtime->tm_mon, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

	xml.AddChildElem("m_CreateTime",sTime);	



/*
#ifdef __NA_000524_20070628_EXTEND_CASH_SLOT
	xml.AddChildElemEx("m_ExtraInventoryTabCount",pPlayer->m_ExtraInventoryTabCount);//BYTE		m_ExtraInventoryTabCount	: 6;
	xml.AddChildElemEx("m_ExtraEquipCashSlotCount",pPlayer->m_ExtraEquipCashSlotCount);//BYTE		m_ExtraEquipCashSlotCount	: 2;
#else
	xml.AddChildElemEx("m_ExtraInventoryTabCount",pPlayer->m_ExtraInventoryTabCount);	
#endif
*/
	xml.AddChildElemEx("m_ExtraInventoryTabCount",pPlayer->m_ExtraInventoryTabCount);//BYTE		m_ExtraInventoryTabCount	: 6;
	xml.AddChildElemEx("m_ExtraEquipCashSlotCount",pPlayer->m_ExtraEquipCashSlotCount);//BYTE		m_ExtraEquipCashSlotCount	: 2;

	xml.AddChildElem("m_InventoryPwd",pPlayer->m_InventoryPwd);

	xml.AddChildElemEx("m_byFreeInitCount",pPlayer->m_byFreeInitCount);

	
	xml.AddChildElemEx("m_eGuildDuty",pPlayer->m_eGuildDuty);

	xml.AddChildElemEx("m_i64Exp",pPlayer->m_i64Exp);

#ifdef __NA_000518_20070622_WHISPER_TRADE_INTERCEPTION
	xml.AddChildElemEx("m_bWhisperFlag",pPlayer->m_bWhisperFlag);
	xml.AddChildElemEx("m_bTradeFlag",pPlayer->m_bTradeFlag);
#endif

#ifdef __NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY
	xml.AddChildElemEx("m_ePenaltyType",pPlayer->m_ePenaltyType);
	xml.AddChildElemEx("m_GuildPenaltyTime",pPlayer->m_GuildPenaltyTime);
#endif

#ifdef __NA_0_PET_SAVE_TO_DB
	xml.AddChildElemEx("m_bSummonedPet",pPlayer->m_bSummonedPet);
	xml.AddChildElemEx("m_bMAXIntimacy",pPlayer->m_bMAXIntimacy);
	xml.AddChildElemEx("m_PetItemPos",pPlayer->m_PetItemPos);
	//xml.AddChildElemEx("m_bMAXOptionRemainTime",pPlayer->m_bMAXOptionRemainTime);
#endif
	xml.AddChildElemEx("m_bMAXOptionRemainTime",pPlayer->m_bMAXOptionRemainTime); //GMS 사용중
	
	xml.OutOfElem();

	*bstrXml = _bstr_t(xml.GetDoc().c_str() ).copy();	
	
	
	xml.Close();
	

	_LOG("<< CCSunOnline::decodeCharStat()");
	
	return S_OK;
}

// T_Quest.ProgrQStream, T_Quest.ComplQStream 바이너리 파싱
STDMETHODIMP CCSunOnline::decodeQuest( BYTE ProgrQStream[MAX_PROGR_QUESTSTREAM_SIZE], 
										BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], 										
										BSTR* bstrXml)
{
	if(!isInitialized())
		return S_FALSE;

	_LOG(">> CCSunOnline::decodeQuest()");

	//QUESTPART_PROGR * pProgrQStream = reinterpret_cast<QUESTPART_PROGR *>(ProgrQStream);
	//QUESTPART_COMPL * pComplQStream = reinterpret_cast<QUESTPART_COMPL *>(ComplQStream);

	uiQuestListContent::DecompileQuestInfo(ProgrQStream, ComplQStream, bstrXml);

	_LOG("<< CCSunOnline::decodeQuest()");

	return S_OK;
}

// 
STDMETHODIMP CCSunOnline::encodeQuest( BSTR bstrXml, BYTE ProgrQStream[MAX_PROGR_QUESTSTREAM_SIZE], BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], int* nResult)
{
	if(!isInitialized())
		return S_FALSE;
	uiQuestListContent QuestList;
    DWORD ret = QuestList.CompileQuestStream(bstrXml, ProgrQStream, ComplQStream);
	if(GM_SUCCEEDED != ret)
    {
        *nResult = ret;
        return S_FALSE;
    }
	return S_OK;
}


// T_Character.ACStream 파싱
#ifdef __NA_1288_AC_RENEWAL_FOR_TIME_LINE
STDMETHODIMP  CCSunOnline::decodeACStream( BYTE ACStream[MAX_ACSTREAM_SIZE], int Version, BYTE Mission[MAX_MISSIONSTREAM_SIZE], BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], BSTR* bstrXml)
#else
STDMETHODIMP  CCSunOnline::decodeACStream( BYTE ACStream[MAX_ACSTREAM_SIZE], BYTE Mission[MAX_MISSIONSTREAM_SIZE], BYTE ComplQStream[MAX_COMPL_QUESTSTREAM_SIZE], BSTR* bstrXml)
#endif
{

#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED
    
	_LOG(">> CCSunOnline::decodeACStream()");
	if(!isInitialized())
		return S_FALSE;

	BYTE* ACStreamBuf;

#ifdef __NA_1288_AC_RENEWAL_FOR_TIME_LINE
	//static BOOL ACStreamConverter( BYTE ACSectionNum, int iFromVersion IN, BYTE * IN pFromStream IN, int & iToVersion OUT, BYTE * pToStream OUT );
	BYTE NewACStream[MAX_ACSTREAM_SIZE];
	int NewVersion = 0;

	AreaConquest::ACStreamConverter(MAX_AC_SECTION_NUM , Version, ACStream, NewVersion, NewACStream);

	ACStreamBuf = NewACStream;
#else
	ACStreamBuf = ACStream;
#endif

	AreaConquestManager ACMgr;
	ACMgr.Init();

	// DB에서 가져온 스트림 정보 셋팅
	ACMgr.Serialize( ACStreamBuf, MAX_ACSTREAM_SIZE, ACSERIALIZE_SAVE );
//	ACMgr.ApplyMissionData( Mission, MAX_MISSION_NUM*4);
//	ACMgr.ApplyQuestData( ComplQStream, MAX_COMPL_QUESTSTREAM_SIZE);

	QuestManager2* pQuestMgr = new QuestManager2(g_pQuestInfoManager);
	pQuestMgr->SerializeComplStream( ComplQStream, MAX_COMPL_QUESTSTREAM_SIZE, QSERIALIZE_SAVE );
	ACMgr.UpdateQuestInfo(pQuestMgr, TRUE);
	delete pQuestMgr;

	MissionManager missionMgr;
	missionMgr.SerializeStream( Mission, MAX_MISSIONSTREAM_SIZE, SERIALIZE_STORE);
	ACMgr.UpdateMissionInfo(&missionMgr, TRUE);

	ACMgr.UpdateAll(bstrXml);
	_LOG("<< CCSunOnline::decodeACStream()");
#endif
	return S_OK;
}

STDMETHODIMP  CCSunOnline::encodeACStream( BSTR bstrXml, BYTE ACStream[MAX_ACSTREAM_SIZE], int* nResult)
{

#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED
    
	_LOG(">> CCSunOnline::encodeACStream()");
	if(!isInitialized())
		return S_FALSE;
	// 보상받았다고 셋팅 : 보상 확률에 도달 안했는데, 보상을 줄 수 없음
//	AreaConquestInfo * pInfo = m_Manager. GetAreaConquestInfo(SectionIndex);
//	// byRewardIndex : 보상 받고자 하는 AC의 보상 인덱스
//	BASE_AreaConquestInfo * pINFO = pInfo.GetBASEAreaConquestInfo();
//	if( CalculateCurrentAchievePoint()*100 < pINFO->m_wAch_Per[byRewardIndex] )
//		return RC::RC_AC_INSUFFICIENT_POINT;
//	DWORD dwRewardIndex = pInfo->m_wAch_Rew[byRewardIndex];
//	pInfo.SetReward( byRewardIndex, true );
//
//	// 보상확률 높이기
//	// ConditionIndex : 높이기 원하는 항목의 인덱스
//	// ConditionIndex : 아이템 스크립트의 AC항목의 인덱스
//	AreaConquestInfo * pInfo = m_Manager. GetAreaConquestInfo(SectionIndex);
//	pInfo->IncConditionValue(ConditionIndex);
//
//	// 수정된 스트림 정보 가져 오기
//	m_Manager.Serialzie( m_pACStream, MAX_ACSTREAM_SIZE, ACSERIALIZE_LOAD );

	AreaConquestManager ACMgr;
	ACMgr.Init();
	ACMgr.CompileACStream(bstrXml);
	ACMgr.Serialize(ACStream, MAX_ACSTREAM_SIZE, ACSERIALIZE_LOAD);

//	WriteToFile("C:\\WOPSConfig\\SUNGMS\\ACdump.dat", ACStream, MAX_ACSTREAM_SIZE);
	*nResult = S_OK;
	_LOG("<< CCSunOnline::encodeACStream()");
#endif
	return S_OK;
}

void CCSunOnline::GetItemChargeExpireTime(IN BYTE byChargeSubType, IN SCItemSlot& slot, CMarkupArchive& xml, OUT char* pszTime)
{   
	switch(byChargeSubType)
	{
	case eCHARGE_SUB_FIXED_AMOUNT:
		{
			// 시간을 셋팅(초단위)
			// if it will be 30일 기한 아이템
			//slot.SetDateTime( 30*24*60*60 );
			// 날짜를 사용
			SYSTEMTIME systime;
			slot.GetDateTimeToSYSTEMTIME(systime);
			sprintf(pszTime, "%04d%02d%02d%02d%02d%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
		//	sprintf(pszTime, "%I64d", slot.GetDateTime() );
			_LOG("** eCHARGE_SUB_FIXED_AMOUNT : %s | %I64d", pszTime, slot.GetDateTime());
		}
		break;
	case eCHARGE_SUB_FIXED_AMOUNT_EX:
#ifdef __NA001164_20081024_CHAR_WAY_POINT
	case eCHARGE_SUB_COORDINATE_EX:
#endif
		{
			// 시간 셋팅(초단위)
			// 다만 초단위의 숫자는 무시됨!(시간단위로 계산됨)
			// if it will be 30시간 이용 아이템
			//slot.SetDateTime( 30*60*60 );

			// 날짜를 사용
			if( slot.IsProgressTime() )
			{
				// 20061107240023
			//	SYSTEMTIME systime;
			//	slot.GetDateTimeToSYSTEMTIME(systime);
			//	sprintf(pszTime, "%04d%02d%02d%02d%02d%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
				sprintf(pszTime, "%I64d", slot.GetDateTime() );
			}
			else
			{				
			//	int remain_hours = slot.GetDateTime()/(60*60);
			//	sprintf(pszTime, "%d", remain_hours);
				sprintf(pszTime, "%I64d", slot.GetDateTime() );
			}

			_LOG("eCHARGE_SUB_FIXED_AMOUNT_EX : %s", pszTime);
		}
		break;
	case eCHARGE_SUB_FIXED_QUANTITY:
		{
			// 사용 시간 입력(초단위로 입력)
			// if it will be 30시간 이용 아이템
		//	slot.SetDateTime( 30*60*60 );

			// 남은 사용 시간
		//	int remain_hours = slot.GetDateTime()/(60*60);			
		//	sprintf(pszTime, "%d", remain_hours);
			sprintf(pszTime, "%I64d", slot.GetDateTime() );
			_LOG("eCHARGE_SUB_FIXED_QUANTITY : %s", pszTime);
		}
		break;
	case eCHARGE_SUB_COORDINATE:
		{
			xml.AddChildElemEx(_T("ItemMapCode"), slot.GetMapCode() );
			xml.AddChildElemEx(_T("ItemCoorX"), slot.GetX() );
			xml.AddChildElemEx(_T("ItemCoorY"), slot.GetY() );
			xml.AddChildElemEx(_T("ItemCoorZ"), slot.GetZ() );
			

//			// 좌표 설정
//			slot.SetMapCode(10001);
//			slot.SetX(13);
//			slot.SetY(145);
//			slot.SetZ(21);
//			// 좌표 이용
//			slot.GetMapCode();
//			slot.GetX();
//			slot.GetY();
//			slot.GetZ();
		}
		break;
	case eCHARGE_SUB_FATE:		
		break;
#ifdef __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    case eCHARGE_SUB_RIDER_WITH_PART:
        {
            /*SYSTEMTIME systime;
            slot.GetDateTimeToSYSTEMTIME(systime);
            sprintf(pszTime, "%04d%02d%02d%02d%02d%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);*/
            sprintf(pszTime, "%I64d", slot.GetDateTime() );            
        }
        break;
    case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:        
        {            
            sprintf(pszTime, "%I64d", slot.GetDateTime() );            
        }
        break;
#endif
	default:
		sprintf(pszTime, "%d", 0);
		break;
	}
	//int a = eCHARGE_SUB_FATE;
}

void CCSunOnline::SetItemChargeExpireTime(IN BYTE byChargeSubType, IN SCItemSlot& slot, CMarkupArchive& xml, IN const char* pszTime)
{

	if(!pszTime)
		return;

	switch(byChargeSubType)
	{
	case eCHARGE_SUB_FIXED_AMOUNT:
		{
			DWORD64 dt = static_cast<DWORD64>(_atoi64(pszTime) );
			
		//	sprintf(pszTime, "%I64d", slot.GetDateTime() );

			SYSTEMTIME systime;
			util::YYYYMMDDHHMMSSToSYSTEMTIME( dt, systime );
			char pszTmp[256] = {0};
			sprintf(pszTmp, "%04d%02d%02d%02d%02d%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);

			// 시간을 셋팅(초단위)
			if(0 == stricmp(pszTmp, pszTime))
			{
				slot.SetFixedDateTime(dt);
			}
			else
				slot.SetDateTime(dt);
		}
		break;
	case eCHARGE_SUB_FIXED_AMOUNT_EX:
#ifdef __NA001164_20081024_CHAR_WAY_POINT
	case eCHARGE_SUB_COORDINATE_EX:
#endif
		{
			
			// 시간 셋팅(초단위)
			// 다만 초단위의 숫자는 무시됨!(시간단위로 계산됨)
			// if it will be 30시간 이용 아이템
			DWORD64 dt = static_cast<DWORD64>(_atoi64(pszTime) );

			//slot.SetDateTime( dt );
			SYSTEMTIME systime;
			util::YYYYMMDDHHMMSSToSYSTEMTIME( dt, systime );
			char pszTmp[256] = {0};
			sprintf(pszTmp, "%04d%02d%02d%02d%02d%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);

			// 시간을 셋팅(초단위)
			if(0 == stricmp(pszTmp, pszTime))
			{
				slot.SetFixedDateTime(dt);
			}
			else
				slot.SetDateTime(dt);
		}
		break;
	case eCHARGE_SUB_FIXED_QUANTITY:
		{
			// 사용 시간 입력(초단위로 입력)
			// if it will be 30시간 이용 아이템
			DWORD64 dt = static_cast<DWORD64>(_atoi64(pszTime) );
			slot.SetDateTime( dt );
		}
		break;
	case eCHARGE_SUB_COORDINATE:
		{

			WORD wMapCode;
			SHORT sX, sY, sZ;

			xml.FindGetChildData(_T("ItemMapCode"), wMapCode);
			xml.FindGetChildData(_T("ItemCoorX"), sX);
			xml.FindGetChildData(_T("ItemCoorY"), sY);
			xml.FindGetChildData(_T("ItemCoorZ"), sZ);
			
			slot.SetMapCode(wMapCode);
			slot.SetX(sX);
			slot.SetY(sY);
			slot.SetZ(sZ);

			

			//			// 좌표 설정
			//			slot.SetMapCode(10001);
			//			slot.SetX(13);
			//			slot.SetY(145);
			//			slot.SetZ(21);
			//			// 좌표 이용
			//			slot.GetMapCode();
			//			slot.GetX();
			//			slot.GetY();
			//			slot.GetZ();
		}
		break;
	case eCHARGE_SUB_FATE:		
		break;
#ifdef __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    case eCHARGE_SUB_RIDER_WITH_PART:
        {   
            DWORD64 dt = static_cast<DWORD64>(_atoi64(pszTime) );
            
            if(slot.IsRiderItem() && dt < 20000000000000)
            {
                time_t now;
                ::time(&now);
                tm* local_time = ::localtime(&now);     
                
                local_time->tm_sec += (int)dt;
                if (mktime(local_time) != (time_t)-1)
                {
                    util::GetDateTime_YYYYMMDDHHMMSS(local_time->tm_year + 1900,
                        local_time->tm_mon + 1,
                        local_time->tm_mday,
                        local_time->tm_hour,
                        local_time->tm_min,
                        local_time->tm_sec,
                        dt);                    
                }
            }                                    
            slot.SetFixedDateTime(dt);
            
        }        
        break;
    case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
        {
            DWORD64 dt = static_cast<DWORD64>(_atoi64(pszTime) );
            slot.SetFixedDateTime(dt);
        }
        break;
#endif

	}
}


// 바이너리에 있는 Limited 속성을 XML 에 추가한다.
void CCSunOnline::WriteLimited(SCItemSlot& slot, CMarkupArchive& xml, int iCurrentVer)
{

	if(iCurrentVer >= 3) //바이너리 버전이 3이상일때만 Limited 속성이 들어간다.
	{	
		TCHAR pszLimited[256];
		ZeroMemory(pszLimited,256);

		if(slot.IsLimited())
		{			
			wsprintf(pszLimited, "[%u:%d:%d:%d:%d:%d]", 
				slot.GetLimitedIndex(),
				slot.GetDamageRatio(),
				slot.GetLimitValueRatio(),
				slot.GetLimitedDuraMax(),
				slot.GetLimitedDuraMMax(),
				slot.GetLimitedRepairCount());			
		}
		xml.AddChildElem("Limited", pszLimited);
	}


}

// 리미티드 속성을 읽어서 바이너리에 넣는다.
void CCSunOnline::ReadLimited(SCItemSlot& slot, CMarkupArchive& xml)
{
	//LimitedItemInfoParser


	std::string strLimited;
	if(xml.FindGetChildData("Limited", strLimited))
	{
//		std::string strLimited = "[1234:56:78:9:0:1]";
		if(strLimited.empty()) return; 

		int nFind = 0;

		nFind = (int)strLimited.find("[");
		if(nFind > -1) strLimited.erase(nFind,1);
		nFind = (int)strLimited.find("]");
		if(nFind > -1) strLimited.erase(nFind,1);

        enum { kBufSize = 256 };
		TCHAR szBuff[kBufSize];
		_tcsncpy(szBuff, strLimited.c_str(), _countof(szBuff));

		slot.SetLimitedIndex(_ttoi(_tcstok(szBuff,":")));        
        slot.CalLimited(); //리미티드 아이템 여부 설정 2009-11-17 youngmoon
		slot.SetDamageRatio(_ttoi(_tcstok(NULL,":")));
		slot.SetLimitValueRatio(_ttoi(_tcstok(NULL,":")));
		slot.SetLimitedDuraMax(_ttoi(_tcstok(NULL,":")));
		slot.SetLimitedDuraMMax(_ttoi(_tcstok(NULL,":")));
		slot.SetLimitedRepairCount(_ttoi(_tcstok(NULL,":")));	        
	}


}

// 바이너리에 있는 Fate 속성을 XML 에 추가한다.
void CCSunOnline::WriteFate(SCItemSlot& slot, CMarkupArchive& xml, OUT char* pszTime)
{
	
	
		
		sprintf(pszTime, "%I64d", slot.GetDateTime());

		TCHAR pszLucky[256];
		ZeroMemory(pszLucky,256);

		if(slot.IsFateItem())
		{			
			wsprintf(pszLucky, "[%u:%d:%d:%u]", 
				slot.GetFateAtkDefRatio(),
				(int)slot.GetFateIdentify(),
				slot.GetFateDuraMax(),
				slot.GetFatePeriod());			
		}
		xml.AddChildElem("Lucky", pszLucky);
	

}

// Fate 속성을 읽어서 바이너리에 넣는다.
void CCSunOnline::ReadFate(SCItemSlot& slot, CMarkupArchive& xml, IN const char* pszTime)
{		
	

	std::string strSubType;
	if(xml.FindGetChildData("SubType", strSubType))
	{
		if(11 == atoi(strSubType.c_str()))
		{
			
			DWORD64 dt = static_cast<DWORD64>(_atoi64(pszTime) );

			//	sprintf(pszTime, "%I64d", slot.GetDateTime() );

			SYSTEMTIME systime;
			util::YYYYMMDDHHMMSSToSYSTEMTIME( dt, systime );
			char pszTmp[256] = {0};
			sprintf(pszTmp, "%04d%02d%02d%02d%02d%02d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);

			// 시간을 셋팅(초단위)
			if(0 == stricmp(pszTmp, pszTime))
			{
			slot.SetFixedDateTime(dt);
			}
			else
				slot.SetDateTime(dt);
			/*
			DWORD64 dt = static_cast<DWORD64>(_atoi64(pszTime) );
			slot.SetDateTime( dt );
			*/
			std::string strLucky;
			if(xml.FindGetChildData("Lucky", strLucky))
			{				
				if(strLucky.empty()) return; 

				int nFind = 0;

				nFind = (int)strLucky.find("[");
				if(nFind > -1) strLucky.erase(nFind,1);
				nFind = (int)strLucky.find("]");
				if(nFind > -1) strLucky.erase(nFind,1);

                enum { kBufSize = 256 };
				TCHAR szBuff[kBufSize];
				_tcsncpy(szBuff, strLucky.c_str(), _countof(szBuff));

				slot.SetFateAtkDefRatio(_ttoi(_tcstok(szBuff,":")));
				slot.SetFateIdentify(_ttoi(_tcstok(NULL,":")));
				slot.SetFateDuraMax(_ttoi(_tcstok(NULL,":")));
				slot.SetFatePeriod(_ttoi(_tcstok(NULL,":")));
			}
		}		
	}	

}

#ifdef __NA000584_20080201_GUILD_WAREHOUSE
STDMETHODIMP CCSunOnline::decodeGuildWarehouseItem( BYTE buff[MAX_GUILD_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], int iCurrentVer, BSTR bstrDesc, BSTR* bstrXml)
{
	return decodeWareHouse(buff, MAX_GUILD_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX), MAX_GUILD_WAREHOUSE_SLOT_NUM,SI_GUILDWAREHOUSE, iCurrentVer, bstrDesc, bstrXml, "decodeGuildWarehouseItem", "GuildWarehouse");	
}

STDMETHODIMP CCSunOnline::encodeGuildWarehouseItem(BSTR bstrXml, BYTE buff[MAX_GUILD_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX)], int* iNewVer, int* nResult)
{
	return encodeWareHouse(bstrXml, buff, MAX_GUILD_WAREHOUSE_SLOT_NUM*sizeof(ITEMSTREAMEX), MAX_GUILD_WAREHOUSE_SLOT_NUM, SI_GUILDWAREHOUSE, iNewVer, nResult, "encodeGuildWarehouseItem", "GuildWarehouse");
}
#endif


//2008-05-26 문상현 펫아이템 처리 추가
void CCSunOnline::encodePetItemInfo(SCItemSlot& slot, CMarkupArchive& xml)
{
#ifdef __NA_0_PET_SYSTEM
	DWORD dwTemp = 0;
	xml.FindGetChildData(_T("IsPetItem"), dwTemp);
	
	if(1 == dwTemp)
	{
		int byTemp = 0;
		SCPetInfo* petinfo = slot.GetPetInfo();

		if(petinfo)
		{
			xml.FindGetChildData(_T("Fullness"), byTemp);//포만감 
			petinfo->SetFullness(byTemp);
			xml.FindGetChildData(_T("Initimacy"), byTemp);//친밀도
			petinfo->SetIntimacy(byTemp);
			xml.FindGetChildData(_T("IsPetName"), byTemp);//
			if(1 == byTemp)
			{
				TCHAR szName[32];
				std::string strName;
				xml.FindGetChildData(_T("PetName"), strName);//
				petinfo->SetPetName(const_cast<char*>(strName.c_str()));
			}	
            xml.FindGetChildData(_T("IsPetLocked"), byTemp);//소환여부
            slot.SetUserBlocked(byTemp == 1 ? TRUE : FALSE);              

		}
		else
		{
			_LOG("encodePetItemInfo GetPetInfo fail");
		}				
	}	
#endif	
}

//2008-05-26 문상현 펫아이템 처리 추가
void CCSunOnline::decodePetItemInfo(SCItemSlot& slot, CMarkupArchive& xml)
{
#ifdef __NA_0_PET_SYSTEM
	if(slot.IsPetItem())
	{
		xml.AddChildElemEx(_T("IsPetItem"), 1);

		SCPetInfo* petinfo = slot.GetPetInfo();

		if(petinfo)
		{
			xml.AddChildElemEx(_T("Fullness"), petinfo->GetFullness());
			xml.AddChildElemEx(_T("Initimacy"), petinfo->GetIntimacy());
			xml.AddChildElemEx(_T("IsPetName"), petinfo->IsSetPetName() ? 1 : 0);

            //펫 네임이 깨졋을때 임의로 펫 네임 설정을 해준다.
            TCHAR szName[MAX_PET_NAME_LENGTH + 1];
            if(IsVertyPetName(const_cast<char*>(petinfo->GetPetName())))
            {                
                StringCchCopy(szName, MAX_PET_NAME_LENGTH + 1, petinfo->GetPetName());
            }
            else
            {
                StringCchCopy(szName, MAX_PET_NAME_LENGTH + 1, "NameFail");
            }	
            xml.AddChildElemEx(_T("PetName"), szName);
			xml.AddChildElemEx(_T("IsPetLocked"), slot.IsPetLocked() ? 1 : 0);
			//현재 소환된 펫 표시 (소환펫:youngmoon)
		}
		else
		{
			_LOG("decodePetItemInfo GetPetInfo fail");
		}
	}
	else
	{
		xml.AddChildElemEx(_T("IsPetItem"), 0);		
	}
#endif
}


//2008-05-26 문상현 에테르 아이템 정보 추가
void CCSunOnline::encodeEtherItemInfo(SCItemSlot& slot, CMarkupArchive& xml)
{
#ifdef __NA_000968_ETHER_EMISSION_DEVICE	
	DWORD dwTemp = 0;
	xml.FindGetChildData(_T("IsEquipEtherDevice"), dwTemp);
	slot.SetEquipEtherDevice((BOOL)dwTemp);

	BYTE byTemp = 0;
	xml.FindGetChildData(_T("EtherBulletIndex"), byTemp);//무기에 탄환 장착 
	slot.SetEtherBulletIndex(byTemp);

#endif
}



//2008-05-26 문상현 에테르 아이템 정보 추가
void CCSunOnline::decodeEtherItemInfo(SCItemSlot& slot, CMarkupArchive& xml)
{
#ifdef __NA_000968_ETHER_EMISSION_DEVICE		
	xml.AddChildElemEx(_T("IsEquipEtherDevice"), slot.equip_info_.is_equip_ether_device_);
	xml.AddChildElemEx(_T("EtherBulletIndex"), slot.GetEtherBulletIndex());
#endif
}


//2009-11-18 문상현 라이트 아이템 
void CCSunOnline::encodeRideItem(SCItemSlot& slot, CMarkupArchive& xml)
{
#ifdef __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    DWORD dwTemp = 0;
    xml.FindGetChildData(_T("IsRideItem"), dwTemp);

    if(1 == dwTemp)
    {
        xml.FindGetChildData(_T("IsRideLocked"), dwTemp);
        slot.SetUserBlocked(1 == dwTemp ? TRUE : FALSE);

        while(TRUE)
        {
            if(!xml.FindChildElem(_T("RidePartItem"))) break;

            xml.IntoElem();

            xml.FindGetChildData(_T("Code"), dwTemp);
            if(0 != dwTemp)
            {
                SCItemSlot Partsslot;
                Partsslot.SetCode(dwTemp);

                xml.FindGetChildData(_T("Type"), dwTemp);
                Partsslot.SetPos(dwTemp);
                xml.FindGetChildData(_T("Enchant"), dwTemp);
                Partsslot.SetEnchant((BYTE)dwTemp);
                xml.FindGetChildData(_T("ChargeOnUse"), dwTemp);
                Partsslot.SetProgressTime(1 == dwTemp ? TRUE : FALSE);
                xml.FindGetChildData(_T("IsExpiredWaitTime"), dwTemp);
                Partsslot.SetExpiredWaitTime(1 == dwTemp ? TRUE : FALSE);
                
                xml.FindGetChildData(_T("ChargeSubType"), dwTemp);        
                std::string sTemp;                
                xml.FindGetChildData(_T("ChargeExpireTime"), sTemp);
                SetItemChargeExpireTime( dwTemp, Partsslot, xml, sTemp.c_str() );                
                
                bool b = slot.EquipRiderParts(Partsslot);
                
            }

            xml.OutOfElem();
        }
        
    }    
#endif
}

//2009-11-18 문상현 라이트 아이템 
void CCSunOnline::decodeRideItem(SCItemSlot& slot, CMarkupArchive& xml)
{
#ifdef __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    const BASE_ITEMINFO * pItemInfo = slot.GetItemInfo();
    if(!pItemInfo)
    {        
        return ;
    }

    if(pItemInfo->IsRiderItem())
    {
        xml.AddChildElemEx(_T("IsRideItem"), 1);
        //slot.GetDateTime();
        //slot.IsExpiredWaitTime(); 
        xml.AddChildElemEx(_T("IsRideLocked"), slot.IsUserBlocked() ? 1 : 0);

        for(int PartIndex = eRIDER_PARTS_POS::StartIndex ; PartIndex < eRIDER_PARTS_POS::EndIndex ; PartIndex++)
        {
            eRIDER_PARTS_POS PartPos;
            PartPos.value = PartIndex;

            SCItemSlot Partsslot;

            if(slot.GetRiderPartsItem(PartPos, &Partsslot))
            {
                xml.AddChildElem(_T("RidePartItem"));
                xml.IntoElem();

                xml.AddChildElemEx("WorldSerial", slot.GetWorldSerial());
                xml.AddChildElemEx("Type", PartIndex);//머리, 몸통, 다리
                xml.AddChildElemEx("Code", Partsslot.GetCode());

                if( 0 == Partsslot.GetCode()) //파츠 아이템 없음
                {

                }
                else
                {   
                    xml.AddChildElemEx("Enchant", Partsslot.GetEnchant()); //왁스 효과.
                    xml.AddChildElemEx("ChargeOnUse", Partsslot.IsProgressTime()); //사용 시작 했나 ?
                    xml.AddChildElemEx("IsExpiredWaitTime", Partsslot.IsExpiredWaitTime());//기간 만료 됐나 ?                    
                    xml.AddChildElemEx("ChargeSubType", Partsslot.GetItemInfo()->m_ChargeSubType);//기간 만료 됐나 ?                    
                    char pszTime[32] = {0};
                    GetItemChargeExpireTime(Partsslot.GetItemInfo()->m_ChargeSubType, Partsslot, xml, pszTime);                    
                    xml.AddChildElem("ChargeExpireTime", pszTime);                    
                }                
                xml.OutOfElem();
            }
        }
        /*slot.EquipRiderParts();
        slot.UnEquipRiderParts();
        pItemInfo->IsCanEquip()*/
    }
    else
    {
        xml.AddChildElemEx(_T("IsRideItem"), 0);
    }
#endif
}


int CCSunOnline::GetSkillType(BYTE bSkillTab1, BYTE bSkillTab2)
{
	switch(bSkillTab1)
	{
	case 1: return 0; break;
	case 2: return 1; break;
	case 0:
		{
			switch(bSkillTab2)
			{			
			case 1: return 2; break;
			case 2: return 3; break;
			}
			break;
		}		
	}
	/*
	BYTE bSkillTab1;   // 0 패시브/스타일 1 숙련도1 2 숙련도2
	BYTE bSkillTab2;   // 0-1 패시브스킬 0-2 스타일 1-1 스킬 2-1 스킬
	*/
	return 0;

	/*
	0: 숙련도1
	1: 숙련도2
	2: 패시브
	3: 스타일
	*/
}



STDMETHODIMP CCSunOnline::GetItemStreamVersion(int* Version)
{
    *Version = ITEM_STREAM_SIZE_LAST_VERSION;

    return S_OK;
}

#ifdef _NA_008323_20150602_MODIFY_ITEM_SERIALNUMBER_40BITDATA
STDMETHODIMP CCSunOnline::GetWorldSerial(BYTE DBType, INT64 Serial, BSTR* WorldSerial)
{	
    ITEMPART_VER3 STItem;

    STItem.m_WorldSerial = 0;

    STItem.byType = DBType;
    STItem.dwSerial = Serial;
    STItem.byReserved2 = 0;
    STItem.byReserved1 = 0;

    char szWorldSerial[128];
    sprintf(szWorldSerial,_T("%I64u"),STItem.m_WorldSerial);

    *WorldSerial = _bstr_t(szWorldSerial).copy();

    return S_OK;
}
#else
STDMETHODIMP CCSunOnline::GetWorldSerial(BYTE DBType, ULONG Serial, BSTR* WorldSerial)
{	
    ITEMPART_VER3 STItem;

    STItem.m_WorldSerial = 0;

    STItem.byType = DBType;
    STItem.dwSerial = Serial;	
    STItem.byReserved2 = 0;
    STItem.byReserved1 = 0;

    char szWorldSerial[128];
    sprintf(szWorldSerial,_T("%I64u"),STItem.m_WorldSerial);

    *WorldSerial = _bstr_t(szWorldSerial).copy();

    return S_OK;
}
#endif //_NA_008323_20150602_MODIFY_ITEM_SERIALNUMBER_40BITDATA



void CCSunOnline::DecodeItem(SCItemSlot& slot, CMarkupArchive& xml, POSTYPE pos, int iCurrentVer)
{


    
	const BASE_ITEMINFO * pItemInfo = slot.GetItemInfo();
	if(!pItemInfo)
	{
		//		_LOG("			(1) can't retrieve item information.");
		return ;
	}

    //아이템 구분자를 넣는다.
    xml.AddChildElem("ITEM");
    xml.IntoElem();

    /*if(slot.GetWorldSerial() == 1100049100279)
    {
        int n = 0 ;
    }*/
    

    WriteLimited(slot, xml, iCurrentVer); //리미티드 속성을 추가한다.
	decodeEtherItemInfo(slot,xml);	//에테르 아이템 처리
	decodePetItemInfo(slot,xml);	//펫 아이템 처리
    decodeRideItem(slot,xml);       //라이드 아이템 처리

	xml.AddChildElemEx("Pos", pos);		//인벤상 위치.
	xml.AddChildElemEx("Enchant", slot.GetEnchant());		//인첸트옵션정보

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    xml.AddChildElemEx("Awakening", slot.GetAwakening());	//각성정보
#else
    xml.AddChildElemEx("Awakening", 0);	//1404이하버전에서 WOPS와 호환성을 고려해 추가된 코드
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

	string sItemName = "";
	TCHAR* pszItemName = STRTBL_PARSER1->GetStringByCode(pItemInfo->m_NCode);
	if(pszItemName != NULL)
		sItemName = pszItemName;

	if(pItemInfo->m_SCNCode != 0)	//두번째 네임코드
	{
		TCHAR* pszSecItemName = STRTBL_PARSER1->GetStringByCode(pItemInfo->m_SCNCode);
        if(pszSecItemName)
		    sItemName += pszSecItemName;
	}

	xml.AddChildElem("Name", sItemName.c_str());		//아이템명
	xml.AddChildElemEx("Code", slot.GetCode());			//코드번호		    

	xml.AddChildElemEx(_T("Type"), pItemInfo->m_wType);		//타입
	xml.AddChildElemEx(_T("SubType"), pItemInfo->m_wSubType);		//서브타입
	xml.AddChildElemEx(_T("ChargeType"), pItemInfo->m_ChargeType);		//유료화타입
	xml.AddChildElemEx(_T("ChargeSubType"), pItemInfo->m_ChargeSubType);		//유료화서브타입

#ifdef __NA_1247_POSSESSION_ITEM
	if(slot.IsPossessionItem())
	{
		xml.AddChildElemEx(_T("IsPossessionType"), 1);
		xml.AddChildElemEx(_T("IsPossession"), slot.GetIdentifyPossession());
	}
	else
	{
		xml.AddChildElemEx(_T("IsPossessionType"), 0);
	}
#endif

#ifdef _NA_0_20100727_SOCKET_SYSTEM_PARSER
    if(slot.IsZardItem())
    {
        xml.AddChildElemEx(_T("IsZardItem"), 1);
        xml.AddChildElemEx(_T("Zard_Index"), slot.GetExtendData());
    }
    else
    {
        xml.AddChildElemEx(_T("IsZardItem"), 0);
        xml.AddChildElemEx(_T("Zard_Index"), 0);
    }    
#endif

	char pszTime[32] = {0};

	if(11 == pItemInfo->m_wSubType)
	{
		WriteFate(slot,xml, pszTime);//페이트 아이템 처리
	}
	else
	{	       
	    GetItemChargeExpireTime(pItemInfo->m_ChargeSubType, slot, xml, pszTime);    
	}      

	xml.AddChildElem(_T("ChargeExpireTime"), pszTime );		// 사용만료 시간

	xml.AddChildElemEx(_T("ChargeOnUse"), slot.IsProgressTime() );		// 장착창에 한번이라도 착용햇는지
	xml.AddChildElemEx(_T("IsExpiredTime"), slot.IsExpiredTime() );		// 완료가 됐는지 안됐는지
	//#ifdef __NA_000493_20070712_SHOP_WEAPON_ARMOR_ITEM
	xml.AddChildElemEx(_T("IsExpiredWaitTime"), slot.IsExpiredWaitTime() );		// 완료가 됐는지 안됐는지
	//#endif
	xml.AddChildElemEx("DBType", slot.GetType() );			//서버군타입
	xml.AddChildElemEx("Serial", slot.GetSerial());			//시리얼
	xml.AddChildElemEx("WorldSerial", slot.GetWorldSerial());			//월드 시리얼

	if( (eITEM_MATERIAL_TYPE_CAN_BOTH == pItemInfo->m_byMaterialType ) || 
		( eITEM_MATERIAL_TYPE_CAN_EQUIP == pItemInfo->m_byMaterialType))
		xml.AddChildElemEx("EqPos", pItemInfo->m_wEqPos);		//장비위치
	else
		xml.AddChildElemEx("EqPos", -1);		//장비위치

	//		if( (slot.GetItemInfo()->m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_USE) || (slot.GetItemInfo()->m_byMaterialType == eITEM_MATERIAL_TYPE_NOT_USE_AND_EQUIP) )
	if(!slot.WithDura() )
	{
		xml.AddChildElemEx("IsDura", 0);			//소모성이면 0이고

	}
	else
	{
		xml.AddChildElemEx("IsDura", 1);			//소모성이 아니면 1이된다.
	}

	if(slot.WithDura())							//내구도 여부판별해서 
	{
		xml.AddChildElemEx("Dura", slot.GetDura());			//내구도를 가지고 있으므로 내구도 표기
	}
	else
	{
		xml.AddChildElemEx("Dura", slot.GetNum());			//내구도가 없으므로 아이템 개수를 표기
	}

#ifdef __NA_0_POSSESSION_ITEM_DROP	
	xml.AddChildElemEx("IsDestroyItem", slot.IsDestroyItem() ? 1 : 0);		//디바인여부
#endif

	xml.AddChildElemEx("IsDivine", slot.IsDivine());		//디바인여부

#ifndef _NA_0_20100727_SOCKET_SYSTEM_PARSER
	string sTemp = ConvertRankType(slot.GetRank());
	xml.AddChildElem("RankType", sTemp.c_str());	//현재 랭크	
#endif


	UINT uiNOptVal =  slot.GetNOptionValueIndex(); //slot.GetNOptionAttrValue();	//NOPTION를 얻어온다.
	xml.AddChildElemEx("NOptionValue", uiNOptVal);

	const sEXTRASTONE_OPT_INFO* pOption = slot.GetNOptionAttrDesc();		//해당 NOPTION의 옵션정보를 얻어온다.

	const sEXTRASTONE_OPT_INFO* pInfo = ExtraStoneOptionInfoParser::Instance()->GetExtraStoneOptionInfo(slot.GetItemInfo()->m_wType/*, slot.GetItemInfo()->m_LV*/);

#ifdef _OLD_EXTRA_STONE_PROCESS_
	if(pOption)				//해당 옵션정보가 있으면 추가한다.
	{
		xml.AddChildElemEx("NOptionNCode", pOption->m_dwOptionNCode);
	}
#else
	if(pInfo)
	{
		xml.AddChildElemEx("NOptionNCode", pInfo->m_dwOptionNCode);
	}
#endif
	xml.AddChildElemEx("Block", slot.IsBlocked());			//아이템 Block여부

	xml.AddChildElemEx("SET", slot.GetSet());		//set

#ifdef _NA_0_20100727_SOCKET_SYSTEM_PARSER
    xml.AddChildElemEx("SocketNum", slot.GetSocketNum());		//소켓넘버
    xml.AddChildElemEx("SocketIdentifyType", (int)slot.GetSocketIdentifyType());		//소켓넘버

	for(int sock = (INT)SOCKET_1; sock < slot.GetSocketNum();sock++)
	{
        const sSOCKET_OPTION* socket_option = slot.GetSocketOption((eSOCKET)sock);
		
		if(socket_option == NULL)	
			continue;

        xml.AddChildElem("ZARD");

        xml.IntoElem();
        xml.AddChildElem("pos", ConvertSocketType((eSOCKET)sock).c_str());                
        xml.AddChildElemEx("index", socket_option->index);
        xml.OutOfElem();
     
	}

#else //_NA_0_20100727_SOCKET_SYSTEM_PARSER

    //랭크 테그 변경
    //2009-11-09
    BYTE byIndex = 0, byValue = 0;
    eATTR_TYPE attrType;
    TCHAR szTemp[256];
    for(BYTE rank = (BYTE)RANK_D; rank < slot.GetRank()+1; rank++)
    {
        //인덱스와 값을 가져온다.
        byValue = slot.GetRankAttrValue((eRANK)rank);

        const sRANK_OPTION* pRankOpt = slot.GetRankAttrDesc((eRANK)rank);

        if(pRankOpt)	//랭크옵션이 있다면.
        {
            if(pRankOpt->m_OptionNCode == 0)
            {
                continue;
            }

            byIndex = pRankOpt->m_iOptionIndex;            

            TCHAR pszText[256];
            GetStringByCode(pRankOpt->m_OptionNCode, pszText, 256);

            xml.AddChildElem("RANK");
            xml.IntoElem();
            xml.AddChildElemEx("WorldSerial", slot.GetWorldSerial());
            xml.AddChildElem("RankName", pszText);
            xml.AddChildElem("RankLevel", ConvertRankType((eRANK)rank).c_str());
            xml.AddChildElemEx("RankValue", byValue);
            xml.AddChildElemEx("RankOptionIndex", byIndex);
            xml.AddChildElemEx("NameCode", pRankOpt->m_OptionNCode);
            xml.OutOfElem();

            //랭크명, 텍스트, 밸류, 인덱스 순으로 넘겨준다.
            //wsprintf(szTemp, "%s:%s:%d:%d ", ConvertRankType((eRANK)rank).c_str(), pszText, byValue, byIndex);
            //sRankOptStr += szTemp;
        }
    }


	xml.AddChildElemEx("SocketNum", slot.GetSocketNum());		//소켓넘버

	//소켓옵션정보
	int iSockOptCount = 0;
	std::string sSockOptStr = "[ ";

	for(int sock = (INT)SOCKET_1; sock < slot.GetSocketNum();sock++)
	{
		eATTR_TYPE attr = slot.GetSocketAttr((eSOCKET)sock);
		if(attr == eATTR_TYPE_INVALID)	
			continue;

		eSOCKET_LEVEL level = slot.GetSocketAttrLevel((eSOCKET)sock);
		//	if(level != SOCKLV_NONE)
		{
			byValue = slot.GetSocketAttrValue((eSOCKET)sock);
			//	attrType = g_SOCKET_OPTION[byIndex];

			const sSOCKET_OPTION* pRankOpt = slot.GetSocketAttrDesc((eSOCKET)sock);

			if(pRankOpt)		//소켓옵션이 있다면.
			{

				//스트링테이블에서 해당 스트링을 찾는다.
				//					TCHAR* pszText = STRTBL_PARSER1->GetStringByCode(pRankOpt->m_NCode);
				//					if(pszText == NULL)
				//					{
				//						_LOG("can't find STRTBL_PARSER1  %d", pRankOpt->m_NCode);
				//						continue;
				//					}
				TCHAR pszText[256];
				GetStringByCode(pRankOpt->m_NCode, pszText, 256);

				byIndex = pRankOpt->m_AttrIndex;
                xml.AddChildElem("SOCKET");
                xml.IntoElem();
                xml.AddChildElemEx("WorldSerial", slot.GetWorldSerial());
                xml.AddChildElem("Socket", ConvertSocketType((eSOCKET)sock).c_str());                
                xml.AddChildElem("SocketName", pszText);
                xml.AddChildElemEx("SocketValue", byValue);
                xml.AddChildElemEx("SocketOptionIndex", byIndex);
                xml.AddChildElemEx("SocketLevel", level);
                xml.AddChildElemEx("NameCode", pRankOpt->m_NCode);
                xml.OutOfElem();
			}
		}
	}

	if(iSockOptCount > 0)
		sSockOptStr += " ]";
	else
		sSockOptStr = "";
#endif //_NA_0_20100727_SOCKET_SYSTEM_PARSER

#ifdef _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    // reference: eEtheriaHistory 
    ;{
        eEtheriaHistory history = slot.GetEtheriaHistory();
        SLOTCODE image_item_code = slot.GetImageCode();
        xml.AddChildElemEx("History", history.value);
        xml.AddChildElemEx("ImageItemCode", image_item_code);
    };
#endif
#ifdef _NA_003740_20111122_RANDOM_ITEM
    {
        const int is_enable_randomize = slot.EnableRandomize() ? 1 : 0;
        xml.AddChildElemEx("IsEnableRandomize", is_enable_randomize);
        
        const SCRandomInfo::OptionInfo& random_option_info = slot.random_info_.option_info_;
        const uint8_t random_option_size = random_option_info.size;
        xml.AddChildElemEx("RandomOptionSize", random_option_size);
        
        BOOST_STATIC_ASSERT(_countof(random_option_info.keys) == 5);
        xml.AddChildElemEx("RandomOptionKey1", random_option_info.keys[0]);
        xml.AddChildElemEx("RandomOptionKey2", random_option_info.keys[1]);
        xml.AddChildElemEx("RandomOptionKey3", random_option_info.keys[2]);
        xml.AddChildElemEx("RandomOptionKey4", random_option_info.keys[3]);
        xml.AddChildElemEx("RandomOptionKey5", random_option_info.keys[4]);
    }
#endif // _NA_003740_20111122_RANDOM_ITEM

	xml.OutOfElem();
}

STDMETHODIMP CCSunOnline::EncodeItem(SCItemSlot& slot, CMarkupArchive& xml, POSTYPE& SlotIdx)
{    
    xml.IntoElem();

	DWORD dwTemp = 0;
	std::string sTemp;

    //long bySocketNum11 = 0;
    //CHECK(xml.FindGetChildData("SocketNum", bySocketNum11), "SocketNum");			//소켓번호
    

    CHECK(xml.FindGetChildData(_T("Code"), dwTemp), "Code");			//Code 셋팅
    slot.SetCode((SLOTCODE)dwTemp);
    
    ReadLimited(slot, xml);	//리미티드 속성을 추가한다.

	encodeEtherItemInfo(slot,xml); //에테르 아이템 정보 추가
	encodePetItemInfo(slot,xml);   //펫 아이템 정보 추가.
    encodeRideItem(slot,xml);       //라이드 아이템 처리

	CHECK(xml.FindGetChildData(_T("Pos"), dwTemp), "Pos");			//아이템 위치
	SlotIdx = dwTemp;										//즉 인벤토리에서의 위치. 슬롯 번호

	CHECK(xml.FindGetChildData(_T("Enchant"), dwTemp), "Enchant");		//Enchant값 셋팅    
	slot.SetEnchant((BYTE)dwTemp);		

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    CHECK(xml.FindGetChildData(_T("Awakening"), dwTemp), "Awakening");    // Awakening값 셋팅    
    slot.SetAwakening((BYTE)dwTemp);	
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#ifdef _NA_0_20100727_SOCKET_SYSTEM_PARSER
    CHECK(xml.FindGetChildData(_T("IsZardItem"), dwTemp), "IsZardItem");
    if(dwTemp == 1)
    {
        CHECK(xml.FindGetChildData(_T("Zard_Index"), dwTemp), "Zard_Index");
        slot.SetExtendData(dwTemp);
    }
#endif
	

	////////////////////////////////
	// set Base ItemInfo
	CHECK(xml.FindGetChildData(_T("ChargeSubType"), dwTemp), "ChargeSubType");
	BYTE ChargeSubType= dwTemp;

	CHECK(xml.FindGetChildData(_T("ChargeExpireTime"), sTemp), "ChargeExpireTime");    
	SetItemChargeExpireTime( ChargeSubType, slot, xml, sTemp.c_str() );
	ReadFate(slot, xml, sTemp.c_str());

	CHECK(xml.FindGetChildData(_T("ChargeOnUse"), dwTemp), "ChargeOnUse");
	slot.SetProgressTime(dwTemp);

	CHECK(xml.FindGetChildData(_T("IsExpiredTime"), dwTemp), "IsExpiredTime");
	slot.SetExpiredTime(dwTemp);
	//#ifdef __NA_000493_20070712_SHOP_WEAPON_ARMOR_ITEM
	if(xml.FindGetChildData(_T("IsExpiredWaitTime"), dwTemp)) slot.SetExpiredWaitTime(dwTemp);
	//#endif
	////////////////////////////////

	CHECK(xml.FindGetChildData(_T("EqPos"), dwTemp), "EqPos");
    

	DBSERIAL dbserial;
	CHECK(xml.FindGetChildData(_T("Serial"), dbserial), "Serial");		//아이템 시리얼 셋팅
	slot.SetSerial(dbserial);

	BYTE bDbtype;
	CHECK(xml.FindGetChildData(_T("DBType"), dwTemp), "DBType");
	bDbtype = dwTemp;
	slot.SetType((BYTE)bDbtype);			//서버군타입

	CHECK(xml.FindGetChildData(_T("IsDura"), dwTemp), "IsDura");
	if(dwTemp == 1)		//소모성아니면 내구도를 입력한다.				
	{
		float fDura = 0.0f;
		CHECK(xml.FindGetChildData(_T("Dura"), fDura), "Dura");
		slot.SetDura((DURATYPE)fDura);								//내구도 셋팅			
	}
	else
	{
		CHECK(xml.FindGetChildData(_T("Dura"), dwTemp), "Dura");
		slot.SetNum(dwTemp);		//소모성이므로 개수를 입력
	}

#ifdef __NA_0_POSSESSION_ITEM_DROP
	CHECK(xml.FindGetChildData(_T("IsDestroyItem"), dwTemp), "IsDestroyItem");
	slot.SetDestroyItem(dwTemp == 1 ? TRUE : FALSE);
#endif

#ifdef __NA_1247_POSSESSION_ITEM
	CHECK(xml.FindGetChildData(_T("IsPossessionType"), dwTemp), "IsPossessionType");


	if(1 == dwTemp)
	{
		CHECK(xml.FindGetChildData(_T("IsPossession"), dwTemp), "IsPossession");
		if(0 == dwTemp) slot.SetIdentifyPossession(FALSE);
		else slot.SetIdentifyPossession(TRUE);
	}
#endif

	CHECK(xml.FindGetChildData(_T("IsDivine"), dwTemp), "IsDivine");
	if(dwTemp == 1)
		slot.SetSet(TRUE);
	else
		slot.SetSet(FALSE);

	CHECK(xml.FindGetChildData(_T("Block"), dwTemp), "Block");
	if(dwTemp == 1)
		slot.SetBlocked(TRUE);
	else
		slot.SetBlocked(FALSE);	

#ifdef _OLD_EXTRA_STONE_PROCESS_

	const sEXTRASTONE_OPT_INFO* pOption = slot.GetNOptionAttrDesc();		//해당 NOPTION의 옵션정보를 얻어온다.

	if(pOption)           // edited 2006.06.25
	{
		slot.SetNOptionAttr(pOption->m_wOptionIndex);
	}
#else
	const sEXTRASTONE_OPT_INFO* pInfo = ExtraStoneOptionInfoParser::Instance()->GetExtraStoneOptionInfo(slot.GetItemInfo()->m_wType/*, slot.GetItemInfo()->m_LV*/);
	if(pInfo)           // edited 2006.06.25
	{
		//slot.SetNOptionAttr(pInfo->m_wOptionIndex);
		CHECK(xml.FindGetChildData(_T("NOptionValue"), dwTemp), "NOptionValue");
		slot.SetNOptionAttr(dwTemp );		
	}
#endif			

	CHECK(xml.FindGetChildData(_T("SET"),dwTemp), "SET");			//Set값 입력
	//slot.SetSet((BYTE)dwTemp);			

   
#ifdef _NA_0_20100727_SOCKET_SYSTEM_PARSER
    long bySocketNum = 0;
    CHECK(xml.FindGetChildData("SocketNum", bySocketNum), "SocketNum");			//소켓번호
    slot.SetSocketNum((BYTE)bySocketNum);

    CHECK(xml.FindGetChildData("SocketIdentifyType", bySocketNum), "SocketIdentifyType");
    slot.SetSocketIdentifyType((eSOCKET_IDENTIFY_TYPE)bySocketNum);    
    
    while(TRUE)
    {
        if(!xml.FindChildElem(_T("ZARD"))) break;
    
        xml.IntoElem();
    
        CHECK(xml.FindGetChildData("pos", sTemp), "pos");
        eSOCKET sock = ConvertSocketType(sTemp);
    
        if(sock == SOCKET_MAX) 
            continue;
    
        SOCKETINDEX index;
    
        CHECK(xml.FindGetChildData("index", index), "index");        
    
        xml.OutOfElem();        
    
        slot.SetSocketOption(sock, index);       
    }
#else //_NA_0_20100727_SOCKET_SYSTEM_PARSER
    CHECK(xml.FindGetChildData(_T("RankType"), sTemp), "RankType");
    slot.SetRank(ConvertRankType(sTemp));					//랭크 타입 입력

    TCHAR szTempBuff[128];
    int byIndex, byValue;
    int iStrIndex;

    //랭크 테그 변경
    //2009-11-09
    for(POSTYPE pos = 0; pos < MAX_RANK_OPTION; pos++)
    {
        if(!xml.FindChildElem(_T("RANK"))) continue;        

        xml.IntoElem();        

        if(!xml.FindChildElem(_T("RankLevel")) && 
            !xml.FindChildElem(_T("RankValue")) && 
            !xml.FindChildElem(_T("NameCode")) && 
            !xml.FindChildElem(_T("RankOptionIndex")))
        {
            xml.OutOfElem();
            continue;
        }

        CHECK(xml.FindGetChildData("RankLevel", sTemp), "RankLevel");
        eRANK rank = ConvertRankType(sTemp);

        CHECK(xml.FindGetChildData("RankValue", byValue), "RankValue");
        CHECK(xml.FindGetChildData("NameCode", iStrIndex), "NameCode");
        CHECK(xml.FindGetChildData("RankOptionIndex", byIndex), "RankOptionIndex");

        xml.OutOfElem();

        sRANK_OPTION* pRankOpt = RankOptionParser::Instance()->GetRankOption(slot.GetItemInfo()->m_wType, byIndex);
        if(pRankOpt)		//랭크옵션을 셋팅한다.
        {
            pRankOpt->m_iOptionIndex = byIndex;
            if(iStrIndex > 0) pRankOpt->m_OptionNCode = iStrIndex;
            pRankOpt->m_Value[rank] = byValue;

            slot.SetRank((eRANK)(rank));
            slot.SetRankAttr((eRANK)(rank), pRankOpt);
        }        
    }

    long bySocketNum = 0;
    CHECK(xml.FindGetChildData("SocketNum", bySocketNum), "SocketNum");			//소켓번호
    slot.SetSocketNum((BYTE)bySocketNum);

    while(TRUE)
    {
        if(!xml.FindChildElem(_T("SOCKET"))) break;

        xml.IntoElem();

        if(!xml.FindChildElem(_T("Socket")) && 
            !xml.FindChildElem(_T("SocketValue")) && 
            !xml.FindChildElem(_T("SocketOptionIndex")) && 
            !xml.FindChildElem(_T("SocketLevel")) && 
            !xml.FindChildElem(_T("NameCode")))
        {
            xml.OutOfElem();
            continue;
        }

        CHECK(xml.FindGetChildData("Socket", sTemp), "Socket");
        eSOCKET sock = ConvertSocketType(sTemp);

        int iStrIndex;
        int byValue;
        int Socketlevel;
        int byIndex;


        CHECK(xml.FindGetChildData("SocketValue", byValue), "SocketValue");
        CHECK(xml.FindGetChildData("SocketOptionIndex", byIndex), "SocketOptionIndex");
        CHECK(xml.FindGetChildData("SocketLevel", Socketlevel), "SocketLevel");
        CHECK(xml.FindGetChildData("NameCode", iStrIndex), "NameCode");

        xml.OutOfElem();

        eSOCKET_LEVEL level = (eSOCKET_LEVEL)Socketlevel;

        const sSOCKET_OPTION* pSockOption = SocketOptionParser::Instance()->GetSocketOption(byIndex);
        sSOCKET_OPTION* pSockOpt = const_cast<sSOCKET_OPTION*>(pSockOption);
        if(pSockOpt)		//소켓옵션 셋팅
        {
            pSockOpt->m_AttrIndex = byIndex;
            if(iStrIndex > 0) pSockOpt->m_NCode = iStrIndex;
            pSockOpt->m_iValue[sock] = byValue;

            slot.SetSocketAttr(sock, level, pSockOpt);
        }             
    }
#endif _NA_0_20100727_SOCKET_SYSTEM_PARSER
    

#ifdef _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    // reference: eEtheriaHistory 
    ;{
        CHECK(xml.FindGetChildData("History", dwTemp), "History");
        eEtheriaHistory history = { static_cast<eEtheriaHistory::value_type>(dwTemp) };
        switch (dwTemp)
        {
        case history.None:
        case history.Combined:
        case history.Etheria:
        case history.Shell:
            break;
        default:
            _LOG("Can't accept 'History' value");
            return E_FAIL;
        };
        if (history.value != history.None)
        {
            CHECK(xml.FindGetChildData("ImageItemCode", dwTemp), "ImageItemCode");
            SLOTCODE image_item_code = static_cast<SLOTCODE>(dwTemp);
            SLOTCODE item_code = slot.GetCode();
            if (image_item_code == 0) {
                image_item_code = history.value == history.Etheria ? HardCode::ITEMCODE_of_ETHERIA
                                : item_code;
            }
            slot.ChangeImageItem(image_item_code, history.value);
        }
    };
#endif

#ifdef _NA_003740_20111122_RANDOM_ITEM
    {
        int is_enable_randomize = 0;
        CHECK(xml.FindGetChildData("IsEnableRandomize", is_enable_randomize), "IsEnableRandomize");
        if (is_enable_randomize == 1)
        {
            int random_option_size = 0;
            CHECK(xml.FindGetChildData("RandomOptionSize", random_option_size), "RandomOptionSize");
            
            SCRandomInfo::OptionInfo& random_option_info = slot.random_info_.option_info_;
            if (random_option_size > _countof(random_option_info.keys))
            {
                _LOG("|RandomOptionSize is invalid.|size = %d|", random_option_size);
                return E_FAIL;
            }

            const RandomItemParser::RandomKeyManager& key_manager = 
                RandomItemParser::Instance()->key_manager_;

            SCRandomInfo::OptionInfo::Key random_option_key1 = 0;
            CHECK(xml.FindGetChildData("RandomOptionKey1", random_option_key1), "RandomOptionKey1");
            if (random_option_key1 != 0 && key_manager.GetKey(random_option_key1) == NULL)
            {
                _LOG("|RandomOptionKey1 is not exist.|key = %d|", random_option_key1);
                return E_FAIL;
            }           
            SCRandomInfo::OptionInfo::Key random_option_key2 = 0;
            CHECK(xml.FindGetChildData("RandomOptionKey2", random_option_key2), "RandomOptionKey2");
            if (random_option_key2 != 0 && key_manager.GetKey(random_option_key2) == NULL)
            {
                _LOG("|RandomOptionKey2 is not exist.|key = %d|", random_option_key2);
                return E_FAIL;
            }
            SCRandomInfo::OptionInfo::Key random_option_key3 = 0;
            CHECK(xml.FindGetChildData("RandomOptionKey3", random_option_key3), "RandomOptionKey3");
            if (random_option_key3 != 0 && key_manager.GetKey(random_option_key3) == NULL)
            {
                _LOG("|RandomOptionKey3 is not exist.|key = %d|", random_option_key3);
                return E_FAIL;
            }
            SCRandomInfo::OptionInfo::Key random_option_key4 = 0;
            CHECK(xml.FindGetChildData("RandomOptionKey4", random_option_key4), "RandomOptionKey4");
            if (random_option_key4 !=0 && key_manager.GetKey(random_option_key4) == NULL)
            {
                _LOG("|RandomOptionKey4 is not exist.|key = %d|", random_option_key4);
                return E_FAIL;
            }
            SCRandomInfo::OptionInfo::Key random_option_key5 = 0;
            CHECK(xml.FindGetChildData("RandomOptionKey5", random_option_key5), "RandomOptionKey5");
            if (random_option_key5 != 0 && key_manager.GetKey(random_option_key5) == NULL)
            {
                _LOG("|RandomOptionKey5 is not exist.|key = %d|", random_option_key5);
                return E_FAIL;
            }

            random_option_info.size = static_cast<uint8_t>(random_option_size);
            random_option_info.keys[0] = random_option_key1;
            random_option_info.keys[1] = random_option_key2;
            random_option_info.keys[2] = random_option_key3;
            random_option_info.keys[3] = random_option_key4;
            random_option_info.keys[4] = random_option_key5;
        }
    }
#endif // _NA_003740_20111122_RANDOM_ITEM

	xml.OutOfElem();

	return S_OK;
}

BOOL CCSunOnline::IsVertyPetName(TCHAR* PetName)
{
    for(int i = 0 ; i < MAX_PET_NAME_LENGTH; i++)
    {
        if(!IsVertyPetName(PetName[i])) return FALSE;
    }
    return TRUE;
}

BOOL CCSunOnline::IsVertyPetName(BYTE PetNameByte)
{
    /*if((1 <= PetNameByte  && PetNameByte <= 47) || 
    (58 <= PetNameByte  && PetNameByte <= 64) || 
    (91 <= PetNameByte  && PetNameByte <= 96) ||
    (123 <= PetNameByte  && PetNameByte <= 127))*/
    if(PetNameByte >= 1 &&
        PetNameByte <= 31)
    {
        return FALSE;
    }
    return TRUE;
}

#ifdef _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_
STDMETHODIMP CCSunOnline::GetSkillInfo(int skill_code, int* skill_level, int* max_skill_level, BSTR* skill_name, BOOL* result)
{
    const SkillScriptInfo* skill_info = SkillInfoParser::Instance()->GetSkillInfo(skill_code);

    if(skill_info)
    {
        *skill_level = skill_info->m_wSkillLv;
        *max_skill_level = skill_info->m_wMaxLv;
        TCHAR pName[256];
        GetStringByCode(skill_info->m_dwSkillNcode, pName, 256);	
        *skill_name = _bstr_t(pName).copy();
        *result = TRUE;
        return S_OK;
    }

    *result = FALSE;
    return S_FALSE;
}
#endif

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
STDMETHODIMP CCSunOnline::GetPerkInfo(unsigned short group_code, unsigned short leve, BSTR* name, BSTR* request_exp, BOOL* result)
{
    PerkID perk_id(group_code, leve);
    const PerkInfo* perk_info = PerkInfoParser::Instance()->FindPerkInfo(perk_id);

    if(perk_info)
    {
        TCHAR pName[256];
        GetStringByCode(perk_info->name_code_, pName, 256);	
        *name = _bstr_t(pName).copy();

        wsprintf(pName, "%I64d", perk_info->exp_value_);
        *request_exp = _bstr_t(pName).copy();

        *result = TRUE;
        return S_OK;
    }

    *result = FALSE;
    return S_FALSE;
}
#endif
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
STDMETHODIMP CCSunOnline::GetACInfo(int index, BSTR* achievement_name, BSTR* location_name, 
                                    int* location_code, int* valid_count, int achievement_valu[MAX_ACHIEVEMENT_OBJECT], BOOL* result)
{
    const Achievement_ParserInfo* achievement_info = AchievementParser::Instance()->GetAchievement_ParserInfo(index);
    
    if(achievement_info)
    {
        *location_code = achievement_info->location_code_;

        int i;
        for( i = 0 ; i < MAX_ACHIEVEMENT_OBJECT ; ++i)
        {
            if(achievement_info->obejcts_[i].object_code_ > 0)
            {
                achievement_valu[i] = achievement_info->obejcts_[i].object_number_;
            }
            else
            {
                break;
            }
        }

        *valid_count = i;

        TCHAR pName[256];
        GetStringByCode(achievement_info->object_string_code_, pName, 256);	
        *achievement_name = _bstr_t(pName).copy();
        
        GetStringByCode(achievement_info->location_code_, pName, 256);	
        *location_name = _bstr_t(pName).copy();

        *result = TRUE;
        return S_OK;
    }

    *result = FALSE;
    return S_FALSE;
}

#endif

#ifdef _NA002217_100728_EP2_SKILL_SYSTEM_DB_
STDMETHODIMP CCSunOnline::DecodeItemInfo(BYTE position, __int64 serial, int code, 
                                      BYTE dura, BYTE version, BYTE option[sizeof(OPTIONPART)], BSTR* bstrXml)
{    

    ITEMSTREAMEX item_stream = { 0, }; // for unused range clear
    BYTE item_version = version;

    item_stream.Part.m_WorldSerial = serial;
    //item_stream.Part.dwSerial = serial;
    item_stream.Part.wCode = code;
    item_stream.Part.byDura = dura;
     
    CopyMemory(&item_stream.OptionPart, option, sizeof(OPTIONPART));   

    SCItemSlot slot;    

    if(version != ITEM_STREAM_SIZE_LAST_VERSION)
    {
        int converted_version = 0;
        ITEMSTREAMEX result_stream;

        FlowControl::FCAssert(VersionControl::ConvertItem(&converted_version, &result_stream,
            version, &item_stream));

        item_version = converted_version;

        slot.Copy(result_stream);
    }
    else
    {
        slot.Copy(item_stream);
    }

    CMarkupArchive xml(CMarkupArchive::ArchiveModeCreate |
        CMarkupArchive::ArchiveModeStoring |
        CMarkupArchive::ArchiveModeNotZipped);

    SetXMLCharSet(xml);

    xml.Open("C:\\WOPSConfig\\SUNGMS\\item.xml");

    xml.AddElem("ITEM");        

    DecodeItem(slot, xml, position, item_version);   
    
    *bstrXml = _bstr_t(xml.GetDoc().c_str() ).copy();

    
    xml.Close();

    return S_OK;
}

STDMETHODIMP CCSunOnline::EncodeItemInfo(BSTR bstrXml, __int64* serial, int* code, 
                                         BYTE* dura, BYTE* version, BYTE option[sizeof(OPTIONPART)], BOOL* result)
{
    std::string strBstrTmp;
    ConvertBSTRToString(bstrXml,strBstrTmp);	
    const char* pszXml = strBstrTmp.c_str();

    CMarkupArchive xml(CMarkupArchive::ArchiveModeNotZipped | CMarkupArchive::ArchiveModeString);
    xml.OpenString((const char*)pszXml);

    if(!xml.FindElem(_T("ITEM")))
    {
        *result = FALSE;
        return S_FALSE;
    }

    SCItemSlot slot;   
    POSTYPE SlotIdx = 0;
    if(S_OK != EncodeItem(slot,xml, SlotIdx))
    {
        *result = FALSE;
        return S_FALSE;
    }
    xml.OutOfElem();
    xml.Close();

    *serial = slot.GetWorldSerial();
    *code = slot.GetCode();
    *dura = slot.WithDura() ? slot.GetDura() : slot.GetNum();
    *version = ITEM_STREAM_SIZE_LAST_VERSION;        
    
    ITEMSTREAMEX item_stream;
    slot.CopyOut(item_stream);        

    CopyMemory(option, &item_stream.OptionPart, sizeof(OPTIONPART));        

    //////////////

    ITEMSTREAMEX item_copy; 
    item_copy.Part.m_WorldSerial = slot.GetWorldSerial();
    item_copy.Part.wCode = slot.GetCode();    
    item_copy.Part.byDura = slot.GetDura();   

    CopyMemory(&item_copy.OptionPart, option, sizeof(OPTIONPART));    
    SCItemSlot copy_slot;    
    copy_slot.Copy(item_copy);
   

    *result = TRUE;
    return S_OK;
}
#endif	

STDMETHODIMP CCSunOnline::GetMissionName(int mission_code, BSTR* mission_name, BSTR* area_name, BOOL* result)
{
    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(mission_code);    

    if(map_info)
    {
        TCHAR pName[256];       
        
        GetStringByCode(map_info->dwNCode, pName, 256);	
        *mission_name = _bstr_t(pName).copy();

        GetStringByCode(map_info->dwANCode, pName, 256);	
        *area_name = _bstr_t(pName).copy();

        *result = TRUE;
        return S_OK;
    }

    *result = FALSE;
    return S_FALSE;
}

bool CCSunOnline::CreateGuildFacilityInfo() const
{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    CMarkupArchive xml(
        CMarkupArchive::ArchiveModeCreate|
        CMarkupArchive::ArchiveModeStoring| 
        CMarkupArchive::ArchiveModeNotZipped);

    SetXMLCharSet(xml);

    TCHAR path[MAX_PATH] = { 0 };
    _snprintf(path, _countof(path), "%s", "C:\\WOPSConfig\\SUNGMS\\getGuildFacilityInfo.xml");
    xml.Open(path);

    xml.AddElem(_T("GUILD_FACILITY"));

    GuildFacilityInfoParser* const parser = GuildFacilityInfoParser::Instance();
    const GuildFacilityInfoParser::FACILITY_MAP& facility_infos = parser->GetGuildFacilityInfoMap();
    for (GuildFacilityInfoParser::FACILITY_MAP::const_iterator it = facility_infos.begin(); 
        it != facility_infos.end(); ++it)
    {
        const FACILITY_INFO& facility_info = it->second;

        xml.AddChildElem(_T("FACILITY_INFO"));
        xml.IntoElem();

        xml.AddChildElemEx(_T("facility_index"), facility_info.facility_index);
        xml.AddChildElemEx(_T("facility_code"), facility_info.facility_code);
        xml.AddChildElemEx(_T("facility_level"), facility_info.facility_level);

        enum { kStrBufSize = 256 };
        TCHAR name_str[kStrBufSize] = { 0 };
        _tcscpy(name_str, _T(facility_info.facility_name.c_str()));
        xml.AddChildElemEx(_T("facility_name"), name_str);
        
        xml.AddChildElemEx(_T("upgrade_type"), facility_info.upgrade_type);
        xml.AddChildElemEx(_T("upgrade_guild_point"), facility_info.upgrade_guild_point);
        xml.AddChildElemEx(_T("upgrade_item_code"), facility_info.upgrade_item_code);
        xml.AddChildElemEx(_T("upgrade_item_num"), facility_info.upgrade_item_num);

        xml.AddChildElemEx(_T("passive_option_type"), facility_info.passive_option.option_type);
        xml.AddChildElemEx(_T("passive_option_value_type"), facility_info.passive_option.option_value_type);
        xml.AddChildElemEx(_T("passive_option_value"), facility_info.passive_option.option_value);

        xml.AddChildElemEx(_T("active_item_code"), facility_info.active_option.active_item_code);
        xml.AddChildElemEx(_T("active_item_count"), facility_info.active_option.active_item_count);
        xml.AddChildElemEx(_T("active_skill_code"), facility_info.active_option.active_skill_code);

        xml.OutOfElem();
    }
    xml.Close();

#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    return true;
}