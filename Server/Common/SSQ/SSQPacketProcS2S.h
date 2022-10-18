#pragma once

#include <SSQ/SSQInfoStruct.h>
#include <PacketStruct_AG.h>
#include <PacketStruct_AW.h>

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//
namespace nsSSQ
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//	<>
	//
	enum eCTRL_MODE_S2S {
		SSQ_S2S_FIELD_CREATE				= SCT_S2S,
		SSQ_S2S_FIELD_CREATED				,
		SSQ_S2S_FIELD_BLOCK					,	// 종료전 상태 복원 처리 및 더 이상의 서비스 불가 상태 처리
		SSQ_S2S_FIELD_DESTORY				,
		SSQ_S2S_FIELD_DESTORYED				,
		SSQ_S2S_GAMBLE_START				,
		SSQ_S2S_GAMBLE_RESULT				,
		SSQ_S2S_BATTLE_COUNTDOWN			,
		SSQ_S2S_BATTLE_PREPARATION			,	// A->G 전투 관련 준비 (몬스터생성 등등)
		SSQ_S2S_BATTLE_START				,
		SSQ_S2S_BATTLE_CANCEL				,
		SSQ_S2S_BATTLE_END_REQUEST			,	// G->A 전투 종료 요청 (시간에 의한 경우가 아닌 상황에서...) SSQ_S2S_BATTLE_END가 돌아올 것이다.
		SSQ_S2S_BATTLE_END					,	// 전투 종료
		SSQ_S2S_BATTLE_CLEAR				,
		SSQ_S2S_BATTLE_CLEARED				,	// G->A 종료처리 요청


		SSQ_S2S_SET_CHAT_BLOCK_ZONE			,	// WorldServer에 해당 존은 Chatting이 되지 않도록/되도록 설정 요청
		SSQ_S2S_GM_CONTROL_REQ				,	// GM Control

		SSQ_S2S_CMS_UPPERBOUND
	};
	enum eCTRL_SUB_S2S {
		SSQ_S2S_ZONE_SSQ_JOIN_NTF			= SSQ_S2S_CMS_UPPERBOUND,	// Battle->Agent
		SSQ_S2S_OBSERVER_MODE_CTRL			,
		SSQ_S2S_OBSERVER_MODE_RESULT		,
		SSQ_S2S_OBSERVER_MODE_SELECT_WINDOW	,
		SSQ_S2S_CSS_UPPERBOUND
	};

	BOOST_STATIC_ASSERT( SSQ_S2S_CSS_UPPERBOUND < 256 );

