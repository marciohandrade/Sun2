#pragma once

#include <StructInPacket.h>
#include <PacketControl/WarMessages.h>

//��������������������������������������������������������������������������������������������������������������������������������������������������
//
//
namespace nsSSQ
{

#define SSQ_MSG_DECL( protocol )	\
	struct MSGSUB_##protocol : WAR_TAG1_< protocol, MSGSUB_##protocol >

	//	Ŭ������ ���Ƿ� �̰��� ����� �����ϱ�� �Ѵ�. �ϳ��� �޼ҵ忡�� SWITCH������ �����ϰ� �����ϱ�� �Ѵ�.
	//	#define SSQ_MSG_HANDLER_IMPL( protocol )	\
	//		BOOL	On##protocol( const MSGSUB_##protocol& rMsg )

	//	<Sample>
	//	case SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE:
	//	{
	//		RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION );
	//		// enable keyword 'rMsg'
	//		rMsg.Count ~~
#define RECV_PATTERNCODE_FOR_SSQ( subprotocol )										\
	const MSGSUB_##subprotocol& rMsg = static_cast<MSGSUB_##subprotocol&>(*pIt);	\
	size = rMsg.GetSize()


	//����������������������������������������������������������������������������������������������������������������������������
	//
	enum eTIME_SECT {
		TS_WAITING_TIME		,
		TS_QUEST_PLAY_TIME	,
	};

	enum eSSQ_CTRL_TAG {
		SCT_HDR							= 0,
		SSQ_PROTOCOL_CTRL_TAG_TAIL		= 254,		// End Of Variable Stream (specialization)
		SCT_S2S							= 200,		// Server<->Server Control Tag
	};

	enum eG2C_NOTICE_MODE {
		SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN		= 10,		// ���� ���� X�� �� ����
		SSQ_PROTOCOL_NOTICE_ENABLE_ENTER				,			// ���� ���� ����
		SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN		,			// ���� ���� X�� �� ����
		SSQ_PROTOCOL_NOTICE_CLOSE_PORTAL				,			// ���� ��Ż ���� ���� ����
		SSQ_PROTOCOL_NOTICE_CLOSE_ENTER					,			// ���� ���� ����
		SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED				,			// ���� ���� ���� ���յ��� ���� ���� ���
		SSQ_PROTOCOL_NOTICE_GAMBLE_START				,			// ���� ���� (������ ���)
		SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT				,			// ���� ���� �� ��� ���� (������ ���)
		SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN			,			// ���� ���� 5�� �� (������ ���)
		SSQ_PROTOCOL_NOTICE_BATTLE_START				,			// ���� ���� (������ ���)
		SSQ_PROTOCOL_NOTICE_BATTLE_END					,			// ���� ���� (����Ʈ ���� �� ���� ���� ���� ����)

		SSQ_PROTOCOL_NOTICE_1TO1_PORTAL_CLOSED			,			// [���� ���] �ð� ���ѿ� �ɷ� ������ ��Ż ���
		SSQ_PROTOCOL_NOTICE_UPPERBOUND	// �� ���ķ� �߰��Ǽ��� �ȵȴ�.
	};

	enum eG2C_TRAP_MODE {
		SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION			= SSQ_PROTOCOL_NOTICE_UPPERBOUND,	// ���� �߻� ¡�� ����
		SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE			,	// �ش� ��ġ ���� �߻� �� �� ���
		SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE			,	// ���� �߻� �� �� ���
		SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE			,	// ��� ���� �� �� ���
		SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED			,	// ��� ���� ���� �� ����Ʈ ����
		SSQ_PROTOCOL_TRAP_SELF_DESTRUCTION				,	// ���� ����
		SSQ_PROTOCOL_TRAP_UPPERBOUND	// �� ���ķ� �߰��Ǽ��� �ȵȴ�.
	};

	enum eG2C_CONTROL_MODE {
		SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION	= SSQ_PROTOCOL_TRAP_UPPERBOUND,		// �������� ��Ȳ���� C/S�� ��ġ ����ġ ���� �ذ� ����
		SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS				,	// ĳ���� ������ ó��
		//GCM_ENTER_CHARACTER_INFO		= GTM_UPPERBOUND,	// ������ ĳ���� ����
		//GCM_LEAVE_CHARACTER_INFO		,					// Ż���� ĳ���� ����
		//GCM_DEAD_CHARACTER_INFO			,					// Ư�� ĳ���� ������
		SSQ_PROTOCOL_CONTROL_UPPERBOUND					,
	};
	
