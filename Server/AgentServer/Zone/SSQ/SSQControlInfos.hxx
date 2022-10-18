#pragma once

#include <SSQ/SSQInfoParser.h>        // ProgramCommon
#include <SSQ/SSQPacketProc.h>        // ProgramCommon
#include <SSQ/SSQPacketProcS2S.h>    // Common

class SSQUpdateCtrl;
class SSQMsgRouterInAgent;

namespace nsSSQ {};
namespace nsSSQ_Util {};
namespace SSQControlInfo {};

using namespace nsSSQ;
using namespace nsSSQ_Util;
using namespace SSQControlInfo;

//==================================================================================================
//
namespace SSQControlInfo
{
    typedef SSQInfo::WzTmFmt    WzTmFmt;    //  h * 100 + m
    typedef WORD                AcmTmFmt;   //  h *  60 + m
    typedef SSQInfo::SSQCODE    SSQCODE;
    enum eTIME_TYPE
    {
        TT_NONE, TT_NOTICE, TT_WAITING, TT_BATTLE, TT_CLEAR, TT_MAX, //(TT_CLEAR + 1) % TT_MAX
    };

    struct sTIME_INFO
    {
        DWORD      CurTick;
        WzTmFmt    WzTime;        //  Webzen Time Format
        AcmTmFmt   AcmTime;    //  Accumulated Time Format
        SYSTEMTIME SysTime;

        inline static VOID  SetTimeValue(sTIME_INFO& OUT timeInfo);
    };

    struct SSQ_UPDATER_NODE
    {
        SSQCODE        CodeOfSSQ;
        KEYTYPE        KEY;
        SSQUpdateCtrl* pUpdater;

        SSQ_UPDATER_NODE() : CodeOfSSQ(0), KEY(0), pUpdater(0) {}
        SSQ_UPDATER_NODE(const SSQCODE codeOfSSQ, SSQUpdateCtrl* const ptr)
            : CodeOfSSQ(codeOfSSQ), KEY(0), pUpdater(ptr) {}

        inline const SSQCODE GetCodeOfSSQ() const { return CodeOfSSQ; }
        inline const KEYTYPE GetKey() const { return KEY; }
        inline VOID          SetKey(const KEYTYPE key) { KEY = key; }
        inline SSQUpdateCtrl* const GetUpdater() const { return pUpdater; }
    };

    struct OpSSQInfoController;
    typedef STLX_VECTOR<SSQ_UPDATER_NODE>   SSQ_UPDATERS;
    typedef SSQ_UPDATERS::iterator          SSQ_UPDATERS_IT;

    class SSQSingletonNotice
    {
    public:
        enum eNotice_Steps
        {
            eNotice_Steps_None     = 0,
            eNotice_OpenCountdown  = 1 <<  0,
            eNotice_Open           = 1 <<  6,
            eNotice_CloseCountdown = 1 <<  7,
            eNotice_ClosePortal    = 1 << 10,
            eNotice_Close          = 1 << 11,
        };

        SSQSingletonNotice()
            : m_bTransaction(FALSE)
            , m_ProcIndex(SSQInfo::MI_ENTER_TIME_SECTION_MAX)
            , m_EndTransactionTime(-1)
            , m_Notice_Steps(eNotice_Steps_None)
            , m_Argument0(ULONG_MAX)
        {}
        ~SSQSingletonNotice() {}

        inline VOID EnterSingletonNotice(const SSQInfo* const pSSQInfo, const DWORD finded_index);
        inline VOID Update(const sTIME_INFO& tmInfo);
        VOID Broadcast_Notice(SSQUpdateCtrl* const pUpdater,
                              const eNotice_Steps notice_type, const DWORD arg0 = 0);
    private:
        BOOLEAN          m_bTransaction;
        BYTE             m_ProcIndex;
        SSQInfo::WzTmFmt m_EndTransactionTime;
        eNotice_Steps    m_Notice_Steps;
        DWORD            m_Argument0;
    };
};


//==================================================================================================


template< eTIME_TYPE _TM_ID, typename _TM_TYPE, DWORD _INTERVAL >
struct SSQUpdateHandler_info : sun_type_info< _TM_TYPE >
{
    static const eTIME_TYPE value = _TM_ID;
    static const DWORD interval = _INTERVAL;
};

typedef AutoTransactionVTLS<SSQ_UPDATER_NODE>   AutoTransactionCtrlNode;


//==================================================================================================


namespace SSQControlInfo
{

inline VOID
sTIME_INFO::SetTimeValue(sTIME_INFO& OUT timeInfo)
{
    timeInfo.CurTick = GetTickCount();
    util::TimeSync::GetLocalTime(&timeInfo.SysTime);
    timeInfo.WzTime = timeInfo.SysTime.wHour * 100 + timeInfo.SysTime.wMinute;
    timeInfo.AcmTime = timeInfo.SysTime.wHour * 60 + timeInfo.SysTime.wMinute;
}


inline VOID
SSQSingletonNotice::EnterSingletonNotice(const SSQInfo* const pSSQInfo, const DWORD finded_index)
{
    if(m_bTransaction)
        return;

    m_bTransaction = TRUE;
    m_EndTransactionTime = pSSQInfo->m_CTimeBattle[finded_index].End;
}

inline VOID
SSQSingletonNotice::Update(const sTIME_INFO& tmInfo)
{
    if(tmInfo.WzTime > m_EndTransactionTime)
        new (this) SSQSingletonNotice;
}

};    // end namespace SSQControlInfo
