#ifndef MESSAGE_H
#define MESSAGE_H

#pragma  once

#define MAX_QUEST_MESSAGE		1000
#define MAX_GENERAL_MESSAGE		1000
#define MAX_TIP_MESSAGE			1000
#define EXTRA_MESSAGE			1000

#define MESSAGE_TOTAL_MAX		(MAX_QUEST_MESSAGE+MAX_GENERAL_MESSAGE+MAX_TIP_MESSAGE+EXTRA_MESSAGE)

enum eMESSAGE_TYPE
{
	MESSAGE_TYPE_AREA_NAME = 0,                     // (0)������
	MESSAGE_TYPE_NORMAL,                            // (1)��� �ý��� �޽��� + ä��â �ý��� �޽���
	MESSAGE_TYPE_NPC,                               // (2)��� �ý��� �޽��� + �̱״Ͻ�â 
	MESSAGE_TYPE_TRIGGER,                           // (3)��� �ý��� �޽��� + ������ ����â
	MESSAGE_TYPE_EVENT,                             // (4)�̼� ���۽� �̺�Ʈ ȭ��� �޽���
	MESSAGE_TYPE_NPC_ONLY,                          // (5)�̱״Ͻ�â 
	MESSAGE_TYPE_TRIGGER_ONLY,                      // (6)������ ����â
	MESSAGE_TYPE_CENTER_MSG_ONLY,                   // (7)��� �޼���
	MESSAGE_TYPE_GUILD_MESSAGE,                     // (8)���޼���
    // _NA_0_20091118_GAMEROOM_BATTLE_RESULT
	MESSAGE_TYPE_BATTLE_RESULT,                     // (9)���� ���â ����
    MESSAGE_TYPE_OBJECT_BALLOON_MSGBOX,             // (10)������Ʈ ��ǳ�� ���
    MESSAGE_TYPE_OBJECT_BALLOON_MSGBOX_WITH_SYSTEM, // (11)������Ʈ ��ǳ�� ��� �� �ý��۸޽���

	MESSAGE_TYPE_MAX,
};

typedef struct _MESSAGE_INFO
{
	size_t  MessageLen;
    TCHAR*  pszMessage;
	int		iTime;			// �����ð�
	BYTE	byType;			// Ÿ��
	DWORD	dwImgCode;		// �̹��� �ڵ�
	DWORD	dwNCode;		// �̸��ڵ�
	DWORD	dwDCode;		// �����ڵ�
} MESSAGE_INFO;


typedef stdext::hash_map<DWORD, MESSAGE_INFO *>	MESSAGE_INFO_HASH;
typedef MESSAGE_INFO_HASH::iterator		MESSAGE_INFO_ITR;
typedef	std::pair<DWORD, MESSAGE_INFO *>		MESSAGE_INFO_PAIR;
#include <Singleton.h>
#include <SolarFileReader.h>
#include <MemoryPoolFactory.h>

class MessageInfoParser : public util::Singleton<MessageInfoParser>, public IParser
{
public:
	MessageInfoParser(void);
	virtual ~MessageInfoParser(void);

public:
	void Init(DWORD dwMaxSize);
	//VOID Init(DWORD dwMaxSize, char * pszPackFileName);
	
	virtual	BOOL    LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	void Release();

	MESSAGE_INFO* GetMessage(DWORD dwID);

    bool SetMessage(DWORD message_id, TCHAR *string);

protected:
	void UnLoad();
	

private:
	DWORD									m_dwMaxSize;

	MESSAGE_INFO_HASH						m_MessageHashmap;

	util::CMemoryPoolFactory<MESSAGE_INFO> *m_pMessagePool;
};

#endif // MESSAGE_H
