#ifndef __GM_MANAGER_H__
#define __GM_MANAGER_H__

/**
    입력된 문자열을 해석 GM 명령어 처리 

    @author
        - 김지완 

    @since
        - 2005.6.13
    @remarks
    - 여기는 GM명령어만 처리하고 일반 명령어( 도움말, /앉기, /귓말, /거래 등등 )들은
		CommandManager.CPP, H 파일에서 처리합니다.
*/


//---------------------------------------------------------------------------------------
//		GM 명령어
//---------------------------------------------------------------------------------------
enum eGM_CMD
{
	//	기타
	eGM_CMD_GHOST				= 0,	//투명			[켬, 끔]
	eGM_CMD_INVINCIBLE			= 1,	//무적			[켬, 끔]


	//	이동
	eGM_CMD_GOTO_MAP			= 2,	//가기			[멥 코드, 방 번호]			[x]						[y]


	//	정보
	eGM_CMD_CHAR_INFO			= 3,	//정보			[캐릭터명]
	eGM_CMD_ROOM_INFO			= 4,	//방정보
	eGM_CMD_ROOM_LIST			= 5,	//방리스트		[전체, 미션, 헌팅]
	eGM_CMD_CHANNEL_INFO		= 6,	//채널정보


	//	경고
	eGM_CMD_MOVE_CHAR			= 7,	//이동			[캐릭터 명, 오브젝트 키]	[멥 코드, 방 번호]		[x]		[y]
	eGM_CMD_WATCH				= 8,	//감시			[캐릭터 명, 오브젝트 키]


	//	공지
	eGM_CMD_NOTICE_WORLD		= 9,	//월드_공지		[공지 메시지]
	eGM_CMD_NOTICE_CHANNEL		= 10,	//채널_공지		[공지 메시지]
	eGM_CMD_NOTICE_LOCAL		= 11,	//지역_공지		[공지 메시지]


	//	전투
	eGM_CMD_REMOVE_ITEM			= 12,	//아이템제거
	eGM_CMD_REMOVE_MONSTER		= 13,	//몬스터제거


	//	에디팅
	eGM_CMD_HEAL,						//회복
	eGM_CMD_LEVEL_UP,					//레벨업		[에디트할 수치]
	eGM_CMD_STAT_UP,					//스텟업		[에디트할 수치]
	eGM_CMD_SKILL_POINT_UP,				//스킬포인트업	[에디트할 수치]
	eGM_CMD_ADD_SKILL,					//스킬추가		[스킬 코드]					[스킬 레벨]
	eGM_CMD_RECALL_MONSTER,				//몬스터소환	[몬스터 코드]				[개체수]
	eGM_CMD_RESURRECTION,				//부활


	//	생성
	eGM_CMD_CREATE_ITEM,				//생성			[아이템 코드]				[갯수]
	eGM_CMD_HYME,						//하임			[액수]
	eGM_CMD_MAKE_OPTION,				//옵션만들기



	eGM_CMD_SPEED,						//속도			[0....5]
	eGM_CMD_ENTRANCE,					//입장
	eGM_CMD_CONNECT_CLOSE,				//접속종료		[캐릭터 명]
	eGM_CMD_ACCOUNT,					//어카운트		[켬, 끔]


	eGM_CMD_MONSTER_DESTROY,			//제거			[몬스터 오브젝트 키]
	eGM_CMD_MONSTER_KILL,				//킬			[몬스터 오브젝트 키]

//	eGM_CMD_PK,							//결투			[켬, 끔]

	eGM_CMD_CREATE_ENCHANT_ITEM,		//인첸트만들기	[아이템코드]	[갯수]	[인첸트등급]


	eGM_CMD_COUNT,						//	GM 명령어 갯수
};

class GmManager 
{
public:
	GmManager();
	~GmManager();

	BOOL	ParseCommand(TCHAR * pszString);

protected:

    //임시 스킬 지정 
    //BOOL	ParseSkill(TCHAR * pszString);
    DWORD	GetSkillCode(TCHAR * pszSkillName);

	BOOL	ParseStyle(TCHAR * pszString);
	DWORD	GetStyleCode(TCHAR * pszString);
	
	BOOL	ParseWeapon(TCHAR * pszString);
	DWORD	GetWeaponCode(TCHAR * pszString);

};

#endif // __GM_MANAGER_H__

