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
#include <AgentParty/AgentPartyManager.h>

#include <Zone/WarControlManager.h>
#include "ISSQHandler.h"
#include "SSQControlInfos.hxx"
#include "SSQUpdateCtrl.hxx"
#include "SSQPacketProcess.hxx"
#include "SSQControlManagerImpl.hxx"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <ChaosZoneSystem/BattleGroundManager.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

//==================================================================================================
//  <SSQControlManagerImpl>


SSQControlManagerImpl::SSQControlManagerImpl()
{
}


SSQControlManagerImpl::~SSQControlManagerImpl()
{
}


//--------------------------------------------------------------------------------------------------


SSQ_UPDATERS_IT
SSQControlManagerImpl::_FindCodeOfSSQ(const SSQCODE CodeOfSSQ)
{
    SSQ_UPDATERS_IT itFind =
        std::find_if(UpdateList.begin(), UpdateList.end(),
                     boost::bind(std::equal_to<SSQCODE>(),
                                 boost::bind(&SSQ_UPDATER_NODE::GetCodeOfSSQ, _1),
                                 CodeOfSSQ));
    return itFind;
}


//--------------------------------------------------------------------------------------------------


SSQ_UPDATERS_IT
SSQControlManagerImpl::_FindZoneKeyofSSQ(const KEYTYPE KeyOfSSQ)
{
    SSQ_UPDATERS_IT itFind =
        std::find_if(UpdateList.begin(), UpdateList.end(),
                     boost::bind(std::equal_to<KEYTYPE>(),
                                 boost::bind(&SSQ_UPDATER_NODE::GetKey, _1),
                                 KeyOfSSQ));
    return itFind;
}


//--------------------------------------------------------------------------------------------------


VOID
SSQControlManagerImpl::_RegisterUpdateCtrl(const SSQCODE CodeOfSSQ, SSQUpdateCtrl* const pCtrl)
{
    SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);

    if(itFind != UpdateList.end())
    {
        SSQ_UPDATER_NODE& rNode = *itFind;
        rNode.pUpdater = pCtrl;
    }
    else
    {
        ASSERT(!"Impossible");
    }
}


//--------------------------------------------------------------------------------------------------


VOID
SSQControlManagerImpl::_InitializeUpdateCtrl()
{
    SSQInfoParser* const pSSQInfoParser = SSQInfoParser::Instance();

    sTIME_INFO timeInfo;
    sTIME_INFO::SetTimeValue(timeInfo);

    typedef SSQInfoParser::SSQINFO_TABLE    SSQINFO_TABLE;
    const SSQINFO_TABLE& rTable = pSSQInfoParser->GetRefSSQInfo();
    FOREACH_CONTAINER(const SSQINFO_TABLE::value_type& rNode, rTable, SSQINFO_TABLE)
    {
        const SSQCODE CodeOfSSQ = rNode.first;
        SSQInfo* const pSSQInfo = rNode.second;
        SSQUpdateCtrl_None* const pCtrl = TAllocNew(SSQUpdateCtrl_None_Info::value_type);
        SSQMsgRouterInAgent* const pRouter = TAllocNew(router_info::value_type);

        pCtrl->MigrationDefault(CodeOfSSQ, this, pRouter, pSSQInfo);
        pCtrl->Init(timeInfo);

        //_RegisterUpdateCtrl(IDofSSQ, pCtrl);    <- 사용상 혼재로 코드를 이곳으로 옮겨버렸다.
        SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);
        ASSERT(itFind == UpdateList.end());
        UpdateList.push_back(SSQ_UPDATER_NODE(CodeOfSSQ, pCtrl));
    }

    //OpSSQInfoController opr(this);
    //pSSQInfoParser->ForeachSSQInfoBin(&OpSSQInfoController::Register, opr);
    pSSQInfoParser->_LoadWat("./data/Map/Object/map_event01/map_event01.wat", false);
}


//--------------------------------------------------------------------------------------------------


VOID
SSQControlManagerImpl::_ReleaseUpdateCtrl()
{
    FOREACH_CONTAINER(SSQ_UPDATER_NODE& rNode, UpdateList, SSQ_UPDATERS)
    {
        if(!rNode.pUpdater)
            continue;

        SSQUpdateCtrl* const pCtrl = rNode.pUpdater;
        TAllocDelete(router_info::value_type, pCtrl->m_pMsgRouter);

        switch(pCtrl->m_CurType)
        {
        #undef SSQ_UPDATE_CTRL_NODE
        #define SSQ_UPDATE_CTRL_NODE(_type)    \
            case _type::value: TAllocDelete(_type::value_type, pCtrl); break;

        SSQ_UPDATE_CTRL_LIST();
        }
    }
}