#undef SSQ_MODE_NODE
#define SSQ_S2S_MODE_LIST()										\
	SSQ_MODE_NODE(	SSQ_S2S_FIELD_CREATE				)		\
	SSQ_MODE_NODE(	SSQ_S2S_FIELD_CREATED				)		\
	SSQ_MODE_NODE(	SSQ_S2S_FIELD_DESTORY				)		\
	SSQ_MODE_NODE(	SSQ_S2S_FIELD_DESTORYED				)		\
	SSQ_MODE_NODE(	SSQ_S2S_GAMBLE_START				)		\
	SSQ_MODE_NODE(	SSQ_S2S_BATTLE_COUNTDOWN			)		\
	SSQ_MODE_NODE(	SSQ_S2S_BATTLE_START				)		\
	SSQ_MODE_NODE(	SSQ_S2S_BATTLE_CANCEL				)		\
	SSQ_MODE_NODE(	SSQ_S2S_BATTLE_CLEAR				)		\
	SSQ_MODE_NODE(	SSQ_S2S_ZONE_SSQ_JOIN_NTF			)


	//////////////////////////////////////////////////////////////////////////
	//
	struct GM_Spec
	{
		enum eCategory {
			eCategory_SSQ,			//ssq관련 gm command
			eCategory_upperbound,
		};

		enum eCmd {
			eCmd_open,
			eCmd_close,
			eCmd_warp,
			eCmd_status,
			eCmd_jump,
			eCmd_summon,
			eCmd_observer,
			eCmd_upperbound,
		};

        enum eOption {
            eOption_None        = 0,
            eOption_FastTrack   = 1 << 0,
        };

		typedef TCHAR						String[32];
		typedef String						StringArray[5];

		struct Category
		{
			eCategory		Cate;
			String			CateString;
		};

		struct SSQ
		{
			//	<FIELDs>
			eCmd			GM_Cmd;
			String			CmdString;

			//	<FORMATs>
			struct Open
			{
			};

			struct Close
			{
			};
		};

		struct MsgFormat
		{
			eCategory			Cate;
			eCmd				GM_Cmd;
			SSQInfo::SSQCODE	CodeOfSSQ;
			DWORD				GMObjectKey;
            eOption             Option;
		};
	};

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//	<SSQ_MSG_STRUCTUREs>
	//	-	서버간 주고 받는 메시지 구조는 여기에 정의한다.
	//
	SSQ_MSG_DECL(	SSQ_S2S_FIELD_CREATE		)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;		// 모든 패킷의 ZoneKey는 m_dwKey
	};

	SSQ_MSG_DECL(	SSQ_S2S_FIELD_CREATED		)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
		KEYTYPE						m_ZoneKey;
	};

	SSQ_MSG_DECL(	SSQ_S2S_FIELD_BLOCK			)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_FIELD_DESTORY		)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_FIELD_DESTORYED		)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
		KEYTYPE						m_ZoneKey;
	};

	SSQ_MSG_DECL(	SSQ_S2S_GAMBLE_START		)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_GAMBLE_RESULT		)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
		DWORD						RewardHeim;		//	갬블로 획득한 하임액
		sSTRING_BUFFER_CHAR_NAME	CharName;		// 전체 공지이므로 캐릭터 이름 전송
	};

	SSQ_MSG_DECL(	SSQ_S2S_BATTLE_COUNTDOWN	)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_BATTLE_PREPARATION	)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_BATTLE_START		)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_BATTLE_CANCEL		)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_BATTLE_END_REQUEST	)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_BATTLE_END			)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_BATTLE_CLEAR		)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_BATTLE_CLEARED		)
	{
		SSQInfo::SSQCODE			CodeOfSSQ;
	};

	SSQ_MSG_DECL(	SSQ_S2S_SET_CHAT_BLOCK_ZONE	)
	{
		DWORD						ZoneKey;
		BOOLEAN						Enabled;
	};

	SSQ_MSG_DECL(	SSQ_S2S_GM_CONTROL_REQ		)
	{
		GM_Spec::MsgFormat			Command;
	};

	SSQ_MSG_DECL(	SSQ_S2S_ZONE_SSQ_JOIN_NTF	)	// 동기화 목적으로 빠른 처리를 수행할 수 있어야 한다.
	{
		DWORD						ObjectKey;
		SLOTCODE					ItemCode;
	};

	SSQ_MSG_DECL(	SSQ_S2S_OBSERVER_MODE_CTRL	)	// 동기화 목적으로 빠른 처리를 수행할 수 있어야 한다.
	{
		typedef nsSSQ::Observer::Info	Info;

		DWORD						ObjectKey;		// 요청한 플레이어
		Info::eRequest				Request;
		union {
			struct {
				Info::eKeyCmd			PressedKey;
			}						ObserverCmd;
		};

		inline	VOID				Init()
				{
					Request					= Info::eRequest_None;
					ObserverCmd.PressedKey	= Info::eKeyCmd_Max;
				}
	};

	SSQ_MSG_DECL(	SSQ_S2S_OBSERVER_MODE_RESULT		)
	{
		DWORD												ObjectKey;
		MSGSUB_SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT	C2GMsg;
	};

	SSQ_MSG_DECL(	SSQ_S2S_OBSERVER_MODE_SELECT_WINDOW	)
	{
		DWORD						ObjectKey;
		
	};

	////━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	////	<SSQ_MSG_ROUTER_CLIENT>
	////	-	Client 지원용 기본 타입

	//template< typename _DERIVED_T, typename _STORAGE_T >
	//struct SSQMsgRouter : public WarMsgRouter< _DERIVED_T, _STORAGE_T, SSQMsgRouter<_DERIVED_T, _STORAGE_T> >
	//{
	//	typedef _DERIVED_T												sub_type;
	//	typedef _STORAGE_T												storage_type;
	//	typedef SSQMsgRouter< sub_type, storage_type >					this_type;
	//	typedef WarMsgRouter< sub_type, storage_type, this_type >		base_type;

	//	friend struct base_type;
	//public:
	//	inline	SSQInfo::COUNT_T	AlignNewNode( const DWORD objectKey )
	//	{
	//		SSQInfo::COUNT_T regIndex = GetCount();
	//		if( FlowControl::FCAssert( regIndex < SSQ_MAX_CHARACTERS ) )
	//		{
	//			regIndex = m_FreeSlots.back();
	//			sNODE& rNewNode = m_CharInfos[regIndex];
	//			rNewNode.SetObjectKey( objectKey );
	//			rNewNode.SetStorage( storage_info::Allocate() );

	//			m_MatchTable[objectKey] = regIndex;
	//			m_FreeSlots.pop_back();
	//			return regIndex;
	//		}

	//		return SSQ_MAX_CHARACTERS;	// error
	//	}
	//	BOOLEAN			Routing( const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header );
	//};

};

