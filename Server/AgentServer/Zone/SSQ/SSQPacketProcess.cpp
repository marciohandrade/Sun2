#include "stdafx.h"
#include <PacketStruct_AG.h>
#include <PacketStruct_AW.h>

#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>

#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/BattleServerSession.h>
#include <ServerSessions/FieldServerSession.h>

#include <Zone/DungeonSSQ.h>
#include <Zone/ZoneManager.h>

//==================================================================================================
//  for debugging control
#define __WAVERIX_LOG_FORMAT_0S(msg)    \
{    SUNLOG(eCRITICAL_LOG, "[Func:%s][Line:%3u] %s", __FUNCTION__, __LINE__, msg);    }

#define __WAVERIX_LOG_FORMAT_0S1N(msg, value)    \
{    SUNLOG(eCRITICAL_LOG, "[Func:%s][Line:%3u] %s (%u)", __FUNCTION__, __LINE__, msg, value);    }


#ifdef _DEBUG
    #define WAVERIX_DEBUG_LOG_0S(msg)   __WAVERIX_LOG_FORMAT_0S(msg)
    #define WAVERIX_DEBUG_LOG_0S1N(msg) __WAVERIX_LOG_FORMAT_0S1N(msg)
#else
    #define WAVERIX_DEBUG_LOG_0S(msg)
    #define WAVERIX_DEBUG_LOG_0S1N(msg)
#endif

//
//==================================================================================================
//
//
#include <Zone/WarControlManager.h>
#include "ISSQHandler.h"
#include "SSQControlInfos.hxx"
#include "SSQUpdateCtrl.hxx"
#include "SSQPacketProcess.hxx"
#include "SSQControlManagerImpl.hxx"


//--------------------------------------------------------------------------------------------------
BROADCASTER_MSG_CG_STATUS_SSQ_INFO_BRD
SSQMsgRouterInAgent::ms_msgBRD_CG;

BROADCASTER_MSG_AW_OPERATION_ROOM_CTRL_CMD
SSQMsgRouterInAgent::ms_msgCMD_AW;
//--------------------------------------------------------------------------------------------------


VOID
SSQMsgRouterInAgent::SlotUpdate()
{
    UserManager* const pUserManager = UserManager::Instance();

    TARGET_LIST::iterator it = m_Container.begin(),
                          itend = m_Container.end();
    while(it != itend)
    {
        TARGET_LIST::value_type& rNode = *it;

        const DWORD playerKey = rNode.first;
        ISSQHandler* const pISSQHandler = rNode.second;

        if(!(playerKey && pISSQHandler))
        {
            TAllocDelete(storage_info::value_type, pISSQHandler);
            it = m_Container.erase(it);    // check - synchronize
            continue;
        }

        if(ISSQHandler::UPDATE_FAIL == pISSQHandler->Update())
        {
            User* const pUser = pUserManager->GetUserByObjKey(playerKey);
            if(pUser)
            {
                pISSQHandler->Release();
                ISSQHandlerD& rHandler = pUser->SSQHandler;
                rHandler = ISSQHandlerD();
            }

            TAllocDelete(storage_info::value_type, pISSQHandler);
            it = m_Container.erase(it);
            continue;
        }

        //  last operation
        ++it;
    }
}


//--------------------------------------------------------------------------------------------------


VOID
SSQMsgRouterInAgent::SlotClear(User* const pUser)
{
    const DWORD playerKey = pUser->GetPlayerKey();
    ISSQHandler* pHandler = FindSlot(playerKey);
    if(pHandler)
    {
        pUser->SSQHandler = ISSQHandlerD();
        //  ReleaseNode(*pNode);
        //  혹시 리스트에 남겨지는 상황이 발생하는 지 확인할 것.
    }

    WAVERIX_DEBUG_CODE(
        SUNLOG(eCRITICAL_LOG, "SlotClear - remain count %u", m_Container.size());
    );
}


//--------------------------------------------------------------------------------------------------


