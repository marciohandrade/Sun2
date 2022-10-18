#include "stdafx.h"

#include <AgentServer.h>
#include "PacketHandler.h"
#include "PacketHandler.Shared.h"
#include <UserSessions/UserSession.h>
#include <UserSessions/UserManager.h>
#include <UserSessions/User.h>

#include "Handler_FromClient.h"
#include "Handler_FromDBProxy.h"
#include "Handler_FromGameServer.h"
#include "Handler_FromMasterServer.h"
#include "Handler_FromWorldServer.h"
#include "Handler_AU_AUTH.h"
#include "Handler_AE.h"

//////////////////////////////////////////////////////////////////////////
//  @history
//  - 09/05/26  waverix, rearrange code
//

//==================================================================================================
//
PACKET_CONTROL::uPACKET_STRUCTURE
PACKET_CONTROL::sRECV_PACKET_STRUCTURE;

PACKET_CONTROL::uPACKET_STRUCTURE
PACKET_CONTROL::sSEND_PACKET_STRUCTURE;

BOOL
PACKET_CONTROL::m_bShowPacketStream = false;

//-------------------------------------------------------
// MODULE VARIABLE
//static PACKET_HANDLER_DELEGATOR ms_pInstance = NULL;
PACKET_HANDLER_DELEGATOR
PacketHandler::ms_pInstance = NULL;



//==================================================================================================
//
ProtocolMatrix::ProtocolMatrix()
{
    ZeroMemory(&m_Array, sizeof(m_Array));
    m_DefaultPhase   = PHASE_UNLIMITED;
    m_DefaultHandler = (PH_fnCAHandler)&PacketHandlerShared::OnUSER_EMPTY_HANDLER;
    m_FowardHandler  = (PH_fnCAHandler)&PacketHandlerShared::OnGAME_FORWARD_HANDLER;
}

inline const ProtocolMatrix::sNode*
ProtocolMatrix::Find(BYTE category, BYTE protocol) const
{
    const DWORD index = MAKEWORD(protocol, category);
    const sNode& rAlign = m_Array[index];
    if(rAlign.m_fnHandler != 0)
        return &rAlign;

    return NULL;
}

inline bool
ProtocolMatrix::RegisterAllDefault(const ePHASE_SHIFT phase, PH_fnCAHandler handler)
{
    sNode align;
    align.wPROTOCOLnCATEGORY = 0;
    align.ePHASE_SHIFT_T = phase;
    align.m_fnHandler = handler;

    for(sNode* pAlign = &m_Array[0]; pAlign != &m_Array[array_t::length_]; ++pAlign)
        *pAlign = align;

    m_DefaultPhase = phase;
    m_DefaultHandler = handler;
    return true;
}

inline bool
ProtocolMatrix::RegisterHandler(const sPACKET_HANDLER_INFO* IN pInfo)
{
    const BYTE category = LOBYTE(pInfo->wCATEGORYnPROTOCOL);
    const BYTE protocol = HIBYTE(pInfo->wCATEGORYnPROTOCOL);

    const DWORD index = MAKEWORD(protocol, category);   // wPROTOCOLnCATEGORY
    sNode& rAlign = m_Array[index];
    if(rAlign.m_fnHandler != 0 &&
       (m_DefaultHandler != rAlign.m_fnHandler) &&
       (m_FowardHandler != rAlign.m_fnHandler))
    {
        return false;
    }

    rAlign.wPROTOCOLnCATEGORY = (WORD)index;
    rAlign.ePHASE_SHIFT_T = pInfo->ePHASE_SHIFT_T;
    rAlign.m_fnHandler = (PH_fnCAHandler)pInfo->pfnHandler;
    return true;
}

inline bool
ProtocolMatrix::RegisterHandlerRange(const BYTE category, const ePHASE_SHIFT phase,
                                     PH_fnCAHandler handler)
{
    sNode align;
    align.ePHASE_SHIFT_T = phase;
    align.m_fnHandler = handler;

    DWORD index = MAKEWORD(0, category);
    const DWORD indexE = index + 256; // MAKEWORD(255, category);
    sNode* pAlign = &m_Array[index],
         * pEnd = &m_Array[indexE];
    for( ; pAlign != pEnd; ++pAlign)
    {
        *pAlign = align;
        pAlign->wPROTOCOLnCATEGORY = (WORD)index++;
    }
    return true;
}


