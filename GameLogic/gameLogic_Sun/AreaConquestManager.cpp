
#include "stdafx.h"
#include <assert.h>

#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED

#include "util/util.h"
#include "QuestEx.h"

#include "AreaConquestManager.h"
#include "AreaConquestInfoParser.h"
#include "AreaConquest.h"

#include "RewardInfoList.h"

//#include "QuestInfoManagerEx.h"
#include "MapInfoParser.h"
#include <Struct.h>
#include <SolarFileReader.h>

#include "CSunOnline.h"
#include "logic/logic.h"

extern QuestInfoManager2* g_pQuestInfoManager;

//#include "QuestInfoManagerEx.h"

/*#include <map>
#include <string>

typedef struct _COLLECTIONITEM
{		
	int nCurrentCount;
	int nMaxCount;
} COLLECTIONITEM;

typedef std::map<std::string, COLLECTIONITEM> MapCOLLECTION;
*/

const int AC_NOT_REVEALED_STRING = 91043; // 미 발견 스트링

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/*
void AreaConquestDialog::OnClickReward()
{
	if (m_iLastRewardSendIndex)
	{
		return;
	}

	MSG_CG_AREACONQUEST_REWARD_SYN SendPacket;

	AreaConquestInfo *pAreaInfo = g_AreaConquest.GetAreaConquestInfo(m_iAreaIndex);	

	SendPacket.m_byAreaIndex	= m_iAreaIndex;
	SendPacket.m_byRewardIndex  = 0;

	if (pAreaInfo)
	{
		for (int a = 0; a < MAX_AC_REWARD_NUM; a++)
		{
			if (!pAreaInfo->GetRewardFlag(a))
			{
				SendPacket.m_byRewardIndex = a;
				break;
			}
		}
	}

	m_iLastRewardSendIndex = m_iAreaIndex;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
}
*/

AreaConquestManager::AreaConquestManager()
{
}

AreaConquestManager::~AreaConquestManager()
{	
}

void AreaConquestManager::UpdateMissionInfo(MissionManager* pMissionMgr, BOOL bUpdate)
{
	for (int a = 0; a < MAX_AC_SECTION_NUM; a++)
	{
		AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(a+1);
		if (!pAreaInfo)
		{
			continue;
		}

		BASE_AreaConquestInfo *pTextInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(a+1);
		if (!pTextInfo)
		{
			continue;
		}

		MapGroup * pGroup = MapInfoParser::Instance()->GetMapGroup(pTextInfo->m_byMissionMapKind);
		if( !pGroup)
		{
			continue;
		}

//        pAreaInfo->SetConditionValue(eACC_MISSION_MAIN,0);
//        pAreaInfo->SetConditionValue(eACC_MISSION_SUB,0);
//        pAreaInfo->SetConditionValue(eACC_MISSION_HIDDEN,0);
//
//        //메인 미션
//        sMAPINFO * pInfo = pGroup->GetMapInfoIndex(0);
//        if( pInfo )
//        {
//            pAreaInfo->SetConditionValue(
//                eACC_MISSION_MAIN,
//                pMissionMgr->GetMissionPoint( (BYTE)pInfo->MapCode )
//                );
//        }
//#if NATION_BUILD_VERSION >= 902	
//        for( size_t i = 0 ; i < 4 ; ++i )
//        {
//            sMAPINFO * pInfo = pGroup->GetMapInfoIndex(i + 1);
//            if( !pInfo ) 
//            {
//                continue;
//            }
//
//
//            pAreaInfo->AddConditionValue(
//                eACC_MISSION_SUB1 + i,
//                pMissionMgr->GetMissionPoint((BYTE)pInfo->MapCode));
//        }
//#else
//        for( size_t i = 1 ; i <= MAX_AC_SECTION_NUM ; ++i )
//        {
//            sMAPINFO * pInfo = pGroup->GetMapInfoIndex(i);
//            if( !pInfo ) 
//            {
//                continue;
//            }
//
//
//            pAreaInfo->AddConditionValue(
//                eACC_MISSION_SUB,
//                pMissionMgr->GetMissionPoint((BYTE)pInfo->MapCode));
//        }
//#endif
//
//
//
//
//        pAreaInfo->CalculateCurrentAchievePoint();


        // 메인미션과 서브미션별 카운트 초기화
        for (int nCondition = eACC_MISSION_MAIN; nCondition <= eACC_MISSION_SUB4; ++nCondition)
        {
            pAreaInfo->SetConditionValue(nCondition,0);
        }

        sMAPINFO * pInfo = pGroup->GetMapInfoIndex(0);
        if( pInfo )
        {
            pAreaInfo->SetConditionValue(eACC_MISSION_MAIN,	pMissionMgr->GetMissionPoint((BYTE)pInfo->MapCode));			
        }

        for( int i = 0 ; i < (MAX_AC_SUB_MISSION_NUM - 1) ; ++i )
        {
            sMAPINFO * pInfo = pGroup->GetMapInfoIndex(i+1);
            if( !pInfo ) 
            {
                continue;
            }

            pAreaInfo->SetConditionValue(eACC_MISSION_SUB1+i, pMissionMgr->GetMissionPoint((BYTE)pInfo->MapCode));
        }
        pAreaInfo->CalculateCurrentAchievePoint();

		
	}
}

void AreaConquestManager::UpdateQuestInfo(QuestManager2* pQuestManager, BOOL bUpdate)
{
    // AC Info 각 지역 정보
    AreaConquestInfo *pAreaInfo[MAX_AC_SECTION_NUM] = {NULL,NULL,NULL,NULL,NULL,NULL};
    for (int a = 0; a < MAX_AC_SECTION_NUM; a++)
    {
        pAreaInfo[a] = GetAreaConquestInfo(a+1);
        if (pAreaInfo[a])
        {
            pAreaInfo[a]->SetConditionValue(eACC_QUEST_NORMAL,0);
        }		
    }

    // 각 퀘스트 순회 각 지역 AC 퀘스트 완료 횟수 가산
    for(QCODE i=0;i<MAX_QUEST_COMPL_NUM;++i)
    {
        QUESTPART_COMPL * pComplQuest = pQuestManager->FindComplQuest(i);
        if( !pComplQuest || pComplQuest->m_Num == 0 ) continue;
        QuestInfo * pInfo = g_pQuestInfoManager->FindQuestInfo(i);
        if( !pInfo )	continue;
        int ACCode = pInfo->GetACCode();
        int QType = pInfo->GetQType();
        if ( ACCode > 0 && ACCode <= MAX_AC_SECTION_NUM && QType == eQTYPE_GENERAL)
        {
            if ( pAreaInfo[(pInfo->GetACCode()-1)] )
            {
                pAreaInfo[pInfo->GetACCode()-1]->AddConditionValue(eACC_QUEST_NORMAL);
            }			
        }
    }

    // AC Info 각 지역 덜성률 계산
    for (int a = 0; a < MAX_AC_SECTION_NUM; a++)
        if (pAreaInfo[a])
            pAreaInfo[a]->CalculateCurrentAchievePoint();
    
    /*
	for (int a = 0; a < MAX_AC_SECTION_NUM; a++)
	{
		AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(a);
		if (!pAreaInfo)
		{
			continue;
		}

		pAreaInfo->SetConditionValue(eACC_QUEST_NORMAL,0);
		pAreaInfo->SetConditionValue(eACC_QUEST_EVENT,0);
		pAreaInfo->SetConditionValue(eACC_QUEST_MISSION,0);
		pAreaInfo->SetConditionValue(eACC_QUEST_HIDDEN,0);

#ifdef __NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
        for(QCODE i=0; i<QUESTPART_COMPL::MAX_QUEST_COMPL_NUM;++i)
#else
        for(QCODE i=0; i<MAX_QUEST_COMPL_NUM;++i)
#endif		
		{
			QUESTPART_COMPL * pComplQuest = pQuestManager->FindComplQuest(i);
			if( !pComplQuest || pComplQuest->m_Num == 0 ) 
				continue;

			QuestInfo * pInfo = g_pQuestInfoManager->FindQuestInfo(i);
			if( !pInfo ) continue;
			if( a == pInfo->GetACCode() )
			{
				switch(pInfo->GetQType())
				{
				case eQTYPE_GENERAL:
					{
						pAreaInfo->AddConditionValue(eACC_QUEST_NORMAL); 
					}
					break;
				case eQTYPE_ELITE:
					{
						pAreaInfo->AddConditionValue(eACC_QUEST_EVENT);
					}
					break;
				case eQTYPE_MISSION:
					{
						pAreaInfo->AddConditionValue(eACC_QUEST_MISSION);					
					}
					break;
				case eQTYPE_HIDDEN:
					{
						pAreaInfo->AddConditionValue(eACC_QUEST_HIDDEN);						
					}
					break;
				}
			}
		}

		pAreaInfo->CalculateCurrentAchievePoint();
	}*/
}

