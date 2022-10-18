//------------------------------------------------------------------------------
//  uiBattleZoneReadyCHuntingConfig_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "uiBattleZoneReadyCHuntingConfig.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "MapInfoParser.h"
#include "RequireFieldLevelInfoParser.h"
#include "BattleScene.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
/**
    sort utility
*/

bool _sort_chunting_index_low_to_high(stCHunting a, stCHunting b)
{
    return (a.iRequireMinLV  < b.iRequireMinLV);
}
//------------------------------------------------------------------------------
/**
*/
bool
uiBattleZoneReadyCHuntingConfig::Clear()
{
    if (this->GetManager())
    {
        BattleLobby * pBattleDlg = (BattleLobby *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_LOBBY);
        if (pBattleDlg)
        {
            this->GetManager()->SetBattleLobby(pBattleDlg);
        }

        m_cfgs.clear();

    #ifdef _INTERNATIONAL_UI
	    m_pTxtControl->SetFontID(StrToWzID("st10"));
    #else
	    m_pTxtControl->SetFontID(StrToWzID("mn12"));
    #endif//_INTERNATIONAL_UI

        this->uif_ListGen();

        if (m_vecCHuntingMapIndex.size() == 0)
        {
            return false;
        }

	    m_cfgs.fieldType = m_vecCHuntingMapIndex[0].wfieldType;
	    m_cfgs.fieldIndex = (WORD)m_vecCHuntingMapIndex[0].wIndex;

	    if (this->GetManager()->IsReadyToModifyCHuntingCfg())
        {
            eHUNTING_MONSTER_DIFFICULT  eDIFF   = (eHUNTING_MONSTER_DIFFICULT)m_cfgs.diffcultType;
            eHUNTING_BONUS_TYPE         eBONUS  = (eHUNTING_BONUS_TYPE)m_cfgs.bonusType;
            eCOMPETITION_RULE_TYPE      eRULE   = (eCOMPETITION_RULE_TYPE)m_cfgs.compRuleType;
            eCOMPETITION_MODE_TYPE      eMODE   = (eCOMPETITION_MODE_TYPE)m_cfgs.compModeType;

            if (!GENERALPARAM->IsTestUIScene())
                this->GetManager()->ModifyCHuntingCfg(m_cfgs.fieldType,
			                                        eDIFF,
                                                    eBONUS,
                                                    eRULE,
                                                    eMODE);
        }

        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneMan*
uiBattleZoneReadyCHuntingConfig::GetManager()
{
    uiBattleZoneMan* battleZoneMan =
        static_cast<uiBattleZoneMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLEZONE));
    assert (battleZoneMan);
    return battleZoneMan;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::uif_ListGen()
{
	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR   szText[INTERFACE_STRING_LENGTH] = {0,};

    m_pLstControl->SetColorSelBar(RGBA(0,0,0,0));
#ifdef _INTERNATIONAL_UI
	m_pLstControl->SetStartLineGapTop(2);
	m_pLstControl->SetSellHeight(19);
#else
	m_pLstControl->SetSellHeight(20);
#endif//_INTERNATIONAL_UI
    m_pLstControl->SetUseReport(false);
    m_pLstControl->SetReportUseCnt(0);
    m_pLstControl->SetHideReportRender(true);

    m_pLstControl->DeleteItemAll();
    m_pLstControl->SetBeginDrawIndex(0);

    const sMAPINFO* mapInfo;

    if (GENERALPARAM->IsTestUIScene())
        mapInfo = MapInfoParser::Instance()->FindMapInfo(30001);
    else
        mapInfo = MapInfoParser::Instance()->FindMapInfo(static_cast<MAPCODE>(BattleScene::GetPendingBattleRoomEntry()->GetMapCode()));

    if (!mapInfo)
        return;

	// ¸Ê ±×·ìÀ» °¡Á®¿Í¼­ sort
	MapGroup * pMapGroup = MapInfoParser::Instance()->GetMapGroup( mapInfo->MapKind );
    if ( !pMapGroup ) 
    {
        assert (pMapGroup);
        return;
    }

    sMAPINFO_HASH & mapHash = pMapGroup->GetMapInfoHash();
	
	// °æÀïÇåÆÃ ¸Ê sort
	m_vecCHuntingMapIndex.clear();
	sMAPINFO * pMapInfo = NULL;
	BASE_RequireFieldLevelInfo * pInfo = NULL;
	int iRequireMinLV = 0;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	BOOL FoundGuildPVP = FALSE;
	BASE_ROOMINFO * pRoomInfo =	GetManager()->GetBaseRoomInfo();
	if( pRoomInfo )
	{
		if (GetManager()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP) )
		{
			FoundGuildPVP = TRUE;
		}
	}
#endif
	stCHunting sCh;

	sMAPINFO_HASH::const_iterator itr = mapHash.begin();

	while( itr != mapHash.end() )
	{
		pMapInfo =  itr->second;
		itr++;
		pInfo = RequireFieldLevelInfoParser::Instance()->GetRequireFieldLevelInfo(
										(WORD)pMapInfo->MapCode,
										(BYTE)0);

		iRequireMinLV = 0;
		if (pInfo)
		{
			iRequireMinLV = pInfo->m_wMinlv;
		}
		else
		{
			iRequireMinLV = pMapInfo->minLV;
		}

		ZeroMemory(&sCh, sizeof(sCh));
		sCh.wIndex = (pMapInfo->MapCode % 40000) - 1;
		sCh.MapCode = pMapInfo->MapCode;
		sCh.iRequireMinLV = iRequireMinLV;
		sCh.wfieldType = pMapInfo->byMType;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		if(FoundGuildPVP && pMapInfo->EntranceItemCode == 0)
			continue;
#endif
		m_vecCHuntingMapIndex.push_back(sCh);

	}

	std::sort( 
			m_vecCHuntingMapIndex.begin(), 
			m_vecCHuntingMapIndex.end(), 
			_sort_chunting_index_low_to_high
			);
	

	CODETYPE fieldCode;
	const sMAPINFO* fieldMapInfo;

	for(size_t i=0; i<m_vecCHuntingMapIndex.size(); ++i)
	{
		fieldCode = m_vecCHuntingMapIndex[i].MapCode;
		fieldMapInfo = MapInfoParser::Instance()->FindMapInfo( static_cast<MAPCODE>(fieldCode));

		if (!fieldMapInfo)
		{
			Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("Field %d"), i);
			m_cfgs.vstrFieldNames[i] = szMessage;
			continue;
		}

		int idx = m_pLstControl->InsertItemBack();

		g_InterfaceManager.GetInterfaceString( fieldMapInfo->dwNCode, szText, INTERFACE_STRING_LENGTH );
		Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s"), szText);

#ifdef _INTERNATIONAL_UI
		DWORD dwFont  = StrToWzID("st10");
#else
		DWORD dwFont  = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
		GlobalFunc::SetItem(m_pLstControl,
			idx,
			szMessage,
			dwFont,
			DT_CENTER | DT_VCENTER,
			RGBA(0,0,0,0),              // BKColor
			RGBA(255, 255, 255, 255),    // TextColor
			RGBA(255, 255, 255, 255)    // SelTextColor
			);

		m_cfgs.vstrFieldNames[i] = szMessage;
	}

    m_pLstControl->ShowWindowWZ(WZ_HIDE);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::updateUIField()
{
    if (m_bShowFieldList)
    {
        if (m_pLstControl->GetState() == WZ_HIDE)
        {
            m_pLstControl->ShowWindowWZ(WZ_SHOW);
            m_pLstControl->SetCurSel(-1);

            for (int i=POS_BTN_BEGIN+1; i<POS_BTN_END; ++i)
            {
                m_pBtnControls[i]->ShowWindowWZ(WZ_HIDE);
            }
        }
    }
    else
    {
        m_pLstControl->ShowWindowWZ(WZ_HIDE);

        for (int i=POS_BTN_BEGIN+1; i<POS_BTN_END; ++i)
        {
            m_pBtnControls[i]->ShowWindowWZ(WZ_SHOW);
        }
    }

    if (m_bShowFieldList)
    {
        int curSel = this->m_pLstControl->GetCurSel();
        if (curSel != -1)
        {
            if (this->GetManager()->IsReadyToModifyCHuntingCfg())
            {
				int iIndex = (int)(m_vecCHuntingMapIndex[curSel].wfieldType);
				
				eHUNTING_MONSTER_DIFFICULT  eDIFF   = (eHUNTING_MONSTER_DIFFICULT)m_cfgs.diffcultType;
                eHUNTING_BONUS_TYPE         eBONUS  = (eHUNTING_BONUS_TYPE)m_cfgs.bonusType;
                eCOMPETITION_RULE_TYPE      eRULE   = (eCOMPETITION_RULE_TYPE)m_cfgs.compRuleType;
                eCOMPETITION_MODE_TYPE      eMODE   = (eCOMPETITION_MODE_TYPE)m_cfgs.compModeType;

                if (!GENERALPARAM->IsTestUIScene())
                    this->GetManager()->ModifyCHuntingCfg(iIndex,
                                                        eDIFF,
                                                        eBONUS,
                                                        eRULE,
                                                        eMODE);
            }

            m_bShowFieldList = false;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::updateFieldName()
{
	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

	for(size_t i=0; i<m_vecCHuntingMapIndex.size(); ++i)
	{
		if(m_vecCHuntingMapIndex[i].wfieldType == m_cfgs.fieldType)
		{
			Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s"), m_cfgs.vstrFieldNames[i].c_str());
			GlobalFunc::SetCaption(m_pTxtControl, szMessage);
			break;
		}
	}
}

//------------------------------------------------------------------------------
/**
    DUAL_PARTY, FREE FOR ALL
*/
void
uiBattleZoneReadyCHuntingConfig::updateCompRuleType()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH];

    if (m_cfgs.compRuleType == eCOMPETITION_RULE_DUAL_PARTY)
    {
        g_InterfaceManager.GetInterfaceString(711, szMessage, INTERFACE_STRING_LENGTH);
    }
    else
    {
        g_InterfaceManager.GetInterfaceString(712, szMessage, INTERFACE_STRING_LENGTH);
    }
    
    GlobalFunc::SetCaption(m_pBtnControls[1], szMessage);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::updateCompModeType()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH];

    if (m_cfgs.compModeType == eCOMPETITION_MODE_SURVIVAL)
    {
        g_InterfaceManager.GetInterfaceString(714, szMessage, INTERFACE_STRING_LENGTH);
    }
    else if(m_cfgs.compModeType == eCOMPETITION_MODE_TIME_ATTACK)
    {
        g_InterfaceManager.GetInterfaceString(715, szMessage, INTERFACE_STRING_LENGTH);
    }
	else
	{
		g_InterfaceManager.GetInterfaceString(9311, szMessage, INTERFACE_STRING_LENGTH);
	}

    GlobalFunc::SetCaption(m_pBtnControls[4], szMessage);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::doRuleMoveLeft()
{
    if (!m_cfgs.compRuleType)
        return;

    if (this->GetManager()->IsReadyToModifyCHuntingCfg())
    {
        eHUNTING_MONSTER_DIFFICULT  eDIFF   = (eHUNTING_MONSTER_DIFFICULT)m_cfgs.diffcultType;
        eHUNTING_BONUS_TYPE         eBONUS  = (eHUNTING_BONUS_TYPE)m_cfgs.bonusType;
        eCOMPETITION_RULE_TYPE      eRULE   = (eCOMPETITION_RULE_TYPE)(m_cfgs.compRuleType - 1);
        eCOMPETITION_MODE_TYPE      eMODE   = (eCOMPETITION_MODE_TYPE)m_cfgs.compModeType;

        if (!GENERALPARAM->IsTestUIScene())
            this->GetManager()->ModifyCHuntingCfg(m_cfgs.fieldType,
			                                    eDIFF,
                                                eBONUS,
                                                eRULE,
                                                eMODE);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::doRuleMoveRight()
{
    if (!(m_cfgs.compRuleType + 1>(eCOMPETITION_RULE_MAX-1)))
        if (this->GetManager()->IsReadyToModifyCHuntingCfg())
        {
            eHUNTING_MONSTER_DIFFICULT  eDIFF   = (eHUNTING_MONSTER_DIFFICULT)m_cfgs.diffcultType;
            eHUNTING_BONUS_TYPE         eBONUS  = (eHUNTING_BONUS_TYPE)m_cfgs.bonusType;
            eCOMPETITION_RULE_TYPE      eRULE   = (eCOMPETITION_RULE_TYPE)(m_cfgs.compRuleType + 1);
            eCOMPETITION_MODE_TYPE      eMODE   = (eCOMPETITION_MODE_TYPE)m_cfgs.compModeType;

            if (!GENERALPARAM->IsTestUIScene())
                this->GetManager()->ModifyCHuntingCfg(m_cfgs.fieldType,
				                                      eDIFF,
                                                      eBONUS,
                                                      eRULE,
                                                      eMODE);
        }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::doPVPMoveLeft()
{
    if (this->GetManager()->IsReadyToModifyCHuntingCfg())
    {
        eHUNTING_MONSTER_DIFFICULT  eDIFF   = (eHUNTING_MONSTER_DIFFICULT)m_cfgs.diffcultType;
        eHUNTING_BONUS_TYPE         eBONUS  = (eHUNTING_BONUS_TYPE)m_cfgs.bonusType;
        eCOMPETITION_RULE_TYPE      eRULE   = (eCOMPETITION_RULE_TYPE)m_cfgs.compRuleType;

		eCOMPETITION_MODE_TYPE      eMODE   = (eCOMPETITION_MODE_TYPE)(m_cfgs.compModeType - 1);

#ifdef	__NA_000000_20070528_TAIWAN_GUILD_PVP
		if( eMODE == 0 && GetManager()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP) )
		{
			eMODE = (eCOMPETITION_MODE_TYPE)(eCOMPETITION_MODE_MAX-1);
		}
#endif
		if(eMODE < 0)
		{
			eMODE = (eCOMPETITION_MODE_TYPE)(eCOMPETITION_MODE_MAX-1);
		}

		m_cfgs.compModeType = eMODE;

        if (!GENERALPARAM->IsTestUIScene())
            this->GetManager()->ModifyCHuntingCfg(m_cfgs.fieldType,
			                                      eDIFF,
                                                  eBONUS,
                                                  eRULE,
                                                  eMODE);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::doPVPMoveRight()
{
    if (this->GetManager()->IsReadyToModifyCHuntingCfg())
    {
        eHUNTING_MONSTER_DIFFICULT  eDIFF   = (eHUNTING_MONSTER_DIFFICULT)m_cfgs.diffcultType;
        eHUNTING_BONUS_TYPE         eBONUS  = (eHUNTING_BONUS_TYPE)m_cfgs.bonusType;
        eCOMPETITION_RULE_TYPE      eRULE   = (eCOMPETITION_RULE_TYPE)m_cfgs.compRuleType;
        eCOMPETITION_MODE_TYPE      eMODE   = (eCOMPETITION_MODE_TYPE)(m_cfgs.compModeType + 1);

		if(eMODE >= eCOMPETITION_MODE_MAX)
		{
#ifdef	__NA_000000_20070528_TAIWAN_GUILD_PVP
			if( GetManager()->IsSpecialType(eROOM_SPECIAL_GUILD_PVP) )
				eMODE =  eCOMPETITION_MODE_TIME_ATTACK;
			else
#endif
				eMODE =  eCOMPETITION_MODE_SURVIVAL;
		}

		m_cfgs.compModeType = eMODE;

        if (!GENERALPARAM->IsTestUIScene())
            this->GetManager()->ModifyCHuntingCfg(m_cfgs.fieldType,
			                                        eDIFF,
                                                    eBONUS,
                                                    eRULE,
                                                    eMODE);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::SetCfg_FieldByMapCode(MAPCODE mapCode)
{
    const sMAPINFO* mapInfo;

    if (GENERALPARAM->IsTestUIScene())
        mapInfo = MapInfoParser::Instance()->FindMapInfo(30001);
    else
        mapInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);

    if (!mapInfo)
        return;

    m_cfgs.fieldType = mapInfo->byMType;
	m_cfgs.fieldIndex = (mapCode % 40000) - 1;

    
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