//==================================================================================================
//  <Speical Utilities Implementation>
//
namespace SSQControlInfo
{
//──────────────────────────────────────────────


VOID
SSQSingletonNotice::Broadcast_Notice(SSQUpdateCtrl* const pUpdater,
                                     const eNotice_Steps notice_type, const DWORD arg0)
{
    if(!m_bTransaction)
        return;

    BOOLEAN bContinue = TRUE;
    if(m_Notice_Steps & notice_type)
    {
        switch(notice_type)
        {
        case eNotice_OpenCountdown: bContinue = m_Argument0 != arg0; break;
        case eNotice_Open: bContinue = FALSE; break;
        case eNotice_CloseCountdown: bContinue = m_Argument0 != arg0; break;
        case eNotice_ClosePortal: bContinue = FALSE; break;
        case eNotice_Close: bContinue = FALSE; break;
        default:    ASSERT(!"Unexpected Status"); bContinue = FALSE; break;
        }
    }

    if(bContinue == 0)
        return;

    const SSQCODE CodeOfSSQ = pUpdater->GetCodeOfSSQ();
    BROADCASTER_MSG_CG_STATUS_SSQ_INFO_BRD& rMsgCMD = pUpdater->GetMsgRouter()->GetBRD_CG();
    WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;

    switch(notice_type)
    {
    case eNotice_OpenCountdown:
        {
            MSGSUB_SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN msg;
            msg.CodeOfSSQ = CodeOfSSQ;
            msg.Countdown = (SSQInfo::MINUTE_T)arg0;
            msg.SerializeTo(pHeader);

            m_Notice_Steps = eNotice_Steps(m_Notice_Steps | notice_type);
            m_Argument0    = arg0;
        }
        break;
    case eNotice_Open:
        {
            MSGSUB_SSQ_PROTOCOL_NOTICE_ENABLE_ENTER msg;
            msg.CodeOfSSQ = CodeOfSSQ;
            msg.SerializeTo(pHeader);

            m_Notice_Steps = eNotice_Steps(m_Notice_Steps | notice_type);
            m_Argument0    = ULONG_MAX;
        }
        break;
    case eNotice_CloseCountdown:
        {
            MSGSUB_SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN msg;
            msg.CodeOfSSQ = CodeOfSSQ;
            msg.Countdown = (SSQInfo::MINUTE_T)arg0;
            msg.SerializeTo(pHeader);

            m_Notice_Steps = eNotice_Steps(m_Notice_Steps | notice_type);
            m_Argument0    = arg0;
        }
        break;
    case eNotice_ClosePortal:
        {
            MSGSUB_SSQ_PROTOCOL_NOTICE_CLOSE_PORTAL msg;
            msg.CodeOfSSQ = CodeOfSSQ;
            msg.SerializeTo(pHeader);

            m_Notice_Steps = eNotice_Steps(m_Notice_Steps | notice_type);
            m_Argument0    = ULONG_MAX;
        }
        break;
    case eNotice_Close:
        {
            MSGSUB_SSQ_PROTOCOL_NOTICE_CLOSE_ENTER msg;
            msg.CodeOfSSQ = CodeOfSSQ;
            msg.SerializeTo(pHeader);

            m_Notice_Steps = eNotice_Steps(m_Notice_Steps | notice_type);
            m_Argument0    = ULONG_MAX;
        }
        break;
    }
}

};    // end namespace SSQControlInfo

//==================================================================================================
//  <Internal Message Handler Implementation>
//


//==================================================================================================
//  <Packet Handler Implementation>
//

#define FINDnALIGN_USER(pUserInstance, UserKey) \
    User* pUserInstance = UserManager::Instance()->GetUser((UserKey));\
    if(!pUserInstance)                                                \
    {                                                                 \
        SUNLOG(eFULL_LOG, _T("[G2A] Can't Find User %u"), (UserKey)); \
        return;                                                       \
    }


//--------------------------------------------------------------------------------------------------


VOID
SSQControlManagerImpl::OnRecv_AG_RoomJoinAck(SSQUpdateCtrl* const pCtrl, User* const pUser,
                                             DungeonSSQ* const pDungeonSSQ)
{
    const DWORD playerKey = pUser->GetPlayerKey();
    SSQMsgRouterInAgent* const pRouter = pCtrl->GetMsgRouter();
    ISSQHandler* const pHandler = pRouter->FindSlot(playerKey);

    BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = pRouter->GetCMD_AG();
    WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
    {
        MSGSUB_SSQ_S2S_ZONE_SSQ_JOIN_NTF msg;
        msg.ObjectKey = pUser->GetPlayerKey();
        msg.ItemCode  = pHandler->GetUsedItemCode();
        msg.SerializeTo(pHeader);
    };
    pRouter->Update(pCtrl->GetCodeOfSSQ());

    MSG_CG_ZONE_EX_SSQ_JOIN_ACK msgACK;
    msgACK.SetSuccessResult(pDungeonSSQ->GetMapCode());
    pUser->SendPacket(&msgACK, sizeof(msgACK));
}


//--------------------------------------------------------------------------------------------------


