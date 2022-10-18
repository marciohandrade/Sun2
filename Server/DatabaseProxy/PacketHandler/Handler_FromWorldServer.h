#pragma once

#include "./PacketHandler.h"

#define Handler_FromWorldServer_DECL( p )	static VOID On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define Handler_FromWorldServer_IMPL( p )	VOID Handler_FromWorldServer::On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )

class ChildServerSession;

class Handler_FromWorldServer
{
public:
	Handler_FromWorldServer(void){}
	~Handler_FromWorldServer(void){}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_FromWorldServer_DECL(DW_CONNECTION_GROUND_CMD);
	Handler_FromWorldServer_DECL( DW_FRIEND_GETLIST_SYN );			//< 친구 목록
	Handler_FromWorldServer_DECL( DW_FRIEND_GETLIST_DBR );			//< 친구 목록
	Handler_FromWorldServer_DECL( DW_FRIEND_ADD_SYN );				//< A->B add friend
	Handler_FromWorldServer_DECL( DW_FRIEND_ADD_DBR );				//< A->B add friend
	Handler_FromWorldServer_DECL( DW_FRIEND_DEL_SYN );				//< 친구 삭제
	Handler_FromWorldServer_DECL( DW_FRIEND_DEL_DBR );				//< 친구 삭제
	Handler_FromWorldServer_DECL( DW_FRIEND_BLOCK_SYN );			//< 유저 차단
	Handler_FromWorldServer_DECL( DW_FRIEND_BLOCK_DBR );			//< 유저 차단
	Handler_FromWorldServer_DECL( DW_FRIEND_BLOCK_FREE_SYN );		//< 차단 삭제
	Handler_FromWorldServer_DECL( DW_FRIEND_BLOCK_FREE_DBR );		//< 차단 삭제
    //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
	Handler_FromWorldServer_DECL( DW_FRIEND_INVITATIONRELATION_FREE_CMD );		//< 친구 추천관계 일반친구로 변경
	Handler_FromWorldServer_DECL( DW_FRIEND_INVITATIONRELATION_FREE_DBR );		//< 친구 추천관계 일반친구로 변경    
    Handler_FromWorldServer_DECL( DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD );		//< 보상 아이템 정보
    Handler_FromWorldServer_DECL( DW_FRIEND_INVITATIONRELATION_REWARDINSERT_DBR );		//< 보상 아이템 정보
    //}
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    Handler_FromWorldServer_DECL( DW_FRIEND_DETAIL_INFO_SYN );      // 상세 정보창 정보 요청(메모,길드) 
    Handler_FromWorldServer_DECL( DW_FRIEND_DATAIL_INFO_DBR );      // 상세 정보창 정보 요청(메모,길드) 
    Handler_FromWorldServer_DECL( DW_FRIEND_MEMO_UPDATE_SYN );      // 상세 정보창 (메모) 
    Handler_FromWorldServer_DECL( DW_FRIEND_MEMO_UPDATE_DBR );      // 상세 정보창 (메모) 

    Handler_FromWorldServer_DECL( DW_GM_CHATBLOCK_SYN );			//< 채팅금지
	Handler_FromWorldServer_DECL( DW_GM_CHATBLOCK_DBR );			//< 채팅금지
	Handler_FromWorldServer_DECL( DW_GM_CHATALLOW_SYN );			//< 채팅허용
	Handler_FromWorldServer_DECL( DW_GM_CHATALLOW_DBR );			//< 채팅허용
	Handler_FromWorldServer_DECL( DW_GM_CHATBLOCK_UPDATE_SYN );		//< 채팅금지 시간 차감
	Handler_FromWorldServer_DECL( DW_GM_CHATBLOCK_UPDATE_DBR );		//< 채팅금지 시간 차감
	Handler_FromWorldServer_DECL( DW_GM_CHATBLOCKTIME_REQ );		//< 채팅금지 시간 요청
	Handler_FromWorldServer_DECL( DW_GM_CHATBLOCKTIME_DBR );		//< 채팅금지 시간 요청
	Handler_FromWorldServer_DECL( DW_CHAO_ADD_REVENGE_CHAR_SYN );	//< Add Revenge Char
	Handler_FromWorldServer_DECL( DW_CHAO_ADD_REVENGE_CHAR_DBR );	//< Add Revenge Char
	Handler_FromWorldServer_DECL( DW_CHAO_REMOVE_REVENGE_CHAR_SYN );	//< Remove Revenge Char
	Handler_FromWorldServer_DECL( DW_CHAO_REMOVE_REVENGE_CHAR_DBR );	//< Remove Revenge Char
	Handler_FromWorldServer_DECL( DW_CHAO_PK_REVENGE_CHAR_SYN );	//< PK Success - Remove Revenge Char
	Handler_FromWorldServer_DECL( DW_CHAO_PK_REVENGE_CHAR_DBR );	//< PK Success - Remove Revenge Char
	Handler_FromWorldServer_DECL( DW_MEMO_SEND_SYN );				//< 쪽지 전송
	Handler_FromWorldServer_DECL( DW_MEMO_SEND_DBR );
	Handler_FromWorldServer_DECL( DW_MEMO_LIST_REQ );				//< 쪽지 목록 조회
	Handler_FromWorldServer_DECL( DW_MEMO_LIST_DBR );
	Handler_FromWorldServer_DECL( DW_MEMO_VIEW_REQ );				//< 쪽지 확인
	Handler_FromWorldServer_DECL( DW_MEMO_VIEW_DBR );
	Handler_FromWorldServer_DECL( DW_MEMO_DELETE_SYN );				//< 쪽지 삭제
	Handler_FromWorldServer_DECL( DW_MEMO_DELETE_DBR );
	Handler_FromWorldServer_DECL( DW_STATUS_INTERCEPTION_REQ );
	Handler_FromWorldServer_DECL( DW_STATUS_INTERCEPTION_DBR );