void AreaConquestManager::UpdateAll(BSTR* bstrXml)
{
	xml.Open("C:\\WOPSConfig\\SUNGMS\\decodeACStream.xml");
	xml.AddElem(_T("ACSystem"));

	for(int iAreaIndex = 1; iAreaIndex <= MAX_AC_SECTION_NUM/*MAX_AC_MAP*/; iAreaIndex++) 
	{
		if(!SetAreaIndex(iAreaIndex)) continue;

		xml.AddChildElem("Map");
		xml.IntoElem();

		UpdateEtc();
		UpdateTreasure(); // 보물
		UpdateReward();   // 보상
		UpdateHunting();  // 사냥
		UpdateCollection(); // 수집품
		UpdateBoss();       // 보스
		UpdateTowerDefense(); // 타워디펜스
		UpdateQuest();      // 퀘스트
		UpdateBattle();     // 배틀
		UpdateMission();    // 미션		
		
		xml.OutOfElem();
	}
	
	*bstrXml = _bstr_t(xml.GetDoc().c_str() ).copy();
	xml.Close();
}

void AreaConquestManager::CheckRewardFlag()
{
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	if (pAreaInfo)
	{
		for (int a = 0; a < MAX_AC_REWARD_NUM; a++)
		{
			if (!pAreaInfo->GetRewardFlag(a))
			{
				pAreaInfo->SetReward(a,true);
				break;
			}
		}
	}
}

void AreaConquestManager::Init()
{
	xml.SetModeFlag(CMarkupArchive::ArchiveModeCreate |
		CMarkupArchive::ArchiveModeStoring |
		CMarkupArchive::ArchiveModeNotZipped);
	
	SetXMLCharSet(xml);
	
	/*AreaConquest::Init();

	for (int a = 0; a < MAX_AC_SECTION_NUM; a++)
	{		
		AreaConquestInfo *pInfo = new AreaConquestInfo();
		pInfo->Init();
		
		SetAreaConquestInfo(a + 1,pInfo);
		BASE_AreaConquestInfo *pACInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(a + 1);

		if (pInfo && pACInfo)
		{
			pInfo->SetAllCondition(pACInfo);
		}
	}*/

    AreaConquest::Init();

    for (int a = 0; a < MAX_AC_SECTION_NUM; a++)
    {		
        AreaConquestInfo *pInfo = new AreaConquestInfo();

        pInfo->Init();

        SetAreaConquestInfo(a + 1,pInfo);
        BASE_AreaConquestInfo *pACInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(a + 1);

        if (pInfo && pACInfo)
        {
            pInfo->SetAllCondition(pACInfo);
        }
    }

	m_fBattleGaugeSize = 100.0f;
	m_iLastRewardSendIndex = 0;
}

void AreaConquestManager::Release()
{
	//////////////////////////////
	// Area Conquset Release
	for (int a = 0; a < MAX_AC_SECTION_NUM; a++)
	{
		if (m_pAreaInfo[a])
		{
			delete m_pAreaInfo[a];
			m_pAreaInfo[a] = NULL;
		}
	}

	AreaConquest::Release();	
}

//////////////////////////////////////////////////////////////////////////

void AreaConquestManager::UpdateTreasure()
{
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);
	if (!pAreaInfo || !pInfo)
	{
		return;
	}

	xml.AddChildElem("TreasureCategory");
	xml.IntoElem();

/*	int iFlag = 0;
	iFlag = pAreaInfo->GetConditionValue(eACC_TREASURE_KEY_FLAG);
	if(iFlag)
	{	
	}
	else
	{	
	}

	iFlag = pAreaInfo->GetConditionValue(eACC_TREASURE_BOX_FLAG);
	if(iFlag)
	{
	}
	else
	{
	}

	for(int a = 0; a < MAX_AC_MAP_PIECE_IMAGE; a++)
	{
		if (pAreaInfo->GetConditionValue(eACC_TREASURE_MAP_FLAG1 + a))
		{
		}
		else
		{
		}
	}*/

	if(pInfo && pAreaInfo)
	{
		TCHAR szString[128];

		int iMapCount = 0;

		for (int a = 0; a < 8; a++)
		{
			if (pAreaInfo->GetConditionValue(eACC_TREASURE_MAP_FLAG1 + a))
			{
				iMapCount++;
			}			
		}

		xml.AddChildElem("Treasure");
		xml.IntoElem();

		GetStringByCode(pInfo->m_Piece_Scode,szString,128);
		//pTextMapName->SetTextWZA(szString);
		xml.AddChildElem("Name", szString);
		_stprintf(szString,"%d",iMapCount);
		//pTextCurMap->SetTextWZ(szString);
		xml.AddChildElem("CurrentPoint", szString);
		_stprintf(szString,"8");
		//pTextMaxMap->SetTextWZ(szString);
		xml.AddChildElem("MaxPoint", szString);

		xml.OutOfElem();
		xml.AddChildElem("Treasure");
		xml.IntoElem();

		GetStringByCode(pInfo->m_Key_Scode,szString,128);
		//pTextKeyName->SetTextWZA(szString);
		xml.AddChildElem("Name", szString);
#ifdef __NA_0_AC_KEY_DIRECT_REGISTER
        int numKeyPieces = 0;
        for (int i = 0; i < 5; ++i) {
            if (pAreaInfo->GetConditionValue(eACC_TREASURE_KEY_FLAG1 + i)) 
            {
                ++numKeyPieces;
            }			
        }
		_stprintf(szString,"%d",numKeyPieces);
#else
        _stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_TREASURE_KEY_FLAG));
#endif
		//pTextCurKey->SetTextWZ(szString);
		xml.AddChildElem("CurrentPoint", szString);
		_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_TREASURE_KEY_FLAG));
		//pTextMaxKey->SetTextWZ(szString);
		xml.AddChildElem("MaxPoint", szString);

		xml.OutOfElem();
		xml.AddChildElem("Treasure");
		xml.IntoElem();

		GetStringByCode(pInfo->m_TBox_Scode,szString,128);
		//pTextBoxName->SetTextWZA(szString);
		xml.AddChildElem("Name", szString);
		_stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_TREASURE_BOX_FLAG));
		//pTextCurBox->SetTextWZ(szString);
		xml.AddChildElem("CurrentPoint", szString);
		_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_TREASURE_BOX_FLAG));
		//pTextMaxBox->SetTextWZ(szString);
		xml.AddChildElem("MaxPoint", szString);

		xml.OutOfElem();
	}

	xml.OutOfElem();
}

