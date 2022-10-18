

#ifndef		COMMANDMANAGER_H
#define		COMMANDMANAGER_H


//---------------------------------------------------------------------------------------
//
//		�Ϲ� ��ɾ�
//
//---------------------------------------------------------------------------------------
enum eCHAT_COMMAND
{
	eCHAT_COMMAND_START_INDEX				= 2001,
	eCHAT_COMMAND_SHOUT_HOTKEY					= eCHAT_COMMAND_START_INDEX,//��ġ�� �ѱ�
	eCHAT_COMMAND_NORMAL_HOTKEY,		//��üü�� �ѱ�
	eCHAT_COMMAND_PARTY_HOTKEY,			//��Ƽü�� �ѱ�
	eCHAT_COMMAND_GUILD_HOTKEY,			//���ü�� �ѱ�
	eCHAT_COMMAND_CHANNEL_HOTKEY,		//ä��ä�� �ѱ�
	eCHAT_COMMAND_NORMAL,
	eCHAT_COMMAND_SHOUT,
	eCHAT_COMMAND_PARTY,
	eCHAT_COMMAND_GUILD,
	eCHAT_COMMAND_WHISPER,
	eCHAT_COMMAND_CHANNEL_CHAT,
	eCHAT_COMMAND_CHANNEL_JOIN,		//ä�� ä�� ����
	eCHAT_COMMAND_CHANNEL_JOIN_ENG,
	eCHAT_COMMAND_CHANNEL_EXIT,		//ä�� ä�� ������
	eCHAT_COMMAND_CHANNEL_EXIT_ENG,
	eCHAT_COMMAND_CHANNEL_INVITE,   //ä�� ä�� �ʴ�
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
	//	�Ϲ� �̸�Ƽ�� ��ɾ� ����
	eGAME_COMMAND_EMOTICON_START_INDEX		= 21101,

	eGAME_COMMAND_BOW	= eGAME_COMMAND_EMOTICON_START_INDEX,	// �λ�
	//	eGAME_COMMAND_BOW,					// �λ�
	eGAME_COMMAND_LEAVE,				// �ۺ�
	eGAME_COMMAND_QUESTION,				// �ǹ�
	eGAME_COMMAND_SIGH,					// �Ѽ�
	eGAME_COMMAND_CHEER,				// ȯȣ
	eGAME_COMMAND_DENIAL,				// �ź�
	eGAME_COMMAND_CONSENT,				// �³�
	eGAME_COMMAND_ATTACK,				// ����
	eGAME_COMMAND_THANKS,				// ����
	eGAME_COMMAND_APOLOGY,				// ���
	eGAME_COMMAND_KISS,					// Ű��
	eGAME_COMMAND_LAUGH,				// ����
	eGAME_COMMAND_CRYING,				// ����
	eGAME_COMMAND_WELCOME,				// ȯ��
	eGAME_COMMAND_HANDCLAP,				// �ڼ�
	eGAME_COMMAND_AWE,					// ���
	eGAME_COMMAND_BEST,					// �ְ�
	eGAME_COMMAND_SULKINESS,			// ��ħ
	eGAME_COMMAND_PROVOCATION,			// ����
	eGAME_COMMAND_SLEEP,				// ����
	eGAME_COMMAND_POSE,					// ����
	eGAME_COMMAND_DANCE,				// ��
	eGAME_COMMAND_GAG,					// ����
	eGAME_COMMAND_MIMICRY,				// ����
	eGAME_COMMAND_SHOCK,				// ��ũ

	/*eGAME_COMMAND_FOLLOW,				//	��������
	eGAME_COMMAND_DELEGATE_ATTACK,		//	���ݱ���
	eGAME_COMMAND_DELEGATE_DEFENSE,		//	������
	eGAME_COMMAND_ATTACK,				//	����
	eGAME_COMMAND_DESTROY,				//	��ȯü����*/

	NUM_GAME_COMMAND_COUNT,

	//	�Ϲ� �׼� ��ɾ� ����
	eGAME_COMMAND_ACTION_START_INDEX		= 100000,

	eGAME_COMMAND_WHISPER	= eGAME_COMMAND_ACTION_START_INDEX,	//	�Ӹ�
//	eGAME_COMMAND_WHISPER	= 0,		//	�Ӹ�
	eGAME_COMMAND_TRADE,				//	�ŷ�
	eGAME_COMMAND_PARTY_INVITE,			//	��Ƽ �ʴ�
	eGAME_COMMAND_PRIVATE_TRADE,		//	���� ����

	eGAME_COMMAND_SCREEN_SHOT,			// >> �̰Ŵ� ��� ����ϴ°� ����?
										//	�� ��ɾ�� ������� ��û�� �ִ� ���̱� �ѵ�,
										//	�Ϳ쿡�� Alt + Z Űó�� UI �� ��� ���̵� ��Ű�� ����Դϴ�.
										//	�츮�� UI �� �� ĳ���� �� �� ��� ���̵� ��Ű�� ����Դϴ�.
										//	������ ���� ��, ����Ѵٰ� �մϴ�.

	eGAME_COMMAND_SET_WHISPER,			//	�Ӹ� ���� ���, �ź� ���

	eGAME_COMMAND_SIT,					//	�ɱ�
	eGAME_COMMAND_STAND,				//	����
	eGAME_COMMAND_FOLLOW_PLAYER,		//	���󰡱�
	eGAME_COMMAND_PARTY_LEAVE,			//	Ż��
	eGAME_COMMAND_GUILD_INVITE,         //	����ʴ�

#ifdef _NA_004612_120329_ADD_ACTION_COMMAND
    eGAME_COMMAND_AUTO_ATTACK,          // �ڵ�����
    eGAME_COMMAND_PICK_ITEM,            // �������ݱ�
    eGAME_COMMAND_SIT_AND_STAND,        // �ɱ�� ����
    eGAME_COMMAND_ASSIST,               // ��ý�Ʈ
    eGAME_COMMAND_ASSIST_AND_ATTACK,    // ��ý�Ʈ�� ����
    eGAME_COMMAND_PARTY_MEMBER_SUMMON,  // ��Ƽ����ȯ
    eGAME_COMMAND_PARTY_MEMBER_WARP,    // ��Ƽ�忡�� �̵�
    eGAME_COMMAND_NORMAL_ATTACK,        // �⺻����
    eGAME_COMMAND_FRIEND_ADD,           // ģ���߰�
    eGAME_COMMAND_CRYSTAL_WARP_CREATE,  // �������� ��ȯ/����
    eGAME_COMMAND_IDENTIFY_SOCKET,      // ���ϵ��Ȯ��
#endif //_NA_004612_120329_ADD_ACTION_COMMAND


	eGAME_COMMAND_ACTION_END_CODE,		//	�Ϲ� ��ɾ� ������ �ε���
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
//		������ ��ɾ�
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

	//�ӽ� ��ų ���� 
	//BOOL   ParseSkill(TCHAR * pszString);
	DWORD  GetSkillCode(TCHAR * pszSkillName);

	BOOL   ParseStyle(TCHAR * pszString);
	DWORD  GetStyleCode(TCHAR * pszString);

	BOOL   ParseWeapon(TCHAR * pszString);
	DWORD  GetWeaponCode(TCHAR * pszString);

    //! �޽� �׷� üũ �ϱ� ���� �Լ�
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


