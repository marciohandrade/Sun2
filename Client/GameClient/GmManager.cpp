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

	//���� 
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

		//	�ϴ� �ι�° �����ڵ常 �̿�( â, ������, ���� ��� )
		g_InterfaceManager.GetInterfaceString( pBASE_STYLEINFO->m_dwStyleNCode, szText, INTERFACE_STRING_LENGTH );
		if( Strcmp( szText, pszString ) == 0 )
		{
			//	ã�Ҵ�.
			return pBASE_STYLEINFO->m_SkillCode;
		}
	}

	return 0xFFFFFFFF;
}

//------------------------------------------------------------------------------
/**
	��ũ��Ʈ���� �о üũ�Ұ� 
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

	if( GENERALPARAM->IsNet() )//��Ʈ�� ����ϰ�� 
	{
		//�ش� ���⸦ �ٴڿ� ������.
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
	else//�̱��ϰ�� ���� ���� ��Ų��.
	{

		BASE_PLAYERINFO* bp = g_pHero->GetCharInfo();
		//��ũ��Ʈ���� �о� üũ �Ұ� 

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
				//	�ش� ĳ���Ϳ� �����Ҽ� ���� ���� �Դϴ�.
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
				//	�ش� ĳ���Ϳ� �����Ҽ� ���� ���� �Դϴ�.
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
				//	�ش� ĳ���Ϳ� �����Ҽ� ���� ���� �Դϴ�.
				g_InterfaceManager.GetInterfaceString( eST_INVALID_WEAPON_EQUIP, szMessage, INTERFACE_STRING_LENGTH );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

		//��Ÿ�� üũ ����� �ȸ����� ��Ÿ�� �������� ���� 
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

		//	�ϴ� �ι�° �����ڵ常 �̿�( â, ������, ���� ��� )
		g_InterfaceManager.GetInterfaceString( pBASE_ITEMINFO->m_SCNCode, szText, INTERFACE_STRING_LENGTH );
		if( Strcmp( szText, pszString ) == 0 )
		{
			//	ã�Ҵ�.
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
			//	ã�Ҵ�.
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
    �Էµ� ���ڿ��� ���� �Ķ���͸� �Ľ�

    TODO:�Էµ� �Ķ������ ��ȿ�� �˻� 
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
	//		StringTableForProgram.txt ���Ͽ� �ִ� GM ��ɾ� ó�� ����� �����Ǿ�
	//		���̻� Ŭ���̾�Ʈ���� GM ��ɾ �������� �ʽ��ϴ�.
	//		GM ��ɾ�� �������� �����ϰ� Ŭ���̾�Ʈ�� '/' ���� �ΰ��� �����ϴ� ����
	//		��ü�� ������ ������ �������� �Ľ��ؼ� ó���մϴ�.
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

		char * pszPos = NULL;	// �Ķ���͸� �̾ ã�� ��� ���
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
				// GM ����� �ƴϴ�.
				if (GENERALPARAM->GetSpecialMode() == eSPECIAL_MODE_NONE)
				{
					break;
				}
			}
			else
			{
				// GM ����̴�.
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
			//		GM ��ɾ�
			//---------------------------------------------------------------------------------------------

			case eGM_CMD_GHOST:				//	����
				{
					//	������ ���
					bool	bObserverMode = false;
					if( _stscanf( pszString, _T("%s %s"), szTemp, szTemp2 ) == 2 )
					{
						//	�迭 ���̸� ª�� �ϸ� �޸� ���� ���ϴ�.
						//	������ �ƹ��� ª�� �����̶� ���̴� �˳��ϰ� �����ּ���
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
							//	�Ķ���Ͱ� �߸��Ǿ����ϴ�.
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
						//	�Ķ���Ͱ� �߸��Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}

					return TRUE;
				}
				break;

			case eGM_CMD_INVINCIBLE:		//	����
				{
					bool	bInvisible = false;
					if( _stscanf( pszString, _T("%s %s"), szTemp, szTemp2 ) == 2 )
					{
						//	�迭 ���̸� ª�� �ϸ� �޸� ���� ���ϴ�.
						//	������ �ƹ��� ª�� �����̶� ���̴� �˳��ϰ� �����ּ���
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
							//	�Ķ���Ͱ� �߸��Ǿ����ϴ�.
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
						//	�Ķ���Ͱ� �߸��Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_GOTO_MAP:			//	����( ���̳� �� )
				{
					return TRUE;
				}
				break;

			case eGM_CMD_CHAR_INFO:			//	���� [ĳ���͸�]
				{
					if (_stscanf(pszString,_T("%s %s"),szTemp,szTemp2) == 2)
					{
						//Todo:ĳ���� �̸����� ������ ĳ���� ���� ��û ��Ŷ�� ������.
						MSG_CG_GM_CHARINFO_SYN SendPacket;

						int		iNumber = 0;

						//	������Ʈ Ű�� �Է� �޾Ҵ�
						if( szTemp2[0] == _T('-') )
						{
							StrnCopy( SendPacket.m_szCharName, &(szTemp2[1]), MAX_CHARNAME_LENGTH );

							iNumber = atoi( SendPacket.m_szCharName );
							Character	*pCharacter = (Character *)g_ObjectManager.GetObject( iNumber );
							if( pCharacter == NULL )
							{
								//	����
								break;
							}

							StrnCopy( SendPacket.m_szCharName, pCharacter->GetName(), MAX_CHARNAME_LENGTH );
						}
						else
						{
							//	�Էµ� �̸�
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
						//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_ROOM_INFO:			//	�� ����
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
						//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_ROOM_LIST:			//	�� ����Ʈ
				{
					g_InterfaceManager.HideAllWindows();

					// ��Ʋ�� ����Ʈ UI ����
					uiBase* pUIBase = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST );

					if (pUIBase)
					{
						BOOL bShow = !pUIBase->IsVisible();

						pUIBase->ShowInterface(bShow);
					}

					return TRUE;
				}
				break;

			case eGM_CMD_CHANNEL_INFO:		//	ä�� ����
				{
					return TRUE;
				}
				break;

			case eGM_CMD_MOVE_CHAR:			//	�̵�
				{
					return TRUE;
				}
				break;

			case eGM_CMD_WATCH:				//	����
				{
					return TRUE;
				}
				break;

			case eGM_CMD_NOTICE_WORLD:		//	���� ����
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

			case eGM_CMD_NOTICE_CHANNEL:	//	ä�� ����
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

			case eGM_CMD_NOTICE_LOCAL:		//	���� ����
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

			case eGM_CMD_REMOVE_ITEM:			//	����������
				{
					//������ ���� ��Ŷ�� ������ 
					MSG_CG_GM_DESTROY_ITEM_AROUND_SYN SendPacket;
					SendPacket.m_byCategory = CG_GM;
					SendPacket.m_byProtocol	= CG_GM_DESTROY_ITEM_AROUND_SYN;
					GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
					return TRUE;
				}
				break;

			case eGM_CMD_REMOVE_MONSTER:		//	��������
				{
					//���� ���� ��Ŷ�� ������.
					MSG_CG_GM_MONSTER_DESTROYNEAR_SYN SendPacket;
					SendPacket.m_byCategory = CG_GM;
					SendPacket.m_byProtocol	= CG_GM_MONSTER_DESTROYNEAR_SYN;
					GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
					return TRUE;
				}
				break;

			case eGM_CMD_HEAL:					//	ȸ��
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

			case eGM_CMD_LEVEL_UP:				//	������
				{
					int level;
					if (_stscanf(pszString,_T("%s %d"),szTemp,&level)==2)
					{

						DWORD upLevel=g_pHero->GetLevel();
						upLevel+=level;
						//���� �� ��Ŷ�� ������.
						MSG_CG_GM_LEVEL_UP_SYN  SendPacket;
						SendPacket.m_byCategory = CG_GM;
						SendPacket.m_byProtocol	= CG_GM_LEVEL_UP_SYN;
						SendPacket.m_wBonusLevel = level;
						GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
						return TRUE;
					}
					else 
					{
						//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_STAT_UP:				//	���ݾ�
				{
					DWORD stat;
					if (_stscanf(pszString,_T("%s %d"),szTemp,&stat)==2)
					{
						if(stat>=1&&stat<=GM_MAX_STAT_UP_VALUE)
						{
							//���� �� ��Ŷ�� ������.
							MSG_CG_GM_STAT_UP_SYN   SendPacket;
							SendPacket.m_byCategory = CG_GM;
							SendPacket.m_byProtocol	= CG_GM_STAT_UP_SYN;
							SendPacket.m_dwBonusStat=stat;
							GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
							return TRUE;
						}
						else
						{
							//	���Ⱦ� ��ġ ����
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_STAT_POINT, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
							return TRUE;
						}

					}
					else 
					{
						//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_SKILL_POINT_UP:		//	��ų����Ʈ��
				{
					DWORD skillPoint;
					if (_stscanf(pszString,_T("%s %d"),szTemp,&skillPoint)==2)
					{
						if(skillPoint>=1&&skillPoint<=GM_MAX_SKILLPOINT_UP_VALUE)
						{
							//��ų ����Ʈ �� ��Ŷ�� ������.
							MSG_CG_GM_SKILLPOINT_UP_SYN    SendPacket;
							SendPacket.m_byCategory = CG_GM;
							SendPacket.m_byProtocol	= CG_GM_SKILLPOINT_UP_SYN;
							SendPacket.m_dwBonusSkill=skillPoint;
							GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
							return TRUE;
						}
						else
						{
							//	��ų ����Ʈ ��ġ ����
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_SKILL_POINT, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
							return TRUE;
						}

					}
					else 
					{
						//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_ADD_SKILL:				//	��ų�߰�
				{
					return TRUE;
				}
				break;

			case eGM_CMD_RECALL_MONSTER:		//	���ͼ�ȯ
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

			case eGM_CMD_RESURRECTION:			//	��Ȱ
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

			case eGM_CMD_CREATE_ITEM:			//	����
				{
					int itemCode,num;
					if (_stscanf(pszString,_T("%s %d %d"),szTemp,&itemCode,&num)==3)
					{
						if(num>=1 && num<=GM_MAX_CREATE_ITEM_NUM)
						{
							//������ ���� ��Ŷ�� ������.
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
							//	������ ���� ����
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_ITEM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
							return TRUE;
						}

					}
					else 
					{
						//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				return TRUE;

			case eGM_CMD_HYME:					//	����
				{
					MONEY money; //TODO:�� �����
					if (_stscanf(pszString,_T("%s %I64u"),szTemp,&money)==2)//todo:%d ����� 
					{
						if(money>=1&&money<=GM_MAX_CREATE_MONEY)
						{
							//�Ӵ� ���� ��Ŷ�� ������.
							MSG_CG_GM_CREATE_MONEY_SYN  SendPacket;
							SendPacket.m_byCategory = CG_GM;
							SendPacket.m_byProtocol	= CG_GM_CREATE_MONEY_SYN;
							SendPacket.m_Money = money;
							GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
							return TRUE;
						}
						else
						{
							//	�ݾ��� ������ ������ϴ�.
							g_InterfaceManager.GetInterfaceString( eST_GM_OVERFLOW_MONEY, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
							return TRUE;
						}

					}
					else 
					{
						//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_MAKE_OPTION:			//	�ɼǸ����
				{
					return TRUE;
				}
				break;

			case eGM_CMD_SPEED:					//	�ӵ�
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
							//	����
							//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
							pDlg->AddString(pszString);
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
							return TRUE;
						}
					}
					else 
					{
						//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
						return TRUE;
					}
				}
				break;

			case eGM_CMD_ENTRANCE:				//	����
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

			case eGM_CMD_CONNECT_CLOSE:				//	��������
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
						//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
					}
					return TRUE;
				}
				break;

			case eGM_CMD_ACCOUNT:					//	��ī��Ʈ
				{
					bool	bStatus = false;
					if( _stscanf( pszString, _T("%s %s"), szTemp, szTemp2 ) == 2 )
					{
						//	�迭 ���̸� ª�� �ϸ� �޸� ���� ���ϴ�.
						//	������ �ƹ��� ª�� �����̶� ���̴� �˳��ϰ� �����ּ���
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
							//	�Ķ���Ͱ� �߸��Ǿ����ϴ�.
							pDlg->AddString(pszString);
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
						}

						return TRUE;
					}
					else
					{
						//	�Ķ���Ͱ� �߸��Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
					}
					return TRUE;
				}
				break;

			case eGM_CMD_MONSTER_DESTROY:			//����			[���� ������Ʈ Ű]
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
							//	�Ķ���Ͱ� �߸��Ǿ����ϴ�.
							pDlg->AddString(pszString);
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
						}
					}
					else
					{
						//	�Ķ���Ͱ� �߸��Ǿ����ϴ�.
						pDlg->AddString(pszString);
						g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
						pDlg->AddString( szMessage );
					}

					return TRUE;
				}
				break;

			case eGM_CMD_MONSTER_KILL:				//ų			[���� ������Ʈ Ű]
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
							//	�Ķ���Ͱ� �߸��Ǿ����ϴ�.
							pDlg->AddString(pszString);
							g_InterfaceManager.GetInterfaceString( eST_GM_INVALID_NUM_PARAM, szMessage, INTERFACE_STRING_LENGTH );
							pDlg->AddString( szMessage );
						}
					}
					else
					{
						//	�Ķ���Ͱ� �߸��Ǿ����ϴ�.
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

						//������,�ɼ� ���� ��Ŷ�� ������.
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
						//	�Ķ���� ������ �߸� �Ǿ����ϴ�.
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