namespace nsSSQ_Util
{
	using namespace ::nsSSQ;
#pragma pack(push, 1)
	//	<MSG_SSQ_CHUNKBLOCK>
	//	-	Server에서 가변 패킷 데이터 스트림 제작시 사용하는 버퍼 블록
	//	<Format>
	//	──────────────────────────────────────────────────
	//	|				   MSG_BASE					| WAR_MSG_HDR	|	SSQ_SUB_MESSAGEs
	//	|	dword(key) | byte(cate) | byte(proto)	| short(size)	|	byte stream[xxxx] ...
	//	──────────────────────────────────────────────────
	struct MSG_SSQ_CHUNKBLOCK : MSG_BASE
	{
		WAR_MSG_HDR		m_Header[1];
		BYTE			m_Stream[MAX_WAR_MSG_SEQUENCIAL_STREAM_SIZE];

        uint16_t GetSize() const {
            return static_cast<uint16_t>(sizeof(MSG_BASE) + sizeof(m_Header) + (m_Header->size));
        }
	};

	struct MSG_SSQ_LIGHTBLOCK : MSG_BASE
	{
		WAR_MSG_HDR		m_Header[1];
		BYTE			m_Stream[128];
        uint16_t GetSize() const {
            return static_cast<uint16_t>(sizeof(MSG_BASE) + sizeof(m_Header) + (m_Header->size));
        }
	};
#pragma pack(pop)

	template< typename _MSG_CASTER, typename _PACKET_T >
	class xxxSender	// Broadcaster, Unicaster
	{
	public:
		
	protected:
		typedef _MSG_CASTER					derived_type;
		typedef _PACKET_T					packet_structure;
		typedef xxxSender< derived_type, packet_structure >	super_type;

	public:		xxxSender()
					: m_pPacket( new (&m_PacketStream) packet_structure )
					, MSG_HEADER( m_pPacket->m_Header[0] )
					//, m_pMsgHeadPtr( (WAR_TAG_P)&m_pPacket->m_Header[1] )
				{
					m_pPacket->m_Header->size = 0;
				};
	public:
		inline	VOID						SetKey( const DWORD userKey ) { m_pPacket->m_dwKey = userKey; }
		inline	VOID						Reset() { new (this) super_type; }
		//inline	WAR_TAG_P					GetEndPointer()	{ return m_pMsgHeadPtr + MSG_HEADER.size; }

	private:	MSG_SSQ_CHUNKBLOCK			m_PacketStream;
	protected:	packet_structure* const		m_pPacket;	// 상속 개체 연속성 목적의 배치구조
	public:		WAR_MSG_HDR&				MSG_HEADER;
	//private:	WAR_TAG_P					m_pMsgHeadPtr;
        __DISABLE_COPY(super_type);
	};


