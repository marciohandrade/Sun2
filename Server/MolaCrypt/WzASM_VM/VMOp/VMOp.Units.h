#pragma once

#include "VMOp.Type.h"
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// IWzASM CmdBlock


struct sCONTROL_INFORMATION
{
	BYTE		TRANSACTION_SENDING_PACKET;		// 패킷 전송할 것이 있는 경우, LOCK 용
	BYTE		ENABLE_SEND_PACKET;				// 패킷 전송이 가능한지 여부
	BYTE		IS_CONNECTED_WITH_GAMESERVER;	// 게임서버와 접속 여부
	BYTE		LOCATED_BATTLESCENE;			// Client입장에서 BattleScene에 속해있는지의 여부
	DWORD		MSGROUNDKEYforSENDING;			// 메시지 전송시 첨부되는 키
	BYTE*		MSGforSENDING;					// 보낼수 있는 경우 보낼 패킷 포인터
	WORD		MSGSZforSENDING;				// 보낼수 있는 경우 보낼 패킷 크기
};


#pragma pack(pop)

class VMOpCPU;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<IVMOpGenerater>
class IVMOpGenerater : public IVMOp
{
public:		static const DWORD	SECOND						= 1000;
			static const DWORD	MINUTE						= 60*SECOND;
			static const DWORD	INTERVAL_MOLA_HEARTBEAT		= 1*SECOND;		// 패킷을 보낼 수 있는 시점에 한번 패킷을 전송한다.

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
			BOOL					m_bEnabled;					// DoProcess 수행여부
			ITimeout				m_UpdateInterval;			// 매 1초의 Heartbeat
			DWORD					m_dwUpdateLoopCount;		// DoProcess 호출 수
			DWORD					m_dwUpdateCount;			// Timeout 만족수
			WzASM_EIP*				m_SetNextStartAction;
			WzASM_EIP*				m_SetEnable;				//
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<IVMOpScheduler>
class IVMOpScheduler : public IVMOp
{
public:		static const DWORD	SECOND						= 1000;
			static const DWORD	MINUTE						= 60*SECOND;
			static const DWORD	INTERVAL_CHECKER			= 3*SECOND;
			static const DWORD	INTERVAL_SEND_DELAY			= 5*SECOND;
			static const DWORD	INTERVAL_MOLA_HEARTBEAT		= 1*MINUTE;		// 패킷을 보낼 수 있는 시점에 한번 패킷을 전송한다.

public:		IVMOpScheduler();
			~IVMOpScheduler();
			virtual BOOL			DoInit();
			virtual BOOL			DoProcess();
			virtual BOOL			IsComplete();
			virtual BOOL			DoRelease();
			// Control Interface
			virtual VOID			OnMsg( eMOLASIG_MSG* pSIG_MSG );

public:		inline BOOL&			Enabled() { return m_bEnabled; }

public:		static uREG_PV			HANDLER_IBF_MakeSendRoundKey( uREG_NODE* pNODE );	// 한번 보낼 때마다 갱신되는 값
			static uREG_PV			HANDLER_IBF_VerifyObject( uREG_NODE* pNODE );		// 오브젝트 무결성 검사

public:		// <FIELDs>
			BOOL						m_bEnabled;					// DoProcess 수행여부
			ITimeout					m_CheckInterval;			// 이 루틴이 검사를 시작하는 시점
			ITimeout					m_SendIntervalBySendMsg;	// 메시지 전송이 필요할 경우
			WzASM_EIP*					m_EIPCheckInterval;
			WzASM_EIP*					m_EIPSendIntervalBySendMsg;

			sCONTROL_INFORMATION*		m_pCONTROL_INFO;			// 패킷 컨트롤 정보
			struct {
				DWORD					m_dwHBLastedUpdateTick;		// 가장 최근에 갱신된 틱
				DWORD					m_dwHBIncreaseCount;		// 갱신된 회수
				DWORD					m_dwHBRandomKey;			// 갱신된 랜덤키
			};
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
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
			BOOL					m_bEnabled;					// DoProcess 수행여부

};

