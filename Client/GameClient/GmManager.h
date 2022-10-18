#ifndef __GM_MANAGER_H__
#define __GM_MANAGER_H__

/**
    �Էµ� ���ڿ��� �ؼ� GM ��ɾ� ó�� 

    @author
        - ������ 

    @since
        - 2005.6.13
    @remarks
    - ����� GM��ɾ ó���ϰ� �Ϲ� ��ɾ�( ����, /�ɱ�, /�Ӹ�, /�ŷ� ��� )����
		CommandManager.CPP, H ���Ͽ��� ó���մϴ�.
*/


//---------------------------------------------------------------------------------------
//		GM ��ɾ�
//---------------------------------------------------------------------------------------
enum eGM_CMD
{
	//	��Ÿ
	eGM_CMD_GHOST				= 0,	//����			[��, ��]
	eGM_CMD_INVINCIBLE			= 1,	//����			[��, ��]


	//	�̵�
	eGM_CMD_GOTO_MAP			= 2,	//����			[�� �ڵ�, �� ��ȣ]			[x]						[y]


	//	����
	eGM_CMD_CHAR_INFO			= 3,	//����			[ĳ���͸�]
	eGM_CMD_ROOM_INFO			= 4,	//������
	eGM_CMD_ROOM_LIST			= 5,	//�渮��Ʈ		[��ü, �̼�, ����]
	eGM_CMD_CHANNEL_INFO		= 6,	//ä������


	//	���
	eGM_CMD_MOVE_CHAR			= 7,	//�̵�			[ĳ���� ��, ������Ʈ Ű]	[�� �ڵ�, �� ��ȣ]		[x]		[y]
	eGM_CMD_WATCH				= 8,	//����			[ĳ���� ��, ������Ʈ Ű]


	//	����
	eGM_CMD_NOTICE_WORLD		= 9,	//����_����		[���� �޽���]
	eGM_CMD_NOTICE_CHANNEL		= 10,	//ä��_����		[���� �޽���]
	eGM_CMD_NOTICE_LOCAL		= 11,	//����_����		[���� �޽���]


	//	����
	eGM_CMD_REMOVE_ITEM			= 12,	//����������
	eGM_CMD_REMOVE_MONSTER		= 13,	//��������


	//	������
	eGM_CMD_HEAL,						//ȸ��
	eGM_CMD_LEVEL_UP,					//������		[����Ʈ�� ��ġ]
	eGM_CMD_STAT_UP,					//���ݾ�		[����Ʈ�� ��ġ]
	eGM_CMD_SKILL_POINT_UP,				//��ų����Ʈ��	[����Ʈ�� ��ġ]
	eGM_CMD_ADD_SKILL,					//��ų�߰�		[��ų �ڵ�]					[��ų ����]
	eGM_CMD_RECALL_MONSTER,				//���ͼ�ȯ	[���� �ڵ�]				[��ü��]
	eGM_CMD_RESURRECTION,				//��Ȱ


	//	����
	eGM_CMD_CREATE_ITEM,				//����			[������ �ڵ�]				[����]
	eGM_CMD_HYME,						//����			[�׼�]
	eGM_CMD_MAKE_OPTION,				//�ɼǸ����



	eGM_CMD_SPEED,						//�ӵ�			[0....5]
	eGM_CMD_ENTRANCE,					//����
	eGM_CMD_CONNECT_CLOSE,				//��������		[ĳ���� ��]
	eGM_CMD_ACCOUNT,					//��ī��Ʈ		[��, ��]


	eGM_CMD_MONSTER_DESTROY,			//����			[���� ������Ʈ Ű]
	eGM_CMD_MONSTER_KILL,				//ų			[���� ������Ʈ Ű]

//	eGM_CMD_PK,							//����			[��, ��]

	eGM_CMD_CREATE_ENCHANT_ITEM,		//��þƮ�����	[�������ڵ�]	[����]	[��þƮ���]


	eGM_CMD_COUNT,						//	GM ��ɾ� ����
};

class GmManager 
{
public:
	GmManager();
	~GmManager();

	BOOL	ParseCommand(TCHAR * pszString);

protected:

    //�ӽ� ��ų ���� 
    //BOOL	ParseSkill(TCHAR * pszString);
    DWORD	GetSkillCode(TCHAR * pszSkillName);

	BOOL	ParseStyle(TCHAR * pszString);
	DWORD	GetStyleCode(TCHAR * pszString);
	
	BOOL	ParseWeapon(TCHAR * pszString);
	DWORD	GetWeaponCode(TCHAR * pszString);

};

#endif // __GM_MANAGER_H__