RC::eROOM_RESULT
SSQControlManagerImpl::EnterenceControl(FlowControl& INOUT flow, User* const pUser,
                                        const EnterenceControlArg& arg)
{
    //////////////////////////////////////////////////////////////////////////
    //  (WAVERIX)(NOTE)(080217)
    //  -  허용 조건(in Client)
    //      -  1) in FIELD_SERVER
    //      -  2) 카오 상태 불가
    //      -  3) 입장 가능시간
    //      -  4) 해당 아이템 존재    <- GameServer
    //      -  5) 입장 가능 하임 보유 여부 <- GameServer
    //      -  6) 입장 가능 레벨
    //  -  허용 조건(in Server)
    //      -  1) 해당 필드 존재하는지? <- 어캐 생성할 까?
    //      -  2) 해당 필드 인원 제한등과 같은 제한 조건에 걸리지 않는지?
    //  (WAVERIX)(NOTE)(080217)
    //  -  CG_ZONE_EX_SSQ_JOIN_INFO_SYN에 의해 할당된 객체의 무결성 검증
    //  -  해당 필드에 대한 재 검증 <- 인원제한과 같은 동기화 문제로 해결되지 않을 수 있다.
    //  -  애초 60초라는 딜레이 안에서 무슨 일이 벌어질 지 감당이 안된다. 여러 가지 가능한 어뷰징을
    //      방지 할 수 있어야 한다.
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FLOWCONTROL_START()
    {
        IfFailThenBreak(!pUser->IsBeginTransaction(), RC::RC_ROOM_ALREADY_DOING_TRANSACTION);
        // in FIELD_SERVER
        ServerSessionEx* const pServerSession = pUser->GetLinkedServer();
        IfFailThenBreak(pServerSession && (pServerSession->GetServerType() == FIELD_SERVER),
                        RC::RC_ROOM_INVALIDROOMTYPE);

    #ifdef __CN_0_SSQ_CAHO_EXCEPTION
    #else
        IfFailThenBreak(!pUser->IsChaoState(), RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE);
    #endif
    
    #ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        IfFailThenBreak(
            BattleGroundManager::Instance()->IsValidReservationMember(pUser->GetUserKey()) == false,
            RC::RC_ROOM_BATTLE_GROUND_RESERVATION);
    #endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

        SSQInfoParser* const pSSQInfoParser = SSQInfoParser::Instance();
        const LEVELTYPE level = pUser->GetSelectedCharLV();
        const SSQInfo* const pSSQInfo = pSSQInfoParser->FindSSQInfoEnterableLevel(level);
        IfFailThenBreak(pSSQInfo, RC::RC_ROOM_NOT_EXIST_ROOM_BY_FITNESS_LEVEL);

        const SSQCODE CodeOfSSQ = pSSQInfo->m_MapCode;

        //  STEP #3
        SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ); // ASSERT(itFind != UpdateList.end());
        SSQ_UPDATER_NODE& rCtrlNode = *itFind;
        SSQUpdateCtrl* const pCtrl = rCtrlNode.GetUpdater(); // ASSERT(pCtrl);

        //  (WAVERIX)(080722)(NOTE)
        //  -  미묘한 상황이 존재한다. 아직 열리는 시간이 아닌 경우,
        //    게임서버가 죽어버린 경우, 등등...
        //  -  SSQUpdateCtrl의 상태를 평가해서 에러처리해야 할 것이다.
        DungeonSSQ* const pDungeon = pCtrl->GetSSQField();

        if(arg.IsJoinAction)
        {
            //  (WAVERIX)(080722)(NOTICE)
            //  check registered warp object
            //  설정된 정보와 일치하는지 여부, 이미 timeout된 경우도 있으므로 그에 관련한 정보도 포함한다.
            //  클라 요청에 의해 제거할 필요가 있었다만... 혹시 경우의 수를 따져보자.
            //IfFailThenBreak(IDofSSQ==arg.Inputed_CodeOfSSQ, RC::RC_ROOM_FAILED); // need new result code
            const BOOLEAN isEnabled = pCtrl->IsEnableEnter();
            const RC::eROOM_RESULT room_result
                = isEnabled ?   RC::RC_ROOM_SUCCESS
                : pDungeon  ?   RC::RC_ROOM_NOT_OPEN_TIME
                :               RC::RC_ROOM_NOT_EXIST_ROOM_BY_FITNESS_LEVEL;

            IfFailThenBreak(room_result == RC::RC_ROOM_SUCCESS, room_result);
        }
        else
        {
            //_NA_20110630_BATTLE_ZONE_RENEWAL

            const BOOLEAN isEnabled = pCtrl->IsEnablePotalCreate();
            const RC::eROOM_RESULT room_result = isEnabled ? RC::RC_ROOM_SUCCESS :
                                                             RC::RC_ROOM_NOT_OPEN_TIME;

            IfFailThenBreak(room_result == RC::RC_ROOM_SUCCESS, room_result);
        }

        SSQMsgRouterInAgent* const pRouter = pCtrl->GetMsgRouter();    // ASSERT(pRouter);
        //WAVERIX_SSQ_PORTAL_TRANSACTION_MODIFY
        const DWORD curcnt = pRouter->GetSlotNumber();
        //const DWORD curcnt = pDungeon->GetCurUserNum();
        const BOOLEAN bValidCnt = SAFE_NUMERIC_TYPECAST(DWORD, curcnt, SSQInfo::COUNT_T);
        IfFailThenBreak(bValidCnt && pSSQInfo->IsValidCountForEnter((SSQInfo::COUNT_T)curcnt),
                        RC::RC_ROOM_FULL_ROOM);

        //  (WAVERIX)(TEMPORARY REMOVED)
        //  <-  사용하려는 아이템에 관련된 처리 수행...
        //      pUser->SetLockItemPosForInstanceDungeon(posOfItem);
        //  <- error 발생시 해제시켜줘야 한다.
        //  <- 필드서버에서의 응답에서 해당 아이템의 부적절한 문제를 평가해야 한다. AG_SYN->AG_ACK에서 처리한다.
        //  <- posOfItem의 값이 JOIN_INFO_SYN과 JOIN_SYN이 달라질 경우도 감안해야 할 것이다.

        //----------------------------------------------
        //  register warp object with timeout(50sec)
        //  먼저 슬롯을 선점하는 경우는 문제가 있다. 필드 서버로부터의 응답 결과에 따라 등록할 필요가 있다.
        //  MSG_AG_ZONE_SSQ_JOIN_INFO_ACK에서 처리하도록 한다.
        const DWORD playerKey = pUser->GetPlayerKey();
        ISSQHandler* pHandler = pRouter->FindSlot(playerKey);
        {
            if(arg.IsJoinAction)
            {    //  JOIN_SYN
                IfFailThenBreak(pHandler, RC::RC_ROOM_ALREADY_NOT_READY_STATE);
                IfFailThenBreak(pHandler->IsWarpObjectAlived(), RC::RC_ROOM_ALREADY_NOT_READY_STATE);
                pHandler->ChangeStatus(ISSQHandler::State<ISSQHandler::USR_PORTAL_TRANSACTION>());
            }
            else
            {    //  JOIN_INFO_SYN
                IfFailThenBreak(pHandler == 0, RC::RC_ROOM_FAILED);

                //  pInfoNode의 최초 설정 - ACK이 도착할 때까지의 상태
                //  -  ACK:TRUE    next_state : USR_PORTAL_OBJECT_CREATED
                //  -  ACK:FALSE    next_state : ~ release
                pHandler = pRouter->AlignNewSlot(playerKey);
                pUser->SSQHandler = ISSQHandlerD(pUser, pHandler);
                pHandler->ChangeStatus(
                    ISSQHandler::State<ISSQHandler::USR_REGISTERED_SSQ_INDEXER>(
                        pUser, playerKey, pRouter, arg.Inputed_PosOfItem));
                //  handler 내부에서 아래와 같은 구문이 동작한다.
                //pUser->SetLockItemPosForInstanceDungeon(arg.Inputed_PosOfItem);
            }
        };
        //
        //----------------------------------------------

        if(arg.IsJoinAction)
        {    // JOIN_SYN
            //  다시 생각하자... Field Server에서 먼저 검증이 이뤄지고, 그 다음 BattleServer로 이전된다.
            //  바로 Linked Server로 가면 플레이어는 존재하지 않는다.
            {    // SUCCESS Control
                //  Send To BattleServer (in SSQField)
                ServerSessionEx* const pLinkedServer =
                    static_cast<ServerSessionEx*>(pDungeon->GetLinkedServer());
                IfFailThenBreak(pLinkedServer, RC::RC_ROOM_NOT_EXIST_ROOM);

                MSG_AG_ZONE_SSQ_JOIN_SYN msgSYN;
                msgSYN.m_ItemPos = arg.Inputed_PosOfItem;
                msgSYN.m_CodeOfSSQ = rCtrlNode.CodeOfSSQ;

                msgSYN.m_dwKey = pUser->GetUserKey();
                pUser->SendToLinkedServer(&msgSYN, sizeof(msgSYN));
                //pLinkedServer->SendPacket(&msgSYN, sizeof(msgSYN));
            };
        }
        else
        {    // JOIN_INFO_SYN
            //  다시 생각하자... Field Server에서 먼저 검증이 이뤄지고, 그 다음 BattleServer로 이전된다.
            //  바로 Linked Server로 가면 플레이어는 존재하지 않는다.
            {    // SUCCESS Control
                //ServerSessionEx* const pLinkedServer = static_cast<ServerSessionEx*>(pDungeon->GetLinkedServer());
                //IfFailThenBreak(pLinkedServer, RC::RC_ROOM_NOT_EXIST_ROOM);

                MSG_AG_ZONE_SSQ_JOIN_INFO_SYN msgSYN;
                msgSYN.m_ItemPos = arg.Inputed_PosOfItem;
                msgSYN.m_CodeOfSSQ = CodeOfSSQ;
                pUser->SendToLinkedServer(&msgSYN, sizeof(msgSYN));
            };
        }

        return RC::RC_ROOM_SUCCESS;
    }
    FLOWCONTROL_END;

    const POSTYPE lockedItemPos = pUser->GetLockItemPosForInstanceDungeon();
    if(arg.IsJoinAction)
    {    // JOIN_SYN
        ISSQHandlerD& rHandler = pUser->SSQHandler;
        if(rHandler.IsAvailable())
            rHandler->ChangeStatus(ISSQHandler::State<ISSQHandler::USR_PORTAL_FAILED>());
    }
    else
    {    // JOIN_INFO_SYN
        if(lockedItemPos == arg.Inputed_PosOfItem)
            pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);
    }

    return (RC::eROOM_RESULT)flow.GetErrorCode();
}


//--------------------------------------------------------------------------------------------------


SSQ_PACKET_HANDLER_CG_DECL(CG_ZONE_EX,        CG_ZONE_EX_SSQ_JOIN_INFO_SYN)
{
    __TOUCHED((pRecvMsg, wSize, pUser));
    BYTE errorGroup = MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK::RET_ROOM;

    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        EnterenceControlArg arg(FALSE, 0, pRecvMsg->m_ItemPos);
        const RC::eROOM_RESULT check_result = EnterenceControl(flow, pUser, arg);
        if(RC::RC_ROOM_SUCCESS != check_result)
            break;

        return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK msgFAIL;
    msgFAIL.SetFailResult(errorGroup, flow.GetErrorCode());
    pUser->SendPacket(&msgFAIL, msgFAIL.GetSize());

    // for Test
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] ErrorCode %2u - Str : {%s}",
           flow.GetErrorCode(), flow.GetErrorString());
    WAVERIX_DEBUG_CODE(
        ;
        //if(pUser)
        //{    // GM Notice
        //  MSG_CG_ETC_DEBUGINFO_CMD msgCMD;
        //  INT iLen = _snprintf(
        //      msgCMD.m_pszChatMsg, _countof(msgCMD.m_pszChatMsg)
        //      , "실패 발생 : 이 메시지는 디버깅용 입니다. 클라 메시지 처리 완료되면 없어질 거와요. errstring [%s]"
        //      , flow.GetErrorString()
        //      );
        //  msgCMD.m_byMsgLength = (BYTE)iLen;
        //  pUser->SendPacket(&msgCMD, msgCMD.GetSize());
        //}
    );
    return /*RETURN FALSE VALUE*/;
}


//--------------------------------------------------------------------------------------------------


SSQ_PACKET_HANDLER_CG_DECL(CG_ZONE_EX,        CG_ZONE_EX_SSQ_JOIN_SYN)
{
    __TOUCHED((pRecvMsg, wSize, pUser));
    BYTE errorGroup = MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK::RET_ROOM;

    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        EnterenceControlArg arg(TRUE,
                                SSQInfo::SSQCODE(pRecvMsg->m_CodeOfSSQ),
                                INVALID_POSTYPE_VALUE);    // 클라 요청에 의해 사용되지 않는다.
        const RC::eROOM_RESULT check_result = EnterenceControl(flow, pUser, arg);
        if(RC::RC_ROOM_SUCCESS != check_result)
            break;

        return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    MSG_CG_ZONE_EX_SSQ_JOIN_ACK msgFAIL;
    msgFAIL.SetFailResult(errorGroup, flow.GetErrorCode());
    pUser->SendPacket(&msgFAIL, msgFAIL.GetSize());

    // for Test
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] ErrorCode %2u - Str : {%s}",
           flow.GetErrorCode(), flow.GetErrorString());
    WAVERIX_DEBUG_CODE(
        ;
        //if(pUser)
        //{    // GM Notice
        //  MSG_CG_ETC_DEBUGINFO_CMD msgCMD;
        //  INT iLen = _snprintf(
        //      msgCMD.m_pszChatMsg, _countof(msgCMD.m_pszChatMsg)
        //      , "실패 발생 : 이 메시지는 디버깅용 입니다. 클라 메시지 처리 완료되면 없어질 거와요. errstring [%s]"
        //      , flow.GetErrorString()
        //      );
        //  msgCMD.m_byMsgLength = (BYTE)iLen;
        //  pUser->SendPacket(&msgCMD, msgCMD.GetSize());
        //}
    );

    return /*RETURN FALSE VALUE*/;
}


//--------------------------------------------------------------------------------------------------


SSQ_PACKET_HANDLER_CG_DECL(CG_ZONE_EX,        CG_ZONE_EX_SSQ_ROOM_INFO_SYN)
{
    __TOUCHED((pRecvMsg, wSize, pUser));
    //  Special C->G SYN...
    WAVERIX_DEBUGGING_OUTPUTn(0);
}


//--------------------------------------------------------------------------------------------------


SSQ_PACKET_HANDLER_CG_DECL(CG_ZONE_EX,        CG_ZONE_EX_SSQ_OBSERVER_SYN)
{
    __TOUCHED((pRecvMsg, wSize, pUser));
    const DWORD ERRCODE_INVALID_FLOWSEQ = 10000;
    using namespace nsSSQ::Observer;
    typedef nsSSQ::Observer::Info    Info;
    const DWORD playerKey = pUser->GetPlayerKey();

    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak(!pUser->IsBeginTransaction(), RC::RC_ROOM_ALREADY_DOING_TRANSACTION);

        const Info::eRequest request = (Info::eRequest)pRecvMsg->Request;
        const Rules::eC2G_ReqRule rules = (Rules::eC2G_ReqRule)Rules::GetC2G_RequestRule(request);

        IfFailThenBreak((rules & Rules::eC2G_ReqRule_BlockRequest) == 0, ERRCODE_INVALID_FLOWSEQ);

        const eZONE_STATE zone_state = pUser->GetZoneState();
        const eZONETYPE zone_type = pUser->GetZoneType();

        IfFailThenBreak((zone_state & ePRS_AT_ZONE) == ePRS_AT_ZONE,
                        RC::RC_ROOM_ALREADY_DOING_TRANSACTION);

        //  일단 관전모드를 위해서라면 타락한 사원 안이라는 조건이 필요하겠지만...
        //  기타 다목적 용도로 사용할 수 있음을 염두해 두자.

        if(rules&Rules::eC2G_ReqRule_CondInFieldServer)
        {
            // in FIELD_SERVER
            ServerSessionEx* const pServerSession = pUser->GetLinkedServer();
            IfFailThenBreak(pServerSession && (pServerSession->GetServerType() == FIELD_SERVER),
                            RC::RC_ROOM_INVALIDROOMTYPE);

            SetFailFc(!"Curversion not supported", RC::RC_ROOM_INVALIDROOMTYPE);
            break;
        }

        const KEYTYPE zone_key = pUser->GetZoneKey();
        if(rules&Rules::eC2G_ReqRule_CondInSSQField)
        {
            InstanceDungeon* const pIndun = (InstanceDungeon*)g_ZoneManager.FindZone(zone_key);
            IfFailThenBreak(pIndun, RC::RC_ROOM_NOTEXISTROOM);
            IfFailThenBreak(pIndun->GetDungeonType() == INDUN_KIND_SS_QUEST,
                            RC::RC_ROOM_INVALID_ROOM_TYPE);

            const SSQCODE CodeOfSSQ = pIndun->GetMapCode();
            SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);    // ASSERT(itFind != UpdateList.end());
            IfFailThenBreak(itFind != UpdateList.end(), RC::RC_ROOM_INVALIDROOMTYPE);

            SSQ_UPDATER_NODE& rCtrlNode = *itFind;
            SSQUpdateCtrl* const pCtrl = rCtrlNode.GetUpdater();    // ASSERT(pCtrl);
            SSQMsgRouterInAgent* const pRouter = pCtrl->GetMsgRouter();
            ISSQHandler* const pHandler = pRouter->FindSlot(playerKey);
            IfFailThenBreak(pHandler, RC::RC_ROOM_INVALID_PLAYER_STATE);

            BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = pRouter->GetCMD_AG();
            WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
            {
                MSGSUB_SSQ_S2S_OBSERVER_MODE_CTRL msg;
                msg.ObjectKey = pUser->GetPlayerKey();
                msg.Request = request;
                msg.ObserverCmd.PressedKey = (Info::eKeyCmd)pRecvMsg->ObserverCmd.PressedKey;
                msg.SerializeTo(pHeader);
            };
        }

        return;
    }
    FLOWCONTROL_END;

    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Invalid Status %u", flow.GetErrorCode());
}

//--------------------------------------------------------------------------------------------------


SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE,        AG_ZONE_SSQ_JOIN_INFO_ACK)
{
    __TOUCHED((pRecvMsg, wSize, pSession));
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);
    BYTE errorGroup = pRecvMsg->m_Result;

    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak(MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::SUCCESS == pRecvMsg->m_Result,
                        RC_ROOM_ACK_FAIL_MESSAGE);

        errorGroup = MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::RET_ROOM;
        IfFailThenBreak(!pUser->IsBeginTransaction(), RC::RC_ROOM_ALREADY_DOING_TRANSACTION);

        //  (NOTE)
        //  -  player를 위해 60 초 간의 warp허용 공간이 존재해야 한다.
        //  -  player가 여기에 입장하는 순간 MSG_CG_ZONE_EX_SSQ_JOIN_SYN으로 입장 요청하게 된다.
        //  (WARNING)
        //  -  여기에는 너무 변수가 많다. 입장 순간에 아이템에 대해서 재 처리를 수행하기로 하고,
        //  -  일단 여기서 LOCK을 풀기로 하자. 아니면 애초 LOCK을 걸지 말자.
        //  (WAVERIX)(TEMPORARY REMOVED)
        //      pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);

        //  유사 군집.... 일단 copy & paste
        const SSQCODE CodeOfSSQ = pRecvMsg->RETINFO_SUCCESS.m_CodeOfSSQ;

        //  check registered warp object
        //  설정된 정보와 일치하는지 여부, 이미 timeout된 경우도 있으므로 그에 관련한 정보도 포함한다.

        SSQInfoParser* const pSSQInfoParser = SSQInfoParser::Instance();
        const SSQInfo* const pSSQInfo = pSSQInfoParser->FindSSQInfo(CodeOfSSQ);

        errorGroup = MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::RET_ROOM;
        IfFailThenBreak(pSSQInfo, RC::RC_ROOM_NOT_EXIST_ROOM);

        //const SSQInfo::INDEX_T IDofSSQ = pSSQInfo->m_Index;
        SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);
        //<- 없을 수 없다...
        //IfFailThenBreak(itFind != UpdateList.end(), RC::RC_ROOM_INVALIDROOMTYPE);

        SSQ_UPDATER_NODE& rCtrlNode = *itFind;
        SSQUpdateCtrl& rCtrl = *rCtrlNode.GetUpdater();

        IfFailThenBreak(rCtrl.IsEnableEnter(), RC::RC_ROOM_NOT_OPEN_TIME);

        DungeonSSQ* const pDungeon = rCtrl.GetSSQField();
        errorGroup = MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::RET_ROOM;
        IfFailThenBreak(pDungeon, RC::RC_ROOM_NOT_EXIST_ROOM_BY_FITNESS_LEVEL);

        SSQMsgRouterInAgent* const pRouter = rCtrl.GetMsgRouter();
        //WAVERIX_SSQ_PORTAL_TRANSACTION_MODIFY
        const DWORD curcnt = pRouter->GetSlotNumber();
        //const DWORD curcnt = pDungeon->GetCurUserNum();

        const BOOL bValidCnt = SAFE_NUMERIC_TYPECAST(DWORD, curcnt, SSQInfo::COUNT_T);
        errorGroup = MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::RET_ROOM;
        IfFailThenBreak(bValidCnt && pSSQInfo->IsValidCountForEnter((SSQInfo::COUNT_T)curcnt),
                        RC::RC_ROOM_FULL_ROOM);

        //----------------------------------------------
        //  register warp object with timeout(50sec)
        ISSQHandler* const pHandler = pRouter->FindSlot(pUser->GetPlayerKey());
        errorGroup = MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::RET_ROOM;
        IfFailThenBreak(pHandler && pHandler->IsPreparedReqTransaction(),
                        RC::RC_ROOM_ALREADY_NOT_READY_STATE);

        pHandler->ChangeStatus(ISSQHandler::State<ISSQHandler::USR_PORTAL_OBJECT_CREATED>());
        pHandler->SetUsedItemCode(pRecvMsg->m_ItemCode);
        //
        //----------------------------------------------

        MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK msgACK;
        msgACK.SetSuccessResult(CodeOfSSQ);
        pUser->SendPacket(&msgACK, msgACK.GetSize());

        return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    // for Test
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] ErrorCode %2u - Str : {%s}",
           flow.GetErrorCode(), flow.GetErrorString());

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    MSG_CG_ZONE_EX_SSQ_JOIN_INFO_ACK msgFAIL;
    switch(const DWORD errCode = flow.GetErrorCode())
    {
    case RC_ROOM_ACK_FAIL_MESSAGE:// Item인 경우 Lock이 걸려서 실패할 수 있다. 하지만 여기서 해제할 수는 없다...
        msgFAIL.SetFailResult(pRecvMsg->RETINFO_FAIL.m_Kind, pRecvMsg->RETINFO_FAIL.m_ErrorCode);
        break;
    default:
        msgFAIL.SetFailResult(errorGroup, errCode);
        break;
    }
    pUser->SendPacket(&msgFAIL, msgFAIL.GetSize());

    return /*RETURN FALSE VALUE*/;
}


//--------------------------------------------------------------------------------------------------


SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE,        AG_ZONE_SSQ_JOIN_ACK)
{
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);
    BYTE errorGroup = pRecvMsg->RET_ROOM;
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        //  AG_ZONE_SSQ_JOIN_SYN --> (BattleServer)
        //  AG_ZONE_SSQ_JOIN_ACK <-- (BattleServer)
        //  AG_ZONE_SSQ_JOIN_SYN --> (FieldServer)
        //  AG_ZONE_SSQ_JOIN_ACK <-- (FieldServer)

        pSession->GetServerType() == BATTLE_SERVER
            ? OnRecv_AG_ZONE_SSQ_JOIN_ACK_ByBattle(pRecvMsg, wSize, pSession, flow, errorGroup)
            : OnRecv_AG_ZONE_SSQ_JOIN_ACK_ByField(pRecvMsg, wSize, pSession, flow, errorGroup);

        if(flow.IsErrorOccured())
            break;

        return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    MSG_CG_ZONE_EX_SSQ_JOIN_ACK msgACK;
    msgACK.SetFailResult(errorGroup, flow.GetErrorCode());
    pUser->SendPacket(&msgACK, sizeof(msgACK));

    return /*RETURN FALSE VALUE*/;
}


//--------------------------------------------------------------------------------------------------


VOID
SSQControlManagerImpl::OnRecv_AG_ZONE_SSQ_JOIN_ACK_ByField(const MSG_AG_ZONE_SSQ_JOIN_ACK* pRecvMsg,
                                                           const WORD wSize,
                                                           GameServerSession* const pSession,
                                                           FlowControl& flow, BYTE& errorGroup)
{
    __TOUCHED((pRecvMsg, wSize, pSession));
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);

    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FLOWCONTROL_START()
    {
        if(pRecvMsg->m_Result != MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::SUCCESS)
        {
            errorGroup = pRecvMsg->RETINFO_FAIL.m_Kind;
            SetFailFc("fail occurred from gameserver", pRecvMsg->RETINFO_FAIL.m_ErrorCode);
            break;
        }
        //IfFailThenBreak(MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::SUCCESS == pRecvMsg->m_Result,
        //                RC_ROOM_ACK_FAIL_MESSAGE);

        errorGroup = MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::RET_ROOM;
        IfFailThenBreak(!pUser->IsBeginTransaction(), RC::RC_ROOM_ALREADY_DOING_TRANSACTION);

        errorGroup = MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::RET_ROOM;
        ZoneInterface* pFromZone = g_ZoneManager.FindZone(pUser->GetZoneKey());
        IfFailThenBreak(pFromZone, RC::RC_ROOM_NOT_EXIST_ROOM);

        const SSQCODE CodeOfSSQ = pRecvMsg->RETINFO_SUCCESS.m_CodeOfSSQ;
        //SSQInfoParser* const pSSQInfoParser = SSQInfoParser::Instance();
        //const SSQInfo* const pSSQInfo = pSSQInfoParser->FindSSQInfo(IDofSSQ);
        //IfFailThenBreak(pSSQInfo, RC::RC_ROOM_NOT_EXIST_ROOM);
        SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);
        IfFailThenBreak(itFind != UpdateList.end(), RC::RC_ROOM_NOT_EXIST_ROOM);

        SSQInfoParser* const pSSQInfoParser = SSQInfoParser::Instance();
        const SSQInfo* const pSSQInfo = pSSQInfoParser->FindSSQInfo(CodeOfSSQ);

        IfFailThenBreak(pSSQInfo, RC::RC_ROOM_NOT_EXIST_ROOM);

        SSQ_UPDATER_NODE& rCtrlNode = *itFind;
        SSQUpdateCtrl& rCtrl = *rCtrlNode.GetUpdater();

        DungeonSSQ* const pDungeon = rCtrl.GetSSQField();
        const BOOLEAN isEnabled = rCtrl.IsEnableEnter();
        const RC::eROOM_RESULT room_result
            = isEnabled ? RC::RC_ROOM_SUCCESS
            : pDungeon  ? RC::RC_ROOM_NOT_OPEN_TIME
            :             RC::RC_ROOM_NOT_EXIST_ROOM_BY_FITNESS_LEVEL;

        IfFailThenBreak(room_result == RC::RC_ROOM_SUCCESS, room_result);

        SSQMsgRouterInAgent* const pRouter = rCtrl.GetMsgRouter();
        //WAVERIX_SSQ_PORTAL_TRANSACTION_MODIFY
        const DWORD curcnt = pRouter->GetSlotNumber();
        //const DWORD curcnt = pDungeon->GetCurUserNum();

        const BOOL bValidCnt = SAFE_NUMERIC_TYPECAST(DWORD, curcnt, SSQInfo::COUNT_T);
        IfFailThenBreak(bValidCnt && pSSQInfo->IsValidCountForEnter((SSQInfo::COUNT_T)curcnt),
                        RC::RC_ROOM_FULL_ROOM);

        //----------------------------------------------
        //  register warp object with timeout(50sec)
        ISSQHandler* pSSQHandler = pRouter->FindSlot(pUser->GetPlayerKey());
        IfFailThenBreak(pSSQHandler && pSSQHandler->IsWarpTransaction(),
                        RC::RC_ROOM_ALREADY_NOT_READY_STATE);

        pSSQHandler->ChangeStatus(ISSQHandler::State<ISSQHandler::USR_ENTERING_SSQ_FIELD>());
        //
        //----------------------------------------------

        //  다시 생각하자... Field Server에서 먼저 검증이 이뤄지고, 그 다음 BattleServer로 이전된다.
        //  바로 Linked Server로 가면 플레이어는 존재하지 않는다.
        {    // SUCCESS Control
            //  Send To BattleServer (in SSQField)
            ServerSessionEx* const pLinkedServer =
                static_cast<ServerSessionEx*>(pDungeon->GetLinkedServer());
            IfFailThenBreak(pLinkedServer, RC::RC_ROOM_NOT_EXIST_ROOM);
            //  (WAVERIX)(TEMPORARY REMOVED)
            //          pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);
            AgentPartyManager::Instance()->LeaveParty(pUser, eREASON_LEAVE_PARTY_INTEND);

            RoomMigrater<ZoneInterface,
                         DungeonSSQ, TR_LEAVE_FIELD_FOR_SSQFIELD> op(pFromZone, pDungeon);
            op(pUser);
        };


        return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    ISSQHandlerD& rHandler = pUser->SSQHandler;
    if(rHandler.IsAvailable())
        rHandler->ChangeStatus(ISSQHandler::State<ISSQHandler::USR_PORTAL_FAILED>());
}


//--------------------------------------------------------------------------------------------------


