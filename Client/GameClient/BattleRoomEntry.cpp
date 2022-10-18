//------------------------------------------------------------------------------
//	BattleRoomEntry
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "battleroomentry.h"
#include "MapInfoParser.h"
#include "interfacemanager.h"

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
#include "uibattlezoneman/uiBattleZoneMan.h"
#endif

#include "ItemShareSystem.h"
#include "uiPartyMan/uiPartyMan.h"

BattleRoomEntry::BattleRoomEntry() : 
	m_dwSerial(0), 
	m_MapCode(0), 
	m_FieldCode(0)								
{
	Init();
}


void BattleRoomEntry::Init()
{
	m_dwSerial			= 0;
	m_MapCode			= 0;
	m_FieldCode			= 0;
	m_eType				= eZONETYPE_CHUNTING;
	m_ePublic			= eZONEPUBLIC_PUBLIC;		
	m_pszRoomTitle[0]	= NULL;	
	m_pszRoomPW[0]		= NULL;	

	m_RoomInfo.m_ClassLimit			= ( eCLASS_BERSERKER	| 
										eCLASS_DRAGON		|	
										eCLASS_SHADOW		|	
									    eCLASS_VALKYRIE		| 
										eCLASS_ELEMENTALIST );

	m_RoomInfo.m_MinLV				= 0;
	m_RoomInfo.m_MaxLV				= 0;
	m_RoomInfo.m_SpecialType		= 0;


	m_AdditionalInfo.m_Difficulty	= (BYTE)eHUNTING_MONSTER_DIFFICULT_EASY;
	m_AdditionalInfo.m_Bonus		= (BYTE)eHUNTING_BONUS_TYPE_ITEM;

	this->m_AdditionalPVPInfo.m_Rule = ePVP_RULE_10;
	this->m_AdditionalPVPInfo.m_Mode = ePVP_PERSONAL_MODE;

	ItemShareSystem* pItemShare = GET_CAST_DIALOG(ItemShareSystem, IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM);
	if(pItemShare)
	{
		pItemShare->SetDisplayDivOpt(false);	// 파티 분배 메시지 출력 불가능하도록 설정
	}
}


BattleRoomEntry & 
BattleRoomEntry::operator=(BattleRoomEntry & rEntry )
{
	memcpy( 
		&this->m_dwSerial, 
		&rEntry.m_dwSerial, 
		sizeof(BattleRoomEntry) );

	return *this;
}


void 
BattleRoomEntry::SetSerial(DWORD dwSerial)
{
	this->m_dwSerial = dwSerial;
}

void	
BattleRoomEntry::SetMapCode(CODETYPE MapCode)
{
	const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo((MAPCODE)MapCode);
	if (!pMapInfo)
	{
		assert(!"멥 코드가 잘못되었습니다. WORLD.TXT 를 확인하세요!");
		return;
	}

	this->m_MapCode = MapCode;
}


void	
BattleRoomEntry::SetFieldCode(CODETYPE FieldCode)
{
	sFIELDINFO * pMapInfo = MapInfoParser::Instance()->GetFieldInfo(FieldCode);
	if (!pMapInfo)
	{
		assert(!"멥 코드가 잘못되었습니다. WORLD.TXT 를 확인하세요!");
		return;
	}

	this->m_FieldCode = FieldCode;
}

void
BattleRoomEntry::SetType(eZONETYPE eType)
{
	this->m_eType = eType;
}


void	
BattleRoomEntry::SetPublic(eZONEPUBLIC ePublic)
{
	this->m_ePublic = ePublic;
}

void	
BattleRoomEntry::SetTitle(char * pszTitle)
{
	if (!pszTitle)
	{
		assert(pszTitle);
		return;
	}

	StrnCopy( this->m_pszRoomTitle, pszTitle, MAX_ROOMTITLE_LENGTH);
}

void	
BattleRoomEntry::SetPassword(char * pszPassWord)
{
	if (!pszPassWord)
	{
		assert(pszPassWord);
		return;
	}

	StrnCopy( this->m_pszRoomPW, pszPassWord, MAX_ROOMPASSWORD_LENGTH);
}

void	
BattleRoomEntry::SetBaseInfo(BASE_ROOMINFO * pBaseRoomInfo)
{
	if (!pBaseRoomInfo)
	{
		assert(pBaseRoomInfo);
		return;
	}
	
	memcpy( &this->m_RoomInfo, pBaseRoomInfo, sizeof(m_RoomInfo));
}

void	
BattleRoomEntry::SetAdditionalPVPInfo(ADDITIONAL_PVPINFO * pAdditionalInfo)
{
	memcpy (
		&this->m_AdditionalPVPInfo,
		pAdditionalInfo,
		sizeof(this->m_AdditionalPVPInfo));

	this->SetFieldIndex(0);
}