VOID
SSQMsgRouterInAgent::Update(const SSQCODE CodeOfSSQ)
{
    if(!(m_pUpdateCtrl && m_LinkedServerSessionIdx))
        return;

    ServerSessionEx* const pServer =
        ServerSessionManager::Instance()->FindServer(m_LinkedServerSessionIdx);
    if(!pServer)
    {    // auto-clear
        m_LinkedServerSessionIdx = 0;
        DungeonSSQ* const pDungeon = m_pUpdateCtrl->GetSSQField();
        if(pDungeon)
            pDungeon->SetClearProcess();
    }

    //SSQMsgRouterInAgent& rThis = *this;
    //Foreach(&SSQMsgRouterInAgent::NodeUpdate, rThis);
    SlotUpdate();

    if(pServer)
    {    // AG
        MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD* const pMsg = m_msgCMD_AG.GetPacket();
        const WORD size = pMsg->GetSize();
        if(sizeof(MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD) != size)
        {
            m_msgCMD_AG.SetKey(CodeOfSSQ);
            pServer->SendPacket(pMsg, size);
            m_msgCMD_AG.Reset();
        }
    };

    {    // CG
        MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD* const pMsg = m_msgCMD_CG.GetPacket();
        const WORD size = pMsg->GetSize();
        if(sizeof(MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD) != size)
        {
            m_msgCMD_CG.ProcessSSQUser(m_pUpdateCtrl->GetSSQField());
            m_msgCMD_CG.Reset();
        }
    };

    UpdateSharedMsg();
};


//--------------------------------------------------------------------------------------------------


VOID
SSQMsgRouterInAgent::UpdateSharedMsg()
{
    {    // CG
        MSG_CG_STATUS_SSQ_INFO_BRD* const pMsg = ms_msgBRD_CG.GetPacket();
        const WORD size = pMsg->GetSize();
        if(sizeof(MSG_CG_STATUS_SSQ_INFO_BRD) != size)
        {
            //m_msgBRD_CG.SetKey(CodeOfSSQ);
            ms_msgBRD_CG.ProcessAllUser();
            ms_msgBRD_CG.Reset();
        }
    };
    {
        MSG_AW_OPERATION_ROOM_CTRL_CMD* const pMsg = ms_msgCMD_AW.GetPacket();
        const WORD size = pMsg->GetSize();
        if(sizeof(MSG_AW_OPERATION_ROOM_CTRL_CMD) != size)
        {
            //world server 연결 여부에 따른 처리도 감안할 것.
            AgentServer::GetInstance()->SendToWorldServer(pMsg, size);
            ms_msgCMD_AW.Reset();
        }
    }
}