void AreaConquestManager::UpdateReward()
{
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);
	if (!pAreaInfo || !pInfo)
		return;

	xml.AddChildElem("RewardCategory");
	xml.IntoElem();

	float fPercent = pAreaInfo->GetCurrentAchievePoint();	

	int iStringCode = 0;
	TCHAR szReward[256];



	for (int a = 0; a < MAX_AC_DIALOG_REWARD_MAX; a++)
	{
		if(pAreaInfo->GetRewardFlag(a))
		{
			

			RewardInfo *pRewardInfo =  RewardInfoList::Instance()->FindRewardInfo(pInfo->m_wAch_Rew[a]);
			if (pRewardInfo)
			{
#ifdef __NA_1288_AC_RENEWAL_FOR_TIME_LINE
				DWORD dwTypeCode = pRewardInfo->GetTypeCode(0);
				BASE_ITEMINFO* ItemInfo = ItemInfoParser::Instance()->GetItemInfo(dwTypeCode);				
				if(ItemInfo) iStringCode = ItemInfo->m_NCode;
				else iStringCode = 0;
#else
				iStringCode = pRewardInfo->GetDCode();
#endif          
			}
			else
			{
				iStringCode = 0;
			}
			
			if (fPercent >= (a + 1) * 5.0f)
			{
				GetStringByCode(iStringCode,szReward,256);
			}
			else
			{
				GetStringByCode(
					AC_NOT_REVEALED_STRING,
					szReward,
					256);			
			}		
			
			if(0 != stricmp(szReward, "NULL"))
			{
				//	pTextReward->SetTextWZA(szReward);
				xml.AddChildElem("Reward");
				xml.IntoElem();
				xml.AddChildElem("Name", szReward);
				xml.AddChildElem("CurrentPoint");
				xml.AddChildElem("MaxPoint");
				xml.OutOfElem();
			}
		}
	}

	xml.OutOfElem();
}

void AreaConquestManager::UpdateHunting()
{
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);
	if (!pAreaInfo || !pInfo)
		return;

	xml.AddChildElem("HuntingCategory");
	xml.IntoElem();

	xml.AddChildElem("Hunting");
	xml.IntoElem();

	TCHAR szMonsterName[256];
//	_stprintf(szMonsterName,
//		" %d / %d ",
//		pAreaInfo->GetConditionValue(eACC_HUNTING_KILL_CAPTAIN),
//		pAreaInfo->GetConditionMaxValue(eACC_HUNTING_KILL_CAPTAIN));
	//pTextCaptainCount->SetTextWZA(szMonsterName);

//	xml.AddChildElem("Name", "캡틴 베기");
	xml.AddChildElem("Name", STRTBL_PARSER5->GetStringByCode(1));
	xml.AddChildElemEx("CurrentPoint", pAreaInfo->GetConditionValue(eACC_HUNTING_KILL_CAPTAIN));
	xml.AddChildElemEx("MaxPoint", pAreaInfo->GetConditionMaxValue(eACC_HUNTING_KILL_CAPTAIN));

	xml.AddChildElem("Detail");
	xml.IntoElem();
	
	INT pos = 0;
	for (int a = 0; a < MAX_AC_HIDDEN_MONSTER/*MAX_AC_DIALOG_LUCKY_MONSTER_MAX*/; a++) // 히든몹 잡기
	{
		
		int offset = a + pos;

		CONDITION_TEXT stText = GetLuckyMonsterConditionIndex(offset,true);
		if (stText.bText)
		{
			if (stText.iValue > 0)
			{
				GetStringByCode(
					pInfo->m_wHMK_StringC[stText.iCondition],
					szMonsterName,
					256 );
			}
			else
			{
				GetStringByCode(
					AC_NOT_REVEALED_STRING,
					szMonsterName,256);
			}
		}
		else
		{
			_stprintf(szMonsterName, "");
		}


		if(strlen(szMonsterName) > 0 )
		{
			xml.AddChildElem("HuntingObjects_Hidden");
			xml.IntoElem();
			xml.AddChildElem("Name", szMonsterName);

			// 잡은 몬스터수
			int iLuckyMonsterCount = stText.iValue;
			xml.AddChildElemEx("CurrentPoint", iLuckyMonsterCount);
			xml.AddChildElemEx("MaxPoint", stText.iMaxValue);

			xml.OutOfElem();
		}

	}

	pos = 0;
	for (int a = 0; a < MAX_AC_LUCKY_MONSTER/*MAX_AC_DIALOG_LUCKY_MONSTER_MAX*/; a++) // 럭키몹 잡기
	{
		
		int offset = a + pos;

		CONDITION_TEXT stText = GetLuckyMonsterConditionIndex(offset,false);
		if (stText.bText)
		{
			if (stText.iValue > 0)
			{
				GetStringByCode(
					pInfo->m_wLMK_StringC[stText.iCondition],
					szMonsterName,
					256 );
			}
			else
			{
				GetStringByCode(
					AC_NOT_REVEALED_STRING,
					szMonsterName,256);
				//_stprintf(szMonsterName,"????");
			}
		}
		else
		{
			_stprintf(szMonsterName, "");
		}

		//pTextMonsterName->SetTextWZA(szMonsterName);

		if(strlen(szMonsterName) > 0 )
		{
			xml.AddChildElem("HuntingObjects_Lucky");
			xml.IntoElem();
			xml.AddChildElem("Name", szMonsterName);

			// 잡은 몬스터수
			int iLuckyMonsterCount = stText.iValue;
			xml.AddChildElemEx("CurrentPoint", iLuckyMonsterCount);
			xml.AddChildElemEx("MaxPoint", stText.iMaxValue);

			xml.OutOfElem();
		}

		// 10개의 몬스터 버튼들 
/*		int imageindex = 0; 
		for (int b = 0; b < 10; b++)
		{
			if (b < iLuckyMonsterCount)
			{			
				imageindex = pInfo->m_wLMK_ImgC[stText.iCondition];				
			}
			else
			{
				imageindex = 0;
			}
			// SetControlImage(pButtonMonster,imageindex,14,14);
		}
*/                
	}

	xml.OutOfElem(); // Detail

	xml.OutOfElem(); // Hunting

	xml.OutOfElem(); // HuntingCategory
}

void AreaConquestManager::UpdateCollection()
{
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);
	if(!pAreaInfo || !pInfo)
		return;

	xml.AddChildElem("CollectionCategory");
	xml.IntoElem();

