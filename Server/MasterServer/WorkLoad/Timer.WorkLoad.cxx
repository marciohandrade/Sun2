// embedded compile  "MasterServerInfo.h"

#include <PacketStruct_ServerCommon.h>

// �ٸ� ��忡�� ���񽺷� ��ȯ�Ǿ��� ��, �۾��� ����
class PROCESS_TIMER_PROCESS : public IWorkNode
{
public:		static const DWORD SECOND						= 1000;
			static const DWORD MINUTE						= 60*SECOND;
			static const DWORD UPDATE_INTERVAL_CCU_REPORT	= 10*MINUTE;
	
public:
			PROCESS_TIMER_PROCESS() {}
			~PROCESS_TIMER_PROCESS() {}

public:
public:
			// <INTERFACE><IMPLEMENTATION>
			virtual BOOL	DoInit()
			{
				m_TimerCCUReport = GetTickCount() + UPDATE_INTERVAL_CCU_REPORT;
				return TRUE;
			}

			virtual BOOL	DoProcess()
			{
				DWORD dwCurTick = GetTickCount();

				if( !(m_TimerCCUReport < dwCurTick) )
					return TRUE;

				m_TimerCCUReport = GetTickCount() + UPDATE_INTERVAL_CCU_REPORT;

				DWORD dwMaxWorldID = 0;
				MSG_SM_SERVER_INFO_CCU_REPORT_NTF msgNTF;
				msgNTF.Clear();
				DWORD* pCCUs = msgNTF.m_INFO;

				ServerTreeNODE* pTREE = ServerSessionManager::GetServerTree();
				DWORD rdwUpperBound;
				sSERVER_NODE* pNODEs = pTREE->GetServerArrayList( rdwUpperBound );
				sSERVER_NODE* pNODE = pNODEs;

				for( DWORD i=0 ; i<rdwUpperBound ; ++i, ++pNODE )
				{
					DWORD dwServerTID_T = (DWORD)(1<<pNODE->m_ServerType);
					if( !((1<<AGENT_SERVER) & dwServerTID_T) )
						continue;

					if( !pNODE->m_pServerSession )
						continue;

					SERVER_KEY& rKey = pNODE->m_ServerKey;
					DWORD dwWorldID = rKey.GetWorldID();
					// ASSERT ( worldID < 10 )	0���� �����ϴ��� 1���� �����ϴ���?
					AgentServerSession* pAGENT = (AgentServerSession*)pNODE->m_pServerSession;
					if( MSG_SM_SERVER_INFO_CCU_REPORT_NTF::MAX_WORLD_CCU_LIST_SIZE > dwWorldID )
					{
						pCCUs[dwWorldID] += pAGENT->GetCCU();
						dwMaxWorldID = dwMaxWorldID < dwWorldID ? dwWorldID : dwMaxWorldID;
					}
					else
					{
						MessageOut( eCRITICAL_LOG, _T("Index Check WorldID %u"), dwWorldID );
					}
				}

				// overflow�߻��ϴ����� solarAuth���� ó���ϱ�� ����. ��? ����
				msgNTF.byCount = (BYTE)(dwMaxWorldID+1);

				MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
                if( pMasterServer->GetPhaseStatus() == PHASE_SERVICE ) // �ʱ� ���� �÷ο� ���� ������
					pMasterServer->SendToSessionType( SOLARAUTH_SERVER, &msgNTF, msgNTF.GetSize() );

				return TRUE;
			}

			virtual BOOL	IsComplete()
			{
				return FALSE;
			}

			virtual BOOL	DoRelease()
			{
				return TRUE;
			}

			virtual VOID	OnMsg( sSIG_MSG* pSIG_MSG );

private:
			DWORD			m_TimerCCUReport;
};




VOID	PROCESS_TIMER_PROCESS::OnMsg( sSIG_MSG* pSIG_MSG )
{
}

//
//
//
//
//
//












