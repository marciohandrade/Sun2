//---------------------------------
//
// 클라이언트에서 사용하는 룸정보..
//
// PacketStruct.h의 프로토콜을 이용한다.
// j0 / [3/25/2005]
//---------------------------------
#ifndef BATTLEROOMENTRY_H
#define BATTLEROOMENTRY_H

#include "atltime.h"
#include "MapInfoParser.h"


enum BATTLE_LIST_PAGE
{
	Battle_List_Lines_Per_Page = 10,
};

class BattleRoomEntry
{
public:
	BattleRoomEntry();


	~BattleRoomEntry() { }

public:
	void Init();

	BattleRoomEntry & operator=(BattleRoomEntry & rEntry);

	void	SetSerial(DWORD dwSerial);
	void	SetMapCode(CODETYPE MapCode);
	void	SetFieldCode(CODETYPE FieldCode);
	void	SetType(eZONETYPE eType);
	void	SetPublic(eZONEPUBLIC ePublic);
	void	SetTitle(char * pszTitle);
	void	SetPassword(char * pszPassWord);
	void	SetBaseInfo(BASE_ROOMINFO * pBaseRoomInfo);
	
	void	SetAdditionalPVPInfo(ADDITIONAL_PVPINFO * pAdditionalInfo);
    void    SetAdditionalCHuntingInfo(ADDITIONAL_CHUNTINGINFO * pAdditionalInfo);

	void	SetMinLevel(BYTE byLevel);
	void	SetMaxLevel(BYTE byLevel);
	void	SetMinPlayer(BYTE byNumPlayers);
	void	SetMaxPlayer(BYTE byNumPlayers);

	void	SetClassLimit( eCLASS_TYPE eClassPermition );
	void	SetFieldIndex(BYTE	byFieldIndex);
	void	SetDifficult(eHUNTING_MONSTER_DIFFICULT eMonsterDifficult);
	void	SetBonusType(eHUNTING_BONUS_TYPE	eBonusType);

	void	SetPVPRule(ePVP_RULE_TYPE eRule);
	void	SetPVPMode(ePVP_MODE_TYPE eMode);
	
	const DWORD 						GetSerial();
	const CODETYPE						GetMapCode();
	const CODETYPE						GetFieldCode();
	const eZONETYPE						GetType();
	const eZONEPUBLIC					GetPublic();
	const TCHAR *						GetName();
	const TCHAR * 						GetTitle();
	const char *						GetPassword();
	const BASE_ROOMINFO *				GetBaseInfo(BASE_ROOMINFO * OUT pBaseRoomInfo);
	const ADDITIONAL_ROOMINFO *			GetAdditionalInfo(ADDITIONAL_ROOMINFO * OUT pAdditionalInfo);
	const ADDITIONAL_PVPINFO *			GetAdditionalPVPInfo(ADDITIONAL_PVPINFO * OUT pAdditionalInfo);
    const ADDITIONAL_CHUNTINGINFO *     GetAdditionalCHuntingInfo(ADDITIONAL_CHUNTINGINFO * OUT pAdditionalInfo);
	
	const BYTE							GetMinLevel();
	const BYTE							GetMaxLevel();
	const BYTE							GetMinPlayer();
	const BYTE							GetMaxPlayer();
	const eCLASS_TYPE					GetClassLimit();
	const BYTE							GetFieldIndex();
	const eHUNTING_MONSTER_DIFFICULT	GetDifficult();
	const eHUNTING_BONUS_TYPE			GetBonusType();
	const ePVP_RULE_TYPE				GetPVPRule();
	const ePVP_MODE_TYPE				GetPVPMode();

	const sMAPINFO *					GetCurMapInfo();
	const sFIELDINFO *					GetCurFieldInfo();
	const MapGroup *					GetCurGroup();
	const BYTE							GetCurLimitMinUser();
	const BYTE							GetCurLimitMaxUser();


public:
	DWORD				m_dwSerial;
	CODETYPE			m_MapCode;
	CODETYPE			m_FieldCode;
	eZONETYPE			m_eType;
	eZONEPUBLIC			m_ePublic;		
	TCHAR				m_pszRoomName[MAX_MAPNAME_LENGTH];
	TCHAR				m_pszRoomTitle[MAX_ROOMTITLE_LENGTH];
	char				m_pszRoomPW[MAX_ROOMPASSWORD_LENGTH];
	BASE_ROOMINFO		m_RoomInfo;
	ADDITIONAL_ROOMINFO m_AdditionalInfo;
	int					m_iMaxUser;
	int					m_iMinUser;

	ADDITIONAL_PVPINFO  m_AdditionalPVPInfo;
    ADDITIONAL_CHUNTINGINFO m_AdditionalCHuntingInfo;
};


#endif
// BATTLEROOMENTRY_H