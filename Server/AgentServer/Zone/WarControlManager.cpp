#include "stdafx.h"
#include <PacketStruct_AG.h>
#include <PacketStruct_AW.h>

#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>

#include <ServerSessions/BattleServerSession.h>
#include <ServerSessions/FieldServerSession.h>

#include <Zone/DungeonSSQ.h>
#include <Zone/ZoneManager.h>
#include <Party/PartyManager.h>

#include "WarControlManager.h"
#include "./SSQ/ISSQHandler.h"

// for debugging control
#undef WAVERIX_DEBUGGING_OUTPUTn
#undef WAVERIX_DEBUGGING_OUTPUTnV

#define WAVERIX_DEBUGGING_OUTPUTn(msg)    \
    SUNLOG(eCRITICAL_LOG, "[%s][%u][%u] %s\n", __FUNCTION__, __LINE__,    \
    (DWORD)((GetTickCount()-tickvalue)/(1000)), msg);                    \
    tickvalue = GetTickCount()

#define WAVERIX_DEBUGGING_OUTPUTnV(value)    \
    { CHAR _message[10];                    \
    WAVERIX_DEBUGGING_OUTPUTn(itoa(value, _message, sizeof(_message))); }

static DWORD tickvalue = GetTickCount();


#define __WAVERIX_LOG_FORMAT_0S(msg)    \
{    SUNLOG(eCRITICAL_LOG, "[Func:%s][Line:%3u] %s", __FUNCTION__, __LINE__, msg);    }

#define __WAVERIX_LOG_FORMAT_0S1N(msg, value)    \
{    SUNLOG(eCRITICAL_LOG, "[Func:%s][Line:%3u] %s (%u)", __FUNCTION__, __LINE__, msg, value);    }


#ifdef _DEBUG
#define WAVERIX_DEBUG_LOG_0S(msg)           __WAVERIX_LOG_FORMAT_0S(msg)
#define WAVERIX_DEBUG_LOG_0S1N(msg, value)  __WAVERIX_LOG_FORMAT_0S1N(msg, value)
#else
#define WAVERIX_DEBUG_LOG_0S(msg)
#define WAVERIX_DEBUG_LOG_0S1N(msg, value)
#endif

#include "./SSQ/SSQControlInfos.hxx"
#include "./SSQ/SSQUpdateCtrl.hxx"
#include "./SSQ/SSQPacketProcess.hxx"
#include "./SSQ/SSQControlManagerImpl.hxx"


//==================================================================================================
//  <DEFINITION #2>
//

#define MGR_SSQIMPL            ((SSQControlManagerImpl&)*m_pSSQImpl)
#define MGR_SSQIMPLS(this_)    ((SSQControlManagerImpl&)*((this_)->m_pSSQImpl))

//==================================================================================================
//  <WarControlManager>
WORK_KEY
WarControlManager::ms_WorkKey   = 0;

WarControlManager*
WarControlManager::ms_pWarControlManager = NULL;


WarControlManager::WarControlManager()
{
    ms_pWarControlManager = this;
    m_pSSQImpl = new SSQControlManagerImpl();
}

WarControlManager::~WarControlManager()
{
    SAFE_DELETE(m_pSSQImpl);
    ms_pWarControlManager = NULL;
}