VOID
SSQControlManagerImpl::OnRecv_AG_ZONE_SSQ_JOIN_ACK_ByBattle(const MSG_AG_ZONE_SSQ_JOIN_ACK* pRecvMsg,
                                                            const WORD wSize,
                                                            GameServerSession* const pSession,
                                                            FlowControl& flow, BYTE& errorGroup)
{
    __TOUCHED((pRecvMsg, wSize, pSession));
    FINDnALIGN_USER(pUser, pRecvMsg->m_dwKey);
    //  1st AG_ZONE_SSQ_JOIN_SYN에 대한 응답,
    FLOWCONTROL_START()
    {
        IfFailThenBreak(MSG_AG_ZONE_SSQ_JOIN_INFO_ACK::SUCCESS == pRecvMsg->m_Result,
                        RC_ROOM_ACK_FAIL_MESSAGE);

        //const SSQInfo::INDEX_T IDofSSQ = pSSQInfo->m_Index;
        const SSQCODE CodeOfSSQ = pRecvMsg->RETINFO_SUCCESS.m_CodeOfSSQ;
        SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);
        IfFailThenBreak(itFind != UpdateList.end(), RC::RC_ROOM_NOT_EXIST_ROOM);

        SSQ_UPDATER_NODE& rCtrlNode = *itFind;
        SSQUpdateCtrl& rCtrl = *rCtrlNode.GetUpdater();

        IfFailThenBreak(rCtrl.IsEnableEnter(), RC::RC_ROOM_NOT_OPEN_TIME);

        DungeonSSQ* const pDungeon = rCtrl.GetSSQField();
        IfFailThenBreak(pDungeon, RC::RC_ROOM_NOT_EXIST_ROOM_BY_FITNESS_LEVEL);

        const SSQInfo* const pSSQInfo = rCtrl.GetSSQInfo();

        SSQMsgRouterInAgent* const pRouter = rCtrl.GetMsgRouter();
        //WAVERIX_SSQ_PORTAL_TRANSACTION_MODIFY
        const DWORD curcnt = pRouter->GetSlotNumber();
        //const DWORD curcnt = pDungeon->GetCurUserNum();

        IfFailThenBreak(pSSQInfo->IsValidCountForEnter((SSQInfo::COUNT_T)curcnt),
                        RC::RC_ROOM_FULL_ROOM);

        //  이 값의 사용은 Warp Object 사용할 것.
        const POSTYPE posOfItem = INVALID_POSTYPE_VALUE; // pRecvMsg->m_ItemPos;
        //  (WAVERIX)(TEMPORARY REMOVED)
        //      pUser->SetLockItemPosForInstanceDungeon(posOfItem);

        //  다시 생각하자... Field Server에서 먼저 검증이 이뤄지고, 그 다음 BattleServer로 이전된다.
        //  바로 Linked Server로 가면 플레이어는 존재하지 않는다.
        {    // SUCCESS Control

            //MSG_CG_ZONE_EX_SSQ_JOIN_ACK msgACK;
            //msgACK.SetSuccessResult((MAPCODE)IDofSSQ);
            //pUser->SendToLinkedServer(&msgACK, sizeof(msgACK));

            // Send to FieldServer
            MSG_AG_ZONE_SSQ_JOIN_SYN msgSYN;
            msgSYN.m_ItemPos    = posOfItem;
            msgSYN.m_CodeOfSSQ    = rCtrlNode.CodeOfSSQ;
            pUser->SendToLinkedServer(&msgSYN, sizeof(msgSYN));
        };

        return;
    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    switch(flow.GetErrorCode())
    {
    case RC_ROOM_ACK_FAIL_MESSAGE:
        {
            MSG_CG_ZONE_EX_SSQ_JOIN_ACK msgFAIL;
            msgFAIL.SetFailResult(pRecvMsg->RETINFO_FAIL.m_Kind, pRecvMsg->RETINFO_FAIL.m_ErrorCode);
            pUser->SendPacket(&msgFAIL, msgFAIL.GetSize());
        }
        return;
    default:
        break;
    }

    return;
}


//--------------------------------------------------------------------------------------------------


SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE,        AG_ZONE_SSQ_ROOM_CTRL_CMD)
{
    __TOUCHED((pRecvMsg, wSize, pSession));
    //  소멸될 때는 키를 해제해야 한다. 해당 포인터를 할당할까? 음...
    const SSQCODE CodeOfSSQ = SSQInfo::SSQCODE(pRecvMsg->m_dwKey);
    SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);
    if(itFind == UpdateList.end())
        return;

    SSQ_UPDATER_NODE& rNode = *itFind;
    const KEYTYPE KEYofSSQ = rNode.GetKey();
    if(!KEYofSSQ)
        return;

    rNode.pUpdater->RecvedMsgProcess(pRecvMsg->m_Header);
}


//--------------------------------------------------------------------------------------------------


SSQ_PACKET_HANDLER_AG_DECL(AG_ZONE,        AG_ZONE_SSQ_ROOM_INFO_BRD)
{
    //  Special Command
    const SSQCODE CodeOfSSQ = SSQInfo::SSQCODE(pRecvMsg->m_dwKey);
    SSQ_UPDATERS_IT itFind = _FindCodeOfSSQ(CodeOfSSQ);
    if(itFind == UpdateList.end())
        return;

    SSQ_UPDATER_NODE& rNode = *itFind;
    rNode.pUpdater->RecvedMsgProcessSpecial(pRecvMsg->m_Header);
}


//--------------------------------------------------------------------------------------------------


SSQ_PACKET_HANDLER_AW_DECL(AW_OPERATION,    AW_OPERATION_ROOM_CTRL_CMD)
{
    __TOUCHED((pRecvMsg, wSize, pSession));
    WAVERIX_DEBUGGING_OUTPUTn(0);
}

