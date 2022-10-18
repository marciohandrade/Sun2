#include "SunClientPrecompiledHeader.h"
#include "uiCharPopupMan.h"
#include "uiCharPopupMenu/uiCharPopupMenu.h"

#include "uiPartyMember/uiPartyMember.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiPartyOption/uiPartyOption.h"

#include "ItemShareSystem.h"

#include "heroactioninput.h"	
#include "objectmanager.h"
#include "ChatDialog.h"
#include "uiCommunityDlg/uiCommunityDlg.h"
#include "GlobalFunc.h"
#include "Object.h"

#ifdef _KIKI_UI_WEB_DIALOG
	#include "uiWebDialogMan/uiWebDialogMan.h"
#endif

#include "uiChatMan/uiChatMan_def.h"
#include "uiTargetMan/uiTargetMan.h"
#include "TargetDialog.h"
#include "uiTradeMan/uiTradeMan.h"
#include "uiCommunityMan/uiCommunityMan.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/GuildSystemPacketFunction.h"
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL

#ifdef _NA_002935_20110704_ITEM_SCORE
  #include "uiInventoryMan/uiInventoryMan.h"
  #include "Player.h"
#endif //_NA_002935_20110704_ITEM_SCORE
#include "Mouse.h"

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
  #include "ChaosSystem/ChaosSystemPacketFunction.h"
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "GameFramework.h"
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

//------------------------------------------------------------------------------
/**
*/
uiCharPopupMan::uiCharPopupMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_bShow = false;
    m_pUICharPopupMenu = NULL;

	m_iPopupType = -1;
	m_dwTargetObjKey = 0;
	m_bGroupType	 = true;
    block_type_ = true;
#ifdef _NA_002935_20110704_ITEM_SCORE
    show_equip_ = true;
#endif //_NA_002935_20110704_ITEM_SCORE

#ifdef _DH_SHOW_VENDOR_LIST_POPUP
    object_type_ = MAX_OBJECT;
    set_character_name("");
#endif//_DH_SHOW_VENDOR_LIST_POPUP
}

//------------------------------------------------------------------------------
/**
*/
void uiCharPopupMan::OnInitialize()
{
    _LoadUI();
}

#ifdef _DH_SHOW_VENDOR_LIST_POPUP
//------------------------------------------------------------------------------
void uiCharPopupMan::CheckPopupType(DWORD target_key, const TCHAR* character_name /* ="" */, eOBJECT_TYPE object_type /* = MAX_OBJECT*/)
{
    //ĳ����â ��ġ ����Ʈ

    static RECT move_position = {0, 0};
    static bool is_first = true;
    if (is_first)
    {
        m_pUICharPopupMenu->ShowInterface(TRUE);
        if (m_pUICharPopupMenu && m_pUICharPopupMenu->GetDialogWZ())
        {
            m_pUICharPopupMenu->GetDialogWZ()->GetClientRect(&move_position);
            is_first = false;
        }
    }
    if (is_vendor_popup_)
    {
        int mouse_x = Mouse::Instance()->GetMouseX();
        int mouse_y = Mouse::Instance()->GetMouseY();
        //��ġ �̵�
        if (m_pUICharPopupMenu)
        {
            m_pUICharPopupMenu->MoveWindowWZ(mouse_x, mouse_y);
        }
    }
    else
    {
        if (m_pUICharPopupMenu)
        {
            m_pUICharPopupMenu->MoveWindowWZ(move_position.left, move_position.top);
        }
        //SetPopupInfo(g_HeroInput.GetCurrentTarget());
    }

	SetPopupInfo(target_key);//m_dwTargetObjKey = dwObjKey;

    Object* object = static_cast<Object*>(g_ObjectManager.GetObject(target_key));        
    if (strcmp(character_name, "") == 0)
    {
        if (object)
        {
            set_character_name(object->GetName());
        }
    }
    else
    {
        set_character_name(character_name);
    }

    if (object_type == MAX_OBJECT)
    {
        if (object)
        {
            set_object_type(object->GetObjectType());
        }
    }
    else
    {
        set_object_type(object_type);
    }
    /*
        eCPK_PARTY  -- have party
        eCPK_NONE   -- haven't party 
    */

    uiPartyMan* partyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (partyMan)
    {
        if (partyMan->IsParty())
        {
            if (target_key == 0 && strcmp(character_name, ""))//Ű�� ���� �̸��� �ִٸ�
            {
                target_key = partyMan->IsPartyMemberForName(character_name);// ����� �ƴϸ� target_key = 0;
            }
			//���õ��ڰ� ��Ƽ������ƴ϶��	
			if(!partyMan->IsPartyMember(target_key))
			{
				m_iPopupType = partyMan->IsLeader() ? (eCPK_NONE):(eCPK_PARTY);
			}	
			else
			{
				if (partyMan->IsLeader())
				{
					m_iPopupType = eCPK_PARTY_MASTER;
				}
				else
				{
					m_iPopupType = eCPK_PARTY;
				}
			}
        }
        else
        {
            m_iPopupType = eCPK_NONE;
        }
    }

	m_bGroupType = IsTargetGroupMember();
    block_type_ = IsTargetBlockMember();
#ifdef _NA_002935_20110704_ITEM_SCORE
    show_equip_ = IsShowEquip(object);
#endif //_NA_002935_20110704_ITEM_SCORE
		
}
//=====================================================================================
//	DESC: Ÿ������ ������ ��´�
//=====================================================================================

