#include "SunClientPrecompiledHeader.h"

#include "uiQuestMan.h"
#include "Application.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#include "uiHelpMan/uiHelpMan.h"
#include "ObjectManager.h"
#include "PacketRequestManager.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"
//------------------------------------------------------------------------------
/**
*/
void
uiQuestMan::InitPKBooleans()
{
    this->m_vPKBool.clear();

    for (int i=ePKBOOL_QUEST_BASE; i<ePKBOOL_QUEST_MAX; ++i)
    {
        uicPacketControl packetControl;
        this->m_vPKBool.push_back(packetControl);
    }
}

//------------------------------------------------------------------------------
/**
*/
void 
uiQuestMan::NET_SEND_CG_QUEST_ACCEPT_SYN(QCODE QuestCode, BOOL IsItemAccept, POSTYPE Atpos)
{
	if ( this->SafePKBool(ePKBOOL_QUEST_ACCEPT) )
	{
		SetPKBool(ePKBOOL_QUEST_ACCEPT);
		MSG_CG_QUEST_ACCEPT_SYN	SendPacket;
		m_PendingQuestCode[ePKBOOL_QUEST_ACCEPT] = SendPacket.m_QuestCode = QuestCode;
		SendPacket.m_byIsItemAccept = IsItemAccept;
		SendPacket.m_QuestItemPos = Atpos;
		GlobalFunc::SendPacketToGameServer(&SendPacket, sizeof(SendPacket));

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
		uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));
		if (helpMan)
		{
			helpMan->ShowQuestHelp((int)QuestCode,eHK_QUEST);
		}
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW
        uiGuideSystemMan* guide_manager =
            static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

        if (guide_manager)
        {
            guide_manager->CheckHelpMessage(TotalGuideNotice::kQuest, TotalGuideNotice::kAcceptQuest,QuestCode);
        }

		if (!GENERALPARAM->IsNet())
		{
			MSG_CG_QUEST_ACCEPT_ACK SendPacketAck;
			g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void 
uiQuestMan::NET_SEND_CG_QUEST_COMPLETE_SYN(QCODE QuestCode)
{
//	if ( SafePKBool(ePKBOOL_QUEST_COMPLETE) )
	if (SetPendingQuestCompletePacket(QuestCode) == false)
		return;

	{
		SetPKBool(ePKBOOL_QUEST_COMPLETE);

		MSG_CG_QUEST_COMPLETE_SYN	SendPacket;
		m_PendingQuestCode[ePKBOOL_QUEST_COMPLETE] = SendPacket.m_QuestCode = QuestCode;
		GlobalFunc::SendPacketToGameServer(&SendPacket, sizeof(SendPacket));

        {
            MSG_CG_QUEST_COMPLETE_SYN* syn = new MSG_CG_QUEST_COMPLETE_SYN;
            MSG_CG_QUEST_COMPLETE_ACK* ack = new MSG_CG_QUEST_COMPLETE_ACK;
            MSG_CG_QUEST_COMPLETE_NAK* nak = new MSG_CG_QUEST_COMPLETE_NAK;
            ack->m_QuestCode = syn->m_QuestCode = SendPacket.m_QuestCode;

            PacketRequestManager::Instance()->PushPacket(syn, 
                                                        ack, 
                                                        nak,
                                                        sizeof(MSG_CG_QUEST_COMPLETE_SYN), 
                                                        PACKET_WAITING_MILLISECOND,
                                                        true);
        }

		if (!GENERALPARAM->IsNet())
		{
			MSG_CG_QUEST_COMPLETE_ACK SendPacketAck;
            SendPacketAck.m_QuestCode = QuestCode;
			g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
		}
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
uiQuestMan::NET_SEND_CG_QUEST_REWARD_SYN(QCODE QuestCode, BYTE bSelectedItem)
{
	if ( this->SafePKBool(ePKBOOL_QUEST_REWARD) )
	{
		SetPKBool(ePKBOOL_QUEST_REWARD);
		MSG_CG_QUEST_REWARD_SYN	SendPacket;
		m_PendingQuestCode[ePKBOOL_QUEST_REWARD] = SendPacket.m_QuestCode = QuestCode;
		SendPacket.m_SelectedItem = bSelectedItem;
		GlobalFunc::SendPacketToGameServer(&SendPacket, sizeof(SendPacket));

		if (!GENERALPARAM->IsNet())
		{
			MSG_CG_QUEST_REWARD_ACK SendPacketAck;

			// 처리부분 고심
			SendPacketAck.m_Money = 100;

			SendPacketAck.m_i64Exp = 0;

			SendPacketAck.m_ItemInfo.m_InvenCount = 0;
			SendPacketAck.m_ItemInfo.m_TmpInvenCount = 0;

			g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, SendPacketAck.GetSize());
		}
	}
}
	
//-------------------------------------------------------------------------------------------
/**
*/
void 
uiQuestMan::NET_SEND_CG_QUEST_FAILED_SYN(QCODE QuestCode)
{
	if ( this->SafePKBool(ePKBOOL_QUEST_FAILED) )
	{
		SetPKBool(ePKBOOL_QUEST_FAILED);
		MSG_CG_QUEST_FAILED_SYN	SendPacket;
		m_PendingQuestCode[ePKBOOL_QUEST_FAILED] = SendPacket.m_QuestCode = QuestCode;
		GlobalFunc::SendPacketToGameServer(&SendPacket, sizeof(SendPacket));

		if (!GENERALPARAM->IsNet())
		{
			MSG_CG_QUEST_FAILED_ACK SendPacketAck;
			g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
		}
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::NET_SEND_CG_QUEST_ABANDON_SYN(QCODE QuestCode,BOOL bForce)
{
	if ( this->SafePKBool(ePKBOOL_QUEST_ABANDON) )
	{
		SetPKBool(ePKBOOL_QUEST_ABANDON);
		MSG_CG_QUEST_ABANDON_SYN	SendPacket;
		m_PendingQuestCode[ePKBOOL_QUEST_ABANDON] = SendPacket.m_QuestCode = QuestCode;
		SendPacket.m_Force = bForce;
		GlobalFunc::SendPacketToGameServer(&SendPacket, sizeof(SendPacket));

		if (!GENERALPARAM->IsNet())
		{
			MSG_CG_QUEST_ABANDON_ACK SendPacketAck;
			SendPacket.m_Force = bForce;
			g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
		}
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void			
uiQuestMan::NET_SEND_CG_QUEST_EVENT_AREA_SYN(QCODE QuestCode, DWORD AreaID)
{
	if ( SafePKBool(ePKBOOL_QUEST_EVENT_AREA) )
	{
		SetPKBool(ePKBOOL_QUEST_EVENT_AREA);
		MSG_CG_QUEST_EVENT_AREA_SYN	SendPacket;
		m_PendingQuestCode[ePKBOOL_QUEST_EVENT_AREA] = SendPacket.m_QuestCode = QuestCode;
		m_PendingArea = SendPacket.m_AreaID = AreaID;

		GlobalFunc::SendPacketToGameServer(&SendPacket, sizeof(SendPacket));

		if (!GENERALPARAM->IsNet())
		{
			MSG_CG_QUEST_EVENT_AREA_ACK SendPacketAck;
			g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
		}
	}	
}

