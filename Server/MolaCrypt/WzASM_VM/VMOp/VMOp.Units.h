#pragma once

#include "VMOp.Type.h"
//����������������������������������������������������������������������������
//

//����������������������������������������������������������������������������
//
//	enum eMOLASIG_TYPE
//	{
//		SIGMSG_NORMAL = 0x4400
//	};
//
//	struct eMOLASIG_MSG
//	{
//		eMOLASIG_TYPE	_TYPE;
//	};
//
#pragma pack(push, 1)

struct SIGMSG
{
	//	MOLA_RCG_T1_xxx
	static const eMOLASIG_TYPE	CONTROL_STOP			= (eMOLASIG_TYPE)MOLA_RCG_T1_020;
	static const eMOLASIG_TYPE	CONTROL_START			= (eMOLASIG_TYPE)MOLA_RCG_T1_021;
	static const eMOLASIG_TYPE	NOTIFY_HEARTBEAT		= (eMOLASIG_TYPE)MOLA_RCG_T1_023;
	static const eMOLASIG_TYPE	NETWORK_MSG_RECVED		= (eMOLASIG_TYPE)MOLA_RCG_T1_024;
	static const eMOLASIG_TYPE	NETWORK_MSG_SENDING		= (eMOLASIG_TYPE)MOLA_RCG_T1_025;
	static const eMOLASIG_TYPE	NOTIFY_WORKNODE			= (eMOLASIG_TYPE)MOLA_RCG_T1_026;
};

struct sSIG_CONTROL_START : eMOLASIG_MSG
{
	sSIG_CONTROL_START()
	{
		_TYPE = SIGMSG::CONTROL_START;
	}
};

struct sSIG_CONTROL_STOP : eMOLASIG_MSG
{
	sSIG_CONTROL_STOP()
	{
		_TYPE = SIGMSG::CONTROL_STOP;
	}
};


struct sSIG_NOTIFY_HEARTBEAT : eMOLASIG_MSG
{
	sSIG_NOTIFY_HEARTBEAT( DWORD dwCount ) : m_dwCount(dwCount)
	{
		_TYPE = SIGMSG::NOTIFY_HEARTBEAT;
	}

	DWORD		m_dwCount;
};

struct sSIG_NETWORK_NTF : eMOLASIG_MSG
{
	sSIG_NETWORK_NTF( eMOLASIG_TYPE sigMSG, MSG_BASE* pMSG, WORD wSIZE ) : m_pMSG(pMSG), m_wSIZE(wSIZE)
	{
		_TYPE = sigMSG;
	}

	MSG_BASE*	m_pMSG;
	WORD		m_wSIZE;
};

struct sSIG_NETWORK_RECVED_NTF : sSIG_NETWORK_NTF
{
	sSIG_NETWORK_RECVED_NTF( MSG_BASE* pMSG ) : sSIG_NETWORK_NTF(SIGMSG::NETWORK_MSG_RECVED, pMSG, 0) {}
};

struct sSIG_NETWORK_SENDING_NTF : sSIG_NETWORK_NTF
{
	sSIG_NETWORK_SENDING_NTF( MSG_BASE* pMSG, WORD wSIZE ) : sSIG_NETWORK_NTF(SIGMSG::NETWORK_MSG_SENDING, pMSG, wSIZE) {}
};

//����������������������������������������������������������������������������
// IWzASM CmdBlock


struct sCONTROL_INFORMATION
{
	BYTE		TRANSACTION_SENDING_PACKET;		// ��Ŷ ������ ���� �ִ� ���, LOCK ��
	BYTE		ENABLE_SEND_PACKET;				// ��Ŷ ������ �������� ����
	BYTE		IS_CONNECTED_WITH_GAMESERVER;	// ���Ӽ����� ���� ����
	BYTE		LOCATED_BATTLESCENE;			// Client���忡�� BattleScene�� �����ִ����� ����
	DWORD		MSGROUNDKEYforSENDING;			// �޽��� ���۽� ÷�εǴ� Ű
	BYTE*		MSGforSENDING;					// ������ �ִ� ��� ���� ��Ŷ ������
	WORD		MSGSZforSENDING;				// ������ �ִ� ��� ���� ��Ŷ ũ��
};


#pragma pack(pop)

class VMOpCPU;