//--------------------------------------------------------------------------------------------------
//  <IWorkNode Interface>
BOOL
WarControlManager::DoInit()
{
    MGR_SSQIMPL.ScheduleUpdateTimer.SetTimer(INTERVALofUPDATE);
    MGR_SSQIMPL.MessageUpdateTimer.SetTimer(INTERVALofUPDATE);
    //__NA001189_081015_SSQ_TICKET_INFO_CTRL__
    sTIME_INFO timeInfo;
    sTIME_INFO::SetTimeValue(timeInfo);

    const DWORD SSQTicketTimeInterval = 1*1000; //1sec
    MGR_SSQIMPL.SSQTicketTimer.SetTimer(SSQTicketTimeInterval);
    MGR_SSQIMPL.SSQTicketTimeInfoCtrl.Init();
    MGR_SSQIMPL.SSQTicketTimeInfoCtrl.Update(&timeInfo.SysTime);
    const SYSTEMTIME& rCountdown = MGR_SSQIMPL.SSQTicketTimeInfoCtrl.GetTicketTimeInfo();
    __TOUCHED(rCountdown); // for debug
    const nsSSQ::SSQTicketInfo& rInfo = MGR_SSQIMPL.SSQTicketTimeInfoCtrl.GetTicketInfo();
    MGR_SSQIMPL.SSQTicketTimeInfoCtrl.QueryResult(rInfo);

    MGR_SSQIMPL._InitializeUpdateCtrl();
    return TRUE;
}

BOOL
WarControlManager::DoProcess()
{
    sTIME_INFO* pTimeInfo = 0;
    sTIME_INFO timeInfo;

    ITimerBase& rTimeScheduler = MGR_SSQIMPL.ScheduleUpdateTimer;
    if(rTimeScheduler.IsExpired())
    {
        sTIME_INFO::SetTimeValue(timeInfo);
        pTimeInfo = &timeInfo;

        SSQ_UPDATERS& rList = MGR_SSQIMPL.UpdateList;
        FOREACH_CONTAINER(SSQ_UPDATER_NODE& rCtrlNode, rList, SSQ_UPDATERS)
        {
            rCtrlNode.pUpdater->UpdateMsg(timeInfo);
            //  (WAVERIX) (WARNING)
            //  -    ->Update(~)�� ȣ���� ������ pCtrl�� ���Ἲ�� ���� �� �ִ�.
            //      (���ο��� pCtrl ��ü�� �����ȴ�, ���� ���¸� �غ����ְ�...)
            //  -    ���� ->Update(~) ���Ŀ� pCtrl�� ���� ��� ���� �ڵ嵵 �־�� �ȵȴ�.
            rCtrlNode.pUpdater->Update(timeInfo);
            // must be do not use 'pCtrl'
        }
    }

    ITimerBase& rSSQTicketUpdate = MGR_SSQIMPL.SSQTicketTimer;
    if(rSSQTicketUpdate.IsExpired())
    {
        if(!pTimeInfo)
            sTIME_INFO::SetTimeValue(timeInfo);

        MGR_SSQIMPL.SSQTicketTimeInfoCtrl.Update(&timeInfo.SysTime);
        MGR_SSQIMPL.SSQSingletonNoticeCtrl.Update(timeInfo);
    }

    return TRUE;
}

BOOL
WarControlManager::IsComplete()
{
    return FALSE;
}

BOOL
WarControlManager::DoRelease()
{
    MGR_SSQIMPL._ReleaseUpdateCtrl();
    return TRUE;
}

// Control Interface
VOID
WarControlManager::OnMsg(sSIG_MSG* pSIG_MSG)
{
    __UNUSED(pSIG_MSG);
}


//--------------------------------------------------------------------------------------------------
//  <INTERFACEs>
VOID
WarControlManager::Update()
{
}


#define SSQ_PACKET_HANDLER_CALL(c, p)    \
    case MAKEWORD(c, p):                    \
        MGR_SSQIMPL.OnRecv_##p(static_cast<const MSG_##p*>(pMsg), wSize, pSession.m_pSession);    \
        break

VOID
WarControlManager::OnRecv(const MSG_BASE* const pMsg, const WORD wSize,
                          const SSQSessionCmd::FromClient& pSession)
{
    const WORD protocol = MAKEWORD(pMsg->m_byCategory, pMsg->m_byProtocol);
    switch(protocol)
    {
    SSQ_PACKET_HANDLER_CALL(CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_INFO_SYN);
    SSQ_PACKET_HANDLER_CALL(CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_SYN);
    SSQ_PACKET_HANDLER_CALL(CG_ZONE_EX, CG_ZONE_EX_SSQ_ROOM_INFO_SYN);
    SSQ_PACKET_HANDLER_CALL(CG_ZONE_EX, CG_ZONE_EX_SSQ_OBSERVER_SYN);
    default:
        FlowControl::FCAssert(!"Unexpected Status");
    }

    return;
}

