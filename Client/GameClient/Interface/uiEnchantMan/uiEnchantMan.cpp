#include "SunClientPrecompiledHeader.h"
#include "uienchantman.h"
#include "interfacemanager.h"
#ifdef __NA_001358_ENCHANT_UNIFICATION
#include "InstantEnchantParser.h"
#include <SCSlotContainer.h>
#include "ItemManager.h"
#include "SCItemSlot.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GlobalFunc.h"
#include "SoundEffect.h"
#else //__NA_001358_ENCHANT_UNIFICATION
#ifdef _NA_003966_20111227_ADD_ENCHANT
#include "EnchantDialog3.h"
#else
#include "EnchantDialog2.h"
#endif
#endif //__NA_001358_ENCHANT_UNIFICATION

#ifdef _NA_003966_20111227_ADD_ENCHANT
TCHAR local_dialog_id[] = _T("0710");
TCHAR local_dialog_resource_path[] = _T("Data\\Interface\\71_enchant2.iwz");
#else
TCHAR local_dialog_id[] = _T("0710");
TCHAR local_dialog_resource_path[] = _T("Data\\Interface\\71_enchant.iwz");
#endif


uiEnchantMan::uiEnchantMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
#ifdef __NA_001358_ENCHANT_UNIFICATION
    m_bPendingPacket = false;
    m_bPendingAsk = false;
#else //__NA_001358_ENCHANT_UNIFICATION
    m_pEnchantDialog = NULL;
#endif //__NA_001358_ENCHANT_UNIFICATION	
}

//------------------------------------------------------------------------------
/**
*/
void	uiEnchantMan::OnInitialize()
{
#ifdef __NA_001358_ENCHANT_UNIFICATION
    //no operation
#else //__NA_001358_ENCHANT_UNIFICATION


    m_pEnchantDialog = new EnchantDialog;
    if( !g_InterfaceManager.LoadInterface( 
        m_pEnchantDialog, 
        local_dialog_resource_path, 
        StrToWzID(local_dialog_id), 
        FALSE ) )
    {
	}

#ifdef _NA_003966_20111227_ADD_ENCHANT
	m_pEnchantDialog->SetDialogKey(StrToWzID(local_dialog_id));
#endif // _NA_003966_20111227_ADD_ENCHANT

#endif //__NA_001358_ENCHANT_UNIFICATION    
}

