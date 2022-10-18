#pragma once

#include <SolarHashTable.h>

#include "QuestStruct.h"

const int MAX_QUEST_COMPL_NUM = QUESTPART_COMPL::MAX_QUEST_COMPL_NUM;
const int MAX_QUEST_PROGR_NUM = QUESTPART_PROGR::MAX_QUEST_PROGR_NUM;
//const int MAX_QUEST_NUM = 3000;

typedef	WORD QCODE;
typedef	WORD QLEVEL;
const int MAX_QUEST_NAME_LENGTH = 128;

const int MAX_QUEST_KILL_MONSTER_MAP_CODE = 5;

enum eQTYPE
{
	eQTYPE_NONE		= 0,
	eQTYPE_GENERAL	= 1,
	eQTYPE_ELITE	= 2,
	eQTYPE_MISSION	= 3,
	eQTYPE_HIDDEN	= 4,
};

enum eQUEST_ACCEPT_TYPE
{
	eQUEST_ACCEPT_GENERAL = 0,		//< �Ϲ�
	eQUEST_ACCEPT_AUTO,				//< �������� ���� ����
    eQUEST_ACCEPT_AUTO_BY_ITEM,		//< ������ ���� ���� ����
};


enum eQUEST_SERIALIZE
{
	QSERIALIZE_SAVE,
	QSERIALIZE_LOAD,
};

enum eQUEST_OPERATOR
{
	QOP_AND = 0,
	QOP_OR	= 1,
};

enum eQUEST_CONDITION_TYPE
{
	eQUEST_CONDITION_REPEAT,
	eQUEST_CONDITION_CHARLV,
	eQUEST_CONDITION_CHARCLASS,
	eQUEST_CONDITION_QUEST,			//< ���� ����Ʈ, �Ϸ� ����Ʈ, ���� ����Ʈ, �ʿ� ����Ʈ, �Ϸ� Ƚ��
	eQUEST_CONDITION_ITEM,			//< �ش�Ŭ����, ���� ������, �ʿ������, ����
	eQUEST_CONDITION_CLASSITEM,
	eQUEST_CONDITION_MONEY,
	eQUEST_CONDITION_KILLMONSTER,
	eQUEST_CONDITION_MISSION,
	eQUEST_CONDITION_AREA,
	eQUEST_CONDITION_ITEM_SPACE,
	eQUEST_CONDITION_CHAO,
    eQUEST_CONDITION_CREATE_PORTAL,
    //_NA_003027_20111013_HONOR_SYSTEM
    eQUEST_CONDITION_HONOR_RATING,
    eQUEST_CONDITION_DAILY_QUEST,
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    eQUEST_CONDITION_MISSION_CLEAR,
#endif //_NA_006607_20130402_ADD_QUESTTYPE
    eQUEST_CONDITION_MAX
};

enum eQUEST_ACTION_TYPE
{
	eQUEST_ACTION_ITEM = eQUEST_CONDITION_MAX,	//< ������ �ο� �� ����
	//eQUEST_ACTION_TIMER,						//< Ÿ�̸� �۵�
	eQUEST_ACTION_CHANGEofCLASS,	
	eQUEST_ACTION_SPAWN_MONSTER,
    //__NA000000_090320_v0902_LOGIC_CHANGE_ABOUT_QUEST__
    eQUEST_ACTION_SPAWN_MONSTER_ABANDON,
    eQUEST_ACTION_CREATE_PORTAL,
    eQUEST_ACTION_CLOSE_PORTAL,
    //__NA001283_20090225_COLLECTION_USE
	eQUEST_ACTION_DELETE_COLLECTIONID,
    eQUEST_ACTION_DAILY_QUEST,          // _NA_003027_20111013_HONOR_SYSTEM
    eQUEST_ACTION_DAILY_QUEST_DELETE,   // _NA_003027_20111013_HONOR_SYSTEM
    //
	eQUEST_ACTION_MAX
};

enum eQUEST_EVENT				//< �̺�Ʈó��
{
	QUEST_EVENT_ITEM,			//< Ŭ
	QUEST_EVENT_CHARLV,			//< Ŭ
	QUEST_EVENT_QUEST,			//< Ŭ
	QUEST_EVENT_MONEY,			//< Ŭ
	QUEST_EVENT_MONSTERKILL,	//< �� + Ŭ
	QUEST_EVENT_TIME,			//< Ŭ
	QUEST_EVENT_MISSION,		//< Ŭ
	QUEST_EVENT_AREA,			//< Ŭ
	QUEST_EVENT_CHAO,			//< Ŭ (( Ȥ�� ������ ������ ���� ���� ))- J0
    QUEST_EVENT_MONSTERKILL_GRADE, //< ��

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    QUEST_EVENT_MISSION_CLEAR,  //< ��
#endif //_NA_006607_20130402_ADD_QUESTTYPE
};

enum eSPECIAL_QUEST_REWARD
{
	SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL					= 1000000, // 100���� ü������
	// ... ChangeUp, ChageOfClass(����) ���� �ܰ� ����

	//	100Level ChangeUp �� ĳ���ͺ� �����ڵ� ��
	SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_BERSERKER			= 2010,
	SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_DRAGON_KNIGHT		= 2011,
	SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_SHADOW			= 2012,
	SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_VALKYRIE			= 2013,
	SPECIAL_REWARD_TYPE_CHANGEUP_100LEVEL_ELEMENTALIST		= 2014,
};

enum eQUEST_REQUEST_NPC_DIALOG
{
    QUEST_NPC_DIALOG_NONE           = 0,
    QUEST_NPC_DIALOG_SPAWN_MOB      = 1,
    QUEST_NPC_DIALOG_CREATE_PORTAL  = 2,
};

#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
    class QuestInfo;
    class QUEST_INFO_HASH : public STLX_MAP<DWORD, QuestInfo*>
    {
    public:
        QuestInfo* GetData(DWORD quest_code) { 
            iterator it = find(quest_code);
            return it == end() ? NULL : it->second;
        }

        void Add(QuestInfo* quest_info, DWORD quest_index) {
            insert(QUEST_INFO_HASH::value_type(quest_index, quest_info));
        }
    };
    typedef QUEST_INFO_HASH::iterator      QUEST_INFO_HASH_ITR;
#else
    class QuestInfo;
    typedef util::SolarHashTable<QuestInfo *>				QUEST_INFO_HASH;
    typedef util::SolarHashTable<QuestInfo *>::iterator		QUEST_INFO_HASH_ITR;
#endif

class Quest;
typedef util::SolarHashTable<Quest *>					QUEST_HASH;
typedef util::SolarHashTable<Quest *>::iterator			QUEST_HASH_ITR;
typedef STLX_VECTOR<QCODE>                  QUEST_CODE_VECTOR;
typedef STLX_MAP<WORD, QUEST_CODE_VECTOR>   QUEST_GROUP_MAP;

struct Deleter
{
	template< class T >
	void operator()( T *& p ) const
	{
		delete p; p = NULL;
	}
};

