//------------------------------------------------------------------------------
//! created : 2010/04/22
//! author : by i4u4me
//! remark : 패킷을 보내고 응답이 오지 않았을경우 다시 요청함.
//------------------------------------------------------------------------------

#ifndef GAMECLIENT_PACKETREQUESTMANAGER_H_
#define GAMECLIENT_PACKETREQUESTMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <Singleton.h>
#include <boost/shared_ptr.hpp>

#define PACKET_WAITING_MILLISECOND (1000 * 30)

//------------------------------------------------------------------------------
//!PacketRequestManager
//------------------------------------------------------------------------------
class PacketRequestManager : public util::Singleton<PacketRequestManager>
{
public:
    class PacketRequestInfo
    {
    public:
        PacketRequestInfo() : syn_packet_(NULL), ack_packet_(NULL), nak_packet_(NULL), 
                            syn_packet_size_(0), send_millisecond_(0), wait_millisecond_(0),
                            is_request_(false){}
        PacketRequestInfo(MSG_BASE* syn_packet, //!< new 생성성해서 넣음
                            MSG_BASE* ack_packet, //!< new로 생성해서 넣음
                            MSG_BASE* nak_packet, //!< new로 생성해서 넣음
                            int syn_packet_size,
                            DWORD send_millisecond, 
                            DWORD wait_millisecond,
                            bool is_request)
        {
            syn_packet_ = syn_packet;
            ack_packet_ = ack_packet;
            nak_packet_ = nak_packet;
            syn_packet_size_ = syn_packet_size;
            send_millisecond_ = send_millisecond;
            wait_millisecond_ = wait_millisecond;
            is_request_ = is_request;
        }
        ~PacketRequestInfo()
        { 
            SAFE_DELETE(syn_packet_); 
            SAFE_DELETE(ack_packet_); 
            SAFE_DELETE(nak_packet_); 
        }

        DWORD send_millisecond() const { return send_millisecond_; }
        void set_send_millisecond(DWORD millisecond) { send_millisecond_ = millisecond; }
        DWORD wait_millisecond() const { return wait_millisecond_; }
        bool is_request() const { return is_request_; }
        MSG_BASE* syn_packet() const { return syn_packet_; }
        MSG_BASE* ack_packet() const { return ack_packet_; }
        MSG_BASE* nak_packet() const { return nak_packet_; }
        int syn_packet_size() const { return syn_packet_size_; }

    private:
        DWORD send_millisecond_;
        DWORD wait_millisecond_;
        int syn_packet_size_;
        bool is_request_;
        MSG_BASE* syn_packet_;  //!< new 로 생성하고 소멸자에서 delete 한다.
        MSG_BASE* ack_packet_;  //!< new 로 생성하고 소멸자에서 delete 한다.
        MSG_BASE* nak_packet_;  //!< new 로 생성하고 소멸자에서 delete 한다.
    };

    typedef boost::shared_ptr<PacketRequestInfo>    PacketRequestInfo_Shared_Ptr;


public:
    PacketRequestManager();
    ~PacketRequestManager();

    void Release();

    void CheckRequestPacket();
    void NetworkProc(MSG_BASE * packet);
    void PushPacket(MSG_BASE* syn_packet, 
                    MSG_BASE* ack_packet, 
                    MSG_BASE* nak_packet, 
                    int syn_packet_size, 
                    DWORD wait_millisecond,
                    bool is_request);

    bool is_sent(BYTE category, BYTE protocol);
    int count() { return static_cast<int>(request_info_vector_.size()); }
    std::vector<PacketRequestInfo_Shared_Ptr>& request_info_vector() { return request_info_vector_; }

private:
    bool ComparePacket_CG_QUEST_COMPLETE_ACK(PacketRequestInfo* info, MSG_CG_QUEST_COMPLETE_ACK* packet);
    bool ComparePacket_CG_SYNC_WINGS_CONTROL_CMD(MSG_CG_SYNC_WINGS_CONTROL_CMD* packet);
    bool ComparePacket_CG_STATUS_STAT_APPLY_ACK(MSG_CG_STATUS_STAT_APPLY_ACK* packet);

    std::vector<PacketRequestInfo_Shared_Ptr>  request_info_vector_;
};


#endif // GAMECLIENT_PACKETREQUESTMANAGER_H_

