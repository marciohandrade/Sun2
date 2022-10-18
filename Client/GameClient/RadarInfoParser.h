
//---------------------------------------------------------------------------------------
//
//		2005�� 11�� 23��  �Ӵ���
//
//		*. ���̴� ���̾�α׿� ���Ǵ� �ý��� �̹����� �� ������ ��ü�ʿ� ����
//			�� �κк��� UV �� ��ǥ ������ �ʿ��մϴ�.
//
//		*. �� ��ǥ���� �ý�Ʈ ��ũ��Ʈ�� ó���Ͽ� ������ ��� �̹��� ���ϰ�
//			�ý�Ʈ ��ũ��Ʈ ���ϸ� �����ϸ� �ǰ� �۾�
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
	eRADAR_NORMAL_QUEST_ACCEPT,// �Ϲ� ����Ʈ����(���)
	eRADAR_REPEAT_QUEST_REWARD,// �ݺ� ����Ʈ�Ϸ�(���)

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
	eRADAR_REPEAT_QUEST_ACCEPT = 67,//�ݺ� ����Ʈ����(���)
	eRADAR_CRYSTAL_WARP = 68,
	eRADAR_BLAST		= 69,
	eRADAR_AIRSHIP		= 70,

    // _YMS_ADD_RADAR_ICON_100622
    //71	0	576	32	32	//Ÿ���� ���ϸ��̼�_��_01
    //72	32	576	32	32	//Ÿ���� ���ϸ��̼�_��_02
    //73	64	576	32	32	//Ÿ���� ���ϸ��̼�_��_03
    //74	96	576	32	32	//Ÿ���� ���ϸ��̼�_��_04
    //75	128	576	32	32	//Ÿ���� ���ϸ��̼�_��_05
    //76	160	576	32	32	//Ÿ���� ���ϸ��̼�_��_06
    //77	192	576	32	32	//Ÿ���� ���ϸ��̼�_��_01
    //78	224	576	32	32	//Ÿ���� ���ϸ��̼�_��_02
    //79	256	576	32	32	//Ÿ���� ���ϸ��̼�_��_03
    //80	288	576	32	32	//Ÿ���� ���ϸ��̼�_��_04
    //81	320	576	32	32	//Ÿ���� ���ϸ��̼�_��_05
    //82	352	576	32	32	//Ÿ���� ���ϸ��̼�_��_06
    //83	96	608	32	32	//����_��
    //84	128	608	32	32	//����_��/�������� ����/ī��
    //85	192	608	32	32	//���ͻ��_��	
    //86	224	608	32	32	//�������� �Ʊ����
    //87	256	608	32	32	//�������� �������/ī�����

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
    eRADAR_AIRSHIP_PORTAL= 90, //EP2 ������̵���Ż

    eRADAR_DAILY_QUEST_ACCEPT = 91, // ��������Ʈ����(����) _NA_003027_20111013_HONOR_SYSTEM

    //_NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR
    eRADAR_COLLECT_TARGET = 92,
    eRADAR_COLLECT_TARGET_ANIMATION = 93,

    // �̼� �׺���̼� ���� 94~102

    // ����/�������Ʈ ����/�Ϸ� ������
    eRADAR_NORMAL_QUEST_REWARD = 103,// �Ϲ�����Ʈ�Ϸ�(���)
    eRADAR_DAILY_QUEST_REWARD = 104,// ��������Ʈ�Ϸ�(����)
    eRADAR_GUILD_QUEST_REWARD = 105,// �������Ʈ�Ϸ�(����)
    eRADAR_GUILD_QUEST_ACCEPT = 106,// �������Ʈ����(����)

    // ����Ʈ ����� �׺���̼�
    eRADAR_NAVI_CIRCLE_RED = 107, 
	eRADAR_NAVI_CIRCLE_GREEN = 108, 
	eRADAR_NAVI_CIRCLE_BLUE = 109, 
	eRADAR_NAVI_CIRCLE_PERPLE = 110, 

    eRADAR_BATTLE_GROUND_STRONGPOINT_NORMAL = 111,     // ���� ���� ������(��)
    eRADAR_BATTLE_GROUND_STRONGPOINT_HALFRED = 112,    // ���� ���� ������(�帰��)
    eRADAR_BATTLE_GROUND_STRONGPOINT_RED = 113,        // ���� ���� ������(��)
    eRADAR_BATTLE_GROUND_STRONGPOINT_HALFBLUE = 114,   // ���� ���� ������(�帰��)
    eRADAR_BATTLE_GROUND_STRONGPOINT_BLUE = 115,       // ���� ���� ������(��)

	eRADAR_MAP_ANIMATION = 116,							// ����� �ִϸ��̼�

	eRADAR_NPC_START2 = 117 , 
	eRADAR_NPC_END2	= 130,

    eRADAR_NPC_DOMINATION_END	    = 131, // ������ ����ܰ�(���ɿϷ�)
    eRADAR_NPC_DOMINATION_YELLOW	= 132, // ������ ����ܰ�(���)
    eRADAR_NPC_DOMINATION_GREEN	    = 133, // ������ ����ܰ�(��)
    eRADAR_NPC_DOMINATION_ORANGE	= 134, // ������ ����ܰ�(��Ȳ)
    eRADAR_NPC_DOMINATION_RED	    = 135, // ������ ����ܰ�(����)

#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
    eRADAR_CHALLENGE_DAILY_QUEST_REWARD = 136, // �������� ����ǥ
    eRADAR_CHALLENGE_DAILY_QUEST_ACCEPT = 137, // �������� ����ǥ
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
	// �ؽ��� �ڵ�
	//-----------------------------------------
	HANDLE				            m_hRadarTexture;
	POINT				            m_RadarTextureSize;
	St_TextureContainer*            m_pRadarTextureInfo[eRADAR_POINT_COUNT];
};