//       MapCOLLECTION mapCollection;

        TCHAR szItemName[256];
	CONDITION_TEXT2 stText;

	int pos = 0;
	for (int a = 0; a < MAX_AC_DIALOG_ITEM_MAX; a++)
	{
		int offset = a + pos;
		int iItemCount = 0;
		int iItemKind = eACC_NONE;

		stText = GetItemConditionIndex(offset);
		switch (stText.iType)
		{
		case eACC_STORY:
			{
				if (stText.bText)
				{
					if (stText.iValue > 0)
					{
						GetStringByCode(pInfo->m_bSI_Scode[stText.iCondition],szItemName,256);
					}
					else
					{
						GetStringByCode(AC_NOT_REVEALED_STRING,szItemName,256);
						//_stprintf(szItemName,"????");
					}					
				}
				else
				{
					_stprintf(szItemName,"");
				}

				//pTextCollectionName->SetTextWZA(szItemName);
			}
			break;
		case eACC_HIDDEN:
			{
				if (stText.bText)
				{
					if (stText.iValue > 0)
					{
						GetStringByCode(pInfo->m_bHI_Scode,szItemName,256);
					}
					else
					{
						GetStringByCode(AC_NOT_REVEALED_STRING,szItemName,256);
						//_stprintf(szItemName,"????");
					}					
				}
				else
				{
					_stprintf(szItemName,"");
				}

				// pTextCollectionName->SetTextWZA(szItemName);
			}
			break;
		case eACC_TREASURE:
			{
				BOOL bTest = FALSE;
				for (int test = 0; test < MAX_AC_TREASURE_IMAGE; test++)
				{
#ifdef __NA_0_AC_KEY_DIRECT_REGISTER
					if (pAreaInfo->GetConditionValue(eACC_TREASURE_KEY_FLAG1 + test))
#else
					if (pAreaInfo->GetConditionValue(eACC_TREASURE_ITEM_FLAG1 + test))
#endif
					{
						bTest = TRUE;
						break;
					}
				}

				if (bTest)
				{
					GetStringByCode(pInfo->m_Tresure_Scode,szItemName,256);			
				}
				else
				{
					GetStringByCode(
						AC_NOT_REVEALED_STRING,szItemName,256);
					//_stprintf(szItemName,"????");
				}

				// pTextCollectionName->SetTextWZA(szItemName);
			}
			break;

		case eACC_SPECIAL:
			{
				BOOL bTest = FALSE;
				for (int test = 0; test < MAX_AC_SPECIAL_PRODUCT_IMAGE; test++)
				{
					if (pAreaInfo->GetConditionValue(eACC_TREASURE_SPECIALTY_FLAG1 + test))
					{
						bTest = TRUE;
						break;
					}
				}

				if (bTest)
				{
					GetStringByCode(pInfo->m_Spe_Scode,szItemName,256);			
				}
				else
				{
					GetStringByCode(AC_NOT_REVEALED_STRING,szItemName,256);
					//_stprintf(szItemName,"????");
				}

				// pTextCollectionName->SetTextWZA(szItemName);
			}
			break;
		}

		if(strlen(szItemName) > 0 )
		{
			xml.AddChildElem("Collection");
			xml.IntoElem();
			xml.AddChildElem("Name", szItemName);

			xml.AddChildElemEx("CurrentPoint", stText.iValue);

			int maxCount = stText.iMaxValue;
//			if(stText.iValue%10)
//                                maxCount = (floor( (float)(stText.iValue/10)+1) )*10;
//			else
//				maxCount = (floor(      (float)(stText.iValue/10)  )    )*10;

			xml.AddChildElemEx("MaxPoint",  maxCount);

			xml.OutOfElem();
		}

		ZeroMemory(szItemName, 256);

//		std::string str = szItemName;
//		MapCOLLECTION::iterator iter = mapCollection.find(str);
//
//		COLLECTIONITEM* pColl = NULL;
//		if(mapCollection.end() == iter)
//		{
//			COLLECTIONITEM Coll;
//			Coll.nCurrentCount = 0;
//			Coll.nMaxCount = 0;
//			mapCollection[str] = Coll;
//
//			pColl = &mapCollection[str];
//		}
//		else
//			pColl = &(iter->second);
//
//		pColl->nCurrentCount += stText.iValue;
//		pColl->nMaxCount += 10;

		// 10개의 콜랙션 버튼들
		/*for (int b = 0; b < 10; b++)
		{
			int imageindex = 0;

			switch (stText.iType)
			{
			case eACC_STORY:
				{
					if (b < stText.iValue)
					{
						imageindex = pInfo->m_wSI_ImgC[stText.iCondition];						
					}
				}
				break;
			case eACC_HIDDEN:
				{
					if (b < stText.iValue)
					{
						imageindex = pInfo->m_wHI_ImgC;						
					}
				}
				break;
			case eACC_TREASURE:
				{
					if (b < MAX_AC_TREASURE_IMAGE)
					{				
						if (pAreaInfo->GetConditionValue(eACC_TREASURE_ITEM_FLAG1 + b))
						{
							imageindex = pInfo->m_wTresureImgC[b];							
						}
					}
				}
				break;
			case eACC_SPECIAL:
				{
					if (b < MAX_AC_SPECIAL_PRODUCT_IMAGE)
					{	
						if (pAreaInfo->GetConditionValue(eACC_TREASURE_SPECIALTY_FLAG1 + b))
						{	
							imageindex = pInfo->m_wSpeImgC[b];							
						}
					}
				}
				break;
			}

//			SetControlImage(pButtonItem,imageindex,38,38);			
		}
		*/
	} // for (int a = 0; a < MAX_AC_DIALOG_ITEM_MAX; a++)

	////////////////////////////////////////////
/*	for(MapCOLLECTION::iterator iter = mapCollection.begin(); iter != mapCollection.end(); iter++)
	{
		xml.AddChildElem("Collection");
		xml.IntoElem();

		xml.AddChildElem("Name", iter->first.c_str() );
		xml.AddChildElemEx("CurrentPoint", iter->second.nCurrentCount);
		xml.AddChildElemEx("MaxPoint", iter->second.nMaxCount);

		xml.OutOfElem();
	}
*/
	xml.OutOfElem();
}

void AreaConquestManager::UpdateBoss()
{
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);
	if (!pAreaInfo || !pInfo)
		return;

	xml.AddChildElem("BossCategory");
	
	TCHAR szString[256];

	_stprintf(szString,"%d%%",pInfo->m_wHMK_Achievement[0]);
	//pTextPercent1->SetTextWZA(szString);

	_stprintf(szString,"%d%%",pInfo->m_wHMK_Achievement[1]);
	//pTextPercent2->SetTextWZA(szString);

	int imageindex = 0;
	if (!pAreaInfo->GetConditionValue(eACC_HUNTING_KILL_HIDDENBOSS1))
		//if (pAreaInfo->GetCurrentAchievePoint() < pInfo->m_wHMK_Achievement[0])
	{
		imageindex = pInfo->m_wHMK_ImgC_Off[0];
	}
	else
	{
		imageindex = pInfo->m_wHMK_ImgC_On[0];
	}

//	SetControlImage(pPictureBoss1,imageindex,164,154);

	if (!pAreaInfo->GetConditionValue(eACC_HUNTING_KILL_HIDDENBOSS2))
	{
		imageindex = pInfo->m_wHMK_ImgC_Off[1];
	}
	else
	{
		imageindex = pInfo->m_wHMK_ImgC_On[1];
	}

//	SetControlImage(pPictureBoss2,imageindex,167,162);

	// DIALOG_BOSS_TEXT_NAME1
	if (pAreaInfo->GetCurrentAchievePoint() < pInfo->m_wHMK_Achievement[0])
	{
		GetStringByCode(AC_NOT_REVEALED_STRING,szString,256);
	}
	else
	{
		GetStringByCode(pInfo->m_wHMK_StringC[0],szString,256);
	}

//	pTextName1->SetTextWZA(szString);

	// DIALOG_BOSS_TEXT_NAME2
	if (pAreaInfo->GetCurrentAchievePoint() < pInfo->m_wHMK_Achievement[1])
	{
		GetStringByCode(AC_NOT_REVEALED_STRING,szString,256);
	}
	else
	{
		GetStringByCode(pInfo->m_wHMK_StringC[1],szString,256);
	}

//	pTextName2->SetTextWZ(szString);
}

void AreaConquestManager::UpdateTowerDefense()
{
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);
	if(!pAreaInfo || !pInfo)
		return;

	xml.AddChildElem("TowerDefenseCategory");

	TCHAR szString[256] = {0};

