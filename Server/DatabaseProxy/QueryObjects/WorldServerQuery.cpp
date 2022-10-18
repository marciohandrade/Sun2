#include "stdafx.h"
#include "WorldServerQuery.h"
#include "./QueryPoolFactory.h"

__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_SelectList)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_Request)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_Delete)
//_KR_0_20091021_FIRENDINVITATION_EXTEND = {
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_Recall_Update)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_Recall_Select)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_InvitationReward_Update)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_InvitationReward_Select)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_InvitationReward_Insert)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_InvitationFree)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_InvitationInsert)
//}
//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_Detail_Info_Select)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_Memo_Update)

//__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_BlockChar)
//__IMPL_SAFECUSTOMPOOL_PTR(Query_Friend_FreeChar)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Chat_BlockChar)
__IMPL_SAFECUSTOMPOOL_PTR(Query_ChatBlock_Info)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Memo_Send)
__IMPL_SAFECUSTOMPOOL_PTR(Query_MMemo_Send)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Memo_ListSelect)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Memo_Select)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Memo_Delete)
//__IMPL_SAFECUSTOMPOOL_PTR(Query_Revenge_AddChar)
//__IMPL_SAFECUSTOMPOOL_PTR(Query_Revenge_RemoveChar)

__IMPL_SAFECUSTOMPOOL_PTR(Query_Interception_Request)

__IMPL_SAFECUSTOMPOOL_PTR(GuildWarehouseLogSelectQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildNoticeSelectQuery)

//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
__IMPL_SAFECUSTOMPOOL_PTR(CommunityDataQuery)
__IMPL_SAFECUSTOMPOOL_PTR(SelectQueryCommunityList )
__IMPL_SAFECUSTOMPOOL_PTR(SelectQueryContactMeList)
//#endif

__IMPL_SAFECUSTOMPOOL_PTR(Query_Group_Insert)           //그룹 맴버 추가
__IMPL_SAFECUSTOMPOOL_PTR(Query_Group_Delete)           //그룹 맴버 삭제
__IMPL_SAFECUSTOMPOOL_PTR(Query_Group_Select)           //그룹 맴버 정보 요청 
__IMPL_SAFECUSTOMPOOL_PTR(Query_Group_AccumExp_Insert)  //그룹 맴버 보너스 경험치 추가
__IMPL_SAFECUSTOMPOOL_PTR(Query_Group_AccumExp_Update)  //그룹 맴버 보너스 경험치 추가

//__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoEntries_Select)    // 하임 로또 응모 내역 로딩
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoResults_Select)    // 하임 로또 결과 정보 로딩
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoWinners_Select)    // 하임 로또 당첨자 목록 로딩
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoEntry_Insert)      // 하임 로또 응모 처리
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoResults_Insert)    // 하임 로또 추첨 및 이월 처리
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoTotal_Update)      // 하임 로또 총 적립액 갱신
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoTicket_Insert)     // 하임 로또 복권 용지 수 증가
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoMoney_Insert)      // 하임 로또 당첨금 지급
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoNotice_Select)     // 하임 로또 당첨안내 쪽지 목록 로딩
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoNotice_Delete)     // 하임 로또 당첨안내 쪽지 삭제
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoPayment_Update)    // 캐릭터 유효 소비 하임 액수 저장
//}

__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_AuctionMoney_Delete)    // 지역점령전 당첨금액 보상 환급 관련 쿼리

