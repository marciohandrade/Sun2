#ifndef SUN_GAMESERVER_ACCUMULATEPOINTPARSER_H_
#define SUN_GAMESERVER_ACCUMULATEPOINTPARSER_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_1419_20100121_ACCUMULATE_POINT

/* Ŭ���� ���̾�׷� : 2010.03.03 by �����

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

/* class AccumulatePointParser : 2010.03.03 by �����

- ���� ����Ʈ ��ũ��Ʈ(AccumulatePoint.txt) �ļ�.

- ��ũ��Ʈ���� ���Ϻ� <����׷��ڵ�, �����ð�, ��������Ʈ,...>�� �ִ�.

- ���� ���Ͽ� �ش��ϴ� ����׷��ڵ带 �����ϰ� �����Ѵ�.

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

        uchar       day_flag;                   // ��, ��, ȭ...
        uchar       require_point;              // ���� �ޱ� ���� �ʿ��� ����Ʈ
        uchar       generate_point;             // �߻� ����Ʈ
        uint        generate_time;              // �߻� �ð�
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
