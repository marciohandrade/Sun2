#pragma once
#ifndef GAMESERVER_PACKET_HANDLER_H
#define GAMESERVER_PACKET_HANDLER_H

//==================================================================================================
/// IOCPNetwork모듈에서 오는 패킷들을 처리하기 위한 클래스
/**
    @remarks
        - 서버간의 카테고리가 중복될 수 있고 전달하는 데이터 형식이 다르므로 나눔
    @note
        - 새로운 종류의 클라이언트나 서버의 연결이 발생하면 맴버를 추가해야 한다. 
        - 현재 연결 :DBProxy, Master, Client
    @history
        - ~100819
        - 100819, waverix, code arrangement and
                           f100819.1L, add a batch register routine to support a partial register.
                           f100819.2L, change a protocol mapping table
*/
//==================================================================================================

#include <Singleton.h>
#include <FunctionMap.h>
#include <PacketStruct.h>

class ServerSession; // in Common project

enum ePACKET_HANDLER
{
    PACKET_HANDLER_AGENT   = 0,
    PACKET_HANDLER_DBPROXY ,
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
    PACKET_HANDLER_GUILDDB = PACKET_HANDLER_DBPROXY,
#endif
#ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
    PACKET_HANDLER_RANKING = PACKET_HANDLER_DBPROXY,
#endif
    //
    PACKET_HANDLER_MASTER  ,
    //
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
    PACKET_HANDLER_GUILDDB ,
#endif
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
    PACKET_HANDLER_RANKING , // Portal server
#endif
#endif // _NA_008012_20150130_RANKING_SYSTEM

    PACKET_HANDLER_SHOP    ,
    // upperbound
    PACKET_HANDLER_MAX
};

//==================================================================================================

namespace nsPacket {
;
struct PacketHandlerNode;
struct PacketHandlerInfo;

}; //end of namespace

//==================================================================================================

class PacketHandler : public util::Singleton<PacketHandler>
{
public:
    typedef void (*fnHandler)(ServerSession* pSession, MSG_BASE* pMsg, WORD wSize);
    //
    PacketHandler();
    ~PacketHandler();
    //
    bool Register();
    bool AddHandler(ePACKET_HANDLER handler_type,
                    BYTE category, BYTE protocol, fnHandler fnHandler);
    void ParsePacket(ePACKET_HANDLER handler_type, ServerSession* pSession,
                     MSG_BASE* pMsg, WORD wSize);
    void ParsePacket(ePACKET_HANDLER handler_type, ServerSession* pSession,
                     MSG_BASE_INTERNAL* pMsg, WORD wSize);
private:
    struct HANDLER_FUNC : public util::BASE_FUNC
    {
        fnHandler m_fnHandler;
    };
    // f100819.1L, a batched process for a partial protocol group
    bool _RegisterPartialBlock(const TCHAR* group_name, ePACKET_HANDLER handler_type,
                               const nsPacket::PacketHandlerInfo& regist_info);
    //
    util::FunctionMap m_FuncMap[PACKET_HANDLER_MAX];
};

//==================================================================================================

namespace nsPacket {

// f100819.1L, PacketHandlerNode
struct PacketHandlerNode
{
    union {
        uint16_t category_and_protocol_;
        struct {
            uint8_t category_;
            uint8_t protocol_;
        };
    };
    ePHASE_SHIFT phase_shift_;
    PacketHandler::fnHandler handler_;
};

// f100819.1L, PacketHandlerInfo
struct PacketHandlerInfo
{
    const PacketHandlerNode* node_array_;
    int number_of_array_;
};

}; //end of namespace

//==================================================================================================


#endif //GAMESERVER_PACKET_HANDLER_H