VOID
WarControlManager::OnRecv(const MSG_BASE* const pMsg, const WORD wSize,
                          const SSQSessionCmd::FromGame& pSession)
{
    const WORD protocol = MAKEWORD(pMsg->m_byCategory, pMsg->m_byProtocol);
    switch(protocol)
    {
    SSQ_PACKET_HANDLER_CALL(AG_ZONE,    AG_ZONE_SSQ_JOIN_INFO_ACK);
    SSQ_PACKET_HANDLER_CALL(AG_ZONE,    AG_ZONE_SSQ_JOIN_ACK);
    SSQ_PACKET_HANDLER_CALL(AG_ZONE,    AG_ZONE_SSQ_ROOM_CTRL_CMD);
    SSQ_PACKET_HANDLER_CALL(AG_ZONE,    AG_ZONE_SSQ_ROOM_INFO_BRD);
    default:
        FlowControl::FCAssert(!"Unexpected Status");
    }

    return;
}

VOID
WarControlManager::OnRecv(const MSG_BASE* const pMsg, const WORD wSize, const SSQSessionCmd::FromWorld& pSession)
{
    const WORD protocol = MAKEWORD(pMsg->m_byCategory, pMsg->m_byProtocol);
    switch(protocol)
    {
    SSQ_PACKET_HANDLER_CALL(AW_OPERATION, AW_OPERATION_ROOM_CTRL_CMD);
    default:
        FlowControl::FCAssert(!"Unexpected Status");
    }

    return;
}

const nsSSQ::SSQTicketInfo&
WarControlManager::GetTicketInfo()
{
    static const BYTE sNullTicketInfo[sizeof(nsSSQ::SSQTicketInfo)] = { 0, };
    if(ms_pWarControlManager == 0)
        return *reinterpret_cast<const nsSSQ::SSQTicketInfo*>(sNullTicketInfo);
    WarControlManager* const this_ = ms_pWarControlManager;
    return MGR_SSQIMPLS(this_).SSQTicketTimeInfoCtrl.GetTicketInfo();
}

VOID
WarControlManager::Serialize()
{
}

BOOL
WarControlManager::SendPacket(const MSG_BASE* pMsg, const WORD wSize)
{
    __UNUSED((pMsg, wSize));
    return TRUE;
}

//==================================================================================================
//  <WarControlManager><SendPacket>

VOID
WarControlManager::OnMsg(const SSQCtrlCmd::Created&, DungeonSSQ* const pDungeonSSQ)
{
    if(ms_pWarControlManager == 0)
        return;
    WarControlManager* const this_ = ms_pWarControlManager;

    // ������ Key�� MapCode ������Ѿ� �Ѵ�.
    const MAPCODE CodeOfSSQ = pDungeonSSQ->GetMapCode();
    const KEYTYPE zoneKey   = pDungeonSSQ->GetKey();

    MSG_AG_ZONE_INSTANCE_CREATE_CMD msg;
    msg.m_dwKey     = 0;
    msg.m_MapCode   = CodeOfSSQ;
    msg.m_RoomKey   = zoneKey;
    msg.m_TypeOfIndun = INDUN_KIND_SS_QUEST;
    msg.m_RoomInfo  = *pDungeonSSQ->GetRoomInfo();        //< ������ �� ����
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    msg.party_zone_info_ = PartyOfTopOfTrialByNormal;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    pDungeonSSQ->SendToLinkedServer(&msg, sizeof(msg));

    //////////////////////////////////////////////////////////////////////////
    //  �ӽ� ó��
    SSQ_UPDATERS_IT itFind = MGR_SSQIMPLS(this_)._FindCodeOfSSQ(CodeOfSSQ);
    if(itFind == MGR_SSQIMPLS(this_).UpdateList.end())
        return;

    SSQ_UPDATER_NODE& rNode = *itFind;
    // �Ʒ��� ���� ��Ȳ�� Ŭ���� ���� ���ϴ�. �ѹ��� �� �� �ֵ��� ����.
    rNode.SetKey(zoneKey);
    SSQUpdateCtrl& rCtrl = *rNode.pUpdater;
    rCtrl.SetSSQField(pDungeonSSQ);

    SSQMsgRouterInAgent* const pMsgRouter = rCtrl.GetMsgRouter();
    if(pDungeonSSQ->GetLinkedServer())
    {
        pMsgRouter->RegisterLinkedServer(pDungeonSSQ->GetLinkedServer()->GetSessionIndex());
        BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = pMsgRouter->GetCMD_AG();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        {
            MSGSUB_SSQ_S2S_FIELD_CREATE msg;
            msg.SerializeTo(pHeader);
        }
    }    
    //////////////////////////////////////////////////////////////////////////
}