	enum eG2C_INFORMATION_MODE {
		SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS		= SSQ_PROTOCOL_CONTROL_UPPERBOUND,	// ���� �ο� (������ ���)
		SSQ_PROTOCOL_INFORMATION_REMAINED_TIME			,	// ���� �ð�(��) (������ ���)
		SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING		,	// �����ʿ� - ������ ��ǰ ��÷ (������ ���)
		SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED		,	// ����� ����
		SSQ_PROTOCOL_INFORMATION_QUEST_FAIL_RESULT		,	// ����Ʈ ����� '����'�� �޽��� ó��
		SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT	,	// ����Ʈ ����� '����'�� �޽��� ó��
		SSQ_PROTOCOL_INFORMATION_UPPERBOUND				// �� ���ķ� �߰��Ǽ��� �ȵȴ�.
	};

	enum eC2G_REQUEST_MODE {
		SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT		= SSQ_PROTOCOL_INFORMATION_UPPERBOUND,	// ������� ��û
		SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_G2C_SELECT	,	// G->C ĳ���� ���� ���� '�������' ���� �޽��� ����
		SSQ_PROTOCOL_REQUEST_UPPERBOUND					// �� ���ķ� �߰��Ǽ��� �ȵȴ�.
	};
	BOOST_STATIC_ASSERT( SSQ_PROTOCOL_REQUEST_UPPERBOUND < SCT_S2S );

#pragma pack(push, 1)

	//����������������������������������������������������������������������������������������������������������������������������
	//	<DEFINITIONs>
	//


	//����������������������������������������������������������������������������������������������������������������������������
	//	<SSQ_MSG_CTRL_TAG_TAIL>
	//
	struct SSQ_MSG_CTRL_TAG_TAIL : WAR_TAG1_< SSQ_PROTOCOL_CTRL_TAG_TAIL, SSQ_MSG_CTRL_TAG_TAIL >
	{
	};

	//����������������������������������������������������������������������������������������������������������������������������
	//	<SSQ_MSG_STRUCTUREs>
	//	-	�� �޽��� ������ ���⿡ �����Ѵ�.
	//
	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN			)
	{	// to All Players
		SSQInfo::SSQCODE				CodeOfSSQ;
		SSQInfo::MINUTE_T				Countdown;
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_ENABLE_ENTER					)
	{	// to All Players
		SSQInfo::SSQCODE				CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN			)
	{	// to All Players
		SSQInfo::SSQCODE				CodeOfSSQ;
		SSQInfo::MINUTE_T				Countdown;
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_CLOSE_PORTAL					)
	{	// to All Players
		SSQInfo::SSQCODE				CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_CLOSE_ENTER						)
	{	// to All Players
		SSQInfo::SSQCODE				CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_1TO1_PORTAL_CLOSED				)
	{	// to 1 Player
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED					)
	{	// to SSQ Players
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_GAMBLE_START					)
	{	// to SSQ Players
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT					)
	{	// to SSQ Players
		sSTRING_BUFFER_CHAR_NAME		CharName;		// ĳ���� �̸� ����
		DWORD							RewardHeim;		// ����� ȹ���� ���Ӿ�
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN				)
	{	// to SSQ Players
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_BATTLE_START					)
	{	// to SSQ Players
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_NOTICE_BATTLE_END						)
	{	// to SSQ Players
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION				)
	{	// to SSQ Players
		SSQPattern::INDEX_T				PatternIndex;	// ���� ��ȣ
		BYTE							StepArrayIndex;	// �ش� ������ �迭 �ε���
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE				)
	{	// to SSQ Players
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE				)
	{	// to SSQ Players
		static const DWORD		MAX_SIZE = 8;
		SSQInfo::COUNT_T		Count;
		WzVector				Position[MAX_SIZE];
		inline	WORD			GetSize() const { return (WORD)( sizeof(*this) - sizeof(WzVector)*(MAX_SIZE-Count) ); }
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE				)
	{	// to SSQ Players
		static const DWORD		MAX_SIZE = SSQ_MAX_CHARACTERS;
		SSQInfo::COUNT_T		Count;
		DAMAGE_INFO				Damages[MAX_SIZE];
		inline	WORD			GetSize() const { return (WORD)( sizeof(*this) - sizeof(DAMAGE_INFO)*(MAX_SIZE-Count) ); }
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED				)
	{
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_TRAP_SELF_DESTRUCTION					)
	{	// to SSQ Players
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION		)
	{
		static const DWORD		MAX_SIZE = 200;
		SSQInfo::COUNT_T		Count;
		struct NODE
		{
			DWORD				ObjectKey;
			WzVector			Position;
		}						PositionInfo[MAX_SIZE];
		inline	WORD			GetSize() const { return (WORD)( sizeof(*this) - sizeof(NODE)*(MAX_SIZE-Count) ); }
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS					)
	{
		static const DWORD		MAX_SIZE = SSQ_MAX_CHARACTERS;
		SSQInfo::COUNT_T		Count;
		DAMAGE_INFO				Damages[MAX_SIZE];
		inline	WORD			GetSize() const { return (WORD)( sizeof(*this) - sizeof(DAMAGE_INFO)*(MAX_SIZE-Count) ); }
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS			)
	{	// to SSQ Players
		SSQInfo::COUNT_T		Remains;	//
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_INFORMATION_REMAINED_TIME				)
	{	// to SSQ Players
		SSQInfo::MINUTE_T		Minute;		// remained minute
		SSQInfo::SECOND_T		Second;
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING			)
	{	// to SSQ Players
		sSTRING_BUFFER_CHAR_NAME	CharName;		// ĳ���� �̸� ����
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED			)
	{	// to 1 target Player
		DWORD					ObjectKey;			// ����� ���� ���
		DWORD					Cost;				// ������ (current_money = total_money - Cost)
	};


