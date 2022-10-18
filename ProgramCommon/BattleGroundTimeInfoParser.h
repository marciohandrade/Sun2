#ifndef PROGRAMCOMMON_BATTLEGROUNDTIMEINFOPARSER_H_
#define PROGRAMCOMMON_BATTLEGROUNDTIMEINFOPARSER_H_


#if _MSC_VER > 1000
#pragma once
#endif

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

namespace ns_func {

struct BattleGroundTime
{
    enum TimeMode
    {
        kNone = 0,
        kCountdown_5Minutes,        // 전장 시작 5분전
        kOpened,                    // 전장 open
        kClosingCountdown_5Minutes, // 전장 종료 5분전
        kClosed,                    // 전장 종료
    };

    typedef util::TimeInfoPerFrame::AcmTmFmt  AcmTmFmt;
    TimeMode time_mode;
};

struct BattleGroundTimeNode : public BattleGroundTime
{
    bool check_event;
    uint16_t day_flag;
    AcmTmFmt start_time;
    AcmTmFmt end_time;
    ulong string_code;
    bool IsInRange(AcmTmFmt query_time) const;
};

inline bool BattleGroundTimeNode::IsInRange(AcmTmFmt query_time) const
{
    if (start_time <= query_time && query_time < end_time) {
        return true;
    }
    return false;
}

}; // end of namespace


class BattleGroundTimeInfoParser : public util::Singleton<BattleGroundTimeInfoParser>, public IParser
{
public:
    enum DayFlag
    {
        kMinDay = 0,
        kMaxDay = 7,
    };

    enum 
    { 
        kOpenTimeMaxCount = 5, 
        kEventTimeModeCounts = 4,
        k24hours = 24 * 60,
    };

    typedef ns_func::BattleGroundTimeNode BattleGroundTimeNode;
    typedef STLX_VECTOR<const BattleGroundTimeNode*> FirstEventList;

    BattleGroundTimeInfoParser() :
    number_of_time_nodes_(0),
    time_node_array_(NULL)
    {
    }
    ~BattleGroundTimeInfoParser()
    {
    }

    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload);

    bool IsExistStartTimeNode(const util::TimeInfoPerFrame& time_info,
        BattleGroundTimeNode* result_node) const;

    bool GetCurrentTimeNode(const util::TimeInfoPerFrame& time_info, 
        uint16_t day_flag,
        ns_func::BattleGroundTimeNode* result_node) const;

    inline const FirstEventList& GetFirstEventList() const{ return first_event_list_; }

private:
    void Unload();
    bool _Load(bool reload);
    void _SetTimeInfo(BattleGroundTimeNode* time_node,
        const BattleGroundTimeNode::AcmTmFmt input_open_time,
        const BattleGroundTimeNode::AcmTmFmt input_close_time,
        const BattleGroundTimeNode::TimeMode time_mode,
        const DWORD string_code);

private:
    typedef STLX_MULTIMAP<uint16_t, const BattleGroundTimeNode*> OpenTimeTable;
    int number_of_time_nodes_;
    BattleGroundTimeNode* time_node_array_;
    OpenTimeTable open_time_table_;
    FirstEventList first_event_list_;
};
#endif //PROGRAMCOMMON_BATTLEGROUNDTIMEINFOPARSER_H_