#ifdef __NA_001358_ENCHANT_UNIFICATION
bool uiEnchantMan::AskInstantEnchant()
{
	uiSystemMan* pUIMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
	SCSlotContainer* pContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
	if(pUIMan && pContainer)
	{
		SCItemSlot& rTargetItemSlot = (SCItemSlot&)pContainer->GetSlot( m_TargetItemPos );
		
		TCHAR szTemp1[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
		GameFunc::GetItemName(&rTargetItemSlot, rTargetItemSlot.GetItemInfo(), szTemp1, INTERFACE_STRING_LENGTH);
        g_InterfaceManager.GetInterfaceString(723,szTemp2,INTERFACE_STRING_LENGTH);
		Snprintf(szMessage, INTERFACE_STRING_LENGTH, "%s%s", szTemp1, szTemp2);

		pUIMan->Sys_Confirm(NULL, GlobalFunc::UseInstantEnchantGem, NULL/*szTitle*/, szMessage);
		m_bPendingAsk = true;
	}
	else
		return false;

	return true;
}

bool uiEnchantMan::CheckEnableEnchant()
{
	if(m_bPendingAsk || m_bPendingPacket)
		return false;

	SCSlotContainer* pContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY); 
	SCItemSlot& rEnchantItemSlot = (SCItemSlot&)pContainer->GetSlot( m_InstantEnchantPos );
	SCItemSlot& rTargetItemSlot = (SCItemSlot&)pContainer->GetSlot( m_TargetItemPos );
	
	if ( !rEnchantItemSlot.GetItemInfo() || eITEMTYPE_ENCHANT != rEnchantItemSlot.GetItemInfo()->m_wType )
	{
		assert(!"Is not InstantEnchantGems");
		return false;
	}

	tag_InstantEnchantScriptInfo* pInfo = InstantEnchantParser::Instance()->GetInstantEnchantInfo(rEnchantItemSlot.GetItemInfo()->m_Code, rTargetItemSlot.GetItemInfo()->m_LV);
	if(!pInfo)
	{
		return false;
	}

	if ( NULL == rTargetItemSlot.GetItemInfo() )
	{
		return false;
	}

	if (ST_ITEM != rTargetItemSlot.GetSlotType() 
		|| (!rTargetItemSlot.GetItemInfo()->IsWeapon() && !rTargetItemSlot.GetItemInfo()->IsArmor()
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
        && (rTargetItemSlot.IsWingItem() == false)
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
        ))
	{
		// 잘못된 인챈트 대상입니다.
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
		g_InterfaceManager.GetInterfaceString( 725, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return false;
	}

	if( pInfo->byEnchantMinLV > rTargetItemSlot.GetEnchant()+1
		|| pInfo->byEnchantMaxLV < rTargetItemSlot.GetEnchant()+1 )
	{
		// 대상 아이템의 인챈트 레벨을 확인해 주십시오.
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
		g_InterfaceManager.GetInterfaceString( 726, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return false;
	}	

	return true;
}

void uiEnchantMan::SendPacketInstantEnchant()
{
	MSG_CG_ITEM_INSTANT_ENCHANT_SYN Msg;
	Msg.m_PotonItemPos = GetInstantEnchantPos();
	Msg.m_TargetItemPos = GetTargetItemPos();
	if (GlobalFunc::SendPacket( CI_GAMESERVERIDX, &Msg, sizeof(Msg)))
	{
		m_bPendingPacket = true;
#ifdef _DEV_VER
		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_OUPUT_DEBUG, "Send InstantEnchant Packet" );
#endif
	}
}

void uiEnchantMan::OnEnchantPacketAck(MSG_BASE * pMsg)
{	
	MSG_CG_ITEM_INSTANT_ENCHANT_ACK* pRecvMsg = (MSG_CG_ITEM_INSTANT_ENCHANT_ACK*)pMsg;
	
	m_bPendingPacket = false;	
	TCHAR szMessage[MAX_MESSAGE_LENGTH];

	switch(pRecvMsg->m_ResultCode)
	{
	case RC::RC_ITEM_ENCHANT_SUCCESS:
		{						
			SCSlotContainer* pContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
			if(pContainer)
			{
				// 포톤 소모(?)
				SCItemSlot& rInstantEnchantItemSlot = (SCItemSlot&)pContainer->GetSlot( m_InstantEnchantPos );
				ItemManager::Instance()->SpendItem( SI_INVENTORY, rInstantEnchantItemSlot.GetCode(), 1 );
				// 인챈트 업
				SCItemSlot& rTargetItemSlot = (SCItemSlot&)pContainer->GetSlot( m_TargetItemPos );
				rTargetItemSlot.SetEnchant(rTargetItemSlot.GetEnchant() + 1);

				// 인챈트에 성공했습니다.
				g_InterfaceManager.GetInterfaceString( 724, szMessage, MAX_MESSAGE_LENGTH );

				g_SoundEffect.PlayUI( UI_SOUND_COMPOSE_ENCHANT_SUCCESS );
			}
		}
		break;
	case RC::RC_ITEM_ENCHANT_FAILED:
		{
			SCSlotContainer* pContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
			if (pContainer)
			{
				// 포톤 소모(?)
				SCItemSlot& rInstantEnchantItemSlot = (SCItemSlot&)pContainer->GetSlot(m_InstantEnchantPos);
				ItemManager::Instance()->SpendItem(SI_INVENTORY, rInstantEnchantItemSlot.GetCode(), 1);
				
				// 인챈트가 실패했습니다.
				g_InterfaceManager.GetInterfaceString(727, szMessage);

				g_SoundEffect.PlayUI(UI_SOUND_COMPOSE_ENCHANT_FAIL);
			}
		}
		break;

	case RC::RC_ITEM_ENCHANT_FAILED_AND_ENCHNAT_DOWN:
		{
			SCSlotContainer* pContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
			if (pContainer)
			{
				// 포톤 소모(?)
				SCItemSlot& rInstantEnchantItemSlot = (SCItemSlot&)pContainer->GetSlot(m_InstantEnchantPos);
				ItemManager::Instance()->SpendItem(SI_INVENTORY, rInstantEnchantItemSlot.GetCode(), 1);
				// 인챈트 다운
				SCItemSlot& rTargetItemSlot = (SCItemSlot&)pContainer->GetSlot(m_TargetItemPos);
				rTargetItemSlot.SetEnchant(rTargetItemSlot.GetEnchant() - 1);

				BASE_ITEMINFO* itemInfo = rTargetItemSlot.GetItemInfo();
				assert(itemInfo);

				// 메시지 출력
				TCHAR szItemName[128], szItemName2[128], szTemp[64];
				TCHAR szTempMessage[MAX_MESSAGE_LENGTH] = {0,};

                GameFunc::GetItemName(&rTargetItemSlot, rTargetItemSlot.GetItemInfo(), szTemp, 64);

				Sprintf(szItemName, "%s %+d", szTemp, rTargetItemSlot.GetEnchant() + 1);
				Sprintf(szItemName2, "%s %+d", szTemp, rTargetItemSlot.GetEnchant());

				// 5318 인챈트에 실패하여, %s가 %s로 다운되었습니다.
				g_InterfaceManager.GetInterfaceString(5318, szTempMessage);
				Sprintf(szMessage, szTempMessage, szItemName, szItemName2);

				g_SoundEffect.PlayUI(UI_SOUND_COMPOSE_ENCHANT_FAIL);
			}
		}					
		break;

	case RC::RC_ITEM_ENCHANT_FAILED_AND_DESTROY:
		{
			SCSlotContainer* pContainer = ItemManager::Instance()->GetContainer(SI_INVENTORY);
			if(pContainer)
			{
				// 포톤 소모(?)
				SCItemSlot& rInstantEnchantItemSlot = (SCItemSlot&)pContainer->GetSlot( m_InstantEnchantPos );
				ItemManager::Instance()->SpendItem( SI_INVENTORY, rInstantEnchantItemSlot.GetCode(), 1 );
				// 아이템 파괴
				SCItemSlot& rTargetItemSlot = (SCItemSlot&)pContainer->GetSlot( m_TargetItemPos );

				BASE_ITEMINFO* itemInfo = rTargetItemSlot.GetItemInfo();
				assert( itemInfo );

				TCHAR szTemp[64]	  = {0,};
				TCHAR szItemName[256] = {0,};

                GameFunc::GetItemName(&rTargetItemSlot, rTargetItemSlot.GetItemInfo(), szTemp, 64);
				Snprintf(szItemName, 255, "%s +%d", szTemp, rTargetItemSlot.GetEnchant() );

				pContainer->DeleteSlot( m_TargetItemPos, NULL );

				// 5319 인챈트에 실패 하여, %s가 소멸 되었습니다.
				TCHAR szTempMessage[MAX_MESSAGE_LENGTH] = { '\0', };
				g_InterfaceManager.GetInterfaceString( 5319, szTempMessage );
				Snprintf( szMessage, INTERFACE_STRING_LENGTH - 1, szTempMessage, szItemName );	

				g_SoundEffect.PlayUI( UI_SOUND_COMPOSE_ENCHANT_FAIL );
			}						
		}
		break;
	}

	GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage);
	GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW, szMessage);
}

void uiEnchantMan::OnEnchantPacketNak(MSG_BASE * pMsg)
{
	MSG_CG_ITEM_INSTANT_ENCHANT_NAK* pRecvMsg = (MSG_CG_ITEM_INSTANT_ENCHANT_NAK*)pMsg;
	m_bPendingPacket = false;
	ItemManager::Instance()->NoticeReasonItemErrorCode( pRecvMsg->m_ResultCode );	
}

#endif //__NA_001358_ENCHANT_UNIFICATION