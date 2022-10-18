#pragma once
#ifndef WORLDSERVER_PACKET_HANDLER_H
#define WORLDSERVER_PACKET_HANDLER_H

#include <FunctionMap.h>
#include <PacketStruct.h>

//==================================================================================================

namespace ns_world {
;

class ServerSessionInWorld;

enum ePACKET_HANDLER
{
    //PACKET_HANDLER_CLIENT = 0,
    PACKET_HANDLER_DBPROXY,
    PACKET_HANDLER_AGENT,
    PACKET_HANDLER_MASTER,
    //
    PACKET_HANDLER_COUNTS
};

struct PacketHandlerNode;
struct PacketHandlerInfo;

//==================================================================================================

class PacketHandler
{
public:
    typedef void (*fnHandler)(ns_world::ServerSessionInWorld* const server_session,
                              const MSG_BASE* const msg, const uint16_t msg_size);

    static PacketHandler* Instance();

    void ParsePacket(ePACKET_HANDLER handler_type,
                     ns_world::ServerSessionInWorld* server_session,
                     MSG_BASE* msg_base, WORD msg_size);
private:
    struct HANDLER_FUNC : public util::BASE_FUNC
    {
        fnHandler m_fnHandler;
    };

    PacketHandler();
    ~PacketHandler();
    static PacketHandler* CreateInstance();

    bool Register();
    bool AddHandler(ePACKET_HANDLER handler_type,
                    BYTE category, BYTE protocol, fnHandler fnHandler);

    // f100819.1L, a batched process for a partial protocol group
    bool _RegisterPartialBlock(const TCHAR* group_name, ePACKET_HANDLER handler_type,
                               const ns_world::PacketHandlerInfo& regist_info);

    // fields...
    util::FunctionMap function_table_[PACKET_HANDLER_COUNTS];
    // last order...
    static PacketHandler* instance_static_;
    __DISABLE_COPY(PacketHandler);
}; //end of class

//==================================================================================================

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

struct PacketHandlerInfo
{
    const PacketHandlerNode* node_array_;
    int number_of_array_;
};

}; //end of namespace

//==================================================================================================

inline ns_world::PacketHandler* ns_world::PacketHandler::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}


#endif //WORLDSERVER_PACKET_HANDLER_H