//	CCtrlStaticWZ *pTextTitle = (CCtrlStaticWZ *)GetControlWZ(m_wzId[DIALOG_M006]);
//	CCtrlPictureWZ *pPictureTower = (CCtrlPictureWZ *)GetControlWZ(m_wzId[DIALOG_TOWER_PICTURE]);	
//	CCtrlStaticWZ *pTextPercent = (CCtrlStaticWZ *)GetControlWZ(m_wzId[DIALOG_TOWER_TEXT_PERCENT]);
//	CCtrlStaticWZ *pTextCondition = (CCtrlStaticWZ *)GetControlWZ(m_wzId[DIALOG_TOWER_TEXT_CONDITION]);

	GetStringByCode(AC_NOT_REVEALED_STRING,szString,256);

	// DIALOG_TOWER_TEXT_PERCENT
	// pTextPercent->SetTextWZ(szString);
	
	// DIALOG_TOWER_TEXT_CONDITION
	// pTextCondition->SetTextWZ(szString);
	
	// DIALOG_M006
	// pTextTitle->SetTextWZ(szString);	
}

void AreaConquestManager::UpdateQuest()
{
        // 초급 중급 정예 레어 순서다.
	AreaConquestInfo * pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo * pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);
	if(!pAreaInfo || !pInfo)
		return;

	xml.AddChildElem("QuestCategory");
	xml.IntoElem();

	TCHAR szString[256];
	for (int a = 0; a < MAX_AC_QUEST_CATEGORY; a++)
	{
		xml.AddChildElem("Quest");
		xml.IntoElem();

		switch(a)
		{
		case eACQ_NORMAL:
			{
				xml.AddChildElem("Name", STRTBL_PARSER5->GetStringByCode(2));

				_stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_QUEST_NORMAL));
		//		pTextClearCount->SetTextWZA(szString);
				xml.AddChildElem("CurrentPoint", szString);
				_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_QUEST_NORMAL));
				xml.AddChildElem("MaxPoint", szString);
			}
			break;
		case eACQ_MISSION:
			{
				xml.AddChildElem("Name", STRTBL_PARSER5->GetStringByCode(3));
				_stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_QUEST_MISSION));
		//		pTextClearCount->SetTextWZA(szString);
				xml.AddChildElem("CurrentPoint", szString);
				_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_QUEST_MISSION));
				xml.AddChildElem("MaxPoint", szString);
			}
			break;
		case eACQ_ELITE:
			{
				xml.AddChildElem("Name", STRTBL_PARSER5->GetStringByCode(4));
				_stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_QUEST_EVENT));
		//		pTextClearCount->SetTextWZA(szString);
				xml.AddChildElem("CurrentPoint", szString);
				_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_QUEST_EVENT));
				xml.AddChildElem("MaxPoint", szString);
			}
			break;
		case eACQ_HIDDEN:
			{
				xml.AddChildElem("Name", STRTBL_PARSER5->GetStringByCode(5));
				_stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_QUEST_HIDDEN));
		//		pTextClearCount->SetTextWZA(szString);
				xml.AddChildElem("CurrentPoint", szString);
				_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_QUEST_HIDDEN));
				xml.AddChildElem("MaxPoint", szString);
			}
			break;
		}

		xml.OutOfElem();
	}

	xml.OutOfElem();
}

void AreaConquestManager::UpdateBattle()
{
	// PVP, UP , 길드전 , 가이던스 영웅 , 무차별존 , 아이템 마스터 , 몬스터 마스터의 순서다
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);
	if(!pAreaInfo || !pInfo)
		return;

	xml.AddChildElem("BattleCategory");
	xml.IntoElem();

	TCHAR szString[256];
	for (int a = 0; a < MAX_AC_BATTLE_CATEGORY; a++)
	{
		xml.AddChildElem("Battle");
		xml.IntoElem();

	//	CCtrlStaticWZ *pTextCategory = (CCtrlStaticWZ *)GetControlWZ(m_wzId[DIALOG_BATTLE_TEXT_CATEGORY1 + a]);
	//	CCtrlStaticWZ *pTextClearCount = (CCtrlStaticWZ *)GetControlWZ(m_wzId[DIALOG_BATTLE_TEXT_COUNT1 + a]);
	//	CCtrlPictureWZ *pPictureGuage= (CCtrlPictureWZ *)GetControlWZ(m_wzId[DIALOG_BATTLE_PICTURE_GUAGE1 + a]);

//		RECT rect;
//		rect = pPictureGuage->GetSizeRect();
		float width ;

//		float height = (float)pPictureGuage->GetSize().bottom;

		if (m_fBattleGaugeSize >= 0.0f )
		{
			switch(a)
			{
			case eACB_ITEM_MASTER:
				width = m_fBattleGaugeSize * pAreaInfo->GetItemMasterRatio();
				break;
			case eACB_MONSTER_MASTER:
				width = m_fBattleGaugeSize * pAreaInfo->GetMonsterMasterRatio();
				break;
			default:
				width = m_fBattleGaugeSize * pAreaInfo->GetConditionRatio(eACC_BATTLE_COMPETITIVE_HUNTING + a);				
				break;
			}
//			pPictureGuage->MoveWindowWZ( (float)rect.left, (float)rect.top, width, height );
		}

		if (width > 0.0f)
		{
			const int StringIndex = 91081; 
			GetStringByCode(StringIndex + a, szString,256);
		}
		else
		{
			GetStringByCode(AC_NOT_REVEALED_STRING,szString,256);
			//_stprintf(szString,"????");
		}
		
		// pTextCategory->SetTextWZ(szString);
		xml.AddChildElem("Name", szString);
		xml.AddChildElemEx("CurrentPoint", width);
		xml.AddChildElemEx("MaxPoint", 100);
                
		// 수치 뺌
		//_stprintf(szString," ");
		// pTextClearCount->SetTextWZA(szString);

		xml.OutOfElem();
	}

	xml.OutOfElem();
}

void AreaConquestManager::UpdateMission()
{
    /*
    AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
    BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);	

    if(!pAreaInfo || !pInfo)
        return;
    

    xml.AddChildElem("MissionCategory");
    xml.IntoElem();
    
    DWORD dwMissionCondition[] = 
    {
        eACC_MISSION_CURSE_TOWER,
            eACC_MISSION_MAIN,
            eACC_MISSION_SUB1,
            eACC_MISSION_SUB2,
            eACC_MISSION_SUB3,
            eACC_MISSION_SUB4,	
    };

    int pos = 0;
    TCHAR szString[256];
    for (int a = 1; a < MAX_AC_MISSION_CATEGORY; a++)
    {
        int AC_CONDITION_CATEGORY = (a - 1) + pos;        

        //if (pTextCategory && pTextClearCount && pTextMaxCount)
        {
            int nValue = 0;
            float fPercentage = 0.0f;

            nValue = pAreaInfo->GetConditionMaxValue(dwMissionCondition[AC_CONDITION_CATEGORY]);
            fPercentage = pAreaInfo->GetConditionMaxPercentage(dwMissionCondition[AC_CONDITION_CATEGORY]);

            //if (fPercentage > 0.0f && nValue)
            {
                xml.AddChildElem("Mission");
                xml.IntoElem();

                _stprintf(szString,_T("%d"),nValue);
                //pTextMaxCount->SetTextWZ(szString);
                xml.AddChildElem("MaxPoint", szString);

                _stprintf(szString,_T("%d"),pAreaInfo->GetConditionValue(dwMissionCondition[AC_CONDITION_CATEGORY]));
                //pTextClearCount->SetTextWZ(szString);
                xml.AddChildElem("CurrentPoint", szString);

                // 미션명 출력
                int nNCode = 0;
                if (dwMissionCondition[AC_CONDITION_CATEGORY] == eACC_MISSION_CURSE_TOWER)
                {
                    nNCode = 90165 + (m_iAreaIndex >= 5 ? 4 : (m_iAreaIndex - 1) );
                }
                else
                {
                    MapGroup* pMap = MapInfoParser::Instance()->GetMapGroup(m_iAreaIndex);
                    if (pMap)
                    {
                        sMAPINFO* pMapInfo = pMap->GetMapInfoIndex(AC_CONDITION_CATEGORY - 1);
                        if (pMapInfo)
                        {
                            nNCode = pMapInfo->dwNCode;
                        }                        
                    }					
                }

                //g_InterfaceManager.GetInterfaceString(nNCode, szString, INTERFACE_STRING_LENGTH-1);			
                //pTextCategory->SetTextWZ(szString);
                GetStringByCode(nNCode,szString,256);
                xml.AddChildElem("Name", szString);
                xml.OutOfElem();
            }            		
        }		
    }
    xml.OutOfElem();
    */
    
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);	
	if(!pAreaInfo || !pInfo)
		return;


	xml.AddChildElem("MissionCategory");
	xml.IntoElem();

	TCHAR szString[256];
