

#ifndef		COMMANDMANAGER_H
#define		COMMANDMANAGER_H


//---------------------------------------------------------------------------------------
//
//		일반 명령어
//
//---------------------------------------------------------------------------------------
enum eCHAT_COMMAND
{
	eCHAT_COMMAND_START_INDEX				= 2001,
	eCHAT_COMMAND_SHOUT_HOTKEY					= eCHAT_COMMAND_START_INDEX,//외치기 켜기
	eCHAT_COMMAND_NORMAL_HOTKEY,		//전체체팅 켜기
	eCHAT_COMMAND_PARTY_HOTKEY,			//파티체팅 켜기
	eCHAT_COMMAND_GUILD_HOTKEY,			//길드체팅 켜기
	eCHAT_COMMAND_CHANNEL_HOTKEY,		//채널채팅 켜기
	eCHAT_COMMAND_NORMAL,
	eCHAT_COMMAND_SHOUT,
	eCHAT_COMMAND_PARTY,
	eCHAT_COMMAND_GUILD,
	eCHAT_COMMAND_WHISPER,
	eCHAT_COMMAND_CHANNEL_CHAT,
	eCHAT_COMMAND_CHANNEL_JOIN,		//채널 채팅 조인
	eCHAT_COMMAND_CHANNEL_JOIN_ENG,
	eCHAT_COMMAND_CHANNEL_EXIT,		//채널 채팅 나가기
	eCHAT_COMMAND_CHANNEL_EXIT_ENG,
	eCHAT_COMMAND_CHANNEL_INVITE,   //채널 채팅 초대
	eCHAT_COMMAND_CHANNEL_INVITE_ENG,
	eCHAT_COMMAND_WHISPER_HOTKEY,
	eCHAT_COMMAND_RECENT_WHISPER,	
	eCHAT_COMMAND_GUILD_NOTICE,

	eCHAT_COMMAND_NORMAL_HOTKEY2,
	eCHAT_COMMAND_NORMAL_HOTKEY3,
	eCHAT_COMMAND_SHOUT_HOTKEY2,
	eCHAT_COMMAND_SHOUT_HOTKEY3,
	eCHAT_COMMAND_PARTY_HOTKEY2,
	eCHAT_COMMAND_PARTY_HOTKEY3,
	eCHAT_COMMAND_GUILD_HOTKEY2,
	eCHAT_COMMAND_CHANNEL_HOTKEY2,
	eCHAT_COMMAND_CHANNEL_HOTKEY3,
	eCHAT_COMMAND_CHANNEL_INFO_REQ,
	eCHAT_COMMAND_TARGET,
	eCHAT_COMMAND_MAX,
};

enum eGAME_COMMAND
{
	//	일반 이모티콘 명령어 시작
	eGAME_COMMAND_EMOTICON_START_INDEX		= 21101,

	eGAME_COMMAND_BOW	= eGAME_COMMAND_EMOTICON_START_INDEX,	// 인사
	//	eGAME_COMMAND_BOW,					// 인사
	eGAME_COMMAND_LEAVE,				// 작별
	eGAME_COMMAND_QUESTION,				// 의문
	eGAME_COMMAND_SIGH,					// 한숨
	eGAME_COMMAND_CHEER,				// 환호
	eGAME_COMMAND_DENIAL,				// 거부
	eGAME_COMMAND_CONSENT,				// 승낙
	eGAME_COMMAND_ATTACK,				// 공격
	eGAME_COMMAND_THANKS,				// 감사
	eGAME_COMMAND_APOLOGY,				// 사과
	eGAME_COMMAND_KISS,					// 키스
	eGAME_COMMAND_LAUGH,				// 웃음
	eGAME_COMMAND_CRYING,				// 울음
	eGAME_COMMAND_WELCOME,				// 환영
	eGAME_COMMAND_HANDCLAP,				// 박수
	eGAME_COMMAND_AWE,					// 경외
	eGAME_COMMAND_BEST,					// 최고
	eGAME_COMMAND_SULKINESS,			// 삐침
	eGAME_COMMAND_PROVOCATION,			// 도발
	eGAME_COMMAND_SLEEP,				// 수면
	eGAME_COMMAND_POSE,					// 포즈
	eGAME_COMMAND_DANCE,				// 댄스
	eGAME_COMMAND_GAG,					// 개그
	eGAME_COMMAND_MIMICRY,				// 동물
	eGAME_COMMAND_SHOCK,				// 쇼크

	/*eGAME_COMMAND_FOLLOW,				//	공격정지
	eGAME_COMMAND_DELEGATE_ATTACK,		//	공격권한
	eGAME_COMMAND_DELEGATE_DEFENSE,		//	방어권한
	eGAME_COMMAND_ATTACK,				//	공격
	eGAME_COMMAND_DESTROY,				//	소환체삭제*/

	NUM_GAME_COMMAND_COUNT,

	//	일반 액션 명령어 시작
	eGAME_COMMAND_ACTION_START_INDEX		= 100000,