#ifdef __NA000000_081104_REGISTERED_PROTOCOL_NAMES__
inline void
ProtocolMatrix::RegisterHandlerName(const BYTE category, const BYTE protocol,
                                    const CHAR* const protocol_name)
{
    const DWORD index = MAKEWORD(protocol, category);   // wPROTOCOLnCATEGORY
    sNode& rAlign = m_Array[index];
    rAlign.m_ProtocolName = protocol_name;
}
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
PacketHandler::PacketHandler()
#ifndef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
    : m_pProtocolMatrix(0)
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
{
    _Initialize();
}

PacketHandler::~PacketHandler()
{
    m_FuncMaps[FIELD_SERVER] = NULL;
    m_FuncMaps[BATTLE_SERVER] = NULL;

    for(DWORD i = 0; i < MAX_SERVER; ++i)
    {
        SAFE_DELETE(m_FuncMaps[i].REFERENCE());
    }

#ifndef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
    SAFE_DELETE(m_pProtocolMatrix);
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
}

VOID
PacketHandler::_Initialize()
{
    _Locking();
    _CALocking();

    ZeroMemory(m_FuncMaps, sizeof(m_FuncMaps));
#ifndef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
    m_pProtocolMatrix = new ProtocolMatrix();
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC

    //m_FuncMaps[UNKNOWN_SERVER] = new FunctionMap();
    m_FuncMaps[AGENT_SERVER] = new util::FunctionMap();
    m_FuncMaps[GAME_DBPROXY] = new util::FunctionMap();
    m_FuncMaps[WORLD_SERVER] = new util::FunctionMap();
    m_FuncMaps[MASTER_SERVER] = new util::FunctionMap();

    //m_FuncMaps[ACCOUNT_DBPROXY] = new util::FunctionMap();
    //m_FuncMaps[TOURNAMENT_SERVER].REFERENCE());
    m_FuncMaps[AUTH_AGENT_SERVER] = new util::FunctionMap();
    //m_FuncMaps[OP_SERVER] = new util::FunctionMap();
    m_FuncMaps[TEMP_SERVER] = new util::FunctionMap();
    //m_FuncMaps[PORTAL_SERVER] = new util::FunctionMap();
    //m_FuncMaps[SOLARAUTH_SERVER] = new util::FunctionMap();

    // 공유한다.
    m_FuncMaps[GAME_SERVER] = new util::FunctionMap();
    m_FuncMaps[FIELD_SERVER] = m_FuncMaps[GAME_SERVER];
    m_FuncMaps[BATTLE_SERVER] = m_FuncMaps[GAME_SERVER];

    //
    //
}

VOID
PacketHandler::ReleaseHandler()
{
    SAFE_DELETE(ms_pInstance.REFERENCE());
}