////////////////0902
#if NATION_BUILD_VERSION >= 902	


	xml.AddChildElem("Mission");
	xml.IntoElem();
	//youngmoon 한글로 메인 퀘스트 하드 코딩 되어있는거 스트링테이블에서 읽도록 수정
    GetStringByCode(1666,szString,	256 );
	xml.AddChildElem("Name", STRTBL_PARSER1->GetStringByCode(1666));    
	_stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_QUEST_NORMAL));
	xml.AddChildElem("CurrentPoint", szString);
	_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_QUEST_NORMAL));
	xml.AddChildElem("MaxPoint", szString);
	xml.OutOfElem();
	
	xml.AddChildElem("Mission");
	xml.IntoElem();
	GetStringByCode(90164 + (m_iAreaIndex > 5 ? 5 : m_iAreaIndex),szString,	256 );
	xml.AddChildElem("Name", szString);//STRTBL_PARSER1->GetStringByCode(90164 + (m_iAreaIndex > 5 ? 5 : m_iAreaIndex) ));
	_stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_MISSION_CURSE_TOWER));
	xml.AddChildElem("CurrentPoint", szString);
	_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_MISSION_CURSE_TOWER));
	xml.AddChildElem("MaxPoint", szString);
	xml.OutOfElem();
	
	for( int i = 0 ; i < MAX_AC_SUB_MISSION_NUM  ; ++i ) //메인(1) + 서브 미션의 합계는 5
	{

		MapGroup* pMap = MapInfoParser::Instance()->GetMapGroup(m_iAreaIndex);
		if(pMap)
		{
			sMAPINFO* pMapInfo = pMap->GetMapInfoIndex(i);
			if(pMapInfo)
			{
				xml.AddChildElem("Mission");
				xml.IntoElem();
				GetStringByCode(pMapInfo->dwNCode,szString,	256 );
				xml.AddChildElem("Name", szString);//STRTBL_PARSER1->GetStringByCode(pMapInfo->dwNCode));
                _stprintf(szString,"%d",pAreaInfo->GetConditionValue(i == 0 ? eACC_MISSION_MAIN : eACC_MISSION_SUB1 + i - 1));
				xml.AddChildElem("CurrentPoint", szString);
				_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(i == 0 ? eACC_MISSION_MAIN : eACC_MISSION_SUB1 + i - 1));
				xml.AddChildElem("MaxPoint", szString);
				xml.OutOfElem();
			}
		}
	}
#else

	

	
//	90165	저주받은탑 대지의시련
//90166	저주받은탑 물의시련
//90167	저주받은탑 불의시련
//90168	저주받은탑 바람의시련
//90169	저주받은탑 암흑의시련
	

///////////////////////////////
	for (int a = 0; a < MAX_AC_MISSION_CATEGORY; a++)// 미션이 3개 이상인데 3개만 보이게 되어있다.
	{
		xml.AddChildElem("Mission");
		xml.IntoElem();

//		CCtrlStaticWZ *pTextCategory = 
//			(CCtrlStaticWZ *)GetControlWZ(m_wzId[DIALOG_MISSION_TEXT_CATEGORY1 + a]);
//		CCtrlStaticWZ *pTextClearCount = 
//			(CCtrlStaticWZ *)GetControlWZ(m_wzId[DIALOG_MISSION_TEXT_CLEARCOUNT1 + a]);
//		CCtrlStaticWZ *pTextMaxCount = 
//			(CCtrlStaticWZ *)GetControlWZ(m_wzId[DIALOG_MISSION_TEXT_MAXCOUNT1 + a]);

		switch (a)
		{
		case eACM_MAIN:
			{
				xml.AddChildElem("Name", STRTBL_PARSER5->GetStringByCode(6));
				
				_stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_MISSION_MAIN));
				//	pTextClearCount->SetTextWZA(szString);
				xml.AddChildElem("CurrentPoint", szString);

				_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_MISSION_MAIN));
			//	pTextMaxCount->SetTextWZA(szString);				
				xml.AddChildElem("MaxPoint", szString);
			}			
			break;
		case eACM_SUB:
			{
				xml.AddChildElem("Name", STRTBL_PARSER5->GetStringByCode(7));
				
				_stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_MISSION_SUB));
				//	pTextClearCount->SetTextWZA(szString);
				xml.AddChildElem("CurrentPoint", szString);

				_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_MISSION_SUB));
			//	pTextMaxCount->SetTextWZA(szString);
				xml.AddChildElem("MaxPoint", szString);
				
			}						
			break;
		case eACM_HIDDEN:
			{
				xml.AddChildElem("Name", STRTBL_PARSER5->GetStringByCode(8));
				
				_stprintf(szString,"%d",pAreaInfo->GetConditionValue(eACC_MISSION_HIDDEN));
				//	pTextClearCount->SetTextWZA(szString);
				xml.AddChildElem("CurrentPoint", szString);

				_stprintf(szString,"%d",pAreaInfo->GetConditionMaxValue(eACC_MISSION_HIDDEN));
			//	pTextMaxCount->SetTextWZA(szString);	
				xml.AddChildElem("MaxPoint", szString);
			}						
			break;
		}

		xml.OutOfElem();
	}
#endif
	xml.OutOfElem();
}

void AreaConquestManager::UpdateEtc()
{
	/////////////////////////////////////
	// <Description>
	xml.AddChildElem("Description");
	xml.IntoElem();

	xml.AddChildElemEx("Index", m_iAreaIndex);

	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	float fAchivePoint = 0.0f;
	if (pAreaInfo)
	{
	
		fAchivePoint = pAreaInfo->CalculateCurrentAchievePoint(); //달성률 달르게 보인다
	}

	TCHAR szMessage[256];
	TCHAR szText2[256];
	_stprintf(szMessage,"%.2f%%", fAchivePoint);

	xml.AddChildElemEx("TotalPoint", fAchivePoint);

	BASE_AreaConquestInfo *pACInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);
	if(pACInfo)
	{
		GetStringByCode(pACInfo->m_wACNCode,szMessage,256);
		//	pTextCurArea->SetTextWZ(szMessage);
	}

	//_stprintf(szText2,m_szAreaFormat,szMessage);
	//pTextAreaName->SetTextWZ(szText2);
	xml.AddChildElem("Name", szMessage);

	//_stprintf(szText2,m_szRewardFormat,szMessage);
	//pTextRewardText->SetTextWZ(szText2);

	xml.OutOfElem();
}