	Handler_FromWorldServer_DECL(DW_GUILD_WAREHOUSE_LOGLIST_REQ);
	Handler_FromWorldServer_DECL(DW_GUILD_WAREHOUSE_LOGLIST_DBR);

	Handler_FromWorldServer_DECL(DW_GUILD_SELECT_NOTICE_SYN);
	Handler_FromWorldServer_DECL(DW_GUILD_SELECT_NOTICE_DBR);

    Handler_FromWorldServer_DECL(DW_DOMINATION_FINDMONEY_SYN);
    Handler_FromWorldServer_DECL(DW_DOMINATION_FINDMONEY_DBR);
//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
	
	Handler_FromWorldServer_DECL(DW_COMMUNITY_MYLIST_SYN);
	Handler_FromWorldServer_DECL(DW_COMMUNITY_MYLIST_DBR);
	Handler_FromWorldServer_DECL(DW_COMMUNITY_INFO_SYN);
	Handler_FromWorldServer_DECL(DW_COMMUNITY_INFO_DBR);
	Handler_FromWorldServer_DECL(DW_COMMUNITY_CONTACT_ME_DBR);
//#endif
    //{__NA001254_20090109_RTTG_FUNC_ADD
	Handler_FromWorldServer_DECL(DW_MULTIMEMO_SEND_SYN);
	Handler_FromWorldServer_DECL(DW_MULTIMEMO_SEND_DBR);
    //}
    // __KR_001355_20090608_GROUPINGSYSTEM_DW = { //그룹맺기 시스템
	Handler_FromWorldServer_DECL(DW_GROUP_ADD_SYN); //그룹 맴버 추가 요청 
	Handler_FromWorldServer_DECL(DW_GROUP_ADD_DBR); //그룹 맴버 추가 DB작업 결과

	Handler_FromWorldServer_DECL(DW_GROUP_DEL_SYN); //그룹 맴버 삭제 요청 
	Handler_FromWorldServer_DECL(DW_GROUP_DEL_DBR); //그룹 맴버 삭제 DB작업 결과

	Handler_FromWorldServer_DECL(DW_GROUP_LIST_REQ); //그룹 맴버 리스트 요청 
	Handler_FromWorldServer_DECL(DW_GROUP_LIST_DBR); //그룹 맴버 리스트 DB작업 결과

    Handler_FromWorldServer_DECL(DW_GROUP_ACCUMEXP_CMD); //상위 맴버 경험치 축적 요청
    Handler_FromWorldServer_DECL(DW_GROUP_ACCUMEXP_DBR); //상위 맴버 경험치 축적 DB작업 결과

    Handler_FromWorldServer_DECL(DW_GROUP_INIT_ACCUMEXP_CMD); //상위 맴버 경험치 축적 요청
    Handler_FromWorldServer_DECL(DW_GROUP_INIT_ACCUMEXP_DBR); //상위 맴버 경험치 축적 DB작업 결과

    Handler_FromWorldServer_DECL(DW_GROUP_SET_MEMBERINFO_CMD); //그룹 정보 세팅
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_DW'

    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    Handler_FromWorldServer_DECL(DW_LOTTO_LOAD_ENTRY_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_LOAD_ENTRY_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_LOAD_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_LOAD_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_WINNER_LIST_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_WINNER_LIST_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_SAVE_ENTRY_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_SAVE_ENTRY_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_ROLLOVER_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_ROLLOVER_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_ACCUM_NTF);
    Handler_FromWorldServer_DECL(DW_LOTTO_ACCUM_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_ADD_TICKET_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_ADD_TICKET_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_REWARD_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_REWARD_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_NOTICE_LIST_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_NOTICE_LIST_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_NOTICE_DEL_SYN);
    Handler_FromWorldServer_DECL(DW_LOTTO_NOTICE_DEL_DBR);
    Handler_FromWorldServer_DECL(DW_LOTTO_PAYMENT_UPDATE_NTF);
    Handler_FromWorldServer_DECL(DW_LOTTO_PAYMENT_UPDATE_DBR);
    //}
};
