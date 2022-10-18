#pragma once
//=======================================================================================================================
/// QuestManager_Concrete class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 3. 29
	@remarks
			- �������� ����Ʈ�� �ٷ� �� ����ϴ� ��� Ŭ����
	@description
			- QuestManager�� ���
			- QuestManager�� virtual VOID OnEvent( Quest* pQuest )�� ���� ���

	@scenario 
			1. DB�κ��� (������and�Ϸ��)����Ʈ�� ������ �޾Ƽ� Player���� Quest��ü�� QuestManager�� ���ؼ� �����Ѵ�(CreateQuest())
			2. �ʱ�ȭ�� QuestManager_Concrete�� �̺�Ʈ�� �߻��ÿ� ������ �̺�Ʈ(Event())�� ȣ�����ְ� ���� ų�� ��� Ŭ���̾�Ʈ�� �˷��ش�
			3. Ŭ���̾�Ʈ�� ��Ŷ�� ��û�� ���� ����üũ�� �׼��� �Ͼ��, ������ ���ش�
	@history
			- 
*/
//=======================================================================================================================

#include <QuestManager.h>
#include "QuestCondition_Concrete.h"
#include "QuestAction_Concrete.h"

class Quest;

//������������������������������������������������������������������������������������������������������������
//	<QuestManager_Concrete>
class QuestManager_Concrete : public QuestManager
{
public:		QuestManager_Concrete( QuestInfoManager* pmgr ) : QuestManager(pmgr) {}
	virtual ~QuestManager_Concrete() { Release(); }

//	DB�� ���� ���� ��
// ����Ʈ ��ü�� �̸� ���� ���´�.Manager�� Quest�� ����صд�.
// �׸��� MonsterKillCondition�� add�ɶ� Establish�Ѵ�?
// 
//			VOID	SerializeStream( QUEST_PROGR_TOTAL_INFO & IN rTotalInfo, eQUEST_SERIALIZE eType );

// Ŭ���̾�Ʈ������ ȣ�����ش�. ���� ���������� �ð� üũ�� ���ϸ� �� �ʿ����.
//			VOID Update();

// �������� ȣ�����ش�.
//			VOID Event( eQUEST_EVENT evt, QUEST_MSG* pMsg );
//			VOID Event( QCODE QCode, eQUEST_EVENT evt, QUEST_MSG* pMsg );


// ó���Ϸ� ------------------------------------------------------------------------------------
// Ŭ���̾�Ʈ�� ���� ����üũ ��Ŷ�� �� ���� ó���� �ش�
//			BOOL CanAccept( QCODE qCode, VOID* pvParam );
//			BOOL CanComplete( QCODE qCode, VOID* pvParam );

	static	VOID			Init( WORD wPoolSize );

			VOID			Init( Player* pPlayer );
			VOID			Release();

			VOID			SendQuestStream();
			//VOID			RecvQuestStream( MSG_CG_QUEST_STREAM_CMD* pcmd );
            BOOLEAN MakeInfoQuestNpcNaviStream(MSG_CG_QUEST_OBJECT_NAVI_POS_CMD* INOUT pMsg,
                                               QCODE questCode = 0);
            BOOLEAN OnEventNpcSummonQuestAbandon();
            inline void RegisterNpcSummonEvent(MONSTERCODE npcCode, CODETYPE npcKey);
protected:
			// �ݺ��� �Ǵ� �κ�
	virtual VOID			OnEvent( eQUEST_EVENT evt, QUEST_MSG* pMsg, Quest* pQuest );

			// ���� Ŭ�������� ������ �ؾ� �ϴ� �κ�
	virtual QuestCondition*	CreateQuestCondition( QuestConditionInfo* pInfo );
	virtual QuestAction*	CreateQuestAction( QuestActionInfo* pInfo );
	virtual VOID			DestroyQuestCondition( QuestCondition* pCondition );
	virtual VOID			DestroyQuestAction( QuestAction* pAction );

	inline	VOID			SetPlayer( Player* pPlayer ) { m_pPlayer = pPlayer; }
    //__NA000000_090320_v0902_LOGIC_CHANGE_ABOUT_QUEST__
    enum eNpcSummonInfo { eNpcSummonInfo_Code, eNpcSummonInfo_Key, eNpcSummonInfo_Count };
    typedef DWORD NpcSummonInfo[eNpcSummonInfo_Count];
private:
	static	VarMsg2			m_VarMsg;
	Player*					m_pPlayer;
    NpcSummonInfo m_NpcSummonInfo;
    //
	static util::PoolRouter<eQUEST_ACTION_MAX>  ms_PoolRouter;
};


// (BUGFIX) (WAVERIX) (091018) change type for 'remove mob key' event on 'mob summon' quest abandon
// �̷�... ���� Ű �뿪�� 50001~�̴�... ����Ʈ ��ҽ� ���Ͱ� ���ŵ��� ���� ���� �־���.
inline void
QuestManager_Concrete::RegisterNpcSummonEvent(MONSTERCODE npcCode, CODETYPE npcKey)
{
    m_NpcSummonInfo[eNpcSummonInfo_Code] = npcCode;
    m_NpcSummonInfo[eNpcSummonInfo_Key] = npcKey;
}

