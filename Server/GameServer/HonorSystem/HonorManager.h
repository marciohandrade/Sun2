#ifndef SUN_GAMESERVER_HONORSYSTEM_HONORMANAGER_H_
#define SUN_GAMESERVER_HONORSYSTEM_HONORMANAGER_H_

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "TitleListParser.h"

class Player;

class HonorManager
{
public:
    HonorManager(void);
    virtual ~HonorManager(void);

    bool init(Player* player);

    //////////////////////////////////////////////////////////////////////////
    // title
    WORD GetHonorTitle();
    bool SetHonorTitle(WORD honor_title);

    void RequestAddHonorTitle( WORD honor_title );
    bool AddHonorTitle(WORD honor_title, bool packet_send = true);

    void RequestRemoveHonorTitle( WORD honor_title );
    bool RemoveHonorTitle(WORD honor_title, bool packet_send = true);

    void SerializeTitles(HonorTitleTable& titles, eSERIALIZE serialize_type);

    //////////////////////////////////////////////////////////////////////////
    // Fame / Repute
    bool SetFameRepute(BYTE type, WORD code, DWORD point, bool packet_send = true);
    int  GetFameReputePoint(BYTE type, WORD code);
    int  GetFameReputeGrade(BYTE type, WORD code);
    int  GetFameReputeGradeCount(BYTE type, WORD grade);

    void SerializeFameRepute( HonorPointTable& points, eSERIALIZE serialize_type );

    void AddFamePoint(WORD code, WORD point);
    void AddReputePoint(WORD code, WORD point);

    void DailyQuestReset();
    void SendDailyQuestResetUseTime();

    //////////////////////////////////////////////////////////////////////////
    //
    void GMTodayQuestReset();
    void GMDailyQuestClear();

private:
    Player*             player_;
    BASE_PLAYERINFO*    playerinfo_;

    typedef STLX_HASH_MAP<WORD, TitleListParser::TitleInfo*> HonorTitleMap;
    HonorTitleMap honor_titles_;

public:
    class HonorPointsKey
    {
        union
        {
            struct 
            {
                BYTE temp;
                BYTE type_;
                WORD code_;
            };
            DWORD index_;
        };

    public:
        HonorPointsKey(BYTE type, WORD code)
            : temp(0)
        {
            type_ = type, code_ = code;
        }

        bool operator<(const HonorPointsKey& _KeyValue) const
        {
            return (index_ < _KeyValue.index_);
        }
    };

    struct HONOR_POINT_VALUE
    {
        HONOR_POINT_INFO point_info;
        WORD grade;
    };

private:
    typedef STLX_MAP<HonorPointsKey, HONOR_POINT_VALUE> HonorPointsMap;
    HonorPointsMap honor_points_;
};
#endif //_NA_003027_20111013_HONOR_SYSTEM
#endif //SUN_GAMESERVER_HONORSYSTEM_HONORMANAGER_H_