//����������������������������������������������������������������������������
//	<IVMOpGenerater>
class IVMOpGenerater : public IVMOp
{
public:		static const DWORD	SECOND						= 1000;
			static const DWORD	MINUTE						= 60*SECOND;
			static const DWORD	INTERVAL_MOLA_HEARTBEAT		= 1*SECOND;		// ��Ŷ�� ���� �� �ִ� ������ �ѹ� ��Ŷ�� �����Ѵ�.

public:		IVMOpGenerater();
			~IVMOpGenerater();
			virtual BOOL			DoInit();
			virtual BOOL			DoProcess();
			virtual BOOL			IsComplete();
			virtual BOOL			DoRelease();
			// Control Interface
			virtual VOID			OnMsg( eMOLASIG_MSG* pSIG_MSG );

public:		inline BOOL&			Enabled() { return m_bEnabled; }
			inline DWORD			GetUpdateCount() { return m_dwUpdateCount; }


public:		// <FIELDs>
			BOOL					m_bEnabled;					// DoProcess ���࿩��
			ITimeout				m_UpdateInterval;			// �� 1���� Heartbeat
			DWORD					m_dwUpdateLoopCount;		// DoProcess ȣ�� ��
			DWORD					m_dwUpdateCount;			// Timeout ������
			WzASM_EIP*				m_SetNextStartAction;
			WzASM_EIP*				m_SetEnable;				//
};

//����������������������������������������������������������������������������
//	<IVMOpScheduler>
class IVMOpScheduler : public IVMOp
{
public:		static const DWORD	SECOND						= 1000;
			static const DWORD	MINUTE						= 60*SECOND;
			static const DWORD	INTERVAL_CHECKER			= 3*SECOND;
			static const DWORD	INTERVAL_SEND_DELAY			= 5*SECOND;
			static const DWORD	INTERVAL_MOLA_HEARTBEAT		= 1*MINUTE;		// ��Ŷ�� ���� �� �ִ� ������ �ѹ� ��Ŷ�� �����Ѵ�.

public:		IVMOpScheduler();
			~IVMOpScheduler();
			virtual BOOL			DoInit();
			virtual BOOL			DoProcess();
			virtual BOOL			IsComplete();
			virtual BOOL			DoRelease();
			// Control Interface
			virtual VOID			OnMsg( eMOLASIG_MSG* pSIG_MSG );

public:		inline BOOL&			Enabled() { return m_bEnabled; }

public:		static uREG_PV			HANDLER_IBF_MakeSendRoundKey( uREG_NODE* pNODE );	// �ѹ� ���� ������ ���ŵǴ� ��
			static uREG_PV			HANDLER_IBF_VerifyObject( uREG_NODE* pNODE );		// ������Ʈ ���Ἲ �˻�

public:		// <FIELDs>
			BOOL						m_bEnabled;					// DoProcess ���࿩��
			ITimeout					m_CheckInterval;			// �� ��ƾ�� �˻縦 �����ϴ� ����
			ITimeout					m_SendIntervalBySendMsg;	// �޽��� ������ �ʿ��� ���
			WzASM_EIP*					m_EIPCheckInterval;
			WzASM_EIP*					m_EIPSendIntervalBySendMsg;

			sCONTROL_INFORMATION*		m_pCONTROL_INFO;			// ��Ŷ ��Ʈ�� ����
			struct {
				DWORD					m_dwHBLastedUpdateTick;		// ���� �ֱٿ� ���ŵ� ƽ
				DWORD					m_dwHBIncreaseCount;		// ���ŵ� ȸ��
				DWORD					m_dwHBRandomKey;			// ���ŵ� ����Ű
			};
};

//����������������������������������������������������������������������������
//	<IVMOpPPU>	- Passive Processor Unit
class IVMOpPPU : public IVMOp
{
public:		IVMOpPPU();
			~IVMOpPPU();
			virtual BOOL			DoInit();
			virtual BOOL			DoProcess();
			virtual BOOL			IsComplete();
			virtual BOOL			DoRelease();
			// Control Interface
			virtual VOID			OnMsg( eMOLASIG_MSG* pSIG_MSG );

public:		inline BOOL&			Enabled() { return m_bEnabled; }
public:		// <FIELDs>
			BOOL					m_bEnabled;					// DoProcess ���࿩��

};