//------------------------------------------------------------------------------
bool uiCharPopupMan::ProcessMenuFunc(int iIndex)
{
    //if (!m_dwTargetObjKey) return false;

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};

	switch (iIndex)
    {
    case eST_NORMAL_WHISPER_STRING_INDEX:		//�ӼӸ�
        {
            ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
			if (pChatDlg && object_type_ == PLAYER_OBJECT && strcmp(character_name_, "") != 0)//�÷��̾�� �̸��� ������ �Ӹ� ���� 
			{
                g_InterfaceManager.GetInterfaceString(2010, szTemp, INTERFACE_STRING_LENGTH);
                Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %s "), szTemp, character_name_);
				if (!pChatDlg->IsVisibleWindow())
                {
					pChatDlg->ShowWindow(TRUE);
                }
				pChatDlg->SetChatMessage(szMessage);
			}
        }
        break;
#ifdef _NA_002935_20110704_ITEM_SCORE
    case eST_NORMAL_EQUIP_SHOW_STRING_INDEX: // ��񺸱�
        {
            //�ش� �÷��̾��� ��� â�� ��������.
            uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
            if (m_dwTargetObjKey && inventory_manager)
            {
                Object* target_object = g_ObjectManager.GetObject(m_dwTargetObjKey);
                if (target_object && (target_object->IsKindOfObject(PLAYER_OBJECT)))
                {
                    Player* player = static_cast<Player *>(target_object);
                    if (player)
                    {
                        inventory_manager->SetOtherPlayerEquipInventoryDialog(player);
                    }
                }
            }
        }
        break;
#endif //_NA_002935_20110704_ITEM_SCORE
    case eST_NORMAL_FRIEND_STRING_INDEX:		//ģ���߰�
	{
        if (object_type_ == PLAYER_OBJECT && strcmp(character_name_, ""))//�÷��̾�� �̸��� ������ �Ӹ� ���� 
        {
			_CallBack_Friend_Add(true, character_name_);
        }
    }
	break;

    case eST_NORMAL_TRADE_STRING_INDEX:		//�ŷ���û
	{
		Object* target_object = g_ObjectManager.GetObject(m_dwTargetObjKey);
		if (target_object == NULL) 
		{
			//5833	�ŷ��� �� ���� �Ÿ��Դϴ�.
			g_InterfaceManager.GetInterfaceString(5833, szMessage ,INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
		}
		else
		{
            uiTradeMan* trade_manager = static_cast<uiTradeMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
            assert (trade_manager);
            if (trade_manager && target_object->IsKindOfObject(PLAYER_OBJECT) && !GlobalFunc::IsSSQScene())
            {
                Player* player = static_cast<Player*>(target_object);
                if (player && player->IsDead())
                {
                    g_InterfaceManager.GetInterfaceString(6340, szMessage, INTERFACE_STRING_LENGTH);
                    GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage );
                }
                else
                {
                    trade_manager->Ask(character_name_);
                }
            }
		}
    }
	break;

    case eST_NORMAL_VIEW_STRING_INDEX:			//���캸��
        break;

    case eST_NORMAL_PVP_STRING_INDEX:			//����û
        break;

    case eST_NORMAL_FOLLOW_STRING_INDEX:		//���󰡱�
	{
		Object *pObj = g_ObjectManager.GetObject(m_dwTargetObjKey);
		if (!pObj) 
		{
			//5141	�Ÿ��� �־� �̿��� �������� �ʽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5141,szMessage,INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
		}
		else
		{
			g_HeroInput.SetFollowState(TRUE, m_dwTargetObjKey);
		}
    }
	break;

#ifdef _KIKI_ADD_G_LOG_MENU
	case eST_NORMAL_G_LOG_STRING_INDEX:			//G-�α�
		{
#ifdef _KIKI_UI_WEB_DIALOG
			Object	*pObj = g_ObjectManager.GetObject(m_dwTargetObjKey);
			uiWebDialogMan* webMan = GET_DIALOG_MANAGER(uiWebDialogMan, UIMAN_WEB);
			if(webMan && pObj)
			{
#ifdef _KIKI_G_LOG_PROTOCOL
				webMan->RequestUserID(pObj->GetName());
#else
				webMan->ShowWebDialog(TRUE);
#endif			
			}
#endif
		}
		break;
#endif

    case eST_PARTY_MASTER_INVITE_STRING_INDEX:	//��Ƽ�ʴ�
        {
            uiPartyMan* partyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            if (partyMan)
            {
				if (partyMan->IsParty() == false)
                {
					partyMan->AskJoinParty(character_name_);
                }
				else if	(partyMan->IsParty() && partyMan->IsLeader())
                {
					partyMan->AskJoinParty(character_name_);
                }
				else 
				{
					g_InterfaceManager.GetInterfaceString( eST_PARTY_ISNOT_MASTEROFPARTY, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
				}
            }
        }
        break;

    case eST_PARTY_MASTER_MIGRATION_STRING_INDEX:	//��Ƽ�� ����
        {
            uiPartyMan* partyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            if (partyMan && partyMan->IsParty() && partyMan->IsLeader())
            {
                MSG_CG_PARTY_CHANGE_MASTER_SYN sendMsg;
                sendMsg.m_dwObjKey      = m_dwTargetObjKey;
                GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_CHANGE_MASTER_SYN) );
            }
			ItemShareSystem* pItemShare = GET_CAST_DIALOG(ItemShareSystem, IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM);
			if(pItemShare)
			{
				pItemShare->CloseItemShareSystemWindow();	// ��Ƽ �й� �ɼ�â �ݱ�
			}
        }
        break;

	case eST_PARTY_MASTER_KICK: // ��Ƽ�߹�
        {
			if (object_type_ == PLAYER_OBJECT)
			{
	            uiPartyMan* partyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
	
	            if (partyMan)
	            {
	                partyMan->ForcedExpulsion();//�̰Ŵ� Ÿ�ٿ� ������ ����
	            }
			}
        }
        break;

    case eST_GROUP_ADD:	//(�׷�)������
        {
            if (object_type_ == PLAYER_OBJECT && strcmp(character_name_, ""))
            {
				MSG_CW_GROUP_ADD_SYN sync;
				ZeroMemory(sync.ptszMemeberName,sizeof(sync.ptszMemeberName));
				TCHAR szString[MAX_CHARNAME_LENGTH+1] ={0,};
				StrnCopy(sync.ptszMemeberName, character_name_, MAX_CHARNAME_LENGTH);
				GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
	        }
        }
        break;
	case eST_GROUP_DEL:	//(�׷�)�������
        {
			if (object_type_ == PLAYER_OBJECT && strcmp(character_name_, ""))
            {
				MSG_CW_GROUP_DEL_SYN sync;
				ZeroMemory(sync.ptszMemeberName,sizeof(sync.ptszMemeberName));
				TCHAR szString[MAX_CHARNAME_LENGTH+1] ={0,};
				StrnCopy(sync.ptszMemeberName, character_name_, MAX_CHARNAME_LENGTH);
				GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
	        }
        }
        break;

    case eST_BLOCK_ADD:	//����
        {
			if (object_type_ == PLAYER_OBJECT && strcmp(character_name_, ""))
            {
        #if defined(_KOREA)
                if (g_slangNameFilter.CheckIncludeString(character_name_))
        #else
                if (GlobalFunc::CheckIncludeStringOnAbroad(character_name_))
        #endif
                {
                    // ������ ������ ���� �޽��� ����ϰ� ��Ŷ������ ����
                    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
                    // 5346	�߸��� ���� �Դϴ�.
                    g_InterfaceManager.GetInterfaceString(5346, szMessage, MAX_MESSAGE_LENGTH);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                }
                else
                {
                    MSG_CW_FRIEND_BLOCK_SYN sync;
                    ZeroMemory(sync.ptszBlockName,sizeof(sync.ptszBlockName));
                    TCHAR szString[MAX_CHARNAME_LENGTH+1] ={0,};
                    StrnCopy(szString, character_name_, MAX_CHARNAME_LENGTH);
                    StrnCopy(sync.ptszBlockName, szString, MAX_CHARNAME_LENGTH);
                    GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
                }
	        }
        }
        break;
    case eST_BLOCK_DELETE:
        {
            if (object_type_ == PLAYER_OBJECT && strcmp(character_name_, ""))
            {
                MSG_CW_FRIEND_BLOCK_FREE_SYN sync;
			    ZeroMemory(sync.ptszBlockName,sizeof(sync.ptszBlockName));
			    StrnCopy(sync.ptszBlockName, character_name_, MAX_CHARNAME_LENGTH);
			    GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
	        }
        }
        break;
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
    case eST_GUILD_INVITE:
        {
            if (object_type_ == PLAYER_OBJECT && strcmp(character_name_, ""))
            {
                GuildSystem::PacketFunction::SEND_CG_GUILD_INVITE_SYN(character_name_);
            }
        }
        break;
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    case eST_BATTLE_GROUND_VOTE:
        {
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            if (GameFramework::GetCurScene() == SCENE_TYPE_BATTLEGROUND)
            {
                ChaosSystem::PacketFunction::SEND_CG_VOTE(character_name_);
            }
            else if (GameFramework::GetCurScene() == SCENE_TYPE_GOLDRUSH)
            {
                ChaosSystem::PacketFunction::SEND_CG_GOLDRUSH_VOTE(character_name_);
            }
#else
            ChaosSystem::PacketFunction::SEND_CG_VOTE(character_name_);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        }
        break;
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

    case eST_CANCEL:
        break;
    }

    return true;
}