void
BattleRoomEntry::SetAdditionalCHuntingInfo(ADDITIONAL_CHUNTINGINFO * pAdditionalInfo)
{
    memcpy (
        &this->m_AdditionalCHuntingInfo,
        pAdditionalInfo,
        sizeof(this->m_AdditionalCHuntingInfo));

    this->SetFieldIndex(0);
}

void	
BattleRoomEntry::SetMinLevel(BYTE byLevel)
{
	m_RoomInfo.m_MinLV = byLevel;
}

void	
BattleRoomEntry::SetMaxLevel(BYTE byLevel)
{
	m_RoomInfo.m_MaxLV = byLevel;
}

void	
BattleRoomEntry::SetMinPlayer(BYTE byNumPlayers)
{
	m_iMinUser = byNumPlayers;
}

void	
BattleRoomEntry::SetMaxPlayer(BYTE byNumPlayers)
{
	m_iMaxUser = byNumPlayers;
}


void	
BattleRoomEntry::SetClassLimit(eCLASS_TYPE eClassPermition )
{
    this->m_RoomInfo.m_ClassLimit = eClassPermition;
}


//----------------------------------------------------------------------------
/**
	@remark 
	 - [prev] : 정상적인 MapCode가 셋되고 난후에 의미가 있다.
	 - 필드 인덱스를 선택하면 그에 상응한 FieldCode를 셋해준다.
*/
void	
BattleRoomEntry::SetFieldIndex(BYTE	byFieldIndex)
{
	assert( byFieldIndex < sMAPINFO::MAX_FIELD_NUM );

	const sMAPINFO * pMapInfo = this->GetCurMapInfo();
	if (!pMapInfo)
	{
		assert(!"멥 코드가 잘못되었습니다. WORLD.TXT 를 확인하세요!");
		return;
	}

	const sFIELDINFO * pFieldInfo = MapInfoParser::Instance()->GetFieldInfo(pMapInfo->FCode[byFieldIndex]);
	if (!pFieldInfo)
	{
		assert(!"필드 인덱스에 코드가 잘못되었습니다. FIELD.TXT 를 확인하세요!");
		return;
	}

    SetFieldCode(pMapInfo->FCode[byFieldIndex]);	
}

void	
BattleRoomEntry::SetDifficult(eHUNTING_MONSTER_DIFFICULT eMonsterDifficult)
{
	this->m_AdditionalInfo.m_Difficulty = (BYTE)eMonsterDifficult;
}


void	
BattleRoomEntry::SetBonusType(eHUNTING_BONUS_TYPE eBonusType)
{
	this->m_AdditionalInfo.m_Bonus = (BYTE)eBonusType;
}
void	
BattleRoomEntry::SetPVPRule(ePVP_RULE_TYPE eRule)
{
	this->m_AdditionalPVPInfo.m_Rule = eRule;
}

void	
BattleRoomEntry::SetPVPMode(ePVP_MODE_TYPE eMode)
{
	this->m_AdditionalPVPInfo.m_Mode = (BYTE)eMode;
}

const DWORD 				
BattleRoomEntry::GetSerial()
{
	return this->m_dwSerial;
}

const CODETYPE 
BattleRoomEntry::GetMapCode()
{
	return this->m_MapCode;
}

const CODETYPE 
BattleRoomEntry::GetFieldCode()
{
	return this->m_FieldCode;
}

const eZONETYPE 
BattleRoomEntry::GetType()
{
	return this->m_eType;
}


const eZONEPUBLIC			
BattleRoomEntry::GetPublic()
{
	return this->m_ePublic;
}


const TCHAR *						
BattleRoomEntry::GetName()
{
	const sMAPINFO * pMapInfo = NULL;

	if ( pMapInfo = this->GetCurMapInfo() )
	{
		g_InterfaceManager.GetInterfaceString( pMapInfo->dwNCode, this->m_pszRoomName, MAX_MAPNAME_LENGTH);
	}
	else
	{
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];

		//	잘못된 멥 이름입니다.
		const int	c_iInvalid_Map_name = 70422;
		g_InterfaceManager.GetInterfaceString( c_iInvalid_Map_name, szMessage, INTERFACE_STRING_LENGTH );
		strncpy(this->m_pszRoomName, szMessage, _tcslen(szMessage));
	}

	return this->m_pszRoomName;
}

const TCHAR * 				
BattleRoomEntry::GetTitle()
{
	return this->m_pszRoomTitle;
}

const char *						
BattleRoomEntry::GetPassword()
{
	return this->m_pszRoomPW;
}

const BASE_ROOMINFO *		
BattleRoomEntry::GetBaseInfo(BASE_ROOMINFO * OUT pBaseRoomInfo)
{
	if (pBaseRoomInfo)
	{
		memcpy(pBaseRoomInfo, &this->m_RoomInfo, sizeof(m_RoomInfo));
	}

	return &this->m_RoomInfo;
}

