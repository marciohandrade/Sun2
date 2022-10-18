#pragma once

#pragma pack(push,1)

// redesigned by __NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM
// 버전값은 진행중과 완료 스트림 같이 쓴다.
////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 진행중 퀘스트 스트림 버전 관리
//
// 9byte
struct QUESTPART_PROGR_V0
{
	enum {
        QUESTPART_PROGR_VERSION = 0,
		MAX_QUEST_MONSTER_KILL_COUNT = 5, 
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        MAX_QUEST_MISSION_CLEAR_COUNT = 2,
#endif //_NA_006607_20130402_ADD_QUESTTYPE
		MAX_QUEST_PROGR_NUM = 50
    };

	// 4byte
	DWORD m_QCode		: 11;	//< 2048 개
	DWORD m_ExpireTime	: 19;	//< 524288 := 145.63시간 => 최대 86400=24시간만 해주면 됨
								//  (남은시간 : 초로 사용) GetQuestInfo()->GetExpiredTime() 이 0이 아니면 사용중 값

	DWORD m_QState		: 2;	//< 0~3 => 0: 진행, 1: 완료, 2:실패
	// 5byte
	BYTE m_KCount[MAX_QUEST_MONSTER_KILL_COUNT];
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    BYTE mission_number[2];
#endif //_NA_006607_20130402_ADD_QUESTTYPE
};

// 11byte
struct QUESTPART_PROGR_V1
{
	enum {
        QUESTPART_PROGR_VERSION = 1, 
		MAX_QUEST_MONSTER_KILL_COUNT = 5, 
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        MAX_QUEST_MISSION_CLEAR_COUNT = 2,
#endif //_NA_006607_20130402_ADD_QUESTTYPE
		MAX_QUEST_PROGR_NUM = 50
    };

	void init() {
		this->m_QCode = 0;
		this->m_Reserved1 = 0;
		this->m_ExpireTime = 0;
		this->m_Reserved2 = 0;
		this->m_QState = 0;
		this->m_Reserved3 = 0;
		ZeroMemory(this->m_KCount,MAX_QUEST_MONSTER_KILL_COUNT);
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        ZeroMemory(this->mission_number, MAX_QUEST_MISSION_CLEAR_COUNT);
#endif //_NA_006607_20130402_ADD_QUESTTYPE
	}

	QUESTPART_PROGR_V1& operator = (const QUESTPART_PROGR_V0& rhs) { 

		this->m_QCode = rhs.m_QCode;
		this->m_Reserved1 = 0;
		this->m_ExpireTime = rhs.m_ExpireTime;
		this->m_Reserved2 = 0;
		this->m_QState = rhs.m_QState;
		this->m_Reserved3 = 0;
		memcpy(this->m_KCount,rhs.m_KCount,QUESTPART_PROGR_V0::MAX_QUEST_MONSTER_KILL_COUNT);
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        memcpy(this->mission_number, rhs.mission_number, QUESTPART_PROGR_V0::MAX_QUEST_MISSION_CLEAR_COUNT);
#endif //_NA_006607_20130402_ADD_QUESTTYPE
		return *this;
	}

	WORD  m_QCode		:	12;	//< 4096 개
	WORD  m_Reserved1	:	4;	//< 이후 여분의 값은 유동적으로 사용.
	
	DWORD m_ExpireTime	:	17;	//< 132072 := 36.4시간 => 최대 86400=24시간만 해주면 됨
	DWORD m_Reserved2	:	11;
	DWORD m_QState		:	2;	//< 0~3 => 0: 진행, 1: 완료, 2:실패
	DWORD m_Reserved3	:	2;
	
	BYTE m_KCount[MAX_QUEST_MONSTER_KILL_COUNT];
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    BYTE mission_number[2];
#endif //_NA_006607_20130402_ADD_QUESTTYPE
};

// 11byte
// m_Reserved2 영역에 탐색퀘스트 진행 여부 추가
struct QUESTPART_PROGR_V2
{
    enum {
        QUESTPART_PROGR_VERSION = 2, 
        MAX_QUEST_MONSTER_KILL_COUNT = 5,
        MAX_QUEST_PROGR_NUM = 50
    };

    void init() {
        this->m_QCode = 0;
        this->m_Reserved1 = 0;
        this->m_ExpireTime = 0;
        this->m_Reserved2 = 0;
        this->m_SearchedArea1 = 0;
        this->m_SearchedArea2 = 0;
        this->m_SearchedArea3 = 0;
        this->m_Reserved3 = 0;
        this->m_QState = 0;
        this->m_Reserved4 = 0;
        ZeroMemory(this->m_KCount,MAX_QUEST_MONSTER_KILL_COUNT);
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        ZeroMemory(this->mission_number, 2);
#endif //_NA_006607_20130402_ADD_QUESTTYPE
    }

    QUESTPART_PROGR_V2& operator = (const QUESTPART_PROGR_V1& rhs) { 

        this->m_QCode = rhs.m_QCode;
        this->m_Reserved1 = 0;
        this->m_ExpireTime = rhs.m_ExpireTime;
        this->m_Reserved2 = 0;
        this->m_SearchedArea1 = 0;
        this->m_SearchedArea2 = 0;
        this->m_SearchedArea3 = 0;
        this->m_Reserved3 = 0;
        this->m_QState = rhs.m_QState;
        this->m_Reserved4 = 0;
        memcpy(this->m_KCount,rhs.m_KCount,QUESTPART_PROGR_V1::MAX_QUEST_MONSTER_KILL_COUNT);
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
        memcpy(this->mission_number, rhs.mission_number, QUESTPART_PROGR_V1::MAX_QUEST_MISSION_CLEAR_COUNT);
#endif //_NA_006607_20130402_ADD_QUESTTYPE

        return *this;
    }