VOID
PacketHandler::RegisterHandler()
{
    if(ms_pInstance() == NULL)
    {
        ms_pInstance = new PacketHandler();
    }

    ms_pInstance->_UnLocking();
    ms_pInstance->_CAUnLocking();
    Handler_AG::RegisterHandler_ALL(ms_pInstance);
    Handler_AD::RegisterHandler_ALL(ms_pInstance);
    Handler_AM::RegisterHandler_ALL(ms_pInstance);
    Handler_AW::RegisterHandler_ALL(ms_pInstance);
    Handler_AU_AUTH::RegisterHandler_ALL(ms_pInstance);
    Handler_AE::RegisterHandler_ALL(ms_pInstance);
    //Handler_AZ::RegisterHandler_ALL(ms_pInstance);

    //    USER
#ifdef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
    ms_pInstance->m_ProtocolMatrix.RegisterAllDefault(
#else
    ms_pInstance->m_pProtocolMatrix->RegisterAllDefault(
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
        PHASE_UNLIMITED, (PH_fnCAHandler)&PacketHandlerShared::OnUSER_EMPTY_HANDLER);

    Handler_CG::RegisterHandler_ALL(ms_pInstance);

    ms_pInstance->_CALocking();
    ms_pInstance->_Locking();
}

BOOL
PacketHandler::_RegisterHandler(eSERVER_TYPE serverType, sPACKET_HANDLER_INFO* pINFOs)
{
    if(_IsLock())
        return false;

    sPACKET_HANDLER_INFO* pOffset = pINFOs;

    BOOL bResult = true;
    if((DWORD)MAX_SERVER > (DWORD)serverType)
    {
        FUNCTION_MAP_DELEGATOR& rFuncMap = m_FuncMaps[serverType];
        if(rFuncMap != NULL)
        {
            for( ; pOffset; ++pOffset)
            {
                if(pOffset->pfnHandler != 0)
                {
                    FUNC_GENERIC_DELEGATOR func = new FUNC_GENERIC;
                    func->m_dwFunctionKey = pOffset->wCATEGORYnPROTOCOL;
                    func->m_fnHandler = pOffset->pfnHandler;
                    bResult = rFuncMap->Add(func());
                    if(bResult == 0)
                        break;
                }
                else if(pOffset->wCATEGORYnPROTOCOL == 0)
                {
                    break;
                }
            }
        }
        else
            bResult = false;
    }
    else
        bResult = false;

    if(bResult == 0)
    {
        BYTE category = BYTE(pOffset->wCATEGORYnPROTOCOL & 0xFF);
        BYTE protocol = BYTE((pOffset->wCATEGORYnPROTOCOL >> 8) & 0xFF);

        MessageOutEx(g_pSunLog,
                     CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
                     "Can't Register %s Packet Handler [C:%03u][P:%03u]",
                     GetServerType2String(serverType), category, protocol);
    }

    return bResult;
}

BOOL
PacketHandler::_RegisterHandlerCA(eUSER_PARSER_TYPE userType, sPACKET_HANDLER_INFO* pINFOs)
{
    __UNUSED(userType);
    if(_IsLock())
        return false;

    sPACKET_HANDLER_INFO* pOffset = pINFOs;    // TEMP_USER는 별도로 PacketHandler가 분리되지 않는다.
    do
    {
        if(pOffset->pfnHandler == 0)
            break;
#ifdef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC 
        if(m_ProtocolMatrix.RegisterHandler(pOffset) == false)
#else
        if(m_pProtocolMatrix->RegisterHandler(pOffset) == false)
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
        {
            MessageOutEx(g_pSunLog,
                         CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
                         "Can't Register %s Packet Handler [C:%03u][P:%03u]",
                         "User Session",
                         LOBYTE(pOffset->wCATEGORYnPROTOCOL), HIBYTE(pOffset->wCATEGORYnPROTOCOL));
        }

    } while(++pOffset);

    return true;
}

bool PacketHandler::RegisterFowardHandlerGroupCG(eUSER_PARSER_TYPE user_parse_type, BYTE category)
{
    __UNUSED(user_parse_type);
    if (_IsLock()) {
        return false;
    }

#ifdef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC 
    return m_ProtocolMatrix.RegisterHandlerRange(category, PHASE_UNLIMITED,
#else
    return m_pProtocolMatrix->RegisterHandlerRange(category, PHASE_UNLIMITED,
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
        (PH_fnCAHandler)&PacketHandlerShared::OnGAME_FORWARD_HANDLER);
}

#ifdef _NA000000_CW_PROTOCOL_UNIFICATION
bool PacketHandler::RegisterFowardHandlerGroupCW(eUSER_PARSER_TYPE user_parse_type, BYTE category)
{
    if (_IsLock()) {
        return false;
    }
    return m_pProtocolMatrix->RegisterHandlerRange(
        category, PHASE_UNLIMITED, (PH_fnCAHandler)&PacketHandlerShared::OnWORLD_FORWARD_HANDLER);
}
#endif
#ifdef __NA000000_081104_REGISTERED_PROTOCOL_NAMES__
BOOLEAN
PacketHandler::RegisterHandlerNames()
{
    const BOOLEAN bEnabled = !!PACKET_CONTROL::GetShowPacketStream();
#ifdef _DEBUG
    if(bEnabled == 0)
        PACKET_CONTROL::SetShowPacketStream(!bEnabled);

    const CPShowFNTable* pCPShow = PACKET_CONTROL::m_CPShowHandler;
    for(DWORD category = 0; category < 256; ++category)
        for(DWORD protocol = 0; protocol < 256; ++protocol)
        {
            const CHAR* pProtocolName = pCPShow->fnQuery(BYTE(category), BYTE(protocol), NULL);
#ifdef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC 
            m_ProtocolMatrix.RegisterHandlerName(BYTE(category), BYTE(protocol),pProtocolName);
#else
            m_pProtocolMatrix->RegisterHandlerName(BYTE(category), BYTE(protocol),pProtocolName);
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
        }

    if(bEnabled == 0)
        PACKET_CONTROL::SetShowPacketStream(bEnabled);
#endif
    return true;
}
#endif

const FUNC_GENERIC*
PacketHandler::GetServerProtocolBinding(eSERVER_TYPE server_type, BYTE category, BYTE protocol)
{
    FUNCTION_MAP_DELEGATOR& function_map = m_FuncMaps[server_type];
    return (const FUNC_GENERIC*)function_map->Find(MAKEWORD(category, protocol));
}

const ProtocolMatrix::sNode* 
PacketHandler::GetClientProtocolBinding(BYTE category, BYTE protocol) const
{
#ifdef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
    return m_ProtocolMatrix.Find(category, protocol);
#else
    return m_pProtocolMatrix->Find(category, protocol);
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
}

//
//
//
inline BOOL
PacketHandler::CanDoProcess(FUNC_GENERIC_DELEGATOR& funcDelegator)
{
    if(funcDelegator == NULL)
    {
        return false;
    }
    return true;
}
//
//
//

BOOL
PacketHandler::_ParsePacket(eSERVER_TYPE serverType, ServerSession* pSession,
                            MSG_BASE* pMsg, WORD wSize)
{
    const DWORD ERRCODE_INVALID_SERVERTYPE       = 100;
    const DWORD ERRCODE_INVALID_FUNCMAP          = 110;
    const DWORD ERRCODE_CANT_FORWARD_PACKET      = 120;
    const DWORD ERRCODE_PROTECTED_HANDLER        = 130;
    const DWORD ERRCODE_UNREGISTERED_AG_PROTOCOL = 140;
    const DWORD ERRCODE_SYNC_MISMATCH            = 200;

    //////////////////////////////////////////////////////////////////////////
    //    <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak((DWORD)MAX_SERVER > (DWORD)serverType, ERRCODE_INVALID_SERVERTYPE);

        FUNCTION_MAP_DELEGATOR& rFuncMap = m_FuncMaps[serverType];
        IfFailThenBreak(rFuncMap != NULL, ERRCODE_INVALID_FUNCMAP);

        const WORD category_and_protocol = MAKEWORD(pMsg->m_byCategory, pMsg->m_byProtocol);
        FUNC_GENERIC_DELEGATOR fnINFO = (FUNC_GENERIC*)rFuncMap->Find(category_and_protocol);

        const DWORD FORWADABLE_SERVERMASK = (1 << BATTLE_SERVER)
                                          | (1 << FIELD_SERVER)
                                          | (1 << GAME_SERVER)
                                    #ifdef _NA002676_WORLD_SERVER_RENEWAL
                                          | (1 << WORLD_SERVER)
                                    #endif
                                          ;
        if(fnINFO == NULL) // CG forward
        {
            IfFailThenBreak(FORWADABLE_SERVERMASK & (1 << serverType), ERRCODE_SYNC_MISMATCH);

            User* const pUser = UserManager::Instance()->GetUser(pMsg->m_dwKey);
            IfFailThenBreak(pUser, ERRCODE_CANT_FORWARD_PACKET);

            pUser->SendPacket(pMsg, wSize);
            return true;
        }
        else
        {
            IfFailThenBreak(CanDoProcess(fnINFO), ERRCODE_PROTECTED_HANDLER);
        }

        //    G->A Handler - 특수 처리가 추가적으로 필요하게 되었다. player->SendPacket으로 AG를 보내는 사례가 있다.
        if(FORWADABLE_SERVERMASK & (1 << serverType))
        {
            const WORD OneToN_Broadcast =
                MAKEWORD(AG_CONNECTION, AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN);

            if(OneToN_Broadcast == category_and_protocol)
            {
                MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN* pBroadcaster =
                    static_cast<MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN*>(pMsg);
                MSG_BASE_FORWARD* const pBrdMsg = pBroadcaster->GetBrdPacketPtr();

                BOOST_STATIC_ASSERT(AG_MAX == CATEGORY_GENERATOR_213);
                switch(pBrdMsg->m_byCategory)
                {
                case AG_CONNECTION  : case AG_CHARINFO : case AG_SYNC : case AG_ZONE :
                case AG_PARTY       : case AG_STATUS   : case AG_GM   : case AG_GUILD:
                case AG_CHUNTING    : case AG_MISSION  : case AG_CHAO : case AG_ITEM :
                case AG_EVENT       :
                    {
                        const WORD category_and_protocol =
                            MAKEWORD(pBrdMsg->m_byCategory, pBrdMsg->m_byProtocol);
                        FUNC_GENERIC_DELEGATOR fnINFO_CHK =
                            (FUNC_GENERIC*)rFuncMap->Find(category_and_protocol);

                        IfFailThenBreak(fnINFO_CHK != NULL, ERRCODE_UNREGISTERED_AG_PROTOCOL);
                        //if(fnINFO_CHK == NULL)
                        //  fnINFO->m_fnHandler(pSession, pMsg, wSize); // forward message
                        fnINFO_CHK->m_fnHandler(pSession, pBrdMsg, pBroadcaster->msg_body_size);
                    }
                    return true;
                }

                if(flow.IsErrorOccured())
                    break;
            }
        }
        fnINFO->m_fnHandler(pSession, pMsg, wSize);

        return true;
    }
    FLOWCONTROL_END;
    
    //////////////////////////////////////////////////////////////////////////
    //    <ERROR CONTROL>
    switch(const DWORD errcode = flow.GetErrorCode())
    {
    case ERRCODE_INVALID_SERVERTYPE :
    case ERRCODE_INVALID_FUNCMAP    :
    case ERRCODE_CANT_FORWARD_PACKET:
    case ERRCODE_PROTECTED_HANDLER  :
        SUNLOG(eCRITICAL_LOG, "%s Handler Null, Invalid Type, UnAuthorized [C:P=%03d:%03d]-ERR(%3u)",
               GetServerType2String(serverType), pMsg->m_byCategory, pMsg->m_byProtocol, errcode);
        return false;
    case ERRCODE_UNREGISTERED_AG_PROTOCOL:
        {
            MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN* pBroadcaster =
                static_cast<MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN*>(pMsg);
            MSG_BASE_FORWARD* const pBrdMsg = pBroadcaster->GetBrdPacketPtr();
            SUNLOG(eCRITICAL_LOG,
                   "%s Handler Null, Invalid Type, UnAuthorized [C:P=%03d:%03d]-ERR(%3u)",
                   GetServerType2String(serverType),
                   pBrdMsg->m_byCategory, pBrdMsg->m_byProtocol, errcode);
        }
        break;
    case ERRCODE_SYNC_MISMATCH:
        SUNLOG(eFULL_LOG, "[SYNC_MISMATCH]:Forwarded Packet [C:P=%03u:%03u]",
               pMsg->m_byCategory, pMsg->m_byProtocol);
        return false;
    }

    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Unknown Error Status ERR(%3u)", flow.GetErrorCode());
    return false;
}

BOOL
PacketHandler::_ParsePacket(eSERVER_TYPE serverType, ServerSession* pSession,
                            MSG_BASE_INTERNAL* pMsg, WORD wSize)
{
    if((DWORD)MAX_SERVER > (DWORD)serverType)
    {
        FUNCTION_MAP_DELEGATOR& rFuncMap = m_FuncMaps[serverType];
        if(rFuncMap != NULL)
        {
            WORD wCP = MAKEWORD(pMsg->m_byCategory, pMsg->m_byProtocol);

            FUNC_GENERIC_DELEGATOR fnINFO = (FUNC_GENERIC*)rFuncMap->Find(wCP);

            if(CanDoProcess(fnINFO))
            {
                fnINFO->m_fnHandler(pSession, (MSG_BASE*)pMsg, wSize);
                return true;
            }
        }
    }

    MessageOutEx(g_pSunLog,
                 CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
                 "%s Handler Null, Invalid Type, UnAuthorized [C:P=%03d:%03d]",
                 GetServerType2String(serverType), pMsg->m_byCategory, pMsg->m_byProtocol);

    return false;
}


#define PACKET_CATEGORY_SKIP_NODE(node)     case node:
#define PACKET_CATEGORY_SKIP_FILTER()          \
    PACKET_CATEGORY_SKIP_NODE(CG_SYNC)         \
    PACKET_CATEGORY_SKIP_NODE(CG_CONVERSATION) \
    PACKET_CATEGORY_SKIP_NODE(CG_TRIGGER)      \
    PACKET_CATEGORY_SKIP_NODE(CG_BATTLE)

#define PACKET_PROTOCOL_SKIP_NODE(node)     case node:
#define PACKET_PROTOCOL_SKIP_FILTER()          \
    PACKET_PROTOCOL_SKIP_NODE(MAKEWORD(CG_CHARINFO, CG_CHARINFO_SELECT_INFO_SYN))


BOOL
PacketHandler::_ParsePacketCA(eUSER_PARSER_TYPE userType, User* pSession, MSG_BASE* pMsg, WORD wSize)
{
    const DWORD ERRCODE_INVALID_USER_TYPE = 1000;
    const DWORD ERRCODE_CANT_FIND_HANDLER = ERRCODE_INVALID_USER_TYPE+1;

#ifdef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
    const DWORD ERRCODE_INVALID_MSG_BASE_STATUS = ERRCODE_CANT_FIND_HANDLER + 1;
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC

    //////////////////////////////////////////////////////////////////////////
    //    <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak((DWORD)UPT_USER_MAX > (DWORD)userType, ERRCODE_INVALID_USER_TYPE);

#ifdef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
        IfFailThenBreak(pMsg, ERRCODE_INVALID_MSG_BASE_STATUS);

        const ProtocolMatrix::sNode* const pINFO = m_ProtocolMatrix.Find(pMsg->m_byCategory, pMsg->m_byProtocol);
#else
        const ProtocolMatrix::sNode* const pINFO = m_pProtocolMatrix->Find(pMsg->m_byCategory, pMsg->m_byProtocol);
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
        IfFailThenBreak(pINFO && pINFO->m_fnHandler, ERRCODE_CANT_FIND_HANDLER);

        //    Routing!!
        (pINFO->m_fnHandler)(pSession, (MSG_BASE*)pMsg, wSize);

        return true;
    }
    FLOWCONTROL_END;
    
    //////////////////////////////////////////////////////////////////////////
    //    <ERROR CONTROL>
    MessageOutEx(g_pSunLog,
                 CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
                 "[ERR:%4u] Critial Fault -or- UnAuthorized [%u][C:P=%03d:%03d]",
                 flow.GetErrorCode(), userType, pMsg->m_byCategory, pMsg->m_byProtocol);

    return false;
}

