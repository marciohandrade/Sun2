#pragma once
#ifndef AGENTSERVER_SECURE_XIGNCODE_H
#define AGENTSERVER_SECURE_XIGNCODE_H

#if USING_XIGNCODE

namespace ns_secure {
;

// security control per user session for XignCode
const int kXignCodePacketCycle = 5;

struct XignCode
{
    HANDLE handle;
    int code_type; // { Z_SCAN_TYPE_ANTI_CHEAT_ENGINE, Z_SCAN_TYPE_ANTI_NONECLIENTBOT }
    int number_of_send_packets;
    //int packet_cycle;
    bool completed_anti_cheat_code; // whether anti cheat full packet sent
    bool is_client_request_packet; // { 0: result code | 1: code request }
};

extern bool XignCode_Initilize();
extern bool XignCode_Uninitialize();

};

#endif //USING_XIGNCODE

#endif AGENTSERVER_SECURE_XIGNCODE_H