	template< typename _PACKET_T, typename _SUBMSG >
	class WAR_MSG_SINGLE_ASSEMBLER
	{
	protected:
		typedef _PACKET_T												packet_structure;
		typedef _SUBMSG													message;
		typedef WAR_MSG_SINGLE_ASSEMBLER< packet_structure, message >	super_type;
	public:		WAR_MSG_SINGLE_ASSEMBLER()
					: m_pPacket( new (&m_PacketStream) packet_structure )
					, MSG( reinterpret_cast<message&>( m_pPacket->m_Header[1] ) )
					, m_bProced( FALSE )
				{
					BOOST_STATIC_ASSERT( sizeof(packet_structure)<=sizeof(MSG_SSQ_LIGHTBLOCK) );
					m_pPacket->m_Header->size = 0;
					MSG._TAG = MSG.value;
				};
	public:		// m_dwKey는 개별적인 처리에 맏긴다.
		inline	packet_structure* const		GetPacket() { return m_pPacket; }
		inline	WORD						GetSize() const { return m_pPacket->GetSize(); }
		inline	VOID						Completion() { m_pPacket->m_Header->size = MSG.GetSize(); }
		inline	BOOLEAN						SerializeFrom( const message& rSrc )
				{
					if( m_bProced )
						return FALSE;
					m_bProced = TRUE;
					WAR_MSG_HDR* const pHeader = m_pPacket->m_Header;
					return rSrc.SerializeTo( pHeader );
				}
	protected:	packet_structure* const		m_pPacket;
	public:		message&					MSG;
	protected:	BOOLEAN						m_bProced;
	private:	MSG_SSQ_LIGHTBLOCK			m_PacketStream;
        __DISABLE_COPY(super_type);
	};

//#if (0)
//	template< typename _DRIVED, typename _PACKET >
//	class Broadcaster
//	{
//	protected:
//		typedef _DRIVED									sub_type;
//		typedef _PACKET									packet_structure;
//		typedef Broadcaster<sub_type, packet_structure>	this_type;
//
//	public:
//		Broadcaster()
//		{
//			m_pPacket = new (&m_PacketStream) packet_structure;
//			m_pPacket->m_Header->size = sizeof(WAR_MSG_HDR);
//			m_PacketSize = m_pPacket->GetSize();
//		}
//
//	private:
//		MSG_SSQ_CHUNKBLOCK			m_PacketStream;
//
//	protected:
//		WORD						m_PacketSize;
//		packet_structure*			m_pPacket;
//
//	public:
//		inline VOID					SetKey( const DWORD key ) { m_pPacket->m_dwKey = key; }
//		WAR_TAG_P					MakeInit_SSQPacketStream( SSQ_MSG_SCT_HDR*& OUT rpHdr )
//		{
//			rpHdr = (SSQ_MSG_SCT_HDR*)m_PacketStream.m_Header;
//			rpHdr->hdr.size = 0;
//			WAR_TAG_P pOffset = (WAR_TAG_P)rpHdr;
//			pOffset = SSQ_MSG_SCT_HDR::Serialize( *rpHdr, (WAR_TAG_P)rpHdr );
//
//			return pOffset;
//		}
//	};
//#endif

};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<TransactionVTLS>
//	-	in Process Object Transaction Mechanism
//	-	_VTLS (Virtual Thread Local Storage) - singleton object
//
template< typename _VTLS >
class TransactionVTLS
{
public:
	typedef _VTLS				vtls_type;
	typedef vtls_type*			vtls_pointer;
public:		TransactionVTLS() {}
public:
	inline	VOID				Enter( vtls_pointer pvtls ) { ms_bTrans=true; ms_pVTLS=pvtls; }
	inline	VOID				Leave() { ms_bTrans = false; ms_pVTLS = 0; }
	inline static bool			IsTransaction() { return ms_bTrans; }
	inline static vtls_pointer	GetStorage() { return ms_pVTLS; }
private:
	static	bool				ms_bTrans;
	static	vtls_pointer		ms_pVTLS;			
};

template< typename _VTLS > bool
TransactionVTLS<_VTLS>::ms_bTrans = 0;
template< typename _VTLS > typename TransactionVTLS<_VTLS>::vtls_pointer
TransactionVTLS<_VTLS>::ms_pVTLS = 0;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<TransactionObject>
//	-	in Process Object Transaction Mechanism
//	-	_LS (Local Storage) - multiple object
//
template< typename _LS >
class TransactionObject
{
public:
	typedef _LS					ls_type;
	typedef ls_type*			ls_pointer;
public:		TransactionObject() : m_bTrans(false), m_pLS(0) {}
public:
	inline	VOID				Enter( ls_pointer pvtls ) { m_bTrans=true; m_pLS=pvtls; }
	inline	VOID				Leave() { m_bTrans = false; m_pLS = 0; }
	inline	bool				IsTransaction() { return m_bTrans; }
	inline	ls_pointer			GetStorage() { return m_pLS; }
private:	bool				m_bTrans;
			ls_pointer			m_pLS;			
};

template< typename _VTLS >
class AutoTransactionVTLS : public TransactionVTLS< _VTLS >
{
public:		AutoTransactionVTLS( vtls_pointer ptr ) { Enter( ptr ); }
			~AutoTransactionVTLS() { Leave(); }
};

template< typename _LS >
class AutoTransactionObject : public TransactionObject< _LS >
{
public:		AutoTransactionObject( ls_pointer ptr ) { Enter( ptr ); }
			~AutoTransactionObject() { Leave(); }
};

