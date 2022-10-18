#pragma once
//=======================================================================================================================
/// QuestInfoManagerEx class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 3. 29
	@remarks
			- QuestInfo蒂 婦葬ж堅, QuestScriptLoader蒂 陛霞 諒旋 偌羹
	@history
			- 
*/
//=======================================================================================================================

#include <questinfomanager.h>
#include <QuestScriptLoader.h>
#include "QuestManager_Concrete.h"
#include "Player.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<QuestInfoManagerEx>
class QuestInfoManagerEx : public QuestInfoManager, public IParser
{
public:		QuestInfoManagerEx(VOID);
			virtual ~QuestInfoManagerEx(VOID);

			VOID			Unload();

	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );
	virtual BOOL			Load( eSCRIPT_CODE ScriptCode, const TCHAR* pszFileName, BOOL bBigData, eSEPERATOR_MODE mode, const TCHAR* pszPackFileName, BOOL bReload );					//File辨,.
	virtual BOOL			SolarLoad( eSCRIPT_CODE ScriptCode, BYTE* pSTREAM, DWORD dwSize, BOOL bBigData );

	virtual VOID				Release();

private:	QuestScriptLoader	m_ScriptLoader;
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
extern QuestInfoManagerEx		g_QuestInfoManager;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
class FindAutoAcceptQuest
{
private:	Player*				m_pPlayer;

public:		FindAutoAcceptQuest( Player* pPlayer ) : m_pPlayer(pPlayer) {}
			~FindAutoAcceptQuest() {}

			bool operator() ( QCODE QCode )
			{
				QuestInfo* pQuestInfo = g_QuestInfoManager.FindQuestInfo( QCode );
				if( !pQuestInfo )
					return false;
				return operator() ( pQuestInfo );
			}

			bool operator() ( QuestInfo* pQuestInfo )
			{
				QuestManager_Concrete* pQMGR = m_pPlayer->GetQuestManager();
				if( pQMGR->IsQuestMax() )
				{
					MSG_CG_QUEST_ACCEPT_NAK msg;
					msg.m_byErrorCode = RC::RC_QUEST_CANNOTQUEST_MAXCOUNT;
					m_pPlayer->SendPacket( &msg, sizeof(msg) );
					return false;
				}
                //
				QCODE questCode = pQuestInfo->GetQuestCode();
				Quest* pQuest = pQMGR->FindQuest( questCode );
				if( pQuest )
					return false;

				pQuest = pQMGR->CreateQuest( questCode );
				if( pQuest && pQuest->CanAccept(m_pPlayer) == RC::RC_QUEST_SUCCESS )			
				{
					MSG_CG_QUEST_ACCEPT_CMD msgCMD;
					msgCMD.m_QuestCode = (WORD)questCode;
					m_pPlayer->SendPacket( &msgCMD, sizeof(msgCMD) );
					return true;
				}

				pQMGR->DestroyQuest( questCode );
				return false;
			}
};