//==================================================================================================
// Server<->Server (A<->G) SSQ_MESSAGE_HANDLERs
BOOLEAN
SSQMsgRouterInAgent::Routing(const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header)
{
    WAR_TAG_P pIt = const_cast<WAR_TAG_P>(pMsg);
    WAR_TAG_P pEnd = pIt + header.size;
    for(WORD size = 0; pIt < pEnd; pIt += size)
    {
        size = 10000;    // 미 설정에 대한 스킵 설정

        switch(pIt->_TAG)
        {
        case SSQ_S2S_FIELD_CREATE:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_FIELD_CREATE);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_FIELD_CREATE] \n");
            }
            break;
        case SSQ_S2S_FIELD_CREATED:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_FIELD_CREATED);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_FIELD_CREATED] \n");
            }
            break;
        case SSQ_S2S_FIELD_BLOCK:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_FIELD_BLOCK);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_FIELD_BLOCK] \n");
            }
            break;
        case SSQ_S2S_FIELD_DESTORY:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_FIELD_DESTORY);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_FIELD_DESTORY] \n");
            }
            break;
        case SSQ_S2S_FIELD_DESTORYED:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_FIELD_DESTORYED);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_FIELD_DESTORYED] \n");
            }
            break;
        case SSQ_S2S_GAMBLE_START:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_GAMBLE_START);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_GAMBLE_START] \n");
            }
            break;
        case SSQ_S2S_GAMBLE_RESULT:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_GAMBLE_RESULT);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_GAMBLE_RESULT] \n");
                const eTIME_TYPE updater_type = m_pUpdateCtrl->GetTimeType();
                if(TT_WAITING == updater_type)
                {
                    SSQUpdateCtrl_Waiting* const pUpdater =
                        static_cast<SSQUpdateCtrl_Waiting*>(m_pUpdateCtrl);
                    pUpdater->SetGambleResult(rMsg);
                }
                else
                {
                    ASSERT(!"Sync Flow Error , 정해진 규칙이 위반되었습니다");
                }
            }
            break;
        case SSQ_S2S_BATTLE_COUNTDOWN:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_BATTLE_COUNTDOWN);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_BATTLE_COUNTDOWN] \n");
            }
            break;
        case SSQ_S2S_BATTLE_START:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_BATTLE_START);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_BATTLE_START] \n");
            }
            break;
        case SSQ_S2S_BATTLE_CANCEL:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_BATTLE_CANCEL);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_BATTLE_CANCEL] \n");
            }
            break;
        case SSQ_S2S_BATTLE_CLEAR:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_BATTLE_CLEAR);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_BATTLE_CLEAR] \n");
            }
            break;
        case SSQ_S2S_BATTLE_CLEARED:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_BATTLE_CLEARED);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_BATTLE_CLEARED] \n");

                m_pUpdateCtrl->ProcessCleared();
            }
            break;
        case SSQ_S2S_BATTLE_END_REQUEST:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_BATTLE_END_REQUEST);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_BATTLE_END_REQUEST] \n");
                m_pUpdateCtrl->BattleEndRequested();
            }
            break;

        case SSQ_S2S_OBSERVER_MODE_RESULT:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_OBSERVER_MODE_RESULT);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_OBSERVER_MODE_RESULT] \n");
                User* const pUser = UserManager::Instance()->GetUserByObjKey(rMsg.ObjectKey);
                if(pUser == 0)
                    break;

                WAR_MSG_SINGLE_ASSEMBLER_SSQ_INFO_BRD<
                    MSGSUB_SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT > assembler;
                assembler.SerializeFrom(rMsg.C2GMsg);
                assembler.Completion();
                assembler.SendPacket(pUser);
            }
            break;
        case SSQ_S2S_OBSERVER_MODE_SELECT_WINDOW:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_OBSERVER_MODE_SELECT_WINDOW);
                WAVERIX_DEBUG_LOG_0S("[SSQ_S2S_OBSERVER_MODE_SELECT_WINDOW] \n");
                User* const pUser = UserManager::Instance()->GetUserByObjKey(rMsg.ObjectKey);
                if(!pUser)
                    break;

                WAR_MSG_SINGLE_ASSEMBLER_SSQ_INFO_BRD<
                    MSGSUB_SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_G2C_SELECT > assembler;
                assembler.Completion();
                assembler.SendPacket(pUser);
            }
            break;

        //------------------------------------------------------------------------------------------
        // C<->G SSQ_MESSAGE_HANDLERs
        case SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_NOTICE_ENABLE_ENTER:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_NOTICE_ENABLE_ENTER);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_NOTICE_ENABLE_ENTER] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_NOTICE_CLOSE_ENTER:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_NOTICE_CLOSE_ENTER);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_NOTICE_CLOSE_ENTER] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_NOTICE_GAMBLE_START:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_NOTICE_GAMBLE_START);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_NOTICE_GAMBLE_START] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_NOTICE_BATTLE_START:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_NOTICE_BATTLE_START);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_NOTICE_BATTLE_START] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_NOTICE_BATTLE_END:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_NOTICE_BATTLE_END);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_NOTICE_BATTLE_END] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION] \n");

                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                };
            }
            break;

        case SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE] \n");

                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                };
            }
            break;

        case SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE);
                //WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE] \n");

                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE);
                //WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_TRAP_SELF_DESTRUCTION:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_TRAP_SELF_DESTRUCTION);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_TRAP_SELF_DESTRUCTION] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION] \n");

                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS] \n");

                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS);
                //WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_INFORMATION_REMAINED_TIME:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_INFORMATION_REMAINED_TIME);
                //WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_INFORMATION_REMAINED_TIME] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        case SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED] \n");

                User* const pUser = UserManager::Instance()->GetUserByObjKey(rMsg.ObjectKey);
                if(!pUser)
                    break;

                WAR_MSG_SINGLE_ASSEMBLER_SSQ_INFO_BRD<
                    MSGSUB_SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED > assembler;
                assembler.SerializeFrom(rMsg);
                assembler.Completion();
                assembler.SendPacket(pUser);
            }
            break;
        case SSQ_PROTOCOL_INFORMATION_QUEST_FAIL_RESULT:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_INFORMATION_QUEST_FAIL_RESULT);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_INFORMATION_QUEST_FAIL_RESULT] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;
        case SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT] \n");
#ifdef _NA_002950_20110627_SSQ_NOTICE
                WAR_MSG_HDR* const pHeader = &ms_msgBRD_CG.MSG_HEADER;
#else
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
#endif
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;
        case SSQ_PROTOCOL_CTRL_TAG_TAIL:
            {
                const SSQ_MSG_CTRL_TAG_TAIL& rMsg = static_cast<const SSQ_MSG_CTRL_TAG_TAIL&>(*pIt);
                size = rMsg.GetSize();
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_CTRL_TAG_TAIL] \n");
                WAR_MSG_HDR* const pHeader = &m_msgCMD_CG.MSG_HEADER;
                {
                    rMsg.SerializeTo(pHeader);
                }
            }
            break;

        default:
            {
                SUNLOG(eCRITICAL_LOG,
                       "["__FUNCTION__"] Can't Readable SSQ Msg Format %2u", pIt->_TAG);
            #ifdef _DEBUG
                ASSERT(!"["__FUNCTION__"] Can't Readable SSQ Msg Format");
            #endif
            }
            break;
        }
    }
    return TRUE;
}


