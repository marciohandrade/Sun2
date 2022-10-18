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

__IMPL_SAFECUSTOMPOOL_PTR(Query_Group_Insert)           //�׷� �ɹ� �߰�
__IMPL_SAFECUSTOMPOOL_PTR(Query_Group_Delete)           //�׷� �ɹ� ����
__IMPL_SAFECUSTOMPOOL_PTR(Query_Group_Select)           //�׷� �ɹ� ���� ��û 
__IMPL_SAFECUSTOMPOOL_PTR(Query_Group_AccumExp_Insert)  //�׷� �ɹ� ���ʽ� ����ġ �߰�
__IMPL_SAFECUSTOMPOOL_PTR(Query_Group_AccumExp_Update)  //�׷� �ɹ� ���ʽ� ����ġ �߰�

//__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoEntries_Select)    // ���� �ζ� ���� ���� �ε�
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoResults_Select)    // ���� �ζ� ��� ���� �ε�
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoWinners_Select)    // ���� �ζ� ��÷�� ��� �ε�
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoEntry_Insert)      // ���� �ζ� ���� ó��
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoResults_Insert)    // ���� �ζ� ��÷ �� �̿� ó��
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoTotal_Update)      // ���� �ζ� �� ������ ����
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoTicket_Insert)     // ���� �ζ� ���� ���� �� ����
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoMoney_Insert)      // ���� �ζ� ��÷�� ����
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoNotice_Select)     // ���� �ζ� ��÷�ȳ� ���� ��� �ε�
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoNotice_Delete)     // ���� �ζ� ��÷�ȳ� ���� ����
__IMPL_SAFECUSTOMPOOL_PTR(Query_LottoPayment_Update)    // ĳ���� ��ȿ �Һ� ���� �׼� ����
//}

__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_AuctionMoney_Delete)    // ���������� ��÷�ݾ� ���� ȯ�� ���� ����

