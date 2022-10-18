#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "GuildSystem/uiGuildMan.h"
#include "GuildSystem/uiGuildMenu.h"
#include "GuildSystem/uiGuildCreate.h"
#include "GuildSystem/uiGuildWareHouse.h"
#include "GuildSystem/uiGuildMarkCombination.h"
#include "GuildSystem/uiGuildIntro.h"
#include "GuildSystem/uiGuildManagement.h"
#include "GuildSystem/uiGuildRelation.h"
#include "GuildSystem/uiGuildLog.h"
#include "GuildSystem/uiGuildRelationAsk.h"
#include "GuildSystem/uiGuildMarkRegister.h"
#include "GuildSystem/uiGuildInfo.h"
#include "GuildSystem/uiGuildPoint.h"
#include "GuildSystem/uiGuildMember.h"
#include "GuildSystem/uiGuildDutyVest.h"
#include "GuildSystem/uiGuildAuthority.h"
#include "GuildSystem/uiGuildDominationAreaInfo.h"
#include "GuildSystem/uiGuildInstitution.h"
#include "GuildSystem/uiGuildRanking.h"
#include "GuildSystem/uiGuildInfoOther.h"
#include "GuildSystem/uiGuildIntroOther.h"
#include "GuildSystem/GuildSystemPacketParser.h"
#include "GuildSystem/GuildSystemData.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildSystem/uiGuildJoinRequestList.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
  #include "GuildSystem/uiGuildPvE.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

uiGuildMan::uiGuildMan(InterfaceManager* interface_manager)
    :uiBaseMan(interface_manager)
{

}

uiGuildMan::~uiGuildMan()
{

}

void uiGuildMan::OnInitialize()
{
    CreateUIDialog<uiGuildMenu>(uiGuildMenu::kDialog_ID, "Data\\Interface\\73_1_Guild_menu.iwz", this);

    CreateUIDialog<uiGuildCreate>(uiGuildCreate::kDialog_ID, "Data\\Interface\\73_Guild_Create.iwz", this);

    CreateUIDialog<uiGuildWareHouse>(uiGuildWareHouse::kDialog_ID, "Data\\Interface\\73_8_Guild_ware_house.iwz", this);

    CreateUIDialog<uiGuildMarkCombination>(uiGuildMarkCombination::kDialog_ID, "Data\\Interface\\73_29_Guild_MarkCombination.iwz", this);

    CreateUIDialog<uiGuildIntro>(uiGuildIntro::kDialog_ID, "Data\\Interface\\73_41_Guild_Intro.iwz", this, TRUE);
    
    CreateUIDialog<uiGuildManagement>(uiGuildManagement::kDialog_ID, "Data\\Interface\\73_44_Guild_Manage.iwz", this, TRUE);

    CreateUIDialog<uiGuildRelation>(uiGuildRelation::kDialog_ID, "Data\\Interface\\73_26_Guild_Relation3.iwz", this);

    CreateUIDialog<uiGuildLog>(uiGuildLog::kDialog_ID, "Data\\Interface\\73_45_Guild_Log.iwz", this, TRUE);

    CreateUIDialog<uiGuildRelationAsk>(uiGuildRelationAsk::kDialog_ID, "Data\\Interface\\73_11_Guild_Relation_Ask.iwz", this);

    CreateUIDialog<uiGuildMarkRegister>(uiGuildMarkRegister::kDialog_ID, "Data\\Interface\\73_28_Guild_Record.iwz", this);

    CreateUIDialog<uiGuildInfo>(uiGuildInfo::kDialog_ID, "Data\\Interface\\73_40_Guild_Info.iwz", this, TRUE);

    CreateUIDialog<uiGuildPoint>(uiGuildPoint::kDialog_ID, "Data\\Interface\\73_46_Guild_Point.iwz", this);

    CreateUIDialog<uiGuildMember>(uiGuildMember::kDialog_ID, "Data\\Interface\\73_42_Guild_Member.iwz", this, TRUE);

    CreateUIDialog<uiGuildDutyVest>(uiGuildDutyVest::kDialog_ID, "Data\\Interface\\73_23_Guild_DutyVest.iwz", this);

    CreateUIDialog<uiGuildAuthority>(uiGuildAuthority::kDialog_ID, "Data\\Interface\\73_27_Guild_Authority.iwz", this);

    CreateUIDialog<uiGuildDominationAreaInfo>(uiGuildDominationAreaInfo::kDialog_ID, "Data\\Interface\\73_47_Guild_Area.iwz", this);

    CreateUIDialog<uiGuildInstitution>(uiGuildInstitution::kDialog_ID, "Data\\Interface\\73_43_Guild_Institution.iwz", this);

    CreateUIDialog<uiGuildRanking>(uiGuildRanking::kDialog_ID, "Data\\Interface\\73_33_Guild_Ranking.iwz", this);

    CreateUIDialog<uiGuildInfoOther>(uiGuildInfoOther::kDialog_ID, "Data\\Interface\\73_40_Guild_Info.iwz", this, TRUE, TRUE);

    CreateUIDialog<uiGuildIntroOther>(uiGuildIntroOther::kDialog_ID, "Data\\Interface\\73_41_Guild_Intro.iwz", this, TRUE, TRUE);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    CreateUIDialog<uiGuildJoinRequestList>(uiGuildJoinRequestList::kDialog_ID, "Data\\Interface\\73_48_Guild_joinpetition_list.iwz", this);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
    CreateUIDialog<uiGuildPvE>(uiGuildPvE::kDialog_ID, "Data\\Interface\\73_49_Guild_PVE.iwz", this, TRUE);
#endif //_NA_008334_20150608_SONNENSCHEIN

    uiGuildInfo* ui_guild_info = GET_CAST_DIALOG(uiGuildInfo, uiGuildInfo::kDialog_ID);
    if (ui_guild_info != NULL)
    {
        ui_guild_info->InitTabController();
    }

    GuildSystem::GetGuildSystemData().Clear();

    GuildSystem::GetGuildSystemData().LoadGuildMarkTexture();
}

void uiGuildMan::OnRelease()
{

}

void uiGuildMan::OnReset()
{
    // 캐릭터 선택시 호출됨 : 초기화 시킬 사항들은 여기서 처리
    uiGuildIntro* ui_guild_intro = GET_CAST_DIALOG(uiGuildIntro, uiGuildIntro::kDialog_ID);
    if (ui_guild_intro != NULL)
    {
        ui_guild_intro->SetDeleteMessage(true);
    }    
}

void uiGuildMan::NetworkProc(MSG_BASE* recv_packet)
{
    GuildSystem::PacketParser::ParsePacketGuildSystem(recv_packet);
}

void uiGuildMan::NetworkProcWorld(MSG_BASE* recv_packet)
{
    GuildSystem::PacketParser::ParsePacketGuildSystemForWorld(recv_packet);
}

void uiGuildMan::InitPKBooleans()
{

}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
