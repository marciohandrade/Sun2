#ifndef SUN_GAMESERVER_ACCUMULATEPOINTPARSER_H_
#define SUN_GAMESERVER_ACCUMULATEPOINTPARSER_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_1419_20100121_ACCUMULATE_POINT

/* 클래스 다이어그램 : 2010.03.03 by 남기봉

 GameScriptParserManager <*>--------------> AccumulatePointParser
                                                    A
                                                    |
                                                    |
 Player <*>----------> AccumulatePoint <>-----------|
                               A
                               |
                               |
 PlayerManager <>--------------|
                               |
                               |
 AccumulatePointImplement <m>--|
            A
            |
            |
           <*>        
 CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN or CG_ITEM_USE_ACCUMULATE_POINT_SYN

*/ 

/* class AccumulatePointParser : 2010.03.03 by 남기봉

- 적립 포인트 스크립트(AccumulatePoint.txt) 파서.

- 스크립트에는 요일별 <보상그룹코드, 적립시간, 적립포인트,...>가 있다.

- 현재 요일에 해당하는 보상그룹코드를 랜덤하게 추출한다.

*/


#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include <ScriptCode.IParser.h>


class AccumulatePointParser : public util::Singleton<AccumulatePointParser>, public IParser
{
public:
    struct RewardGroup
    {
        enum
        { 
            Max_Reward = 5,
        };

        struct Reward
        {
            uint    reward_code;
            char    reward_ratio;
        };

#ifdef _NA_0_20100528_ACCUMULATE_POINT_EVENT
        struct RewardEvent
        {
            uint    start_time;
            uint    end_time;
            uint    reward_code;
        };

        RewardEvent reward_event;
#endif

#ifdef _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION
        struct LimitOption
        {
            uchar   receptible_min_level;
            uchar   receptible_max_level;
            uint    limit_max_point;
        };

        LimitOption limit_option;
#endif

        uchar       day_flag;                   // 일, 월, 화...
        uchar       require_point;              // 보상 받기 위해 필요한 포인트
        uchar       generate_point;             // 발생 포인트
        uint        generate_time;              // 발생 시간
        Reward      reward_array[Max_Reward];
    };

    enum DayFlag
    {
        Min_Day = 0,
        Max_Day = 7,
    };

public:
    AccumulatePointParser(void);
	virtual	~AccumulatePointParser(void);

    VOID Init();
	VOID Init( char * pszPackFileName );
	VOID Release();

	virtual	BOOL LoadScript( eSCRIPT_CODE scriptCode, BOOL bReload );

    const RewardGroup& get_reward_group(ushort dayofweek) const;


private:
    BOOL _Load( BOOL bReload );

    RewardGroup     reward_group_array_[Max_Day];
};


inline const AccumulatePointParser::RewardGroup& 
AccumulatePointParser::get_reward_group(ushort dayofweek) const
{
    return (dayofweek < Max_Day) ? reward_group_array_[dayofweek] : reward_group_array_[Min_Day];
}

#endif //_NA_1419_20100121_ACCUMULATE_POINT

#endif //SUN_GAMESERVER_ACCUMULATEPOINTPARSER_H_
