#ifndef _AUTH_INTERNAL_HXX_
#define _AUTH_INTERNAL_HXX_


//#define _GSP00000_091123_SPECIAL_OPEN_MODE_CONTROL_     //글로벌 임시 모드 제어(외부 오픈에서 유럽만 사내 접속 가능)

//#define _DEBUG_SHOW_THE_PACKET_FLOW_
#if defined(_DEBUG_SHOW_THE_PACKET_FLOW_)

#define SHOW_AUTH_FUNCTION_FLOW()   \
    AuthPacketFlowAnalysis::TracingFlow(__FILE__, __FUNCTION__, __LINE__);
#define SHOW_AUTH_PACKET_FLOW_TRACE(msg, size)  \
    AuthPacketFlowAnalysis::ParsePacket(msg, size)
#define SHOW_AUTH_INTER_PACKET_FLOW_TRACE(msg, size)    \
    AuthPacketFlowAnalysis::ParsePacket(msg, size)

struct MSG_BASE;
struct MSG_BASE_INTERNAL;
struct MSG_BASE_BILLING;
struct AuthPacketFlowAnalysis {
    static void Instance();
    static void TracingFlow(const char* file, const char* function, const size_t line);
    static void ParsePacket(MSG_BASE* msg, WORD size);
    static void ParsePacket(MSG_BASE_INTERNAL* msg, WORD size);
    static void ParsePacket(MSG_BASE_BILLING* msg, WORD size);
};

#else //!_DEBUG_SHOW_THE_PACKET_FLOW_

#define SHOW_AUTH_FUNCTION_FLOW()
#define SHOW_AUTH_PACKET_FLOW_TRACE(msg, size)
#define SHOW_AUTH_INTER_PACKET_FLOW_TRACE(msg, size)

#endif //!_DEBUG_SHOW_THE_PACKET_FLOW_

#define WAVERIX_AUTH_TEST_CODE       (0)
#define WAVERIX_AUTH_TEST_LOG        (0)
#define AUTH_TEST_CODE (0)

//_AUTH_NORTHA_TEST_MODE 0:DevTest 1:Dev(ID/PWD) 2:grater equal than 'Alpha'
#define _AUTH_NORTHA_TEST_MODE__DEVTEST__   0
#define _AUTH_NORTHA_TEST_MODE__DEVIDPWD__  1
#define _AUTH_NORTHA_TEST_MODE__NOTDEV__    2

#define _AUTH_NORTHA_TEST_MODE  _AUTH_NORTHA_TEST_MODE__NOTDEV__

#if WAVERIX_AUTH_TEST_LOG
    #define NORTHA_DEBUGLOG     AUTH_DEBUGLOG
#else
    #define NORTHA_DEBUGLOG(msg, arg)
#endif

#if WAVERIX_AUTH_TEST_LOG
    #define GLOBAL_DEBUGLOG     AUTH_DEBUGLOG
#else
    #define GLOBAL_DEBUGLOG(msg, arg)
#endif

#if WAVERIX_AUTH_TEST_LOG
    #define AUTH_DEBUGLOG   printf("[DEBUG] "); printf
#else
    #define AUTH_DEBUGLOG(msg, arg)
#endif

#define _BILLING_CODE_REFACTORING_  // 090818, Billing관련 코드 정리

#endif //_AUTH_INTERNAL_HXX_


