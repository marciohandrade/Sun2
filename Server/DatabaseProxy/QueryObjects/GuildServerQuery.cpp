#include "stdafx.h"
#include "GuildServerQuery.h"
#include "./QueryPoolFactory.h"


__IMPL_SAFECUSTOMPOOL_PTR(SelectGuildQuery)
__IMPL_SAFECUSTOMPOOL_PTR(SelectGuildMemberQuery)
__IMPL_SAFECUSTOMPOOL_PTR(CreateGuildQuery)
__IMPL_SAFECUSTOMPOOL_PTR(DestroyGuildQuery)
__IMPL_SAFECUSTOMPOOL_PTR(JoinGuildMemberQuery)
__IMPL_SAFECUSTOMPOOL_PTR(WithdrawGuildMemberQuery)
__IMPL_SAFECUSTOMPOOL_PTR(ChangeGuildPositionQuery)
__IMPL_SAFECUSTOMPOOL_PTR(ChangeGuildNoticeQuery)
__IMPL_SAFECUSTOMPOOL_PTR(DonateUPQuery)
__IMPL_SAFECUSTOMPOOL_PTR(ConvertUP2GPQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildRankUpQuery)

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
__IMPL_SAFECUSTOMPOOL_PTR(UpdateGuildChuntingRecordQuery)
#endif

__IMPL_SAFECUSTOMPOOL_PTR(ChangeGuildDutyQuery)
__IMPL_SAFECUSTOMPOOL_PTR(ChangeGuildMasterQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildRightUpdateQuery)

__IMPL_SAFECUSTOMPOOL_PTR(CorpsCreateQuery)
__IMPL_SAFECUSTOMPOOL_PTR(CorpsDestroyQuery)
__IMPL_SAFECUSTOMPOOL_PTR(ChangeCommanderQuery)
__IMPL_SAFECUSTOMPOOL_PTR(ChangeCorpsQuery)
__IMPL_SAFECUSTOMPOOL_PTR(CreateRearCampQuery)
__IMPL_SAFECUSTOMPOOL_PTR(DestroyRearCampQuery)
__IMPL_SAFECUSTOMPOOL_PTR(SelectGuildCorpsQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildPointQuery)

__IMPL_SAFECUSTOMPOOL_PTR(RelationGuildQuery)
__IMPL_SAFECUSTOMPOOL_PTR(SelectGuildRelationQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildRecoveryQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildDestroyCompleteQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildMarkQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildWarehouseSelectQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildWarehouseUpdateQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildWarehouseLogInsertQuery)
__IMPL_SAFECUSTOMPOOL_PTR(GuildWarehouseTabExQuery)
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildWarehouse_ItemSlotInfo_Select);
__IMPL_SAFECUSTOMPOOL_PTR(ChangeGuildName)

__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Select)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Update)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_DefenseWall_Update)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_DisGuild_Update)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_TotalPoint_Update)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Insert)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Auction_Select)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Action_Update)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Info_Action_Init)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_ActionMoney_Update)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_ReturnMoney_Delete)

//_NA_20100604_BETTER_DOMINATION_MANAGER
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Increase_Count)

//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_MemberList_Select)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Member_Insert)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_Member_Delete)
__IMPL_SAFECUSTOMPOOL_PTR(Query_Domination_MemberList_Init)

#ifdef _NA_003923_20120130_GUILD_RENEWAL
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildInGameLog_Insert)
__IMPL_SAFECUSTOMPOOL_PTR(Query_GuildInGameLog_Select)
#endif
__IMPL_SAFECUSTOMPOOL_PTR(RelationBothAgainstGuildQuery)

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
__IMPL_SAFECUSTOMPOOL_PTR(Query_Continent_Lord_Info_Select)
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#ifdef _NA_008334_20150608_SONNENSCHEIN
__IMPL_SAFECUSTOMPOOL_PTR(SonnenScheinInitializeQuery)
__IMPL_SAFECUSTOMPOOL_PTR(SonnenScheinUpdateQuery)
#endif //_NA_008334_20150608_SONNENSCHEIN

/////////////// DB ERROR CODE CONVERT TO SERVER ERROR CODE ///////////////
const INT DBResultConvert<RelationGuildQuery>::ErrorCode[] = 
{ 
	RC::RC_GUILD_SUCCESS,
	RC::RC_GUILD_RELATION_LIST_ERROR,
	RC::RC_GUILD_NOT_EXIST_GUILD,
	RC::RC_GUILD_RELATION_ALLIENCE_TYPE1_FAILED,
	RC::RC_GUILD_RELATION_HOSTILITY_FAILED,
	RC::RC_GUILD_RELATION_CHANGE_RIGHT_NOT,
	RC::RC_GUILD_RELATION_ALEADY_SET,
	RC::RC_GUILD_RELATION_ME_FULL,
	RC::RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED,
	RC::RC_GUILD_RELATION_GUILD_GRADE_LOW_ME,
	RC::RC_GUILD_RELATION_GUILD_GRADE_LOW_OTHER, //10
};

const INT DBResultConvert<RelationBothAgainstGuildQuery>::ErrorCode[] = 
{ 
    RC::RC_GUILD_SUCCESS,
    RC::RC_GUILD_RELATION_LIST_ERROR,
    RC::RC_GUILD_NOT_EXIST_GUILD,
    RC::RC_GUILD_RELATION_ALLIENCE_TYPE1_FAILED,
    RC::RC_GUILD_RELATION_HOSTILITY_FAILED,
    RC::RC_GUILD_RELATION_CHANGE_RIGHT_NOT,
    RC::RC_GUILD_RELATION_ALEADY_SET,
    RC::RC_GUILD_RELATION_ME_FULL,
    RC::RC_GUILD_RELATION_BOTH_HOSTILITY_FAILED,
    RC::RC_GUILD_RELATION_GUILD_GRADE_LOW_ME,
    RC::RC_GUILD_RELATION_GUILD_GRADE_LOW_OTHER, //10
};

const INT DBResultConvert<DestroyGuildQuery>::ErrorCode[] = 
{ 
	RC::RC_GUILD_SUCCESS,
	RC::RC_GUILD_FAILED,
	RC::RC_GUILD_DESTROY_WAIT_CANNOT_NONE_RIGHT,
	RC::RC_GUILD_DESTROY_WAIT_CANNOT_MEMBER_EXIST,
	RC::RC_GUILD_DESTROY_WAIT_CANNOT_RELATION_REMAIN,
	RC::RC_GUILD_DESTROY_WAIT_CANNOT_GUILDWAREHOUSE_ITEM_REMAIN,
    RC::RC_GUILD_DOMINATION_ACTIONINFO_CANNOT_DESTROY,  
    RC::RC_GUILD_DOMINATION_INFO_CANNOT_DESTROY,        
};

