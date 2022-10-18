#pragma once
#ifndef COMMON_TOWER_OF_TRIAL_TIME_PARSER_H
#define COMMON_TOWER_OF_TRIAL_TIME_PARSER_H

#include <ITimeout.h>
#include <SolarDateTime.h>
#include <Singleton.h>

#if __ITIMERBASE_VERSION_CRTL >= 0x0120
//==================================================================================================

namespace ns_func {
;

struct eBattleTime
{
    enum eTimeMode {
        eNone = 0,
        eCountdown_5Minutes,
        eCountdown_1Minutes,
        eOpened,
        eClosingCountdown_3Minutes,
        eClosingCountdown_1Minutes,
        eClosed,
        //
        eState_Counts
    };
    typedef uint8_t mode_t;
    typedef util::TimeInfoPerFrame::AcmTmFmt  AcmTmFmt;
    //
    mode_t time_mode_;
};

struct BattleTimeNode : public eBattleTime
{
    bool check_event_; // to support user(agent server) check field
    uint16_t index_;
    AcmTmFmt start_tm_, end_tm_;
    ulong string_code_;
    bool IsInRange(AcmTmFmt query_time) const;
};

inline bool BattleTimeNode::IsInRange(AcmTmFmt query_time) const
{
    if (start_tm_ <= query_time && query_time < end_tm_) {
        return true;
    }
    return false;
}

}; //end of namespace

//==================================================================================================

class CurseTowerTimeParser : public util::Singleton<CurseTowerTimeParser>, public IParser
{
public:
    typedef ns_func::BattleTimeNode BattleTimeNode;
    //
    enum eIndexType {
        eIndex_None = 0,
        eIndex_CurseTower,
        eIndex_Counts
    };
    static const int kNumberOfEventsPerNode = 6;
    static const BattleTimeNode::AcmTmFmt k24hours = 24 * 60; // 24h
    static const BattleTimeNode::AcmTmFmt kLowerbound = 10; // 00h 10m
    typedef STLX_VECTOR<const BattleTimeNode*> FirstEventList;
    //
    CurseTowerTimeParser();
    virtual ~CurseTowerTimeParser();
    //
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reload);
    //
    bool is_loaded() const;
    const FirstEventList& GetFirstEventList() const;
    bool IsExistStartTimeNode(const util::TimeInfoPerFrame& time_info,
                              BattleTimeNode* result_node) const;
    bool GetCurrentTimeNode(const util::TimeInfoPerFrame& time_info, uint16_t index,
                            BattleTimeNode* result_node) const;

private:
    //                     index
    typedef STLX_MULTIMAP<uint16_t, const BattleTimeNode*> OpenTimeTable;
    //
    bool _Load(bool reload);
    void SetTimeInfo(BattleTimeNode* time_node,
                     const BattleTimeNode::AcmTmFmt open_time,
                     const BattleTimeNode::mode_t time_mode,
                     const DWORD string_code);
    //
    bool loaded_;
    int number_of_time_nodes_;
    BattleTimeNode* time_node_array_;
    OpenTimeTable open_time_table_;
    FirstEventList first_event_list_;
    //
    TCHAR file_name_[MAX_PATH]; // for debug
};

inline bool CurseTowerTimeParser::is_loaded() const
{
    return loaded_;
}

inline const CurseTowerTimeParser::FirstEventList&
CurseTowerTimeParser::GetFirstEventList() const
{
    return first_event_list_;
}

//==================================================================================================
#else // __ITIMERBASE_VERSION_CRTL < 0x0120
//==================================================================================================

#include <SolarHashTable.h>
#include <hash_map>

//////////////////////////////////////////<st_BattleOpenTimeNode>////////////////////////////////////////////
struct st_BattleOpenTimeNode
{
	enum
	{
		STATE_BEFORE_OPEN_5MIN,
		STATE_BEFORE_OPEN_1MIN,
		STATE_OPEN,
		STATE_BEFORE_CLOSE_3MIN,
		STATE_BEFORE_CLOSE_1MIN,
		STATE_CLOSE,
	};
    WORD		wIndexCode;
	WORD		OpenState;	

	DWORD		dwBeforeOpen5MinTick;
	DWORD		dwBeforeOpen1MinTick;
	DWORD		dwOpenTick;
	DWORD		dwBeforeClose3MinTick;
	DWORD		dwBeforeClose1MinTick;
	DWORD		dwCloseTick;

	DWORD		dwStrCodeBeforeOpen5Min;
	DWORD		dwStrCodeBeforeOpen1Min;
	DWORD		dwStrCodeOpen;
	DWORD		dwStrCodeBeforeClose3Min;
	DWORD		dwStrCodeBeforeClose1Min;
	DWORD		dwStrCodeClose;

	st_BattleOpenTimeNode();
	VOID		CalTick(BYTE Hour, BYTE Min);
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



typedef STLX_LIST<st_BattleOpenTimeNode*>						BATTLE_OPEN_TIME_LIST;
////////////////////////////////////<st_BattleOpenTimeList>/////////////////////////////////////////////
struct st_BattleOpenTimeList
{
	BATTLE_OPEN_TIME_LIST	m_List;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////<CurseTowerTimeParser>/////////////////////////////////////////////
typedef STLX_HASH_MAP<WORD, st_BattleOpenTimeList*>			BATTLE_OPEN_TIME_LIST_HASH;

class CurseTowerTimeParser : public util::Singleton<CurseTowerTimeParser>, public IParser
{
public:
	CurseTowerTimeParser();
	~CurseTowerTimeParser();
	enum
	{
		TIME_CHECK_NO_USE		= 0,
		TIME_CHECK_CURSE_TOWER	= 1,
	};

	VOID					Release();

	virtual BOOL			LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	BATTLE_OPEN_TIME_LIST_HASH* GetListHash(){ return &m_BattleOpenTimeHash; }
	BATTLE_OPEN_TIME_LIST*		GetRegisterList(){ return &m_RegisterNodeList; }

private:
	BOOL					_Load(BOOL bReload);
	TCHAR											m_pszFileName[MAX_PATH];
	BATTLE_OPEN_TIME_LIST_HASH						m_BattleOpenTimeHash;
	BATTLE_OPEN_TIME_LIST							m_RegisterNodeList;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//==================================================================================================
#endif //__ITIMERBASE_VERSION_CRTL

#endif //COMMON_TOWER_OF_TRIAL_TIME_PARSER_H