//------------------------------------------------------------------------------
bool uiCharPopupMan::IsTargetGroupMember()
{
	int iSel = -1;
	uiCommunityMan* pComMan = GET_DIALOG_MANAGER(uiCommunityMan, UIMAN_COMMUNITY);

	if(!pComMan)
	{
		return false;
	}
	iSel = pComMan->FindGroup(character_name_);

	return (0 <= iSel);	// �׷����� �ε����� ���´�. -1���� ũ�ٸ� �����
}

//------------------------------------------------------------------------------
bool uiCharPopupMan::IsTargetBlockMember()
{
	int iSel = -1;
	uiCommunityMan* pComMan = GET_DIALOG_MANAGER(uiCommunityMan, UIMAN_COMMUNITY);
	if(!pComMan)
	{
		return false;
	}
	iSel = pComMan->FindBlockFriend(character_name_);
	return (0 <= iSel);	// ���� �ε����� ���´�. -1���� ũ�ٸ� �����
}

#ifdef _NA_002935_20110704_ITEM_SCORE
bool uiCharPopupMan::IsShowEquip(Object* object)
{
    if (object == NULL)
    {
        return false;
    }
    return true;
}
#endif //_NA_002935_20110704_ITEM_SCORE

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

#else //_DH_SHOW_VENDOR_LIST_POPUP
//------------------------------------------------------------------------------
void uiCharPopupMan::CheckPopupType()
{
    SetPopupInfo(g_HeroInput.GetCurrentTarget());

    /*
    eCPK_PARTY  -- have party
    eCPK_NONE   -- haven't party 
    */


    uiPartyMan* partyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);

    if (partyMan)
    {
        if (partyMan->IsParty())
        {
            //���õ��ڰ� ��Ƽ������ƴ϶��	
            if(!partyMan->IsPartyMember(g_HeroInput.GetCurrentTarget()))
            {
                m_iPopupType = partyMan->IsLeader() ? (eCPK_NONE):(eCPK_PARTY);
            }	
            else
            {
                if (partyMan->IsLeader())
                {
                    m_iPopupType = eCPK_PARTY_MASTER;
                }
                else
                {
                    m_iPopupType = eCPK_PARTY;
                }
            }

        }
        else
        {
            m_iPopupType = eCPK_NONE;
        }
    }

    m_bGroupType = IsTargetGroupMember();
    block_type_ = IsTargetBlockMember();
    #ifdef _NA_002935_20110704_ITEM_SCORE
    show_equip_ = IsShowEquip();
    #endif //_NA_002935_20110704_ITEM_SCORE

}
//=====================================================================================
//	DESC: Ÿ������ ������ ��´�
//=====================================================================================

