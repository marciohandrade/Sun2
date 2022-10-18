#pragma once
#ifndef __HANDLER_CG_ROOM_H__
#define __HANDLER_CG_ROOM_H__

#include "./PacketHandler.h"





class Handler_CG_ROOM
{
public:
    Handler_CG_ROOM();
    ~Handler_CG_ROOM();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();
};


#endif // __HANDLER_CG_ROOM_H__