VOID
WarControlManager::OnMsg(const SSQCtrlCmd::Destroy&, DungeonSSQ* const pDungeonSSQ)
{
    //  1) Agent�� SSQ Process�� ���� ���Ḧ �߻���ų ���,
    //  2) Agent�� BattleServer down���� ������ ���� ������ �����ϴ� ������ �ϰ� �����Ű�� ���
    //  1-1) 1�� �����, �ش� BattleServer::SSQ�� �������� Ŭ���� �� ��Ȳ�̾�� �Ѵ�.
    //      BattleServer�� SSQ�� ���� <- SSQ���� Instance Static Dungeon ����, ������ ����, Ÿ�Ӿƿ��� ���� ��Ȳ���� ��� ����ֱ� ����.
    //  2-1) 2�� ����� ��ġ�� ���� ������. ���� ���, ���� ����� ���� ������ ���ؾ� �� ���̴�.
    //      (080724)(FOCUS)
    //      - �ش� ������ ���� �޽��� ���� ���Ἲ ������ �ذ��϶�.
    //      - �ϴ� ���� ó�� ������ LinkedServer�� Ptr�� ���� ��ü�� ��ȿ�ϴ�.
    //      - ...�ֽô��� ������ �������ٰ� ����Ǵ� ��Ȳ���� ��� �ľ��� ���ΰ�?...
    if(ms_pWarControlManager == 0)
        return;
    WarControlManager* const this_ = ms_pWarControlManager;

    // ������ Key�� MapCode ������Ѿ� �Ѵ�.
    const MAPCODE CodeOfSSQ = pDungeonSSQ->GetMapCode();
    const KEYTYPE zoneKey   = pDungeonSSQ->GetKey();

    //////////////////////////////////////////////////////////////////////////
    //  �ӽ� ó��
    SSQ_UPDATERS_IT itFind = MGR_SSQIMPLS(this_)._FindCodeOfSSQ(CodeOfSSQ);
    if(itFind == MGR_SSQIMPLS(this_).UpdateList.end())
        return;

    SSQ_UPDATER_NODE& rNode = *itFind;
    SSQUpdateCtrl* const pCtrl  = rNode.pUpdater;
    SSQMsgRouterInAgent* const pMsgRouter = pCtrl->GetMsgRouter();
    pMsgRouter->Update(CodeOfSSQ);            // flush buffer
    //pMsgRouter->RegisterLinkedServer(0);    // in Update : Auto-clear

    //  Last Process
    rNode.SetKey(0);
    pCtrl->SetSSQField(0); // clear

    //  replace pCtrl
    const eTIME_TYPE updater_type = pCtrl->GetTimeType();

    SSQUpdateCtrl_None* const pNoneCtrl = TAllocNew(SSQUpdateCtrl_None_Info::value_type);
    {
        pNoneCtrl->Migration(pNoneCtrl, pCtrl);

        sTIME_INFO tmInfo;
        sTIME_INFO::SetTimeValue(tmInfo);
        pNoneCtrl->Init(tmInfo);

        pNoneCtrl->RegisterUpdateCtrl(pNoneCtrl);
        rNode.pUpdater = pNoneCtrl;
    }

    //  < �̰� �Լ� ȣ�� �ϳ��� �ذ��� �� �ֵ��� ����.
    switch(updater_type)
    {
    case TT_NONE:   TAllocDelete(SSQUpdateCtrl_None_Info::value_type, pCtrl); break;
    case TT_NOTICE: TAllocDelete(SSQUpdateCtrl_Notice_Info::value_type, pCtrl); break;
    case TT_WAITING:TAllocDelete(SSQUpdateCtrl_Waiting_Info::value_type, pCtrl); break;
    case TT_BATTLE: TAllocDelete(SSQUpdateCtrl_Battle_Info::value_type, pCtrl); break;
    case TT_CLEAR:  TAllocDelete(SSQUpdateCtrl_Clear_Info::value_type, pCtrl); break;
    }
    //////////////////////////////////////////////////////////////////////////
    //  
}

