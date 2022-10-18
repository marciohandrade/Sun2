
//---------------------------------------------------------------------------------------
//
//		2005년 11월 23일  임대진
//
//		*. 레이더 다이얼로그에 사용되는 택스쳐 이미지가 한 장으로 교체됨에 따라
//			각 부분별로 UV 용 좌표 정보가 필요합니다.
//
//		*. 각 좌표들을 택스트 스크립트로 처리하여 수정할 경우 이미지 파일과
//			택스트 스크립트 파일만 수정하면 되게 작업
//
//---------------------------------------------------------------------------------------



#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

using namespace std;


#define		ARROW_DISTANCE			10.0f
#define		ARROW_SCALE				0.10f


enum eRADAR_POINT_COLOR
{
	eRADAR_NOT_FOUND_OBJECT = -1,
	eRADAR_PC_POINT=0,
	eRADAR_PARTY_POINT,
	eRADAR_NPC_POINT,
	eRADAR_MONSTER_POINT,
	eRADAR_HERO_ARROW,
	eRADAR_MONSTER_DEAD_POINT,
	eRADAR_PARTY_POINT_01,
	eRADAR_PARTY_POINT_02,
	eRADAR_PARTY_POINT_03,
	eRADAR_PARTY_POINT_04,
	eRADAR_PARTY_POINT_05,
	eRADAR_PARTY_POINT_06,
	eRADAR_PARTY_POINT_07,
	eRADAR_PARTY_POINT_08,
	eRADAR_PARTY_POINT_09,
	eRADAR_PARTY_POINT_10,

	eRADAR_CLICK_POINT_01,
	eRADAR_CLICK_POINT_02,
	eRADAR_CLICK_POINT_03,
	eRADAR_CLICK_POINT_04,
	eRADAR_CLICK_POINT_05,
	eRADAR_CLICK_POINT_06,
	eRADAR_CLICK_POINT_07,
	eRADAR_CLICK_POINT_08,
	eRADAR_CLICK_POINT_09,
	eRADAR_CLICK_POINT_10,
	eRADAR_CLICK_POINT_11,
	eRADAR_NORMAL_QUEST_ACCEPT,// 일반 퀘스트수락(노랑)
	eRADAR_REPEAT_QUEST_REWARD,// 반복 퀘스트완료(녹색)

	eRADAR_TIMEATTACK,

	eRADAR_NPC_START = 30 , 
	eRADAR_NPC_END	= 49,
	eRADAR_SAME_PARTY_MEMBER = 50 ,

	eRADAR_TARGET_PLAYER = 51,

	eRADAR_NAVI_POINT_YELLOW  = 52,
	eRADAR_NAVI_POINT_RED     = 53,
	eRADAR_NAVI_POINT_GREEN   = 54,
	eRADAR_NAVI_POINT_BLUE    = 55,
	eRADAR_NAVI_POINT_PERPLE  = 56,

	eRADAR_NAVI_ARROW_YELLOW = 57,
	eRADAR_NAVI_ARROW_RED    = 58,
	eRADAR_NAVI_ARROW_GREEN  = 59,
	eRADAR_NAVI_ARROW_BLUE   = 60,
	eRADAR_NAVI_ARROW_PERPLE = 61,

	eRADAR_CONTINENT_MAP_HERO_POS = 62,

	eRADAR_NORMAL_PORTAL    = 63,
	eRADAR_MISSION_PORTAL   = 64,
	eRADAR_WAY_POINT		= 65,

	eRADAR_DEACTIVE_QUEST_NPC = 66,
	eRADAR_REPEAT_QUEST_ACCEPT = 67,//반복 퀘스트수락(녹색)
	eRADAR_CRYSTAL_WARP = 68,
	eRADAR_BLAST		= 69,
	eRADAR_AIRSHIP		= 70,

    // _YMS_ADD_RADAR_ICON_100622
    //71	0	576	32	32	//타겟팅 에니메이션_대_01
    //72	32	576	32	32	//타겟팅 에니메이션_대_02
    //73	64	576	32	32	//타겟팅 에니메이션_대_03
    //74	96	576	32	32	//타겟팅 에니메이션_대_04
    //75	128	576	32	32	//타겟팅 에니메이션_대_05
    //76	160	576	32	32	//타겟팅 에니메이션_대_06
    //77	192	576	32	32	//타겟팅 에니메이션_소_01
    //78	224	576	32	32	//타겟팅 에니메이션_소_02
    //79	256	576	32	32	//타겟팅 에니메이션_소_03
    //80	288	576	32	32	//타겟팅 에니메이션_소_04
    //81	320	576	32	32	//타겟팅 에니메이션_소_05
    //82	352	576	32	32	//타겟팅 에니메이션_소_06
    //83	96	608	32	32	//몬스터_소
    //84	128	608	32	32	//몬스터_대/경쟁헌팅 적군/카오
    //85	192	608	32	32	//몬스터사망_대	
    //86	224	608	32	32	//경쟁헌팅 아군사망
    //87	256	608	32	32	//경쟁헌팅 적군사망/카오사망

    eRADAR_TARGET_BOSS_MONSTER_ANI_POINT_0 = 71,
    eRADAR_TARGET_BOSS_MONSTER_ANI_POINT_1 = 72,
    eRADAR_TARGET_BOSS_MONSTER_ANI_POINT_2 = 73,
    eRADAR_TARGET_BOSS_MONSTER_ANI_POINT_3 = 74,
    eRADAR_TARGET_BOSS_MONSTER_ANI_POINT_4 = 75,
    eRADAR_TARGET_BOSS_MONSTER_ANI_POINT_5 = 76,

