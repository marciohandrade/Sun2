#pragma once

#include <StructInPacket.h>
#include <PacketControl/WarMessages.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//
namespace nsSSQ
{

#define SSQ_MSG_DECL( protocol )	\
	struct MSGSUB_##protocol : WAR_TAG1_< protocol, MSGSUB_##protocol >

	//	클라팀의 항의로 이것은 사용을 지양하기로 한다. 하나의 메소드에서 SWITCH문으로 구분하고 루핑하기로 한다.
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


	//──────────────────────────────────────────────────────────────
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
		SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN		= 10,		// 입장 가능 X분 전 공지
		SSQ_PROTOCOL_NOTICE_ENABLE_ENTER				,			// 입장 가능 공지
		SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN		,			// 입장 종료 X분 전 공지
		SSQ_PROTOCOL_NOTICE_CLOSE_PORTAL				,			// 입장 포탈 생성 종료 공지
		SSQ_PROTOCOL_NOTICE_CLOSE_ENTER					,			// 입장 종료 공지
		SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED				,			// 전투 시작 조건 부합되지 못해 전투 취소
		SSQ_PROTOCOL_NOTICE_GAMBLE_START				,			// 갬블 시작 (참가자 대상)
		SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT				,			// 갬블 종료 및 결과 공지 (참가자 대상)
		SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN			,			// 전투 시작 5초 전 (참가자 대상)
		SSQ_PROTOCOL_NOTICE_BATTLE_START				,			// 전투 시작 (참가자 대상)
		SSQ_PROTOCOL_NOTICE_BATTLE_END					,			// 전투 종료 (퀘스트 성공 및 실패 관련 정보 포함)

		SSQ_PROTOCOL_NOTICE_1TO1_PORTAL_CLOSED			,			// [개인 대상] 시간 제한에 걸려 생성된 포탈 취소
		SSQ_PROTOCOL_NOTICE_UPPERBOUND	// 이 이후로 추가되서는 안된다.
	};

	enum eG2C_TRAP_MODE {
		SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION			= SSQ_PROTOCOL_NOTICE_UPPERBOUND,	// 지진 발생 징조 예고
		SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE			,	// 해당 위치 지진 발생 및 그 결과
		SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE			,	// 번개 발생 및 그 결과
		SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE			,	// 결계 동작 및 그 결과
		SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED			,	// 결계 동작 정지 및 이펙트 제거
		SSQ_PROTOCOL_TRAP_SELF_DESTRUCTION				,	// 몬스터 자폭
		SSQ_PROTOCOL_TRAP_UPPERBOUND	// 이 이후로 추가되서는 안된다.
	};

	enum eG2C_CONTROL_MODE {
		SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION	= SSQ_PROTOCOL_TRAP_UPPERBOUND,		// 지진등의 상황으로 C/S간 위치 불일치 문제 해결 목적
		SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS				,	// 캐릭터 데미지 처리
		//GCM_ENTER_CHARACTER_INFO		= GTM_UPPERBOUND,	// 입장한 캐릭터 정보
		//GCM_LEAVE_CHARACTER_INFO		,					// 탈퇴한 캐릭터 정보
		//GCM_DEAD_CHARACTER_INFO			,					// 특정 캐릭터 저세상
		SSQ_PROTOCOL_CONTROL_UPPERBOUND					,
	};
	
	enum eG2C_INFORMATION_MODE {
		SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS		= SSQ_PROTOCOL_CONTROL_UPPERBOUND,	// 남은 인원 (참가자 대상)
		SSQ_PROTOCOL_INFORMATION_REMAINED_TIME			,	// 남은 시간(분) (참가자 대상)
		SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING		,	// 문의필요 - 전투중 경품 당첨 (참가자 대상)
		SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED		,	// 입장료 차감
		SSQ_PROTOCOL_INFORMATION_QUEST_FAIL_RESULT		,	// 퀘스트 종료시 '실패'용 메시지 처리
		SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT	,	// 퀘스트 종료시 '성공'용 메시지 처리
		SSQ_PROTOCOL_INFORMATION_UPPERBOUND				// 이 이후로 추가되서는 안된다.
	};

	enum eC2G_REQUEST_MODE {
		SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT		= SSQ_PROTOCOL_INFORMATION_UPPERBOUND,	// 관전모드 요청
		SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_G2C_SELECT	,	// G->C 캐릭터 죽은 이후 '관전모드' 선택 메시지 띄우기
		SSQ_PROTOCOL_REQUEST_UPPERBOUND					// 이 이후로 추가되서는 안된다.
	};
	BOOST_STATIC_ASSERT( SSQ_PROTOCOL_REQUEST_UPPERBOUND < SCT_S2S );

#pragma pack(push, 1)

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//	<DEFINITIONs>
	//


	//──────────────────────────────────────────────────────────────
	//	<SSQ_MSG_CTRL_TAG_TAIL>
	//
	struct SSQ_MSG_CTRL_TAG_TAIL : WAR_TAG1_< SSQ_PROTOCOL_CTRL_TAG_TAIL, SSQ_MSG_CTRL_TAG_TAIL >
	{
	};

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//	<SSQ_MSG_STRUCTUREs>
	//	-	각 메시지 구조는 여기에 정의한다.
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
		sSTRING_BUFFER_CHAR_NAME		CharName;		// 캐릭터 이름 전송
		DWORD							RewardHeim;		// 갬블로 획득한 하임액
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
		SSQPattern::INDEX_T				PatternIndex;	// 패턴 번호
		BYTE							StepArrayIndex;	// 해당 패턴의 배열 인덱스
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
		sSTRING_BUFFER_CHAR_NAME	CharName;		// 캐릭터 이름 전송
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED			)
	{	// to 1 target Player
		DWORD					ObjectKey;			// 입장료 차감 대상
		DWORD					Cost;				// 차감액 (current_money = total_money - Cost)
	};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	C->G

	SSQ_MSG_DECL(	SSQ_PROTOCOL_INFORMATION_QUEST_FAIL_RESULT			)	// 퀘스트 종료시 '실패'용 메시지 처리
	{
		BOOLEAN					bIsNotStarted;		// 전투 시작전 취소된 경우인지?
	};

	SSQ_MSG_DECL(	SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT		)	// 퀘스트 종료시 '성공'용 메시지 처리
	{
		sSTRING_BUFFER_CHAR_NAME	WinnerName;								// 최종 승자 이름
#ifdef _NA_002950_20110627_SSQ_NOTICE
        typedef util::cSTRING_BUFFER<MAX_GUILDNAME_LENGTH+1> GuildNameBuffer;
        GuildNameBuffer guild_name; // 승자가 속한 길드 이름, 무소속이면 빈문자열
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

	SSQ_MSG_DECL(	SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_G2C_SELECT	)	// G->C 캐릭터 죽은 이후 '관전모드' 선택 메시지 띄우기
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