    WORD  m_QCode		    :	12;	//< 4096 개
    WORD  m_Reserved1	    :	4;	//< 이후 여분의 값은 유동적으로 사용.

    DWORD m_ExpireTime	    :	17;	//< 132072 := 36.4시간 => 최대 86400=24시간만 해주면 됨
    DWORD m_Reserved2	    :	3;
    DWORD m_SearchedArea1	:	1;
    DWORD m_SearchedArea2   :	1;
    DWORD m_SearchedArea3   :	1;
    DWORD m_Reserved3   	:	5;
    DWORD m_QState		    :	2;	//< 0~3 => 0: 진행, 1: 완료, 2:실패
    DWORD m_Reserved4	    :	2;

    BYTE m_KCount[MAX_QUEST_MONSTER_KILL_COUNT];
#ifdef _NA_006607_20130402_ADD_QUESTTYPE
    BYTE mission_number[2];
#endif //_NA_006607_20130402_ADD_QUESTTYPE
};

struct QUESTPART_PROGR : public QUESTPART_PROGR_V2 {};

////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 퀘스트 완료 스트림 버전 관리
//
struct QUESTPART_COMPL_V0
{
	enum { 
        QUESTPART_COMPL_VERSION = 0, 
		MAX_QUEST_COMPL_NUM = 1500
    };

	BYTE m_Num;			//< 완료한 횟수
};

// 0번 사용 불가 (1~3000)
// (define.h,DefineForLog.h,LogCommonDefine.h)
struct QUESTPART_COMPL_V1
{
	enum { 
        QUESTPART_COPML_VERSION = 1, 
		MAX_QUEST_COMPL_NUM = 3001
    };

	BYTE m_Num;			//< 완료한 횟수
};

// 내부 변화 없음
struct QUESTPART_COMPL_V2
{
    enum { 
        QUESTPART_COPML_VERSION = 2, 
        MAX_QUEST_COMPL_NUM = MAX_COMPL_QUESTSTREAM_SIZE
    };

    BYTE m_Num;			//< 완료한 횟수
};

#ifdef _NA_004157_20120409_QUEST_BANDWITH_INCREASING
struct QUESTPART_COMPL_V3
{
    enum { 
        QUESTPART_COPML_VERSION = 3, 
        MAX_QUEST_COMPL_NUM = 3001
    };

    WORD m_Num;			//< 완료한 횟수
};
struct QUESTPART_COMPL : public QUESTPART_COMPL_V3 {};
#else
struct QUESTPART_COMPL : public QUESTPART_COMPL_V2 {};
#endif //_NA_004157_20120409_QUEST_BANDWITH_INCREASING

////////////////////////////////////////////////////////////////////////////////////////////////////


struct QUESTPART_COMPL_EX : public QUESTPART_COMPL
{
	WORD m_QCode;
};

struct QUEST_PROGR_TOTAL_INFO
{
	BYTE			m_Count;
	QUESTPART_PROGR	m_Info[QUESTPART_PROGR::MAX_QUEST_PROGR_NUM];
	int GetSize() {
		return (sizeof(QUEST_PROGR_TOTAL_INFO)-(QUESTPART_PROGR::MAX_QUEST_PROGR_NUM-m_Count)*sizeof(QUESTPART_PROGR));
	}
};

// 값을 사용하는 배열의 최대 인덱스 만큼을 보낸다
struct QUEST_COMPL_TOTAL_INFO
{
	WORD				m_Count;
	QUESTPART_COMPL_EX	m_Info[QUESTPART_COMPL::MAX_QUEST_COMPL_NUM];
	int GetSize() 
	{
		return (sizeof(QUEST_COMPL_TOTAL_INFO)-(QUESTPART_COMPL::MAX_QUEST_COMPL_NUM-m_Count)*sizeof(QUESTPART_COMPL_EX));
	}
};

struct QUEST_DAILY_INFO
{
    WORD code_;
    BYTE status_;
    DWORD64 start_datetime_;
    DWORD64 end_datetime_;

    QUEST_DAILY_INFO()
        : code_(0), status_(0), start_datetime_(0), end_datetime_(0)
    {

    }
};

// (WAVERIX) define제거했습니당
//#ifdef __NA_0_20070521_QUEST_CONDITION_RESULT_CHANGE
//#endif

#pragma pack(pop)



struct QUEST_MSG
{

};

struct QUEST_MONSTER_KILL_MSG : public QUEST_MSG
{
	WORD m_MonsterCode;
	//WORD m_MonsterNum;			//< 기본적으로 1이겠지?
};

struct QUEST_MONSTER_KILL_GRADE_MSG : public QUEST_MSG
{
    BYTE m_MonsterGrade;
    WORD m_MonsterLevel;
};

struct QUEST_AREA_MSG : public QUEST_MSG
{
    DWORD m_AreaID;
};

#ifdef _NA_006607_20130402_ADD_QUESTTYPE
struct QUEST_MISSION_CLEAR_MSG : public QUEST_MSG
{
    WORD mission_code;
};

#endif //_NA_006607_20130402_ADD_QUESTTYPE