    eRADAR_TARGET_NORMAL_MONSTER_ANI_POINT_0 = 77,
    eRADAR_TARGET_NORMAL_MONSTER_ANI_POINT_1 = 78,
    eRADAR_TARGET_NORMAL_MONSTER_ANI_POINT_2 = 79,
    eRADAR_TARGET_NORMAL_MONSTER_ANI_POINT_3 = 80,
    eRADAR_TARGET_NORMAL_MONSTER_ANI_POINT_4 = 81,
    eRADAR_TARGET_NORMAL_MONSTER_ANI_POINT_5 = 82,

    eRADAR_NORMAL_MONSTER_POINT     = eRADAR_MONSTER_POINT,
    eRADAR_NORMAL_MONSTER_DEAD_POINT= eRADAR_MONSTER_DEAD_POINT,

    eRADAR_BOSS_MONSTER_POINT = 84,
    eRADAR_BOSS_MONSTER_DEAD_POINT = 85,

    eRADAR_TEAM_PLAYER_DEAD_POINT = 86,
    eRADAR_ENEMY_PLAYER_DEAD_POINT = 87,
    
    eRADAR_AIRSHIP_ON = 88,
    eRADAR_AIRSHIP_OVER = 89,
    eRADAR_AIRSHIP_PORTAL= 90, //EP2 비공정이동포탈

    eRADAR_DAILY_QUEST_ACCEPT = 91, // 일일퀘스트수락(빨강) _NA_003027_20111013_HONOR_SYSTEM

    //_NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR
    eRADAR_COLLECT_TARGET = 92,
    eRADAR_COLLECT_TARGET_ANIMATION = 93,

    // 미션 네비게이션 예약 94~102

    // 일일/길드퀘스트 수락/완료 아이콘
    eRADAR_NORMAL_QUEST_REWARD = 103,// 일반퀘스트완료(노랑)
    eRADAR_DAILY_QUEST_REWARD = 104,// 일일퀘스트완료(빨강)
    eRADAR_GUILD_QUEST_REWARD = 105,// 길드퀘스트완료(보라)
    eRADAR_GUILD_QUEST_ACCEPT = 106,// 길드퀘스트수락(보라)

    // 퀘스트 대륙맵 네비게이션
    eRADAR_NAVI_CIRCLE_RED = 107, 
	eRADAR_NAVI_CIRCLE_GREEN = 108, 
	eRADAR_NAVI_CIRCLE_BLUE = 109, 
	eRADAR_NAVI_CIRCLE_PERPLE = 110, 

    eRADAR_BATTLE_GROUND_STRONGPOINT_NORMAL = 111,     // 전장 거점 아이콘(흰)
    eRADAR_BATTLE_GROUND_STRONGPOINT_HALFRED = 112,    // 전장 거점 아이콘(흐린빨)
    eRADAR_BATTLE_GROUND_STRONGPOINT_RED = 113,        // 전장 거점 아이콘(빨)
    eRADAR_BATTLE_GROUND_STRONGPOINT_HALFBLUE = 114,   // 전장 거점 아이콘(흐린파)
    eRADAR_BATTLE_GROUND_STRONGPOINT_BLUE = 115,       // 전장 거점 아이콘(파)

	eRADAR_MAP_ANIMATION = 116,							// 대륙맵 애니메이션

	eRADAR_NPC_START2 = 117 , 
	eRADAR_NPC_END2	= 130,

    eRADAR_NPC_DOMINATION_END	    = 131, // 점령전 진행단계(점령완료)
    eRADAR_NPC_DOMINATION_YELLOW	= 132, // 점령전 진행단계(노랑)
    eRADAR_NPC_DOMINATION_GREEN	    = 133, // 점령전 진행단계(녹)
    eRADAR_NPC_DOMINATION_ORANGE	= 134, // 점령전 진행단계(주황)
    eRADAR_NPC_DOMINATION_RED	    = 135, // 점령전 진행단계(빨강)

#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
    eRADAR_CHALLENGE_DAILY_QUEST_REWARD = 136, // 도전일퀘 물음표
    eRADAR_CHALLENGE_DAILY_QUEST_ACCEPT = 137, // 도전일퀘 느낌표
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST

	eRADAR_POINT_COUNT,
};



struct RADAR_POINT
{
	DWORD					dwObjectKey;
	eRADAR_POINT_COLOR		eRadarPointColor;
	float					fPosX, fPosY;
	TCHAR					szName[MAX_PATH];
};


typedef		std::vector<RADAR_POINT *>		RadarPoint_vector;
typedef		RadarPoint_vector::iterator		RadarPoint_vector_Itr;



class RadarInfoParser : public Singleton<RadarInfoParser> ,public IParser
{
public:
	RadarInfoParser();
	~RadarInfoParser();

	VOID			Init(DWORD dwPoolSize);

	VOID			Init(DWORD dwPoolSize , char *szPackfileName);
	VOID			Release();

	virtual	BOOL	LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	POINT			GetTextureSize(void);
	RECT*           GetRadarImgInfo(int iIndex);
	HANDLE			GetTexture(void) { return m_hRadarTexture; }


private:
	VOID		    Unload();
	util::SolarHashTable<RECT*>*    m_pDataTable;


	//-----------------------------------------
	// 텍스쳐 핸들
	//-----------------------------------------
	HANDLE				            m_hRadarTexture;
	POINT				            m_RadarTextureSize;
	St_TextureContainer*            m_pRadarTextureInfo[eRADAR_POINT_COUNT];
};



