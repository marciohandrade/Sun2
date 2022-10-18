#ifndef     __UIBATTLEZONECREATE_DEFINE_H__
#define     __UIBATTLEZONECREATE_DEFINE_H__
//------------------------------------------------------------------------------

enum
{
    eCHKMAP_CODE_MISSION = 0,
    eCHKMAP_CODE_HUNTING = 1000,
    eCHKMAP_CODE_PVP = 20000,
    eCHKMAP_CODE_CHUNTING = 40000,
    eCHKMAP_CODE_MAX,
};

/// mapType
enum
{
    eCHKMAP_FOREST=0,
    eCHKMAP_UNDERWAY,
    eCHKMAP_FROZENPALACE,
    eCHKMAP_VALLYOFDRAGON,
	eCHKMAP_PUBLIC_CEMETERY,
	eCHKMAP_HELLON_TOWER,
	eCHKMAP_TUTORIAL,
    eCHKMAP_MAX,
    eCHKMAP_THE_EARTH_OF_TREES_SHADOW=eCHKMAP_MAX,
    eCHKMAP_REST_AREA_OF_SPIRITS,
    eCHKMAP_BOALESS_MOUNTAIN_CHAIN,
    eCHKMAP_VALLY_OF_LIFE,
    eCHKMAP_EP2_MAX,
};



/// roomType
enum eCREATE_BATTLE_TYPE
{
#ifdef _CN_EXCECPT_MISSION
	//no operation
#else
    eCREATE_BATTLE_MISSION_TYPE = 0,
#endif//_CN_EXCECPT_MISSION
    eCREATE_BATTLE_PVP_TYPE,
#ifndef _EXCECPT_HUNTING_001011
    eCREATE_BATTLE_CHUNTING,
#endif
    eCREATE_BATTLE_MAX
};

class uicBattleZoneCreate_Configs
{
public:
    uicBattleZoneCreate_Configs()
    { clear(); }
    ~uicBattleZoneCreate_Configs()
    {
        vRoomCode.clear();
        vRoomDescCode.clear();
    }

    void clear()
    {
        ZeroMemory (&strTitle, MAX_ROOMTITLE_LENGTH-1);
        ZeroMemory (&strPassword, MAX_ROOMPASSWORD_LENGTH-1);
#ifdef _CN_EXCECPT_MISSION
		roomType = eCREATE_BATTLE_PVP_TYPE;
#else
        roomType = eCREATE_BATTLE_MISSION_TYPE;
#endif//_CN_EXCECPT_MISSION
		
		//주의:vRoomCode와 vRoomDescCode 순서는 항상 맞춰야합니다.
	
#ifdef _CN_EXCECPT_MISSION
		//no operation
#else
        vRoomCode.push_back(640);   // 미션
#endif//_CN_EXCECPT_MISSION(미션선택제거)
        vRoomCode.push_back(685);   // PVP
#ifndef _EXCECPT_HUNTING_001011
        vRoomCode.push_back(604);   // 경쟁 헌팅
#endif

#ifdef _CN_EXCECPT_MISSION
		//no operation
#else
		vRoomDescCode.push_back(594);   // 미션 설명
#endif//_CN_EXCECPT_MISSION(미션선택제거)
        vRoomDescCode.push_back(597);   // PVP 설명
#ifndef _EXCECPT_HUNTING_001011
		vRoomDescCode.push_back(596);   // 경쟁 헌팅 설명 
#endif
        minLevel = 1;
        fix_minLevel = minLevel;
        maxLevel = 100;
        fix_maxLevel = maxLevel;
        peopleType = 9;
        fix_peopleType = peopleType;
        mapType = eCHKMAP_FOREST;
        classLimit = eCLASS_ALL;
    }

    TCHAR           strTitle[MAX_ROOMTITLE_LENGTH];
    TCHAR           strPassword[MAX_ROOMPASSWORD_LENGTH];
    WORD            roomType;
    WORD            peopleType;
    WORD            fix_peopleType;
    std::vector<int>    vRoomCode;
    std::vector<int>    vRoomDescCode;
    WORD            minLevel;
    WORD            fix_minLevel;
    WORD            maxLevel;
    WORD            fix_maxLevel;
    WORD            mapType;
    
    eCLASS_TYPE     classLimit;
    MAPCODE         mapCode;
};

enum eMAP_IDENTY_VALUE
{
    eMAP_IDENTY_FOREST = 0,		// 야수의 숲
    eMAP_IDENTY_UNDERWAY,		// 지하수로
    eMAP_IDENTY_FROZENPALACE,	// 설원
    eMAP_IDENTY_HILLOFDRAGON,	// 화산
    eMAP_IDENTY_MAX,
    // 아직 추가 되지 않은 상태.
    eMAP_IDENTY_GRAVEYARD,		// 공동묘지
    eMAP_IDENTY_TOWEROFREGEND,	// 헬론의 성
};

struct stCHuntingMap
{
    MAPCODE	MapCode;
    int		iRequireMinLV;
};

extern const int    c_iZONE_CREATE_ROOMTYPES[];
extern const int    c_iZONE_CREATE_ROOMTYPES_DESCS[];

const int c_iDefaultRoomNameStartIndex_Mission = 1941;
const int c_iDefaultRoomNameStartIndex_CHunting = 1961;
const int c_iDefaultRoomNameStartIndex_PVP = 1981;
const int c_iDefaultRoomNameNumMax = 19;

//------------------------------------------------------------------------------
#endif  //  __UIBATTLEZONECREATE_DEFINE_H__