bool AreaConquestManager::SetAreaIndex(int index)
{
	m_iAreaIndex = index;	

	m_iMaxLuckyMonsterLineCount  = 0;
	m_iMaxStoryItemLineCount = 0;

	m_iHiddenItemLineCount = 0;
	m_iTreasureItem = 0;
	m_iSpecialItem = 0;

	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);

	if(!pInfo)
	{
		return false;
	}

	for (int a = 0; a < MAX_AC_LUCKY_MONSTER; a++)
	{
		if (pInfo->m_wLMK_StringC[a])
		{
			int iCount = pInfo->m_bLMK_MaxPt[a];
			while (iCount > 0)
			{
				m_iMaxLuckyMonsterLineCount++;
				iCount -= 10;
			}
		}
	}

	for (int a = 0; a < MAX_AC_STORY_ITEM; a++)
	{
		if (pInfo->m_bSI_Scode[a])
		{
			int iCount = pInfo->m_bSI_Max[a];
			while (iCount > 0)
			{
				m_iMaxStoryItemLineCount++;
				iCount -= 10;
			}
		}
	}

	if (pInfo->m_bHI_Scode)
	{
		int iCount = pInfo->m_bHI_Max;

		while (iCount > 0)
		{
			m_iHiddenItemLineCount++;
			iCount -= 10;
		}
	}

	if (pInfo->m_Tresure_Scode)
	{
		m_iTreasureItem = 1;
	}

	if (pInfo->m_Spe_Scode)
	{
		m_iSpecialItem = 1;
	}	
	return true;
}

//////////////////////////////////////////////////////////////////////////
CONDITION_TEXT AreaConquestManager::GetLuckyMonsterConditionIndex(int offset, bool bHidden)
{
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);

	int iStart = 0;

	CONDITION_TEXT stText;

	stText.iCondition = 0;
	stText.iValue = 0;
	stText.bText = FALSE;
	stText.iMaxValue = 0;

	int nCount = 0;
	if(bHidden)
	{
		nCount = MAX_AC_HIDDEN_MONSTER;
	}
	else
	{
		nCount = MAX_AC_LUCKY_MONSTER;
	}

	for (int a = 0; a < nCount; a++)
	{
		if (pInfo->m_wLMK_StringC[a])
		{
			int iCount = pInfo->m_bLMK_MaxPt[a];

			int iValue = 0;

			if(bHidden)
			{
				iValue = pAreaInfo->GetConditionValue(eACC_HUNTING_KILL_HIDDENBOSS1 + a);
			}
			else
			{
				iValue = pAreaInfo->GetConditionValue(eACC_HUNTING_KILL_LUCKYMONSTER1 + a);
			}

			stText.bText = TRUE;
			stText.iMaxValue = iCount;

			while (iCount > 0)
			{
				if (iStart == offset)
				{
					stText.iCondition = a;
					stText.iValue = iValue;
					
					if (stText.iValue < 0)
					{
						stText.iValue = 0;
					}

					return stText;
				}

				iCount -= 10;
				iValue -= 10;

				iStart++;					
				stText.bText = FALSE;
			}
		}
	}

	return stText;
}

CONDITION_TEXT2    AreaConquestManager::GetItemConditionIndex(int offset)
{
	AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(m_iAreaIndex);
	BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(m_iAreaIndex);

	int iStart = 0;

	CONDITION_TEXT2 stText;

	stText.iType = eACC_NONE;
	stText.iCondition = 0;
	stText.iValue = 0;
	stText.bText = FALSE;
	stText.iMaxValue = 0;

	for (int a = 0; a < MAX_AC_STORY_ITEM; a++)
	{
		if (pInfo->m_bSI_Scode[a])
		{
			int iCount = pInfo->m_bSI_Max[a];
			int iValue = pAreaInfo->GetConditionValue(eACC_COLLECTING_ITEM1 + a);
			int iMaxValue = iCount;

			stText.bText = TRUE;
			stText.iMaxValue = iMaxValue;

			while (iCount > 0)
			{
				if (iStart == offset)
				{
					stText.iType = eACC_STORY;
					stText.iCondition = a;
					stText.iValue = iValue;
					
					if (stText.iValue < 0)
					{
						stText.iValue = 0;
					}

					return stText;
				}

				iCount -= 10;
				iValue -= 10;

				iStart++;					
				stText.bText = FALSE;
			}
		}
	}

	if (pInfo->m_bHI_Scode)
	{
		int iCount = pInfo->m_bHI_Max;
		int iValue = pAreaInfo->GetConditionValue(eACC_COLLECTING_HIDDEN_ITEM);
		int iMaxValue = iCount;

		stText.bText = TRUE;
		stText.iMaxValue = iMaxValue;

		while (iCount > 0)
		{
			if (iStart == offset)
			{
				stText.iType = eACC_HIDDEN;
				stText.iCondition = 0;
				stText.iValue = iValue;
				

				if (stText.iValue < 0)
				{
					stText.iValue = 0;
				}

				return stText;
			}

			iCount -= 10;
			iValue -= 10;

			stText.bText = FALSE;
			iStart++;	
		}
	}

	if (pInfo->m_Tresure_Scode)
	{
		stText.bText = TRUE;
		
		if (iStart == offset)
		{
			stText.iType = eACC_TREASURE;
			stText.iCondition = 0;
			stText.iValue = 0;
			return stText;
		}

		iStart++;	
	}

	if (pInfo->m_Spe_Scode)
	{
		stText.bText = TRUE;

		if (iStart == offset)
		{
			stText.iType = eACC_SPECIAL;
			stText.iCondition = 0;
			stText.iValue = 0;
	
			return stText;
		}

		iStart++;	
	}

	return stText;
}

//////////////////////////////////////////////////////////////////////////