VOID
WarControlManager::OnMsg(const SSQCtrlCmd::UserState& rInfo, DungeonSSQ* const pDungeonSSQ)
{
    if(ms_pWarControlManager == 0)
        return;
    WarControlManager* const this_ = ms_pWarControlManager;

    //  (WAVERIX) (080727)
    //  -    ������ SSQ�� ���� ���� �Ŀ� ���� ó�� ����� ��������.
    //  ���� Ʈ������� SSQ�� �����ϱ� ������ �Ͼ��. �Ͼ �Ŀ��� �󸶰��� �����ϴ� ���¸� ���?
    const MAPCODE CodeOfSSQ = pDungeonSSQ->GetMapCode();
    SSQ_UPDATERS_IT itFind = MGR_SSQIMPLS(this_)._FindCodeOfSSQ(CodeOfSSQ);
    if(itFind == MGR_SSQIMPLS(this_).UpdateList.end())
        return;

    SSQ_UPDATER_NODE& rNode = *itFind;
    SSQUpdateCtrl* const pCtrl = rNode.pUpdater;

    User* const pUser = rInfo.pUser;
    rInfo.isEnter ? pCtrl->UserEnter(pUser)
                  : pCtrl->UserLeave(pUser);
}

VOID
WarControlManager::OnMsg(const SSQCtrlCmd::UserEvent& rInfo, DungeonSSQ* const pDungeonSSQ)
{
    if(ms_pWarControlManager == 0)
        return;
    WarControlManager* const this_ = ms_pWarControlManager;

    //  (WAVERIX) (080727)
    //  -    ������ SSQ�� ���� ���� �Ŀ� ���� ó�� ����� ��������.
    //  ���� Ʈ������� SSQ�� �����ϱ� ������ �Ͼ��. �Ͼ �Ŀ��� �󸶰��� �����ϴ� ���¸� ���?
    const MAPCODE CodeOfSSQ    = pDungeonSSQ->GetMapCode();
    SSQ_UPDATERS_IT itFind = MGR_SSQIMPLS(this_)._FindCodeOfSSQ(CodeOfSSQ);
    if(itFind == MGR_SSQIMPLS(this_).UpdateList.end())
        return;

    SSQ_UPDATER_NODE& rNode = *itFind;
    SSQUpdateCtrl* const pCtrl = rNode.pUpdater;

    User* const pUser = rInfo.pUser;
    switch(rInfo.event)
    {
    case SSQCtrlCmd::OnAG_RoomJoinAck:
        {
            MGR_SSQIMPLS(this_).OnRecv_AG_RoomJoinAck(pCtrl, pUser, pDungeonSSQ);
        }
        break;
    }
}