//����������������������������������������������������������������������������������������������������������������������
//	C->G

	SSQ_MSG_DECL(	SSQ_PROTOCOL_INFORMATION_QUEST_FAIL_RESULT			)	// ����Ʈ ����� '����'�� �޽��� ó��
	{
		BOOLEAN					bIsNotStarted;		// ���� ������ ��ҵ� �������?
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT		)	// ����Ʈ ����� '����'�� �޽��� ó��
	{
		sSTRING_BUFFER_CHAR_NAME	WinnerName;								// ���� ���� �̸�
#ifdef _NA_002950_20110627_SSQ_NOTICE
        typedef util::cSTRING_BUFFER<MAX_GUILDNAME_LENGTH+1> GuildNameBuffer;
        GuildNameBuffer guild_name; // ���ڰ� ���� ��� �̸�, ���Ҽ��̸� ���ڿ�
#endif
	};

	//SSQ_MSG_DECL(	SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL					)
	//{
	//	typedef nsSSQ::Observer::Info	Info;
	//	Info::eRequest				Cmd;
	//	union {
	//		struct {
	//			Info::eKeyCmd		PressedKey;
	//		}						ObserverCmd;
	//	};

	//	inline	VOID				Init()
	//	{
	//		Cmd						= Info::eRequest_None;
	//		ObserverCmd.PressedKey	= Info::eKeyCmd_Max;
	//	}
	//};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT		)
	{
		typedef nsSSQ::Observer::Info	Info;
		enum eNtfCmd {
			eNtfCmd_None					,
			eNtfCmd_ResultByRequest_Fail	,
			eNtfCmd_ResultByRequest_Success	,
		};
		BYTE						Result;
		BYTE						Coordinates;		//Info::eCoordinates

		inline	VOID				Init()
		{
			Result		= eNtfCmd_None;
			Coordinates	= Info::eCoord_Slots;
		}
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_G2C_SELECT	)	// G->C ĳ���� ���� ���� '�������' ���� �޽��� ����
	{
	};
	////	Client Packet Serializer
	//struct WarPacketAssembler
	//{
	//	static	WORD			msg_size;
	//	static	BYTE			msg_stream[4096];

	//	template< typename _MSG, typename _MSGSUB >
	//	static VOID				MakePacket( const _MSGSUB& rMsg )
	//	{
	//		typedef _MSG		message;
	//		typedef _MSGSUB		sub_message;

	//		message* const pMsgRoot = new (msg_stream) message;
	//		pMsgRoot->m_Header.size = 0;
	//		rMsg.SerializeTo( &pMsgRoot->m_Header );

	//		msg_size = pMsgRoot->GetSize();
	//	}
	//};


#pragma pack(pop)
}; // end namespace nsSSQ