//------------------------------------------------------------------------------
bool uiCharPopupMan::ProcessMenuFunc(int iIndex)
{
    if(!m_dwTargetObjKey) return false;

    TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR   szTemp[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR   szTemp2[INTERFACE_STRING_LENGTH] = {0,};

    switch(iIndex)
    {
    case eST_NORMAL_WHISPER_STRING_INDEX:		//�ӼӸ�
        {
            Object* pObj = (Object*)g_ObjectManager.GetObject(m_dwTargetObjKey);

            ChatDialog* pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

            uiPartyMan* partyMan  = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            assert(partyMan);

            if (pChatDlg && (partyMan->GetPartyMemberInfo(m_dwTargetObjKey) || pObj)) 
            {
                //1.��Ƽ���� ����� ��Ƽ������ 1���������� �ƴҰ�� ������Ʈ ��������
                if (partyMan->GetPartyMemberInfo(m_dwTargetObjKey))
                {
                    _PARTY_MEMBER_INFO rMember = (partyMan->GetPartyMemberInfo(m_dwTargetObjKey))->Get();
                    g_InterfaceManager.GetInterfaceString( 2010, szTemp, INTERFACE_STRING_LENGTH );
                    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %s "), szTemp, rMember.m_szCharName);
                }
                else
                {	
                    //2.��Ƽ�� �ƴ����� ������Ʈ���� ����� ������Ʈ����.
                    g_InterfaceManager.GetInterfaceString(2010, szTemp, INTERFACE_STRING_LENGTH);
                    Snprintf (szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %s "), szTemp, pObj->GetName());
                }

                if (!pChatDlg->IsVisibleWindow())
                {
                    pChatDlg->ShowWindow(TRUE);
                }

                pChatDlg->SetChatMessage(szMessage);
            }
        }
        break;
#ifdef _NA_002935_20110704_ITEM_SCORE
    case eST_NORMAL_EQUIP_SHOW_STRING_INDEX: // ��񺸱�
        {
            //�ش� �÷��̾��� ��� â�� ��������.
            uiInventoryMan* inventory_manager = GET_DIALOG_MANAGER(uiInventoryMan, UIMAN_INVENTORY);
            TargetDialog* target_pointer = 
                GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);

            if (target_pointer && inventory_manager)
            {
                Object* target_object = g_ObjectManager.GetObject(target_pointer->GetTargetKey());

                if (target_object && (target_object->IsKindOfObject(PLAYER_OBJECT)))
                {
                    Player* player = static_cast<Player *>(target_object);
                    if (player)
                    {
                        inventory_manager->SetOtherPlayerEquipInventoryDialog(player);
                    }
                }
            }
        }
        break;
#endif //_NA_002935_20110704_ITEM_SCORE
    case eST_NORMAL_FRIEND_STRING_INDEX:		//ģ���߰�
        {
            uiPartyMan* partyMan  = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            assert(partyMan);

            //1.��Ƽ����϶� ��������� �����ϰ� �׿��� ��� ������Ʈ �Ŵ��������� ����
            if (partyMan->GetPartyMemberInfo(m_dwTargetObjKey))
            {
                _PARTY_MEMBER_INFO rMember = (partyMan->GetPartyMemberInfo(m_dwTargetObjKey))->Get();
                char szCharName[MAX_CHARNAME_LENGTH + 1] = {0,};
                StrnCopy(szCharName , rMember.m_szCharName, MAX_CHARNAME_LENGTH);
                _CallBack_Friend_Add(true, szCharName);
            }
            else
            {	//2.������Ʈ����������
                g_HeroInput.ProcessUserAction(eACTION_FRIEND_ADD + eACTION_START_SKILL_CODE);
            }
        }
        break;

    case eST_NORMAL_TRADE_STRING_INDEX:		//�ŷ���û
        {
            Object* pObj = g_ObjectManager.GetObject(m_dwTargetObjKey);
            if (!pObj) 
            {
                //5833	�ŷ��� �� ���� �Ÿ��Դϴ�.
                g_InterfaceManager.GetInterfaceString(5833, szMessage ,INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
            }
            else
            {
                g_HeroInput.ProcessUserAction(eACTION_TRADE + eACTION_START_SKILL_CODE);
            }
        }
        break;

    case eST_NORMAL_VIEW_STRING_INDEX:			//���캸��
        break;

    case eST_NORMAL_PVP_STRING_INDEX:			//����û
        break;

    case eST_NORMAL_FOLLOW_STRING_INDEX:		//���󰡱�
        {
            Object	*pObj = g_ObjectManager.GetObject(m_dwTargetObjKey);
            if(!pObj) 
            {
                //5141	�Ÿ��� �־� �̿��� �������� �ʽ��ϴ�.
                g_InterfaceManager.GetInterfaceString(5141,szMessage,INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
            }
            else
            {
                g_HeroInput.ProcessUserAction(eACTION_FOLLOW_PLAYER + eACTION_START_SKILL_CODE);
            }

        }
        break;

#ifdef _KIKI_ADD_G_LOG_MENU
    case eST_NORMAL_G_LOG_STRING_INDEX:			//G-�α�
        {
#ifdef _KIKI_UI_WEB_DIALOG
            Object	*pObj = g_ObjectManager.GetObject(m_dwTargetObjKey);
            uiWebDialogMan* webMan = GET_DIALOG_MANAGER(uiWebDialogMan, UIMAN_WEB);
            if(webMan && pObj)
            {
#ifdef _KIKI_G_LOG_PROTOCOL
                webMan->RequestUserID(pObj->GetName());
#else
                webMan->ShowWebDialog(TRUE);
#endif			
            }

#endif
        }
        break;
#endif

    case eST_PARTY_MASTER_INVITE_STRING_INDEX:	//��Ƽ�ʴ�
        {
            uiPartyMan* partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY );
            Object	* pObj = g_ObjectManager.GetObject( m_dwTargetObjKey );
            if ( partyMan && pObj )
            {
                if ( partyMan->IsParty() == false )
                    partyMan->AskJoinParty( pObj->GetName() );
                else if	( partyMan->IsParty() && partyMan->IsLeader() )
                    partyMan->AskJoinParty( pObj->GetName() );
                else 
                {
                    g_InterfaceManager.GetInterfaceString( eST_PARTY_ISNOT_MASTEROFPARTY, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
                }
            }
        }
        break;

    case eST_PARTY_MASTER_MIGRATION_STRING_INDEX:	//��Ƽ�� ����
        {
            uiPartyMan* partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY );

            if (partyMan)
            {
                partyMan->ChangeMaster();
            }

            ItemShareSystem* pItemShare = GET_CAST_DIALOG(ItemShareSystem, IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM);
            if(pItemShare)
            {
                pItemShare->CloseItemShareSystemWindow();	// ��Ƽ �й� �ɼ�â �ݱ�
            }
        }
        break;

    case eST_PARTY_MASTER_KICK:					// ��Ƽ�߹�
        {
            uiPartyMan* partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY );

            if (partyMan)
            {
                partyMan->ForcedExpulsion();
            }
        }
        break;

    case eST_GROUP_ADD:	//(�׷�)������
        {
            TargetDialog* pTarget = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
            if ( pTarget )
            {
                MSG_CW_GROUP_ADD_SYN sync;
                ZeroMemory(sync.ptszMemeberName,sizeof(sync.ptszMemeberName));
                TCHAR szString[MAX_CHARNAME_LENGTH+1] ={0,};
                StrnCopy(sync.ptszMemeberName ,pTarget->GetTargetName(), MAX_CHARNAME_LENGTH);
                GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
            }
        }
        break;
    case eST_GROUP_DEL:	//(�׷�)�������
        {
            TargetDialog* pTarget = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
            if ( pTarget )
            {
                MSG_CW_GROUP_DEL_SYN sync;
                ZeroMemory(sync.ptszMemeberName,sizeof(sync.ptszMemeberName));
                TCHAR szString[MAX_CHARNAME_LENGTH+1] ={0,};
                StrnCopy(sync.ptszMemeberName ,pTarget->GetTargetName(), MAX_CHARNAME_LENGTH);
                GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
            }
        }
        break;

    case eST_BLOCK_ADD:	//����
        {
            TargetDialog* pTarget = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
            if (pTarget)
            {
#if defined(_KOREA)
                if (g_slangNameFilter.CheckIncludeString(pTarget->GetTargetName()))
#else
                if (GlobalFunc::CheckIncludeStringOnAbroad(pTarget->GetTargetName()))
#endif
                {
                    // ������ ������ ���� �޽��� ����ϰ� ��Ŷ������ ����
                    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
                    // 5346	�߸��� ���� �Դϴ�.
                    g_InterfaceManager.GetInterfaceString( 5346, szMessage, MAX_MESSAGE_LENGTH );
                    GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage );			
                }
                else
                {
                    MSG_CW_FRIEND_BLOCK_SYN sync;
                    ZeroMemory(sync.ptszBlockName,sizeof(sync.ptszBlockName));
                    TCHAR szString[MAX_CHARNAME_LENGTH+1] ={0,};
                    StrnCopy(szString, pTarget->GetTargetName(), MAX_CHARNAME_LENGTH);
                    StrnCopy(sync.ptszBlockName ,szString, MAX_CHARNAME_LENGTH);

                    GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));
                }
            }
        }
        break;
    case eST_BLOCK_DELETE:
        {
            TargetDialog* pTarget = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
            if (pTarget)
            {
                MSG_CW_FRIEND_BLOCK_FREE_SYN sync;
                ZeroMemory(sync.ptszBlockName,sizeof(sync.ptszBlockName));
                StrnCopy(sync.ptszBlockName , pTarget->GetTargetName(), MAX_CHARNAME_LENGTH);
                GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
            }
        }
        break;
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
    case eST_GUILD_INVITE:
        {
            TargetDialog* target_dialog = 
                GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
            if (target_dialog != NULL)
            {
                GuildSystem::PacketFunction::SEND_CG_GUILD_INVITE_SYN(target_dialog->GetTargetName());
            }
        }
        break;
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
    case eST_CANCEL:
        break;

    }

    return true;
}

