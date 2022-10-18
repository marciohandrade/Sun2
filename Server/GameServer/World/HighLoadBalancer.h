#pragma once
#ifndef GAMESERVER_HIGHLOAD_BALANCER_H
#define GAMESERVER_HIGHLOAD_BALANCER_H

// @history
//  08.09.03, waverix, create
//  11.01.11, waverix, upgrade to v0200 =
//                     { added skill events, extended buffer format, remove a post action buffer,
//                       make to larger chunked size message using NPC hearbeat timer,
//                       flexible network buffer to solve overhead - TODO: dual buffer on network module
//            waverix, refactoring, changed by _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
namespace nsGrid {
;
struct HLB_Buffer;
struct HLB_SequentialNode;

}; //end of namespace

//==================================================================================================
//
class HLB_BufferInfo
{
public:
    typedef uint16_t NetMsgSizeType;
    WAR_MSG_HDR* GetHeader() const;
    void AppendTimeSyncMsg(MSG_BASE* const msg, const WORD msg_size);
    void AppendTimeSyncMsgArray(const DWORD number_of_messages,
                                BYTE** const IN message_array, WORD* const message_size_array);
private:
    GameField* game_field_;
    WAR_MSG_HDR* region_header_;
    HLB_SequentialNode* sequential_root_;
    //
    friend class HighLoadBalancer;
};

inline WAR_MSG_HDR* HLB_BufferInfo::GetHeader() const {
    return region_header_;
}


//==================================================================================================
// @name: HighLoadBalancer
//
class HighLoadBalancer
{
private:
    typedef HLB_BufferInfo::NetMsgSizeType NetMsgSizeType;
    typedef STLX_HASH_MAP<ulong, nsGrid::HLB_Buffer*>   MSG_STREAM_BUFFER_INDEXER;
    typedef STLX_DEQUE<nsGrid::HLB_Buffer*>               MSG_STREAM_BUFFER_POOLLIST;
public:
    static const DWORD kMaxRegionStreamSize = (1024 * 256) - 64;
    //static const DWORD kMaxRegionStreamAlarmSize = (1024 * 512) - (1024 * 72);
    //
    static void CreateInstance();
    static void DestroyInstance();
    static HighLoadBalancer* Instance();

public:
    void RegisterBuffer(GameField* const game_field);
    void UnRegisterBuffer(GameField* const game_field);
    void RecyclingBuffer(GameField* const game_field); // need move to private section
    void Update(GameField* const game_field, const BOOLEAN bFlush);

private:
    HighLoadBalancer() {}
    ~HighLoadBalancer();
    //
    MSG_STREAM_BUFFER_INDEXER   msg_buffer_indexer_;
    MSG_STREAM_BUFFER_POOLLIST  msg_buffer_pool_;
    //
    static HighLoadBalancer* instance_static_;
};

inline HighLoadBalancer* HighLoadBalancer::Instance() {
    return instance_static_;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif //GAMESERVER_HIGHLOAD_BALANCER_H