BOOL
PacketHandler::_ParsePacketCA(eUSER_PARSER_TYPE userType, User* pSession, MSG_BASE_INTERNAL* pMsg, WORD wSize)
{
    __UNUSED((userType, pSession, pMsg, wSize));
    ASSERT(!"사용하지 않는다");
    return false;
}


#ifdef __NA000000_081104_REGISTERED_PROTOCOL_NAMES__
inline const CHAR*
PacketHandler::GetProtocolName(const BYTE category, const BYTE protocol)
{
    // CHANGES: f110609.3L, f110304.2L, prevent crash state when an agent server is closed
    // while on connect a game server.
#ifdef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC 
    if (this == NULL) {
#else
    if (this == NULL || m_pProtocolMatrix == NULL) {
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
        return NULL;
    };
    const DWORD index = MAKEWORD(protocol, category);
#ifdef _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC 
    const ProtocolMatrix::sNode& rAlign = m_ProtocolMatrix.m_Array[index];
#else
    const ProtocolMatrix::sNode& rAlign = m_pProtocolMatrix->m_Array[index];
#endif // _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC
    return rAlign.m_ProtocolName;
}
#endif



#ifdef _DEBUG
const CPShowFNTable*
PACKET_CONTROL::m_CPShowHandler = NULL;
#endif //_DEBUG

static FILE*
s_FD = NULL;

static BOOL
SetFileOutput()
{
    //s_FD = fopen("packetsteam.txt", "w+");
    return true;
}

#define TO_CODE(pstr)    pstr

const struct sFLOW_CODE
{
    CHAR* flow;
} s_FLOW_CODE[4] = 
{
    TO_CODE("C2A"),
    TO_CODE("A2C"),
    TO_CODE("G2C"),
    TO_CODE("C2G"),
};

#undef TO_CODE

// 테스트용 코드, 이후 키를 누르면 ('S'키 정도로) 보일 수 있도록 작업할 예정
VOID
PACKET_CONTROL::ShowPacketStream(MSG_BASE* pBASE, WORD wSize, CHAR* pPrefix)
{
    if(!GetShowPacketStream())
        return;

    static BOOL bOUTPUT = SetFileOutput();

#define TO_CODE(num)        ((DWORD)(__int64)(DWORD*)(s_FLOW_CODE[(num)].flow))
#define TO_CODE_STK(pstr)   ((DWORD)(__int64)(DWORD*)(pstr))
    DWORD prefix = TO_CODE_STK(pPrefix);
    prefix = (TO_CODE(0) == prefix) ?   1
           : (TO_CODE(1) == prefix) ?   1
           : (TO_CODE(2) == prefix) ?   1
           : (TO_CODE(3) == prefix) ?   1
           :                            2;

    const WORD skip_protocol = MAKEWORD(pBASE->m_byCategory, pBASE->m_byProtocol);
    switch(skip_protocol)
    {
    case MAKEWORD(CG_CONNECTION, CG_CONNECTION_HEARTBEAT_SYN):
    case MAKEWORD(CG_CONNECTION, CG_PREPARE_WORLD_CONNECT_SYN):
    case MAKEWORD(CG_CONNECTION, CG_PREPARE_WORLD_CONNECT_NAK):
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    case MAKEWORD(CG_CONNECTION, CG_CONNECTION_TICK_QUERY_SYN):
    case MAKEWORD(CG_CONNECTION, CG_CONNECTION_TICK_QUERY_ACK):
#endif
    case MAKEWORD(CG_SYNC, CG_SYNC_KBMOVE_SYN):
    case MAKEWORD(CG_SYNC, CG_SYNC_MOVE_SYN):
    //case MAKEWORD(CG_BATTLE, CG_BATTLE_MONSTER_ATTACK_CMD):
    //case MAKEWORD(CG_BATTLE, CG_BATTLE_PLAYER_ATTACK_BRD):
    //case MAKEWORD(CG_SKILL, CG_SKILL_ACTION_DELAY_START_BRD):
    //case MAKEWORD(CG_SKILL, CG_SKILL_ACTION_DELAY_RESULT_BRD):
    //case MAKEWORD(CG_SKILL, CG_SKILL_ACTION_INSTANT_RESULT_BRD):
    //case MAKEWORD(CG_SKILL, CG_SKILL_PERIODIC_DAMAGE_BRD):
    case MAKEWORD(CG_STATUS, CG_STATUS_REMOVE_BRD):
    case MAKEWORD(CG_STATUS, CG_STATUS_CHANGE_HP_BRD):
    case MAKEWORD(CG_STATUS, CG_STATUS_CHANGE_MP_BRD):
    case MAKEWORD(CG_MAP, CG_MAP_KBMOVE_SERIAL_CHECK_CMD):
    case MAKEWORD(CG_MAP, CG_MAP_KBMOVE_SERIAL_CHECK_ACK):
        return;
    }

    CHAR str[1024 * 32];
    int iWritten = 0;

    if(wSize > 10000)
    {
        SUNLOG(eCRITICAL_LOG, "말도 안되게 큰 패킷이 송신 또는 수신되었다.");
        return;
    }

    //"USER  C2A\t%03d\t%02X\t%02X\t%03d\t%03d"
#ifdef _DEBUG
    BOOL bShowCPNumber = true;
    if(m_CPShowHandler && (prefix == 1))
    {
    #ifdef __NA000000_081104_REGISTERED_PROTOCOL_NAMES__
        const CHAR* pMSG_STR =
            PacketHandler::GetHandler()->GetProtocolName(pBASE->m_byCategory, pBASE->m_byProtocol);
        bShowCPNumber = !pMSG_STR;
    #else
        const CHAR* pMSG_STR =
            m_CPShowHandler->fnQuery(pBASE->m_byCategory, pBASE->m_byProtocol, NULL);
        bShowCPNumber = pMSG_STR == 0;
    #endif
        if(bShowCPNumber == 0)
            iWritten = _snprintf(str, 200, "[%s]\tS(%03d)\t%s", pPrefix, wSize, pMSG_STR);
    }

    if(bShowCPNumber)
        iWritten = _snprintf(str, 200, "[%s]\tS(%03d)\tC(%03d)\tP(%03d)",
                             pPrefix, wSize, pBASE->m_byCategory, pBASE->m_byProtocol);

#else
    iWritten = _snprintf(str, 200, "[%s]\tS(%03d)\tC(%03d)\tP(%03d)",
                         pPrefix, wSize, pBASE->m_byCategory, pBASE->m_byProtocol);
#endif

    //MessageOutEx(g_pSecureLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_CONNECTION, str);


    //str[iWritten++] = '\n';
    //for(int i=0 ; i<4 ; ++i)
    //{
    //    BYTE chr = ((BYTE*)pBASE)[i];

    //    str[iWritten++] = '[';
    //    BYTE chrHI = (BYTE)(chr/0x10);
    //    str[iWritten++] = (chrHI<10) ? chrHI+'0' : (chrHI-10)+'A';
    //    BYTE chrLO = (BYTE)(chr%0x10);
    //    str[iWritten++] = (chrLO<10) ? chrLO+'0' : (chrLO-10)+'A';
    //    str[iWritten++] = ']';
    //}

    //int spliter = 0;
    //for(int i=4 ; i<wSize ; ++i)
    //{
    //    BYTE chr = ((BYTE*)pBASE)[i];
    //    if(1024*32 < iWritten + 5)
    //        break;

    //    if((spliter++)%16 == 0)
    //        str[iWritten++] = '\n';

    //    str[iWritten++] = '[';
    //    BYTE chrHI = (BYTE)(chr/0x10);
    //    str[iWritten++] = (chrHI<10) ? chrHI+'0' : (chrHI-10)+'A';
    //    BYTE chrLO = (BYTE)(chr%0x10);
    //    str[iWritten++] = (chrLO<10) ? chrLO+'0' : (chrLO-10)+'A';
    //    str[iWritten++] = ']';

    //    //int limit = (1024*32)-iWritten;
    //    //iWritten += _snprintf(str+iWritten, limit, "[%02X]", (((BYTE*)pBASE)[i]));
    //}

    str[iWritten++] = '\0';
    if('\0' != str[0])
        puts(str);

    //str[iWritten-1] = '\r';
    //str[iWritten] = '\n';
    //str[iWritten+1] = '\0';
    //fputs(str, s_FD);
}