//------------------------------------------------------------------------------
bool uiCharPopupMan::IsTargetGroupMember()
{
    int iSel = -1;
    DWORD dwTargetKey = g_HeroInput.GetCurrentTarget();
    uiCommunityMan* pComMan = GET_DIALOG_MANAGER(uiCommunityMan, UIMAN_COMMUNITY);

    TargetDialog* pTarget = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);

    if(!pTarget)
    {
        return false;
    }

    if(!pComMan)
    {
        return false;
    }
    iSel = pComMan->FindGroup(pTarget->GetTargetName());

    return (0 <= iSel);	// �׷����� �ε����� ���´�. -1���� ũ�ٸ� �����
}

//------------------------------------------------------------------------------
bool uiCharPopupMan::IsTargetBlockMember()
{
    int iSel = -1;
    DWORD dwTargetKey = g_HeroInput.GetCurrentTarget();
    uiCommunityMan* pComMan = GET_DIALOG_MANAGER(uiCommunityMan, UIMAN_COMMUNITY);
    TargetDialog* pTarget = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
    if(!pTarget)
    {
        return false;
    }

    if(!pComMan)
    {
        return false;
    }
    iSel = pComMan->FindBlockFriend(pTarget->GetTargetName());
    return (0 <= iSel);	// ���� �ε����� ���´�. -1���� ũ�ٸ� �����
}

#ifdef _NA_002935_20110704_ITEM_SCORE
bool uiCharPopupMan::IsShowEquip()
{
    Object* object = static_cast<Object*>(g_ObjectManager.GetObject(m_dwTargetObjKey));
    if (object == NULL)
    {
        return false;
    }
    return true;
}
#endif //_NA_002935_20110704_ITEM_SCORE


#endif//_DH_SHOW_VENDOR_LIST_POPUP