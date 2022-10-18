#pragma once

#include <ServerSessions/ServerSessionManager.h>

class SSQUpdateCtrl;

namespace nsSSQ_Util {};
namespace SSQControlInfo
{
    struct SSQ_UPDATER_NODE;
};

using namespace nsSSQ;
using namespace nsSSQ_Util;
using namespace SSQControlInfo;

#pragma warning(push)
#pragma warning(disable : 4512)
// (NOTE) below objects have not default constructor, current specification have meaningful only
// base constructor 'xxxSender'
//==================================================================================================
//
template< typename _DERIVED, typename _PACKET_T >
class BROADCASTER_MSG_CG : public nsSSQ_Util::xxxSender< _DERIVED, _PACKET_T >
{
public:
    typedef BROADCASTER_MSG_CG<_DERIVED, _PACKET_T> this_type;

    inline packet_structure* const GetPacket() { return m_pPacket; }

    VOID SendPacket(const USER_HASH_PAIR& rPAIR) const
    {
        User* const pUser = rPAIR.second;
        pUser->SendPacket(m_pPacket, m_pPacket->GetSize());
    }

    VOID SendPacket(User* const pUser) const
    {
        pUser->SendPacket(m_pPacket, m_pPacket->GetSize());
    }

    VOID ProcessSSQUser(DungeonSSQ* const pDungeonSSQ) const
    {
        pDungeonSSQ->SendToAll(m_pPacket, m_pPacket->GetSize());
    }

    VOID ProcessAllUser() const
    {
        UserManager* const user_manager = UserManager::Instance();
        user_manager->SendMessageToAllUsers(m_pPacket, m_pPacket->GetSize());
    }
};



template< typename _SUBMSG >
class WAR_MSG_SINGLE_ASSEMBLER_SSQ_INFO_BRD : public WAR_MSG_SINGLE_ASSEMBLER< MSG_CG_STATUS_SSQ_INFO_BRD, _SUBMSG >
{
public:
    VOID SendPacket(User* const pUser) const
    {
        pUser->SendPacket(m_pPacket, m_pPacket->GetSize());
    }

    VOID ProcessAllUser() const
    {
        UserManager* const user_manager = UserManager::Instance();
        user_manager->SendMessageToAllUsers(m_pPacket, m_pPacket->GetSize());
    }
};


class BROADCASTER_MSG_CG_STATUS_SSQ_INFO_BRD
    : public BROADCASTER_MSG_CG< BROADCASTER_MSG_CG_STATUS_SSQ_INFO_BRD, MSG_CG_STATUS_SSQ_INFO_BRD >
{
};

class BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD
    : public BROADCASTER_MSG_CG< BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD, MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD >
{
};


class BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_INFO_BRD
    : public BROADCASTER_MSG_CG< BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_INFO_BRD, MSG_CG_ZONE_EX_SSQ_ROOM_INFO_BRD>
{
};

template< typename _DERIVED, typename _PACKET_T >
class UNICASTER_BETWEEN_SERVER : public nsSSQ_Util::xxxSender< _DERIVED, _PACKET_T >
{
public:
    typedef UNICASTER_BETWEEN_SERVER<_DERIVED, _PACKET_T>        this_type;

    inline packet_structure* const    GetPacket() { return m_pPacket; }

    BOOL SendPacket(ServerSessionEx* const pServerSession, const KEYTYPE KeyOfSSQ) const
    {
        m_pPacket->m_dwKey = KeyOfSSQ;
        pServerSession->SendPacket(m_pPacket, m_pPacket->GetSize());
        //printf("%p\t%u\n"
        //    , const_cast<packet_structure*>(m_pPacket)
        //    , m_PacketSize
        //);
    }
};

class BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD
    : public UNICASTER_BETWEEN_SERVER< BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD, MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD >
{
};


class BROADCASTER_MSG_AW_OPERATION_ROOM_CTRL_CMD
    : public UNICASTER_BETWEEN_SERVER< BROADCASTER_MSG_AW_OPERATION_ROOM_CTRL_CMD, MSG_AW_OPERATION_ROOM_CTRL_CMD >
{
};