//--------------------------------------------------------------------------------------------------


BOOLEAN
SSQMsgRouterInAgent::RoutingSpecial(const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header)
{
    const SSQCODE CodeOfSSQ = m_pUpdateCtrl->GetCodeOfSSQ();
    //const SSQInfo* const pSSQInfo = m_pUpdateCtrl->GetSSQInfo();
    //WAR_MSG_HDR* const pHeader = &m_msgCMD_AG.MSG_HEADER;

    WAR_TAG_P pIt = const_cast<WAR_TAG_P>(pMsg);
    WAR_TAG_P pEnd = pIt + header.size;
    for(WORD size = 0; pIt < pEnd; pIt += size)
    {
        size = 10000;    // 미 설정에 대한 스킵 설정

        switch(pIt->_TAG)
        {
        case SSQ_S2S_GM_CONTROL_REQ:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_GM_CONTROL_REQ);
                __WAVERIX_LOG_FORMAT_0S("[SSQ_S2S_GM_CONTROL_REQ]");

                const GM_Spec::MsgFormat& rCommand = rMsg.Command;
                if(rCommand.GM_Cmd == GM_Spec::eCmd_open)
                {
                    const DWORD ERRCODE_EXIST_FIELD = 100;
                    const DWORD ERRCODE_CONFIG_FAIL = 110;
                    //////////////////////////////////////////////////////////////////////////
                    //  <LOGIC CONTROL>
                    FlowControl flow;
                    FLOWCONTROL_START()
                    {
                        DungeonSSQ* const pDungeon = m_pUpdateCtrl->GetSSQField();
                        IfFailThenBreak(!pDungeon, ERRCODE_EXIST_FIELD);

                        SSQInfoParser* const pSSQInfoParser = SSQInfoParser::Instance();
                        //SSQInfo* pSSQInfoT = const_cast<SSQInfo*>(pSSQInfo);

                        sTIME_INFO tmInfo;
                        sTIME_INFO::SetTimeValue(tmInfo);

                        BOOLEAN isFastTrack = !!(rCommand.Option & GM_Spec::eOption_FastTrack);
                        IfFailThenBreak(pSSQInfoParser->GM_SpecialControl_OpenStart(
                            rCommand.CodeOfSSQ, tmInfo.AcmTime, isFastTrack), ERRCODE_CONFIG_FAIL);

                        //  replace pCtrl
                        SSQUpdateCtrl* pBkUpdate = m_pUpdateCtrl;
                        const eTIME_TYPE updater_type = pBkUpdate->GetTimeType();

                        SSQUpdateCtrl_None* const pNoneCtrl = TAllocNew(SSQUpdateCtrl_None_Info::value_type);
                        {
                            pNoneCtrl->Migration(pNoneCtrl, m_pUpdateCtrl);
                            pNoneCtrl->Init(tmInfo);
                            pNoneCtrl->RegisterUpdateCtrl(pNoneCtrl);
                            if(isFastTrack)
                                pNoneCtrl->EnableFastTrack();
                        }

                        //  < 이것 함수 호출 하나로 해결할 수 있도록 하자.
                        switch(updater_type)
                        {
                        case TT_NONE:   TAllocDelete(SSQUpdateCtrl_None_Info::value_type, pBkUpdate); break;
                        case TT_NOTICE: TAllocDelete(SSQUpdateCtrl_Notice_Info::value_type, pBkUpdate); break;
                        case TT_WAITING:TAllocDelete(SSQUpdateCtrl_Waiting_Info::value_type, pBkUpdate);break;
                        case TT_BATTLE: TAllocDelete(SSQUpdateCtrl_Battle_Info::value_type, pBkUpdate); break;
                        case TT_CLEAR:  TAllocDelete(SSQUpdateCtrl_Clear_Info::value_type, pBkUpdate);  break;
                        }

                        m_pUpdateCtrl = pNoneCtrl;

                    }
                    FLOWCONTROL_END;
                    
                    const DWORD errCode = flow.GetErrorCode();
                    const TCHAR* pszMsgFormat
                        = errCode == ERRCODE_EXIST_FIELD ? "Open Porcess Fail - mapcode [%u] - already opened"
                        : errCode == ERRCODE_CONFIG_FAIL ? "Open Process Fail - mapcode [%u] - 시간 제약조건이 위반되었을 가능성이 높아용."
                        :                                  "Open Process Complete - mapcode [%u] - wait Notice";

                    User* const pGMUser =
                        UserManager::Instance()->GetUserByObjKey(rCommand.GMObjectKey);
                    if(pGMUser)
                    {    // GM Notice
                        MSG_CG_ETC_DEBUGINFO_CMD msgCMD;
                        INT iLen = _snprintf(msgCMD.m_pszChatMsg, _countof(msgCMD.m_pszChatMsg),
                                             pszMsgFormat, rCommand.CodeOfSSQ);
                        msgCMD.m_byMsgLength = (BYTE)iLen;
                        pGMUser->SendPacket(&msgCMD, msgCMD.GetSize());
                    }
                }
                else if(rCommand.GM_Cmd == GM_Spec::eCmd_close)
                {
                    //  replace pCtrl
                    SSQUpdateCtrl* pBkUpdate = m_pUpdateCtrl;
                    const eTIME_TYPE updater_type = pBkUpdate->GetTimeType();

                    sTIME_INFO tmInfo;
                    sTIME_INFO::SetTimeValue(tmInfo);

                    SSQUpdateCtrl_Clear* const pClearCtrl = TAllocNew(SSQUpdateCtrl_Clear_Info::value_type);
                    {
                        pClearCtrl->Migration(pClearCtrl, m_pUpdateCtrl);
                        pClearCtrl->Init(tmInfo);

                        pClearCtrl->RegisterUpdateCtrl(pClearCtrl);
                        //rNode.pUpdater = pClearCtrl;
                    }

                    //  < 이것 함수 호출 하나로 해결할 수 있도록 하자.
                    switch(updater_type)
                    {
                    case TT_NONE:   TAllocDelete(SSQUpdateCtrl_None_Info::value_type, pBkUpdate); break;
                    case TT_NOTICE: TAllocDelete(SSQUpdateCtrl_Notice_Info::value_type, pBkUpdate); break;
                    case TT_WAITING:TAllocDelete(SSQUpdateCtrl_Waiting_Info::value_type, pBkUpdate); break;
                    case TT_BATTLE: TAllocDelete(SSQUpdateCtrl_Battle_Info::value_type, pBkUpdate); break;
                    case TT_CLEAR:  TAllocDelete(SSQUpdateCtrl_Clear_Info::value_type, pBkUpdate); break;
                    }

                    m_pUpdateCtrl = pClearCtrl;

                    User* const pGMUser =
                        UserManager::Instance()->GetUserByObjKey(rCommand.GMObjectKey);
                    if(pGMUser)
                    {    // GM Notice
                        MSG_CG_ETC_DEBUGINFO_CMD msgCMD;
                        INT iLen = _snprintf(msgCMD.m_pszChatMsg, _countof(msgCMD.m_pszChatMsg),
                                             "Closing Process - mapcode [%u] - wait a minute... "
                                             "a minute! a minute!",
                                             rCommand.CodeOfSSQ);
                        msgCMD.m_byMsgLength = (BYTE)iLen;
                        pGMUser->SendPacket(&msgCMD, msgCMD.GetSize());
                    }
                }
                //case GM_Spec::eCmd_status:
                //    break;
                //case GM_Spec::eCmd_jump:
                //    break;
                //}
            }
            break;
        case SSQ_PROTOCOL_CTRL_TAG_TAIL:
            {
                const SSQ_MSG_CTRL_TAG_TAIL& rMsg = static_cast<const SSQ_MSG_CTRL_TAG_TAIL&>(*pIt);
                size = rMsg.GetSize();

                //pOffset += sizeof(SSQ_MSG_CTRL_TAG_TAIL);
                //rHdr.size -= sizeof(SSQ_MSG_CTRL_TAG_TAIL);
                WAVERIX_DEBUG_LOG_0S("[SSQ_PROTOCOL_CTRL_TAG_TAIL] \n");
            }
            break;
        default:
            {
                SUNLOG(eCRITICAL_LOG,
                       "["__FUNCTION__"] Can't Readable SSQ Msg Format %2u", pIt->_TAG);
            #ifdef _DEBUG
                ASSERT(!"["__FUNCTION__"] Can't Readable SSQ Msg Format");
            #endif
            }
            break;
        }
    }

    return TRUE;
}


//--------------------------------------------------------------------------------------------------


