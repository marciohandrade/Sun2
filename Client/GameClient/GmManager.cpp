//------------------------------------------------------------------------------
//  SkillStateAssault.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "Chatdialog.h"
#include "GmManager.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "ResourceManager.h"
#include "LoadScene.h"
#include "MapInfoParser.h"
#include "../../Utility/Nstring.h"
#include "Camera.h"
#include "HeroActionInput.h"
#include "EquipmentContainer.h"
#include <SCItemSlot.h>
#include "StyleInfoParser.h"
#include "SkillInfoParser.h"
#include "ItemInfoParser.h"
#include "GameOptionParser.h"
#include "uiTrade/uiTrade.h"
#include "ObjectManager.h"
#include "globalfunc.h"
#include "BattleScene.h"
#include "uiBattleZoneMan/uiBattleZoneMan_def.h"
#include "uiChatMan/uiChatMan_def.h"
/**
*/
GmManager::GmManager()
{
}

//------------------------------------------------------------------------------
/**
*/
GmManager::~GmManager()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL GmManager::ParseStyle( TCHAR * pszString )
{
	DWORD dwStyleCode = 0;

	TCHAR szTemp[256];

	_stscanf(pszString, _T("%s"),szTemp);

	dwStyleCode = GetStyleCode(szTemp);
	if(dwStyleCode==0xFFFFFFFF)
	{
		return FALSE;
	}

	//적용 
	g_pHero->SetCurrentAttackStyle(dwStyleCode);

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
DWORD GmManager::GetStyleCode(TCHAR * pszString)
{
	TCHAR	szText[INTERFACE_STRING_LENGTH];

	SkillInfoParser::Instance()->SetFirst();
	BASE_STYLEINFO	*pBASE_STYLEINFO = NULL;

	while( true )
	{
		pBASE_STYLEINFO = SkillInfoParser::Instance()->GetNextStyle();
		if( pBASE_STYLEINFO == NULL )
		{
			return 0xFFFFFFFF;
		}

		//	일단 두번째 네임코드만 이용( 창, 지팡이, 오브 등등 )
		g_InterfaceManager.GetInterfaceString( pBASE_STYLEINFO->m_dwStyleNCode, szText, INTERFACE_STRING_LENGTH );
		if( Strcmp( szText, pszString ) == 0 )
		{
			//	찾았다.
			return pBASE_STYLEINFO->m_SkillCode;
		}
	}

	return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
	스크립트에서 읽어서 체크할것 
*/
BOOL GmManager::ParseWeapon(TCHAR * pszString)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];


	SLOTCODE dwWeaponCode = 0;

	TCHAR szTemp[256];

	_stscanf(pszString,_T("%s"),szTemp);

	dwWeaponCode = ( SLOTCODE )GetWeaponCode(szTemp);
	if(dwWeaponCode==0xFFFFFFFF)
	{
		return FALSE;
	}

	enum
	{
		eWT_TWOHANDAXE		= 5,
		eWT_TWOHANDSWORD	= 16,
		eWT_ONEHANDSWORD	= 31,
		eWT_SPEAR			= 46,
		eWT_STAFF			= 137,
		eWT_ORB				= 152,
	};

	if( GENERALPARAM->IsNet() )//네트웍 모드일경우 
	{
		//해당 무기를 바닥에 떨군다.
		MSG_CG_GM_CREATE_ITEM_SYN SendPacket;
		SendPacket.m_byCategory = CG_GM;
		SendPacket.m_byProtocol	= CG_GM_CREATE_ITEM_SYN;

		DWORD dwCode = eWT_TWOHANDAXE;

		if(dwWeaponCode==eWEAPONTYPE_TWOHANDAXE)
		{
			dwCode = eWT_TWOHANDAXE;
		}
		else if(dwWeaponCode==eWEAPONTYPE_TWOHANDSWORD)
		{
			dwCode = eWT_TWOHANDSWORD;
		}
		else if(dwWeaponCode==eWEAPONTYPE_ONEHANDSWORD)
		{
			dwCode = eWT_ONEHANDSWORD;
		}
		else if(dwWeaponCode==eWT_SPEAR)
		{
			dwCode = eWT_SPEAR;
		}
		else if(dwWeaponCode==eWEAPONTYPE_STAFF)
		{
			dwCode = eWT_STAFF;
		}
		else if(dwWeaponCode==eWEAPONTYPE_ORB)
		{
			dwCode = eWT_ORB;
		}
		SendPacket.m_ItemCode = ( SLOTCODE )dwCode;
		SendPacket.m_dwItemCount = 1;
		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );    

	}
	else//싱글일경우 강제 장착 시킨다.
	{

		BASE_PLAYERINFO* bp = g_pHero->GetCharInfo();
		//스크립트에서 읽어 체크 할것 

		if(bp&&bp->m_byClassCode==eCHAR_BERSERKER)
		{
			if(dwWeaponCode==eWEAPONTYPE_TWOHANDAXE)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_TWOHANDAXE );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else if(dwWeaponCode==eWEAPONTYPE_TWOHANDSWORD)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_TWOHANDSWORD );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else if(dwWeaponCode==eWEAPONTYPE_PUNCH)
			{
				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
			}
			else
			{
				//	해당 캐릭터에 적용할수 없는 무기 입니다.
				g_InterfaceManager.GetInterfaceString( eST_INVALID_WEAPON_EQUIP, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

				return FALSE;
			}
		}
		else if(bp&&bp->m_byClassCode==eCHAR_DRAGON)
		{
			if(dwWeaponCode==eWEAPONTYPE_ONEHANDSWORD)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_ONEHANDSWORD );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else if(dwWeaponCode==eWEAPONTYPE_SPEAR)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_SPEAR );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else if(dwWeaponCode==eWEAPONTYPE_INVALID)
			{
				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
			}
			else
			{
				//	해당 캐릭터에 적용할수 없는 무기 입니다.
				g_InterfaceManager.GetInterfaceString( eST_INVALID_WEAPON_EQUIP, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
				return FALSE;
			}
		}
		else if(bp&&bp->m_byClassCode==eCHAR_MAGICIAN)
		{
			if(dwWeaponCode==eWEAPONTYPE_PUNCH)
			{
				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
			}
			else if(dwWeaponCode==eWEAPONTYPE_STAFF)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_STAFF );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else if(dwWeaponCode==eWEAPONTYPE_ORB)
			{
				SCItemSlot ItemSlot;
				ItemSlot.SetCode( eWT_ORB );

				SCSlotContainer * pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
				pContainer->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1,NULL);
				pContainer->InsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			}
			else
			{
				//	해당 캐릭터에 적용할수 없는 무기 입니다.
				g_InterfaceManager.GetInterfaceString( eST_INVALID_WEAPON_EQUIP, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		//스타일 체크 무기와 안맞을때 스타일 없음으로 설정 
		g_pHero->CheckStyleForWeapon(dwWeaponCode);

	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
DWORD  GmManager::GetWeaponCode(TCHAR * pszString)
{
	TCHAR	szText[INTERFACE_STRING_LENGTH];

	ItemInfoParser::Instance()->SetFirst();
	BASE_ITEMINFO *pBASE_ITEMINFO = NULL;

	while( true )
	{
		pBASE_ITEMINFO = ItemInfoParser::Instance()->GetNext();
		if( pBASE_ITEMINFO == NULL )
		{
			return 0xFFFFFFFF;
		}

		//	일단 두번째 네임코드만 이용( 창, 지팡이, 오브 등등 )
		g_InterfaceManager.GetInterfaceString( pBASE_ITEMINFO->m_SCNCode, szText, INTERFACE_STRING_LENGTH );
		if( Strcmp( szText, pszString ) == 0 )
		{
			//	찾았다.
			return pBASE_ITEMINFO->m_Code;
		}
	}

	return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
*/

DWORD  GmManager::GetSkillCode(TCHAR * pszSkillName)
{
	TCHAR	szText[INTERFACE_STRING_LENGTH];

	SkillInfoParser::Instance()->SetFirst();
	SkillScriptInfo *pSkill = NULL;

	while( true )
	{	
		pSkill = SkillInfoParser::Instance()->GetNextSkill();
		if( pSkill == NULL )
		{
			return 0xFFFFFFFF;
		}

		g_InterfaceManager.GetInterfaceString( pSkill->m_dwSkillNcode, szText, INTERFACE_STRING_LENGTH );
		if( Strcmp( szText, pszSkillName ) == 0 )
		{
			//	찾았다.
			return pSkill->m_SkillCode;
		}
	}

	return 0xFFFFFFFF;
}

//
////------------------------------------------------------------------------------
///**
//*/
//BOOL GmManager::ParseSkill(TCHAR * pszString)
//{
//	DWORD	dwSkillCode = 0;
//	int		iLevel = 1;
//	TCHAR	szTemp[256];
//
//	_stscanf(pszString,_T("%s %d"),szTemp,&iLevel);
//
//	dwSkillCode = GetSkillCode(szTemp);
//	if( dwSkillCode == 0xFFFFFFFF )
//	{
//		return FALSE;
//	}
//
//	if (iLevel <= 0)
//	{
//		iLevel = 1;
//	}
//	else if (iLevel > 10)
//	{
//		iLevel = 10;
//	}
//
//	DWORD dwFinalSkillCode;
//	
//	if (dwSkillCode == 0)
//	{
//		dwFinalSkillCode = 0;
//		
//	}
//	else
//	{
//		dwFinalSkillCode = dwSkillCode + iLevel - 1;
//	}
//
//    g_HeroInput.SetCurrentSkillID((SLOTCODE)dwFinalSkillCode);
//    return TRUE;
//}
//


//------------------------------------------------------------------------------
/**
    입력된 문자열로 부터 파라미터를 파싱

    TODO:입력된 파라미터의 유효성 검사 
*/
BOOL GmManager::ParseCommand(TCHAR * pszString)
{
    if(pszString[0]!='/')
        return FALSE;

    if( g_pHero == NULL )
    {
        return TRUE;
	}


	//-----------------------------------------------------------------------------------
	//
	//		StringTableForProgram.txt 파일에 있던 GM 명령어 처리 방식이 수정되어
	//		더이상 클라이언트에서 GM 명령어를 관리하지 않습니다.
	//		GM 명령어는 서버에서 관리하고 클라이언트는 '/' 문자 두개로 시작하는 문장
	//		전체를 서버에 보내면 서버에서 파싱해서 처리합니다.
	//
	//-----------------------------------------------------------------------------------

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];
	TCHAR	szCommand[INTERFACE_STRING_LENGTH];

    ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

	int iCommandcnt;
	int iCommandLen = 0;
	int ChatStringLen = lstrlen(pszString);
    TCHAR szTemp[64];
    TCHAR szTemp2[64];
	TCHAR szTemp3[64];
    TCHAR szParam[64];
	DWORD dwKey;

	iCommandcnt = eGM_CMD_COUNT;
	iCommandLen = 0;

	for (int i = 0; i < iCommandcnt; ++i)
	{
		g_InterfaceManager.GetInterfaceString( (eST_GM_COMMAND + i), szCommand, INTERFACE_STRING_LENGTH );

		iCommandLen = _tcslen( szCommand );

		if (iCommandLen > ChatStringLen )
			continue;

		char * pszPos = NULL;	// 파라미터를 이어서 찾을 경우 사용
		if ( CSTR_EQUAL == CompareString(
							    LOCALE_USER_DEFAULT,
								NULL, 
								pszString, 
								iCommandLen, 
								szCommand,
								iCommandLen) )
		{
			if( i >= eGM_CMD_COUNT )
			{
				// GM 명령이 아니다.
				if (GENERALPARAM->GetSpecialMode() == eSPECIAL_MODE_NONE)
				{
					break;
				}
			}
			else
			{
				// GM 명령이다.
				if(GENERALPARAM->GetSpecialMode() == eSPECIAL_MODE_NONE)
				{
					if( g_pHero->GetGMGrade() == 0 )
					{
						return TRUE;
					}
				}
			}

			switch( i ) 
			{


			//---------------------------------------------------------------------------------------------
			//		GM 명령어
			//---------------------------------------------------------------------------------------------

			case eGM_CMD_GHOST:				//	투명
				{
					//	옵저버 모드
					bool	bObserverMode = false;
					if( _stscanf( pszString, _T("%s %s"), szTemp, szTemp2 ) == 2 )
					{
						//	배열 길이를 짧게 하면 메모리 에러 납니다.
						//	내용이 아무리 짧은 문장이라도 길이는 넉넉하게 맞춰주세요
						TCHAR	szON[INTERFACE_STRING_LENGTH], szOFF[INTERFACE_STRING_LENGTH];
						g_InterfaceManager.GetInterfaceString( eST_GM_INVINCIBLE_ON, szON, INTERFACE_STRING_LENGTH );
						g_InterfaceManager.GetInterfaceString( eST_GM_INVINCIBLE_OFF, szOFF, INTERFACE_STRING_LENGTH );

						if( _tcsnccmp( szTemp2, szON, 4 ) == 0 )
						{
							bObserverMode = true;
						}
						else if( _tcsnccmp( szTemp2, szOFF, 4 ) == 0 )
						{
							bObserverMode = false;
						}
						else
						{
							//	파라미터가 잘못되었습니다.
							pDlg->AddString(pszString);
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
						}

						MSG_CG_GM_OBSERVER_SYN SendPacket;
						SendPacket.m_byCategory		= CG_GM;
						SendPacket.m_byProtocol		= CG_GM_OBSERVER_SYN;
						SendPacket.m_byObserverOn	= bObserverMode;
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
						return TRUE;
					}
					else
					{
						//	파라미터가 잘못되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}

					return TRUE;
				}
				break;

			case eGM_CMD_INVINCIBLE:		//	무적
				{
					bool	bInvisible = false;
					if( _stscanf( pszString, _T("%s %s"), szTemp, szTemp2 ) == 2 )
					{
						//	배열 길이를 짧게 하면 메모리 에러 납니다.
						//	내용이 아무리 짧은 문장이라도 길이는 넉넉하게 맞춰주세요
						TCHAR	szON[INTERFACE_STRING_LENGTH], szOFF[INTERFACE_STRING_LENGTH];
						g_InterfaceManager.GetInterfaceString( eST_GM_INVINCIBLE_ON, szON, INTERFACE_STRING_LENGTH );
						g_InterfaceManager.GetInterfaceString( eST_GM_INVINCIBLE_OFF, szOFF, INTERFACE_STRING_LENGTH );

						if( _tcsnccmp( szTemp2, szON, 4 ) == 0 )
						{
							bInvisible = true;
						}
						else if( _tcsnccmp( szTemp2, szOFF, 4 ) == 0 )
						{
							bInvisible = false;
						}
						else
						{
							//	파라미터가 잘못되었습니다.
							pDlg->AddString(pszString);
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
						}

						MSG_CG_GM_UNDEAD_MODE_SYN SendPacket;
						SendPacket.m_byCategory	= CG_GM;
						SendPacket.m_byProtocol	= CG_GM_UNDEAD_MODE_SYN;
						SendPacket.m_bUndeadOn	= bInvisible;
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
						return TRUE;
					}
					else
					{
						//	파라미터가 잘못되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_GOTO_MAP:			//	가기( 멥이나 방 )
				{
					return TRUE;
				}
				break;

			case eGM_CMD_CHAR_INFO:			//	정보 [캐릭터명]
				{
					if (_stscanf(pszString,_T("%s %s"),szTemp,szTemp2) == 2)
					{
						//Todo:캐릭터 이름으로 서버에 캐릭터 정보 요청 패킷을 날린다.
						MSG_CG_GM_CHARINFO_SYN SendPacket;

						int		iNumber = 0;

						//	오브젝트 키를 입력 받았다
						if( szTemp2[0] == _T('-') )
						{
							StrnCopy( SendPacket.m_szCharName, &(szTemp2[1]), MAX_CHARNAME_LENGTH );

							iNumber = atoi( SendPacket.m_szCharName );
							Character	*pCharacter = (Character *)g_ObjectManager.GetObject( iNumber );
							if( pCharacter == NULL )
							{
								//	없다
								break;
							}

							StrnCopy( SendPacket.m_szCharName, pCharacter->GetName(), MAX_CHARNAME_LENGTH );
						}
						else
						{
							//	입력된 이름
							StrCopy(SendPacket.m_szCharName, szTemp2);
						}

						SendPacket.m_dwUserID = 0;
						SendPacket.m_byCategory = CG_GM;
						SendPacket.m_byProtocol	= CG_GM_CHARINFO_SYN;
						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
						return TRUE;
					}
					else 
					{
						//	파라미터 갯수가 잘못 되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_ROOM_INFO:			//	방 정보
				{
					if (_stscanf(pszString,_T("%s %d"),szTemp,&dwKey) == 2)
					{
						MSG_CG_GM_ROOMINFO_SYN SendPacket;
						SendPacket.m_RoomKey = (KEYTYPE)dwKey;
						SendPacket.m_byCategory = CG_GM;
						SendPacket.m_byProtocol	= CG_GM_ROOMINFO_SYN;
						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );

						return TRUE;
					}
					else if (_stscanf(pszString,_T("%s"),szTemp) == 1)
					{
						MSG_CG_GM_ROOMINFO_SYN SendPacket;
						SendPacket.m_RoomKey = 0;
						SendPacket.m_byCategory = CG_GM;
						SendPacket.m_byProtocol	= CG_GM_ROOMINFO_SYN;
						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );

						return TRUE;
					}
					else 
					{
						//	파라미터 갯수가 잘못 되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_ROOM_LIST:			//	방 리스트
				{
					g_InterfaceManager.HideAllWindows();

					// 배틀존 리스트 UI 띄우기
					uiBase* pUIBase = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST );

					if (pUIBase)
					{
						BOOL bShow = !pUIBase->IsVisible();

						pUIBase->ShowInterface(bShow);
					}

					return TRUE;
				}
				break;

			case eGM_CMD_CHANNEL_INFO:		//	채널 정보
				{
					return TRUE;
				}
				break;

			case eGM_CMD_MOVE_CHAR:			//	이동
				{
					return TRUE;
				}
				break;

			case eGM_CMD_WATCH:				//	감시
				{
					return TRUE;
				}
				break;

			case eGM_CMD_NOTICE_WORLD:		//	월드 공지
				{
					if( _stscanf( pszString, _T("%s %s"), szTemp2, szTemp ) == 2 )
					{
						pszString += _tcslen( szTemp2 );
						int pos = _tcscspn( pszString, szTemp );
						pszString += pos;
						SendGMNotice( pszString );
					}
					return TRUE;
				}
				break;

			case eGM_CMD_NOTICE_CHANNEL:	//	채널 공지
				{
					if( _stscanf( pszString, _T("%s %s"), szTemp2, szTemp ) == 2 )
					{
						pszString += _tcslen( szTemp2 );
						int pos = _tcscspn( pszString, szTemp );
						pszString += pos;
						SendGMNotice( pszString, MSG_CW_GM_NOTICE_SYN::NOTICE_CHANNEL );
					}
					return TRUE;
				}
				break;

			case eGM_CMD_NOTICE_LOCAL:		//	지역 공지
				{
					if( _stscanf( pszString, _T("%s %s"), szTemp2, szTemp ) == 2 )
					{
						pszString += _tcslen( szTemp2 );
						int pos = _tcscspn( pszString, szTemp );
						pszString += pos;
						SendGMNotice( pszString, MSG_CW_GM_NOTICE_SYN::NOTICE_ZONE );
					}
					return TRUE;
				}
				break;

			case eGM_CMD_REMOVE_ITEM:			//	아이템제거
				{
					//아이템 제거 패킷을 날린다 
					MSG_CG_GM_DESTROY_ITEM_AROUND_SYN SendPacket;
					SendPacket.m_byCategory = CG_GM;
					SendPacket.m_byProtocol	= CG_GM_DESTROY_ITEM_AROUND_SYN;
					GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
					return TRUE;
				}
				break;

			case eGM_CMD_REMOVE_MONSTER:		//	몬스터제거
				{
					//몬스터 제거 패킷을 날린다.
					MSG_CG_GM_MONSTER_DESTROYNEAR_SYN SendPacket;
					SendPacket.m_byCategory = CG_GM;
					SendPacket.m_byProtocol	= CG_GM_MONSTER_DESTROYNEAR_SYN;
					GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
					return TRUE;
				}
				break;

			case eGM_CMD_HEAL:					//	회복
				{
					if( g_pHero )
					{
						MSG_CG_GM_RECOVERY_SYN  SendPacket;
						SendPacket.m_byCategory = CG_GM;
						SendPacket.m_byProtocol	= CG_GM_RECOVERY_SYN;
						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
						return TRUE;
					}
					return TRUE;
				}
				break;

			case eGM_CMD_LEVEL_UP:				//	레벨업
				{
					int level;
					if (_stscanf(pszString,_T("%s %d"),szTemp,&level)==2)
					{

						DWORD upLevel=g_pHero->GetLevel();
						upLevel+=level;
						//레벨 업 패킷을 날린다.
						MSG_CG_GM_LEVEL_UP_SYN  SendPacket;
						SendPacket.m_byCategory = CG_GM;
						SendPacket.m_byProtocol	= CG_GM_LEVEL_UP_SYN;
						SendPacket.m_wBonusLevel = level;
						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
						return TRUE;
					}
					else 
					{
						//	파라미터 갯수가 잘못 되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_STAT_UP:				//	스텟업
				{
					DWORD stat;
					if (_stscanf(pszString,_T("%s %d"),szTemp,&stat)==2)
					{
						if(stat>=1&&stat<=GM_MAX_STAT_UP_VALUE)
						{
							//스텟 업 패킷을 날린다.
							MSG_CG_GM_STAT_UP_SYN   SendPacket;
							SendPacket.m_byCategory = CG_GM;
							SendPacket.m_byProtocol	= CG_GM_STAT_UP_SYN;
							SendPacket.m_dwBonusStat=stat;
							GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
							return TRUE;
						}
						else
						{
							//	스탯업 수치 오류
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_STAT_POINT, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
							return TRUE;
						}

					}
					else 
					{
						//	파라미터 갯수가 잘못 되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_SKILL_POINT_UP:		//	스킬포인트업
				{
					DWORD skillPoint;
					if (_stscanf(pszString,_T("%s %d"),szTemp,&skillPoint)==2)
					{
						if(skillPoint>=1&&skillPoint<=GM_MAX_SKILLPOINT_UP_VALUE)
						{
							//스킬 포인트 업 패킷을 날린다.
							MSG_CG_GM_SKILLPOINT_UP_SYN    SendPacket;
							SendPacket.m_byCategory = CG_GM;
							SendPacket.m_byProtocol	= CG_GM_SKILLPOINT_UP_SYN;
							SendPacket.m_dwBonusSkill=skillPoint;
							GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
							return TRUE;
						}
						else
						{
							//	스킬 포인트 수치 오류
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_SKILL_POINT, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
							return TRUE;
						}

					}
					else 
					{
						//	파라미터 갯수가 잘못 되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_ADD_SKILL:				//	스킬추가
				{
					return TRUE;
				}
				break;

			case eGM_CMD_RECALL_MONSTER:		//	몬스터소환
				{
					int mon, count;
					if (_stscanf(pszString,_T("%s %ld %ld"),szMessage,&mon,&count) == 3)
					{
						GameFunc::SpawnMonster( mon, count );
						return TRUE;
					}
					return TRUE;
				}
				break;

			case eGM_CMD_RESURRECTION:			//	부활
				{
					if( g_pHero )
					{
						MSG_CG_GM_RESURRECTION_SYN	SendPacket;
						SendPacket.m_byCategory	= CG_GM;
						SendPacket.m_byProtocol	= CG_GM_RESURRECTION_SYN;
						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
						return TRUE;
					}
					return TRUE;
				}
				break;

			case eGM_CMD_CREATE_ITEM:			//	생성
				{
					int itemCode,num;
					if (_stscanf(pszString,_T("%s %d %d"),szTemp,&itemCode,&num)==3)
					{
						if(num>=1 && num<=GM_MAX_CREATE_ITEM_NUM)
						{
							//아이템 생성 패킷을 날린다.
							MSG_CG_GM_CREATE_ITEM_SYN SendPacket;
							SendPacket.m_byCategory = CG_GM;
							SendPacket.m_byProtocol	= CG_GM_CREATE_ITEM_SYN;
							SendPacket.m_ItemCode = itemCode;
							SendPacket.m_dwItemCount =num;

							GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );    
							return TRUE;
						}
						else
						{
							//	아이템 갯수 오류
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_ITEM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
							return TRUE;
						}

					}
					else 
					{
						//	파라미터 갯수가 잘못 되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				return TRUE;

			case eGM_CMD_HYME:					//	하임
				{
					MONEY money; //TODO:형 맞출것
					if (_stscanf(pszString,_T("%s %I64u"),szTemp,&money)==2)//todo:%d 맞출것 
					{
						if(money>=1&&money<=GM_MAX_CREATE_MONEY)
						{
							//머니 생성 패킷을 날린다.
							MSG_CG_GM_CREATE_MONEY_SYN  SendPacket;
							SendPacket.m_byCategory = CG_GM;
							SendPacket.m_byProtocol	= CG_GM_CREATE_MONEY_SYN;
							SendPacket.m_Money = money;
							GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
							return TRUE;
						}
						else
						{
							//	금액이 범위를 벗어났습니다.
							g_InterfaceManager.GetInterfaceString( eST_GM_OVERFLOW_MONEY, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
							return TRUE;
						}

					}
					else 
					{
						//	파라미터 갯수가 잘못 되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_MAKE_OPTION:			//	옵션만들기
				{
					return TRUE;
				}
				break;

			case eGM_CMD_SPEED:					//	속도
				{
					int	iSpeed;
					if (_stscanf(pszString,_T("%s %d"),szTemp,&iSpeed)==2)
					{
						if( iSpeed >= 0 && iSpeed <= 5 )
						{
							MSG_CG_GM_SET_SPEED_SYN  SendPacket;
							SendPacket.m_byCategory		= CG_GM;
							SendPacket.m_byProtocol		= CG_GM_SET_SPEED_SYN;
							SendPacket.m_byMoveLevel	= iSpeed;
							GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
							return TRUE;
						}
						else
						{
							//	에러
							//	파라미터 갯수가 잘못 되었습니다.
							pDlg->AddString(pszString);
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
							return TRUE;
						}
					}
					else 
					{
						//	파라미터 갯수가 잘못 되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_ENTRANCE:				//	입장
				{
					if( g_pHero )
					{
						MSG_CG_GM_ENTRANCE_SYN	SendPacket;
						SendPacket.m_byCategory	= CG_GM;
						SendPacket.m_byProtocol	= CG_GM_ENTRANCE_SYN;
						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
						g_pHero->SetAccountState( TRUE );
						return TRUE;
					}
					return TRUE;
				}
				break;

			case eGM_CMD_CONNECT_CLOSE:				//	접속종료
				{
					if (_stscanf(pszString,_T("%s %s"),szTemp,&szTemp2)==2)
					{
						Object	*pObj = g_ObjectManager.GetObject( szTemp2 );
						if( pObj )
						{
							MSG_CG_GM_FORCE_DISCONNECT_SYN  SendPacket;
							SendPacket.m_byCategory		= CG_GM;
							SendPacket.m_byProtocol		= CG_GM_FORCE_DISCONNECT_SYN;
							SendPacket.m_dwUserKey		= pObj->GetObjectKey();
							GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
							return TRUE;
						}
					}
					else 
					{
						//	파라미터 갯수가 잘못 되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
					}
					return TRUE;
				}
				break;

			case eGM_CMD_ACCOUNT:					//	어카운트
				{
					bool	bStatus = false;
					if( _stscanf( pszString, _T("%s %s"), szTemp, szTemp2 ) == 2 )
					{
						//	배열 길이를 짧게 하면 메모리 에러 납니다.
						//	내용이 아무리 짧은 문장이라도 길이는 넉넉하게 맞춰주세요
						TCHAR	szON[INTERFACE_STRING_LENGTH], szOFF[INTERFACE_STRING_LENGTH];
						g_InterfaceManager.GetInterfaceString( eST_GM_INVINCIBLE_ON, szON, INTERFACE_STRING_LENGTH );
						g_InterfaceManager.GetInterfaceString( eST_GM_INVINCIBLE_OFF, szOFF, INTERFACE_STRING_LENGTH );

						if( _tcsnccmp( szTemp2, szON, 4 ) == 0 )
						{
							bStatus = true;
							g_pHero->SetAccountState( TRUE );
						}
						else if( _tcsnccmp( szTemp2, szOFF, 4 ) == 0 )
						{
							bStatus = false;
							g_pHero->SetAccountState( FALSE );
						}
						else
						{
							//	파라미터가 잘못되었습니다.
							pDlg->AddString(pszString);
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
						}

						return TRUE;
					}
					else
					{
						//	파라미터가 잘못되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
					}
					return TRUE;
				}
				break;

			case eGM_CMD_MONSTER_DESTROY:			//제거			[몬스터 오브젝트 키]
				{
					if( _stscanf( pszString, _T("%s %d"), szTemp, &dwKey ) == 2 )
					{
						MSG_CG_GM_MONSTER_DESTROYONE_SYN	SendPacket;
						SendPacket.m_byCategory		= CG_GM;
						SendPacket.m_byProtocol		= CG_GM_MONSTER_DESTROYONE_SYN;
						SendPacket.m_dwMonsterKey	= dwKey;
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
					}
					else if( _stscanf( pszString, _T("%s"), szTemp ) == 1 )
					{
						dwKey = g_HeroInput.GetCurrentTarget();
						Object *pObj = g_ObjectManager.GetObject( dwKey );
						if( pObj )
						{
							SCObject SCObj = pObj->GetSCObject();
							if( SCObj.GetObjectType() != MONSTER_OBJECT )
							{
								dwKey = 0;
							}
						}
						else
						{
							dwKey = 0;
						}

						if( dwKey )
						{
							MSG_CG_GM_MONSTER_DESTROYONE_SYN	SendPacket;
							SendPacket.m_byCategory		= CG_GM;
							SendPacket.m_byProtocol		= CG_GM_MONSTER_DESTROYONE_SYN;
							SendPacket.m_dwMonsterKey	= dwKey;
							GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
						}
						else
						{
							//	파라미터가 잘못되었습니다.
							pDlg->AddString(pszString);
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
						}
					}
					else
					{
						//	파라미터가 잘못되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
					}

					return TRUE;
				}
				break;

			case eGM_CMD_MONSTER_KILL:				//킬			[몬스터 오브젝트 키]
				{
					if( _stscanf( pszString, _T("%s %d"), szTemp, &dwKey ) == 2 )
					{
						MSG_CG_GM_MONSTER_KILLONE_SYN	SendPacket;
						SendPacket.m_byCategory		= CG_GM;
						SendPacket.m_byProtocol		= CG_GM_MONSTER_KILLONE_SYN;
						SendPacket.m_dwMonsterKey	= dwKey;
						GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
					}
					else if( _stscanf( pszString, _T("%s"), szTemp ) == 1 )
					{
						dwKey = g_HeroInput.GetCurrentTarget();
						Object *pObj = g_ObjectManager.GetObject( dwKey );
						if( pObj )
						{
							SCObject SCObj = pObj->GetSCObject();
							if( SCObj.GetObjectType() != MONSTER_OBJECT )
							{
								dwKey = 0;
							}
						}
						else
						{
							dwKey = 0;
						}

						if( dwKey )
						{
							MSG_CG_GM_MONSTER_KILLONE_SYN	SendPacket;
							SendPacket.m_byCategory		= CG_GM;
							SendPacket.m_byProtocol		= CG_GM_MONSTER_KILLONE_SYN;
//							SendPacket.m_dwKey			= g_pHero->GetObjectKey();
							SendPacket.m_dwMonsterKey	= dwKey;
							GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
						}
						else
						{
							//	파라미터가 잘못되었습니다.
							pDlg->AddString(pszString);
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
						}
					}
					else
					{
						//	파라미터가 잘못되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
					}

					return TRUE;
				}
				break;

			case eGM_CMD_CREATE_ENCHANT_ITEM:
				{
					SLOTCODE	itemCode;
					DWORD		dwitemCount;
					BYTE		inchent;

					if (_stscanf(pszString,_T("%s %s %s %s"),szTemp,szTemp2,szTemp3,szParam)==4)
					{
						itemCode = _ttoi( szTemp2 );
						dwitemCount = _ttoi( szTemp3 );
						inchent = _ttoi( szParam );

						//아이템,옵션 생성 패킷을 날린다.
						MSG_CG_GM_CREATE_ENCHANT_ITEM_SYN SendPacket;
						SendPacket.m_byCategory = CG_GM;
						SendPacket.m_byProtocol	= CG_GM_CREATE_ENCHANT_ITEM_SYN;
						SendPacket.m_ItemCode	= itemCode;
						SendPacket.m_dwItemCount= dwitemCount;
						SendPacket.m_byEnchant =inchent;

						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
					}
					else 
					{
						//	파라미터 갯수가 잘못 되었습니다.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
					}
				}
				return TRUE;

			}	//	switch( i )

		}	//	CompareString()

	}	//	for (int i = 0; i < iCommandcnt; ++i)

	return FALSE;
}