const ADDITIONAL_ROOMINFO * 
BattleRoomEntry::GetAdditionalInfo(ADDITIONAL_ROOMINFO * OUT pAdditionalInfo)
{
	if (pAdditionalInfo)
	{
		memcpy(pAdditionalInfo, &this->m_AdditionalInfo, sizeof(m_AdditionalInfo));
	}

	return &this->m_AdditionalInfo;
}


const ADDITIONAL_PVPINFO *			
BattleRoomEntry::GetAdditionalPVPInfo(ADDITIONAL_PVPINFO * OUT pAdditionalInfo)
{
	if (pAdditionalInfo)
	{
		memcpy(
			pAdditionalInfo, 
			&this->m_AdditionalPVPInfo, 
			sizeof(m_AdditionalPVPInfo));
	}

	return &this->m_AdditionalPVPInfo;
}


const ADDITIONAL_CHUNTINGINFO *			
BattleRoomEntry::GetAdditionalCHuntingInfo(ADDITIONAL_CHUNTINGINFO * OUT pAdditionalInfo)
{
    if (pAdditionalInfo)
    {
        memcpy(
            pAdditionalInfo, 
            &this->m_AdditionalCHuntingInfo, 
            sizeof(m_AdditionalCHuntingInfo));
    }

    return &this->m_AdditionalCHuntingInfo;
}

const BYTE 
BattleRoomEntry::GetMinLevel()
{
	return this->m_RoomInfo.m_MinLV;
}


const BYTE					
BattleRoomEntry::GetMaxLevel()
{
	return this->m_RoomInfo.m_MaxLV;
}


const BYTE							
BattleRoomEntry::GetMinPlayer()
{
	return this->m_iMinUser;
}


const BYTE					
BattleRoomEntry::GetMaxPlayer()
{
	return this->m_iMaxUser;
}

const eCLASS_TYPE 
BattleRoomEntry::GetClassLimit()
{
	return (eCLASS_TYPE)this->m_RoomInfo.m_ClassLimit;
}


const BYTE							
BattleRoomEntry::GetFieldIndex()
{
	const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( (MAPCODE)GetMapCode() ); 
	if ( pMapInfo )
	{
		return pMapInfo->byMType;
	}
	else
	{
		return 0;
	}
}


const eHUNTING_MONSTER_DIFFICULT		
BattleRoomEntry::GetDifficult()
{
	return (eHUNTING_MONSTER_DIFFICULT)this->m_AdditionalInfo.m_Difficulty;
}



const eHUNTING_BONUS_TYPE			
BattleRoomEntry::GetBonusType()
{
	return (eHUNTING_BONUS_TYPE)this->m_AdditionalInfo.m_Bonus;
}

const ePVP_RULE_TYPE						
BattleRoomEntry::GetPVPRule()
{
	return (ePVP_RULE_TYPE)this->m_AdditionalPVPInfo.m_Rule;
}


const ePVP_MODE_TYPE						
BattleRoomEntry::GetPVPMode()
{
	return (ePVP_MODE_TYPE)this->m_AdditionalPVPInfo.m_Mode;
}

const sMAPINFO *					
BattleRoomEntry::GetCurMapInfo()
{
	const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo((MAPCODE)m_MapCode);
	return pMapInfo;
}

const sFIELDINFO *					
BattleRoomEntry::GetCurFieldInfo()
{
	sFIELDINFO * pFieldInfo = MapInfoParser::Instance()->GetFieldInfo(m_FieldCode);
	return pFieldInfo;
}

const MapGroup *					
BattleRoomEntry::GetCurGroup()
{
	const sMAPINFO * pMapInfo = GetCurMapInfo();
	if ( !pMapInfo )
	{
		assert(pMapInfo);
		return NULL;
	}

	const MapGroup * pMapGroup = MapInfoParser::Instance()->GetMapGroup( pMapInfo->MapKind );
	if ( !pMapGroup )
	{
		assert(pMapInfo);
		return NULL;
	}

	return pMapGroup;
}

const BYTE							
BattleRoomEntry::GetCurLimitMinUser()
{
	if ( this->GetCurMapInfo() )
	{
		return this->GetCurMapInfo()->byMinUserNum;
	}
	else
	{
		return 1;
	}
}

const BYTE							
BattleRoomEntry::GetCurLimitMaxUser()
{
	if ( this->GetCurMapInfo() )
	{
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		uiBattleZoneMan * pBattlezoneMan = (uiBattleZoneMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE);
		if( pBattlezoneMan && pBattlezoneMan->GetSuperMasterType() & eROOM_SPECIAL_GUILD_PVP)
		{
			return MAX_GUILD_PVP_JOIN_MEMBER;
		}
		else
		{
			return this->GetCurMapInfo()->byMaxUserNum;
		}
#else
		return this->GetCurMapInfo()->byMaxUserNum;
#endif
	}
	else
	{
		return 10;
	}
}