	eGAME_COMMAND_WHISPER	= eGAME_COMMAND_ACTION_START_INDEX,	//	귓말
//	eGAME_COMMAND_WHISPER	= 0,		//	귓말
	eGAME_COMMAND_TRADE,				//	거래
	eGAME_COMMAND_PARTY_INVITE,			//	파티 초대
	eGAME_COMMAND_PRIVATE_TRADE,		//	개인 상점

	eGAME_COMMAND_SCREEN_SHOT,			// >> 이거는 어디서 사용하는건 가요?
										//	이 명령어는 운영팀에서 요청이 있던 것이긴 한데,
										//	와우에서 Alt + Z 키처럼 UI 를 모두 하이드 시키는 기능입니다.
										//	우리는 UI 와 내 캐릭터 둘 다 모두 하이드 시키는 기능입니다.
										//	스샷을 찍을 때, 사용한다고 합니다.

	eGAME_COMMAND_SET_WHISPER,			//	귓말 수신 허용, 거부 기능

	eGAME_COMMAND_SIT,					//	앉기
	eGAME_COMMAND_STAND,				//	서기
	eGAME_COMMAND_FOLLOW_PLAYER,		//	따라가기
	eGAME_COMMAND_PARTY_LEAVE,			//	탈퇴
	eGAME_COMMAND_GUILD_INVITE,         //	길드초대

#ifdef _NA_004612_120329_ADD_ACTION_COMMAND
    eGAME_COMMAND_AUTO_ATTACK,          // 자동공격
    eGAME_COMMAND_PICK_ITEM,            // 아이템줍기
    eGAME_COMMAND_SIT_AND_STAND,        // 앉기와 서기
    eGAME_COMMAND_ASSIST,               // 어시스트
    eGAME_COMMAND_ASSIST_AND_ATTACK,    // 어시스트와 공격
    eGAME_COMMAND_PARTY_MEMBER_SUMMON,  // 파티원소환
    eGAME_COMMAND_PARTY_MEMBER_WARP,    // 파티장에게 이동
    eGAME_COMMAND_NORMAL_ATTACK,        // 기본공격
    eGAME_COMMAND_FRIEND_ADD,           // 친구추가
    eGAME_COMMAND_CRYSTAL_WARP_CREATE,  // 워프수정 소환/해제
    eGAME_COMMAND_IDENTIFY_SOCKET,      // 소켓등급확인
#endif //_NA_004612_120329_ADD_ACTION_COMMAND


	eGAME_COMMAND_ACTION_END_CODE,		//	일반 명령어 마지막 인덱스
};

//////////////////////////////////////////////////////////////////////////

class CommandManager
{

public:
	CommandManager( void );
	~CommandManager( void );


	DWORD ParseCommand(TCHAR* pszString);
    void ChangeLanguage(LanguageType new_type);
private:
	BOOL isChatCommand(TCHAR firstString);
};


extern	CommandManager		g_CommandManager;







//---------------------------------------------------------------------------------------
//
//		디버깅용 명령어
//
//---------------------------------------------------------------------------------------
enum eDEBUG_COMMAND
{
	eDEBUG_COMMAND_SAVE_INVENTORY	= 1,
	eDEBUG_COMMAND_SAVE_WAREHOUSE	= 2,
};


class SCItemSlotContainer;

class DebugCommandManager
{
public:
	DebugCommandManager( void );
	~DebugCommandManager( void );


	BOOL ParseCommand( TCHAR * pszString );

	//임시 스킬 지정 
	//BOOL   ParseSkill(TCHAR * pszString);
	DWORD  GetSkillCode(TCHAR * pszSkillName);

	BOOL   ParseStyle(TCHAR * pszString);
	DWORD  GetStyleCode(TCHAR * pszString);

	BOOL   ParseWeapon(TCHAR * pszString);
	DWORD  GetWeaponCode(TCHAR * pszString);

    //! 메쉬 그룹 체크 하기 위한 함수
    bool Check_MashGroup();

private:
    void SaveItemList(eDEBUG_COMMAND eCOMD);
	void SaveItemContainer(TCHAR* szFilename, SCItemSlotContainer* pCont);

    int GetCharacterResourceCode(DWORD class_define_code);
#ifdef _DEV_VER //#ifdef _SCJ_GM_CMD_CREATE_ITEM_TO
    bool SendMaketoPacket(TCHAR* command_string);
#endif//_DEV_VER

#ifdef _DEV_VER
    void OpenGMWin(BYTE gm_window_type);
    void ShowGMWin(BYTE gm_window_type, bool value);
    void ItemSearch(TCHAR* search_word);
#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
	void OpenTextureViewerWin(BYTE gm_window_type);
#endif
#endif

#endif//_DEV_VER//_DH_GM_ITEM_WINDOW
};

extern	DebugCommandManager		g_DebugCommandManager;




#endif	//	COMMANDMANAGER_H