//template< typename _DRIVED, typename _PACKET >
//class BROADCASTER_IN_AGENTSERVER : public Broadcaster< _DRIVED, _PACKET >
//{
//    typedef BROADCASTER_IN_AGENTSERVER<_DRIVED, _PACKET>        this_type;
//public:
//    inline MSG_BASE*                GetPacketMsgHeader() { return m_pPacket; }
//    inline WORD                        GetPacketMsgSize() { return m_PacketSize = m_pPacket->GetSize(); }
//
//    SSQ_MSG_SCT_HDR*                m_pMsgHeader;
//    WAR_TAG_P                        m_pOffset;
//public:
//    inline VOID Reset()
//    {
//        new (this) this_type();
//        m_pOffset = MakeInit_SSQPacketStream(m_pMsgHeader);
//    }
//
//    inline SSQ_MSG_SCT_HDR*&        RefMsgHeader() { return m_pMsgHeader; }
//    inline WAR_TAG_P&                RefMsgOffset() { return m_pOffset; }
//};
//class BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD
//    : public BROADCASTER_IN_AGENTSERVER< BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD, MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD >
//{
//public:
//    VOID SendPacket(ServerSession* const pGameServer, const KEYTYPE KeyOfSSQ) const
//    {
////#define _WAVERIX_SSQ_TEST
////#ifndef _WAVERIX_SSQ_TEST
//        m_pPacket->m_dwKey = KeyOfSSQ;
//        pGameServer->SendPacket(m_pPacket, m_PacketSize);
////#else
//        printf("%p\t%u\n"
//            , const_cast<packet_structure*>(m_pPacket)
//            , m_PacketSize
//        );
////#endif
//    }
//};



//==================================================================================================
//    <SSQControlManager>
//
class SSQMsgRouterInAgent : public WarMsgRouter<SSQMsgRouterInAgent, ISSQHandler>
{
    friend struct WarMsgRouter<SSQMsgRouterInAgent, ISSQHandler>;

public:
    typedef sun_type_info<SSQMsgRouterInAgent>    router_info;

    DWORD   m_LinkedServerSessionIdx;    // Pointer 무결성 보장목적으로 키로 대체 Key
    SSQUpdateCtrl* m_pUpdateCtrl;

    BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD&      m_msgCMD_AG;
    BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD&   m_msgCMD_CG;
    static BROADCASTER_MSG_CG_STATUS_SSQ_INFO_BRD       ms_msgBRD_CG;
    static BROADCASTER_MSG_AW_OPERATION_ROOM_CTRL_CMD   ms_msgCMD_AW;

    SSQMsgRouterInAgent()
        : m_LinkedServerSessionIdx(0)
        , m_msgCMD_AG(*new BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD)
        , m_msgCMD_CG(*new BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD)
        , m_pUpdateCtrl(0)
    {
        m_msgCMD_AG.Reset();
        m_msgCMD_CG.Reset();

        ms_msgBRD_CG.Reset();
        ms_msgCMD_AW.Reset();
    }
    ~SSQMsgRouterInAgent()
    {
        delete &m_msgCMD_AG;
        delete &m_msgCMD_CG;
        m_pUpdateCtrl = NULL;
    }

public:
    inline BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD&    GetCMD_AG() { return m_msgCMD_AG; }
    inline BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD& GetCMD_CG() { return m_msgCMD_CG; }

    inline static BROADCASTER_MSG_CG_STATUS_SSQ_INFO_BRD&     GetBRD_CG() { return ms_msgBRD_CG; }
    inline static BROADCASTER_MSG_AW_OPERATION_ROOM_CTRL_CMD& GetCMD_AW() { return ms_msgCMD_AW; }

    inline VOID SetUpdateCtrl(SSQUpdateCtrl* const pCtrl) { m_pUpdateCtrl = pCtrl; }
    inline SSQUpdateCtrl* GetUpdateCtrl() const { return m_pUpdateCtrl; }
    inline DWORD GetSlotNumber() const { return (DWORD)m_Container.size(); }
    inline VOID RegisterLinkedServer(const DWORD serverSessionIndex) { m_LinkedServerSessionIdx = serverSessionIndex; }

    BOOLEAN RoutingC2G(const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header,
                       User* const pUser, const nsSSQ::Rules::eC2G_ReqRule rules);
private:
    BOOLEAN Routing(const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header);
    BOOLEAN RoutingSpecial(const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header);

    VOID SlotUpdate();
public:
    VOID SlotClear(User* const pUser);
    VOID UpdateSharedMsg();
    VOID Update(const SSQCODE CodeOfSSQ);

//#undef SSQ_MODE_NODE
//#define SSQ_MODE_NODE(protocol)                                    \
//    BOOL On##protocol(const nsSSQ::SSQ_MSG_##protocol& rMsg) {    \
//        printf("OnRecv " #protocol "\n");    \
//        BYTE* ptr = (BYTE*)&rMsg;            \
//        BYTE* ptrend = ptr+sizeof(rMsg);    \
//        for(; ptr != ptrend ; ++ptr)        \
//            printf("[%03u]", *ptr);        \
//        puts("");                            \
//        return TRUE;                        \
//    }
};

namespace SSQControlInfo
{
    typedef SSQMsgRouterInAgent::router_info    router_info;
};

#pragma warning(pop)
