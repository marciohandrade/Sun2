#ifndef _HANAUTH_STRUCT_H
#define _HANAUTH_STRUCT_H

//reference : "__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__"

#ifndef _HANAUTH_QUERY_MANAGER_H
    #error "Can't direct include"
#endif

namespace nsHanAuth {
#pragma pack(push, 8)

struct Interfaces
{
    enum eInterfaces {
        eInterfaces_Close         = 0,
        eInterfaces_Sync          ,
        eInterfaces_HanAuthForSvr ,
        eInterfaces_HanAuthForSvrResult ,
        eInterfaces_Counts        , // end of interfaces
    };
};

typedef char GAME_ID_BLOCK[LIMIT_GAMEID];
typedef char MEMBER_ID_BLOCK[LIMIT_MEMBERID];
typedef char AUTH_STRING_BLOCK[SIZE_AUTHSTRING];
typedef char RESULT_BLOCK[SIZE_OUTBUFFER];

typedef DWORD INDEX_ID_T; // query key index
const INDEX_ID_T INVALID_INDEX_ID = 0;


struct Message_ChunkBlock;
struct Message_Close;
struct Message_Sync;
struct Message_Authentication;
struct Message_AuthenticationResult;

#pragma pack(push, 1)
struct Message
{
    WORD length_;
    WORD interface_type_;
    INDEX_ID_T query_index_; 

    static const DWORD MaxMessageSize;
};
#pragma pack(pop)

struct __declspec(align(4)) Message_ChunkBlock : Message
{
    BYTE stream[sizeof(GAME_ID_BLOCK) + sizeof(MEMBER_ID_BLOCK) +
                sizeof(AUTH_STRING_BLOCK) + sizeof(RESULT_BLOCK)];
};

struct __declspec(align(4)) Message_Close : Message
{
    static const WORD MessageID = Interfaces::eInterfaces_Close;
};

struct __declspec(align(4)) Message_Sync : Message
{
    static const WORD MessageID = Interfaces::eInterfaces_Sync;
    DWORD check_start_tick_;
    DWORD check_reply_tick_;
};

struct __declspec(align(4)) Message_Authentication : Message
{
    static const WORD MessageID = Interfaces::eInterfaces_HanAuthForSvr;
    MEMBER_ID_BLOCK member_id_;
    AUTH_STRING_BLOCK auth_string_;
    // no client ip check - inter process
};

struct __declspec(align(4)) Message_AuthenticationResult : Message
{
    static const WORD MessageID = Interfaces::eInterfaces_HanAuthForSvrResult;
    int result_code_;
    // 성공일 때만 얻을 수 있다...
    char user_id_[ID_MAX_LEN + 1];
};

#pragma pack(pop)
}; //end of namespace 'nsHanAuth'

#endif //_HANAUTH_STRUCT_H