BOOL AreaConquestManager::CompileACStream(BSTR bstrXml)
{
	USES_CONVERSION;

	std::string tmpStr;
	ConvertBSTRToString(bstrXml,tmpStr);	

	CMarkupArchive xml(CMarkupArchive::ArchiveModeNotZipped | CMarkupArchive::ArchiveModeString);
	xml.OpenString(tmpStr.c_str());

//	WriteToFile("C:\\WOPSConfig\\SUNGMS\\zzz.xml", (BYTE*)tmpStr.c_str(), tmpStr.size()+1);

	CHECK(xml.FindElem(_T("ACSystem")), "ACSystem");				//Inventory를 XML스트링에서 찾는다.

	for(int iAreaIndex =1; iAreaIndex <= MAX_AC_SECTION_NUM; iAreaIndex++)
	{
		std::string str;
		DWORD dwCount;

		if(!xml.FindChildElem(_T("Map")) )
			break;

		xml.IntoElem();

		/////////////////////////
		// Description
		/////////////////////////
		CHECK(xml.FindChildElem(_T("Description")),"Description");

		
		AreaConquestInfo *pAreaInfo = GetAreaConquestInfo(iAreaIndex);		
		BASE_AreaConquestInfo *pInfo = AreaConquestInfoParser::Instance()->GetAreaConquestInfo(iAreaIndex);				
		
		/////////////////////////////////
		// Reset all Values
		for( int i = 0 ; i < MAX_AC_STORY_ITEM_NUM ; ++i )
		{
			pAreaInfo->SetConditionValue( eACC_COLLECTING_ITEM1+i, 0);
		}

		for (int a = 0; a < 8; a++)
		{
			pAreaInfo->SetConditionValue(eACC_TREASURE_MAP_FLAG1 + a, 0); // 맵조각 모음.
		}
#ifdef __NA_0_AC_KEY_DIRECT_REGISTER
        for (int i = 0; i < 5; ++i) {
            pAreaInfo->SetConditionValue(eACC_TREASURE_KEY_FLAG1 + i, 0); // 열쇠조각 모음
        }
#endif
		pAreaInfo->SetConditionValue(eACC_TREASURE_KEY_FLAG, 0);
		pAreaInfo->SetConditionValue(eACC_TREASURE_BOX_FLAG, 0);

		for (int a = 0; a < MAX_AC_REWARD_NUM; a++)
		{
			pAreaInfo->SetReward(a, false);
		}

		pAreaInfo->SetConditionValue(eACC_HUNTING_KILL_CAPTAIN, 0); // 캡틴 몬스터

		for (int a = 0; a < MAX_AC_HIDDEN_MONSTER; a++) //히든 몬스터
		{
			pAreaInfo->SetConditionValue(eACC_HUNTING_KILL_HIDDENBOSS1 + a, 0);
		}

		for (int a = 0; a < MAX_AC_LUCKY_MONSTER; a++) //럭키 몬스터
		{
			pAreaInfo->SetConditionValue(eACC_HUNTING_KILL_LUCKYMONSTER1 + a, 0);
		}

		/////////////////////////
		// TreasureCategory
		/////////////////////////
		CHECK(xml.FindChildElem(_T("TreasureCategory")),"TreasureCategory");
		xml.IntoElem();

		DWORD dwTreasurePoint[3];
		for(int n = 0; n < 3; n++)
		{
			xml.FindChildElem(_T("Treasure"));
			xml.IntoElem();
			CHECK(xml.FindGetChildData(_T("CurrentPoint"), dwTreasurePoint[n]),"CurrentPoint");
			xml.OutOfElem();
		}

		int iMapCount = dwTreasurePoint[0];		
		for (int a = 0; a < iMapCount; a++)
		{
			pAreaInfo->SetConditionValue(eACC_TREASURE_MAP_FLAG1 + a, true); // 맵조각 모음.
		}
#ifdef __NA_0_AC_KEY_DIRECT_REGISTER
        // 일단 맵조각 모음처럼 넣긴 하지만..
        // 같은 수라도 어떤 조각을 모았는지에 대한 정보가 리셋되어 버리는 논리적 오류가 있음
        // XML 데이터를 수정하기 전엔 GM 툴에서 이 기능을 쓰는 것 자체가 위험함
        int numKeyPieces = dwTreasurePoint[1];
        for (int i = 0; i < numKeyPieces; ++i) 
        {
            pAreaInfo->SetConditionValue(eACC_TREASURE_KEY_FLAG1 + i, true); // 열쇠조각 모음
            //열쇠조각 오류 수정 eACC_TREASURE_MAP_FLAG1 => eACC_TREASURE_KEY_FLAG1
        }
#else
		pAreaInfo->SetConditionValue(eACC_TREASURE_KEY_FLAG, dwTreasurePoint[1]);
#endif
		pAreaInfo->SetConditionValue(eACC_TREASURE_BOX_FLAG, dwTreasurePoint[2]);

		xml.OutOfElem();

//		_LOG("CompileACStream #TreasureCategory");

		/////////////////////////
		// RewardCategory
		/////////////////////////
		CHECK(xml.FindChildElem(_T("RewardCategory")),"RewardCategory");
		xml.IntoElem();
		
		for (int a = 0; a < MAX_AC_DIALOG_REWARD_MAX; a++)
		{
			if(!xml.FindChildElem(_T("Reward")) )
				break;
			xml.IntoElem();
			CHECK(xml.FindGetChildData(_T("Name"), str),"Name");
			xml.OutOfElem();

			if( 0 ==  _stricmp(str.c_str(), "미확인") )
				continue;
			else
			{
				pAreaInfo->SetReward(a, true);
	//			_LOG(" SetReward  %d -> TRUE", a);
			}
		}
		xml.OutOfElem();

	//	_LOG("CompileACStream #RewardCategory");
		/////////////////////////
		// HuntingCategory
		/////////////////////////
		CHECK(xml.FindChildElem(_T("HuntingCategory")),"HuntingCategory");
		xml.IntoElem();

		xml.FindChildElem(_T("Hunting"));
		xml.IntoElem();
		CHECK(xml.FindGetChildData(_T("Name"), str),"Name");
		CHECK(xml.FindGetChildData(_T("CurrentPoint"), dwCount),"CurrentPoint");
		pAreaInfo->SetConditionValue(eACC_HUNTING_KILL_CAPTAIN, dwCount); // 캡틴 몬스터
		CHECK(xml.FindGetChildData(_T("MaxPoint"), dwCount),"MaxPoint");
		pAreaInfo->SetConditionMaxValue(eACC_HUNTING_KILL_CAPTAIN, dwCount); // 캡틴 몬스터
		
	//	_LOG(" eACC_HUNTING_KILL_CAPTAIN  %d", dwCount);

		xml.FindChildElem(_T("Detail"));
		xml.IntoElem();

		// 히든 몬스터
		for (int a = 0; a < MAX_AC_HIDDEN_MONSTER; a++)
		{
			if(!xml.FindChildElem(_T("HuntingObjects_Hidden")) )
				break;
			
			xml.IntoElem();
			CHECK(xml.FindGetChildData(_T("CurrentPoint"), dwCount),"CurrentPoint");
			xml.OutOfElem();
			pAreaInfo->SetConditionValue(eACC_HUNTING_KILL_HIDDENBOSS1 + a, dwCount);
		}

		// 럭키 몬스터
		for (int a = 0; a < MAX_AC_LUCKY_MONSTER; a++)
		{
			if(!xml.FindChildElem(_T("HuntingObjects_Lucky")) )
				break;
			
			xml.IntoElem();
			CHECK(xml.FindGetChildData(_T("CurrentPoint"), dwCount),"CurrentPoint");
			xml.OutOfElem();
			pAreaInfo->SetConditionValue(eACC_HUNTING_KILL_LUCKYMONSTER1 + a, dwCount);
		}

		xml.OutOfElem(); // Detail

		xml.OutOfElem(); // Hunting

		xml.OutOfElem(); // HuntingCategory

	//	_LOG("CompileACStream #HuntingCategory");

		/////////////////////////
		// CollectionCategory
		/////////////////////////
		CHECK(xml.FindChildElem(_T("CollectionCategory")),"CollectionCategory");
		xml.IntoElem();

		for(int a = 0; a < MAX_AC_STORY_ITEM_NUM; a++)
		{
			xml.FindChildElem(_T("Collection"));
			xml.IntoElem();
			CHECK(xml.FindGetChildData(_T("Name"), str),"Name");
			CHECK(xml.FindGetChildData(_T("CurrentPoint"), dwCount),"CurrentPoint");
			xml.OutOfElem();

//			if( 0 ==  _stricmp(str.c_str(), "미확인") )
//				continue;

			pAreaInfo->SetConditionValue( eACC_COLLECTING_ITEM1+a, dwCount);
		}

		xml.OutOfElem();

	//	_LOG("CompileACStream #CollectionCategory");

		/////////////////////////
		// BossCategory 
		/////////////////////////
		CHECK(xml.FindChildElem(_T("BossCategory")),"BossCategory");
//		xml.IntoElem();
//		xml.OutOfElem();

		/////////////////////////
		// TowerDefenseCategory
		/////////////////////////

		CHECK(xml.FindChildElem(_T("TowerDefenseCategory")),"TowerDefenseCategory");
//		xml.IntoElem();	
//		xml.OutOfElem();

		/////////////////////////
		// QuestCategory 
		/////////////////////////
		CHECK(xml.FindChildElem(_T("QuestCategory")),"QuestCategory");
//		xml.IntoElem();
//		xml.OutOfElem();

		/////////////////////////
		// BattleCategory 
		/////////////////////////
		CHECK(xml.FindChildElem(_T("BattleCategory")),"BattleCategory");
//		xml.IntoElem();
//		xml.OutOfElem();

		/////////////////////////
		// MissionCategory 
		/////////////////////////
		CHECK(xml.FindChildElem(_T("MissionCategory")),"MissionCategory");
//		xml.IntoElem();

		xml.OutOfElem();
	}	

	return TRUE;
}

#endif //#ifndef C_NA_0_20100520_ACSYSTEM_CHANGED

