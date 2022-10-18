#pragma once
#include "RadarInfoParser.h"

class Object;
class Player;
class NPC;
class Monster;
class MapObject;

typedef	STLX_DEQUE<RADAR_POINT*> RADAR_POINT_DEQUE;
typedef	STLX_DEQUE<RADAR_POINT*>::iterator RADAR_POINT_DEQUE_ITR;

class uiRadarPointCollecter
{
    struct QuestNaviInfo
    {
        eRADAR_POINT_COLOR navi_point_color;
        float navi_position_x;
        float navi_position_y;
        DWORD navi_object_key;
        STLX_STRING navi_quest_name;

        void Clear()
        {
            navi_point_color = eRADAR_POINT_COUNT;
            navi_position_x = 0.0f;
            navi_position_y = 0.0f;
            navi_object_key = 0;
            navi_quest_name.clear();
        }
    };

    enum
    {
        kMaxNaviCount = 4,
    };

public:
    uiRadarPointCollecter() {}
    ~uiRadarPointCollecter() {}

    void Initialize();

    /// 레이더 포인터 수집
    void CollectRadarPoint();

    RADAR_POINT_DEQUE& radar_point_container() { return radar_point_container_; }

private:
    void ClearAll();

    RADAR_POINT* NewRadarPoint();

    void CollectQuestNaviPoint();
    void CollectPartyNaviPoint();
    void CollectTargetObjectPoint();
    void CollectQuestNaviInfo(RADAR_POINT* radar_point, Object* object_ptr);    

    WzVector CalculatePointPosition(WzVector object_position);
    void InsertRadarPoint(RADAR_POINT* radar_point);
    void MergeCollectPoint();

    eRADAR_POINT_COLOR GetPointColorPlayer(Player* player_ptr);
    eRADAR_POINT_COLOR GetPointColorNpc(NPC* npc_ptr);
    eRADAR_POINT_COLOR GetPointColorMonster(Monster* monster_ptr);
    eRADAR_POINT_COLOR GetPointColorMapObject(MapObject* map_object_ptr);

private:
    util::CMemoryPoolFactory<RADAR_POINT> radar_point_memorypool;

    RADAR_POINT_DEQUE radar_point_container_;
    RADAR_POINT_DEQUE npc_quest_accept_point_container_; //느낌표 위치
    RADAR_POINT_DEQUE npc_quest_reward_point_container_; //물음표 위치

    QuestNaviInfo quest_navi_info_[kMaxNaviCount];
};