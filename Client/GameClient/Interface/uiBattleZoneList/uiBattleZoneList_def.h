#ifndef     __UIBATTLEZONELIST_DEFINE_H__
#define     __UIBATTLEZONELIST_DEFINE_H__
//------------------------------------------------------------------------------
class cBattleZoneListItem;
#include "StringTableInfo.h"

/// viewType
enum BattleRoomViewType
{
    eCHKVIEW_TOTAL=0,
    eCHKVIEW_POSSIBLE,
    eCHKVIEW_MAX,
};

/// searchType
enum
{
    eCHKSEARCH_ID,
    eCHKSEARCH_ROOMNO,
    eCHKSEARCH_MAX,
};

class uicBattleZoneList_Tab
{
public:
    uicBattleZoneList_Tab()
    { init(); }
    ~uicBattleZoneList_Tab()
    { vTabCodes.clear(); }

    void init()
    {
        int tabCode;

        // No
        tabCode = eST_INDEX;
        vTabCodes.push_back(tabCode);

        // Type
        tabCode = eST_TYPE;
        vTabCodes.push_back(tabCode);

        // Status
        tabCode = eST_TYPE;
        vTabCodes.push_back(tabCode);

        // Title
        tabCode = eST_ROOM_TITLE;
        vTabCodes.push_back(tabCode);

        // People
        tabCode = eST_PERSON_NUMBER;
        vTabCodes.push_back(tabCode);

        // Map name
        tabCode = eST_MAP_NAME;
        vTabCodes.push_back(tabCode);

        dwListTabColor = WzColor_RGBA (255, 162, 38, 255);
        dwListTabColorShadowed = WzColor_RGBA (255, 100, 8, 100);
    }

    std::vector<int>    vTabCodes;
    DWORD               dwListTabColor;
    DWORD               dwListTabColorShadowed;
};

class uicBattleZoneList_Configs
{
public:
    uicBattleZoneList_Configs()
    { clear(); }

    void clear(void)
    {
        viewType = eCHKVIEW_TOTAL;
        searchType = eCHKSEARCH_ID;
        searchNo = 0;
    }

    BattleRoomViewType    viewType;       // 전체/미션맵/헌팅대기/경쟁헌팅/PVP
    WORD    searchType;     // 아이디로 찾기/번호로 찾기
    TCHAR   strSearchID[20];
    SHORT   searchNo;
    
    uicBattleZoneList_Tab   mTab;   // 탭 관련 속성들
};

class BattleRoomEntry;
class BattleEntryTable;

const int   c_iMAX_ROOM = 20;
const int   c_iMAX_ROOMLIST = 100;
const int   c_iCYCLE_REFRESH_ROOM_LIST = 30;
const int   c_iREADY_TO_REFRESH = 2;

//------------------------------------------------------------------------------
#endif  //  __UIBATTLEZONELIST_DEFINE_H__
